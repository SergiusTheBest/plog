if(ANDROID)
    add_library(AndroidJNI SHARED AndroidJNI/Main.cpp)
    target_link_libraries(AndroidJNI plog)

    add_executable(AndroidNative AndroidNative/Main.cpp)
    target_link_libraries(AndroidNative plog)
else()
    add_custom_target(AndroidJNI SOURCES AndroidJNI/Main.cpp)
    add_custom_target(AndroidNative SOURCES AndroidNative/Main.cpp)
endif()

set_target_properties(AndroidJNI PROPERTIES FOLDER Samples/Android)
set_target_properties(AndroidNative PROPERTIES FOLDER Samples/Android)
