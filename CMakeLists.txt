cmake_minimum_required(VERSION 3.14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(data-visualization-cpp VERSION 1.0)
include(FetchContent)

FetchContent_Declare(matplotplusplus
        GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus
        GIT_TAG "v1.2.1")

FetchContent_GetProperties(matplotplusplus)
if(NOT matplotplusplus_POPULATED)
    FetchContent_Populate(matplotplusplus)
    add_subdirectory(${matplotplusplus_SOURCE_DIR} ${matplotplusplus_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
    GIT_TAG "v3.0"
)

FetchContent_MakeAvailable(argparse)

add_executable(data-visualization-cpp ./src/main.cpp)
target_link_libraries(data-visualization-cpp PUBLIC argparse matplot rapidcvs)