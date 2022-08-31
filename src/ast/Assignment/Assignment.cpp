#include "ast/Assignment/Assignment.hpp"

// Constructor
Assignment::Assignment(NodePtr left, NodePtr right)
{
  branches.push_back(left);
  branches.push_back(right);
}

// Destructor
Assignment::~Assignment()
{
  delete branches[0];
  delete branches[1];
}

// Visualising
void Assignment::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Assignment [" << std::endl;
  dst << indent << "Left ["<<std::endl;
  branches[0]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
  dst << indent << "Right ["<<std::endl;
  branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

// Context
void Assignment::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ) {
  blockContext = frameContext;
  branches[0] -> generateContext(blockContext, parameterOffset, isGlobal);
  branches[1] -> generateContext(blockContext, parameterOffset, isGlobal);
  type = branches[0]->returnType();
  stackStartAddress = parameterOffset+24;
}

// MIPS
void Assignment::generateMIPS(std::string indent, int dstReg, ContextPtr frameContext ) const 
{
  
  std::string varName = branches[0] -> returnIdentifier();

  bool isArrayAssignment = branches[0]->getIsArray();
  bool isPointerAssignment = branches[0]->returnIsPointer();

  int offset = frameContext->getOffset(varName);
  bool isGlobal = frameContext->getIsGlobal(varName);

  switch (type){
    case _float:{
      if(isGlobal){
        if(isArrayAssignment==false){
          if (isPointerAssignment==true){
            int addressReg = frameContext->alloCalReg();
            int immReg = frameContext->alloFloatCalReg();
            frameContext->dealloReg(addressReg);
            frameContext->dealloFloatReg(immReg);
            branches[1]->generateMIPS(indent,immReg,frameContext);
            std::cout << "lui" << indent << "$" << addressReg << ",%hi(" << varName << ")" << std::endl;
            std::cout << "lw" << indent << "$" << addressReg << ",%lo(" << varName << ")($" << addressReg << ")" << std::endl;
            std::cout << "swc1" << indent << "$f" << immReg << ",0($" << addressReg << ")" << std::endl;
          }
          else{
            int addressReg = frameContext->alloCalReg();
            int immReg = frameContext->alloFloatCalReg();
            frameContext->dealloReg(addressReg);
            frameContext->dealloFloatReg(immReg);
            branches[1]->generateMIPS(indent,immReg,frameContext);
            std::cout << "lui" << indent << "$" << addressReg << ",%hi(" << varName << ")" << std::endl;
            std::cout << "swc1" << indent << "$f" << immReg << ",%lo(" << varName << ")($" << addressReg << ")" << std::endl;
          }
        }
        else{

          int addressReg = frameContext->alloCalReg();
          int immReg = frameContext->alloFloatCalReg();
          frameContext->dealloReg(addressReg);
          frameContext->dealloFloatReg(immReg);
          branches[1]->generateMIPS(indent,immReg,frameContext);
          std::cout << "lui" << indent << "$" << addressReg << ",%hi(" << varName << ")" << std::endl;
          std::cout << "swc1" << indent << "$f" << immReg << ",%lo(" << varName << ")($" << addressReg << ")" << std::endl;
        }
      }
      else{
        if(isArrayAssignment==false){
          if (isPointerAssignment==true){
            int leftReg = frameContext->alloCalReg();
            int rightReg = frameContext->alloFloatCalReg();
            frameContext->dealloReg(leftReg);
            frameContext->dealloFloatReg(rightReg);
            branches[0] -> generateMIPSPointerAssignment(indent,leftReg,frameContext);
            frameContext->alloReg(leftReg);
            branches[1]->generateMIPS(indent,rightReg,frameContext);
            frameContext->dealloReg(leftReg);
            std::cout << "swc1" << indent << "$f" << rightReg << ",0($" << leftReg << ")" << std::endl;
          }
          else{
            if (frameContext->getIsPointer(branches[0]->returnIdentifier())==true){
              int immReg = frameContext->alloCalReg();
              frameContext->dealloReg(immReg);
              branches[1]->generateMIPS(indent,immReg,frameContext);
              std::cout<< "sw" << indent << "$" << immReg << "," << offset<< "($fp)"<<std::endl;
            }
            else{
              int immReg = frameContext->alloFloatCalReg();
              frameContext->dealloFloatReg(immReg);
              branches[1]->generateMIPS(indent,immReg,frameContext);
              std::cout<< "swc1" << indent << "$f" << immReg << "," << offset<< "($fp)"<<std::endl;
            }
          }
        }
        else{
          
          int rightReg = frameContext->alloFloatCalReg();
          int addressReg = frameContext->alloCalReg();
          frameContext->dealloReg(addressReg);
          frameContext->dealloFloatReg(rightReg);

          branches[0]->generateMIPSArrayAssignment(indent,addressReg,frameContext);
          frameContext->alloReg(addressReg);
          branches[1]->generateMIPS(indent,rightReg,frameContext);
          frameContext->dealloReg(addressReg);
          std::cout << "swc1" << indent << "$f" << rightReg << "," << offset-stackStartAddress << "($" << addressReg << ")" << std::endl;
        }
      }
      break;
    }
    case _double:{
      if(isGlobal){
        
        if(isArrayAssignment==false){
          if (isPointerAssignment==true){
            int addressReg = frameContext->alloCalReg();
            int immReg = frameContext->alloFloatCalReg();
            frameContext->dealloReg(addressReg);
            frameContext->dealloFloatReg(immReg);
            branches[1]->generateMIPS(indent,immReg,frameContext);
            std::cout << "lui" << indent << "$" << addressReg << ",%hi(" << varName << ")" << std::endl;
            std::cout << "lw" << indent << "$" << addressReg << ",%lo(" << varName << ")($" << addressReg << ")" << std::endl;
            std::cout << "swc1" << indent << "$f" << immReg << ",4($" << addressReg << ")" << std::endl;
            std::cout << "swc1" << indent << "$f" << immReg+1 << ",0($" << addressReg << ")" << std::endl;
          }
          else{
            if (frameContext->getIsPointer(branches[0]->returnIdentifier())==true){
           
              int leftReg = frameContext->alloCalReg();
              int rightReg = frameContext->alloCalReg();
              frameContext->dealloReg(leftReg);
              frameContext->dealloReg(rightReg);
              branches[1]->generateMIPS(indent,leftReg,frameContext);
              std::cout << "lui" << indent << "$" << rightReg << ",%hi(" << varName << ")" << std::endl;
              std::cout << "sw" << indent << "$" << leftReg << ",%lo(" << varName << ")($" << rightReg << ")" << std::endl;
            }
            else{
             
              int immReg = frameContext->alloFloatCalReg();
              frameContext->dealloFloatReg(immReg);
              branches[1]->generateMIPSDouble(indent,immReg,frameContext);
              int immReg2 = frameContext->alloCalReg();
              frameContext->dealloReg(immReg2);
              std::cout << "lui" << indent << "$" << immReg2 << ",%hi(" << varName << ")" << std::endl;
              std::cout<< "swc1" << indent << "$f" << immReg << ",%lo(" << varName << "+4)($" << immReg2 << ")" <<std::endl;
              std::cout<< "swc1" << indent << "$f" << immReg+1 << ",%lo(" << varName << ")($" << immReg2 << ")" << std::endl;
            }
          }
        }
        else{
          int rightReg = frameContext->alloFloatCalReg();
          int addressReg = frameContext->alloCalReg();
          frameContext->dealloReg(addressReg);
          frameContext->dealloFloatReg(rightReg);

          branches[0]->generateMIPSArrayAssignment(indent,addressReg,frameContext);
          frameContext->alloReg(addressReg);
          branches[1]->generateMIPSDouble(indent,rightReg,frameContext);
          frameContext->dealloReg(addressReg);
          std::cout << "swc1" << indent << "$f" << rightReg << "," << offset-stackStartAddress+4 << "($" << addressReg << ")" << std::endl;
          std::cout << "swc1" << indent << "$f" << rightReg+1 << "," << offset-stackStartAddress << "($" << addressReg << ")" << std::endl;

        }
      }
      else{
        if(isArrayAssignment==false){
          if (isPointerAssignment==true){
            int leftReg = frameContext->alloCalReg();
            int rightReg = frameContext->alloFloatCalReg();
            frameContext->dealloReg(leftReg);
            frameContext->dealloFloatReg(rightReg);
            branches[0] -> generateMIPSPointerAssignment(indent,leftReg,frameContext);
            frameContext->alloReg(leftReg);
            branches[1]->generateMIPS(indent,rightReg,frameContext);
            frameContext->dealloReg(leftReg);
            std::cout << "swc1" << indent << "$f" << rightReg << ",4($" << leftReg << ")" << std::endl;
            std::cout << "swc1" << indent << "$f" << rightReg+1 << ",0($" << leftReg << ")" << std::endl;
          }
          else{
            if (frameContext->getIsPointer(branches[0]->returnIdentifier())==true){
              int immReg = frameContext->alloCalReg();
              frameContext->dealloReg(immReg);
              branches[1]->generateMIPS(indent,immReg,frameContext);
              std::cout<< "sw" << indent << "$" << immReg << "," << offset<< "($fp)"<<std::endl;
            }
            else{
              int immReg = frameContext->alloFloatCalReg();
              frameContext->dealloFloatReg(immReg);
              branches[1]->generateMIPSDouble(indent,immReg,frameContext);
              std::cout<< "swc1" << indent << "$f" << immReg << "," << offset+4<< "($fp)"<<std::endl;
              std::cout<< "swc1" << indent << "$f" << immReg+1 << "," << offset<< "($fp)"<<std::endl;
            }
          }
        }
        else{
          int rightReg = frameContext->alloFloatCalReg();
          int addressReg = frameContext->alloCalReg();
          frameContext->dealloReg(addressReg);
          frameContext->dealloFloatReg(rightReg);

          branches[0]->generateMIPSArrayAssignment(indent,addressReg,frameContext);
          frameContext->alloReg(addressReg);
          branches[1]->generateMIPSDouble(indent,rightReg,frameContext);
          frameContext->dealloReg(addressReg);
          std::cout << "swc1" << indent << "$f" << rightReg << "," << offset-stackStartAddress+4 << "($" << addressReg << ")" << std::endl;
          std::cout << "swc1" << indent << "$f" << rightReg+1 << "," << offset-stackStartAddress << "($" << addressReg << ")" << std::endl;
        }
      }
      break;
    }
    case _char:{
      if (isGlobal){
        if (isArrayAssignment==false){
          if (isPointerAssignment == true){
            // 没做呢
            std::cout << "TO BE DONE" << std::endl;
          }
          else{
            // 没做呢
            std::cout << "TO BE DONE" << std::endl;
          }
        }
        else{
          // 没做呢
          std::cout << "TO BE DONE" << std::endl;
        }
      }
      else{
        if (isArrayAssignment==false){
          if(isPointerAssignment == true){
            std::cout << "TO BE DONE" << std::endl;
          }
          else{
            int immReg = frameContext->alloCalReg();
            frameContext->dealloReg(immReg);
            
            branches[1]->generateMIPS(indent,immReg,frameContext);
            if ((frameContext->getIsPointer(branches[0]->returnIdentifier()))==true) {
              std::cout<< "sw" << indent << "$" << immReg << "," << offset<< "($fp)"<<std::endl;
            }
            else{
              std::cout<< "sb" << indent << "$" << immReg << "," << offset<< "($fp)"<<std::endl;
            }
          }
        }
        else{
          int leftReg = frameContext->alloCalReg();
          int rightReg = frameContext->alloCalReg();
          frameContext->dealloReg(leftReg);
          frameContext->dealloReg(rightReg);
          branches[0]->generateMIPSArrayAssignment(indent,leftReg,frameContext);
          frameContext->alloReg(leftReg);
          branches[1]->generateMIPS(indent,rightReg,frameContext);
          frameContext->dealloReg(leftReg);
          frameContext->clearReservedRegsForFunctionCall();
          std::cout << "sb" << indent << "$" << rightReg << ",0($" << leftReg << ")" << std::endl;
        }
      }
      break;
    }
  

    default:{
      if (isGlobal){
        if (isArrayAssignment==false){
          if (isPointerAssignment == true){
            int leftReg = frameContext->alloCalReg();
            int rightReg = frameContext->alloCalReg();
            frameContext->dealloReg(leftReg);
            frameContext->dealloReg(rightReg);
            branches[0] -> generateMIPSPointerGlobalAssignment(indent,leftReg,frameContext);
            frameContext->alloReg(leftReg);
            branches[1]->generateMIPS(indent,rightReg,frameContext);
            std::cout << "sw" << indent << "$" << rightReg << ",0($" << leftReg << ")" << std::endl;
          }
          else{
            int addressReg = frameContext->alloCalReg();
            int immReg = frameContext->alloCalReg();
            frameContext->dealloReg(addressReg);
            frameContext->dealloReg(immReg);
            branches[1]->generateMIPS(indent,immReg,frameContext);
            std::cout << "lui" << indent << "$" << addressReg << ",%hi(" << varName << ")" << std::endl;
            std::cout << "sw" << indent << "$" << immReg << ",%lo(" << varName << ")($" << addressReg << ")" << std::endl;
          }
          
        }
        else{
          int leftReg = frameContext->alloCalReg();
          int rightReg = frameContext->alloCalReg();
          frameContext->dealloReg(leftReg);
          frameContext->dealloReg(rightReg);

          branches[0]->generateMIPSArrayGlobalAssignment(indent,leftReg,frameContext);
          frameContext->alloReg(leftReg);
          branches[1]->generateMIPS(indent,rightReg,frameContext);
          frameContext->dealloReg(leftReg);
          std::cout << "sw" << indent << "$" << rightReg << ",0($" << leftReg << ")" << std::endl;
        }
      }
      else{
        if (isArrayAssignment==false){
          if(isPointerAssignment == true){
            int leftReg = frameContext->alloCalReg();
            int rightReg = frameContext->alloCalReg();
            frameContext->dealloReg(leftReg);
            frameContext->dealloReg(rightReg);
            branches[0] -> generateMIPSPointerAssignment(indent,leftReg,frameContext);
            frameContext->alloReg(leftReg);
            branches[1]->generateMIPS(indent,rightReg,frameContext);
            std::cout << "sw" << indent << "$" << rightReg << ",0($" << leftReg << ")" << std::endl;
          }
          else{
            int immReg = frameContext->alloCalReg();
            frameContext->dealloReg(immReg);
            branches[1]->generateMIPS(indent,immReg,frameContext);
            std::cout<< "sw" << indent << "$" << immReg << "," << offset<< "($fp)"<<std::endl;
          }
        }
        else{
          int leftReg = frameContext->alloCalReg();
          int rightReg = frameContext->alloCalReg();
          frameContext->dealloReg(leftReg);
          frameContext->dealloReg(rightReg);
          branches[0]->generateMIPSArrayAssignment(indent,leftReg,frameContext);
          frameContext->alloReg(leftReg);
          branches[1]->generateMIPS(indent,rightReg,frameContext);
          frameContext->dealloReg(leftReg);
          frameContext->clearReservedRegsForFunctionCall();
          std::cout << "sw" << indent << "$" << rightReg << "," << (offset-stackStartAddress) << "($" << leftReg << ")" << std::endl;
        }
      }
    }
  }
}

// Functions
void Assignment::countFunctionParameter(int* functionCallParameterOffset) const
{
  branches[1]->countFunctionParameter(functionCallParameterOffset);
}
