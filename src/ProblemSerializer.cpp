#include "ProblemSerializer.h"

#include <stdexcept>

#include <nlohmann/json.hpp>

namespace
{
    std::string stateToString(State state)
    {
        switch (state)
        {
            case State::NotStarted:
                return "NotStarted";

            case State::Ready:
                return "Ready";

            case State::InProgress:
                return "InProgress";

            case State::Completed:
                return "Completed";

            case State::Failed:
                return "Failed";

            case State::Blocked:
                return "Blocked";
        }

        throw std::invalid_argument(
            "Invalid State value."
        );
    }

    std::string validationResultToString(
        ValidationResult result
    )
    {
        switch (result)
        {
            case ValidationResult::Pending:
                return "Pending";

            case ValidationResult::Pass:
                return "Pass";

            case ValidationResult::Fail:
                return "Fail";
        }

        throw std::invalid_argument(
            "Invalid ValidationResult value."
        );
    }

    std::string constraintTypeToString(
        ConstraintType type
    )
    {
        switch (type)
        {
            case ConstraintType::Cost:
                return "Cost";

            case ConstraintType::Time:
                return "Time";

            case ConstraintType::Resource:
                return "Resource";

            case ConstraintType::Physical:
                return "Physical";

            case ConstraintType::Performance:
                return "Performance";

            case ConstraintType::Other:
                return "Other";
        }

        throw std::invalid_argument(
            "Invalid ConstraintType value."
        );
    }

    std::string constraintStatusToString(
        ConstraintStatus status
    )
    {
        switch (status)
        {
            case ConstraintStatus::Unknown:
                return "Unknown";

            case ConstraintStatus::Satisfied:
                return "Satisfied";

            case ConstraintStatus::Violated:
                return "Violated";
        }

        throw std::invalid_argument(
            "Invalid ConstraintStatus value."
        );
    }
}

std::string serializeProblemDataToJson(
    const ProblemData& problemData
)
{
    using nlohmann::json;

    json root;

    root["goal"] =
    {
        {"name", problemData.goal.name},
        {"description", problemData.goal.description},
        {
            "success_condition",
            problemData.goal.successCondition
        }
    };

    root["nodes"] = json::array();

    for (const Node& node : problemData.nodes)
    {
        json nodeJson;

        nodeJson["id"] = node.id;
        nodeJson["name"] = node.name;
        nodeJson["description"] = node.description;
        nodeJson["state"] = stateToString(node.state);
        nodeJson["priority"] = node.priority;

        nodeJson["validation"] =
        {
            {
                "description",
                node.validation.description
            },
            {
                "method",
                node.validation.method
            },
            {
                "expected_result",
                node.validation.expectedResult
            },
            {
                "result",
                validationResultToString(
                    node.validation.result
                )
            }
        };

        root["nodes"].push_back(nodeJson);
    }

    root["dependencies"] = json::array();

    for (const Dependency& dependency :
         problemData.dependencies)
    {
        root["dependencies"].push_back(
        {
            {"from_node", dependency.fromNode},
            {"to_node", dependency.toNode}
        });
    }

    root["constraints"] = json::array();

    for (const Constraint& constraint :
         problemData.constraints)
    {
        root["constraints"].push_back(
        {
            {"name", constraint.name},
            {
                "type",
                constraintTypeToString(
                    constraint.type
                )
            },
            {"value", constraint.value},
            {
                "status",
                constraintStatusToString(
                    constraint.status
                )
            }
        });
    }

    return root.dump(4);
}