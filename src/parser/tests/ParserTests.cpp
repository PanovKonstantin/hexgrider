#include "HexgridErrors.h"
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
    class TestParser : public Parser{
        public:
            using Parser::Parser;
            std::unique_ptr<Node> parseExpression(){
                advance();
                return readExpression();
            }
    };

    std::unique_ptr<Node> result;
    void parse(const std::string& str)
    {
        std::istringstream in(str);
        TestParser p(std::make_unique<Lexer>(in));
        result = p.parse();
    }
    void parseExpression(const std::string& str)
    {
        std::istringstream in(str);
        TestParser p(std::make_unique<Lexer>(in));
        result = p.parseExpression();
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
    parseExpression("boo");
    BOOST_CHECK_EQUAL(result->toString(), "Variable reference (boo)\n");
}

BOOST_AUTO_TEST_CASE(reads_expression_with_function_call_without_args)
{
    parseExpression("foo()");
    BOOST_CHECK_EQUAL(result->toString(), "Function Call (foo)\n");
}

BOOST_AUTO_TEST_CASE(reads_expression_with_function_call_with_args)
{
    parseExpression("foo(a, b)");
    BOOST_CHECK_EQUAL(result->toString(), "Function Call (foo)\n"
                                          "|Variable reference (a)\n"
                                          "|Variable reference (b)\n");
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
                      "|Variable reference (foo)\n"
                      "|Integer Literal (0)\n");
    
}

BOOST_AUTO_TEST_CASE(reads_multiple_indexing_expression)
{
    parseExpression("foo[0][1][2]");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Indexing Expression\n"
                      "|Indexing Expression\n"
                      "||Indexing Expression\n"
                      "|||Variable reference (foo)\n"
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
                      "|Variable reference (is_something)\n");
    
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

BOOST_AUTO_TEST_CASE(reads_first_order_arithmetical_expression_modulo)
{
    parseExpression("10 % 3");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Modulo Expression\n"
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
                      "|Variable reference (my_hexgrid)\n"
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
                      "|Variable reference (my_hexgrid)\n"
                      "|Text Literal (cell name)\n");
}

BOOST_AUTO_TEST_CASE(reads_hexgrid_expression_beside)
{
    parseExpression("my_hexgrid beside [3, 3, 3]");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Hexgrid Beside Expression\n"
                      "|Variable reference (my_hexgrid)\n"
                      "|Array of length (3)\n"
                      "||Integer Literal (3)\n"
                      "||Integer Literal (3)\n"
                      "||Integer Literal (3)\n");
}

BOOST_AUTO_TEST_CASE(reads_hexgrid_from_right_to_left)
{
    parseExpression("my_hexgrid beside my_hexgrid by \"red\"");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Hexgrid Beside Expression\n"
                      "|Variable reference (my_hexgrid)\n" 
                      "|Hexgrid By Expression\n"
                      "||Variable reference (my_hexgrid)\n"
                      "||Text Literal (red)\n");
}


BOOST_AUTO_TEST_CASE(reads_comparison_expression_less)
{
    parseExpression("10 < 3");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Less Expression\n"
                      "|Integer Literal (10)\n"
                      "|Integer Literal (3)\n");
}

BOOST_AUTO_TEST_CASE(reads_comparison_expression_less_or_equal)
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
                      "||||Variable reference (hexgrid_id)\n"
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
                      "Program\n"
                      "|Variable Declaration (int foo)\n");
}
BOOST_AUTO_TEST_CASE(reads_script_with_array_variable_declaration)
{
    parse("array foo;");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Program\n"
                      "|Variable Declaration (array foo)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_variable_init)
{
    parse("int foo = 30;");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Program\n"
                      "|Initialization (int foo)\n"
                      "||Integer Literal (30)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_assignment)
{
    parse("foo = 30;");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Program\n"
                      "|Assignment (foo)\n"
                      "||Integer Literal (30)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_function_call_without_args)
{
    parse("foo();");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Program\n"
                      "|Function Call (foo)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_function_call_with_args)
{
    parse("foo(1, 2.30, \"Hello\");");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Program\n"
                      "|Function Call (foo)\n"
                      "||Integer Literal (1)\n"
                      "||Decimal Literal (" + std::to_string(double(2.30)) + ")\n"
                      "||Text Literal (Hello)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_if_statement)
{
    parse("if (1 > 0) { int foo; }");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Program\n"
                      "|If Statement\n"
                      "||Condition Block\n"
                      "|||Greater Expression\n"
                      "||||Integer Literal (1)\n"
                      "||||Integer Literal (0)\n"
                      "|||StatementBlock\n"
                      "||||Variable Declaration (int foo)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_if_else_statement)
{
    parse("if (1 > 0) { int foo; }\n"
          "else {int boo; }");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Program\n"
                      "|If Statement\n"
                      "||Condition Block\n"
                      "|||Greater Expression\n"
                      "||||Integer Literal (1)\n"
                      "||||Integer Literal (0)\n"
                      "|||StatementBlock\n"
                      "||||Variable Declaration (int foo)\n"
                      "||StatementBlock\n"
                      "|||Variable Declaration (int boo)\n");
}
BOOST_AUTO_TEST_CASE(reads_script_with_if_elif_statement)
{
    parse("if (x > 5) { int foo; }\n"
          "elif (x < 0 ) {int boo; }\n");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Program\n"
                      "|If Statement\n"
                      "||Condition Block\n"
                      "|||Greater Expression\n"
                      "||||Variable reference (x)\n"
                      "||||Integer Literal (5)\n"
                      "|||StatementBlock\n"
                      "||||Variable Declaration (int foo)\n"
                      "||Condition Block\n"
                      "|||Less Expression\n"
                      "||||Variable reference (x)\n"
                      "||||Integer Literal (0)\n"
                      "|||StatementBlock\n"
                      "||||Variable Declaration (int boo)\n");
}
BOOST_AUTO_TEST_CASE(reads_script_with_if_elif_elif_statement)
{
    parse("if (x > 5) { int foo; }\n"
          "elif (x < 0 ) {int boo; }\n"
          "elif (x < 3 ) {int goo; }\n");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Program\n"
                      "|If Statement\n"
                      "||Condition Block\n"
                      "|||Greater Expression\n"
                      "||||Variable reference (x)\n"
                      "||||Integer Literal (5)\n"
                      "|||StatementBlock\n"
                      "||||Variable Declaration (int foo)\n"
                      "||Condition Block\n"
                      "|||Less Expression\n"
                      "||||Variable reference (x)\n"
                      "||||Integer Literal (0)\n"
                      "|||StatementBlock\n"
                      "||||Variable Declaration (int boo)\n"
                      "||Condition Block\n"
                      "|||Less Expression\n"
                      "||||Variable reference (x)\n"
                      "||||Integer Literal (3)\n"
                      "|||StatementBlock\n"
                      "||||Variable Declaration (int goo)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_foreach_statement)
{
    parse("foreach int i in [0, 1, 2] { sum = sum + i; }");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Program\n"
                      "|Foreach Statement\n"
                      "||Variable Declaration (int i)\n"
                      "||Array of length (3)\n"
                      "|||Integer Literal (0)\n"
                      "|||Integer Literal (1)\n"
                      "|||Integer Literal (2)\n"
                      "||StatementBlock\n"
                      "|||Assignment (sum)\n"
                      "||||Add Expression\n"
                      "|||||Variable reference (sum)\n"
                      "|||||Variable reference (i)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_return_statement)
{
    parse("return i * 3;");
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Program\n"
                      "|Return Statement\n"
                      "||Multiply Expression\n"
                      "|||Variable reference (i)\n"
                      "|||Integer Literal (3)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_add_statement)
{
    parse("add cell_name to hexgrid_name at pos;");
    BOOST_CHECK_EQUAL(result->toString(),
                      "Program\n"
                      "|Add Statement\n"
                      "||Variable reference (cell_name)\n"
                      "||Variable reference (hexgrid_name)\n"
                      "||Variable reference (pos)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_remove_statement)
{
    parse("remove pos from hexgrid_name;");
    BOOST_CHECK_EQUAL(result->toString(),
                      "Program\n"
                      "|Remove Statement\n"
                      "||Variable reference (pos)\n"
                      "||Variable reference (hexgrid_name)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_move_statement)
{
    parse("move pos1 from hexgrid_name1 to hexgrid2 at pos2;");
    BOOST_CHECK_EQUAL(result->toString(),
                      "Program\n"
                      "|Move Statement\n"
                      "||Variable reference (pos1)\n"
                      "||Variable reference (hexgrid_name1)\n"
                      "||Variable reference (hexgrid2)\n"
                      "||Variable reference (pos2)\n");
}

BOOST_AUTO_TEST_CASE(reads_script_with_function_definition)
{
    parse("func int sum(int a, int b) {return a + b;}");
    BOOST_CHECK_EQUAL(result->toString(),
                      "Program\n"
                      "|Function Defenition (int sum)\n"
                      "||Variable Declaration (int a)\n"
                      "||Variable Declaration (int b)\n"
                      "||StatementBlock\n"
                      "|||Return Statement\n"
                      "||||Add Expression\n"
                      "|||||Variable reference (a)\n"
                      "|||||Variable reference (b)\n");
}


BOOST_AUTO_TEST_CASE(reads_example1)
{
    parse("hexgrid example_hexgrid = <\"blue\"   at [0, 0, 0],  \n"
          "                           \"red\"    at [0, 0, 1],  \n"
          "                           \"blue\"   at [0, 0, -1], \n"
          "                           \"red\"    at [0, 1, 0],  \n"
          "                           \"yellow\" at [0, -1, 0], \n"
          "                           \"red\"    at [1, 0, 0],  \n"
          "                           \"blue\"   at [-1, 0, 0]>;\n"
          "int blue_count = 0;                                  \n"
          "foreach array pos in example_hexgrid beside [0, 0, 0]\n"
          "{                                                    \n"
          "    if (example_hexgrid on pos == \"blue\")          \n"
          "    {                                                \n"
          "        blue_count = blue_count + 1;                 \n"
          "    }                                                \n"
          "}                                                    \n"
        );
    BOOST_CHECK_EQUAL(result->toString(), 
                      "Program\n"
                      "|Initialization (hexgrid example_hexgrid)\n"
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
                      "|Initialization (int blue_count)\n"
                      "||Integer Literal (0)\n"
                      "|Foreach Statement\n"
                      "||Variable Declaration (array pos)\n"
                      "||Hexgrid Beside Expression\n"
                      "|||Variable reference (example_hexgrid)\n"
                      "|||Array of length (3)\n"
                      "||||Integer Literal (0)\n"
                      "||||Integer Literal (0)\n"
                      "||||Integer Literal (0)\n"
                      "||StatementBlock\n"
                      "|||If Statement\n"
                      "||||Condition Block\n"
                      "|||||Equal Expression\n"
                      "||||||Hexgrid On Expression\n"
                      "|||||||Variable reference (example_hexgrid)\n"
                      "|||||||Variable reference (pos)\n"
                      "||||||Text Literal (blue)\n"
                      "|||||StatementBlock\n"
                      "||||||Assignment (blue_count)\n"
                      "|||||||Add Expression\n"
                      "||||||||Variable reference (blue_count)\n"
                      "||||||||Integer Literal (1)\n");
}

BOOST_AUTO_TEST_CASE(reads_example2)
{
    parse(  "hexgrid example_hexgrid = < \"blue\"   at [0, 0, 0],   \n"
                                        "\"red\"    at [0, 0, 1],   \n"
                                        "\"blue\"   at [0, 0, -1],  \n"
                                        "\"red\"    at [0, 1, 0],   \n"
                                        "\"yellow\" at [0, -1, 0],  \n"
                                        "\"red\"    at [1, 0, 0],   \n"
                                        "\"blue\"   at [-1, 0, 0]>; \n"
            "foreach array pos in example_hexgrid by \"blue\"       \n"
            "{                                                      \n"
            "    if (pos[0] > 0) {remove pos from example_hexgrid;} \n"
            "}                                                      \n"
        );
    BOOST_CHECK_EQUAL(result->toString(),
                        "Program\n"
                        "|Initialization (hexgrid example_hexgrid)\n"
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
                        "||Variable Declaration (array pos)\n"
                        "||Hexgrid By Expression\n"
                        "|||Variable reference (example_hexgrid)\n"
                        "|||Text Literal (blue)\n"
                        "||StatementBlock\n"
                        "|||If Statement\n"
                        "||||Condition Block\n"
                        "|||||Greater Expression\n"
                        "||||||Indexing Expression\n"
                        "|||||||Variable reference (pos)\n"
                        "|||||||Integer Literal (0)\n"
                        "||||||Integer Literal (0)\n"
                        "|||||StatementBlock\n"
                        "||||||Remove Statement\n"
                        "|||||||Variable reference (pos)\n"
                        "|||||||Variable reference (example_hexgrid)\n"
);
}
bool throws_on_unfinished_function_def_correct_msg(const hexgrid_errors::UnexpectedInput& ex){
    BOOST_CHECK_EQUAL(ex.what(), std::string("(1, 5) Recieved unexpected input: End of file, expected a return value declaration\n"));
    return true;
}
BOOST_AUTO_TEST_CASE(throws_on_unfinished_function_def)
{
    BOOST_CHECK_EXCEPTION(
        parse("func"),
        hexgrid_errors::UnexpectedInput,
        throws_on_unfinished_function_def_correct_msg);
}

bool throws_on_unfinished_init_correct_msg(const hexgrid_errors::UnexpectedInput& ex){
    BOOST_CHECK_EQUAL(ex.what(), std::string("(1, 8) Recieved unexpected input: End of file, expected a variable or a value\n"));
    return true;
}
BOOST_AUTO_TEST_CASE(throws_on_unfinished_init)
{
    BOOST_CHECK_EXCEPTION(
        parse("int f ="),
        hexgrid_errors::UnexpectedInput,
        throws_on_unfinished_init_correct_msg);
}
bool throws_on_function_redefinition_correct_msg(const hexgrid_errors::FunctionRedefinition& ex){
    BOOST_CHECK_EQUAL(ex.what(), std::string("From (2, 1) to (2, 17) Redefinition of a function: foo\n"));
    return true;
}
BOOST_AUTO_TEST_CASE(throws_on_function_redefinition)
{
    BOOST_CHECK_EXCEPTION(
        parse("func int foo(){}\nfunc int foo(){}"),
        hexgrid_errors::FunctionRedefinition,
        throws_on_function_redefinition_correct_msg);
}




BOOST_AUTO_TEST_SUITE_END()
