
// include this file one time. In main.cpp for example

#include "ScenarioInterface.h"
#include "Step.h"
#include "gtest/gtest.h"
#include <regex>

namespace gurkenlaeufer {

// User has to implement this function to return the list of test steps
// which should be executed by the test runner.
extern std::list<Scenario> getScenarios();

namespace detail {
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
                        std::vector<std::string> params;
                        for (size_t i = 1; i < match.size(); ++i) {
                            params.emplace_back(match[i]);
                        }

                        std::cout << step << std::endl;
                        entry.second->setScenarioContext(&_currentScenarioContext);
                        StepContext stepCtx(std::move(params));
                        entry.second->runStep(stepCtx);
                    }
                }

                if (!ignoreNotFound && !foundStep) {
                    throw std::runtime_error("Did not find matching step for line: " + step);
                }
            }
        }

    private:
        detail::ScenarioContext _currentScenarioContext;
    };

    TEST_P(CucumberTest, runStepsInScenario)
    {
        const auto& testcase = GetParam();
        runStepList(testcase.tags, BaseHook<Hooktype::Before>::getStepRegistry(), true);
        runStepList(testcase.mainSteps, BaseStep::getStepRegistry(), false);
        runStepList(testcase.tags, BaseHook<Hooktype::After>::getStepRegistry(), true);
    }

    INSTANTIATE_TEST_CASE_P(
        gurkenlaeufer,
        CucumberTest,
        testing::ValuesIn(gurkenlaeufer::getScenarios()));
}
}
