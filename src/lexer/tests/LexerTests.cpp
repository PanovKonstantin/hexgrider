#include "lexer/Lexer.h"
#include <sstream>
#include <boost/test/unit_test.hpp>
using namespace lexer;
using namespace token;

BOOST_AUTO_TEST_SUITE(LexerTests)

// Eof token

BOOST_AUTO_TEST_CASE(lexer_reads_token_before_first_advance_returns_eof)
{
  std::istringstream in;
  Lexer l(in);
  BOOST_CHECK_EQUAL(l.getToken().getType(), Token::Type::EndOfFile);
}

BOOST_AUTO_TEST_CASE(lexer_reads_empty_input_returns_eof)
{
  std::istringstream in("");
  Lexer l(in);
  l.readNextToken();
  BOOST_CHECK_EQUAL(l.getToken().getType(), Token::Type::EndOfFile);
}

// Integer token

BOOST_AUTO_TEST_CASE(lexer_reads_int_token)
{
  std::istringstream in("123");
  Lexer l(in);

  l.readNextToken();
  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Integer);
  BOOST_CHECK_EQUAL(t.getInteger(), 123);
}

// Decimal token

BOOST_AUTO_TEST_CASE(lexer_reads_dec_token)
{
  std::istringstream in("123.321");
  Lexer l(in);

  l.readNextToken();
  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Decimal);
  BOOST_CHECK_EQUAL(t.getDecimal(), 123.321);
}

BOOST_AUTO_TEST_CASE(lexer_reads_dec_token_with_leading_zeros_in_fractal_part)
{
  std::istringstream in("0.0001");
  Lexer l(in);

  l.readNextToken();
  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Decimal);
  BOOST_CHECK_EQUAL(t.getDecimal(), 0.0001);
}

// Text token

BOOST_AUTO_TEST_CASE(lexer_reads_text_token)
{
  std::istringstream in("\"Hello, World!\"");
  Lexer l(in);

  l.readNextToken();
  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Text);
  BOOST_CHECK_EQUAL(t.getText(), "Hello, World!");
}

BOOST_AUTO_TEST_CASE(lexer_reads_text_token_with_digits)
{
  std::istringstream in("\"Hello, World 1234!\"");
  Lexer l(in);

  l.readNextToken();
  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Text);
  BOOST_CHECK_EQUAL(t.getText(), "Hello, World 1234!");
}


BOOST_AUTO_TEST_CASE(lexer_throws_exception_on_bad_end_text_token)
{
  std::istringstream in("\"Hello, World!");
  Lexer l(in);
  BOOST_CHECK_THROW(l.readNextToken();, std::invalid_argument );
}

BOOST_AUTO_TEST_CASE(lexer_reads_text_token_with_escaped_quotes)
{
  std::istringstream in("\"Hello, \\\"World\\\"\"");
  Lexer l(in);

  l.readNextToken();
  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Text);
  BOOST_CHECK_EQUAL(t.getText(), "Hello, \"World\"");
}

BOOST_AUTO_TEST_CASE(lexer_throws_exception_on_bad_end_after_bad_end_text_token)
{
  std::istringstream in("\"Hello, World!\\");
  Lexer l(in);
  BOOST_CHECK_THROW(l.readNextToken();, std::invalid_argument );
}

// Identifier_token

BOOST_AUTO_TEST_CASE(lexer_reads_identifier_token)
{
  std::istringstream in("foo_2");
  Lexer l(in);

  l.readNextToken();
  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Identifier);
  BOOST_CHECK_EQUAL(t.getText(), "foo_2");
}

BOOST_AUTO_TEST_CASE(lexer_throws_exception_on_forbidden_chars_in_identifier_token)
{
  std::istringstream in("&dsa");
  Lexer l(in);
  BOOST_CHECK_THROW(l.readNextToken();, std::runtime_error );
}

// Keyword token

BOOST_AUTO_TEST_CASE(lexer_reads_keyword_token_1)
{
  std::istringstream in("foreach");
  Lexer l(in);

  l.readNextToken();
  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Keyword);
  BOOST_CHECK_EQUAL(t.getText(), "foreach");
}

BOOST_AUTO_TEST_CASE(lexer_reads_keyword_token_2)
{
  std::istringstream in("elif");
  Lexer l(in);

  l.readNextToken();
  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Keyword);
  BOOST_CHECK_EQUAL(t.getText(), "elif");
}

// Operator token

BOOST_AUTO_TEST_CASE(lexer_reads_alpha_operator_token)
{
  std::istringstream in("beside");
  Lexer l(in);
  l.readNextToken();
  const auto t = l.getToken();
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Operator);
  BOOST_CHECK_EQUAL(t.getText(), "beside");
}

BOOST_AUTO_TEST_CASE(lexer_reads_sign_one_char_operator_token)
{
  std::istringstream in("=");
  Lexer l(in);
  l.readNextToken();
  const auto t = l.getToken();
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Operator);
  BOOST_CHECK_EQUAL(t.getText(), "=");
}

BOOST_AUTO_TEST_CASE(lexer_reads_sign_two_char_operator_token)
{
  std::istringstream in("==");
  Lexer l(in);
  l.readNextToken();
  const auto t = l.getToken();
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Operator);
  BOOST_CHECK_EQUAL(t.getText(), "==");
}

// Type token

BOOST_AUTO_TEST_CASE(lexer_reads_type_operator_token)
{
  std::istringstream in("int");
  Lexer l(in);
  l.readNextToken();
  const auto t = l.getToken();
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Type);
  BOOST_CHECK_EQUAL(t.getText(), "int");
}


// Other

BOOST_AUTO_TEST_CASE(lexer_returns_eof_after_last_token)
{
  std::istringstream in("123");
  Lexer l(in);

  l.readNextToken();
  l.readNextToken();

  BOOST_CHECK_EQUAL(l.getToken().getType(), Token::Type::EndOfFile);
}

BOOST_AUTO_TEST_CASE(lexer_ignores_whitespaces)
{
  std::istringstream in("  \t\n  \t \n  \n\t \r \t \v 123");
  Lexer l(in);

  l.readNextToken();
  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Integer);
  BOOST_CHECK_EQUAL(t.getInteger(), 123);
}

BOOST_AUTO_TEST_CASE(lexer_throws_on_unknown_token)
{
  std::istringstream in("#");
  Lexer l(in);
  BOOST_CHECK_THROW(l.readNextToken(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(lexer_read_multiple_tokens)
{
  std::istringstream in("int foo= 321");
  Lexer l(in);
  l.readNextToken();
  BOOST_CHECK_EQUAL(l.getToken().getType(), Token::Type::Type);
  BOOST_CHECK_EQUAL(l.getToken().getText(), "int");
  l.readNextToken();
  BOOST_CHECK_EQUAL(l.getToken().getType(), Token::Type::Identifier);
  BOOST_CHECK_EQUAL(l.getToken().getText(), "foo");
  l.readNextToken();
  BOOST_CHECK_EQUAL(l.getToken().getType(), Token::Type::Operator);
  BOOST_CHECK_EQUAL(l.getToken().getText(), "=");
  l.readNextToken();
  BOOST_CHECK_EQUAL(l.getToken().getType(), Token::Type::Integer);
  BOOST_CHECK_EQUAL(l.getToken().getInteger(), 321);
  l.readNextToken();
  BOOST_CHECK_EQUAL(l.getToken().getType(), Token::Type::EndOfFile);
}



BOOST_AUTO_TEST_SUITE_END()
