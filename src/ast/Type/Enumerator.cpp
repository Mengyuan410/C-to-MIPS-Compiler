#include "ast/Type/Enumerator.hpp"

Enumerator::Enumerator(std::string _identifier){
    identifier = _identifier;
}

Enumerator::Enumerator(std::string _identifier, NodePtr constantExpression){
    identifier = _identifier;
    branches.push_back(constantExpression);
}

Enumerator::~Enumerator()
{
    if (branches.size()==1){
        delete branches[0];
    }
}

// Context
void Enumerator::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ){
    blockContext = frameContext;
}

// Parser
void Enumerator::generateParser(std::ostream &dst, std::string indent) const{
    dst << indent << "Enumerator [" << std::endl;
    dst << indent << "]" << std::endl;
}


// Functions
std::string Enumerator::returnIdentifier() const{
    return identifier;
}
int Enumerator::getValue() const{
    return branches[0]->getValue();
}
bool Enumerator::hasValue() const{
    if (branches.size()==0){
        return false;
    }
    else{
        return true;
    }
}
