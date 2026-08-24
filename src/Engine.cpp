#include "Engine.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace
{
    const Node* findNodeById(
        const std::vector<Node>& nodes,
        int id
    )
    {
        for (const Node& node : nodes)
        {
            if (node.id == id)
            {
                return &node;
            }
        }

        return nullptr;
    }

    bool nodeIdExists(
        const std::vector<Node>& nodes,
        int id
    )
    {
        return findNodeById(nodes, id) != nullptr;
    }

    bool visitForCycle(
        int nodeId,
        const std::unordered_map<int, std::vector<int>>& graph,
        std::unordered_set<int>& visiting,
        std::unordered_set<int>& visited
    )
    {
        if (visiting.count(nodeId) > 0)
        {
            return true;
        }

        if (visited.count(nodeId) > 0)
        {
            return false;
        }

        visiting.insert(nodeId);

        auto it = graph.find(nodeId);

        if (it != graph.end())
        {
            for (int nextNodeId : it->second)
            {
                if (visitForCycle(
                        nextNodeId,
                        graph,
                        visiting,
                        visited
                    ))
                {
                    return true;
                }
            }
        }

        visiting.erase(nodeId);
        visited.insert(nodeId);

        return false;
    }
}

bool isValidStateTransition(State from, State to)
{
    switch (from)
    {
        case State::NotStarted:
            return to == State::Ready ||
                   to == State::Blocked;

        case State::Ready:
            return to == State::InProgress ||
                   to == State::Blocked;

        case State::InProgress:
            return to == State::Completed ||
                   to == State::Failed;

        case State::Blocked:
            return to == State::Ready;

        case State::Completed:
        case State::Failed:
            return false;
    }

    return false;
}

ConstraintStatus evaluateConstraintStatus(
    const std::vector<Constraint>& constraints
)
{
    bool hasUnknown = false;

    for (const Constraint& constraint : constraints)
    {
        if (constraint.status == ConstraintStatus::Violated)
        {
            return ConstraintStatus::Violated;
        }

        if (constraint.status == ConstraintStatus::Unknown)
        {
            hasUnknown = true;
        }
    }

    if (hasUnknown)
    {
        return ConstraintStatus::Unknown;
    }

    return ConstraintStatus::Satisfied;
}

State evaluateReadyState(
    const Node& node,
    const std::vector<Node>& nodes,
    const std::vector<Dependency>& dependencies,
    const std::vector<Constraint>& constraints
)
{
    if (node.state == State::InProgress ||
        node.state == State::Completed ||
        node.state == State::Failed)
    {
        return node.state;
    }

    if (evaluateConstraintStatus(constraints)
        != ConstraintStatus::Satisfied)
    {
        return State::Blocked;
    }

    for (const Dependency& dependency : dependencies)
    {
        if (dependency.toNode != node.id)
        {
            continue;
        }

        const Node* upstreamNode =
            findNodeById(nodes, dependency.fromNode);

        if (upstreamNode == nullptr ||
            upstreamNode->state != State::Completed)
        {
            return State::Blocked;
        }
    }

    return State::Ready;
}

bool areDependenciesValid(
    const std::vector<Node>& nodes,
    const std::vector<Dependency>& dependencies
)
{
    for (const Dependency& dependency : dependencies)
    {
        if (dependency.fromNode == dependency.toNode)
        {
            return false;
        }

        if (!nodeIdExists(nodes, dependency.fromNode) ||
            !nodeIdExists(nodes, dependency.toNode))
        {
            return false;
        }
    }

    return true;
}

bool hasDependencyCycle(
    const std::vector<Node>& nodes,
    const std::vector<Dependency>& dependencies
)
{
    std::unordered_map<int, std::vector<int>> graph;

    for (const Dependency& dependency : dependencies)
    {
        graph[dependency.fromNode].push_back(
            dependency.toNode
        );
    }

    std::unordered_set<int> visiting;
    std::unordered_set<int> visited;

    for (const Node& node : nodes)
    {
        if (visitForCycle(
                node.id,
                graph,
                visiting,
                visited
            ))
        {
            return true;
        }
    }

    return false;
}