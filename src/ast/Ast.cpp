#include "Ast.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
using namespace ast;
using namespace std;

AddStatement::AddStatement(
                std::unique_ptr<Node> being_added_,
                std::unique_ptr<Node> added_to_,
                std::unique_ptr<Node> added_at_)
{
    being_added = std::move(being_added_);
    added_to = std::move(added_to_);
    added_at = std::move(added_at_);
}

std::string AddStatement::toString(int depth) const
{
    return prefix(depth) + "Add Statement\n" + 
            being_added->toString(depth + 1) + 
            added_to->toString(depth + 1) + 
            added_at->toString(depth + 1);
}

AndExpression::AndExpression(std::unique_ptr<Node> lvalue_,
                             std::unique_ptr<Node> rvalue_)
{
    lvalue = std::move(lvalue_);
    rvalue = std::move(rvalue_);
}

std::string AndExpression::toString(int depth) const
{
    return prefix(depth) + "And Expression\n" +
            lvalue->toString(depth + 1) +
            rvalue->toString(depth + 1);
}

ArithmeticalNegation::ArithmeticalNegation(std::unique_ptr<Node>  value_)
{
    value = std::move(value_);
}

std::string ArithmeticalNegation::toString(int depth) const
{
    return prefix(depth) + "Arithmetical Negation Expression\n" + value->toString(depth + 1);
}

Array::Array(std::unique_ptr<std::vector<std::unique_ptr<Node>>> elements_)
{
    elements = std::move(elements_);
}

std::string Array::toString(int depth) const
{
    std::string ret_str = prefix(depth) + "Array of length (" + 
                          std::to_string(elements->size()) + ")\n";
    for(auto const& element: *elements)
    {
        ret_str += element->toString(depth + 1);
    }
    return ret_str;
}

AssignmentStatement::AssignmentStatement(std::unique_ptr<Node> target_, 
                                         std::unique_ptr<Node> value_)
{
    target = std::move(target_);
    value = std::move(value_);
}

std::string AssignmentStatement::toString(int depth) const
{
    return prefix(depth) + "Assignment\n" + 
           target->toString(depth+1) + 
           value->toString(depth+1);
           
}

ComparisonExpression::ComparisonExpression(std::unique_ptr<Node> lvalue_,
                                       Operator op_,
                                       std::unique_ptr<Node> rvalue_)
    : op(op_)
{
    lvalue = std::move(lvalue_);
    rvalue = std::move(rvalue_);
}

std::string ComparisonExpression::toString(int depth) const
{
    return prefix(depth) + toString(op) +
            lvalue->toString(depth + 1) +
            rvalue->toString(depth + 1);
}

std::string ComparisonExpression::toString(Operator op_) const
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
        throw std::runtime_error("Unkown comparison operator");
        return "";
    }
}

ConditionBlock::ConditionBlock(std::unique_ptr<Node> condition_,
                 std::unique_ptr<Node> scope_)
{
    condition = std::move(condition_);
    scope = std::move(scope_);
}

std::string ConditionBlock::toString(int depth) const
{
    return prefix(depth) + "Condition Block\n" + 
            condition->toString(depth + 1) + 
            scope->toString(depth + 1);
}

DecimalLiteral::DecimalLiteral(double value_)
    : value(value_)
{
    
}

std::string DecimalLiteral::toString(int depth) const
{
    return prefix(depth) + "Decimal Literal (" + std::to_string(value) + ")\n";
}

double DecimalLiteral::getValue() const {
    return value;
}

DeclarationStatement::DeclarationStatement(Type type_, 
                                            std::unique_ptr<Node> var_,
                                            int dimenstion_)
{
    type = type_;
    var = std::move(var_);
    dimenstion=dimenstion_;
}

std::string DeclarationStatement::toString(int depth) const
{
    return prefix(depth) + "Declaration\n" + 
           prefix(depth + 1) + "Variable Type (" + getType() + ")\n" +
           var->toString(depth+1);
}

std::string DeclarationStatement::getType() const
{
    if (dimenstion > 0)
        return std::to_string(dimenstion) + "d array of type " + toString(type);
    else
        return toString(type);
}

std::string DeclarationStatement::toString(Type type_) const
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
        throw std::runtime_error("Unkown comparison operator");
        return "";
    }
}

FOArithmExpression::FOArithmExpression(std::unique_ptr<Node> lvalue_,
                                       Operator op_,
                                       std::unique_ptr<Node> rvalue_)
    : op(op_)
{
    lvalue = std::move(lvalue_);
    rvalue = std::move(rvalue_);
}

std::string FOArithmExpression::toString(int depth) const
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
        throw std::runtime_error("Arithmetical Expression without operator");
        return "";
    };
}

ForeachStatement::ForeachStatement(
                std::unique_ptr<Node> iterator_,
                std::unique_ptr<Node> iterated_,
                std::unique_ptr<Node> scope_)
{
    iterator = std::move(iterator_);
    iterated = std::move(iterated_);
    scope = std::move(scope_);
}

std::string ForeachStatement::toString(int depth) const
{
    return prefix(depth) + "Foreach Statement\n" + 
            iterator->toString(depth + 1) + 
            iterated->toString(depth + 1) + 
            scope->toString(depth + 1);
}

FunctionCall::FunctionCall(std::unique_ptr<Node> func_, 
                           std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> args_)
{
    func = std::move(func_);
    args = std::move(args_);
}

std::string FunctionCall::toString(int depth) const
{
    auto ret_str =  prefix(depth) + "Function Call\n" + 
                    func->toString(depth+1);
    for(auto const& arg: *args)
    {
        ret_str += arg->toString(depth + 1);
    }
    return ret_str;
           
}

FunctionDefinition::FunctionDefinition(std::unique_ptr<Node> fun_,
                         std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> params_,
                         std::unique_ptr<Node> scope_)
{
    fun = std::move(fun_);
    params = std::move(params_);
    scope = std::move(scope_);
}

std::string FunctionDefinition::toString(int depth) const
{
    std::string ret_str = prefix(depth) + "Function Defenition\n" + 
                          fun->toString(depth + 1);
    for(auto const& param: *params)
        ret_str += param->toString(depth + 1);
    ret_str += scope->toString(depth + 1);
    return ret_str;
}

Hexgrid::Hexgrid(std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> cells_)
{
    cells = std::move(cells_);
}

std::string Hexgrid::toString(int depth) const
{
    std::string ret_str = prefix(depth) + "Hexgrid\n";
    for(auto const& cell: *cells)
    {
        ret_str += cell->toString(depth + 1);
    }
    return ret_str;
}

HexgridCell::HexgridCell(std::unique_ptr<Node> value_, std::unique_ptr<Node> pos_)
{
    pos = std::move(pos_);
    value = std::move(value_);
}

std::string HexgridCell::toString(int depth) const
{
    return prefix(depth) + "Hexgrid Cell\n" +
           value->toString(depth + 1) + 
           pos->toString(depth + 1);
}

HexgridExpression::HexgridExpression(std::unique_ptr<Node> lvalue_,
                                       Operator op_,
                                       std::unique_ptr<Node> rvalue_)
    : op(op_)
{
    lvalue = std::move(lvalue_);
    rvalue = std::move(rvalue_);
}

std::string HexgridExpression::toString(int depth) const
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
        throw std::runtime_error("Hexgrid Expression without operator");
        return "";
    };
}

Identifier::Identifier(std::string value_)
    : value(value_)
{
}

std::string Identifier::toString(int depth) const
{
    return prefix(depth) + "Identifier (" + value + ")\n";
}

IfStatement::IfStatement(std::unique_ptr<Node> if_block_,
                         std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> elif_blocks_,
                         std::unique_ptr<Node> else_block_)
{
    if_block = std::move(if_block_);
    else_block = std::move(else_block_);
    elif_blocks = std::move(elif_blocks_);
}

std::string IfStatement::toString(int depth) const
{
    std::string ret_str = prefix(depth) + "If Statement\n" + 
                          if_block->toString(depth + 1);
    if (elif_blocks)
        for(auto const& elif_block: *elif_blocks)
            ret_str += elif_block->toString(depth + 1);
    if (else_block)
        ret_str += else_block->toString(depth + 1);
    return ret_str;
}

IndexingExpression::IndexingExpression(std::unique_ptr<Node> indexOn_,
                                       std::unique_ptr<Node> indexBy_)
{
    indexOn = std::move(indexOn_);
    indexBy = std::move(indexBy_);
}

std::string IndexingExpression::toString(int depth) const
{
    return prefix(depth) + "Indexing Expression\n" + 
           indexOn->toString(depth + 1) + 
           indexBy->toString(depth + 1);
}

IntegerLiteral::IntegerLiteral(int value_)
    : value(value_)
{
    
}

std::string IntegerLiteral::toString(int depth) const
{
    return prefix(depth) + "Integer Literal (" + std::to_string(value) + ")\n";
}

Literal::Literal()
{
}

std::string Literal::toString(int depth) const
{
    return prefix(depth) + "Blank literal\n";
}

LogicalNegation::LogicalNegation(std::unique_ptr<Node>  value_)
{
    value = std::move(value_);
}

std::string LogicalNegation::toString(int depth) const
{
    return prefix(depth) + "Logical Negation Expression\n" + value->toString(depth + 1);
}

MoveStatement::MoveStatement(std::unique_ptr<Node> position_source_,
                             std::unique_ptr<Node> grid_source_,
                             std::unique_ptr<Node> grid_target_,
                             std::unique_ptr<Node> position_target_)
{
    position_source = std::move(position_source_);
    position_target = std::move(position_target_);
    grid_source = std::move(grid_source_);
    grid_target = std::move(grid_target_);
}

std::string MoveStatement::toString(int depth) const
{
    return prefix(depth) + "Move Statement\n" +
            position_source->toString(depth + 1) + 
            grid_source->toString(depth + 1) + 
            grid_target->toString(depth + 1) + 
            position_target->toString(depth + 1);
}

Node::~Node()
{}


OrExpression::OrExpression(std::unique_ptr<Node> lvalue_,
                             std::unique_ptr<Node> rvalue_)
{
    lvalue = std::move(lvalue_);
    rvalue = std::move(rvalue_);
}

std::string OrExpression::toString(int depth) const
{
    return prefix(depth) + "Or Expression\n" +
            lvalue->toString(depth + 1) +
            rvalue->toString(depth + 1);
}

RemoveStatement::RemoveStatement(std::unique_ptr<Node> position_,
                                 std::unique_ptr<Node> grid_)
{
    position = std::move(position_);
    grid = std::move(grid_);
}

std::string RemoveStatement::toString(int depth) const
{
    return prefix(depth) + "Remove Statement\n" +
            position->toString(depth + 1) + 
            grid->toString(depth + 1);
}

ReturnStatement::ReturnStatement(std::unique_ptr<Node> expr_)
{
    expr = std::move(expr_);
}

std::string ReturnStatement::toString(int depth) const
{
    return prefix(depth) + "Return Statement\n" + 
            expr->toString(depth + 1);
}

Scope::Scope(std::unique_ptr<std::vector<std::unique_ptr<Node>>> stmnts_)
{
    stmnts = std::move(stmnts_);
}

std::string Scope::toString(int depth) const
{
    std::string ret_str = prefix(depth) + "Scope\n";
    for(auto const& stmnt: *stmnts)
    {
        ret_str += stmnt->toString(depth + 1);
    }
    return ret_str;
}

SOArithmExpression::SOArithmExpression(std::unique_ptr<Node> lvalue_,
                                       Operator op_,
                                       std::unique_ptr<Node> rvalue_)
    : op(op_)
{
    lvalue = std::move(lvalue_);
    rvalue = std::move(rvalue_);
}

std::string SOArithmExpression::toString(int depth) const
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
        throw std::runtime_error("Arithmetical Expression without operator");
        return "";
    };
}

TextLiteral::TextLiteral(std::string value_): value(value_){}
std::string TextLiteral::toString(int depth) const
{
    return prefix(depth) + "Text Literal (" + value + ")\n";
}
void AssignmentStatement::accept(AstVisitor &visitor) const {};
void AddStatement::accept(AstVisitor &visitor) const {};
void ConditionBlock::accept(AstVisitor &visitor) const {};
void DeclarationStatement::accept(AstVisitor &visitor) const {};
void ForeachStatement::accept(AstVisitor &visitor) const {};
void FunctionCall::accept(AstVisitor &visitor) const {};
void FunctionDefinition::accept(AstVisitor &visitor) const {};
void IfStatement::accept(AstVisitor &visitor) const {};
void MoveStatement::accept(AstVisitor &visitor) const {};
void RemoveStatement::accept(AstVisitor &visitor) const {};
void ReturnStatement::accept(AstVisitor &visitor) const {};
void Scope::accept(AstVisitor &visitor) const {};
void OrExpression::accept(AstVisitor &visitor) const {};
void AndExpression::accept(AstVisitor &visitor) const {};
void ComparisonExpression::accept(AstVisitor &visitor) const {};
void HexgridExpression::accept(AstVisitor &visitor) const {};
void SOArithmExpression::accept(AstVisitor &visitor) const {};
void FOArithmExpression::accept(AstVisitor &visitor) const {};
void LogicalNegation::accept(AstVisitor &visitor) const {};
void ArithmeticalNegation::accept(AstVisitor &visitor) const {};
void IndexingExpression::accept(AstVisitor &visitor) const {};
void Literal::accept(AstVisitor &visitor) const {};
void TextLiteral::accept(AstVisitor &visitor) const {};
void IntegerLiteral::accept(AstVisitor &visitor) const {};
void DecimalLiteral::accept(AstVisitor &visitor) const {};
void Hexgrid::accept(AstVisitor &visitor) const {};
void HexgridCell::accept(AstVisitor &visitor) const {};
void Identifier::accept(AstVisitor &visitor) const {};
void Array::accept(AstVisitor &visitor) const {};


CodeGenVisitor::CodeGenVisitor(llvm::LLVMContext &context)
:TheContext(context){
    TheModule = make_unique<llvm::Module>("hexgrider", TheContext);
    Builder = std::make_unique<llvm::IRBuilder<>>(TheContext);
}

llvm::Value* CodeGenVisitor::LogErrorV(const char *Str) {
    throw runtime_error(Str);
    return nullptr;
}

llvm::Value* CodeGenVisitor::getValue(const DecimalLiteral &dl){
    return llvm::ConstantFP::get(TheContext, llvm::APFloat(dl.getValue()));
}