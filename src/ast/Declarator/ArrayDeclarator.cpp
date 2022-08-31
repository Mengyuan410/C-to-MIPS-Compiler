#include "ast/Declarator/ArrayDeclarator.hpp"

// Constructors

ArrayDeclarator::ArrayDeclarator(NodePtr directDeclarator){
  branches.push_back(directDeclarator);
}
ArrayDeclarator::ArrayDeclarator(NodePtr directDeclarator, NodePtr constExpression){
  branches.push_back(directDeclarator);
  branches.push_back(constExpression);
} 

// Destructor
ArrayDeclarator::~ArrayDeclarator()
{
  delete branches[0];
  if (branches.size()>1){
    delete branches[1];
  }
}

// Context
void ArrayDeclarator::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
  branches[0]->generateContext(frameContext,parameterOffset,isGlobal   );
  if (branches.size()>1){
    branches[1]->generateContext(frameContext,parameterOffset,isGlobal   );
  }
}

//Visualising
void ArrayDeclarator::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Array Declarator [" << std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  if (branches.size()>1){ 
    branches[1]->generateParser(dst, indent+"  ");
  }
  dst << indent << "]" << std::endl;
}


// Functions
std::string ArrayDeclarator::returnIdentifier() const{
    return branches[0] -> returnIdentifier();
}

int ArrayDeclarator::returnArrayNumber() const{
    if (branches.size()>1){
      return branches[1]->integerValue();
    }
    else{
      return 0;
    }
}
bool ArrayDeclarator::isArrayDeclarator() const{
  return true;
}
bool ArrayDeclarator::returnIsPointer()const{
  return true;
}