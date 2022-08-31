#include "ast/DoubleSideOperator/MulOperator.hpp"

MulOperator::MulOperator(NodePtr Left, NodePtr Right)
{
  branches.push_back(Left);
  branches.push_back(Right);
}

MulOperator::~MulOperator()
{
  delete branches[0];
  delete branches[1];
}

// Visualizing
void MulOperator::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Multiply [" << std::endl;
  dst << indent << "Left ["<<std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
  dst << indent << "Right ["<<std::endl;
  branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// Context
void MulOperator::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
  branches[0]->generateContext(frameContext,parameterOffset,isGlobal   );
  branches[1]->generateContext(frameContext,parameterOffset,isGlobal   );
  enum Specifier type1 = branches[0]->returnType();
  enum Specifier type2 = branches[1]->returnType();
  if (type1 ==_double || type2 ==_double){
    type = _double;
  }
  else {
    type = type1;
  }
}

// MIPS
void MulOperator::generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const
{
  switch(type){
    case _float:{
      int leftReg = frameContext->alloFloatCalReg();
      int rightReg = frameContext->alloFloatCalReg();
      frameContext->dealloFloatReg(leftReg);
      frameContext->dealloFloatReg(rightReg);

      branches[0]->generateMIPS(indent,leftReg,frameContext);
      frameContext->alloFloatReg(leftReg);

      branches[1]->generateMIPS(indent,rightReg,frameContext);
      frameContext->dealloFloatReg(leftReg);
      std::cout <<"mul.s" << indent << "$f"<<destReg<< ",$f"<<leftReg<<",$f"<<rightReg<<std::endl;
      break;
    }
    case _double:{
      int leftReg = frameContext->alloFloatCalReg();
      int rightReg = frameContext->alloFloatCalReg();
      frameContext->dealloFloatReg(leftReg);
      frameContext->dealloFloatReg(rightReg);

      branches[0]->generateMIPSDouble(indent,leftReg,frameContext);
      frameContext->alloFloatReg(leftReg);

      branches[1]->generateMIPSDouble(indent,rightReg,frameContext);
      frameContext->dealloFloatReg(leftReg);
      std::cout <<"mul.d" << indent << "$f"<<destReg<< ",$f"<<leftReg<<",$f"<<rightReg<<std::endl;
      break;
    }
    
    default:{
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
      
      std::cout << "mult" << indent << "$" <<leftReg<<",$"<<rightReg<<std::endl;
      std::cout << "mflo" << indent << "$" << destReg << std::endl;
      
      frameContext->deleteReservedRegsForFunctionCall(leftReg);
      frameContext->deleteReservedRegsForFunctionCall(rightReg);
    }
  }
}

// Functions

bool MulOperator::isAssignmentExp() const
{
  return false;
}
void MulOperator::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
  branches[1]->countFunctionParameter(functionCallParameterOffset);
}

int MulOperator::getValue() const{
  return (branches[0]->getValue() * branches[1]->getValue());
}

enum Specifier MulOperator::returnType() const{
  return type;
}

void MulOperator::generateMIPSDouble(std::string indent, int destReg, ContextPtr frameContext) const{ 

  int leftReg = frameContext->alloFloatCalReg();
  int rightReg = frameContext->alloFloatCalReg();
  frameContext->dealloFloatReg(leftReg);
  frameContext->dealloFloatReg(rightReg);

  branches[0]->generateMIPSDouble(indent,leftReg,frameContext);
  frameContext->alloFloatReg(leftReg);

  branches[1]->generateMIPSDouble(indent,rightReg,frameContext);
  frameContext->dealloFloatReg(leftReg);
  std::cout <<"mul.d" << indent << "$f"<<destReg<< ",$f"<<leftReg<<",$f"<<rightReg<<std::endl;
}