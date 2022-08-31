#ifndef Enumerator_hpp
#define Enumerator_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"


class Enumerator : public Node
{
public:
  
  Enumerator(std::string _identifier);
  Enumerator(std::string _identifier, NodePtr enumerators);

  ~Enumerator();
  
  void generateParser(std::ostream &dst, std::string indent) const override;

  void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;

  // Functions
  std::string returnIdentifier() const override;
  int getValue() const override;
  bool hasValue() const override;

protected:
  ContextPtr blockContext;
  std::string identifier;
};

#endif
