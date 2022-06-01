#ifndef TKOM_AST_H
#define TKOM_AST_H

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
#include <string>
#include <memory>
#include <map>
#include <vector>
namespace ast
{

class Node
{
public:
  virtual ~Node();
  virtual std::string toString(int depth = 0) const = 0;
  virtual std::string prefix(int depth) const
  {
    return std::string(depth, '|');
  }
  virtual void accept(class AstVisitor &visitor) const = 0;
};


class AssignmentStatement : public Node
{
public:
    AssignmentStatement(std::unique_ptr<Node> target_, 
                        std::unique_ptr<Node> value_);
    ~AssignmentStatement(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;

    std::unique_ptr<Node> target;
    std::unique_ptr<Node> value;
};


class AddStatement : public Node
{
public:
    AddStatement( 
                std::unique_ptr<Node> being_added_,
                std::unique_ptr<Node> added_to_,
                std::unique_ptr<Node> added_at_);
    ~AddStatement(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;
    std::unique_ptr<ast::Node> being_added;
    std::unique_ptr<ast::Node> added_to;
    std::unique_ptr<ast::Node> added_at;
};


class ConditionBlock : public Node
{
public:
    ConditionBlock(std::unique_ptr<Node> condition_,
            std::unique_ptr<Node> scope_);
    ~ConditionBlock(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;
    std::unique_ptr<ast::Node> condition;
    std::unique_ptr<ast::Node> scope;
};


class DeclarationStatement : public Node
{
public:
    enum class Type
    {
        Int,
        Float,
        String,
        Hexgrid
    };
    DeclarationStatement(Type type_, std::unique_ptr<Node> var_, int dimenstion_=0);
    ~DeclarationStatement(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;
    std::string getType() const;
    std::string toString(Type type_) const;

    Type type;
    std::unique_ptr<Node> var;
    int dimenstion;
};


class ForeachStatement : public Node
{
public:
    ForeachStatement( 
                std::unique_ptr<Node> iterator_,
                std::unique_ptr<Node> iterated_,
                std::unique_ptr<Node> scope_);
    ~ForeachStatement(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;
    std::unique_ptr<ast::Node> iterator;
    std::unique_ptr<ast::Node> iterated;
    std::unique_ptr<ast::Node> scope;
};


class FunctionCall : public Node
{
public:
    FunctionCall(std::unique_ptr<Node> func_, 
                 std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> args_);
    ~FunctionCall(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;

    std::unique_ptr<Node> func;
    std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> args;
};


class FunctionDefinition : public Node
{
public:
    FunctionDefinition( 
                std::unique_ptr<Node> fun_,
                std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> params_,
                std::unique_ptr<Node> scope_);
    ~FunctionDefinition(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;
    std::unique_ptr<ast::Node> fun;
    std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> params;
    std::unique_ptr<ast::Node> scope;
};

class IfStatement : public Node
{
public:
    IfStatement( 
                std::unique_ptr<Node> if_block_,
                std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> elif_blocks_,
                std::unique_ptr<Node> else_block_);
    ~IfStatement(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;
    std::unique_ptr<ast::Node> if_block;
    std::unique_ptr<ast::Node> else_block;
    std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> elif_blocks;
};


class MoveStatement : public Node
{
public:
    MoveStatement(std::unique_ptr<Node> position_source_,
                  std::unique_ptr<Node> grid_source_,
                  std::unique_ptr<Node> grid_target_,
                  std::unique_ptr<Node> position_target_);
    ~MoveStatement(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> position_source;
    std::unique_ptr<Node> position_target;
    std::unique_ptr<Node> grid_source;
    std::unique_ptr<Node> grid_target;
};


class RemoveStatement : public Node
{
public:
    RemoveStatement(std::unique_ptr<Node> position_,
                    std::unique_ptr<Node> grid_);
    ~RemoveStatement(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> position;
    std::unique_ptr<Node> grid;
};


class ReturnStatement : public Node
{
public:
    ReturnStatement(std::unique_ptr<Node> expr_);
    ~ReturnStatement(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;
    std::unique_ptr<ast::Node> expr;
};


class Scope : public Node
{
public:
    Scope(std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> stmnts_);
    ~Scope(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;
    std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> stmnts;
};


/*------------------------------*/
/*          Expressions         */
/*------------------------------*/


class OrExpression : public Node
{
public:
    OrExpression(std::unique_ptr<Node> lvalue_,
                 std::unique_ptr<Node> rvalue_);
    ~OrExpression(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> lvalue;
    std::unique_ptr<Node> rvalue;
};


class AndExpression : public Node
{
public:
    AndExpression(std::unique_ptr<Node> lvalue_,
                       std::unique_ptr<Node> rvalue_);
    ~AndExpression(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> lvalue;
    std::unique_ptr<Node> rvalue;
};


class ComparisonExpression : public Node
{
public:
    enum class Operator
    {
        Less,
        LessOrEqual,
        Greater,
        GreaterOrEqual,
        Equal,
        NotEqual
    };
    ComparisonExpression(std::unique_ptr<Node> lvalue_,
                       Operator op_,
                       std::unique_ptr<Node> rvalue_);
    ~ComparisonExpression(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;
    std::string toString(Operator op_)  const;
    std::unique_ptr<Node> lvalue;
    Operator op;
    std::unique_ptr<Node> rvalue;
};


class HexgridExpression : public Node
{
public:
    enum class Operator
    {
        On,
        By,
        Beside
    };
    HexgridExpression(std::unique_ptr<Node> lvalue_,
                       Operator op_,
                       std::unique_ptr<Node> rvalue_);
    ~HexgridExpression(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> lvalue;
    Operator op;
    std::unique_ptr<Node> rvalue;
};


class SOArithmExpression : public Node
{
public:
    enum class Operator
    {
        Add,
        Substruct
    };
    SOArithmExpression(std::unique_ptr<Node> lvalue_,
                       Operator op_,
                       std::unique_ptr<Node> rvalue_);
    ~SOArithmExpression(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> lvalue;
    Operator op;
    std::unique_ptr<Node> rvalue;
};


class FOArithmExpression : public Node
{
public:
    enum class Operator
    {
        Multiply,
        Divide
    };
    void accept(AstVisitor &visitor) const override;
    FOArithmExpression(std::unique_ptr<Node> lvalue_,
                       Operator op_,
                       std::unique_ptr<Node> rvalue_);
    ~FOArithmExpression(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> lvalue;
    Operator op;
    std::unique_ptr<Node> rvalue;
};


class LogicalNegation : public Node
{
public:
    LogicalNegation(std::unique_ptr<Node> value_);
    ~LogicalNegation(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> value;
};


class ArithmeticalNegation : public Node
{
public:
    ArithmeticalNegation(std::unique_ptr<Node> value_);
    ~ArithmeticalNegation(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> value;
};


class IndexingExpression : public Node
{
public:
    IndexingExpression(std::unique_ptr<ast::Node> indexOn_,
                       std::unique_ptr<ast::Node> indexBy_);
    ~IndexingExpression(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;

    std::unique_ptr<ast::Node> indexOn;
    std::unique_ptr<ast::Node> indexBy;
};




/*--------------------------*/
/*          Literals        */
/*--------------------------*/

class Literal : public Node
{
public:
    Literal();
    ~Literal(){};
    void accept(AstVisitor &visitor) const override;
    std::string toString(int depth = 0) const override;
};


class TextLiteral : public Node
{
public:
    TextLiteral(std::string value_);
    ~TextLiteral(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;
    std::string value;
};


class IntegerLiteral : public Literal
{
public:
    IntegerLiteral(int value_);
    ~IntegerLiteral(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;
    int value;
};


class DecimalLiteral : public Node
{
public:
    DecimalLiteral(double value_);
    ~DecimalLiteral(){};
    void accept(AstVisitor &visitor) const override;
    std::string toString(int depth = 0) const override;
    double getValue() const;
private:
    double value;
};


class Hexgrid : public Node
{
public:
    Hexgrid(std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> cells_);
    ~Hexgrid(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;
    std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> cells;
};


class HexgridCell : public Node
{
public:
    HexgridCell(std::unique_ptr<Node> value_,
                std::unique_ptr<Node> pos_);
    ~HexgridCell(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;
    std::unique_ptr<ast::Node> pos;
    std::unique_ptr<ast::Node> value;
};


class Identifier : public Node
{
public:
    Identifier(std::string value_);
    ~Identifier(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;
    std::string value;
};


class Array : public Node
{
public:
    Array(std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> elements_);
    ~Array(){};
    void accept(AstVisitor &visitor) const override;

    std::string toString(int depth = 0) const override;
    std::unique_ptr<std::vector<std::unique_ptr<ast::Node>>> elements;
};


class AstVisitor
{
public:
    virtual ~AstVisitor() = default;
    // virtual void operator()(const class AssignmentStatement &) = 0;
    // virtual void operator()(const class AddStatement &) = 0;
    // virtual void operator()(const class ConditionBlock &) = 0;
    // virtual void operator()(const class DeclarationStatement &) = 0;
    // virtual void operator()(const class ForeachStatement &) = 0;
    // virtual void operator()(const class FunctionCall &) = 0;
    // virtual void operator()(const class FunctionDefinition &) = 0;
    // virtual void operator()(const class IfStatement &) = 0;
    // virtual void operator()(const class MoveStatement &) = 0;
    // virtual void operator()(const class RemoveStatement &) = 0;
    // virtual void operator()(const class ReturnStatement &) = 0;
    // virtual void operator()(const class Scope &) = 0;
    // virtual void operator()(const class OrExpression &) = 0;
    // virtual void operator()(const class AndExpression &) = 0;
    // virtual void operator()(const class ComparisonExpression &) = 0;
    // virtual void operator()(const class HexgridExpression &) = 0;
    // virtual void operator()(const class SOArithmExpression &) = 0;
    // virtual void operator()(const class FOArithmExpression &) = 0;
    // virtual void operator()(const class LogicalNegation &) = 0;
    // virtual void operator()(const class ArithmeticalNegation &) = 0;
    // virtual void operator()(const class IndexingExpression &) = 0;
    // virtual void operator()(const class Literal &) = 0;
    // virtual void operator()(const class TextLiteral &) = 0;
    // virtual void operator()(const class IntegerLiteral &) = 0;
    
    virtual llvm::Value *getValue(const DecimalLiteral &dl) = 0;
    // virtual void operator()(const class DecimalLiteral &) = 0;

    // virtual void operator()(const class Hexgrid &) = 0;
    // virtual void operator()(const class HexgridCell &) = 0;
    // virtual void operator()(const class Identifier &) = 0;
    // virtual void operator()(const class Array &) = 0;
};

class CodeGenVisitor : public AstVisitor {
    public:
    CodeGenVisitor(llvm::LLVMContext &context);
    llvm::Value *getValue(const DecimalLiteral &dl) override;
    
    // llvm::Value *getVa

    llvm::Value *LogErrorV(const char *Str);    
    private:
    llvm::LLVMContext &TheContext;
    std::unique_ptr<llvm::Module> TheModule;
    std::unique_ptr<llvm::IRBuilder<>> Builder;
    std::map<std::string, llvm::Value *> NamedValues;
    llvm::Value *result{};
};




} // namespace ast

#endif // TKOM_AST_H
