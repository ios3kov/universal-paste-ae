# API Compatibility Audit — Research Phase

Product: Universal Paste for After Effects  
Phase: pre-implementation research  
Rules baseline: AE-Development-Rules v8.0.0

No compatibility status is claimed yet. Selected SDK headers and target AE builds are not committed to the repository yet.

## Source verification

| API / capability | Contract currently verified from | Status | Limitation / next proof |
| --- | --- | --- | --- |
| `AEGP_RegisterCommandHook` | After Effects C++ SDK Guide, AEGP Register Suite | DOCUMENTED | Exact selected SDK header/revision still required before implementation. |
| `AEGP_Command_ALL` | SDK guide/examples/community documentation | DOCUMENTED/EXAMPLE | Runtime behavior for Paste must be observed in target AE build. |
| command hook handled flag | SDK sample signatures / AEGP examples | DOCUMENTED/EXAMPLE | Must prove selective consume + native fallback. |
| `AEGP_DoCommand` | After Effects C++ SDK Guide | DOCUMENTED | IDs are explicitly not guaranteed stable; not accepted as a hard-coded Paste strategy. |
| `app.findMenuCommandId(text)` | After Effects SDK guide recommendation | DOCUMENTED DEVELOPMENT AID | Localization/version stability unresolved for production use. |
| AE UXP host API | Adobe After Effects UXP docs | BETA / EVOLVING | Not selected for v1 core. |
| UXP clipboard | Adobe UXP clipboard docs | DOCUMENTED PLATFORM API | Exact AE-host image MIME behavior not verified. |
| macOS clipboard API | not yet selected against project toolchain | UNKNOWN | Verify exact native API + deployment target before code. |
| Windows clipboard API | not yet selected against project toolchain | UNKNOWN | Verify exact native API + Win target before code. |

## Compatibility status

| Target | Status |
| --- | --- |
| After Effects exact versions | UNKNOWN |
| macOS exact versions/architectures | UNKNOWN |
| Windows exact versions/architectures | UNKNOWN |
| Runtime Paste interception | UNKNOWN — PoC-01 required |

## Rule consequence

No source code may depend on an invented command ID, unverified SDK suite revision or assumed clipboard MIME behavior.

Independent work allowed now:
- product contract;
- command-hook PoC design;
- build/repository scaffolding that does not invent host API contracts;
- test fixtures for clipboard classification independent of AE.
