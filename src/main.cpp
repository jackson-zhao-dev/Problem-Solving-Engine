#include <exception>
#include <iostream>
#include <string>

#include "AIProblemNormalizer.h"
#include "AIProblemPipeline.h"
#include "CurlHttpClient.h"
#include "OpenAIClient.h"
#include "OpenAIConfig.h"
#include "OpenAIUsageGuard.h"

using namespace std;

int main(int argc, char* argv[])
{
    cout
        << "Problem-Solving Engine is running."
        << endl;

    if (argc < 2 ||
        string(argv[1]) != "--ai")
    {
        cout
            << "AI-assisted mode is disabled by default."
            << endl;

        cout
            << "Run with --ai to enter a problem."
            << endl;

        return 0;
    }

    try
    {
        OpenAIConfig config =
            loadOpenAIConfigFromEnvironment();

        OpenAIUsageGuard usageGuard(
            config
        );

        CurlHttpClient httpClient;

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

        cout
            << "Enter a problem: ";

        string userInput;
        getline(cin, userInput);

        if (userInput.empty())
        {
            cerr
                << "Error: Problem input cannot be empty."
                << endl;

            return 1;
        }

        const ProblemData problemData =
            pipeline.process(
                userInput
            );

        cout
            << endl
            << "Problem accepted by the engine."
            << endl;

        cout
            << "Goal: "
            << problemData.goal.name
            << endl;

        cout
            << "Success condition: "
            << problemData.goal.successCondition
            << endl;

        cout
            << endl
            << "Plan:"
            << endl;

        for (const Node& node :
             problemData.nodes)
        {
            cout
                << "- ["
                << node.id
                << "] "
                << node.name
                << endl;

            cout
                << "  "
                << node.description
                << endl;
        }

        cout
            << endl
            << "Constraints:"
            << endl;

        for (const Constraint& constraint :
             problemData.constraints)
        {
            cout
                << "- "
                << constraint.name
                << ": "
                << constraint.value
                << endl;
        }

        cout
            << endl
            << "AI-generated problem structure passed "
            << "engine validation."
            << endl;
    }
    catch (const exception& error)
    {
        cerr
            << "Error: "
            << error.what()
            << endl;

        return 1;
    }

    return 0;
}