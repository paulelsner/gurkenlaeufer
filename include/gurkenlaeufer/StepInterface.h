#pragma once

#include <sstream>
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

    class ScenarioContext {
        std::vector<std::shared_ptr<IFixture>> _fixtures;

    public:
        template <typename T>
        std::shared_ptr<T> getFixture()
        {
            for (auto& fixture : _fixtures) {
                auto ptr = std::dynamic_pointer_cast<T>(fixture);
                if (ptr != nullptr) {
                    return ptr;
                }
            }

            auto fixture = std::make_shared<Fixture<T>>();
            _fixtures.emplace_back(fixture);
            return fixture;
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
        std::vector<std::string> _params;
        std::size_t _currentParamIdx = 0u;
    };

    template <typename TStep>
    class CommonStep {
    public:
        virtual ~CommonStep() = default;
        virtual void runStep() = 0;

        using StepRegistry = std::list<std::pair<std::string, TStep*>>;
        static StepRegistry& getStepRegistry()
        {
            static StepRegistry registry;
            return registry;
        }

        void setScenarioContext(ScenarioContext* scenarioContext)
        {
            _scenarioContext = scenarioContext;
        }

        template <typename T>
        std::shared_ptr<T> getFixture()
        {
            return _scenarioContext->getFixture<T>();
        }

        void setStepContext(StepContext&& stepContext)
        {
            _stepContext = std::move(stepContext);
        }

        template <typename T>
        T getParam(std::size_t i) const
        {
            return _stepContext.getParam<T>(i);
        }

        template <typename T>
        const T getNextParam()
        {
            return _stepContext.getNextParam<T>();
        }

    protected:
        CommonStep(std::string RegEx, TStep* Step)
            : _stepContext({})
        {
            getStepRegistry().emplace_back(std::make_pair(RegEx, Step));
        }

    private:
        ScenarioContext* _scenarioContext;
        StepContext _stepContext;
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
