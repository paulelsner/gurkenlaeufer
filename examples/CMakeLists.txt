add_executable(gurkenlaeufer_example
  addition.feature
  main.cpp
  TestSteps.cpp
)

target_link_libraries(gurkenlaeufer_example gurkenlaeufer gtest)

add_executable(gurkenlaeufer_example_cucumber_cpp
  addition.feature
  main.cpp
  TestSteps_cucumber_cpp.cpp
)

target_link_libraries(gurkenlaeufer_example_cucumber_cpp gurkenlaeufer gtest)
