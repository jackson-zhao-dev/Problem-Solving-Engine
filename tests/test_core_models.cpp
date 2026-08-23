#include <cassert>
#include <iostream>

#include "State.h"
#include "Validation.h"
#include "Constraint.h"
#include "Dependency.h"
#include "Node.h"
#include "Goal.h"

int main()
{
    State state = State::Ready;
    assert(state == State::Ready);

    Validation validation;
    assert(validation.result == ValidationResult::Pending);

    Constraint constraint;
    assert(constraint.type == ConstraintType::Other);
    assert(constraint.status == ConstraintStatus::Unknown);

    Dependency dependency;
    dependency.fromNode = 1;
    dependency.toNode = 2;
    assert(dependency.fromNode == 1);
    assert(dependency.toNode == 2);

    Node node;
    assert(node.state == State::NotStarted);

    Goal goal;
    goal.name = "Test Goal";
    assert(goal.name == "Test Goal");

    std::cout << "All core model tests passed." << std::endl;

    return 0;
}