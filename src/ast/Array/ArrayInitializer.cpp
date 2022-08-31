#include "ast/Array/ArrayInitializer.hpp"

// Constructors


ArrayInitializer::ArrayInitializer(std::vector<NodePtr> initializer){
  for(unsigned i = 0; i < initializer.size(); i++){
    branches.push_back(initializer[i]);
  }
}

ArrayInitializer::~ArrayInitializer()
{
  for(unsigned i = 0; i < branches.size(); i++){
    delete branches[i];
  }
}


// Visualising
void ArrayInitializer::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Array Initializer [" << std::endl;
  for(unsigned i = 0; i < branches.size(); i++){
    branches[i]->generateParser(dst, indent+"  ");
  }
  dst << indent << "]" << std::endl;
}

// Context
void ArrayInitializer::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
    blockContext = frameContext;
    for (unsigned i = 0; i < branches.size(); i++){ 
        branches[i]->generateContext(blockContext, parameterOffset, isGlobal);  
    }
}


std::vector<int> ArrayInitializer::getArrayInitializerListInt() const{
    std::vector<int> arrayInitializerList;
    for (unsigned i=0;i<branches.size();i++){
        int value = branches[i]->integerValue();
        arrayInitializerList.push_back(value);
    }
    return arrayInitializerList;
}

std::vector<double> ArrayInitializer::getArrayInitializerListFloatDouble() const{
    std::vector<double> arrayInitializerList;
  
    for (unsigned i=0;i<branches.size();i++){
       
        double value = branches[i]->doubleValue();
        
        arrayInitializerList.push_back(value);
    }
    return arrayInitializerList;
}

std::vector<int> ArrayInitializer::getArrayInitializerListString() const{
    std::vector<int> arrayInitializerList;
    for (unsigned i=0;i<branches.size();i++){
        std::string value = branches[i]->stringValue();
        char character = value[0];
        int ascii_val = (int) character;
        arrayInitializerList.push_back(ascii_val);
    }
    return arrayInitializerList;
}