#include "ast/Statement/While.hpp"

// Constructors
While::While(NodePtr condition, NodePtr whileBlock)
{
  branches.push_back(condition);
  branches.push_back(whileBlock);
}

// Destructor
While::~While()
{
  delete branches[0];
  delete branches[1];
}

// Visualising
void While::generateParser(std::ostream &dst, std::string indent) const 
{
  dst << indent << "Condition [" << std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "] endCondition" << std::endl;
  dst << indent << "While [" << std::endl;
  branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "] endWhile" << std::endl;
}

// Context 
void While::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  label3 = makeName("C"); // for 'continue'
  frameContext->updateContinueLabel(label3);

  label4 = makeName("B");
  frameContext->updateBreakLabel(label4);

  branches[0]->generateContext(frameContext, parameterOffset, isGlobal );
  branches[1]->generateContext(frameContext, parameterOffset, isGlobal );
  blockContext = frameContext;
}

// MIPS
void While::generateMIPS(std::string indent, int dstReg, ContextPtr frameContext ) const 
{
  std::string label1 = makeName("L"); 
  std::string label2 = makeName("L");
  
  std::cout << "b" << indent << "$" << label1 << std::endl;

  // in while loop
  std::cout << "$" << label2 << ":" << std::endl;
  branches[1]->generateMIPS(indent, dstReg, frameContext);
  std::cout << std::endl;


  std::cout << "$" << label3 << ":" << std::endl;  // for 'continue' to jump to

  // Evaluate condition
  std::cout << "$" << label1 << ":" << std::endl;
  branches[0]->generateMIPS(indent, dstReg, frameContext);
  frameContext->clearReservedRegsForFunctionCall();
  std::cout << "bne $" << dstReg << ",$0,$" << label2 << std::endl;
  std::cout << "nop" << std::endl;

  std::cout << "$" << label4 << ":" << std::endl;
}

void While::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
  branches[1]->countFunctionParameter(functionCallParameterOffset);
}

