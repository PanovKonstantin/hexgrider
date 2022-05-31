#ifndef TKOM_AST_INDEXING_EXPRESSION_H_
#define TKOM_AST_INDEXING_EXPRESSION_H_

#include "Node.h"
#include <memory>
#include <string>

namespace ast
{
class IndexingExpression : public Node
{
public:
    IndexingExpression(std::unique_ptr<ast::Node> indexOn_,
                       std::unique_ptr<ast::Node> indexBy_);

    std::string toString(int depth = 0) const override;

    std::unique_ptr<ast::Node> indexOn;
    std::unique_ptr<ast::Node> indexBy;
};
} //namespace ast




#endif // TKOM_AST_INDEXING_EXPRESSION_H_