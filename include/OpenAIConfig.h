#pragma once

#include <string>

struct OpenAIConfig
{
    std::string apiKey;
    std::string model;

    int maxRequestsPerRun;
    int maxInputChars;
};

OpenAIConfig loadOpenAIConfigFromEnvironment();