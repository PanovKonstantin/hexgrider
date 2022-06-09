#include <iostream>
#include <sstream>
#include "lexer/Lexer.h"
#include "parser/Ast.h"
#include "interpreter/StandardScope.h"
#include "parser/Parser.h"
#include "HexgridErrors.h"
#include "interpreter/Interpreter.h"

using namespace lexer;
using namespace parser;
using namespace ast;
using namespace intprt;

std::unique_ptr<Node> readAndParseStdin()
{
  Parser p(std::make_unique<Lexer>(std::cin));
  return p.parse();
}

int main()
{
  // std::cout << readAndParseStdin()->toString();
  auto i = Interpreter();
  readAndParseStdin()->accept(i);
  return 0;
}
