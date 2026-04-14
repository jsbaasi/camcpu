cmake_minimum_required(VERSION 3.16)
project(camcpu)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)
set(SDL3_DIR "vendor/SDL")

# Create your game executable target as usual
add_executable(
    camcpu
    main.c
)

# Link to the actual SDL2 library.
target_link_libraries(camcpu PRIVATE SDL3::SDL3)

# library for code used by both main and tests
add_library(camcpu_lib STATIC
    linear.c
)

target_include_directories(camcpu_lib PUBLIC
    "include/"
)

# target_link_libraries(camcpu_lib PRIVATE SDL3::SDL3)
# Link main against the shared lib
target_link_libraries(camcpu PRIVATE camcpu_lib)


# Test target
add_executable(camcpu_tests
    tests/test_main.c
)
target_link_libraries(camcpu_tests PRIVATE camcpu_lib)
enable_testing()

add_test(NAME camcpu_tests COMMAND camcpu_tests)
