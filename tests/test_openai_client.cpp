#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "HttpClient.h"
#include "OpenAIClient.h"
#include "OpenAIConfig.h"
#include "OpenAIUsageGuard.h"

class FakeHttpClient : public HttpClient
{
public:
    HttpResponse responseToReturn;

    int postCallCount = 0;

    std::string lastUrl;
    std::vector<std::string> lastHeaders;
    std::string lastBody;

    HttpResponse post(
        const std::string& url,
        const std::vector<std::string>& headers,
        const std::string& body
    ) override
    {
        ++postCallCount;

        lastUrl = url;
        lastHeaders = headers;
        lastBody = body;

        return responseToReturn;
    }
};

bool containsHeader(
    const std::vector<std::string>& headers,
    const std::string& expectedHeader
)
{
    return std::find(
        headers.begin(),
        headers.end(),
        expectedHeader
    ) != headers.end();
}

int main()
{
    OpenAIConfig config{};

    config.apiKey = "test-key-not-real";
    config.model = "test-model";
    config.maxRequestsPerRun = 2;
    config.maxInputChars = 1000;

    OpenAIUsageGuard usageGuard(config);

    FakeHttpClient httpClient;

    httpClient.responseToReturn.statusCode = 200;

    httpClient.responseToReturn.body = R"(
{
    "status": "completed",
    "output": [
        {
            "type": "message",
            "role": "assistant",
            "content": [
                {
                    "type": "output_text",
                    "text": "fake answer",
                    "annotations": []
                }
            ]
        }
    ]
}
)";

    OpenAIClient client(
        config,
        usageGuard,
        httpClient
    );

    const std::string result =
        client.sendRequest(
            "hello"
        );

    assert(result == "fake answer");

    assert(httpClient.postCallCount == 1);

    assert(
        httpClient.lastUrl
        == "https://api.openai.com/v1/responses"
    );

    assert(
        containsHeader(
            httpClient.lastHeaders,
            "Content-Type: application/json"
        )
    );

    assert(
        containsHeader(
            httpClient.lastHeaders,
            "Authorization: Bearer test-key-not-real"
        )
    );

    const nlohmann::json requestBody =
        nlohmann::json::parse(
            httpClient.lastBody
        );

    assert(
        requestBody.at("model")
        == "test-model"
    );

    assert(
        requestBody.at("input")
        == "hello"
    );

    assert(
        usageGuard.getRequestsUsed()
        == 1
    );

    return 0;
}