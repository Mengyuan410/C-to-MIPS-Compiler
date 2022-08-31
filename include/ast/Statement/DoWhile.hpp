#ifndef DoWhile_hpp
#define DoWhile_hpp

#include "ast/Node.hpp"

class DoWhile : public Node
{
public:
  // Constructors
  DoWhile(NodePtr whileBlock, NodePtr condition);

  // Destructor
  virtual ~DoWhile();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;

  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;
  void countFunctionParameter(int* functionCallParameterOffset) const override;
  
protected:
  ContextPtr blockContext;
  std::string label2; // for 'continue' to jump to
  std::string label3;
};

#endif
