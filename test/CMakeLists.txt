if(${CMAKE_VERSION} VERSION_LESS 3.27.0)
    cmake_minimum_required(VERSION 3.1)
else()
    cmake_minimum_required(VERSION 3.6)
endif()

project(PlogTest CXX)

#
# Specify test sources
#

set(SOURCES
    CastToString.cpp
    Common.h
    Conditional.cpp
    TestAppender.h
    Main.cpp
    NullCharPointer.cpp
    Path.cpp
    Printf.cpp
    SimpleTypes.cpp
    StringTypes.cpp
    StdContainers.cpp
    StdManipulators.cpp
    StdStreamable.cpp
)

#
# Check wchar_t platform support
#

include(CheckTypeSize)
CHECK_TYPE_SIZE(wchar_t PLOG_SIZEOF_WCHAR)

#
# Test creation functions to avoid code duplication
#

function(plog_add_test _target)
    add_executable(${_target} ${SOURCES})

    target_link_libraries(${_target} plog::plog)
    target_compile_definitions(${_target} PRIVATE DOCTEST_CONFIG_NO_POSIX_SIGNALS DOCTEST_CONFIG_NO_MULTITHREADING)
    set_target_properties(${_target} PROPERTIES FOLDER Test)

    # Add std::filesystem support library for GCC
    if(CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 7.0 AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
        target_link_libraries(${_target} stdc++fs)
    endif()

    # Enable conformance mode to be more strict
    if(MSVC AND NOT (MSVC_VERSION LESS 1900)) # Visual Studio 2015 and higher
        target_compile_options(${_target} PRIVATE "/permissive-")
    endif()

    add_test(${_target} ${_target})
endfunction()

function(plog_add_test_utf8 _target)
    if(MSVC AND NOT (MSVC_VERSION LESS 1900)) # Visual Studio 2015 and higher
        plog_add_test(${_target})
        target_compile_options(${_target} PRIVATE "/utf-8")
    endif()
endfunction()

function(plog_add_test_wchar _target)
    if(NOT WIN32 AND NOT BSD AND PLOG_SIZEOF_WCHAR) # FIXME: for some reason wchar_t is not working on BSD
        plog_add_test(${_target})
        target_compile_definitions(${_target} PRIVATE PLOG_ENABLE_WCHAR_INPUT=1)

        if(APPLE)
            target_link_libraries(${_target} -liconv)
        endif()
    endif()
endfunction()

#
# Create a basic test with default compiler features
#

plog_add_test(${PROJECT_NAME})

#
# Create a test for Utf8Everywhere mode with default compiler features
#

plog_add_test_utf8(${PROJECT_NAME}_utf8)

#
# Create a test for wchar input with default compiler features
#

plog_add_test_wchar(${PROJECT_NAME}_wchar)


#
# Check if running a descent version of CMake and enable it
#

if(CMAKE_VERSION VERSION_LESS 3.8.0)
    return()
endif()

cmake_policy(VERSION 3.8)

set(PLOG_CXX_STANDARDS_LIST LIST cxx_std_11 cxx_std_14 cxx_std_17 cxx_std_20 cxx_std_23)

function(plog_get_latest_cxx_std _output)
    foreach(cxx_std IN ITEMS ${PLOG_CXX_STANDARDS_LIST})
        if(${cxx_std} IN_LIST CMAKE_CXX_COMPILE_FEATURES)
            set(${_output} ${cxx_std} PARENT_SCOPE)
        endif()
    endforeach()
endfunction()

plog_get_latest_cxx_std(PLOG_LATEST_CXX_STD)

function(plog_set_target_cxx_standard _target _cxx_std)
    if(TARGET ${_target})
        target_compile_features(${_target} PRIVATE ${_cxx_std})
    endif()
endfunction()

#
# Create basic tests for different C++ Standard versions
#

foreach(cxx_std IN ITEMS ${PLOG_CXX_STANDARDS_LIST})
    if(${cxx_std} IN_LIST CMAKE_CXX_COMPILE_FEATURES)
        plog_add_test(${PROJECT_NAME}_${cxx_std})
        plog_set_target_cxx_standard(${PROJECT_NAME}_${cxx_std} ${cxx_std})
    endif()
endforeach()

#
# Create a test for Utf8Everywhere mode with the latest C++ Standard
#

if(PLOG_LATEST_CXX_STD)
    plog_add_test_utf8(${PROJECT_NAME}_${PLOG_LATEST_CXX_STD}_utf8)
    plog_set_target_cxx_standard(${PROJECT_NAME}_${PLOG_LATEST_CXX_STD}_utf8 ${PLOG_LATEST_CXX_STD})
endif()

#
# Create a test for wchar input with the latest C++ Standard
#

if(PLOG_LATEST_CXX_STD)
    plog_add_test_wchar(${PROJECT_NAME}_${PLOG_LATEST_CXX_STD}_wchar)
    plog_set_target_cxx_standard(${PROJECT_NAME}_${PLOG_LATEST_CXX_STD}_wchar ${PLOG_LATEST_CXX_STD})
endif()
