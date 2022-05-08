#include "include/DeclarationStatement.h"

using namespace ast;

DeclarationStatement::DeclarationStatement(Type type_, 
                                            std::unique_ptr<Node> var_,
                                            int dimenstion_)
{
    type = type_;
    var = std::move(var_);
    dimenstion=dimenstion_;
}

std::string DeclarationStatement::toString(int depth) const
{
    return prefix(depth) + "Declaration\n" + 
           prefix(depth + 1) + "Variable Type (" + getType() + ")\n" +
           var->toString(depth+1);
}

std::string DeclarationStatement::getType() const
{
    if (dimenstion > 0)
        return std::to_string(dimenstion) + "d array of type " + toString(type);
    else
        return toString(type);
}

std::string DeclarationStatement::toString(Type type_) const
{
    switch (type_)
    {
    case Type::Int:
        return "int";
    case Type::Float:
        return "float";
    case Type::String:
        return "string";
    default:
        throw std::runtime_error("Unkown comparison operator");
        return "";
    }
}