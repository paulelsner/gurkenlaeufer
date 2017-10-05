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

    ParserStatePtr createInitialState(TestSteps::StepList backgroudSteps) override
    {
        return ParserStatePtr(new InitialState(*this, std::move(backgroudSteps)));
    }
    ParserStatePtr createExamplesState(TestSteps steps, TestSteps::StepList backgroudSteps) override
    {
        return ParserStatePtr(new ExamplesState(*this, std::move(steps), std::move(backgroudSteps), _testcases));
    }
    ParserStatePtr createScenarioState(TestSteps::StepList backgroudSteps, TestSteps::StepList tags) override
    {
        return ParserStatePtr(new ScenarioState(*this, std::move(backgroudSteps), std::move(tags), _testcases));
    }
    ParserStatePtr createScenarioOutlineState(TestSteps::StepList backgroudSteps, TestSteps::StepList tags) override
    {
        return ParserStatePtr(new ScenarioOutlineState(*this, std::move(backgroudSteps), std::move(tags)));
    }

    ParserStatePtr createBackgroundState() override
    {
        return ParserStatePtr(new BackgroundState(*this));
    }

private:
    ITestcaseCollectionSPtr _testcases;
};