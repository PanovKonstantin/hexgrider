#ifndef TKOM_AST_SCOPE_H_
#define TKOM_AST_SCOPE_H_

#include "Node.h"
#include <vector>
#include <memory>

namespace ast
{
class Scope : public Node
{
public:
    Scope(std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> stmnts_);

    std::string toString(int depth = 0) const override;
    std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> stmnts;
};
} //namespace ast




#endif // TKOM_AST_SCOPE_H_