#ifndef rootNode_hpp
#define rootNode_hpp

#include "Node.hpp"

class RootNode : public Node
{
public:
    RootNode(NodePtr GlobalScope);   //RootNode contains a pointer to the GlobalScope, which contains pointer to different branches
    virtual ~RootNode();
    void generateParser (std::ostream &dst,std::string indent) const override;
    void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const override;

};

#endif