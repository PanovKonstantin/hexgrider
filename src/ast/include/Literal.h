#ifndef TKOM_AST_LITERAL_H_
#define TKOM_AST_LITERAL_H_

#include <string>
#include "Node.h"

namespace ast
{
class Literal : public Node
{
public:
    Literal();
    std::string toString(int depth = 0) const override;
};
} //namespace ast




#endif //TKOM_AST_LITERAL_H_