#ifndef BitwiseNotOperator_hpp
#define BitwiseNotOperator_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"

class BitwiseNotOperator : public Node
{
public:
  // Constructors
  BitwiseNotOperator(NodePtr Left);

  // Destructor
  virtual ~BitwiseNotOperator();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;

  // Functions
  void countFunctionParameter(int* functionCallParameterOffset) const override;
  int getValue() const override;
  
  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;

protected:
  ContextPtr blockContext;
};

#endif