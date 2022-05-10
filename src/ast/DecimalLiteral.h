#ifndef TKOM_AST_DECIMAL_LITERAL_H_
#define TKOM_AST_DECIMAL_LITERAL_H_

#include "Node.h"

namespace ast
{
class DecimalLiteral : public Node
{
public:
    DecimalLiteral(double value_);

    std::string toString(int depth = 0) const override;
    double value;
};
} //namespace ast




#endif //TKOM_AST_DECIMAL_LITERAL_H_