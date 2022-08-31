#ifndef AddressOfOperator_hpp
#define AddressOfOperator_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"

class AddressOfOperator : public Node
{
public:
  // Constructors
  AddressOfOperator(NodePtr Left);

  // Destructor
  virtual ~AddressOfOperator();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;

  // Functions
  void countFunctionParameter(int* functionCallParameterOffset) const override;
  int getValue() const override;
  bool getHasPointerForArithmetic() const override;

  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;

protected:
  ContextPtr blockContext;
};

#endif