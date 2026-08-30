#include "AIProblemNormalizer.h"

#include <string>

#include "ProblemPromptBuilder.h"

AIProblemNormalizer::AIProblemNormalizer(
    OpenAIClient& openAIClient
)
    : openAIClient_(openAIClient)
{
}

std::string AIProblemNormalizer::normalize(
    const std::string& userInput
)
{
    const std::string prompt =
        buildProblemStructurePrompt(
            userInput
        );

    return openAIClient_.sendRequest(
        prompt
    );
}