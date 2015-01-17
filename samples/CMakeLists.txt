cmake_minimum_required(VERSION 2.8)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(Samples)

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Werror -Wno-multichar")
endif()

include_directories(${CMAKE_SOURCE_DIR}/../include)

file(GLOB_RECURSE PLOG_HEADERS ${CMAKE_SOURCE_DIR}/../include/*.h)
add_library(plog STATIC ${PLOG_HEADERS})
set_target_properties(plog PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(plog PROPERTIES FOLDER Include)

add_executable(Simple simple/Main.cpp simple/MyClass.h simple/MyClass.cpp) 
set_target_properties(Simple PROPERTIES FOLDER Samples)

add_executable(Multi multi/Main.cpp) 
set_target_properties(Multi PROPERTIES FOLDER Samples)