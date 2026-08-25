#pragma once

#include <string>

#include "ProblemData.h"

ProblemData parseProblemDataFromJson(
    const std::string& jsonText
);