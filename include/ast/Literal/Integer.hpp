#ifndef Integer_hpp
#define Integer_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"

class Integer : public Node
{
private:
  int value;

public:
  // Constructors
  Integer(int _value);

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;

  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;

  // Functions
  int integerValue()const override;
  void countFunctionParameter(int* functionCallParameterOffset) const override;
  int getValue() const override;
  enum Specifier returnType() const override;

protected:
  ContextPtr blockContext;
};

#endif
