#pragma once

#include <vector>

#include "Goal.h"
#include "Node.h"
#include "Dependency.h"
#include "Constraint.h"

struct ProblemData
{
    Goal goal;
    std::vector<Node> nodes;
    std::vector<Dependency> dependencies;
    std::vector<Constraint> constraints;
};