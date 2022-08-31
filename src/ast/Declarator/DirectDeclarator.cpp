#include "ast/Declarator/DirectDeclarator.hpp"

// Constructors


DirectDeclarator::DirectDeclarator(std::string Id){
  branches.push_back(new Identifier(Id));
} 

// Destructor
DirectDeclarator::~DirectDeclarator()
{
  delete branches[0];
}

// Context
void DirectDeclarator::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
}

//Visualising
void DirectDeclarator::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Direct Declarator [" << std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

bool DirectDeclarator::IsInitializeDeclaration() const
{
  return false;
}

std::string DirectDeclarator::returnIdentifier() const{
  return branches[0] -> returnIdentifier();
}

bool  DirectDeclarator::IsDirectDeclarator()const {
  return true;
}

