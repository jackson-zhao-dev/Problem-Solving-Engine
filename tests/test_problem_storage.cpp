#include <cassert>
#include <cstdio>
#include <fstream>
#include <stdexcept>
#include <string>

#include "ProblemStorage.h"
#include "ProblemValidator.h"

int main()
{
    const std::string filePath =
        "test_problem_storage.json";

    ProblemData original{};

    // Goal
    original.goal.name = "Storage Test Goal";
    original.goal.description =
        "Test saving and loading.";
    original.goal.successCondition =
        "Data survives file storage.";

    // Node 1
    Node node1{};
    node1.id = 1;
    node1.name = "First Node";
    node1.description =
        "First storage test node.";
    node1.state = State::Ready;
    node1.priority = 3;

    node1.validation.description =
        "Check first node.";
    node1.validation.method =
        "Manual check";
    node1.validation.expectedResult =
        "First node is correct.";
    node1.validation.result =
        ValidationResult::Pass;

    original.nodes.push_back(node1);

    // Node 2
    Node node2{};
    node2.id = 2;
    node2.name = "Second Node";
    node2.description =
        "Second storage test node.";
    node2.state = State::NotStarted;
    node2.priority = 2;

    node2.validation.description =
        "Check second node.";
    node2.validation.method =
        "Manual check";
    node2.validation.expectedResult =
        "Second node is correct.";
    node2.validation.result =
        ValidationResult::Pending;

    original.nodes.push_back(node2);

    // Dependency
    Dependency dependency{};
    dependency.fromNode = 1;
    dependency.toNode = 2;

    original.dependencies.push_back(dependency);

    // Constraint
    Constraint constraint{};
    constraint.name = "Budget";
    constraint.type = ConstraintType::Cost;
    constraint.value = "50 CAD";
    constraint.status =
        ConstraintStatus::Satisfied;

    original.constraints.push_back(constraint);

    // Original data must be valid
    assert(
        isProblemDataValid(original)
    );

    // Save to disk
    saveProblemDataToFile(
        original,
        filePath
    );

    // Load from disk
    const ProblemData restored =
        loadProblemDataFromFile(filePath);

    // Restored data must be valid
    assert(
        isProblemDataValid(restored)
    );

    // Goal checks
    assert(
        restored.goal.name
        == original.goal.name
    );

    assert(
        restored.goal.description
        == original.goal.description
    );

    assert(
        restored.goal.successCondition
        == original.goal.successCondition
    );

    // Node checks
    assert(restored.nodes.size() == 2);

    assert(restored.nodes[0].id == 1);
    assert(restored.nodes[0].name == "First Node");
    assert(restored.nodes[0].state == State::Ready);
    assert(restored.nodes[0].priority == 3);

    assert(
        restored.nodes[0].validation.result
        == ValidationResult::Pass
    );

    assert(restored.nodes[1].id == 2);
    assert(restored.nodes[1].name == "Second Node");

    assert(
        restored.nodes[1].state
        == State::NotStarted
    );

    assert(restored.nodes[1].priority == 2);

    assert(
        restored.nodes[1].validation.result
        == ValidationResult::Pending
    );

    // Dependency checks
    assert(restored.dependencies.size() == 1);

    assert(
        restored.dependencies[0].fromNode == 1
    );

    assert(
        restored.dependencies[0].toNode == 2
    );

    // Constraint checks
    assert(restored.constraints.size() == 1);

    assert(
        restored.constraints[0].name
        == "Budget"
    );

    assert(
        restored.constraints[0].type
        == ConstraintType::Cost
    );

    assert(
        restored.constraints[0].value
        == "50 CAD"
    );

    assert(
        restored.constraints[0].status
        == ConstraintStatus::Satisfied
    );

    // Clean up successful round-trip file
    std::remove(filePath.c_str());

    // Invalid ProblemData must not be saved
    ProblemData invalidProblem = original;

    invalidProblem.nodes[1].id = 1;

    assert(
        !isProblemDataValid(invalidProblem)
    );

    bool rejectedInvalidSave = false;

    try
    {
        saveProblemDataToFile(
            invalidProblem,
            "invalid_problem.json"
        );
    }
    catch (const std::invalid_argument&)
    {
        rejectedInvalidSave = true;
    }

    assert(rejectedInvalidSave);

    std::remove("invalid_problem.json");

    // Missing file must be rejected
    const std::string missingFilePath =
        "missing_problem_file.json";

    std::remove(missingFilePath.c_str());

    bool rejectedMissingFile = false;

    try
    {
        loadProblemDataFromFile(
            missingFilePath
        );
    }
    catch (const std::runtime_error&)
    {
        rejectedMissingFile = true;
    }

    assert(rejectedMissingFile);

    // Malformed JSON file must be rejected
    const std::string malformedFilePath =
        "malformed_problem.json";

    {
        std::ofstream malformedFile(
            malformedFilePath
        );

        malformedFile <<
            "{ this is not valid json }";
    }

    bool rejectedMalformedFile = false;

    try
    {
        loadProblemDataFromFile(
            malformedFilePath
        );
    }
    catch (const std::exception&)
    {
        rejectedMalformedFile = true;
    }

    assert(rejectedMalformedFile);

    std::remove(malformedFilePath.c_str());

    return 0;
}