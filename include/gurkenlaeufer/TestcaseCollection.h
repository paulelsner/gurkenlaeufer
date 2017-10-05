#pragma once

#include "TestcaseInterface.h"

class TestcaseCollection : public ITestcaseCollection {
public:
    void appendTest(TestSteps steps)
    {
        _testCases.emplace_back(std::move(steps));
    }
    std::list<TestSteps> getSteps() const
    {
        return _testCases;
    }

private:
    std::list<TestSteps> _testCases;
};