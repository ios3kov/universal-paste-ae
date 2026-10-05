# Universal Paste for After Effects

**Copied → pasted → animate.**

Universal Paste turns external clipboard content into ready-to-use After Effects layers without the save/import detour.

## v1 scope

- image → layer with alpha preserved;
- text → Text Layer;
- HEX / RGB / HSL → Solid;
- multiple files → multiple layers;
- automatic clipboard-type detection;
- Paste As… for ambiguous content;
- durable storage for clipboard-created assets;
- safe fallback to native AE paste.

## Current status

Development branch: `feat/foundation`  
Rules baseline: `AE-Development-Rules v8.0.0`  
Risk Profile: **Critical**  
Delivery Gate: **Development**

Implemented so far:
- product contract and requirements;
- AEGP command-interception research;
- API compatibility audit;
- portable C++17 clipboard classification model;
- strict HEX/RGB/HSL color parser;
- core unit tests;
- macOS / Windows / Linux core CI.

The native AE command interception is still a **PoC**, not a verified production mechanism. No hard-coded Paste command ID is accepted without runtime evidence.

## Core tests

```bash
cmake -S . -B build -DUNIVERSAL_PASTE_BUILD_TESTS=ON
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

## Project records

- [Product Discovery](docs/PRODUCT_DISCOVERY.md)
- [Technical Research](docs/TECHNICAL_RESEARCH.md)
- [API Compatibility Audit](docs/API_COMPATIBILITY_AUDIT.md)
- [Development Plan](docs/PLAN.md)
- [Requirement Traceability](docs/REQUIREMENT_TRACEABILITY.md)
- [Current Status](docs/STATUS.md)

## Release

No release/publication is authorized at the current stage.
