cmake_minimum_required(VERSION 2.8)

PROJECT(ctemplate)

# Enable the C++11 option for GNU compilers.
if(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions(-std=c++11)
endif()

FIND_PACKAGE( PythonInterp 2.7 REQUIRED )

IF(UNIX)
    ADD_DEFINITIONS(-DCTEMPLATE_DLL_DECL=)
    ADD_DEFINITIONS(-DCTEMPLATE_DLL_DECL_FOR_UNITTESTS=)

    FUNCTION(TEST_ADD TEST_NAME)
        FILE(GLOB_RECURSE TEST_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/${TEST_NAME}.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/template_test_util.cc)
        ADD_EXECUTABLE(${TEST_NAME} ${TEST_SOURCES})
        TARGET_LINK_LIBRARIES(${TEST_NAME} libctemplate pthread)
    ENDFUNCTION(TEST_ADD)

    FUNCTION(TEST_ADD_C TEST_NAME)
        FILE(GLOB_RECURSE TEST_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/${TEST_NAME}.c)
        ADD_EXECUTABLE(${TEST_NAME} ${TEST_SOURCES})
        TARGET_LINK_LIBRARIES(${TEST_NAME} libctemplate)
    ENDFUNCTION(TEST_ADD_C)


    SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/lin)

    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src)
    LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/lib/lin)

    FILE(GLOB_RECURSE CTEMPLATE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/base/arena.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/htmlparser/htmlparser.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/htmlparser/jsparser.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/htmlparser/statemachine.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/src/diff_tpl_auto_escape.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/make_tpl_varnames_h.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/per_expand_data.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/src/template.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/template_annotator.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/template_cache.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/template_dictionary.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/template_modifiers.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/src/template_namelist.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/template_pathops.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/template_string.cc
        )

    ADD_LIBRARY(libctemplate ${CTEMPLATE_SOURCES})

    TEST_ADD("compile_test")
    TEST_ADD("htmlparser_cpp_test")
    TEST_ADD("template_cache_test")
    TEST_ADD("template_dictionary_unittest")
    TEST_ADD("template_modifiers_unittest")
    TEST_ADD("template_regtest")
    TEST_ADD("template_setglobals_unittest")
    TEST_ADD("template_test_util_test")
    TEST_ADD("template_unittest")
ELSEIF(WIN32)
    ADD_DEFINITIONS(-DCTEMPLATE_DLL_DECL=)
    ADD_DEFINITIONS(-DCTEMPLATE_DLL_DECL_FOR_UNITTESTS=)

    FUNCTION(TEST_ADD TEST_NAME)
        FILE(GLOB_RECURSE TEST_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/${TEST_NAME}.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/template_test_util.cc)
        ADD_EXECUTABLE(${TEST_NAME} ${TEST_SOURCES})
        TARGET_LINK_LIBRARIES(${TEST_NAME} libctemplate)
    ENDFUNCTION(TEST_ADD)

    FUNCTION(TEST_ADD_C TEST_NAME)
        FILE(GLOB_RECURSE TEST_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/${TEST_NAME}.c)
        ADD_EXECUTABLE(${TEST_NAME} ${TEST_SOURCES})
        TARGET_LINK_LIBRARIES(${TEST_NAME} libctemplate)
    ENDFUNCTION(TEST_ADD_C)

    SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/win)

    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src/windows)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src)
    LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/lib/win)

    FILE(GLOB_RECURSE CTEMPLATE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/base/arena.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/htmlparser/htmlparser.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/htmlparser/jsparser.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/htmlparser/statemachine.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/src/diff_tpl_auto_escape.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/make_tpl_varnames_h.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/per_expand_data.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/src/template.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/template_annotator.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/template_cache.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/template_dictionary.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/template_modifiers.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/src/template_namelist.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/template_pathops.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/template_string.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/port.cc
        )

    ADD_LIBRARY(libctemplate ${CTEMPLATE_SOURCES})

    TEST_ADD("compile_test")
    TEST_ADD("htmlparser_cpp_test")
    TEST_ADD("template_cache_test")
    TEST_ADD("template_dictionary_unittest")
    TEST_ADD("template_modifiers_unittest")
    TEST_ADD("template_regtest")
    TEST_ADD("template_setglobals_unittest")
    TEST_ADD("template_test_util_test")
    TEST_ADD("template_unittest")
ENDIF(UNIX)

# Add custom commands for generation of headers via. a python script.
#add_custom_command(
#  TARGET libctemplate
#  PRE_BUILD
#  COMMAND ${PYTHON_EXECUTABLE} generate_fsm.py htmlparser_fsm.config > htmlparser_fsm.h
#  MAIN_DEPENDENCY
#    htmlparser_fsm.config
#  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/htmlparser
#  DEPENDS 
#    generate_fsm.py )
#add_custom_command(
#  TARGET libctemplate
#  PRE_BUILD
#  COMMAND ${PYTHON_EXECUTABLE} generate_fsm.py jsparser_fsm.config > jsparser_fsm.h
#  MAIN_DEPENDENCY
#    jsparser_fsm.config
#  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/htmlparser
#  DEPENDS 
#    generate_fsm.py )