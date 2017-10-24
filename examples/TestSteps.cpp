#define GURKE_STEP_NAME_PREFIX TestSteps

#include "gurkenlaeufer/Step.h"
#include "gtest/gtest.h"
#include <iostream>
#include <vector>

struct Calculator {
    std::vector<int> values;
};

STEP(".*have entered (\\d+) into the calculator$")
{
    // This step uses the gurkenlaeufer native API
    auto calc = getFixture<Calculator>();
    calc->values.push_back(getNextParam<int>());
}

STEP(".*press (\\w+)")
{
    std::cout << getNextParam<std::string>() << std::endl;
}

STEP(".*nostep")
{
}

STEP(".*the result should be (\\d+) on the screen$")
{
    // This step uses the cucumber-cpp adaption layer
    auto calc = getFixture<Calculator>();
    int sum = 0;
    for (auto& n : calc->values)
        sum += n;
    auto result = getParam<int>(0u);
    std::cout << "Sum=" << sum << " " << result << std::endl;
    EXPECT_EQ(result, sum);
}

STEP(".*nobody should know")
{
    auto docString = getNextParam<std::string>();
    std::cout << docString << std::endl;
}

BEFORE("@Print")
{
    std::cout << "ctrl + p" << std::endl;
}

AFTER("@Echo")
{
    std::cout << "echo ho ho ho" << std::endl;
}
