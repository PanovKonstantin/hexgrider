#ifndef TKOM_AST_RETURN_STATEMENT_H_
#define TKOM_AST_RETURN_STATEMENT_H_

#include <string>
#include <memory>
#include "Node.h"

namespace ast
{
class ReturnStatement : public Node
{
public:
    ReturnStatement(std::unique_ptr<Node> expr_);

    std::string toString(int depth = 0) const override;
    std::unique_ptr<ast::Node> expr;
};
} //namespace ast




#endif // TKOM_AST_RETURN_STATEMENT_H_