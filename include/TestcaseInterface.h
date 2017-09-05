#pragma once

#include <list>
#include <memory>
#include <string>

using TestSteps = std::list<std::string>;
struct ITestcaseCollection {
    virtual void appendTest(TestSteps steps) = 0;
    virtual std::list<TestSteps> getSteps() const = 0;
};

using ITestcaseCollectionSPtr = std::shared_ptr<ITestcaseCollection>;