#include "include/ComparisonExpression.h"

using namespace ast;

ComparisonExpression::ComparisonExpression(std::unique_ptr<Node> lvalue_,
                                       Operator op_,
                                       std::unique_ptr<Node> rvalue_)
    : op(op_)
{
    lvalue = std::move(lvalue_);
    rvalue = std::move(rvalue_);
}

std::string ComparisonExpression::toString(int depth) const
{
    return prefix(depth) + toString(op) +
            lvalue->toString(depth + 1) +
            rvalue->toString(depth + 1);
}

std::string ComparisonExpression::toString(Operator op_) const
{
    switch (op_)
    {
    case Operator::Less:            return "Less Expression\n";
    case Operator::LessOrEqual:     return "LessOrEqual Expression\n";
    case Operator::Greater:         return "Greater Expression\n";
    case Operator::GreaterOrEqual:  return "GreaterOrEqual Expression\n";
    case Operator::Equal:           return "Equal Expression\n";
    case Operator::NotEqual:        return "NotEqual Expression\n";
    default:
        throw std::runtime_error("Unkown comparison operator");
        return "";
    }
}