#include "ast/Type/StructSpecifier.hpp"

StructSpecifier::StructSpecifier(std::string identifier){
    structIdentifier = identifier;
}


StructSpecifier::StructSpecifier(std::vector<NodePtr> structDeclarations){
  for(unsigned i = 0; i < structDeclarations.size(); i++){
    branches.push_back(structDeclarations[i]);
  }
  structIdentifier = "ymystructcathy";
}


StructSpecifier::StructSpecifier(std::string identifier, std::vector<NodePtr> structDeclarations){
  for(unsigned i = 0; i < structDeclarations.size(); i++){
    branches.push_back(structDeclarations[i]);
  }
  structIdentifier = identifier;
}

StructSpecifier::~StructSpecifier()
{
  if (branches.size()!=0){
    for(unsigned i = 0; i < branches.size(); i++){
      delete branches[i];
    }
  }
}

// Parser
void StructSpecifier::generateParser(std::ostream &dst, std::string indent) const{
  dst << indent << "Struct Specifier [" << std::endl;
  for(unsigned i = 0; i < branches.size(); i++){
    branches[i]->generateParser(dst, indent+"  ");
  }
  dst << indent << "]" << std::endl;
}

// Context
void StructSpecifier::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  int countSize = 0;
  for (unsigned i = 0; i < branches.size(); i++){ 
    enum Specifier structMemberType = branches[i]->DeclarationType();
    std::string structMemberName = branches[i]->returnIdentifier();
    frameContext->changeStructMemberBindings(structIdentifier, structMemberName,structMemberType);
  }
  blockContext = frameContext;
  
}

// MIPS
void StructSpecifier::generateMIPS(std::string indent, int destReg, ContextPtr _blockContext) const{}

// Functions
std::string StructSpecifier::returnIdentifier() const{
    return structIdentifier;
}
bool StructSpecifier::isStructSpecifier() const{
  return true;
}










