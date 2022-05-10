#ifndef TKOM_PARSER_H_
#define TKOM_PARSER_H_

#include <memory>
#include <cctype>
#include <iostream>
#include <math.h>
#include <vector>
#include <istream>
#include <lexer/Token.h>
#include <lexer/Lexer.h>
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


namespace parser
{
class Parser
{
public:
    Parser(std::unique_ptr<lexer::Lexer> lexer_);
    ~Parser();

    std::unique_ptr<ast::Node> parse();

    std::unique_ptr<ast::Node> readScript();
    std::unique_ptr<ast::Node> readStatementOrFuncDef();
    std::unique_ptr<ast::Node> readFuncDef();
    std::unique_ptr<ast::Node> readStatement();
    std::unique_ptr<ast::Node> readDeclrOrInit();
    std::unique_ptr<ast::Node> readFuncCallOrAssignment();
    std::unique_ptr<ast::Node> readDeclr();
    std::unique_ptr<ast::Node> readAssignment(std::unique_ptr<ast::Node>);
    std::unique_ptr<ast::Node> readFunctionCall(std::unique_ptr<ast::Node>);
    std::unique_ptr<ast::Node> readIfStatement();
    std::unique_ptr<ast::Node> readForeachStatement();
    std::unique_ptr<ast::Node> readReturnStatement();
    std::unique_ptr<ast::Node> readAddStatement();
    std::unique_ptr<ast::Node> readRemoveStatement();
    std::unique_ptr<ast::Node> readMoveStatement();

    std::unique_ptr<ast::Node> readCondition();
    std::unique_ptr<ast::Node> readStatementBlock();

    std::unique_ptr<ast::Node> readExpression();
    std::unique_ptr<ast::Node> readOrExpression();
    std::unique_ptr<ast::Node> readAndExpression();
    std::unique_ptr<ast::Node> readComparisonExpression();
    std::unique_ptr<ast::Node> readHexgridExpression();
    std::unique_ptr<ast::Node> readSOArithmExpression();
    std::unique_ptr<ast::Node> readFOArithmExpression();
    std::unique_ptr<ast::Node> readArithmNegExpression();
    std::unique_ptr<ast::Node> readLogicNegExpression();
    std::unique_ptr<ast::Node> readIndexingExpression();


    std::unique_ptr<ast::Node> readTerm();

    std::unique_ptr<ast::Node> readTextLiteral();
    std::unique_ptr<ast::Node> readDecimalLiteral();
    std::unique_ptr<ast::Node> readIntegerLiteral();
    std::unique_ptr<ast::Node> readIdentifier();
    std::unique_ptr<ast::Node> readIdentifierOrFuncCall();
    std::unique_ptr<ast::Node> readArray();
    std::unique_ptr<ast::Node> readHexgrid();

    bool isComparisonOperator();

    void throwOnUnexpectedInput(token::Token::Type expected);
    void throwOnUnexpectedInput(std::string expected);
    void throwOnUnexpectedInput(token::Token t);
    token::Token requireToken(token::Token::Type expected_type);
    token::Token requireToken(token::Token::Type expected_type, std::string expected_value);
    token::Token requireToken(token::Token::Type expected_type, int expected_value);
    token::Token requireToken(token::Token::Type expected_type, double expected_value);
    bool checkTokenType(token::Token::Type expected) const;
    bool checkTokenValue(std::string expected) const;
    bool checkTokenValue(int expected) const;
    bool checkTokenValue(double expected) const;
    bool checkToken(token::Token::Type type, std::string value) const;
    bool checkToken(token::Token::Type type, int value) const;
    bool checkToken(token::Token::Type type, double value) const;

    void advance();

    std::unique_ptr<lexer::Lexer> lexer;
};
} // namespace parser

#endif // TKOM_PARSER_H_

