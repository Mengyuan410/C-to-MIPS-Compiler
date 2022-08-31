#ifndef PrimitiveType_hpp
#define PrimitiveType_hpp

#include "ast/Node.hpp"
#include "ast/Context.hpp"

class PrimitiveType
  : public Node
{
public:
  // Constructor
  PrimitiveType(enum Specifier _type);

  // Destructor
  ~PrimitiveType();

  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;

  //Function
  enum Specifier DeclarationType() const override;

private:
  enum Specifier type;
};

#endif
