
// include this file one time. In main.cpp for example

#include "Run.h"
#include "gtest/gtest.h"

namespace gurkenlaeufer {

// User has to implement this function to return the list of test steps
// which should be executed by the test runner.
extern std::list<Scenario> getScenarios();

namespace detail {
    class CucumberTest : public testing::TestWithParam<Scenario> {
    };

    TEST_P(CucumberTest, runStepsInScenario)
    {
        runScenario(GetParam());
    }

    INSTANTIATE_TEST_CASE_P(
        gurkenlaeufer,
        CucumberTest,
        testing::ValuesIn(gurkenlaeufer::getScenarios()));
}
}
