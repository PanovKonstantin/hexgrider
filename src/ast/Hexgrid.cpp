#include "Hexgrid.h"

using namespace ast;

Hexgrid::Hexgrid(std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> cells_)
{
    cells = std::move(cells_);
}

std::string Hexgrid::toString(int depth) const
{
    std::string ret_str = prefix(depth) + "Hexgrid\n";
    for(auto const& cell: *cells)
    {
        ret_str += cell->toString(depth + 1);
    }
    return ret_str;
}