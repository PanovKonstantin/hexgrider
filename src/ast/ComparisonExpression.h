#ifndef TKOM_AST_COMPARISON_EXPRESSION_H_
#define TKOM_AST_COMPARISON_EXPRESSION_H_

#include <memory>
#include "Node.h"

namespace ast
{
class ComparisonExpression : public Node
{
public:
    enum class Operator
    {
        Less,
        LessOrEqual,
        Greater,
        GreaterOrEqual,
        Equal,
        NotEqual
    };
    ComparisonExpression(std::unique_ptr<Node> lvalue_,
                       Operator op_,
                       std::unique_ptr<Node> rvalue_);

    std::string toString(int depth = 0) const override;
    std::string toString(Operator op_)  const;
    std::unique_ptr<Node> lvalue;
    Operator op;
    std::unique_ptr<Node> rvalue;
};
} //namespace ast

#endif // TKOM_AST_COMPARISON_EXPRESSION_H_