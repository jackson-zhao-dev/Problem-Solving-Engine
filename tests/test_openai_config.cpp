#include <cassert>
#include <cstdlib>
#include <stdexcept>
#include <string>

#include "OpenAIConfig.h"

namespace
{
    void clearOpenAIEnvironment()
    {
        unsetenv("OPENAI_API_KEY");
        unsetenv("OPENAI_MODEL");
        unsetenv("OPENAI_MAX_REQUESTS_PER_RUN");
        unsetenv("OPENAI_MAX_INPUT_CHARS");
    }

    void setValidBaseEnvironment()
    {
        setenv(
            "OPENAI_API_KEY",
            "test-key-not-real",
            1
        );

        setenv(
            "OPENAI_MODEL",
            "test-model",
            1
        );

        setenv(
            "OPENAI_MAX_REQUESTS_PER_RUN",
            "5",
            1
        );

        setenv(
            "OPENAI_MAX_INPUT_CHARS",
            "12000",
            1
        );
    }
}

int main()
{
    // Missing API key must be rejected
    clearOpenAIEnvironment();

    bool missingApiKeyRejected = false;

    try
    {
        loadOpenAIConfigFromEnvironment();
    }
    catch (const std::runtime_error&)
    {
        missingApiKeyRejected = true;
    }

    assert(missingApiKeyRejected);

    // Missing model must be rejected
    clearOpenAIEnvironment();

    setenv(
        "OPENAI_API_KEY",
        "test-key-not-real",
        1
    );

    bool missingModelRejected = false;

    try
    {
        loadOpenAIConfigFromEnvironment();
    }
    catch (const std::runtime_error&)
    {
        missingModelRejected = true;
    }

    assert(missingModelRejected);

    // Missing request limit must be rejected
    clearOpenAIEnvironment();

    setenv(
        "OPENAI_API_KEY",
        "test-key-not-real",
        1
    );

    setenv(
        "OPENAI_MODEL",
        "test-model",
        1
    );

    bool missingRequestLimitRejected = false;

    try
    {
        loadOpenAIConfigFromEnvironment();
    }
    catch (const std::runtime_error&)
    {
        missingRequestLimitRejected = true;
    }

    assert(missingRequestLimitRejected);

    // Zero request limit must be rejected
    setValidBaseEnvironment();

    setenv(
        "OPENAI_MAX_REQUESTS_PER_RUN",
        "0",
        1
    );

    bool zeroRequestLimitRejected = false;

    try
    {
        loadOpenAIConfigFromEnvironment();
    }
    catch (const std::runtime_error&)
    {
        zeroRequestLimitRejected = true;
    }

    assert(zeroRequestLimitRejected);

    // Negative request limit must be rejected
    setValidBaseEnvironment();

    setenv(
        "OPENAI_MAX_REQUESTS_PER_RUN",
        "-3",
        1
    );

    bool negativeRequestLimitRejected = false;

    try
    {
        loadOpenAIConfigFromEnvironment();
    }
    catch (const std::runtime_error&)
    {
        negativeRequestLimitRejected = true;
    }

    assert(negativeRequestLimitRejected);

    // Non-numeric request limit must be rejected
    setValidBaseEnvironment();

    setenv(
        "OPENAI_MAX_REQUESTS_PER_RUN",
        "abc",
        1
    );

    bool textRequestLimitRejected = false;

    try
    {
        loadOpenAIConfigFromEnvironment();
    }
    catch (const std::runtime_error&)
    {
        textRequestLimitRejected = true;
    }

    assert(textRequestLimitRejected);

    // Partially numeric value must be rejected
    setValidBaseEnvironment();

    setenv(
        "OPENAI_MAX_INPUT_CHARS",
        "12000abc",
        1
    );

    bool partialNumberRejected = false;

    try
    {
        loadOpenAIConfigFromEnvironment();
    }
    catch (const std::runtime_error&)
    {
        partialNumberRejected = true;
    }

    assert(partialNumberRejected);

    // Valid configuration must load successfully
    setValidBaseEnvironment();

    const OpenAIConfig config =
        loadOpenAIConfigFromEnvironment();

    assert(
        config.apiKey
        == "test-key-not-real"
    );

    assert(
        config.model
        == "test-model"
    );

    assert(
        config.maxRequestsPerRun
        == 5
    );

    assert(
        config.maxInputChars
        == 12000
    );

    clearOpenAIEnvironment();

    return 0;
}