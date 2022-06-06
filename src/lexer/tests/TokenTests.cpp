#include "../Token.h"
#include "HexgridErrors.h"

#include <sstream>

#include <boost/test/unit_test.hpp>

using namespace token;


BOOST_AUTO_TEST_SUITE(TokenTests)

// EOF token

BOOST_AUTO_TEST_CASE(eof_token_by_default)
{
  Token t;
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::EndOfFile);
}

BOOST_AUTO_TEST_CASE(eof_token_prints_correctly)
{
  Token t;
  BOOST_CHECK_EQUAL(t.toString(), "End of file");
}

// Integer token

BOOST_AUTO_TEST_CASE(int_token_has_correct_type)
{
  Token t(Token::Type::Integer, 100);
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Integer);
}

BOOST_AUTO_TEST_CASE(int_token_doesnt_construct_with_incorrect_type)
{
  BOOST_CHECK_THROW(Token t(Token::Type::Decimal, 100), hexgrid_errors::WrongTokenConstructor );
}

BOOST_AUTO_TEST_CASE(int_token_has_correct_value)
{
  Token t(Token::Type::Integer, 100);
  BOOST_CHECK_EQUAL(t.getInteger(), 100);
}

BOOST_AUTO_TEST_CASE(int_token_prints_correctly)
{
  Token t(Token::Type::Integer, 100);
  BOOST_CHECK_EQUAL(t.toString(), "Integer (100)");
}

// Decimal token

BOOST_AUTO_TEST_CASE(dec_token_has_correct_type)
{
  Token t(Token::Type::Decimal, 100.001);
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Decimal);
}

BOOST_AUTO_TEST_CASE(dec_token_doesnt_construct_with_incorrect_type)
{
  BOOST_CHECK_THROW(Token t(Token::Type::Integer, 100.001), hexgrid_errors::WrongTokenConstructor);
}

BOOST_AUTO_TEST_CASE(dec_token_has_correct_value)
{
  Token t(Token::Type::Decimal, 100.001);
  BOOST_CHECK_EQUAL(t.getDecimal(), 100.001);
}

BOOST_AUTO_TEST_CASE(dec_token_prints_correctly)
{
  Token t(Token::Type::Decimal, 100.001);
  BOOST_CHECK_EQUAL(t.toString(), "Decimal (" + std::to_string(double(100.001)) + ")");
}

// Text token

BOOST_AUTO_TEST_CASE(text_token_has_correct_type)
{
  Token t(Token::Type::Text, "Hello 2 u!");
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Text);
}

BOOST_AUTO_TEST_CASE(text_token_doesnt_construct_with_incorrect_type)
{
  BOOST_CHECK_THROW(Token t(Token::Type::Decimal, "Hello 2 u!"), hexgrid_errors::WrongTokenConstructor );
}


BOOST_AUTO_TEST_CASE(text_token_has_correct_value)
{
  Token t(Token::Type::Text, "Hello 2 u!");
  BOOST_CHECK_EQUAL(t.getText(), "Hello 2 u!");
}

BOOST_AUTO_TEST_CASE(text_token_prints_correctly)
{
  Token t(Token::Type::Text, "Hello 2 u!");
  BOOST_CHECK_EQUAL(t.toString(), "Text (Hello 2 u!)");
}

// Operator token

BOOST_AUTO_TEST_CASE(operator_token_has_correct_type)
{
  Token t(Token::Type::MultiplyOperator);
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::MultiplyOperator);
}
BOOST_AUTO_TEST_CASE(operator_token_doesnt_construct_with_incorrect_type)
{
  BOOST_CHECK_THROW(Token t(Token::Type::Decimal, "*"), hexgrid_errors::WrongTokenConstructor );
}

BOOST_AUTO_TEST_CASE(operator_token_prints_correctly)
{
  Token t(Token::Type::MultiplyOperator);
  BOOST_CHECK_EQUAL(t.toString(), "\"multiply\" operator");
}

// Keyword token

BOOST_AUTO_TEST_CASE(keyword_token_has_correct_type)
{
  Token t(Token::Type::FuncKeyword);
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::FuncKeyword);
}
BOOST_AUTO_TEST_CASE(keyword_token_doesnt_construct_with_incorrect_type)
{
  BOOST_CHECK_THROW(Token t(Token::Type::Decimal, "*"), hexgrid_errors::WrongTokenConstructor );
}

BOOST_AUTO_TEST_CASE(keyword_token_prints_correctly)
{
  Token t(Token::Type::ForeachKeyword);
  BOOST_CHECK_EQUAL(t.toString(), "\"foreach\" keyword");
}

// Identifier token

BOOST_AUTO_TEST_CASE(identifier_token_has_correct_type)
{
  Token t(Token::Type::Identifier, "foo");
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Identifier);
}
BOOST_AUTO_TEST_CASE(identifier_token_doesnt_construct_with_incorrect_type)
{
  BOOST_CHECK_THROW(Token t(Token::Type::Decimal, "*"), hexgrid_errors::WrongTokenConstructor);
}

BOOST_AUTO_TEST_CASE(identifier_token_has_correct_value)
{
  Token t(Token::Type::Identifier, "foo");
  BOOST_CHECK_EQUAL(t.getText(), "foo");
}

BOOST_AUTO_TEST_CASE(identifier_token_prints_correctly)
{
  Token t(Token::Type::Identifier, "foo");
  BOOST_CHECK_EQUAL(t.toString(), "Identifier (foo)");
}
// Type token

BOOST_AUTO_TEST_CASE(type_token_has_correct_type)
{
  Token t(Token::Type::IntType);
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::IntType);
}
BOOST_AUTO_TEST_CASE(type_token_doesnt_construct_with_incorrect_type)
{
  BOOST_CHECK_THROW(Token t(Token::Type::Decimal, "int"), hexgrid_errors::WrongTokenConstructor );
}

BOOST_AUTO_TEST_CASE(type_token_prints_correctly)
{
  Token t(Token::Type::IntType);
  BOOST_CHECK_EQUAL(t.toString(), "\"int\" type");
}
BOOST_AUTO_TEST_SUITE_END()
