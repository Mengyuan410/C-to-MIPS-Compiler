#include "ast/SingleSideOperator/BitwiseNotOperator.hpp"

BitwiseNotOperator::BitwiseNotOperator(NodePtr Left)
{
  branches.push_back(Left);
}

BitwiseNotOperator::~BitwiseNotOperator()
{
  delete branches[0];
}

// Visualization
void BitwiseNotOperator::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Not [" << std::endl;
  dst << indent << "Target ["<<std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// MIPS
void BitwiseNotOperator::generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const
{
  
  branches[0]->generateMIPS(indent,destReg,frameContext);

  std::cout << "nor" << indent << "$" << destReg << ",$0,$" << destReg << std::endl;
}

// Context
void BitwiseNotOperator::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
  branches[0]->generateContext(frameContext,parameterOffset,isGlobal   );
}

// Functions
void BitwiseNotOperator::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
}

int BitwiseNotOperator::getValue() const{
  return (~branches[0]->getValue());
}