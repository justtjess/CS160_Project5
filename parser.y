%{
    #include <cstdlib>
    #include <cstdio>
    #include <iostream>
    #include "ast.hpp"
    
    #define YYDEBUG 1
    #define YYINITDEPTH 10000
    
    int yylex(void);
    void yyerror(const char *);
    
    extern ASTNode* astRoot;
%}

%error-verbose
// %glr-parser
/* NOTE: You may use the %glr-parser directive, which may allow your parser to
         work even with some shift/reduce conflicts remianing. */

/* WRITEME: Copy your token and precedence specifiers from Project 4 here. */

%token T_NUM
%token T_INT
%token T_EXTENDS
%token T_OPENCURLY
%token T_CLOSECURLY
%token T_OPENPAREN
%token T_CLOSEPAREN
%token T_RETURN
%token T_ID
%token T_BOOL
%token T_IF
%token T_ELSE
%token T_WHILE
%token T_PRINT
%token T_DOT
%token T_OR
%token T_AND
%token T_LESSEQUAL
%token T_LESS
%token T_PLUS
%token T_MINUS
%token T_TIMES
%token T_DIV
%token T_NOT
%token T_ARROW
%token T_EQUAL
%token T_NONE
%token T_NEW
%token T_COMMA
%token T_TRUE
%token T_FALSE
%token T_COLON
%token T_E

/* Precedence */
%left T_OR
%left T_AND
%left T_LESSEQUAL T_LESS T_EQUAL
%left T_PLUS T_MINUS
%left T_TIMES T_DIV
%right T_NOT

/* WRITEME: Copy your type specifiers from Project 4 here. */
%type <program_ptr> Start
%type <class_list_ptr> Classes
%type <class_ptr> Class
%type <declaration_list_ptr> Members Declarations
%type <declaration_ptr> Member
%type <identifier_list_ptr> Declaration
%type <method_list_ptr> Methods
%type <method_ptr> Method
%type <expression_ptr> Expressions
%type <parameter_list_ptr> Parameters Parameter
%type <returnstatement_ptr> Return
%type <type_ptr> Type ReturnType
%type <methodbody_ptr> Body
%type <statement_list_ptr> Statements Block
%type <parameter_ptr> Parameter_p
%type <assignment_ptr> Assignment
%type <methodcall_ptr> Method_Call MethodCall
%type <ifelse_ptr> ifelse
%type <while_ptr> While
%type <print_ptr> print
%type <integertype_ptr> T_INT
%type <booleantype_ptr> T_BOOL
%type <statement_ptr> Statement
%type <expression_list_ptr> Arguments Arguments_p


%type <base_char_ptr> T_ID
%type <base_int> T_NUM T_FALSE T_TRUE

%%

/* WRITEME: This rule is a placeholder. Replace it with your grammar
            rules and actions from Project 4. */

Start : Classes { $$ = new ProgramNode($1); astRoot = $$; }

Classes : Class Classes { $$ = $2; $$->push_front($1); }
      | Class { $$ = new std::list<ClassNode*>(); $$->push_back($1); }
      ;

/* WRITME: Write your Bison grammar specification here */

Class : T_ID T_EXTENDS T_ID T_OPENCURLY Members Methods T_CLOSECURLY { $$ = new ClassNode(new IdentifierNode($1), new IdentifierNode($3), $5, $6); }
      | T_ID T_OPENCURLY Members Methods T_CLOSECURLY { $$ = new ClassNode(new IdentifierNode($1), NULL, $3, $4); }
      ;

Members : Members Member { $$ = $1; $$->push_back($2); }
      | { $$ = new std::list<DeclarationNode*>(); }
      ;

Member : Type T_ID { $$ = new DeclarationNode($1,new std::list<IdentifierNode*>(1, new IdentifierNode($2))); }

Type : T_INT { $$ = new IntegerTypeNode(); }
      | T_BOOL { $$ = new BooleanTypeNode(); }
      | T_ID { $$ = new ObjectTypeNode(new IdentifierNode($1)); }
      ;

Methods : Method Methods { $$ = $2; $$->push_front($1); }
      | { $$ = new std::list<MethodNode*>(); }
      ;

Method : T_ID T_OPENPAREN Parameters T_CLOSEPAREN T_ARROW ReturnType T_OPENCURLY Body T_CLOSECURLY { $$ = new MethodNode(new IdentifierNode($1), $3, $6, $8); }
      ;

Body :  Declarations Statements Return { $$ = new MethodBodyNode( $1, $2, $3); }
      ;

Parameters : Parameter  { $$ = $1; }
      | { $$ = NULL; }
      ;

Parameter : Parameter_p {$$ = new std::list<ParameterNode*>(); $$->push_back($1); }
      | Parameter_p T_COMMA Parameter { $$ = $3; $$->push_front($1); }
      ;

Parameter_p : T_ID T_COLON Type {$$ = new ParameterNode($3,new IdentifierNode($1)); }

Declarations : Declarations Type Declaration { $$ = $1; $$->push_back(new DeclarationNode($2,$3)); }
      | { $$ = new std::list<DeclarationNode*>(); }
      ;

Declaration : Declaration T_COMMA T_ID { $$ = $1; $$->push_back(new IdentifierNode($3)); }
      | T_ID { $$ = new std::list<IdentifierNode*>(); $$->push_back(new IdentifierNode($1));}
      ;

Statements : Statement Statements { $$ = $2; $$->push_front($1); }
      | { $$ = new std::list<StatementNode*>(); }
      ;

Statement : Assignment { $$ = $1; }
      | Method_Call { $$ = new CallNode($1); }
      | ifelse { $$ = $1; }
      | While { $$ = $1; }
      | print { $$ = $1; }
      ;

Return : T_RETURN Expressions { $$ = new ReturnStatementNode($2); }
      | { $$ = NULL; }
      ;

Assignment : T_ID T_DOT T_ID T_E Expressions { $$ = new AssignmentNode(new IdentifierNode($1), new IdentifierNode($3), $5); }
      | T_ID T_E Expressions { $$ = new AssignmentNode(new IdentifierNode($1), NULL, $3); }
      ;

Method_Call : T_ID T_DOT T_ID T_OPENPAREN Arguments T_CLOSEPAREN { $$ = new MethodCallNode(new IdentifierNode($1), new IdentifierNode($3), $5); }
      | T_ID T_OPENPAREN Arguments T_CLOSEPAREN { $$ = new MethodCallNode(new IdentifierNode($1), NULL, $3); }
      ;

ifelse : T_IF Expressions T_OPENCURLY Block T_CLOSECURLY { $$ = new IfElseNode ($2,$4, NULL); }
      | T_IF Expressions T_OPENCURLY Block T_CLOSECURLY T_ELSE T_OPENCURLY Block T_CLOSECURLY { $$ = new IfElseNode($2, $4, $8); }
      ;

While : T_WHILE Expressions T_OPENCURLY Block T_CLOSECURLY {$$ = new WhileNode($2,$4); }
      ;

print : T_PRINT Expressions { $$ = new PrintNode($2); }
      ;

Block : Statement Block { $$ = $2; $$->push_front($1); }
      | Statement { $$ = new std::list<StatementNode*>(); $$->push_back($1); }
      ;

Expressions : Expressions T_PLUS Expressions { $$ = new PlusNode($1, $3); }
      | Expressions T_MINUS Expressions { $$ = new MinusNode($1, $3); }
      | Expressions T_TIMES Expressions { $$ = new TimesNode($1, $3); }
      | Expressions T_DIV Expressions { $$ = new DivideNode($1, $3); } 
      | Expressions T_LESS Expressions { $$ = new LessNode($1, $3); } 
      | Expressions T_LESSEQUAL Expressions { $$ = new LessEqualNode($1, $3); } 
      | Expressions T_EQUAL Expressions { $$ = new EqualNode($1, $3); } 
      | Expressions T_AND Expressions { $$ = new AndNode($1, $3); } 
      | Expressions T_OR Expressions { $$ = new OrNode($1, $3); } 
      | T_NOT Expressions { $$ = new NotNode($2); } 
      | T_MINUS Expressions %prec T_NOT { $$ = new NegationNode($2); } 
      | T_ID  { $$ = new VariableNode(new IdentifierNode($1)); }
      | T_ID T_DOT T_ID { $$ = new MemberAccessNode(new IdentifierNode($1), new IdentifierNode($3)); }
      | MethodCall { $$ = $1; }
      | T_OPENPAREN Expressions T_CLOSEPAREN  { $$ = $2; }
      | T_NUM {  $$ = new IntegerLiteralNode(new IntegerNode($1)); $$->basetype = bt_integer;}
      | T_TRUE {  $$ = new BooleanLiteralNode(new IntegerNode($1)); $$->basetype = bt_boolean;}
      | T_FALSE {  $$ = new BooleanLiteralNode(new IntegerNode($1)); $$->basetype = bt_boolean;}
      | T_NEW T_ID { $$ = new NewNode(new IdentifierNode($2), NULL); }
      | T_NEW T_ID T_OPENPAREN Arguments T_CLOSEPAREN { $$ = new NewNode(new IdentifierNode($2), $4); }
      ;

ReturnType : T_NONE { $$ = new NoneNode; }
      | Type { $$ = $1; astRoot = $$; }
      ;


MethodCall : T_ID T_OPENPAREN Arguments T_CLOSEPAREN { $$ = new MethodCallNode(new IdentifierNode($1), NULL, $3); }
      | T_ID T_DOT T_ID T_OPENPAREN Arguments T_CLOSEPAREN { $$ = new MethodCallNode(new IdentifierNode($1), new IdentifierNode($3), $5); }
      ;

Arguments : Arguments_p { $$ = $1; }
      | { $$ = new std::list<ExpressionNode*>(); }
      ;

Arguments_p : Arguments_p T_COMMA Expressions { $$ = $1; $$->push_back($3); }
      | Expressions {$$ = new std::list<ExpressionNode*>(); $$->push_back($1); }
      ;

%%

extern int yylineno;

void yyerror(const char *s) {
  fprintf(stderr, "%s at line %d\n", s, yylineno);
  exit(1);
}