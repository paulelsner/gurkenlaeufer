#pragma once

#include "StepInterface.h"

// cucumber-cpp adaption of CUKE_OBJECT_PREFIX
#ifndef GURKE_STEP_NAME_PREFIX
#ifdef CUKE_OBJECT_PREFIX
#define GURKE_STEP_NAME_PREFIX CUKE_OBJECT_PREFIX
#endif
#endif

#ifndef GURKE_STEP_NAME_PREFIX
#error GURKE_STEP_NAME_PREFIX has to be defined!
#endif

#define GURKE_STEP_INTERNAL(Name, Instance, RegEx)               \
    struct Name final : public gurkenlaeufer::detail::BaseStep { \
        Name()                                                   \
            : gurkenlaeufer::detail::BaseStep(RegEx, this)       \
        {                                                        \
        }                                                        \
        void runStep() override;                                 \
    } Instance;                                                  \
    void Name::runStep()

#define GURKE_BEFORE_HOOK_INTERNAL(Name, Instance, RegEx)                                                 \
    struct Name final : public gurkenlaeufer::detail::BaseHook<gurkenlaeufer::detail::Hooktype::Before> { \
        Name()                                                                                            \
            : gurkenlaeufer::detail::BaseHook<gurkenlaeufer::detail::Hooktype::Before>(RegEx, this)       \
        {                                                                                                 \
        }                                                                                                 \
        void runStep() override;                                                                          \
    } Instance;                                                                                           \
    void Name::runStep()

#define GURKE_AFTER_HOOK_INTERNAL(Name, Instance, RegEx)                                                 \
    struct Name final : public gurkenlaeufer::detail::BaseHook<gurkenlaeufer::detail::Hooktype::After> { \
        Name()                                                                                           \
            : gurkenlaeufer::detail::BaseHook<gurkenlaeufer::detail::Hooktype::After>(RegEx, this)       \
        {                                                                                                \
        }                                                                                                \
        void runStep() override;                                                                         \
    } Instance;                                                                                          \
    void Name::runStep()

#define GURKE_STR2(x) #x
#define GURKE_STR(x) x
#define GURKE_JOIN(X, Y) GURKE_DO_GURKE_JOIN(X, Y)
#define GURKE_DO_GURKE_JOIN(X, Y) GURKE_GURKE_DO_GURKE_JOIN2(X, Y)
#define GURKE_GURKE_DO_GURKE_JOIN2(X, Y) X##Y
#define STEP(RegEx)                                                                                   \
    GURKE_STEP_INTERNAL(                                                                              \
        GURKE_JOIN(GURKE_STR(GURKE_STEP_NAME_PREFIX), GURKE_JOIN(_Step, GURKE_STR(__COUNTER__))),     \
        GURKE_JOIN(GURKE_STR(GURKE_STEP_NAME_PREFIX), GURKE_JOIN(_Instance, GURKE_STR(__COUNTER__))), \
        RegEx)
#define BEFORE(RegEx)                                                                                   \
    GURKE_BEFORE_HOOK_INTERNAL(                                                                         \
        GURKE_JOIN(GURKE_STR(GURKE_STEP_NAME_PREFIX), GURKE_JOIN(_BeforeHook, GURKE_STR(__COUNTER__))), \
        GURKE_JOIN(GURKE_STR(GURKE_STEP_NAME_PREFIX), GURKE_JOIN(_Instance, GURKE_STR(__COUNTER__))),   \
        RegEx)
#define AFTER(RegEx)                                                                                   \
    GURKE_AFTER_HOOK_INTERNAL(                                                                         \
        GURKE_JOIN(GURKE_STR(GURKE_STEP_NAME_PREFIX), GURKE_JOIN(_AfterHook, GURKE_STR(__COUNTER__))), \
        GURKE_JOIN(GURKE_STR(GURKE_STEP_NAME_PREFIX), GURKE_JOIN(_Instance, GURKE_STR(__COUNTER__))),  \
        RegEx)

// cucumber-cpp adaption layer
// to replace cucumber-cpp with gurkenlaeufer you have to do the following:
// * #include "gurkenlaeufer/Step.h" instead of cucumber-cpp for your step definitions
// * replace "cucumber::ScenarioScope<Context> Ctx;" by "gurkenlaeufer::ScenarioScope<Context> Ctx(this);"
#define GIVEN(RegEx) STEP(RegEx)
#define WHEN(RegEx) STEP(RegEx)
#define THEN(RegEx) STEP(RegEx)
#define REGEX_PARAM(type, name) const type name(getNextParam<type>())

namespace gurkenlaeufer {
template <class T>
class ScenarioScope {
public:
    template <class TStep>
    ScenarioScope(detail::CommonStep<TStep>* ptr);

    T& operator*();
    T* operator->();
    T* get();

private:
    std::shared_ptr<T> context;
};

template <class T>
template <class TStep>
ScenarioScope<T>::ScenarioScope(detail::CommonStep<TStep>* ptr)
{
    context = ptr->template getFixture<T>();
}

template <class T>
T& ScenarioScope<T>::operator*()
{
    return *(context.get());
}

template <class T>
T* ScenarioScope<T>::operator->()
{
    return (context.get());
}

template <class T>
T* ScenarioScope<T>::get()
{
    return context.get();
}
}
