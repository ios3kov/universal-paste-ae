# API Compatibility Audit — Research Phase

Product: Universal Paste for After Effects  
Phase: pre-implementation research  
Rules baseline: AE-Development-Rules v8.0.0  
Primary PoC target: After Effects 26.5 + After Effects SDK 26.5

No broad compatibility status is claimed yet. The 26.5 target is the first development probe, not the final minimum supported version.

## Source verification

| API / capability | Contract currently verified from | Status | Limitation / next proof |
| --- | --- | --- | --- |
| `AEGP_RegisterCommandHook` | After Effects C++ SDK Guide, AEGP Register Suite | DOCUMENTED | Verify exact declaration in selected 26.5 SDK headers before native source is committed. |
| `AEGP_Command_ALL` | SDK guide/examples/community documentation | DOCUMENTED/EXAMPLE | Runtime behavior for Paste must be observed in AE 26.5. |
| command hook handled flag | SDK sample signatures / AEGP examples | DOCUMENTED/EXAMPLE | Must prove selective consume + native fallback. |
| `AEGP_DoCommand` | After Effects C++ SDK Guide | DOCUMENTED | IDs are explicitly not guaranteed stable; not accepted as a hard-coded Paste strategy. |
| `app.findMenuCommandId(text)` | After Effects SDK guide recommendation | DOCUMENTED DEVELOPMENT AID | Localization/version stability unresolved for production use. |
| AE 26.5 / SDK 26.5 release | Adobe AE release notes + SDK What's New | CURRENT TARGET | Runtime evidence still required. |
| AE UXP host API | Adobe After Effects UXP docs | BETA / EVOLVING | Not selected for v1 core. |
| UXP clipboard | Adobe UXP clipboard docs | DOCUMENTED PLATFORM API | Exact AE-host image MIME behavior not verified. |
| macOS clipboard API | not yet selected against project toolchain | UNKNOWN | Verify exact native API + deployment target before adapter code. |
| Windows clipboard API | not yet selected against project toolchain | UNKNOWN | Verify exact native API + Win target before adapter code. |

## Compatibility status

| Target | Status |
| --- | --- |
| After Effects 26.5 | RESEARCH TARGET — runtime NOT RUN |
| After Effects older versions | UNKNOWN — minimum support not selected |
| macOS exact versions/architectures | UNKNOWN |
| Windows exact versions/architectures | UNKNOWN |
| Runtime Paste interception | UNKNOWN — PoC-01 required |

## Rule consequence

No source code may depend on an invented command ID, unverified SDK suite revision or assumed clipboard MIME behavior.

Independent work allowed now:
- product contract;
- command-hook PoC design;
- portable classifier/parser;
- build/test scaffolding independent of AE SDK;
- platform API research.

## Sources

- Adobe After Effects release notes: current release 26.5 (September 2026)
- After Effects C++ SDK Guide: What's New in 26.5 SDK
- After Effects C++ SDK Guide: AEGP Suites / Command Hook
- Adobe UXP / After Effects UXP documentation
