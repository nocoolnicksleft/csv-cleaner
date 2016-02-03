/* 
 * File:   csv.h
 * Author: bseip
 *
 * Created on May 27, 2014, 10:07 AM
 */

#ifndef CSV_H
#define	CSV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 1024

#define TOKEN_STRING 256
#define TOKEN_SEPARATOR 257
#define TOKEN_NEWLINE 258
#define TOKEN_INT 259
#define TOKEN_UNQUOTED_STRING 260
#define TOKEN_FLOAT 261

#define TYPE_EMPTY 0
#define TYPE_INT 1
#define TYPE_FLOAT 2
#define TYPE_ALPHA 3



typedef union {
    char * s;
    float f;
    int i;
} yylval_t;


#ifdef	__cplusplus
extern "C" {
#endif
    
extern int yylex (void);
extern FILE* yyin;
extern yylval_t yylval;

#ifdef	__cplusplus
}
#endif


#endif	/* CSV_H */

