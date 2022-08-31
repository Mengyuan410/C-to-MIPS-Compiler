#ifndef ArrayIndex_hpp
#define ArrayIndex_hpp

#include "ast/Node.hpp"
#include "ast/Identifier.hpp"

class ArrayIndex : public Node
{
public:
  // Constructors
  ArrayIndex(NodePtr arrayIdentifier, NodePtr arrayIndex);

  // Destructor
  virtual ~ArrayIndex();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;
  
  // MIPS
  void generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const override;
  void generateMIPSArrayGlobalAssignment(std::string indent, int dstReg, ContextPtr frameContext) const override;
  void generateMIPSArrayAssignment(std::string indent, int dstReg, ContextPtr frameContext) const override;

  // Function
  std::string returnIdentifier() const override; 
  std::string returnIdentifierIndexed() const override;
  bool getIsArray() const override;
  enum Specifier returnType() const override;
  //std::string returnIdentifierIndexed() const override;
  

protected:
  ContextPtr blockContext;
  int stackStartAddress;
};


#endif
