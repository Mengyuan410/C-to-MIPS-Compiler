#ifndef Declaration_hpp
#define Declaration_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"

class Declaration
  : public Node
{
public:
  // Constructors
  Declaration(NodePtr DeclarationSpecifier, NodePtr Declarator);
 
  // Destructor
  virtual ~Declaration();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // MIPS code
  void generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const override;
  
  // Function
  enum Specifier DeclarationType() const override;
  bool IsInitializeDeclaration()const override;
  std::string returnIdentifier() const override;
  int integerValue()const override;
  float floatValue()const override;
  double doubleValue()const override;
  std::string stringValue()const override;
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;
  void countFunctionParameter(int* functionCallParameterOffset) const override;
  std::vector<int> getArrayInitializerListInt() const override;
  std::vector<double> getArrayInitializerListFloatDouble() const override;
  std::vector<int> getArrayInitializerListString() const override;
  int returnArrayNumber() const override;
  bool isFunctionDeclarator() const override;
  std::vector<enum Specifier> returnParameterTypes() const override;
  std::vector<bool> returnParameterIsPointer() const override;
  bool IsTypeDef() const override;
  bool IsDirectDeclarator() const override;

  
protected:
  enum Specifier type;
  std::string identifierName;
  ContextPtr blockContext;
  int arraySize;
  int charType = -1;
};

#endif
