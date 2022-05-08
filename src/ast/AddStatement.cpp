#include "include/AddStatement.h"

using namespace ast;

AddStatement::AddStatement(
                std::unique_ptr<Node> being_added_,
                std::unique_ptr<Node> added_to_,
                std::unique_ptr<Node> added_at_)
{
    being_added = std::move(being_added_);
    added_to = std::move(added_to_);
    added_at = std::move(added_at_);
}

std::string AddStatement::toString(int depth) const
{
    return prefix(depth) + "Add Statement\n" + 
            being_added->toString(depth + 1) + 
            added_to->toString(depth + 1) + 
            added_at->toString(depth + 1);
}