#include "ast/SingleSideOperator/AddressOfOperator.hpp"

AddressOfOperator::AddressOfOperator(NodePtr Left)
{
  branches.push_back(Left);
}

AddressOfOperator::~AddressOfOperator()
{
  delete branches[0];
}

// Visualization
void AddressOfOperator::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "AddressOf [" << std::endl;
  dst << indent << "Target ["<<std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// MIPS
void AddressOfOperator::generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const
{
  bool isArray = branches[0]->getIsArray();
  std::string id;
  if (isArray){
    id = branches[0]->returnIdentifierIndexed();
  }
  else {
    id = branches[0]->returnIdentifier();
  }
  int offset = frameContext->getOffset(id);
  std::cout << "addiu" << indent << "$" << destReg << ",$fp," << offset << std::endl;
}

// Context
void AddressOfOperator::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
  branches[0]->generateContext(frameContext,parameterOffset,isGlobal   );
}

// Functions
void AddressOfOperator::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
}

int AddressOfOperator::getValue() const{
  return (~branches[0]->getValue());
}

bool AddressOfOperator::getHasPointerForArithmetic() const{
  return true;
}