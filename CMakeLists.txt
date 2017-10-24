cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project("gurkenlaeufer")

include(CMakeToolsHelpers OPTIONAL)

set(CMAKE_CXX_STANDARD 11)

if(NOT MSVC)
  find_package(Threads REQUIRED)
  link_libraries(pthread)
endif()

add_library(gurkenlaeufer INTERFACE)

target_sources(
    gurkenlaeufer
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gurkenlaeufer/GTestRunner.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gurkenlaeufer/Parser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gurkenlaeufer/ParserCommon.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gurkenlaeufer/ParserInterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gurkenlaeufer/ParserStateFactory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gurkenlaeufer/ParserStates.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gurkenlaeufer/Run.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gurkenlaeufer/ScenarioCollection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gurkenlaeufer/ScenarioInterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gurkenlaeufer/Step.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gurkenlaeufer/StepInterface.h)

target_include_directories(gurkenlaeufer INTERFACE include/)

add_subdirectory(gtest)
add_subdirectory(examples)
