#include "HexgridErrors.h"
#include "lexer/Lexer.h"
#include <sstream>
#include <utility>
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
  BOOST_CHECK_EQUAL(l.getToken().getType(), Token::Type::EndOfFile);
}

// Integer token

BOOST_AUTO_TEST_CASE(lexer_reads_int_token)
{
  std::istringstream in("123");
  Lexer l(in);

  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Integer);
  BOOST_CHECK_EQUAL(t.getInteger(), 123);
}

BOOST_AUTO_TEST_CASE(lexer_reads_double_zero_as_one_token)
{
  std::istringstream in("00123");
  Lexer l(in);

  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Integer);
  BOOST_CHECK_EQUAL(t.getInteger(), 123);
  BOOST_CHECK_EQUAL(l.getToken().getType(), Token::Type::EndOfFile);
}

BOOST_AUTO_TEST_CASE(lexer_catches_integer_overflow)
{
  std::istringstream in("21474836471");
  Lexer l(in);
  BOOST_CHECK_THROW(l.getToken(), hexgrid_errors::IntegerOverflow);
}

BOOST_AUTO_TEST_CASE(lexer_parses_very_big_int)
{
  std::istringstream in("2147483647");
  Lexer l(in);
  const auto t = l.getToken();
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Integer);
  BOOST_CHECK_EQUAL(t.getInteger(), 2147483647);
}

// Decimal token

BOOST_AUTO_TEST_CASE(lexer_reads_dec_token)
{
  std::istringstream in("123.321");
  Lexer l(in);

  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Decimal);
  BOOST_CHECK_EQUAL(t.getDecimal(), 123.321);
}

BOOST_AUTO_TEST_CASE(lexer_reads_dec_token_with_only_dot)
{
  std::istringstream in("123.");
  Lexer l(in);

  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Decimal);
  BOOST_CHECK_EQUAL(t.getDecimal(), 123);
}

BOOST_AUTO_TEST_CASE(lexer_reads_dec_token_with_leading_zeros_in_fractal_part)
{
  std::istringstream in("0.0001");
  Lexer l(in);

  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Decimal);
  BOOST_CHECK_EQUAL(t.getDecimal(), 0.0001);
  BOOST_CHECK_EQUAL(l.getToken().getType(), Token::Type::EndOfFile);
}

BOOST_AUTO_TEST_CASE(lexer_reads_dec_token_with_multi_leading_zeros_in_fractal_part)
{
  std::istringstream in("00000.0001");
  Lexer l(in);

  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Decimal);
  BOOST_CHECK_EQUAL(t.getDecimal(), 0.0001);
}

BOOST_AUTO_TEST_CASE(lexer_parses_decimal_over_integer_size)
{
  std::istringstream in("123.2147483647112321");
  Lexer l(in);
  const auto t = l.getToken();
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Decimal);
  BOOST_CHECK_EQUAL(t.getDecimal(), double(123.2147483647112321));
}

// Text token

BOOST_AUTO_TEST_CASE(lexer_reads_text_token)
{
  std::istringstream in("\"Hello, World!\"");
  Lexer l(in);

  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Text);
  BOOST_CHECK_EQUAL(t.getText(), "Hello, World!");
}

BOOST_AUTO_TEST_CASE(lexer_reads_text_token_with_digits)
{
  std::istringstream in("\"Hello, World 1234!\"");
  Lexer l(in);

  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Text);
  BOOST_CHECK_EQUAL(t.getText(), "Hello, World 1234!");
}


BOOST_AUTO_TEST_CASE(lexer_throws_exception_on_bad_end_text_token)
{
  std::istringstream in("\"Hello, World!");
  Lexer l(in);
  BOOST_CHECK_THROW(l.getToken();, hexgrid_errors::OpenQuotes);
}

BOOST_AUTO_TEST_CASE(lexer_reads_text_token_with_escaped_quotes)
{
  std::istringstream in("\"Hello, \\\"World\\\"\"");
  Lexer l(in);

  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Text);
  BOOST_CHECK_EQUAL(t.getText(), "Hello, \"World\"");
}

BOOST_AUTO_TEST_CASE(lexer_reads_text_token_with_new_line)
{
  std::istringstream in("\"Hello,\\nWorld\"");
  Lexer l(in);

  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Text);
  BOOST_CHECK_EQUAL(t.getText(), "Hello,\nWorld");
}

BOOST_AUTO_TEST_CASE(lexer_throws_on_not_closed_string)
{
  std::istringstream in("\"abc\\");
  Lexer l(in);
  BOOST_CHECK_THROW(l.getToken(), hexgrid_errors::OpenQuotes);
}

BOOST_AUTO_TEST_CASE(lexer_throws_exception_on_bad_end_after_escape_token)
{
  std::istringstream in("\"Hello, World!\\");
  Lexer l(in);
  BOOST_CHECK_THROW(l.getToken(), hexgrid_errors::OpenQuotes);
}

// Identifier_token

BOOST_AUTO_TEST_CASE(lexer_reads_identifier_token)
{
  std::istringstream in("foo_2");
  Lexer l(in);

  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Identifier);
  BOOST_CHECK_EQUAL(t.getText(), "foo_2");
}

bool correctUnkownCharacterExceptionMessage1(const hexgrid_errors::UnkownCharacterException& ex){
    BOOST_CHECK_EQUAL(std::string(ex.what()), "(1, 1) Unkown character \"&\"");
    return true;
}

BOOST_AUTO_TEST_CASE(lexer_throws_exception_on_forbidden_chars_in_identifier_token)
{
  std::istringstream in("&dsa");
  Lexer l(in);
  BOOST_CHECK_EXCEPTION(l.getToken(), 
    hexgrid_errors::UnkownCharacterException,
    correctUnkownCharacterExceptionMessage1
    );
}

// Keyword token

BOOST_AUTO_TEST_CASE(lexer_reads_keyword_token_1)
{
  std::istringstream in("foreach");
  Lexer l(in);

  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::ForeachKeyword);
  BOOST_CHECK_EQUAL(t.getText(), "");
}

BOOST_AUTO_TEST_CASE(lexer_reads_keyword_token_2)
{
  std::istringstream in("elif");
  Lexer l(in);

  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::ElifKeyword);
  BOOST_CHECK_EQUAL(t.getText(), "");
}

// Operator token

BOOST_AUTO_TEST_CASE(lexer_reads_alpha_operator_token)
{
  std::istringstream in("beside");
  Lexer l(in);
  const auto t = l.getToken();
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::BesideOperator);
  BOOST_CHECK_EQUAL(t.getText(), "");
}

BOOST_AUTO_TEST_CASE(lexer_reads_sign_one_char_operator_token)
{
  std::istringstream in("=");
  Lexer l(in);
  const auto t = l.getToken();
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::AssignOperator);
  BOOST_CHECK_EQUAL(t.getText(), "");
}

BOOST_AUTO_TEST_CASE(lexer_reads_sign_two_char_operator_token)
{
  std::istringstream in("==");
  Lexer l(in);
  const auto t = l.getToken();
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::EqualOperator);
  BOOST_CHECK_EQUAL(t.getText(), "");
}

BOOST_AUTO_TEST_CASE(lexer_reads_less_operator_token)
{
  std::istringstream in("<");
  Lexer l(in);
  const auto t = l.getToken();
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::LessOperator);
  BOOST_CHECK_EQUAL(t.getText(), "");
}

// Type token

BOOST_AUTO_TEST_CASE(lexer_reads_type_operator_token)
{
  std::istringstream in("int");
  Lexer l(in);
  const auto t = l.getToken();
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::IntType);
  BOOST_CHECK_EQUAL(t.getText(), "");
}


// Other

BOOST_AUTO_TEST_CASE(lexer_returns_eof_after_last_token)
{
  std::istringstream in("123");
  Lexer l(in);

  l.getToken();

  BOOST_CHECK_EQUAL(l.getToken().getType(), Token::Type::EndOfFile);
}

BOOST_AUTO_TEST_CASE(lexer_ignores_whitespaces)
{
  std::istringstream in("  \t\n  \t \n  \n\t \r \t \v 123");
  Lexer l(in);

  const auto t = l.getToken();

  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Integer);
  BOOST_CHECK_EQUAL(t.getInteger(), 123);
}

bool correctUnkownCharacterExceptionMessage2(const hexgrid_errors::UnkownCharacterException& ex){
    BOOST_CHECK_EQUAL(std::string(ex.what()), "(1, 1) Unkown character \"#\"");
    return true;
}
BOOST_AUTO_TEST_CASE(lexer_throws_on_unknown_token)
{
  std::istringstream in("#");
  Lexer l(in);
  BOOST_CHECK_EXCEPTION(l.getToken(), 
    hexgrid_errors::UnkownCharacterException,
    correctUnkownCharacterExceptionMessage2
    );
}

BOOST_AUTO_TEST_CASE(lexer_read_multiple_tokens)
{
  std::istringstream in("int foo= 321");
  Lexer l(in);
  auto t = l.getToken();
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::IntType);
  BOOST_CHECK_EQUAL(t.getText(), "");
  t = l.getToken();
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Identifier);
  BOOST_CHECK_EQUAL(t.getText(), "foo");
  t = l.getToken();
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::AssignOperator);
  BOOST_CHECK_EQUAL(t.getText(), "");
  t = l.getToken();
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::Integer);
  BOOST_CHECK_EQUAL(t.getInteger(), 321);
  t = l.getToken();
  BOOST_CHECK_EQUAL(t.getType(), Token::Type::EndOfFile);
}

//Token Location

BOOST_AUTO_TEST_CASE(lexer_reads_eof_location_correctly){
  std::istringstream in("\n123");
  Lexer l(in);
  auto t = l.getToken();
  t = l.getToken();
  BOOST_CHECK_EQUAL(t.getStart().first, 2);
  BOOST_CHECK_EQUAL(t.getStart().second, 4);
  BOOST_CHECK_EQUAL(t.getEnd().first, 2);
  BOOST_CHECK_EQUAL(t.getEnd().second, 4);
  
}

BOOST_AUTO_TEST_CASE(lexer_reads_int_token_location_correctly)
{
  std::istringstream in("123 321\n 123");
  Lexer l(in);
  auto t = l.getToken();
  BOOST_CHECK_EQUAL(t.getStart().first, 1);
  BOOST_CHECK_EQUAL(t.getStart().second, 1);
  BOOST_CHECK_EQUAL(t.getEnd().first, 1);
  BOOST_CHECK_EQUAL(t.getEnd().second, 4);
  t = l.getToken();
  BOOST_CHECK_EQUAL(t.getStart().first, 1);
  BOOST_CHECK_EQUAL(t.getStart().second, 5);
  BOOST_CHECK_EQUAL(t.getEnd().first, 1);
  BOOST_CHECK_EQUAL(t.getEnd().second, 8);
  t = l.getToken();
  BOOST_CHECK_EQUAL(t.getStart().first, 2);
  BOOST_CHECK_EQUAL(t.getStart().second, 2);
  BOOST_CHECK_EQUAL(t.getEnd().first, 2);
  BOOST_CHECK_EQUAL(t.getEnd().second, 5);
}

BOOST_AUTO_TEST_CASE(lexer_reads_dec_token_location_correctly)
{
  std::istringstream in("123.321 \n 123.22");
  Lexer l(in);
  auto t = l.getToken();
  BOOST_CHECK_EQUAL(t.getStart().first, 1);
  BOOST_CHECK_EQUAL(t.getStart().second, 1);
  BOOST_CHECK_EQUAL(t.getEnd().first, 1);
  BOOST_CHECK_EQUAL(t.getEnd().second, 8);
  t = l.getToken();
  BOOST_CHECK_EQUAL(t.getStart().first, 2);
  BOOST_CHECK_EQUAL(t.getStart().second, 2);
  BOOST_CHECK_EQUAL(t.getEnd().first, 2);
  BOOST_CHECK_EQUAL(t.getEnd().second, 8);
}

BOOST_AUTO_TEST_CASE(lexer_reads_text_token_location_correctly)
{
  std::istringstream in(" \"hello\"\n \"123.22\"");
  Lexer l(in);
  auto t = l.getToken();
  BOOST_CHECK_EQUAL(t.getStart().first, 1);
  BOOST_CHECK_EQUAL(t.getStart().second, 2);
  BOOST_CHECK_EQUAL(t.getEnd().first, 1);
  BOOST_CHECK_EQUAL(t.getEnd().second, 9);
  t = l.getToken();
  BOOST_CHECK_EQUAL(t.getStart().first, 2);
  BOOST_CHECK_EQUAL(t.getStart().second, 2);
  BOOST_CHECK_EQUAL(t.getEnd().first, 2);
  BOOST_CHECK_EQUAL(t.getEnd().second, 10);
}

BOOST_AUTO_TEST_CASE(lexer_reads_alpha_num_token_location_correctly)
{
  std::istringstream in("int\nfunc foo3");
  Lexer l(in);
  auto t = l.getToken();
  BOOST_CHECK_EQUAL(t.getStart().first, 1);
  BOOST_CHECK_EQUAL(t.getStart().second, 1);
  BOOST_CHECK_EQUAL(t.getEnd().first, 1);
  BOOST_CHECK_EQUAL(t.getEnd().second, 4);
  t = l.getToken();
  BOOST_CHECK_EQUAL(t.getStart().first, 2);
  BOOST_CHECK_EQUAL(t.getStart().second, 1);
  BOOST_CHECK_EQUAL(t.getEnd().first, 2);
  BOOST_CHECK_EQUAL(t.getEnd().second, 5);
  t = l.getToken();
  BOOST_CHECK_EQUAL(t.getStart().first, 2);
  BOOST_CHECK_EQUAL(t.getStart().second, 6);
  BOOST_CHECK_EQUAL(t.getEnd().first, 2);
  BOOST_CHECK_EQUAL(t.getEnd().second, 10);
}

BOOST_AUTO_TEST_CASE(lexer_reads_sign_token_location_correctly)
{
  std::istringstream in("==\n{ \n } +");
  Lexer l(in);
  auto t = l.getToken();
  BOOST_CHECK_EQUAL(t.getStart().first, 1);
  BOOST_CHECK_EQUAL(t.getStart().second, 1);
  BOOST_CHECK_EQUAL(t.getEnd().first, 1);
  BOOST_CHECK_EQUAL(t.getEnd().second, 3);
  t = l.getToken();
  BOOST_CHECK_EQUAL(t.getStart().first, 2);
  BOOST_CHECK_EQUAL(t.getStart().second, 1);
  BOOST_CHECK_EQUAL(t.getEnd().first, 2);
  BOOST_CHECK_EQUAL(t.getEnd().second, 2);
  t = l.getToken();
  BOOST_CHECK_EQUAL(t.getStart().first, 3);
  BOOST_CHECK_EQUAL(t.getStart().second, 2);
  BOOST_CHECK_EQUAL(t.getEnd().first, 3);
  BOOST_CHECK_EQUAL(t.getEnd().second, 3);
  t = l.getToken();
  BOOST_CHECK_EQUAL(t.getStart().first, 3);
  BOOST_CHECK_EQUAL(t.getStart().second, 4);
  BOOST_CHECK_EQUAL(t.getEnd().first, 3);
  BOOST_CHECK_EQUAL(t.getEnd().second, 5);
}

BOOST_AUTO_TEST_SUITE_END()
