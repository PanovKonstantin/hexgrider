#include "Token.h"

using namespace token;


Token::Token():type(Type::EndOfFile) {}
Token::Token(Type t, int v):type(t), value(v)
{
    switch (t)
    {
    case Type::Integer: break;
    default: throw std::invalid_argument( "Type should be Integer" );
    }
}
Token::Token(Type t, double v):type(t), value(v)
{
    switch (t)
    {
    case Type::Decimal: break;
    default: throw std::invalid_argument( "Type should be Decimal" );
    }
}
Token::Token(Type t, std::string v):type(t), value(v)
{
    switch (t)
    {
    case Type::Text:
    case Type::Operator:
    case Type::Keyword:
    case Type::Type:
    case Type::Identifier: break;
    default: throw std::invalid_argument( "Type should be Text, Operator, Keyword, Type or Identifier" );
    }
}

std::string Token::toString() const { return toString(type) + valueToString(); }

std::string Token::toString(Token::Type type)
{
    switch (type)
    {
    case Type::EndOfFile:   return "End of file";
    case Type::Integer:     return "Integer";
    case Type::Decimal:     return "Decimal";
    case Type::Operator:    return "Operator";
    case Type::Text:        return "Text";
    case Type::Keyword:     return "Keyword";
    case Type::Identifier:  return "Identifier";
    case Type::Type:        return "Type";
    default:                return "Unkown token";
    }
}


std::string Token::valueToString() const
{
    switch (type)
    {
    case Type::Integer:     return " (" + std::to_string(getInteger()) + ")";
    case Type::Decimal:     return " (" + std::to_string(getDecimal()) + ")";
    case Type::Operator:    
    case Type::Text:        
    case Type::Keyword:    
    case Type::Type:      
    case Type::Identifier:  return " (" + getText() + ")";
    default:                return std::string();
    }
}