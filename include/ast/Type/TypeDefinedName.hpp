#ifndef TypeDefinedName_hpp
#define TypeDefinedName_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"


class TypeDefinedName : public Node
{
public:
  TypeDefinedName(std::string _name);

  ~TypeDefinedName();
  
  void generateParser(std::ostream &dst, std::string indent) const override;
  
  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;

  // Context
  void generateContext(ContextPtr parentContext, int parameterOffset, bool isGlobal  ) override;

  // Functions
 
  enum Specifier DeclarationType() const override;
  bool returnIsPointer() const override;

protected:
  ContextPtr blockContext;
  enum Specifier type;
  bool isPointer;
  std::string name;

};

#endif