# Problem-Solving Engine

A C++17 problem-solving and reasoning engine that combines deterministic decision logic with AI-assisted natural-language problem interpretation.

> **AI proposes structure; Engine makes decisions.**

The system accepts an open-ended problem, converts it into a structured representation, validates that representation against deterministic engine rules, and exposes it to the reasoning engine only after validation succeeds.

---

## Overview

The Problem-Solving Engine is designed around a strict separation between:

- AI-assisted interpretation
- Deterministic reasoning and validation

AI is used to propose a structured interpretation of a user's problem.

The C++ engine remains responsible for:

- validating structure,
- enforcing dependencies,
- enforcing constraints,
- controlling state transitions,
- selecting next steps,
- diagnosing failures.

AI output is never trusted automatically.

---

## Current Capabilities

### Core Problem Model

The engine represents problems using:

- Goals
- Nodes
- Dependencies
- Constraints
- States
- Validation rules
- Aggregated `ProblemData`

### Deterministic Engine Logic

Implemented reasoning features include:

- State-transition rules
- Ready-state evaluation
- Dependency validation
- Cycle detection
- Constraint checking
- Next-step selection
- Deterministic tie-breaking
- Failure diagnosis
- Root-suspect tracing
- Affected downstream-node discovery

### Structured Data

The engine supports:

- Structured JSON problem input
- Semantic validation
- JSON serialization
- Save/load support
- Invalid-data rejection
- Round-trip persistence

### AI-Assisted Problem Interpretation

The AI integration includes:

- Controlled prompt construction
- OpenAI Responses API integration
- Natural-language problem normalization
- AI-generated JSON parsing
- AI-specific validation
- End-to-end problem-processing pipeline

The AI-assisted path is:

```text
Natural-language problem
        |
        v
AIProblemPipeline
        |
        v
AIProblemNormalizer
        |
        v
ProblemPromptBuilder
        |
        v
OpenAIClient
        |
        v
OpenAI Responses API
        |
        v
Structured JSON proposal
        |
        v
AIProblemParser
        |
        v
AIProblemValidator
        |
        v
Validated ProblemData
        |
        v
Problem-Solving Engine
```

---

## Design Principle

The central architectural rule is:

> **AI proposes structure; Engine makes decisions.**

The AI may interpret an open-ended problem and propose:

- a goal,
- a success condition,
- task nodes,
- dependencies,
- constraints.

However, AI cannot directly bypass deterministic engine rules.

AI-generated data must be parsed and validated before it is accepted.

The deterministic engine controls:

- state,
- readiness,
- dependency validity,
- constraint validity,
- next-step decisions,
- failure diagnosis.

---

## Safety Boundary

The AI-assisted architecture intentionally limits AI authority.

AI-generated structures cannot directly mark new work as:

- Ready
- InProgress
- Completed
- Failed

AI output must pass:

```text
AI response
    |
    v
JSON parsing
    |
    v
AI-specific validation
    |
    v
Deterministic semantic validation
    |
    v
Accepted ProblemData
```

Malformed, incomplete, or unauthorized AI output is rejected.

---

## Requirements

The project currently requires:

- C++17-compatible compiler
- CMake 3.16 or newer
- libcurl
- Git
- Internet access for AI-assisted mode
- OpenAI API credentials for live AI requests

The project uses `nlohmann/json`, which is fetched automatically through CMake.

---

## Build

From the project root:

```bash
cmake -S . -B build
cmake --build build
```

---

## Run Without AI

Normal execution is safe by default:

```bash
./build/problem-solving-engine
```

Expected output:

```text
Problem-Solving Engine is running.
AI-assisted mode is disabled by default.
Run with --ai to enter a problem.
```

Normal execution does not intentionally enter the live AI-assisted path.

---

## Run Automated Tests

Run the full automated test suite with:

```bash
ctest --test-dir build --output-on-failure
```

Current project status:

```text
15/15 tests passed
0 tests failed
```

The test suite covers:

1. Core models
2. Engine logic
3. JSON parsing
4. Problem validation
5. Problem loading
6. Problem serialization
7. Problem storage
8. OpenAI configuration
9. OpenAI usage guard
10. OpenAI client
11. Problem prompt builder
12. AI problem normalizer
13. AI problem parser
14. AI problem validator
15. AI problem pipeline

Most AI integration behavior can therefore be tested without making live API requests.

---

## OpenAI Configuration

Copy the example environment configuration:

```bash
cp .env.example .env
```

Then edit the local `.env` file:

```text
OPENAI_API_KEY=your_api_key
OPENAI_MODEL=your_model

OPENAI_MAX_REQUESTS_PER_RUN=5
OPENAI_MAX_INPUT_CHARS=12000
```

Do not commit real API credentials.

The local `.env` file should remain outside source control.

Load the configuration into the current terminal session:

```bash
set -a
source .env
set +a
```

---

## Run AI-Assisted Mode

After loading the environment configuration:

```bash
./build/problem-solving-engine --ai
```

The program will prompt:

```text
Enter a problem:
```

Example input:

```text
I want to build a rubber-band-powered car that can travel at least 10 meters.
```

The program then:

1. sends a controlled interpretation request,
2. receives a structured AI proposal,
3. parses the proposal,
4. validates it,
5. rejects unauthorized AI decisions,
6. converts it into validated `ProblemData`,
7. displays the accepted problem structure.

Example output structure:

```text
Problem accepted by the engine.

Goal: ...
Success condition: ...

Plan:
- [1] ...
- [2] ...
- [3] ...

Constraints:
- ...

AI-generated problem structure passed engine validation.
```

---

## Usage Guard

Live API usage is protected through `OpenAIUsageGuard`.

Configurable limits include:

```text
OPENAI_MAX_REQUESTS_PER_RUN
OPENAI_MAX_INPUT_CHARS
```

The guard limits:

- requests per program run,
- maximum user-input length.

Rejected requests do not consume the per-run request quota.

---

## Project Structure

```text
Problem-Solving-Engine/
|
|-- include/
|   |-- core data models
|   |-- engine interfaces
|   |-- OpenAI integration interfaces
|   `-- AI problem-processing interfaces
|
|-- src/
|   |-- deterministic engine logic
|   |-- JSON processing
|   |-- persistence
|   |-- OpenAI client and HTTP transport
|   `-- AI problem-processing pipeline
|
|-- tests/
|   `-- automated unit and integration tests
|
|-- docs/
|   |-- implementation-progress.md
|   `-- ai-output-contract-v1.0.md
|
|-- CMakeLists.txt
|-- .env.example
`-- README.md
```

---

## Main Components

### `Engine`

Deterministic reasoning layer responsible for engine-controlled decisions.

### `ProblemValidator`

Checks semantic correctness of structured problem data.

### `ProblemSerializer` / `ProblemStorage`

Provide structured persistence and round-trip save/load support.

### `OpenAIClient`

Handles communication with the OpenAI Responses API through an abstract HTTP layer.

### `OpenAIUsageGuard`

Protects live API usage with configurable limits.

### `ProblemPromptBuilder`

Creates controlled prompts that define the AI's role and output requirements.

### `AIProblemNormalizer`

Uses the AI client to transform natural-language input into a candidate structured representation.

### `AIProblemParser`

Converts the AI-generated JSON representation into internal C++ data structures.

### `AIProblemValidator`

Applies AI-specific authority restrictions together with deterministic validation.

### `AIProblemPipeline`

Coordinates the complete AI-assisted interpretation path and returns validated `ProblemData`.

---

## End-to-End Validation

The complete live path has been tested with a real problem:

```text
Build a rubber-band-powered car that can travel at least 10 meters.
```

The system successfully executed:

```text
User input
    ->
OpenAI API
    ->
Structured AI proposal
    ->
Parsing
    ->
AI-specific validation
    ->
Deterministic validation
    ->
Validated ProblemData
    ->
Command-line output
```

The final structure passed engine validation successfully.

---

## Documentation

Detailed implementation history:

```text
docs/implementation-progress.md
```

AI output authority and structure contract:

```text
docs/ai-output-contract-v1.0.md
```

---

## Current Status

Stages completed:

- Core Data Models
- Engineering Foundation
- Engine Logic
- Structured JSON Data
- Save / Load and Storage
- OpenAI API Integration and Connectivity
- AI-Assisted Problem Interpretation
- End-to-End Integration

Current automated test result:

> **15/15 tests passing**

Current end-to-end status:

> **Natural-language input -> AI interpretation -> deterministic validation -> validated problem structure is operational.**

---

## Future Development

Potential future work includes:

- deeper integration between validated AI-generated structures and the deterministic execution engine,
- improved command-line interaction,
- expanded failure-path testing,
- network timeout and retry policies,
- richer persistence workflows,
- additional engine reasoning strategies,
- improved observability and diagnostics,
- broader end-to-end test coverage.

Future AI functionality should continue to preserve the project's architectural boundary:

> **AI proposes structure; Engine makes decisions.**