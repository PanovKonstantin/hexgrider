#include <sstream>
#include <boost/test/unit_test.hpp>
#include "ast/Ast.h"
#include "ast/Interpreter.h"
#include "ast/StandardScope.h"
using namespace ast;
using namespace std;

BOOST_AUTO_TEST_SUITE(InterpreterTests)

BOOST_AUTO_TEST_CASE(interpreter_declare_var)
{/* 
    auto id = "hello";
    auto dst = DeclarationStatement::Type::Int;
    unique_ptr<Node> ds = make_unique<DeclarationStatement>(dst, id);
    auto statements = vector<unique_ptr<Node>>();
    statements.push_back(move(ds));
    auto scope = make_unique<Scope>(move(statements));
    scope->calculate(standard_scope);
    // auto vars = scope.variables.
    BOOST_CHECK_EQUAL(scope->variables.count("hello"), 1);
    auto var = scope->variables["hello"];
    auto t = var.getType();

    BOOST_CHECK(t == ast::Variable::Type::Int); */
}


BOOST_AUTO_TEST_SUITE_END()
