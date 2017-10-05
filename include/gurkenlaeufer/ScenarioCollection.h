#pragma once

#include "ScenarioInterface.h"

namespace gurkenlaeufer {

class ScenarioCollection : public IScenarioCollection {
public:
    void appendScenario(Scenario scenario)
    {
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
