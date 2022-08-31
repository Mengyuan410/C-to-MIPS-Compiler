#ifndef PostIncrementOperator_hpp
#define PostIncrementOperator_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"

class PostIncrementOperator : public Node
{
public:
  // Constructors
  PostIncrementOperator(NodePtr Left);

  // Destructor
  virtual ~PostIncrementOperator();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;
  void generateMIPSDouble(std::string indent, int destReg, ContextPtr frameContext) const override;
  // Functions
  void countFunctionParameter(int* functionCallParameterOffset) const override;
  bool getHasPointerForArithmetic() const override;

  
  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;

protected:
  ContextPtr blockContext;
  enum Specifier type;

};

#endif
