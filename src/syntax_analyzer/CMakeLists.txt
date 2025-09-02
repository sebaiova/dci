set(CMAKE_BUILD_TYPE Release)

add_library(
    syntax_analyzer STATIC
    syntax_analyzer.cpp
)

target_include_directories(syntax_analyzer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(syntax_analyzer 
            common
            lexical_analyzer
            )
