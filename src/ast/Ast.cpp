#include "Ast.h"
using namespace ast;
using namespace std;

AddStatement::AddStatement(
                unique_ptr<Node> being_added_,
                unique_ptr<Node> added_to_,
                unique_ptr<Node> added_at_)
{
    being_added = move(being_added_);
    added_to = move(added_to_);
    added_at = move(added_at_);
}

string AddStatement::toString(int depth) const
{
    return string(depth, '|') + "Add Statement\n" + 
            being_added->toString(depth + 1) + 
            added_to->toString(depth + 1) + 
            added_at->toString(depth + 1);
}

string AndExpression::toString(int depth) const
{
    return string(depth, '|') + "And Expression\n" + 
            BinaryExpression::toString(depth);
}

ArithmeticalNegation::ArithmeticalNegation(unique_ptr<Node>  value_)
{
    value = move(value_);
}

string ArithmeticalNegation::toString(int depth) const
{
    return string(depth, '|') + "Arithmetical Negation Expression\n" + value->toString(depth + 1);
}

Array::Array(vector<unique_ptr<Node>> elements_)
{
    elements = move(elements_);
}

string Array::toString(int depth) const
{
    string ret_str = string(depth, '|') + "Array of length (" + 
                          to_string(elements.size()) + ")\n";
    for(auto const& element: elements)
    {
        ret_str += element->toString(depth + 1);
    }
    return ret_str;
}

AssignmentStatement::AssignmentStatement(
    string iden_, 
    unique_ptr<Node> value_)
{
    iden = iden_;
    value = move(value_);
}

string AssignmentStatement::toString(int depth) const
{
    return  string(depth, '|') + "Assignment (" + iden + ")\n" +
            value->toString(depth+1);
}

ConditionBlock::ConditionBlock(unique_ptr<Node> condition_,
                 unique_ptr<Node> statementBlock_)
{
    condition = move(condition_);
    statementBlock = move(statementBlock_);
}

string ConditionBlock::toString(int depth) const
{
    return string(depth, '|') + "Condition Block\n" + 
            condition->toString(depth + 1) + 
            statementBlock->toString(depth + 1);
}

DecimalLiteral::DecimalLiteral(double value_)
    : value(value_)
{
    
}

string DecimalLiteral::toString(int depth) const
{
    return string(depth, '|') + "Decimal Literal (" + to_string(value) + ")\n";
}

double DecimalLiteral::getValue() const {
    return value;
}

VariableDeclarationStatement::VariableDeclarationStatement(Variable::Type type_, 
                                            string iden_,
                                            int dimension_)
:type(type_), identifier(iden_), dimension(dimension_){}

string VariableDeclarationStatement::toString(int depth) const
{
    return  string(depth, '|') + "Variable Declaration (" + 
            Variable::typeToString(type) + " " + identifier + ")\n";
}

Variable::Variable(Variable::Type t):type(t){};
Variable::Variable(Variable::Type t, int v):type(t), value(v){};
// Variable::Variable(Variable::Type t):type(t){};

string Variable::typeToString(Variable::Type t)
{
    switch (t)
    {
    case Variable::Type::Int:     return "int";
    case Variable::Type::Float:   return "float";
    case Variable::Type::String:  return "string";
    case Variable::Type::Hexgrid: return "hexgrid";
    case Variable::Type::Array:   return "array";
    default:                    return "";
    }
}


InitializationStatement::InitializationStatement(
    Variable::Type type_, string name_, std::unique_ptr<Node> value_, int dim)
:type(type_), name(name_), value(move(value_)), dimension(dim){}


string InitializationStatement::getType() const
{
    return Variable::typeToString(type);
}

string InitializationStatement::toString(int depth) const
{
    return  string(depth, '|') + "Initialization (" +
            Variable::typeToString(type) + " " + name + ")\n" + 
            value->toString(depth+1);
}


string InitializationStatement::toString(Variable::Type type_) const
{
    switch (type_)
    {
    case Variable::Type::Int:
        return "int";
    case Variable::Type::Float:
        return "float";
    case Variable::Type::String:
        return "string";
    case Variable::Type::Hexgrid:
        return "hexgrid";
    default:
        throw runtime_error("Unkown comparison operator");
        return "";
    }
}

string MultiplyExpression::toString(int depth) const
{
    return string(depth, '|') + "Multiply Expression\n" + 
            BinaryExpression::toString(depth);
}

string DivideExpression::toString(int depth) const
{
    return string(depth, '|') + "Divide Expression\n" + 
            BinaryExpression::toString(depth);
}

string ModuloExpression::toString(int depth) const
{
    return string(depth, '|') + "Modulo Expression\n" + 
            BinaryExpression::toString(depth);
}

string NotEqualExpression::toString(int depth) const
{
    return string(depth, '|') + "NotEqual Expression\n" + 
            BinaryExpression::toString(depth);
}

string EqualExpression::toString(int depth) const
{
    return string(depth, '|') + "Equal Expression\n" + 
            BinaryExpression::toString(depth);
}

string GreaterOrEqualExpression::toString(int depth) const
{
    return string(depth, '|') + "GreaterOrEqual Expression\n" + 
            BinaryExpression::toString(depth);
}

string GreaterExpression::toString(int depth) const
{
    return string(depth, '|') + "Greater Expression\n" + 
            BinaryExpression::toString(depth);
}

string LessOrEqualExpression::toString(int depth) const
{
    return string(depth, '|') + "LessOrEqual Expression\n" + 
            BinaryExpression::toString(depth);
}

string LessExpression::toString(int depth) const
{
    return string(depth, '|') + "Less Expression\n" + 
            BinaryExpression::toString(depth);
}

ForeachStatement::ForeachStatement(
                unique_ptr<Node> iterator_,
                unique_ptr<Node> iterated_,
                unique_ptr<Node> statementBlock_)
{
    iterator = move(iterator_);
    iterated = move(iterated_);
    statementBlock = move(statementBlock_);
}

string ForeachStatement::toString(int depth) const
{
    return string(depth, '|') + "Foreach Statement\n" + 
            iterator->toString(depth + 1) + 
            iterated->toString(depth + 1) + 
            statementBlock->toString(depth + 1);
}

FunctionCall::FunctionCall(string funcName_, 
                           vector<unique_ptr<Node>> args_)
                           :funcName(funcName_), args(move(args_)){};

string FunctionCall::toString(int depth) const
{
    auto ret_str =  string(depth, '|') + "Function Call (" + funcName + ")\n";
    for(auto const& arg: args)
    {
        ret_str += arg->toString(depth + 1);
    }
    return ret_str;
           
}

FunctionDefinition::FunctionDefinition(
    Variable::Type type_, string name_,
    vector<unique_ptr<VariableDeclarationStatement>> params_,
    unique_ptr<Node> statementBlock_,
    pair<int, int> start_, pair<int, int> end_)
:   type(type_), name(name_), params(move(params_)), 
    statementBlock(move(statementBlock_)),
    startLoc(start_), endLoc(end_){}

string FunctionDefinition::toString(int depth) const
{
    string ret_str = string(depth, '|') + "Function Defenition ("+
    Variable::typeToString(type) + " " + name + ")\n";
    for(auto const& param: params)
        ret_str += param->toString(depth + 1);
    ret_str += statementBlock->toString(depth + 1);
    return ret_str;
}

Hexgrid::Hexgrid(vector<unique_ptr<Node>> cells_)
{
    cells = move(cells_);
}

string Hexgrid::toString(int depth) const
{
    string ret_str = string(depth, '|') + "Hexgrid\n";
    for(auto const& cell: cells)
    {
        ret_str += cell->toString(depth + 1);
    }
    return ret_str;
}

HexgridCell::HexgridCell(unique_ptr<Node> value_, unique_ptr<Node> pos_)
{
    pos = move(pos_);
    value = move(value_);
}

string HexgridCell::toString(int depth) const
{
    return string(depth, '|') + "Hexgrid Cell\n" +
           value->toString(depth + 1) + 
           pos->toString(depth + 1);
}

string ByExpression::toString(int depth) const
{
    return string(depth, '|') + "Hexgrid By Expression\n" + 
            BinaryExpression::toString(depth);
}

string BesideExpression::toString(int depth) const
{
    return string(depth, '|') + "Hexgrid Beside Expression\n" + 
            BinaryExpression::toString(depth);
}

string OnExpression::toString(int depth) const
{
    return string(depth, '|') + "Hexgrid On Expression\n" + 
            BinaryExpression::toString(depth);
}

VariableReference::VariableReference(string name_)
    : name(name_)
{
}
string VariableReference::getValue() {
    return name;
}
string VariableReference::toString(int depth) const
{
    return string(depth, '|') + "Variable reference (" + name + ")\n";
}

IfStatement::IfStatement(unique_ptr<Node> if_block_,
                         vector<unique_ptr<Node>> elif_blocks_,
                         unique_ptr<Node> else_block_)
{
    if_block = move(if_block_);
    else_block = move(else_block_);
    elif_blocks = move(elif_blocks_);
}

string IfStatement::toString(int depth) const
{
    string ret_str = string(depth, '|') + "If Statement\n" + 
                          if_block->toString(depth + 1);
    if (!elif_blocks.empty())
        for(auto const& elif_block: elif_blocks)
            ret_str += elif_block->toString(depth + 1);
    if (else_block)
        ret_str += else_block->toString(depth + 1);
    return ret_str;
}

IndexingExpression::IndexingExpression(unique_ptr<Node> indexOn_,
                                       unique_ptr<Node> indexBy_)
{
    indexOn = move(indexOn_);
    indexBy = move(indexBy_);
}

string IndexingExpression::toString(int depth) const
{
    return string(depth, '|') + "Indexing Expression\n" + 
           indexOn->toString(depth + 1) + 
           indexBy->toString(depth + 1);
}

IntegerLiteral::IntegerLiteral(int value_)
    : value(value_)
{
    
}

string IntegerLiteral::toString(int depth) const
{
    return string(depth, '|') + "Integer Literal (" + to_string(value) + ")\n";
}

Literal::Literal()
{
}

string Literal::toString(int depth) const
{
    return string(depth, '|') + "Blank literal\n";
}

LogicalNegation::LogicalNegation(unique_ptr<Node>  value_)
{
    value = move(value_);
}

string LogicalNegation::toString(int depth) const
{
    return string(depth, '|') + "Logical Negation Expression\n" + value->toString(depth + 1);
}

MoveStatement::MoveStatement(unique_ptr<Node> position_source_,
                             unique_ptr<Node> grid_source_,
                             unique_ptr<Node> grid_target_,
                             unique_ptr<Node> position_target_)
{
    position_source = move(position_source_);
    position_target = move(position_target_);
    grid_source = move(grid_source_);
    grid_target = move(grid_target_);
}

string MoveStatement::toString(int depth) const
{
    return string(depth, '|') + "Move Statement\n" +
            position_source->toString(depth + 1) + 
            grid_source->toString(depth + 1) + 
            grid_target->toString(depth + 1) + 
            position_target->toString(depth + 1);
}

Node::~Node()
{}


BinaryExpression::BinaryExpression(unique_ptr<Node> lvalue_,
                                   unique_ptr<Node> rvalue_)
: lvalue(move(lvalue_)), rvalue(move(rvalue_)){};

string BinaryExpression::toString(int depth) const {
    return lvalue->toString(depth + 1) +
           rvalue->toString(depth + 1);
}

string OrExpression::toString(int depth) const
{
    return string(depth, '|') + "Or Expression\n" + 
            BinaryExpression::toString(depth);
}

RemoveStatement::RemoveStatement(unique_ptr<Node> position_,
                                 unique_ptr<Node> grid_)
{
    position = move(position_);
    grid = move(grid_);
}

string RemoveStatement::toString(int depth) const
{
    return string(depth, '|') + "Remove Statement\n" +
            position->toString(depth + 1) + 
            grid->toString(depth + 1);
}

ReturnStatement::ReturnStatement(unique_ptr<Node> expr_)
{
    expr = move(expr_);
}

string ReturnStatement::toString(int depth) const
{
    return string(depth, '|') + "Return Statement\n" + 
            expr->toString(depth + 1);
}

StatementBlock::StatementBlock(vector<unique_ptr<Node>> stmnts_)
{
    stmnts = move(stmnts_);
}

string StatementBlock::toString(int depth) const
{
    string ret_str = string(depth, '|') + "StatementBlock\n";
    for(auto const& stmnt: stmnts)
    {
        ret_str += stmnt->toString(depth + 1);
    }
    return ret_str;
}

Program::Program(){
    stmnts = vector<unique_ptr<Node>>();
};

string Program::toString(int depth) const
{
    string ret_str = string(depth, '|') + "Program\n";
    for(auto const& func: funcs) ret_str += func.second->toString(depth + 1);
    for(auto const& stmnt: stmnts) ret_str += stmnt->toString(depth + 1);
    return ret_str;
}

void Program::insertStatement(unique_ptr<Node> stmnt)
{
    stmnts.push_back(std::move(stmnt));
}

void Program::insertFunction(unique_ptr<FunctionDefinition> func)
{
    if (funcs.count(func->name)) throw hexgrid_errors::FunctionRedefinition(func->startLoc, func->endLoc, func->name);
    funcs[func->name] = move(func);
    // funcs.push_back(std::move(func));
}

string AddExpression::toString(int depth) const
{
    return string(depth, '|') + "Add Expression\n" + 
            BinaryExpression::toString(depth);
}

string SubtructExpression::toString(int depth) const
{
    return string(depth, '|') + "Substruct Expression\n" + 
            BinaryExpression::toString(depth);
}

TextLiteral::TextLiteral(string value_): value(value_){}
string TextLiteral::toString(int depth) const
{
    return string(depth, '|') + "Text Literal (" + value + ")\n";
}
/* 
Variable::Variable(Type t, int d): type(t), dimension(d){}
Variable::Variable():type(Type::Undefiend), dimension(0){}
int Variable::getInteger(vector<int> index_){
    if (type != Type::Int){
        throw runtime_error("Got wrong type");
    }
    if (index_.empty() && dimension==0){
        return get<int>(value);
    }
    auto elem = get<vector<Variable>>(value);
    auto sub_elem = elem[index_.back()];
    index_.pop_back();
    return sub_elem.getInteger(index_);
}

void Variable::setValue(int value_, vector<int> index_){
    if (type != Type::Int){
        throw runtime_error("Got wrong type");
    }
    if (index_.empty() && !dimension){
        value = value_;
        return;
    }
    if (index_.size() == dimension){
        auto elem = get<vector<Variable>>(value);
        auto sub_elem = elem[index_.back()];
        index_.pop_back();
        sub_elem.setValue(value_, index_);
    }
}

Variable* StatementBlock::calculate(StatementBlock& sc) {
    upper_statementBlock = &sc;
    for(auto const& stmnt : stmnts){
        stmnt->calculate(*this);
    }
    return nullptr;
}

void StatementBlock::declareVariable(string name, ast::Variable var){
    variables[name] = var;
}

void StatementBlock::assignVariable(string name, ast::Variable var){
    Variable variabl;
    if (variables.count(name)){
        variabl = variables[name];
        
    }
    if (!variables.count(name))
    {
        if (!upper_statementBlock){
            throw runtime_error("Variable " + name + " assigned before declared\n");
        }
        upper_statementBlock->assignVariable(name, variabl);
    }
    // if 
}

Variable* VariableDeclarationStatement::calculate(StatementBlock& statementBlock) {
    Variable::Type t; 
    switch (type){
        case Type::Int: t = Variable::Type::Int;
                        break;
        default:
            throw runtime_error("Unkowwn type");
    }
    Variable v = Variable(t);
    statementBlock.declareVariable(identifier, v); 
    return nullptr;
}

string VariableDeclarationStatement::getIdentifier(){
    return identifier;
} */

/* Variable* AssignmentStatement::calculate(StatementBlock& statementBlock) {
    string name;
    if(decl){
        decl->calculate(statementBlock);
        name = decl->getIdentifier();
    } else {
        name = iden->getValue();
    }
    Variable* v = value->calculate(statementBlock);
    return nullptr;
}
 */

/* 
Variable* AddStatement::calculate(StatementBlock& statementBlock) {return nullptr;}
Variable* ConditionBlock::calculate(StatementBlock& statementBlock) {return nullptr;}
Variable* ForeachStatement::calculate(StatementBlock& statementBlock) {return nullptr;}
Variable* FunctionCall::calculate(StatementBlock& statementBlock) {return nullptr;}
Variable* FunctionDefinition::calculate(StatementBlock& statementBlock) {return nullptr;}
Variable* IfStatement::calculate(StatementBlock& statementBlock) {return nullptr;}
Variable* MoveStatement::calculate(StatementBlock& statementBlock) {return nullptr;}
Variable* RemoveStatement::calculate(StatementBlock& statementBlock) {return nullptr;}
Variable* ReturnStatement::calculate(StatementBlock& statementBlock) {return nullptr;}
Variable* OrExpression::calculate(StatementBlock& statementBlock) {return nullptr;}
Variable* AndExpression::calculate(StatementBlock& statementBlock) {return nullptr;}
Variable* LogicalNegation::calculate(StatementBlock& statementBlock) {return nullptr;}
Variable* ArithmeticalNegation::calculate(StatementBlock& statementBlock) {return nullptr;}
Variable* IndexingExpression::calculate(StatementBlock& statementBlock) {return nullptr;}
Variable* Literal::calculate(StatementBlock& statementBlock) {return nullptr;}
Variable* TextLiteral::calculate(StatementBlock& statementBlock) {return nullptr;}
Variable* IntegerLiteral::calculate(StatementBlock& statementBlock) {return nullptr;}
Variable* DecimalLiteral::calculate(StatementBlock& statementBlock) {return nullptr;}
Variable* Hexgrid::calculate(StatementBlock& statementBlock) {return nullptr;}
Variable* HexgridCell::calculate(StatementBlock& statementBlock) {return nullptr;}
Variable* Identifier::calculate(StatementBlock& statementBlock) {return nullptr;}
Variable* Array::calculate(StatementBlock& statementBlock) {return nullptr;}
 */
