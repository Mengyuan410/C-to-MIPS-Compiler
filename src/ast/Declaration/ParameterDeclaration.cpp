#include "ast/Declaration/ParameterDeclaration.hpp"

ParameterDeclaration::ParameterDeclaration(NodePtr DeclarationSpecifier, NodePtr Declarator)
{
  branches.push_back(DeclarationSpecifier);
  branches.push_back(Declarator);
  type = branches[0]->DeclarationType();
  identifierName = branches[1]->returnIdentifier();
}

ParameterDeclaration::~ParameterDeclaration()
{
  delete branches[0];
  delete branches[1];
}

void ParameterDeclaration::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "ParameterDeclaration [" << std::endl;
  dst << indent << "Type: ";
  branches[0]->generateParser(dst, indent+"  ");
  branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// MIPS
void ParameterDeclaration::generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const{}

// Functions
std::string ParameterDeclaration::returnIdentifier() const
{
  return identifierName;
}
enum Specifier ParameterDeclaration::DeclarationType() const
{

  return type;
  
}

bool ParameterDeclaration::returnIsPointer() const
{
  return branches[1]->returnIsPointer();
}




