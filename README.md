# CS2 Offline Review

C++20 foundation for reviewing **authorized, offline Counter-Strike 2 demo/spectator exports**. It does not attach to the game, inspect process memory, inject code, install drivers, access DMA, or provide live-match information advantages.

## Current milestone (0.1.0)

- Portable C++20 core and CMake build.
- CSV ingestion for normalized player snapshots and aim telemetry.
- Player geometry helpers and skeleton-segment data model.
- Offline aim metrics: angular error mean/deviation, view angular speed, target switches, and tracking coverage.
- JSON settings persistence and CLI report.
- Dependency-free unit tests.

The DirectX 11 / Dear ImGui frontend, native demo parser, HTTPS updater transport, SHA-256 package verification, and code-signature trust policy are **not implemented in this milestone**. They must be added and tested separately. The DX11 UI build option fails explicitly rather than pretending a UI exists.

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

Player CSV header: `time_seconds,steam_id,name,team,health,weapon,distance,screen_x,screen_y,screen_w,screen_h,bones`. Optional bones are semicolon-separated segments in `x1:y1|x2:y2` format. Aim CSV header: `time_seconds,view_yaw,view_pitch,target_yaw,target_pitch,target_id`. Inputs must come from an authorized offline demo/spectator export pipeline.

## Layout

- `include/cs2/`: interfaces and data model.
- `src/core/`: CSV and dataset loading.
- `src/visuals/`: renderer-independent geometry.
- `src/aim_analysis/`: offline statistics.
- `src/configuration/`: settings persistence.
- `src/interface/` and `src/updater/`: reserved for later milestones.
- `tests/`: unit tests.

A future updater must use HTTPS certificate validation, publisher signature trust, SHA-256 verification, and atomic staging/rollback. A hash alone is not authenticity. No updater is shipped yet.
