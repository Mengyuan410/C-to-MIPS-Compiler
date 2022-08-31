#include "ast/Definition/FunctionDefinition.hpp"
#include <math.h>

// Constructors
FunctionDefinition::FunctionDefinition(NodePtr Declaration, NodePtr Scope)
{
  branches.push_back(Declaration);
  branches.push_back(Scope);
}

// Destructor
FunctionDefinition::~FunctionDefinition()
{
  delete branches[0];
  delete branches[1];
}

// Visualising
void FunctionDefinition::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Function Definition [" << std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// Context
void FunctionDefinition::generateContext(ContextPtr frameContext, int _parameterOffset, bool isGlobal) {
  
  int* functionCallParameterOffset = new int;
  *functionCallParameterOffset = 0;
  branches[1]->countFunctionParameter(functionCallParameterOffset);
  int parameterOffset = *functionCallParameterOffset;
  if (*functionCallParameterOffset > 4){
    parameterOffset = (*functionCallParameterOffset - 4)*4;
  } 
  else{
    parameterOffset = 0;
  }
  
  ContextPtr functionContext = new Context(frameContext);
  std::string functionReturnLabel = makeName("L");
  functionContext->updateFunctionLabel(functionReturnLabel);
  
  enum Specifier functionType = branches[0]->returnType();
  functionContext->updateFunctionType(functionType);
  functionContext->updateFunctionName(branches[0]->returnIdentifier());
  
  branches[1]->generateContext(functionContext, parameterOffset, false);
  // functionContext = branches[1]->getContextPtr();
  stackSize = functionContext->getCurrentStackSize();
  branches[0]->generateContext(functionContext, parameterOffset, false);
  functionContext->updateCurrentStackSize(0);
  branches[1]->generateContext(functionContext, parameterOffset, false);
  //functionContext = branches[0]->getContextPtr();
  blockContext = functionContext;
  delete functionCallParameterOffset;   //needs to free pointer

  std::map<std::tuple<double,enum Specifier>,int> updateGlobalFloatDoubleBindings = frameContext->getFloatDoubleLabelBindings();
  //updateGlobalFloatDoubleBindings.merge(functionContext->getFloatDoubleLabelBindings());
  //updateGlobalFloatDoubleBindings.insert((functionContext->getFloatDoubleLabelBindings()).begin(),(functionContext->getFloatDoubleLabelBindings()).end());
  for (auto element : functionContext->getFloatDoubleLabelBindings()){
    if ( updateGlobalFloatDoubleBindings.count(element.first) ==0 ){
      updateGlobalFloatDoubleBindings.insert(element);
      functionContext->getFloatDoubleLabelBindings().erase(element.first);
    }
  }
  frameContext->updateFloatDoubleLabelBindings(updateGlobalFloatDoubleBindings);
  
  std::map<std::string,int> updateGlobalStringBindings = frameContext->getStringLabelBindings();
  //updateGlobalStringBindings.merge(functionContext->getStringLabelBindings());
  //updateGlobalStringBindings.insert((functionContext->getStringLabelBindings()).begin(),(functionContext->getStringLabelBindings()).end());
  for (auto element : functionContext->getStringLabelBindings()){
    if ( updateGlobalStringBindings.count(element.first) ==0 ){
      updateGlobalStringBindings.insert(element);
      functionContext->getStringLabelBindings().erase(element.first);
    }
  }
  frameContext->updateStringLabelBindings(updateGlobalStringBindings);
}

// MIPS
void FunctionDefinition::generateMIPS(std::string indent, int destReg, ContextPtr globalContext) const{
  // we want to make a copy of global context so that the change inside one function definition will not affect its brother. 
  // So we copy global Context into function Context first to avoid any change in global context.
  int size = ( ( 1+ceil(stackSize))/2 )*8 + 64;  // 32: $fp, $31, 2 empty, 4 parameters   32 for $16-$23
  std::cout << "addiu $sp,$sp,-"<< size<<std::endl;
  
  // Check if there is any function call - if there is, we would need to store $sp into $31
  if (blockContext->getHasFunction()==true){
    std::cout << "sw $31," << (size - 4) << "($sp)" << std::endl;
    std::cout << "sw $fp," << (size - 8) << "($sp)" << std::endl;
  }
  else{
    std::cout << "sw $fp," << (size - 4) << "($sp)" << std::endl;
  }

  // store the preserved registers $16-23 onto stack
  for (unsigned i = 0; i<=7; i++){
    std::cout << "sw" << indent << "$" << i+16 << "," << size-12-i*4 << "($sp)" << std::endl; // subtract from top instead of add from bottom
  }
  
  std::cout << "move $fp,$sp" << std::endl;
  branches[0]->generateMIPS(indent,destReg,blockContext);
  
  branches[1]->generateMIPS(indent,destReg,blockContext);

  std::cout << "$" << blockContext->getFunctionLabel() << ":" << std::endl;
  std::cout << "move $sp,$fp" << std::endl;

  // If there is function call, we need to restore $31 back before exiting the stack
  if (blockContext->getHasFunction()==true){
    std::cout << "lw $31," << (size - 4) << "($sp)" << std::endl;
    std::cout << "lw $fp," << (size - 8) << "($sp)" << std::endl;
  }
  else{
    std::cout << "lw $fp," << (size - 4) << "($sp)" << std::endl;
  }

  // load from stack the values of the preserved registers $16-23, load back to registers
  for (unsigned i = 0; i<=7; i++){
    std::cout << "lw" << indent << "$" << i+16 << "," << size-12-i*4 << "($sp)" << std::endl; // subtract from top instead of add from bottom
  }

  std::cout << "addiu $sp,$sp," << size << std::endl;
  std::cout << "jr $31" << std::endl;
  std::cout << "nop" << std::endl;
  std::map<std::tuple<double,enum Specifier>,int> updateGlobalFloatDoubleBindings = globalContext->getFloatDoubleLabelBindings();
  //updateGlobalFloatDoubleBindings.merge(blockContext->getFloatDoubleLabelBindings());
  //updateGlobalFloatDoubleBindings.insert((blockContext->getFloatDoubleLabelBindings()).begin(),(blockContext->getFloatDoubleLabelBindings()).end());
  for (auto element : blockContext->getFloatDoubleLabelBindings()){
    if ( updateGlobalFloatDoubleBindings.count(element.first) ==0 ){
      updateGlobalFloatDoubleBindings.insert(element);
      blockContext->getFloatDoubleLabelBindings().erase(element.first);
    }
  }
  globalContext->updateFloatDoubleLabelBindings(updateGlobalFloatDoubleBindings);
  

}

// Functions
std::string FunctionDefinition::returnIdentifier() const
{
  return branches[0]->returnIdentifier();
}

bool FunctionDefinition::IsInitializeDeclaration() const
{
  return false;
}

bool FunctionDefinition::IsFunctionDefinition() const
{
  return true;
}

std::vector<enum Specifier> FunctionDefinition::returnParameterTypes() const{
  return branches[0]->returnParameterTypes();
}

std::vector<bool> FunctionDefinition::returnParameterIsPointer() const{
  return branches[0]->returnParameterIsPointer();
}