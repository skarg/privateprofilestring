# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
# set the project name
project(Unit_Tests)

# In tests allow newer C standard than in the library.
set(CMAKE_C_STANDARD 99)

if (CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_C_COMPILER_ID MATCHES "GNU")
    add_compile_options(-g -O0 -fprofile-arcs -ftest-coverage)
    add_link_options(-fprofile-arcs -ftest-coverage)
    # enable all relevant warnings that find bugs
    add_compile_options(-Wall -Wextra -pedantic)
    add_compile_options(-Wfloat-equal -Wconversion)
    add_compile_options(-Wfloat-conversion -Wdouble-promotion)
    add_compile_options(-Wredundant-decls -Wmissing-declarations)
    add_compile_options(-Wswitch-default)
    add_compile_options(-Wunused-variable)
    add_compile_options(-Wcast-qual)
    add_compile_options(-Wwrite-strings)

    # don't warn about conversion, sign, compares, long long and attributes
    # since they are common in embedded
    add_compile_options(-Wno-sign-conversion -Wno-conversion)
    add_compile_options(-Wno-sign-compare)

    # Just noise from clang
    if (CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_C_COMPILER_ID MATCHES "AppleClang")
      add_compile_options(-Wno-gnu-zero-variadic-macro-arguments)
    endif()
endif()

# Create the lcov target. Run result tests with 'make lcov'
add_custom_target(lcov
    COMMAND mkdir -p lcoverage
    )
add_custom_command(TARGET lcov
    COMMAND echo "=================== LCOV ===================="
    COMMAND echo "-- Passing lcov tool under code coverage"
    COMMAND lcov --capture --directory ../ --output-file lcoverage/main_coverage.info
    COMMAND echo "-- Generating HTML output files"
    COMMAND genhtml lcoverage/main_coverage.info --output-directory lcoverage
    )

#
# add tests
#

list(APPEND testdirs
    src/profile
    src/stptok
    src/rmspace
)

enable_testing()
foreach(testdir IN ITEMS ${testdirs})
    get_filename_component(basename ${testdir} NAME)
    add_subdirectory(${testdir})
    add_test(build_${basename}
    "${CMAKE_COMMAND}"
    --build "${CMAKE_BINARY_DIR}"
    --config "$<CONFIG>"
    --target test_${basename}
    )
    add_test(test_${basename} ${testdir}/test_${basename})
    set_tests_properties(test_${basename}  PROPERTIES FIXTURES_REQUIRED test_fixture)
    set_tests_properties(build_${basename} PROPERTIES FIXTURES_SETUP    test_fixture)
endforeach()

message(STATUS "BACNET: using cmake:....................\"${CMAKE_VERSION}\"")
message(STATUS "BACNET: CMAKE_C_COMPILER_ID:............\"${CMAKE_C_COMPILER_ID}\"")
message(STATUS "BACNET: CMAKE_C_COMPILER_VERSION:.......\"${CMAKE_C_COMPILER_VERSION}\"")
message(STATUS "BACNET: CMAKE_CXX_COMPILER_ID:..........\"${CMAKE_CXX_COMPILER_ID}\"")
message(STATUS "BACNET: CMAKE_CXX_COMPILER_VERSION:.....\"${CMAKE_CXX_COMPILER_VERSION}\"")
message(STATUS "BACNET: CMAKE_INSTALL_PREFIX:...........\"${CMAKE_INSTALL_PREFIX}\"")
