#include <iostream>
#include <sstream>
#include "lexer/Lexer.h"
#include "ast/IntegerLiteral.h"
#include "parser/Parser.h"
/*

using namespace token;
using namespace parser; */

using namespace lexer;
using namespace parser;
using namespace ast;

namespace {
  
std::unique_ptr<Node> readAndParseStdin()
{
  Parser p(std::make_unique<Lexer>(std::cin));
  return p.parse();
}
}


int main()
{
  std::cout << readAndParseStdin()->toString();
/*     Token t;
  do
  {
    l.readNextToken();
    t = l.getToken();
    std::cout << t << " ";
  } while(t.getType() != Token::Type::EndOfFile); */
  return 0;
}
