#include "parser/Parser.h"
#include <lexer/Lexer.h>
#include <sstream>
#include <boost/test/unit_test.hpp>
using namespace ast;
using namespace parser;
using namespace lexer;
using namespace token;

struct ParserTestsFixture
{
    std::unique_ptr<Node> result;
    void parse(const std::string& str)
    {
        std::istringstream in(str);
        Parser p(std::make_unique<Lexer>(in));
        result = p.parse();
    }
    void parseExpression(const std::string& str)
    {
        std::istringstream in(str);
        Parser p(std::make_unique<Lexer>(in));
        p.advance();
        result = p.readExpression();
    }
};


BOOST_FIXTURE_TEST_SUITE(ParserTests, ParserTestsFixture)


BOOST_AUTO_TEST_CASE(empty_input_parses_to_empty_script)
{

}

BOOST_AUTO_TEST_CASE(reads_expression_with_integer_literal)
{
    parseExpression("123");
    BOOST_CHECK_EQUAL(result->toString(), "Integer Literal (123)\n");
}

BOOST_AUTO_TEST_CASE(reads_expression_with_decimal_literal)
{
    parseExpression("123.321");
    BOOST_CHECK_EQUAL(result->toString(), "Decimal Literal (" + std::to_string(double(123.321)) + ")\n");
}

BOOST_AUTO_TEST_CASE(reads_expression_with_text_literal)
{
    parseExpression("\"Hello, World\"");
    BOOST_CHECK_EQUAL(result->toString(), "Text Literal (Hello, World)\n");
}

BOOST_AUTO_TEST_CASE(reads_expression_with_identifier)
{
    parseExpression("foo");
    BOOST_CHECK_EQUAL(result->toString(), "Identifier (foo)\n");
}

BOOST_AUTO_TEST_CASE(reads_expression_with_function_call_without_args)
{
    parseExpression("foo()");
    BOOST_CHECK_EQUAL(result->toString(), "Function Call\n"
                                          "|Identifier (foo)\n");
}

BOOST_AUTO_TEST_CASE(reads_expression_with_function_call_with_args)
{
    parseExpression("foo(a, b)");
    BOOST_CHECK_EQUAL(result->toString(), "Function Call\n"
                                          "|Identifier (foo)\n"
                                          "|Identifier (a)\n"
                                          "|Identifier (b)\n");
}


BOOST_AUTO_TEST_CASE(reads_expression_with_empty_array)
{
    parseExpression("[]");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Array of length (0)\n");
}

BOOST_AUTO_TEST_CASE(reads_expression_with_array_1)
{
    parseExpression("[10]");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Array of length (1)\n"
                      "|Integer Literal (10)\n");
}

BOOST_AUTO_TEST_CASE(reads_expression_with_hexgrid)
{
    parseExpression("<\"blue\" at [0, 0, 0], \"red\" at [0, 0, 1], \"blue\" at [0, 0, -1]>");
    BOOST_CHECK_EQUAL(result->toString(), "Hexgrid\n"
                                          "|Hexgrid Cell\n"
                                          "||Text Literal (blue)\n"
                                          "||Array of length (3)\n"
                                          "|||Integer Literal (0)\n"
                                          "|||Integer Literal (0)\n"
                                          "|||Integer Literal (0)\n"
                                          "|Hexgrid Cell\n"
                                          "||Text Literal (red)\n"
                                          "||Array of length (3)\n"
                                          "|||Integer Literal (0)\n"
                                          "|||Integer Literal (0)\n"
                                          "|||Integer Literal (1)\n"
                                          "|Hexgrid Cell\n"
                                          "||Text Literal (blue)\n"
                                          "||Array of length (3)\n"
                                          "|||Integer Literal (0)\n"
                                          "|||Integer Literal (0)\n"
                                          "|||Arithmetical Negation Expression\n"
                                          "||||Integer Literal (1)\n");
}

BOOST_AUTO_TEST_CASE(reads_expression_with_array_2)
{
    parseExpression("[1, 2, 3, 4]");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Array of length (4)\n"
                      "|Integer Literal (1)\n"
                      "|Integer Literal (2)\n"
                      "|Integer Literal (3)\n"
                      "|Integer Literal (4)\n");
}

BOOST_AUTO_TEST_CASE(reads_indexing_expression)
{
    parseExpression("foo[0]");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Indexing Expression\n"
                      "|Identifier (foo)\n"
                      "|Integer Literal (0)\n");
    
}

BOOST_AUTO_TEST_CASE(reads_multiple_indexing_expression)
{
    parseExpression("foo[0][1][2]");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Indexing Expression\n"
                      "|Indexing Expression\n"
                      "||Indexing Expression\n"
                      "|||Identifier (foo)\n"
                      "|||Integer Literal (0)\n"
                      "||Integer Literal (1)\n"
                      "|Integer Literal (2)\n");
}

BOOST_AUTO_TEST_CASE(reads_logical_negation_expression)
{
    parseExpression("!0");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Logical Negation Expression\n"
                      "|Integer Literal (0)\n");
    
}

BOOST_AUTO_TEST_CASE(reads_logical_negation_with_identifier_expression)
{
    parseExpression("! is_something");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Logical Negation Expression\n"
                      "|Identifier (is_something)\n");
    
}

BOOST_AUTO_TEST_CASE(reads_arithmetical_negation_expression)
{
    parseExpression("-123");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Arithmetical Negation Expression\n"
                      "|Integer Literal (123)\n");
    
}

BOOST_AUTO_TEST_CASE(reads_first_order_arithmetical_expression_multiply)
{
    parseExpression("10 * 3");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Multiply Expression\n"
                      "|Integer Literal (10)\n"
                      "|Integer Literal (3)\n");
}

BOOST_AUTO_TEST_CASE(reads_first_order_arithmetical_expression_devide)
{
    parseExpression("10 / 3");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Divide Expression\n"
                      "|Integer Literal (10)\n"
                      "|Integer Literal (3)\n");
}


BOOST_AUTO_TEST_CASE(reads_multiple_first_order_arithmetical_expression)
{
    parseExpression("10.20 * 3 / 2 * 5");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Multiply Expression\n"
                      "|Divide Expression\n"
                      "||Multiply Expression\n"
                      "|||Decimal Literal (" + std::to_string(double(10.20)) + ")\n"
                      "|||Integer Literal (3)\n"
                      "||Integer Literal (2)\n"
                      "|Integer Literal (5)\n");
}

BOOST_AUTO_TEST_CASE(reads_first_order_arithmetical_expression_with_negation)
{
    parseExpression("10.20 * -3");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Multiply Expression\n"
                      "|Decimal Literal (" + std::to_string(double(10.20)) + ")\n"
                      "|Arithmetical Negation Expression\n"
                      "||Integer Literal (3)\n");
}

BOOST_AUTO_TEST_CASE(reads_second_order_arithmetical_expression_add)
{
    parseExpression("10 + 3");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Add Expression\n"
                      "|Integer Literal (10)\n"
                      "|Integer Literal (3)\n");
}

BOOST_AUTO_TEST_CASE(reads_second_order_arithmetical_expression_substruct)
{
    parseExpression("10 - 3");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Substruct Expression\n"
                      "|Integer Literal (10)\n"
                      "|Integer Literal (3)\n");
}

BOOST_AUTO_TEST_CASE(reads_multiple_second_order_arithmetical_expression)
{
    parseExpression("2 + 10 - 3 - 1");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Substruct Expression\n"
                      "|Substruct Expression\n"
                      "||Add Expression\n"
                      "|||Integer Literal (2)\n"
                      "|||Integer Literal (10)\n"
                      "||Integer Literal (3)\n"
                      "|Integer Literal (1)\n");
}

BOOST_AUTO_TEST_CASE(reads_hexgrid_expression_on)
{
    parseExpression("my_hexgrid on [1, 2, 3]");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Hexgrid On Expression\n"
                      "|Identifier (my_hexgrid)\n"
                      "|Array of length (3)\n"
                      "||Integer Literal (1)\n"
                      "||Integer Literal (2)\n"
                      "||Integer Literal (3)\n");
}

BOOST_AUTO_TEST_CASE(reads_hexgrid_expression_by)
{
    parseExpression("my_hexgrid by \"cell name\"");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Hexgrid By Expression\n"
                      "|Identifier (my_hexgrid)\n"
                      "|Text Literal (cell name)\n");
}

BOOST_AUTO_TEST_CASE(reads_hexgrid_expression_beside)
{
    parseExpression("my_hexgrid beside [3, 3, 3]");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Hexgrid Beside Expression\n"
                      "|Identifier (my_hexgrid)\n"
                      "|Array of length (3)\n"
                      "||Integer Literal (3)\n"
                      "||Integer Literal (3)\n"
                      "||Integer Literal (3)\n");
}

BOOST_AUTO_TEST_CASE(reads_comparison_expression_less)
{
    parseExpression("10 < 3");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Less Expression\n"
                      "|Integer Literal (10)\n"
                      "|Integer Literal (3)\n");
}

BOOST_AUTO_TEST_CASE(reads_comparison_expression_less_or_equalt)
{
    parseExpression("11 <= 3");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "LessOrEqual Expression\n"
                      "|Integer Literal (11)\n"
                      "|Integer Literal (3)\n");
}

BOOST_AUTO_TEST_CASE(reads_comparison_expression_greater_or_equal)
{
    parseExpression("11 >= 3");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "GreaterOrEqual Expression\n"
                      "|Integer Literal (11)\n"
                      "|Integer Literal (3)\n");
}

BOOST_AUTO_TEST_CASE(reads_comparison_expression_greater)
{
    parseExpression("11 > 3");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Greater Expression\n"
                      "|Integer Literal (11)\n"
                      "|Integer Literal (3)\n");
}

BOOST_AUTO_TEST_CASE(reads_comparison_expression_equal)
{
    parseExpression("11 == 3");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Equal Expression\n"
                      "|Integer Literal (11)\n"
                      "|Integer Literal (3)\n");
}

BOOST_AUTO_TEST_CASE(reads_comparison_expression_not_equal)
{
    parseExpression("11 != 3");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "NotEqual Expression\n"
                      "|Integer Literal (11)\n"
                      "|Integer Literal (3)\n");
}

BOOST_AUTO_TEST_CASE(reads_and_expression)
{
    parseExpression("0 and 1");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "And Expression\n"
                      "|Integer Literal (0)\n"
                      "|Integer Literal (1)\n");
}

BOOST_AUTO_TEST_CASE(reads_or_expression)
{
    parseExpression("0 or 1");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Or Expression\n"
                      "|Integer Literal (0)\n"
                      "|Integer Literal (1)\n");
}

BOOST_AUTO_TEST_CASE(reads_nested_expression_1)
{
    parseExpression("(1 + 3) * 2");
    BOOST_CHECK_EQUAL(result->toString(),
                      "Multiply Expression\n"
                      "|Add Expression\n"
                      "||Integer Literal (1)\n"
                      "||Integer Literal (3)\n"
                      "|Integer Literal (2)\n");
}

BOOST_AUTO_TEST_CASE(reads_nested_expression_2)
{
    parseExpression("2 * (1 + 3)");
    BOOST_CHECK_EQUAL(result->toString(),
                      "Multiply Expression\n"
                      "|Integer Literal (2)\n"
                      "|Add Expression\n"
                      "||Integer Literal (1)\n"
                      "||Integer Literal (3)\n");
}

BOOST_AUTO_TEST_CASE(reads_full_expression_tree)
{
    // This expression does not have to be correct nor compile.
    // It just creates a tree of every epression type. 
    parseExpression("0 or 1 and \"word\" == hexgrid_id by 3 + 4 * -!0[3]");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Or Expression\n"
                      "|Integer Literal (0)\n"
                      "|And Expression\n"
                      "||Integer Literal (1)\n"
                      "||Equal Expression\n"
                      "|||Text Literal (word)\n"
                      "|||Hexgrid By Expression\n"
                      "||||Identifier (hexgrid_id)\n"
                      "||||Add Expression\n"
                      "|||||Integer Literal (3)\n"
                      "|||||Multiply Expression\n"
                      "||||||Integer Literal (4)\n"
                      "||||||Arithmetical Negation Expression\n"
                      "|||||||Logical Negation Expression\n"
                      "||||||||Indexing Expression\n"
                      "|||||||||Integer Literal (0)\n"
                      "|||||||||Integer Literal (3)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_variable_declaration)
{
    parse("int foo;");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Scope\n"
                      "|Declaration\n"
                      "||Variable Type (int)\n"
                      "||Identifier (foo)\n");
}
BOOST_AUTO_TEST_CASE(reads_script_with_array_variable_declaration)
{
    parse("int[][] foo;");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Scope\n"
                      "|Declaration\n"
                      "||Variable Type (2d array of type int)\n"
                      "||Identifier (foo)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_variable_init)
{
    parse("int foo = 30;");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Scope\n"
                      "|Assignment\n"
                      "||Declaration\n"
                      "|||Variable Type (int)\n"
                      "|||Identifier (foo)\n"
                      "||Integer Literal (30)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_assignment)
{
    parse("foo = 30;");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Scope\n"
                      "|Assignment\n"
                      "||Identifier (foo)\n"
                      "||Integer Literal (30)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_function_call_without_args)
{
    parse("foo();");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Scope\n"
                      "|Function Call\n"
                      "||Identifier (foo)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_function_call_with_args)
{
    parse("foo(1, 2.30, \"Hello\");");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Scope\n"
                      "|Function Call\n"
                      "||Identifier (foo)\n"
                      "||Integer Literal (1)\n"
                      "||Decimal Literal (" + std::to_string(double(2.30)) + ")\n"
                      "||Text Literal (Hello)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_if_statement)
{
    parse("if (1 > 0) { int foo; }");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Scope\n"
                      "|If Statement\n"
                      "||Condition Block\n"
                      "|||Greater Expression\n"
                      "||||Integer Literal (1)\n"
                      "||||Integer Literal (0)\n"
                      "|||Scope\n"
                      "||||Declaration\n"
                      "|||||Variable Type (int)\n"
                      "|||||Identifier (foo)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_if_else_statement)
{
    parse("if (1 > 0) { int foo; }"
          "else {int boo; }");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Scope\n"
                      "|If Statement\n"
                      "||Condition Block\n"
                      "|||Greater Expression\n"
                      "||||Integer Literal (1)\n"
                      "||||Integer Literal (0)\n"
                      "|||Scope\n"
                      "||||Declaration\n"
                      "|||||Variable Type (int)\n"
                      "|||||Identifier (foo)\n"
                      "||Scope\n"
                      "|||Declaration\n"
                      "||||Variable Type (int)\n"
                      "||||Identifier (boo)\n");
}
BOOST_AUTO_TEST_CASE(reads_script_with_if_elif_statement)
{
    parse("if (x > 5) { int foo; }"
          "elif (x < 0 ) {int boo; }");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Scope\n"
                      "|If Statement\n"
                      "||Condition Block\n"
                      "|||Greater Expression\n"
                      "||||Identifier (x)\n"
                      "||||Integer Literal (5)\n"
                      "|||Scope\n"
                      "||||Declaration\n"
                      "|||||Variable Type (int)\n"
                      "|||||Identifier (foo)\n"
                      "||Condition Block\n"
                      "|||Less Expression\n"
                      "||||Identifier (x)\n"
                      "||||Integer Literal (0)\n"
                      "|||Scope\n"
                      "||||Declaration\n"
                      "|||||Variable Type (int)\n"
                      "|||||Identifier (boo)\n");
}
BOOST_AUTO_TEST_CASE(reads_script_with_if_elif_elif_statement)
{
    parse("if (x > 5) { int foo; }"
          "elif (x < 0 ) {int boo; }"
          "elif (x < 3 ) {int goo; }");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Scope\n"
                      "|If Statement\n"
                      "||Condition Block\n"
                      "|||Greater Expression\n"
                      "||||Identifier (x)\n"
                      "||||Integer Literal (5)\n"
                      "|||Scope\n"
                      "||||Declaration\n"
                      "|||||Variable Type (int)\n"
                      "|||||Identifier (foo)\n"
                      "||Condition Block\n"
                      "|||Less Expression\n"
                      "||||Identifier (x)\n"
                      "||||Integer Literal (0)\n"
                      "|||Scope\n"
                      "||||Declaration\n"
                      "|||||Variable Type (int)\n"
                      "|||||Identifier (boo)\n"
                      "||Condition Block\n"
                      "|||Less Expression\n"
                      "||||Identifier (x)\n"
                      "||||Integer Literal (3)\n"
                      "|||Scope\n"
                      "||||Declaration\n"
                      "|||||Variable Type (int)\n"
                      "|||||Identifier (goo)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_foreach_statement)
{
    parse("foreach int i in [0, 1, 2] { sum = sum + i; }");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Scope\n"
                      "|Foreach Statement\n"
                      "||Declaration\n"
                      "|||Variable Type (int)\n"
                      "|||Identifier (i)\n"
                      "||Array of length (3)\n"
                      "|||Integer Literal (0)\n"
                      "|||Integer Literal (1)\n"
                      "|||Integer Literal (2)\n"
                      "||Scope\n"
                      "|||Assignment\n"
                      "||||Identifier (sum)\n"
                      "||||Add Expression\n"
                      "|||||Identifier (sum)\n"
                      "|||||Identifier (i)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_return_statement)
{
    parse("return i * 3;");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Scope\n"
                      "|Return Statement\n"
                      "||Multiply Expression\n"
                      "|||Identifier (i)\n"
                      "|||Integer Literal (3)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_add_statement)
{
    parse("add cell_name to hexgrid_name at pos;");
    BOOST_CHECK_EQUAL(result->toString(),
                      "Scope\n"
                      "|Add Statement\n"
                      "||Identifier (cell_name)\n"
                      "||Identifier (hexgrid_name)\n"
                      "||Identifier (pos)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_remove_statement)
{
    parse("remove pos from hexgrid_name;");
    BOOST_CHECK_EQUAL(result->toString(),
                      "Scope\n"
                      "|Remove Statement\n"
                      "||Identifier (pos)\n"
                      "||Identifier (hexgrid_name)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_move_statement)
{
    parse("move pos1 from hexgrid_name1 to hexgrid2 at pos2;");
    BOOST_CHECK_EQUAL(result->toString(),
                      "Scope\n"
                      "|Move Statement\n"
                      "||Identifier (pos1)\n"
                      "||Identifier (hexgrid_name1)\n"
                      "||Identifier (hexgrid2)\n"
                      "||Identifier (pos2)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_function_definition)
{
    parse("func int sum(int a, int b) {return a + b;}");
    BOOST_CHECK_EQUAL(result->toString(),
                      "Scope\n"
                      "|Function Defenition\n"
                      "||Declaration\n"
                      "|||Variable Type (int)\n"
                      "|||Identifier (sum)\n"
                      "||Declaration\n"
                      "|||Variable Type (int)\n"
                      "|||Identifier (a)\n"
                      "||Declaration\n"
                      "|||Variable Type (int)\n"
                      "|||Identifier (b)\n"
                      "||Scope\n"
                      "|||Return Statement\n"
                      "||||Add Expression\n"
                      "|||||Identifier (a)\n"
                      "|||||Identifier (b)\n");
}


BOOST_AUTO_TEST_CASE(reads_example1)
{
    parse("hexgrid example_hexgrid = <\"blue\"   at [0, 0, 0],  "
          "                           \"red\"    at [0, 0, 1],  "
          "                           \"blue\"   at [0, 0, -1], "
          "                           \"red\"    at [0, 1, 0],  "
          "                           \"yellow\" at [0, -1, 0], "
          "                           \"red\"    at [1, 0, 0],  "
          "                           \"blue\"   at [-1, 0, 0]>;"
          "int blue_count = 0;                                  "
          "foreach int[] pos in example_hexgrid beside [0, 0, 0]"
          "{                                                    "
          "    if (example_hexgrid on pos == \"blue\")          "
          "    {                                                "
          "        blue_count = blue_count + 1;                 "
          "    }                                                "
          "}                                                    "
        );
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Scope\n"
                      "|Assignment\n"
                      "||Declaration\n"
                      "|||Variable Type (hexgrid)\n"
                      "|||Identifier (example_hexgrid)\n"
                      "||Hexgrid\n"
                      "|||Hexgrid Cell\n"
                      "||||Text Literal (blue)\n"
                      "||||Array of length (3)\n"
                      "|||||Integer Literal (0)\n"
                      "|||||Integer Literal (0)\n"
                      "|||||Integer Literal (0)\n"
                      "|||Hexgrid Cell\n"
                      "||||Text Literal (red)\n"
                      "||||Array of length (3)\n"
                      "|||||Integer Literal (0)\n"
                      "|||||Integer Literal (0)\n"
                      "|||||Integer Literal (1)\n"
                      "|||Hexgrid Cell\n"
                      "||||Text Literal (blue)\n"
                      "||||Array of length (3)\n"
                      "|||||Integer Literal (0)\n"
                      "|||||Integer Literal (0)\n"
                      "|||||Arithmetical Negation Expression\n"
                      "||||||Integer Literal (1)\n"
                      "|||Hexgrid Cell\n"
                      "||||Text Literal (red)\n"
                      "||||Array of length (3)\n"
                      "|||||Integer Literal (0)\n"
                      "|||||Integer Literal (1)\n"
                      "|||||Integer Literal (0)\n"
                      "|||Hexgrid Cell\n"
                      "||||Text Literal (yellow)\n"
                      "||||Array of length (3)\n"
                      "|||||Integer Literal (0)\n"
                      "|||||Arithmetical Negation Expression\n"
                      "||||||Integer Literal (1)\n"
                      "|||||Integer Literal (0)\n"
                      "|||Hexgrid Cell\n"
                      "||||Text Literal (red)\n"
                      "||||Array of length (3)\n"
                      "|||||Integer Literal (1)\n"
                      "|||||Integer Literal (0)\n"
                      "|||||Integer Literal (0)\n"
                      "|||Hexgrid Cell\n"
                      "||||Text Literal (blue)\n"
                      "||||Array of length (3)\n"
                      "|||||Arithmetical Negation Expression\n"
                      "||||||Integer Literal (1)\n"
                      "|||||Integer Literal (0)\n"
                      "|||||Integer Literal (0)\n"
                      "|Assignment\n"
                      "||Declaration\n"
                      "|||Variable Type (int)\n"
                      "|||Identifier (blue_count)\n"
                      "||Integer Literal (0)\n"
                      "|Foreach Statement\n"
                      "||Declaration\n"
                      "|||Variable Type (1d array of type int)\n"
                      "|||Identifier (pos)\n"
                      "||Hexgrid Beside Expression\n"
                      "|||Identifier (example_hexgrid)\n"
                      "|||Array of length (3)\n"
                      "||||Integer Literal (0)\n"
                      "||||Integer Literal (0)\n"
                      "||||Integer Literal (0)\n"
                      "||Scope\n"
                      "|||If Statement\n"
                      "||||Condition Block\n"
                      "|||||Equal Expression\n"
                      "||||||Hexgrid On Expression\n"
                      "|||||||Identifier (example_hexgrid)\n"
                      "|||||||Identifier (pos)\n"
                      "||||||Text Literal (blue)\n"
                      "|||||Scope\n"
                      "||||||Assignment\n"
                      "|||||||Identifier (blue_count)\n"
                      "|||||||Add Expression\n"
                      "||||||||Identifier (blue_count)\n"
                      "||||||||Integer Literal (1)\n");
}

BOOST_AUTO_TEST_CASE(reads_example2)
{
    parse(  "hexgrid example_hexgrid = < \"blue\"   at [0, 0, 0],"
                                        "\"red\"    at [0, 0, 1],"
                                        "\"blue\"   at [0, 0, -1],"
                                        "\"red\"    at [0, 1, 0],"
                                        "\"yellow\" at [0, -1, 0],"
                                        "\"red\"    at [1, 0, 0],"
                                        "\"blue\"   at [-1, 0, 0]>;"
            "foreach int[] pos in example_hexgrid by \"blue\""
            "{"
            "    if (pos[0] > 0) {remove pos from example_hexgrid;}"
            "}"
        );
    BOOST_CHECK_EQUAL(result->toString(),
                        "Scope\n"
                        "|Assignment\n"
                        "||Declaration\n"
                        "|||Variable Type (hexgrid)\n"
                        "|||Identifier (example_hexgrid)\n"
                        "||Hexgrid\n"
                        "|||Hexgrid Cell\n"
                        "||||Text Literal (blue)\n"
                        "||||Array of length (3)\n"
                        "|||||Integer Literal (0)\n"
                        "|||||Integer Literal (0)\n"
                        "|||||Integer Literal (0)\n"
                        "|||Hexgrid Cell\n"
                        "||||Text Literal (red)\n"
                        "||||Array of length (3)\n"
                        "|||||Integer Literal (0)\n"
                        "|||||Integer Literal (0)\n"
                        "|||||Integer Literal (1)\n"
                        "|||Hexgrid Cell\n"
                        "||||Text Literal (blue)\n"
                        "||||Array of length (3)\n"
                        "|||||Integer Literal (0)\n"
                        "|||||Integer Literal (0)\n"
                        "|||||Arithmetical Negation Expression\n"
                        "||||||Integer Literal (1)\n"
                        "|||Hexgrid Cell\n"
                        "||||Text Literal (red)\n"
                        "||||Array of length (3)\n"
                        "|||||Integer Literal (0)\n"
                        "|||||Integer Literal (1)\n"
                        "|||||Integer Literal (0)\n"
                        "|||Hexgrid Cell\n"
                        "||||Text Literal (yellow)\n"
                        "||||Array of length (3)\n"
                        "|||||Integer Literal (0)\n"
                        "|||||Arithmetical Negation Expression\n"
                        "||||||Integer Literal (1)\n"
                        "|||||Integer Literal (0)\n"
                        "|||Hexgrid Cell\n"
                        "||||Text Literal (red)\n"
                        "||||Array of length (3)\n"
                        "|||||Integer Literal (1)\n"
                        "|||||Integer Literal (0)\n"
                        "|||||Integer Literal (0)\n"
                        "|||Hexgrid Cell\n"
                        "||||Text Literal (blue)\n"
                        "||||Array of length (3)\n"
                        "|||||Arithmetical Negation Expression\n"
                        "||||||Integer Literal (1)\n"
                        "|||||Integer Literal (0)\n"
                        "|||||Integer Literal (0)\n"
                        "|Foreach Statement\n"
                        "||Declaration\n"
                        "|||Variable Type (1d array of type int)\n"
                        "|||Identifier (pos)\n"
                        "||Hexgrid By Expression\n"
                        "|||Identifier (example_hexgrid)\n"
                        "|||Text Literal (blue)\n"
                        "||Scope\n"
                        "|||If Statement\n"
                        "||||Condition Block\n"
                        "|||||Greater Expression\n"
                        "||||||Indexing Expression\n"
                        "|||||||Identifier (pos)\n"
                        "|||||||Integer Literal (0)\n"
                        "||||||Integer Literal (0)\n"
                        "|||||Scope\n"
                        "||||||Remove Statement\n"
                        "|||||||Identifier (pos)\n"
                        "|||||||Identifier (example_hexgrid)\n"
);
}


BOOST_AUTO_TEST_SUITE_END()
