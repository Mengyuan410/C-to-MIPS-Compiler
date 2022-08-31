#include "ast/Declarator/FunctionDeclarator.hpp"

// Constructors

FunctionDeclarator::FunctionDeclarator(NodePtr direct_declarator){
    branches.push_back(direct_declarator);   
}

FunctionDeclarator::FunctionDeclarator(NodePtr direct_declarator, std::vector<NodePtr> _parameters){
    branches.push_back(direct_declarator);  
    parameters = _parameters;
    
}
// Destructor
FunctionDeclarator::~FunctionDeclarator()
{
  delete branches[0];
}
//Visualising
void FunctionDeclarator::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Function Declarator [" << std::endl;
  dst << indent << indent << "Identifier: ";
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << indent <<"Parameter: ";
  for(unsigned i = 0; i < parameters.size(); i++){
    parameters[i]->generateParser(dst, indent+"  ");
  }
  dst << indent << "]" << std::endl;
}

// Context
void FunctionDeclarator::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  )
{
  int size = ( (1+frameContext->getCurrentStackSize()) /2 ) *8 + 64; // frame size
  for(unsigned i = 0; i < parameters.size(); i++){
    enum Specifier type = parameters[i]->DeclarationType();
    int count = 0;
    switch(type){
      case _double:{ 
        count = 2; 
        break;
      }
      default:{
        count = 1;
      }
    }
    frameContext->changeVarBindings(parameters[i]->returnIdentifier(), size, type, isGlobal, parameters[i]->returnIsPointer(), parameters[i]->returnIsPointer());
    size = size+count*4;
  }
  blockContext = frameContext;
}

// MIPS
void FunctionDeclarator::generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const
{
  
  if (parameters.size() > 0){
    int f = 12;
    bool intCharAppearFlag = 0;
    for(unsigned i = 0; i < parameters.size(); i++){
      std::string identifierName = parameters[i]->returnIdentifier();
      int offset = frameContext -> getOffset(identifierName);
      if (i<4){
        enum Specifier type = parameters[i]->DeclarationType();
        switch (type){
          case _float:{
            if((f <= 14) & (intCharAppearFlag == 0)){
              std::cout << "swc1" << indent << "$f" << f << "," << offset << "($fp)" << std::endl; // floating point arguments stored in $f12 & $f14
              f += 2;
            }
            else if(i < 4){
              
              std::cout << "sw" << indent << "$" << i+4 << "," << offset << "($fp)" << std::endl; // stores 2 extra fp arguments in $6-$7
            }
            break;
          }
          case _double:
            if((f <= 14) & (intCharAppearFlag == 0)){
              std::cout << "swc1" << indent << "$f" << f << "," << offset+4 << "($fp)" << std::endl; // floating point arguments stored in $f12 & $f14
              std::cout << "swc1" << indent << "$f" << f+1 << "," << offset << "($fp)" << std::endl;
              f += 2;
            }
            else if(i < 3){ // double needs 2 spaces
              std::cout << "sw" << indent << "$" << i+4 << "," << offset+4 << "($fp)" << std::endl;
              std::cout << "sw" << indent << "$" << i+4+1 << "," << offset << "($fp)" << std::endl;
            }
            break;
          case _char:{     
            intCharAppearFlag = 1;      
            if (parameters[i]->returnIsPointer()==1){
              std::cout << "sw" << indent << "$" << i+4 << "," << offset << "($fp)" << std::endl;
            }
            else{
              std::cout << "sb" << indent << "$" << i+4 << ","  << offset << "($fp)" << std::endl;
            }
            break;
          }
          default:{
            intCharAppearFlag = 1;
            std::cout << "sw" << indent << "$" << i+4 << "," << offset << "($fp)" << std::endl;
          }
        }
      }
    }
  }
}
// Functions
bool FunctionDeclarator::IsInitializeDeclaration() const
{
  return false;
}

std::string FunctionDeclarator::returnIdentifier() const{
  return branches[0] -> returnIdentifier();
}

bool FunctionDeclarator::isFunctionDeclarator() const{
  return true;
}

std::vector<enum Specifier> FunctionDeclarator::returnParameterTypes() const{
  std::vector <enum Specifier> parameterTypes;
  for (unsigned i = 0; i < parameters.size(); i++){
    enum Specifier type = parameters[i]->DeclarationType();
    parameterTypes.push_back(type);
  }
  
  return parameterTypes;
}

std::vector<bool> FunctionDeclarator::returnParameterIsPointer() const{
  std::vector<bool> parameterIsPointer;
  for (unsigned i = 0; i < parameters.size(); i++){
    bool isPointer = parameters[i]->returnIsPointer();
    parameterIsPointer.push_back(isPointer);
  }
  return parameterIsPointer;
}