#pragma once

#include "ParserCommon.h"
#include "ScenarioInterface.h"

namespace gurkenlaeufer {

class ScenarioCollection final : public IScenarioCollection {
public:
    void appendScenario(Scenario scenario)
    {
        if (scenario.tags.end() != std::find_if(scenario.tags.begin(), scenario.tags.end(), [](const Step& step) {
            if (detail::toLower(step.step) == "@ignore") {
                return true;
            }
            return false; })) {
            std::cout << "Ignore scenario " << scenario.description;
            return;
        }
        _scenarios.emplace_back(std::move(scenario));
    }
    std::list<Scenario> getScenarios() const
    {
        return _scenarios;
    }

private:
    std::list<Scenario> _scenarios;
};
}
