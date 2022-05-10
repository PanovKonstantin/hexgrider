#ifndef TKOM_LEXER_H
#define TKOM_LEXER_H

#include <istream>
#include <set>
#include "Token.h"

namespace lexer
{
class Lexer
{
public:
    Lexer(std::istream& in);
    const Lexer& operator=(const Lexer&) = delete;

    void readNextToken();
    token::Token getToken() const;

private:
    bool tryEof();
    bool tryNumber();
    bool tryInQuotes();
    bool tryAlphaNumeric();
    bool trySigns();

    void ignoreWhitespaces();
    void throwUnknownToken();
    void throwIfStreamError();

    std::istream& in;
    token::Token curr_token;
};

static std::set<std::string> keywords = {"func", "return", "if", "elif", "else", "move", 
                                         "foreach", "in", "add", "remove", "to", "from","at"};
static std::set<std::string> alphaOperators = {"and", "or", "beside", "by", "on"};
static std::set<char> signs = {'<', '>', '/', '%', '*', '+', '-', '!', '=', '{', '}', '[',
                               ']', '(', ')', ',', ';'};
static std::set<std::string> operators = {"<", ">", "/", "%", "*", "+", "-", "!", "=", "{", 
                                         "}", "[", "]", "(", ")","==", "!=", ">=", "<=",
                                         ",", ";"};
static std::set<std::string> types = {"int", "float", "string", "hexgrid"};

} // namespace lexer

#endif // TKOM_LEXER_H

