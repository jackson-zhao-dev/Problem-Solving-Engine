#pragma once

#include <vector>

#include "State.h"
#include "Node.h"
#include "Dependency.h"
#include "Constraint.h"

bool isValidStateTransition(State from, State to);

State evaluateReadyState(
    const Node& node,
    const std::vector<Node>& nodes,
    const std::vector<Dependency>& dependencies,
    const std::vector<Constraint>& constraints
);

bool areDependenciesValid(
    const std::vector<Node>& nodes,
    const std::vector<Dependency>& dependencies
);

bool hasDependencyCycle(
    const std::vector<Node>& nodes,
    const std::vector<Dependency>& dependencies
);

ConstraintStatus evaluateConstraintStatus(
    const std::vector<Constraint>& constraints
);