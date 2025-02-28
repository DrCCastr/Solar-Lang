cmake_minimum_required(VERSION 3.10)
project(SolarProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic -g)

set(SRC_DIR src)
set(BUILD_DIR build)

set(TARGET solar)

file(GLOB_RECURSE SRCS "${SRC_DIR}/*.cpp")

add_executable(${TARGET} ${SRCS})

target_include_directories(${TARGET} PRIVATE ${SRC_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})