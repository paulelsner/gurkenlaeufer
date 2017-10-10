#pragma once

#include "ScenarioInterface.h"
#include "StepInterface.h"
#include <regex>

namespace gurkenlaeufer {
    namespace detail {
        template <typename T>
        void runStepList(ScenarioContext& scenarioContext, const Scenario::StepList& list, const T& registry, bool ignoreNotFound)
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
                        entry.second->setScenarioContext(&scenarioContext);
                        StepContext stepCtx(std::move(params));
                        entry.second->setStepContext(std::move(stepCtx));
                        entry.second->runStep();
                    }
                }

                if (!ignoreNotFound && !foundStep) {
                    throw std::runtime_error("Did not find matching step for line: " + step);
                }
            }
        }
    }

void runScenario(const Scenario& scenario)
{
    detail::ScenarioContext scenarioContext;
    detail::runStepList(scenarioContext, scenario.tags, detail::BaseHook<detail::Hooktype::Before>::getStepRegistry(), true);
    detail::runStepList(scenarioContext, scenario.mainSteps, detail::BaseStep::getStepRegistry(), false);
    detail::runStepList(scenarioContext, scenario.tags, detail::BaseHook<detail::Hooktype::After>::getStepRegistry(), true);
}
}
