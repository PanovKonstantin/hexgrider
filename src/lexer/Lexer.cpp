#include "Lexer.h"
#include <cctype>
#include <optional>
#include <math.h>

using namespace lexer;
using namespace token;
namespace{
     std::set<std::string> keywords = { "func", "return", "if", "elif", 
                                        "else", "move", "foreach", 
                                        "in", "add", "remove", "to",
                                        "from","at"};
    std::set<std::string> alphaOperators = {"and", "or", "beside",
                                            "by", "on"};
    std::set<char> signs = {'<', '>', '/', '%', '*', '+', '-', '!','=',
                             '{', '}', '[', ']', '(', ')', ',', ';'};
    static std::set<std::string> operators = {  "<", ">", "/", "%", 
                                                "*", "+", "-", "!",
                                                "=", "{", "}", "[",
                                                "]", "(", ")", "==",
                                                "!=", ">=", "<=",",",
                                                ";"};
    static std::set<std::string> types = {  "int", "float", "string", 
                                            "hexgrid"};
}

Lexer::Lexer(std::istream& in_):in(in_), curr_token(Token()) {}

Token Lexer::getToken()
{
    ignoreWhitespaces();
    std::optional<Token> t = tryEof();
    if (!t){
        throwIfStreamError();
        t = tryNumber();
    }
    if(!t) t = tryInQuotes();
    if(!t) t = tryAlphaNumeric();
    if(!t) t = trySigns();
    if(!t) throwUnknownToken();
    return t.value();
}

std::optional<token::Token> Lexer::tryEof()
{
    if (!in.eof()) return {};
    return Token();
}

std::optional<token::Token> Lexer::tryNumber()
{
    if (!std::isdigit(in.peek())) return {};
    int integer = parseInteger();
    if (in.peek() != '.'){
        return Token(Token::Type::Integer, integer);
    }
    in.get();
    double decimal = double(integer) + parseFraction();
    return Token(Token::Type::Decimal, decimal);;
}

int Lexer::parseInteger(){
    int integer = 0;
    int increase = 0;
    while(std::isdigit(in.peek())){
        increase = in.get() - '0';
        if(isIntegerOverflow(integer, increase))
            throwOverflowInteger();
        integer = integer * 10 + increase;
    }
    return integer;
}

double Lexer::parseFraction(){
    int fraction = 0;
    int decimal_places = 0;
    int increase = 0;
    while (std::isdigit(in.peek()))
    {
        increase = in.get() - '0';
        if(isIntegerOverflow(fraction, increase))
            throwOverflowFraction();
        fraction = fraction * 10 + increase;
        decimal_places++;
    }
    return fraction / std::pow(10, decimal_places);
}

std::optional<token::Token> Lexer::tryInQuotes()
{
    if (in.peek() != '\"') return {};
    in.get();
    std::string text = "";
    while(in.peek() != '\"')
    {
        if (in.eof()) throw std::runtime_error("Enf of text while quotation are not closed");
        if (in.peek() == '\\'){
            in.get();
            if (in.eof()) throw std::runtime_error("End of text while quotations are not closed and escape symbol is opened");
            switch (in.peek())
            {
            case 'n':
                text += '\n';
                break;
            case '\"':
                text += '\"';
                break;
            case '\\':
                text += '\\';
                break;
            default:
                throw std::runtime_error("Unkown escape combination");
                break;
            }
            in.get();
        } else {
            text += in.get();
        }
    }
    in.get();
    return Token(Token::Type::Text, text);
}

std::optional<token::Token> Lexer::tryAlphaNumeric()
{
    if(!std::isalpha(in.peek())) return {};
    std::string token_value = "";
    token_value += in.get();
    while (std::isalpha(in.peek()) || std::isdigit(in.peek()) || in.peek() == '_')
        token_value += in.get();
    Token::Type type;
    if (keywords.count(token_value))
        type = Token::Type::Keyword;
    else if (alphaOperators.count(token_value))
        type = Token::Type::Operator;
    else if (types.count(token_value))
        type = Token::Type::Type;
    else
        type = Token::Type::Identifier;
    return Token(type, token_value);
}

std::optional<token::Token> Lexer::trySigns()
{
    if (!signs.count(in.peek())) return {};
    std::string oneCharOp = "";
    oneCharOp += in.get();
    std::string twoCharOp = oneCharOp;
    twoCharOp += in.peek();
    if (operators.count(twoCharOp))
    {
        in.get();
        return Token(Token::Type::Operator, twoCharOp);
    }
    else if (operators.count(oneCharOp))
        return Token(Token::Type::Operator, oneCharOp);
    throwUnknownOperator(oneCharOp);
    return {};
}

void Lexer::ignoreWhitespaces()
{
    while (in && std::isspace(in.peek()))
        in.get();
}

void Lexer::throwIfStreamError()
{
    if (!in)
        throw std::runtime_error("The end of input was not EOF");
}

void Lexer::throwUnknownToken()
{
  const std::string msg = "Unknown token, starting with: ";
  throw std::runtime_error(msg + static_cast<char>(in.get()));
}

void Lexer::throwOverflowInteger()
{
  const std::string msg = "Integer overflow. Integer literal can only read values up to +-2,147,483,647";
  throw std::runtime_error(msg);
}

void Lexer::throwOverflowFraction()
{
  const std::string msg = "Decimal overflow. Fraction part of decimal literal can only read values up to 0.2,147,483,647";
  throw std::runtime_error(msg);
}

void Lexer::throwUnknownOperator(std::string op)
{
  const std::string msg = "Unkown operator: ";
  throw std::runtime_error(msg + op);
}


bool Lexer::isIntegerOverflow(int value, int increase)
{
    if (value >= 214748368) return true;
    if (2147483647-increase<value*10) return true;
    return false;
}