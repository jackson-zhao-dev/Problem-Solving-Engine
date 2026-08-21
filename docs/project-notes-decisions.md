# Project Notes and Decisions

Last Updated: 2026-08-21

## Project Memory System

- The Project Design Document is the source of truth for the overall architecture and project goals.
- `implementation-progress.md` records the current implementation status and next step.
- This file records important technical decisions, risks, and future work that must not be forgotten.
- Git commit history records how the project changes over time.

## Core Project Principle

AI proposes structure; Engine makes decisions.

The AI layer may interpret natural-language problems and propose structured data, but the deterministic C++ engine remains responsible for validation, state transitions, dependency analysis, next-step selection, and failure diagnosis.

## Current Decisions

- Keep the GitHub repository public.
- Never commit API keys, passwords, or other secrets to GitHub.
- OpenAI API keys must later be stored using environment variables or another ignored local configuration method.
- Version 1.0 should focus on the designed core system and avoid unnecessary feature expansion.
- Build the six core data models before moving into the main engine logic.

## Future Required Work

- Complete all six core data models:
  - State
  - Validation
  - Constraint
  - Dependency
  - Node
  - Goal
- Add a CMake build system when the project begins using multiple source files.
- Add automated tests when engine logic development begins.
- Implement state-transition rules.
- Implement dependency validation and cycle detection.
- Implement constraint checking.
- Implement the next-step selection algorithm.
- Implement failure diagnosis.
- Define the JSON data contract.
- Add project save/load support.
- Prepare API key protection and spending limits before AI integration.
- Integrate the external AI API only after the engine core is working.
- Create multiple example problems for testing.
- Improve the README as the project becomes usable.
- Write the technical report using actual implementation and test results.

## Development Rules

- Compile and test each meaningful implementation step before committing it.
- Commit and push coherent milestones to GitHub.
- Update `implementation-progress.md` after important milestones.
- Review the project for missing work, risks, and technical debt at the end of each major phase.