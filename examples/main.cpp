#include "gurkenlaeufer/GTestRunner.h"
#include "gurkenlaeufer/Parser.h"
#include "gurkenlaeufer/ParserStateFactory.h"
#include "gurkenlaeufer/ScenarioCollection.h"
#include "gtest/gtest.h"
#include <memory>

// gurkenlaeufer GTestRunner requires to implement this function
std::list<gurkenlaeufer::Scenario> gurkenlaeufer::getScenarios()
{
    auto testCases = std::make_shared<gurkenlaeufer::ScenarioCollection>();

    Parser parser(IParserStateFactoryPtr(new gurkenlaeufer::ParserStateFactory(testCases)));
    parser.parseFile("addition.feature");

    return testCases->getScenarios();
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
