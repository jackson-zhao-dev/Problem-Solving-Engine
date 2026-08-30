#include "AIProblemValidator.h"

#include "ProblemValidator.h"

bool isAIProblemDataValid(
    const ProblemData& problemData
)
{
    if (!isProblemDataValid(problemData))
    {
        return false;
    }

    for (const Node& node : problemData.nodes)
    {
        if (node.state != State::NotStarted)
        {
            return false;
        }

        if (node.validation.result
            != ValidationResult::Pending)
        {
            return false;
        }
    }

    for (const Constraint& constraint :
         problemData.constraints)
    {
        if (constraint.status
            != ConstraintStatus::Unknown)
        {
            return false;
        }
    }

    return true;
}