cmake_minimum_required(VERSION 3.0)
project(tsnn)

add_library(tsnn SHARED src/tsnn/network.cpp src/tsnn/layer.cpp src/tsnn/config.cpp src/layers/concat.cpp src/layers/split.cpp src/layers/dense.cpp)

target_include_directories(tsnn
        PUBLIC
            $<INSTALL_INTERFACE:include,thirdparty/eigen>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/eigen>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/tsnn
            ${CMAKE_CURRENT_SOURCE_DIR}/src/layers
        )

target_compile_features(tsnn PRIVATE cxx_std_11)

