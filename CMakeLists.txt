cmake_minimum_required(VERSION 3.16)

project(compiler VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

enable_testing()

add_subdirectory(src/app)
add_subdirectory(src/common)
add_subdirectory(src/lexical_analyzer)
add_subdirectory(src/semantic_analyzer)
add_subdirectory(src/syntax_analyzer)
add_subdirectory(tests)