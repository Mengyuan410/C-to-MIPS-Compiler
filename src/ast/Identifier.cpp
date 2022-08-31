#include "ast/Identifier.hpp"

// Constructor
Identifier::Identifier(std::string _id) : id(_id){}

// Visualising
void Identifier::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Identifier: " << id << std::endl;
}

std::string Identifier::returnIdentifier() const{
  return id;
}

// Context
void Identifier::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
  type = frameContext->getType(id);
}

// MIPS

void Identifier::generateMIPS(std::string indent, int destReg, ContextPtr frameContext) const
{
  bool isGlobal = frameContext->getIsGlobal(id);
  int offset = frameContext->getOffset(id);
  bool isEnum = frameContext->isEnum(id);
  bool isPointer = frameContext->getIsPointer(id);
  switch(type){
    // float
    case _float:{
      if (isPointer==true){
        if (isGlobal){
          std::cout << "lui" << indent << "$" << destReg << ",%hi(" << id << ")"<< std::endl;
          std::cout << "lw" << indent << "$" << destReg << ",%lo(" << id << ")($" << destReg << ")"<< std::endl;
        }
        else if (isEnum){
          int value = frameContext->getEnumValue(id);
          std::cout << "li" << indent << "$" << destReg << "," << value << std::endl;
        }
        else {
          std::cout << "lw" << indent << "$"<<destReg << "," << offset << "($fp)" << std::endl;
        }
      }
      else{
        int intermediateReg = frameContext->alloCalReg();
        frameContext->dealloReg(intermediateReg);
        if (isGlobal){
          std::cout << "lui" << indent << "$" << intermediateReg  << ",%hi(" << id << ")"<< std::endl;
          std::cout << "lwc1" << indent << "$f" << destReg << ",%lo(" << id << ")($" << intermediateReg << ")" << std::endl;
        }
        else {
          std::cout << "lwc1" << indent << "$f" << destReg << "," << offset << "($fp)" << std::endl;
        }
      }
      break;
    }
    case _double:{
      if (isPointer==true){
        if (isGlobal){
          std::cout << "lui" << indent << "$" << destReg << ",%hi(" << id << ")"<< std::endl;
          std::cout << "lw" << indent << "$" << destReg << ",%lo(" << id << ")($" << destReg << ")"<< std::endl;
        }
        else if (isEnum){
          int value = frameContext->getEnumValue(id);
          std::cout << "li" << indent << "$" << destReg << "," << value << std::endl;
        }
        else {
          std::cout << "lw" << indent << "$"<<destReg << "," << offset << "($fp)" << std::endl;
        }
      }
      else{
        bool isGlobal = frameContext->getIsGlobal(id);
        int offset = frameContext->getOffset(id);
        int intermediateReg = frameContext->alloCalReg();
        frameContext->dealloReg(intermediateReg);
        if (isGlobal){
          std::cout << "lui" << indent << "$" << intermediateReg  << ",%hi(" << id << ")"<< std::endl;
          std::cout << "lwc1" << indent << "$f" << destReg << ",%lo(" << id << "+4)($" << intermediateReg << ")" << std::endl;
          std::cout << "lwc1" << indent << "$f" << destReg+1 << ",%lo(" << id << ")($" << intermediateReg << ")" << std::endl;
        }
        else {
          std::cout << "lwc1" << indent << "$f" << destReg << "," << offset+4 << "($fp)" << std::endl;
          std::cout << "lwc1" << indent << "$f" << destReg+1 << "," << offset << "($fp)" << std::endl;
        }
      }
      break;
    }
    case _char:{
      int intermediateReg = frameContext->alloCalReg();
      frameContext->dealloReg(intermediateReg);
      if (isGlobal){
        std::cout << "lui" << indent << "$" << intermediateReg  << ",%hi(" << id << ")"<< std::endl;
        std::cout << "lb" << indent << "$" << destReg << ",%lo(" << id << ")($" << intermediateReg << ")" << std::endl;
      }
      else {
        if (frameContext->getIsPointer(id) == true){
          std::cout << "lw" << indent << "$" << destReg << "," << offset << "($fp)" << std::endl;
        }
        else{
          std::cout << "lb" << indent << "$" << destReg << "," << offset << "($fp)" << std::endl;
        }
      }
      break;
    }
    // int
    default:{
      if (isGlobal){
        std::cout << "lui" << indent << "$" << destReg << ",%hi(" << id << ")"<< std::endl;
        std::cout << "lw" << indent << "$" << destReg << ",%lo(" << id << ")($" << destReg << ")"<< std::endl;
      }
      else if (isEnum){
        int value = frameContext->getEnumValue(id);
        std::cout << "li" << indent << "$" << destReg << "," << value << std::endl;
      }
      else {
        std::cout << "lw" << indent << "$"<<destReg << "," << offset << "($fp)" << std::endl;
      }
    }
  }
}

void Identifier::generateMIPSDouble(std::string indent, int destReg, ContextPtr frameContext) const{
  bool isGlobal = frameContext->getIsGlobal(id);
  int offset = frameContext->getOffset(id);
  int intermediateReg = frameContext->alloCalReg();
  frameContext->dealloReg(intermediateReg);
  if (isGlobal){
    std::cout << "lui" << indent << "$" << intermediateReg  << ",%hi(" << id << ")"<< std::endl;
    std::cout << "lwc1" << indent << "$f" << destReg << ",%lo(" << id << "+4)($" << intermediateReg << ")" << std::endl;
    std::cout << "lwc1" << indent << "$f" << destReg+1 << ",%lo(" << id << ")($" << intermediateReg << ")" << std::endl;
  }
  else {
    std::cout << "lwc1" << indent << "$f" << destReg << "," << offset+4 << "($fp)" << std::endl;
    std::cout << "lwc1" << indent << "$f" << destReg+1 << "," << offset << "($fp)" << std::endl;
  }
}

// Functions

void Identifier::countFunctionParameter(int* functionCallParameterOffset) const{}

bool Identifier::getHasPointerForArithmetic() const{
  enum Specifier type = blockContext->getType(id);
  if (type==_char){
    return false;
  }
  else if (blockContext->getIsPointer(id)==true){
    return true;
  } 
  else{
    return false;
  }
}
enum Specifier Identifier::returnType() const{
  return type;
}

enum Specifier Identifier::DeclarationType() const{
  return type;
}
