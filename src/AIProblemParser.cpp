#include "AIProblemParser.h"

#include "JsonParser.h"

ProblemData parseAIProblemResponse(
    const std::string& aiResponse
)
{
    return parseProblemDataFromJson(
        aiResponse
    );
}