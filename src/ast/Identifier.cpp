#include "include/Identifier.h"

using namespace ast;

Identifier::Identifier(std::string value_)
    : value(value_)
{
}

std::string Identifier::toString(int depth) const
{
    return prefix(depth) + "Identifier (" + value + ")\n";
}