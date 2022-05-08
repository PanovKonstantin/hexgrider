#ifndef TKOM_AST_INTEGER_LITERAL_H_
#define TKOM_AST_INTEGER_LITERAL_H_

#include <string>
#include "Literal.h"

namespace ast
{
class IntegerLiteral : public Literal
{
public:
    IntegerLiteral(int value_);

    std::string toString(int depth = 0) const override;
    int value;
};
} //namespace ast




#endif //TKOM_AST_INTEGER_LITERAL_H_