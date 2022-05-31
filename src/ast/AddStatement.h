#ifndef TKOM_AST_ADD_STATEMENT_H_
#define TKOM_AST_ADD_STATEMENT_H_

#include <string>
#include <memory>
#include "Node.h"

namespace ast
{
class AddStatement : public Node
{
public:
    AddStatement( 
                std::unique_ptr<Node> being_added_,
                std::unique_ptr<Node> added_to_,
                std::unique_ptr<Node> added_at_);

    std::string toString(int depth = 0) const override;
    std::unique_ptr<ast::Node> being_added;
    std::unique_ptr<ast::Node> added_to;
    std::unique_ptr<ast::Node> added_at;
};
} //namespace ast




#endif // TKOM_AST_ADD_STATEMENT_H_