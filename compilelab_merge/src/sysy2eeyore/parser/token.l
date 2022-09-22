%option noyywrap
%option prefix = "sysy2eeyore_"
%{
#include "../../sysy2eeyore/parser/token.h"
#include "../../sysy2eeyore/parser/parser.tab.hpp"
#include <string.h>


#define Debug_Token 0
#define Reserved_Word_Handler(t) do{\
    if (Debug_Token)\
        printf("Find_Reserved_Word (%d %d): %d\n",yylineno,yycolumn,t);\
    return (sysy2eeyore_lval.token = t);\
} while(0);
#define Operation_Handler(t) do{\
    if (Debug_Token)\
        printf("Find_Operation (%d %d): %d\n",yylineno,yycolumn,t);\
    return (sysy2eeyore_lval.token = t);\
} while(0);
#define Symbol_Handler(t) do{\
    if (Debug_Token)\
        printf("Find_Symbol (%d %d): %d\n",yylineno,yycolumn,t);\
    return (sysy2eeyore_lval.token = t);\
} while(0);
#define STARTTIME_Handler do{\
    sysy2eeyore_lval.string = "_sysy_starttime";\
    *yy_cp = yy_hold_char;\
    yy_hold_char='(';\
    yy_cp--;\
    yyleng--;\
    yy_c_buf_p--;\
    *yy_c_buf_p='_';yy_c_buf_p--;\
    *yy_c_buf_p='_';\
    yy_c_buf_p--;\
    *yy_c_buf_p='E';\
    yy_c_buf_p--;\
    *yy_c_buf_p='N';\
    yy_c_buf_p--;\
    *yy_c_buf_p='I';\
    yy_c_buf_p--;\
    *yy_c_buf_p='L';\
    yy_c_buf_p--;\
    *yy_c_buf_p='_';\
    yy_c_buf_p--;\
    *yy_c_buf_p='_';\
    yy_c_buf_p--;\
    return IDENTIFIER;\
}while (0);
#define STOPTIME_Handler do{\
    sysy2eeyore_lval.string = "_sysy_stoptime";\
    *yy_cp = yy_hold_char;\
    yy_hold_char='(';\
    yy_cp--;\
    yyleng--;\
    yy_c_buf_p--;\
    *yy_c_buf_p='_';\
    yy_c_buf_p--;\
    *yy_c_buf_p='_';\
    yy_c_buf_p--;\
    *yy_c_buf_p='E';\
    yy_c_buf_p--;\
    *yy_c_buf_p='N';\
    yy_c_buf_p--;\
    *yy_c_buf_p='I';\
    yy_c_buf_p--;\
    *yy_c_buf_p='L';\
    yy_c_buf_p--;\
    *yy_c_buf_p='_';\
    yy_c_buf_p--;\
    *yy_c_buf_p='_';\
    yy_c_buf_p--;\
    return IDENTIFIER;\
}while (0);
#define IDENTIFIER_Handler do{\
    sysy2eeyore_lval.string = yytext;\
    if (Debug_Token)\
        printf("Find_Identifier (%d %d) %s\n",yylineno,yycolumn,sysy2eeyore_lval.string);\
    return IDENTIFIER;\
} while(0);
#define INTEGER_VALUE_Handler(INTEGER_BASE) do{\
    sysy2eeyore_lval.string = yytext;\
    if (Debug_Token)\
        printf("Find_Integer_Value (%d %d) %s\n",yylineno,yycolumn,sysy2eeyore_lval.string);\
    return INTEGER_BASE;\
} while(0);
#define LINE_Handler do{\
    sysy2eeyore_lval.number = yylineno;\
    return INTEGER_VALUE_INT;\
}while (0)

#define YY_USER_ACTION do{\
    for (int i=0;i<yyleng;i++){\
        if (yytext[i]=='\n')\
            yylineno++,yycolumn=0;\
        else yycolumn++;\
    }\
}while(0);
int yycolumn = 0;
%}

%%


\/\/.*\n?                                                       ;//Print_Define();
"/*"                                                            do{
                                                                    int c;
                                                                    while((c=yyinput())!=0){
                                                                        if(c == '\n')yylineno++,yycolumn=0;
                                                                        else{
                                                                            yycolumn++;
                                                                            if (c=='*'){
                                                                                if((c=yyinput())=='/'){yycolumn++;break;}
                                                                                else unput(c);
                                                                            }
                                                                        }
                                                                    }
                                                                }while(0);
[ ]                                                             ;
\n                                                              ;
[\t\r]                                                          ;
"if"                                                            Reserved_Word_Handler(IF);
"else"                                                          Reserved_Word_Handler(ELSE);
"while"                                                         Reserved_Word_Handler(WHILE);
"for"                                                           Reserved_Word_Handler(FOR);
"break"                                                         Reserved_Word_Handler(BREAK);
"continue"                                                      Reserved_Word_Handler(CONTINUE);
"return"                                                        Reserved_Word_Handler(RETURN);
"const"                                                         Reserved_Word_Handler(CONST);
"int"                                                           Reserved_Word_Handler(INT);
"void"                                                          Reserved_Word_Handler(VOID);
"starttime"[ \t\n]*"("                                          STARTTIME_Handler;
"stoptime"[ \t\n]*"("                                           STOPTIME_Handler;
"__LINE__"                                                      LINE_Handler;
[a-zA-Z_][a-zA-Z0-9_]*                                          IDENTIFIER_Handler;
0[0-9]+                                                         INTEGER_VALUE_Handler(INTEGER_VALUE_BASE_8);
0|[1-9][0-9]*                                                   INTEGER_VALUE_Handler(INTEGER_VALUE_BASE_10);
"0x"[0-9a-fA-F]+|"0X"[0-9a-fA-F]+                               INTEGER_VALUE_Handler(INTEGER_VALUE_BASE_16);
"="                                                             Operation_Handler(ASSIGN);
"=="                                                            Operation_Handler(EQ);
"!="                                                            Operation_Handler(NE);
"<"                                                             Operation_Handler(LT);
"<="                                                            Operation_Handler(LE);
">"                                                             Operation_Handler(GT);
">="                                                            Operation_Handler(GE);
"&&"                                                            Operation_Handler(AND);
"||"                                                            Operation_Handler(OR);
"+"                                                             Operation_Handler(PLUS);
"-"                                                             Operation_Handler(MINUS);
"*"                                                             Operation_Handler(MUL);
"/"                                                             Operation_Handler(DIV);
"%"                                                             Operation_Handler(MOD);
"!"                                                             Operation_Handler(NOT);
"."                                                             Operation_Handler(DOT);
"("                                                             Symbol_Handler(LPAREN);
")"                                                             Symbol_Handler(RPAREN);
"["                                                             Symbol_Handler(LSQUARE);
"]"                                                             Symbol_Handler(RSQUARE);
"{"                                                             Symbol_Handler(LBRACE);
"}"                                                             Symbol_Handler(RBRACE);
","                                                             Symbol_Handler(COMMA);
";"                                                             Symbol_Handler(SEMI);


%%
