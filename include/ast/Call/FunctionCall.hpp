#ifndef FunctionCall_hpp
#define FunctionCall_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"

class FunctionCall : public Node
{
public:
  // Constructors
  FunctionCall(NodePtr id);
  FunctionCall(NodePtr id, std::vector<NodePtr> _parameters);
 
  // Destructor
  virtual ~FunctionCall();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // MIPS code
  void generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const override;
  void generateMIPSDouble(std::string indent, int dstReg, ContextPtr frameContext) const override;
  
  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;

  // Functions
  std::string returnIdentifier() const override;
  void countFunctionParameter(int* functionCallParameterOffset) const override;
  
protected:
  ContextPtr blockContext;
  std::vector<NodePtr> parameters;
};

#endif