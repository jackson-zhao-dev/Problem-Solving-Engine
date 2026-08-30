#include <cassert>

#include "AIProblemValidator.h"

int main()
{
    Node node1{};
    node1.id = 1;
    node1.priority = 3;
    node1.state = State::NotStarted;
    node1.validation.result =
        ValidationResult::Pending;

    Node node2{};
    node2.id = 2;
    node2.priority = 2;
    node2.state = State::NotStarted;
    node2.validation.result =
        ValidationResult::Pending;

    Dependency dependency{};
    dependency.fromNode = 1;
    dependency.toNode = 2;

    Constraint constraint{};
    constraint.name = "Minimum distance";
    constraint.type =
        ConstraintType::Performance;
    constraint.value =
        "At least 10 meters";
    constraint.status =
        ConstraintStatus::Unknown;

    ProblemData validAIProblem{};
    validAIProblem.nodes =
    {
        node1,
        node2
    };

    validAIProblem.dependencies =
    {
        dependency
    };

    validAIProblem.constraints =
    {
        constraint
    };

    assert(
        isAIProblemDataValid(
            validAIProblem
        )
    );

    // AI must not decide node state.
    ProblemData readyNodeProblem =
        validAIProblem;

    readyNodeProblem.nodes[0].state =
        State::Ready;

    assert(
        !isAIProblemDataValid(
            readyNodeProblem
        )
    );

    // AI must not decide validation result.
    ProblemData passedValidationProblem =
        validAIProblem;

    passedValidationProblem
        .nodes[0]
        .validation
        .result =
        ValidationResult::Pass;

    assert(
        !isAIProblemDataValid(
            passedValidationProblem
        )
    );

    // AI must not decide constraint status.
    ProblemData satisfiedConstraintProblem =
        validAIProblem;

    satisfiedConstraintProblem
        .constraints[0]
        .status =
        ConstraintStatus::Satisfied;

    assert(
        !isAIProblemDataValid(
            satisfiedConstraintProblem
        )
    );

    // Generic validation must still reject
    // duplicate node IDs.
    ProblemData duplicateIdProblem =
        validAIProblem;

    duplicateIdProblem.nodes[1].id = 1;

    assert(
        !isAIProblemDataValid(
            duplicateIdProblem
        )
    );

    // Generic validation must still reject
    // invalid dependency references.
    ProblemData invalidDependencyProblem =
        validAIProblem;

    invalidDependencyProblem
        .dependencies[0]
        .toNode = 99;

    assert(
        !isAIProblemDataValid(
            invalidDependencyProblem
        )
    );

    // Generic validation must still reject
    // dependency cycles.
    ProblemData cycleProblem =
        validAIProblem;

    Dependency reverseDependency{};
    reverseDependency.fromNode = 2;
    reverseDependency.toNode = 1;

    cycleProblem.dependencies.push_back(
        reverseDependency
    );

    assert(
        !isAIProblemDataValid(
            cycleProblem
        )
    );

    return 0;
}