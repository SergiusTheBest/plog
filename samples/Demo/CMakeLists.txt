add_executable(Demo Main.cpp MyClass.h MyClass.cpp Customer.h)
target_link_libraries(Demo plog)
set_target_properties(Demo PROPERTIES FOLDER Samples)

if(MSVC AND NOT (MSVC_VERSION LESS 1700)) # Visual Studio 2012 and higher
    add_executable(DemoManaged Main.cpp MyClass.h MyClass.cpp Customer.h)
    target_link_libraries(DemoManaged plog)
    set_property(TARGET DemoManaged PROPERTY COMPILE_FLAGS "/clr /EHa")
    set_target_properties(DemoManaged PROPERTIES FOLDER Samples)
endif()

if(NOT WIN32)
    add_executable(DemoWchar Main.cpp MyClass.h MyClass.cpp Customer.h)
    target_link_libraries(DemoWchar plog)
    set_target_properties(DemoWchar PROPERTIES COMPILE_FLAGS "-DPLOG_ENABLE_WCHAR_INPUT=1")
    set_target_properties(DemoWchar PROPERTIES FOLDER Samples)

    if(APPLE)
        target_link_libraries(DemoWchar -liconv)
    endif()
endif()