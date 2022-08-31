#include "ast/Type/TypeDefinedName.hpp"

TypeDefinedName::TypeDefinedName(std::string _name){
    name = _name;
}


TypeDefinedName::~TypeDefinedName()
{}

// Parser
void TypeDefinedName::generateParser(std::ostream &dst, std::string indent) const{}

// Context
void TypeDefinedName::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal) {

  type = frameContext->getTypedefType(name);
  isPointer = frameContext->getTypedefIsPointer(name);
  blockContext = frameContext;
}

// MIPS
void TypeDefinedName::generateMIPS(std::string indent, int destReg, ContextPtr _blockContext) const{}

// Functions

enum Specifier TypeDefinedName::DeclarationType() const{
  return type;
}

bool TypeDefinedName::returnIsPointer() const{
  return isPointer;
}


