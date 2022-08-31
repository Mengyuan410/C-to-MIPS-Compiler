#include "ast/SingleSideOperator/SizeOfOperator.hpp"

SizeOfOperator::SizeOfOperator(NodePtr _type)
{
  branches.push_back(_type);
}

SizeOfOperator::~SizeOfOperator()
{
  delete branches[0];
}

// Visualization
void SizeOfOperator::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "SizeOf [" << std::endl;
  dst << indent << "Target ["<<std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// MIPS
void SizeOfOperator::generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const
{
  enum Specifier type = branches[0]->DeclarationType();
  int size = 0;
  switch(type){
      case _char:{
          size = 1;
          break;
      }
      case _double:{
          size = 8;
          break;
      }
      default:{
          size = 4;
      }
  }
  std::cout <<"li" << indent << "$" << destReg << "," << size << std::endl;
}

// Context
void SizeOfOperator::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
  branches[0]->generateContext(frameContext,parameterOffset,  isGlobal);
}

// Functions
