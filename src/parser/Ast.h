#ifndef TKOM_AST_H
#define TKOM_AST_H

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <variant>
#include <iostream>
#include <HexgridErrors.h>

namespace ast
{

class Program;
class ReturnStatement;
class RemoveStatement;
class MoveStatement;
class IfStatement;
class ForeachStatement;
class ConditionBlock;
class AddStatement;
class InitializationStatement;
class AssignmentStatement;
class IndexingExpression;
class ArithmeticalNegation;
class LogicalNegation;
class ModuloExpression;
class DivideExpression;
class MultiplyExpression;
class SubtructExpression;
class AddExpression;
class OnExpression;
class ByExpression;
class BesideExpression;
class NotEqualExpression;
class EqualExpression;
class GreaterOrEqualExpression;
class GreaterExpression;
class LessOrEqualExpression;
class LessExpression;
class AndExpression;
class OrExpression;
class ArrayLiteral;
class HexgridCell;
class HexgridLiteral;
class DecimalLiteral;
class IntegerLiteral;
class TextLiteral;
class VariableReference;
class FunctionCall;
class StatementBlock;
class FunctionDefinition;
class VariableDeclarationStatement;
class Print;

class AstVisitor
{
public:
virtual ~AstVisitor() = default;
virtual void visit(ast::Program&) = 0;
virtual void visit(ast::VariableDeclarationStatement&) = 0;
// virtual void visit(ast::IntVarDeclaration&) = 0;
virtual void visit(ast::ReturnStatement&) = 0;
virtual void visit(ast::RemoveStatement&) = 0;
virtual void visit(ast::MoveStatement&) = 0;
virtual void visit(ast::IfStatement&) = 0;
virtual void visit(ast::ForeachStatement&) = 0;
virtual void visit(ast::ConditionBlock&) = 0;
virtual void visit(ast::AddStatement&) = 0;
virtual void visit(ast::InitializationStatement&) = 0;
virtual void visit(ast::AssignmentStatement&) = 0;
virtual void visit(ast::IndexingExpression&) = 0;
virtual void visit(ast::ArithmeticalNegation&) = 0;
virtual void visit(ast::LogicalNegation&) = 0;
virtual void visit(ast::ModuloExpression&) = 0;
virtual void visit(ast::DivideExpression&) = 0;
virtual void visit(ast::MultiplyExpression&) = 0;
virtual void visit(ast::SubtructExpression&) = 0;
virtual void visit(ast::AddExpression&) = 0;
virtual void visit(ast::OnExpression&) = 0;
virtual void visit(ast::ByExpression&) = 0;
virtual void visit(ast::BesideExpression&) = 0;
virtual void visit(ast::NotEqualExpression&) = 0;
virtual void visit(ast::EqualExpression&) = 0;
virtual void visit(ast::GreaterOrEqualExpression&) = 0;
virtual void visit(ast::GreaterExpression&) = 0;
virtual void visit(ast::LessOrEqualExpression&) = 0;
virtual void visit(ast::LessExpression&) = 0;
virtual void visit(ast::AndExpression&) = 0;
virtual void visit(ast::OrExpression&) = 0;
virtual void visit(ast::ArrayLiteral&) = 0;
virtual void visit(ast::HexgridCell&) = 0;
virtual void visit(ast::HexgridLiteral&) = 0;
virtual void visit(ast::DecimalLiteral&) = 0;
virtual void visit(ast::IntegerLiteral&) = 0;
virtual void visit(ast::TextLiteral&) = 0;
virtual void visit(ast::VariableReference&) = 0;
virtual void visit(ast::FunctionCall&) = 0;
virtual void visit(ast::StatementBlock&) = 0;
virtual void visit(ast::FunctionDefinition&) = 0;
};


class Node
{
public:
    virtual ~Node();
    virtual std::string toString(int depth = 0) const = 0;
    // virtual void accept(AstVisitor& v) {throw std::runtime_error("accept not implemented");}
    virtual void accept(AstVisitor&) = 0;
};

class Variable
{
public:
    enum class Type
    {
        Int,
        Float,
        String,
        Hexgrid,
        Array
    };
    Variable();
    Variable(Type);
    static std::string typeToString(Type);
    std::string typeToString();
private:
    Type type;
};


/* class IntVarDeclaration : public Node{
public: 
    IntVarDeclaration(std::string);
    ~IntVarDeclaration(){};
    std::string toString(int=0) const override;
private:
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
    std::string name;
}; */

class VariableDeclarationStatement : public Node
{
public:
    VariableDeclarationStatement(Variable::Type type_, std::string id_);
    ~VariableDeclarationStatement(){};

    std::string toString(int depth = 0) const override;
    std::string getType() const;
    std::string toString(Variable::Type type_) const;
    // std::string getIdentifier();

    virtual void accept(AstVisitor& v) override {v.visit(*this);}

    Variable::Type type;
    std::string identifier;
};

class FunctionDefinition : public Node
{
public:
    FunctionDefinition( 
                Variable::Type,
                std::string,
                std::vector<std::unique_ptr<VariableDeclarationStatement>>,
                std::unique_ptr<Node>,
                std::pair<int, int>, std::pair<int, int>);
    ~FunctionDefinition(){};

    std::string toString(int depth = 0) const override;
    std::string getName() const;
    std::pair<int, int> getStart()const ;
    std::pair<int, int> getEnd()const;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
    size_t getParamCount() const;
    void declareParam(int, AstVisitor&);
    void runStatementBlock(AstVisitor&);
private:
    Variable::Type type;
    std::string name;
    std::vector<std::unique_ptr<VariableDeclarationStatement>> params;
    std::unique_ptr<Node> statementBlock;
    std::pair<int, int> startLoc;
    std::pair<int, int> endLoc;
};

class Program : public Node
{
public:
    Program();
    ~Program(){};
    void insertStatement(std::unique_ptr<Node>);    
    void insertFunction(std::unique_ptr<FunctionDefinition>);    
    std::string toString(int depth = 0) const override;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
    
    std::vector<std::unique_ptr<Node>> stmnts;
    std::map<std::string, std::unique_ptr<FunctionDefinition>> funcs;
    // std::vector<std::unique_ptr<FunctionDefinition>> funcs;
};

class StatementBlock : public Node
{
public:
    StatementBlock(std::vector<std::unique_ptr<Node>>);
    ~StatementBlock(){};

    std::string toString(int depth = 0) const override;
    std::vector<std::unique_ptr<Node>> stmnts;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};

class FunctionCall : public Node
{
public:
    FunctionCall(std::string, std::vector<std::unique_ptr<Node>>);
    ~FunctionCall(){};

    std::string toString(int depth = 0) const override;

    std::string funcName;
    std::vector<std::unique_ptr<Node>> args;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};


class VariableReference : public Node
{
public:
    VariableReference(std::string);
    ~VariableReference(){};

    std::string toString(int depth = 0) const override;
    std::string getName() const;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
private:
    std::string name;
};


class TextLiteral : public Node
{
public:
    TextLiteral(std::string value_);
    ~TextLiteral(){};

    std::string toString(int depth = 0) const override;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
    std::string getValue() const;
private:
    std::string value;
};


class IntegerLiteral : public Node
{
public:
    IntegerLiteral(int value_);
    ~IntegerLiteral(){};

    std::string toString(int depth = 0) const override;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
    int getValue() const ;

private:
    int value;
};


class DecimalLiteral : public Node
{
public:
    DecimalLiteral(double value_);
    ~DecimalLiteral(){};
    std::string toString(int depth = 0) const override;
    double getValue() const;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
private:
    double value;
};


class HexgridLiteral : public Node
{
public:
    HexgridLiteral(std::vector<std::unique_ptr<Node>> cells_);
    ~HexgridLiteral(){};

    std::string toString(int depth = 0) const override;
    std::vector<std::unique_ptr<Node>> cells;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};


class HexgridCell : public Node
{
public:
    HexgridCell(std::unique_ptr<Node> value_,
                std::unique_ptr<Node> pos_);
    ~HexgridCell(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> pos;
    std::unique_ptr<Node> value;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};


class ArrayLiteral : public Node
{
public:
    ArrayLiteral(std::vector<std::unique_ptr<Node>> elements_);
    ~ArrayLiteral(){};

    std::string toString(int depth = 0) const override;
    std::vector<std::unique_ptr<Node>> elements;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};

/*------------------------------*/
/*          Expressions         */
/*------------------------------*/

class BinaryExpression : public Node
{
    public:
     BinaryExpression(std::unique_ptr<Node> lvalue_,
                      std::unique_ptr<Node> rvalue_);
    ~BinaryExpression(){};
    virtual std::string toString(int depth = 0) const;
    std::unique_ptr<Node> lvalue;
    std::unique_ptr<Node> rvalue;
};

class OrExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};


class AndExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};

class LessExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};

class LessOrEqualExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};

class GreaterExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};

class GreaterOrEqualExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};

class EqualExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};

class NotEqualExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};

class BesideExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};

class ByExpression : public BinaryExpression
{
public:

    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};

class OnExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};

class AddExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};

class SubtructExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};

class MultiplyExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};

class DivideExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};

class ModuloExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};

class LogicalNegation : public Node
{
public:
    LogicalNegation(std::unique_ptr<Node> value_);
    ~LogicalNegation(){};

    std::string toString(int depth = 0) const override;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
    std::unique_ptr<Node> value;
};


class ArithmeticalNegation : public Node
{
public:
    ArithmeticalNegation(std::unique_ptr<Node> value_);
    ~ArithmeticalNegation(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> value;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};


class IndexingExpression : public Node
{
public:
    IndexingExpression(std::unique_ptr<Node> indexOn_,
                       std::unique_ptr<Node> indexBy_);
    ~IndexingExpression(){};

    std::string toString(int depth = 0) const override;

    std::unique_ptr<Node> indexOn;
    std::unique_ptr<Node> indexBy;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};

/*---------------------------------------*/
/*              Statements               */
/*---------------------------------------*/


class AssignmentStatement : public Node
{
public:
    AssignmentStatement(std::string name_, 
                        std::unique_ptr<Node> value_);
    ~AssignmentStatement(){};

    std::string toString(int depth = 0) const override;

    std::string name;
    std::unique_ptr<Node> value;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};


class InitializationStatement : public Node
{
public:
    InitializationStatement(Variable::Type, std::string, std::unique_ptr<Node>);
    ~InitializationStatement(){};

    std::string toString(int depth = 0) const override;
    std::string getType() const;

    Variable::Type type;
    std::string name;
    std::unique_ptr<Node> value;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};

class AddStatement : public Node
{
public:
    AddStatement( 
                std::unique_ptr<Node> being_added_,
                std::unique_ptr<VariableReference> added_to_,
                std::unique_ptr<Node> added_at_);
    ~AddStatement(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> being_added;
    std::unique_ptr<VariableReference> added_to;
    std::unique_ptr<Node> added_at;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};


class ConditionBlock : public Node
{
public:
    ConditionBlock(std::unique_ptr<Node>,std::unique_ptr<Node>);
    ~ConditionBlock(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> condition;
    std::unique_ptr<Node> statementBlock;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};


class ForeachStatement : public Node
{
public:
    ForeachStatement( 
                std::unique_ptr<Node> iterator_,
                std::unique_ptr<Node> iterated_,
                std::unique_ptr<Node> statementBlock_);
    ~ForeachStatement(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> iterator;
    std::unique_ptr<Node> iterated;
    std::unique_ptr<Node> statementBlock;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};

class IfStatement : public Node
{
public:
    IfStatement( 
                std::unique_ptr<Node> ifBlock_,
                std::vector<std::unique_ptr<Node>> elifBlocks_,
                std::unique_ptr<Node> elseBlock_);
    ~IfStatement(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> ifBlock;
    std::unique_ptr<Node> elseBlock;
    std::vector<std::unique_ptr<Node>> elifBlocks;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};


class MoveStatement : public Node
{
public:
    MoveStatement(std::unique_ptr<Node> position_source_,
                  std::unique_ptr<VariableReference> grid_source_,
                  std::unique_ptr<VariableReference> grid_target_,
                  std::unique_ptr<Node> position_target_);
    ~MoveStatement(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> position_source;
    std::unique_ptr<Node> position_target;
    std::unique_ptr<VariableReference> grid_source;
    std::unique_ptr<VariableReference> grid_target;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};


class RemoveStatement : public Node
{
public:
    RemoveStatement(std::unique_ptr<Node> position_,
                    std::unique_ptr<VariableReference> grid_);
    ~RemoveStatement(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> position;
    std::unique_ptr<VariableReference> grid;
    virtual void accept(AstVisitor& v) override {v.visit(*this);}
};


class ReturnStatement : public Node
{
public:
    ReturnStatement(std::unique_ptr<Node> expr_);
    ~ReturnStatement(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> expr;
    void accept(AstVisitor& v) override {v.visit(*this);}
};
} // namespace ast

#endif // TKOM_AST_H
