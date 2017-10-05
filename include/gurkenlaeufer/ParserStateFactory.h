#pragma once

#include "ParserInterface.h"
#include "ParserStates.h"
#include "ScenarioInterface.h"

namespace gurkenlaeufer {

class ParserStateFactory : public IParserStateFactory {
public:
    ParserStateFactory(const IScenarioCollectionSPtr& testcases)
        : _testcases(testcases)
    {
    }

    ParserStatePtr createInitialState(Scenario::StepList backgroudSteps) override
    {
        return ParserStatePtr(new InitialState(*this, std::move(backgroudSteps)));
    }
    ParserStatePtr createExamplesState(Scenario scenario, Scenario::StepList backgroudSteps) override
    {
        return ParserStatePtr(new ExamplesState(*this, std::move(scenario), std::move(backgroudSteps), _testcases));
    }
    ParserStatePtr createScenarioState(Scenario::StepList backgroudSteps, Scenario::StepList tags) override
    {
        return ParserStatePtr(new ScenarioState(*this, std::move(backgroudSteps), std::move(tags), _testcases));
    }
    ParserStatePtr createScenarioOutlineState(Scenario::StepList backgroudSteps, Scenario::StepList tags) override
    {
        return ParserStatePtr(new ScenarioOutlineState(*this, std::move(backgroudSteps), std::move(tags)));
    }

    ParserStatePtr createBackgroundState() override
    {
        return ParserStatePtr(new BackgroundState(*this));
    }

private:
    IScenarioCollectionSPtr _testcases;
};
}
