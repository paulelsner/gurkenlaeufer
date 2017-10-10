#pragma once

#include "ParserInterface.h"
#include "ParserCommon.h"
#include "ScenarioInterface.h"

namespace gurkenlaeufer {

inline bool isStepKeyword(std::string word)
{
    return word == "given" || word == "when" || word == "then" || word == "and";
}

class CommonParserState : public IParserState {
protected:
    CommonParserState(IParserStateFactory& factory)
        : _factory(factory)
    {
    }

    std::unique_ptr<IParserState> forwardToNextState(const std::string& trimmedLine, Scenario::StepList backgroundSteps) const
    {
        // forward to next state
        auto initState = _factory.createInitialState(std::move(backgroundSteps));
        auto nextState = initState->parseLine(trimmedLine);
        if (nextState != nullptr) {
            return nextState;
        }
        return initState;
    }

    IParserStateFactory& _factory;
};

class ExamplesState : public CommonParserState {
public:
    ExamplesState(IParserStateFactory& factory, Scenario scenario, Scenario::StepList backgroundSteps, const IScenarioCollectionSPtr& scenarioCollection)
        : CommonParserState(factory)
        , _scenario(std::move(scenario))
        , _backgroundSteps(std::move(backgroundSteps))
        , _scenarioCollection(scenarioCollection)
        , _isTableHead(true)
    {
    }

    std::unique_ptr<IParserState> parseLine(const std::string& trimmedLine) override
    {
        if (trimmedLine[0] != '|') {
            return forwardToNextState(trimmedLine, std::move(_backgroundSteps));
        }

        auto splittedLine = detail::split(trimmedLine, '|');
        for (auto & entry : splittedLine) {
            entry = detail::trim(entry);
        }

        if (_isTableHead) {
            _isTableHead = false;
            _TableHead = splittedLine;
        }
        else
        {
            auto entries = splittedLine;
            _generateSteps(entries);
        }

        return nullptr;
    }

private:
    void _generateSteps(const std::vector<std::string>& params)
    {
        Scenario scenarioCopy = _scenario;
        for (auto& step : scenarioCopy.mainSteps) {
            step = _replaceParamsInStep(step, params);
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
    Scenario::StepList _backgroundSteps;
    IScenarioCollectionSPtr _scenarioCollection;
    std::vector<std::string> _TableHead;
    bool _isTableHead;
};

class ScenarioOutlineState : public CommonParserState {
public:
    ScenarioOutlineState(IParserStateFactory& factory, const std::string& description, Scenario::StepList backgroundSteps, Scenario::StepList tags)
        : CommonParserState(factory)
        , _backgroundSteps(std::move(backgroundSteps))
    {
        _scenario.description = description;
        _scenario.mainSteps = _backgroundSteps;
        _scenario.tags = tags;
    }

    std::unique_ptr<IParserState> parseLine(const std::string& trimmedLine) override
    {
        auto firstWordEnd = std::find_if(trimmedLine.begin(), trimmedLine.end(), ::isspace);

        auto firstWord = detail::toLower(std::string(trimmedLine.begin(), firstWordEnd));
        if (isStepKeyword(firstWord)) {
            auto secondWordBegin = firstWordEnd + 1;
            _scenario.mainSteps.emplace_back(std::string(secondWordBegin, trimmedLine.end()));
        } else {
            return _factory.createExamplesState(std::move(_scenario), std::move(_backgroundSteps));
        }
        return nullptr;
    }

private:
    Scenario::StepList _backgroundSteps;
    Scenario _scenario;
};

class ScenarioState : public CommonParserState {
public:
    ScenarioState(IParserStateFactory& factory, const std::string& description, Scenario::StepList backgroundSteps, Scenario::StepList tags, const IScenarioCollectionSPtr& testcases)
        : CommonParserState(factory)
        , _backgroundSteps(std::move(backgroundSteps))
        , _scenarioCollection(testcases)
    {
        _scenario.description = description;
        _scenario.mainSteps = _backgroundSteps;
        _scenario.tags = tags;
    }

    std::unique_ptr<IParserState> parseLine(const std::string& trimmedLine) override
    {
        auto firstWordEnd = std::find_if(trimmedLine.begin(), trimmedLine.end(), ::isspace);

        auto firstWord = detail::toLower(std::string(trimmedLine.begin(), firstWordEnd));
        if (isStepKeyword(firstWord)) {
            auto secondWordBegin = firstWordEnd + 1;
            _scenario.mainSteps.emplace_back(std::string(secondWordBegin, trimmedLine.end()));
        } else {
            _scenarioCollection->appendScenario(std::move(_scenario));
            return forwardToNextState(trimmedLine, std::move(_backgroundSteps));
        }
        return nullptr;
    }

private:
    Scenario::StepList _backgroundSteps;
    IScenarioCollectionSPtr _scenarioCollection;
    Scenario _scenario;
};

class BackgroundState : public CommonParserState {
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
            auto secondWordBegin = firstWordEnd + 1;
            _backgroundSteps.emplace_back(std::string(secondWordBegin, trimmedLine.end()));
        } else {
            return forwardToNextState(trimmedLine, std::move(_backgroundSteps));
        }
        return nullptr;
    }

private:
    Scenario::StepList _backgroundSteps;
};

class InitialState : public IParserState {
public:
    InitialState(IParserStateFactory& factory, Scenario::StepList backgroundSteps)
        : _factory(factory)
        , _backgroundSteps(std::move(backgroundSteps))
    {
    }

    std::unique_ptr<IParserState> parseLine(const std::string& trimmedLine) override
    {
        auto firstWordEnd = std::find_if(trimmedLine.begin(), trimmedLine.end(), ::isspace);

        if (trimmedLine[0] == '@') {
            auto splittedLine = detail::split(std::string(++trimmedLine.begin(), trimmedLine.end()), '@');
            for (const auto & tag : splittedLine) {
                _hooks.push_back("@" + detail::trim(tag));
            }
            return nullptr;
        }
        const std::string firstWord(trimmedLine.begin(), firstWordEnd);
        if (firstWord == "Scenario") {
            auto secondWordBegin = firstWordEnd + 1;
            auto secondWordEnd = std::find_if(secondWordBegin, trimmedLine.end(), ::isspace);
            const std::string secondWord(secondWordBegin, secondWordEnd);
            if (secondWord == "Outline:") {
                return _factory.createScenarioOutlineState(trimmedLine, std::move(_backgroundSteps), std::move(_hooks));
            }
        } else if (firstWord == "Scenario:") {
            return _factory.createScenarioState(trimmedLine, std::move(_backgroundSteps), std::move(_hooks));
        } else if (firstWord == "Background:") {
            return _factory.createBackgroundState();
        }
        return nullptr;
    }

private:
    IParserStateFactory& _factory;
    Scenario::StepList _backgroundSteps;
    Scenario::StepList _hooks;
};
}
