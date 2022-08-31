#ifndef EnumSpecifier_hpp
#define EnumSpecifier_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"


class EnumSpecifier : public Node
{
public:
  EnumSpecifier(std::vector<NodePtr> enumerators);
  EnumSpecifier(std::string identifier);
  EnumSpecifier(std::string identifier, std::vector<NodePtr> enumerators);

  ~EnumSpecifier();
  
  void generateParser(std::ostream &dst, std::string indent) const override;
  
  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;

  // Context
  void generateContext(ContextPtr parentContext, int parameterOffset, bool isGlobal  ) override;

  // Functions
  bool isEnumSpecifier() const override;
  enum Specifier DeclarationType() const override;
  
protected:
  ContextPtr blockContext;
};

#endif
