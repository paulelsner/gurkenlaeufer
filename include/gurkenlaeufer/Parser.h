#pragma once

#include "ParserInterface.h"

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
            auto trimmedLine = _trim(line);

            if (line.empty() || line[0] == '#') {
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
    //from http://stackoverflow.com/a/17976541
    std::string _trim(const std::string& s)
    {
        auto wsfront = std::find_if_not(s.begin(), s.end(), ::isspace);
        return std::string(wsfront, std::find_if_not(s.rbegin(), std::string::const_reverse_iterator(wsfront), ::isspace).base());
    }

private:
    IParserStateFactoryPtr _factory;
    ParserStatePtr _currentState;
};
}
