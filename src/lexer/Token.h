#ifndef TKOM_TOKEN_H
#define TKOM_TOKEN_H

#include <variant>
#include <ostream>
#include <string>
#include <utility>


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
        FuncKeyword,
        ReturnKeyword,
        IfKeyword,
        ElifKeyword,
        ElseKeyword,
        MoveKeyword,
        ForeachKeyword,
        InKeyword,
        AddKeyword,
        RemoveKeyword,
        ToKeyword,
        FromKeyword,
        AtKeyword,
        AndOperator,
        OrOperator,
        BesideOperator,
        ByOperator,
        OnOperator,
        LessOperator,
        GreaterOperator,
        DivideOperator,
        ModuloOperator,
        MultiplyOperator,
        AddOperator,
        SubstructOperator,
        LogicalNegationOperator,
        AssignOperator,
        LeftBrace,
        RightBrace,
        LeftBracket,
        RightBracket,
        LeftParenthese,
        RightParenthese,
        EqualOperator,
        NotEqualOperator,
        GreaterOrEqualOperator,
        LessOrEqualOperator,
        Comma,
        Semicolon,
        IntType,
        FloatType,
        StringType,
        HexgridType,
        ArrayType,
        Identifier,
        UnkownToken
    };

    Token(  std::pair<int, int> start_=std::pair<int, int>(0,0),
            std::pair<int, int> end_=std::pair<int, int>(0,0));

    Token(  Type t,
            int v,
            std::pair<int, int> start_=std::pair<int, int>(0,0),
            std::pair<int, int> end_=std::pair<int, int>(0,0));

    Token(  Type t,
            double v,
            std::pair<int, int> start_=std::pair<int, int>(0,0),
            std::pair<int, int> end_=std::pair<int, int>(0,0));

    Token(  Type t,
            std::string v,
            std::pair<int, int> start_=std::pair<int, int>(0,0),
            std::pair<int, int> end_=std::pair<int, int>(0,0));

    Token(  Type t,
            std::pair<int, int> start_=std::pair<int, int>(0,0),
            std::pair<int, int> end_=std::pair<int, int>(0,0));

    Token(  std::string v,
            std::pair<int, int> start_=std::pair<int, int>(0,0),
            std::pair<int, int> end_=std::pair<int, int>(0,0));

    Type getType() const { return type; }

    int getInteger() const;
    double getDecimal() const;
    std::string getText() const;

    std::string toString() const;
    static std::string toString(Token::Type type);

    std::pair<int, int> getStart(){return start;};
    std::pair<int, int> getEnd(){return end;};
    
private:
    std::string valueToString() const;
    Type type;
    std::variant< int, double, std::string > value;
    std::pair<int, int> start;
    std::pair<int, int> end;
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