# some systems have no shared libraries support, so check it
if(NOT DEFINED TARGET_SUPPORTS_SHARED_LIBS OR TARGET_SUPPORTS_SHARED_LIBS)
    add_executable(ChainedApp ChainedApp/Main.cpp)
    target_link_libraries(ChainedApp ChainedLib plog)
    set_target_properties(ChainedApp PROPERTIES FOLDER Samples/Chained)
    # set PLOG to local, so instances will not be shared across modules
    target_compile_definitions(ChainedApp PRIVATE PLOG_LOCAL)

    add_library(ChainedLib SHARED ChainedLib/Main.cpp)
    target_link_libraries(ChainedLib plog)
    set_target_properties(ChainedLib PROPERTIES FOLDER Samples/Chained)
    # set PLOG to local, so instances will not be shared across modules
    target_compile_definitions(ChainedLib PRIVATE PLOG_LOCAL)
endif()
