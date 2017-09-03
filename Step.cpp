#include "Step.h"

std::list<std::pair<std::string, BaseStep*>> BaseStep::StepRegistry;

TestContext CurrentTestContext;