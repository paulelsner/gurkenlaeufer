add_library(gtest 
  src/gmock-gtest-all.cpp
)

target_include_directories(gtest SYSTEM PUBLIC include)
