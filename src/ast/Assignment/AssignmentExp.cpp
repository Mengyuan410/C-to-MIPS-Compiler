#include "ast/Assignment/AssignmentExp.hpp"

// Constructor
AssignmentExp::AssignmentExp(NodePtr assignmentExpression)
{
  branches.push_back(assignmentExpression);
}

// Destructor
AssignmentExp::~AssignmentExp()
{
  delete branches[0];
}

// Visualising
void AssignmentExp::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "AssignmentExp [" << std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}



// MIPS
void AssignmentExp::generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const 
{
  branches[0] -> generateMIPS(indent, dstReg, frameContext);
}

// Context
void AssignmentExp::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) 
{
    branches[0] -> generateContext(frameContext, parameterOffset, isGlobal);
    blockContext = frameContext;
} 


//Functions
bool AssignmentExp::isAssignmentExp() const
{
    return true;
}

// Functions
void AssignmentExp::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[0]->countFunctionParameter(functionCallParameterOffset);
}

