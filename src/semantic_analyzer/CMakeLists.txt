#set(CMAKE_BUILD_TYPE Release)

add_library(semantic_analyzer INTERFACE)
target_include_directories(semantic_analyzer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(  semantic_analyzer
                        INTERFACE lexical_analyzer

                        )
