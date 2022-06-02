#include "Ast.h"
using namespace ast;
using namespace std;
using namespace llvm;

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
    return prefix(depth) + "Add Statement\n" + 
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
    return prefix(depth) + "And Expression\n" +
            lvalue->toString(depth + 1) +
            rvalue->toString(depth + 1);
}

ArithmeticalNegation::ArithmeticalNegation(unique_ptr<Node>  value_)
{
    value = move(value_);
}

string ArithmeticalNegation::toString(int depth) const
{
    return prefix(depth) + "Arithmetical Negation Expression\n" + value->toString(depth + 1);
}

Array::Array(vector<unique_ptr<Node>> elements_)
{
    elements = move(elements_);
}

string Array::toString(int depth) const
{
    string ret_str = prefix(depth) + "Array of length (" + 
                          to_string(elements.size()) + ")\n";
    for(auto const& element: elements)
    {
        ret_str += element->toString(depth + 1);
    }
    return ret_str;
}

AssignmentStatement::AssignmentStatement(unique_ptr<Node> target_, 
                                         unique_ptr<Node> value_)
{
    target = move(target_);
    value = move(value_);
}

string AssignmentStatement::toString(int depth) const
{
    return prefix(depth) + "Assignment\n" + 
           target->toString(depth+1) + 
           value->toString(depth+1);
           
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
    return prefix(depth) + toString(op) +
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
    return prefix(depth) + "Condition Block\n" + 
            condition->toString(depth + 1) + 
            scope->toString(depth + 1);
}

DecimalLiteral::DecimalLiteral(double value_)
    : value(value_)
{
    
}

string DecimalLiteral::toString(int depth) const
{
    return prefix(depth) + "Decimal Literal (" + to_string(value) + ")\n";
}

double DecimalLiteral::getValue() const {
    return value;
}

DeclarationStatement::DeclarationStatement(Type type_, 
                                            unique_ptr<Node> var_,
                                            int dimenstion_)
{
    type = type_;
    var = move(var_);
    dimenstion=dimenstion_;
}

string DeclarationStatement::toString(int depth) const
{
    return prefix(depth) + "Declaration\n" + 
           prefix(depth + 1) + "Variable Type (" + getType() + ")\n" +
           var->toString(depth+1);
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
        return prefix(depth) + "Multiply Expression\n" +
               lvalue->toString(depth + 1) +
               rvalue->toString(depth + 1);
    case Operator::Divide:
        return prefix(depth) + "Divide Expression\n" +
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
    return prefix(depth) + "Foreach Statement\n" + 
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
    auto ret_str =  prefix(depth) + "Function Call\n" + 
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
    string ret_str = prefix(depth) + "Function Defenition\n" + 
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
    string ret_str = prefix(depth) + "Hexgrid\n";
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
    return prefix(depth) + "Hexgrid Cell\n" +
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
        return prefix(depth) + "Hexgrid On Expression\n" +
               lvalue->toString(depth + 1) +
               rvalue->toString(depth + 1);
    case Operator::By:
        return prefix(depth) + "Hexgrid By Expression\n" +
               lvalue->toString(depth + 1) +
               rvalue->toString(depth + 1);
    case Operator::Beside:
        return prefix(depth) + "Hexgrid Beside Expression\n" +
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
string Identifier::getValue() const {
    return value;
}
string Identifier::toString(int depth) const
{
    return prefix(depth) + "Identifier (" + value + ")\n";
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
    string ret_str = prefix(depth) + "If Statement\n" + 
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
    return prefix(depth) + "Indexing Expression\n" + 
           indexOn->toString(depth + 1) + 
           indexBy->toString(depth + 1);
}

IntegerLiteral::IntegerLiteral(int value_)
    : value(value_)
{
    
}

string IntegerLiteral::toString(int depth) const
{
    return prefix(depth) + "Integer Literal (" + to_string(value) + ")\n";
}

Literal::Literal()
{
}

string Literal::toString(int depth) const
{
    return prefix(depth) + "Blank literal\n";
}

LogicalNegation::LogicalNegation(unique_ptr<Node>  value_)
{
    value = move(value_);
}

string LogicalNegation::toString(int depth) const
{
    return prefix(depth) + "Logical Negation Expression\n" + value->toString(depth + 1);
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
    return prefix(depth) + "Move Statement\n" +
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
    return prefix(depth) + "Or Expression\n" +
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
    return prefix(depth) + "Remove Statement\n" +
            position->toString(depth + 1) + 
            grid->toString(depth + 1);
}

ReturnStatement::ReturnStatement(unique_ptr<Node> expr_)
{
    expr = move(expr_);
}

string ReturnStatement::toString(int depth) const
{
    return prefix(depth) + "Return Statement\n" + 
            expr->toString(depth + 1);
}

Scope::Scope(vector<unique_ptr<Node>> stmnts_)
{
    stmnts = move(stmnts_);
}

string Scope::toString(int depth) const
{
    string ret_str = prefix(depth) + "Scope\n";
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
        return prefix(depth) + "Add Expression\n" +
               lvalue->toString(depth + 1) +
               rvalue->toString(depth + 1);
    case Operator::Substruct:
        return prefix(depth) + "Substruct Expression\n" +
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
    return prefix(depth) + "Text Literal (" + value + ")\n";
}
llvm::Value * AssignmentStatement::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);};
llvm::Value * AddStatement::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * ConditionBlock::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value *DeclarationStatement::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * ForeachStatement::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * FunctionCall::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * FunctionDefinition::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * IfStatement::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * MoveStatement::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * RemoveStatement::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * ReturnStatement::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * Scope::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * OrExpression::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * AndExpression::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * ComparisonExpression::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * HexgridExpression::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * SOArithmExpression::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * FOArithmExpression::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * LogicalNegation::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * ArithmeticalNegation::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * IndexingExpression::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * Literal::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * TextLiteral::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * IntegerLiteral::accept(AstVisitor &visitor) const {
    return visitor.getIdentifier(*this);
};
llvm::Value * DecimalLiteral::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * Hexgrid::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * HexgridCell::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * Identifier::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};
llvm::Value * Array::accept(AstVisitor &visitor) const {
    return visitor.getValue(*this);
};


CodeGenVisitor::CodeGenVisitor(LLVMContext &context)
:TheContext(context){
    TheModule = make_unique<Module>("hexgrider", TheContext);
    Builder = make_unique<IRBuilder<>>(TheContext);
}

void CodeGenVisitor::print(){
    TheModule->print(errs(), nullptr);
}

Value* CodeGenVisitor::LogErrorV(const char *Str) {
    throw runtime_error(Str);
    return nullptr;
}

Value* CodeGenVisitor::getValue(const DecimalLiteral &dl){
    return ConstantFP::get(TheContext, APFloat(dl.getValue()));
}
string CodeGenVisitor::getIdentifier(const Identifier &id){
    return id.value;
}

Value *CodeGenVisitor::getValue(const RemoveStatement &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const MoveStatement &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const IfStatement &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const FunctionDefinition &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const FunctionCall &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const ForeachStatement &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const ConditionBlock &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const AddStatement &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const AssignmentStatement &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const ReturnStatement &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const Scope &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const OrExpression &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const AndExpression &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const ComparisonExpression &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const HexgridExpression &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const SOArithmExpression &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const FOArithmExpression &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const LogicalNegation &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const ArithmeticalNegation &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const IndexingExpression &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const Literal &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const TextLiteral &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const IntegerLiteral &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const Hexgrid &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const HexgridCell &dl){return nullptr;}
Value *CodeGenVisitor::getValue(const Array &dl){return nullptr;}

Function* CodeGenVisitor::operator()(const Scope &sc){
    FunctionType *FT = 
        FunctionType::get(Type::getVoidTy(TheContext), false);
    Function *F =
        Function::Create(FT, Function::ExternalLinkage, "Top level", TheModule.get());
    BasicBlock *BB = BasicBlock::Create(TheContext, "entry", F);
    Builder->SetInsertPoint(BB);
    NamedValues.clear();

    if (Value* RetVal = sc.stmnts[0]->accept(*this)){
        Builder->CreateRet(RetVal);
        verifyFunction(*F);
        return F;
    }
    F->eraseFromParent();
    return nullptr;;
}

Value *CodeGenVisitor::getValue(const DeclarationStatement &ds){
    string name = ds.var->accept(*this);
    NamedValues[name] = 
    return nullptr;
}
