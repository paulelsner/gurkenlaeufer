#include "gurkenlaeufer/Step.h"
#include "gtest/gtest.h"
#include <iostream>
#include <vector>

struct Calculator {
    std::vector<int> values;
};

GIVEN(".*have entered (\\d+) into the calculator$")
{
    // This step uses the gurkenlaeufer native API
    auto calc = getFixture<Calculator>();
    calc->values.push_back(stepCtx.getNextParam<int>());
}

WHEN(".*press (\\w+)")
{
    std::cout << stepCtx.getNextParam<std::string>() << std::endl;
}

STEP(".*nostep")
{
}

THEN(".*the result should be (\\d+) on the screen$")
{
    // This step uses the cucumber-cpp adaption layer
    gurkenlaeufer::ScenarioScope<Calculator> calc(this);
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
