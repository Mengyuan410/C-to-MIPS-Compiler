#ifndef Identifier_hpp
#define Identifier_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"

class Identifier : public Node
{
public:
  // Constructor
  Identifier(std::string _id);

  // Visualising
  void generateParser(std::ostream &dst,std::string indent) const override;

  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;

  // MIPS
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;
  void generateMIPSDouble(std::string indent, int destReg, ContextPtr frameContext) const override;


  // Functions
  std::string returnIdentifier() const override;
  void countFunctionParameter(int* functionCallParameterOffset) const override;
  bool getHasPointerForArithmetic() const override;
  enum Specifier returnType() const override;
  enum Specifier DeclarationType() const override;
 

protected:
  std::string id;
  ContextPtr blockContext;
  enum Specifier type;
};

#endif
