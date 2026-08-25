# Problem-Solving Engine
## JSON Data Contract · Version 1.0

**Status:** Initial Contract  
**Version:** 1.0

## 1. Purpose

This document defines the structured JSON format used to transfer problem data into the Problem-Solving Engine.

The JSON layer represents the same six core object types used by the C++ engine:

- Goal
- Node
- Dependency
- Constraint
- State
- Validation

The AI layer may generate this structure later, but the C++ engine is responsible for validating and processing it.

## 2. Root Structure

Every Version 1.0 problem object must contain:

```json
{
  "goal": {},
  "nodes": [],
  "dependencies": [],
  "constraints": []
}
```

## 3. Goal

JSON fields:

```json
{
  "name": "string",
  "description": "string",
  "success_condition": "string"
}
```

Mapping to C++:

- `name` -> `Goal::name`
- `description` -> `Goal::description`
- `success_condition` -> `Goal::successCondition`

All three fields are required.

## 4. Node

JSON fields:

```json
{
  "id": 1,
  "name": "string",
  "description": "string",
  "state": "NotStarted",
  "priority": 2,
  "validation": {}
}
```

Mapping to C++:

- `id` -> `Node::id`
- `name` -> `Node::name`
- `description` -> `Node::description`
- `state` -> `Node::state`
- `priority` -> `Node::priority`
- `validation` -> `Node::validation`

Rules:

- `id` must be an integer.
- Every node ID must be unique.
- `priority` must be an integer from 1 to 3.
- `state` must use one of the allowed State values.

## 5. State

Allowed JSON values:

```text
NotStarted
Ready
InProgress
Completed
Failed
Blocked
```

These values map directly to the C++ `State` enum.

## 6. Validation

JSON fields:

```json
{
  "description": "string",
  "method": "string",
  "expected_result": "string",
  "result": "Pending"
}
```

Mapping to C++:

- `description` -> `Validation::description`
- `method` -> `Validation::method`
- `expected_result` -> `Validation::expectedResult`
- `result` -> `Validation::result`

Allowed `result` values:

```text
Pending
Pass
Fail
```

## 7. Dependency

JSON fields:

```json
{
  "from_node": 1,
  "to_node": 2
}
```

Mapping to C++:

- `from_node` -> `Dependency::fromNode`
- `to_node` -> `Dependency::toNode`

Meaning:

```text
from_node -> to_node
```

The `to_node` depends on the successful completion of `from_node`.

Rules:

- Both referenced node IDs must exist.
- A node cannot depend on itself.
- Dependency cycles are forbidden.

## 8. Constraint

JSON fields:

```json
{
  "name": "string",
  "type": "Other",
  "value": "string",
  "status": "Unknown"
}
```

Mapping to C++:

- `name` -> `Constraint::name`
- `type` -> `Constraint::type`
- `value` -> `Constraint::value`
- `status` -> `Constraint::status`

Allowed `type` values:

```text
Cost
Time
Resource
Physical
Performance
Other
```

Allowed `status` values:

```text
Unknown
Satisfied
Violated
```

## 9. Complete Example

```json
{
  "goal": {
    "name": "Build a Test Vehicle",
    "description": "Create a small vehicle and verify that it moves correctly.",
    "success_condition": "The vehicle completes the test route successfully."
  },

  "nodes": [
    {
      "id": 1,
      "name": "Build chassis",
      "description": "Construct the main vehicle frame.",
      "state": "Ready",
      "priority": 3,
      "validation": {
        "description": "Verify structural stability.",
        "method": "Manual inspection",
        "expected_result": "Frame remains stable.",
        "result": "Pending"
      }
    },

    {
      "id": 2,
      "name": "Install motor",
      "description": "Attach the motor to the completed chassis.",
      "state": "NotStarted",
      "priority": 2,
      "validation": {
        "description": "Verify motor operation.",
        "method": "Power test",
        "expected_result": "Motor rotates normally.",
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
      "name": "Budget limit",
      "type": "Cost",
      "value": "50 CAD",
      "status": "Satisfied"
    }
  ]
}
```

## 10. Version 1.0 Validation Rules

Before JSON data is accepted by the engine:

1. The root object must contain `goal`, `nodes`, `dependencies`, and `constraints`.
2. Required fields must be present.
3. Node IDs must be unique.
4. Node priority must be between 1 and 3.
5. Enum strings must use allowed values.
6. Every dependency reference must point to an existing node.
7. Self-dependencies are forbidden.
8. Dependency cycles are forbidden.
9. Invalid input must be rejected instead of silently accepted.

## 11. Design Principle

The JSON contract is the boundary between the open-ended AI layer and the deterministic C++ engine.

**AI proposes structure; Engine validates and makes decisions.**