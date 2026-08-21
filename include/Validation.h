#pragma once

#include <string>

enum class ValidationResult
{
    Pending,
    Pass,
    Fail
};

struct Validation
{
    std::string description;
    std::string method;
    std::string expectedResult;
    ValidationResult result = ValidationResult::Pending;
};