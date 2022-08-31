#include "ast/Declaration/Declaration.hpp"
#include <math.h>
#include "ast.hpp"

std::vector <std::string> typedefNames;

Declaration::Declaration(NodePtr DeclarationSpecifier, NodePtr Declarator)
{
  
  branches.push_back(DeclarationSpecifier);
  branches.push_back(Declarator);
  if (branches[0]->IsTypeDef()){
    typedefNames.push_back(branches[1]->returnIdentifier());
  }
  
  identifierName = branches[1]->returnIdentifier();
  arraySize = branches[1]->returnArrayNumber();
  if (branches[0]->DeclarationType()==_char && branches[1]->returnIsPointer()==1){
    charType = 1;
  }
  else if (branches[0]->DeclarationType()==_char){
    charType = 0;
  }
}

Declaration::~Declaration()
{
  delete branches[0];
  delete branches[1];
}

void Declaration::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Declaration [" << std::endl;
  dst << indent << "Type: ";
  branches[0]->generateParser(dst, indent+"  ");
  branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// Context
void Declaration::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  branches[0]->generateContext(frameContext, parameterOffset,isGlobal);

  branches[1]->generateContext(frameContext,parameterOffset,isGlobal);
  type = branches[0]->DeclarationType(); 
  bool isPointer = branches[1]->returnIsPointer();
  if (branches[0]->returnIsPointer()==true){
    isPointer = branches[0]->returnIsPointer();
  }

  if (branches[0]->isStructSpecifier()==true){
    
    std::string structIdentifierName = branches[0]->returnIdentifier();
    std::vector<std::tuple<std::string,enum Specifier>> structMembersInfo = frameContext->getStructMembers(structIdentifierName); 
 
    for (unsigned i=0; i<structMembersInfo.size(); i++){
    
      int offset = 24 + 4*(frameContext->getCurrentStackSize())+parameterOffset;
      
      std::string combinedIdentifier = identifierName+"."+std::get<0>(structMembersInfo[i]);
      enum Specifier type = std::get<1>(structMembersInfo[i]);
      frameContext->changeVarBindings(combinedIdentifier,offset,type,isGlobal,isPointer,false);
      if (!isGlobal){
        frameContext->addCurrentStackSize(frameContext->getStructMemberSize(structIdentifierName));
      }
    }
  }
  else{
    double count = 0;
    // pointer of any type always take 4 bytes
 
    switch(type){
      case _double:
        count = 2;
        break;
      case _char:
        count = 0.25;
        if (branches[1]->returnIsPointer()==1 && arraySize==0){
          count = 1;
        }
        break;
      case _int:
        count = 1;
        break;
      case _float:
        count = 1;
        break;
      case _unsigned:
        count = 1;
        break;
      default:
        count = 0;
    }
    // Find the offset
    int offset;
    if (type == _char){
      offset = 24 + 4*(frameContext->getCurrentStackSize())+parameterOffset;
      if (!isGlobal){
        frameContext->addCurrentStackSize(count); 
      } 
    }
    else{
      offset = 24 + 4*(ceil(frameContext->getCurrentStackSize()))+parameterOffset;
      if (!isGlobal){
        double round_up_difference = ceil(frameContext->getCurrentStackSize()) - frameContext->getCurrentStackSize();
        frameContext->addCurrentStackSize(round_up_difference + count); 
      } 
    }
    
    // std::cout << " identifierName " << identifierName << " offset " << offset << " type " <<(type==_char) << " pointer " << isPointer<< std::endl;
    if (branches[0]->IsTypeDef()==false){
      frameContext->changeVarBindings(identifierName, offset, type, isGlobal,isPointer,false);  // 24: 4 arguments + 2 empty 
    }
    // for array: x[0]->x:{}, x[1]-> x1:{}, x[2]-> x2:{} 
    if (arraySize>1){
      for (unsigned i = 1; i<arraySize;i++){
        std::string identifierNameModified = identifierName+"ymycathy"+std::to_string(i);
        int offsetModified = offset + i*4*count;
        frameContext->changeVarBindings(identifierNameModified, offsetModified, type, isGlobal, isPointer,false);
      }
      if (!isGlobal){
        frameContext->addCurrentStackSize(count*(arraySize-1)); 
      } 
    }
  }

  
  
  if (branches[0]->IsTypeDef()==true){
    enum Specifier defType = branches[0]->returnType();
    
    std::string name = branches[1]->returnIdentifier();
    frameContext->updateTypedef(name,defType,isPointer);
  }
  blockContext = frameContext;

}

// MIPS
void Declaration::generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const
{
  
  bool IsInitDeclaration = branches[1]->IsInitializeDeclaration();
  if (branches[0]->isStructSpecifier()==true ){
    if (branches[0]->returnIdentifier() =="ymystructcathy"){
      std::string structIdentifierName = branches[0]->returnIdentifier();
      std::vector<std::tuple<std::string,enum Specifier>> structMembersInfo = frameContext->getStructMembers(structIdentifierName);
      std::string combinedIdentifier0 = identifierName+"."+std::get<0>(structMembersInfo[0]);
      
      enum Specifier typefixed = frameContext->getType(combinedIdentifier0);

      switch(typefixed){
        case _float:{
          std::vector<double> arrayInitializerListFloatDouble = branches[1]->getArrayInitializerListFloatDouble(); // wrong 
          for (unsigned i=0; i<arrayInitializerListFloatDouble.size();i++){
            std::string combinedIdentifier = identifierName+"."+std::get<0>(structMembersInfo[i]);
            int offset = blockContext -> getOffset(combinedIdentifier);
            double value = arrayInitializerListFloatDouble[i];
            int immReg = frameContext->alloCalReg();
            frameContext->dealloReg(immReg);
            std::string numberLabel = frameContext->getFloatDoubleLabel(value,_float);
            std::cout << "lui" << indent << "$" << immReg << ",%hi($" << numberLabel << ")" << std::endl;
            std::cout << "lwc1" << indent << "$f" << dstReg << ",%lo($" << numberLabel << ")($" << immReg << ")" << std::endl;
            std::cout << "swc1" << indent << "$f" << dstReg << "," << offset << "($fp)" << std::endl;
          }
          break;
        }
        case _double:{
          std::vector<double> arrayInitializerListFloat = branches[1]->getArrayInitializerListFloatDouble();
          for (unsigned i=0; i<arrayInitializerListFloat.size();i++){
            std::string combinedIdentifier = identifierName+"."+std::get<0>(structMembersInfo[i]);
            int offset = blockContext -> getOffset(identifierName);
            double value = arrayInitializerListFloat[0];
            std::string numberLabel = frameContext->getFloatDoubleLabel(value,_double);
            int immReg = frameContext->alloCalReg();
            frameContext->dealloReg(immReg);
            std::cout << "lui" << indent << "$" << immReg << ",%hi($" << numberLabel << ")" << std::endl;
            std::cout << "lwc1" << indent << "$f" << dstReg << ",%lo($" << numberLabel << "+4)($" << immReg << ")" << std::endl;
            std::cout << "lwc1" << indent << "$f" << dstReg+1 << ",%lo($" << numberLabel << ")($" << immReg << ")" << std::endl;
            std::cout << "swc1" << indent << "$f" << dstReg << "," << offset+4 << "($fp)" << std::endl;
            std::cout << "swc1" << indent << "$f" << dstReg+1 << "," << offset << "($fp)" << std::endl;
          }
          break;
        }
        default:{
    
          std::vector<int> arrayInitializerListInt = branches[1]->getArrayInitializerListInt(); // wrong 
          for (unsigned i=0; i<arrayInitializerListInt.size();i++){
            std::string combinedIdentifier = identifierName+"."+std::get<0>(structMembersInfo[i]);
            int offset = blockContext -> getOffset(combinedIdentifier);
            int value = arrayInitializerListInt[i];
            std::cout << "li" << indent << "$" << dstReg << "," << value << indent << "# 0x" << value << std::endl;
            std::cout << "sw" << indent << "$" << dstReg << "," << offset << "($fp)" << std::endl;
          }
        }
      }
    }
  }
  else {
    if (arraySize==0){
      if (IsInitDeclaration==true){
        switch (type){
          case _float:{
            if (frameContext->getIsPointer(identifierName)!=true){
              int offset = blockContext -> getOffset(identifierName);
              int immReg = frameContext->alloFloatCalReg();
              frameContext->dealloFloatReg(immReg);
              branches[1]->generateMIPS(indent,immReg,blockContext);
              std::cout << "swc1" << indent << "$f" << immReg << "," << offset << "($fp)" << std::endl;
            }
            else{
              int offset = blockContext -> getOffset(identifierName);
              branches[1]->generateMIPS(indent,dstReg,blockContext);
              std::cout << "sw" << indent << "$" << dstReg << "," << offset << "($fp)" << std::endl;
            }
        
            break;
          }
          case _double:{
            if (frameContext->getIsPointer(identifierName)!=true){
              int offset = blockContext -> getOffset(identifierName);
              int immReg = frameContext->alloFloatCalReg();
              frameContext->dealloFloatReg(immReg);
              branches[1]->generateMIPSDouble(indent,immReg,blockContext);
              std::cout << "swc1" << indent << "$f" << immReg << "," << offset+4 << "($fp)" << std::endl;
              std::cout << "swc1" << indent << "$f" << immReg+1 << "," << offset << "($fp)" << std::endl;
            }
            else{
              int offset = blockContext -> getOffset(identifierName);
              branches[1]->generateMIPS(indent,dstReg,blockContext);
              std::cout << "sw" << indent << "$" << dstReg << "," << offset << "($fp)" << std::endl;
            }
            break;
          }
          case _char:{
            // single char
            int offset = blockContext -> getOffset(identifierName);
            branches[1]->generateMIPS(indent,dstReg,blockContext);
            if (charType == 0){
              std::cout << "sb" << indent << "$" << dstReg << "," << offset << "($fp)" << std::endl;
            }
            // multi-chars string
            else if (charType == 1){
              std::cout << "sw" << indent << "$" << dstReg << "," << offset << "($fp)" << std::endl;
            }
            break;
          }
          default:{
            int offset = blockContext -> getOffset(identifierName);
            branches[1]->generateMIPS(indent,dstReg,blockContext);
            std::cout << "sw" << indent << "$" << dstReg << "," << offset << "($fp)" << std::endl;
          }
        }
      }
    }
    else{ 
      if (IsInitDeclaration==true){
        switch (type){
          case _float:{
            std::vector<double> arrayInitializerListFloat = branches[1]->getArrayInitializerListFloatDouble();
            int offset = blockContext -> getOffset(identifierName);
            double value = arrayInitializerListFloat[0];
            std::string numberLabel = frameContext->getFloatDoubleLabel(value,_float);
            int immReg = frameContext->alloCalReg();
            frameContext->dealloReg(immReg);
            std::cout << "lui" << indent << "$" << immReg << ",%hi($" << numberLabel << ")" << std::endl;
            std::cout << "lwc1" << indent << "$f" << dstReg << ",%lo($" << numberLabel << ")($" << immReg << ")" << std::endl;
            std::cout << "swc1" << indent << "$f" << dstReg << "," << offset << "($fp)" << std::endl;
                                                                                                                                                  
            for (unsigned i = 1; i<arraySize;i++){
              value = arrayInitializerListFloat[i];
              std::string numberLabel = frameContext->getFloatDoubleLabel(value,_float);
              std::string identifierNameModified = identifierName+"ymycathy"+std::to_string(i);
              offset = blockContext -> getOffset(identifierNameModified);
              std::cout << "lui" << indent << "$" << immReg << ",%hi($" << numberLabel << ")" << std::endl;
              std::cout << "lwc1" << indent << "$f" << dstReg << ",%lo($" << numberLabel << ")($" << immReg << ")" << std::endl;
              std::cout << "swc1" << indent << "$f" << dstReg << "," << offset << "($fp)" << std::endl;
            }
            break;
          }
          case _double:{
            std::vector<double> arrayInitializerListFloat = branches[1]->getArrayInitializerListFloatDouble();
            int offset = blockContext -> getOffset(identifierName);
            double value = arrayInitializerListFloat[0];
            std::string numberLabel = frameContext->getFloatDoubleLabel(value,_double);
            int immReg = frameContext->alloCalReg();
            frameContext->dealloReg(immReg);
            std::cout << "lui" << indent << "$" << immReg << ",%hi($" << numberLabel << ")" << std::endl;
            std::cout << "lwc1" << indent << "$f" << dstReg << ",%lo($" << numberLabel << "+4)($" << immReg << ")" << std::endl;
            std::cout << "lwc1" << indent << "$f" << dstReg+1 << ",%lo($" << numberLabel << ")($" << immReg << ")" << std::endl;
            std::cout << "swc1" << indent << "$f" << dstReg << "," << offset+4 << "($fp)" << std::endl;
            std::cout << "swc1" << indent << "$f" << dstReg+1 << "," << offset << "($fp)" << std::endl;
                                                                                                                                                  
            for (unsigned i = 1; i<arraySize;i++){
              value = arrayInitializerListFloat[i];
              std::string numberLabel = frameContext->getFloatDoubleLabel(value,_double);
              std::string identifierNameModified = identifierName+"ymycathy"+std::to_string(i);
              offset = blockContext -> getOffset(identifierNameModified);
              std::cout << "lui" << indent << "$" << immReg << ",%hi($" << numberLabel << ")" << std::endl;
              std::cout << "lwc1" << indent << "$f" << dstReg << ",%lo($" << numberLabel << "+4)($" << immReg << ")" << std::endl;
              std::cout << "lwc1" << indent << "$f" << dstReg+1 << ",%lo($" << numberLabel << ")($" << immReg << ")" << std::endl;
              std::cout << "swc1" << indent << "$f" << dstReg << "," << offset+4 << "($fp)" << std::endl;
              std::cout << "swc1" << indent << "$f" << dstReg+1 << "," << offset << "($fp)" << std::endl;
            }
            break;
          }
          case _char:{
            std::vector<int> arrayInitializerListString = branches[1]->getArrayInitializerListString();
            int offset = blockContext -> getOffset(identifierName);
            int value = arrayInitializerListString[0];
            std::cout << "li" << indent << "$" << dstReg << "," << value << indent << "# 0x" << value << std::endl;
            std::cout << "sb" << indent << "$" << dstReg << "," << offset << "($fp)" << std::endl;

            for (unsigned i = 1; i<arraySize;i++){
              value = arrayInitializerListString[i];
              std::string identifierNameModified = identifierName+"ymycathy"+std::to_string(i);
              offset = blockContext -> getOffset(identifierNameModified);
              std::cout << "li" << indent << "$" << dstReg << "," << value << indent << "# 0x" << value << std::endl;
              std::cout << "sb" << indent << "$" << dstReg << "," << offset << "($fp)" << std::endl;
            }    
            break;                                                                              
          }
          default:{
            std::vector<int> arrayInitializerListInt = branches[1]->getArrayInitializerListInt();
            int offset = blockContext -> getOffset(identifierName);
            int value = arrayInitializerListInt[0];
            std::cout << "li" << indent << "$" << dstReg << "," << value << indent << "# 0x" << value << std::endl;
            std::cout << "sw" << indent << "$" << dstReg << "," << offset << "($fp)" << std::endl;
                                                                                                                                                  
            for (unsigned i = 1; i<arraySize;i++){
              value = arrayInitializerListInt[i];
              std::string identifierNameModified = identifierName+"ymycathy"+std::to_string(i);
              offset = blockContext -> getOffset(identifierNameModified);
              std::cout << "li" << indent << "$" << dstReg << "," << value << indent << "# 0x" << value << std::endl;
              std::cout << "sw" << indent << "$" << dstReg << "," << offset << "($fp)" << std::endl;
            }
          }
        }
      }
    }
  }
}

// Functions
bool Declaration::IsInitializeDeclaration()const{
  return branches[1]->IsInitializeDeclaration();
}

enum Specifier Declaration::DeclarationType()const
{
  return type;
}

std::string Declaration::returnIdentifier() const
{
  return identifierName;
}

int Declaration::integerValue()const
{
    return branches[1]->integerValue();
}

float Declaration::floatValue()const
{
    return branches[1]->floatValue();
}

double Declaration::doubleValue()const
{
    return branches[1]->doubleValue();
}

std::string Declaration::stringValue()const
{
    return branches[1]->stringValue();
}

void Declaration::countFunctionParameter(int* functionCallParameterOffset) const{
    return branches[1]->countFunctionParameter(functionCallParameterOffset);
}

std::vector<int> Declaration::getArrayInitializerListInt() const{
  return branches[1]->getArrayInitializerListInt();
}

std::vector<double> Declaration::getArrayInitializerListFloatDouble() const{
  return branches[1]->getArrayInitializerListFloatDouble();
}

std::vector<int> Declaration::getArrayInitializerListString() const{
  return branches[1]->getArrayInitializerListString();
}


int Declaration::returnArrayNumber() const{
  return branches[1]->returnArrayNumber();
} 

bool Declaration::isFunctionDeclarator() const{
  return branches[1]->isFunctionDeclarator();
} 

std::vector<enum Specifier> Declaration::returnParameterTypes() const{
  std::vector<enum Specifier> parameterTypes = branches[1]->returnParameterTypes();
  parameterTypes.push_back(type);
  return parameterTypes;
}

std::vector<bool> Declaration::returnParameterIsPointer() const{
  std::vector<bool> parameterIsPointer = branches[1]->returnParameterIsPointer();
  parameterIsPointer.push_back(0);
  return parameterIsPointer;
}

bool Declaration::IsTypeDef() const{
  return branches[0]->IsTypeDef();
}

bool Declaration::IsDirectDeclarator() const{
  return branches[1]->IsDirectDeclarator();
}