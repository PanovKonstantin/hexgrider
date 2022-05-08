#include "include/ArithmeticalNegation.h"

using namespace ast;

ArithmeticalNegation::ArithmeticalNegation(std::unique_ptr<Node>  value_)
{
    value = std::move(value_);
}

std::string ArithmeticalNegation::toString(int depth) const
{
    return prefix(depth) + "Arithmetical Negation Expression\n" + value->toString(depth + 1);
}