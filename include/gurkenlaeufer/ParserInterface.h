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

    virtual IParserStatePtr createInitialState(std::list<Step> backgroudSteps) = 0;
    virtual IParserStatePtr createExamplesState(Scenario testCase, std::list<Step> backgroudSteps) = 0;
    virtual IParserStatePtr createScenarioState(bool scenariouOutline, const std::string& description, std::list<Step> backgroudSteps, std::list<Step> tags) = 0;
    virtual IParserStatePtr createBackgroundState() = 0;
};

using IParserStateFactoryPtr = std::unique_ptr<IParserStateFactory>;
}
