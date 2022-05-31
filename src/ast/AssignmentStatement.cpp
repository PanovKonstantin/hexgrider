#include "AssignmentStatement.h"

using namespace ast;

AssignmentStatement::AssignmentStatement(std::unique_ptr<Node> target_, 
                                         std::unique_ptr<Node> value_)
{
    target = std::move(target_);
    value = std::move(value_);
}

std::string AssignmentStatement::toString(int depth) const
{
    return prefix(depth) + "Assignment\n" + 
           target->toString(depth+1) + 
           value->toString(depth+1);
           
}