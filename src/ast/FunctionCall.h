#ifndef TKOM_AST_FUNCTION_CALL_H_
#define TKOM_AST_FUNCTION_CALL_H_

#include <memory>
#include <vector>
#include "Node.h"

namespace ast
{
class FunctionCall : public Node
{
public:
    FunctionCall(std::unique_ptr<Node> func_, 
                 std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> args_);

    std::string toString(int depth = 0) const override;

    std::unique_ptr<Node> func;
    std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> args;
};
} //namespace ast




#endif // TKOM_AST_FUNCTION_CALL_H_