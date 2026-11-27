# Build validation

Configure with CMake 3.22 or newer and a C++20 compiler:

```sh
cmake -S . -B build -DCS2_BUILD_TESTS=ON
cmake --build build --parallel 2
ctest --test-dir build --output-on-failure
```

The Windows Dear ImGui/DirectX 11 frontend and the updater are not included in milestone 0.1.0.