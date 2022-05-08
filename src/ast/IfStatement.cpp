#include "include/IfStatement.h"

using namespace ast;

IfStatement::IfStatement(std::unique_ptr<Node> if_block_,
                         std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> elif_blocks_,
                         std::unique_ptr<Node> else_block_)
{
    if_block = std::move(if_block_);
    else_block = std::move(else_block_);
    elif_blocks = std::move(elif_blocks_);
}

std::string IfStatement::toString(int depth) const
{
    std::string ret_str = prefix(depth) + "If Statement\n" + 
                          if_block->toString(depth + 1);
    if (elif_blocks)
        for(auto const& elif_block: *elif_blocks)
            ret_str += elif_block->toString(depth + 1);
    if (else_block)
        ret_str += else_block->toString(depth + 1);
    return ret_str;
}