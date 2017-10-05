add_executable(example
  addition.feature
  main.cpp
  TestSteps.cpp
)

target_link_libraries(example gurkenlaeufer gtest)
