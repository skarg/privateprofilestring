# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

get_filename_component(basename ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(test_${basename}
    VERSION 1.0.0
    LANGUAGES C)

string(REGEX REPLACE
    "/test/src/[a-zA-Z0-9_/-]*$"
    "/src"
    SRC_DIR
    ${CMAKE_CURRENT_SOURCE_DIR})
string(REGEX REPLACE
    "/test/src/[a-zA-Z0-9_/-]*$"
    "/test"
    TST_DIR
    ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
    ${SRC_DIR}
    ${TST_DIR}
    )

add_executable(${PROJECT_NAME}
    # File(s) under test
    ${SRC_DIR}/profile.c
    # Support files and stubs (pathname alphabetical)
    ${SRC_DIR}/rmspace.c
    ${SRC_DIR}/stptok.c
    # Test and test library files
    ./src/main.c
    )
