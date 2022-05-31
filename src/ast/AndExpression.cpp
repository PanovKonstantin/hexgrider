#include "AndExpression.h"

using namespace ast;

AndExpression::AndExpression(std::unique_ptr<Node> lvalue_,
                             std::unique_ptr<Node> rvalue_)
{
    lvalue = std::move(lvalue_);
    rvalue = std::move(rvalue_);
}

std::string AndExpression::toString(int depth) const
{
    return prefix(depth) + "And Expression\n" +
            lvalue->toString(depth + 1) +
            rvalue->toString(depth + 1);
}