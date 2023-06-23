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

add_test(NAME gurkenlaeufer_example COMMAND gurkenlaeufer_example WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME gurkenlaeufer_example_cucumber_cpp COMMAND gurkenlaeufer_example_cucumber_cpp WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})