if (NOT CMAKE_VERSION VERSION_LESS 3.3.0)
    cmake_minimum_required(VERSION 3.3)

    if(cxx_std_11 IN_LIST CMAKE_CXX_COMPILE_FEATURES)
        add_executable(CXX11 Main.cpp)
        target_link_libraries(CXX11 plog::plog)
        set_target_properties(CXX11 PROPERTIES FOLDER Samples CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)
    endif()
endif()
