#include "gurkenlaeufer/Step.h"
#include "gtest/gtest.h"
#include <iostream>
#include <vector>

struct Calculator {
    std::vector<int> values;
};

GIVEN(".*have entered (\\d+) into the calculator$")
{
    auto calc = getFixture<Calculator>();
    REGEX_PARAM(int, integer);
    calc->values.push_back(integer);
}

WHEN(".*press (\\w+)")
{
    REGEX_PARAM(std::string, string);
    std::cout << string << std::endl;
}

STEP(".*nostep")
{
}

THEN(".*the result should be (\\d+) on the screen$")
{
    auto calc = getFixture<Calculator>();
    int sum = 0;
    for (auto& n : calc->values)
        sum += n;
    REGEX_PARAM(int, result);
    std::cout << "Sum=" << sum << " " << result << std::endl;
    EXPECT_EQ(result, sum);
}

BEFORE("Print")
{
    std::cout << "ctrl + p" << std::endl;
}

AFTER("Echo")
{
    std::cout << "echo ho ho ho" << std::endl;
}
