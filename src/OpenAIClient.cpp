#include "OpenAIClient.h"

#include <stdexcept>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

OpenAIClient::OpenAIClient(
    const OpenAIConfig& config,
    OpenAIUsageGuard& usageGuard,
    HttpClient& httpClient
)
    : config_(config),
      usageGuard_(usageGuard),
      httpClient_(httpClient)
{
}

std::string OpenAIClient::sendRequest(
    const std::string& input
)
{
    usageGuard_.validateAndRecordRequest(
        input
    );

    nlohmann::json requestBody;

    requestBody["model"] =
        config_.model;

    requestBody["input"] =
        input;

    std::vector<std::string> headers =
    {
        "Content-Type: application/json",
        "Authorization: Bearer " +
            config_.apiKey
    };

    const HttpResponse response =
        httpClient_.post(
            "https://api.openai.com/v1/responses",
            headers,
            requestBody.dump()
        );

    if (response.statusCode < 200 ||
        response.statusCode >= 300)
    {
        throw std::runtime_error(
            "OpenAI API request failed with HTTP status " +
            std::to_string(
                response.statusCode
            ) +
            "."
        );
    }

    nlohmann::json responseBody;

    try
    {
        responseBody =
            nlohmann::json::parse(
                response.body
            );
    }
    catch (const nlohmann::json::exception&)
    {
        throw std::runtime_error(
            "OpenAI API returned invalid JSON."
        );
    }

    if (responseBody.contains("status") &&
        responseBody.at("status").is_string() &&
        responseBody.at("status") != "completed")
    {
        throw std::runtime_error(
            "OpenAI API response was not completed."
        );
    }

    if (!responseBody.contains("output") ||
        !responseBody.at("output").is_array())
    {
        throw std::runtime_error(
            "OpenAI API response is missing output."
        );
    }

    for (const auto& outputItem :
         responseBody.at("output"))
    {
        if (!outputItem.contains("content") ||
            !outputItem.at("content").is_array())
        {
            continue;
        }

        for (const auto& contentPart :
             outputItem.at("content"))
        {
            if (!contentPart.contains("type") ||
                !contentPart.contains("text"))
            {
                continue;
            }

            if (contentPart.at("type")
                    == "output_text" &&
                contentPart.at("text").is_string())
            {
                return contentPart.at("text")
                    .get<std::string>();
            }
        }
    }

    throw std::runtime_error(
        "OpenAI API response contains no output text."
    );
}