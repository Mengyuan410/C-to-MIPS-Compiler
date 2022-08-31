#ifndef DirectDeclarator_hpp
#define DirectDeclarator_hpp

#include "ast/Node.hpp"
#include "ast/Identifier.hpp"

class DirectDeclarator : public Node
{
public:
  // Constructors
  DirectDeclarator(std::string Id);

  // Destructor
  virtual ~DirectDeclarator();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // Function
  bool IsInitializeDeclaration() const override;


  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;

  std::string returnIdentifier() const override;
  bool IsDirectDeclarator() const override;

protected:
    ContextPtr blockContext;

};


#endif
