#ifndef TKOM_AST_HEXGRID_CELL_H_
#define TKOM_AST_HEXGRID_CELL_H_

#include <string>
#include <memory>
#include "Node.h"

namespace ast
{
class HexgridCell : public Node
{
public:
    HexgridCell(std::unique_ptr<Node> value_,
                std::unique_ptr<Node> pos_);

    std::string toString(int depth = 0) const override;
    std::unique_ptr<ast::Node> pos;
    std::unique_ptr<ast::Node> value;
};
} //namespace ast




#endif // TKOM_AST_HEXGRID_CELL_H_