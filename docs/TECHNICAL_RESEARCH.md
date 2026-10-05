# Technical Research — Universal Paste

Date: 2026-10-05  
Rules baseline: AE-Development-Rules v8.0.0  
Status: architecture candidate, not final production decision

## Research question

Can Universal Paste safely intercept the normal AE Paste command, inspect external clipboard content, handle supported data itself, and otherwise leave native AE paste unchanged?

## Finding 1 — AEGP command hooks are the strongest current candidate

The After Effects C++ SDK documents `AEGP_RegisterCommandHook`. An AEGP can register a hook for commands, including `AEGP_Command_ALL`, and the hook receives a handled flag so it can decide whether AE should continue processing the command.

Adobe's SDK guide also states that some After Effects commands can be replaced via this mechanism.

Implication: a native AEGP can plausibly observe Paste before AE handles it and consume only Universal Paste cases.

## Finding 2 — command identity is the critical risk

The SDK documentation explicitly says After Effects menu command IDs are not guaranteed to remain stable between versions.

The SDK documentation suggests `app.findMenuCommandId(text)` as a way to discover a menu command ID during development, but this is not yet accepted as a production strategy because:
- localized menu text may differ;
- IDs may differ between AE versions;
- relying on an undocumented numeric ID without runtime evidence would violate the API-source rule.

PoC must therefore:
1. register an all-command hook;
2. observe the exact command(s) produced by Copy/Paste in target AE builds;
3. prove that the Paste command can be identified and selectively consumed;
4. prove native AE copy/paste still works when the plugin returns unhandled.

## Finding 3 — AEGP execution constraints

AEGP code runs through host callbacks and is effectively main-thread constrained. Clipboard parsing and disk writes must therefore remain bounded; heavier work may require a safe staged architecture later.

No helper process is justified yet.

## Finding 4 — UXP is not the v1 foundation yet

Adobe announced After Effects UXP public beta by November 2026 and has begun publishing AE-specific UXP documentation.

The shared UXP platform has permissioned clipboard APIs, including MIME-style clipboard access in current documentation. However, availability and exact behavior for the After Effects host/beta and image clipboard formats are not yet verified for this product.

Therefore:
- do not build v1 around CEP;
- do not assume AE UXP clipboard parity until tested;
- keep the core clipboard classifier/storage design separable so a future UXP UI/command layer can reuse it if appropriate.


## Finding 5 — external source applications are intentionally source-agnostic

Universal Paste must not contain separate integration logic for Chrome, Safari, Photoshop, Figma, Finder or Explorer.

The platform adapter consumes standard operating-system clipboard representations and maps them into one common `ClipboardSnapshot`:
- macOS: AppKit pasteboard exposes common image, URL/file, string and other representations; PNG/TIFF are standard image pasteboard types.
- Windows: Unicode text, DIB/DIBV5 bitmaps and CF_HDROP file lists are standard clipboard paths.

A source application may publish several representations at the same time. Universal Paste therefore chooses a deterministic primary representation while retaining alternatives for Paste As….

Private source-app formats are optional future compatibility work only when no suitable standard representation is present.

## Finding 6 — native AE Copy/Cut needs ownership protection

A critical false-positive case exists:

1. user copies an image/text in another app;
2. user later performs Copy/Cut inside After Effects;
3. the system clipboard may still contain the old external data;
4. a naive Paste hook could consume that stale external data instead of allowing AE to paste its internal object.

Mitigation implemented in the portable core:
- observe AE Copy/Cut;
- mark native paste as preferred;
- settle/store an opaque OS clipboard change token;
- while the token is unchanged, Paste stays native AE;
- when the system clipboard token changes externally, Universal Paste becomes eligible again;
- if a token cannot be read, fail safe by preferring native AE paste.

Platform token sources:
- macOS: `NSPasteboard.changeCount`;
- Windows: `GetClipboardSequenceNumber()`.

This state machine is independent of AE command IDs and is covered by unit tests. Runtime PoC must still prove that Copy/Cut/Paste hooks are observed in the required order.

## Provisional architecture

```
AE Paste command
      │
      ▼
Native AEGP command hook
      │
      ├── recent AE Copy/Cut owns paste → return UNHANDLED → native AE Paste
      ├── clipboard unsupported / AE-owned → return UNHANDLED → native AE Paste
      │
      └── supported external content
              │
              ▼
        Clipboard classifier
              │
      ┌───────┼──────────┬─────────┐
      ▼       ▼          ▼         ▼
    Image    Text       Color     Files
      │       │          │         │
      └───────┴──────────┴─────────┘
              │
              ▼
       AE mutation adapter
              │
              ▼
       single user action / recovery
```

Platform-specific clipboard access should sit behind an adapter:
- macOS adapter: AppKit `NSPasteboard` standard image/text/file representations and `changeCount`.
- Windows adapter: Win32 clipboard standard formats (Unicode text, DIB/DIBV5, CF_HDROP) and `GetClipboardSequenceNumber`.

## Asset persistence direction

File-backed clipboard data must be materialized into plugin-owned durable storage before AE depends on it.

Two states must be designed explicitly:
1. saved AE project;
2. unsaved AE project.

Do not silently use OS temp storage as the only source of truth.

## Architecture decision status

**Candidate:** native AEGP core + platform clipboard adapters.

**Not yet approved as production architecture.** It becomes eligible after the command-interception PoC and exact SDK/header audit pass.

## PoC-01 — Paste interception

Goal: answer one question only: can we safely intercept external Paste without breaking native AE Paste?

Required observations:
- plugin loads;
- all-command hook fires;
- Copy/Paste command IDs are observed in the target AE build;
- supported test command can be marked handled;
- returning unhandled preserves native AE behavior;
- no recursive command loop;
- no project mutation in the probe build.

PoC-01 is research evidence, not v1 implementation.

## Sources

- After Effects C++ SDK Guide — AEGP Suites: https://ae-plugins.docsforadobe.dev/aegps/aegp-suites/
- After Effects C++ SDK Guide — AEGP implementation: https://ae-plugins.docsforadobe.dev/aegps/implementation/
- Adobe developer announcement — CEP → UXP timeline: https://blog.developer.adobe.com/en/publish/2026/09/investing-in-the-future-of-creative-cloud-extensibility-uxp-comes-to-our-flagship-applications
- Adobe After Effects UXP docs: https://developer.adobe.com/after-effects/uxp/
- Adobe UXP clipboard recipe: https://developer.adobe.com/uxp/guides/how-to/recipes/clipboard/
- Apple NSPasteboard: https://developer.apple.com/documentation/appkit/nspasteboard
- Microsoft Clipboard Formats: https://learn.microsoft.com/windows/win32/dataxchg/clipboard-formats
- Microsoft GetClipboardSequenceNumber: https://learn.microsoft.com/windows/win32/api/winuser/nf-winuser-getclipboardsequencenumber
