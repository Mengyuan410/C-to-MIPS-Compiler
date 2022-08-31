#ifndef InitializeDeclarator_hpp
#define InitializeDeclarator_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"

class InitializeDeclarator : public Node
{
public:
  // Constructors
  InitializeDeclarator(NodePtr Declarator, NodePtr Initializer);

  // Destructor
  virtual ~InitializeDeclarator();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // MIPS
  void generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const override;
  void generateMIPSDouble(std::string indent, int dstReg, ContextPtr frameContext) const override;

  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;
  

  // Functions
  bool IsInitializeDeclaration() const override;
  std::string returnIdentifier() const override;
  int integerValue()const override;
  float floatValue()const override;
  double doubleValue()const override;
  std::string stringValue()const override;
  void countFunctionParameter(int* functionCallParameterOffset) const override;
  int returnArrayNumber() const override;
  std::vector<int> getArrayInitializerListInt() const override;
  std::vector<double> getArrayInitializerListFloatDouble() const override;
  std::vector<int> getArrayInitializerListString() const override;
  bool returnIsPointer() const override;


protected:
  ContextPtr blockContext;
};

#endif