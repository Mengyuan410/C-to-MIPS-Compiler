#ifndef listmanipulation_hpp
#define listmanipulation_hpp

#include "ast/Node.hpp"

#include <vector>

// Functions to be used in parser for instantiation / concatenation
typedef std::vector<NodePtr> List;
typedef List *ListPtr;

// Functions to be used in parser for instantiation / concatenation
inline ListPtr initialize_list(NodePtr statement)
{
  ListPtr statement_list = new List(); // Potentially replace with Nodelist(NodePtr) constructor
  statement_list->push_back(statement);
  return statement_list;
}

inline ListPtr append_list(ListPtr statement_list, NodePtr statement)
{
  statement_list->push_back(statement);
  return statement_list;
}

#endif