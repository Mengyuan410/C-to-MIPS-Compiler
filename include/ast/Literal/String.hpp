#ifndef String_hpp
#define String_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"

class String : public Node
{
private:
  std::string value;

public:
  // Constructors
  String(std::string _value);

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;

  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;

  // Functions

  void countFunctionParameter(int* functionCallParameterOffset) const override;
  enum Specifier returnType() const override;
  int returnStringLength() const override;
  std::string stringValue() const override;

protected:
  ContextPtr blockContext;
  enum Specifier type = _char;
  int length;
  int char_type; // 0 for char, 1 for string
  std::string string_label;
};

#endif