#include "OpenAIConfig.h"

#include <cstdlib>
#include <stdexcept>
#include <string>

namespace
{
    std::string requireEnvironmentVariable(
        const char* variableName
    )
    {
        const char* value =
            std::getenv(variableName);

        if (value == nullptr ||
            std::string(value).empty())
        {
            throw std::runtime_error(
                "Missing required environment variable: "
                + std::string(variableName)
            );
        }

        return std::string(value);
    }

    int requirePositiveIntegerEnvironmentVariable(
        const char* variableName
    )
    {
        const std::string value =
            requireEnvironmentVariable(
                variableName
            );

        std::size_t processedCharacters = 0;
        int parsedValue = 0;

        try
        {
            parsedValue =
                std::stoi(
                    value,
                    &processedCharacters
                );
        }
        catch (const std::exception&)
        {
            throw std::runtime_error(
                "Invalid positive integer environment variable: "
                + std::string(variableName)
            );
        }

        if (processedCharacters != value.size() ||
            parsedValue <= 0)
        {
            throw std::runtime_error(
                "Invalid positive integer environment variable: "
                + std::string(variableName)
            );
        }

        return parsedValue;
    }
}

OpenAIConfig loadOpenAIConfigFromEnvironment()
{
    OpenAIConfig config{};

    config.apiKey =
        requireEnvironmentVariable(
            "OPENAI_API_KEY"
        );

    config.model =
        requireEnvironmentVariable(
            "OPENAI_MODEL"
        );

    config.maxRequestsPerRun =
        requirePositiveIntegerEnvironmentVariable(
            "OPENAI_MAX_REQUESTS_PER_RUN"
        );

    config.maxInputChars =
        requirePositiveIntegerEnvironmentVariable(
            "OPENAI_MAX_INPUT_CHARS"
        );

    return config;
}