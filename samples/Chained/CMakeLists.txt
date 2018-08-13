# some systems have no shared libraries support, so check it
if(NOT DEFINED TARGET_SUPPORTS_SHARED_LIBS OR TARGET_SUPPORTS_SHARED_LIBS)
    add_executable(ChainedApp ChainedApp/Main.cpp)
    target_link_libraries(ChainedApp ChainedLib plog)
    set_target_properties(ChainedApp PROPERTIES FOLDER Samples/Chained)    

    add_library(ChainedLib SHARED ChainedLib/Main.cpp)
    target_link_libraries(ChainedLib plog)
    set_target_properties(ChainedLib PROPERTIES FOLDER Samples/Chained)    
endif()