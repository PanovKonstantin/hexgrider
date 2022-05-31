#include "Literal.h"

using namespace ast;

Literal::Literal()
{
}

std::string Literal::toString(int depth) const
{
    return prefix(depth) + "Blank literal\n";
}