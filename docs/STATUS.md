# Project Status — Universal Paste

Last updated: 2026-10-05  
Rules baseline: AE-Development-Rules v8.0.0  
Branch: `feat/foundation`  
Risk Profile: Critical  
Delivery Gate: Development

## Goal

Create a predictable Universal Paste workflow for After Effects:
external clipboard content → correct AE layer/object → durable assets → continue animating.

## Confirmed product scope

See [PRODUCT_DISCOVERY.md](PRODUCT_DISCOVERY.md).

v1 core:
- image → layer;
- text → Text Layer;
- HEX/RGB/HSL → Solid;
- multiple files → layers;
- automatic detection;
- durable clipboard assets;
- native AE paste fallback.

## Completed blocks

- repository created and verified;
- current rules v8.0.0 read and applied;
- Stage 0 product contract drafted from confirmed user brief;
- AEGP command-hook feasibility research recorded;
- API compatibility audit started;
- milestone/traceability plan established;
- portable C++17 clipboard model and deterministic classifier implemented;
- strict HEX/RGB/HSL parser implemented;
- core tests built and passed locally;
- GitHub Actions matrix added for macOS / Windows / Linux.

## Current test evidence

Candidate commit for first CI run: `68637fea508577361ad33e20f17eb1a608e7a273`

- local CMake build + CTest: PASS, 1/1;
- GitHub Actions macOS: PASS;
- GitHub Actions Ubuntu: queued at last observation;
- GitHub Actions Windows: queued at last observation.

Queued checks are **NOT RUN/PENDING**, not PASS.

## Current engineering state

Primary PoC target: **After Effects 26.5 + SDK 26.5**.

Architecture candidate: native AEGP core + platform clipboard adapters.

Architecture is **provisional**, pending PoC-01. No production source currently depends on a guessed Paste command ID or unverified AE SDK signature.

## Current blockers / unknowns

| Item | Blocks | Independent work still allowed |
| --- | --- | --- |
| exact 26.5 SDK headers/toolchain in build environment | native plugin compile | core tests, docs, PoC design |
| final minimum AE version | compatibility claim | current-version PoC |
| final OS/architecture support matrix | release build matrix | portable core |
| Paste command identity/runtime behavior | production interception | command probe design |
| unsaved-project storage policy | final persistence design | saved-project storage research |

## Next concrete actions

1. Finish cross-platform core CI.
2. Verify exact `AEGP_RegisterCommandHook` contract against the selected 26.5 SDK headers.
3. Add the non-mutating AEGP command probe.
4. Run PoC-01 in real AE 26.5 and record command/fallback evidence.
5. Freeze Technical Design only after PoC-01 resolves the interception risk.

## Claims allowed now

- Product scope: defined enough for independent foundation work.
- Portable classifier/parser: implemented; local + macOS CI evidence exists.
- AEGP command interception: documented candidate, runtime unverified.
- Native AE plugin implementation: not yet started.
- Validation build: not ready.
- Release: not authorized.
