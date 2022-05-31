#ifndef TKOM_AST_TEXT_LITERAL_H_
#define TKOM_AST_TEXT_LITERAL_H_

#include "Node.h"

namespace ast
{
class TextLiteral : public Node
{
public:
    TextLiteral(std::string value_);

    std::string toString(int depth = 0) const override;
    std::string value;
};
} //namespace ast




#endif //TKOM_AST_TEXT_LITERAL_H_