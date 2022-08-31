#include "ast/DoubleSideOperator/RightShiftOperator.hpp"

RightShiftOperator::RightShiftOperator(NodePtr Left, NodePtr Right)
{
  branches.push_back(Left);
  branches.push_back(Right);
}

RightShiftOperator::~RightShiftOperator()
{
  delete branches[0];
  delete branches[1];
}

// Visualizing
void RightShiftOperator::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "ShiftLeft [" << std::endl;
  dst << indent << indent << "Left ["<<std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << indent << "Right ["<<std::endl;
  branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// Context
void RightShiftOperator::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
  branches[0]->generateContext(frameContext,parameterOffset,isGlobal   );
  branches[1]->generateContext(frameContext,parameterOffset,isGlobal   );
  type = branches[0]->returnType();
}

// MIPS
void RightShiftOperator::generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const
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

  std::cout << "sra" << indent << "$"<<destReg<< ",$"<<leftReg<<",$"<<rightReg<<std::endl;

  frameContext->deleteReservedRegsForFunctionCall(leftReg);
  frameContext->deleteReservedRegsForFunctionCall(rightReg);
}

// Functions

bool RightShiftOperator::isAssignmentExp() const
{
  return false;
}

void RightShiftOperator::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
  branches[1]->countFunctionParameter(functionCallParameterOffset);
}

int RightShiftOperator::getValue() const{
  return (branches[0]->getValue() >> branches[1]->getValue());
}

enum Specifier RightShiftOperator::returnType() const{
  return type;
}