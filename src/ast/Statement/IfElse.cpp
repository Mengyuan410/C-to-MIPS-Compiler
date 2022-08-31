#include "ast/Statement/IfElse.hpp"

// Constructors
IfElse::IfElse(NodePtr condition, NodePtr ifBranch, NodePtr elseBranch)
{
  branches.push_back(condition);
  branches.push_back(ifBranch);
  branches.push_back(elseBranch);
}

// Destructor
IfElse::~IfElse()
{
  delete branches[0];
  delete branches[1];
  delete branches[2];
}

// Visualising
void IfElse::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Condition [" << std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "] endCondition" << std::endl;
  dst << indent << "If Scope [" << std::endl;
  branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "] endifBranch" << std::endl;
  dst << indent << "Else Scope [" << std::endl;
  branches[2]->generateParser(dst, indent+"  ");
  dst << indent << "] endelseBranch" << std::endl;
}

// Context 
void IfElse::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  branches[0]->generateContext(frameContext, parameterOffset, isGlobal);
  branches[1]->generateContext(frameContext, parameterOffset, isGlobal);
  branches[2]->generateContext(frameContext, parameterOffset, isGlobal);
  blockContext = frameContext;
}

// MIPS
void IfElse::generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const 
{
   // Evaluate condition
  branches[0]->generateMIPS(indent, dstReg, frameContext);
  frameContext->clearReservedRegsForFunctionCall();

  std::string label1 = makeName("L");
  std::string label2 = makeName("L");

  // If branch
  std::cout << "beq $" << dstReg << ",$0,$" << label1 << std::endl;

  branches[1]->generateMIPS(indent, dstReg, frameContext   );
  std::cout << "b" << indent << "$" << label2 << std::endl;

  // else branch
  std::cout << "$" << label1 << ":" << std::endl;
  branches[2]->generateMIPS(indent, dstReg, frameContext   );

  // end if
  std::cout << "$" << label2 << ":" << std::endl;
}

void IfElse::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
  branches[1]->countFunctionParameter(functionCallParameterOffset);
  branches[2]->countFunctionParameter(functionCallParameterOffset);
}
