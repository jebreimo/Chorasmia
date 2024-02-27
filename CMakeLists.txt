# ===========================================================================
# Copyright © 2020 Jan Erik Breimo. All rights reserved.
# Created by Jan Erik Breimo on 2020-12-27.
#
# This file is distributed under the BSD License.
# License text is included with the source distribution.
# ===========================================================================
cmake_minimum_required(VERSION 3.16)
project(Chorasmia VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 20)

# Set CHORASMIA_MASTER_PROJECT to ON if Chorasmia is the root level project.
if (NOT DEFINED CHORASMIA_MASTER_PROJECT)
    if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(CHORASMIA_MASTER_PROJECT ON)
    else ()
        set(CHORASMIA_MASTER_PROJECT OFF)
    endif ()
endif ()

# Test option
option(CHORASMIA_BUILD_TEST "Build tests" ${CHORASMIA_MASTER_PROJECT})

# Install option
option(CHORASMIA_INSTALL "Generate the install target" ${CHORASMIA_MASTER_PROJECT})

include(GNUInstallDirs)

add_library(Chorasmia INTERFACE)

target_include_directories(Chorasmia
    INTERFACE
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    )

add_library(Chorasmia::Chorasmia ALIAS Chorasmia)

export(TARGETS Chorasmia
    NAMESPACE Chorasmia::
    FILE ChorasmiaConfig.cmake)

if (CHORASMIA_BUILD_TEST)
    enable_testing()
    add_subdirectory(tests/ChorasmiaTest)
endif ()

if (CHORASMIA_INSTALL)
    install(TARGETS Chorasmia
        EXPORT ChorasmiaConfig
        )

    install(EXPORT ChorasmiaConfig
        FILE
            ChorasmiaConfig.cmake
        NAMESPACE
            Chorasmia::
        DESTINATION
            ${CMAKE_INSTALL_LIBDIR}/cmake/Chorasmia
        )

    install(
        DIRECTORY
            include/Chorasmia
        DESTINATION
            ${CMAKE_INSTALL_INCLUDEDIR}
    )

    include(CMakePackageConfigHelpers)

    write_basic_package_version_file(ChorasmiaConfigVersion.cmake
        COMPATIBILITY SameMajorVersion
        )

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ChorasmiaConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Chorasmia
        )

endif ()
