#include "ast/Context.hpp"

// Constructor
Context::Context(){
    currentStackSize = 0;
    varBindings = {};
}


Context::Context(ContextPtr _parentContext) : parentContext(_parentContext){
    currentStackSize = parentContext->getCurrentStackSize();
    varBindings = parentContext->getVarBindings();
    functionLabel = parentContext->getFunctionLabel();
    continueLabel = parentContext->getContinueLabel();
    breakLabel = parentContext->getBreakLabel();
    enumVarBindings = parentContext->getEnumVarBindings();
    hasFunction = parentContext->getHasFunction();
    structMemberBindings = parentContext->getStructMemberBindings();
    floatdoubleLabelBindings = parentContext->getFloatDoubleLabelBindings();
    stringLabelBindings = parentContext->getStringLabelBindings();
    functionType = parentContext->getFunctionType();
    functionName = parentContext->getFunctionName();
    functionInformation = parentContext->getFunctionInformation();
    typedefList = parentContext->getTypedefList();

}

// Copy Constructor //

// Context::Context(ContextPtr source){
//     currentStackSize = source -> getCurrentStackSize();
//     varBindings = source -> getVarBindings();
//     hasFunction = source -> getHasFunction();

// }

Context::~Context(){}


/////////////////
// Stack Frame //
/////////////////

// Change Current Context Information
void Context::changeVarBindings(std::string varName, unsigned int offset, enum Specifier type, bool isGlobal,bool isPointer,bool isParameterPointer)
{
    // std::cout << "Create var binding" << varName << " " << offset << " " << type << " "<< isPointer << std::endl;
    varBindings[varName] = std::make_tuple(offset, type, isGlobal,isPointer,isParameterPointer);   
}

void Context::addCurrentStackSize(double addSize)
{   
    currentStackSize += addSize;
    // std::cout << "Stack size " << currentStackSize << std::endl;
}

void Context::updateCurrentStackSize(unsigned int updateSize)
{
    currentStackSize = updateSize;
}

void Context::changeHasFunction()
{
    hasFunction = true;
}

void Context::updateHasFunction(bool updateHasFunction)
{
    hasFunction = updateHasFunction;
}


// Get Stack Information

// used to determine the position in stack

int Context::getOffset(std::string varName)
{

    return std::get<0>(varBindings[varName]);
}

enum Specifier Context:: getType(std::string varName)
{
    return std::get<1>(varBindings[varName]);
}

bool Context:: getIsGlobal(std::string varName)
{
    return std::get<2>(varBindings[varName]);
}

bool Context:: getIsPointer(std::string varName)
{
    return std::get<3>(varBindings[varName]);
}

bool Context::getIsParameterPointer(std::string varName)
{
    return std::get<4>(varBindings[varName]);
}



// Check if there is any function call - if there is, we would need to store $sp into $31
bool Context::getHasFunction(){
    return hasFunction;
}

std::unordered_map<std::string, std::tuple <unsigned int, enum Specifier, bool, bool,bool>> Context::getVarBindings()
{
    return varBindings;
}

// to find the current size of the stack (this function doesn't have +8: fp, $31, 2 empty, 4 arguments)
// this function is used for: 
//   1. finding the frame size (will do a +8 outside this function)  
//   2. finding the current size so that the new declared variable can be added to the correct position on stack

double Context::getCurrentStackSize(){
    return currentStackSize;
}

// ContextPtr Context::getParentContext(){
//     return parentContext;
// }

//////////////
// REGISTER //
//////////////

void Context::alloReg(int i){
    Regs[i] = 1;
}

void Context::dealloReg(int i){
    Regs[i] = 0;
}

int Context::alloCalReg(){

    int calRegList[16] = {2,3,8,9,10,11,12,13,14,15,24,25};
    for (int i=0;i<16;i++){
        if (Regs[calRegList[i]]==0){
            Regs[calRegList[i]] = 1;
            return calRegList[i];
        }
    }
}

int Context::alloReservedReg(){
    int reservedRegList[8] = {16,17,18,19,20,21,22,23};
    for (int i=0;i<16;i++){
        if (Regs[reservedRegList[i]]==0){
            Regs[reservedRegList[i]] = 1;
            return reservedRegList[i];
        }
    }
}

// float
int Context::alloFloatCalReg(){
    for (int i=0;i<32;i++){
        if (floatRegs[i]==0){
            floatRegs[i]=1;
            return i;
        }
    }
}

void Context::alloFloatReg(int i){
    floatRegs[i] = 1;
}

void Context::dealloFloatReg(int i){
    floatRegs[i] = 0;
}

///////////
// LABEL //
///////////

void Context::updateFunctionLabel(std::string _functionLabel){
    functionLabel = _functionLabel;
}
std::string Context::getFunctionLabel(){
    return functionLabel;
}
void Context::updateBreakLabel(std::string _breakLabel){
    breakLabel = _breakLabel;
}
std::string Context::getBreakLabel(){
    return breakLabel;
}

void Context::updateContinueLabel(std::string _continueLabel){
    continueLabel = _continueLabel;
}
std::string Context::getContinueLabel(){
    return continueLabel;
}

//////////////
// MAKENAME //
//////////////
int makeNameUnq = 1;
std::string makeName(std::string base)
{
    return base+std::to_string(makeNameUnq++);
}

///////////////////
// FUNCTION CALL //
///////////////////

void Context::addReservedRegsForFunctionCall(int reg){
    reservedRegsForFunctionCall.push_back(reg);
}

void Context::deleteReservedRegsForFunctionCall(int reg){
    for (unsigned i=0; i< reservedRegsForFunctionCall.size();i++){
        if (reservedRegsForFunctionCall[i]==reg){
            reservedRegsForFunctionCall.erase(reservedRegsForFunctionCall.begin()+i);
        }
    }
}

std::vector<int> Context::returnReservedRegsForFunctionCall(){
    return reservedRegsForFunctionCall;
}

void Context::clearReservedRegsForFunctionCall(){
    reservedRegsForFunctionCall.clear();
}


//////////
// ENUM //
//////////

void Context::changeEnumVarBindings(std::string enumeratorVarName, int value)
{
    enumVarBindings[enumeratorVarName] = value;
}

int Context::getEnumValue(std::string enumeratorVarName)
{
    int value = enumVarBindings[enumeratorVarName];
    return value;
}

bool Context::isEnum(std::string enumeratorVarName)
{
    if (enumVarBindings.count( enumeratorVarName )){ // check if this key exists in enumVarBindings
        return true;
    }
    else{
        return false;
    }
}

std::unordered_map<std::string, int> Context::getEnumVarBindings(){
    return enumVarBindings;
}


////////////
// Struct //
////////////


void Context::changeStructMemberBindings(std::string structName, std::string memberName, enum Specifier memberType)
{
    structMemberBindings[structName].push_back(std::make_tuple(memberName, memberType));   
}

int Context::getStructMemberSize(std::string structName)
{
    int size=0;
    std::vector<std::tuple<std::string, enum Specifier>> memberVector = structMemberBindings[structName];
    double count = 0;

    for (int i=0; i < memberVector.size(); i++){
        enum Specifier type = std::get<1>(memberVector[i]);
        switch(type)
        {
        case _double:
            count += 2;
            break;
        case _char:
            count += 0.25;
            break;
        case _int:
            count += 1;
            break;
        case _float:
            count += 1;
            break;
        case _unsigned:
            count += 1;
            break;
        default:
            count += 0;
        }
    }
    return count;
}


std::vector<std::tuple<std::string, enum Specifier>> Context::getStructMembers(std::string structName)
{
    return structMemberBindings[structName];
}

std::unordered_map<std::string, std::vector<std::tuple<std::string, enum Specifier>>> Context::getStructMemberBindings()
{
    return structMemberBindings;
}

//////////////////////////////
// Binding for float number //
//////////////////////////////
std::string Context::getFloatDoubleLabel(double number, enum Specifier type){

    std::tuple <double,enum Specifier> floatdoubleNameType = std::make_tuple(number, type) ;
    if (floatdoubleLabelBindings.count(floatdoubleNameType)){
        return "LC"+std::to_string(floatdoubleLabelBindings[floatdoubleNameType]);
    }
    else{
        floatdoubleLabelBindings[floatdoubleNameType] = floatdoubleLabelBindings.size();
        return "LC"+std::to_string(floatdoubleLabelBindings.size()-1);
    }
}

std::map<std::tuple<double,enum Specifier>,int> Context::getFloatDoubleLabelBindings()
{
    return floatdoubleLabelBindings;
}

void Context::updateFloatDoubleLabelBindings(std::map<std::tuple<double,enum Specifier>,int> _floatdoubleLabelBindings){
    floatdoubleLabelBindings = _floatdoubleLabelBindings;
}

///////////////////////////////
// Binding for string number //
///////////////////////////////

std::string Context::getStringLabel(std::string value){
    
    if (stringLabelBindings.count(value)){
        return "LS"+std::to_string(stringLabelBindings[value]);
    }
    else{
        stringLabelBindings[value] = stringLabelBindings.size();
        return "LS"+std::to_string(stringLabelBindings.size()-1);
    }
}

std::map<std::string,int> Context::getStringLabelBindings()
{
    return stringLabelBindings;
}

void Context::updateStringLabelBindings(std::map<std::string,int> _stringLabelBindings){
    stringLabelBindings = _stringLabelBindings;
}


//////////////
// Function //
//////////////

void Context::updateFunctionType(enum Specifier _functionType){
    functionType = _functionType;
}

enum Specifier Context::getFunctionType(){
    return functionType;
}

void Context::updateFunctionName(std::string _functionName){
    functionName = _functionName;
}

std::string Context::getFunctionName(){
    return functionName;
}

enum Specifier Context::getFunctionCallType(std::string functionName){
    return std::get<0>( functionInformation[functionName][functionInformation[functionName].size()-1] );
}

void Context::updateFunctionAndParamCallType(std::string functionName, std::vector<enum Specifier> types, std::vector<bool> isPointer)
{
    std::vector<std::tuple<enum Specifier, bool>> type_and_isPointer; 
    for (int i=0; i<types.size(); i++){
        type_and_isPointer.push_back(std::make_tuple(types[i], isPointer[i]));
    }
    functionInformation[functionName] = type_and_isPointer;
}

std::vector<enum Specifier> Context::getParamCallType(std::string functionName){
    std::vector<enum Specifier> paramVector;
    for (int i=0; i<functionInformation[functionName].size(); i++){
        paramVector.push_back(std::get<0>(functionInformation[functionName][i]));
    }
    return paramVector;
}

std::vector<bool> Context::getParamCallIsPointer(std::string functionName){
    std::vector<bool> paramVector;
    for (int i=0; i<functionInformation[functionName].size(); i++){
        paramVector.push_back(std::get<1>(functionInformation[functionName][i]));
    }
    return paramVector;
}

std::map<std::string, std::vector<std::tuple<enum Specifier, bool>>> Context::getFunctionInformation(){
    return functionInformation;
}



/////////////
// Typedef //
/////////////

enum Specifier Context::getTypedefType(std::string typedefName)
{
   
    return std::get<0>(typedefList[typedefName]);
}

bool Context::getTypedefIsPointer(std::string typedefName)
{
    return std::get<1>(typedefList[typedefName]);
}

void Context::updateTypedef(std::string typedefName, enum Specifier type, bool isPointer)
{
    typedefList[typedefName] = std::make_tuple(type, isPointer);  
}

std::map<std::string, std::tuple <enum Specifier, bool>> Context::getTypedefList()
{
    return typedefList;
}