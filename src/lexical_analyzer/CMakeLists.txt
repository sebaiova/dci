set(CMAKE_BUILD_TYPE Release)

add_library(
    lexical_analyzer STATIC
    lexical_analyzer.cpp
    state.cpp
)

target_include_directories(lexical_analyzer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lexical_analyzer common)
