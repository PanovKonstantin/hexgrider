#include <sstream>
#include <boost/test/unit_test.hpp>
#include <parser/Ast.h>
#include <lexer/Lexer.h>
#include "interpreter/Interpreter.h"
#include "interpreter/StandardScope.h"
using namespace ast;
using namespace lexer;
using namespace parser;
using namespace std;
using namespace intprt;


struct InterpreterTestsFixture
{
    Interpreter interpreter = Interpreter();
    void interpret_text(const std::string& str)
    {
        std::istringstream in(str);
        Parser p(std::make_unique<Lexer>(in));
        p.parse()->accept(interpreter);
    }

    void dumpAst(const std::string & input)
    {
        std::istringstream in(input);
        Parser p(std::make_unique<Lexer>(in));
        cout << p.parse()->toString();

    }
};

BOOST_FIXTURE_TEST_SUITE(InterpreterTests, InterpreterTestsFixture)

BOOST_AUTO_TEST_CASE(interpreter_declare_var)
{
    interpret_text("int x;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
}
BOOST_AUTO_TEST_CASE(interpreter_declare_and_assign_var_int)
{
    interpret_text("int x;\nx=2;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 2);
}
BOOST_AUTO_TEST_CASE(interpreter_init_var_int)
{
    interpret_text("int x=2;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 2);
}
BOOST_AUTO_TEST_CASE(interpreter_declare_var_float)
{
    interpret_text("float x;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
}
BOOST_AUTO_TEST_CASE(interpreter_declare_and_assign_var_float)
{
    interpret_text("float x;\nx=2.;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<double>(interpreter.getValue("x")), double(2));
}
BOOST_AUTO_TEST_CASE(interpreter_init_var_float)
{
    interpret_text("float x=2.;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<double>(interpreter.getValue("x")), double(2));
}
BOOST_AUTO_TEST_CASE(interpreter_declare_var_string)
{
    interpret_text("string x;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
}
BOOST_AUTO_TEST_CASE(interpreter_declare_and_assign_var_string)
{
    interpret_text("string x;\nx=\"Hello\";");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<string>(interpreter.getValue("x")), "Hello");
}
BOOST_AUTO_TEST_CASE(interpreter_init_var_string)
{
    interpret_text("string x=\"Hello\";");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<string>(interpreter.getValue("x")), "Hello");
}
BOOST_AUTO_TEST_CASE(interpreter_declare_var_array)
{
    interpret_text("array x;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
}
BOOST_AUTO_TEST_CASE(interpreter_declare_and_assign_var_array)
{
    interpret_text( "array x;   \n"
                    "x=[1, 2];  ");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    auto ar =get<Array>(interpreter.getValue("x"));
    // BOOST_CHECK_EQUAL(ar.get(0), 1);
    // BOOST_CHECK_EQUAL(ar.get(1), 2);
}
BOOST_AUTO_TEST_CASE(interpreter_init_var_array)
{
    interpret_text("array x=[1, 2.2, \"Hello\", [1]];");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    auto ar =get<Array>(interpreter.getValue("x"));
    BOOST_CHECK_EQUAL(get<int>(ar.get(0)), 1);
    BOOST_CHECK_EQUAL(get<double>(ar.get(1)), 2.2);
    BOOST_CHECK_EQUAL(get<string>(ar.get(2)), "Hello");
    auto subArr = get<Array>(ar.get(3));
    BOOST_CHECK_EQUAL(get<int>(subArr.get(0)), 1);
}
BOOST_AUTO_TEST_CASE(interpreter_init_var_hexgrid)
{
    interpret_text("hexgrid x=<\"blue\" at [0, 0, 0], \"red\" at [0, 1, -1]>;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    auto hex =get<Hexgrid>(interpreter.getValue("x"));
    BOOST_CHECK_EQUAL(get<string>(hex.on(0, 0, 0)), "blue");
    BOOST_CHECK_EQUAL(get<string>(hex.on(0, 1, -1)), "red");
}

BOOST_AUTO_TEST_CASE(interpreter_arithmetical_negation_int)
{
    interpret_text("int x = -1;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), -1);
}

BOOST_AUTO_TEST_CASE(interpreter_arithmetical_negation_float)
{
    interpret_text("float x = -1.12;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<double>(interpreter.getValue("x")), -1.12);
}

BOOST_AUTO_TEST_CASE(interpreter_logical_negation_int)
{
    interpret_text("int x = !1;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 0);
    interpret_text("int x = !0;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 1);
}

BOOST_AUTO_TEST_CASE(interpreter_logical_negation_float)
{
    interpret_text("float x = !1.12;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<double>(interpreter.getValue("x")), 0);
    interpret_text("float x = !0.;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<double>(interpreter.getValue("x")), 1);
}

BOOST_AUTO_TEST_CASE(interpreter_add_expression_int)
{
    interpret_text("int x = 1 + 1;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 2);
}

BOOST_AUTO_TEST_CASE(interpreter_add_expression_float)
{
    interpret_text("float x = 1.0 + 1.9;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<double>(interpreter.getValue("x")), 2.9);
}

BOOST_AUTO_TEST_CASE(interpreter_add_expression_string)
{
    interpret_text("string x = \"a\" + \"b\";");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<string>(interpreter.getValue("x")), "ab");
}


BOOST_AUTO_TEST_CASE(interpreter_subtruct_expression_int)
{
    interpret_text("int x = 1 - 1;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 0);
}

BOOST_AUTO_TEST_CASE(interpreter_subtruct_expression_float)
{
    interpret_text("float x = 1.0 - 1.9;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<double>(interpreter.getValue("x")), double(1-1.9));
}

BOOST_AUTO_TEST_CASE(interpreter_modulo_expression)
{
    interpret_text("int x = 3 % 2;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 1);
}

BOOST_AUTO_TEST_CASE(interpreter_divide_expression_int)
{
    interpret_text("int x = 4 / 2;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 2);
    interpret_text("int x = 3 / 2;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 1);
}

BOOST_AUTO_TEST_CASE(interpreter_divide_expression_float)
{
    interpret_text("float x = 4. / 3;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<double>(interpreter.getValue("x")), double(4./3));
    interpret_text("float x = 3. / 1.5;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<double>(interpreter.getValue("x")), 2);
}

BOOST_AUTO_TEST_CASE(interpreter_multiply_expression_int)
{
    interpret_text("int x = 4 * 2;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 8);
}

BOOST_AUTO_TEST_CASE(interpreter_multiply_expression_float)
{
    interpret_text("float x = 4.5 * 3;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<double>(interpreter.getValue("x")), 13.5);
}

BOOST_AUTO_TEST_CASE(interpreter_variable_reference){
    interpret_text("int x = 5; int y = x + 3;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("y"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("y")), 8);
}


BOOST_AUTO_TEST_CASE(interpreter_indexing_expression)
{
    interpret_text( "array x = [1, 2, 3]; \n"
                    "int y = x[1];");
    BOOST_CHECK_EQUAL(interpreter.containsVar("y"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("y")), 2);
}

BOOST_AUTO_TEST_CASE(interpreter_equal_expression_int)
{
    interpret_text( "int x = 2 == 1 + 1;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 1);
    interpret_text( "int x = 2 == 1 - 1;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 0);
}

BOOST_AUTO_TEST_CASE(interpreter_equal_expression_float)
{
    interpret_text( "int x = 2.3 == 1.2 + 1.1;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 1);
    interpret_text( "int x = 2.3 == 1.2 - 1.112;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 0);
}

BOOST_AUTO_TEST_CASE(interpreter_equal_expression_int_float)
{
    interpret_text( "int x = 3 == 1.4 + 1.6;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 1);
    interpret_text( "int x = 2 == 1.2 - 1.112;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 0);
}


BOOST_AUTO_TEST_CASE(interpreter_equal_expression_float_int)
{
    interpret_text( "int x = 1.4 + 1.6 == 3;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 1);
    interpret_text( "int x = 1.2 - 1.112 == 2;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 0);
}


BOOST_AUTO_TEST_CASE(interpreter_equal_expression_string)
{
    interpret_text( "int x = \"asd\" == \"asd\";");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 1);
    interpret_text( "int x = \"dsa\" == \"asd\";");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 0);
}


BOOST_AUTO_TEST_CASE(interpreter_not_equal_expression_int)
{
    interpret_text( "int x = 1 != 2;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 1);
    interpret_text( "int x = 1 != 2-1;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 0);
}

BOOST_AUTO_TEST_CASE(interpreter_greater_expression_int)
{
    interpret_text( "int x = 1 > 2;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 0);
    interpret_text( "int x = 2 > 1;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 1);
}

BOOST_AUTO_TEST_CASE(interpreter_less_or_equal_expression_int)
{
    interpret_text( "int x = 1 <= 2;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 1);
    interpret_text( "int x = 2 <= 1;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 0);
    interpret_text( "int x = 2 <= 2;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 1);
}

BOOST_AUTO_TEST_CASE(interpreter_less_expression_int)
{
    interpret_text( "int x = 1 < 2;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 1);
    interpret_text( "int x = 2 < 1;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 0);
    interpret_text( "int x = 2 < 2;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 0);
}

BOOST_AUTO_TEST_CASE(interpreter_greater_or_equal_expression_int)
{
    interpret_text( "int x = 1 >= 2;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 0);
    interpret_text( "int x = 2 >= 1;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 1);
    interpret_text( "int x = 2 >= 2;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 1);
}

BOOST_AUTO_TEST_CASE(interpreter_and_expression)
{
    interpret_text( "int x = 1 and 0;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 0);
    interpret_text( "int x = 1 and 1;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 1);
    interpret_text( "int x = 2 and -2;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 1);
}

BOOST_AUTO_TEST_CASE(interpreter_or_expression)
{
    interpret_text( "int x = 1 or 0;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 1);
    interpret_text( "int x = 0 or 0;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 0);
    interpret_text( "int x = 2 or -2;");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 1);
}

BOOST_AUTO_TEST_CASE(interpreter_on_expression_string)
{
    interpret_text( "hexgrid x = <\"asd\" at [2, -1, -1]>; string word = x on [2, -1, -1];");
    BOOST_CHECK_EQUAL(interpreter.containsVar("word"), true);
    BOOST_CHECK_EQUAL(get<string>(interpreter.getValue("word")), "asd");
}

BOOST_AUTO_TEST_CASE(interpreter_on_expression_int)
{
    interpret_text( "hexgrid x = <123 at [2, -1, -1]>; int num = x on [2, -1, -1];");
    BOOST_CHECK_EQUAL(interpreter.containsVar("num"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("num")), 123);
}

BOOST_AUTO_TEST_CASE(interpreter_by_expression)
{
    interpret_text( "hexgrid x = <\"blue\" at [2, -1, -1], \"blue\" at [4, 0, -4]>; \n" 
                    "array positions = x by \"blue\";       \n"
                    "array pos = positions[0];              \n"
                    "int q = pos[0]; int r = pos[1]; int s = pos[2];"
                    "array pos2 = positions[1];              \n"
                    "int q2 = pos2[0]; int r2 = pos2[1]; int s2 = pos2[2];");
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("q")), 2);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("r")), -1);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("s")), -1);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("q2")), 4);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("r2")), 0);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("s2")), -4);
}

BOOST_AUTO_TEST_CASE(interpreter_beside_expression)
{
    interpret_text( "hexgrid x = <\"blue\" at [2, -1, -1], 1 at [2, 0, -2], 10.2 at [3, -2, -1]>; \n" 
                    "array positions = x beside [2, -1, -1];        \n");
    auto array = get<Array>(interpreter.getValue("positions"));
    BOOST_CHECK_EQUAL(array.size(), 2);
    auto pos1 = get<Array>(array.get(0));
    BOOST_CHECK_EQUAL(get<int>(pos1.get(0)), 2);
    BOOST_CHECK_EQUAL(get<int>(pos1.get(1)), 0);
    BOOST_CHECK_EQUAL(get<int>(pos1.get(2)), -2);
    auto pos2 = get<Array>(array.get(1));
    BOOST_CHECK_EQUAL(get<int>(pos2.get(0)), 3);
    BOOST_CHECK_EQUAL(get<int>(pos2.get(1)), -2);
    BOOST_CHECK_EQUAL(get<int>(pos2.get(2)), -1);
}


BOOST_AUTO_TEST_CASE(interpreter_if_statement)
{
    interpret_text( "int x = 1; if (1) {x=2;}");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 2);
}

BOOST_AUTO_TEST_CASE(interpreter_if_statement_scope)
{
    interpret_text( "if (1) {int x=2;}");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), false);
}

BOOST_AUTO_TEST_CASE(interpreter_if_else_statement)
{
    interpret_text( "int x = 1; if (0) {x=2;} else {x=3;}");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 3);
    interpret_text( "int x = 1; if (1) {x=2;} else {x=3;}");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 2);
}
BOOST_AUTO_TEST_CASE(interpreter_if_elif_statement)
{
    interpret_text( "int x = 1; if (0) {x=2;} elif (1) {x=4;}");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 4);
    interpret_text( "int x = 1; if (1) {x=2;} elif (0) {x=4;}");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 2);
}
BOOST_AUTO_TEST_CASE(interpreter_if_elif_elif_statement)
{
    interpret_text( "int x = 1; if (0) {x=2;} elif (0) {x=4;} elif (1) {x=5;}");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 5);
    interpret_text( "int x = 1; if (0) {x=2;} elif (1) {x=4;} elif (0) {x=5;}");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 4);
}
BOOST_AUTO_TEST_CASE(interpreter_if_elif_else_statement)
{
    interpret_text( "int x = 1; if (0) {x=2;} elif (0) {x=4;} else {x=6;}");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 6);
}

BOOST_AUTO_TEST_CASE(interpreter_foreach_statement_int)
{
    interpret_text( "int x = 0; array a = [1, 2, 3]; foreach int y in a { x = x + y; }");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 6);
}

BOOST_AUTO_TEST_CASE(interpreter_foreach_statement_float)
{
    interpret_text( "float x = 0.; array a = [1.5, 2.5, 3.5]; foreach float y in a { x = x + y; }");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<double>(interpreter.getValue("x")), 7.5);
}

BOOST_AUTO_TEST_CASE(interpreter_foreach_statement_mixed)
{
    interpret_text( "int x = 0; array a = [1, \"bad\", 3, 213.5, 5]; foreach int y in a { x = x + y; }");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 9);
}

BOOST_AUTO_TEST_CASE(interpreter_function_definition)
{
    interpret_text( "func int test_func(){}");
    BOOST_CHECK_EQUAL(interpreter.containsFun("test_func"), true);
}

BOOST_AUTO_TEST_CASE(interpreter_function_call)
{
    interpret_text( "int x=2; func int test_func(){x = x + 1;} test_func();");
    BOOST_CHECK_EQUAL(interpreter.containsFun("test_func"), true);
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 3);
}

BOOST_AUTO_TEST_CASE(interpreter_function_call_args)
{
    interpret_text( "int x=0; func int addNums(int a, int b){x = a + b;} addNums(1, 3);");
    BOOST_CHECK_EQUAL(interpreter.containsFun("addNums"), true);
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 4);
}

BOOST_AUTO_TEST_CASE(interpreter_nested_function_call_scopes)
{
    interpret_text( "int x=0; func int a(){int x; b();} func int b(){x=1;} a();");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 1);
}

BOOST_AUTO_TEST_CASE(interpreter_function_call_return)
{
    interpret_text( "func int a(){return 1;} int x=0; x=a();");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 1);
}

BOOST_AUTO_TEST_CASE(interpreter_function_return_assignment_without_return_statement)
{
    interpret_text( "int x=0; func int a(){int asd=213;} x=a();");
    BOOST_CHECK_EQUAL(interpreter.containsVar("x"), true);
    BOOST_CHECK_EQUAL(get<int>(interpreter.getValue("x")), 0);
}

BOOST_AUTO_TEST_CASE(interpreter_add_statement)
{
    interpret_text( "hexgrid a = <\"test\" at [0, 0, 0]>;   \n"
                    "add \"test2\" to a at [1, 0, -1];      \n");
    auto a = get<Hexgrid>(interpreter.getValue("a"));
    auto test2 = a.on(1, 0, -1);
    BOOST_CHECK_EQUAL(get<string>(test2), "test2");
}

bool interpreter_remove_statement_correct_msg(const runtime_error ex){
    BOOST_CHECK_EQUAL(ex.what(), std::string("No such cell"));
    return true;
}
BOOST_AUTO_TEST_CASE(interpreter_remove_statement)
{
        interpret_text("hexgrid a = <\"test\" at [1, -1, 0]>; remove [1, -1, 0] from a;");
        auto a = get<Hexgrid>(interpreter.getValue("a"));
        BOOST_CHECK_EXCEPTION(a.on(1, -1, 0), std::runtime_error,
            interpreter_remove_statement_correct_msg);
}
BOOST_AUTO_TEST_CASE(interpreter_move_statement_hexgrid)
{
        interpret_text( "hexgrid a = <\"test\" at [1, -1, 0]>; \n"
                        "hexgrid b = <>; \n"
                        "move [1, -1, 0] from a to b at [2, 0, -2];");
        auto a = get<Hexgrid>(interpreter.getValue("a"));
        auto b = get<Hexgrid>(interpreter.getValue("b"));
        auto test = b.on(2, 0, -2);
        BOOST_CHECK_EQUAL(get<string>(test), "test");
        BOOST_CHECK_EXCEPTION(a.on(1, -1, 0), std::runtime_error,
            interpreter_remove_statement_correct_msg);
}



BOOST_AUTO_TEST_SUITE_END()
