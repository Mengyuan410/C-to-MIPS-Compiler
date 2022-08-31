#include "ast/SingleSideOperator/PointerOperator.hpp"

PointerOperator::PointerOperator(NodePtr Left)
{
  branches.push_back(Left);
}

PointerOperator::~PointerOperator()
{
  delete branches[0];
}

// Visualization
void PointerOperator::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Pointer [" << std::endl;
  dst << indent << "Target ["<<std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// MIPS
void PointerOperator::generateMIPS(std::string indent, int destReg, ContextPtr frameContext ) const
{
  
  if (branches[0]->returnType()==_char){
    branches[0]->generateMIPS(indent, destReg, frameContext);
    std::cout << "lb" << indent << "$" << destReg << ",0($" << destReg << ")" << std::endl;
  }
  else if (branches[0]->returnType()==_float){
    std::string varName = branches[0]->returnIdentifier();
    int immReg = frameContext->alloCalReg();
    frameContext->dealloReg(immReg);
    branches[0]->generateMIPS(indent, immReg, frameContext);
    std::cout << "lwc1" << indent << "$f" << destReg << ",0($" << immReg << ")" << std::endl;
  }
  else if (branches[0]->returnType()==_double){
    std::string varName = branches[0]->returnIdentifier();
    int immReg = frameContext->alloCalReg();
    frameContext->dealloReg(immReg);
    branches[0]->generateMIPS(indent, immReg, frameContext);
    std::cout << "lwc1" << indent << "$f" << destReg << ",4($" << immReg << ")" << std::endl;
    std::cout << "lwc1" << indent << "$f" << destReg+1 << ",0($" << immReg << ")" << std::endl;
  }
  else{
    branches[0]->generateMIPS(indent, destReg, frameContext);
    std::cout << "lw" << indent << "$" << destReg << ",0($" << destReg << ")" << std::endl;
  }
}

void PointerOperator::generateMIPSPointerAssignment(std::string indent, int destReg, ContextPtr frameContext) const{
  std::string id = branches[0]->returnIdentifier();
  int offset = frameContext->getOffset(id);

  std::cout << "lw" << indent << "$" << destReg << "," << offset << "($fp)" << std::endl;
}

void PointerOperator::generateMIPSPointerGlobalAssignment(std::string indent, int destReg, ContextPtr frameContext ) const
{
  branches[0]->generateMIPS(indent, destReg, frameContext);
}

// Context
void PointerOperator::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
  branches[0]->generateContext(frameContext,parameterOffset,isGlobal   );
}

// Functions
void PointerOperator::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
}

int PointerOperator::getValue() const{
  return (~branches[0]->getValue());
}

bool PointerOperator::returnIsPointer() const{
  return true;
}

std::string PointerOperator::returnIdentifier() const{
  return branches[0]->returnIdentifier();
}

enum Specifier PointerOperator::returnType() const{
  return branches[0]->returnType();
}

void PointerOperator::generateMIPSDouble(std::string indent, int destReg, ContextPtr frameContext ) const{
  std::string varName = branches[0]->returnIdentifier();
  if (frameContext->getIsGlobal(varName)==true){
    int immReg = frameContext->alloCalReg();
    std::cout << "lui" << indent << "$" << immReg << ",%hi(" << varName << ")"  << std::endl;
    std::cout << "lw" << indent << "$" << immReg << ",%lo(" << varName << ")($" << immReg << ")" << std::endl; 
    std::cout << "lwc1" << indent << "$f" << destReg << ",4($" << immReg << ")" << std::endl;
    std::cout << "lwc1" << indent << "$f" << destReg+1 << ",0($" << immReg << ")" << std::endl;
  }
  else{
    int immReg = frameContext->alloCalReg();
    frameContext->dealloReg(immReg);
    int offset = frameContext->getOffset(branches[0]->returnIdentifier());
    std::cout << "lw" << indent << "$" << immReg << "," << offset <<"($fp)" << std::endl;
    std::cout << "lwc1" << indent << "$f" << destReg << ",4($" << immReg << ")" << std::endl;
    std::cout << "lwc1" << indent << "$f" << destReg+1 << ",0($" << immReg << ")" << std::endl;
  }
  
}