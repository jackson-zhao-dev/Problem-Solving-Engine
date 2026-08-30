#include <cassert>
#include <string>

#include "ProblemPromptBuilder.h"

int main()
{
    const std::string userInput =
        "I want to build a rubber-band-powered car "
        "that can travel at least 10 meters.";

    const std::string prompt =
        buildProblemStructurePrompt(
            userInput
        );

    assert(
        prompt.find(userInput)
        != std::string::npos
    );

    assert(
        prompt.find("Return JSON only.")
        != std::string::npos
    );

    assert(
        prompt.find("\"state\": \"NotStarted\"")
        != std::string::npos
    );

    assert(
        prompt.find("\"result\": \"Pending\"")
        != std::string::npos
    );

    assert(
        prompt.find("\"status\": \"Unknown\"")
        != std::string::npos
    );

    assert(
        prompt.find(
            "select the Recommended Next Step"
        )
        != std::string::npos
    );

    assert(
        prompt.find(
            "AI proposes structure; Engine makes decisions."
        )
        != std::string::npos
    );

    return 0;
}