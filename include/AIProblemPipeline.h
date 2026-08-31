#pragma once

#include <string>

#include "AIProblemNormalizer.h"
#include "ProblemData.h"

class AIProblemPipeline
{
public:
    explicit AIProblemPipeline(
        AIProblemNormalizer& normalizer
    );

    ProblemData process(
        const std::string& userInput
    );

private:
    AIProblemNormalizer& normalizer_;
};