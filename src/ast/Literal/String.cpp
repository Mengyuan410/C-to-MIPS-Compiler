#include "ast/Literal/String.hpp"

// Constructor
String::String(std::string _value) : value(std::string(_value)){
    //char_type: 0 is char , 1 is string
    if (value[0] == '\''){
        char_type = 0;
    }
    else if (value[0] == '\"'){
        char_type = 1;
    }
    value = value.substr(1,value.size()-2);
    length = value.size();
}

// Visualising
void String::generateParser(std::ostream &dst, std::string indent) const
{
    dst << indent << "String = " << value << std::endl;
}

// Context
void String::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
  if (char_type == 1){
    string_label = frameContext->getStringLabel(value);
  }
}

// MIPS
void String::generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const
{
  if (char_type == 0){
    char character = value[0];
    int ascii_val = (int) character;
    std::cout << "li" << indent << "$" << destReg << "," << ascii_val << std::endl;
  }
  else if (char_type == 1){
    std::cout << "lui" << indent << "$" << destReg << "," << "%hi($" << string_label << ")" << std::endl;
    std::cout << "addiu" << indent << "$" << destReg << "," << "$" << destReg << "," << "%lo($" << string_label << ")" << std::endl;
  }
  else{
    std::cout << "error" << std::endl;
  }
}

// Functions
void String::countFunctionParameter(int* functionCallParameterOffset) const {};

enum Specifier String::returnType() const{
  return _char;
}

int String::returnStringLength() const{
    return length;
}

std::string String::stringValue()const
{
    return value;
}
