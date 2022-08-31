#include "ast/Statement/ContinueStatement.hpp"

// Constructors
ContinueStatement::ContinueStatement() {}

// Destructor
ContinueStatement::~ContinueStatement() {}


// Visualising
void ContinueStatement::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Continue: []" << std::endl;
}

// Context
void ContinueStatement::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
}

// MIPS
void ContinueStatement::generateMIPS(std::string indent, int destReg, ContextPtr frameContext ) const
{
  std::cout << "b" << indent << "$" << frameContext->getContinueLabel() << std::endl;
}

void ContinueStatement::countFunctionParameter(int* functionCallParameterOffset) const{}
