#include "ast/Label/Default.hpp"

// Constructor
Default::Default(NodePtr defaultBranch)
{
  branches.push_back(defaultBranch);
}

// Destructor
Default::~Default()
{
  delete branches[0];
}

void Default::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Default [" << std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// Context 
void Default::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  branches[0]->generateContext(frameContext, parameterOffset, isGlobal);
  blockContext = frameContext;
}

void Default::generateMIPS(std::string indent, int dstReg, ContextPtr frameContext ) const 
{
  std::cout << "$" << labelName << ":" << std::endl; 
  branches[0]->generateMIPS(indent, dstReg, frameContext);
}

void Default::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
}

void Default::getCaseInfo(std::vector<std::tuple<int, std::string>> &casesVector) {}

void Default::getDefaultInfo(std::vector<std::string> &defaultVector) {
    labelName = makeName("L");
    defaultVector.push_back(labelName);
}

