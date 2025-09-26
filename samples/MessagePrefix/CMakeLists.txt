add_executable(MessagePrefix Main.cpp)
target_link_libraries(MessagePrefix plog::plog)
set_target_properties(MessagePrefix PROPERTIES FOLDER Samples)
