#pragma once

#include <string>
#include "State.h"
#include "Validation.h"

struct Node
{
    int id;
    std::string name;
    std::string description;
    State state = State::NotStarted;
    int priority;
    Validation validation;
};