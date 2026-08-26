#pragma once

#include <string>

#include "ProblemData.h"

void saveProblemDataToFile(
    const ProblemData& problemData,
    const std::string& filePath
);

ProblemData loadProblemDataFromFile(
    const std::string& filePath
);