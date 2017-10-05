#pragma once

#include <list>
#include <memory>
#include <string>
#include <vector>

namespace gurkenlaeufer {
namespace detail {

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

    template <typename T>
    T fromString(const std::string& s)
    {
        T t;
        std::istringstream iss(s);
        iss >> t;
        if (iss.fail()) {
            throw std::invalid_argument("Failed to convert parameter!");
        }
        return t;
    }

    template <>
    inline std::string fromString(const std::string& s)
    {
        return s;
    }

    class StepContext {
    public:
        StepContext(std::vector<std::string>&& params)
            : _params(std::move(params))
        {
        }

        template <typename T>
        T getParam(std::size_t i) const
        {
            if (i >= _params.size()) {
                throw std::invalid_argument("Parameter index out of range!");
            }
            return fromString<T>(_params.at(i));
        }

        template <typename T>
        const T getNextParam()
        {
            return getParam<T>(_currentParamIdx++);
        }

    private:
        const std::vector<std::string> _params;
        std::size_t _currentParamIdx = 0u;
    };

    template <typename TStep>
    class CommonStep {
    public:
        virtual ~CommonStep() = default;
        virtual void runStep(StepContext&) = 0;

        using StepRegistry = std::list<std::pair<std::string, TStep*>>;
        static StepRegistry& getStepRegistry()
        {
            static StepRegistry registry;
            return registry;
        }

        void setContext(TestContext* context)
        {
            _context = context;
        }

    protected:
        CommonStep(std::string RegEx, TStep* Step)
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

    class BaseStep : public CommonStep<BaseStep> {
    public:
        virtual ~BaseStep() = default;

    protected:
        BaseStep(std::string RegEx, BaseStep* Step)
            : CommonStep<BaseStep>(RegEx, Step)
        {
        }
    };

    enum class Hooktype {
        Before,
        After,
    };

    template <Hooktype hooktype>
    class BaseHook : public CommonStep<BaseHook<hooktype>> {
    public:
        virtual ~BaseHook() = default;

    protected:
        BaseHook(std::string RegEx, BaseHook<hooktype>* Step)
            : CommonStep<BaseHook<hooktype>>(RegEx, Step)
        {
        }
    };
}
}

#define STEP_INTERNAL(Name, Instance, RegEx)                                \
    struct Name : public gurkenlaeufer::detail::BaseStep {                  \
        Name()                                                              \
            : gurkenlaeufer::detail::BaseStep(RegEx, this)                  \
        {                                                                   \
        }                                                                   \
        void runStep(gurkenlaeufer::detail::StepContext& stepCtx) override; \
    } Instance;                                                             \
    void Name::runStep(gurkenlaeufer::detail::StepContext& stepCtx)

#define BEFORE_HOOK_INTERNAL(Name, Instance, RegEx)                                                 \
    struct Name : public gurkenlaeufer::detail::BaseHook<gurkenlaeufer::detail::Hooktype::Before> { \
        Name()                                                                                      \
            : gurkenlaeufer::detail::BaseHook<gurkenlaeufer::detail::Hooktype::Before>(RegEx, this) \
        {                                                                                           \
        }                                                                                           \
        void runStep(gurkenlaeufer::detail::StepContext& stepCtx) override;                         \
    } Instance;                                                                                     \
    void Name::runStep(gurkenlaeufer::detail::StepContext& stepCtx)

#define AFTER_HOOK_INTERNAL(Name, Instance, RegEx)                                                 \
    struct Name : public gurkenlaeufer::detail::BaseHook<gurkenlaeufer::detail::Hooktype::After> { \
        Name()                                                                                     \
            : gurkenlaeufer::detail::BaseHook<gurkenlaeufer::detail::Hooktype::After>(RegEx, this) \
        {                                                                                          \
        }                                                                                          \
        void runStep(gurkenlaeufer::detail::StepContext& stepCtx) override;                        \
    } Instance;                                                                                    \
    void Name::runStep(gurkenlaeufer::detail::StepContext& stepCtx)

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

#define GIVEN(RegEx) STEP(RegEx)
#define WHEN(RegEx) STEP(RegEx)
#define THEN(RegEx) STEP(RegEx)
#define REGEX_PARAM(type, name) const type name(stepCtx.getNextParam<type>())
