#ifndef Context_hpp
#define Context_hpp

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <utility>
#include <unordered_map>
#include <tuple>

// Enum Type
enum Specifier {
  _int,
  _void,
  _unsigned, // Not implemented yet
  _char, // Not implemented yet
  _float,
  _double,
  _ptr
};

//std::tuple <unsigned int, enum Specifier, bool>;

// makeName for labels
// static int makeNameUnq = 1;
std::string makeName(std::string base);

/////////////
// CONTEXT //
/////////////

class Context; // Context for Stack
typedef Context *ContextPtr;
class Context
{
public:

    //constructor
    Context();
    Context(ContextPtr _parentContext);

    virtual ~Context();

    //// Stack frame ////
    
    void changeVarBindings(std::string varName, unsigned int offset, enum Specifier type, bool isGlobal, bool isPointer,bool isParameterPointer);
    void changeHasFunction();
    void addCurrentStackSize(double addSize);

    void updateCurrentStackSize(unsigned int updateSize);
    void updateHasFunction(bool updateHasFunction);


    int getOffset(std::string varName);
    enum Specifier getType(std::string varName);
    bool getIsGlobal(std::string varName);
    bool getHasFunction();
    bool getIsPointer(std::string varName);
    bool getIsParameterPointer(std::string varName);
    std::unordered_map<std::string, std::tuple <unsigned int, enum Specifier, bool,bool,bool>> getVarBindings();
    double getCurrentStackSize();




    //// REGISTER ////

    int alloCalReg();
    int alloReservedReg();
    void alloReg(int i);
    void dealloReg(int i);
    
    void addReservedRegsForFunctionCall(int reg);
    void deleteReservedRegsForFunctionCall(int reg);
    std::vector<int> returnReservedRegsForFunctionCall();
    void clearReservedRegsForFunctionCall();

    // For float
    void alloFloatReg(int i);
    int alloFloatCalReg();
    void dealloFloatReg(int i);


    //// LABEL ////
    std::string getFunctionLabel();
    void updateFunctionLabel(std::string _functionLabel);
    std::string getBreakLabel();
    void updateBreakLabel(std::string _breakLabel);
    std::string getContinueLabel();
    void updateContinueLabel(std::string _continueLabel);

    //// ENUM ////
    void changeEnumVarBindings(std::string enumeratorVarName, int value);
    int getEnumValue(std::string enumeratorVarName);
    bool isEnum(std::string enumeratorVarName);
    std::unordered_map<std::string, int> getEnumVarBindings();

    //// STRUCT ////
    void changeStructMemberBindings(std::string structName, std::string memberName, enum Specifier memberType);
    int getStructMemberSize(std::string structName);
    std::vector<std::tuple<std::string, enum Specifier>> getStructMembers(std::string structName);
    std::unordered_map<std::string, std::vector<std::tuple<std::string, enum Specifier>>> getStructMemberBindings();

    //// FLOAT ////
    std::string getFloatDoubleLabel(double number, enum Specifier type);
    std::map<std::tuple<double,enum Specifier>,int> getFloatDoubleLabelBindings();
    void updateFloatDoubleLabelBindings(std::map<std::tuple<double,enum Specifier>,int> _floatdoubleLabelBindings);

    //// String ////
    std::string getStringLabel(std::string value);
    std::map<std::string,int> getStringLabelBindings();
    void updateStringLabelBindings(std::map<std::string,int> _stringLabelBindings);

    //// Function ////
    void updateFunctionType(enum Specifier _functionType);
    enum Specifier getFunctionType();

    void updateFunctionName(std::string _functionName);
    std::string getFunctionName();

    enum Specifier getFunctionCallType(std::string functionName);
    void updateFunctionAndParamCallType(std::string functionName, std::vector<enum Specifier> types, std::vector<bool> isPointer);
    std::vector<enum Specifier> getParamCallType(std::string functionName);
    std::vector<bool> getParamCallIsPointer(std::string functionName);
    std::map<std::string, std::vector<std::tuple<enum Specifier, bool>>> getFunctionInformation();

    //// Typedef ////
    enum Specifier getTypedefType(std::string typedefName);
    bool getTypedefIsPointer(std::string typedefName);
    void updateTypedef(std::string typedefName, enum Specifier type, bool isPointer);
    std::map<std::string, std::tuple <enum Specifier, bool> > getTypedefList();

protected:

    //// Stack frame ////

    double currentStackSize = 0; 
    bool hasFunction = false;
    std::unordered_map<std::string, std::tuple <unsigned int, enum Specifier, bool, bool,bool> > varBindings; //Variable binding: track the names + offset of variables & argument

    ContextPtr parentContext;

    //// REGISTER ////

    int Regs[32] =
    { 1,                          // $0 = 0 [0]
      0,                          // $at don't use [1]
      0, 0,                       // $v0-$v1, $2 for return value, $2 $3 used for calculation [2-3]
      0, 0, 0, 0,                 // $a0-$a3, function arguments, just use them for that [4-7]
      0, 0, 0, 0, 0, 0, 0, 0,     // $t0-$t7, temporary registers [8-15]
      0, 0, 0, 0, 0, 0, 0, 0,     // $s0-$s7, preserved registers for function calls [16-23]
      0, 0,                       // $t8-$t9, temporary registers [24-25]
      0, 0,                       // $k0-$k1, kernel registers, don't use [26-27]
      0,                          // $gp [28]
      0,                          // $sp [29]
      0,                          // $fp [30]
      0                           // $31 for function return [31]
    };

    int floatRegs[32]=
    {
      0, 1, // $f0-1 used for return of floats/doubles
      0, 1, // $f2-3 available
      0, 1, 0, 1, 0, 1, 0, 1, // $f4-$f11 temporaries, available
      1, 1, 1, 1, // $f12-$f15 used for arguments, stored in memory => available
      0, 1, 0, 1, // $f16-$f19 more temporaries, available
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 // $f20-$f30 saved registers, preserved across fn calls
    };
    //// LABEL ////
    std::string functionLabel;
    std::string breakLabel;
    std::string continueLabel;

    //// Reserved registers for $16 to $23 function call ///
    std::vector<int> reservedRegsForFunctionCall;

    //// ENUM ////
    std::unordered_map<std::string, int> enumVarBindings;
    
    //// STRUCT ////
    std::unordered_map<std::string, std::vector<std::tuple<std::string, enum Specifier>>> structMemberBindings; //struct name; member name; type

    //// FLOAT ////
    std::map<std::tuple<double,enum Specifier>,int> floatdoubleLabelBindings;

    //// String ////
    std::map<std::string,int> stringLabelBindings;
    
    //// Function ////
    enum Specifier functionType;
    std::string functionName;
    std::map<std::string, std::vector<std::tuple<enum Specifier, bool>>> functionInformation;

    //// Typedef ////
    std::map<std::string, std::tuple <enum Specifier, bool> > typedefList;
};

#endif