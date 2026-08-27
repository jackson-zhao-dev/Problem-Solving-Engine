#pragma once

#include <string>

#include "HttpClient.h"
#include "OpenAIConfig.h"
#include "OpenAIUsageGuard.h"

class OpenAIClient
{
public:
    OpenAIClient(
        const OpenAIConfig& config,
        OpenAIUsageGuard& usageGuard,
        HttpClient& httpClient
    );

    std::string sendRequest(
        const std::string& input
    );

private:
    OpenAIConfig config_;
    OpenAIUsageGuard& usageGuard_;
    HttpClient& httpClient_;
};