#ifndef TKOM_PARSER_H
#define TKOM_PARSER_H

#include <memory>
#include <cctype>
#include <iostream>
#include <math.h>
#include <vector>
#include <istream>
#include <token/Token.h>
#include <lexer/Lexer.h>
#include <ast/include/Node.h>
#include <ast/include/Scope.h>
#include <ast/include/IntegerLiteral.h>
#include <ast/include/DecimalLiteral.h>
#include <ast/include/TextLiteral.h>
#include <ast/include/Identifier.h>
#include <ast/include/Array.h>
#include <ast/include/Hexgrid.h>
#include <ast/include/HexgridCell.h>
#include <ast/include/IndexingExpression.h>
#include <ast/include/LogicalNegation.h>
#include <ast/include/ArithmeticalNegation.h>
#include <ast/include/FOArithmExpression.h>
#include <ast/include/SOArithmExpression.h>
#include <ast/include/HexgridExpression.h>
#include <ast/include/ComparisonExpression.h>
#include <ast/include/AndExpression.h>
#include <ast/include/OrExpression.h>
#include <ast/include/DeclarationStatement.h>
#include <ast/include/AssignmentStatement.h>
#include <ast/include/FunctionCall.h>
#include <ast/include/IfStatement.h>
#include <ast/include/ConditionBlock.h>
#include <ast/include/ForeachStatement.h>
#include <ast/include/ReturnStatement.h>
#include <ast/include/AddStatement.h>
#include <ast/include/RemoveStatement.h>
#include <ast/include/MoveStatement.h>
#include <ast/include/FunctionDefinition.h>


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

#endif // TKOM_PARSER_H

