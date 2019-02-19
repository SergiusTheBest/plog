cmake_minimum_required(VERSION 2.8)

project(plog LANGUAGES CXX)

add_library(plog INTERFACE)
target_include_directories(plog INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(plog::plog ALIAS plog)

#making sure we can build standalone under windows
get_filename_component(CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}   ABSOLUTE)
get_filename_component(SOURCE_DIR ${CMAKE_SOURCE_DIR} ABSOLUTE)

# check if building as a stand-alone project
if(${CURRENT_SOURCE_DIR} STREQUAL ${SOURCE_DIR})
    # add a pseudo-project to make plog headers visible in IDE
    file(GLOB_RECURSE PLOG_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
    add_library(plog-headers STATIC ${PLOG_HEADERS})
    set_target_properties(plog-headers PROPERTIES LINKER_LANGUAGE CXX)
    set_target_properties(plog-headers PROPERTIES FOLDER Include)

    # add samples
    add_subdirectory(samples)
endif()