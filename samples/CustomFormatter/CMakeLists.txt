add_executable(CustomFormatter Main.cpp)
target_link_libraries(CustomFormatter plog)
set_target_properties(CustomFormatter PROPERTIES FOLDER Samples)