#include "ast/Call/FunctionCall.hpp"

// Constructors

FunctionCall::FunctionCall(NodePtr id){
    branches.push_back(id);
}

FunctionCall::FunctionCall(NodePtr id, std::vector<NodePtr> _parameters){
    branches.push_back(id);  
    parameters = _parameters;
}

// Destructor
FunctionCall::~FunctionCall()
{
  delete branches[0];
}

// Visualising
void FunctionCall::generateParser(std::ostream &dst, std::string indent) const
{
  dst << indent << "Function Call [" << std::endl;
  dst << indent << "Identifier: " << std::endl;
  branches[0]->generateParser(dst, indent+"  ");
//   dst << indent << "Parameters: " << std::endl;
//   branches[1]->generateParser(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}


// Context
void FunctionCall::generateContext(ContextPtr frameContext, int parameterOffset, bool isGlobal  ){

  frameContext -> changeHasFunction();
  if (parameters.size() > 4){
    frameContext -> addCurrentStackSize(parameters.size()-4);  // Note: changed the input parameter of addCurrentStackSize to double, but haven't made any changes to this
  }
  branches[0] -> generateContext(frameContext, parameterOffset, isGlobal);
  for (unsigned i = 0; i<parameters.size();i++){
    parameters[i] -> generateContext(frameContext, parameterOffset, isGlobal);
  }
  blockContext = frameContext;
}

// MIPS
void FunctionCall::generateMIPS(std::string indent, int dstReg, ContextPtr frameContext) const {
    std::vector<int> reservedRegsForFunctionCall = frameContext->returnReservedRegsForFunctionCall();
    std::unordered_map<int, int> reservedRegsBinding; 
    
    for (unsigned i=0;i<reservedRegsForFunctionCall.size();i++){ // move all regs need to be reserved into $16, $17,...
      int reservedReg = frameContext->alloReservedReg();
      reservedRegsBinding[reservedRegsForFunctionCall[i]] = reservedReg;
      std::cout << "move" << indent << "$" << reservedReg << ",$" << reservedRegsForFunctionCall[i] << std::endl;
    }

    std::string functionName = branches[0]->returnIdentifier();
    if (parameters.size()>0){

      int f = 12;
      bool intCharAppearFlag = 0;
      // normal case

      if (functionName!=frameContext->getFunctionName()){  // not recursive
        std::vector<enum Specifier> parameterTypes = frameContext->getParamCallType(functionName);
        // std::vector<bool> parameterIsPointer = frameContext->getParamCallIsPointer(functionName);
        int num = 0;
        for (int i = 0; i<parameters.size();i++){
          switch(parameterTypes[i]){
            case _float:{
              int floatReg = frameContext->alloFloatCalReg();
              frameContext->dealloFloatReg(floatReg);
              if (num<4){
                if((f <= 14) & (intCharAppearFlag == 0)){
                  parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                  std::cout << "mov.s" << indent << "$f" << f << ",$f" << floatReg << std::endl; // floating point arguments stored in $f12 & $f14
                  f += 2;
                }
                else{
                  parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                  std::cout << "mfc1" << indent << "$" << num+4 << ",$f" << floatReg << std::endl; // stores 2 extra fp arguments in $6-$7
                }
              }
              else{
                parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                std::cout << "swc1" << indent << "$" << floatReg << "," << (num*4)  << "($sp)" << std::endl;
              }
              break;
            }
            case _double:{
              int floatReg = frameContext->alloFloatCalReg();
              frameContext->dealloFloatReg(floatReg);
              if (num<3){
                if((f <= 14) & (intCharAppearFlag == 0)){
                  parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                  std::cout << "mov.s" << indent << "$f" << f << ",$f" << floatReg << std::endl; // floating point arguments stored in $f12 & $f14
                  std::cout << "mov.s" << indent << "$f" << f+1 << ",$f" << floatReg+1 << std::endl;
                  f += 2;
                }
                else{
                  parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                  std::cout << "mfc1" << indent << "$" << num+4 << ",$f" << floatReg << std::endl; // stores 2 extra fp arguments in $6-$7
                  std::cout << "mfc1" << indent << "$" << num+4+1 << ",$f" << floatReg+1 << std::endl;
                }
              }
              else{
                parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                std::cout << "swc1" << indent << "$" << floatReg << "," << (num*4)  << "($sp)" << std::endl;
                std::cout << "swc1" << indent << "$" << floatReg+1 << "," << ((num+1)*4)  << "($sp)" << std::endl;
              }
              num++;
              break;
            }
            default:{ // include char and int
              intCharAppearFlag = 1;
              int immReg = frameContext->alloCalReg();
              frameContext->dealloReg(immReg);
              if (num>3){  // when more than 4 parameters, move the extra parameters onto stack instead of storing them in registers
                parameters[i] -> generateMIPS(indent, immReg, frameContext);
                std::cout << "sw" << indent << "$" << immReg << "," << (num*4)  << "($sp)" << std::endl;
              }
              else{  // first 4 parameters can be stored in registers
                parameters[i] -> generateMIPS(indent, immReg, frameContext);
                std::cout << "move" << indent << "$" << num+4 << ",$" << immReg << std::endl;
              }
            }
          }
          num++;
        }
      }
      else{
        int num = 0;
        for (int i = 0; i<parameters.size();i++){
          enum Specifier type = parameters[i]->returnType();
          switch(type){
            case _float:{
              int floatReg = frameContext->alloFloatCalReg();
              frameContext->dealloFloatReg(floatReg);
              if (num<4){
                if((f <= 14) & (intCharAppearFlag == 0)){
                  parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                  std::cout << "mov.s" << indent << "$f" << f << ",$f" << floatReg << std::endl; // floating point arguments stored in $f12 & $f14
                  f += 2;
                }
                else{
                  parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                  std::cout << "mfc1" << indent << "$" << num+4 << ",$f" << floatReg << std::endl; // stores 2 extra fp arguments in $6-$7
                }
              }
              else{
                parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                std::cout << "swc1" << indent << "$" << floatReg << "," << (num*4)  << "($sp)" << std::endl;
              }
              break;
            }
            case _double:{
              int floatReg = frameContext->alloFloatCalReg();
              frameContext->dealloFloatReg(floatReg);
              if (num<3){
                if((f <= 14) & (intCharAppearFlag == 0)){
                  parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                  std::cout << "mov.s" << indent << "$f" << f << ",$f" << floatReg << std::endl; // floating point arguments stored in $f12 & $f14
                  std::cout << "mov.s" << indent << "$f" << f+1 << ",$f" << floatReg+1 << std::endl;
                  f += 2;
                }
                else{
                  parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                  std::cout << "mfc1" << indent << "$" << num+4 << ",$f" << floatReg << std::endl; // stores 2 extra fp arguments in $6-$7
                  std::cout << "mfc1" << indent << "$" << num+4+1 << ",$f" << floatReg+1 << std::endl;
                }
              }
              else{
                parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                std::cout << "swc1" << indent << "$" << floatReg << "," << (num*4)  << "($sp)" << std::endl;
                std::cout << "swc1" << indent << "$" << floatReg+1 << "," << ((num+1)*4)  << "($sp)" << std::endl;
              }
              num++;
              break;
            }
            default:{ // include char and int
              intCharAppearFlag = 1;
              int immReg = frameContext->alloCalReg();
              frameContext->dealloReg(immReg);
              if (num>3){  // when more than 4 parameters, move the extra parameters onto stack instead of storing them in registers
                parameters[i] -> generateMIPS(indent, immReg, frameContext);
                std::cout << "sw" << indent << "$" << immReg << "," << (num*4)  << "($sp)" << std::endl;
              }
              else{  // first 4 parameters can be stored in registers
                parameters[i] -> generateMIPS(indent, immReg, frameContext);
                std::cout << "move" << indent << "$" << num+4 << ",$" << immReg << std::endl;
              }
            }
          }
          num++;
        }
      }
    }
    std::string label = branches[0]->returnIdentifier();
    std::cout << "jal " << label << std::endl;
    std::cout << "nop" << std::endl;

    enum Specifier functionType;
    if (functionName!=frameContext->getFunctionName()){
      functionType = frameContext->getFunctionCallType(functionName);
    }
    else{
      functionType = frameContext->getFunctionType();
    }
    if (functionType == _double || functionType == _float) {
      std::cout << "mov.s" << indent << "$f" << dstReg << ",$f0" << std::endl;
    }
    else{
      std::cout << "move" << indent << "$" << dstReg << ",$2" << std::endl;
    }

    for (unsigned i=0;i<reservedRegsForFunctionCall.size();i++){ //move back from $16,$17 when function ends
      int reservedReg = reservedRegsBinding[reservedRegsForFunctionCall[i]];
      std::cout << "move" << indent << "$" <<  reservedRegsForFunctionCall[i] << ",$" << reservedReg << std::endl;
      frameContext->dealloReg(reservedReg);
    }

    std::cout << std::endl;
}

// Really bad code
void FunctionCall::generateMIPSDouble(std::string indent, int dstReg, ContextPtr frameContext) const {
    std::vector<int> reservedRegsForFunctionCall = frameContext->returnReservedRegsForFunctionCall();
    std::unordered_map<int, int> reservedRegsBinding; 
    
    for (unsigned i=0;i<reservedRegsForFunctionCall.size();i++){ // move all regs need to be reserved into $16, $17,...
      int reservedReg = frameContext->alloReservedReg();
      reservedRegsBinding[reservedRegsForFunctionCall[i]] = reservedReg;
      std::cout << "move" << indent << "$" << reservedReg << ",$" << reservedRegsForFunctionCall[i] << std::endl;
    }

    std::string functionName = branches[0]->returnIdentifier();
    if (parameters.size()>0){

      int f = 12;
      bool intCharAppearFlag = 0;
      // normal case

      if (functionName!=frameContext->getFunctionName()){  // not recursive
        std::vector<enum Specifier> parameterTypes = frameContext->getParamCallType(functionName);
        // std::vector<bool> parameterIsPointer = frameContext->getParamCallIsPointer(functionName);
        int num = 0;
        for (int i = 0; i<parameters.size();i++){
          switch(parameterTypes[i]){
            case _float:{
              int floatReg = frameContext->alloFloatCalReg();
              frameContext->dealloFloatReg(floatReg);
              if (num<4){
                if((f <= 14) & (intCharAppearFlag == 0)){
                  parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                  std::cout << "mov.s" << indent << "$f" << f << ",$f" << floatReg << std::endl; // floating point arguments stored in $f12 & $f14
                  f += 2;
                }
                else{
                  parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                  std::cout << "mfc1" << indent << "$" << num+4 << ",$f" << floatReg << std::endl; // stores 2 extra fp arguments in $6-$7
                }
              }
              else{
                parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                std::cout << "swc1" << indent << "$" << floatReg << "," << (num*4)  << "($sp)" << std::endl;
              }
              break;
            }
            case _double:{
              int floatReg = frameContext->alloFloatCalReg();
              frameContext->dealloFloatReg(floatReg);
              if (num<3){
                if((f <= 14) & (intCharAppearFlag == 0)){
                  parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                  std::cout << "mov.s" << indent << "$f" << f << ",$f" << floatReg << std::endl; // floating point arguments stored in $f12 & $f14
                  std::cout << "mov.s" << indent << "$f" << f+1 << ",$f" << floatReg+1 << std::endl;
                  f += 2;
                }
                else{
                  parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                  std::cout << "mfc1" << indent << "$" << num+4 << ",$f" << floatReg << std::endl; // stores 2 extra fp arguments in $6-$7
                  std::cout << "mfc1" << indent << "$" << num+4+1 << ",$f" << floatReg+1 << std::endl;
                }
              }
              else{
                parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                std::cout << "swc1" << indent << "$" << floatReg << "," << (num*4)  << "($sp)" << std::endl;
                std::cout << "swc1" << indent << "$" << floatReg+1 << "," << ((num+1)*4)  << "($sp)" << std::endl;
              }
              num++;
              break;
            }
            default:{ // include char and int
              intCharAppearFlag = 1;
              int immReg = frameContext->alloCalReg();
              frameContext->dealloReg(immReg);
              if (num>3){  // when more than 4 parameters, move the extra parameters onto stack instead of storing them in registers
                parameters[i] -> generateMIPS(indent, immReg, frameContext);
                std::cout << "sw" << indent << "$" << immReg << "," << (num*4)  << "($sp)" << std::endl;
              }
              else{  // first 4 parameters can be stored in registers
                parameters[i] -> generateMIPS(indent, immReg, frameContext);
                std::cout << "move" << indent << "$" << num+4 << ",$" << immReg << std::endl;
              }
            }
          }
          num++;
        }
      }
      else{
        int num = 0;
        for (int i = 0; i<parameters.size();i++){
          enum Specifier type = parameters[i]->returnType();
          switch(type){
            case _float:{
              int floatReg = frameContext->alloFloatCalReg();
              frameContext->dealloFloatReg(floatReg);
              if (num<4){
                if((f <= 14) & (intCharAppearFlag == 0)){
                  parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                  std::cout << "mov.s" << indent << "$f" << f << ",$f" << floatReg << std::endl; // floating point arguments stored in $f12 & $f14
                  f += 2;
                }
                else{
                  parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                  std::cout << "mfc1" << indent << "$" << num+4 << ",$f" << floatReg << std::endl; // stores 2 extra fp arguments in $6-$7
                }
              }
              else{
                parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                std::cout << "swc1" << indent << "$" << floatReg << "," << (num*4)  << "($sp)" << std::endl;
              }
              break;
            }
            case _double:{
              int floatReg = frameContext->alloFloatCalReg();
              frameContext->dealloFloatReg(floatReg);
              if (num<3){
                if((f <= 14) & (intCharAppearFlag == 0)){
                  parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                  std::cout << "mov.s" << indent << "$f" << f << ",$f" << floatReg << std::endl; // floating point arguments stored in $f12 & $f14
                  std::cout << "mov.s" << indent << "$f" << f+1 << ",$f" << floatReg+1 << std::endl;
                  f += 2;
                }
                else{
                  parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                  std::cout << "mfc1" << indent << "$" << num+4 << ",$f" << floatReg << std::endl; // stores 2 extra fp arguments in $6-$7
                  std::cout << "mfc1" << indent << "$" << num+4+1 << ",$f" << floatReg+1 << std::endl;
                }
              }
              else{
                parameters[i] -> generateMIPS(indent, floatReg, frameContext);
                std::cout << "swc1" << indent << "$" << floatReg << "," << (num*4)  << "($sp)" << std::endl;
                std::cout << "swc1" << indent << "$" << floatReg+1 << "," << ((num+1)*4)  << "($sp)" << std::endl;
              }
              num++;
              break;
            }
            default:{ // include char and int
              intCharAppearFlag = 1;
              int immReg = frameContext->alloCalReg();
              frameContext->dealloReg(immReg);
              if (num>3){  // when more than 4 parameters, move the extra parameters onto stack instead of storing them in registers
                parameters[i] -> generateMIPS(indent, immReg, frameContext);
                std::cout << "sw" << indent << "$" << immReg << "," << (num*4)  << "($sp)" << std::endl;
              }
              else{  // first 4 parameters can be stored in registers
                parameters[i] -> generateMIPS(indent, immReg, frameContext);
                std::cout << "move" << indent << "$" << num+4 << ",$" << immReg << std::endl;
              }
            }
          }
          num++;
        }
      }
    }
    std::string label = branches[0]->returnIdentifier();
    std::cout << "jal " << label << std::endl;
    std::cout << "nop" << std::endl;

    enum Specifier functionType;
    if (functionName!=frameContext->getFunctionName()){
      functionType = frameContext->getFunctionCallType(functionName);
    }
    else{
      functionType = frameContext->getFunctionType();
    }
    if (functionType == _double || functionType == _float) {
      std::cout << "mov.s" << indent << "$f" << dstReg << ",$f0" << std::endl;
    }
    else{
      std::cout << "move" << indent << "$" << dstReg << ",$2" << std::endl;
    }

    for (unsigned i=0;i<reservedRegsForFunctionCall.size();i++){ //move back from $16,$17 when function ends
      int reservedReg = reservedRegsBinding[reservedRegsForFunctionCall[i]];
      std::cout << "move" << indent << "$" <<  reservedRegsForFunctionCall[i] << ",$" << reservedReg << std::endl;
      frameContext->dealloReg(reservedReg);
    }

    std::cout << std::endl;
}


// Functions
std::string FunctionCall::returnIdentifier() const{
  return branches[0] -> returnIdentifier();
}

void FunctionCall::countFunctionParameter(int* functionCallParameterOffset) const{
  if (*functionCallParameterOffset < parameters.size()){
    *functionCallParameterOffset = parameters.size();
  }
}