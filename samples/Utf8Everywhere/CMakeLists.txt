if(MSVC AND NOT (MSVC_VERSION LESS 1900)) # Visual Studio 2015 and higher
    add_executable(Utf8Everywhere Main.cpp)
    target_link_libraries(Utf8Everywhere plog)
    set_target_properties(Utf8Everywhere PROPERTIES FOLDER Samples COMPILE_FLAGS "/utf-8")
endif()
