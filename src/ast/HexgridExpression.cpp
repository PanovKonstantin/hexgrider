#include "HexgridExpression.h"

using namespace ast;

HexgridExpression::HexgridExpression(std::unique_ptr<Node> lvalue_,
                                       Operator op_,
                                       std::unique_ptr<Node> rvalue_)
    : op(op_)
{
    lvalue = std::move(lvalue_);
    rvalue = std::move(rvalue_);
}

std::string HexgridExpression::toString(int depth) const
{
    switch (op)
    {
    case Operator::On:
        return prefix(depth) + "Hexgrid On Expression\n" +
               lvalue->toString(depth + 1) +
               rvalue->toString(depth + 1);
    case Operator::By:
        return prefix(depth) + "Hexgrid By Expression\n" +
               lvalue->toString(depth + 1) +
               rvalue->toString(depth + 1);
    case Operator::Beside:
        return prefix(depth) + "Hexgrid Beside Expression\n" +
               lvalue->toString(depth + 1) +
               rvalue->toString(depth + 1);
    default:
        throw std::runtime_error("Hexgrid Expression without operator");
        return "";
    };
}