#ifndef TypeDef_hpp
#define TypeDef_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"


class TypeDef : public Node
{
public:
  TypeDef(NodePtr _type);

  ~TypeDef();
  
  void generateParser(std::ostream &dst, std::string indent) const override;
  
  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;

  // Context
  void generateContext(ContextPtr parentContext, int parameterOffset, bool isGlobal  ) override;

  // Functions
 
  enum Specifier returnType() const override;
  bool IsTypeDef() const override;

  
protected:
  ContextPtr blockContext;
  enum Specifier type;

};

#endif