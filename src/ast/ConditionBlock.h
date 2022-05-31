#ifndef TKOM_AST_IF_BLOCK_H_
#define TKOM_AST_IF_BLOCK_H_

#include <string>
#include <memory>
#include <vector>
#include "Node.h"

namespace ast
{
class ConditionBlock : public Node
{
public:
    ConditionBlock(std::unique_ptr<Node> condition_,
            std::unique_ptr<Node> scope_);

    std::string toString(int depth = 0) const override;
    std::unique_ptr<ast::Node> condition;
    std::unique_ptr<ast::Node> scope;
};
} //namespace ast




#endif // TKOM_AST_IF_BLOCK_H_