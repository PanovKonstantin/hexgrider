#include "include/FunctionDefinition.h"

using namespace ast;

FunctionDefinition::FunctionDefinition(std::unique_ptr<Node> fun_,
                         std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> params_,
                         std::unique_ptr<Node> scope_)
{
    fun = std::move(fun_);
    params = std::move(params_);
    scope = std::move(scope_);
}

std::string FunctionDefinition::toString(int depth) const
{
    std::string ret_str = prefix(depth) + "Function Defenition\n" + 
                          fun->toString(depth + 1);
    for(auto const& param: *params)
        ret_str += param->toString(depth + 1);
    ret_str += scope->toString(depth + 1);
    return ret_str;
}