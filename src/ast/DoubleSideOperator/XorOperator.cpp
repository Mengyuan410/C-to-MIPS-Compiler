#include "ast/DoubleSideOperator/XorOperator.hpp"

XorOperator::XorOperator(NodePtr Left, NodePtr Right)
{
  branches.push_back(Left);
  branches.push_back(Right);
}

XorOperator::~XorOperator()
{
  delete branches[0];
  delete branches[1];
}

// Visualization
void XorOperator::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Xor [" << std::endl;
  dst << indent << "Left ["<<std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
  dst << indent << "Right ["<<std::endl;
  branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// Context
void XorOperator::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
  branches[0]->generateContext(frameContext,parameterOffset,isGlobal   );
  branches[1]->generateContext(frameContext,parameterOffset,isGlobal   );
  type = branches[0]->returnType();
}

// MIPS
void XorOperator::generateMIPS(std::string indent, int destReg, ContextPtr frameContext ) const
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

  std::cout << "xor" << indent << "$"<<destReg<< ",$"<<leftReg<<",$"<<rightReg<<std::endl;
  
  frameContext->deleteReservedRegsForFunctionCall(leftReg);
  frameContext->deleteReservedRegsForFunctionCall(rightReg);

}

// Functions

bool XorOperator::isAssignmentExp() const
{
  return false;
}
void XorOperator::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
  branches[1]->countFunctionParameter(functionCallParameterOffset);
}

int XorOperator::getValue() const{
  return (branches[0]->getValue() ^ branches[1]->getValue());
}

enum Specifier XorOperator::returnType() const{
  return type;
}