#include "include/IntegerLiteral.h"

using namespace ast;

IntegerLiteral::IntegerLiteral(int value_)
    : value(value_)
{
    
}

std::string IntegerLiteral::toString(int depth) const
{
    return prefix(depth) + "Integer Literal (" + std::to_string(value) + ")\n";
}