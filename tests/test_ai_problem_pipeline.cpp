#include <cassert>
#include <exception>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "AIProblemNormalizer.h"
#include "AIProblemPipeline.h"
#include "HttpClient.h"
#include "OpenAIClient.h"
#include "OpenAIConfig.h"
#include "OpenAIUsageGuard.h"

class FakeHttpClient : public HttpClient
{
public:
    HttpResponse responseToReturn;

    int postCallCount = 0;

    HttpResponse post(
        const std::string&,
        const std::vector<std::string>&,
        const std::string&
    ) override
    {
        ++postCallCount;

        return responseToReturn;
    }
};

std::string buildFakeApiResponse(
    const std::string& outputText
)
{
    nlohmann::json response;

    response["status"] = "completed";

    response["output"] = {
        {
            {"type", "message"},
            {"role", "assistant"},
            {
                "content",
                {
                    {
                        {"type", "output_text"},
                        {"text", outputText},
                        {"annotations", nlohmann::json::array()}
                    }
                }
            }
        }
    };

    return response.dump();
}

int main()
{
    OpenAIConfig config{};

    config.apiKey = "test-key-not-real";
    config.model = "test-model";
    config.maxRequestsPerRun = 2;
    config.maxInputChars = 12000;

    OpenAIUsageGuard usageGuard(config);

    FakeHttpClient httpClient;

    OpenAIClient openAIClient(
        config,
        usageGuard,
        httpClient
    );

    AIProblemNormalizer normalizer(
        openAIClient
    );

    AIProblemPipeline pipeline(
        normalizer
    );

    const std::string validProblemJson = R"(
{
  "goal": {
    "name": "Build Rubber-Band Car",
    "description": "Build a rubber-band-powered car.",
    "success_condition": "The car travels at least 10 meters."
  },
  "nodes": [
    {
      "id": 1,
      "name": "Build prototype",
      "description": "Build the first prototype.",
      "state": "NotStarted",
      "priority": 3,
      "validation": {
        "description": "Check prototype movement.",
        "method": "Movement test",
        "expected_result": "The prototype moves.",
        "result": "Pending"
      }
    }
  ],
  "dependencies": [],
  "constraints": [
    {
      "name": "Minimum distance",
      "type": "Performance",
      "value": "At least 10 meters",
      "status": "Unknown"
    }
  ]
}
)";

    httpClient.responseToReturn.statusCode = 200;
    httpClient.responseToReturn.body =
        buildFakeApiResponse(
            validProblemJson
        );

    const ProblemData problemData =
        pipeline.process(
            "Build a rubber-band-powered car "
            "that travels at least 10 meters."
        );

    assert(
        problemData.goal.name
        == "Build Rubber-Band Car"
    );

    assert(
        problemData.nodes.size() == 1
    );

    assert(
        problemData.nodes[0].state
        == State::NotStarted
    );

    assert(
        problemData.nodes[0].validation.result
        == ValidationResult::Pending
    );

    assert(
        problemData.constraints.size() == 1
    );

    assert(
        problemData.constraints[0].status
        == ConstraintStatus::Unknown
    );

    assert(
        httpClient.postCallCount == 1
    );

    const std::string invalidProblemJson = R"(
{
  "goal": {
    "name": "Invalid AI Problem",
    "description": "AI improperly decides engine state.",
    "success_condition": "Test rejection."
  },
  "nodes": [
    {
      "id": 1,
      "name": "Invalid node",
      "description": "This node is incorrectly marked Ready.",
      "state": "Ready",
      "priority": 3,
      "validation": {
        "description": "Invalid validation.",
        "method": "Test",
        "expected_result": "Rejected",
        "result": "Pending"
      }
    }
  ],
  "dependencies": [],
  "constraints": []
}
)";

    httpClient.responseToReturn.body =
        buildFakeApiResponse(
            invalidProblemJson
        );

    bool invalidProblemRejected = false;

    try
    {
        pipeline.process(
            "Return an invalid problem."
        );
    }
    catch (const std::exception&)
    {
        invalidProblemRejected = true;
    }

    assert(invalidProblemRejected);

    assert(
        httpClient.postCallCount == 2
    );

    assert(
        usageGuard.getRequestsUsed() == 2
    );

    return 0;
}