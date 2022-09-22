%option noyywrap
%option prefix = "eeyore2tigger_"
%{
#include<string.h>
#include "../../eeyore2tigger/parser/parser.tab.hpp"
#include "../../eeyore2tigger/ast/node.h"
%}


number                              -?[0-9]+
identifier                          [a-zA-Z_]([a-zA-Z_0-9])*

%%
\n                                  {}
[ \t]+                              {}
"/*"(([^\*]*(\*[^\/])?)*)"*/"       {}
\/\/.*                              {}
"if"                                {return IF;}
"var"                               {return VAR;}
"return"                            {return RETURN;}
"goto"                              {return GOTO;}
"call"                              {return CALL;}
"end"                               {return END;}
"param"                             {return PARAM;}
l{number}                           {eeyore2tigger_lval.string = strdup(eeyore2tigger_text);return LABEL;}
{number}                            {eeyore2tigger_lval.value = atoi(eeyore2tigger_text);return NUMBER;}
[Ttp]{number}                       {eeyore2tigger_lval.string = strdup(eeyore2tigger_text);return SYMBOL;}
f_{identifier}                      {eeyore2tigger_lval.string = strdup(eeyore2tigger_text);return FUNCTION;}
"="                                 {return ASSIGN;}
"=="                                {eeyore2tigger_lval.op=new OpNode(EQ_t);return LOGICOP;}
"!="                                {eeyore2tigger_lval.op=new OpNode(NE_t);return LOGICOP;}
"<"                                 {eeyore2tigger_lval.op=new OpNode(LT_t);return LOGICOP;}
"<="                                {eeyore2tigger_lval.op=new OpNode(LE_t);return LOGICOP;}
">"                                 {eeyore2tigger_lval.op=new OpNode(GT_t);return LOGICOP;}
">="                                {eeyore2tigger_lval.op=new OpNode(GE_t);return LOGICOP;}
"&&"                                {eeyore2tigger_lval.op=new OpNode(AND_t);return OP;}
"||"                                {eeyore2tigger_lval.op=new OpNode(OR_t);return OP;}
"+"                                 {eeyore2tigger_lval.op=new OpNode(PLUS_t);return OP;}
"-"                                 {eeyore2tigger_lval.op=new OpNode(SUB_t);return OP;}
"*"                                 {eeyore2tigger_lval.op=new OpNode(MUL_t);return OP;}
"/"                                 {eeyore2tigger_lval.op=new OpNode(DIV_t);return OP;}
"%"                                 {eeyore2tigger_lval.op=new OpNode(REM_t);return OP;}
"!"                                 {eeyore2tigger_lval.op=new OpNode(NOT_t);return OP;}
"["                                 {return LSQUARE;}
"]"                                 {return RSQUARE;}
":"                                 {return COLON;}
%%
