#include <cassert>
#include <exception>
#include <string>

#include "AIProblemParser.h"

int main()
{
    const std::string validAIResponse = R"(
{
  "goal": {
    "name": "Build Rubber-Band Car",
    "description": "Build a rubber-band-powered car.",
    "success_condition": "The car travels at least 10 meters."
  },
  "nodes": [
    {
      "id": 1,
      "name": "Design chassis",
      "description": "Design the chassis and wheel layout.",
      "state": "NotStarted",
      "priority": 3,
      "validation": {
        "description": "Check the chassis design.",
        "method": "Manual inspection",
        "expected_result": "The design supports the required components.",
        "result": "Pending"
      }
    },
    {
      "id": 2,
      "name": "Build prototype",
      "description": "Build the first working prototype.",
      "state": "NotStarted",
      "priority": 3,
      "validation": {
        "description": "Check that the prototype can move.",
        "method": "Short movement test",
        "expected_result": "The prototype moves under rubber-band power.",
        "result": "Pending"
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
      "name": "Minimum distance",
      "type": "Performance",
      "value": "At least 10 meters",
      "status": "Unknown"
    }
  ]
}
)";

    const ProblemData problemData =
        parseAIProblemResponse(
            validAIResponse
        );

    assert(
        problemData.goal.name
        == "Build Rubber-Band Car"
    );

    assert(
        problemData.goal.successCondition
        == "The car travels at least 10 meters."
    );

    assert(
        problemData.nodes.size() == 2
    );

    assert(
        problemData.nodes[0].id == 1
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
        problemData.nodes[1].id == 2
    );

    assert(
        problemData.dependencies.size() == 1
    );

    assert(
        problemData.dependencies[0].fromNode == 1
    );

    assert(
        problemData.dependencies[0].toNode == 2
    );

    assert(
        problemData.constraints.size() == 1
    );

    assert(
        problemData.constraints[0].type
        == ConstraintType::Performance
    );

    assert(
        problemData.constraints[0].status
        == ConstraintStatus::Unknown
    );

    bool invalidJsonRejected = false;

    try
    {
        parseAIProblemResponse(
            "{ invalid json"
        );
    }
    catch (const std::exception&)
    {
        invalidJsonRejected = true;
    }

    assert(invalidJsonRejected);

    return 0;
}