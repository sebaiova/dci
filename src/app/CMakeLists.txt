set(CMAKE_BUILD_TYPE Release)

add_executable(dci
    main.cpp
    parser.cpp
    logger.cpp
    print_error.cpp
)

# (In src/app/CMakeLists.txt, after add_executable)
if(CMAKE_CROSSCOMPILING)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --static")
endif()

target_link_libraries(dci PRIVATE lexical_analyzer syntax_analyzer common)
