cmake_minimum_required(VERSION 3.16)

project(ProblemSolvingEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(problem-solving-engine
    src/main.cpp
    src/Engine.cpp
)

target_include_directories(problem-solving-engine PRIVATE
    include
)

enable_testing()

add_executable(core-model-tests
    tests/test_core_models.cpp
)

target_include_directories(core-model-tests PRIVATE
    include
)

add_test(
    NAME CoreModelTests
    COMMAND core-model-tests
)

add_executable(engine-logic-tests
    tests/test_engine_logic.cpp
    src/Engine.cpp
)

target_include_directories(engine-logic-tests PRIVATE
    include
)

add_test(
    NAME EngineLogicTests
    COMMAND engine-logic-tests
)