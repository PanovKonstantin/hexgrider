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
#include <ast/Ast.h>
#include <HexgridErrors.h>


namespace parser
{
class Parser
{
public:
    Parser(std::unique_ptr<lexer::Lexer> lexer_);
    ~Parser();

    std::unique_ptr<ast::Program> parse();
protected:
    std::unique_ptr<ast::Node> readScript();
    std::unique_ptr<ast::Node> readStatementOrFuncDef();
    std::unique_ptr<ast::FunctionDefinition> readFuncDef();
    std::vector<std::unique_ptr<ast::VariableDeclarationStatement>> readParamList();
    std::unique_ptr<ast::Node> readStatement();
    std::unique_ptr<ast::Node> readDeclrOrInit();
    std::unique_ptr<ast::Node> readFuncCallOrAssignment();
    std::unique_ptr<ast::VariableDeclarationStatement> readDeclr();
    std::unique_ptr<ast::Node> readAssignment(std::string);
    std::unique_ptr<ast::Node> readIfStatement();
    std::unique_ptr<ast::Node> readIfBlock();
    std::vector<std::unique_ptr<ast::Node>> readElifBlocks();
    std::unique_ptr<ast::Node> readElifBlock();
    std::unique_ptr<ast::Node> readElseBlock();
    std::unique_ptr<ast::ConditionBlock> readConditionBlock();
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
    std::unique_ptr<ast::Node> readAddSubExpression();
    std::unique_ptr<ast::Node> readMulModDivExpression();
    std::unique_ptr<ast::Node> readArithmNegExpression();
    std::unique_ptr<ast::Node> readLogicNegExpression();
    std::unique_ptr<ast::Node> readIndexingExpression();


    std::unique_ptr<ast::Node> readTerm();

    std::unique_ptr<ast::Node> readTextLiteral();
    std::unique_ptr<ast::Node> readDecimalLiteral();
    std::unique_ptr<ast::Node> readIntegerLiteral();
    std::unique_ptr<ast::Node> readVariableOrFuncCall();
    std::unique_ptr<ast::Node> readFunctionCall(std::string);
    std::unique_ptr<ast::Node> readArray();
    std::unique_ptr<ast::Node> readHexgrid();
    std::unique_ptr<ast::Node> readSubExpression();
    std::string readIdentifier();
    std::vector<std::unique_ptr<ast::Node>> readElementList();
    std::vector<std::unique_ptr<ast::Node>> readHexgridCellList();
    std::unique_ptr<ast::Node> readHexgridCell();
    std::unique_ptr<ast::BinaryExpression> buildComparison(token::Token::Type, std::unique_ptr<ast::Node>, std::unique_ptr<ast::Node>); 
    bool isComparisonOperator();
    bool isVarType();
    ast::Variable::Type getVarType();

    void throwOnUnexpectedInput(token::Token::Type expected);
    void throwOnUnexpectedInput(std::string expected);
    token::Token requireToken(token::Token::Type expected_type);



    void advance();
    bool checkToken(token::Token::Type expected) const;
    void consume(token::Token::Type);
    bool consumeIfCheck(token::Token::Type expected);
    

    std::unique_ptr<lexer::Lexer> lexer;
    token::Token current_token;
    std::pair<int, int> prevTokenStart;
    std::pair<int, int> prevTokenEnd;
};
} // namespace parser

#endif // TKOM_PARSER_H_

