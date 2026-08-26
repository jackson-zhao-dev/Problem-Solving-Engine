cmake_minimum_required(VERSION 3.16)

project(ProblemSolvingEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(nlohmann_json)

add_executable(problem-solving-engine
    src/main.cpp
    src/Engine.cpp
    src/JsonParser.cpp
    src/ProblemValidator.cpp
    src/ProblemLoader.cpp
    src/ProblemSerializer.cpp
    src/ProblemStorage.cpp
)

target_include_directories(problem-solving-engine PRIVATE
    include
)

target_link_libraries(problem-solving-engine PRIVATE
    nlohmann_json::nlohmann_json
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

add_executable(json-parser-tests
    tests/test_json_parser.cpp
    src/JsonParser.cpp
)

target_include_directories(json-parser-tests PRIVATE
    include
)

target_link_libraries(json-parser-tests PRIVATE
    nlohmann_json::nlohmann_json
)

add_test(
    NAME JsonParserTests
    COMMAND json-parser-tests
)

add_executable(problem-validator-tests
    tests/test_problem_validator.cpp
    src/ProblemValidator.cpp
    src/Engine.cpp
)

target_include_directories(problem-validator-tests PRIVATE
    include
)

add_test(
    NAME ProblemValidatorTests
    COMMAND problem-validator-tests
)

add_executable(problem-loader-tests
    tests/test_problem_loader.cpp
    src/ProblemLoader.cpp
    src/JsonParser.cpp
    src/ProblemValidator.cpp
    src/Engine.cpp
)

target_include_directories(problem-loader-tests PRIVATE
    include
)

target_link_libraries(problem-loader-tests PRIVATE
    nlohmann_json::nlohmann_json
)

add_test(
    NAME ProblemLoaderTests
    COMMAND problem-loader-tests
)

add_executable(problem-serializer-tests
    tests/test_problem_serializer.cpp
    src/ProblemSerializer.cpp
    src/JsonParser.cpp
    src/ProblemValidator.cpp
    src/Engine.cpp
)

target_include_directories(problem-serializer-tests PRIVATE
    include
)

target_link_libraries(problem-serializer-tests PRIVATE
    nlohmann_json::nlohmann_json
)

add_test(
    NAME ProblemSerializerTests
    COMMAND problem-serializer-tests
)

add_executable(problem-storage-tests
    tests/test_problem_storage.cpp
    src/ProblemStorage.cpp
    src/ProblemLoader.cpp
    src/ProblemSerializer.cpp
    src/JsonParser.cpp
    src/ProblemValidator.cpp
    src/Engine.cpp
)

target_include_directories(problem-storage-tests PRIVATE
    include
)

target_link_libraries(problem-storage-tests PRIVATE
    nlohmann_json::nlohmann_json
)

add_test(
    NAME ProblemStorageTests
    COMMAND problem-storage-tests
)