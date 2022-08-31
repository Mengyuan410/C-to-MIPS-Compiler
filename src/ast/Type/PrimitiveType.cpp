#include "ast/Type/PrimitiveType.hpp"

// Constructor
PrimitiveType::PrimitiveType(Specifier _type) : type(_type)
{}

// Destructor
PrimitiveType::~PrimitiveType()
{};

// Visualising
void PrimitiveType::generateParser(std::ostream &dst, std::string indent) const
{
  switch(type){
    case _int:
      dst << "int";
      break;
    case _void:
      dst << "void";
      break;
    case _unsigned:
      dst << "unsigned";
      break;
    case _char:
      dst << "char";
      break;
    case _float:
      dst << "float";
      break;
    case _double:
      dst << "double";
      break;
    default:
      dst << "unknown type ";
  }
  dst << std::endl;
}

enum Specifier PrimitiveType::DeclarationType()const
{
  return type;
}
