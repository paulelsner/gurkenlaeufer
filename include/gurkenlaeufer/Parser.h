#pragma once

#include "ParserInterface.h"
#include "ParserCommon.h"

#include <fstream>
#include <string>

namespace gurkenlaeufer {
class Parser {
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
            auto trimmedLine = detail::trim(line);

            if (trimmedLine.empty() || trimmedLine[0] == '#') {
                continue;
            }

            parseLine(trimmedLine);
        }
    }

    void parseLine(const std::string& trimmedLine)
    {
        try {
            auto nextState = _currentState->parseLine(trimmedLine);
            if (nextState != nullptr) {
                _currentState = std::move(nextState);
            }
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

private:
    IParserStateFactoryPtr _factory;
    ParserStatePtr _currentState;
};
}
