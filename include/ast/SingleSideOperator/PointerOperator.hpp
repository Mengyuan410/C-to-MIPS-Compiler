#ifndef PointerOperator_hpp
#define PointerOperator_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"

class PointerOperator : public Node
{
public:
  // Constructors
  PointerOperator(NodePtr Left);

  // Destructor
  virtual ~PointerOperator();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;
  void generateMIPSPointerAssignment(std::string indent, int destReg, ContextPtr frameContext) const override;
  void generateMIPSPointerGlobalAssignment(std::string indent, int destReg, ContextPtr frameContext) const override;
  void generateMIPSDouble(std::string indent, int destReg, ContextPtr frameContext) const override;

  // Functions
  void countFunctionParameter(int* functionCallParameterOffset) const override;
  int getValue() const override;
  bool returnIsPointer() const override;
  std::string returnIdentifier() const override;
  enum Specifier returnType() const override;

  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;

protected:
  ContextPtr blockContext;
};

#endif