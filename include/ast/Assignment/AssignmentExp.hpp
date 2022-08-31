#ifndef AssignmentExp_hpp
#define AssignmentExp_hpp

#include <iostream>
#include "ast/Node.hpp"

class AssignmentExp: public Node
{ 
public:
  // Constructors
  AssignmentExp(NodePtr assignmentExpression);

  // Destructor
  virtual ~AssignmentExp();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;
  // MIPS
  void generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const override;

  // Functions
  bool isAssignmentExp() const override;
  void countFunctionParameter(int* functionCallParameterOffset) const override;


protected:
  ContextPtr blockContext;
};

#endif