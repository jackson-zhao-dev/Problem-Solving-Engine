cmake_minimum_required(VERSION 3.16)

project(ProblemSolvingEngine LANGUAGES CXX)
find_package(CURL REQUIRED)

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
    src/OpenAIConfig.cpp
    src/OpenAIUsageGuard.cpp
    src/OpenAIClient.cpp
    src/CurlHttpClient.cpp
    src/ProblemPromptBuilder.cpp
    src/AIProblemNormalizer.cpp
    src/AIProblemParser.cpp
    src/AIProblemValidator.cpp
)

target_include_directories(problem-solving-engine PRIVATE
    include
)

target_link_libraries(problem-solving-engine PRIVATE
    nlohmann_json::nlohmann_json
    CURL::libcurl
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

add_executable(openai-config-tests
    tests/test_openai_config.cpp
    src/OpenAIConfig.cpp
)

target_include_directories(openai-config-tests PRIVATE
    include
)

add_test(
    NAME OpenAIConfigTests
    COMMAND openai-config-tests
)

add_executable(openai-usage-guard-tests
    tests/test_openai_usage_guard.cpp
    src/OpenAIUsageGuard.cpp
)

target_include_directories(openai-usage-guard-tests PRIVATE
    include
)

add_test(
    NAME OpenAIUsageGuardTests
    COMMAND openai-usage-guard-tests
)

add_executable(openai-client-tests
    tests/test_openai_client.cpp
    src/OpenAIClient.cpp
    src/OpenAIUsageGuard.cpp
)

target_include_directories(openai-client-tests PRIVATE
    include
)

target_link_libraries(openai-client-tests PRIVATE
    nlohmann_json::nlohmann_json
)

add_test(
    NAME OpenAIClientTests
    COMMAND openai-client-tests
)

add_executable(problem-prompt-builder-tests
    tests/test_problem_prompt_builder.cpp
    src/ProblemPromptBuilder.cpp
)

target_include_directories(problem-prompt-builder-tests PRIVATE
    include
)

add_test(
    NAME ProblemPromptBuilderTests
    COMMAND problem-prompt-builder-tests
)

add_executable(ai-problem-normalizer-tests
    tests/test_ai_problem_normalizer.cpp
    src/AIProblemNormalizer.cpp
    src/ProblemPromptBuilder.cpp
    src/OpenAIClient.cpp
    src/OpenAIUsageGuard.cpp
)

target_include_directories(ai-problem-normalizer-tests PRIVATE
    include
)

target_link_libraries(ai-problem-normalizer-tests PRIVATE
    nlohmann_json::nlohmann_json
)

add_test(
    NAME AIProblemNormalizerTests
    COMMAND ai-problem-normalizer-tests
)

add_executable(ai-problem-parser-tests
    tests/test_ai_problem_parser.cpp
    src/AIProblemParser.cpp
    src/JsonParser.cpp
)

target_include_directories(ai-problem-parser-tests PRIVATE
    include
)

target_link_libraries(ai-problem-parser-tests PRIVATE
    nlohmann_json::nlohmann_json
)

add_test(
    NAME AIProblemParserTests
    COMMAND ai-problem-parser-tests
)

add_executable(ai-problem-validator-tests
    tests/test_ai_problem_validator.cpp
    src/AIProblemValidator.cpp
    src/ProblemValidator.cpp
    src/Engine.cpp
)

target_include_directories(ai-problem-validator-tests PRIVATE
    include
)

add_test(
    NAME AIProblemValidatorTests
    COMMAND ai-problem-validator-tests
)