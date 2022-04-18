#include <iostream>
#include <sstream>
#include "lexer/Lexer.h"
#include "token/Token.h"

using namespace lexer;
using namespace token;

int main()
{
  Lexer l(std::cin);
  Token t;
  do
  {
    l.readNextToken();
    t = l.getToken();
    std::cout << t << " ";
  } while(t.getType() != Token::Type::EndOfFile);
  return 0;
}
