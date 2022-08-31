#ifndef Node_hpp
#define Node_hpp

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <bitset>

#include "ast/Context.hpp"

class Node;

typedef Node *NodePtr;  // const or not?

class Node
{
public:
    //constructor
    Node();
    Node(std::vector<NodePtr> _branches);
    

    virtual ~Node();

    //visualize parsed result
    virtual void generateParser (std::ostream &dst,std::string indent) const = 0;
    friend std::ostream& operator<<(std::ostream &dst, const NodePtr Node);

    //generate MIPS code
    virtual void generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const;
    virtual void generateMIPSArrayGlobalAssignment(std::string indent, int dstReg, ContextPtr frameContext) const;
    virtual void generateMIPSArrayAssignment(std::string indent, int dstReg, ContextPtr frameContext) const;
    virtual void generateMIPSPointerAssignment(std::string indent, int dstReg, ContextPtr frameContext) const;
    virtual void generateMIPSPointerGlobalAssignment(std::string indent, int dstReg, ContextPtr frameContext)const;
    virtual void generateMIPSDouble(std::string indent, int dstReg, ContextPtr frameContext)const;


    //context
    virtual void generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  );

    //Functions
    virtual ContextPtr getContextPtr() const;
    virtual enum Specifier DeclarationType() const;
    virtual bool IsInitializeDeclaration() const;
    virtual std::string returnIdentifier() const;
    virtual int integerValue() const;
    virtual bool isAssignmentExp() const;
    virtual void countFunctionParameter(int* currentMaxArguments) const;
    virtual std::vector<std::tuple<int, std::string>> getSwitchCases() const;
    virtual std::vector<std::string> getSwitchDefault() const;
    virtual void getCaseInfo(std::vector<std::tuple<int, std::string>> &DefaultsVector);
    virtual void getDefaultInfo(std::vector<std::string> &defaultVector);
    virtual int returnArrayNumber() const;
    virtual std::vector<int> getArrayInitializerListInt() const;
    virtual std::vector<double> getArrayInitializerListFloatDouble() const;
    virtual std::vector<int> getArrayInitializerListString() const;
    virtual bool getIsArray() const;
    virtual int getValue() const;
    virtual bool hasValue() const;
    virtual bool isEnumSpecifier() const;
    virtual bool IsFunctionDefinition() const;
    virtual bool returnIsPointer() const;
    virtual bool isFunctionDeclarator() const;
    virtual bool getHasPointerForArithmetic() const;
    virtual bool isStructSpecifier() const;
    virtual bool isArrayDeclarator() const;
    virtual std::string returnIdentifierIndexed() const;
    virtual enum Specifier returnType() const;
    virtual std::string getFloatBinaryInDec() const;
    virtual float floatValue() const;
    virtual double doubleValue() const;
    virtual int returnStringLength() const;
    virtual std::string stringValue() const;
    virtual std::vector<enum Specifier> returnParameterTypes() const;
    virtual std::vector<bool> returnParameterIsPointer() const;
    virtual bool IsTypeDef() const;
    virtual bool IsDirectDeclarator() const;
    
protected:
    std::vector<NodePtr> branches;
};

#endif


