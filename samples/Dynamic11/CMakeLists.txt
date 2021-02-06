if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.3.0)
    cmake_minimum_required(VERSION 3.3)

    if(cxx_std_11 IN_LIST CMAKE_CXX_COMPILE_FEATURES)
        add_executable(Dynamic11 Main.cpp)
        target_link_libraries(Dynamic11 plog)
        set_target_properties(Dynamic11 PROPERTIES FOLDER Samples CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)
    endif()
endif()
