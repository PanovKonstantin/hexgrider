#ifndef TKOM_AST_REMOVE_STATEMENT_H_
#define TKOM_AST_REMOVE_STATEMENT_H_

#include <memory>
#include "Node.h"

namespace ast
{
class RemoveStatement : public Node
{
public:
    RemoveStatement(std::unique_ptr<Node> position_,
                    std::unique_ptr<Node> grid_);

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> position;
    std::unique_ptr<Node> grid;
};
} //namespace ast

#endif // TKOM_AST_REMOVE_STATEMENT_H_