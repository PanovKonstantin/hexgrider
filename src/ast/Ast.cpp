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

AndExpression::AndExpression(unique_ptr<Node> lvalue_,
                             unique_ptr<Node> rvalue_)
{
    lvalue = move(lvalue_);
    rvalue = move(rvalue_);
}

string AndExpression::toString(int depth) const
{
    return string(depth, '|') + "And Expression\n" +
            lvalue->toString(depth + 1) +
            rvalue->toString(depth + 1);
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
    unique_ptr<DeclarationStatement> declr_, 
    unique_ptr<Node> value_)
{
    iden = nullptr;
    decl = move(declr_);
    value = move(value_);
}

AssignmentStatement::AssignmentStatement(
    unique_ptr<Identifier> iden_, 
    unique_ptr<Node> value_)
{
    decl = nullptr;
    iden = move(iden_);
    value = move(value_);
}

string AssignmentStatement::toString(int depth) const
{
    if (decl){
        return  string(depth, '|') + "Assignment\n" + 
                decl->toString(depth+1) + 
                value->toString(depth+1);
    } else {
        return  string(depth, '|') + "Assignment\n" + 
                iden->toString(depth+1) + 
                value->toString(depth+1);
    }
           
}

ComparisonExpression::ComparisonExpression(unique_ptr<Node> lvalue_,
                                       Operator op_,
                                       unique_ptr<Node> rvalue_)
    : op(op_)
{
    lvalue = move(lvalue_);
    rvalue = move(rvalue_);
}

string ComparisonExpression::toString(int depth) const
{
    return string(depth, '|') + toString(op) +
            lvalue->toString(depth + 1) +
            rvalue->toString(depth + 1);
}

string ComparisonExpression::toString(Operator op_) const
{
    switch (op_)
    {
    case Operator::Less:            return "Less Expression\n";
    case Operator::LessOrEqual:     return "LessOrEqual Expression\n";
    case Operator::Greater:         return "Greater Expression\n";
    case Operator::GreaterOrEqual:  return "GreaterOrEqual Expression\n";
    case Operator::Equal:           return "Equal Expression\n";
    case Operator::NotEqual:        return "NotEqual Expression\n";
    default:
        throw runtime_error("Unkown comparison operator");
        return "";
    }
}

ConditionBlock::ConditionBlock(unique_ptr<Node> condition_,
                 unique_ptr<Node> scope_)
{
    condition = move(condition_);
    scope = move(scope_);
}

string ConditionBlock::toString(int depth) const
{
    return string(depth, '|') + "Condition Block\n" + 
            condition->toString(depth + 1) + 
            scope->toString(depth + 1);
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

DeclarationStatement::DeclarationStatement(Type type_, 
                                            unique_ptr<Identifier> iden_,
                                            int dimenstion_)
{
    type = type_;
    iden = move(iden_);
    dimenstion=dimenstion_;
}

string DeclarationStatement::toString(int depth) const
{
    return string(depth, '|') + "Declaration\n" + 
           string(depth + 1, '|') + "Variable Type (" + getType() + ")\n" +
           iden->toString(depth+1);
}

string DeclarationStatement::getType() const
{
    if (dimenstion > 0)
        return to_string(dimenstion) + "d array of type " + toString(type);
    else
        return toString(type);
}


string DeclarationStatement::toString(Type type_) const
{
    switch (type_)
    {
    case Type::Int:
        return "int";
    case Type::Float:
        return "float";
    case Type::String:
        return "string";
    case Type::Hexgrid:
        return "hexgrid";
    default:
        throw runtime_error("Unkown comparison operator");
        return "";
    }
}
FOArithmExpression::FOArithmExpression(unique_ptr<Node> lvalue_,
                                       Operator op_,
                                       unique_ptr<Node> rvalue_)
    : op(op_)
{
    lvalue = move(lvalue_);
    rvalue = move(rvalue_);
}

string FOArithmExpression::toString(int depth) const
{
    switch (op)
    {
    case Operator::Multiply:
        return string(depth, '|') + "Multiply Expression\n" +
               lvalue->toString(depth + 1) +
               rvalue->toString(depth + 1);
    case Operator::Divide:
        return string(depth, '|') + "Divide Expression\n" +
               lvalue->toString(depth + 1) +
               rvalue->toString(depth + 1);
    default:
        throw runtime_error("Arithmetical Expression without operator");
        return "";
    };
}

ForeachStatement::ForeachStatement(
                unique_ptr<Node> iterator_,
                unique_ptr<Node> iterated_,
                unique_ptr<Node> scope_)
{
    iterator = move(iterator_);
    iterated = move(iterated_);
    scope = move(scope_);
}

string ForeachStatement::toString(int depth) const
{
    return string(depth, '|') + "Foreach Statement\n" + 
            iterator->toString(depth + 1) + 
            iterated->toString(depth + 1) + 
            scope->toString(depth + 1);
}

FunctionCall::FunctionCall(unique_ptr<Node> func_, 
                           vector<unique_ptr<Node>> args_)
{
    func = move(func_);
    args = move(args_);
}

string FunctionCall::toString(int depth) const
{
    auto ret_str =  string(depth, '|') + "Function Call\n" + 
                    func->toString(depth+1);
    for(auto const& arg: args)
    {
        ret_str += arg->toString(depth + 1);
    }
    return ret_str;
           
}

FunctionDefinition::FunctionDefinition(unique_ptr<Node> fun_,
                         vector<unique_ptr<Node>> params_,
                         unique_ptr<Node> scope_)
{
    fun = move(fun_);
    params = move(params_);
    scope = move(scope_);
}

string FunctionDefinition::toString(int depth) const
{
    string ret_str = string(depth, '|') + "Function Defenition\n" + 
                          fun->toString(depth + 1);
    for(auto const& param: params)
        ret_str += param->toString(depth + 1);
    ret_str += scope->toString(depth + 1);
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

HexgridExpression::HexgridExpression(unique_ptr<Node> lvalue_,
                                       Operator op_,
                                       unique_ptr<Node> rvalue_)
    : op(op_)
{
    lvalue = move(lvalue_);
    rvalue = move(rvalue_);
}

string HexgridExpression::toString(int depth) const
{
    switch (op)
    {
    case Operator::On:
        return string(depth, '|') + "Hexgrid On Expression\n" +
               lvalue->toString(depth + 1) +
               rvalue->toString(depth + 1);
    case Operator::By:
        return string(depth, '|') + "Hexgrid By Expression\n" +
               lvalue->toString(depth + 1) +
               rvalue->toString(depth + 1);
    case Operator::Beside:
        return string(depth, '|') + "Hexgrid Beside Expression\n" +
               lvalue->toString(depth + 1) +
               rvalue->toString(depth + 1);
    default:
        throw runtime_error("Hexgrid Expression without operator");
        return "";
    };
}

Identifier::Identifier(string value_)
    : value(value_)
{
}
string Identifier::getValue() {
    return value;
}
string Identifier::toString(int depth) const
{
    return string(depth, '|') + "Identifier (" + value + ")\n";
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


OrExpression::OrExpression(unique_ptr<Node> lvalue_,
                             unique_ptr<Node> rvalue_)
{
    lvalue = move(lvalue_);
    rvalue = move(rvalue_);
}

string OrExpression::toString(int depth) const
{
    return string(depth, '|') + "Or Expression\n" +
            lvalue->toString(depth + 1) +
            rvalue->toString(depth + 1);
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

Scope::Scope(vector<unique_ptr<Node>> stmnts_)
{
    stmnts = move(stmnts_);
}

string Scope::toString(int depth) const
{
    string ret_str = string(depth, '|') + "Scope\n";
    for(auto const& stmnt: stmnts)
    {
        ret_str += stmnt->toString(depth + 1);
    }
    return ret_str;
}

SOArithmExpression::SOArithmExpression(unique_ptr<Node> lvalue_,
                                       Operator op_,
                                       unique_ptr<Node> rvalue_)
    : op(op_)
{
    lvalue = move(lvalue_);
    rvalue = move(rvalue_);
}

string SOArithmExpression::toString(int depth) const
{
    switch (op)
    {
    case Operator::Add:
        return string(depth, '|') + "Add Expression\n" +
               lvalue->toString(depth + 1) +
               rvalue->toString(depth + 1);
    case Operator::Substruct:
        return string(depth, '|') + "Substruct Expression\n" +
               lvalue->toString(depth + 1) +
               rvalue->toString(depth + 1);
    default:
        throw runtime_error("Arithmetical Expression without operator");
        return "";
    };
}

TextLiteral::TextLiteral(string value_): value(value_){}
string TextLiteral::toString(int depth) const
{
    return string(depth, '|') + "Text Literal (" + value + ")\n";
}

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

Variable* Scope::calculate(Scope& sc) {
    upper_scope = &sc;
    for(auto const& stmnt : stmnts){
        stmnt->calculate(*this);
    }
    return nullptr;
}

void Scope::declareVariable(string name, ast::Variable var){
    variables[name] = var;
}

void Scope::assignVariable(string name, ast::Variable var){
    Variable var;
    if (variable.cout(name)){
        var = variables[name];
        
    }
    if (!variables.count(name))
    {
        if (!upper_scope){
            throw runtime_error("Variable " + name + " assigned before declared\n");
        }
        upper_scope->assignVariable(name, var);
    }
    if 
}

Variable* DeclarationStatement::calculate(Scope& scope) {
    string name = iden->getValue();
    Variable::Type t; 
    switch (type){
        case Type::Int: t = Variable::Type::Int;
                        break;
        default:
            throw runtime_error("Unkowwn type");
    }
    Variable v = Variable(t);
    scope.declareVariable(name, v); 
    return nullptr;
}

string DeclarationStatement::getIdentifier(){
    return iden->getValue();
}

Variable* AssignmentStatement::calculate(Scope& scope) {
    string name;
    if(decl){
        decl->calculate(scope);
        name = decl->getIdentifier();
    } else {
        name = iden->getValue();
    }
    Variable* v = value.calculate(scope);
    return nullptr;
}



Variable* AddStatement::calculate(Scope& scope) {return nullptr;}
Variable* ConditionBlock::calculate(Scope& scope) {return nullptr;}
Variable* ForeachStatement::calculate(Scope& scope) {return nullptr;}
Variable* FunctionCall::calculate(Scope& scope) {return nullptr;}
Variable* FunctionDefinition::calculate(Scope& scope) {return nullptr;}
Variable* IfStatement::calculate(Scope& scope) {return nullptr;}
Variable* MoveStatement::calculate(Scope& scope) {return nullptr;}
Variable* RemoveStatement::calculate(Scope& scope) {return nullptr;}
Variable* ReturnStatement::calculate(Scope& scope) {return nullptr;}
Variable* OrExpression::calculate(Scope& scope) {return nullptr;}
Variable* AndExpression::calculate(Scope& scope) {return nullptr;}
Variable* ComparisonExpression::calculate(Scope& scope) {return nullptr;}
Variable* HexgridExpression::calculate(Scope& scope) {return nullptr;}
Variable* SOArithmExpression::calculate(Scope& scope) {return nullptr;}
Variable* FOArithmExpression::calculate(Scope& scope) {return nullptr;}
Variable* LogicalNegation::calculate(Scope& scope) {return nullptr;}
Variable* ArithmeticalNegation::calculate(Scope& scope) {return nullptr;}
Variable* IndexingExpression::calculate(Scope& scope) {return nullptr;}
Variable* Literal::calculate(Scope& scope) {return nullptr;}
Variable* TextLiteral::calculate(Scope& scope) {return nullptr;}
Variable* IntegerLiteral::calculate(Scope& scope) {return nullptr;}
Variable* DecimalLiteral::calculate(Scope& scope) {return nullptr;}
Variable* Hexgrid::calculate(Scope& scope) {return nullptr;}
Variable* HexgridCell::calculate(Scope& scope) {return nullptr;}
Variable* Identifier::calculate(Scope& scope) {return nullptr;}
Variable* Array::calculate(Scope& scope) {return nullptr;}

