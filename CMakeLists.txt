cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project("gurkenlaeufer")

include(CMakeToolsHelpers OPTIONAL)

set(CMAKE_CXX_STANDARD 11)

add_library(gurkenlaeufer INTERFACE)

target_sources(
    gurkenlaeufer
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gurkenlaeufer/GTestRunner.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gurkenlaeufer/Parser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gurkenlaeufer/ParserInterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gurkenlaeufer/ParserStateFactory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gurkenlaeufer/ParserStates.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gurkenlaeufer/Step.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gurkenlaeufer/TestcaseCollection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gurkenlaeufer/TestcaseInterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gurkenlaeufer/Variant.h)

target_include_directories(gurkenlaeufer INTERFACE include/)

add_subdirectory(gtest)
add_subdirectory(examples)
