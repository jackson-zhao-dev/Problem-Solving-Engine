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

    assert(
        evaluateReadyState(
            node2,
            nodes,
            dependencies,
            satisfiedConstraints
        ) == State::Ready
    );

    nodes[0].state = State::InProgress;

    assert(
        evaluateReadyState(
            node2,
            nodes,
            dependencies,
            satisfiedConstraints
        ) == State::Blocked
    );

    nodes[0].state = State::Completed;

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

    // Dependency Validation tests
    Node dependencyNode1{};
    dependencyNode1.id = 1;

    Node dependencyNode2{};
    dependencyNode2.id = 2;

    Node dependencyNode3{};
    dependencyNode3.id = 3;

    std::vector<Node> dependencyNodes =
    {
        dependencyNode1,
        dependencyNode2,
        dependencyNode3
    };

    Dependency validDependency1{};
    validDependency1.fromNode = 1;
    validDependency1.toNode = 2;

    Dependency validDependency2{};
    validDependency2.fromNode = 2;
    validDependency2.toNode = 3;

    std::vector<Dependency> validDependencies =
    {
        validDependency1,
        validDependency2
    };

    assert(
        areDependenciesValid(
            dependencyNodes,
            validDependencies
        )
    );

    Dependency missingNodeDependency{};
    missingNodeDependency.fromNode = 1;
    missingNodeDependency.toNode = 99;

    std::vector<Dependency> invalidReferenceDependencies =
    {
        missingNodeDependency
    };

    assert(
        !areDependenciesValid(
            dependencyNodes,
            invalidReferenceDependencies
        )
    );

    Dependency selfDependency{};
    selfDependency.fromNode = 2;
    selfDependency.toNode = 2;

    std::vector<Dependency> selfDependencies =
    {
        selfDependency
    };

    assert(
        !areDependenciesValid(
            dependencyNodes,
            selfDependencies
        )
    );

    // Cycle Detection tests
    assert(
        !hasDependencyCycle(
            dependencyNodes,
            validDependencies
        )
    );

    Dependency cycleDependency1{};
    cycleDependency1.fromNode = 1;
    cycleDependency1.toNode = 2;

    Dependency cycleDependency2{};
    cycleDependency2.fromNode = 2;
    cycleDependency2.toNode = 3;

    Dependency cycleDependency3{};
    cycleDependency3.fromNode = 3;
    cycleDependency3.toNode = 1;

    std::vector<Dependency> cyclicDependencies =
    {
        cycleDependency1,
        cycleDependency2,
        cycleDependency3
    };

    assert(
        hasDependencyCycle(
            dependencyNodes,
            cyclicDependencies
        )
    );

    return 0;
}