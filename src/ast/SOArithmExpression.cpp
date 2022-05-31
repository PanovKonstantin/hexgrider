#include "SOArithmExpression.h"

using namespace ast;

SOArithmExpression::SOArithmExpression(std::unique_ptr<Node> lvalue_,
                                       Operator op_,
                                       std::unique_ptr<Node> rvalue_)
    : op(op_)
{
    lvalue = std::move(lvalue_);
    rvalue = std::move(rvalue_);
}

std::string SOArithmExpression::toString(int depth) const
{
    switch (op)
    {
    case Operator::Add:
        return prefix(depth) + "Add Expression\n" +
               lvalue->toString(depth + 1) +
               rvalue->toString(depth + 1);
    case Operator::Substruct:
        return prefix(depth) + "Substruct Expression\n" +
               lvalue->toString(depth + 1) +
               rvalue->toString(depth + 1);
    default:
        throw std::runtime_error("Arithmetical Expression without operator");
        return "";
    };
}