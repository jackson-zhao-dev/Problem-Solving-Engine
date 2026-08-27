#include <cassert>
#include <stdexcept>
#include <string>

#include "OpenAIUsageGuard.h"

namespace
{
    OpenAIConfig makeConfig(
        int maxRequestsPerRun,
        int maxInputChars
    )
    {
        OpenAIConfig config{};

        config.apiKey = "test-key-not-real";
        config.model = "test-model";
        config.maxRequestsPerRun =
            maxRequestsPerRun;
        config.maxInputChars =
            maxInputChars;

        return config;
    }
}

int main()
{
    // Valid configuration
    OpenAIConfig config =
        makeConfig(
            2,
            10
        );

    OpenAIUsageGuard guard(config);

    assert(
        guard.getRequestsUsed()
        == 0
    );

    // First valid request
    guard.validateAndRecordRequest(
        "hello"
    );

    assert(
        guard.getRequestsUsed()
        == 1
    );

    // Oversized input must be rejected
    bool oversizedInputRejected = false;

    try
    {
        guard.validateAndRecordRequest(
            "this-input-is-too-long"
        );
    }
    catch (const std::runtime_error&)
    {
        oversizedInputRejected = true;
    }

    assert(oversizedInputRejected);

    // Rejected request must not consume quota
    assert(
        guard.getRequestsUsed()
        == 1
    );

    // Second valid request
    guard.validateAndRecordRequest(
        "second"
    );

    assert(
        guard.getRequestsUsed()
        == 2
    );

    // Third request exceeds request limit
    bool requestLimitRejected = false;

    try
    {
        guard.validateAndRecordRequest(
            "third"
        );
    }
    catch (const std::runtime_error&)
    {
        requestLimitRejected = true;
    }

    assert(requestLimitRejected);

    assert(
        guard.getRequestsUsed()
        == 2
    );

    // Invalid request limit
    bool invalidRequestLimitRejected = false;

    try
    {
        OpenAIConfig invalidConfig =
            makeConfig(
                0,
                10
            );

        OpenAIUsageGuard invalidGuard(
            invalidConfig
        );
    }
    catch (const std::invalid_argument&)
    {
        invalidRequestLimitRejected = true;
    }

    assert(invalidRequestLimitRejected);

    // Invalid input limit
    bool invalidInputLimitRejected = false;

    try
    {
        OpenAIConfig invalidConfig =
            makeConfig(
                2,
                0
            );

        OpenAIUsageGuard invalidGuard(
            invalidConfig
        );
    }
    catch (const std::invalid_argument&)
    {
        invalidInputLimitRejected = true;
    }

    assert(invalidInputLimitRejected);

    return 0;
}