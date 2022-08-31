#include "ast/Node.hpp"

//Constructor
Node::Node() : Node(std::vector<NodePtr>{})
{}

Node::Node(std::vector<NodePtr> _branches) : branches(_branches)
{}

//Destructor 
Node::~Node()
{
  for(unsigned i = 0; i < branches.size(); i++){
    delete branches[i];
  }
}

//Visualization

std::ostream &operator<<(std::ostream &dst, const NodePtr Node)
{
  Node->generateParser(dst, "");
  return dst;
}

//MIPS
void Node::generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const {}
void Node::generateMIPSArrayGlobalAssignment(std::string indent, int dstReg, ContextPtr frameContext) const {}
void Node::generateMIPSArrayAssignment(std::string indent, int dstReg, ContextPtr frameContext) const {}
void Node::generateMIPSPointerAssignment(std::string indent, int dstReg, ContextPtr frameContext)const{}
void Node::generateMIPSPointerGlobalAssignment(std::string indent, int dstReg, ContextPtr frameContext)const{}
void Node::generateMIPSDouble(std::string indent, int dstReg, ContextPtr frameContext)const {}


//Context
void Node::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ){}


//Functions
ContextPtr Node::getContextPtr() const {}
bool Node::IsInitializeDeclaration() const {}
int Node::integerValue() const {}
enum Specifier Node::DeclarationType() const {}
std::string Node::returnIdentifier() const {}
bool Node::isAssignmentExp() const{}
void Node::countFunctionParameter(int* currentMaxArguments) const{}
std::vector<std::tuple<int, std::string>> Node::getSwitchCases() const {}
std::vector<std::string> Node::getSwitchDefault() const {}
void Node::getCaseInfo(std::vector<std::tuple<int, std::string>> &DefaultsVector){}
void Node::getDefaultInfo(std::vector<std::string> &defaultVector) {}
int Node::returnArrayNumber() const {return 0;}
std::vector<int> Node::getArrayInitializerListInt() const {}
std::vector<double> Node::getArrayInitializerListFloatDouble() const{}
std::vector<int> Node::getArrayInitializerListString() const {}
bool Node::getIsArray() const { return false; }
int Node::getValue() const {}
bool Node::hasValue() const {}
bool Node::isEnumSpecifier() const { return false; }
bool Node::IsFunctionDefinition() const { return false; }
bool Node::returnIsPointer() const {return false;};
bool Node::isFunctionDeclarator() const{};
bool Node::getHasPointerForArithmetic() const {return false;};
bool Node::isStructSpecifier() const {return false;};
bool Node::isArrayDeclarator() const{return false;};
std::string Node::returnIdentifierIndexed() const {};
enum Specifier Node::returnType() const {};
std::string Node::getFloatBinaryInDec() const {};
float Node::floatValue() const { return -1; };
double Node::doubleValue() const { return -1; };
std::string Node::stringValue() const {};
int Node::returnStringLength() const {return 0;};
std::vector<enum Specifier> Node::returnParameterTypes() const{};
std::vector<bool> Node::returnParameterIsPointer() const{};
bool Node::IsTypeDef() const {return false;};
bool Node::IsDirectDeclarator() const{return false;};

