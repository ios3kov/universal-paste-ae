# Requirement Traceability — Universal Paste

Scope revision: v1 foundation / 2026-10-05  
Delivery Gate: Development  
Risk Profile: Critical

| Requirement | Task / milestone | Observable acceptance | Check / phase | Current status |
| --- | --- | --- | --- | --- |
| UP-CORE-01 image → layer | M3 | clipboard image appears in active comp with alpha preserved | TC-IMG-01 runtime | NOT RUN |
| UP-CORE-02 text → Text Layer | M4 | exact Unicode text appears in Text Layer | TC-TEXT-01 runtime | NOT RUN |
| UP-CORE-03 color → Solid | M2 + M4 | parser accepts valid HEX/RGB/HSL; AE solid matches value | TC-COLOR-UNIT + TC-COLOR-AE | NOT RUN |
| UP-CORE-04 multi-file → layers | M4 | N files create N layers in deterministic order | TC-FILES-01 runtime | NOT RUN |
| UP-CORE-05 auto detect | M2 + M3/M4 | supported clipboard type is classified deterministically | TC-CLASSIFY-* | NOT RUN |
| UP-CORE-06 Paste As… | M5 | user can choose another valid interpretation | TC-PASTEAS-01 | NOT RUN |
| UP-CORE-07 persistent assets | M3 | reopened project has no clipboard/temp dependency | TC-ASSET-REOPEN | NOT RUN |
| UP-SAFE-01 safe failure | M3/M4 | failed operation has no silent project corruption | TC-FAIL-* | NOT RUN |
| UP-SAFE-02 native AE paste preserved | M0 + regression | plugin does not consume unsupported/AE-owned paste | POC-01 + TC-NATIVE-PASTE | NOT RUN |

## Risk-control tasks

| Risk | Control |
| --- | --- |
| unstable AE command IDs | PoC-01 + per-supported-build runtime evidence; no invented stable ID |
| command recursion / double paste | handled/unhandled state tests and re-entry guard if evidence shows needed |
| clipboard ambiguity | deterministic classifier + Paste As… |
| missing footage | durable write before dependency + reopen tests |
| partial AE/filesystem mutation | explicit operation state and recovery policy |
| platform divergence | common acceptance tests over separate macOS/Windows adapters |

## Reconciliation status

Implementation is not claimed complete. No runtime PASS exists yet.

The next required evidence is PoC-01 in a real target After Effects build after the selected SDK/toolchain is present.
