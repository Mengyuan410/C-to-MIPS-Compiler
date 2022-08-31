#ifndef Default_hpp
#define Default_hpp

#include "ast/Node.hpp"

class Default : public Node
{
public:
  // Constructors
  Default(NodePtr defaultBranch);

  // Destructor
  virtual ~Default();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;

  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;
  void countFunctionParameter(int* functionCallParameterOffset) const override;
  void getCaseInfo(std::vector<std::tuple<int, std::string>> &DefaultsVector) override;
  void getDefaultInfo(std::vector<std::string> &defaultVector) override;

protected:
  ContextPtr blockContext;
  std::string labelName;
};

#endif
