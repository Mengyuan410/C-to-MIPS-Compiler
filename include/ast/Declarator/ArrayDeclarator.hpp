#ifndef ArrayDeclarator_hpp
#define ArrayDeclarator_hpp

#include "ast/Node.hpp"
#include "ast/Identifier.hpp"

class ArrayDeclarator : public Node
{
public:
  // Constructors
  ArrayDeclarator(NodePtr directDeclarator);
  ArrayDeclarator(NodePtr directDeclarator, NodePtr constExpression);

  // Destructor
  virtual ~ArrayDeclarator();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;

  // Function
  std::string returnIdentifier() const override;
  int returnArrayNumber() const override;
  bool returnIsPointer()const override;
  bool isArrayDeclarator() const override;

protected:
    ContextPtr blockContext;
};


#endif
