
// include this file one time. In main.cpp for example

#include "Step.h"
#include "TestcaseInterface.h"
#include "gtest/gtest.h"
#include <regex>

class CucumberTest : public testing::TestWithParam<TestSteps> {
};

TEST_P(CucumberTest, runSteps)
{
    const auto& testcase = GetParam();
    TestContext currentContext;
    for (const auto& step : testcase) {
        bool foundStep = false;

        for (const auto& entry : BaseStep::getStepRegistry()) {
            std::smatch match;
            if (std::regex_match(step, match, std::regex(entry.first))) {
                foundStep = true;
                std::vector<Variant> paramList;
                for (size_t i = 1; i < match.size(); ++i) {
                    paramList.emplace_back(match[i]);
                }

                std::cout << step << std::endl;
                entry.second->setContext(&currentContext);
                entry.second->runStep(paramList);
            }
        }

        if (!foundStep) {
            throw std::runtime_error("Did not find matching step for line: " + step);
        }
    }
}
