#ifndef ParameterDeclaration_hpp
#define ParameterDeclaration_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"

class ParameterDeclaration
  : public Node
{
public:
  // Constructors
  ParameterDeclaration(NodePtr DeclarationSpecifier, NodePtr Declarator);
 
  // Destructor
  virtual ~ParameterDeclaration();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // MIPS
  void generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const override;
  
  // Function
  std::string returnIdentifier() const override;
  enum Specifier DeclarationType() const override;
  bool returnIsPointer() const override;
  
protected:
  enum Specifier type;
  std::string identifierName;
  ContextPtr blockContext;
};

#endif