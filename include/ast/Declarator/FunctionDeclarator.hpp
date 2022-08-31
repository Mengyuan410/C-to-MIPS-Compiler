#ifndef FunctionDeclarator_hpp
#define FunctionDeclarator_hpp

#include "ast/Node.hpp"

class FunctionDeclarator : public Node
{
public:
    // Constructors
    FunctionDeclarator(NodePtr direct_declarator);
    FunctionDeclarator(NodePtr direct_declarator, std::vector<NodePtr> _parameters);

    // Destructor
    virtual ~FunctionDeclarator();

 
    void generateParser(std::ostream &dst,std::string indent) const override;
    void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) override;
    void generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const override;
    
    // Functions
    bool IsInitializeDeclaration() const override;
    std::string returnIdentifier() const override;
    bool isFunctionDeclarator() const override;
    std::vector<enum Specifier> returnParameterTypes() const override;
    std::vector<bool> returnParameterIsPointer() const override;

protected:
    std::vector<NodePtr> parameters;
    ContextPtr blockContext;
};

#endif

