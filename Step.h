#pragma once

#include "Variant.h"
#include <list>
#include <memory>
#include <string>
#include <vector>

struct IFixture {
    virtual ~IFixture() = default;
};

template <typename T>
struct Fixture : public IFixture, public T {
};

class TestContext {
    std::vector<std::unique_ptr<IFixture>> _fixtures;

public:
    template <typename T>
    T* getFixture()
    {
        for (auto& fixture : _fixtures) {
            T* ptr = dynamic_cast<T*>(fixture.get());
            if (ptr != nullptr) {
                return ptr;
            }
        }

        auto ptr = new Fixture<T>();
        _fixtures.emplace_back(std::unique_ptr<IFixture>(ptr));
        return ptr;
    }

    void reset()
    {
        _fixtures.clear();
    }
};
//extern TestContext CurrentTestContext;

/*template <typename T>
struct ScenarioScope
{
    ScenarioScope() : _fixture(CurrentTestContext.getFixture<T>())
    {
    }

    T *operator->()
    {
        return _fixture;
    }

  private:
    T *_fixture;
};
*/
class BaseStep {
public:
    virtual ~BaseStep() = default;
    virtual void runStep(const std::vector<Variant>&) = 0;

    static std::list<std::pair<std::string, BaseStep*>> StepRegistry;

    void setContext(TestContext* context)
    {
        _context = context;
    }

protected:
    BaseStep(std::string RegEx, BaseStep* Step)
    {
        StepRegistry.emplace_back(std::make_pair(RegEx, Step));
    }
    template <typename T>
    T* getFixture()
    {
        return _context->getFixture<T>();
    }

private:
    TestContext* _context;
};

#define STEP_INTERNAL(Name, Instance, RegEx)                \
    struct Name : public BaseStep {                         \
        Name()                                              \
            : BaseStep(RegEx, this)                         \
        {                                                   \
        }                                                   \
        void runStep(const std::vector<Variant>&) override; \
    } Instance;                                             \
    void Name::runStep(const std::vector<Variant>& params)

#define STR2(x) #x
#define STR(x) x
#define JOIN(X, Y) DO_JOIN(X, Y)
#define DO_JOIN(X, Y) DO_JOIN2(X, Y)
#define DO_JOIN2(X, Y) X##Y
#define STEP(RegEx) \
    STEP_INTERNAL(JOIN(JOIN(Step, STR(__COUNTER__)), STR(__LINE__)), JOIN(JOIN(Instance, STR(__COUNTER__)), STR(__LINE__)), RegEx)
