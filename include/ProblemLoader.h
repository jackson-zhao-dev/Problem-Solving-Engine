#pragma once

#include <string>

#include "ProblemData.h"

ProblemData loadProblemDataFromJson(
    const std::string& jsonText
);