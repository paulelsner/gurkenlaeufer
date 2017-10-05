#include "gurkenlaeufer/Step.h"
#include "gtest/gtest.h"
#include <iostream>
#include <vector>

struct Calculator {
    std::vector<int> values;
};

STEP(".*have entered (\\d+) into the calculator$")
{
    auto calc = getFixture<Calculator>();
    calc->values.push_back(params[0].getInt());
}

STEP(".*press (\\w+)")
{
    std::cout << params[0].getString() << std::endl;
}

STEP(".*nostep")
{
}

STEP(".*the result should be (\\d+) on the screen$")
{
    auto calc = getFixture<Calculator>();
    int sum = 0;
    for (auto& n : calc->values)
        sum += n;
    std::cout << "Sum=" << sum << " " << params[0].getInt() << std::endl;
    EXPECT_EQ(params[0].getInt(), sum);
}

BEFORE("Print")
{
    std::cout << " ctrl + p" << std::endl;
}

BEFORE("Echo")
{
    std::cout << " echo ho ho ho" << std::endl;
}
