cmake_minimum_required(VERSION 2.8)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

macro(checkObjCXX)
    file(WRITE "${CMAKE_BINARY_DIR}/CMakeFiles/dummy.mm" "int main(){return 0;}\n")
    execute_process(
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/CMakeFiles
      COMMAND ${CMAKE_CXX_COMPILER} dummy.mm
      RESULT_VARIABLE result
      ERROR_QUIET
      OUTPUT_QUIET
      )
    file(REMOVE "${CMAKE_BINARY_DIR}/CMakeFiles/dummy.mm")

    if("${result}" STREQUAL "0")
        set(CMAKE_OBJCXX_AVAILABLE 1)
        message("-- ObjectiveC++ support is detected")
    endif()
endmacro()

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
    checkObjCXX()
endif()

include_directories(${CMAKE_SOURCE_DIR}/../include)

file(GLOB_RECURSE PLOG_HEADERS ${CMAKE_SOURCE_DIR}/../include/*.h)
add_library(plog STATIC ${PLOG_HEADERS})
set_target_properties(plog PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(plog PROPERTIES FOLDER Include)

add_executable(Demo Demo/Main.cpp Demo/MyClass.h Demo/MyClass.cpp) 
set_target_properties(Demo PROPERTIES FOLDER Samples)

add_executable(MultiAppender MultiAppender/Main.cpp) 
set_target_properties(MultiAppender PROPERTIES FOLDER Samples)

add_executable(MultiInstance MultiInstance/Main.cpp) 
set_target_properties(MultiInstance PROPERTIES FOLDER Samples)

add_executable(Facilities Facilities/Main.cpp) 
set_target_properties(Facilities PROPERTIES FOLDER Samples)

add_executable(Hello Hello/Main.cpp) 
set_target_properties(Hello PROPERTIES FOLDER Samples)

add_executable(Performance Performance/Main.cpp) 
set_target_properties(Performance PROPERTIES FOLDER Samples)

add_executable(CustomConverter CustomConverter/Main.cpp) 
set_target_properties(CustomConverter PROPERTIES FOLDER Samples)

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

if(CMAKE_OBJCXX_AVAILABLE AND NOT CMAKE_COMPILER_IS_CLANGXX)
    add_executable(ObjectiveC ObjectiveC/Main.mm)
    target_link_libraries(ObjectiveC objc)
else()
    add_custom_target(ObjectiveC SOURCES ObjectiveC/Main.mm)
endif()
set_target_properties(ObjectiveC PROPERTIES FOLDER Samples)

add_custom_target(Android SOURCES Android/jni/Sample.cpp) 
set_target_properties(Android PROPERTIES FOLDER Samples)

add_executable(LibraryApp Library/LibraryApp/Main.cpp) 
set_target_properties(LibraryApp PROPERTIES FOLDER Samples/Library)
target_link_libraries(LibraryApp LibraryLib)

add_library(LibraryLib STATIC Library/LibraryLib/Lib.cpp) 
set_target_properties(LibraryLib PROPERTIES FOLDER Samples/Library)

add_executable(ColorConsole ColorConsole/Main.cpp) 
set_target_properties(ColorConsole PROPERTIES FOLDER Samples)