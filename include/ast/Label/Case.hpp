#ifndef Case_hpp
#define Case_hpp

#include "ast/Node.hpp"

class Case : public Node
{
public:
  // Constructors
  Case(NodePtr condition, NodePtr ifBranch);

  // Destructor
  virtual ~Case();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;

  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContexte) const override;
  void countFunctionParameter(int* functionCallParameterOffset) const override;
  void getCaseInfo(std::vector<std::tuple<int, std::string>> &casesVector) override;

protected:
  ContextPtr blockContext;
  std::string labelName;
};

#endif
