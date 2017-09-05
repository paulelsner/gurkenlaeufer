
add_executable(example
  main.cpp
  TestSteps.cpp
)

target_include_directories(example PRIVATE ../include)
target_link_libraries(example gtest)