# some systems have no shared libraries support, so check it
if(NOT DEFINED TARGET_SUPPORTS_SHARED_LIBS OR TARGET_SUPPORTS_SHARED_LIBS)
    # reset visibility to default to test more harsh conditions
    # in real code it's recommended to use hidden visibility
    set(CMAKE_CXX_VISIBILITY_PRESET default)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN 0)

    add_executable(NotSharedApp NotSharedApp/Main.cpp)
    target_link_libraries(NotSharedApp PRIVATE plog::plog NotSharedLib1 NotSharedLib2)
    set_target_properties(NotSharedApp PROPERTIES FOLDER Samples/NotShared)
    # define PLOG_LOCAL to make plog instances local (not shared between shared objects)
    target_compile_definitions(NotSharedApp PRIVATE PLOG_LOCAL)

    add_library(NotSharedLib1 SHARED NotSharedLib1/Main.cpp)
    target_link_libraries(NotSharedLib1 PRIVATE plog::plog)
    set_target_properties(NotSharedLib1 PROPERTIES FOLDER Samples/NotShared)
    # define PLOG_LOCAL to make plog instances local (not shared between shared objects)
    target_compile_definitions(NotSharedLib1 PRIVATE PLOG_LOCAL)

    add_library(NotSharedLib2 SHARED NotSharedLib2/Main.cpp)
    target_link_libraries(NotSharedLib2 PRIVATE plog::plog)
    set_target_properties(NotSharedLib2 PROPERTIES FOLDER Samples/NotShared)
    # define PLOG_LOCAL to make plog instances local (not shared between shared objects)
    target_compile_definitions(NotSharedLib2 PRIVATE PLOG_LOCAL)
endif()
