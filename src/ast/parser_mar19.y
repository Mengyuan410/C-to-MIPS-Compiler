%code requires{
  #include "ast.hpp"

  #include <cassert>

  extern Node *g_root; // A way of getting the AST out
  extern FILE *yyin;

  int yylex(void);
  void yyerror(const char *);
}

%union{
  NodePtr expr;
  ListPtr exprList;
  int number;
  double numberFloat;
  std::string *string;
}


%token IDENTIFIER INT_LITERAL FLOAT_LITERAL STRING_LITERAL SIZEOF
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token TYPEDEF EXTERN STATIC AUTO REGISTER
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM ELLIPSIS

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%type <expr> primary_expression postfix_expression unary_expression
%type <expr> multiplicative_expression additive_expression shift_expression cast_expression
%type <expr> relational_expression equality_expression and_expression
%type <expr> exclusive_or_expression inclusive_or_expression logical_and_expression
%type <expr> logical_or_expression conditional_expression assignment_expression
%type <expr> expression constant_expression

%type <expr> declaration init_declarator
%type <expr> declaration_specifiers type_specifier
%type <expr> struct_declaration struct_or_union_specifier struct_or_union
%type <expr> struct_declarator declarator
%type <expr> enum_specifier enumerator direct_declarator pointer

%type <expr> parameter_declaration type_name abstract_declarator direct_abstract_declarator
%type <expr> initializer statement labeled_statement compound_statement
%type <expr> expression_statement selection_statement iteration_statement
%type <expr> jump_statement external_declaration function_definition

%type <exprList> translation_unit struct_declaration_list argument_expression_list
%type <exprList> specifier_qualifier_list struct_declarator_list
%type <exprList> enumerator_list parameter_list parameter_type_list
%type <exprList> identifier_list initializer_list declaration_list statement_list

%type <number> INT_LITERAL STRING_LITERAL
%type <numberFloat> FLOAT_LITERAL
%type <string> IDENTIFIER


%start ROOT
%%

ROOT : translation_unit {g_root = new RootNode(new GlobalScope(*$1)); delete $1;}  //$1 is a pointer to a node list containing 1.全局变量 and 2. int f() 3. int main()
																					
translation_unit   
	: external_declaration { $$ = initialize_list($1); }
	| translation_unit external_declaration { $$ = append_list($1,$2); }  //returns a pointer to a list of node pointers (each points to a branch)
	;

external_declaration
	: function_definition { $$ = $1; } //int f() or int main()
	| declaration { $$ = $1;} //int x=0 or int z = x+y
	;

declaration
	: declaration_specifiers ';' { $$ = $1; }
	| declaration_specifiers init_declarator ';' {$$ = new Declaration($1,$2); }
	;

init_declarator
	: declarator { $$ = $1;}
	| declarator '=' initializer {$$ = new InitializeDeclarator($1,$3); }
	;

initializer
	: assignment_expression { $$ = $1; }
	| '{' initializer_list '}' { $$ = new ArrayInitializer(*$2); delete $2; }
	| '{' initializer_list ',' '}' { std::cerr << "Unsupported" << std::endl; }
	;

function_definition
	: declaration_specifiers declarator compound_statement { $$ = new FunctionDefinition( $$ = new FunctionDeclaration($1, $2), $3); }  //FunctionDefinition: *Declaration, *Scope //Declaration: *Declaration Specifier, *Declarator	
	| declaration_specifiers declarator declaration_list compound_statement { std::cerr << "Unsupported" << std::endl; }
	| declarator declaration_list compound_statement { std::cerr << "Unsupported" << std::endl; }
	| declarator compound_statement { std::cerr << "Unsupported" << std::endl; } 
	;

declaration_specifiers
	: type_specifier { $$ = $1; }
	| type_specifier declaration_specifiers { std::cerr << "Unsupported" << std::endl; }
	| type_qualifier { std::cerr << "Unsupported" << std::endl; }
	| type_qualifier declaration_specifiers { std::cerr << "Unsupported" << std::endl; }
	;

type_specifier
	: VOID { std::cerr << "Unsupported" << std::endl; }
	| CHAR { std::cerr << "Unsupported" << std::endl; }
	| SHORT { std::cerr << "Unsupported" << std::endl; }
	| INT { $$ = new PrimitiveType(Specifier::_int); }
	| LONG { std::cerr << "Unsupported" << std::endl; }
	| FLOAT { std::cerr << "Unsupported" << std::endl; }
	| DOUBLE { std::cerr << "Unsupported" << std::endl; }
	| SIGNED { std::cerr << "Unsupported" << std::endl; }
	| UNSIGNED { std::cerr << "Unsupported" << std::endl; }
	| struct_or_union_specifier { $$ = $1; }
	| enum_specifier { $$ = $1; }
	| TYPE_NAME { std::cerr << "Unsupported" << std::endl; }
	;

declarator
	: pointer direct_declarator { $$ = new PointerDeclarator($2); }
	| direct_declarator { $$ = $1; }
	;

direct_declarator
	: IDENTIFIER { $$ = new DirectDeclarator(*$1); delete $1;}
	| '(' declarator ')' { std::cerr << "Unsupported" << std::endl; }
	| direct_declarator '[' constant_expression ']' { $$ = new ArrayDeclarator($1,$3);}
	| direct_declarator '[' ']' { std::cerr << "Unsupported" << std::endl; }
	| direct_declarator '(' parameter_type_list ')' { $$ = new FunctionDeclarator($1,*$3); delete $3; } //FunctionDeclarator: *Identifier, List of parameter nodes pointers
	| direct_declarator '(' identifier_list ')' { std::cerr << "Unsupported" << std::endl; }
	| direct_declarator '(' ')' { $$ = new FunctionDeclarator($1); } //FunctionDeclarator: *Identifier
	;

parameter_type_list
	: parameter_list {$$ = $1;}
	| parameter_list ',' ELLIPSIS { std::cerr << "Unsupported" << std::endl; }
	;

parameter_list
	: parameter_declaration {$$ = initialize_list($1);}
	| parameter_list ',' parameter_declaration {$$ = append_list($1,$3); } //return a pointer to a list containing parameter declaration nodes
	;

parameter_declaration
	: declaration_specifiers declarator { $$ = new ParameterDeclaration($1,$2); }
	| declaration_specifiers abstract_declarator { std::cerr << "Unsupported" << std::endl; }
	| declaration_specifiers { std::cerr << "Unsupported" << std::endl; }
	;

compound_statement
	: '{' '}' { $$ = new Scope(); }
	| '{' statement_list '}' { $$ = new Scope(*$2); delete $2; }
	| '{' declaration_list '}' { $$ = new Scope(*$2); delete $2; }
	| '{' declaration_list statement_list '}' { $$ = new Scope(*$2,*$3); delete $2;delete $3; }
	;

declaration_list
	: declaration { $$ = initialize_list($1);}
	| declaration_list declaration {$$ = append_list($1,$2); }
	;

statement_list
	: statement { $$ = initialize_list($1); }
	| statement_list statement { $$ = append_list($1,$2); }
	;
	
statement
	: labeled_statement {$$ = $1;}
	| compound_statement { $$ = $1; }
	| expression_statement {$$ = $1; }
	| selection_statement { $$ = $1; }
	| iteration_statement { $$ = $1; }
	| jump_statement { $$ = $1;}
	;
expression_statement
	: ';' { std::cerr << "Unsupported" << std::endl; }
	| expression ';' { $$ = $1; }
	;

jump_statement
	: GOTO IDENTIFIER ';' { std::cerr << "Unsupported" << std::endl; }
	| CONTINUE ';' { $$ = new ContinueStatement(); }
	| BREAK ';' { $$ = new BreakStatement(); }
	| RETURN ';' {std::cerr << "Unsupported" << std::endl;}
	| RETURN expression ';' { $$ = new ReturnStatement($2);}
	;

expression
	: assignment_expression {$$ = $1; }
	| expression ',' assignment_expression { std::cerr << "Unsupported" << std::endl; }
	;
	
assignment_expression
	: conditional_expression { $$ = $1; }
	| unary_expression '=' assignment_expression {$$ = new Assignment($1,$3); }
	| unary_expression MUL_ASSIGN assignment_expression {$$ = new Assignment($1, new MulOperator($1, new AssignmentExp($3))); }
	| unary_expression DIV_ASSIGN assignment_expression { $$ = new Assignment($1, new DivOperator($1, new AssignmentExp($3))); }
	| unary_expression MOD_ASSIGN assignment_expression { $$ = new Assignment($1, new ModOperator($1, new AssignmentExp($3))); }
	| unary_expression ADD_ASSIGN assignment_expression { $$ = new Assignment($1, new AddOperator($1, new AssignmentExp($3))); }
	| unary_expression SUB_ASSIGN assignment_expression { $$ = new Assignment($1, new MinusOperator($1, new AssignmentExp($3))); }
	| unary_expression LEFT_ASSIGN assignment_expression { $$ = new Assignment($1, new LeftShiftOperator($1, new AssignmentExp($3))); }
	| unary_expression RIGHT_ASSIGN assignment_expression { $$ = new Assignment($1, new RightShiftOperator($1,new AssignmentExp($3))); }
	| unary_expression AND_ASSIGN assignment_expression { $$ = new Assignment($1, new BitwiseAndOperator($1,new AssignmentExp($3))); }
	| unary_expression XOR_ASSIGN assignment_expression { $$ = new Assignment($1, new XorOperator($1,new AssignmentExp($3))); }
	| unary_expression OR_ASSIGN assignment_expression { $$ = new Assignment($1, new BitwiseOrOperator($1, new AssignmentExp($3))); }
	;

unary_expression
	: postfix_expression { $$ = $1;}
	| INC_OP unary_expression { $$ = new PreIncrementOperator($2); }
	| DEC_OP unary_expression { $$ = new PreDecrementOperator($2); }
	| '&' cast_expression { $$ = new AddressOfOperator($2); }
	| '*' cast_expression { $$ = new PointerOperator($2); }
	| '+' cast_expression { std::cerr << "Unsupported" << std::endl; }
	| '-' cast_expression { $$ = new NegativeOperator($2); }
	| '~' cast_expression { $$ = new BitwiseNotOperator($2); }
	| '!' cast_expression { $$ = new LogicalNotOperator($2); }
	| SIZEOF unary_expression { std::cerr << "Unsupported" << std::endl; }
	| SIZEOF '(' type_name ')' { std::cerr << "Unsupported" << std::endl; }
	;

postfix_expression
	: primary_expression { $$ = $1; }
	| postfix_expression '[' expression ']' { $$ = new ArrayIndex($1,$3);}
	| postfix_expression '(' ')' { $$ = new FunctionCall($1); }
	| postfix_expression '(' argument_expression_list ')' {$$ = new FunctionCall($1, *$3); delete $3; }
	| postfix_expression '.' IDENTIFIER { $$ = new StructMember($1, *$3); delete $3; }
	| postfix_expression PTR_OP IDENTIFIER { std::cerr << "Unsupported" << std::endl; }
	| postfix_expression INC_OP { $$ = new PostIncrementOperator($1); }
	| postfix_expression DEC_OP { $$ = new PostDecrementOperator($1); }
	;


conditional_expression
	: logical_or_expression { $$ = $1; }
	| logical_or_expression '?' expression ':' conditional_expression { $$ = new TernaryConditionalOperator($1,$3,$5); }
	;

logical_or_expression
	: logical_and_expression { $$ = $1; }
	| logical_or_expression OR_OP logical_and_expression { $$ = new LogicalOrOperator($1,$3);  }
	;

logical_and_expression
	: inclusive_or_expression { $$ = $1; }
	| logical_and_expression AND_OP inclusive_or_expression { $$ = new LogicalAndOperator($1,$3); }
	;

inclusive_or_expression
	: exclusive_or_expression { $$ = $1; }
	| inclusive_or_expression '|' exclusive_or_expression { $$ = new BitwiseOrOperator($1,$3); }
	;

exclusive_or_expression
	: and_expression { $$ = $1; }
	| exclusive_or_expression '^' and_expression { $$ = new XorOperator($1,$3); }
	;

and_expression
	: equality_expression { $$ = $1; }
	| and_expression '&' equality_expression { $$ = new BitwiseAndOperator($1,$3); }
	;
	
equality_expression
	: relational_expression {$$ = $1;}
	| equality_expression EQ_OP relational_expression { $$ = new EqualToOperator($1,$3); }
	| equality_expression NE_OP relational_expression { $$ = new NotEqualToOperator($1,$3); }
	;

relational_expression
	: shift_expression {$$ = $1; }
	| relational_expression '<' shift_expression { $$ = new LessThanOperator($1,$3);}
	| relational_expression '>' shift_expression { $$ = new GreaterThanOperator($1,$3);}
	| relational_expression LE_OP shift_expression { $$ = new LessThanEqualToOperator($1,$3); }
	| relational_expression GE_OP shift_expression { $$ = new GreaterThanEqualToOperator($1,$3); }
	;
	
shift_expression
	: additive_expression { $$ = $1; }
	| shift_expression LEFT_OP additive_expression { $$ = new LeftShiftOperator($1,$3);}
	| shift_expression RIGHT_OP additive_expression { $$ = new RightShiftOperator($1,$3);}
	;

additive_expression
	: multiplicative_expression { $$ = $1; }
	| additive_expression '+' multiplicative_expression { $$ = new AddOperator($1,$3); }
	| additive_expression '-' multiplicative_expression { $$ = new MinusOperator($1,$3); }
	;

multiplicative_expression
	: cast_expression { $$ = $1; }
	| multiplicative_expression '*' cast_expression { $$ = new MulOperator($1,$3); }
	| multiplicative_expression '/' cast_expression { $$ = new DivOperator($1,$3); }
	| multiplicative_expression '%' cast_expression { $$ = new ModOperator($1,$3); }
	;

cast_expression
	: unary_expression { $$ = $1; }
	| '(' type_name ')' cast_expression { std::cerr << "Unsupported" << std::endl; }
	;


argument_expression_list
	: assignment_expression { $$ = initialize_list($1); }
	| argument_expression_list ',' assignment_expression {$$ = append_list($1, $3); }
	;
	
primary_expression
	: IDENTIFIER {$$ = new Identifier(*$1); delete $1; }
	| INT_LITERAL { $$ = new Integer($1); } 
	| FLOAT_LITERAL {std::cerr << "Unsupported" << std::endl; }
	| STRING_LITERAL { std::cerr << "Unsupported" << std::endl; }
	| '(' expression ')' { std::cerr << "Unsupported" << std::endl; }
	;

pogument_expression_list
	: assignment_expression { std::cerr << "Unsupported" << std::endl; }

constant_expression
	: conditional_expression { $$ = $1; }
	;

storage_class_specifier
	: TYPEDEF { std::cerr << "Unsupported" << std::endl; }
	| EXTERN { std::cerr << "Unsupported" << std::endl; }
	| STATIC { std::cerr << "Unsupported" << std::endl; }
	| AUTO { std::cerr << "Unsupported" << std::endl; }
	| REGISTER { std::cerr << "Unsupported" << std::endl; }
	;

struct_or_union_specifier
	: struct_or_union IDENTIFIER '{' struct_declaration_list '}' { $$ = new StructSpecifier(*$2,*$4); delete $2; delete$4; }
	| struct_or_union '{' struct_declaration_list '}' { std::cerr << "Unsupported" << std::endl; }
	| struct_or_union IDENTIFIER { $$ = new StructSpecifier(*$2); delete $2; }  // struct call!
	;

struct_or_union
	: STRUCT {  }
	| UNION { std::cerr << "Unsupported" << std::endl; }
	;

struct_declaration_list
	: struct_declaration { $$ = initialize_list($1); }
	| struct_declaration_list struct_declaration { $$ = append_list($1,$2); }
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';' { $$ = new StructDeclaration($1,$2);} //assume only int x;
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list { std::cerr << "Unsupported" << std::endl; }
	| type_specifier { $$ = $1; }
	| type_qualifier specifier_qualifier_list { std::cerr << "Unsupported" << std::endl; }
	| type_qualifier { std::cerr << "Unsupported" << std::endl; }
	;

struct_declarator_list
	: struct_declarator { $$ = $1; }
	| struct_declarator_list ',' struct_declarator { std::cerr << "Unsupported" << std::endl; }
	;

struct_declarator
	: declarator { $$ = $1; }
	| ':' constant_expression { std::cerr << "Unsupported" << std::endl; }
	| declarator ':' constant_expression { std::cerr << "Unsupported" << std::endl; }
	;

enum_specifier
	: ENUM '{' enumerator_list '}' { std::cerr << "Unsupported" << std::endl; }
	| ENUM IDENTIFIER '{' enumerator_list '}' { $$ = new EnumSpecifier(*$2,*$4); delete $2; delete $4; }
	| ENUM IDENTIFIER {$$ = new EnumSpecifier(*$2); delete $2; }
	;

enumerator_list
	: enumerator { $$ = initialize_list($1); }
	| enumerator_list ',' enumerator { $$ = append_list($1,$3); }
	;

enumerator
	: IDENTIFIER {$$ = new Enumerator(*$1); delete $1; }
	| IDENTIFIER '=' constant_expression { $$ = new Enumerator (*$1,$3); delete $1; }
	;

type_qualifier
	: CONST { std::cerr << "Unsupported" << std::endl; }
	| VOLATILE { std::cerr << "Unsupported" << std::endl; }
	;

pointer
	: '*' { }
	| '*' type_qualifier_list { std::cerr << "Unsupported" << std::endl; }
	| '*' pointer { std::cerr << "Unsupported" << std::endl; }
	| '*' type_qualifier_list pointer { std::cerr << "Unsupported" << std::endl; }
	;

type_qualifier_list
	: type_qualifier { std::cerr << "Unsupported" << std::endl; }
	| type_qualifier_list type_qualifier { std::cerr << "Unsupported" << std::endl; }
	;

identifier_list
	: IDENTIFIER { std::cerr << "Unsupported" << std::endl; }
	| identifier_list ',' IDENTIFIER { std::cerr << "Unsupported" << std::endl; }
	;

type_name
	: specifier_qualifier_list { std::cerr << "Unsupported" << std::endl; }
	| specifier_qualifier_list abstract_declarator { std::cerr << "Unsupported" << std::endl; }
	;

abstract_declarator
	: pointer { std::cerr << "Unsupported" << std::endl; }
	| direct_abstract_declarator { std::cerr << "Unsupported" << std::endl; }
	| pointer direct_abstract_declarator { std::cerr << "Unsupported" << std::endl; }
	;

direct_abstract_declarator
	: '(' abstract_declarator ')' { std::cerr << "Unsupported" << std::endl; }
	| '[' ']' { std::cerr << "Unsupported" << std::endl; }
	| '[' constant_expression ']' { std::cerr << "Unsupported" << std::endl; }
	| direct_abstract_declarator '[' ']' { std::cerr << "Unsupported" << std::endl; }
	| direct_abstract_declarator '[' constant_expression ']' { std::cerr << "Unsupported" << std::endl; }
	| '(' ')' { std::cerr << "Unsupported" << std::endl; }
	| '(' parameter_type_list ')' { std::cerr << "Unsupported" << std::endl; }
	| direct_abstract_declarator '(' ')' { std::cerr << "Unsupported" << std::endl; }
	| direct_abstract_declarator '(' parameter_type_list ')' { std::cerr << "Unsupported" << std::endl; }
	;

initializer_list
	: initializer { $$ = initialize_list($1);}
	| initializer_list ',' initializer { $$ = append_list($1,$3); }
	;

labeled_statement
	: IDENTIFIER ':' statement { std::cerr << "Unsupported" << std::endl; }
	| CASE constant_expression ':' statement { $$ = new Case($2,$4); }
	| DEFAULT ':' statement { $$ = new Default($3); }
	;

selection_statement
	: IF '(' expression ')' statement { $$ = new If($3,$5); }
	| IF '(' expression ')' statement ELSE statement { $$ = new IfElse($3,$5,$7);}
	| SWITCH '(' expression ')' statement { $$ = new Switch($3,$5); }
	;

iteration_statement
	: WHILE '(' expression ')' statement { $$ = new While($3,$5); }
	| DO statement WHILE '(' expression ')' ';' { $$ = new DoWhile($2,$5); }
	| FOR '(' expression_statement expression_statement ')' statement { $$ = new For($3,$4,$6);  }
	| FOR '(' expression_statement expression_statement expression ')' statement { $$ = new For($3,$4,$5,$7); }
	;

%%
#include <stdio.h>

Node *g_root;

Node *parseAST(std::string filename)
{
  yyin = fopen(filename.c_str(), "r");
  g_root = NULL;
  yyparse();
  return g_root;
}

extern char yytext[];
extern int column;

void yyerror (char const *s)
{
  fprintf(stderr, "Lexing error: %s\n", s);
  exit(1);
}
