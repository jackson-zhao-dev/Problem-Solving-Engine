#include <iostream>
#include "../include/State.h"
#include "../include/Validation.h"
using namespace std;

int main()
{
    State currentState = State::Ready;
    
    Validation startupValidation;
    startupValidation.description = "Check startup output";
    startupValidation.method = "Run the program";
    startupValidation.expectedResult = "Problem-Solving Engine is running.";
    
    cout << "Problem-Solving Engine is running." << endl;

    return 0;
}