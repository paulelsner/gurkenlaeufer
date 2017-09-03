
#include "ParserInterface.h"

class Parser {
public:
    Parser(IParserStateFactoryPtr Factory)
        : _factory(std::move(Factory))
        , _currentState(_factory->createInitialState({}))
    {
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