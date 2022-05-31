#ifndef TKOM_AST_NODE_H
#define TKOM_AST_NODE_H

#include <string>

namespace ast
{

class Node
{
public:
  virtual ~Node();
  virtual std::string toString(int depth = 0) const = 0;
  virtual std::string prefix(int depth) const
  {
    return std::string(depth, '|');
  }
};

} // namespace ast

#endif // TKOM_AST_NODE_H
