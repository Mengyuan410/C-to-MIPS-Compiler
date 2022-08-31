#include "ast/Literal/Float.hpp"

// Constructor
Float::Float(double _value) : value(_value){}

// Visualising
void Float::generateParser(std::ostream &dst, std::string indent) const
{
    dst << indent << "Float = " << value << std::endl;
}
// Context
void Float::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
}

// MIPS
void Float::generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const
{
  enum Specifier type = _float;
  std::string number_label = frameContext->getFloatDoubleLabel(value, type);
  int immReg = frameContext->alloCalReg();
  frameContext->dealloReg(immReg);

  std::cout << "lui" << indent << "$" << immReg << ",%hi($" << number_label << ")" << std::endl;
  std::cout << "lwc1" << indent << "$f" << destReg << ",%lo($" << number_label << ")($" << immReg << ")" << std::endl;
}

void Float::generateMIPSDouble(std::string indent, int destReg, ContextPtr frameContext) const
{
  enum Specifier type = _double;
  std::string number_label = frameContext->getFloatDoubleLabel(value, type);

  int immReg = frameContext->alloCalReg();
  frameContext->dealloReg(immReg);

  std::cout << "lui" << indent << "$" << immReg << ",%hi($" << number_label << ")" << std::endl;
  std::cout << "lwc1" << indent << "$f" << destReg << ",%lo($" << number_label << "+4)($" << immReg << ")" << std::endl;
  std::cout << "lwc1" << indent << "$f" << destReg+1 << ",%lo($" << number_label << ")($" << immReg << ")" << std::endl;
}


// Functions
float Float::floatValue()const
{
    return value;
}
double Float::doubleValue()const
{
    return value;
}
void Float::countFunctionParameter(int* functionCallParameterOffset) const {};

enum Specifier Float::returnType() const{
  return _float;
}

