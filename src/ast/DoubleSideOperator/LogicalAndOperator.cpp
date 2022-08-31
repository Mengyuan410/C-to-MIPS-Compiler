#include "ast/DoubleSideOperator/LogicalAndOperator.hpp"

LogicalAndOperator::LogicalAndOperator(NodePtr Left, NodePtr Right)
{
  branches.push_back(Left);
  branches.push_back(Right);
}

LogicalAndOperator::~LogicalAndOperator()
{
  delete branches[0];
  delete branches[1];
}

// Visualization
void LogicalAndOperator::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "LogicalAnd [" << std::endl;
  dst << indent << "Left ["<<std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
  dst << indent << "Right ["<<std::endl;
  branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// Context
void LogicalAndOperator::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
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
void LogicalAndOperator::generateMIPS(std::string indent, int destReg, ContextPtr frameContext ) const
{
  switch(type){
    case _float:{
      std::string label1 = makeName("L");
      std::string label2 = makeName("L");
      int immReg = frameContext->alloFloatCalReg();
      int zeroReg = frameContext->alloFloatCalReg();
      frameContext->dealloFloatReg(immReg);
      frameContext->dealloFloatReg(zeroReg);

      branches[0]->generateMIPS(indent,immReg,frameContext);
      std::cout << "mtc1" << indent << "$0,$f" << zeroReg << std::endl;
      std::cout << "c.eq.s" << indent << "$f" << zeroReg << ",$f" << immReg << std::endl;
      std::cout << "bc1t" << indent << "$" << label1 << ":" << std::endl;
      std::cout << "nop" << std::endl;

      branches[1]->generateMIPS(indent,immReg,frameContext);
      std::cout << "mtc1" << indent << "$0,$f" << zeroReg << std::endl;
      std::cout << "c.eq.s" << indent << "$f" << zeroReg << ",$f" << immReg << std::endl;
      std::cout << "bc1t" << indent << "$" << label1 << ":" << std::endl;
      std::cout << "nop" << std::endl;

      int immReg2 = frameContext->alloCalReg();
      frameContext->dealloReg(immReg2);
      std::cout << "li" << indent << "$" << immReg2 << ",1"<<std::endl;
      std::cout << "b" << indent << "$" << label2 << std::endl;
      std::cout << "nop" << std::endl;

      std::cout << "$" << label1 << ":" <<std::endl;
      std::cout << "move" << indent << "$" << immReg2 << ",$0" << std::endl;

      std::cout << "$" << label2 << ":" <<std::endl;
      std::cout << "mtc1" << indent << "$" << immReg2 << ",$f" << destReg << std::endl;
      std::cout << "cvt.s.w" << indent << "$f" << destReg << ",$f" << destReg << std::endl;
      break;
    }
    case _double:{
      std::string label1 = makeName("L");
      std::string label2 = makeName("L");
      int immReg = frameContext->alloFloatCalReg();
      int zeroReg = frameContext->alloFloatCalReg();
      frameContext->dealloFloatReg(immReg);
      frameContext->dealloFloatReg(zeroReg);

      branches[0]->generateMIPSDouble(indent,immReg,frameContext);
      std::cout << "mtc1" << indent << "$0,$f" << zeroReg << std::endl;
      std::cout << "mtc1" << indent << "$0,$f" << zeroReg+1 << std::endl;
      std::cout << "c.eq.d" << indent << "$f" << zeroReg << ",$f" << immReg << std::endl;
      std::cout << "bc1t" << indent << "$" << label1 << ":" << std::endl;
      std::cout << "nop" << std::endl;

      branches[1]->generateMIPSDouble(indent,immReg,frameContext);
      std::cout << "mtc1" << indent << "$0,$f" << zeroReg << std::endl;
      std::cout << "mtc1" << indent << "$0,$f" << zeroReg+1 << std::endl;
      std::cout << "c.eq.d" << indent << "$f" << zeroReg << ",$f" << immReg << std::endl;
      std::cout << "bc1t" << indent << "$" << label1 << ":" << std::endl;
      std::cout << "nop" << std::endl;

      int immReg2 = frameContext->alloCalReg();
      frameContext->dealloReg(immReg2);
      std::cout << "li" << indent << "$" << immReg2 << ",1"<<std::endl;
      std::cout << "b" << indent << "$" << label2 << std::endl;
      std::cout << "nop" << std::endl;

      std::cout << "$" << label1 << ":" <<std::endl;
      std::cout << "move" << indent << "$" << immReg2 << ",$0" << std::endl;

      std::cout << "$" << label2 << ":" <<std::endl;
      std::cout << "mtc1" << indent << "$" << immReg2 << ",$f" << destReg << std::endl;
      std::cout << "cvt.d.w" << indent << "$f" << destReg << ",$f" << destReg << std::endl;
      break;
    }
    default:{
      int leftReg = frameContext->alloCalReg();
      int rightReg = frameContext->alloCalReg();
      frameContext->dealloReg(leftReg);
      frameContext->dealloReg(rightReg);

      std::string label1 = makeName("L");
      std::string label2 = makeName("L");

      branches[0]->generateMIPS(indent,leftReg,frameContext);
      
      std::cout << "beq" << indent << "$" << leftReg << ",$0,$" << label1 << std::endl;
      frameContext->alloReg(leftReg);
      frameContext->addReservedRegsForFunctionCall(leftReg);


      branches[1]->generateMIPS(indent,rightReg,frameContext);
      frameContext->addReservedRegsForFunctionCall(rightReg);
      frameContext->dealloReg(leftReg);
      std::cout << "beq" << indent << "$" << rightReg << ",$0,$" << label1 << std::endl;

      

      std::cout << "li" << indent << "$" << destReg << ",1" << std::endl; 
      std::cout << "b" << indent << "$" << label2 << std::endl;
      std::cout << std::endl;

      std::cout << "$" << label1 << ":" << std::endl;
      std::cout << "move" << indent << "$" << destReg << ",$0" << std::endl;
      std::cout << std::endl;

      std::cout << "$" << label2 << ":" << std::endl;

      frameContext->deleteReservedRegsForFunctionCall(leftReg);
      frameContext->deleteReservedRegsForFunctionCall(rightReg);
    }
  }
}

// Functions
bool LogicalAndOperator::isAssignmentExp() const
{
  return false;
}

void LogicalAndOperator::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
  branches[1]->countFunctionParameter(functionCallParameterOffset);
}

int LogicalAndOperator::getValue() const{
  return (branches[0]->getValue() && branches[1]->getValue());
}

enum Specifier LogicalAndOperator::returnType() const{
  return type;
}

void LogicalAndOperator::generateMIPSDouble(std::string indent, int destReg, ContextPtr frameContext) const{ 
  std::string label1 = makeName("L");
  std::string label2 = makeName("L");
  int immReg = frameContext->alloFloatCalReg();
  int zeroReg = frameContext->alloFloatCalReg();
  frameContext->dealloFloatReg(immReg);
  frameContext->dealloFloatReg(zeroReg);

  branches[0]->generateMIPSDouble(indent,immReg,frameContext);
  std::cout << "mtc1" << indent << "$0,$f" << zeroReg << std::endl;
  std::cout << "mtc1" << indent << "$0,$f" << zeroReg+1 << std::endl;
  std::cout << "c.eq.d" << indent << "$f" << zeroReg << ",$f" << immReg << std::endl;
  std::cout << "bc1t" << indent << "$" << label1 << ":" << std::endl;
  std::cout << "nop" << std::endl;

  branches[1]->generateMIPSDouble(indent,immReg,frameContext);
  std::cout << "mtc1" << indent << "$0,$f" << zeroReg << std::endl;
  std::cout << "mtc1" << indent << "$0,$f" << zeroReg+1 << std::endl;
  std::cout << "c.eq.d" << indent << "$f" << zeroReg << ",$f" << immReg << std::endl;
  std::cout << "bc1t" << indent << "$" << label1 << ":" << std::endl;
  std::cout << "nop" << std::endl;

  int immReg2 = frameContext->alloCalReg();
  frameContext->dealloReg(immReg2);
  std::cout << "li" << indent << "$" << immReg2 << ",1"<<std::endl;
  std::cout << "b" << indent << "$" << label2 << std::endl;
  std::cout << "nop" << std::endl;

  std::cout << "$" << label1 << ":" <<std::endl;
  std::cout << "move" << indent << "$" << immReg2 << ",$0" << std::endl;

  std::cout << "$" << label2 << ":" <<std::endl;
  std::cout << "mtc1" << indent << "$" << immReg2 << ",$f" << destReg << std::endl;
  std::cout << "cvt.d.w" << indent << "$f" << destReg << ",$f" << destReg << std::endl;
}