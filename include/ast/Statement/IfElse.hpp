#ifndef ast_IfElse_hpp
#define ast_IfElse_hpp

#include "ast/Node.hpp"

class IfElse
  : public Node
{
public:
  // Constructors
  IfElse(NodePtr condition, NodePtr ifBranch, NodePtr elseBranch);

  // Destructor
  virtual ~IfElse();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;
  
  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;
  void countFunctionParameter(int* functionCallParameterOffset) const override;
  
protected:
  ContextPtr blockContext;
};

#endif
