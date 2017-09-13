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

class BaseStep {
public:
    virtual ~BaseStep() = default;
    virtual void runStep(const std::vector<Variant>&) = 0;

    void setContext(TestContext* context)
    {
        _context = context;
    }

    using StepRegistry = std::list<std::pair<std::string, BaseStep*>>;
    static StepRegistry& getStepRegistry()
    {
        static StepRegistry registry;
        return registry;
    }

protected:
    BaseStep(std::string RegEx, BaseStep* Step)
    {
        getStepRegistry().emplace_back(std::make_pair(RegEx, Step));
    }
    template <typename T>
    T* getFixture()
    {
        return _context->getFixture<T>();
    }

private:
    TestContext* _context;
};

enum class Hooktype {
    Before,
    After,
};

template <Hooktype hooktype>
class BaseHook {
public:
    virtual ~BaseHook() = default;
    virtual void runStep(const std::vector<Variant>& params) = 0;

    void setContext(TestContext* context)
    {
        _context = context;
    }

    using StepRegistry = std::list<std::pair<std::string, BaseHook<hooktype>*>>;
    static StepRegistry& getStepRegistry()
    {
        static StepRegistry registry;
        return registry;
    }

protected:
    BaseHook(std::string RegEx, BaseHook<hooktype>* Step)
    {
        getStepRegistry().emplace_back(std::make_pair(RegEx, Step));
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

#define BEFORE_HOOK_INTERNAL(Name, Instance, RegEx)                \
    struct Name : public BaseHook<Hooktype::Before> {              \
        Name()                                                     \
            : BaseHook<Hooktype::Before>(RegEx, this)              \
        {                                                          \
        }                                                          \
        void runStep(const std::vector<Variant>& params) override; \
    } Instance;                                                    \
    void Name::runStep(const std::vector<Variant>& params)

#define AFTER_HOOK_INTERNAL(Name, Instance, RegEx)                 \
    struct Name : public BaseHook<Hooktype::After> {               \
        Name()                                                     \
            : BaseHook<Hooktype::After>(RegEx, this)               \
        {                                                          \
        }                                                          \
        void runStep(const std::vector<Variant>& params) override; \
    } Instance;                                                    \
    void Name::runStep(const std::vector<Variant>& params)

#define STR2(x) #x
#define STR(x) x
#define JOIN(X, Y) DO_JOIN(X, Y)
#define DO_JOIN(X, Y) DO_JOIN2(X, Y)
#define DO_JOIN2(X, Y) X##Y
#define STEP(RegEx) \
    STEP_INTERNAL(JOIN(JOIN(Step, STR(__COUNTER__)), STR(__LINE__)), JOIN(JOIN(Instance, STR(__COUNTER__)), STR(__LINE__)), RegEx)
#define BEFORE(RegEx) \
    BEFORE_HOOK_INTERNAL(JOIN(JOIN(BeforeHook, STR(__COUNTER__)), STR(__LINE__)), JOIN(JOIN(Instance, STR(__COUNTER__)), STR(__LINE__)), RegEx)
#define AFTER(RegEx) \
    AFTER_HOOK_INTERNAL(JOIN(JOIN(AfterHook, STR(__COUNTER__)), STR(__LINE__)), JOIN(JOIN(Instance, STR(__COUNTER__)), STR(__LINE__)), RegEx)
