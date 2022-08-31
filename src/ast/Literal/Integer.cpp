#include "ast/Literal/Integer.hpp"

// Constructor
Integer::Integer(int _value) : value(_value){}

// Visualising
void Integer::generateParser(std::ostream &dst, std::string indent) const
{
    dst << indent << "Integer = " << value << std::endl;
}
// Context
void Integer::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
}
// MIPS
void Integer::generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const
{
  std::cout << "li" << indent << "$" << destReg << "," << value << indent << "# 0x" << value << std::endl;
}

// Functions
int Integer::integerValue()const
{
    return value;
}
void Integer::countFunctionParameter(int* functionCallParameterOffset) const {};
int Integer::getValue() const{
  return value;
}
enum Specifier Integer::returnType() const{
  return _int;
}

