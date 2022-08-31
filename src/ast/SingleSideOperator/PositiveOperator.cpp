#include "ast/SingleSideOperator/PositiveOperator.hpp"

PositiveOperator::PositiveOperator(NodePtr Left)
{
  branches.push_back(Left);
}

PositiveOperator::~PositiveOperator()
{
  delete branches[0];
}

// Visualization
void PositiveOperator::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Positive [" << std::endl;
  dst << indent << "Target ["<<std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// MIPS
void PositiveOperator::generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const
{
  branches[0]->generateMIPS(indent,destReg,frameContext);
}

// Context
void PositiveOperator::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
  branches[0]->generateContext(frameContext,parameterOffset,isGlobal   );
}

// Functions
void PositiveOperator::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
}

int PositiveOperator::getValue() const{
  return (branches[0]->getValue());
}


