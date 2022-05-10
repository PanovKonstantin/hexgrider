#include "DecimalLiteral.h"

using namespace ast;

DecimalLiteral::DecimalLiteral(double value_)
    : value(value_)
{
    
}

std::string DecimalLiteral::toString(int depth) const
{
    return prefix(depth) + "Decimal Literal (" + std::to_string(value) + ")\n";
}