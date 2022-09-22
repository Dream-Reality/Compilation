%option noyywrap
%option prefix = "tigger2riscv_"
%{
#include<string>
#include "../../tigger2riscv/parser/parser.tab.hpp"
#include "../../tigger2riscv/parser/token.h"
%}

number                              -?[0-9]+
identifier                          [a-zA-Z_]([a-zA-Z_0-9])*
%%

\n                                                      {}
[ \t\r]+                                                {}
"/*"(([^\*]*(\*[^\/])?)*)"*/"                           {}
\/\/.*                                                  {}
"if"                                                    {return IF;}
"return"                                                {return RETURN;}
"goto"                                                  {return GOTO;}
"call"                                                  {return CALL;}
"malloc"                                                {return MALLOC;}
"store"                                                 {return STORE;}
"load"                                                  {return LOAD;}
"loadaddr"                                              {return LOADADDR;}
"end"                                                   {return END;}
((x0)|(s([0-9]|(10)|(11)))|(t[0-6])|(a[0-7]))           {tigger2riscv_lval.string = strdup(tigger2riscv_text); return REGISTER;}
l{number}                                               {tigger2riscv_lval.string = strdup(tigger2riscv_text); return LABEL;}
{number}                                                {tigger2riscv_lval.number = atoi(tigger2riscv_text); return NUMBER;}
v{number}                                               {tigger2riscv_lval.string = strdup(tigger2riscv_text); return VARIABLE;}
f_{identifier}                                          {tigger2riscv_lval.string = strdup(tigger2riscv_text); return FUNCTION;}
"<"                                                     {tigger2riscv_lval.op = new OP_t(LT_t);return LOGICOP;}
">"                                                     {tigger2riscv_lval.op = new OP_t(GT_t);return LOGICOP;}
"<="                                                    {tigger2riscv_lval.op = new OP_t(LE_t);return LOGICOP;}
">="                                                    {tigger2riscv_lval.op = new OP_t(GE_t);return LOGICOP;}
"!="                                                    {tigger2riscv_lval.op = new OP_t(NE_t);return LOGICOP;}
"=="                                                    {tigger2riscv_lval.op = new OP_t(EQ_t);return LOGICOP;}
"+"                                                     {tigger2riscv_lval.op = new OP_t(PLUS_t);return OP;}
"-"                                                     {tigger2riscv_lval.op = new OP_t(SUB_t);return OP;}
"*"                                                     {tigger2riscv_lval.op = new OP_t(MUL_t);return OP;}
"/"                                                     {tigger2riscv_lval.op = new OP_t(DIV_t);return OP;}
"%"                                                     {tigger2riscv_lval.op = new OP_t(REM_t);return OP;}
"&&"                                                    {tigger2riscv_lval.op = new OP_t(AND_t);return OP;}
"||"                                                    {tigger2riscv_lval.op = new OP_t(OR_t);return OP;}
"!"                                                     {tigger2riscv_lval.op = new OP_t(NOT_t);return OP;}
"="                                                     {return ASSIGN;}
"["                                                     {return LSQUARE;}
"]"                                                     {return RSQUARE;}
":"                                                     {return COLON;}
%%
