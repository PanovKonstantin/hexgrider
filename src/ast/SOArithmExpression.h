#ifndef TKOM_AST_SECOND_ORDER_ARITHMETICAL_EXPRESSION_H_
#define TKOM_AST_SECOND_ORDER_ARITHMETICAL_EXPRESSION_H_

#include <memory>
#include "Node.h"

namespace ast
{
class SOArithmExpression : public Node
{
public:
    enum class Operator
    {
        Add,
        Substruct
    };
    SOArithmExpression(std::unique_ptr<Node> lvalue_,
                       Operator op_,
                       std::unique_ptr<Node> rvalue_);

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> lvalue;
    Operator op;
    std::unique_ptr<Node> rvalue;
};
} //namespace ast

#endif // TKOM_AST_SECOND_ORDER_ARITHMETICAL_EXPRESSION_H_