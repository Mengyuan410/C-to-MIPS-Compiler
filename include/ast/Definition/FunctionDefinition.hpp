#ifndef FunctionDefinition_hpp
#define FunctionDefinition_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"

class FunctionDefinition
  : public Node
{
public:
  // Constructors
  FunctionDefinition(NodePtr Declaration, NodePtr Scope);

  // Destructor
  virtual ~FunctionDefinition();

  // output parsing result
  void generateParser(std::ostream &dst, std::string indent) const override;

  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;
  
  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;

  //Functions
  std::string returnIdentifier() const override;
  bool IsInitializeDeclaration()const override;
  bool IsFunctionDefinition() const override;
  std::vector<enum Specifier> returnParameterTypes() const override;
  std::vector<bool> returnParameterIsPointer() const override;

protected:
  int stackSize = 0;
  ContextPtr blockContext;
  int functionParameterOffset = 0;

};

#endif