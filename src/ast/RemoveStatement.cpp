#include "RemoveStatement.h"

using namespace ast;

RemoveStatement::RemoveStatement(std::unique_ptr<Node> position_,
                                 std::unique_ptr<Node> grid_)
{
    position = std::move(position_);
    grid = std::move(grid_);
}

std::string RemoveStatement::toString(int depth) const
{
    return prefix(depth) + "Remove Statement\n" +
            position->toString(depth + 1) + 
            grid->toString(depth + 1);
}