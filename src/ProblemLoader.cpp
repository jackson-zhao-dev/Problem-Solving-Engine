#include "ProblemLoader.h"

#include <stdexcept>

#include "JsonParser.h"
#include "ProblemValidator.h"

ProblemData loadProblemDataFromJson(
    const std::string& jsonText
)
{
    ProblemData problemData =
        parseProblemDataFromJson(jsonText);

    if (!isProblemDataValid(problemData))
    {
        throw std::invalid_argument(
            "Invalid problem data."
        );
    }

    return problemData;
}