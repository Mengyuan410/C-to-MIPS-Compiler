#ifndef StructDeclaration_hpp
#define StructDeclaration_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"

class StructDeclaration
  : public Node
{
public:
  // Constructors
  StructDeclaration(NodePtr typeSpecifier, NodePtr declarator);
 
  // Destructor
  virtual ~StructDeclaration();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

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