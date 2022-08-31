#ifndef LogicalNotOperator_hpp
#define LogicalNotOperator_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"

class LogicalNotOperator : public Node
{
public:
  // Constructors
  LogicalNotOperator(NodePtr Left);

  // Destructor
  virtual ~LogicalNotOperator();

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