#include "Parser.h"
#include <ast/Node.h>
#include <ast/Scope.h>
#include "ast/IntegerLiteral.h"
#include <ast/DecimalLiteral.h>
#include <ast/TextLiteral.h>
#include <ast/Identifier.h>
#include <ast/Array.h>
#include <ast/Hexgrid.h>
#include <ast/HexgridCell.h>
#include <ast/IndexingExpression.h>
#include <ast/LogicalNegation.h>
#include <ast/ArithmeticalNegation.h>
#include <ast/FOArithmExpression.h>
#include <ast/SOArithmExpression.h>
#include <ast/HexgridExpression.h>
#include <ast/ComparisonExpression.h>
#include <ast/AndExpression.h>
#include <ast/OrExpression.h>
#include <ast/DeclarationStatement.h>
#include <ast/AssignmentStatement.h>
#include <ast/FunctionCall.h>
#include <ast/IfStatement.h>
#include <ast/ConditionBlock.h>
#include <ast/ForeachStatement.h>
#include <ast/ReturnStatement.h>
#include <ast/AddStatement.h>
#include <ast/RemoveStatement.h>
#include <ast/MoveStatement.h>
#include <ast/FunctionDefinition.h>
using namespace parser;
using namespace lexer;
using namespace token;
using namespace ast;

Parser::Parser(std::unique_ptr<Lexer> lexer_)
    : lexer(std::move(lexer_))
{
}

Parser::~Parser()
{
}

// Parses script, return statement and function definitions in a scope.
// Function definitions can be only placed outside 
std::unique_ptr<Node> Parser::parse()
{
    advance();
    auto script_statments = std::make_unique<std::vector<std::unique_ptr<Node>>>();
    while (!checkTokenType(Token::Type::EndOfFile))
        script_statments->push_back(readStatementOrFuncDef());
    return std::make_unique<Scope>(std::move(script_statments));
}

std::unique_ptr<Node> Parser::readStatementOrFuncDef()
{
    if (checkToken(Token::Type::Keyword, "func"))
    {
        return readFuncDef();
    }
    return readStatement();

}

std::unique_ptr<Node> Parser::readStatement()
{
    std::unique_ptr<Node> stmnt;
    if (checkTokenType(Token::Type::Type))
        stmnt = readDeclrOrInit();
    else if (checkTokenType(Token::Type::Identifier))
        stmnt = readFuncCallOrAssignment();
    else if (checkToken(Token::Type::Keyword, "if"))
        return readIfStatement();
    else if (checkToken(Token::Type::Keyword, "foreach"))
        return readForeachStatement();
    else if (checkToken(Token::Type::Keyword, "return"))
        stmnt = readReturnStatement();
    else if (checkToken(Token::Type::Keyword, "add"))
        stmnt = readAddStatement();
    else if (checkToken(Token::Type::Keyword, "remove"))
        stmnt = readRemoveStatement();
    else if (checkToken(Token::Type::Keyword, "move"))
        stmnt = readMoveStatement();
    requireToken(Token::Type::Operator, ";");
    advance();
    return stmnt;
}

std::unique_ptr<Node> Parser::readDeclrOrInit()
{
    auto declr = readDeclr();
    if (checkToken(Token::Type::Operator, "="))
        return readAssignment(std::move(declr));
    return declr;
}

std::unique_ptr<Node> Parser::readFuncCallOrAssignment()
{
    auto id = readIdentifier();
    if (checkToken(Token::Type::Operator, "="))
        return readAssignment(std::move(id));
    if (checkToken(Token::Type::Operator, "("))
        return readFunctionCall(std::move(id));
    throwOnUnexpectedInput("Assignment or Function call");
    return std::make_unique<Literal>(); // Returned as a blank node
}

std::unique_ptr<Node> Parser::readFunctionCall(std::unique_ptr<ast::Node> func)
{
    requireToken(Token::Type::Operator, "(");
    advance();
    auto args = std::make_unique<std::vector<std::unique_ptr<Node>>>();
    while (!checkToken(Token::Type::Operator, ")"))
    {
        args->push_back(readExpression());
        if (checkToken(Token::Type::Operator, ","))
            advance();
    }
    advance();
    return std::make_unique<FunctionCall>(std::move(func), std::move(args));
}

std::unique_ptr<Node> Parser::readAssignment(std::unique_ptr<ast::Node> lvalue)
{
    requireToken(Token::Type::Operator, "=");
    advance();
    return std::make_unique<AssignmentStatement>(std::move(lvalue), readExpression());
}

std::unique_ptr<Node> Parser::readDeclr()
{
    requireToken(Token::Type::Type);
    DeclarationStatement::Type t;
    if(checkTokenValue("int"))
        t = DeclarationStatement::Type::Int;
    else if(checkTokenValue("float"))
        t = DeclarationStatement::Type::Float;
    else if(checkTokenValue("string"))
        t = DeclarationStatement::Type::String;
    advance();
    int dimension = 0;
    while(checkToken(Token::Type::Operator, "["))
    {
        advance();
        requireToken(Token::Type::Operator, "]");
        advance();
        dimension++;
    }
    return std::make_unique<DeclarationStatement>(t, readIdentifier(), dimension);

}

std::unique_ptr<Node> Parser::readIfStatement()
{
    std::unique_ptr<Node> if_block = nullptr;
    std::unique_ptr<Node> else_block = nullptr;
    auto elif_blocks = std::make_unique<std::vector<std::unique_ptr<Node>>>();
    requireToken(Token::Type::Keyword, "if");
    advance();
    auto cond = readCondition();
    auto scope = readStatementBlock();
    if_block = std::make_unique<ConditionBlock>(std::move(cond), std::move(scope));
    while(checkToken(Token::Type::Keyword, "elif"))
    {
        advance();
        cond = readCondition();
        scope = readStatementBlock();
        elif_blocks->push_back(std::make_unique<ConditionBlock>(std::move(cond),
                                                                std::move(scope)));
    }
    if(checkToken(Token::Type::Keyword, "else"))
    {
        advance();
        else_block = readStatementBlock();
    }
    return std::make_unique<IfStatement>(
        std::move(if_block),
        std::move(elif_blocks),
        std::move(else_block)
    );
}

std::unique_ptr<Node> Parser::readForeachStatement()
{
    requireToken(Token::Type::Keyword, "foreach");
    advance();
    auto iterator = readDeclr();
    requireToken(Token::Type::Keyword, "in");
    advance();
    auto iterated = readExpression();
    auto scope = readStatementBlock();
    return std::make_unique<ForeachStatement>(std::move(iterator),
                                              std::move(iterated),
                                              std::move(scope));
}

std::unique_ptr<Node> Parser::readReturnStatement()
{
    requireToken(Token::Type::Keyword, "return");
    advance();
    return std::make_unique<ReturnStatement>(readExpression());
}

std::unique_ptr<Node> Parser::readAddStatement()
{
    requireToken(Token::Type::Keyword, "add");
    advance();
    auto being_added = readExpression();
    requireToken(Token::Type::Keyword, "to");
    advance();
    auto added_to = readExpression();
    requireToken(Token::Type::Keyword, "at");
    advance();
    auto added_at = readExpression();
    return std::make_unique<AddStatement>(std::move(being_added),
                                          std::move(added_to),
                                          std::move(added_at));
}

std::unique_ptr<Node> Parser::readRemoveStatement()
{
    requireToken(Token::Type::Keyword, "remove");
    advance();
    auto pos = readExpression();
    requireToken(Token::Type::Keyword, "from");
    advance();
    auto grid = readExpression();
    return std::make_unique<RemoveStatement>(std::move(pos),
                                          std::move(grid));
}

std::unique_ptr<Node> Parser::readMoveStatement()
{
    requireToken(Token::Type::Keyword, "move");
    advance();
    auto pos1 = readExpression();
    requireToken(Token::Type::Keyword, "from");
    advance();
    auto grid1 = readExpression();
    requireToken(Token::Type::Keyword, "to");
    advance();
    auto grid2 = readExpression();
    requireToken(Token::Type::Keyword, "at");
    advance();
    auto pos2 = readExpression();
    return std::make_unique<MoveStatement>(std::move(pos1),
                                          std::move(grid1),
                                          std::move(grid2),
                                          std::move(pos2));
}

std::unique_ptr<Node> Parser::readCondition()
{
    requireToken(Token::Type::Operator, "(");
    advance();
    auto cond = readExpression();
    requireToken(Token::Type::Operator, ")");
    advance();
    return cond;
}

std::unique_ptr<Node> Parser::readStatementBlock()
{
    requireToken(Token::Type::Operator, "{");
    advance();
    auto statments = std::make_unique<std::vector<std::unique_ptr<Node>>>();
    while (!checkToken(Token::Type::Operator, "}"))
        statments->push_back(readStatement());
    advance();
    return std::make_unique<Scope>(std::move(statments));
}

std::unique_ptr<Node> Parser::readFuncDef()
{
    requireToken(Token::Type::Keyword, "func");
    advance();
    auto fun = readDeclr();
    auto params = std::make_unique<std::vector<std::unique_ptr<Node>>>();
    requireToken(Token::Type::Operator, "(");
    advance();
    while (!checkToken(Token::Type::Operator, ")"))
    {
        params->push_back(readDeclr());
        if (checkToken(Token::Type::Operator, ","))
            advance();
    }
    advance();
    auto scope = readStatementBlock();
    return std::make_unique<FunctionDefinition>(
        std::move(fun),
        std::move(params),
        std::move(scope)
    );
}

std::unique_ptr<Node> Parser::readExpression()
{
    return readOrExpression();
} 

std::unique_ptr<Node> Parser::readOrExpression()
{
    auto expr = readAndExpression();
    while(checkToken(Token::Type::Operator, "or"))
    {
        advance();
        auto lvalue = std::move(expr);
        auto rvalue = readAndExpression();
        expr = std::make_unique<OrExpression>(std::move(lvalue),
                                              std::move(rvalue));
    }
    return expr;
}

std::unique_ptr<Node> Parser::readAndExpression()
{
    auto expr = readComparisonExpression();
    while(checkToken(Token::Type::Operator, "and"))
    {
        advance();
        auto lvalue = std::move(expr);
        auto rvalue = readComparisonExpression();
        expr = std::make_unique<AndExpression>(std::move(lvalue),
                                                      std::move(rvalue));
    }
    return expr;
}

std::unique_ptr<Node> Parser::readComparisonExpression()
{
    auto expr = readHexgridExpression();
    while(isComparisonOperator())
    {
        ComparisonExpression::Operator op;
        if(checkTokenValue("<"))
            op = ComparisonExpression::Operator::Less;
        else if(checkTokenValue("<="))
            op = ComparisonExpression::Operator::LessOrEqual;
        else if(checkTokenValue(">"))
            op = ComparisonExpression::Operator::Greater;
        else if(checkTokenValue(">="))
            op = ComparisonExpression::Operator::GreaterOrEqual;
        else if(checkTokenValue("=="))
            op = ComparisonExpression::Operator::Equal;
        else if(checkTokenValue("!="))
            op = ComparisonExpression::Operator::NotEqual;
        advance();
        auto lvalue = std::move(expr);
        auto rvalue = readHexgridExpression();
        expr = std::make_unique<ComparisonExpression>(std::move(lvalue),
                                                      op,
                                                      std::move(rvalue));
    }
    return expr;
}

std::unique_ptr<Node> Parser::readHexgridExpression()
{
    auto expr = readSOArithmExpression();
    while(checkToken(Token::Type::Operator, "on") ||
          checkToken(Token::Type::Operator, "by") || 
          checkToken(Token::Type::Operator, "beside"))
    {
        HexgridExpression::Operator op;
        if(checkTokenValue("on"))
            op = HexgridExpression::Operator::On;
        else if(checkTokenValue("by"))
            op = HexgridExpression::Operator::By;
        else if(checkTokenValue("beside"))
            op = HexgridExpression::Operator::Beside;
        advance();
        auto lvalue = std::move(expr);
        auto rvalue = readHexgridExpression(); // Read from right to left
        expr = std::make_unique<HexgridExpression>(std::move(lvalue),
                                                    op,
                                                    std::move(rvalue));
    }
    return expr;
}

std::unique_ptr<Node> Parser::readSOArithmExpression()
{
    auto expr = readFOArithmExpression();
    while(checkToken(Token::Type::Operator, "+") || checkToken(Token::Type::Operator, "-"))
    {
        SOArithmExpression::Operator op;
        if(checkTokenValue("+"))
            op = SOArithmExpression::Operator::Add;
        else if(checkTokenValue("-"))
            op = SOArithmExpression::Operator::Substruct;
        advance();
        auto lvalue = std::move(expr);
        auto rvalue = readFOArithmExpression();
        expr = std::make_unique<SOArithmExpression>(std::move(lvalue),
                                                    op,
                                                    std::move(rvalue));
    }
    return expr;
}

std::unique_ptr<Node> Parser::readFOArithmExpression()
{
    auto expr = readArithmNegExpression();
    while(checkToken(Token::Type::Operator, "*") || checkToken(Token::Type::Operator, "/"))
    {
        FOArithmExpression::Operator op;
        if(checkTokenValue("*"))
            op = FOArithmExpression::Operator::Multiply;
        else if(checkTokenValue("/"))
            op = FOArithmExpression::Operator::Divide;
        advance();
        auto lvalue = std::move(expr);
        auto rvalue = readArithmNegExpression();
        expr = std::make_unique<FOArithmExpression>(std::move(lvalue),
                                                    op,
                                                    std::move(rvalue));
    }
    return expr;
}

std::unique_ptr<Node> Parser::readArithmNegExpression()
{
    if (checkToken(Token::Type::Operator, "-"))
    {
        advance();
        return std::make_unique<ArithmeticalNegation>(readLogicNegExpression());
    }
    return readLogicNegExpression();
}

std::unique_ptr<Node> Parser::readLogicNegExpression()
{
    if (checkToken(Token::Type::Operator, "!"))
    {
        advance();
        return std::make_unique<LogicalNegation>(readIndexingExpression());
    }
    return readIndexingExpression();
}

std::unique_ptr<Node> Parser::readIndexingExpression()
{
    auto expr = readTerm();
    while (checkToken(Token::Type::Operator, "["))
    {
        advance();
        auto indexOn = std::move(expr);
        auto indexBy = readExpression();
        expr = std::make_unique<IndexingExpression>(std::move(indexOn), std::move(indexBy));
        requireToken(Token::Type::Operator, "]");
        advance();
    }
    return expr;
}

std::unique_ptr<Node> Parser::readTerm()
{
    // TODO expand term list
    if (checkTokenType(Token::Type::Integer))
        return readIntegerLiteral();
    if (checkTokenType(Token::Type::Decimal))
        return readDecimalLiteral();
    if (checkTokenType(Token::Type::Text))
        return readTextLiteral();
    if (checkTokenType(Token::Type::Identifier))
        return readIdentifierOrFuncCall();
    if (checkToken(Token::Type::Operator, "["))
        return readArray();
    if (checkToken(Token::Type::Operator, "<"))
        return readHexgrid();
    if (checkToken(Token::Type::Operator, "("))
    {
        advance();
        auto expr = readExpression();
        requireToken(Token::Type::Operator, ")");
        advance();
        return expr;
    }
    throwOnUnexpectedInput("Term");
    return std::make_unique<Literal>();
}

std::unique_ptr<Node> Parser::readTextLiteral()
{
    const auto val = requireToken(Token::Type::Text).getText();
    advance();
    return std::make_unique<TextLiteral>(val);
}

std::unique_ptr<Node> Parser::readDecimalLiteral()
{
    const auto val = requireToken(Token::Type::Decimal).getDecimal();
    advance();
    return std::make_unique<DecimalLiteral>(val);
}

std::unique_ptr<Node> Parser::readIntegerLiteral()
{
    const auto val = requireToken(Token::Type::Integer).getInteger();
    advance();
    return std::make_unique<IntegerLiteral>(val);
}

std::unique_ptr<Node> Parser::readIdentifier()
{
    const auto val = requireToken(Token::Type::Identifier).getText();
    advance();
    return std::make_unique<Identifier>(val);
}

std::unique_ptr<Node> Parser::readIdentifierOrFuncCall()
{
    const auto val = requireToken(Token::Type::Identifier).getText();
    advance();
    auto id = std::make_unique<Identifier>(val);
    if (checkToken(Token::Type::Operator, "("))
    {
        advance();
        auto args = std::make_unique<std::vector<std::unique_ptr<Node>>>();
        while (!checkToken(Token::Type::Operator, ")"))
        {
            args->push_back(readExpression());
            if (checkToken(Token::Type::Operator, ","))
                advance();
        }
        advance();
        return std::make_unique<FunctionCall>(std::move(id), std::move(args));
    }
    return id;
}

std::unique_ptr<Node> Parser::readArray()
{
    requireToken(Token::Type::Operator, "[");
    advance();
    auto elements = std::make_unique<std::vector<std::unique_ptr<Node>>>();
    while (!checkToken(Token::Type::Operator, "]"))
    {
        elements->push_back(readExpression());
        if (checkToken(Token::Type::Operator, ","))
            advance();
    }
    advance();
    return std::make_unique<Array>(std::move(elements));
}

std::unique_ptr<Node> Parser::readHexgrid()
{
    requireToken(Token::Type::Operator, "<");
    advance();
    auto cells = std::make_unique<std::vector<std::unique_ptr<Node>>>();
    std::unique_ptr<Node> pos = nullptr;
    std::unique_ptr<Node> value = nullptr;
    std::unique_ptr<Node> cell = nullptr;
    while (!checkToken(Token::Type::Operator, ">"))
    {
        value = readExpression();
        requireToken(Token::Type::Keyword, "at");
        advance();
        pos = readTerm();
        if (checkToken(Token::Type::Operator, ","))
            advance();
        cell = std::make_unique<HexgridCell>(std::move(value), std::move(pos));
        cells->push_back(std::move(cell));
    }
    advance();
    return std::make_unique<Hexgrid>(std::move(cells));
}


bool Parser::isComparisonOperator()
{
    return checkToken(Token::Type::Operator, "<")    || 
           checkToken(Token::Type::Operator, "<=")   || 
           checkToken(Token::Type::Operator, ">")    || 
           checkToken(Token::Type::Operator, ">=")   || 
           checkToken(Token::Type::Operator, "==")   || 
           checkToken(Token::Type::Operator, "!=");
}

void Parser::throwOnUnexpectedInput(std::string expected)
{
    throw std::runtime_error("Unexpected token: " + lexer->getToken().toString()
                           + ", expecting: " + expected);
}

void Parser::throwOnUnexpectedInput(Token::Type expected)
{
    throw std::runtime_error("Unexpected token: " + lexer->getToken().toString()
                           + ", expecting: " + Token::toString(expected));
}

void Parser::throwOnUnexpectedInput(token::Token t)
{
    throw std::runtime_error("Unexpected token: " + lexer->getToken().toString()
                           + ", expecting: " + t.toString());
}

Token Parser::requireToken(Token::Type expected_type)
{
    const auto token = lexer->getToken();
    const auto type = token.getType();
    if (type != expected_type)
        throwOnUnexpectedInput(expected_type);
    return token;
}

Token Parser::requireToken(Token::Type expected_type, std::string expected_value)
{
    const auto token = lexer->getToken();
    const auto type = token.getType();
    const auto value = token.getText();
    if (type != expected_type || value != expected_value)
        throwOnUnexpectedInput(Token(expected_type, expected_value));
    return token;
}

Token Parser::requireToken(Token::Type expected_type, int expected_value)
{
    const auto token = lexer->getToken();
    const auto type = token.getType();
    const auto value = token.getInteger();
    if (type != expected_type || value != expected_value)
        throwOnUnexpectedInput(expected_type);
    return token;
}

Token Parser::requireToken(Token::Type expected_type, double expected_value)
{
    const auto token = lexer->getToken();
    const auto type = token.getType();
    const auto value = token.getDecimal();
    if (type != expected_type || value != expected_value)
        throwOnUnexpectedInput(expected_type);
    return token;
}

bool Parser::checkTokenType(Token::Type expected) const
{
    return lexer->getToken().getType() == expected;
}

bool Parser::checkTokenValue(std::string expected) const
{
    return lexer->getToken().getText() == expected;
}

bool Parser::checkTokenValue(int expected) const
{
    return lexer->getToken().getInteger() == expected;
}

bool Parser::checkTokenValue(double expected) const
{
    return lexer->getToken().getDecimal() == expected;
}

bool Parser::checkToken(Token::Type type, std::string value) const
{
    return checkTokenType(type) && checkTokenValue(value);
}

bool Parser::checkToken(Token::Type type, int value) const
{
    return checkTokenType(type) && checkTokenValue(value);
}

bool Parser::checkToken(Token::Type type, double value) const
{
    return checkTokenType(type) && checkTokenValue(value);
}

void Parser::advance()
{
    lexer->readNextToken();
}
