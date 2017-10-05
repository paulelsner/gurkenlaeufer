#pragma once

#include <list>
#include <memory>
#include <string>

struct TestSteps {
    using StepList = std::list<std::string>;
    StepList tags;
    StepList mainSteps;
};

struct ITestcaseCollection {
    virtual void appendTest(TestSteps steps) = 0;
    virtual std::list<TestSteps> getSteps() const = 0;
};

using ITestcaseCollectionSPtr = std::shared_ptr<ITestcaseCollection>;