#ifndef While_hpp
#define While_hpp

#include "ast/Node.hpp"

class While : public Node
{
public:
  // Constructors
  While(NodePtr condition, NodePtr whileBlock);

  // Destructor
  virtual ~While();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;

  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;

  void countFunctionParameter(int* functionCallParameterOffset) const override;
  
protected:
  ContextPtr blockContext;
  std::string label3; // for 'continue' to jump to
  std::string label4;
};
#endif
