#include "include/HexgridCell.h"

using namespace ast;

HexgridCell::HexgridCell(std::unique_ptr<Node> value_, std::unique_ptr<Node> pos_)
{
    pos = std::move(pos_);
    value = std::move(value_);
}

std::string HexgridCell::toString(int depth) const
{
    return prefix(depth) + "Hexgrid Cell\n" +
           value->toString(depth + 1) + 
           pos->toString(depth + 1);
}