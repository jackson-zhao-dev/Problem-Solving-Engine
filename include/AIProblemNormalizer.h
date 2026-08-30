#pragma once

#include <string>

#include "OpenAIClient.h"

class AIProblemNormalizer
{
public:
    explicit AIProblemNormalizer(
        OpenAIClient& openAIClient
    );

    std::string normalize(
        const std::string& userInput
    );

private:
    OpenAIClient& openAIClient_;
};