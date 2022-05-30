#ifndef TKOM_TOKEN_H
#define TKOM_TOKEN_H

#include <variant>
#include <ostream>
#include <string>


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
        UnkownToken
    };

    Token();
    Token(Type t, int v);
    Token(Type t, double v);
    Token(Type t, std::string v);
    Token(Type t);

    Type getType() const { return type; }

    int getInteger() const { return std::get<int>(value); }
    double getDecimal() const { return std::get<double>(value); }
    std::string getText() const { return std::get<std::string>(value); }

    std::string toString() const;
    static std::string toString(Token::Type type);
    
private:
    std::string valueToString() const;
    Type type;
    std::variant< int, double, std::string > value;
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