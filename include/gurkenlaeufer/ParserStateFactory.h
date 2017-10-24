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

    IParserStatePtr createInitialState(std::list<Step> backgroudSteps) override
    {
        return IParserStatePtr(new InitialState(*this, std::move(backgroudSteps)));
    }
    IParserStatePtr createExamplesState(Scenario scenario, std::list<Step> backgroudSteps) override
    {
        return IParserStatePtr(new ExamplesState(*this, std::move(scenario), std::move(backgroudSteps), _scenarioCollection));
    }
    IParserStatePtr createScenarioState(bool scenariouOutline, const std::string& description, std::list<Step> backgroudSteps, std::list<Step> tags) override
    {
        return IParserStatePtr(new ScenarioState(*this, scenariouOutline, description, std::move(backgroudSteps), std::move(tags), _scenarioCollection));
    }

    IParserStatePtr createBackgroundState() override
    {
        return IParserStatePtr(new BackgroundState(*this));
    }

private:
    IScenarioCollectionSPtr _scenarioCollection;
};
}
