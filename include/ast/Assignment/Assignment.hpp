#ifndef Assignment_hpp
#define Assignment_hpp

#include <iostream>
#include "ast/Node.hpp"

class Assignment: public Node
{ 
public:
  // Constructors
  Assignment(NodePtr left, NodePtr right);

  // Destructor
  virtual ~Assignment();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // MIPS
  void generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const override;

  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;

  // Functions
  void countFunctionParameter(int* functionCallParameterOffset) const override;
  
protected:
  ContextPtr blockContext;
  int stackStartAddress;
  enum Specifier type;
};

#endif