#ifndef TKOM_AST_ARRAY_H_
#define TKOM_AST_ARRAY_H_

#include <string>
#include <memory>
#include <vector>
#include "Node.h"

namespace ast
{
class Array : public Node
{
public:
    Array(std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> elements_);

    std::string toString(int depth = 0) const override;
    std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> elements;
};
} //namespace ast




#endif // TKOM_AST_ARRAY_H_