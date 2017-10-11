#pragma once

#include "ParserInterface.h"
#include "ParserStates.h"
#include "ScenarioInterface.h"

namespace gurkenlaeufer {

class ParserStateFactory final : public IParserStateFactory {
public:
    ParserStateFactory(const IScenarioCollectionSPtr& scenarioCollection)
        : _scenarioCollection(scenarioCollection)
    {
    }

    IParserStatePtr createInitialState(Scenario::StepList backgroudSteps) override
    {
        return IParserStatePtr(new InitialState(*this, std::move(backgroudSteps)));
    }
    IParserStatePtr createExamplesState(Scenario scenario, Scenario::StepList backgroudSteps) override
    {
        return IParserStatePtr(new ExamplesState(*this, std::move(scenario), std::move(backgroudSteps), _scenarioCollection));
    }
    IParserStatePtr createScenarioState(const std::string& description, Scenario::StepList backgroudSteps, Scenario::StepList tags) override
    {
        return IParserStatePtr(new ScenarioState(*this, description, std::move(backgroudSteps), std::move(tags), _scenarioCollection));
    }
    IParserStatePtr createScenarioOutlineState(const std::string& description, Scenario::StepList backgroudSteps, Scenario::StepList tags) override
    {
        return IParserStatePtr(new ScenarioOutlineState(*this, description, std::move(backgroudSteps), std::move(tags)));
    }

    IParserStatePtr createBackgroundState() override
    {
        return IParserStatePtr(new BackgroundState(*this));
    }

private:
    IScenarioCollectionSPtr _scenarioCollection;
};
}
