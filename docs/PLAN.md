# Development Plan — Universal Paste

Rules baseline: AE-Development-Rules v8.0.0  
Risk Profile: Critical  
Delivery Gate: Development  
Current branch: `feat/foundation`

## Why Critical

Universal Paste intends to intercept a core host command, mutate projects and create persistent files. A bad implementation can break normal AE paste behavior, create missing footage or leave partial project state.

## Milestones

### M0 — Product + feasibility
Status: IN PROGRESS

- Product discovery / v1 scope.
- API/source audit.
- Prove whether Paste can be safely observed and selectively consumed.
- Resolve target AE/OS matrix enough for implementation.
- Define unsaved-project asset policy.

Exit:
- PoC-01 has real AE evidence.
- No hard-coded undocumented Paste ID is accepted without a version-safe strategy.
- Technical architecture can be justified from evidence.

### M1 — Core foundation
- Build system.
- Build ID / source identity.
- Native AEGP shell using verified selected SDK.
- Structured diagnostic logging for command probe.
- No production clipboard interception yet.

Exit:
- plugin loads in target AE;
- exact loaded build is identifiable;
- command probe produces evidence without changing project state.

### M2 — Clipboard classifier
- Common clipboard type model.
- Color parser: HEX/RGB/HSL.
- deterministic precedence rules;
- ambiguity model for Paste As…;
- unit tests independent of AE.

Exit:
- classification tests cover valid, invalid and ambiguous inputs.

### M3 — Safe persistence + image path
- platform clipboard image adapter;
- durable write contract;
- import into AE;
- layer insertion;
- alpha preservation;
- undo/recovery behavior;
- saved/unsaved project storage semantics.

### M4 — Text / color / multi-file
- text → Text Layer;
- color → Solid;
- files → imported layers;
- deterministic ordering;
- partial-failure handling.

### M5 — Paste As…
- explicit command/menu/UI;
- only valid interpretations;
- keyboard workflow;
- accessibility/HiDPI checks if UI is present.

### M6 — Compatibility + validation
- macOS/Windows builds for approved matrix;
- runtime Regression Level 2 because command interception is host-wide;
- lifecycle/restart/reload;
- native AE copy/paste regression;
- asset persistence after restart/reopen;
- validation build only if explicitly requested.

### M7 — Release
Not authorized.

Release/package/publication starts only on an explicit user instruction to release/publish.

## Parallel work policy

Do in parallel where state is independent:
- API research;
- pure parser/classifier implementation and tests;
- fixtures;
- docs/traceability;
- platform adapter research.

Do not parallelize runtime AE tests against one shared AE instance or shared preferences.

## Stop / escalation conditions

Stop only the dependent path when:
- selected AE SDK/header does not support a required API;
- Paste cannot be distinguished safely;
- consuming Paste breaks native AE fallback;
- asset persistence would risk data loss;
- a required runtime test is unavailable.

Independent safe work continues while a blocker is investigated.
