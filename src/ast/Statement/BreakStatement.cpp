#include "ast/Statement/BreakStatement.hpp"

// Constructors
BreakStatement::BreakStatement() {}

// Destructor
BreakStatement::~BreakStatement() {}

// Visualising
void BreakStatement::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Break: []" << std::endl;
}

// Context
void BreakStatement::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
}

// MIPS
void BreakStatement::generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const
{
  std::cout << "b" << indent << "$" << frameContext->getBreakLabel() << std::endl;
}

void BreakStatement::countFunctionParameter(int* functionCallParameterOffset) const{}
