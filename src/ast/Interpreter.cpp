#include "Interpreter.h"
using namespace ast;
using namespace std;

void Scope::declare(Variable::Type type, string name){
    uninitialized.insert(name);
    variables[name] = Variable(type);
}

void Environment::declare(Variable::Type type, string name){
    if(callStack.empty()){
        globalScope.declare(type, name);
    }
}

void InterpreterVisitor::visit(Program& p){
    for(auto const& stmnt: p.stmnts)
    {
        stmnt->accept(*this);
    }
}

void InterpreterVisitor::visit(VariableDeclarationStatement& vds){
    // if(callStack.empty())
    env.declare(vds.type, vds.identifier);
}