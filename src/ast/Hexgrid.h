#ifndef TKOM_AST_HEXGRID_H_
#define TKOM_AST_HEXGRID_H_

#include <string>
#include <memory>
#include <vector>
#include "Node.h"

namespace ast
{
class Hexgrid : public Node
{
public:
    Hexgrid(std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> cells_);

    std::string toString(int depth = 0) const override;
    std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> cells;
};
} //namespace ast




#endif // TKOM_AST_HEXGRID_H_