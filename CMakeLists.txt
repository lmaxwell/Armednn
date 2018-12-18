cmake_minimum_required(VERSION 3.0)
project(tsnn)


add_library(tsnn SHARED  src/ops/base.cpp src/tsnn/network.cpp src/tsnn/layer.cpp src/tsnn/config.cpp src/layers/concat.cpp src/layers/split.cpp src/layers/dense.cpp src/tsnn/core.cpp)

target_include_directories(tsnn
        PUBLIC
            $<INSTALL_INTERFACE:include,thirdparty/eigen>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/eigen>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/tsnn
            ${CMAKE_CURRENT_SOURCE_DIR}/src/layers
        )
#set(CMAKE_CXX_FLAGS "-std=c++11 -march=native -o3 -fabi-version=6 ")
set(CMAKE_CXX_FLAGS "-std=c++11 -msse4.2 -mavx2 -mfma -O3 -fabi-version=6 -DNDEBUG ")
target_compile_definitions(tsnn PRIVATE __DEBUG__)
#target_compile_features(tsnn PRIVATE cxx_std_11 -march=native )

