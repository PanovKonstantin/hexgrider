#include "include/ForeachStatement.h"

using namespace ast;

ForeachStatement::ForeachStatement(
                std::unique_ptr<Node> iterator_,
                std::unique_ptr<Node> iterated_,
                std::unique_ptr<Node> scope_)
{
    iterator = std::move(iterator_);
    iterated = std::move(iterated_);
    scope = std::move(scope_);
}

std::string ForeachStatement::toString(int depth) const
{
    return prefix(depth) + "Foreach Statement\n" + 
            iterator->toString(depth + 1) + 
            iterated->toString(depth + 1) + 
            scope->toString(depth + 1);
}