#ifndef Float_hpp
#define Float_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"

class Float : public Node
{
private:
  double value;

public:
  // Constructors
  Float(double _value);

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;
  void generateMIPSDouble(std::string indent, int destReg, ContextPtr frameContext) const override;
 

  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;

  // Functions
  float floatValue()const override;
  double doubleValue()const override;
  void countFunctionParameter(int* functionCallParameterOffset) const override;
  enum Specifier returnType() const override;
  

protected:
  ContextPtr blockContext;
  std::string number_label;
};

#endif
