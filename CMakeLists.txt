cmake_minimum_required(VERSION 2.8)

PROJECT(dmgen)

IF (WIN32)  
    MESSAGE(STATUS "Now is windows")

    SET(DMOS_NAME "win")

    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )    
ELSEIF (APPLE)  
    MESSAGE(STATUS "Now is Apple systems")  

    SET(DMOS_NAME "mac") 

    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread" )
ELSEIF (UNIX)  
    MESSAGE(STATUS "Now is UNIX-like OS")  
 
    SET(DMOS_NAME "lin") 
    
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread" )    
ENDIF ()

ADD_DEFINITIONS(-DCTEMPLATE_DLL_DECL=)

IF (WIN32)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/windows)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/ctemplate)

    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/test)

    LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/lib/${DMOS_NAME})

    SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/${DMOS_NAME})

    FILE(GLOB CTEMPLATE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/base/arena.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/htmlparser/htmlparser.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/htmlparser/jsparser.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/htmlparser/statemachine.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/diff_tpl_auto_escape.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/make_tpl_varnames_h.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/per_expand_data.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template_annotator.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template_cache.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template_dictionary.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template_modifiers.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template_namelist.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template_pathops.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template_string.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/windows/port.cc
        )

    ADD_LIBRARY(libctemplate ${CTEMPLATE_SOURCES})

    FILE(GLOB DMGEN_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/arg_parser.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/dmbin2array.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/dmgen.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

    ADD_EXECUTABLE(dmgen ${DMGEN_SOURCES})
    TARGET_LINK_LIBRARIES(dmgen libctemplate)
ELSEIF (APPLE)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/ctemplate)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src)

    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/test)

    LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/lib/${DMOS_NAME})

    SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/${DMOS_NAME})

    FILE(GLOB CTEMPLATE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/base/arena.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/htmlparser/htmlparser.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/htmlparser/jsparser.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/htmlparser/statemachine.cc

    ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/diff_tpl_auto_escape.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/make_tpl_varnames_h.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/per_expand_data.cc

    ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template_annotator.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template_cache.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template_dictionary.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template_modifiers.cc

    ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template_namelist.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template_pathops.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template_string.cc
    )

    ADD_LIBRARY(libctemplate ${CTEMPLATE_SOURCES})
    TARGET_LINK_LIBRARIES(libctemplate)

    FILE(GLOB DMGEN_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/arg_parser.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/dmbin2array.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/dmgen.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

    ADD_EXECUTABLE(dmgen ${DMGEN_SOURCES})
    TARGET_LINK_LIBRARIES(dmgen libctemplate)

ELSEIF (UNIX)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/ctemplate)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src)

    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/test)

    LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/lib/${DMOS_NAME})

    SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/${DMOS_NAME})

    FILE(GLOB CTEMPLATE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/base/arena.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/htmlparser/htmlparser.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/htmlparser/jsparser.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/htmlparser/statemachine.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/diff_tpl_auto_escape.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/make_tpl_varnames_h.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/per_expand_data.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template_annotator.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template_cache.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template_dictionary.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template_modifiers.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template_namelist.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template_pathops.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ctemplate/src/template_string.cc
        )

    ADD_LIBRARY(libctemplate ${CTEMPLATE_SOURCES})
    TARGET_LINK_LIBRARIES(libctemplate)

    FILE(GLOB DMGEN_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/arg_parser.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/dmbin2array.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/dmgen.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

    ADD_EXECUTABLE(dmgen ${DMGEN_SOURCES})
    TARGET_LINK_LIBRARIES(dmgen libctemplate)
ENDIF ()