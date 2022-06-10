#include "Lexer.h"

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
                                            "hexgrid, array"};
}

Lexer::Lexer(std::istream& in_):in(in_), curr_token(Token()), line(1), column(1) {}

Token Lexer::getToken()
{
    ignoreWhitespaces();
    std::optional<Token> t = tryEof();
    if(!t) t = tryNumber();
    if(!t) t = tryInQuotes();
    if(!t) t = tryAlphaNumeric();
    if(!t) t = trySigns();
    if(!t) throw hexgrid_errors::UnkownCharacterException(getLocation(), in.peek());
    return t.value_or(Token(Token::Type::UnkownToken));
}

std::optional<token::Token> Lexer::tryEof()
{
    if (in.eof()) return Token(getLocation(), getLocation());
    if (!in) throw hexgrid_errors::BadEndOfInput();
    return {};
}

std::optional<token::Token> Lexer::tryNumber()
{
    if (!std::isdigit(in.peek())) return {};
    auto start = getLocation();
    int integer = parseInteger();
    if (in.peek() != '.'){
        auto end = getLocation();
        return Token(Token::Type::Integer, integer, start, end);
    }
    get();
    double decimal = double(integer) + parseFraction();
    auto end = getLocation();
    return Token(Token::Type::Decimal, decimal, start, end);;
}

int Lexer::parseInteger(){
    int integer = 0;
    int increase = 0;
    while(std::isdigit(in.peek())){
        increase = get() - '0';
        if(isIntegerOverflow(integer, increase))
            throw hexgrid_errors::IntegerOverflow(getLocation());
        integer = integer * 10 + increase;
    }
    return integer;
}

double Lexer::parseFraction(){
    int fraction = 0;
    int decimal_places = 0;
    int increase = 0;
    double result = 0;
    while (std::isdigit(in.peek()))
    {
        increase = get() - '0';
        if(isIntegerOverflow(fraction, increase)){
            result += fraction / std::pow(10, decimal_places);
            fraction = 0;
        }
        fraction = fraction * 10 + increase;
        decimal_places++;
    }
    result += fraction / std::pow(10, decimal_places);
    return result;
}

std::optional<token::Token> Lexer::tryInQuotes()
{
    if (in.peek() != '\"') return {};
    auto start = getLocation();
    get();
    std::string text = "";
    while(in.peek() != '\"')
    {
        if (in.eof()) throw hexgrid_errors::OpenQuotes(getLocation());
        if (in.peek() == '\\'){
            get();
            if (in.eof()||in.peek()<0) throw hexgrid_errors::OpenQuotes(getLocation());
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
                throw hexgrid_errors::UnkownEscapeCombination(getLocation(), in.peek());
                break;
            }
            get();
        } else {
            text += get();
        }
    }
    get();
    auto end = getLocation();
    return Token(Token::Type::Text, text, start, end);
}

std::optional<token::Token> Lexer::tryAlphaNumeric()
{
    if(!std::isalpha(in.peek())) return {};
    auto start = getLocation();
    std::string token_value = "";
    token_value += get();
    while (std::isalpha(in.peek()) || std::isdigit(in.peek()) || in.peek() == '_')
        token_value += get();
    auto t = getAlphaNumericTokenType(token_value);
    auto end = getLocation();
    if (t==Token::Type::Identifier)
        return Token(t, token_value, start, end);
    else 
        return Token(t, start, end);
}

std::optional<token::Token> Lexer::trySigns()
{
    if (!signs.count(in.peek())) return {};
    auto start = getLocation();

    std::string oneCharOp = "";
    oneCharOp += get();
    std::string twoCharOp = oneCharOp;
    twoCharOp += in.peek();
    if (operators.count(twoCharOp))
    {
        get();
        auto t = getAlphaNumericTokenType(twoCharOp);
        auto end = getLocation();
        return Token(t, start, end);
    }
    else if (operators.count(oneCharOp)){
        auto t = getAlphaNumericTokenType(oneCharOp);
        auto end = getLocation();
        return Token(t, start, end);
    }
    throw hexgrid_errors::UnkownOperator(start, oneCharOp);
    return {};
}

void Lexer::ignoreWhitespaces()
{
    while (in && std::isspace(in.peek()))
        get();
}


bool Lexer::isIntegerOverflow(int value, int increase)
{
    if (value >= 214748368) return true;
    if (2147483647-increase<value*10) return true;
    return false;
}

char Lexer::get(){
    char c = in.get(); 
    if (c == '\n'){
        line++;
        column=1;
    } else {
        column++;
    }
    return c;
}

std::pair<int, int> Lexer::getLocation(){
    std::pair<int, int> loc;
    loc.first = line;
    loc.second = column;
    return loc;
}


Token::Type Lexer::getAlphaNumericTokenType(std::string value){
    if (value == "func")    return Token::Type::FuncKeyword;
    if (value == "return")  return Token::Type::ReturnKeyword;
    if (value == "if")      return Token::Type::IfKeyword;
    if (value == "elif")    return Token::Type::ElifKeyword;
    if (value == "else")    return Token::Type::ElseKeyword;
    if (value == "move")    return Token::Type::MoveKeyword;
    if (value == "foreach") return Token::Type::ForeachKeyword;
    if (value == "in")      return Token::Type::InKeyword;
    if (value == "add")     return Token::Type::AddKeyword;
    if (value == "remove")  return Token::Type::RemoveKeyword;
    if (value == "to")      return Token::Type::ToKeyword;
    if (value == "from")    return Token::Type::FromKeyword;
    if (value == "at")      return Token::Type::AtKeyword;
    if (value == "and")     return Token::Type::AndOperator;
    if (value == "or")      return Token::Type::OrOperator;
    if (value == "beside")  return Token::Type::BesideOperator;
    if (value == "by")      return Token::Type::ByOperator;
    if (value == "on")      return Token::Type::OnOperator;
    if (value == "<")       return Token::Type::LessOperator;
    if (value == ">")       return Token::Type::GreaterOperator;
    if (value == "/")       return Token::Type::DivideOperator;
    if (value == "%")       return Token::Type::ModuloOperator;
    if (value == "*")       return Token::Type::MultiplyOperator;
    if (value == "+")       return Token::Type::AddOperator;
    if (value == "-")       return Token::Type::SubstructOperator;
    if (value == "!")       return Token::Type::LogicalNegationOperator;
    if (value == "=")       return Token::Type::AssignOperator;
    if (value == "{")       return Token::Type::LeftBrace;
    if (value == "}")       return Token::Type::RightBrace;
    if (value == "[")       return Token::Type::LeftBracket;
    if (value == "]")       return Token::Type::RightBracket;
    if (value == "(")       return Token::Type::LeftParenthese;
    if (value == ")")       return Token::Type::RightParenthese;
    if (value == "==")      return Token::Type::EqualOperator;
    if (value == "!=")      return Token::Type::NotEqualOperator;
    if (value == ">=")      return Token::Type::GreaterOrEqualOperator;
    if (value == "<=")      return Token::Type::LessOrEqualOperator;
    if (value == ",")       return Token::Type::Comma;
    if (value == ";")       return Token::Type::Semicolon;
    if (value == "int")     return Token::Type::IntType;
    if (value == "float")   return Token::Type::FloatType;
    if (value == "string")  return Token::Type::StringType;
    if (value == "hexgrid") return Token::Type::HexgridType;
    if (value == "array") return Token::Type::ArrayType;
    return Token::Type::Identifier;
}