cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project("CucumberCxx")

include(CMakeToolsHelpers OPTIONAL)

set (CMAKE_CXX_STANDARD 11)

add_library(gtest 
  gtest/src/gmock-gtest-all.cpp
)

target_include_directories(gtest PUBLIC
  gtest/include)

add_executable(CucumberCxx
  main.cpp
  Step.cpp
  TestSteps.cpp
)

target_link_libraries(CucumberCxx gtest)
