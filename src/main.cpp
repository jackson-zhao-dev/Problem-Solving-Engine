#include <iostream>
#include "../include/State.h"
#include "../include/Validation.h"
#include "../include/Constraint.h"
#include "../include/Dependency.h"
#include "../include/Node.h"
#include "../include/Goal.h"
using namespace std;

int main()
{
    State currentState = State::Ready;
    
    Validation startupValidation;
    startupValidation.description = "Check startup output";
    startupValidation.method = "Run the program";
    startupValidation.expectedResult = "Problem-Solving Engine is running.";

    Constraint budgetConstraint;
    budgetConstraint.name = "Budget Limit";
    budgetConstraint.type = ConstraintType::Cost;
    budgetConstraint.value = "Under $30";
    budgetConstraint.status = ConstraintStatus::Satisfied;

    Dependency buildDependency;
    buildDependency.fromNode = 1;
    buildDependency.toNode = 2;

    Node testNode;
    testNode.id = 1;
    testNode.name = "Prepare Materials";
    testNode.description = "Prepare all required materials.";
    testNode.state = State::Ready;
    testNode.priority = 3;
    testNode.validation = startupValidation;

    Goal projectGoal;
    projectGoal.name = "Build Rubber-Band Car";
    projectGoal.description = "Build a small car powered by a rubber band.";
    projectGoal.successCondition = "The car travels at least 10 meters.";

    cout << "Problem-Solving Engine is running." << endl;

    return 0;
}