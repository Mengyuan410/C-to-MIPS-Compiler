#ifndef ast_hpp
#define ast_hpp

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <math.h>

#include "ast/Identifier.hpp"
#include "ast/RootNode.hpp"
#include "ast/Node.hpp"
#include "ast/listmanipulation.hpp"
#include "ast/Context.hpp"


//Block
#include "ast/Block/Scope.hpp"
#include "ast/Block/GlobalScope.hpp"

//Call
#include "ast/Call/FunctionCall.hpp"
#include "ast/Call/StructMember.hpp"

//Definition
#include "ast/Definition/FunctionDefinition.hpp"

//Declaration
#include "ast/Declaration/Declaration.hpp"
#include "ast/Declaration/FunctionDeclaration.hpp"
#include "ast/Declaration/ParameterDeclaration.hpp"
#include "ast/Declaration/StructDeclaration.hpp"

//Declarator
#include "ast/Declarator/DirectDeclarator.hpp"
#include "ast/Declarator/FunctionDeclarator.hpp"
#include "ast/Declarator/InitializeDeclarator.hpp"
#include "ast/Declarator/ArrayDeclarator.hpp"
#include "ast/Declarator/PointerDeclarator.hpp"

// Array
#include "ast/Array/ArrayInitializer.hpp"
#include "ast/Array/ArrayIndex.hpp"

//Literal
#include "ast/Literal/Integer.hpp"
#include "ast/Literal/Float.hpp"
#include "ast/Literal/String.hpp"

//Statement
#include "ast/Statement/ReturnStatement.hpp"
#include "ast/Statement/If.hpp"
#include "ast/Statement/IfElse.hpp"
#include "ast/Statement/While.hpp"
#include "ast/Statement/DoWhile.hpp"
#include "ast/Statement/For.hpp"
#include "ast/Statement/Switch.hpp"
#include "ast/Statement/BreakStatement.hpp"
#include "ast/Statement/ContinueStatement.hpp"

// Label
#include "ast/Label/Case.hpp"
#include "ast/Label/Default.hpp"
//Type
#include "ast/Type/PrimitiveType.hpp"
#include "ast/Type/EnumSpecifier.hpp"
#include "ast/Type/Enumerator.hpp"
#include "ast/Type/StructSpecifier.hpp"
#include "ast/Type/TypeDef.hpp"
#include "ast/Type/TypeDefinedName.hpp"
//SingleSideOperator
#include "ast/SingleSideOperator/PreDecrementOperator.hpp"
#include "ast/SingleSideOperator/PreIncrementOperator.hpp"
#include "ast/SingleSideOperator/PostIncrementOperator.hpp"
#include "ast/SingleSideOperator/PostDecrementOperator.hpp"
#include "ast/SingleSideOperator/NegativeOperator.hpp"
#include "ast/SingleSideOperator/LogicalNotOperator.hpp"
#include "ast/SingleSideOperator/BitwiseNotOperator.hpp"
#include "ast/SingleSideOperator/PointerOperator.hpp"
#include "ast/SingleSideOperator/AddressOfOperator.hpp"
#include "ast/SingleSideOperator/PositiveOperator.hpp"
#include "ast/SingleSideOperator/SizeOfOperator.hpp"

//DoubleSideOperator
#include "ast/DoubleSideOperator/AddOperator.hpp"
#include "ast/DoubleSideOperator/MinusOperator.hpp"
#include "ast/DoubleSideOperator/MulOperator.hpp"
#include "ast/DoubleSideOperator/DivOperator.hpp"
#include "ast/DoubleSideOperator/ModOperator.hpp"
#include "ast/DoubleSideOperator/LeftShiftOperator.hpp"
#include "ast/DoubleSideOperator/RightShiftOperator.hpp"
#include "ast/DoubleSideOperator/LessThanOperator.hpp"
#include "ast/DoubleSideOperator/LessThanEqualToOperator.hpp"
#include "ast/DoubleSideOperator/GreaterThanOperator.hpp"
#include "ast/DoubleSideOperator/GreaterThanEqualToOperator.hpp"
#include "ast/DoubleSideOperator/EqualToOperator.hpp"
#include "ast/DoubleSideOperator/NotEqualToOperator.hpp"
#include "ast/DoubleSideOperator/BitwiseAndOperator.hpp"
#include "ast/DoubleSideOperator/XorOperator.hpp"
#include "ast/DoubleSideOperator/BitwiseOrOperator.hpp"
#include "ast/DoubleSideOperator/LogicalAndOperator.hpp"
#include "ast/DoubleSideOperator/LogicalOrOperator.hpp"

// Ternary Operator
#include "ast/TernaryOperator/TernaryConditionalOperator.hpp"

//Assignment
#include "ast/Assignment/Assignment.hpp"
#include "ast/Assignment/AssignmentExp.hpp"

extern Node *parseAST(std::string filename);
//extern std::vector <std::string> typedefNames;

#endif