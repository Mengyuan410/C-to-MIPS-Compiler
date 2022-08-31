#ifndef PointerDeclarator_hpp
#define PointerDeclarator_hpp

#include "ast/Node.hpp"
#include "ast/Identifier.hpp"

class PointerDeclarator : public Node
{
public:
  // Constructors
  PointerDeclarator(NodePtr directDeclarator);

  // Destructor
  virtual ~PointerDeclarator();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // Function
  bool IsInitializeDeclaration() const override;
  bool returnIsPointer() const override;
  std::string returnIdentifier() const override;
  std::vector<enum Specifier> returnParameterTypes() const override;
  std::vector<bool> returnParameterIsPointer() const override;

  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;


  // MIPS
  
// MIPS
void generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const override;

protected:
    ContextPtr blockContext;

};


#endif