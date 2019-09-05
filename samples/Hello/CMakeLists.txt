add_executable(Hello Main.cpp)
target_link_libraries(Hello plog)
set_target_properties(Hello PROPERTIES FOLDER Samples)