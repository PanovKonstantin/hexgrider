//#include "ast/"
// #include <sstream>
#include <boost/test/unit_test.hpp>
/* using namespace parser;
using namespace lexer;
using namespace token; */

BOOST_AUTO_TEST_SUITE(AstTests)

BOOST_AUTO_TEST_CASE(ast_empty_test)
{
  BOOST_CHECK_EQUAL(1 + 1, 2);
}


BOOST_AUTO_TEST_SUITE_END()
