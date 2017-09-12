#include "GTestRunner.h"
#include "Parser.h"
#include "ParserStateFactory.h"
#include "TestcaseCollection.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

//from http://stackoverflow.com/a/17976541
inline std::string trim(const std::string& s)
{
    auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c) { return std::isspace(c); });
    return std::string(wsfront, std::find_if_not(s.rbegin(), std::string::const_reverse_iterator(wsfront), [](int c) { return std::isspace(c); }).base());
}

// GTestRunner requires to implement this function
std::list<TestSteps> getTestCases()
{
    auto testcases = std::make_shared<TestcaseCollection>();
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

    return testcases->getSteps();
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
