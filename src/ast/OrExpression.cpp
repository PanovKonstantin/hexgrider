#include "OrExpression.h"

using namespace ast;

OrExpression::OrExpression(std::unique_ptr<Node> lvalue_,
                             std::unique_ptr<Node> rvalue_)
{
    lvalue = std::move(lvalue_);
    rvalue = std::move(rvalue_);
}

std::string OrExpression::toString(int depth) const
{
    return prefix(depth) + "Or Expression\n" +
            lvalue->toString(depth + 1) +
            rvalue->toString(depth + 1);
}