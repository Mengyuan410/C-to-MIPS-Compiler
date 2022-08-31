#ifndef BreakStatement_hpp
#define BreakStatement_hpp

#include <vector>
#include <string>
#include <iostream>
#include <memory>

#include "ast/Node.hpp"
#include "ast/Context.hpp"

class BreakStatement : public Node
{
public:
    BreakStatement();
    ~BreakStatement();
    void generateParser(std::ostream &dst, std::string indent) const override;
    void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;
    void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;
    void countFunctionParameter(int* functionCallParameterOffset) const override;
protected:
  ContextPtr blockContext;
};

#endif