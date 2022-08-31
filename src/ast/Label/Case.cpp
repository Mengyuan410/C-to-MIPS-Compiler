#include "ast/Label/Case.hpp"

// Constructor
Case::Case(NodePtr condition, NodePtr caseBranch)
{
  branches.push_back(condition);
  branches.push_back(caseBranch);
}

// Destructor
Case::~Case()
{
  delete branches[0];
  delete branches[1];
}

void Case::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Case condition [" << std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
  dst << indent << "Execute [" << std::endl;
  branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "] endCase" << std::endl;
}

// Context 
void Case::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  branches[1]->generateContext(frameContext, parameterOffset, isGlobal);
  blockContext = frameContext;
}

void Case::generateMIPS(std::string indent, int destReg, ContextPtr frameContext ) const
{
  std::cout << "$" << labelName << ":" << std::endl; 
  branches[1]->generateMIPS(indent, destReg, blockContext);
}

void Case::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
  branches[1]->countFunctionParameter(functionCallParameterOffset);
}

void Case::getCaseInfo(std::vector<std::tuple<int, std::string>> &casesVector) {
  int caseCondition = branches[0] -> integerValue(); // we kind of assumed it is an integer -> need to double check....
  labelName = makeName("L");
  std::tuple<int, std::string> caseInfo = std::make_tuple(caseCondition, labelName);
  casesVector.push_back(caseInfo);
}
