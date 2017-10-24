#pragma once

#include <list>
#include <map>
#include <memory>
#include <string>

namespace gurkenlaeufer {

struct Step {
    std::string step;
    std::string docString;
};

struct Scenario {
    std::string description;
    std::list<Step> tags;
    std::list<Step> mainSteps;
};

struct IScenarioCollection {
    virtual ~IScenarioCollection() = default;

    virtual void appendScenario(Scenario steps) = 0;
    virtual std::list<Scenario> getScenarios() const = 0;
};

using IScenarioCollectionSPtr = std::shared_ptr<IScenarioCollection>;
}
