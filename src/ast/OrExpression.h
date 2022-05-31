#ifndef TKOM_AST_OR_EXPRESSION_H_
#define TKOM_AST_OR_EXPRESSION_H_

#include <memory>
#include "Node.h"

namespace ast
{
class OrExpression : public Node
{
public:
    OrExpression(std::unique_ptr<Node> lvalue_,
                 std::unique_ptr<Node> rvalue_);

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> lvalue;
    std::unique_ptr<Node> rvalue;
};
} //namespace ast

#endif // TKOM_AST_OR_EXPRESSION_H_