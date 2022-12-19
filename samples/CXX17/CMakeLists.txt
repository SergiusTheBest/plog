if (NOT CMAKE_VERSION VERSION_LESS 3.3.0)
    cmake_minimum_required(VERSION 3.3)

    # Unfortunately cxx_std_17 in CMAKE_CXX_COMPILE_FEATURES is not reliable, so check include files
    include(CheckIncludeFileCXX)
    CHECK_INCLUDE_FILE_CXX("string_view" HAS_STRING_VIEW)
    CHECK_INCLUDE_FILE_CXX("filesystem" HAS_FILESYSTEM)

    if(HAS_STRING_VIEW AND HAS_FILESYSTEM)
        add_executable(CXX17 Main.cpp)
        target_link_libraries(CXX17 plog::plog)
        set_target_properties(CXX17 PROPERTIES FOLDER Samples CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
        if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 7.0 AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
            target_link_libraries(CXX17 stdc++fs)
        endif()
    endif()
endif()
