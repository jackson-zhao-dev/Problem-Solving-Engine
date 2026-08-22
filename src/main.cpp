#include <iostream>
#include "../include/State.h"
#include "../include/Validation.h"
#include "../include/Constraint.h"
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
    
    cout << "Problem-Solving Engine is running." << endl;

    return 0;
}