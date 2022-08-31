#ifndef StructMember_hpp
#define StructMember_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"

class StructMember : public Node
{
public:
  // Constructors
  StructMember(NodePtr structIdentifier, std::string _structMemberName);
 
  // Destructor
  virtual ~StructMember();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // MIPS code
  void generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const override;
  
  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;
  
  // Functions
  std::string returnIdentifier() const override;
protected:
  ContextPtr blockContext;
  std::string structMemberName;
};

#endif