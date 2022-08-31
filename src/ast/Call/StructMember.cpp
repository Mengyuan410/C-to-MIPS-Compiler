#include "ast/Call/StructMember.hpp"

// Constructor
StructMember::StructMember(NodePtr structIdentifier, std::string _structMemberName){
    branches.push_back(structIdentifier);  
    structMemberName = _structMemberName;
}

// Destructor
StructMember::~StructMember()
{
  delete branches[0];
}

// Visualising
void StructMember::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "StructMember: " << structMemberName << std::endl;
}

// Context
void StructMember::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
}

void StructMember::generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const
{
  std::string modifiedName = branches[0]->returnIdentifier() + '.' + structMemberName;
  int offset = frameContext->getOffset(modifiedName);
  std::cout << "lw" << indent << "$"<<destReg << "," << offset << "($fp)" << std::endl;
}

// Functions
std::string StructMember::returnIdentifier() const{
  std::string modifiedName = branches[0]->returnIdentifier() + '.' + structMemberName;
  return modifiedName;
}