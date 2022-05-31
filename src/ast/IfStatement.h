#ifndef TKOM_AST_IF_STATEMENT_H_
#define TKOM_AST_IF_STATEMENT_H_

#include <string>
#include <memory>
#include <vector>
#include "Node.h"

namespace ast
{
class IfStatement : public Node
{
public:
    IfStatement( 
                std::unique_ptr<Node> if_block_,
                std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> elif_blocks_,
                std::unique_ptr<Node> else_block_);

    std::string toString(int depth = 0) const override;
    std::unique_ptr<ast::Node> if_block;
    std::unique_ptr<ast::Node> else_block;
    std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> elif_blocks;
};
} //namespace ast




#endif // TKOM_AST_IF_STATEMENT_H_