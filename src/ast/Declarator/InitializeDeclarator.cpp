#include "ast/Declarator/InitializeDeclarator.hpp"

// Constructors


InitializeDeclarator::InitializeDeclarator(NodePtr Declarator, NodePtr Initializer){
  branches.push_back(Declarator);
  branches.push_back(Initializer);
} 

// Destructor
InitializeDeclarator::~InitializeDeclarator()
{
  delete branches[0];
  delete branches[1];
}
// Visualising
void InitializeDeclarator::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent <<"Initialize Declarator [" << std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// MIPS
void InitializeDeclarator::generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const
{
  branches[1] -> generateMIPS(indent, dstReg, frameContext);
}

// MIPS
void InitializeDeclarator::generateMIPSDouble(std::string indent, int dstReg, ContextPtr frameContext) const
{
  branches[1] -> generateMIPSDouble(indent, dstReg, frameContext);
}

// Context
void InitializeDeclarator::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal) {
  blockContext = frameContext;
  branches[0]->generateContext(frameContext,parameterOffset,isGlobal);
  branches[1]->generateContext(frameContext,parameterOffset,isGlobal);
}

// Functions
bool InitializeDeclarator::IsInitializeDeclaration() const
{
  return true;
}

std::string InitializeDeclarator::returnIdentifier() const{
  return branches[0] -> returnIdentifier();
}

int InitializeDeclarator::integerValue()const
{
    return branches[1]->integerValue();
}

float InitializeDeclarator::floatValue()const
{
    return branches[1]->floatValue();
}

double InitializeDeclarator::doubleValue()const
{
    return branches[1]->doubleValue();
}

std::string InitializeDeclarator::stringValue()const
{
    return branches[1]->stringValue();
}

void InitializeDeclarator::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[1]->countFunctionParameter(functionCallParameterOffset);
}
int InitializeDeclarator::returnArrayNumber() const{
  if (branches[0]->isArrayDeclarator()){
    if (branches[0]->returnArrayNumber()==0){
      int arraySize = (branches[1]->getArrayInitializerListInt()).size();
      return arraySize;
    }
    else{
      return branches[0]->returnArrayNumber();
    }
  }
} 

std::vector<int> InitializeDeclarator::getArrayInitializerListInt() const{
  return branches[1]->getArrayInitializerListInt();
}

std::vector<double> InitializeDeclarator::getArrayInitializerListFloatDouble() const{
  return branches[1]->getArrayInitializerListFloatDouble();
}
std::vector <int> InitializeDeclarator::getArrayInitializerListString() const{
  return branches[1]->getArrayInitializerListString();
}

bool InitializeDeclarator::returnIsPointer() const{
  return branches[0]-> returnIsPointer();
}

