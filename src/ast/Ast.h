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

class Node;

class AstVisitor
{
public:
    virtual void visit(Node&) {throw std::runtime_error("visit is not implemented");}
};


class Node
{
public:
    virtual ~Node();
    virtual std::string toString(int depth = 0) const = 0;
    virtual void accept(AstVisitor& v) {throw std::runtime_error("accept not implemented");}
    // virtual Variable* calculate(StatementBlock&) = 0;
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
    Variable(Type, int);
    static std::string typeToString(Type);
    std::string typeToString();
    int getIntValue(std::string);
    // double getDoubleValue(std::string);
    // std::string getStringValue(std::string);
private:
    Type type;
    std::variant<int, double, std::string> value;

};


class VariableDeclarationStatement : public Node
{
public:
    VariableDeclarationStatement(Variable::Type type_, std::string id_, int dimenstion_=0);
    ~VariableDeclarationStatement(){};

    std::string toString(int depth = 0) const override;
    std::string getType() const;
    std::string toString(Variable::Type type_) const;
    // std::string getIdentifier();

    // Variable* calculate(StatementBlock&) override;

    Variable::Type type;
    std::string identifier;
    int dimension;
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
    
    Variable::Type type;
    std::string name;
    std::vector<std::unique_ptr<VariableDeclarationStatement>> params;
    std::unique_ptr<Node> statementBlock;
    std::pair<int, int> startLoc;
    std::pair<int, int> endLoc;
    // Variable* calculate(StatementBlock&) override;
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
    // std::vector<std::unique_ptr<FunctionDefinition> funcDefs;
    // std::vector<std::unique_ptr<Vara> funcDefs;
    // std::vector<std::unique_ptr<Node>> stmnts;
    // std::map<std::string, Variable> variables;
    // std::map<std::string, Node> functions;
    // Variable* calculate(StatementBlock&) override;

    // void declareVariable(std::string name, Variable var);
    // void assignVariable(std::string name, Variable var);
};

class FunctionCall : public Node
{
public:
    FunctionCall(std::string, std::vector<std::unique_ptr<Node>>);
    ~FunctionCall(){};

    std::string toString(int depth = 0) const override;

    std::string funcName;
    std::vector<std::unique_ptr<Node>> args;
    // Variable* calculate(StatementBlock&) override;
};


class VariableReference : public Node
{
public:
    VariableReference(std::string);
    ~VariableReference(){};

    std::string toString(int depth = 0) const override;
    std::string getValue();
    // VariableReference* calculate(StatementBlock&) override;
private:
    std::string name;
};



class Literal : public Node
{
public:
    Literal();
    ~Literal(){};
    std::string toString(int depth = 0) const override;
    // Variable* calculate(StatementBlock&) override;
};


class TextLiteral : public Node
{
public:
    TextLiteral(std::string value_);
    ~TextLiteral(){};

    std::string toString(int depth = 0) const override;
    std::string value;
    // Variable* calculate(StatementBlock&) override;
};


class IntegerLiteral : public Node
{
public:
    IntegerLiteral(int value_);
    ~IntegerLiteral(){};

    std::string toString(int depth = 0) const override;
    int value;
    // Variable* calculate(StatementBlock&) override;
};


class DecimalLiteral : public Node
{
public:
    DecimalLiteral(double value_);
    ~DecimalLiteral(){};
    std::string toString(int depth = 0) const override;
    double getValue() const;
    // Variable* calculate(StatementBlock&) override;
private:
    double value;
};


class Hexgrid : public Node
{
public:
    Hexgrid(std::vector<std::unique_ptr<Node>> cells_);
    ~Hexgrid(){};

    std::string toString(int depth = 0) const override;
    std::vector<std::unique_ptr<Node>> cells;
    // Variable* calculate(StatementBlock&) override;
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
    // Variable* calculate(StatementBlock&) override;
};


class Array : public Node
{
public:
    Array(std::vector<std::unique_ptr<Node>> elements_);
    ~Array(){};

    std::string toString(int depth = 0) const override;
    std::vector<std::unique_ptr<Node>> elements;
    // Variable* calculate(StatementBlock&) override;
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
    // Variable* calculate(StatementBlock&) override;
};


class AndExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    // Variable* calculate(StatementBlock&) override;
};

class LessExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    // Variable* calculate(StatementBlock&) override;
};

class LessOrEqualExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    // Variable* calculate(StatementBlock&) override;
};

class GreaterExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    // Variable* calculate(StatementBlock&) override;
};

class GreaterOrEqualExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    // Variable* calculate(StatementBlock&) override;
};

class EqualExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    // Variable* calculate(StatementBlock&) override;
};

class NotEqualExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    // Variable* calculate(StatementBlock&) override;
};

class BesideExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    // Variable* calculate(StatementBlock&) override;
};

class ByExpression : public BinaryExpression
{
public:

    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    // Variable* calculate(StatementBlock&) override;
};

class OnExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    // Variable* calculate(StatementBlock&) override;
};

class AddExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    // Variable* calculate(StatementBlock&) override;
};

class SubtructExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    // Variable* calculate(StatementBlock&) override;
};

class MultiplyExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    // Variable* calculate(StatementBlock&) override;
};

class DivideExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    // Variable* calculate(StatementBlock&) override;
};

class ModuloExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
    std::string toString(int depth = 0) const override;
    // Variable* calculate(StatementBlock&) override;
};

class LogicalNegation : public Node
{
public:
    LogicalNegation(std::unique_ptr<Node> value_);
    ~LogicalNegation(){};

    std::string toString(int depth = 0) const override;
    // Variable* calculate(StatementBlock&) override;
    std::unique_ptr<Node> value;
};


class ArithmeticalNegation : public Node
{
public:
    ArithmeticalNegation(std::unique_ptr<Node> value_);
    ~ArithmeticalNegation(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> value;
    // Variable* calculate(StatementBlock&) override;
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
    // Variable* calculate(StatementBlock&) override;
};

/*---------------------------------------*/
/*              Statements               */
/*---------------------------------------*/


class AssignmentStatement : public Node
{
public:
    AssignmentStatement(std::string iden_, 
                        std::unique_ptr<Node> value_);
    ~AssignmentStatement(){};

    std::string toString(int depth = 0) const override;

    std::string iden;
    std::unique_ptr<Node> value;
    // Variable* calculate(StatementBlock&) override;
};


class InitializationStatement : public Node
{
public:
    InitializationStatement(Variable::Type, std::string, std::unique_ptr<Node>, int);
    ~InitializationStatement(){};

    std::string toString(int depth = 0) const override;
    std::string toString(Variable::Type) const;
    std::string getType() const;

    Variable::Type type;
    std::string name;
    std::unique_ptr<Node> value;
    int dimension;
    // Variable* calculate(StatementBlock&) override;
};

class AddStatement : public Node
{
public:
    AddStatement( 
                std::unique_ptr<Node> being_added_,
                std::unique_ptr<Node> added_to_,
                std::unique_ptr<Node> added_at_);
    ~AddStatement(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> being_added;
    std::unique_ptr<Node> added_to;
    std::unique_ptr<Node> added_at;
    // Variable* calculate(StatementBlock&) override;
};


class ConditionBlock : public Node
{
public:
    ConditionBlock(std::unique_ptr<Node>,std::unique_ptr<Node>);
    ~ConditionBlock(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> condition;
    std::unique_ptr<Node> statementBlock;
    // Variable* calculate(StatementBlock&) override;
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
    // Variable* calculate(StatementBlock&) override;
};

class IfStatement : public Node
{
public:
    IfStatement( 
                std::unique_ptr<Node> if_block_,
                std::vector<std::unique_ptr<Node>> elif_blocks_,
                std::unique_ptr<Node> else_block_);
    ~IfStatement(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> if_block;
    std::unique_ptr<Node> else_block;
    std::vector<std::unique_ptr<Node>> elif_blocks;
    // Variable* calculate(StatementBlock&) override;
};


class MoveStatement : public Node
{
public:
    MoveStatement(std::unique_ptr<Node> position_source_,
                  std::unique_ptr<Node> grid_source_,
                  std::unique_ptr<Node> grid_target_,
                  std::unique_ptr<Node> position_target_);
    ~MoveStatement(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> position_source;
    std::unique_ptr<Node> position_target;
    std::unique_ptr<Node> grid_source;
    std::unique_ptr<Node> grid_target;
    // Variable* calculate(StatementBlock&) override;
};


class RemoveStatement : public Node
{
public:
    RemoveStatement(std::unique_ptr<Node> position_,
                    std::unique_ptr<Node> grid_);
    ~RemoveStatement(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> position;
    std::unique_ptr<Node> grid;
    // Variable* calculate(StatementBlock&) override;
};


class ReturnStatement : public Node
{
public:
    ReturnStatement(std::unique_ptr<Node> expr_);
    ~ReturnStatement(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> expr;
    // Variable* calculate(StatementBlock&) override;
};
} // namespace ast

#endif // TKOM_AST_H
