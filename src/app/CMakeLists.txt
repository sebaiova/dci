set(CMAKE_BUILD_TYPE Release)

add_executable(dci
    main.cpp
    parser.cpp
    logger.cpp
)

target_link_libraries(dci PRIVATE lexical_analyzer syntax_analyzer common)
