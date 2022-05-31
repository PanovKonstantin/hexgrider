#include "MoveStatement.h"

using namespace ast;

MoveStatement::MoveStatement(std::unique_ptr<Node> position_source_,
                             std::unique_ptr<Node> grid_source_,
                             std::unique_ptr<Node> grid_target_,
                             std::unique_ptr<Node> position_target_)
{
    position_source = std::move(position_source_);
    position_target = std::move(position_target_);
    grid_source = std::move(grid_source_);
    grid_target = std::move(grid_target_);
}

std::string MoveStatement::toString(int depth) const
{
    return prefix(depth) + "Move Statement\n" +
            position_source->toString(depth + 1) + 
            grid_source->toString(depth + 1) + 
            grid_target->toString(depth + 1) + 
            position_target->toString(depth + 1);
}