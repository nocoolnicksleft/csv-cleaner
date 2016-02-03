

%option noyywrap


%x incl
%x define
%x definition
%x title

%{

#include "csv.h"
 
 char buf[MAX_STRING_LENGTH]; 
 char *s; 

 int lineno = 1;

%}
 
%x STRSECT

%%
<<EOF>>     {
                          yyterminate();
            }
[\n]        {
                          lineno++;
 			  return TOKEN_NEWLINE;
            }
["]?[0-9]+["]?      {
                if (*yytext == '"') yylval.i = atoi(yytext+1);
                else yylval.i = atoi(yytext);
                return TOKEN_INT;
            }
["]?[0-9]*[0-9.][0-9]*([Ee][-+]?[0-9]+)?["]? {
                if (*yytext == '"') yylval.f = atof(yytext+1);
                else yylval.f = atof(yytext);
                return TOKEN_FLOAT;
            }
\"          { 
                BEGIN STRSECT; 
                s = buf; 
            }

<STRSECT>\" { 
                *s = 0; 
                yylval.s = buf; 
                BEGIN 0; 
                return TOKEN_STRING;
            }

<STRSECT>.   { 
                *s++ = *yytext; 
            }
<STRSECT>\n {
                printf("\nError: Unterminated string in line %d\n",lineno);
                exit(1);
            }
[ \r\l]     {  }
[,;\t]      { 
                return TOKEN_SEPARATOR; 
            }
[^\",;\t\r\n]*       {
                yylval.s = yytext;
                return TOKEN_UNQUOTED_STRING;
            }
%%





