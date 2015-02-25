%option yylineno
%pointer

%{
    #include <cstdlib>
    #include <cerrno>
    #include <climits>
    #include <limits>
    #include "ast.hpp"
    #include "parser.hpp"
    
	void yyerror(const char *);
%}

/* WRITEME: Copy any definitions and start conditions from Project 4 here. */
%x comment


%%

<INITIAL>{
 "--!"              BEGIN(comment);
}
<comment>{
 "!--"      BEGIN(INITIAL);
 "!"        
 [^!]+      // eat comment in chunks
 <<EOF>>  { yyerror("dangling comment"); }
}

  /* WRITEME: Copy your Flex rules and actions from Project 4 here. */
[ \t\n]
"("                     { return T_OPENPAREN; }
")"                     { return T_CLOSEPAREN; }
"{"                     { return T_OPENCURLY; }
"}"                     { return T_CLOSECURLY; }
"extends"               { return T_EXTENDS; }
"true"                  { yylval.base_int = 1; return T_TRUE; }
"false"                 { yylval.base_int = 0; return T_FALSE; }
"return"                { return T_RETURN; }
"if"                    { return T_IF; }
"else"                  { return T_ELSE; }
"while"                 { return T_WHILE; }
"print"                 { return T_PRINT; }
"integer"               { return T_INT; }
"."                     { return T_DOT; }
","                     { return T_COMMA; }
":"                     { return T_COLON; }
"or"                    { return T_OR; }
"and"                   { return T_AND; }
"<="                    { return T_LESSEQUAL; }
"="                     { return T_E; }
"<"                     { return T_LESS; }
"+"                     { return T_PLUS; }
"-"                     { return T_MINUS; }
"*"                     { return T_TIMES; }
"/"                     { return T_DIV; }
"not"                   { return T_NOT; }
"->"                    { return T_ARROW; }
"equals"                { return T_EQUAL; } 
"none"                  { return T_NONE; }     
"new"                   { return T_NEW; }
"boolean"               { return T_BOOL; }


[0-9][0-9]*             { yylval.base_int = atoi(yytext); return T_NUM; }
[a-zA-Z][a-zA-Z0-9]*    { yylval.base_char_ptr = strdup(yytext); return T_ID; }

.                 { yyerror("invalid character"); }

%%

int yywrap(void) {
  return 1;
}
