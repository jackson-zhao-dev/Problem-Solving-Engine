#include "ProblemValidator.h"

#include <unordered_set>

#include "Engine.h"

bool isProblemDataValid(
    const ProblemData& problemData
)
{
    std::unordered_set<int> nodeIds;

    for (const Node& node : problemData.nodes)
    {
        if (nodeIds.count(node.id) > 0)
        {
            return false;
        }

        nodeIds.insert(node.id);

        if (node.priority < 1 ||
            node.priority > 3)
        {
            return false;
        }
    }

    if (!areDependenciesValid(
            problemData.nodes,
            problemData.dependencies
        ))
    {
        return false;
    }

    if (hasDependencyCycle(
            problemData.nodes,
            problemData.dependencies
        ))
    {
        return false;
    }

    return true;
}