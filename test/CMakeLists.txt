cmake_minimum_required(VERSION 3.0)

project(PlogTest CXX)

add_executable(${PROJECT_NAME}
    doctest.h
    Conditional.cpp
    TestAppender.h
    Main.cpp
    Printf.cpp
    SimpleTypes.cpp
    StringTypes.cpp
    StdManipulators.cpp
    StdContainers.cpp
)
target_link_libraries(${PROJECT_NAME} plog::plog)
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DDOCTEST_CONFIG_NO_POSIX_SIGNALS")
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Test)

add_test(${PROJECT_NAME} ${PROJECT_NAME})
