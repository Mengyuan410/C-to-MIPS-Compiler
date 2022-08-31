#include "ast/TernaryOperator/TernaryConditionalOperator.hpp"

TernaryConditionalOperator::TernaryConditionalOperator(NodePtr Condition, NodePtr Left, NodePtr Right)
{
  branches.push_back(Condition);
  branches.push_back(Left);
  branches.push_back(Right);
}

TernaryConditionalOperator::~TernaryConditionalOperator()
{
  delete branches[0];
  delete branches[1];
  delete branches[2];
}

// Visualization
void TernaryConditionalOperator::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "TernaryConditional [" << std::endl;
  dst << indent << "Condition ["<<std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
  dst << indent << "True ["<<std::endl;
  branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
  dst << indent << "False ["<<std::endl;
  branches[2]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// Context
void TernaryConditionalOperator::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
  if (branches[0]->returnType()==_double){
    branches[0]->generateContext(frameContext,parameterOffset,isGlobal   );
  }
  else{

  }
  branches[0]->generateContext(frameContext,parameterOffset,isGlobal   );
  branches[1]->generateContext(frameContext,parameterOffset,isGlobal   );
  branches[2]->generateContext(frameContext,parameterOffset,isGlobal   );
}

// MIPS
void TernaryConditionalOperator::generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const
{
  // int leftReg = frameContext->alloCalReg();
  // int rightReg = frameContext->alloCalReg();
  // frameContext->dealloReg(leftReg);
  // frameContext->dealloReg(rightReg);

  std::string label1 = makeName("L");
  std::string label2 = makeName("L");

  branches[0]->generateMIPS(indent,destReg,frameContext);

  std::cout << "beq" << indent << "$" << destReg << ",$0,$" << label1 << std::endl;
  branches[1]->generateMIPS(indent,destReg,frameContext);
  std::cout << "b" << indent << "$" << label2 << std::endl;
  
  std::cout << "$" << label1 << ":" << std::endl;
  branches[2]->generateMIPS(indent,destReg,frameContext); ///
  std::cout << std::endl;

  std::cout << "$" << label2 << ":" << std::endl;
}

// Functions
void TernaryConditionalOperator::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
  branches[1]->countFunctionParameter(functionCallParameterOffset);
  branches[2]->countFunctionParameter(functionCallParameterOffset);
}
int TernaryConditionalOperator::getValue() const{
  return (branches[0]->getValue() ? branches[1]->getValue() : branches[2]->getValue());
}