#ifndef TKOM_TOKEN_H
#define TKOM_TOKEN_H

#include "boost/variant.hpp"

#include <ostream>
#include <string>
#include <variant>


namespace token
{

class Token
{
public:
    enum class Type
    {
        EndOfFile,
        Integer,
        Decimal,
        Text,
        Operator,
        Keyword,
        Type,
        Identifier,
    };

    Token();
    Token(Type t, int v);
    Token(Type t, double v);
    Token(Type t, std::string v);

    Type getType() const { return type; }

    int getInteger() const { return boost::get<int>(value); }
    double getDecimal() const { return boost::get<double>(value); }
    std::string getText() const { return boost::get<std::string>(value); }

    std::string toString() const;
    static std::string toString(Token::Type type);
    
private:
    std::string valueToString() const;
    Type type;
    boost::variant< int, double, std::string > value;
};

inline std::ostream& operator<<(std::ostream& o, Token::Type type)
{
    return o << Token::toString(type);
}
inline std::ostream& operator<<(std::ostream& o, Token t)
{
    return o << t.toString();
}


} //namespace token

#endif //TKOM_TOKEN_H