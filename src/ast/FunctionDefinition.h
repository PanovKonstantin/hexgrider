#ifndef TKOM_AST_FUNCTION_DEFINITION_H_
#define TKOM_AST_FUNCTION_DEFINITION_H_

#include <string>
#include <memory>
#include <vector>
#include "Node.h"

namespace ast
{
class FunctionDefinition : public Node
{
public:
    FunctionDefinition( 
                std::unique_ptr<Node> fun_,
                std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> params_,
                std::unique_ptr<Node> scope_);

    std::string toString(int depth = 0) const override;
    std::unique_ptr<ast::Node> fun;
    std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> params;
    std::unique_ptr<ast::Node> scope;
};
} //namespace ast




#endif // TKOM_AST_IF_STATEMENT_H_