add_executable(DisableLogging Main.cpp)
target_link_libraries(DisableLogging plog)
set_target_properties(DisableLogging PROPERTIES FOLDER Samples)
