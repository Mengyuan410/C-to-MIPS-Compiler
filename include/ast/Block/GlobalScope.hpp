#ifndef GlobalScope_hpp
#define GlobalScope_hpp

#include "ast/Node.hpp"

class GlobalScope : public Node
{
public:

  GlobalScope(std::vector<NodePtr> _branches);
  
  ~GlobalScope();
 
  // Visualising
  void generateParser(std::ostream &dst, std::string indent) const override;
  void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;

  // Functions
  int getFloatBinaryInDec(float value) const;
  std::vector<long> getDoubleBinaryInDec(double value) const;
};

#endif