# Problem-Solving Engine
## Project Design Document · Version 1.0

**Document Status:** Design Complete  
**Date:** 2026-08-16  
**Implementation Language:** C++  
**AI Integration:** External large-language-model API

## 1. Executive Summary
The Problem-Solving Engine is a small, interpretable, and extensible C++ system. Its goal is not to “know every field of knowledge,” but to convert different real-world problems into a unified problem structure and then use deterministic program logic to perform dependency analysis, state updates, next-step selection, and failure diagnosis.

The system uses a two-layer capability model: AI is responsible for understanding open-ended natural language and domain context and for normalizing the input, while the C++ engine is responsible for the actual structured computation and decision-making. The core design principle is:

> **AI proposes structure; Engine makes decisions.**

Version 1.0 does not aim to become a large platform, a complex graphical application, or a machine-learning system. Instead, it aims to be a minimal system that can run end to end, explain every major decision it makes, and be tested across several different engineering cases.

## 2. Project Origin and Motivation
The idea for this project grew out of a long-term effort to break down the processes of learning, problem solving, and review. The original question was not “How can I build a piece of software?” Instead, it was: when a person faces unfamiliar knowledge or a complex task, how do they move from confusion to structure, how do they execute a plan, how do they identify where a failure occurred, and how do they use feedback to decide what to change next?

Once the learning-specific context is removed, a more general loop appears:

**Understand → Decompose → Execute → Validate → Diagnose → Improve → Repeat**

This project attempts to formalize that underlying logic as a general computational framework and test whether it can transfer to different types of engineering problems, such as a rubber-band airplane, a robot, a small vehicle, a circuit model, or a software task.

## 3. Problem Definition
Real-world problems are highly open-ended. A user might enter goals as different as “build a rubber-band airplane,” “design an obstacle-avoiding car,” or “construct a circuit model.” A small C++ program cannot realistically contain all of the domain knowledge required for every possible project.

Therefore, the core engine is not expected to understand the domain semantics of “airplanes,” “circuits,” or “robots.” Instead, an AI layer first converts any open-ended problem into a fixed structured representation. The engine only operates on that structured problem representation.

**Core research question:** Can complex problems from different domains be represented using six common object types—Goal, Node, Dependency, Constraint, State, and Validation—and can deterministic graph and state logic then be used to select the next action and diagnose failures?

## 4. Project Goals and Non-Goals
### 4.1 Goals
- Normalize open-ended real-world problems into a common problem structure.
- Represent complex tasks using a Graph and Dependencies.
- Automatically determine Ready and Blocked nodes from State and Constraint information.
- Use a custom Next-Step Algorithm to select a recommended task.
- Perform Failure Diagnosis through the dependency graph when Validation fails.
- Allow AI to handle natural language and domain semantics without allowing AI to replace the core decision algorithms.
- Preserve clear iteration records so the project can later be tested, explained, and extended.

### 4.2 Non-Goals for Version 1.0
- Do not train a custom large-language model or machine-learning model.
- Do not build a large cloud platform, multi-user collaboration system, or account system.
- Do not pursue a complex GUI; Version 1.0 may use a console interface or lightweight interface.
- Do not automatically purchase parts or directly control a robot or vehicle.
- Do not allow AI to directly choose the Next Step or Root Suspect.
- Do not attempt to become a professional knowledge base for every engineering field.

## 5. Core Design Principle
> **AI proposes structure; Engine makes decisions.**

This principle prevents the project from becoming a thin wrapper that simply calls an AI model and displays its response. AI may propose nodes, dependencies, constraints, and validation methods, but all core state decisions, graph checks, task recommendations, and failure localization are performed by the C++ engine.

## 6. High-Level Architecture
1. **User Input:** The user describes a goal, available resources, constraints, or a current failure in natural language.
2. **AI Normalization Layer:** The open-ended input is converted into the six fixed object types.
3. **Input Validation:** The C++ engine checks whether nodes, dependencies, states, and required fields are valid.
4. **Problem Graph Engine:** The engine builds the dependency graph and updates node states.
5. **Decision Layer:** The engine runs either the Next-Step Algorithm or the Failure Diagnosis algorithm.
6. **Structured Result:** The engine outputs information such as the recommended node, the root suspect, and blocked nodes.
7. **AI Explanation Layer:** AI uses the original project context to convert the structured result into an explanation that is easy for the user to understand.

## 7. Common Problem Representation
Version 1.0 recognizes only six core object types. AI must convert different real-world problems into these six objects.

| Object | Purpose | Version 1.0 Fields |
|---|---|---|
| Goal | Defines what the overall system is ultimately trying to achieve. | `name`, `description`, `success_condition` |
| Node | Represents a task, module, or subproblem that can be handled independently. | `id`, `name`, `description`, `state`, `priority`, `validation` |
| Dependency | Indicates that one node depends on another node. | `from_node`, `to_node` |
| Constraint | Represents a restriction that must not be violated during problem solving. | `name`, `type`, `value`, `status` |
| State | Represents the current stage of a node. | `Not Started`, `Ready`, `In Progress`, `Completed`, `Failed`, `Blocked` |
| Validation | Defines what must be true for a node to count as genuinely completed. | `description`, `method`, `expected_result`, `result` |

## 8. State Machine
Node states are not entered entirely by the user. In Version 1.0, some state transitions are calculated automatically by the C++ engine.

- **Ready:** All required Dependencies have been completed and all required Constraints are satisfied.
- **In Progress:** The user has formally started working on the node.
- **Completed:** The node has been executed and its Validation result is Pass.
- **Failed:** The node has been executed, but its Validation result is Fail.
- **Blocked:** A required upstream dependency is incomplete or failed, or a critical constraint is violated.

`Not Started → Ready → In Progress → Completed / Failed`

When a dependency or constraint becomes invalid: `Not Started / Ready → Blocked`. When the blocking problem is resolved: `Blocked → Ready`.

## 9. Dependency Graph
A Dependency is represented as a directed edge: `from_node → to_node`. This means that `to_node` must wait until `from_node` satisfies the required conditions before it can become executable.

Version 1.0 does not introduce categories such as strong dependencies, weak dependencies, or probabilistic dependencies. The core goal is to keep the graph simple, interpretable, and easy to implement. The engine must detect structural errors such as invalid references, self-dependencies, and dependency cycles.

## 10. Next-Step Algorithm
When multiple nodes are Ready at the same time, the system must select a Recommended Next Step using a deterministic algorithm instead of handing the decision directly to AI.

### 10.1 Candidate Filtering
- Only nodes whose State is Ready are considered.
- If a required Constraint is violated, the node becomes Blocked and is excluded from scoring.

### 10.2 Scoring Rule
`NextStepScore = 2 × Priority + UnlockValue`

Priority uses a 1–3 scale and represents the importance of the node itself. AI may propose an initial priority, but the user can modify it.

UnlockValue is calculated by the C++ engine from the Dependency Graph:
- Directly unlocks 0 downstream nodes → 1 point
- Directly unlocks 1 downstream node → 2 points
- Directly unlocks 2 or more downstream nodes → 3 points

### 10.3 Tie-Break Rules
1. If total scores are equal, choose the node with the higher UnlockValue.
2. If still equal, choose the node with the higher Priority.
3. If still completely equal, choose the node with the smaller ID so that program behavior remains stable and predictable.

The algorithm is intentionally simple. Version 1.0 does not introduce time risk, probabilistic optimization, or complex weighting simply to make the system appear more advanced.

## 11. Failure Diagnosis
When Validation fails, the current failed node is not necessarily the earliest source of the problem. Failure Diagnosis traces upstream through the Dependency Graph to find the earliest node with an unreliable result.

1. The node whose Validation fails is marked Failed and becomes the current Diagnostic Target.
2. The engine checks the node's direct upstream dependencies. If an upstream node is incomplete, failed, or has unreliable validation, the engine continues tracing upward along that branch.
3. The earliest upstream node with an unreliable validation result is marked as the Root Suspect. If all upstream nodes have reliably passed validation, the currently failed node itself becomes the Root Suspect.
4. Downstream nodes affected by the failure become Blocked. After the root problem is repaired and passes Validation again, the relevant downstream node states are recalculated and may return to Ready or require revalidation.

Version 1.0 identifies the earliest structurally unreliable point. It does not claim to automatically diagnose every real mechanical, electrical, or software fault. AI may use the structured diagnosis to provide domain-specific explanations and candidate checks.

## 12. AI Integration Contract
### 12.1 AI Responsibilities
- Understand the user's open-ended natural-language input.
- Identify likely modules, tasks, restrictions, and validation methods within the specific domain.
- Convert the real-world problem into the six fixed object types.
- After the engine produces a structured result, explain that result to the user in the context of the original problem.

### 12.2 Engine Responsibilities
- Check whether AI output conforms to the data protocol.
- Build and maintain the Dependency Graph.
- Calculate State values and determine Ready and Blocked nodes.
- Calculate UnlockValue and run the Next-Step Algorithm.
- Run Failure Diagnosis.
- Store structured state and iteration results.

### 12.3 Interface Validation
- Node IDs must be unique.
- Every node referenced by a Dependency must exist.
- Self-dependencies such as `A → A` are forbidden.
- Obvious dependency cycles must be detected.
- Priority must remain inside the allowed range.
- State must be one of the six allowed states.
- If required fields are missing, the input must be rejected and regenerated or corrected.

## 13. Planned Structured Data Format
During implementation, AI should return JSON rather than free-form text. The following structure describes the intended protocol direction. Exact field names may be adjusted during implementation to match the C++ design.

```json
{
  "goal": {
    "name": "...",
    "description": "...",
    "success_condition": "..."
  },
  "nodes": [
    {
      "id": 1,
      "name": "...",
      "description": "...",
      "state": "Not Started",
      "priority": 2,
      "validation": {
        "description": "...",
        "method": "...",
        "expected_result": "...",
        "result": "Pending"
      }
    }
  ],
  "dependencies": [
    {"from_node": 1, "to_node": 2}
  ],
  "constraints": [
    {
      "name": "...",
      "type": "Cost | Time | Resource | Physical | Performance | Other",
      "value": "...",
      "status": "Satisfied | Violated | Unknown"
    }
  ]
}
```

## 14. End-to-End Program Flow
1. The user enters an open-ended real-world goal, such as “design a rubber-band airplane that can fly stably.”
2. AI analyzes the domain semantics and outputs structured data for the Goal, Nodes, Dependencies, Constraints, State, and Validation.
3. The C++ engine performs interface validation. Invalid input is rejected or sent back for regeneration.
4. The engine builds the Dependency Graph and calculates the current state of each node.
5. The engine finds all Ready nodes and runs the Next-Step Algorithm.
6. The user performs the Recommended Next Step.
7. The user submits the Validation Result: Pass or Fail.
8. If the result is Pass, the node becomes Completed and the engine recalculates downstream node states.
9. If the result is Fail, the node becomes Failed and the engine runs Failure Diagnosis, producing a Root Suspect and the affected Blocked nodes.
10. AI explains the engine's structured conclusion in the context of the specific project and explains why that diagnostic direction is recommended.
11. The user modifies the project and validates again. The system repeats the loop until the Goal's Success Condition is satisfied.

## 15. MVP Scope
Version 1.0 only needs to form a complete working loop:

- Accept a natural-language goal from the user.
- Call an external AI service to convert the input into the standard structure.
- Read and validate the structured result.
- Build the dependency graph.
- Automatically update node states.
- Run the Next-Step Algorithm.
- Accept user validation results.
- Run Failure Diagnosis.
- Output structured results and optionally call AI again to generate a natural-language explanation.

If this loop can operate reliably across multiple different types of cases, Version 1.0 will be considered complete.

## 16. Planned Evaluation
This design document does not claim in advance that the project is effective. After implementation, the system must be tested with real cases to determine whether it can handle problems from different domains. At least three meaningfully different case types are planned, for example:

- A rubber-band airplane or another simple mechanical system.
- An autonomous car or robotics task.
- A simple circuit model or software project.

Evaluation will focus on whether AI can consistently produce valid structures; whether the same core engine can process different cases without modifying its central algorithms; whether the Next-Step Algorithm produces reasonable and explainable recommendations; whether Failure Diagnosis correctly identifies the earliest structurally unreliable node; and which kinds of input cause the model to fail or require human correction.

The final Technical Project Report will replace the planned statements in this section with actual implementation results, failure cases, and iteration records.

## 17. Security and API Handling
- API keys for external AI services must never be hard-coded into source code or committed to GitHub.
- Sensitive configuration should be stored in environment variables or local configuration files and excluded from version control.
- AI output must be treated as untrusted external input and structurally validated before entering the core engine.

## 18. Expected Repository Structure
```text
Problem-Solving-Engine/
├── README.md
├── src/
├── include/
├── examples/
├── tests/
├── docs/
│   └── project-design-document.md
└── report/
    └── technical-report.pdf   # project-completion stage
```

## 19. Known Design Limitations
- The six object types are a human-designed abstraction and cannot be guaranteed to represent every real-world problem.
- AI may incorrectly decompose open-ended input, omit important elements, or over-decompose a problem. Interface validation and user correction are therefore necessary.
- Priority still contains a subjective component; the Version 1.0 scoring formula is not a theoretically optimal scheduling algorithm.
- Failure Diagnosis is based on dependencies and validation states and can only identify structurally suspicious sources. It cannot replace diagnosis by a domain expert.
- In Version 1.0, Constraints primarily determine whether execution may continue. The engine does not automatically reason about every physical, budgetary, or safety constraint.

## 20. Future Extensions
The following directions are outside the scope of Version 1.0 and should only be considered after the first version is fully operational:

- Add richer Constraint types and time/risk scheduling.
- Use historical iteration data to improve priority selection or failure prediction.
- Add a visual Dependency Graph interface.
- Allow users to manually edit AI-generated structures.
- Add an adapter layer for multiple AI providers.
- Connect the engine to a real robot, vehicle, or other physical system so sensor test results can automatically update Validation.

## 21. Design Completion Criteria
As of the Version 1.0 design document, the project has completed the following design decisions: project purpose, system boundaries, division of responsibilities between AI and C++, the six core object types, the state machine, the dependency graph, the Next-Step Algorithm, the Failure Diagnosis algorithm, the AI integration contract, the MVP scope, and the complete program flow.

The design stage can therefore be considered complete. The next phase is Implementation and Evaluation. Unless coding or testing exposes a real design flaw, the scope of Version 1.0 should not be expanded proactively.

## 22. Final Design Statement
**The Problem-Solving Engine does not attempt to make a program understand the entire world. It attempts to identify the common structure behind different complex problems: goals, components, dependencies, constraints, states, and validation. AI translates the open world into that structure; the C++ engine performs deterministic computation, decision-making, and failure tracing inside the structure. The value of the project comes not from the number of features, but from turning a long-developed problem-solving approach into a computational system that can be implemented, tested, and explained.**
