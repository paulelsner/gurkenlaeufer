#pragma once

#include "ParserCommon.h"
#include "ParserInterface.h"
#include "ScenarioInterface.h"

namespace gurkenlaeufer {

inline bool isStepKeyword(std::string word)
{
    return word == "given" || word == "when" || word == "then" || word == "and" || word == "but";
}

class CommonParserState : public IParserState {
protected:
    CommonParserState(IParserStateFactory& factory)
        : _factory(factory)
    {
    }

    std::unique_ptr<IParserState> forwardToNextState(const std::string& trimmedLine, std::list<Step> backgroundSteps) const
    {
        auto initState = _factory.createInitialState(std::move(backgroundSteps));
        auto nextState = initState->parseLine(trimmedLine);
        if (nextState != nullptr) {
            return nextState;
        }
        return initState;
    }

    IParserStateFactory& _factory;
};

class ExamplesState final : public CommonParserState {
public:
    ExamplesState(IParserStateFactory& factory, Scenario scenario, std::list<Step> backgroundSteps, const IScenarioCollectionSPtr& scenarioCollection)
        : CommonParserState(factory)
        , _scenario(std::move(scenario))
        , _backgroundSteps(std::move(backgroundSteps))
        , _scenarioCollection(scenarioCollection)
        , _isTableHead(true)
        , _exampleCount(0)
    {
    }

    std::unique_ptr<IParserState> parseLine(const std::string& trimmedLine) override
    {
        if (trimmedLine[0] != '|') {
            return forwardToNextState(trimmedLine, std::move(_backgroundSteps));
        }

        auto splittedLine = detail::split(trimmedLine, '|');
        for (auto& entry : splittedLine) {
            entry = detail::trim(entry);
        }

        if (_isTableHead) {
            _isTableHead = false;
            _TableHead = splittedLine;
        } else {
            auto entries = splittedLine;
            _generateSteps(entries);
        }

        return nullptr;
    }

private:
    void _generateSteps(const std::vector<std::string>& params)
    {
        Scenario scenarioCopy = _scenario;
        std::stringstream scenarioDesc;
        scenarioDesc << scenarioCopy.description << " example " << _exampleCount;
        _exampleCount++;
        scenarioCopy.description = scenarioDesc.str();

        for (auto& step : scenarioCopy.mainSteps) {
            step.step = _replaceParamsInStep(step.step, params);
        }
        _scenarioCollection->appendScenario(std::move(scenarioCopy));
    }

    std::string _replaceParamsInStep(const std::string& step, const std::vector<std::string>& params) const
    {
        auto isLessThanSign = [](int c) { return c == '<'; };
        auto isGreaterThanSign = [](int c) { return c == '>'; };

        auto outStep = step;
        for (;;) {
            const auto start = std::find_if(outStep.begin(), outStep.end(), isLessThanSign);
            if (start == outStep.end()) {
                return outStep;
            }
            const auto end = std::find_if(start, outStep.end(), isGreaterThanSign);
            if (end == outStep.end()) {
                throw std::runtime_error("found '<' but no matching '>'");
            }
            const auto variableName = std::string(start + 1, end);

            const auto indexOfVariable = std::find(_TableHead.begin(), _TableHead.end(), variableName) - _TableHead.begin();

            outStep = std::string(outStep.begin(), start) + params[indexOfVariable] + std::string(end + 1, outStep.end());
        }
    }

    Scenario _scenario;
    std::list<Step> _backgroundSteps;
    IScenarioCollectionSPtr _scenarioCollection;
    std::vector<std::string> _TableHead;
    bool _isTableHead;
    int _exampleCount;
};

class ScenarioState final : public CommonParserState {
public:
    ScenarioState(IParserStateFactory& factory, bool scenarioOutline, const std::string& description, std::list<Step> backgroundSteps, std::list<Step> tags, const IScenarioCollectionSPtr& testcases)
        : CommonParserState(factory)
        , _scenarioOutline(scenarioOutline)
        , _backgroundSteps(std::move(backgroundSteps))
        , _scenarioCollection(testcases)
        , _withinDocString(false)
    {
        _scenario.description = description;
        _scenario.mainSteps = _backgroundSteps;
        _scenario.tags = tags;
    }

    std::unique_ptr<IParserState> parseLine(const std::string& trimmedLine) override
    {
        auto firstWordEnd = std::find_if(trimmedLine.begin(), trimmedLine.end(), ::isspace);

        auto firstWord = detail::toLower(std::string(trimmedLine.begin(), firstWordEnd));
        if (firstWord == "\"\"\"") {
            if (_scenario.mainSteps.empty()) {
                throw std::runtime_error("Doc string found without prior step!");
            }
            _withinDocString = !_withinDocString;
        } else if (_withinDocString) {
            auto& docString = _scenario.mainSteps.back().docString;
            if (!docString.empty()) {
                docString += "\n";
            }
            docString += trimmedLine;
        } else {
            if (isStepKeyword(firstWord)) {
                Step step;
                step.step = detail::trim(std::string(firstWordEnd, trimmedLine.end()));
                _scenario.mainSteps.emplace_back(step);
            } else {
                if (_scenarioOutline) {
                    if (firstWord == "examples:") {
                        return _factory.createExamplesState(std::move(_scenario), std::move(_backgroundSteps));
                    } else {
                        throw std::runtime_error("Unable to parse line beginning with '" + firstWord + "'.");
                    }
                } else {
                    _scenarioCollection->appendScenario(std::move(_scenario));
                    return forwardToNextState(trimmedLine, std::move(_backgroundSteps));
                }
            }
        }

        return nullptr;
    }

private:
    bool _scenarioOutline;
    std::list<Step> _backgroundSteps;
    IScenarioCollectionSPtr _scenarioCollection;
    Scenario _scenario;
    bool _withinDocString;
};

class BackgroundState final : public CommonParserState {
public:
    BackgroundState(IParserStateFactory& factory)
        : CommonParserState(factory)
    {
    }

    std::unique_ptr<IParserState> parseLine(const std::string& trimmedLine) override
    {
        auto firstWordEnd = std::find_if(trimmedLine.begin(), trimmedLine.end(), ::isspace);

        auto firstWord = detail::toLower(std::string(trimmedLine.begin(), firstWordEnd));
        if (isStepKeyword(firstWord)) {
            Step step;
            step.step = detail::trim(std::string(firstWordEnd, trimmedLine.end()));
            _backgroundSteps.emplace_back(step);
        } else {
            return forwardToNextState(trimmedLine, std::move(_backgroundSteps));
        }
        return nullptr;
    }

private:
    std::list<Step> _backgroundSteps;
};

class InitialState final : public IParserState {
public:
    InitialState(IParserStateFactory& factory, std::list<Step> backgroundSteps)
        : _factory(factory)
        , _backgroundSteps(std::move(backgroundSteps))
    {
    }

    std::unique_ptr<IParserState> parseLine(const std::string& trimmedLine) override
    {
        auto firstWordEnd = std::find_if(trimmedLine.begin(), trimmedLine.end(), ::isspace);

        if (trimmedLine[0] == '@') {
            auto splittedLine = detail::split(std::string(++trimmedLine.begin(), trimmedLine.end()), '@');
            for (const auto& tag : splittedLine) {
                Step step;
                step.step = "@" + detail::trim(tag);
                _tags.push_back(step);
            }
            return nullptr;
        }
        const std::string firstWord(trimmedLine.begin(), firstWordEnd);
        if (firstWord == "Scenario") {
            auto secondWordBegin = firstWordEnd + 1;
            auto secondWordEnd = std::find_if(secondWordBegin, trimmedLine.end(), ::isspace);
            const std::string secondWord(secondWordBegin, secondWordEnd);
            if (secondWord == "Outline:") {
                const std::string description(secondWordEnd + 1, trimmedLine.end());
                return _factory.createScenarioState(true, description, std::move(_backgroundSteps), std::move(_tags));
            }
        } else if (firstWord == "Scenario:") {
            const std::string description(firstWordEnd + 1, trimmedLine.end());
            return _factory.createScenarioState(false, description, std::move(_backgroundSteps), std::move(_tags));
        } else if (firstWord == "Background:") {
            return _factory.createBackgroundState();
        }
        return nullptr;
    }

private:
    IParserStateFactory& _factory;
    std::list<Step> _backgroundSteps;
    std::list<Step> _tags;
};
}
