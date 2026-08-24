#include <cassert>
#include <vector>

#include "Engine.h"

int main()
{
    // State Transition tests
    assert(isValidStateTransition(State::NotStarted, State::Ready));
    assert(isValidStateTransition(State::NotStarted, State::Blocked));

    assert(isValidStateTransition(State::Ready, State::InProgress));
    assert(isValidStateTransition(State::Ready, State::Blocked));

    assert(isValidStateTransition(State::InProgress, State::Completed));
    assert(isValidStateTransition(State::InProgress, State::Failed));

    assert(isValidStateTransition(State::Blocked, State::Ready));

    assert(!isValidStateTransition(State::NotStarted, State::Completed));
    assert(!isValidStateTransition(State::Ready, State::Completed));
    assert(!isValidStateTransition(State::Completed, State::Ready));
    assert(!isValidStateTransition(State::Failed, State::Ready));

    // Ready-state Evaluation tests
    Node node1{};
    node1.id = 1;
    node1.state = State::Completed;

    Node node2{};
    node2.id = 2;
    node2.state = State::NotStarted;

    std::vector<Node> nodes = {node1, node2};

    Dependency dependency{};
    dependency.fromNode = 1;
    dependency.toNode = 2;

    std::vector<Dependency> dependencies = {dependency};

    Constraint satisfiedConstraint{};
    satisfiedConstraint.status = ConstraintStatus::Satisfied;

    std::vector<Constraint> satisfiedConstraints =
    {
        satisfiedConstraint
    };

    assert(
        evaluateReadyState(
            node2,
            nodes,
            dependencies,
            satisfiedConstraints
        ) == State::Ready
    );

    nodes[0].state = State::InProgress;

    assert(
        evaluateReadyState(
            node2,
            nodes,
            dependencies,
            satisfiedConstraints
        ) == State::Blocked
    );

    nodes[0].state = State::Completed;

    Constraint violatedConstraint{};
    violatedConstraint.status = ConstraintStatus::Violated;

    std::vector<Constraint> violatedConstraints =
    {
        violatedConstraint
    };

    assert(
        evaluateReadyState(
            node2,
            nodes,
            dependencies,
            violatedConstraints
        ) == State::Blocked
    );

    Node node3{};
    node3.id = 3;
    node3.state = State::NotStarted;

    std::vector<Dependency> noDependencies;

    assert(
        evaluateReadyState(
            node3,
            nodes,
            noDependencies,
            satisfiedConstraints
        ) == State::Ready
    );

    // Dependency Validation tests
    Node dependencyNode1{};
    dependencyNode1.id = 1;

    Node dependencyNode2{};
    dependencyNode2.id = 2;

    Node dependencyNode3{};
    dependencyNode3.id = 3;

    std::vector<Node> dependencyNodes =
    {
        dependencyNode1,
        dependencyNode2,
        dependencyNode3
    };

    Dependency validDependency1{};
    validDependency1.fromNode = 1;
    validDependency1.toNode = 2;

    Dependency validDependency2{};
    validDependency2.fromNode = 2;
    validDependency2.toNode = 3;

    std::vector<Dependency> validDependencies =
    {
        validDependency1,
        validDependency2
    };

    assert(
        areDependenciesValid(
            dependencyNodes,
            validDependencies
        )
    );

    Dependency missingNodeDependency{};
    missingNodeDependency.fromNode = 1;
    missingNodeDependency.toNode = 99;

    std::vector<Dependency> invalidReferenceDependencies =
    {
        missingNodeDependency
    };

    assert(
        !areDependenciesValid(
            dependencyNodes,
            invalidReferenceDependencies
        )
    );

    Dependency selfDependency{};
    selfDependency.fromNode = 2;
    selfDependency.toNode = 2;

    std::vector<Dependency> selfDependencies =
    {
        selfDependency
    };

    assert(
        !areDependenciesValid(
            dependencyNodes,
            selfDependencies
        )
    );

    // Cycle Detection tests
    assert(
        !hasDependencyCycle(
            dependencyNodes,
            validDependencies
        )
    );

    Dependency cycleDependency1{};
    cycleDependency1.fromNode = 1;
    cycleDependency1.toNode = 2;

    Dependency cycleDependency2{};
    cycleDependency2.fromNode = 2;
    cycleDependency2.toNode = 3;

    Dependency cycleDependency3{};
    cycleDependency3.fromNode = 3;
    cycleDependency3.toNode = 1;

    std::vector<Dependency> cyclicDependencies =
    {
        cycleDependency1,
        cycleDependency2,
        cycleDependency3
    };

    assert(
        hasDependencyCycle(
            dependencyNodes,
            cyclicDependencies
        )
    );

    // Constraint Checking tests
    std::vector<Constraint> emptyConstraints;

    assert(
        evaluateConstraintStatus(emptyConstraints)
        == ConstraintStatus::Satisfied
    );

    Constraint constraintSatisfied1{};
    constraintSatisfied1.status = ConstraintStatus::Satisfied;

    Constraint constraintSatisfied2{};
    constraintSatisfied2.status = ConstraintStatus::Satisfied;

    std::vector<Constraint> allSatisfiedConstraints =
    {
        constraintSatisfied1,
        constraintSatisfied2
    };

    assert(
        evaluateConstraintStatus(allSatisfiedConstraints)
        == ConstraintStatus::Satisfied
    );

    Constraint constraintUnknown{};
    constraintUnknown.status = ConstraintStatus::Unknown;

    std::vector<Constraint> unknownConstraints =
    {
        constraintSatisfied1,
        constraintUnknown
    };

    assert(
        evaluateConstraintStatus(unknownConstraints)
        == ConstraintStatus::Unknown
    );

    Constraint constraintViolated{};
    constraintViolated.status = ConstraintStatus::Violated;

    std::vector<Constraint> mixedConstraints =
    {
        constraintUnknown,
        constraintViolated
    };

    assert(
        evaluateConstraintStatus(mixedConstraints)
        == ConstraintStatus::Violated
    );

    // Next-Step Algorithm tests
    Node nextNode1{};
    nextNode1.id = 1;
    nextNode1.state = State::Ready;
    nextNode1.priority = 2;

    Node nextNode2{};
    nextNode2.id = 2;
    nextNode2.state = State::Ready;
    nextNode2.priority = 3;

    Node nextNode3{};
    nextNode3.id = 3;
    nextNode3.state = State::Blocked;
    nextNode3.priority = 3;

    Node nextNode4{};
    nextNode4.id = 4;
    nextNode4.state = State::NotStarted;
    nextNode4.priority = 1;

    Node nextNode5{};
    nextNode5.id = 5;
    nextNode5.state = State::NotStarted;
    nextNode5.priority = 1;

    Dependency nextDependency1{};
    nextDependency1.fromNode = 1;
    nextDependency1.toNode = 4;

    Dependency nextDependency2{};
    nextDependency2.fromNode = 1;
    nextDependency2.toNode = 5;

    Dependency nextDependency3{};
    nextDependency3.fromNode = 2;
    nextDependency3.toNode = 4;

    std::vector<Dependency> nextDependencies =
    {
        nextDependency1,
        nextDependency2,
        nextDependency3
    };

    assert(
        calculateUnlockValue(
            nextNode1.id,
            nextDependencies
        ) == 3
    );

    assert(
        calculateUnlockValue(
            nextNode2.id,
            nextDependencies
        ) == 2
    );

    assert(
        calculateUnlockValue(
            nextNode3.id,
            nextDependencies
        ) == 1
    );

    assert(
        calculateNextStepScore(
            nextNode1,
            nextDependencies
        ) == 7
    );

    assert(
        calculateNextStepScore(
            nextNode2,
            nextDependencies
        ) == 8
    );

    std::vector<Node> nextStepNodes =
    {
        nextNode1,
        nextNode2,
        nextNode3,
        nextNode4,
        nextNode5
    };

    const Node* selectedNode =
        selectNextStep(
            nextStepNodes,
            nextDependencies
        );

    assert(selectedNode != nullptr);
    assert(selectedNode->id == 2);

    // Tie-break by UnlockValue
    Node tieUnlockNode1{};
    tieUnlockNode1.id = 10;
    tieUnlockNode1.state = State::Ready;
    tieUnlockNode1.priority = 2;

    Node tieUnlockNode2{};
    tieUnlockNode2.id = 11;
    tieUnlockNode2.state = State::Ready;
    tieUnlockNode2.priority = 3;

    Node tieUnlockTarget1{};
    tieUnlockTarget1.id = 12;

    Node tieUnlockTarget2{};
    tieUnlockTarget2.id = 13;

    Dependency tieUnlockDependency1{};
    tieUnlockDependency1.fromNode = 10;
    tieUnlockDependency1.toNode = 12;

    Dependency tieUnlockDependency2{};
    tieUnlockDependency2.fromNode = 10;
    tieUnlockDependency2.toNode = 13;

    std::vector<Dependency> tieUnlockDependencies =
    {
        tieUnlockDependency1,
        tieUnlockDependency2
    };

    std::vector<Node> tieUnlockNodes =
    {
        tieUnlockNode1,
        tieUnlockNode2,
        tieUnlockTarget1,
        tieUnlockTarget2
    };

    const Node* tieUnlockSelected =
        selectNextStep(
            tieUnlockNodes,
            tieUnlockDependencies
        );

    assert(tieUnlockSelected != nullptr);
    assert(tieUnlockSelected->id == 10);

    // Tie-break by smaller ID
    Node tieIdNode1{};
    tieIdNode1.id = 20;
    tieIdNode1.state = State::Ready;
    tieIdNode1.priority = 2;

    Node tieIdNode2{};
    tieIdNode2.id = 19;
    tieIdNode2.state = State::Ready;
    tieIdNode2.priority = 2;

    std::vector<Node> tieIdNodes =
    {
        tieIdNode1,
        tieIdNode2
    };

    std::vector<Dependency> tieIdDependencies;

    const Node* tieIdSelected =
        selectNextStep(
            tieIdNodes,
            tieIdDependencies
        );

    assert(tieIdSelected != nullptr);
    assert(tieIdSelected->id == 19);

    // No Ready node
    Node blockedNode{};
    blockedNode.id = 30;
    blockedNode.state = State::Blocked;
    blockedNode.priority = 3;

    Node completedNode{};
    completedNode.id = 31;
    completedNode.state = State::Completed;
    completedNode.priority = 3;

    std::vector<Node> noReadyNodes =
    {
        blockedNode,
        completedNode
    };

    const Node* noReadySelected =
        selectNextStep(
            noReadyNodes,
            noDependencies
        );

    assert(noReadySelected == nullptr);

    // Failure Diagnosis tests

    // Reliable validation
    Node reliableNode{};
    reliableNode.id = 100;
    reliableNode.state = State::Completed;
    reliableNode.validation.result = ValidationResult::Pass;

    assert(
        isNodeReliablyValidated(reliableNode)
    );

    Node pendingValidationNode{};
    pendingValidationNode.id = 101;
    pendingValidationNode.state = State::Completed;
    pendingValidationNode.validation.result =
        ValidationResult::Pending;

    assert(
        !isNodeReliablyValidated(
            pendingValidationNode
        )
    );

    Node failedValidationNode{};
    failedValidationNode.id = 102;
    failedValidationNode.state = State::Failed;
    failedValidationNode.validation.result =
        ValidationResult::Fail;

    assert(
        !isNodeReliablyValidated(
            failedValidationNode
        )
    );

    // All upstream nodes are reliable:
    // failed node itself is the Root Suspect
    Node reliableUpstream{};
    reliableUpstream.id = 110;
    reliableUpstream.state = State::Completed;
    reliableUpstream.validation.result =
        ValidationResult::Pass;

    Node currentFailedNode{};
    currentFailedNode.id = 111;
    currentFailedNode.state = State::Failed;
    currentFailedNode.validation.result =
        ValidationResult::Fail;

    std::vector<Node> reliableUpstreamNodes =
    {
        reliableUpstream,
        currentFailedNode
    };

    Dependency reliableUpstreamDependency{};
    reliableUpstreamDependency.fromNode = 110;
    reliableUpstreamDependency.toNode = 111;

    std::vector<Dependency> reliableUpstreamDependencies =
    {
        reliableUpstreamDependency
    };

    const Node* currentRootSuspect =
        findRootSuspect(
            currentFailedNode,
            reliableUpstreamNodes,
            reliableUpstreamDependencies
        );

    assert(currentRootSuspect != nullptr);
    assert(currentRootSuspect->id == 111);

    // Trace through unreliable upstream nodes
    Node earliestUnreliableNode{};
    earliestUnreliableNode.id = 120;
    earliestUnreliableNode.state = State::Completed;
    earliestUnreliableNode.validation.result =
        ValidationResult::Pending;

    Node middleUnreliableNode{};
    middleUnreliableNode.id = 121;
    middleUnreliableNode.state = State::Completed;
    middleUnreliableNode.validation.result =
        ValidationResult::Pending;

    Node finalFailedNode{};
    finalFailedNode.id = 122;
    finalFailedNode.state = State::Failed;
    finalFailedNode.validation.result =
        ValidationResult::Fail;

    std::vector<Node> diagnosisChainNodes =
    {
        earliestUnreliableNode,
        middleUnreliableNode,
        finalFailedNode
    };

    Dependency diagnosisDependency1{};
    diagnosisDependency1.fromNode = 120;
    diagnosisDependency1.toNode = 121;

    Dependency diagnosisDependency2{};
    diagnosisDependency2.fromNode = 121;
    diagnosisDependency2.toNode = 122;

    std::vector<Dependency> diagnosisChainDependencies =
    {
        diagnosisDependency1,
        diagnosisDependency2
    };

    const Node* chainRootSuspect =
        findRootSuspect(
            finalFailedNode,
            diagnosisChainNodes,
            diagnosisChainDependencies
        );

    assert(chainRootSuspect != nullptr);
    assert(chainRootSuspect->id == 120);

    // Equal-depth unreliable branches:
    // smaller ID is selected deterministically
    Node branchUnreliableNode1{};
    branchUnreliableNode1.id = 130;
    branchUnreliableNode1.state = State::Completed;
    branchUnreliableNode1.validation.result =
        ValidationResult::Pending;

    Node branchUnreliableNode2{};
    branchUnreliableNode2.id = 131;
    branchUnreliableNode2.state = State::Completed;
    branchUnreliableNode2.validation.result =
        ValidationResult::Pending;

    Node branchFailedNode{};
    branchFailedNode.id = 132;
    branchFailedNode.state = State::Failed;
    branchFailedNode.validation.result =
        ValidationResult::Fail;

    std::vector<Node> branchDiagnosisNodes =
    {
        branchUnreliableNode1,
        branchUnreliableNode2,
        branchFailedNode
    };

    Dependency branchDependency1{};
    branchDependency1.fromNode = 131;
    branchDependency1.toNode = 132;

    Dependency branchDependency2{};
    branchDependency2.fromNode = 130;
    branchDependency2.toNode = 132;

    std::vector<Dependency> branchDiagnosisDependencies =
    {
        branchDependency1,
        branchDependency2
    };

    const Node* branchRootSuspect =
        findRootSuspect(
            branchFailedNode,
            branchDiagnosisNodes,
            branchDiagnosisDependencies
        );

    assert(branchRootSuspect != nullptr);
    assert(branchRootSuspect->id == 130);

    // Affected downstream nodes
    Dependency affectedDependency1{};
    affectedDependency1.fromNode = 140;
    affectedDependency1.toNode = 141;

    Dependency affectedDependency2{};
    affectedDependency2.fromNode = 141;
    affectedDependency2.toNode = 142;

    Dependency affectedDependency3{};
    affectedDependency3.fromNode = 140;
    affectedDependency3.toNode = 143;

    Dependency affectedDependency4{};
    affectedDependency4.fromNode = 143;
    affectedDependency4.toNode = 144;

    std::vector<Dependency> affectedDependencies =
    {
        affectedDependency1,
        affectedDependency2,
        affectedDependency3,
        affectedDependency4
    };

    std::vector<int> affectedNodes =
        findAffectedDownstreamNodes(
            140,
            affectedDependencies
        );

    assert(affectedNodes.size() == 4);
    assert(affectedNodes[0] == 141);
    assert(affectedNodes[1] == 142);
    assert(affectedNodes[2] == 143);
    assert(affectedNodes[3] == 144);

    return 0;
}