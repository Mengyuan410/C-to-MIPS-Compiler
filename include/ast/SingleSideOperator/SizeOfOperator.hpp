#ifndef SizeOfOperator_hpp
#define SizeOfOperator_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"

class SizeOfOperator : public Node
{
public:
  // Constructors
  SizeOfOperator(NodePtr Left);

  // Destructor
  virtual ~SizeOfOperator();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;

  

  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;

protected:
  ContextPtr blockContext;
};

#endif