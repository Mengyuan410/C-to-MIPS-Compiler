#include "ast/Block/GlobalScope.hpp"
#include "ast/Node.hpp"
#include <bitset>

GlobalScope::GlobalScope(std::vector<NodePtr> _branches) : Node(_branches){}

GlobalScope::~GlobalScope()
{
  for(unsigned i = 0; i < branches.size(); i++){
    delete branches[i];
  }
}

void GlobalScope::generateParser(std::ostream &dst, std::string indent) const{
  dst << indent << "Global Scope [" << std::endl;
  for(unsigned i = 0; i < branches.size(); i++){
    branches[i]->generateParser(dst, indent+"  ");
  }
  dst << indent << "]" << std::endl;
}

void GlobalScope::generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const{
  // Generate Context
  ContextPtr globalContext =  new Context(frameContext);
  for (unsigned i = 0; i < branches.size(); i++){
    branches[i]->generateContext(globalContext, 0,true);
    if (branches[i]->isFunctionDeclarator()==true || branches[i]->IsFunctionDefinition()==true){
        std::string functionName = branches[i]->returnIdentifier();
        std::vector<enum Specifier> parameterFunctionTypes = branches[i]->returnParameterTypes();
        std::vector<bool> parameterFunctionIsPointer = branches[i]->returnParameterIsPointer();
        globalContext->updateFunctionAndParamCallType(functionName,parameterFunctionTypes,parameterFunctionIsPointer);
    }
  }

  // Generate MIPS

  // print out the string and their labels 
  std::map<std::string,int> stringLabelBindings = globalContext->getStringLabelBindings();
  for (auto& element : stringLabelBindings) { // loop through all key-value pairs in the map
    std::string label = "LS"+std::to_string(element.second); // label is LS followed by the count number
    std::string stringLiteral = element.first;
    std::cout << "$" << label << ":" << std::endl;
    std::cout << indent << ".ascii" << indent << "\"" << stringLiteral << "\\000\"" << std::endl;
    std::cout << indent << ".align 2" << std::endl;
  }

  for (int i = 0; i < branches.size(); i++){

    if (branches[i]->isEnumSpecifier()==false && branches[i]->isStructSpecifier()==false){
      enum Specifier type = branches[i]->DeclarationType();
      std::string identifierName = branches[i]-> returnIdentifier();
      if (branches[i]->IsInitializeDeclaration()==true){ 
        // Section: .data
        // declares variable names used in program; storage allocated in main memory (RAM)
        std::cout << ".data" << std::endl;
        std::cout << ".globl " << identifierName<<std::endl;
        std::cout << identifierName <<":" << std::endl;
        if (branches[i]->returnArrayNumber()==0){
          switch(type){
            case _float:
              std::cout << indent << ".word" << indent << getFloatBinaryInDec(branches[i]->floatValue())<< std::endl;
              break;
            case _double:
              std::cout << indent << ".word" << indent << getDoubleBinaryInDec(branches[i]->doubleValue())[0] << std::endl; 
              std::cout << indent << ".word" << indent << getDoubleBinaryInDec(branches[i]->doubleValue())[1] << std::endl;
              break;
            case _char:{
              std::string string_literal = branches[i]->stringValue();
              if (string_literal.size() == 1){
                std::cout << indent << ".byte" << indent << (int) string_literal[0] << std::endl;
              } 
              else if (string_literal.size() > 1){
                std::cout << indent << ".word" << indent << "$" << globalContext->getStringLabel(string_literal) << std::endl;
              }
              break;
            }
            default:
              std::cout << indent << ".word" << indent << branches[i]->integerValue()<< std::endl;
          }
        }
        else{
          
          switch(type){
            case _double:{
              std::vector<double> arrayInitializeList = branches[i]->getArrayInitializerListFloatDouble();
              for (unsigned i = 0; i<arrayInitializeList.size();i++){
                std::cout << indent << ".word" << indent << getDoubleBinaryInDec(arrayInitializeList[i])[0] << std::endl;
                std::cout << indent << ".word" << indent << getDoubleBinaryInDec(arrayInitializeList[i])[1] << std::endl;
              }
              break;
            }
            case _float:{
              std::vector<double> arrayInitializeList = branches[i]->getArrayInitializerListFloatDouble();
              for (unsigned i = 0; i<arrayInitializeList.size();i++){
                std::cout << indent << ".word" << indent << getFloatBinaryInDec(arrayInitializeList[i]) << std::endl;
              }
              break;      
            }     
            case _char:{
              std::vector<int> arrayInitializeList = branches[i]->getArrayInitializerListString();
              std::string entireString;
              for (unsigned i = 0; i<arrayInitializeList.size();i++){
                std::cout << indent << ".byte" << indent << arrayInitializeList[i] << std::endl;
              }
             
              break;
            }
            default:{
              std::vector<int> arrayInitializeList = branches[i]->getArrayInitializerListInt();
              for (unsigned i = 0; i<arrayInitializeList.size();i++){
                std::cout << indent << ".word" << indent << arrayInitializeList[i] << std::endl;
              }
              break;
            }
          }
        }
      }
      else if (branches[i]->IsFunctionDefinition()==true){ 
        // Section: .text
        // contains program code (instructions)
        std::cout << ".text" << std::endl;
        std::cout << ".globl " << identifierName << std::endl;
        std::cout << identifierName <<":" << std::endl;
        branches[i] -> generateMIPS(indent, destReg, globalContext);
      }
      else if (branches[i]->isFunctionDeclarator()==true){   // for int g();
        branches[i] -> generateMIPS(indent, destReg, globalContext);
      }
      else if (branches[i]->IsTypeDef() == true){
        branches[i] -> generateMIPS(indent, destReg, globalContext);
      }
      else if (branches[i]->isEnumSpecifier()== false){
        std::cout << ".data" << std::endl;
        std::cout << ".globl " << identifierName << std::endl;
        std::cout << identifierName <<":" << std::endl;
        branches[i] -> generateMIPS(indent, destReg, globalContext);
      }
      else if (branches[i]->IsDirectDeclarator() == true){
        continue;
      }
      else{
        std::cout << ".data" << std::endl;
        std::cout << ".globl " << identifierName << std::endl;
        std::cout << identifierName <<":" << std::endl;
        branches[i] -> generateMIPS(indent, destReg, globalContext);
      }
      std::cout << std::endl;
    }
  }

  // print out the float & double numbers and their labels 
  std::map<std::tuple<double,enum Specifier>,int> floatDoubleLabelBindings = globalContext->getFloatDoubleLabelBindings();
  for (auto& element : floatDoubleLabelBindings) { // loop through all key-value pairs in the map
    std::string label = "LC"+std::to_string(element.second); // label is LC followed by the count number
    std::cout << "$" << label << ":" << std::endl;

    if (std::get<1>(element.first) == _float){
      float floatValue = (float) std::get<0>(element.first); // the value from map is a double, so convert it to float
      std::cout << indent << ".word" << indent << getFloatBinaryInDec(floatValue) << std::endl;
    } 
    else if (std::get<1>(element.first) == _double){
      
      double doubleValue = std::get<0>(element.first);
      std::cout << indent << ".word" << indent << getDoubleBinaryInDec(doubleValue)[0] << std::endl;
      std::cout << indent << ".word" << indent << getDoubleBinaryInDec(doubleValue)[1] << std::endl;
    }
  }

}


//Functions
int GlobalScope::getFloatBinaryInDec(float value) const
{
    char* bits = reinterpret_cast<char*>(&value);
    std::string floatBinary;
    for(std::size_t n = 0; n < sizeof value; ++n){
      floatBinary.insert(0, std::bitset<8>(bits[n]).to_string()); // each byte is inserted from LSB to MSB
    }
    std::bitset<32> b(floatBinary);
    int dec=b.to_ulong();
    return dec;
}

std::vector<long> GlobalScope::getDoubleBinaryInDec(double value) const
{
    value = (double) value;
    char* bits = reinterpret_cast<char*>(&value);
    std::string doubleBinary_lower;
    std::string doubleBinary_higher;
    for(std::size_t n = 0; n < sizeof value; ++n){
      if (n<4){
        doubleBinary_lower.insert(0,std::bitset<8>(bits[n]).to_string());
      }
      else{
        doubleBinary_higher.insert(0,std::bitset<8>(bits[n]).to_string());
      }
    }
    std::bitset<32> b(doubleBinary_lower);
    long dec=b.to_ulong();
    
    std::bitset<32> b2(doubleBinary_higher);
    long dec2=b2.to_ulong();
    
    std::vector<long> output;
    output.push_back(dec2);
    output.push_back(dec);
    return output;
}




   
