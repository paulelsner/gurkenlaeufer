#pragma once

#include <memory>

namespace gurkenlaeufer {

struct IParserState {
    virtual ~IParserState() = default;

    virtual std::unique_ptr<IParserState> parseLine(const std::string& trimmedLine) = 0;
};

using IParserStatePtr = std::unique_ptr<IParserState>;

struct IParserStateFactory {
    virtual ~IParserStateFactory() = default;

    virtual IParserStatePtr createInitialState(Scenario::StepList backgroudSteps) = 0;
    virtual IParserStatePtr createExamplesState(Scenario testCase, Scenario::StepList backgroudSteps) = 0;
    virtual IParserStatePtr createScenarioState(const std::string& description, Scenario::StepList backgroudSteps, Scenario::StepList tags) = 0;
    virtual IParserStatePtr createScenarioOutlineState(const std::string& description, Scenario::StepList backgroudSteps, Scenario::StepList tags) = 0;
    virtual IParserStatePtr createBackgroundState() = 0;
};

using IParserStateFactoryPtr = std::unique_ptr<IParserStateFactory>;
}
