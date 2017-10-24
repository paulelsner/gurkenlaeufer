add_executable(gurkenlaeufer_example
  addition.feature
  main.cpp
  TestSteps.cpp
)

target_link_libraries(gurkenlaeufer_example gurkenlaeufer gtest)
