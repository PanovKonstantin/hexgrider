#include "include/Array.h"

using namespace ast;

Array::Array(std::unique_ptr<std::vector<std::unique_ptr<Node>>> elements_)
{
    elements = std::move(elements_);
}

std::string Array::toString(int depth) const
{
    std::string ret_str = prefix(depth) + "Array of length (" + 
                          std::to_string(elements->size()) + ")\n";
    for(auto const& element: *elements)
    {
        ret_str += element->toString(depth + 1);
    }
    return ret_str;
}