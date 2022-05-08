#include "Lexer.h"
#include <token/Token.h>
#include <cctype>
#include <math.h>

using namespace lexer;
using namespace token;

Lexer::Lexer(std::istream& in_):in(in_), curr_token(Token()) {}
Token Lexer::getToken() const { return curr_token; }

void Lexer::readNextToken()
{
    ignoreWhitespaces();
    if (tryEof()) return;
    throwIfStreamError();
    if (tryNumber() || tryInQuotes() || tryAlphaNumeric() || trySigns()) 
        return;
    throwUnknownToken();
}

bool Lexer::tryEof()
{
    if (!in.eof()) 
        return false;
    curr_token = Token();
    return true;
}

bool Lexer::tryNumber()
{
    if (!std::isdigit(in.peek())) return false;
    int integer = 0;
    if (in.peek() != '0')
        while(std::isdigit(in.peek()))
            integer = integer * 10 + in.get() - '0';
    else in.get();
    if (in.peek() == '.')
    {
        in.get();
        int fraction = 0;
        int decimal_places = 0;
        while (std::isdigit(in.peek()))
        {
            fraction = fraction * 10 + in.get() - '0';
            decimal_places++;
        }
        double decimal = double(integer) + fraction / std::pow(10, decimal_places);
        curr_token = Token(Token::Type::Decimal, decimal);
        return true;
    }
    curr_token = Token(Token::Type::Integer, integer);
    return true;
}

bool Lexer::tryInQuotes()
{
    if (in.peek() != '\"') return false;
    in.get();
    std::string text = "";
    while(in.peek() != '\"')
    {
        if (in.eof()) throw std::invalid_argument("Quotation is not closed");
        if (in.peek() == '\\') in.get();
        text += in.get();
    }
    in.get();
    curr_token = Token(Token::Type::Text, text);
    return true;
}

bool Lexer::tryAlphaNumeric()
{
    if(!std::isalpha(in.peek())) return false;
    std::string token_value = "";
    token_value += in.get();
    while (std::isalpha(in.peek()) || std::isdigit(in.peek()) || in.peek() == '_')
        token_value += in.get();
    Token::Type type;
    if (lexer::keywords.count(token_value))
        type = Token::Type::Keyword;
    else if (lexer::alphaOperators.count(token_value))
        type = Token::Type::Operator;
    else if (lexer::types.count(token_value))
        type = Token::Type::Type;
    else
        type = Token::Type::Identifier;
    curr_token = Token(type, token_value);
    return true;
}

bool Lexer::trySigns()
{
    if (!lexer::signs.count(in.peek())) return false;
    std::string oneCharOp = "";
    oneCharOp += in.get();
    std::string twoCharOp = oneCharOp;
    twoCharOp += in.peek();
    if (lexer::operators.count(twoCharOp))
    {
        in.get();
        curr_token = Token(Token::Type::Operator, twoCharOp);
    }
    else if (lexer::operators.count(oneCharOp))
        curr_token = Token(Token::Type::Operator, oneCharOp);
    else
        return false;
    return true;
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