#include "Interpreter.h"
using namespace ast;
using namespace parser;
using namespace std;
using namespace intprt;

Array::Array(){
    values = vector<Var>();
}
Var Array::get(int i) const {
    return values[i];
}
void Array::add(Var v){
    values.push_back(v);
}
int Array::size() const {
    return values.size();
}
std::string Array::toString()const{
    string returnString="[ ";
    for(auto& elem : values){
        switch(elem.index()){
            case 1: 
                returnString+=to_string(std::get<int>(elem));
                break;
            case 2: 
                returnString+=to_string(std::get<double>(elem));
                break;
            case 3: returnString+=std::get<string>(elem);
                break;
            case 4: returnString+=std::get<Array>(elem).toString();
                break;
            case 5: returnString+=std::get<Hexgrid>(elem).toString();
                break;
        }
        returnString += ", ";
    }
    return returnString + "]";
}

Hexgrid::Hexgrid(){
}

tuple<int, int, int> Hexgrid::arrayToTuple(Var v){
    auto arr = get<Array>(v);
    auto q = get<int>(arr.get(0));
    auto r = get<int>(arr.get(1));
    auto s = get<int>(arr.get(2));
    if(q + r + s != 0) throw std::runtime_error("Incorrect hexgrid coordinate. sum must be equal 0.");
    tuple<int, int, int> t(q, r, s);
    return t;
}

Var Hexgrid::on(int q, int r, int s)  {
    tuple<int, int, int> key(q, r, s);
    return on(key);
}

Var Hexgrid::on(tuple<int, int, int> key)  {
    if(!cells.count(key)) throw std::runtime_error("No such cell");
    return cells[key];
}
void Hexgrid::add(Var arr, Var value){
    auto key = arrayToTuple(arr);
    if(cells.count(key)) throw std::runtime_error("Cell is taken");
    cells[key] = value;
}
Var Hexgrid::by(Var value){
    auto foundPositions = Array();
    for(auto const& [pos, cellValue] : cells){
        const size_t index = cellValue.index();
        if(index == value.index()){
            if( (index == 1 && std::get<1>(value) == std::get<1>(cellValue)) ||
                (index == 2 && std::get<2>(value) == std::get<2>(cellValue)) ||
                (index == 3 && std::get<3>(value) == std::get<3>(cellValue))){
                auto posArray = Array();
                posArray.add(std::get<0>(pos));
                posArray.add(std::get<1>(pos));
                posArray.add(std::get<2>(pos));
                foundPositions.add(posArray);
            }
        }
    }
    Var returnArray = Var();
    returnArray = foundPositions;
    return returnArray;
}

Var Hexgrid::beside(int q, int r, int s){
    auto positions = Array();
    for(auto const& direction : directions){
        int q_ = q + get<0>(direction);
        int r_ = r + get<1>(direction);
        int s_ = s + get<2>(direction);
        tuple<int, int, int> pos(q_, r_, s_);
        if(cells.count(pos)){
            auto position = Array();
            position.add(q_);
            position.add(r_);
            position.add(s_);
            positions.add(position);
        }
    }
    Var returnArray = positions;
    return returnArray;
}

Var Hexgrid::remove(Var v){
    auto pos = arrayToTuple(v);
    auto value = cells[pos];
    cells.erase(pos);
    return value;
}

int Hexgrid::size(){
    return cells.size();
}

std::string Hexgrid::toString()const{
    string returnString="< ";
    for(auto const& [pos, elem] : cells){
        switch(elem.index()){
            case 1: returnString+=to_string(get<int>(elem));
                break;
            case 2: returnString+=to_string(get<double>(elem));
                break;
            case 3: returnString+=get<string>(elem);
                break;
            case 4: returnString+=get<Array>(elem).toString();
                break;
            case 5: returnString+=get<Hexgrid>(elem).toString();
                break;
        }
        returnString += " at [";
        returnString += to_string(get<0>(pos)) + ", ";
        returnString += to_string(get<1>(pos)) + ", ";
        returnString += to_string(get<2>(pos)) + "], ";
    }
    return returnString + ">";
}

vector<tuple<int, int, int>> Hexgrid::getKeys(){
    auto keys = vector<tuple<int, int, int>>();
    for(auto const& [key, elem] : cells){
        keys.push_back(key);
    }
    return keys;
}

void Scope::declare(int type, string name){
    uninitialized.insert(name);
    variable_types[name] = type;
}

void Scope::assign(string name, Var value){
    if(!containsVar(name)) throw runtime_error("No variable " + name);
    if(int(value.index()) != variable_types[name]) throw runtime_error("type doesnt match");
    variables[name] = value;
}

bool Scope::containsVar(string name){
    return uninitialized.count(name) || variables.count(name);
}
Var Scope::getValue(string name){
    return variables[name];
}

size_t Scope::getIndex(string name){
    return variable_types[name];
}

int FunctionCallContext::findScope(string name){
    for(int i=scopes.size()-1; i>=0; i--){
        if(scopes[i].containsVar(name)) return i;
    }
    return -1;
}
int FunctionCallContext::getScopeCount(){
    return scopes.size();
}

Var FunctionCallContext::getValue(string name){
    int scope = findScope(name);
    return scopes[scope].getValue(name);
}

void FunctionCallContext::declare(int type, string name){
    scopes.back().declare(type, name);
}

void FunctionCallContext::assign(string name, Var value){
    int scope = findScope(name);
    assign(scope, name, value);
}

void FunctionCallContext::assign(int scope, string name, Var value){
    scopes[scope].assign(name, value);
}

void FunctionCallContext::pushScope(){
    scopes.push_back(Scope());
}

void FunctionCallContext::popScope(){
    scopes.pop_back();
}

size_t FunctionCallContext::getIndex(string name){
    int scope = findScope(name);
    return scopes[scope].getIndex(name);
}

Interpreter::Interpreter()
{
    contextStack.push_back(FunctionCallContext());
    returning=false;
}

bool Interpreter::isGlobalscope(){
    return (contextStack.size() == 1 && contextStack[0].getScopeCount() == 0);
}

void Interpreter::declare(int type, string name){
    if(isGlobalscope()){
        globalScope.declare(type, name);
    } else {
        contextStack.back().declare(type, name);
    }
    lastDeclared = name;
}
void Interpreter::assign(string name, Var value){
    if(value.index()==0) return;
    int scope;
    scope = contextStack.back().findScope(name);
    if(scope>=0) contextStack.back().assign(scope, name, value);
    else globalScope.assign(name, value);
}

int Interpreter::findContextWithVariable(string name){
    for(int i=contextStack.size()-1; i>=0; i--){
        if(contextStack[i].findScope(name) >= 0) return i;
    }
    return -1;
}

bool Interpreter::containsVar(string name){
    if (findContextWithVariable(name) >= 0) return true;
    return globalScope.containsVar(name);
}

bool Interpreter::containsFun(string name){
    return funcs.count(name);
}

Var Interpreter::getValue(string name){
    int context = findContextWithVariable(name);
    if (context >= 0) return contextStack[context].getValue(name);
    if (!globalScope.containsVar(name)) throw hexgrid_errors::VariableIsNotDeclared(name);
    return globalScope.getValue(name);
}

void Interpreter::pushScope(){
    contextStack.back().pushScope();
}
void Interpreter::popScope(){
    contextStack.back().popScope();
}

void Interpreter::pushContext(){
    contextStack.push_back(FunctionCallContext());
    pushScope();
}
void Interpreter::popContext(){
    contextStack.pop_back();
}

size_t Interpreter::getIndex(string name){
    int context = findContextWithVariable(name);
    if (context >= 0) return contextStack[context].getIndex(name);
    if (!globalScope.containsVar(name)) throw hexgrid_errors::VariableIsNotDeclared(name);
    return globalScope.getIndex(name);
}

void Interpreter::visit(Program& p){
    funcs.swap(p.funcs);
    for(auto const& stmnt: p.stmnts)
        stmnt->accept(*this);
}

void Interpreter::visit(VariableDeclarationStatement& vds){
    switch(vds.type){
        case Variable::Type::Int:       declare(1, vds.identifier); break;
        case Variable::Type::Float:     declare(2, vds.identifier); break;
        case Variable::Type::String:    declare(3, vds.identifier); break;
        case Variable::Type::Array:     declare(4, vds.identifier); break;
        case Variable::Type::Hexgrid:   declare(5, vds.identifier); break;
    }
}

void Interpreter::visit(AssignmentStatement& as){
    as.value->accept(*this);
    assign(as.name, result);
}

void Interpreter::visit(InitializationStatement& initialization){
    switch(initialization.type){
        case Variable::Type::Int:       declare(1, initialization.name);    break;
        case Variable::Type::Float:     declare(2, initialization.name);    break;
        case Variable::Type::String:    declare(3, initialization.name);    break;
        case Variable::Type::Array:     declare(4, initialization.name);    break;
        case Variable::Type::Hexgrid:   declare(5, initialization.name); break;
    }
    initialization.value->accept(*this);
    assign(initialization.name, result);
}

void Interpreter::visit(IntegerLiteral& intLit){
    result = intLit.getValue();
}
void Interpreter::visit(DecimalLiteral& floatLit){
    result = floatLit.getValue();
}
void Interpreter::visit(TextLiteral& textLit){
    result = textLit.getValue();
}

void Interpreter::visit(ArrayLiteral& arrLit){
    auto arr = Array();
    for(size_t i=0;i<arrLit.elements.size(); i++){
        arrLit.elements[i]->accept(*this);
        arr.add(result);
    }
    result = arr;
}

void Interpreter::visit(HexgridLiteral& hexLit){
    auto hex = Hexgrid();
    for(auto const& cell : hexLit.cells){
        cell->accept(*this);
        hex.add(result2, result);
    }
    result = hex;
}

void Interpreter::visit(HexgridCell& hexCell){
    hexCell.pos->accept(*this);
    if(result.index() != 4 || get<Array>(result).size() != 3) throw std::runtime_error("Cant be a hexgrid coordnate");
    result2 = result;
    hexCell.value->accept(*this);
}


void Interpreter::visit(VariableReference& varRef){
    result = getValue(varRef.getName());
}

void Interpreter::visit(ArithmeticalNegation& expr){
    expr.value->accept(*this);
    std::visit(overload{
        [](int& v)      {v=-v;},
        [](double& v)   {v=-v;},
        [](auto&)     {throw std::runtime_error("type mismatch");},
    }, result);

}
void Interpreter::visit(LogicalNegation& expr){
    expr.value->accept(*this);
    std::visit(overload{
        [](int& v)      {v=!v;},
        [](double& v)   {v=!v;},
        [](auto&)     {throw std::runtime_error("type mismatch");},
    }, result);
}

void Interpreter::visit(AddExpression& expr){
    
    expr.rvalue->accept(*this);
    auto rvalue = result;
    expr.lvalue->accept(*this);
    std::visit(overload{
        [](int& l, int& r)                  {l+=r;},
        [](int& l, double& r)               {l+=r;},
        [](double& l, int& r)               {l+=r;},
        [](double& l, double& r)            {l+=r;},
        [](std::string& l, std::string& r)  {l+=r;},
        [](auto&, auto&)                {throw std::runtime_error("type mismatch");},
    }, result, rvalue);
}

void Interpreter::visit(SubtructExpression& expr){
    
    expr.rvalue->accept(*this);
    auto rvalue = result;
    expr.lvalue->accept(*this);
    std::visit(overload{
        [](int& l, int& r)                  {l-=r;},
        [](int& l, double& r)               {l-=r;},
        [](double& l, int& r)               {l-=r;},
        [](double& l, double& r)            {l-=r;},
        [](auto&, auto&)    {throw std::runtime_error("type mismatch");},
    }, result, rvalue);
}

void Interpreter::visit(ModuloExpression& expr){
    expr.rvalue->accept(*this);
    auto rvalue = result;
    expr.lvalue->accept(*this);
    std::visit(overload{
        [](int& l, int& r)      {l=l%r;},
        [](auto&, auto&)    {throw std::runtime_error("type mismatch");},
    }, result, rvalue);
}


void Interpreter::visit(DivideExpression& expr)
{
    expr.rvalue->accept(*this);
    auto rvalue = result;
    expr.lvalue->accept(*this);
    std::visit(overload{
        [](int& l, int& r)      {l=l/r;},
        [](int& l, double& r)   {l=l/r;},
        [](double& l, int& r)   {l=l/r;},
        [](double& l, double& r){l=l/r;},
        [](auto&, auto&)    {throw std::runtime_error("type mismatch");},
    }, result, rvalue);
}

void Interpreter::visit(MultiplyExpression& expr){
    expr.rvalue->accept(*this);
    auto rvalue = result;
    expr.lvalue->accept(*this);
    std::visit(overload{
        [](int& l, int& r)      {l=l*r;},
        [](int& l, double& r)   {l=l*r;},
        [](double& l, int& r)   {l=l*r;},
        [](double& l, double& r){l=l*r;},
        [](auto&, auto&)    {throw std::runtime_error("type mismatch");},
    }, result, rvalue);
}

void Interpreter::visit(IndexingExpression& expr){
    expr.indexBy->accept(*this);
    auto indexBy = result;
    expr.indexOn->accept(*this);
    auto indexOn = result;
    if(indexOn.index() == 4 && indexBy.index() == 1){
        int i = get<int>(indexBy);
        result = get<Array>(indexOn).get(i);
    }
}

void Interpreter::visit(EqualExpression& expr){
    expr.rvalue->accept(*this);
    auto rvalue = result;
    expr.lvalue->accept(*this);
    auto lvalue = result;
    result = 0;
    std::visit(overload{
        [](int& res, int& l, int& r)        {res = int(l == r);},
        [](int& res, double& l, double& r)  {res = int(l == r);},
        [](int& res, int& l, double& r)     {res = int(l == r);},
        [](int& res, double& l, int& r)     {res = int(l == r);},
        [](int& res, string& l, string& r)  {res = int(l == r);},
        [](auto&, auto&, auto&)         {throw std::runtime_error("type mismatch");},
    }, result, lvalue, rvalue);
}
void Interpreter::visit(NotEqualExpression& expr){
    expr.rvalue->accept(*this);
    auto rvalue = result;
    expr.lvalue->accept(*this);
    auto lvalue = result;
    result = 0;
    std::visit(overload{
        [](int& res, int& l, int& r)        {res = int(l != r);},
        [](int& res, double& l, double& r)  {res = int(l != r);},
        [](int& res, int& l, double& r)     {res = int(l != r);},
        [](int& res, double& l, int& r)     {res = int(l != r);},
        [](int& res, string& l, string& r)  {res = int(l != r);},
        [](auto&, auto&, auto&)         {throw std::runtime_error("type mismatch");},
    }, result, lvalue, rvalue);

}

void Interpreter::visit(GreaterExpression& expr){
    expr.rvalue->accept(*this);
    auto rvalue = result;
    expr.lvalue->accept(*this);
    auto lvalue = result;
    result = 0;
    std::visit(overload{
        [](int& res, int& l, int& r)        {res = int(l > r);},
        [](int& res, double& l, double& r)  {res = int(l > r);},
        [](int& res, int& l, double& r)     {res = int(l > r);},
        [](int& res, double& l, int& r)     {res = int(l > r);},
        [](auto&, auto&, auto&)         {throw std::runtime_error("type mismatch");},
    }, result, lvalue, rvalue);
}

void Interpreter::visit(LessOrEqualExpression& expr){
    expr.rvalue->accept(*this);
    auto rvalue = result;
    expr.lvalue->accept(*this);
    auto lvalue = result;
    result = 0;
    std::visit(overload{
        [](int& res, int& l, int& r)        {res = int(l <= r);},
        [](int& res, double& l, double& r)  {res = int(l <= r);},
        [](int& res, int& l, double& r)     {res = int(l <= r);},
        [](int& res, double& l, int& r)     {res = int(l <= r);},
        [](auto&, auto&, auto&)         {throw std::runtime_error("type mismatch");},
    }, result, lvalue, rvalue);
}

void Interpreter::visit(LessExpression& expr){
    expr.rvalue->accept(*this);
    auto rvalue = result;
    expr.lvalue->accept(*this);
    auto lvalue = result;
    result = 0;
    std::visit(overload{
        [](int& res, int& l, int& r)        {res = int(l < r);},
        [](int& res, double& l, double& r)  {res = int(l < r);},
        [](int& res, int& l, double& r)     {res = int(l < r);},
        [](int& res, double& l, int& r)     {res = int(l < r);},
        [](auto&, auto&, auto&)         {throw std::runtime_error("type mismatch");},
    }, result, lvalue, rvalue);
}

void Interpreter::visit(GreaterOrEqualExpression& expr){
    expr.rvalue->accept(*this);
    auto rvalue = result;
    expr.lvalue->accept(*this);
    auto lvalue = result;
    result = 0;
    std::visit(overload{
        [](int& res, int& l, int& r)        {res = int(l >= r);},
        [](int& res, double& l, double& r)  {res = int(l >= r);},
        [](int& res, int& l, double& r)     {res = int(l >= r);},
        [](int& res, double& l, int& r)     {res = int(l >= r);},
        [](auto&, auto&, auto&)         {throw std::runtime_error("type mismatch");},
    }, result, lvalue, rvalue);
}

void Interpreter::visit(AndExpression& expr){
    expr.rvalue->accept(*this);
    auto rvalue = result;
    expr.lvalue->accept(*this);
    auto lvalue = result;
    result = 0;
    std::visit(overload{
        [](int& res, int& l, int& r)        {res = int(l && r);},
        [](int& res, double& l, double& r)  {res = int(l && r);},
        [](int& res, int& l, double& r)     {res = int(l && r);},
        [](int& res, double& l, int& r)     {res = int(l && r);},
        [](auto&, auto&, auto&)         {throw std::runtime_error("type mismatch");},
    }, result, lvalue, rvalue);
}

void Interpreter::visit(OrExpression& expr){
    expr.rvalue->accept(*this);
    auto rvalue = result;
    expr.lvalue->accept(*this);
    auto lvalue = result;
    result = 0;
    std::visit(overload{
        [](int& res, int& l, int& r)        {res = int(l || r);},
        [](int& res, double& l, double& r)  {res = int(l || r);},
        [](int& res, int& l, double& r)     {res = int(l || r);},
        [](int& res, double& l, int& r)     {res = int(l || r);},
        [](auto&, auto&, auto&)         {throw std::runtime_error("type mismatch");},
    }, result, lvalue, rvalue);
}

void Interpreter::visit(OnExpression& expr){
    expr.lvalue->accept(*this);
    auto hexgrid = result;
    expr.rvalue->accept(*this);
    if(hexgrid.index()==5 && result.index()==4 && get<Array>(result).size()==3){
        int q = get<int>(get<Array>(result).get(0));
        int r = get<int>(get<Array>(result).get(1));
        int s = get<int>(get<Array>(result).get(2));
        result = get<Hexgrid>(hexgrid).on(q, r, s);
    }
}

void Interpreter::visit(ByExpression& expr){
    expr.lvalue->accept(*this);
    auto hexgrid = result;
    expr.rvalue->accept(*this);
    if(hexgrid.index()==5)
        result = get<Hexgrid>(hexgrid).by(result);
}

void Interpreter::visit(BesideExpression& expr){
    expr.lvalue->accept(*this);
    auto hexgrid = result;
    expr.rvalue->accept(*this);
    if(hexgrid.index()==5 && result.index()==4 && get<Array>(result).size()==3){
        int q = get<int>(get<Array>(result).get(0));
        int r = get<int>(get<Array>(result).get(1));
        int s = get<int>(get<Array>(result).get(2));
        result = get<Hexgrid>(hexgrid).beside(q, r, s);
    }
}



void Interpreter::visit(IfStatement& ifStmnt){
    ifStmnt.ifBlock->accept(*this);
    if(result.index() == 1 && get<int>(result) == 1) return;
    for(auto const& elifBlock : ifStmnt.elifBlocks){
        elifBlock->accept(*this);
        if(result.index() == 1 && get<int>(result) == 1) return;
    }
    if(ifStmnt.elseBlock) ifStmnt.elseBlock->accept(*this);
}
void Interpreter::visit(ConditionBlock& conditionBlock){
    conditionBlock.condition->accept(*this);
    if(result.index() == 1 && get<int>(result) == 1){
        pushScope();
        conditionBlock.statementBlock->accept(*this);
        popScope();
        result = 1;
    } else if (result.index() != 1) throw std::runtime_error("Wrong condition");
    else{
        result = 0;
    }

}
void Interpreter::visit(StatementBlock& statementBlock){
    for(auto const& stmnt: statementBlock.stmnts){
        stmnt->accept(*this);
        if(returning){
            returning=false;
            return;
        }
    }
    result = {};
}

void Interpreter::visit(ForeachStatement& foreachStatement){
    foreachStatement.iterated->accept(*this);
    if(result.index() == 4){
        auto iterated = get<4>(result);
        for(int i = 0; i<iterated.size(); i++){
            pushScope();
            auto elem = iterated.get(i);
            foreachStatement.iterator->accept(*this);
            if(getIndex(lastDeclared) == elem.index()){
                assign(lastDeclared, elem);
                foreachStatement.statementBlock->accept(*this);
            }
            popScope();
        }
    } else if(result.index() == 5){
        auto iterated = get<5>(result);
        auto keys = iterated.getKeys();
        for(size_t i = 0; i<keys.size();i++){
            pushScope();
            auto arr = Array();
            arr.add(get<0>(keys[i]));
            arr.add(get<1>(keys[i]));
            arr.add(get<2>(keys[i]));
            Var elem = arr;
            foreachStatement.iterator->accept(*this);
            if(getIndex(lastDeclared) == elem.index()){
                assign(lastDeclared, elem);
                foreachStatement.statementBlock->accept(*this);
            }
            popScope();
        }
    }
    else  throw std::runtime_error("Can only iterate array or hexgrid");

}

void Interpreter::visit(FunctionDefinition& funcDef){
    if(funcDef.getParamCount() != functionArgs.size()) throw std::runtime_error("Wrong arg count");
    pushContext();
    for(int i = 0; i<int(funcDef.getParamCount()); i++){
        funcDef.declareParam(i, *this);
        assign(lastDeclared, functionArgs[i]);
    }
    funcDef.runStatementBlock(*this);
    popContext();
}

void Interpreter::visit(FunctionCall& funcCall){
    functionArgs.clear();
    for(auto const& arg:funcCall.args){
        arg->accept(*this);
        functionArgs.push_back(result);
    }
    funcs[funcCall.funcName]->accept(*this);
}

void Interpreter::visit(ReturnStatement& returnStatement){
    if(returnStatement.expr) returnStatement.expr->accept(*this);
    else result = {};
    if(contextStack.size() == 1){
        std::visit(overload{
            [](int& res)        {cout << res << '\n';},
            [](double& res)     {cout << res << '\n';},
            [](string& res)     {cout << res << '\n';},
            [](Array& res)      {cout << res.toString() << '\n';},
            [](Hexgrid& res)    {cout << res.toString() << '\n';},
            [](auto&)           {},
        }, result);
    } else {
        returning=true;
    }
}

bool Interpreter::isPosition(Var value){
    if(value.index()!=4 || get<Array>(value).size() != 3) return false;
    auto pos = get<Array>(value);
    return (pos.get(0).index() == 1 && pos.get(1).index() == 1 && pos.get(2).index() == 1);
}

void Interpreter::visit(AddStatement& addStatement){
    addStatement.being_added->accept(*this);
    auto beingAdded = result;
    addStatement.added_to->accept(*this);
    auto addedTo = result;
    if(addedTo.index()!=5) throw std::runtime_error("Can add only to hexgrid\n");
    addStatement.added_at->accept(*this);
    if(!isPosition(result)) throw std::runtime_error("Position must be an array of 3 integers\n");
    auto pos = get<Array>(result);
    auto hexgrid = get<Hexgrid>(addedTo);
    hexgrid.add(pos, beingAdded);
    assign(addStatement.added_to->getName(), hexgrid);
}

void Interpreter::visit(RemoveStatement& removeStatement){
    removeStatement.grid->accept(*this);
    auto grid = result;
    if(grid.index()!=5) throw std::runtime_error("Can remove only from hexgrid\n");
    removeStatement.position->accept(*this);
    if(!isPosition(result)) throw std::runtime_error("Position must be an array of 3 integers\n");
    auto pos = get<Array>(result);
    auto hexgrid = get<Hexgrid>(grid);
    hexgrid.remove(pos);
    assign(removeStatement.grid->getName(), hexgrid);
}

void Interpreter::visit(MoveStatement& moveStatement){
    moveStatement.grid_source->accept(*this);
    auto grid_source = result;
    moveStatement.grid_target->accept(*this);
    auto grid_target = result;
    if(grid_source.index()!=5) throw std::runtime_error("Can only remove from hexgrid\n");
    
    moveStatement.position_source->accept(*this);
    if(!isPosition(result)) throw std::runtime_error("Position must be an array of 3 integers\n");
    auto position_source_arr = get<Array>(result);
    auto hexgrid_source = get<Hexgrid>(grid_source);
    auto value = hexgrid_source.remove(position_source_arr);
    
    if(moveStatement.position_target){
        if(grid_target.index()!=5) throw std::runtime_error("If moving at antoher position, targer must be hexgrid\n");
        moveStatement.position_target->accept(*this);
        auto position_target = result;
        if(!isPosition(position_target)) throw std::runtime_error("Position must be an array of 3 integers\n");
        auto position_target_arr = get<Array>(position_target);
        auto hexgrid_target = get<Hexgrid>(grid_target);
        hexgrid_target.add(position_target_arr, value);
        value = hexgrid_target;
    }
    assign(moveStatement.grid_target->getName(), value);
    assign(moveStatement.grid_source->getName(), hexgrid_source);
}