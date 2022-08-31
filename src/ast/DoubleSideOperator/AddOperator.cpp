#include "ast/DoubleSideOperator/AddOperator.hpp"

AddOperator::AddOperator(NodePtr Left, NodePtr Right)
{
  branches.push_back(Left);
  branches.push_back(Right);
}

AddOperator::~AddOperator()
{
  delete branches[0];
  delete branches[1];
}

// Visualization
void AddOperator::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Add [" << std::endl;
  dst << indent << "Left ["<<std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
  dst << indent << "Right ["<<std::endl;
  branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// Context
void AddOperator::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
  branches[0]->generateContext(frameContext,parameterOffset,isGlobal);
  branches[1]->generateContext(frameContext,parameterOffset,isGlobal);
  

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
void AddOperator::generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const
{
  switch(type){
    case _float:{
      if (branches[1]->getHasPointerForArithmetic()==true || branches[0]->getHasPointerForArithmetic()==true){ //pointer arithmetic
        int leftReg = frameContext->alloCalReg();
        int rightReg = frameContext->alloCalReg();
        frameContext->dealloReg(leftReg);
        frameContext->dealloReg(rightReg);

        branches[0]->generateMIPS(indent,leftReg,frameContext);
        frameContext->alloReg(leftReg);
        if (branches[1]->getHasPointerForArithmetic()==true ){
          std::cout << "sll" << indent << "$" << leftReg << ",$" << leftReg << ",2" << std::endl;
        }

        branches[1]->generateMIPS(indent,rightReg,frameContext);
      
        frameContext->dealloReg(leftReg);

        if (branches[0]->getHasPointerForArithmetic()==true){
          std::cout << "sll" << indent << "$" << rightReg << ",$" << rightReg << ",2" << std::endl;
        }
        std::cout <<"addu" << indent << "$"<<destReg<< ",$"<<leftReg<<",$"<<rightReg<<std::endl;
      }
      else{
        int leftReg = frameContext->alloFloatCalReg();
        int rightReg = frameContext->alloFloatCalReg();
        frameContext->dealloFloatReg(leftReg);
        frameContext->dealloFloatReg(rightReg);

        branches[0]->generateMIPS(indent,leftReg,frameContext);
        frameContext->alloFloatReg(leftReg);

        branches[1]->generateMIPS(indent,rightReg,frameContext);
        frameContext->dealloFloatReg(leftReg);
        std::cout <<"add.s" << indent << "$f"<<destReg<< ",$f"<<leftReg<<",$f"<<rightReg<<std::endl;
      }
      break;
    }
    case _double:{
      if (branches[1]->getHasPointerForArithmetic()==true || branches[0]->getHasPointerForArithmetic()==true){ //pointer arithmetic
        int leftReg = frameContext->alloCalReg();
        int rightReg = frameContext->alloCalReg();
        frameContext->dealloReg(leftReg);
        frameContext->dealloReg(rightReg);

        branches[0]->generateMIPS(indent,leftReg,frameContext);
        frameContext->addReservedRegsForFunctionCall(leftReg);
        frameContext->alloReg(leftReg);
        if (branches[1]->getHasPointerForArithmetic()==true ){
          std::cout << "sll" << indent << "$" << leftReg << ",$" << leftReg << ",3" << std::endl;
        }
        branches[1]->generateMIPS(indent,rightReg,frameContext);
        frameContext->addReservedRegsForFunctionCall(rightReg);
        frameContext->dealloReg(leftReg);

        if (branches[0]->getHasPointerForArithmetic()==true){
          std::cout << "sll" << indent << "$" << rightReg << ",$" << rightReg << ",3" << std::endl;
        }
        std::cout <<"addu" << indent << "$"<<destReg<< ",$"<<leftReg<<",$"<<rightReg<<std::endl;
      }
      else{
        int leftReg = frameContext->alloFloatCalReg();
        int rightReg = frameContext->alloFloatCalReg();
        frameContext->dealloFloatReg(leftReg);
        frameContext->dealloFloatReg(rightReg);
        branches[0]->generateMIPSDouble(indent,leftReg,frameContext);
        frameContext->alloFloatReg(leftReg);
    
        branches[1]->generateMIPSDouble(indent,rightReg,frameContext);
        frameContext->dealloFloatReg(leftReg);
        std::cout <<"add.d" << indent << "$f"<<destReg<< ",$f"<<leftReg<<",$f"<<rightReg<<std::endl;
      }
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
      if (branches[1]->getHasPointerForArithmetic()==true ){
        std::cout << "sll" << indent << "$" << leftReg << ",$" << leftReg << ",2" << std::endl;
      }

      branches[1]->generateMIPS(indent,rightReg,frameContext);
      frameContext->addReservedRegsForFunctionCall(rightReg);
      frameContext->dealloReg(leftReg);

      if (branches[0]->getHasPointerForArithmetic()==true){
        std::cout << "sll" << indent << "$" << rightReg << ",$" << rightReg << ",2" << std::endl;
      }
      std::cout <<"addu" << indent << "$"<<destReg<< ",$"<<leftReg<<",$"<<rightReg<<std::endl;
      frameContext->deleteReservedRegsForFunctionCall(leftReg);
      frameContext->deleteReservedRegsForFunctionCall(rightReg);
    }
  }
}

void AddOperator::generateMIPSDouble(std::string indent, int destReg, ContextPtr frameContext) const
{
  int leftReg = frameContext->alloFloatCalReg();
  int rightReg = frameContext->alloFloatCalReg();
  frameContext->dealloFloatReg(leftReg);
  frameContext->dealloFloatReg(rightReg);

  branches[0]->generateMIPSDouble(indent,leftReg,frameContext);
  
  frameContext->alloFloatReg(leftReg);

  branches[1]->generateMIPSDouble(indent,rightReg,frameContext);

  frameContext->dealloFloatReg(leftReg);
  std::cout <<"add.d" << indent << "$f"<<destReg<< ",$f"<<leftReg<<",$f"<<rightReg<<std::endl;
}

// Functions

bool AddOperator::isAssignmentExp() const
{
  return false;
}
void AddOperator::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
  branches[1]->countFunctionParameter(functionCallParameterOffset);
}

int AddOperator::getValue() const{
  return (branches[0]->getValue()+branches[1]->getValue());
}

bool AddOperator::getHasPointerForArithmetic() const{
  bool leftbool = branches[0]->getHasPointerForArithmetic();
  bool rightbool = branches[1]->getHasPointerForArithmetic();
  return leftbool || rightbool;
}

enum Specifier AddOperator::returnType() const{
  return type;
}