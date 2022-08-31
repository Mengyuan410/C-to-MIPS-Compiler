#ifndef AddOperator_hpp
#define AddOperator_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"

class AddOperator : public Node
{
public:
  // Constructors
  AddOperator(NodePtr Left, NodePtr Right);

  // Destructor
  virtual ~AddOperator();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;
  

  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;
  void generateMIPSDouble(std::string indent, int destReg, ContextPtr frameContext) const override;

  // Functions
  bool isAssignmentExp() const override;
  void countFunctionParameter(int* functionCallParameterOffset) const override;
  int getValue() const override;
  bool getHasPointerForArithmetic() const override;
  enum Specifier returnType() const override;

protected:
  ContextPtr blockContext;
  enum Specifier type;
};

#endif
