#ifndef TKOM_AST_ASSIGNMENT_STATEMENT_H_
#define TKOM_AST_ASSIGNMENT_STATEMENT_H_

#include <memory>
#include "Node.h"

namespace ast
{
class AssignmentStatement : public Node
{
public:
    AssignmentStatement(std::unique_ptr<Node> target_, 
                        std::unique_ptr<Node> value_);

    std::string toString(int depth = 0) const override;

    std::unique_ptr<Node> target;
    std::unique_ptr<Node> value;
};
} //namespace ast




#endif // TKOM_AST_ASSIGNMENT_STATEMENT_H_