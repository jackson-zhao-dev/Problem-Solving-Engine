#pragma once

#include <string>

enum class ConstraintType
{
    Cost,
    Time,
    Resource,
    Physical,
    Performance,
    Other
};

enum class ConstraintStatus
{
    Unknown,
    Satisfied,
    Violated
};

struct Constraint
{
    std::string name;
    ConstraintType type = ConstraintType::Other;
    std::string value;
    ConstraintStatus status = ConstraintStatus::Unknown;
};