#include "OpenAIUsageGuard.h"

#include <stdexcept>

OpenAIUsageGuard::OpenAIUsageGuard(
    const OpenAIConfig& config
)
    : maxRequestsPerRun_(
          config.maxRequestsPerRun
      ),
      maxInputChars_(
          config.maxInputChars
      )
{
    if (maxRequestsPerRun_ <= 0)
    {
        throw std::invalid_argument(
            "Maximum requests per run must be positive."
        );
    }

    if (maxInputChars_ <= 0)
    {
        throw std::invalid_argument(
            "Maximum input characters must be positive."
        );
    }
}

void OpenAIUsageGuard::validateAndRecordRequest(
    const std::string& input
)
{
    if (requestsUsed_ >= maxRequestsPerRun_)
    {
        throw std::runtime_error(
            "OpenAI request limit reached."
        );
    }

    if (input.size() >
        static_cast<std::size_t>(
            maxInputChars_
        ))
    {
        throw std::runtime_error(
            "OpenAI input exceeds maximum allowed length."
        );
    }

    ++requestsUsed_;
}

int OpenAIUsageGuard::getRequestsUsed() const
{
    return requestsUsed_;
}