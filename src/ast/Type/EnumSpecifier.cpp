#include "ast/Type/EnumSpecifier.hpp"

EnumSpecifier::EnumSpecifier(std::vector<NodePtr> enumerators){
  for(unsigned i = 0; i < enumerators.size(); i++){
    branches.push_back(enumerators[i]);
  }
}
EnumSpecifier::EnumSpecifier(std::string identifier){}
EnumSpecifier::EnumSpecifier(std::string identifier, std::vector<NodePtr> enumerators){
  for(unsigned i = 0; i < enumerators.size(); i++){
    branches.push_back(enumerators[i]);
  }
}

EnumSpecifier::~EnumSpecifier()
{
  if (branches.size()!=0){
    for(unsigned i = 0; i < branches.size(); i++){
      delete branches[i];
    }
  }
}

// Parser
void EnumSpecifier::generateParser(std::ostream &dst, std::string indent) const{
  dst << indent << "Enum Specifier [" << std::endl;
  if (branches.size()!=0){
    for(unsigned i = 0; i < branches.size(); i++){
      branches[i]->generateParser(dst, indent+"  ");
    }
  }
  dst << indent << "]" << std::endl;
}

// Context
void EnumSpecifier::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  int prev_value = 0;
  for (unsigned i = 0; i < branches.size(); i++){ 
    if (branches[i]->hasValue()){  // if the enumerator is set to constant
         
        int value = branches[i]->getValue();
        frameContext -> changeEnumVarBindings(branches[i]->returnIdentifier(),value);
        prev_value = value;
    }
    else if (i == 0){  // the first enumerator has a constant 0
        frameContext -> changeEnumVarBindings(branches[i]->returnIdentifier(), 0);
    }
    else{  // adding 1 to the value of the previous enumeration constant
        frameContext -> changeEnumVarBindings(branches[i]->returnIdentifier(), prev_value+1);
        prev_value += 1;
    }
  }
  blockContext = frameContext;
}

// MIPS
void EnumSpecifier::generateMIPS(std::string indent, int destReg, ContextPtr _blockContext) const{}

// Functions
bool EnumSpecifier::isEnumSpecifier() const{
  return true;
}

enum Specifier EnumSpecifier::DeclarationType() const{
  return _int;
}







