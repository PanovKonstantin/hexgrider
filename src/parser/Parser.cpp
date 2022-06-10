#include "Parser.h"
using namespace parser;
using namespace lexer;
using namespace token;
using namespace ast;
using namespace std;

Parser::Parser(unique_ptr<Lexer> lexer_):lexer(move(lexer_)){}
Parser::~Parser(){}

// Parses script, return statement and function definitions in a scope.
// Function definitions can be only placed outside 
unique_ptr<Program> Parser::parse()
{
    advance();
    auto program = make_unique<Program>();
    unique_ptr<Node> stmnt;
    unique_ptr<FunctionDefinition> func;
    while((stmnt = readStatement()) || (func = readFuncDef())){
        if(stmnt)   program->insertStatement(move(stmnt));
        else        program->insertFunction(move(func));
    }
    return program;
}

unique_ptr<Node> Parser::readStatement()
{
    unique_ptr<Node> stmnt;
    if  (   (stmnt = readIfStatement())
        ||  (stmnt = readForeachStatement())
    ) return stmnt; // Statements that do not require semicolon in the end
    if  (
        !(  (stmnt = readFuncCallOrAssignment())
        ||  (stmnt = readDeclrOrInit())
        ||  (stmnt = readReturnStatement())
        ||  (stmnt = readAddStatement())
        ||  (stmnt = readRemoveStatement())
        ||  (stmnt = readMoveStatement())
        )) return nullptr;
    consume(Token::Type::Semicolon);
    return stmnt;
}

unique_ptr<Node> Parser::readDeclrOrInit()
{
    auto declr = readDeclr();
    if(!declr) return nullptr;
    if(!consumeIfCheck(Token::Type::AssignOperator)) return declr;
    auto expr = readExpression();
    if(!expr) throwOnUnexpectedInput("a variable or a value");
    return make_unique<InitializationStatement>(
        declr->type, declr->identifier, move(expr));
}

unique_ptr<Node> Parser::readFuncCallOrAssignment()
{
    if(!checkToken(Token::Type::Identifier)) return nullptr;
    const auto id = current_token.getText();
    advance();
    auto assignment = readAssignment(id);
    if(assignment) return assignment;
    auto functionCall = readFunctionCall(id);
    if(!functionCall) throwOnUnexpectedInput("an assignment or a function call");
    return functionCall;
}

unique_ptr<Node> Parser::readAssignment(string name)
{
    if(!consumeIfCheck(Token::Type::AssignOperator)) return nullptr;
    auto expr = readExpression();
    if (!expr) throwOnUnexpectedInput("a variable or a value");
    return make_unique<AssignmentStatement>(name, move(expr));
}

unique_ptr<VariableDeclarationStatement> Parser::readDeclr()
{
    if(!isVarType()) return nullptr;
    auto varType = getVarType();
    advance();
    requireToken(Token::Type::Identifier);
    const auto identifier = current_token.getText();
    advance();
    return make_unique<VariableDeclarationStatement>(varType, identifier);
}

unique_ptr<Node> Parser::readIfStatement()
{
    auto ifBlock = readIfBlock();
    if(!ifBlock) return nullptr;
    auto elifBlocks = readElifBlocks();
    auto elseBlock = readElseBlock();
    return make_unique<IfStatement>(
        move(ifBlock),
        move(elifBlocks),
        move(elseBlock)
    );
}

unique_ptr<Node> Parser::readIfBlock(){
    if(!consumeIfCheck(Token::Type::IfKeyword)) return nullptr;
    auto ifBlock = readConditionBlock();
    if(!ifBlock) throwOnUnexpectedInput("an \"if\" block");
    return ifBlock;
}

vector<unique_ptr<Node>> Parser::readElifBlocks(){
    auto elifBlocks = vector<unique_ptr<Node>>();
    unique_ptr<Node> elifBlock;
    while((elifBlock = readElifBlock()))
        elifBlocks.push_back(move(elifBlock));
    return elifBlocks;
}

unique_ptr<Node> Parser::readElifBlock(){
    if(!consumeIfCheck(Token::Type::ElifKeyword)) return nullptr;
    auto elifBlock = readConditionBlock();
    if(!elifBlock) throwOnUnexpectedInput("an \"elif\" block");
    return elifBlock;
}

unique_ptr<Node> Parser::readElseBlock(){
    if(!consumeIfCheck(Token::Type::ElseKeyword)) return nullptr;
    auto elseBlock = readStatementBlock();
    if(!elseBlock) throwOnUnexpectedInput("an \"else\" block");
    return elseBlock;
}

unique_ptr<ConditionBlock> Parser::readConditionBlock()
{
    auto cond = readCondition();
    if(!cond) return nullptr;
    auto block = readStatementBlock();
    if(!block) throwOnUnexpectedInput("a statement block");
    return make_unique<ConditionBlock>(move(cond), move(block));
}

unique_ptr<Node> Parser::readCondition()
{
    if(!consumeIfCheck(Token::Type::LeftParenthese)) return nullptr;
    auto cond = readExpression();
    consume(Token::Type::RightParenthese);
    return cond;
}

unique_ptr<Node> Parser::readForeachStatement()
{
    if(!consumeIfCheck(Token::Type::ForeachKeyword)) return nullptr;
    auto iterator = readDeclr();
    if(!iterator) throwOnUnexpectedInput("a declaration");
    consume(Token::Type::InKeyword);
    auto iterated = readExpression();
    if(!iterated) throwOnUnexpectedInput("a variable or a value");
    auto scope = readStatementBlock();
    if(!scope) throwOnUnexpectedInput("a statement block");
    return make_unique<ForeachStatement>(move(iterator),
                                              move(iterated),
                                              move(scope));
}

unique_ptr<Node> Parser::readReturnStatement()
{
    if(!consumeIfCheck(Token::Type::ReturnKeyword)) return nullptr;
    auto expr = readExpression();
    return make_unique<ReturnStatement>(move(expr));
}

unique_ptr<Node> Parser::readAddStatement()
{
    if(!consumeIfCheck(Token::Type::AddKeyword)) return nullptr;
    auto being_added = readExpression();
    if(!being_added) throwOnUnexpectedInput("a value or a variable");
    consume(Token::Type::ToKeyword);
    auto added_to = readVariableReference();
    if(!added_to) throwOnUnexpectedInput("a variable");
    consume(Token::Type::AtKeyword);
    auto added_at = readExpression();
    if(!added_at) throwOnUnexpectedInput("a value or a variable");
    return make_unique<AddStatement>(move(being_added),
                                          move(added_to),
                                          move(added_at));
}

unique_ptr<Node> Parser::readRemoveStatement()
{    
    if(!consumeIfCheck(Token::Type::RemoveKeyword)) return nullptr;
    auto pos = readExpression();
    if(!pos) throwOnUnexpectedInput("a value or a variable");
    consume(Token::Type::FromKeyword);
    auto grid = readVariableReference();
    if(!grid) throwOnUnexpectedInput("a variable");
    return make_unique<RemoveStatement>(move(pos),
                                        move(grid));
}

unique_ptr<Node> Parser::readMoveStatement()
{
    if(!consumeIfCheck(Token::Type::MoveKeyword)) return nullptr;
    auto pos1 = readExpression();
    if(!pos1) throwOnUnexpectedInput("a value or a variable");
    consume(Token::Type::FromKeyword);
    auto source = readVariableReference();
    if(!source) throwOnUnexpectedInput("a variable");
    consume(Token::Type::ToKeyword);
    auto target = readVariableReference();
    if(!target) throwOnUnexpectedInput("a variable");
    if(consumeIfCheck(Token::Type::AtKeyword)){
        auto pos2 = readExpression();
        if(!pos2) throwOnUnexpectedInput("a value or a variable");
        return make_unique<MoveStatement>(
            move(pos1), move(source), move(target), move(pos2)
        );
    }
    return make_unique<MoveStatement>(
        move(pos1), move(source), move(target), nullptr
    );
}

unique_ptr<Node> Parser::readStatementBlock()
{
    if(!consumeIfCheck(Token::Type::LeftBrace)) return nullptr;
    auto statments = vector<unique_ptr<Node>>();
    unique_ptr<Node> stmnt;
    while((stmnt = readStatement()))
        statments.push_back(move(stmnt));
    consume(Token::Type::RightBrace);
    return make_unique<StatementBlock>(move(statments));
}

unique_ptr<FunctionDefinition> Parser::readFuncDef()
{
    if(!consumeIfCheck(Token::Type::FuncKeyword)) return nullptr;
    auto start = prevTokenStart;
    auto declr = readDeclr();
    if(!declr) throwOnUnexpectedInput("a return value declaration");

    consume(Token::Type::LeftParenthese); 
    auto params = readParamList();
    consume(Token::Type::RightParenthese);
    auto scope = readStatementBlock();
    if(!scope) throwOnUnexpectedInput("a statement block");
    auto end = prevTokenEnd;
    return make_unique<FunctionDefinition>(
        declr->type,
        declr->identifier,
        move(params),
        move(scope),
        start, end
    );
}

vector<unique_ptr<VariableDeclarationStatement>> Parser::readParamList()
{
    auto params = vector<unique_ptr<VariableDeclarationStatement>>();
    auto param = readDeclr();
    if(!param) return params;
    params.push_back(move(param));
    while(consumeIfCheck(Token::Type::Comma)){
        param = readDeclr();
        if(!param) throwOnUnexpectedInput("a parameter after a comma");
        params.push_back(move(param));
    }
    return params;    
}


unique_ptr<Node> Parser::readExpression()
{
    return readOrExpression();
} 

unique_ptr<Node> Parser::readOrExpression()
{
    auto expr = readAndExpression();
    if(!expr) return nullptr;
    while(consumeIfCheck(Token::Type::OrOperator))
    {
        auto rvalue = readAndExpression();
        if(!rvalue) throwOnUnexpectedInput("a value or a variable");
        expr = make_unique<OrExpression>(move(expr), move(rvalue));
    }
    return expr;
}

unique_ptr<Node> Parser::readAndExpression()
{
    auto expr = readComparisonExpression();
    if(!expr) return nullptr;
    while(consumeIfCheck(Token::Type::AndOperator))
    {
        auto rvalue = readComparisonExpression();
        if(!rvalue) throwOnUnexpectedInput("a value or a variable");
        expr = make_unique<AndExpression>(move(expr), move(rvalue));
    }
    return expr;
}

unique_ptr<Node> Parser::readComparisonExpression()
{
    auto expr = readHexgridExpression();
    if(!expr) return nullptr;
    if(isComparisonOperator()){
        auto op = current_token.getType();
        advance();
        auto rvalue = readHexgridExpression();
        if(!rvalue) throwOnUnexpectedInput("a value or a variable");
        expr = buildComparison(op, move(expr), move(rvalue));
    }
    return expr;
}

unique_ptr<BinaryExpression> Parser::buildComparison(
    Token::Type op, unique_ptr<Node> l, unique_ptr<Node> r
) {
    switch(op)
    {
        case Token::Type::LessOperator:
            return make_unique<LessExpression>(move(l), move(r));
        case Token::Type::LessOrEqualOperator:
            return make_unique<LessOrEqualExpression>(move(l), move(r));
        case Token::Type::GreaterOperator:
            return make_unique<GreaterExpression>(move(l), move(r));
        case Token::Type::GreaterOrEqualOperator:
            return make_unique<GreaterOrEqualExpression>(move(l), move(r));
        case Token::Type::EqualOperator:
            return make_unique<EqualExpression>(move(l), move(r));
        case Token::Type::NotEqualOperator:
            return make_unique<NotEqualExpression>(move(l), move(r));
        default:
            return nullptr;
    }

}

unique_ptr<Node> Parser::readHexgridExpression()
{
    auto expr = readAddSubExpression();
    if(!expr) return nullptr;
    if(consumeIfCheck(Token::Type::OnOperator)){
        auto rvalue = readHexgridExpression();
        expr = make_unique<OnExpression>(move(expr), move(rvalue));
    } else if (consumeIfCheck(Token::Type::ByOperator)){
        auto rvalue = readHexgridExpression();
        expr = make_unique<ByExpression>(move(expr), move(rvalue));
    } else if (consumeIfCheck(Token::Type::BesideOperator)){
        auto rvalue = readHexgridExpression();
        expr = make_unique<BesideExpression>(move(expr), move(rvalue));
    }
    return expr;
}

unique_ptr<Node> Parser::readAddSubExpression()
{
    auto expr = readMulModDivExpression();
    if(!expr) return nullptr;
    while(checkToken(Token::Type::AddOperator) ||
          checkToken(Token::Type::SubstructOperator))
    {
        auto op = current_token.getType();
        advance();
        auto rvalue = readMulModDivExpression();
        if(!rvalue) throwOnUnexpectedInput("a value or a variable");
        if (op == Token::Type::AddOperator)
            expr = make_unique<AddExpression>( move(expr),
                                                    move(rvalue));
        else expr = make_unique<SubtructExpression>( 
                                                    move(expr),
                                                    move(rvalue));
    }
    return expr;
}
unique_ptr<Node> Parser::readMulModDivExpression()
{
    auto expr = readArithmNegExpression();
    if(!expr) return nullptr;
    while(checkToken(Token::Type::MultiplyOperator) 
        || checkToken(Token::Type::ModuloOperator) 
        || checkToken(Token::Type::DivideOperator))
    {
        auto op = current_token.getType();
        advance();
        auto rvalue = readArithmNegExpression();
        if(!rvalue) throwOnUnexpectedInput("a value or a variable");
        if (op == Token::Type::MultiplyOperator)
            expr = make_unique<MultiplyExpression>(
                move(expr), move(rvalue));
        else if (op == Token::Type::ModuloOperator)
            expr = make_unique<ModuloExpression>(
                move(expr), move(rvalue));
        else
            expr = make_unique<DivideExpression>(
                move(expr), move(rvalue));
    }
    return expr;
}

unique_ptr<Node> Parser::readArithmNegExpression()
{
    if (!consumeIfCheck(Token::Type::SubstructOperator))
        return readLogicNegExpression();
    auto expr = readLogicNegExpression();
    if(!expr) throwOnUnexpectedInput("a variable or a value");
    return make_unique<ArithmeticalNegation>(move(expr));
}

unique_ptr<Node> Parser::readLogicNegExpression()
{
    if (!consumeIfCheck(Token::Type::LogicalNegationOperator))
        return readIndexingExpression();
    auto expr = readIndexingExpression();
    if(!expr) throwOnUnexpectedInput("a variable or a value");
    return make_unique<LogicalNegation>(move(expr));
}

unique_ptr<Node> Parser::readIndexingExpression()
{
    auto expr = readTerm();
    if(!expr) return nullptr;
    while (consumeIfCheck(Token::Type::LeftBracket))
    {
        auto indexOn = move(expr);
        auto indexBy = readExpression();
        if(!indexBy) throwOnUnexpectedInput("an index");
        expr = make_unique<IndexingExpression>(move(indexOn), move(indexBy));
        consume(Token::Type::RightBracket);
    }
    return expr;
}

unique_ptr<Node> Parser::readTerm()
{
    auto term = readIntegerLiteral(); 
    if (!term) term = readDecimalLiteral();
    if (!term) term = readTextLiteral();
    if (!term) term = readVariableOrFuncCall();
    if (!term) term = readArray();
    if (!term) term = readHexgrid();
    if (!term) term = readSubExpression();
    return term;
}

unique_ptr<Node> Parser::readSubExpression()
{
    if(!consumeIfCheck(Token::Type::LeftParenthese)) return nullptr;
    auto expr = readExpression();
    if(!expr) throwOnUnexpectedInput("a value or a variable");
    consume(Token::Type::RightParenthese);
    return expr;
}


unique_ptr<Node> Parser::readTextLiteral()
{
    
    if(!checkToken(Token::Type::Text)) return nullptr;
    auto literal = make_unique<TextLiteral>(current_token.getText());
    advance(); 
    return literal;
}

unique_ptr<Node> Parser::readDecimalLiteral()
{
    if(!checkToken(Token::Type::Decimal)) return nullptr;
    auto literal = make_unique<DecimalLiteral>(current_token.getDecimal());
    advance(); 
    return literal;
}

unique_ptr<Node> Parser::readIntegerLiteral()
{
    if(!checkToken(Token::Type::Integer)) return nullptr;
    auto literal = make_unique<IntegerLiteral>(current_token.getInteger());
    advance(); 
    return literal;
}

unique_ptr<VariableReference> Parser::readVariableReference(){
    if(!checkToken(Token::Type::Identifier)) return nullptr;
    const auto id = current_token.getText();
    advance();
    return make_unique<VariableReference>(id);
}

unique_ptr<Node> Parser::readVariableOrFuncCall()
{
    if(!checkToken(Token::Type::Identifier)) return nullptr;
    const auto id = current_token.getText();
    advance();
    auto funcCall = readFunctionCall(id);
    if (funcCall) return funcCall;
    return make_unique<VariableReference>(id);
}

unique_ptr<Node> Parser::readFunctionCall(string func_name)
{
    if (!consumeIfCheck(Token::Type::LeftParenthese)) return nullptr;
    auto args = readElementList();
    consume(Token::Type::RightParenthese);
    return make_unique<FunctionCall>(func_name, move(args));
}

vector<unique_ptr<Node>> Parser::readElementList(){
    auto args = vector<unique_ptr<Node>>();
    auto arg = readExpression();
    if(!arg) return args;
    args.push_back(move(arg));
    while(consumeIfCheck(Token::Type::Comma)){
        arg = readExpression();
        if(!arg) throwOnUnexpectedInput("an expression after a comma");;
        args.push_back(move(arg));
    }
    return args;
}

vector<unique_ptr<Node>> Parser::readHexgridCellList(){
    auto cells = vector<unique_ptr<Node>>();
    auto cell = readHexgridCell();
    if(!cell) return cells;
    cells.push_back(move(cell));
    while(consumeIfCheck(Token::Type::Comma)){
        cell = readHexgridCell();
        if(!cell) throwOnUnexpectedInput("a hexgrid cell");
        cells.push_back(move(cell));
    }
    return cells;
}

unique_ptr<Node> Parser::readHexgridCell(){
    auto value = readExpression();
    if(!value) return nullptr;
    consume(Token::Type::AtKeyword);
    auto pos = readTerm();
    if(!pos)  throwOnUnexpectedInput("a value or a variable");
    return make_unique<HexgridCell>(move(value), move(pos));
}


unique_ptr<Node> Parser::readArray()
{
    if(!consumeIfCheck(Token::Type::LeftBracket)) return nullptr;
    auto elements = readElementList();
    consume(Token::Type::RightBracket);
    return make_unique<ArrayLiteral>(move(elements));
}

unique_ptr<Node> Parser::readHexgrid()
{
    if(!consumeIfCheck(Token::Type::LessOperator)) return nullptr;
    auto cells = readHexgridCellList();
    consume(Token::Type::GreaterOperator);
    return make_unique<HexgridLiteral>(move(cells));
}


bool Parser::isVarType()
{
    switch(current_token.getType()){
        case Token::Type::IntType:      
        case Token::Type::FloatType:    
        case Token::Type::StringType:   
        case Token::Type::HexgridType:  
        case Token::Type::ArrayType:    return true;
        default:                        return false;
    }
}
Variable::Type Parser::getVarType()
{
    Variable::Type t;
    switch(current_token.getType()){
        case Token::Type::IntType:      t = Variable::Type::Int;
                                        break;
        case Token::Type::FloatType:    t = Variable::Type::Float;
                                        break;
        case Token::Type::StringType:   t = Variable::Type::String;
                                        break;
        case Token::Type::HexgridType:  t = Variable::Type::Hexgrid;
                                        break;
        case Token::Type::ArrayType:    t = Variable::Type::Array;
                                        break;
        default:                        throwOnUnexpectedInput("a type specifier");
    }
    return t;
}


bool Parser::isComparisonOperator()
{
    return checkToken(Token::Type::LessOperator)            || 
           checkToken(Token::Type::LessOrEqualOperator)     || 
           checkToken(Token::Type::GreaterOperator)          || 
           checkToken(Token::Type::GreaterOrEqualOperator)  || 
           checkToken(Token::Type::EqualOperator)           || 
           checkToken(Token::Type::NotEqualOperator);
}

void Parser::throwOnUnexpectedInput(string expected)
{
    throw hexgrid_errors::UnexpectedInput(
        current_token.getStart(), 
        current_token.toString(),
        expected);
}

void Parser::throwOnUnexpectedInput(Token::Type expected)
{
    throw hexgrid_errors::UnexpectedInput(
        current_token.getStart(), 
        current_token.toString(),
        Token::toString(expected));
}


Token Parser::requireToken(Token::Type expected_type)
{
    const auto token = current_token;
    const auto type = token.getType();
    if (type != expected_type)
        throwOnUnexpectedInput(expected_type);
    return token;
}

bool Parser::checkToken(Token::Type expected) const
{
    return current_token.getType() == expected;
}


void Parser::advance()
{
    prevTokenStart = current_token.getStart();
    prevTokenEnd = current_token.getEnd();
    current_token = lexer->getToken();
}

void Parser::consume(Token::Type t)
{
    requireToken(t);
    advance();
}
bool Parser::consumeIfCheck(Token::Type t)
{
    if (!checkToken(t)) return false;
    advance();
    return true;
}