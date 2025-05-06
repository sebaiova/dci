set(CMAKE_BUILD_TYPE Release)

add_executable(app
    main.cpp
)

target_link_libraries(app PRIVATE
    lexical_analyzer
)