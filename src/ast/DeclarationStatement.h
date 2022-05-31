#ifndef TKOM_AST_DECLARATION_STATEMENT_H_
#define TKOM_AST_DECLARATION_STATEMENT_H_

#include <memory>
#include "Node.h"

namespace ast
{
class DeclarationStatement : public Node
{
public:
    enum class Type
    {
        Int,
        Float,
        String,
        Hexgrid
    };
    DeclarationStatement(Type type_, std::unique_ptr<Node> var_, int dimenstion_=0);

    std::string toString(int depth = 0) const override;
    std::string getType() const;
    std::string toString(Type type_) const;

    Type type;
    std::unique_ptr<Node> var;
    int dimenstion;
};
} //namespace ast




#endif // TKOM_AST_DECLARATION_STATEMENT_H_