#include "JsonParser.h"

#include <stdexcept>

#include <nlohmann/json.hpp>

namespace
{
    State parseState(const std::string& value)
    {
        if (value == "NotStarted")
        {
            return State::NotStarted;
        }

        if (value == "Ready")
        {
            return State::Ready;
        }

        if (value == "InProgress")
        {
            return State::InProgress;
        }

        if (value == "Completed")
        {
            return State::Completed;
        }

        if (value == "Failed")
        {
            return State::Failed;
        }

        if (value == "Blocked")
        {
            return State::Blocked;
        }

        throw std::invalid_argument(
            "Invalid State value: " + value
        );
    }

    ValidationResult parseValidationResult(
        const std::string& value
    )
    {
        if (value == "Pending")
        {
            return ValidationResult::Pending;
        }

        if (value == "Pass")
        {
            return ValidationResult::Pass;
        }

        if (value == "Fail")
        {
            return ValidationResult::Fail;
        }

        throw std::invalid_argument(
            "Invalid ValidationResult value: " + value
        );
    }

    ConstraintType parseConstraintType(
        const std::string& value
    )
    {
        if (value == "Cost")
        {
            return ConstraintType::Cost;
        }

        if (value == "Time")
        {
            return ConstraintType::Time;
        }

        if (value == "Resource")
        {
            return ConstraintType::Resource;
        }

        if (value == "Physical")
        {
            return ConstraintType::Physical;
        }

        if (value == "Performance")
        {
            return ConstraintType::Performance;
        }

        if (value == "Other")
        {
            return ConstraintType::Other;
        }

        throw std::invalid_argument(
            "Invalid ConstraintType value: " + value
        );
    }

    ConstraintStatus parseConstraintStatus(
        const std::string& value
    )
    {
        if (value == "Unknown")
        {
            return ConstraintStatus::Unknown;
        }

        if (value == "Satisfied")
        {
            return ConstraintStatus::Satisfied;
        }

        if (value == "Violated")
        {
            return ConstraintStatus::Violated;
        }

        throw std::invalid_argument(
            "Invalid ConstraintStatus value: " + value
        );
    }
}

ProblemData parseProblemDataFromJson(
    const std::string& jsonText
)
{
    using nlohmann::json;

    const json root = json::parse(jsonText);

    ProblemData problemData{};

    const json& goalJson = root.at("goal");

    problemData.goal.name =
        goalJson.at("name").get<std::string>();

    problemData.goal.description =
        goalJson.at("description").get<std::string>();

    problemData.goal.successCondition =
        goalJson.at("success_condition").get<std::string>();

    for (const json& nodeJson : root.at("nodes"))
    {
        Node node{};

        node.id =
            nodeJson.at("id").get<int>();

        node.name =
            nodeJson.at("name").get<std::string>();

        node.description =
            nodeJson.at("description").get<std::string>();

        node.state =
            parseState(
                nodeJson.at("state").get<std::string>()
            );

        node.priority =
            nodeJson.at("priority").get<int>();

        const json& validationJson =
            nodeJson.at("validation");

        node.validation.description =
            validationJson.at("description")
                .get<std::string>();

        node.validation.method =
            validationJson.at("method")
                .get<std::string>();

        node.validation.expectedResult =
            validationJson.at("expected_result")
                .get<std::string>();

        node.validation.result =
            parseValidationResult(
                validationJson.at("result")
                    .get<std::string>()
            );

        problemData.nodes.push_back(node);
    }

    for (const json& dependencyJson :
         root.at("dependencies"))
    {
        Dependency dependency{};

        dependency.fromNode =
            dependencyJson.at("from_node").get<int>();

        dependency.toNode =
            dependencyJson.at("to_node").get<int>();

        problemData.dependencies.push_back(dependency);
    }

    for (const json& constraintJson :
         root.at("constraints"))
    {
        Constraint constraint{};

        constraint.name =
            constraintJson.at("name").get<std::string>();

        constraint.type =
            parseConstraintType(
                constraintJson.at("type")
                    .get<std::string>()
            );

        constraint.value =
            constraintJson.at("value").get<std::string>();

        constraint.status =
            parseConstraintStatus(
                constraintJson.at("status")
                    .get<std::string>()
            );

        problemData.constraints.push_back(constraint);
    }

    return problemData;
}