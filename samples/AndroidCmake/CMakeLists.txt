add_executable(AndroidCmake Main.cpp)
target_link_libraries(AndroidCmake plog)
set_target_properties(AndroidCmake PROPERTIES FOLDER Samples)
