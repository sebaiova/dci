add_library(
    lexical_analyzer STATIC
    lexical_analyzer.cpp
    state.cpp
)

target_include_directories(lexical_analyzer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})