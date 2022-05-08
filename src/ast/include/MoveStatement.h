#ifndef TKOM_AST_MOVE_STATEMENT_H_
#define TKOM_AST_MOVE_STATEMENT_H_

#include <memory>
#include "Node.h"

namespace ast
{
class MoveStatement : public Node
{
public:
    MoveStatement(std::unique_ptr<Node> position_source_,
                  std::unique_ptr<Node> grid_source_,
                  std::unique_ptr<Node> grid_target_,
                  std::unique_ptr<Node> position_target_);

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> position_source;
    std::unique_ptr<Node> position_target;
    std::unique_ptr<Node> grid_source;
    std::unique_ptr<Node> grid_target;
};
} //namespace ast

#endif // TKOM_AST_MOVE_STATEMENT_H_