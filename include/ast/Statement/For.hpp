#ifndef For_hpp
#define For_hpp

#include "ast/Node.hpp"

class For : public Node
{
public:
  // Constructors
  For(NodePtr initExpressionStatement, NodePtr conditionExpressionStatement, NodePtr loopExpression, NodePtr loopBlockStatement);
  For(NodePtr initExpressionStatement, NodePtr conditionExpressionStatement, NodePtr loopBlockStatement);

  // Destructor
  virtual ~For();

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