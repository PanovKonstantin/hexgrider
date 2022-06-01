#include <iostream>
#include <sstream>
#include "lexer/Lexer.h"
#include "ast/Ast.h"
#include "parser/Parser.h"
#include "llvm/IR/LLVMContext.h"

using namespace lexer;
using namespace parser;
using namespace ast;

std::unique_ptr<Node> readAndParseStdin()
{
  Parser p(std::make_unique<Lexer>(std::cin));
  return p.parse();
}


int main()
{/* 
  llvm::LLVMContext ctx = llvm::LLVMContext();
  CodeGenVisitor cgv = CodeGenVisitor(ctx);
  std::unique_ptr<Node> n = readAndParseStdin();
  cgv(n);
  cgv.print(); */
  std::cout << readAndParseStdin()->toString();
  return 0;
}
