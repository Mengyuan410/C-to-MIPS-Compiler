#ifndef FunctionDeclaration_hpp
#define FunctionDeclaration_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"

class FunctionDeclaration
  : public Node
{
public:
  // Constructors
  FunctionDeclaration(NodePtr DeclarationSpecifier, NodePtr Declarator);
 
  // Destructor
  virtual ~FunctionDeclaration();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // MIPS code
  void generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const override;
  
  // Function
  std::string returnIdentifier() const override;
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;
  ContextPtr getContextPtr() const override;
  enum Specifier returnType() const override;
  std::vector<enum Specifier> returnParameterTypes() const override;
  std::vector<bool> returnParameterIsPointer() const override;
  
protected:
  enum Specifier type;
  std::string identifierName;
  ContextPtr blockContext;
};

#endif