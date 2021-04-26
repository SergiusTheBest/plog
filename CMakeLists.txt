cmake_minimum_required(VERSION 3.0)

project(plog VERSION 1.1.5 LANGUAGES CXX)

include(GNUInstallDirs)

add_library(plog INTERFACE)
target_include_directories(plog
    INTERFACE
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)

add_library(plog::plog ALIAS plog)

# workaround for stand-alone build detection under windows (as it's case insensitive)
get_filename_component(CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE)
get_filename_component(SOURCE_DIR ${CMAKE_SOURCE_DIR} ABSOLUTE)

# check if building as a stand-alone project
if(CURRENT_SOURCE_DIR STREQUAL SOURCE_DIR)
    set(PLOG_MASTER_PROJECT ON)
endif()

# options
option(PLOG_BUILD_SAMPLES "Build plog's samples" ${PLOG_MASTER_PROJECT})
option(PLOG_INSTALL "Generate plog's install target" ${PLOG_MASTER_PROJECT})

if(PLOG_BUILD_SAMPLES)
    # add a pseudo-project to make plog headers visible in IDE
    file(GLOB_RECURSE PLOG_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
    add_library(plog-headers STATIC ${PLOG_HEADERS})
    set_target_properties(plog-headers PROPERTIES LINKER_LANGUAGE CXX)
    set_target_properties(plog-headers PROPERTIES FOLDER Include)

    # add samples
    add_subdirectory(samples)
endif()

if(PLOG_INSTALL)
    # install plog as cmake module
    install(TARGETS plog EXPORT plogConfig)

    install(
        EXPORT plogConfig
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/plog"
        NAMESPACE plog::
    )

    install(
        DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/plog"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        FILES_MATCHING PATTERN "*.h" # headers only
    )

    install(
        FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
            "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"
        DESTINATION "${CMAKE_INSTALL_DOCDIR}"
    )

    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/plogConfigVersion.cmake"
        COMPATIBILITY SameMajorVersion
    )

    install(
        FILES "${CMAKE_CURRENT_BINARY_DIR}/plogConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/plog"
    )
endif()
