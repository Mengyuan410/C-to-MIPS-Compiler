#include "ast/Declaration/StructDeclaration.hpp"

StructDeclaration::StructDeclaration(NodePtr typeSpecifier, NodePtr declarator)
{
  branches.push_back(typeSpecifier);
  branches.push_back(declarator);
  type = branches[0]->DeclarationType();
  identifierName = branches[1]->returnIdentifier();
}

StructDeclaration::~StructDeclaration()
{
  delete branches[0];
  delete branches[1];
}

void StructDeclaration::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "StructDeclaration [" << std::endl;
  dst << indent << "Type: ";
  branches[0]->generateParser(dst, indent+"  ");
  branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}


// Functions
std::string StructDeclaration::returnIdentifier() const
{
  return identifierName;
}
enum Specifier StructDeclaration::DeclarationType() const
{
  return type;
}

bool StructDeclaration::returnIsPointer() const
{
  return branches[1]->returnIsPointer();
}

