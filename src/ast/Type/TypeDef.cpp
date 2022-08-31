#include "ast/Type/TypeDef.hpp"

TypeDef::TypeDef(NodePtr _type){
    branches.push_back(_type);
    type = branches[0]->DeclarationType();
}


TypeDef::~TypeDef()
{
  delete branches[0];
}

// Parser
void TypeDef::generateParser(std::ostream &dst, std::string indent) const{
  dst << indent << "TypeDef[" << std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// Context
void TypeDef::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal) {
  blockContext = frameContext;
}

// MIPS
void TypeDef::generateMIPS(std::string indent, int destReg, ContextPtr _blockContext) const{}

// Functions
bool TypeDef::IsTypeDef() const{
    return true;
}
enum Specifier TypeDef::returnType() const{
 
  return type;
}










