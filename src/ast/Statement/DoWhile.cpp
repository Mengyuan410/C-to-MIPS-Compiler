#include "ast/Statement/DoWhile.hpp"

// Constructors
DoWhile::DoWhile(NodePtr whileBlock, NodePtr condition)
{
  branches.push_back(whileBlock);
  branches.push_back(condition);
}

// Destructor
DoWhile::~DoWhile()
{
  delete branches[0];
  delete branches[1];
}

// Visualising
void DoWhile::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Do [" << std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "] endDo" << std::endl;
  dst << indent << "Condition [" << std::endl;
  branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "] endCondition" << std::endl;
}

// Context 
void DoWhile::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  label2 = makeName("C"); // for 'continue'
  frameContext->updateContinueLabel(label2);

  label3 = makeName("B"); // for 'continue'
  frameContext->updateContinueLabel(label3);

  branches[0]->generateContext(frameContext, parameterOffset, isGlobal);
  branches[1]->generateContext(frameContext, parameterOffset, isGlobal);
  blockContext = frameContext;
}

// MIPS
void DoWhile::generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const 
{
  std::string label1 = makeName("L"); 
  
  // in while loop
  std::cout << "$" << label1 << ":" << std::endl;
  branches[0]->generateMIPS(indent, dstReg, frameContext);

  std::cout << "$" << label2 << ":" << std::endl; // for 'continue' to jump to
  // Evaluate condition
  branches[1]->generateMIPS(indent, dstReg, frameContext);
  frameContext->clearReservedRegsForFunctionCall();
  std::cout << "bne $" << dstReg << ",$0,$" << label1<< std::endl;

  std::cout << "$" << label3 << ":" << std::endl; // 'break' jumps to here
  std::cout << "nop" << std::endl;
}

// Functions
void DoWhile::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
  branches[1]->countFunctionParameter(functionCallParameterOffset);
}
