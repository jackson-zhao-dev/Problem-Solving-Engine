#pragma once

#include <string>

#include "OpenAIConfig.h"

class OpenAIUsageGuard
{
public:
    explicit OpenAIUsageGuard(
        const OpenAIConfig& config
    );

    void validateAndRecordRequest(
        const std::string& input
    );

    int getRequestsUsed() const;

private:
    int maxRequestsPerRun_;
    int maxInputChars_;
    int requestsUsed_ = 0;
};