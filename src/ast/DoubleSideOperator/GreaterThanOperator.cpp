#include "ast/DoubleSideOperator/GreaterThanOperator.hpp"

GreaterThanOperator::GreaterThanOperator(NodePtr Left, NodePtr Right)
{
  branches.push_back(Left);
  branches.push_back(Right);
}

GreaterThanOperator::~GreaterThanOperator()
{
  delete branches[0];
  delete branches[1];
}

// Visualizing
void GreaterThanOperator::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "GreaterThan [" << std::endl;
  dst << indent << indent << "Left ["<<std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << indent << "Right ["<<std::endl;
  branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// Context
void GreaterThanOperator::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
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
void GreaterThanOperator::generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const
{
  switch(type){
    case _float:{
      std::string number_label = frameContext->getFloatDoubleLabel(1.0, type);
      std::string label1 = makeName("L");
      std::string label2 = makeName("L");
      int leftReg = frameContext->alloFloatCalReg();
      int rightReg = frameContext->alloFloatCalReg();
      frameContext->dealloFloatReg(leftReg);
      frameContext->dealloFloatReg(rightReg);

      branches[0]->generateMIPS(indent,leftReg,frameContext);
      frameContext->alloFloatReg(leftReg);

      branches[1]->generateMIPS(indent,rightReg,frameContext);
      frameContext->dealloFloatReg(leftReg);
      std::cout <<"c.lt.s" << indent << "$f"<<rightReg<<",$f"<<leftReg<<std::endl;
      std::cout << "bc1f" << indent << "$" << label1 << std::endl;
      std::cout << "nop" << std::endl;

      int addressReg = frameContext->alloCalReg();
      frameContext->dealloReg(addressReg);

      std::cout << "lui" << indent << "$" << addressReg << ",%hi($" << number_label << ")" <<std::endl;
      std::cout << "lwc1" << indent << "$f" << destReg << ",%lo($" << number_label << ")($" << addressReg << ")" << std::endl;
      std::cout << "b" << indent << "$" << label2 << std::endl;
      std::cout << "nop" << std::endl;

      std::cout << "$" << label1 << ":" << std::endl;
      std::cout << "mtc1" << indent << "$0,$f" << destReg << std::endl;
      std::cout << "nop" << std::endl;

      std::cout << "$" << label2 << ":" << std::endl;
      break;
    }
    case _double:{
      std::string number_label = frameContext->getFloatDoubleLabel(1.0, type);
      std::string label1 = makeName("L");
      std::string label2 = makeName("L");
      int leftReg = frameContext->alloFloatCalReg();
      int rightReg = frameContext->alloFloatCalReg();
      frameContext->dealloFloatReg(leftReg);
      frameContext->dealloFloatReg(rightReg);

      branches[0]->generateMIPSDouble(indent,leftReg,frameContext);
      frameContext->alloFloatReg(leftReg);

      branches[1]->generateMIPSDouble(indent,rightReg,frameContext);
      frameContext->dealloFloatReg(leftReg);
      std::cout <<"c.lt.d" << indent << "$f"<<rightReg<<",$f"<<leftReg<<std::endl;
      std::cout << "bc1f" << indent << "$" << label1 << std::endl;
      std::cout << "nop" << std::endl;

      int addressReg = frameContext->alloCalReg();
      frameContext->dealloReg(addressReg);

      std::cout << "lui" << indent << "$" << addressReg << ",%hi($" << number_label << ")" <<std::endl;
      std::cout << "lwc1" << indent << "$f" << destReg << ",%lo($" << number_label << "+4)($" << addressReg << ")" << std::endl;
      std::cout << "lwc1" << indent << "$f" << destReg+1 << ",%lo($" << number_label << ")($" << addressReg << ")" << std::endl;
      std::cout << "b" << indent << "$" << label2 << std::endl;
      std::cout << "nop" << std::endl;

      std::cout << "$" << label1 << ":" << std::endl;
      std::cout << "mtc1" << indent << "$0,$f" << destReg << std::endl;
      std::cout << "mtc1" << indent << "$0,$f" << destReg+1 << std::endl;
      std::cout << "nop" << std::endl;

      std::cout << "$" << label2 << ":" << std::endl;
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
      
      // left and right reg are switched around because slt is used for greater than
      std::cout << "slt" << indent << "$"<<destReg<< ",$"<<rightReg<<",$"<<leftReg<<std::endl; 
      std::cout << "andi" << indent << "$" << destReg << ",$" << destReg << ",0x00ff"<<std::endl;

      frameContext->deleteReservedRegsForFunctionCall(leftReg);
      frameContext->deleteReservedRegsForFunctionCall(rightReg);
    }
  }
}

// Functions
bool GreaterThanOperator::isAssignmentExp() const
{
  return false;
}
void GreaterThanOperator::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
  branches[1]->countFunctionParameter(functionCallParameterOffset);
}

int GreaterThanOperator::getValue() const{
  return (branches[0]->getValue() > branches[1]->getValue());
}

enum Specifier GreaterThanOperator::returnType() const{
  return type;
}

void GreaterThanOperator::generateMIPSDouble(std::string indent, int destReg, ContextPtr frameContext) const{ 
  std::string number_label = frameContext->getFloatDoubleLabel(1.0, type);
  std::string label1 = makeName("L");
  std::string label2 = makeName("L");
  int leftReg = frameContext->alloFloatCalReg();
  int rightReg = frameContext->alloFloatCalReg();
  frameContext->dealloFloatReg(leftReg);
  frameContext->dealloFloatReg(rightReg);

  branches[0]->generateMIPSDouble(indent,leftReg,frameContext);
  frameContext->alloFloatReg(leftReg);

  branches[1]->generateMIPSDouble(indent,rightReg,frameContext);
  frameContext->dealloFloatReg(leftReg);
  std::cout <<"c.lt.d" << indent << "$f"<<rightReg<<",$f"<<leftReg<<std::endl;
  std::cout << "bc1f" << indent << "$" << label1 << std::endl;
  std::cout << "nop" << std::endl;

  int addressReg = frameContext->alloCalReg();
  frameContext->dealloReg(addressReg);

  std::cout << "lui" << indent << "$" << addressReg << ",%hi($" << number_label << ")" <<std::endl;
  std::cout << "lwc1" << indent << "$f" << destReg << ",%lo($" << number_label << "+4)($" << addressReg << ")" << std::endl;
  std::cout << "lwc1" << indent << "$f" << destReg+1 << ",%lo($" << number_label << ")($" << addressReg << ")" << std::endl;
  std::cout << "b" << indent << "$" << label2 << std::endl;
  std::cout << "nop" << std::endl;

  std::cout << "$" << label1 << ":" << std::endl;
  std::cout << "mtc1" << indent << "$0,$f" << destReg << std::endl;
  std::cout << "mtc1" << indent << "$0,$f" << destReg+1 << std::endl;
  std::cout << "nop" << std::endl;

  std::cout << "$" << label2 << ":" << std::endl;
}