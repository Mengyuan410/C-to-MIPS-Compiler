#include "ast/Statement/For.hpp"

// Constructors
For::For(NodePtr initExpressionStatement, NodePtr conditionExpressionStatement, NodePtr loopExpression, NodePtr loopBlockStatement)
{
  branches.push_back(initExpressionStatement);
  branches.push_back(conditionExpressionStatement);
  branches.push_back(loopExpression);
  branches.push_back(loopBlockStatement);
}

For::For(NodePtr initExpressionStatement, NodePtr conditionExpressionStatement, NodePtr loopBlockStatement)
{
  branches.push_back(initExpressionStatement);

  branches.push_back(conditionExpressionStatement);
 
  branches.push_back(loopBlockStatement);

}

// Destructor
For::~For()
{
  for (unsigned i = 0; i<branches.size();i++){
    delete branches[i];
  }
}

// Visualising
void For::generateParser(std::ostream &dst, std::string indent) const 
{

  if (branches.size()==4){
    
    dst << indent << "For Initialization [" << std::endl;
    branches[0]->generateParser(dst, indent+"  ");
    dst << indent << "]" << std::endl;
    dst << indent << "For EndCondition [" << std::endl;
    branches[1]->generateParser(dst, indent+"  ");
    dst << indent << "]" << std::endl;
    dst << indent << "For In/Decrement [" << std::endl;
    branches[2]->generateParser(dst, indent+"  ");
    dst << indent << "]" << std::endl;
    dst << indent << "For Block [" << std::endl;
    branches[3]->generateParser(dst, indent+"  ");
    dst << indent << "]" << std::endl;
    }
  else{
      
    dst << indent << "For Initialization [" << std::endl;
    branches[0]->generateParser(dst, indent+"  ");
    dst << indent << "]" << std::endl;
    dst << indent << "For EndCondition [" << std::endl;
    branches[1]->generateParser(dst, indent+"  ");
    dst << indent << "]" << std::endl;
    dst << indent << "For Block [" << std::endl;
    branches[2]->generateParser(dst, indent+"  ");
    dst << indent << "]" << std::endl;
  }
  
}

// Context 
void For::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {

  label3 = makeName("C"); // for 'continue' to jump to
  frameContext->updateContinueLabel(label3);
  label4 = makeName("B");
  frameContext->updateBreakLabel(label4);
  
  for (unsigned i=0;i<branches.size();i++){
    branches[i]->generateContext(frameContext, parameterOffset, isGlobal);
  }
  blockContext = frameContext;
}

// MIPS
void For::generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const 
{
  std::string label1 = makeName("L"); 
  std::string label2 = makeName("L"); 

  // Initialization Condition
  branches[0]->generateMIPS(indent, dstReg, frameContext);
  frameContext->clearReservedRegsForFunctionCall();
  std::cout << "b" << indent << "$" << label1 << std::endl;

  if (branches.size()==4){
    // in for loop
    std::cout << std::endl;
    std::cout << "$" << label2 << ":" << std::endl;
    branches[3]->generateMIPS(indent, dstReg, frameContext);

    std::cout << "$" << label3 << ":" << std::endl; // 'continue' jumps to here
    std::cout << "nop" << std::endl;

    // In/Decrement 
    branches[2]->generateMIPS(indent, dstReg, frameContext);
    frameContext->clearReservedRegsForFunctionCall();
    std::cout << std::endl;

    // Evaluate condition
    std::cout << "$" << label1 << ":" << std::endl;
    branches[1]->generateMIPS(indent, dstReg, frameContext);
    frameContext->clearReservedRegsForFunctionCall();
    std::cout << "bne $" << dstReg << ",$0,$" << label2 << std::endl;
    std::cout << "nop" << std::endl;

    std::cout << "$" << label4 << ":" << std::endl; // 'break' jumps to here
    std::cout << "nop" << std::endl;
  }
  else{
    // in for loop
    std::cout << std::endl;
    std::cout << "$" << label2 << ":" << std::endl;
    branches[2]->generateMIPS(indent, dstReg, frameContext);

    // Evaluate condition
    std::cout << "$" << label3 << ":" << std::endl; // 'continue' jumps to here
    std::cout << "$" << label1 << ":" << std::endl;
    branches[1]->generateMIPS(indent, dstReg, frameContext);
    std::cout << "bne $" << dstReg << ",$0,$" << label2 << std::endl;
    std::cout << "nop" << std::endl;

    std::cout << "$" << label4 << ":" << std::endl; // 'break' jumps to here
    std::cout << "nop" << std::endl;
  }
}

void For::countFunctionParameter(int* functionCallParameterOffset) const
{
  for (unsigned i=0;i<branches.size();i++){
    branches[i]->countFunctionParameter(functionCallParameterOffset);
  }
}
