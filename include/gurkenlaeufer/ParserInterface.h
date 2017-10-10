#pragma once

#include <memory>

namespace gurkenlaeufer {

struct ParserState {
    virtual std::unique_ptr<ParserState> parseLine(const std::string& trimmedLine) = 0;
};

using ParserStatePtr = std::unique_ptr<ParserState>;

struct IParserStateFactory {
    virtual ParserStatePtr createInitialState(Scenario::StepList backgroudSteps) = 0;
    virtual ParserStatePtr createExamplesState(Scenario testCase, Scenario::StepList backgroudSteps) = 0;
    virtual ParserStatePtr createScenarioState(const std::string& description, Scenario::StepList backgroudSteps, Scenario::StepList tags) = 0;
    virtual ParserStatePtr createScenarioOutlineState(const std::string& description, Scenario::StepList backgroudSteps, Scenario::StepList tags) = 0;
    virtual ParserStatePtr createBackgroundState() = 0;
};

using IParserStateFactoryPtr = std::unique_ptr<IParserStateFactory>;
}
