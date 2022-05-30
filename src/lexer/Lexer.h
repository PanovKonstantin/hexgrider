#ifndef TKOM_LEXER_H
#define TKOM_LEXER_H

#include <istream>
#include <set>
#include <optional>
#include "Token.h"

namespace lexer
{
class Lexer
{
public:
    Lexer(std::istream& in);
    const Lexer& operator=(const Lexer&) = delete;

    token::Token getToken();

private:
    std::optional<token::Token> tryEof();
    std::optional<token::Token> tryNumber();
    std::optional<token::Token> tryInQuotes();
    std::optional<token::Token> tryAlphaNumeric();
    std::optional<token::Token>  trySigns();

    int parseInteger();
    double parseFraction();

    void ignoreWhitespaces();
    void throwUnknownToken();
    void throwIfStreamError();
    void throwOverflowInteger();
    void throwOverflowFraction();
    void throwUnknownOperator(std::string op);

    bool isIntegerOverflow(int integer);
    std::istream& in;
    token::Token curr_token;
};
} // namespace lexer

#endif // TKOM_LEXER_H

