# Project Notes and Decisions

Last Updated: 2026-08-30

## Project Memory System

- The original Project Design Document preserves the Version 1.0 design baseline and initial project goals.
- `implementation-progress.md` records the actual implementation status and completed engineering milestones.
- This file records important technical decisions, architectural boundaries, risks, resolved work, and future work that must not be forgotten.
- Git commit history records how the project changed over time.
- README.md provides the current user-facing project overview, setup instructions, architecture summary, and usage instructions.
- The final Technical Project Report will summarize the completed implementation, engineering decisions, validation results, limitations, and future work.

---

## Core Project Principle

> **AI proposes structure; Engine makes decisions.**

The AI layer may interpret natural-language problems and propose structured data, but the deterministic C++ engine remains responsible for validation, state transitions, dependency analysis, next-step selection, failure diagnosis, and other engine-controlled decisions.

This principle is a permanent architectural boundary unless a future design revision explicitly changes it.

---

## Historical Decisions Preserved

The following decisions were made during early development and remain important to the project history:

- Keep the GitHub repository public.
- Never commit API keys, passwords, or other secrets to GitHub.
- Store OpenAI API credentials in environment variables or ignored local configuration.
- Keep Version 1.0 focused on the designed core system rather than expanding features without a defined requirement.
- Build the deterministic core before integrating external AI.
- Build the core data models before implementing higher-level engine logic.
- Add automated tests as engine logic becomes substantial.
- Compile and test meaningful implementation steps before committing them.
- Commit and push coherent milestones to GitHub.
- Maintain an implementation-progress document rather than relying only on memory or commit history.

These decisions guided the completed implementation.

---

## Resolved Required Work

The early project plan identified the following required work. These items are now complete.

### Core Data Models

Completed:

- State
- Validation
- Constraint
- Dependency
- Node
- Goal
- ProblemData integration

### Engineering Foundation

Completed:

- CMake build system
- C++17 project configuration
- Automated CTest infrastructure
- Git and GitHub workflow
- Multi-file project structure

### Deterministic Engine Logic

Completed:

- State-transition rules
- Ready-state evaluation
- Dependency validation
- Invalid-reference handling
- Self-dependency rejection
- Dependency cycle detection
- Constraint checking
- Next-step selection algorithm
- Deterministic tie-breaking
- Failure diagnosis
- Root-suspect tracing
- Downstream impact discovery

### Structured Data and Persistence

Completed:

- JSON data contract
- JSON parsing
- Semantic validation
- Problem loading
- Problem serialization
- Problem storage
- Save/load round-trip support

### AI Safety and Integration

Completed:

- API-key protection through environment variables
- Local `.env` configuration
- `.env` exclusion from Git
- OpenAI configuration validation
- Request-count guard
- Input-size guard
- HTTP abstraction
- libcurl transport
- OpenAI Responses API client
- Fake HTTP testing
- Live API connectivity validation

### AI-Assisted Problem Interpretation

Completed:

- AI output contract
- Controlled prompt builder
- AI problem normalizer
- AI problem parser
- AI-specific validator
- AI problem pipeline
- Natural-language command-line input
- Explicit `--ai` mode
- End-to-end live validation

### Documentation

Completed:

- Expanded README
- Implementation progress ledger
- Original Version 1.0 design document
- JSON data contract
- AI output contract

---

## Current Technical Decisions

### 1. Preserve the Deterministic Decision Boundary

AI-generated content is treated as an external proposal.

It must not automatically become trusted engine state.

AI output must pass:

AI generation  
→ parsing  
→ AI-specific validation  
→ deterministic validation  
→ accepted `ProblemData`

The engine remains authoritative.

### 2. AI Mode Must Remain Explicit

Normal program execution must not unintentionally make live API requests.

AI-assisted behavior requires explicit invocation through:

`--ai`

This provides a simple operational safety boundary.

### 3. Secrets Must Remain Outside Source Control

Real API keys must never appear in:

- source code,
- documentation examples,
- committed `.env` files,
- Git history.

`.env.example` may document variable names but must not contain real credentials.

### 4. Live API Usage Must Remain Guarded

Live requests are subject to configurable safeguards including:

- maximum requests per run,
- maximum input length.

Future live-AI features should continue using the same safety path rather than bypassing the usage guard.

### 5. AI Output Must Remain Untrusted

Malformed JSON, incomplete responses, unauthorized states, invalid dependencies, invalid constraints, and other invalid structures must be rejected.

Successful API communication alone does not mean the engine should accept the result.

### 6. Automated Tests Should Avoid Unnecessary Live Requests

Most AI behavior should be tested through fake or controlled HTTP infrastructure.

Live API requests should be reserved for deliberate connectivity and end-to-end validation.

### 7. The Original Design Document Should Remain Historical

`Problem-Solving-Engine_Project-Design-Document_v1.0_EN.md` should remain the original design baseline rather than being rewritten to look as if every implemented feature had been known in advance.

The difference between the original design and the completed implementation is useful engineering evidence.

### 8. Documentation Should Reflect Actual Behavior

README and implementation-progress documentation should describe implemented behavior rather than planned behavior.

Future major milestones should update the implementation ledger.

---

## Current Risks and Limitations

### AI Interpretation Quality

AI may:

- misunderstand an open-ended problem,
- omit useful nodes,
- create unnecessary nodes,
- infer inappropriate constraints,
- produce malformed or semantically invalid structured output.

The deterministic validation boundary reduces risk but cannot guarantee that every accepted decomposition is the best decomposition.

### Domain Knowledge Limitations

The deterministic engine does not contain complete mechanical, electrical, software, robotics, or other domain expertise.

Its reasoning is structural.

AI may provide domain interpretation, but the engine cannot independently verify every real-world domain claim.

### Failure Diagnosis Scope

Failure Diagnosis identifies structurally suspicious upstream points.

It does not prove physical or causal responsibility in every real system.

### Next-Step Algorithm Scope

The current scoring system is intentionally simple and deterministic.

It is not claimed to be a globally optimal scheduling or planning algorithm.

### Constraint Reasoning Scope

Constraints are represented and validated, but the engine does not perform complete mathematical or physical reasoning about every possible constraint.

### Live API Dependency

AI-assisted mode depends on:

- valid local credentials,
- network connectivity,
- a compatible configured model,
- external API availability.

The deterministic engine itself should remain usable independently of live API availability.

---

## Remaining Project Completion Work

The major implementation stages are complete.

The remaining Version 1.0 closeout work is primarily documentation, evaluation, and hardening.

### Required

- Write the final Technical Project Report using actual implementation results.
- Compare the original design against the final implementation.
- Record important deviations between planned and implemented architecture.
- Summarize automated testing results.
- Record the successful live end-to-end validation.
- Document known limitations and unresolved risks.
- Perform a repository-wide documentation consistency review.
- Perform a final repository consistency check.
- Run the complete automated test suite after final documentation changes.

### Recommended Evaluation Work

The original design proposed evaluation across multiple meaningfully different problem types.

Before making strong claims about generality, test the completed AI-assisted pipeline with additional domains such as:

- a mechanical problem,
- a software problem,
- an electronics or robotics problem.

Record both successful and unsuccessful outputs.

This evidence can strengthen the final Technical Project Report.

### Optional Future Engineering

Only begin a new engineering stage if it serves a defined project goal.

Possible directions include:

- deeper connection between AI-generated `ProblemData` and interactive engine execution,
- richer command-line interaction,
- additional failure-path tests,
- timeout and retry policy,
- improved diagnostics and observability,
- richer persistence workflows,
- additional reasoning strategies,
- broader cross-domain evaluation.

Do not add AI functionality merely to make the project appear more complex.

---

## Version 1.0 Closeout Principle

Version 1.0 should be judged by whether the implemented system demonstrates a coherent engineering idea:

> An AI layer can interpret open-ended input and propose a structured problem representation, while a deterministic C++ engine retains authority over validation and decision logic.

The remaining work should focus on documenting and evaluating that claim rather than expanding the system without evidence or a defined requirement.