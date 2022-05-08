#include "include/Scope.h"
#include <vector>
#include <memory>
using namespace ast;

Scope::Scope(std::unique_ptr<std::vector<std::unique_ptr<Node>>> stmnts_)
{
    stmnts = std::move(stmnts_);
}

std::string Scope::toString(int depth) const
{
    std::string ret_str = prefix(depth) + "Scope\n";
    for(auto const& stmnt: *stmnts)
    {
        ret_str += stmnt->toString(depth + 1);
    }
    return ret_str;
}