# Copyright (c) 2004-2020 Microchip Technology Inc. and its subsidiaries.
# SPDX-License-Identifier: MIT

project(meba)

cmake_minimum_required(VERSION 2.8.12)

include_directories(include)
include_directories(../mesa/include)
include_directories(../include)
include_directories(./src)
include_directories(./src/drivers)

option(BUILD_MEBA_ALL "Build all MEBA layers" OFF)
mark_as_advanced(BUILD_MEBA_ALL)

macro(MEBA_LIB)
    set(LIST_OF_MEBA_TARGETS ${ARGN})

    foreach(arg ${ARGN})
        option(BUILD_MEBA_${arg}        "Build the MEBA layer for ${arg}" OFF)
        option(BUILD_MEBA_${arg}_static "Build the STATIC MEBA layer for ${arg}" OFF)
        mark_as_advanced(BUILD_MEBA_${arg} BUILD_MEBA_${arg}_static)
        FILE(GLOB c_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/src/${arg}/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/*.c")

        add_library(meba_${arg} SHARED ${c_files})
        add_library(meba_${arg}_static STATIC ${c_files})
        set_target_properties(meba_${arg}_static PROPERTIES OUTPUT_NAME meba_${arg})

        if (${BUILD_ALL} OR ${BUILD_MEBA_ALL})
            if (${BUILD_SHARED_LIBS})
                set(BUILD_MEBA_${arg} ON CACHE BOOL "" FORCE)
            endif()
            if (${BUILD_STATIC_LIBS})
                set(BUILD_MEBA_${arg}_static ON CACHE BOOL "" FORCE)
            endif()
        endif()

        if (${BUILD_MEBA_${arg}})
            message(STATUS "Build MEBA layer for ${arg}")
            set_target_properties(meba_${arg}        PROPERTIES EXCLUDE_FROM_ALL FALSE)
        else()
            set_target_properties(meba_${arg}        PROPERTIES EXCLUDE_FROM_ALL TRUE)
        endif()

        if (${BUILD_MEBA_${arg}_static})
            message(STATUS "Build STATIC MEBA layer for ${arg}")
            set_target_properties(meba_${arg}_static PROPERTIES EXCLUDE_FROM_ALL FALSE)
        else()
            set_target_properties(meba_${arg}_static PROPERTIES EXCLUDE_FROM_ALL TRUE)
        endif()
    endforeach()
endmacro()

MEBA_LIB(caracal jr2 servalt ocelot ocelot_pcb121 sparx5)

file(GLOB_RECURSE MEBA_HDRS "include/**.h")
doxygen_target(TARGET meba NAME "MEBA" FILES ${MEBA_HDRS} INCLUDES -I${CMAKE_CURRENT_SOURCE_DIR}/include)

