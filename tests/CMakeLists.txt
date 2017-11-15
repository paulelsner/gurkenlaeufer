add_executable(unit_tests
  main.cpp
  parsertests.cpp
)

add_test(unit_tests unit_tests)
target_link_libraries(unit_tests gurkenlaeufer gtest)
