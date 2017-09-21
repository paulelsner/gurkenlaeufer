cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project("gurkenlaeufer")

include(CMakeToolsHelpers OPTIONAL)

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)

if(NOT MSVC)
  find_package(Threads REQUIRED)
  link_libraries(pthread)
endif()

add_library(gtest 
  gtest/src/gmock-gtest-all.cpp
)

include_directories(SYSTEM
  gtest/include)

add_subdirectory(examples)
