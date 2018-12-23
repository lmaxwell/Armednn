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

IF (WIN32)
    MESSAGE(STATUS "windows")
ELSEIF (APPLE)
    MESSAGE(STATUS "Apple")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -std=c++11")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -std=c++11 -msse4.2 -mavx2 -mfma ")
ELSEIF (UNIX)
    MESSAGE(STATUS "UNIX OS")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -std=c++11")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -Wall -std=c++11 -msse4.2 -mavx2 -mfma -fabi-version=6 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -std=c++11 -msse4.2 -mavx2 -mfma  -fabi-version=6")
ENDIF ()

target_compile_definitions(tsnn PRIVATE __DEBUG__)

