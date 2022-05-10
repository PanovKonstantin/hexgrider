#ifndef TKOM_AST_FOREACH_STATEMENT_H_
#define TKOM_AST_FOREACH_STATEMENT_H_

#include <string>
#include <memory>
#include <vector>
#include "Node.h"

namespace ast
{
class ForeachStatement : public Node
{
public:
    ForeachStatement( 
                std::unique_ptr<Node> iterator_,
                std::unique_ptr<Node> iterated_,
                std::unique_ptr<Node> scope_);

    std::string toString(int depth = 0) const override;
    std::unique_ptr<ast::Node> iterator;
    std::unique_ptr<ast::Node> iterated;
    std::unique_ptr<ast::Node> scope;
};
} //namespace ast




#endif // TKOM_AST_FOREACH_STATEMENT_H_