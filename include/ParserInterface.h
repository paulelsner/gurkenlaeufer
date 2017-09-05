#pragma once

#include <memory>

struct ParserState {
    virtual std::unique_ptr<ParserState> parseLine(const std::string& trimmedLine) = 0;
};

using ParserStatePtr = std::unique_ptr<ParserState>;

struct IParserStateFactory {
    virtual ParserStatePtr createInitialState(TestSteps backgroudSteps) = 0;
    virtual ParserStatePtr createExamplesState(TestSteps steps, TestSteps backgroudSteps) = 0;
    virtual ParserStatePtr createScenarioState(TestSteps backgroudSteps) = 0;
    virtual ParserStatePtr createScenarioOutlineState(TestSteps backgroudSteps) = 0;
    virtual ParserStatePtr createBackgroundState() = 0;
};

using IParserStateFactoryPtr = std::unique_ptr<IParserStateFactory>;
