#include "LogicalNegation.h"

using namespace ast;

LogicalNegation::LogicalNegation(std::unique_ptr<Node>  value_)
{
    value = std::move(value_);
}

std::string LogicalNegation::toString(int depth) const
{
    return prefix(depth) + "Logical Negation Expression\n" + value->toString(depth + 1);
}