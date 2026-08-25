#include <cassert>
#include <vector>

#include "ProblemValidator.h"

int main()
{
    // Valid problem data
    Node node1{};
    node1.id = 1;
    node1.priority = 3;

    Node node2{};
    node2.id = 2;
    node2.priority = 2;

    Dependency dependency{};
    dependency.fromNode = 1;
    dependency.toNode = 2;

    ProblemData validProblem{};
    validProblem.nodes =
    {
        node1,
        node2
    };

    validProblem.dependencies =
    {
        dependency
    };

    assert(
        isProblemDataValid(validProblem)
    );

    // Duplicate node ID
    Node duplicateNode1{};
    duplicateNode1.id = 1;
    duplicateNode1.priority = 2;

    Node duplicateNode2{};
    duplicateNode2.id = 1;
    duplicateNode2.priority = 3;

    ProblemData duplicateIdProblem{};
    duplicateIdProblem.nodes =
    {
        duplicateNode1,
        duplicateNode2
    };

    assert(
        !isProblemDataValid(
            duplicateIdProblem
        )
    );

    // Invalid priority
    Node invalidPriorityNode{};
    invalidPriorityNode.id = 10;
    invalidPriorityNode.priority = 4;

    ProblemData invalidPriorityProblem{};
    invalidPriorityProblem.nodes =
    {
        invalidPriorityNode
    };

    assert(
        !isProblemDataValid(
            invalidPriorityProblem
        )
    );

    // Invalid dependency reference
    Node dependencyNode{};
    dependencyNode.id = 20;
    dependencyNode.priority = 2;

    Dependency invalidDependency{};
    invalidDependency.fromNode = 20;
    invalidDependency.toNode = 99;

    ProblemData invalidDependencyProblem{};
    invalidDependencyProblem.nodes =
    {
        dependencyNode
    };

    invalidDependencyProblem.dependencies =
    {
        invalidDependency
    };

    assert(
        !isProblemDataValid(
            invalidDependencyProblem
        )
    );

    // Dependency cycle
    Node cycleNode1{};
    cycleNode1.id = 30;
    cycleNode1.priority = 2;

    Node cycleNode2{};
    cycleNode2.id = 31;
    cycleNode2.priority = 2;

    Dependency cycleDependency1{};
    cycleDependency1.fromNode = 30;
    cycleDependency1.toNode = 31;

    Dependency cycleDependency2{};
    cycleDependency2.fromNode = 31;
    cycleDependency2.toNode = 30;

    ProblemData cycleProblem{};
    cycleProblem.nodes =
    {
        cycleNode1,
        cycleNode2
    };

    cycleProblem.dependencies =
    {
        cycleDependency1,
        cycleDependency2
    };

    assert(
        !isProblemDataValid(
            cycleProblem
        )
    );

    return 0;
}