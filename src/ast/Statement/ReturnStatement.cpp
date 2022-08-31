#include "ast/Statement/ReturnStatement.hpp"

// Constructors
ReturnStatement::ReturnStatement(NodePtr returnObject)
{
  branches.push_back(returnObject);
}

ReturnStatement::ReturnStatement() {} // return nothing for void

// Destructor
ReturnStatement::~ReturnStatement()
{
  for(unsigned i = 0; i < branches.size(); i++){
    delete branches[i];
  }
}

// Visualising
void ReturnStatement::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Return: [" << std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// Context
void ReturnStatement::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  if (branches.size()!=0){
    branches[0]->generateContext(frameContext,parameterOffset,isGlobal   );
  }
  blockContext = frameContext;
  
}

// MIPS
void ReturnStatement::generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const
{
  
  if (branches.size() != 0){
    enum Specifier type = frameContext->getFunctionType();
    switch (type){
      case _float:
        branches[0]->generateMIPS(indent,0,frameContext); 
        break; // only use f0 as destreg when returning 
      case _double:
        branches[0]->generateMIPSDouble(indent,0,frameContext); 
        break; // only use f0 as destreg when returning 
      default:
        branches[0]->generateMIPS(indent,destReg,frameContext);
    }
  }
  frameContext->clearReservedRegsForFunctionCall();
  std::cout << "b" << indent << "$" << frameContext->getFunctionLabel() << std::endl;
  std::cout << "nop" << std::endl;
  
}

void ReturnStatement::countFunctionParameter(int* functionCallParameterOffset) const{
  if (branches.size() != 0){
    branches[0]->countFunctionParameter(functionCallParameterOffset);
  }
}

