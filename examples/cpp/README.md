# Build Paddle2ONNX C++ Executable with prebuilt library

1. Use [p2o_exec.cpp](../../p2o_exec.cpp) at the Paddle2ONNX project root.
1. Install `boost-program-options` (via vcpkg is fine).
1. Download prebuilt library from Paddle2ONNX release.
1. Create a directory named `3rdparty/paddle2onnx` (or elsewhere, but remeber to modify the `paddle2onnx_install_path` in the *CMakeLists.txt*), then put `include` and `lib` from the build under it.
1. The project structure should look like:
   ```
   PROJECT_ROOT
   │ CMakeLists.txt
   | p2o_exec.cpp
   └───3rdparty
       └───paddle2onnx
           ├───include
           │   └───paddle2onnx
           │           converter.h
           └───lib
                   paddle2onnx.dll
                   paddle2onnx.lib
   ```
1. Use the *CMakeLists.txt* as shown below.

```cmake
cmake_minimum_required(VERSION 3.25)
project(p2o)

set(CMAKE_CXX_STANDARD 17)
set(paddle2onnx_install_path ${PROJECT_SOURCE_DIR}/3rdparty/paddle2onnx)

find_package(Boost REQUIRED COMPONENTS program_options)
find_library(paddle2onnx NAMES paddle2onnx PATHS ${paddle2onnx_install_path}/lib NO_DEFAULT_PATH)

add_executable(p2o p2o_exec.cpp)
target_include_directories(p2o PRIVATE ${paddle2onnx_install_path}/include)
target_link_directories(p2o PRIVATE ${paddle2onnx_install_path}/lib)
target_link_libraries(p2o PRIVATE Boost::program_options paddle2onnx)

if (WIN32)
    add_custom_command(
            TARGET p2o POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${PROJECT_SOURCE_DIR}/lib/paddle2onnx.dll
                ${PROJECT_BINARY_DIR}
            )
endif()
```
