#ifndef TKOM_INTERPRETER_H
#define TKOM_INTERPRETER_H

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <variant>
#include <set>
#include <iostream>
#include <utility>
#include <string>
#include <HexgridErrors.h>
#include <parser/Ast.h>
#include <parser/Parser.h>
namespace intprt
{

class Array;
class Hexgrid;
using Var = std::variant<std::monostate, int, double, std::string, Array, Hexgrid>;
class Array
{
public:
    Array();
    Var get(int) const;
    void add(Var);
    int size() const;
    std::string toString() const;
private:
    std::vector<Var> values;
};
class Hexgrid
{
public:
    Hexgrid();
    Var on(int, int, int);
    Var on(std::tuple<int, int, int>);
    Var beside(int, int, int) ;
    Var by(Var);
    void add(Var, Var);
    Var remove(Var);
    std::string toString()const;
    std::tuple<int, int, int> arrayToTuple(Var);
    std::vector<std::tuple<int, int, int>> getKeys();
    int size();
private:
    std::map<std::tuple<int, int, int>, Var> cells;
    std::vector<std::tuple<int, int, int>> directions = {
        {0, -1, 1}, {0, 1, -1}, {1, 0, -1}, 
        {-1, 0, 1}, {1, -1, 0}, {-1, 1, 0}};
};


class Scope
{
public:
    void declare(int, std::string);
    void assign(std::string, Var);
    Var getValue(std::string);
    bool containsVar(std::string);
    size_t getIndex(std::string);
private:
    std::set<std::string> uninitialized;
    std::map<std::string, int> variable_types;
    std::map<std::string, Var> variables;
};

class FunctionCallContext
{
public:
    void declare(int, std::string);
    void assign(std::string, Var);
    void assign(int, std::string, Var);
    Var getValue(std::string);
    int getScopeCount();
    int findScope(std::string);
    void pushScope();
    void popScope();
    void pushContext();
    void popContext();
    size_t getIndex(std::string);

private:
    std::vector<Scope> scopes;
};

class Interpreter : public ast::AstVisitor
{
private: 
    std::map<std::string, std::unique_ptr<ast::FunctionDefinition>> funcs;
    std::vector<FunctionCallContext> contextStack;
    Scope globalScope;
    Var result;
    Var result2;
    std::string lastDeclared;
    std::vector<Var> functionArgs;
    bool returning;

public:
    Interpreter();
    void declare(int, std::string);
    void assign(std::string);
    void assign(std::string, Var);
    int findContextWithVariable(std::string);
    bool isGlobalscope();
    bool containsVar(std::string);
    bool containsFun(std::string);
    Var getValue(std::string);
    void pushScope();
    void popScope();
    void pushContext();
    void popContext();
    size_t getIndex(std::string);
    bool isPosition(Var);


    void visit(ast::Program&) override;
    void visit(ast::VariableDeclarationStatement&) override;
    void visit(ast::FunctionDefinition&) override;
    void visit(ast::StatementBlock&) override;
    void visit(ast::FunctionCall&) override;
    void visit(ast::VariableReference&) override;
    void visit(ast::TextLiteral&) override;
    void visit(ast::IntegerLiteral&) override;
    void visit(ast::DecimalLiteral&) override;
    void visit(ast::HexgridLiteral&) override;
    void visit(ast::HexgridCell&) override;
    void visit(ast::ArrayLiteral&) override;
    void visit(ast::OrExpression&) override;
    void visit(ast::AndExpression&) override;
    void visit(ast::LessExpression&) override;
    void visit(ast::LessOrEqualExpression&) override;
    void visit(ast::GreaterExpression&) override;
    void visit(ast::GreaterOrEqualExpression&) override;
    void visit(ast::EqualExpression&) override;
    void visit(ast::NotEqualExpression&) override;
    void visit(ast::BesideExpression&) override;
    void visit(ast::ByExpression&) override;
    void visit(ast::OnExpression&) override;
    void visit(ast::AddExpression&) override;
    void visit(ast::SubtructExpression&) override;
    void visit(ast::MultiplyExpression&) override;
    void visit(ast::DivideExpression&) override;
    void visit(ast::ModuloExpression&) override;
    void visit(ast::LogicalNegation&) override;
    void visit(ast::ArithmeticalNegation&) override;
    void visit(ast::IndexingExpression&) override;
    void visit(ast::AssignmentStatement&) override;
    void visit(ast::InitializationStatement&) override;
    void visit(ast::AddStatement&) override;
    void visit(ast::ConditionBlock&) override;
    void visit(ast::ForeachStatement&) override;
    void visit(ast::IfStatement&) override;
    void visit(ast::MoveStatement&) override;
    void visit(ast::RemoveStatement&) override; 
    void visit(ast::ReturnStatement&) override; 
};


template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;


} // namespace intprt

#endif // TKOM_INTERPRETER_H
