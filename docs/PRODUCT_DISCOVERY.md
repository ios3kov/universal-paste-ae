# Product Discovery — Universal Paste for After Effects

Baseline: AE-Development-Rules v8.0.0  
Stage: 0 / greenfield product  
Delivery Gate: Development

## Product Vision

Universal Paste turns clipboard content into ready-to-use After Effects layers with one predictable paste action.

Primary user: After Effects motion designers and editors who repeatedly move images, text, colors and files from other applications into a composition.

Problem: AE requires different manual import/create flows for content that users already have in the clipboard. This breaks flow, creates temporary-file clutter and can leave projects with missing assets.

Main journey: copy externally → paste in AE → Universal Paste detects the content → creates the correct AE object → stores any file-backed asset safely → user continues animating.

Value proposition: **Copied → pasted → animate.**

## Confirmed requirements

| ID | Requirement | Priority | Source |
| --- | --- | --- | --- |
| UP-CORE-01 | Clipboard image becomes an AE layer and preserves transparency. | Core | User product brief |
| UP-CORE-02 | Clipboard text becomes a new Text Layer. | Core | User product brief |
| UP-CORE-03 | HEX / RGB / HSL color becomes a Solid. | Core | User product brief |
| UP-CORE-04 | Multiple copied files become multiple layers. | Core | User product brief |
| UP-CORE-05 | One paste command automatically detects supported clipboard content. | Core | User product brief |
| UP-CORE-06 | Paste As… lets the user override ambiguous detection. | Important | User product brief |
| UP-CORE-07 | Clipboard-created image assets are stored persistently so the AE project does not depend on temporary clipboard data. | Core | User product brief |
| UP-SAFE-01 | Unsupported/ambiguous clipboard states must fail safely and must not destroy or corrupt existing AE state. | Core | Derived from the core workflow |
| UP-SAFE-02 | Normal AE copy/paste must remain usable when Universal Paste should not consume the command. | Core | Derived from replacing/intercepting Paste |

## Scope

### Core v1
- Image → footage/layer with alpha preserved.
- Text → Text Layer.
- HEX/RGB/HSL → Solid.
- Multiple files → multiple imported layers.
- Automatic type detection.
- Persistent storage for generated/imported clipboard assets.
- Safe fallback to native AE paste when Universal Paste does not own the clipboard content.

### Important
- Paste As… override.
- Clear user-facing errors for unsupported/failed paste operations.
- Deterministic layer ordering for multi-file paste.

### Later
- SVG → editable Shape Layers.
- URL → download/import.
- Color → apply to selected layer.
- Simple CSS → shape/text styling.

### Out of scope for v1
- Lottie/JSON import.
- Full HTML/CSS parser.
- Cloud asset manager.
- Replacing AE's internal project/layer clipboard format.

## Core user flows

### Flow A — Automatic paste
- Trigger: user executes normal Paste in AE.
- Initial state: an active AE project; composition context may or may not be valid.
- Product action: inspect clipboard without mutating AE.
- If supported external content is present: classify it, create/import the matching object, and mark the operation handled.
- If not: leave the command to AE.
- Expected result: one predictable paste action.
- Error boundary: no partial orphan layers/files without a recorded recovery path.

### Flow B — Paste As…
- Trigger: explicit Universal Paste command.
- Product action: show only applicable interpretations of current clipboard content.
- Expected result: user forces image/text/color/files handling without changing global defaults.

### Flow C — persistent asset creation
- Trigger: clipboard content requires file-backed footage.
- Product action: write owned asset bytes to durable storage before/with import.
- Expected result: project remains valid after clipboard changes, app restart and temp cleanup.
- Recovery: failed write/import leaves AE unchanged or cleanly reports a partial failure.

## Success criteria

| ID | Observable result | Target |
| --- | --- | --- |
| SC-01 | PNG/clipboard image with alpha is pasted into an active comp. | Visual/pixel content and alpha preserved. |
| SC-02 | Unicode clipboard text is pasted. | Text Layer contains the exact text. |
| SC-03 | Valid HEX/RGB/HSL clipboard color is pasted. | Solid matches parsed color within AE color precision. |
| SC-04 | N copied files are pasted. | N corresponding layers are created in deterministic order. |
| SC-05 | Unsupported clipboard content is pasted. | Native AE Paste remains available; no project mutation by Universal Paste. |
| SC-06 | Asset created from clipboard is reopened after restart. | No missing-footage dependency on OS temp/clipboard storage. |
| SC-07 | A supported paste fails mid-operation. | No silent corruption; result is recoverable and reported. |
| SC-08 | Operation is undone. | All AE-side mutations created by one Universal Paste action undo together where host APIs permit. |

## Constraints and open questions

### Known constraints
- Must run inside After Effects desktop.
- Clipboard inspection must precede AE mutation.
- Asset storage must not rely only on temporary directories.
- Native AE behavior must not be consumed unless Universal Paste intentionally handles the current external content.
- CEP is not selected for a new long-lived architecture.

### Open questions
| Question | Impact | Blocking? |
| --- | --- | --- |
| Exact minimum supported After Effects version/build. | SDK/API matrix and testing. | Blocks final compatibility claim, not research. |
| Final macOS/Windows support matrix and architectures. | Clipboard adapter/build matrix. | Blocks final technical design/build matrix. |
| Exact persistent storage policy for an unsaved AE project. | Asset relocation/relink behavior. | Blocks final persistence contract. |
| Whether current AE UXP beta exposes all clipboard/image capabilities needed. | Could affect future UI/bridge architecture. | Does not block native PoC. |

## Stage 0 status

The core problem, value, v1 scope, main flows and observable success criteria are defined.

Stage 0 is **not fully closed** until platform/version targets and the unsaved-project asset policy are resolved. Those unknowns do not block the current API/feasibility research.