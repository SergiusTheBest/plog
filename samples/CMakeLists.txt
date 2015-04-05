cmake_minimum_required(VERSION 2.8)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(Samples)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_COMPILER_IS_CLANGXX 1)
endif ()

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
elseif(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
endif()

include_directories(${CMAKE_SOURCE_DIR}/../include)

file(GLOB_RECURSE PLOG_HEADERS ${CMAKE_SOURCE_DIR}/../include/*.h)
add_library(plog STATIC ${PLOG_HEADERS})
set_target_properties(plog PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(plog PROPERTIES FOLDER Include)

add_executable(Simple Simple/Main.cpp Simple/MyClass.h Simple/MyClass.cpp) 
set_target_properties(Simple PROPERTIES FOLDER Samples)

add_executable(MultiAppender MultiAppender/Main.cpp) 
set_target_properties(MultiAppender PROPERTIES FOLDER Samples)

add_executable(MultiInstance MultiInstance/Main.cpp) 
set_target_properties(MultiInstance PROPERTIES FOLDER Samples)

add_executable(Hello Hello/Main.cpp) 
set_target_properties(Hello PROPERTIES FOLDER Samples)

add_executable(CustomFormatter CustomFormatter/Main.cpp) 
set_target_properties(CustomFormatter PROPERTIES FOLDER Samples)

add_executable(CustomAppender CustomAppender/Main.cpp) 
set_target_properties(CustomAppender PROPERTIES FOLDER Samples)

add_executable(CustomType CustomType/Main.cpp) 
set_target_properties(CustomType PROPERTIES FOLDER Samples)

add_executable(ChainedApp Chained/ChainedApp/Main.cpp) 
set_target_properties(ChainedApp PROPERTIES FOLDER Samples/Chained)
target_link_libraries(ChainedApp ChainedLib)

add_library(ChainedLib SHARED Chained/ChainedLib/Main.cpp) 
set_target_properties(ChainedLib PROPERTIES FOLDER Samples/Chained)

if(NOT MSVC AND NOT CMAKE_COMPILER_IS_CLANGXX)
    add_executable(ObjectiveC ObjectiveC/Main.mm) 
    set_target_properties(ObjectiveC PROPERTIES FOLDER Samples)
    target_link_libraries(ObjectiveC objc)
endif()