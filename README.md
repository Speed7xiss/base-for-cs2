# CS2 Offline Review

C++20 foundation for reviewing authorized, offline Counter-Strike 2 demo/spectator exports. It does not attach to the game, inspect process memory, inject code, install drivers, access DMA, or provide live-match information advantages.

## Current milestone (0.2.0)

- Portable C++20 core, CMake build, CSV ingestion, and CLI reporting.
- Offline player geometry data model and aim-analysis metrics.
- Versioned export-schema registry: logical field names map to CSV column names, with duplicate/missing-header validation. These are not live-game memory offsets.
- Update policy coordinator with product/channel/version checks, HTTPS-only URL enforcement, signed-manifest verification hook, SHA-256 verification hook, and staged-installer hook.
- Dependency-free unit-test target and CI workflow.

## Build

```sh
cmake -S . -B build -DCS2_BUILD_TESTS=ON
cmake --build build --config Release
ctest --test-dir build --output-on-failure
```

## Run

```sh
./build/cs2_review configs/players.example.csv configs/aim.example.csv
```

Inputs must come from an authorized offline demo/spectator export pipeline.

## Important implementation boundary

The updater currently provides policy and interfaces only. It does not yet include a concrete HTTPS client, Ed25519/SHA-256 implementation, package format, or platform installer. Automatic updates must remain disabled until those adapters are implemented and independently tested. See [docs/UPDATE_SECURITY.md](docs/UPDATE_SECURITY.md).

The DirectX 11 / Dear ImGui frontend and native demo parser are also not implemented. The application remains an offline review tool; it is not an external live-game cheat.

## Layout

- `include/cs2/`: data model and interfaces.
- `src/core/`: CSV, dataset, and export-schema handling.
- `src/visuals/`: renderer-independent geometry.
- `src/aim_analysis/`: offline statistics.
- `src/configuration/`: settings persistence.
- `src/updater/`: update policy orchestration.
- `docs/`: security and architecture notes.
- `tests/`: unit tests.
