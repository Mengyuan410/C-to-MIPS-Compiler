#include "ast/DoubleSideOperator/LeftShiftOperator.hpp"

LeftShiftOperator::LeftShiftOperator(NodePtr Left, NodePtr Right)
{
  branches.push_back(Left);
  branches.push_back(Right);
}

LeftShiftOperator::~LeftShiftOperator()
{
  delete branches[0];
  delete branches[1];
}

// Visualizing
void LeftShiftOperator::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "ShiftLeft [" << std::endl;
  dst << indent << indent << "Left ["<<std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << indent << "Right ["<<std::endl;
  branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// Context
void LeftShiftOperator::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
  branches[0]->generateContext(frameContext,parameterOffset,isGlobal   );
  branches[1]->generateContext(frameContext,parameterOffset,isGlobal   );
  type = branches[0]->returnType();
}

// MIPS
void LeftShiftOperator::generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const
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

  std::cout << "sll" << indent << "$"<<destReg<< ",$"<<leftReg<<",$"<<rightReg<<std::endl;

  frameContext->deleteReservedRegsForFunctionCall(leftReg);
  frameContext->deleteReservedRegsForFunctionCall(rightReg);
}

// Functions

bool LeftShiftOperator::isAssignmentExp() const
{
  return false;
}
void LeftShiftOperator::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
  branches[1]->countFunctionParameter(functionCallParameterOffset);
}

int LeftShiftOperator::getValue() const{
  return (branches[0]->getValue() << branches[1]->getValue());
}
enum Specifier LeftShiftOperator::returnType() const{
  return type;
}