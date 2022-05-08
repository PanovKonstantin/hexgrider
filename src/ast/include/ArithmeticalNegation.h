#ifndef TKOM_AST_ARITHMETICAL_NEGATION_H_
#define TKOM_AST_ARITHMETICAL_NEGATION_H_

#include <memory>
#include "Node.h"

namespace ast
{
class ArithmeticalNegation : public Node
{
public:
    ArithmeticalNegation(std::unique_ptr<Node> value_);

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> value;
};
} //namespace ast

#endif // TKOM_AST_ARITHMETICAL_NEGATION_H_