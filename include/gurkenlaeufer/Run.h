#pragma once

#include "ScenarioInterface.h"
#include "StepInterface.h"
#include <iostream>
#include <regex>

namespace gurkenlaeufer {
namespace detail {
    template <typename T>
    struct MatchingSteps {
        const typename T::value_type* stepDefinition;
        std::vector<std::string> params;
    };

    template <typename T>
    void runStepList(ScenarioContext& scenarioContext, const std::list<Step>& list, const T& registry, bool ignoreNotFound)
    {
        for (const auto& step : list) {

            std::vector<MatchingSteps<T>> matchingSteps;

            for (const auto& entry : registry) {
                std::smatch match;
                if (std::regex_match(step.step, match, std::regex(entry.first))) {
                    MatchingSteps<T> mathingStep;
                    mathingStep.stepDefinition = &entry;
                    for (size_t i = 1; i < match.size(); ++i) {
                        mathingStep.params.emplace_back(match[i]);
                    }
                    mathingStep.params.emplace_back(step.docString);
                    matchingSteps.push_back(mathingStep);
                }
            }

            if (matchingSteps.size() > 1u) {
                std::string msg = "Ambiguous step definitions found for step: '" + step.step + "'. \nPossible matches:";
                for (const auto& matchingStep : matchingSteps) {
                    msg += "\n'" + matchingStep.stepDefinition->first + "'";
                }
                throw std::runtime_error(msg.c_str());
            } else if (matchingSteps.size() == 0u) {
                if (!ignoreNotFound) {
                    throw std::runtime_error("Did not find matching step for line: " + step.step);
                }
            } else {
                auto& matchingStep = matchingSteps[0u];
                std::cout << "[" << step.step << "]" << std::endl;
                matchingStep.stepDefinition->second->setScenarioContext(&scenarioContext);
                StepContext stepCtx(std::move(matchingStep.params));
                matchingStep.stepDefinition->second->setStepContext(std::move(stepCtx));
                matchingStep.stepDefinition->second->runStep();
            }
        }
    }
}

void runScenario(const Scenario& scenario)
{
    detail::ScenarioContext scenarioContext;
    std::cout << "[" << scenario.description << "]" << std::endl;
    detail::runStepList(scenarioContext, scenario.tags, detail::BaseHook<detail::Hooktype::Before>::getStepRegistry(), true);
    detail::runStepList(scenarioContext, scenario.mainSteps, detail::BaseStep::getStepRegistry(), false);
    detail::runStepList(scenarioContext, scenario.tags, detail::BaseHook<detail::Hooktype::After>::getStepRegistry(), true);
}
}
