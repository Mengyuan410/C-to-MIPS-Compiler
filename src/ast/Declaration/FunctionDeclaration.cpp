#include "ast/Declaration/FunctionDeclaration.hpp"

FunctionDeclaration::FunctionDeclaration(NodePtr DeclarationSpecifier, NodePtr Declarator)
{
  branches.push_back(DeclarationSpecifier);
  branches.push_back(Declarator);
  type = branches[0]->DeclarationType();
  identifierName = branches[1]->returnIdentifier();
}

FunctionDeclaration::~FunctionDeclaration()
{
  delete branches[0];
  delete branches[1];
}

void FunctionDeclaration::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "FunctionDeclaration [" << std::endl;
  dst << indent << "Type: ";
  branches[0]->generateParser(dst, indent+"  ");
  branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// Context
void FunctionDeclaration::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) 
{
    branches[1] -> generateContext(frameContext, parameterOffset, isGlobal);
    blockContext = frameContext;
} 

// MIPS
void FunctionDeclaration::generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const
{
  branches[1] -> generateMIPS(indent, dstReg, frameContext);
}

// Functions

std::string FunctionDeclaration::returnIdentifier() const
{
  return identifierName;
}

ContextPtr FunctionDeclaration::getContextPtr() const{
  return blockContext;
}

enum Specifier FunctionDeclaration::returnType() const{
  return type;
}

std::vector<enum Specifier> FunctionDeclaration::returnParameterTypes() const{
  std::vector<enum Specifier> parameterTypes = branches[1]->returnParameterTypes();
  
  parameterTypes.push_back(type);
  return parameterTypes;
}

std::vector<bool> FunctionDeclaration::returnParameterIsPointer() const{
  std::vector<bool> parameterIsPointer = branches[1]->returnParameterIsPointer();
  parameterIsPointer.push_back(0);
  return parameterIsPointer;
}



