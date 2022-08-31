#include "ast/DoubleSideOperator/BitwiseAndOperator.hpp"

BitwiseAndOperator::BitwiseAndOperator(NodePtr Left, NodePtr Right)
{
  branches.push_back(Left);
  branches.push_back(Right);
}

BitwiseAndOperator::~BitwiseAndOperator()
{
  delete branches[0];
  delete branches[1];
}

// Visualizing
void BitwiseAndOperator::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "BitwiseAnd [" << std::endl;
  dst << indent << indent << "Left ["<<std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << indent << "Right ["<<std::endl;
  branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// Context
void BitwiseAndOperator::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
  branches[0]->generateContext(frameContext,parameterOffset,isGlobal   );
  branches[1]->generateContext(frameContext,parameterOffset,isGlobal   );
  type = branches[0]->returnType();
}

// MIPS
void BitwiseAndOperator::generateMIPS(std::string indent, int destReg, ContextPtr frameContext ) const
{
  int leftReg = frameContext->alloCalReg();
  int rightReg = frameContext->alloCalReg();
  frameContext->dealloReg(leftReg);
  frameContext->dealloReg(rightReg);

  branches[0]->generateMIPS(indent,leftReg,frameContext);

  frameContext->addReservedRegsForFunctionCall(leftReg);

  frameContext->alloReg(leftReg);
  branches[1]->generateMIPS(indent,rightReg,frameContext);

  frameContext->addReservedRegsForFunctionCall(rightReg);

  frameContext->dealloReg(leftReg);
  
  std::cout << "and" << indent << "$" << destReg << ",$" << leftReg << ",$" << rightReg << std::endl;

  frameContext->deleteReservedRegsForFunctionCall(leftReg);
  frameContext->deleteReservedRegsForFunctionCall(rightReg);
}

// Functions

bool BitwiseAndOperator::isAssignmentExp() const
{
  return false;
}
void BitwiseAndOperator::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
  branches[1]->countFunctionParameter(functionCallParameterOffset);
}

int BitwiseAndOperator::getValue() const{
  return (branches[0]->getValue() & branches[1]->getValue());
}

enum Specifier BitwiseAndOperator::returnType() const{
  return type;
}