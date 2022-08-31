#include "ast/Block/Scope.hpp"

Scope::Scope(){}

Scope::Scope(std::vector<NodePtr> _branches){
  for(unsigned i = 0; i < _branches.size(); i++){
    branches.push_back(_branches[i]);
  }
}

Scope::Scope(std::vector<NodePtr> _declaration_branches, std::vector<NodePtr> _statement_branches){
  for(unsigned i = 0; i < _declaration_branches.size(); i++){
    branches.push_back(_declaration_branches[i]);
  }
  for (unsigned i = 0; i < _statement_branches.size(); i++){
    branches.push_back(_statement_branches[i]);
  }
}

Scope::~Scope()
{
  for(unsigned i = 0; i < branches.size(); i++){
    delete branches[i];
  }
}

// Parser
void Scope::generateParser(std::ostream &dst, std::string indent) const{
  dst << indent << "Scope [" << std::endl;
  for(unsigned i = 0; i < branches.size(); i++){
    branches[i]->generateParser(dst, indent+"  ");
  }
  dst << indent << "]" << std::endl;
}

// Context
void Scope::generateContext(ContextPtr parentContext, int parameterOffset, bool isGlobal  ) {
  
  blockContext =  new Context(parentContext);
  
  for (unsigned i = 0; i < branches.size(); i++){
    
    branches[i]->generateContext(blockContext, parameterOffset, isGlobal);  
  }
  

  parentContext->updateCurrentStackSize(blockContext->getCurrentStackSize());
  parentContext->updateHasFunction(blockContext->getHasFunction());
  parentContext->updateFloatDoubleLabelBindings(blockContext->getFloatDoubleLabelBindings());
  parentContext->updateStringLabelBindings(blockContext->getStringLabelBindings());
}


// MIPS
void Scope::generateMIPS(std::string indent, int destReg, ContextPtr _blockContext) const{
  blockContext->updateFloatDoubleLabelBindings(_blockContext->getFloatDoubleLabelBindings());
  for (unsigned i = 0; i < branches.size(); i++){
    branches[i]->generateMIPS(indent, destReg, blockContext);  
  }
  _blockContext->updateFloatDoubleLabelBindings(blockContext->getFloatDoubleLabelBindings());
}

// Functions
ContextPtr Scope::getContextPtr() const{
  return blockContext ;
}

void Scope::countFunctionParameter(int* functionCallParameterOffset) const{
  for (unsigned i = 0; i < branches.size();i++){
    branches[i]->countFunctionParameter(functionCallParameterOffset);
  }
}

std::vector<std::tuple<int, std::string>> Scope::getSwitchCases() const { 
  std::vector<std::tuple<int, std::string>> casesVector;
  for (unsigned i = 0; i < branches.size();i++){
    branches[i]->getCaseInfo(casesVector);
  }
  return casesVector;
}

std::vector<std::string> Scope::getSwitchDefault() const{
  std::vector<std::string> defaultVector;
  for (unsigned i = 0; i < branches.size();i++){
    branches[i]->getDefaultInfo(defaultVector);
  }
  return defaultVector;
}





