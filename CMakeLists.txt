cmake_minimum_required(VERSION 3.22)

set(target "SeachInAI")
set(compile_flags -pipe -Wall -Wextra -Weffc++ -Ofast -g)
set(compile_def _REENTRANT _FORTIFY_SOURCE=2 _GLIBCXX_ASSERTIONS LOG)
set(linking_flags -rdynamic)
set(linking_libs )

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(${target})

file(GLOB_RECURSE lib_includes "src/*.h")
file(GLOB_RECURSE lib_sources "src/*.cpp")
add_library(${target} ${lib_includes} ${lib_sources})


set(run-experiments-sources "executables/run-experiments.cpp")
set(run-experiments "run-experiments")
add_executable(${run-experiments} ${run-experiments-sources})
target_link_libraries(${run-experiments} PRIVATE ${target})

set(run-single-scenario-sources "executables/run-single-scenario.cpp")
set(run-single-scenario "run-single-scenario")
add_executable(${run-single-scenario} ${run-single-scenario-sources})
target_link_libraries(${run-single-scenario} PRIVATE ${target})