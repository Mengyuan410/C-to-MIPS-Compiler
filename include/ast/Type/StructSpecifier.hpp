#ifndef StructSpecifier_hpp
#define StructSpecifier_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"


class StructSpecifier : public Node
{
public:
  StructSpecifier(std::string identifier);
  StructSpecifier(std::vector<NodePtr> structDeclarations);
  StructSpecifier(std::string identifier, std::vector<NodePtr> structDeclarations);

  ~StructSpecifier();
  
  void generateParser(std::ostream &dst, std::string indent) const override;
  
  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;

  // Context
  void generateContext(ContextPtr parentContext, int parameterOffset, bool isGlobal  ) override;

  // Functions
 
  std::string returnIdentifier() const override;
  bool isStructSpecifier() const override;
  
protected:
  ContextPtr blockContext;
  std::string structIdentifier;
};

#endif
