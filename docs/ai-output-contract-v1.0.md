# Problem-Solving Engine
## AI Output Contract · Version 1.0

**Status:** Initial Contract  
**Version:** 1.0

## 1. Purpose

This document defines the contract between the AI normalization layer and the deterministic C++ Problem-Solving Engine.

The AI receives open-ended natural-language input from the user and proposes a structured representation of the problem.

The C++ engine does not trust AI output automatically. All AI-generated data must follow this contract and must be parsed and validated before entering engine logic.

The core principle is:

> **AI proposes structure; Engine makes decisions.**

---

## 2. AI Responsibilities

The AI may:

- Interpret the user's natural-language problem.
- Identify the overall Goal.
- Propose Nodes representing tasks or subproblems.
- Propose Dependencies between Nodes.
- Identify relevant Constraints.
- Propose Node priorities.
- Propose Validation methods and expected results.
- Convert the problem into the required JSON structure.

The AI provides a candidate problem structure only.

---

## 3. AI Authority Restrictions

The AI must not make deterministic engine decisions.

The AI must not:

- Decide whether a Node is Ready.
- Decide whether a Node is Blocked.
- Mark a newly generated Node as InProgress.
- Mark a newly generated Node as Completed.
- Mark a newly generated Node as Failed.
- Claim that a Validation has passed or failed.
- Select the final Recommended Next Step.
- Calculate UnlockValue.
- Calculate NextStepScore.
- Select a Root Suspect.
- Perform the authoritative Failure Diagnosis.
- Override dependency rules.
- Override constraint rules.
- Bypass C++ validation.

These decisions belong to the deterministic C++ engine.

---

## 4. Output Format

The AI must return exactly one valid JSON object.

The AI response must contain JSON only.

The AI must not include:

- Markdown code fences.
- Explanatory text before the JSON.
- Explanatory text after the JSON.
- Comments inside the JSON.
- Headings.
- Conversational responses.

The root JSON object must contain exactly these four top-level fields:

```text
goal
nodes
dependencies
constraints
```

The required root structure is:

```json
{
  "goal": {},
  "nodes": [],
  "dependencies": [],
  "constraints": []
}
```

---

## 5. Goal Contract

The `goal` object must contain:

```json
{
  "name": "string",
  "description": "string",
  "success_condition": "string"
}
```

Rules:

- `name` must describe the overall goal clearly.
- `description` must summarize what the user is trying to accomplish.
- `success_condition` must describe an observable condition that indicates success.
- All three fields are required.

---

## 6. Node Contract

Each Node must contain:

```json
{
  "id": 1,
  "name": "string",
  "description": "string",
  "state": "NotStarted",
  "priority": 2,
  "validation": {
    "description": "string",
    "method": "string",
    "expected_result": "string",
    "result": "Pending"
  }
}
```

Rules:

- `id` must be an integer.
- Every Node ID must be unique.
- `name` must identify one concrete task or subproblem.
- `description` must explain what the Node represents.
- `priority` must be an integer from 1 to 3.
- Every newly generated Node must use:

```text
state = NotStarted
```

The AI must not assign Ready, InProgress, Completed, Failed, or Blocked to a newly generated Node.

Node execution states are controlled by the C++ engine and later user actions.

---

## 7. Validation Contract

Every Node must contain one Validation object.

The Validation object must contain:

```json
{
  "description": "string",
  "method": "string",
  "expected_result": "string",
  "result": "Pending"
}
```

Rules:

- `description` describes what must be checked.
- `method` describes how the check can be performed.
- `expected_result` describes what successful validation should look like.
- Every newly generated Validation must use:

```text
result = Pending
```

The AI must never generate `Pass` or `Fail` as the initial result.

Validation results must come from actual execution, observation, testing, or user input.

---

## 8. Dependency Contract

Each Dependency must contain:

```json
{
  "from_node": 1,
  "to_node": 2
}
```

Meaning:

```text
from_node -> to_node
```

`to_node` depends on the successful completion of `from_node`.

Rules:

- Both Node IDs must exist.
- Self-dependencies are forbidden.
- Dependency cycles are forbidden.
- Dependencies should represent real execution requirements rather than arbitrary ordering preferences.
- The AI may propose Dependencies, but the C++ engine performs authoritative validation.

---

## 9. Constraint Contract

Each Constraint must contain:

```json
{
  "name": "string",
  "type": "Other",
  "value": "string",
  "status": "Unknown"
}
```

Allowed `type` values:

```text
Cost
Time
Resource
Physical
Performance
Other
```

For newly generated problem structures, the default Constraint status is:

```text
Unknown
```

The AI must not assume that a real-world Constraint is satisfied unless the system has explicit verified information supporting that state.

The deterministic system or user-provided evidence must determine whether a Constraint later becomes:

```text
Satisfied
Violated
```

---

## 10. Priority Contract

Priority must use the existing Version 1.0 scale:

```text
1 = Low
2 = Medium
3 = High
```

The AI may propose an initial priority based on the user's problem context.

However:

- Priority is only an input to the deterministic Next-Step Algorithm.
- The AI does not calculate the final Recommended Next Step.
- The user may later modify priority.

---

## 11. Required Initial Values

For every newly generated problem:

```text
Node.state = NotStarted
Validation.result = Pending
Constraint.status = Unknown
```

These defaults separate AI-generated proposals from real execution state.

The AI describes the problem structure.

The engine and user establish what is actually ready, completed, failed, satisfied, or violated.

---

## 12. Structural Rules

AI-generated output must satisfy the following requirements:

1. The root must be a JSON object.
2. The root must contain `goal`, `nodes`, `dependencies`, and `constraints`.
3. Required fields must not be omitted.
4. Node IDs must be unique.
5. Node priority must be between 1 and 3.
6. Enum values must match the Version 1.0 JSON Data Contract.
7. Every Dependency must reference existing Nodes.
8. Self-dependencies are forbidden.
9. Dependency cycles are forbidden.
10. Newly generated Node states must be `NotStarted`.
11. Newly generated Validation results must be `Pending`.
12. Newly generated Constraint statuses must default to `Unknown`.
13. The response must not contain free-form text outside the JSON object.
14. Invalid AI output must be rejected rather than silently accepted.

---

## 13. Example

User input:

```text
I want to build a rubber-band-powered car that can travel at least 10 meters.
```

Valid AI output:

```json
{
  "goal": {
    "name": "Build a Rubber-Band-Powered Car",
    "description": "Build a small vehicle powered by stored elastic energy from a rubber band.",
    "success_condition": "The completed car travels at least 10 meters under rubber-band power."
  },
  "nodes": [
    {
      "id": 1,
      "name": "Design chassis",
      "description": "Determine the chassis layout and dimensions.",
      "state": "NotStarted",
      "priority": 3,
      "validation": {
        "description": "Verify that the chassis design can support the required components.",
        "method": "Review the layout and dimensions.",
        "expected_result": "The chassis has sufficient space and support for axles, wheels, and the rubber-band drive.",
        "result": "Pending"
      }
    },
    {
      "id": 2,
      "name": "Build wheel and axle system",
      "description": "Construct and install freely rotating wheels and axles.",
      "state": "NotStarted",
      "priority": 3,
      "validation": {
        "description": "Verify that the wheels and axles rotate correctly.",
        "method": "Rotate the wheels manually and inspect alignment.",
        "expected_result": "All wheels rotate freely without excessive friction or misalignment.",
        "result": "Pending"
      }
    },
    {
      "id": 3,
      "name": "Install rubber-band drive",
      "description": "Attach the rubber-band energy storage and drive mechanism.",
      "state": "NotStarted",
      "priority": 3,
      "validation": {
        "description": "Verify that stored rubber-band energy can rotate the drive axle.",
        "method": "Wind and release the rubber band while observing the drivetrain.",
        "expected_result": "The rubber band transfers rotational energy to the drive axle reliably.",
        "result": "Pending"
      }
    },
    {
      "id": 4,
      "name": "Perform distance test",
      "description": "Test the completed vehicle on a suitable flat surface.",
      "state": "NotStarted",
      "priority": 3,
      "validation": {
        "description": "Measure the travel distance of the car.",
        "method": "Run the vehicle and measure its travel distance.",
        "expected_result": "The car travels at least 10 meters.",
        "result": "Pending"
      }
    }
  ],
  "dependencies": [
    {
      "from_node": 1,
      "to_node": 2
    },
    {
      "from_node": 2,
      "to_node": 3
    },
    {
      "from_node": 3,
      "to_node": 4
    }
  ],
  "constraints": []
}
```

The AI does not decide that Node 1 is Ready.

After parsing and validation, the C++ engine determines executable states and selects the Recommended Next Step.

---

## 14. Processing Boundary

The intended pipeline is:

```text
User Natural-Language Input
        |
        v
AI Normalization
        |
        v
AI Output Contract JSON
        |
        v
JSON Parsing
        |
        v
Semantic Validation
        |
        v
ProblemData
        |
        v
Deterministic C++ Engine
        |
        +--> State Evaluation
        +--> Dependency Analysis
        +--> Constraint Checking
        +--> Next-Step Selection
        +--> Failure Diagnosis
```

AI output is untrusted external input until it passes parsing and semantic validation.

---

## 15. Contract Relationship

This document does not replace the existing:

```text
docs/json-data-contract-v1.0.md
```

The JSON Data Contract defines the general data representation accepted by the C++ system.

This AI Output Contract defines the stricter rules that apply specifically when AI creates a new problem structure.

Both contracts must remain compatible.

---

## 16. Version 1.0 Principle

The purpose of this contract is to prevent the AI layer from becoming the decision engine.

AI contributes open-ended language understanding and domain interpretation.

The C++ engine remains responsible for deterministic reasoning.

> **AI proposes structure; Engine validates and makes decisions.**