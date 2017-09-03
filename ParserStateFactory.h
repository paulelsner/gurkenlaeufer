#pragma once

#include "ParserInterface.h"
#include "ParserStates.h"
#include "TestcaseInterface.h"

class ParserStateFactory : public IParserStateFactory {
public:
    ParserStateFactory(const ITestcaseCollectionSPtr& testcases)
        : _testcases(testcases)
    {
    }

    ParserStatePtr createInitialState(TestSteps backgroudSteps) override
    {
        return ParserStatePtr(new InitialState(*this, std::move(backgroudSteps)));
    }
    ParserStatePtr createExamplesState(TestSteps steps, TestSteps backgroudSteps) override
    {
        return ParserStatePtr(new ExamplesState(*this, std::move(steps), std::move(backgroudSteps), _testcases));
    }
    ParserStatePtr createScenarioState(TestSteps backgroudSteps) override
    {
        return ParserStatePtr(new ScenarioState(*this, std::move(backgroudSteps), _testcases));
    }
    ParserStatePtr createScenarioOutlineState(TestSteps backgroudSteps) override
    {
        return ParserStatePtr(new ScenarioOutlineState(*this, std::move(backgroudSteps)));
    }

    ParserStatePtr createBackgroundState() override
    {
        return ParserStatePtr(new BackgroundState(*this));
    }

private:
    ITestcaseCollectionSPtr _testcases;
};