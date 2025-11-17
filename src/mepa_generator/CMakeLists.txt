#set(CMAKE_BUILD_TYPE Release)

add_library(mepa_generator INTERFACE)
target_include_directories(mepa_generator INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(  mepa_generator
                        )
