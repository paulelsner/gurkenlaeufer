#pragma once

#include "ParserCommon.h"
#include "ParserInterface.h"

#include <fstream>
#include <string>

namespace gurkenlaeufer {
class Parser final {
public:
    Parser(IParserStateFactoryPtr Factory)
        : _factory(std::move(Factory))
        , _currentState(_factory->createInitialState({}))
    {
    }

    void parseFile(const std::string& filePath)
    {
        std::ifstream fin(filePath);
        if (fin.fail()) {
            throw std::invalid_argument("The given cucumber feature file '" + filePath + "' can not be opened!");
        }

        for (std::string line; std::getline(fin, line);) {
            std::cout << line << std::endl;

            // Trim off comments
            auto isCommentSign = [](int c) { return c == '#'; };
            const auto commentBegin = std::find_if(line.begin(), line.end(), isCommentSign);
            if (commentBegin != line.end()) {
                line = std::string(line.begin(), commentBegin);
            }

            // Trim off whitespace
            auto trimmedLine = detail::trim(line);

            if (trimmedLine.empty()) {
                continue;
            }

            parseLine(trimmedLine);
        }
    }

    void parseLine(const std::string& trimmedLine)
    {
        auto nextState = _currentState->parseLine(trimmedLine);
        if (nextState != nullptr) {
            _currentState = std::move(nextState);
        }
    }

    void finish()
    {
        _currentState->finish();
        _currentState.reset();
    }

private:
    IParserStateFactoryPtr _factory;
    IParserStatePtr _currentState;
};
}
