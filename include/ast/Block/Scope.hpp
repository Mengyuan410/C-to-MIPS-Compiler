#ifndef Scope_hpp
#define Scope_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"


class Scope : public Node
{
public:

  Scope();
  Scope(std::vector<NodePtr> _branches);

  Scope(std::vector<NodePtr> _declaration_branches, std::vector<NodePtr> _statement_branches);

  ~Scope();
  
  void generateParser(std::ostream &dst, std::string indent) const override;
  
  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;

  // Context
  void generateContext(ContextPtr parentContext, int parameterOffset, bool isGlobal  ) override;
  
  // Functions
  ContextPtr getContextPtr() const override;

  void countFunctionParameter(int* functionCallParameterOffset) const override;

  std::vector<std::tuple<int, std::string>> getSwitchCases() const override;
  std::vector<std::string> getSwitchDefault() const override;

protected:
  ContextPtr blockContext;
};

#endif
