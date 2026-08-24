#include <cassert>
#include <vector>

#include "Engine.h"

int main()
{
    // State Transition tests
    assert(isValidStateTransition(State::NotStarted, State::Ready));
    assert(isValidStateTransition(State::NotStarted, State::Blocked));

    assert(isValidStateTransition(State::Ready, State::InProgress));
    assert(isValidStateTransition(State::Ready, State::Blocked));

    assert(isValidStateTransition(State::InProgress, State::Completed));
    assert(isValidStateTransition(State::InProgress, State::Failed));

    assert(isValidStateTransition(State::Blocked, State::Ready));

    assert(!isValidStateTransition(State::NotStarted, State::Completed));
    assert(!isValidStateTransition(State::Ready, State::Completed));
    assert(!isValidStateTransition(State::Completed, State::Ready));
    assert(!isValidStateTransition(State::Failed, State::Ready));

    // Ready-state Evaluation tests
    Node node1{};
    node1.id = 1;
    node1.state = State::Completed;

    Node node2{};
    node2.id = 2;
    node2.state = State::NotStarted;

    std::vector<Node> nodes = {node1, node2};

    Dependency dependency{};
    dependency.fromNode = 1;
    dependency.toNode = 2;

    std::vector<Dependency> dependencies = {dependency};

    Constraint satisfiedConstraint{};
    satisfiedConstraint.status = ConstraintStatus::Satisfied;

    std::vector<Constraint> satisfiedConstraints =
    {
        satisfiedConstraint
    };

    // Completed upstream dependency + satisfied constraint -> Ready
    assert(
        evaluateReadyState(
            node2,
            nodes,
            dependencies,
            satisfiedConstraints
        ) == State::Ready
    );

    // Incomplete upstream dependency -> Blocked
    nodes[0].state = State::InProgress;

    assert(
        evaluateReadyState(
            node2,
            nodes,
            dependencies,
            satisfiedConstraints
        ) == State::Blocked
    );

    // Restore upstream dependency
    nodes[0].state = State::Completed;

    // Violated constraint -> Blocked
    Constraint violatedConstraint{};
    violatedConstraint.status = ConstraintStatus::Violated;

    std::vector<Constraint> violatedConstraints =
    {
        violatedConstraint
    };

    assert(
        evaluateReadyState(
            node2,
            nodes,
            dependencies,
            violatedConstraints
        ) == State::Blocked
    );

    // Node with no dependencies and satisfied constraints -> Ready
    Node node3{};
    node3.id = 3;
    node3.state = State::NotStarted;

    std::vector<Dependency> noDependencies;

    assert(
        evaluateReadyState(
            node3,
            nodes,
            noDependencies,
            satisfiedConstraints
        ) == State::Ready
    );

    return 0;
}