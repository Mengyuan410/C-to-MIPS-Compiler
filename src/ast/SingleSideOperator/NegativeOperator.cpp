#include "ast/SingleSideOperator/NegativeOperator.hpp"

NegativeOperator::NegativeOperator(NodePtr Left)
{
  branches.push_back(Left);
}

NegativeOperator::~NegativeOperator()
{
  delete branches[0];
}

// Visualization
void NegativeOperator::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Negative [" << std::endl;
  dst << indent << "Target ["<<std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// Context
void NegativeOperator::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
  branches[0]->generateContext(frameContext,parameterOffset,isGlobal   );
  type = branches[0]->returnType();
}

// MIPS
void NegativeOperator::generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const
{
  switch (type){
    case _float:{
      int leftReg = frameContext->alloCalReg();
      int rightReg = frameContext->alloCalReg();
      frameContext->dealloReg(leftReg);
      frameContext->dealloReg(rightReg);

      int floatReg = frameContext->alloFloatCalReg(); 
      frameContext->dealloFloatReg(floatReg);
      branches[0]->generateMIPS(indent,floatReg,frameContext);
      
      // this line is different to godbolt - to move it from float register
      std::cout << "mfc1" << indent << "$"<<leftReg<< ",$f" << floatReg << std::endl;

      std::cout << "li" << indent << "$"<<rightReg<< ",-2147483648" << std::endl;
      std::cout << "xor" << indent << "$"<<rightReg<< ",$" << leftReg << ",$" << rightReg << std::endl;

      // this line is different to godbolt - to move it to float register
      std::cout << "mtc1" << indent << "$"<<rightReg<< ",$f" << destReg << std::endl;
      break;
    }
    case _double:{
      // this part doesn't follow godbolt. godbolt uses normal registers but we used float registers
      int leftReg = frameContext->alloCalReg();
      int rightReg = frameContext->alloCalReg();
      frameContext->dealloReg(leftReg);
      frameContext->dealloReg(rightReg);

      int floatReg = frameContext->alloFloatCalReg(); 
      frameContext->dealloFloatReg(floatReg);

      branches[0]->generateMIPSDouble(indent,floatReg,frameContext);

      // this line is different to godbolt - to move the lower half from float register
      std::cout << "mfc1" << indent << "$"<<leftReg<< ",$f" << floatReg+1 << std::endl;

      std::cout << "li" << indent << "$"<<rightReg<< ",-2147483648" << std::endl;
      std::cout << "xor" << indent << "$"<<rightReg<< ",$" << leftReg << ",$" << rightReg << std::endl;

      // this line is different to godbolt - to move it to float register
      std::cout << "mtc1" << indent << "$"<<rightReg<< ",$f" << destReg+1 << std::endl;
      std::cout << "mov.s" << indent << "$f"<<destReg<< ",$f" << floatReg << std::endl;

      break;
    }
    default:{
      branches[0]->generateMIPS(indent,destReg,frameContext);
      std::cout << "subu" << indent << "$"<<destReg<< ",$0,$"<<destReg<<std::endl;
      break;
    }
  }
}

// Functions
void NegativeOperator::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
}

int NegativeOperator::getValue() const{
  return (-branches[0]->getValue());
}

void NegativeOperator::generateMIPSDouble(std::string indent, int destReg, ContextPtr frameContext) const{
  int leftReg = frameContext->alloCalReg();
  int rightReg = frameContext->alloCalReg();
  frameContext->dealloReg(leftReg);
  frameContext->dealloReg(rightReg);

  int floatReg = frameContext->alloFloatCalReg(); 
  frameContext->dealloFloatReg(floatReg);

  branches[0]->generateMIPSDouble(indent,floatReg,frameContext);

  // this line is different to godbolt - to move the lower half from float register
  std::cout << "mfc1" << indent << "$"<<leftReg<< ",$f" << floatReg+1 << std::endl;

  std::cout << "li" << indent << "$"<<rightReg<< ",-2147483648" << std::endl;
  std::cout << "xor" << indent << "$"<<rightReg<< ",$" << leftReg << ",$" << rightReg << std::endl;

  // this line is different to godbolt - to move it to float register
  std::cout << "mtc1" << indent << "$"<<rightReg<< ",$f" << destReg+1 << std::endl;
  std::cout << "mov.s" << indent << "$f"<<destReg<< ",$f" << floatReg  << std::endl;
}
