#ifndef TKOM_LEXER_H
#define TKOM_LEXER_H

#include <cctype>
#include <istream>
#include <set>
#include <optional>
#include <math.h>
#include <utility>
#include "Token.h"
#include <HexgridErrors.h>

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
    std::optional<token::Token> trySigns();

    token::Token::Type getAlphaNumericTokenType(std::string);
    int parseInteger();
    double parseFraction();
    void ignoreWhitespaces();

    char get();
    std::pair<int, int> getLocation();

    bool isIntegerOverflow(int integer, int increase);
    std::istream& in;
    token::Token curr_token;
    int line;
    int column;
};
} // namespace lexer

#endif // TKOM_LEXER_H

