#include "ProblemStorage.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

#include "ProblemLoader.h"
#include "ProblemSerializer.h"
#include "ProblemValidator.h"

void saveProblemDataToFile(
    const ProblemData& problemData,
    const std::string& filePath
)
{
    if (!isProblemDataValid(problemData))
    {
        throw std::invalid_argument(
            "Cannot save invalid problem data."
        );
    }

    std::ofstream outputFile(filePath);

    if (!outputFile)
    {
        throw std::runtime_error(
            "Failed to open file for writing: "
            + filePath
        );
    }

    outputFile <<
        serializeProblemDataToJson(problemData);

    if (!outputFile)
    {
        throw std::runtime_error(
            "Failed to write problem data: "
            + filePath
        );
    }
}

ProblemData loadProblemDataFromFile(
    const std::string& filePath
)
{
    std::ifstream inputFile(filePath);

    if (!inputFile)
    {
        throw std::runtime_error(
            "Failed to open file for reading: "
            + filePath
        );
    }

    std::ostringstream buffer;
    buffer << inputFile.rdbuf();

    if (inputFile.bad())
    {
        throw std::runtime_error(
            "Failed to read problem data: "
            + filePath
        );
    }

    return loadProblemDataFromJson(
        buffer.str()
    );
}