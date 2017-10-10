#pragma once

#include "ParserInterface.h"
#include "ParserStates.h"
#include "ScenarioInterface.h"

namespace gurkenlaeufer {

class ParserStateFactory : public IParserStateFactory {
public:
    ParserStateFactory(const IScenarioCollectionSPtr& scenarioCollection)
        : _scenarioCollection(scenarioCollection)
    {
    }

    ParserStatePtr createInitialState(Scenario::StepList backgroudSteps) override
    {
        return ParserStatePtr(new InitialState(*this, std::move(backgroudSteps)));
    }
    ParserStatePtr createExamplesState(Scenario scenario, Scenario::StepList backgroudSteps) override
    {
        return ParserStatePtr(new ExamplesState(*this, std::move(scenario), std::move(backgroudSteps), _scenarioCollection));
    }
    ParserStatePtr createScenarioState(const std::string& description, Scenario::StepList backgroudSteps, Scenario::StepList tags) override
    {
        return ParserStatePtr(new ScenarioState(*this, description, std::move(backgroudSteps), std::move(tags), _scenarioCollection));
    }
    ParserStatePtr createScenarioOutlineState(const std::string& description, Scenario::StepList backgroudSteps, Scenario::StepList tags) override
    {
        return ParserStatePtr(new ScenarioOutlineState(*this, description, std::move(backgroudSteps), std::move(tags)));
    }

    ParserStatePtr createBackgroundState() override
    {
        return ParserStatePtr(new BackgroundState(*this));
    }

private:
    IScenarioCollectionSPtr _scenarioCollection;
};
}
