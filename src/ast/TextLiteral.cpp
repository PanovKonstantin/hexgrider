#include "TextLiteral.h"

using namespace ast;

TextLiteral::TextLiteral(std::string value_)
    : value(value_)
{
    
}

std::string TextLiteral::toString(int depth) const
{
    return prefix(depth) + "Text Literal (" + value + ")\n";
}