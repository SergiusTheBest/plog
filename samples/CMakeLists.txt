cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0063) #Honor visibility properties for all target types
    cmake_policy(SET CMP0063 NEW)
endif()

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

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_COMPILER_IS_CLANGXX 1)
endif ()

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
    string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
elseif(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -Wnon-virtual-dtor -Wundef -pedantic -Werror")
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
    checkObjCXX()
endif()

include_directories(${CMAKE_SOURCE_DIR}/../include)

file(GLOB_RECURSE PLOG_HEADERS ${CMAKE_SOURCE_DIR}/../include/*.h)
add_library(plog STATIC ${PLOG_HEADERS})
set_target_properties(plog PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(plog PROPERTIES FOLDER Include)

add_executable(Demo Demo/Main.cpp Demo/MyClass.h Demo/MyClass.cpp Demo/Customer.h)
set_target_properties(Demo PROPERTIES FOLDER Samples)

if(MSVC AND NOT (MSVC_VERSION LESS 1700)) # Visual Studio 2012 and higher
    add_executable(DemoManaged Demo/Main.cpp Demo/MyClass.h Demo/MyClass.cpp Demo/Customer.h)
    set_target_properties(DemoManaged PROPERTIES FOLDER Samples)
    set_property(TARGET DemoManaged PROPERTY COMPILE_FLAGS "/clr /EHa")
endif()

if(NOT WIN32)
    add_executable(DemoWchar Demo/Main.cpp Demo/MyClass.h Demo/MyClass.cpp Demo/Customer.h)
    set_target_properties(DemoWchar PROPERTIES FOLDER Samples)
    set_target_properties(DemoWchar PROPERTIES COMPILE_FLAGS "-DPLOG_ENABLE_WCHAR_INPUT=1")
    if(APPLE)
        target_link_libraries(DemoWchar -liconv)
    endif()
endif()

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

# some systems have no shared libraries support, so check it
if(NOT DEFINED TARGET_SUPPORTS_SHARED_LIBS OR TARGET_SUPPORTS_SHARED_LIBS)
    add_executable(ChainedApp Chained/ChainedApp/Main.cpp)
    set_target_properties(ChainedApp PROPERTIES FOLDER Samples/Chained)
    target_link_libraries(ChainedApp ChainedLib)

    add_library(ChainedLib SHARED Chained/ChainedLib/Main.cpp)
    set_target_properties(ChainedLib PROPERTIES FOLDER Samples/Chained)
endif()

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

add_executable(NativeEOL NativeEOL/Main.cpp)
set_target_properties(NativeEOL PROPERTIES FOLDER Samples)

if(WIN32)
    add_executable(EventLog EventLog/Main.cpp)
    set_target_properties(EventLog PROPERTIES FOLDER Samples)

    add_executable(DebugOutput DebugOutput/Main.cpp)
    set_target_properties(DebugOutput PROPERTIES FOLDER Samples)
endif()