#ifndef ArrayInitializer_hpp
#define ArrayInitializer_hpp

#include "ast/Node.hpp"
#include "ast/Identifier.hpp"

class ArrayInitializer : public Node
{
public:
  // Constructors
  ArrayInitializer(std::vector<NodePtr> initializer);

  // Destructor
  virtual ~ArrayInitializer();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  // Context
  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;

  // Function
  std::vector<int> getArrayInitializerListInt() const override;
  std::vector<double> getArrayInitializerListFloatDouble() const override;
  std::vector<int> getArrayInitializerListString() const override;
  
protected:
  ContextPtr blockContext;
};


#endif
