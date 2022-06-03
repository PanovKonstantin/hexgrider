#ifndef TKOM_AST_H
#define TKOM_AST_H

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <variant>
#include <iostream>
namespace ast
{
class Node;
class Scope;

class Variable{
public:
    enum class Type{
        Int,
        String,
        Float,
        Hexgrid,
        Undefiend
    };

    Variable();
    Variable(Type, int = 0);


    int getInteger(std::vector<int> = {});
    double getFloat(std::vector<int> = {});
    std::string getString(std::vector<int> = {});
    std::vector<Variable> getArray(std::vector<int> = {});

    Type getType() const {return type;};

    void setValue(int, std::vector<int> = {});
    void setValue(double, std::vector<int> = {});
    void setValue(std::string, std::vector<int> = {});

    Type type;
    int dimension;
    std::variant<int, double, std::string, std::vector<Variable>> value;

};
class Node
{
public:
    virtual ~Node();
    virtual std::string toString(int depth = 0) const = 0;
    virtual Variable* calculate(Scope&) = 0;
};

class Scope : public Node
{
public:
    Scope(std::vector<std::unique_ptr<ast::Node>> stmnts_);
    ~Scope(){};

    std::string toString(int depth = 0) const override;
    std::vector<std::unique_ptr<ast::Node>> stmnts;
    ast::Scope* upper_scope;
    std::map<std::string, ast::Variable> variables;
    // std::map<std::string, ast::Node> functions;
    Variable* calculate(Scope&) override;

    void declareVariable(std::string name, ast::Variable var);
    void assignVariable(std::string name, ast::Variable var);
};

/*----------------------------------*/
/*          Unary Expressions       */
/*----------------------------------*/

class FunctionCall : public Node
{
public:
    FunctionCall(std::unique_ptr<Node> func_, 
                 std::vector<std::unique_ptr<ast::Node>> args_);
    ~FunctionCall(){};

    std::string toString(int depth = 0) const override;

    std::unique_ptr<Node> func;
    std::vector<std::unique_ptr<ast::Node>> args;
    Variable* calculate(Scope&) override;
};


class Identifier : public Node
{
public:
    Identifier(std::string value_);
    ~Identifier(){};

    std::string toString(int depth = 0) const override;
    std::string getValue();
    Variable* calculate(Scope&) override;
private:
    std::string value;
};



class Literal : public Node
{
public:
    Literal();
    ~Literal(){};
    std::string toString(int depth = 0) const override;
    Variable* calculate(Scope&) override;
};


class TextLiteral : public Node
{
public:
    TextLiteral(std::string value_);
    ~TextLiteral(){};

    std::string toString(int depth = 0) const override;
    std::string value;
    Variable* calculate(Scope&) override;
};


class IntegerLiteral : public Node
{
public:
    IntegerLiteral(int value_);
    ~IntegerLiteral(){};

    std::string toString(int depth = 0) const override;
    int value;
    Variable* calculate(Scope&) override;
};


class DecimalLiteral : public Node
{
public:
    DecimalLiteral(double value_);
    ~DecimalLiteral(){};
    std::string toString(int depth = 0) const override;
    double getValue() const;
    Variable* calculate(Scope&) override;
private:
    double value;
};


class Hexgrid : public Node
{
public:
    Hexgrid(std::vector<std::unique_ptr<ast::Node>> cells_);
    ~Hexgrid(){};

    std::string toString(int depth = 0) const override;
    std::vector<std::unique_ptr<ast::Node>> cells;
    Variable* calculate(Scope&) override;
};


class HexgridCell : public Node
{
public:
    HexgridCell(std::unique_ptr<Node> value_,
                std::unique_ptr<Node> pos_);
    ~HexgridCell(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<ast::Node> pos;
    std::unique_ptr<ast::Node> value;
    Variable* calculate(Scope&) override;
};


class Array : public Node
{
public:
    Array(std::vector<std::unique_ptr<ast::Node>> elements_);
    ~Array(){};

    std::string toString(int depth = 0) const override;
    std::vector<std::unique_ptr<ast::Node>> elements;
    Variable* calculate(Scope&) override;
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

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> lvalue;
    std::unique_ptr<Node> rvalue;
    Variable* calculate(Scope&) override;
};


class AndExpression : public Node
{
public:
    AndExpression(std::unique_ptr<Node> lvalue_,
                       std::unique_ptr<Node> rvalue_);
    ~AndExpression(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> lvalue;
    std::unique_ptr<Node> rvalue;
    Variable* calculate(Scope&) override;
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

    std::string toString(int depth = 0) const override;
    std::string toString(Operator op_)  const;
    std::unique_ptr<Node> lvalue;
    Operator op;
    std::unique_ptr<Node> rvalue;
    Variable* calculate(Scope&) override;
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

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> lvalue;
    Operator op;
    std::unique_ptr<Node> rvalue;
    Variable* calculate(Scope&) override;
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

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> lvalue;
    Operator op;
    std::unique_ptr<Node> rvalue;
    Variable* calculate(Scope&) override;
};


class FOArithmExpression : public Node
{
public:
    enum class Operator
    {
        Multiply,
        Divide
    };
    FOArithmExpression(std::unique_ptr<Node> lvalue_,
                       Operator op_,
                       std::unique_ptr<Node> rvalue_);
    ~FOArithmExpression(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> lvalue;
    Operator op;
    std::unique_ptr<Node> rvalue;
    Variable* calculate(Scope&) override;
};


class LogicalNegation : public Node
{
public:
    LogicalNegation(std::unique_ptr<Node> value_);
    ~LogicalNegation(){};

    std::string toString(int depth = 0) const override;
    Variable* calculate(Scope&) override;
    std::unique_ptr<Node> value;
};


class ArithmeticalNegation : public Node
{
public:
    ArithmeticalNegation(std::unique_ptr<Node> value_);
    ~ArithmeticalNegation(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<Node> value;
    Variable* calculate(Scope&) override;
};


class IndexingExpression : public Node
{
public:
    IndexingExpression(std::unique_ptr<ast::Node> indexOn_,
                       std::unique_ptr<ast::Node> indexBy_);
    ~IndexingExpression(){};

    std::string toString(int depth = 0) const override;

    std::unique_ptr<ast::Node> indexOn;
    std::unique_ptr<ast::Node> indexBy;
    Variable* calculate(Scope&) override;
};

/*---------------------------------------*/
/*              Statements               */
/*---------------------------------------*/

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
    DeclarationStatement(Type type_, std::unique_ptr<Identifier> var_, int dimenstion_=0);
    ~DeclarationStatement(){};

    std::string toString(int depth = 0) const override;
    std::string getType() const;
    std::string toString(Type type_) const;
    std::string getIdentifier();

    Variable* calculate(Scope&) override;

    Type type;
    std::unique_ptr<Identifier> iden;
    int dimenstion;
};

class AssignmentStatement : public Node
{
public:
    AssignmentStatement(std::unique_ptr<DeclarationStatement> declr_, 
                        std::unique_ptr<Node> value_);
    AssignmentStatement(std::unique_ptr<Identifier> iden_, 
                        std::unique_ptr<Node> value_);
    ~AssignmentStatement(){};

    std::string toString(int depth = 0) const override;

    std::unique_ptr<DeclarationStatement> decl;
    std::unique_ptr<Identifier> iden;
    std::unique_ptr<Node> value;
    Variable* calculate(Scope&) override;
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
    std::unique_ptr<ast::Node> being_added;
    std::unique_ptr<ast::Node> added_to;
    std::unique_ptr<ast::Node> added_at;
    Variable* calculate(Scope&) override;
};


class ConditionBlock : public Node
{
public:
    ConditionBlock(std::unique_ptr<Node> condition_,
            std::unique_ptr<Node> scope_);
    ~ConditionBlock(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<ast::Node> condition;
    std::unique_ptr<ast::Node> scope;
    Variable* calculate(Scope&) override;
};


class ForeachStatement : public Node
{
public:
    ForeachStatement( 
                std::unique_ptr<Node> iterator_,
                std::unique_ptr<Node> iterated_,
                std::unique_ptr<Node> scope_);
    ~ForeachStatement(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<ast::Node> iterator;
    std::unique_ptr<ast::Node> iterated;
    std::unique_ptr<ast::Node> scope;
    Variable* calculate(Scope&) override;
};

class FunctionDefinition : public Node
{
public:
    FunctionDefinition( 
                std::unique_ptr<Node> fun_,
                std::vector<std::unique_ptr<ast::Node>> params_,
                std::unique_ptr<Node> scope_);
    ~FunctionDefinition(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<ast::Node> fun;
    std::vector<std::unique_ptr<ast::Node>> params;
    std::unique_ptr<ast::Node> scope;
    Variable* calculate(Scope&) override;
};

class IfStatement : public Node
{
public:
    IfStatement( 
                std::unique_ptr<Node> if_block_,
                std::vector<std::unique_ptr<ast::Node>> elif_blocks_,
                std::unique_ptr<Node> else_block_);
    ~IfStatement(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<ast::Node> if_block;
    std::unique_ptr<ast::Node> else_block;
    std::vector<std::unique_ptr<ast::Node>> elif_blocks;
    Variable* calculate(Scope&) override;
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
    Variable* calculate(Scope&) override;
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
    Variable* calculate(Scope&) override;
};


class ReturnStatement : public Node
{
public:
    ReturnStatement(std::unique_ptr<Node> expr_);
    ~ReturnStatement(){};

    std::string toString(int depth = 0) const override;
    std::unique_ptr<ast::Node> expr;
    Variable* calculate(Scope&) override;
};




} // namespace ast

#endif // TKOM_AST_H
