#ifndef TKOM_INTERPRETER_H
#define TKOM_INTERPRETER_H

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <variant>
#include <set>
#include <iostream>
#include "Ast.h"
// #include "Parser.h"
namespace ast
{
    

class Scope
{
public:
    void declare(Variable::Type, std::string);
    void assign(std::string, int);
    // void assign(std::string, double);
    // void assign(std::string, std::string);
    int getIntValue(std::string);
    // double getDoubleValue(std::string);
    // std::string getStringValue(std::string);
private:
    std::set<std::string> uninitialized;
    std::map<std::string, Variable> variables;
};

class FunctionCallContext
{
public:
    void declare(std::string, VariableDeclarationStatement);
    void assign(std::string, int);
    // void assign(std::string, double);
    // void assign(std::string, std::string);
    int getIntValue(std::string);
    // double getDoubleValue(std::string);
    // std::string getStringValue(std::string);
    void popContext();
    void pushContext();

private:
    std::vector<Scope> scopes;
};

class Environment
{
public:
    void declare(Variable::Type, std::string);
private:
    // std::map<std::string, 
    std::vector<FunctionCallContext> callStack;
    Scope globalScope;
};

class InterpreterVisitor : public AstVisitor
{
public:
    virtual void visit(Program&);
    virtual void visit(VariableDeclarationStatement&);

    Environment env;
};

} // namespace ast

#endif // TKOM_INTERPRETER_H
