#include "include/ReturnStatement.h"

using namespace ast;

ReturnStatement::ReturnStatement(std::unique_ptr<Node> expr_)
{
    expr = std::move(expr_);
}

std::string ReturnStatement::toString(int depth) const
{
    return prefix(depth) + "Return Statement\n" + 
            expr->toString(depth + 1);
}