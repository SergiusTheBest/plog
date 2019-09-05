add_executable(LibraryApp LibraryApp/Main.cpp)
target_link_libraries(LibraryApp LibraryLib plog)
set_target_properties(LibraryApp PROPERTIES FOLDER Samples/Library)

add_library(LibraryLib STATIC LibraryLib/Lib.cpp)
target_link_libraries(LibraryLib plog)
set_target_properties(LibraryLib PROPERTIES FOLDER Samples/Library)