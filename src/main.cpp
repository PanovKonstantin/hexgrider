#include <iostream>
#include <sstream>
#include "lexer/Lexer.h"
#include "ast/Ast.h"
#include "parser/Parser.h"

using namespace lexer;
using namespace parser;
using namespace ast;

std::unique_ptr<Node> readAndParseStdin()
{
  Parser p(std::make_unique<Lexer>(std::cin));
  return p.parse();
}


int main()
{
  std::cout << readAndParseStdin()->toString();
  return 0;
}
