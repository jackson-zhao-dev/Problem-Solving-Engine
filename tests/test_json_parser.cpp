#include <cassert>
#include <string>

#include "JsonParser.h"

int main()
{
    const std::string jsonText = R"(
    {
        "goal": {
            "name": "Build a Test Vehicle",
            "description": "Create a small vehicle.",
            "success_condition": "The vehicle completes the test route."
        },

        "nodes": [
            {
                "id": 1,
                "name": "Build chassis",
                "description": "Construct the frame.",
                "state": "Ready",
                "priority": 3,
                "validation": {
                    "description": "Check stability.",
                    "method": "Manual inspection",
                    "expected_result": "Frame remains stable.",
                    "result": "Pending"
                }
            },
            {
                "id": 2,
                "name": "Install motor",
                "description": "Attach the motor.",
                "state": "NotStarted",
                "priority": 2,
                "validation": {
                    "description": "Check motor.",
                    "method": "Power test",
                    "expected_result": "Motor rotates.",
                    "result": "Pass"
                }
            }
        ],

        "dependencies": [
            {
                "from_node": 1,
                "to_node": 2
            }
        ],

        "constraints": [
            {
                "name": "Budget limit",
                "type": "Cost",
                "value": "50 CAD",
                "status": "Satisfied"
            }
        ]
    }
    )";

    const ProblemData problemData =
        parseProblemDataFromJson(jsonText);

    // Goal
    assert(
        problemData.goal.name
        == "Build a Test Vehicle"
    );

    assert(
        problemData.goal.description
        == "Create a small vehicle."
    );

    assert(
        problemData.goal.successCondition
        == "The vehicle completes the test route."
    );

    // Nodes
    assert(problemData.nodes.size() == 2);

    assert(problemData.nodes[0].id == 1);
    assert(problemData.nodes[0].name == "Build chassis");
    assert(problemData.nodes[0].state == State::Ready);
    assert(problemData.nodes[0].priority == 3);

    assert(
        problemData.nodes[0].validation.result
        == ValidationResult::Pending
    );

    assert(problemData.nodes[1].id == 2);

    assert(
        problemData.nodes[1].state
        == State::NotStarted
    );

    assert(
        problemData.nodes[1].validation.result
        == ValidationResult::Pass
    );

    // Dependencies
    assert(problemData.dependencies.size() == 1);

    assert(
        problemData.dependencies[0].fromNode == 1
    );

    assert(
        problemData.dependencies[0].toNode == 2
    );

    // Constraints
    assert(problemData.constraints.size() == 1);

    assert(
        problemData.constraints[0].type
        == ConstraintType::Cost
    );

    assert(
        problemData.constraints[0].status
        == ConstraintStatus::Satisfied
    );

    return 0;
}