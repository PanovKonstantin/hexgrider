#include "include/FunctionCall.h"

using namespace ast;

FunctionCall::FunctionCall(std::unique_ptr<Node> func_, 
                           std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> args_)
{
    func = std::move(func_);
    args = std::move(args_);
}

std::string FunctionCall::toString(int depth) const
{
    auto ret_str =  prefix(depth) + "Function Call\n" + 
                    func->toString(depth+1);
    for(auto const& arg: *args)
    {
        ret_str += arg->toString(depth + 1);
    }
    return ret_str;
           
}