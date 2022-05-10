#ifndef TKOM_AST_IDENTIFIER_H_
#define TKOM_AST_IDENTIFIER_H_

#include <string>
#include "Node.h"

namespace ast
{
class Identifier : public Node
{
public:
    Identifier(std::string value_);

    std::string toString(int depth = 0) const override;
    std::string value;
};
} //namespace ast




#endif // TKOM_AST_IDENTIFIER_H_