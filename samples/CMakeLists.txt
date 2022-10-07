cmake_minimum_required(VERSION 3.0)

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

add_subdirectory(Android)
add_subdirectory(AscDump)
add_subdirectory(Chained)
add_subdirectory(ColorConsole)
add_subdirectory(CustomAppender)
add_subdirectory(CustomConverter)
add_subdirectory(CustomFormatter)
add_subdirectory(CustomType)
add_subdirectory(CXX11)
add_subdirectory(CXX17)
add_subdirectory(DebugOutput)
add_subdirectory(Demo)
add_subdirectory(DisableLogging)
add_subdirectory(DynamicAppender)
add_subdirectory(EventLog)
add_subdirectory(Facilities)
add_subdirectory(Hello)
add_subdirectory(HexDump)
add_subdirectory(Library)
add_subdirectory(MultiAppender)
add_subdirectory(MultiInstance)
add_subdirectory(NotShared)
add_subdirectory(ObjectiveC)
add_subdirectory(Path)
add_subdirectory(Performance)
add_subdirectory(PrintVar)
add_subdirectory(SetFileName)
add_subdirectory(Shared)
add_subdirectory(SkipNativeEOL)
add_subdirectory(UtcTime)
