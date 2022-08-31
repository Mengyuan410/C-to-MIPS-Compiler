#include "ast/RootNode.hpp"

// Constructor
RootNode::RootNode(NodePtr GlobalScope)
{
  branches.push_back(GlobalScope); 
}

// Destructor
RootNode::~RootNode()
{
  delete branches[0];
}

// Visualising
void RootNode::generateParser(std::ostream &dst, std::string indent) const
{
  branches[0]->generateParser(dst, indent); 
}

void RootNode::generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const
{
  branches[0]->generateMIPS(indent,destReg,frameContext);
}