#include "ast/SingleSideOperator/LogicalNotOperator.hpp"

LogicalNotOperator::LogicalNotOperator(NodePtr Left)
{
  branches.push_back(Left);
}

LogicalNotOperator::~LogicalNotOperator()
{
  delete branches[0];
}

// Visualization
void LogicalNotOperator::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Not [" << std::endl;
  dst << indent << "Target ["<<std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// Context
void LogicalNotOperator::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal ) {
  blockContext = frameContext;
  branches[0]->generateContext(frameContext,parameterOffset,isGlobal);
  type = branches[0]->returnType();
}

// MIPS
void LogicalNotOperator::generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const
{
  switch (type){
    case _float:{
      std::string number_label = frameContext->getFloatDoubleLabel(1, type);
      std::string label1 = makeName("L");
      std::string label2 = makeName("L");
      int leftReg = frameContext->alloFloatCalReg();
      int rightReg = frameContext->alloFloatCalReg();
      frameContext->dealloFloatReg(leftReg);
      frameContext->dealloFloatReg(rightReg);

      branches[0]->generateMIPS(indent,leftReg,frameContext);
      
      std::cout << "mtc1" << indent << "$0,$f" << rightReg << std::endl;

      std::cout <<"c.eq.s" << indent << "$f"<<leftReg<<",$f"<<rightReg<<std::endl;
      std::cout << "bc1f" << indent << "$" << label1 << std::endl;
      std::cout << "nop" << std::endl;

      int addressReg = frameContext->alloCalReg();
      frameContext->dealloReg(addressReg);
      std::cout << "lui" << indent << "$" << addressReg << ",%hi($" << number_label << ")" << std::endl;
      std::cout << "lwc1" << indent << "$f" << destReg << ",%lo($" << number_label << ")($" << addressReg << ")" << std::endl;
      std::cout << "b" << indent << "$" << label2 << std::endl;
      std::cout << "nop" << std::endl;

      std::cout << "$" << label1 << ":" << std::endl;
      std::cout << "mtc1" << indent << "$0" << ",$f" << destReg << std::endl;

      std::cout << "$" << label2 << ":" << std::endl;

      break;
    }
    case _double:{
      std::string number_label = frameContext->getFloatDoubleLabel(1, type);
      std::string label1 = makeName("L");
      std::string label2 = makeName("L");
      int leftReg = frameContext->alloFloatCalReg();
      int rightReg = frameContext->alloFloatCalReg();
      frameContext->dealloFloatReg(leftReg);
      frameContext->dealloFloatReg(rightReg);

      branches[0]->generateMIPS(indent,leftReg,frameContext);
      
      std::cout << "mtc1" << indent << "$0,$f" << rightReg << std::endl;
      std::cout << "mtc1" << indent << "$0,$f" << rightReg+1 << std::endl;

      std::cout <<"c.eq.d" << indent << "$f"<<leftReg<<",$f"<<rightReg<<std::endl;
      std::cout << "bc1f" << indent << "$" << label1 << std::endl;
      std::cout << "nop" << std::endl;

      int addressReg = frameContext->alloCalReg();
      frameContext->dealloReg(addressReg);
      std::cout << "lui" << indent << "$" << addressReg << ",%hi($" << number_label << ")" << std::endl;
      std::cout << "lwc1" << indent << "$f" << destReg << ",%lo($" << number_label << "+4)($" << addressReg << ")" << std::endl;
      std::cout << "lwc1" << indent << "$f" << destReg+1 << ",%lo($" << number_label << ")($" << addressReg << ")" << std::endl;
      std::cout << "b" << indent << "$" << label2 << std::endl;
      std::cout << "nop" << std::endl;

      std::cout << "$" << label1 << ":" << std::endl;
      std::cout << "mtc1" << indent << "$0" << ",$f" << destReg << std::endl;
      std::cout << "mtc1" << indent << "$0" << ",$f" << destReg+1 << std::endl;

      std::cout << "$" << label2 << ":" << std::endl;

      break;
    }
    default:{
      branches[0]->generateMIPS(indent,destReg,frameContext);
      std::cout << "sltu" << indent << "$" << destReg << ",$" << destReg << ",1" << std::endl;
      std::cout << "andi" << indent << "$" << destReg << ",$" << destReg << ",0x00ff" << std::endl;
    }
  } 
  
}


// Functions
void LogicalNotOperator::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
}

int LogicalNotOperator::getValue() const{
  return (!branches[0]->getValue());
}

void LogicalNotOperator::generateMIPSDouble(std::string indent, int destReg, ContextPtr frameContext) const{
  std::string number_label = frameContext->getFloatDoubleLabel(1, type);
  std::string label1 = makeName("L");
  std::string label2 = makeName("L");
  int leftReg = frameContext->alloFloatCalReg();
  int rightReg = frameContext->alloFloatCalReg();
  frameContext->dealloFloatReg(leftReg);
  frameContext->dealloFloatReg(rightReg);

  branches[0]->generateMIPS(indent,leftReg,frameContext);
  
  std::cout << "mtc1" << indent << "$0,$f" << rightReg << std::endl;
  std::cout << "mtc1" << indent << "$0,$f" << rightReg+1 << std::endl;

  std::cout <<"c.eq.d" << indent << "$f"<<leftReg<<",$f"<<rightReg<<std::endl;
  std::cout << "bc1f" << indent << "$" << label1 << std::endl;
  std::cout << "nop" << std::endl;

  int addressReg = frameContext->alloCalReg();
  frameContext->dealloReg(addressReg);
  std::cout << "lui" << indent << "$" << addressReg << ",%hi($" << number_label << ")" << std::endl;
  std::cout << "lwc1" << indent << "$f" << destReg << ",%lo($" << number_label << "+4)($" << addressReg << ")" << std::endl;
  std::cout << "lwc1" << indent << "$f" << destReg+1 << ",%lo($" << number_label << ")($" << addressReg << ")" << std::endl;
  std::cout << "b" << indent << "$" << label2 << std::endl;
  std::cout << "nop" << std::endl;

  std::cout << "$" << label1 << ":" << std::endl;
  std::cout << "mtc1" << indent << "$0" << ",$f" << destReg << std::endl;
  std::cout << "mtc1" << indent << "$0" << ",$f" << destReg+1 << std::endl;

  std::cout << "$" << label2 << ":" << std::endl;
}