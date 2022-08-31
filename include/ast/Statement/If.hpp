#ifndef If_hpp
#define If_hpp

#include "ast/Node.hpp"

class If : public Node
{
public:
  // Constructors
  If(NodePtr condition, NodePtr ifBranch);

  // Destructor
  virtual ~If();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal) override;

  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;
  void countFunctionParameter(int* functionCallParameterOffset) const override;
  
protected:
  ContextPtr blockContext;
};

#endif
