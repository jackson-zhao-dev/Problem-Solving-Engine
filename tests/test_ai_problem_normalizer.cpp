#include <cassert>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "AIProblemNormalizer.h"
#include "HttpClient.h"
#include "OpenAIClient.h"
#include "OpenAIConfig.h"
#include "OpenAIUsageGuard.h"

class FakeHttpClient : public HttpClient
{
public:
    HttpResponse responseToReturn;

    int postCallCount = 0;
    std::string lastBody;

    HttpResponse post(
        const std::string&,
        const std::vector<std::string>&,
        const std::string& body
    ) override
    {
        ++postCallCount;
        lastBody = body;

        return responseToReturn;
    }
};

int main()
{
    OpenAIConfig config{};

    config.apiKey = "test-key-not-real";
    config.model = "test-model";
    config.maxRequestsPerRun = 2;
    config.maxInputChars = 12000;

    OpenAIUsageGuard usageGuard(config);

    FakeHttpClient httpClient;

    const std::string fakeProblemJson = R"(
{
  "goal": {
    "name": "Build Rubber-Band Car",
    "description": "Build a small rubber-band-powered car.",
    "success_condition": "The car travels at least 10 meters."
  },
  "nodes": [],
  "dependencies": [],
  "constraints": []
}
)";

    nlohmann::json fakeApiResponse;

    fakeApiResponse["status"] = "completed";

    fakeApiResponse["output"] = {
        {
            {"type", "message"},
            {"role", "assistant"},
            {
                "content",
                {
                    {
                        {"type", "output_text"},
                        {"text", fakeProblemJson},
                        {"annotations", nlohmann::json::array()}
                    }
                }
            }
        }
    };

    httpClient.responseToReturn.statusCode = 200;
    httpClient.responseToReturn.body =
        fakeApiResponse.dump();

    OpenAIClient openAIClient(
        config,
        usageGuard,
        httpClient
    );

    AIProblemNormalizer normalizer(
        openAIClient
    );

    const std::string userInput =
        "I want to build a rubber-band-powered car "
        "that can travel at least 10 meters.";

    const std::string result =
        normalizer.normalize(
            userInput
        );

    assert(result == fakeProblemJson);

    assert(httpClient.postCallCount == 1);

    const nlohmann::json requestBody =
        nlohmann::json::parse(
            httpClient.lastBody
        );

    const std::string sentPrompt =
        requestBody.at("input");

    assert(
        sentPrompt.find(userInput)
        != std::string::npos
    );

    assert(
        sentPrompt.find(
            "AI proposes structure; Engine makes decisions."
        )
        != std::string::npos
    );

    assert(
        sentPrompt.find(
            "Return JSON only."
        )
        != std::string::npos
    );

    assert(
        usageGuard.getRequestsUsed()
        == 1
    );

    return 0;
}