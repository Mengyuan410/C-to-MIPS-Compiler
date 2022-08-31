#ifndef LessThanOperator_hpp
#define LessThanOperator_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"

class LessThanOperator : public Node
{
public:
  // Constructors
   LessThanOperator(NodePtr Left, NodePtr Right);

  // Destructor
  virtual ~ LessThanOperator();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;
  void generateMIPSDouble(std::string indent, int destReg, ContextPtr frameContext) const override;

  // Functions
  bool isAssignmentExp() const override;
  void countFunctionParameter(int* functionCallParameterOffset) const override;
  int getValue() const override;
  enum Specifier returnType() const override;

  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;

protected:
  ContextPtr blockContext;
  enum Specifier type;
};

#endif
