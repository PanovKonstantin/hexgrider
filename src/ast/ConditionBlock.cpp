#include "ConditionBlock.h"

using namespace ast;

ConditionBlock::ConditionBlock(std::unique_ptr<Node> condition_,
                 std::unique_ptr<Node> scope_)
{
    condition = std::move(condition_);
    scope = std::move(scope_);
}

std::string ConditionBlock::toString(int depth) const
{
    return prefix(depth) + "Condition Block\n" + 
            condition->toString(depth + 1) + 
            scope->toString(depth + 1);
}