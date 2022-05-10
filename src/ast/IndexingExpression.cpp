#include "IndexingExpression.h"

#include <string>
using namespace ast;

IndexingExpression::IndexingExpression(std::unique_ptr<Node> indexOn_,
                                       std::unique_ptr<Node> indexBy_)
{
    indexOn = std::move(indexOn_);
    indexBy = std::move(indexBy_);
}

std::string IndexingExpression::toString(int depth) const
{
    return prefix(depth) + "Indexing Expression\n" + 
           indexOn->toString(depth + 1) + 
           indexBy->toString(depth + 1);
}