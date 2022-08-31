#include "ast/SingleSideOperator/PostIncrementOperator.hpp"

PostIncrementOperator::PostIncrementOperator(NodePtr Left)
{
  branches.push_back(Left);
}

PostIncrementOperator::~PostIncrementOperator()
{
  delete branches[0];
}

// Visualization
void PostIncrementOperator::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "PostIncrement [" << std::endl;
  dst << indent << "Target ["<<std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// Context
void PostIncrementOperator::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal) {
  blockContext = frameContext;
  branches[0]->generateContext(frameContext,parameterOffset,isGlobal   );
  type = branches[0]->returnType();
}

// MIPS
void PostIncrementOperator::generateMIPS(std::string indent, int destReg, ContextPtr frameContext ) const
{ 
  // find the register that is not the destReg; the immReg can then be used more often to avoid (possible errors?)
  int offset = frameContext->getOffset(branches[0]->returnIdentifier());
  switch (type){
    case _float:{
      std::string number_label = frameContext->getFloatDoubleLabel(1.0, type);
   
      frameContext->alloFloatReg(destReg);
      int immReg = frameContext->alloFloatCalReg(); 
      frameContext->dealloFloatReg(destReg);
      frameContext->dealloFloatReg(immReg);

      branches[0]->generateMIPS(indent,destReg,frameContext);
      int addressReg = frameContext->alloCalReg();
      std::cout << "lui" << indent << "$" << addressReg << ",%hi($" << number_label << ")" << std::endl;
      std::cout << "lwc1" << indent << "$f" << immReg << ",%lo($" << number_label << ")($" << addressReg << ")" << std::endl; 
      std::cout << "add.s" << indent << "$f" << immReg << ",$f" << destReg << ",f$" << immReg << std::endl;
      std::cout << "swc1" << indent << "$" << immReg << "," << offset << "($fp)" << std::endl;
      break;
    }
    case _double:{
      std::string number_label = frameContext->getFloatDoubleLabel(1.0, type);
      frameContext->alloFloatReg(destReg);
      int immReg = frameContext->alloFloatCalReg(); 
      frameContext->dealloFloatReg(destReg);
      frameContext->dealloFloatReg(immReg);

      branches[0]->generateMIPSDouble(indent,destReg,frameContext);
      int addressReg = frameContext->alloCalReg();
      std::cout << "lui" << indent << "$" << addressReg << ",%hi($" << number_label << ")" << std::endl;
      std::cout << "lwc1" << indent << "$f" << immReg << ",%lo($" << number_label << "+4)($" << addressReg << ")" << std::endl; 
      std::cout << "lwc1" << indent << "$f" << immReg+1 << ",%lo($" << number_label << ")($" << addressReg << ")" << std::endl; 
      std::cout << "add.d" << indent << "$f" << immReg << ",$f" << destReg << ",f$" << immReg << std::endl;
      std::cout << "swc1" << indent << "$" << immReg << "," << offset+4 << "($fp)" << std::endl;
      std::cout << "swc1" << indent << "$" << immReg+1 << "," << offset << "($fp)" << std::endl;
      break;
    }
    default:{
      frameContext->alloReg(destReg);
      int immReg = frameContext->alloCalReg(); 
      frameContext->dealloReg(destReg);
      frameContext->dealloReg(immReg);
      branches[0]->generateMIPS(indent,destReg,frameContext);

      if (branches[0]->getHasPointerForArithmetic()==true){
        std::cout << "addiu" << indent << "$"<<immReg<< ",$"<<destReg<<",4"<<std::endl;
      }
      else{
        std::cout << "addiu" << indent << "$"<<immReg<< ",$"<<destReg<<",1"<<std::endl;
      }
      frameContext->addReservedRegsForFunctionCall(destReg);
      std::cout << "sw" << indent << "$" << immReg << "," << offset << "($fp)" << std::endl;
    }
  }
  
}





// Functions

void PostIncrementOperator::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
}

bool PostIncrementOperator::getHasPointerForArithmetic() const{
  return branches[0]->getHasPointerForArithmetic();
}

void PostIncrementOperator::generateMIPSDouble(std::string indent, int destReg, ContextPtr frameContext) const{
  int offset = frameContext->getOffset(branches[0]->returnIdentifier());
  std::string number_label = frameContext->getFloatDoubleLabel(1.0, type);
  frameContext->alloFloatReg(destReg);
  int immReg = frameContext->alloFloatCalReg(); 
  frameContext->dealloFloatReg(destReg);
  frameContext->dealloFloatReg(immReg);

  branches[0]->generateMIPSDouble(indent,destReg,frameContext);
  int addressReg = frameContext->alloCalReg();
  std::cout << "lui" << indent << "$" << addressReg << ",%hi($" << number_label << ")" << std::endl;
  std::cout << "lwc1" << indent << "$f" << immReg << ",%lo($" << number_label << "+4)($" << addressReg << ")" << std::endl; 
  std::cout << "lwc1" << indent << "$f" << immReg+1 << ",%lo($" << number_label << ")($" << addressReg << ")" << std::endl; 
  std::cout << "add.d" << indent << "$f" << immReg << ",$f" << destReg << ",f$" << immReg << std::endl;
  std::cout << "swc1" << indent << "$" << immReg << "," << offset+4 << "($fp)" << std::endl;
  std::cout << "swc1" << indent << "$" << immReg+1 << "," << offset << "($fp)" << std::endl;
}