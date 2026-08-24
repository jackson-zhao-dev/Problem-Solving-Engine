#include "Engine.h"

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

    for (const Constraint& constraint : constraints)
    {
        if (constraint.status != ConstraintStatus::Satisfied)
        {
            return State::Blocked;
        }
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