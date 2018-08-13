add_executable(CustomAppender Main.cpp)
target_link_libraries(CustomAppender plog)
set_target_properties(CustomAppender PROPERTIES FOLDER Samples)