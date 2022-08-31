#ifndef ast_Switch_hpp
#define ast_Switch_hpp

#include "ast/Node.hpp"

class Switch
  : public Node
{
public:
  // Constructors
  Switch(NodePtr condition, NodePtr switchBranch);

  // Destructor
  virtual ~Switch();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;
  
  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;

  // Functions
  void countFunctionParameter(int* functionCallParameterOffset) const override;
  
protected:
  ContextPtr blockContext;
  std::string label;
};

#endif