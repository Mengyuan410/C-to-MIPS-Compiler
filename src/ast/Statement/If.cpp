#include "ast/Statement/If.hpp"

// Constructors
If::If(NodePtr condition, NodePtr ifBranch)
{
  branches.push_back(condition);
  branches.push_back(ifBranch);
}

// Destructor
If::~If()
{
  delete branches[0];
  delete branches[1];
}

// Visualising
void If::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Condition [" << std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "] endCondition" << std::endl;
  dst << indent << "If Scope [" << std::endl;
  branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "] endifBranch" << std::endl;
}

// Context 
void If::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  branches[0]->generateContext(frameContext, parameterOffset, isGlobal);
  branches[1]->generateContext(frameContext, parameterOffset, isGlobal);
  blockContext = frameContext;
}

// MIPS
void If::generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const 
{
  // Evaluate condition
  branches[0]->generateMIPS(indent, dstReg, frameContext   );
  frameContext->clearReservedRegsForFunctionCall();

  std::string label = makeName("L"); 

  // If branch
  std::cout << "beq $" << dstReg << ",$0,$" << label << std::endl;
  std::cout << "nop" << std::endl;

  branches[1]->generateMIPS(indent, dstReg, frameContext);

  std::cout << "$" << label << ":" << std::endl;
}

void If::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
  branches[1]->countFunctionParameter(functionCallParameterOffset);
}
