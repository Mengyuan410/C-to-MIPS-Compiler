#include "ast/Array/ArrayIndex.hpp"

// Constructors


ArrayIndex::ArrayIndex(NodePtr arrayIdentifier, NodePtr arrayIndex){
  branches.push_back(arrayIdentifier);
  branches.push_back(arrayIndex);
}

ArrayIndex::~ArrayIndex()
{
  delete branches[0];
  delete branches[1];
}


// Visualising
void ArrayIndex::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Array Index [" << std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// Context
void ArrayIndex::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
    blockContext = frameContext;
    branches[0]->generateContext(blockContext, parameterOffset, isGlobal);  
    branches[1]->generateContext(blockContext, parameterOffset, isGlobal);  
    stackStartAddress = parameterOffset+24;
}


// MIPS
void ArrayIndex::generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const
{  
    std::string varName = branches[0]->returnIdentifier();
    int offset = frameContext->getOffset(varName);
    enum Specifier type = branches[0]->returnType();
    bool isGlobalArray = frameContext->getIsGlobal(varName);
    switch (type){
      
      case _float:{
    
        int leftReg = frameContext->alloCalReg();
        int rightReg = frameContext->alloCalReg();
        frameContext->dealloReg(leftReg);
        frameContext->dealloReg(rightReg);
        
        if (isGlobalArray==true){
          branches[1]->generateMIPS(indent,leftReg,frameContext);
          std::cout << "sll" << indent << "$" << leftReg << ",$" << leftReg << ",2" << std::endl;
          std::cout << "lui" << indent << "$" << rightReg << ",%hi(" << varName << ")" << std::endl;
          std::cout << "addiu" << indent << "$" << rightReg << ",$" << rightReg << ",%lo(" << varName << ")" << std::endl;
          std::cout << "addu" << indent << "$" << rightReg << ",$" << leftReg << ",$" <<rightReg << std::endl; 
          std::cout << "lwc1" << indent << "$f" << dstReg << ",0($" << rightReg << ")" << std::endl;
        }
        else{
          //没做完什么parameter啥玩意的还没做
          branches[1]->generateMIPS(indent,leftReg,frameContext);
          std::cout << "sll" << indent << "$" << leftReg << ",$" << leftReg << ",2" << std::endl; 
          std::cout << "addiu" << indent << "$" << rightReg <<  ",$fp," << stackStartAddress << std::endl;
          std::cout << "addu" << indent << "$" << rightReg << ",$" << leftReg << ",$" <<rightReg << std::endl;  
          std::cout << "lwc1" << indent << "$f" << dstReg << "," << (offset-stackStartAddress) << "($" << rightReg << ")" << std::endl; 
        }
        break;
      }
      case _char:{
        //最普通情况，global和pointer没做呢
        int leftReg = frameContext->alloCalReg();
        int rightReg = frameContext->alloCalReg();
        int immReg = frameContext->alloCalReg();
        frameContext->dealloReg(leftReg);
        frameContext->dealloReg(rightReg);
        frameContext->dealloReg(immReg);

        bool isGlobalArray = frameContext->getIsGlobal(varName);
        if (isGlobalArray==true){
          std::string modifiedName = varName+"ymycathy1";
          if (frameContext->getVarBindings().count(modifiedName)){
            branches[1]->generateMIPS(indent,leftReg,frameContext);
            std::cout << "sll" << indent << "$" << leftReg << ",$" << leftReg << ",3" << std::endl; 
            frameContext->alloReg(leftReg);
            std::cout << "lui" << indent << "$" << immReg  << ",%hi(" << varName << ")"<< std::endl;
            std::cout << "addiu" << indent << "$" << rightReg << ",%lo(" << varName << ")" << std::endl;
            frameContext->dealloReg(leftReg);
            std::cout << "addu" << indent << "$" << dstReg << ",$" << leftReg << ",$" <<rightReg << std::endl; 
            std::cout << "lb" << indent << "$" << dstReg << ",0($" << dstReg << ")" << std::endl;
          }
          else{
            
            branches[1]->generateMIPS(indent,leftReg,frameContext);
          
            std::cout << "lui" << indent << "$" << immReg  << ",%hi(" << varName << ")"<< std::endl;
            std::cout << "lw" << indent << "$" << rightReg << ",%lo(" << varName << ")($"<<immReg << ")" << std::endl;
            std::cout << "addu" << indent << "$" << dstReg << ",$" << leftReg << ",$" <<rightReg << std::endl; 
            std::cout << "lb" << indent << "$" << dstReg << ",0($" << dstReg << ")" << std::endl;

          }
        }
        else{
          branches[1]->generateMIPS(indent,leftReg,frameContext);
          frameContext->alloReg(leftReg);
          std::cout << "lw" << indent << "$" << rightReg << "," << offset << "($fp)" << std::endl;
          std::cout << "addu" << indent << "$" << leftReg << ",$" << leftReg << ",$" <<rightReg << std::endl;  
          frameContext->dealloReg(leftReg);
          std::cout << "lbu" << indent << "$" << dstReg << ",0($" << leftReg << ")" << std::endl;
        }
        break;
      }
      default:{
        int leftReg = frameContext->alloCalReg();
        int rightReg = frameContext->alloCalReg();
        frameContext->dealloReg(leftReg);
        frameContext->dealloReg(rightReg);

        if (isGlobalArray==true){
          branches[1]->generateMIPS(indent,leftReg,frameContext);
          std::cout << "sll" << indent << "$" << leftReg << ",$" << leftReg << ",2" << std::endl;
          std::cout << "lui" << indent << "$" << rightReg << ",%hi(" << varName << ")" << std::endl;
          std::cout << "addiu" << indent << "$" << rightReg << ",$" << rightReg << ",%lo(" << varName << ")" << std::endl;
          std::cout << "addu" << indent << "$" << rightReg << ",$" << leftReg << ",$" <<rightReg << std::endl; 
          std::cout << "lw" << indent << "$" << dstReg << ",0($" << rightReg << ")" << std::endl;
        }
        else{
          branches[1]->generateMIPS(indent,leftReg,frameContext);
          std::cout << "sll" << indent << "$" << leftReg << ",$" << leftReg << ",2" << std::endl; 
          if (frameContext->getIsParameterPointer(varName)==false){ // the array is not passed into the function as a parameter
            std::cout << "addiu" << indent << "$" << rightReg <<  ",$fp," << stackStartAddress << std::endl;
            std::cout << "addu" << indent << "$" << rightReg << ",$" << leftReg << ",$" <<rightReg << std::endl;  
            std::cout << "lw" << indent << "$" << dstReg << "," << (offset-stackStartAddress) << "($" << rightReg << ")" << std::endl; 
          }
          else{
            std::cout << "lw" << indent << "$" << rightReg << "," << offset << "($fp)" << std::endl;
            std::cout << "addu" << indent << "$" << rightReg << ",$" << leftReg << ",$" <<rightReg << std::endl;  
            std::cout << "lw" << indent << "$" << dstReg << ",0($" << rightReg << ")" << std::endl;
          }
        }
      }
    }

    
}

void ArrayIndex::generateMIPSArrayGlobalAssignment(std::string indent, int dstReg, ContextPtr frameContext) const
{
    std::string varName = branches[0]->returnIdentifier();
    int leftReg = frameContext->alloCalReg();
    int rightReg = frameContext->alloCalReg();
    frameContext->dealloReg(leftReg);
    frameContext->dealloReg(rightReg);

    branches[1]->generateMIPS(indent,leftReg,frameContext);
    std::cout << "sll" << indent << "$" << leftReg << ",$" << leftReg << ",2" << std::endl; 
    std::cout << "lui" << indent << "$" << rightReg << ",%hi(" << varName << ")" << std::endl;
    std::cout << "addiu" << indent << "$" << rightReg << ",$" << rightReg << ",%lo(" << varName << ")" << std::endl;  
    std::cout << "addu" << indent << "$" << dstReg << ",$" << leftReg << ",$" <<rightReg << std::endl;  
}

void ArrayIndex::generateMIPSArrayAssignment(std::string indent, int dstReg, ContextPtr frameContext) const
{
  enum Specifier type = branches[0]->returnType();
  switch (type){
    case _double:{
      int leftReg = frameContext->alloCalReg();
      int rightReg = frameContext->alloCalReg();
      frameContext->dealloReg(leftReg);
      frameContext->dealloReg(rightReg);

      branches[1]->generateMIPS(indent,leftReg,frameContext);
      std::cout << "sll" << indent << "$" << leftReg << ",$" << leftReg << ",3" << std::endl; 
      std::cout << "addiu" << indent << "$" << rightReg <<  ",$fp," << stackStartAddress << std::endl;
      std::cout << "addu" << indent << "$" << dstReg << ",$" << leftReg << ",$" <<rightReg << std::endl; 
      break;
    }
    case _char:{
      std::string varName = branches[0]->returnIdentifier();
      int offset = frameContext->getOffset(varName);
      int leftReg = frameContext->alloCalReg();
      int rightReg = frameContext->alloCalReg();
      frameContext->dealloReg(leftReg);
      frameContext->dealloReg(rightReg);
      branches[1]->generateMIPS(indent,leftReg,frameContext);
      std::cout << "lw" << indent << "$" << rightReg << "," << offset << "($fp)" << std::endl;
      std::cout << "addu" << indent << "$" << dstReg << ",$" << leftReg << ",$" <<rightReg << std::endl;
      break;  
    }
    default:{ 
      int leftReg = frameContext->alloCalReg();
      int rightReg = frameContext->alloCalReg();
      frameContext->dealloReg(leftReg);
      frameContext->dealloReg(rightReg);

      branches[1]->generateMIPS(indent,leftReg,frameContext);
      std::cout << "sll" << indent << "$" << leftReg << ",$" << leftReg << ",2" << std::endl; 
      std::cout << "addiu" << indent << "$" << rightReg <<  ",$fp," << stackStartAddress << std::endl;
      std::cout << "addu" << indent << "$" << dstReg << ",$" << leftReg << ",$" <<rightReg << std::endl; 
    }
  }
}

// Functions
std::string ArrayIndex::returnIdentifier() const{
    return branches[0]->returnIdentifier();
}
std::string ArrayIndex::returnIdentifierIndexed() const{
  std::string arrayName = branches[0]->returnIdentifier();
  int index = branches[1]->integerValue();
  if (index==0){
    return arrayName;
  }
  else{
    return arrayName+"ymycathy"+std::to_string(index);
  }
}

bool ArrayIndex::getIsArray() const{
    return true;
}

enum Specifier ArrayIndex::returnType() const{
  return branches[0]->returnType();
}
