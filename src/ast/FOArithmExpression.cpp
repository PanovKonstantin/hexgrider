#include "FOArithmExpression.h"

using namespace ast;

FOArithmExpression::FOArithmExpression(std::unique_ptr<Node> lvalue_,
                                       Operator op_,
                                       std::unique_ptr<Node> rvalue_)
    : op(op_)
{
    lvalue = std::move(lvalue_);
    rvalue = std::move(rvalue_);
}

std::string FOArithmExpression::toString(int depth) const
{
    switch (op)
    {
    case Operator::Multiply:
        return prefix(depth) + "Multiply Expression\n" +
               lvalue->toString(depth + 1) +
               rvalue->toString(depth + 1);
    case Operator::Divide:
        return prefix(depth) + "Divide Expression\n" +
               lvalue->toString(depth + 1) +
               rvalue->toString(depth + 1);
    default:
        throw std::runtime_error("Arithmetical Expression without operator");
        return "";
    };
}