#pragma once

#include "Step.h"
#include "TestcaseInterface.h"

#include <regex>

class TestcaseRunner {
public:
    TestcaseRunner(const ITestcaseCollectionSPtr& testcases)
        : _testcases(testcases)
    {
    }
    void run()
    {
        const auto& testcases = _testcases->getSteps();
        for (const auto& testcase : testcases) {
            TestContext currentContext;
            for (const auto& step : testcase) {
                bool foundStep = false;

                for (const auto& entry : BaseStep::StepRegistry) {
                    std::smatch match;
                    if (std::regex_match(step, match, std::regex(entry.first))) {
                        foundStep = true;
                        std::vector<Variant> paramList;
                        for (size_t i = 1; i < match.size(); ++i) {
                            paramList.emplace_back(match[i]);
                        }
                        entry.second->setContext(&currentContext);
                        entry.second->runStep(paramList);
                    }
                }

                if (!foundStep) {
                    throw std::runtime_error("Did not find matching step for line: " + step);
                }
            }
        }
    }

private:
    ITestcaseCollectionSPtr _testcases;
};
