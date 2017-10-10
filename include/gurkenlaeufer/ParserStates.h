#pragma once

#include "ParserInterface.h"
#include "ParserCommon.h"
#include "ScenarioInterface.h"

namespace gurkenlaeufer {

inline bool isStepKeyword(std::string word)
{
    return word == "given" || word == "when" || word == "then" || word == "and";
}

class ExamplesState : public ParserState {
public:
    ExamplesState(IParserStateFactory& factory, Scenario scenario, Scenario::StepList backgroudSteps, const IScenarioCollectionSPtr& scenarioCollection)
        : _factory(factory)
        , _scenario(std::move(scenario))
        , _backgroudSteps(std::move(backgroudSteps))
        , _scenarioCollection(scenarioCollection)
        , _isTableHead(true)
    {
    }

    std::unique_ptr<ParserState> parseLine(const std::string& trimmedLine) override
    {
        if (trimmedLine[0] != '|') {
            // forward to next state
            return _factory.createInitialState(std::move(_backgroudSteps))->parseLine(trimmedLine);
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

    IParserStateFactory& _factory;
    Scenario _scenario;
    Scenario::StepList _backgroudSteps;
    IScenarioCollectionSPtr _scenarioCollection;
    std::vector<std::string> _TableHead;
    bool _isTableHead;
};

class ScenarioOutlineState : public ParserState {
public:
    ScenarioOutlineState(IParserStateFactory& factory, const std::string& description, Scenario::StepList backgroudSteps, Scenario::StepList tags)
        : _factory(factory)
        , _backgroudSteps(std::move(backgroudSteps))
    {
        _scenario.description = description;
        _scenario.mainSteps = _backgroudSteps;
        _scenario.tags = tags;
    }

    std::unique_ptr<ParserState> parseLine(const std::string& trimmedLine) override
    {
        auto firstWordEnd = std::find_if(trimmedLine.begin(), trimmedLine.end(), ::isspace);

        auto firstWord = detail::toLower(std::string(trimmedLine.begin(), firstWordEnd));
        if (isStepKeyword(firstWord)) {
            auto secondWordBegin = firstWordEnd + 1;
            _scenario.mainSteps.emplace_back(std::string(secondWordBegin, trimmedLine.end()));
        } else {
            return _factory.createExamplesState(std::move(_scenario), std::move(_backgroudSteps));
        }
        return nullptr;
    }

private:
    IParserStateFactory& _factory;
    Scenario::StepList _backgroudSteps;
    Scenario _scenario;
};

class ScenarioState : public ParserState {
public:
    ScenarioState(IParserStateFactory& factory, const std::string& description, Scenario::StepList backgroudSteps, Scenario::StepList tags, const IScenarioCollectionSPtr& testcases)
        : _factory(factory)
        , _backgroudSteps(std::move(backgroudSteps))
        , _scenarioCollection(testcases)
    {
        _scenario.description = description;
        _scenario.mainSteps = _backgroudSteps;
        _scenario.tags = tags;
    }

    std::unique_ptr<ParserState> parseLine(const std::string& trimmedLine) override
    {
        auto firstWordEnd = std::find_if(trimmedLine.begin(), trimmedLine.end(), ::isspace);

        auto firstWord = detail::toLower(std::string(trimmedLine.begin(), firstWordEnd));
        if (isStepKeyword(firstWord)) {
            auto secondWordBegin = firstWordEnd + 1;
            _scenario.mainSteps.emplace_back(std::string(secondWordBegin, trimmedLine.end()));
        } else {
            _scenarioCollection->appendScenario(std::move(_scenario));
            // forward to next state
            auto initState = _factory.createInitialState(std::move(_backgroudSteps));
            auto nextState = initState->parseLine(trimmedLine);
            if (nextState != nullptr) {
                return nextState;
            }
            return initState;
        }
        return nullptr;
    }

private:
    IParserStateFactory& _factory;
    Scenario::StepList _backgroudSteps;
    IScenarioCollectionSPtr _scenarioCollection;
    Scenario _scenario;
};

class BackgroundState : public ParserState {
public:
    BackgroundState(IParserStateFactory& factory)
        : _factory(factory)
    {
    }

    std::unique_ptr<ParserState> parseLine(const std::string& trimmedLine) override
    {
        auto firstWordEnd = std::find_if(trimmedLine.begin(), trimmedLine.end(), ::isspace);

        auto firstWord = detail::toLower(std::string(trimmedLine.begin(), firstWordEnd));
        if (isStepKeyword(firstWord)) {
            auto secondWordBegin = firstWordEnd + 1;
            _Steps.emplace_back(std::string(secondWordBegin, trimmedLine.end()));
        } else {
            // forward to next state
            auto initState = _factory.createInitialState(std::move(_Steps));
            auto nextState = initState->parseLine(trimmedLine);
            if (nextState != nullptr) {
                return nextState;
            }
            return initState;
        }
        return nullptr;
    }

private:
    IParserStateFactory& _factory;
    Scenario::StepList _Steps;
};

class InitialState : public ParserState {
public:
    InitialState(IParserStateFactory& factory, Scenario::StepList backgroudSteps)
        : _factory(factory)
        , _backgroundSteps(std::move(backgroudSteps))
    {
    }

    std::unique_ptr<ParserState> parseLine(const std::string& trimmedLine) override
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
