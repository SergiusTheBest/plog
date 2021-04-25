cmake_minimum_required(VERSION 3.0)

option(PLOG_BUILD_SAMPLES "Build plog's samples." ON)

project(plog LANGUAGES CXX)

# Make sure install paths work on all platforms.
if(NOT CMAKE_INSTALL_INCLUDEDIR)
    include(GNUInstallDirs)
endif()

add_library(plog INTERFACE)
target_include_directories(plog
    INTERFACE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

add_library(plog::plog ALIAS plog)

#making sure we can build standalone under windows
get_filename_component(CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}   ABSOLUTE)
get_filename_component(SOURCE_DIR ${CMAKE_SOURCE_DIR} ABSOLUTE)

# check if building as a stand-alone project
if(${CURRENT_SOURCE_DIR} STREQUAL ${SOURCE_DIR} AND PLOG_BUILD_SAMPLES)
    # add a pseudo-project to make plog headers visible in IDE
    file(GLOB_RECURSE PLOG_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
    add_library(plog-headers STATIC ${PLOG_HEADERS})
    set_target_properties(plog-headers PROPERTIES LINKER_LANGUAGE CXX)
    set_target_properties(plog-headers PROPERTIES FOLDER Include)

    # add samples
    add_subdirectory(samples)
endif()


# install plog as cmake module
install(TARGETS plog EXPORT plogTargets)

include(GNUInstallDirs)
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/plog
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING # headers only
    PATTERN "*.h"
)
install(FILES "${CMAKE_CURRENT_LIST_DIR}/LICENSE" DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(FILES "${CMAKE_CURRENT_LIST_DIR}/README.md" DESTINATION ${CMAKE_INSTALL_DOCDIR})

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
# this might be a bit too restrictive, since for other (BSD, ...) this might apply also
# but this can be fixed later in extra pull requests from people on the platform
	install(FILES plog-config.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/plog)
	install(EXPORT plogTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/plog)
else()
	install(FILES plog-config.cmake DESTINATION CMake)
	install(EXPORT plogTargets DESTINATION CMake)
endif()

