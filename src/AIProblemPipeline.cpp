#include "AIProblemPipeline.h"

#include <stdexcept>

#include "AIProblemParser.h"
#include "AIProblemValidator.h"

AIProblemPipeline::AIProblemPipeline(
    AIProblemNormalizer& normalizer
)
    : normalizer_(normalizer)
{
}

ProblemData AIProblemPipeline::process(
    const std::string& userInput
)
{
    const std::string normalizedProblemJson =
        normalizer_.normalize(
            userInput
        );

    const ProblemData problemData =
        parseAIProblemResponse(
            normalizedProblemJson
        );

    if (!isAIProblemDataValid(problemData))
    {
        throw std::runtime_error(
            "AI-generated problem data failed validation."
        );
    }

    return problemData;
}