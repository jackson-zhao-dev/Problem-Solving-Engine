#include <cassert>
#include <stdexcept>
#include <string>

#include "ProblemLoader.h"

int main()
{
    // Valid JSON should load successfully
    const std::string validJson = R"(
    {
        "goal": {
            "name": "Test Goal",
            "description": "A valid test problem.",
            "success_condition": "The test succeeds."
        },
        "nodes": [
            {
                "id": 1,
                "name": "First node",
                "description": "Do the first step.",
                "state": "Ready",
                "priority": 3,
                "validation": {
                    "description": "Check first step.",
                    "method": "Manual check",
                    "expected_result": "Step is correct.",
                    "result": "Pending"
                }
            }
        ],
        "dependencies": [],
        "constraints": []
    }
    )";

    const ProblemData validProblem =
        loadProblemDataFromJson(validJson);

    assert(validProblem.goal.name == "Test Goal");
    assert(validProblem.nodes.size() == 1);
    assert(validProblem.nodes[0].id == 1);
    assert(validProblem.nodes[0].priority == 3);

    // Semantically invalid JSON should be rejected
    const std::string invalidJson = R"(
    {
        "goal": {
            "name": "Invalid Goal",
            "description": "Contains duplicate node IDs.",
            "success_condition": "Should not load."
        },
        "nodes": [
            {
                "id": 1,
                "name": "Node A",
                "description": "First node.",
                "state": "Ready",
                "priority": 2,
                "validation": {
                    "description": "Check A.",
                    "method": "Manual check",
                    "expected_result": "A is valid.",
                    "result": "Pending"
                }
            },
            {
                "id": 1,
                "name": "Node B",
                "description": "Duplicate ID.",
                "state": "NotStarted",
                "priority": 2,
                "validation": {
                    "description": "Check B.",
                    "method": "Manual check",
                    "expected_result": "B is valid.",
                    "result": "Pending"
                }
            }
        ],
        "dependencies": [],
        "constraints": []
    }
    )";

    bool rejectedInvalidProblem = false;

    try
    {
        loadProblemDataFromJson(invalidJson);
    }
    catch (const std::invalid_argument&)
    {
        rejectedInvalidProblem = true;
    }

    assert(rejectedInvalidProblem);

    return 0;
}