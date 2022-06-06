#include "Token.h"
#include "HexgridErrors.h"
using namespace token;



Token::Token(
    std::pair<int, int> start_,
    std::pair<int, int> end_)
    :type(Type::EndOfFile), start(start_), end(end_) {}


Token::Token(
    Type t,
    std::pair<int, int> start_,
    std::pair<int, int> end_)
    :type(t), start(start_), end(end_){}

    
Token::Token(
    Type t, 
    int v,
    std::pair<int, int> start_,
    std::pair<int, int> end_)
    :type(t), value(v), start(start_), end(end_)
{
    switch (t)
    {
    case Type::Integer: break;
    default: throw hexgrid_errors::WrongTokenConstructor(start_, toString(t), "integer");
    }
}
Token::Token(
    Type t, double v,
    std::pair<int, int> start_,
    std::pair<int, int> end_)
    :type(t), value(v), start(start_), end(end_)
{
    switch (t)
    {
    case Type::Decimal: break;
    default: throw hexgrid_errors::WrongTokenConstructor(start_, toString(t), "decimal");
    }
}
Token::Token(
    Type t, std::string v,
    std::pair<int, int> start_,
    std::pair<int, int> end_)
    :type(t), value(v), start(start_), end(end_)
{
    switch (t)
    {
    case Type::Text:
    case Type::Identifier: break;
    default: throw hexgrid_errors::WrongTokenConstructor(start_, toString(t), "text");
    }
}

std::string Token::toString() const { return toString(type) + valueToString(); }

std::string Token::toString(Token::Type type)
{
    switch (type)
    {
    case Type::EndOfFile:               return "End of file";
    case Type::Integer:                 return "Integer";
    case Type::Decimal:                 return "Decimal";
    case Type::Text:                    return "Text";
    case Type::Identifier:              return "Identifier";
    case Type::FuncKeyword:             return "\"func\" keyword";
    case Type::ReturnKeyword:           return "\"return\" keyword";
    case Type::IfKeyword:               return "\"if\" keyword";
    case Type::ElifKeyword:             return "\"elif\" keyword";
    case Type::ElseKeyword:             return "\"else\" keyword";
    case Type::MoveKeyword:             return "\"move\" keyword";
    case Type::ForeachKeyword:          return "\"foreach\" keyword";
    case Type::InKeyword:               return "\"in\" keyword";
    case Type::AddKeyword:              return "\"add\" keyword";
    case Type::RemoveKeyword:           return "\"remove\" keyword";
    case Type::ToKeyword:               return "\"to\" keyword";
    case Type::FromKeyword:             return "\"from\" keyword";
    case Type::AtKeyword:               return "\"at\" keyword";
    case Type::AndOperator:             return "\"and\" operator";
    case Type::OrOperator:              return "\"or\" operator";
    case Type::BesideOperator:          return "\"beside\" operator";
    case Type::ByOperator:              return "\"by\" operator";
    case Type::OnOperator:              return "\"on\" operator";
    case Type::LessOperator:            return "\"less\" operator";
    case Type::GreaterOperator:         return "\"greater\" operator";
    case Type::DivideOperator:          return "\"divide\" operator";
    case Type::ModuloOperator:          return "\"modulo\" operator";
    case Type::MultiplyOperator:        return "\"multiply\" operator";
    case Type::AddOperator:             return "\"add\" operator";
    case Type::SubstructOperator:       return "\"substruct\" operator";
    case Type::LogicalNegationOperator: return "\"logical negation\" operator";
    case Type::AssignOperator:          return "\"assign\" operator";
    case Type::LeftBrace:               return "Left brace";
    case Type::RightBrace:              return "Right brace";
    case Type::LeftBracket:             return "Left bracket";
    case Type::RightBracket:            return "Right bracket";
    case Type::LeftParenthese:          return "Left parenthese";
    case Type::RightParenthese:         return "Right parenthese";
    case Type::EqualOperator:           return "\"equal\" operator";
    case Type::NotEqualOperator:        return "\"not equal\" operator";
    case Type::GreaterOrEqualOperator:  return "\"greater or equal\" operator";
    case Type::LessOrEqualOperator:     return "\"less or equal\" operator";
    case Type::Comma:                   return "Comma";
    case Type::Semicolon:               return "Semicolon";
    case Type::IntType:                 return "\"int\" type";
    case Type::FloatType:               return "\"float\" type";
    case Type::StringType:              return "\"string\" type";
    case Type::HexgridType:             return "\"hexgrid\" type";
    case Type::ArrayType:               return "\"array\" type";
    default:                            return "Unkown token";
    }
}


std::string Token::valueToString() const
{
    switch (type)
    {
    case Type::Integer:     return " (" + std::to_string(getInteger()) + ")";
    case Type::Decimal:     return " (" + std::to_string(getDecimal()) + ")";
    case Type::Text:        
    case Type::Identifier:  return " (" + getText() + ")";
    default:                return "";
    }
}


int  Token::getInteger() const {
    if (type != Type::Integer) return 0;
    return std::get<int>(value);
}
double  Token::getDecimal() const {
    if (type != Type::Decimal) return 0;
    return std::get<double>(value);
}
std::string  Token::getText() const {
    if (type != Type::Text && type !=Type::Identifier) return "";
    return std::get<std::string>(value);
}
