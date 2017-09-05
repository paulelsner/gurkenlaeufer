#include "GTestRunner.h"
#include "Parser.h"
#include "ParserStateFactory.h"
#include "TestcaseCollection.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <stdint.h>
#include <string>
#include <vector>

//from http://stackoverflow.com/a/17976541
inline std::string trim(const std::string& s)
{
    auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c) { return std::isspace(c); });
    return std::string(wsfront, std::find_if_not(s.rbegin(), std::string::const_reverse_iterator(wsfront), [](int c) { return std::isspace(c); }).base());
}
auto testcases = std::make_shared<TestcaseCollection>();

int main(int argc, char** argv)
{
    std::ifstream fin("examples/addition.feature");

    Parser parser(IParserStateFactoryPtr(new ParserStateFactory(testcases)));

    for (std::string line; getline(fin, line);) {
        std::cout << line << std::endl;
        auto trimmedLine = trim(line);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        parser.parseLine(trimmedLine);
    }

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

INSTANTIATE_TEST_CASE_P(
    GeneralAndSpecial,
    CucumberTest,
    testing::ValuesIn(testcases->getSteps()));