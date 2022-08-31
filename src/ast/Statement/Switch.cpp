#include "ast/Statement/Switch.hpp"

// Constructors
Switch::Switch(NodePtr condition, NodePtr SwitchBranch)
{
  branches.push_back(condition);
  branches.push_back(SwitchBranch);
}

// Destructor
Switch::~Switch()
{
  delete branches[0];
  delete branches[1];
}

// Visualising
void Switch::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "SwitchCondition [" << std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "] endCondition" << std::endl;
  dst << indent << "SwitchBranch [" << std::endl;
  branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "] endSwitchBranch" << std::endl;
}

// Context 
void Switch::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  label = makeName("B");
  frameContext->updateBreakLabel(label);
  
  branches[0]->generateContext(frameContext, parameterOffset, isGlobal );
  branches[1]->generateContext(frameContext, parameterOffset, isGlobal  );
  blockContext = frameContext;
}

// MIPS
void Switch::generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const 
{

  int leftReg = frameContext->alloCalReg();
  int rightReg = frameContext->alloCalReg();
  frameContext->dealloReg(leftReg);
  frameContext->dealloReg(rightReg);
  
  std::vector<std::tuple<int,std::string>> switchCases = branches[1]->getSwitchCases();
  std::vector<std::string> defaultLabel = branches[1]->getSwitchDefault();

  for (unsigned i = 0; i<switchCases.size(); i++){
    branches[0]->generateMIPS(indent,leftReg,frameContext   );
    frameContext->clearReservedRegsForFunctionCall();
    std::cout << "li" << indent << "$" << rightReg << "," << std::get<0>(switchCases[i]) << std::endl;
    std::cout << "beq" << indent << "$" << rightReg << ",$" << leftReg << ",$" << std::get<1>(switchCases[i]) << std::endl;
    std::cout << "nop" << std::endl;
  }

  if (defaultLabel.size()!=0){
    std::cout << "b" << indent << "$" << defaultLabel[0] << std::endl;
    std::cout << "nop" << std::endl;
    branches[1]->generateMIPS(indent, dstReg, frameContext   );

    std::cout << "$" << label << ":" << std::endl; // 'break' jumps to here
    std::cout << "nop" << std::endl;
  }
  else{
    std::string defaultMakeLabel = makeName("L");
    std::cout << "b" << indent << "$" << defaultMakeLabel << std::endl;
    std::cout << "nop" << std::endl;
    branches[1]->generateMIPS(indent, dstReg, frameContext   );
    std::cout << "$" << defaultMakeLabel << ":" << std::endl;

    std::cout << "$" << label << ":" << std::endl; // 'break' jumps to here
    std::cout << "nop" << std::endl;
  }
}

void Switch::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
  branches[1]->countFunctionParameter(functionCallParameterOffset);
}
