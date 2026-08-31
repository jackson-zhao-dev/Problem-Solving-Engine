# Problem-Solving Engine
## Technical Project Report — Version 1.0

**Report Date:** 2026-08-30  
**Implementation Language:** C++17  
**Build System:** CMake  
**AI Integration:** OpenAI Responses API  
**Project Status:** Stage 7 Implementation Complete

---

## 1. Executive Summary

The Problem-Solving Engine is a C++17 system designed to investigate whether open-ended real-world problems can be converted into a common structured representation and then processed using deterministic program logic.

The central architectural principle is:

> **AI proposes structure; Engine makes decisions.**

The system separates two responsibilities.

The AI layer interprets open-ended natural-language problems and proposes a structured representation.

The deterministic C++ engine validates that representation and remains responsible for engine-controlled reasoning, including:

- state management,
- dependency analysis,
- constraint handling,
- next-step selection,
- validation,
- failure diagnosis,
- downstream impact analysis.

Version 1.0 contains:

- core problem data models,
- deterministic engine logic,
- dependency and cycle validation,
- constraint checking,
- next-step selection,
- failure diagnosis,
- JSON parsing and serialization,
- save/load support,
- OpenAI API integration,
- API usage safeguards,
- controlled prompt construction,
- AI-generated JSON parsing,
- AI-specific validation,
- an end-to-end AI problem-processing pipeline,
- command-line AI-assisted input,
- fifteen automated test targets,
- and a successful real API end-to-end validation.

The project does not claim that Version 1.0 can optimally solve arbitrary real-world problems or that cross-domain generality has already been proven.

Its main engineering result is a working boundary between probabilistic AI interpretation and deterministic engine authority.

---

## 2. Project Motivation

The project originated from a broader question about how complex problems are solved.

A recurring problem-solving process can be represented as:

    Understand
        ->
    Decompose
        ->
    Execute
        ->
    Validate
        ->
    Diagnose
        ->
    Improve
        ->
    Repeat

The project asks whether part of this process can be formalized computationally.

The original design considered problems from different engineering domains, such as:

- mechanical systems,
- robotics,
- circuits,
- software projects,
- and other structured tasks.

A small C++ program cannot contain complete expert knowledge about every possible domain.

Therefore, the project separates domain interpretation from deterministic reasoning:

1. AI interprets the open-ended problem.
2. AI proposes a structured representation.
3. C++ parses and validates that representation.
4. The deterministic engine reasons only over accepted data.

This division became the foundation of the system.

---

## 3. Research and Engineering Question

The central question is:

> Can complex problems from different domains be represented using a small set of common structural concepts, and can deterministic graph and state logic operate on that representation to make explainable decisions?

The project represents problems using:

- Goal
- Node
- Dependency
- Constraint
- State
- Validation

These concepts are aggregated into `ProblemData`.

The final implementation also addresses an additional engineering question:

> Can AI assist with natural-language interpretation without being allowed to bypass deterministic engine rules?

---

## 4. Core Design Principle

The most important architectural rule is:

> **AI proposes structure; Engine makes decisions.**

AI may propose:

- a goal,
- a success condition,
- task nodes,
- descriptions,
- priorities,
- dependencies,
- constraints,
- validation procedures.

However, AI-generated output is treated as untrusted external input.

The deterministic engine remains authoritative over:

- semantic validity,
- state validity,
- dependency validity,
- cycle detection,
- constraint validity,
- ready-state logic,
- next-step selection,
- deterministic tie-breaking,
- validation reliability,
- failure diagnosis,
- downstream impact analysis.

Successful API communication is therefore not enough for AI-generated data to enter the engine.

The structure must first pass parsing and validation.

---

## 5. Version 1.0 Architecture

The final AI-assisted path is:

    User natural-language problem
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
    HttpClient / CurlHttpClient
            |
            v
    OpenAI Responses API
            |
            v
    Structured AI-generated JSON
            |
            v
    AIProblemParser
            |
            v
    AIProblemValidator
            |
            v
    Deterministic semantic validation
            |
            v
    Validated ProblemData
            |
            v
    Problem-Solving Engine

Four important boundaries exist in this architecture.

### 5.1 Natural-Language Boundary

The user can describe a problem without manually creating JSON.

AI handles open-ended linguistic interpretation.

### 5.2 Structured-Data Boundary

The AI must return a controlled structured representation rather than unrestricted conversational output.

### 5.3 Validation Boundary

AI-generated data is not automatically trusted.

It must pass parsing, AI-specific validation, and deterministic semantic validation.

### 5.4 Decision Boundary

The AI does not replace deterministic engine algorithms.

The C++ engine remains responsible for engine decisions.

---

## 6. Core Data Model

The engine is built around reusable problem abstractions.

### 6.1 Goal

A `Goal` represents the overall desired outcome.

It includes information such as:

- name,
- description,
- success condition.

### 6.2 Node

A `Node` represents a task, component, stage, or subproblem.

A node includes information such as:

- unique ID,
- name,
- description,
- state,
- priority,
- validation information.

### 6.3 Dependency

A `Dependency` represents a directed relationship between nodes.

Conceptually:

    fromNode -> toNode

The downstream node depends on the upstream node.

### 6.4 Constraint

A `Constraint` represents a restriction or condition.

Examples include:

- cost,
- time,
- resources,
- physical limitations,
- performance requirements.

### 6.5 State

State represents the current status of work.

The deterministic engine controls state behavior instead of treating state as arbitrary AI input.

### 6.6 Validation

Validation represents how completion or success is checked.

### 6.7 ProblemData

`ProblemData` aggregates the complete structured representation so that the engine can process a problem consistently.

---

## 7. Deterministic Engine Logic

The deterministic engine was implemented before live AI integration.

This order was intentional.

The project first established the rules that later AI-generated structures would be required to obey.

### 7.1 State and Readiness Logic

The engine contains logic for deciding whether work is:

- not started,
- ready,
- in progress,
- completed,
- failed,
- blocked.

Readiness depends on deterministic structural conditions.

### 7.2 Dependency Validation

The system rejects invalid dependency structures.

Checks include:

- nonexistent node references,
- self-dependencies,
- dependency cycles.

This prevents malformed graphs from entering normal engine processing.

### 7.3 Constraint Handling

Constraints participate in deterministic decision logic.

Unknown or violated constraints can prevent a task from being treated as executable.

### 7.4 Next-Step Selection

When multiple nodes are eligible, the engine uses a deterministic scoring rule.

The Version 1.0 design uses:

    NextStepScore = 2 * Priority + UnlockValue

`Priority` represents task importance.

`UnlockValue` represents how much downstream work a node directly enables.

Deterministic tie-breaking ensures stable behavior for identical input.

The algorithm is intentionally simple and explainable.

It is not claimed to be globally optimal.

---

## 8. Failure Diagnosis

Failure Diagnosis is implemented as a structural debugging mechanism.

When validation fails, the currently failing node may not be the earliest source of the problem.

The engine can trace dependencies upstream to identify a structurally suspicious point.

The implementation includes:

- validation reliability handling,
- root-suspect tracing,
- branch termination when upstream validation is reliable,
- deterministic suspect selection,
- affected downstream-node discovery.

This allows the engine to distinguish between:

- a failure at the current task,
- and a failure whose structural source may be earlier in the dependency graph.

Failure Diagnosis does not claim to prove physical causality.

Its result is a deterministic diagnostic direction.

---

## 9. Structured JSON Layer

A structured JSON layer was added after the deterministic core.

It allows problem representations to move between:

- external input,
- AI output,
- storage,
- internal C++ models.

Implemented functionality includes:

- JSON parsing,
- enum conversion,
- JSON-to-model mapping,
- semantic validation,
- malformed JSON rejection,
- duplicate node ID rejection,
- invalid priority rejection,
- invalid dependency rejection,
- cycle rejection,
- unified problem loading.

The project uses `nlohmann/json`, integrated through CMake.

This JSON layer forms a machine-readable boundary between probabilistic AI output and deterministic C++ logic.

---

## 10. Persistence

The system includes save/load support.

### 10.1 ProblemSerializer

`ProblemSerializer` converts complete problem structures into JSON.

It supports serialization of:

- core models,
- enums,
- complete `ProblemData`.

Round-trip tests verify that serialized data can be reconstructed consistently.

### 10.2 ProblemStorage

`ProblemStorage` handles file persistence.

Implemented behavior includes:

- saving valid problem data,
- loading stored problem data,
- validating before saving,
- rejecting missing files,
- rejecting malformed stored JSON,
- validating loaded structures.

Persistence allows structured problem state to exist independently of one program execution.

---

## 11. OpenAI Integration

The AI layer was introduced only after the deterministic engine and structured-data layers were functioning.

This followed an early project decision:

> Build the deterministic core before integrating external AI.

### 11.1 OpenAIConfig

`OpenAIConfig` loads configuration from environment variables.

Important values include:

    OPENAI_API_KEY
    OPENAI_MODEL
    OPENAI_MAX_REQUESTS_PER_RUN
    OPENAI_MAX_INPUT_CHARS

Required values are validated before use.

### 11.2 OpenAIUsageGuard

`OpenAIUsageGuard` provides basic live-API safeguards.

It enforces:

- maximum requests per program run,
- maximum input length.

Rejected requests do not consume the allowed request count.

### 11.3 HTTP Abstraction

The project defines a generic HTTP abstraction rather than coupling all AI logic directly to libcurl.

This includes:

- `HttpClient`
- `HttpResponse`

The abstraction allows tests to substitute fake HTTP behavior.

### 11.4 CurlHttpClient

`CurlHttpClient` provides the real transport implementation using libcurl.

It supports:

- POST requests,
- headers,
- request bodies,
- response bodies,
- HTTP status capture,
- transport error handling.

### 11.5 OpenAIClient

`OpenAIClient` communicates with the OpenAI Responses API.

Its responsibilities include:

- constructing API requests,
- using the configured model,
- attaching authorization,
- applying the usage guard,
- checking HTTP success,
- parsing API JSON,
- rejecting incomplete responses,
- rejecting missing output,
- extracting usable output text.

---

## 12. AI Output Contract

A dedicated AI-output contract defines what AI is allowed to do.

AI is allowed to propose structure.

AI is not allowed to make authoritative deterministic engine decisions.

The AI may produce structured problem elements such as:

- goal,
- nodes,
- dependencies,
- constraints.

The system intentionally separates two questions:

> What structure does the AI propose?

and:

> What structure does the deterministic engine accept?

These are not treated as the same decision.

---

## 13. ProblemPromptBuilder

`ProblemPromptBuilder` converts raw user input into a controlled AI instruction.

The builder communicates:

- the expected structure,
- the output contract,
- JSON requirements,
- authority restrictions.

This reduces dependence on an unconstrained conversational prompt.

Prompt construction is separately testable.

---

## 14. AIProblemNormalizer

`AIProblemNormalizer` sends the controlled request through the OpenAI client.

Its output is treated as a candidate structured representation.

The normalizer does not grant authority to the returned data.

Fake HTTP responses allow most normalization behavior to be tested without live API calls.

---

## 15. AIProblemParser

`AIProblemParser` converts AI-generated JSON into internal `ProblemData`.

Instead of creating an independent AI parsing system, it reuses the project's existing JSON infrastructure.

This reduces duplicated logic and ensures that AI-generated structures enter the same internal representation used by the rest of the engine.

Malformed or unusable AI output is rejected.

---

## 16. AIProblemValidator

`AIProblemValidator` creates an additional authority boundary specifically for AI-generated data.

A major implementation concern was preventing AI from assigning states that belong to deterministic engine logic.

AI-generated nodes are therefore restricted from directly claiming engine-controlled states such as:

- Ready
- InProgress
- Completed
- Failed

AI-generated validation and constraint values also cannot be used to bypass deterministic checks.

This is one of the most important differences between the project and a simple AI-output parser.

The system does not only ask whether the JSON is syntactically valid.

It also asks whether the AI is attempting to make a decision reserved for the engine.

---

## 17. AIProblemPipeline

`AIProblemPipeline` coordinates the complete AI-assisted input path.

The implemented sequence is:

    Natural-language input
            |
            v
    AIProblemNormalizer
            |
            v
    AI-generated JSON
            |
            v
    AIProblemParser
            |
            v
    ProblemData
            |
            v
    AIProblemValidator
            |
            v
    Validated ProblemData

The pipeline centralizes the rule that AI output must pass validation before being accepted.

Automated testing covers:

- successful processing of valid AI output,
- rejection of output that attempts unauthorized engine decisions.

---

## 18. Command-Line Integration

The final Stage 7 implementation connects the AI pipeline to the executable program.

Normal execution is:

    ./build/problem-solving-engine

AI-assisted execution is explicitly requested with:

    ./build/problem-solving-engine --ai

The `--ai` requirement is an operational safety decision.

Normal execution does not intentionally enter the live API path.

In AI mode, the program prompts:

    Enter a problem:

The user may then provide an open-ended natural-language problem.

After processing, the program displays the accepted structured result only after validation succeeds.

Output may include:

- Goal
- Success condition
- Plan
- Constraints

The program also reports whether the generated problem structure passed engine validation.

---

## 19. Security and Operational Safety

Several safeguards were implemented because the system communicates with an external API.

### 19.1 Secret Management

Real API credentials are not hard-coded.

The project uses local environment configuration.

The real `.env` file remains outside source control.

`.env.example` documents configuration names without containing real credentials.

### 19.2 Explicit AI Mode

Live AI behavior requires explicit `--ai` invocation.

This reduces accidental API calls during ordinary execution.

### 19.3 Request Limits

`OpenAIUsageGuard` limits requests per program execution.

### 19.4 Input Limits

The usage guard limits maximum input length.

### 19.5 Untrusted AI Output

AI output is never accepted solely because the API request succeeded.

Returned structures must still pass parsing and deterministic validation.

### 19.6 Fake HTTP Testing

Most AI integration logic can be tested without making live API requests.

This reduces unnecessary network and billing dependency during development.

---

## 20. Automated Testing

The final automated test suite contains fifteen test targets:

1. `CoreModelTests`
2. `EngineLogicTests`
3. `JsonParserTests`
4. `ProblemValidatorTests`
5. `ProblemLoaderTests`
6. `ProblemSerializerTests`
7. `ProblemStorageTests`
8. `OpenAIConfigTests`
9. `OpenAIUsageGuardTests`
10. `OpenAIClientTests`
11. `ProblemPromptBuilderTests`
12. `AIProblemNormalizerTests`
13. `AIProblemParserTests`
14. `AIProblemValidatorTests`
15. `AIProblemPipelineTests`

The final recorded result is:

    15/15 tests passed
    0 tests failed

The test suite covers both the deterministic core and the AI-integration boundary.

The correctness of the project therefore does not depend only on whether a real API request succeeds.

Internal components are tested independently.

---

## 21. Real End-to-End Validation

A real live end-to-end run was completed using the problem:

    I want to build a rubber-band-powered car that can travel at least 10 meters.

The production path executed:

    Natural-language user input
            |
            v
    OpenAI API request
            |
            v
    Structured AI proposal
            |
            v
    Normalization
            |
            v
    JSON parsing
            |
            v
    AI-specific validation
            |
            v
    Deterministic validation
            |
            v
    Validated ProblemData
            |
            v
    Command-line output

The generated structure included:

- a goal,
- a success condition,
- an implementation plan,
- constraints.

The program reported:

    AI-generated problem structure passed engine validation.

This confirms that the implemented production integration works end to end for this example.

It does not by itself prove reliable performance across arbitrary domains.

---

## 22. Original Design vs. Final Implementation

The original Version 1.0 design provided the baseline for implementation.

Several major ideas survived directly into the completed system.

### 22.1 Design Elements Implemented as Planned

Implemented concepts include:

- C++ as the deterministic engine language,
- AI as an external interpretation layer,
- six core problem abstractions,
- dependency graph logic,
- state logic,
- deterministic next-step selection,
- failure diagnosis,
- structured JSON,
- API-key protection,
- deterministic validation of AI output,
- console-based interaction,
- separation of AI interpretation from engine authority.

The central principle remained unchanged:

> **AI proposes structure; Engine makes decisions.**

### 22.2 Infrastructure Added During Implementation

The final system contains engineering infrastructure that was less detailed in the original design:

- CMake FetchContent integration,
- CTest automated testing,
- `ProblemData` aggregation,
- serializer and storage modules,
- abstract HTTP transport,
- fake HTTP testing,
- libcurl transport,
- OpenAI configuration validation,
- request and input usage guards,
- dedicated AI output contract,
- prompt builder,
- AI normalizer,
- AI parser,
- AI validator,
- AI pipeline,
- explicit `--ai` mode.

These additions emerged from practical implementation requirements.

### 22.3 Original Interactive Loop Not Fully Exposed

The original design described a larger user workflow:

1. receive a recommended next step,
2. perform the work,
3. submit validation results,
4. update state or trigger failure diagnosis,
5. repeat until the goal is satisfied.

Many deterministic components needed for this reasoning are implemented.

However, the current Stage 7 command-line AI workflow primarily performs:

    natural-language input
    -> structured AI interpretation
    -> validation
    -> accepted ProblemData

The complete iterative execution cycle is not yet exposed as one polished interactive workflow.

### 22.4 Output-Side AI Explanation

The original architecture also proposed an AI explanation layer that could convert deterministic conclusions back into domain-specific natural language.

Version 1.0 focuses mainly on the input-side interpretation pipeline and deterministic validation boundary.

A complete output-side AI explanation system is not a central completed feature.

---

## 23. Evaluation Status

The original design proposed evaluation across several different domains.

Possible domains included:

- mechanical systems,
- robotics,
- circuits,
- software.

Current evidence is stronger at the component and structural level than at the broad cross-domain empirical level.

### 23.1 Completed Evaluation

Completed evaluation includes:

- deterministic unit and integration testing,
- malformed-data testing,
- dependency validation testing,
- persistence testing,
- AI client testing,
- AI authority-boundary testing,
- AI pipeline testing,
- one real mechanical-style end-to-end problem.

### 23.2 Evaluation Not Yet Completed

The following have not yet been established:

- repeated live AI runs across unrelated domains,
- statistical reliability of AI decomposition,
- systematic comparison between models,
- systematic measurement of human correction frequency,
- broad evaluation of next-step quality,
- broad evaluation of failure-diagnosis usefulness on real physical systems.

Therefore, Version 1.0 should not claim that broad cross-domain effectiveness has already been proven.

A more accurate conclusion is:

> The architecture is implemented and operational, and its structural components are tested. Broader cross-domain effectiveness remains an evaluation question.

---

## 24. Known Limitations

### 24.1 AI Interpretation Quality

AI may:

- misunderstand user intent,
- omit useful tasks,
- over-decompose a problem,
- under-decompose a problem,
- infer inappropriate constraints,
- produce invalid JSON,
- produce structurally valid but practically weak plans.

Validation reduces structural risk but does not guarantee optimal interpretation.

### 24.2 Domain Knowledge

The deterministic engine is intentionally domain-independent.

It does not independently contain complete expertise in:

- mechanical engineering,
- electronics,
- robotics,
- software architecture,
- other specialized fields.

AI may help interpret those domains, but the deterministic engine cannot independently verify every domain claim.

### 24.3 Next-Step Optimality

The scoring algorithm is deliberately simple and explainable.

It is not proven to be globally optimal.

### 24.4 Failure Diagnosis Scope

Failure Diagnosis identifies structurally suspicious upstream nodes.

It does not prove real-world physical causality.

### 24.5 Constraint Reasoning

The engine represents and validates constraints but does not perform complete mathematical reasoning over every possible constraint type.

### 24.6 External API Dependency

AI-assisted mode depends on:

- network connectivity,
- API availability,
- valid local credentials,
- a compatible configured model.

### 24.7 Current User Interface

The project currently uses a command-line interface.

The deterministic engine contains more capability than is currently exposed through one continuous user workflow.

---

## 25. Engineering Process

The project used an incremental development strategy.

The recurring development cycle was:

    Design a component
        ->
    Implement
        ->
    Build
        ->
    Test
        ->
    Fix
        ->
    Commit
        ->
    Push
        ->
    Continue

Meaningful milestones were committed separately.

The project also maintained several forms of engineering memory:

- original design document,
- implementation progress ledger,
- project decisions document,
- Git history,
- README documentation,
- automated tests.

This preserves both:

- what the project originally intended to build,
- and what was actually implemented.

---

## 26. Major Development Stages

Version 1.0 development was organized into seven major stages.

### Stage 1 — Core Data Models

Completed.

### Stage 2 — Engineering Foundation

Completed.

### Stage 3 — Engine Logic

Completed.

### Stage 4 — Structured JSON Data

Completed.

### Stage 5 — Save / Load and Storage

Completed.

### Stage 6 — OpenAI API Integration and Connectivity

Completed.

### Stage 7 — AI-Assisted Problem Interpretation and End-to-End Integration

Completed.

At the end of Stage 7, the project contained:

- a functioning deterministic core,
- a structured data layer,
- persistence,
- guarded live API access,
- an AI interpretation pipeline,
- a command-line AI input path,
- fifteen passing automated tests,
- and a successful real end-to-end AI run.

---

## 27. What the Project Demonstrates

The strongest Version 1.0 result is not simply that AI can generate a plan.

A language model can already generate plans.

The more important engineering result is the separation of authority.

The project demonstrates an architecture in which:

1. a probabilistic model interprets open-ended language,
2. the model proposes structured data,
3. the proposal crosses a formal parsing boundary,
4. AI-specific authority restrictions are applied,
5. deterministic validation is applied,
6. only accepted data enters the engine.

This provides a concrete implementation of the principle:

> **Probabilistic interpretation does not require probabilistic authority.**

The project therefore demonstrates a broader software-engineering pattern:

> AI can be placed at the boundary of a deterministic system without becoming the final decision-maker inside that system.

---

## 28. Future Work

Future work should be driven by specific engineering or evaluation goals rather than by feature count.

### 28.1 Cross-Domain Evaluation

Test meaningfully different problems, such as:

- mechanical,
- software,
- electronics or robotics.

Record:

- accepted outputs,
- rejected outputs,
- malformed responses,
- human corrections,
- differences in decomposition quality.

### 28.2 Interactive Engine Execution

Expose more of the deterministic engine through a continuous workflow:

    Problem input
    -> validated structure
    -> ready-node calculation
    -> next-step recommendation
    -> user validation result
    -> state update
    -> failure diagnosis when necessary
    -> repeat

### 28.3 Improved Diagnostics

Provide clearer explanations for:

- rejected AI structures,
- invalid dependencies,
- constraint failures,
- unauthorized AI state assignments.

### 28.4 Network Hardening

Possible improvements include:

- timeout configuration,
- retry policies,
- clearer transport diagnostics.

### 28.5 Broader Testing

Additional tests could cover:

- unusual AI responses,
- partial responses,
- larger problem graphs,
- pipeline edge cases,
- repeated end-to-end workflows.

### 28.6 Richer Interfaces

A future interface could visualize:

- dependency graphs,
- node states,
- blocked paths,
- recommended next steps,
- failure-diagnosis paths.

This is outside the current Version 1.0 requirement.

---

## 29. Conclusion

Version 1.0 of the Problem-Solving Engine reached its major implementation goal.

The project began with the idea that diverse problems might share a common structural representation.

That design was translated into a functioning C++ system containing:

- reusable problem models,
- deterministic dependency logic,
- state handling,
- constraint handling,
- next-step selection,
- failure diagnosis,
- JSON processing,
- persistence,
- API integration,
- AI safety boundaries,
- and a complete natural-language-to-validated-structure pipeline.

The final automated test suite reports:

    15/15 tests passed
    0 tests failed

A real OpenAI API end-to-end run also successfully converted a natural-language mechanical problem into structured `ProblemData` that passed engine validation.

At the same time, the project deliberately avoids claiming more than the evidence supports.

Broad cross-domain effectiveness has not yet been demonstrated through systematic evaluation.

The current result is therefore best summarized as:

> The architecture is implemented, testable, and operational. AI can propose structured interpretations of open-ended problems, while the deterministic C++ engine retains authority over what the system accepts and how engine decisions are made.

The core Version 1.0 principle remains:

> **AI proposes structure; Engine makes decisions.**