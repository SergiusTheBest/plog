cmake_minimum_required(VERSION 3.0)

project(plog VERSION 1.1.8 LANGUAGES CXX)

# check if building as a stand-alone project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(IS_TOPLEVEL_PROJECT TRUE)
else()
    set(IS_TOPLEVEL_PROJECT FALSE)
endif()

# options
option(PLOG_BUILD_SAMPLES "Build ${PROJECT_NAME} samples" ${IS_TOPLEVEL_PROJECT})
option(PLOG_INSTALL "Generate ${PROJECT_NAME} install target" ${IS_TOPLEVEL_PROJECT})

# make sure install paths work on all platforms
include(GNUInstallDirs)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    INTERFACE
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

if(ANDROID)
    target_link_libraries(${PROJECT_NAME} INTERFACE log)
endif()

if(PLOG_BUILD_SAMPLES)
    # add a pseudo-project to make plog headers visible in IDE
    file(GLOB_RECURSE ${PROJECT_NAME}_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
    add_custom_target(${PROJECT_NAME}-headers SOURCES ${${PROJECT_NAME}_HEADERS})
    set_target_properties(${PROJECT_NAME}-headers PROPERTIES FOLDER Include)

    # add samples
    add_subdirectory(samples)
endif()

if(PLOG_INSTALL)
    install(
        TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Config
    )

    install(
        EXPORT ${PROJECT_NAME}Config
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        NAMESPACE ${PROJECT_NAME}::
    )

    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(
        FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/README.md
            ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
    )

    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        COMPATIBILITY AnyNewerVersion
    )

    install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )
endif()
