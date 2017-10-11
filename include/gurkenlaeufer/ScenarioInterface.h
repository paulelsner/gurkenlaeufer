#pragma once

#include <list>
#include <memory>
#include <string>

namespace gurkenlaeufer {

struct Scenario {
    std::string description;
    using StepList = std::list<std::string>;
    StepList tags;
    StepList mainSteps;
};

struct IScenarioCollection {
    virtual ~IScenarioCollection() = default;

    virtual void appendScenario(Scenario steps) = 0;
    virtual std::list<Scenario> getScenarios() const = 0;
};

using IScenarioCollectionSPtr = std::shared_ptr<IScenarioCollection>;
}
