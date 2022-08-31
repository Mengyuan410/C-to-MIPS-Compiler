#ifndef NegativeOperator_hpp
#define NegativeOperator_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"

class NegativeOperator : public Node
{
public:
  // Constructors
  NegativeOperator(NodePtr Left);

  // Destructor
  virtual ~NegativeOperator();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;
  void generateMIPSDouble(std::string indent, int destReg, ContextPtr frameContext) const override;
  
  // Functions
  void countFunctionParameter(int* functionCallParameterOffset) const override;
  int getValue() const override;
  
  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;

protected:
  ContextPtr blockContext;
  enum Specifier type;
};

#endif

