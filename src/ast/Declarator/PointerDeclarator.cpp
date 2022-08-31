#include "ast/Declarator/PointerDeclarator.hpp"

// Constructors


PointerDeclarator::PointerDeclarator(NodePtr directDeclarator){
  branches.push_back(directDeclarator);
} 

// Destructor
PointerDeclarator::~PointerDeclarator()
{
  delete branches[0];
}
// Visualising
void PointerDeclarator::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent <<"Pointer Declarator [" << std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// Context
void PointerDeclarator::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
  branches[0]->generateContext(frameContext,parameterOffset,isGlobal   );

}


// MIPS
void PointerDeclarator::generateMIPS(std::string indent, int dstReg, ContextPtr frameContext ) const
{
  branches[0] -> generateMIPS(indent, dstReg, frameContext);
}

// Functions
bool PointerDeclarator::IsInitializeDeclaration() const
{
  return false;
}

std::string PointerDeclarator::returnIdentifier() const{
  return branches[0] -> returnIdentifier();
}


bool PointerDeclarator::returnIsPointer()const{
  return true;
}

std::vector<enum Specifier> PointerDeclarator::returnParameterTypes() const{
  return branches[0]->returnParameterTypes();
}

std::vector<bool> PointerDeclarator::returnParameterIsPointer() const{
  return branches[0]->returnParameterIsPointer();
}
