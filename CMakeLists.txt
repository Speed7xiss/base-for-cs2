cmake_minimum_required(VERSION 3.22)
project(cs2_offline_review VERSION 0.2.0 LANGUAGES CXX)
option(CS2_BUILD_TESTS "Build unit tests" ON)
option(CS2_BUILD_DX11_UI "Build the Windows Dear ImGui + DirectX 11 viewer" OFF)
add_library(cs2_core
 src/core/csv_reader.cpp
 src/core/demo_dataset.cpp
 src/core/schema_registry.cpp
 src/aim_analysis/metrics.cpp
 src/configuration/settings.cpp
 src/visuals/geometry.cpp
 src/updater/update_manager.cpp)
target_include_directories(cs2_core PUBLIC include)
target_compile_features(cs2_core PUBLIC cxx_std_20)
if(MSVC)
 target_compile_options(cs2_core PRIVATE /W4 /permissive- /EHsc)
else()
 target_compile_options(cs2_core PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
endif()
add_executable(cs2_review src/main.cpp)
target_link_libraries(cs2_review PRIVATE cs2_core)
if(CS2_BUILD_DX11_UI)
 message(FATAL_ERROR "DX11/ImGui frontend is not implemented; see README.md.")
endif()
if(CS2_BUILD_TESTS)
 enable_testing()
 add_executable(cs2_tests tests/test_main.cpp)
 target_link_libraries(cs2_tests PRIVATE cs2_core)
 add_test(NAME cs2_tests COMMAND cs2_tests)
endif()
