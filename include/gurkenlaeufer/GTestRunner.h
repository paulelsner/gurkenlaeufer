
// include this file one time. In main.cpp for example

#include "ScenarioInterface.h"
#include "Step.h"
#include "gtest/gtest.h"
#include <regex>

namespace gurkenlaeufer {

// User has to implement this function to return the list of test steps
// which should be executed by the test runner.
extern std::list<Scenario> getScenarios();

class CucumberTest : public testing::TestWithParam<Scenario> {
protected:
    template <typename T>
    void runStepList(const Scenario::StepList& list, const T& registry, bool ignoreNotFound)
    {
        for (const auto& step : list) {
            bool foundStep = false;

            for (const auto& entry : registry) {
                std::smatch match;
                if (std::regex_match(step, match, std::regex(entry.first))) {
                    foundStep = true;
                    std::vector<Variant> paramList;
                    for (size_t i = 1; i < match.size(); ++i) {
                        paramList.emplace_back(match[i]);
                    }

                    std::cout << step << std::endl;
                    entry.second->setContext(&_currentContext);
                    entry.second->runStep(paramList);
                }
            }

            if (!ignoreNotFound && !foundStep) {
                throw std::runtime_error("Did not find matching step for line: " + step);
            }
        }
    }

    TestContext _currentContext;
};

TEST_P(CucumberTest, gurkenlaeuferRunStepsInTestCase)
{
    const auto& testcase = GetParam();
    runStepList(testcase.tags, BaseHook<Hooktype::Before>::getStepRegistry(), true);
    runStepList(testcase.mainSteps, BaseStep::getStepRegistry(), false);
    runStepList(testcase.tags, BaseHook<Hooktype::After>::getStepRegistry(), true);
}

INSTANTIATE_TEST_CASE_P(
    GeneralAndSpecial,
    CucumberTest,
    testing::ValuesIn(gurkenlaeufer::getScenarios()));
}
