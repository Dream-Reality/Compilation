%define api.prefix {tigger2riscv_}
%{
#include"../../tigger2riscv/parser/token.h"
#include "../../tigger2riscv/parser/parser.tab.hpp"
#include<string>
extern int yydebug;
extern int yylex();

int STK;
#define YYERROR_VERBOSE true
void yyerror(const char *s) {
    printf("%s\n",s);
}
bool is_int12(int x){
    return x>=-2048&&x<=2047;
}
bool is_int10(int x){
    return x>=-512&&x<=511;
}
void operation_register_and_register(const char* reg1,const char* reg2,const char* reg3,OPType_t op){
    switch (op){
        case PLUS_t:printf("add %s, %s, %s\n",reg1,reg2,reg3);break;
        case SUB_t:printf("sub %s, %s, %s\n",reg1,reg2,reg3);break;
        case MUL_t:printf("mul %s, %s, %s\n",reg1,reg2,reg3);break;
        case DIV_t:printf("div %s, %s, %s\n",reg1,reg2,reg3);break;
        case REM_t:printf("rem %s, %s, %s\n",reg1,reg2,reg3);break;
        case LT_t:printf("slt %s, %s, %s\n",reg1,reg2,reg3);break;
        case GT_t:printf("sgt %s, %s, %s\n",reg1,reg2,reg3);break;
        case LE_t:printf("sgt %s, %s, %s\nseqz %s, %s\n",reg1,reg2,reg3,reg1,reg1);break;
        case GE_t:printf("slt %s, %s, %s\nseqz %s, %s\n",reg1,reg2,reg3,reg1,reg1);break;
        case AND_t:printf("snez %s, %s\nsnez t0, %s\nand %s, %s, t0\n",reg1,reg2,reg3,reg1,reg1);break;
        case OR_t:printf("or %s, %s, %s\nsnez %s, %s\n",reg1,reg2,reg3,reg1,reg1);break;
        case NE_t:printf("xor %s, %s, %s\nsnez %s, %s\n",reg1,reg2,reg3,reg1,reg1);break;
        case EQ_t:printf("xor %s, %s, %s\nseqz %s, %s\n",reg1,reg2,reg3,reg1,reg1);break;
        default:printf("operation_register_and_register: error\n");exit(0);
    }
}
void operation_register_and_number(const char* reg1,const char* reg2,int number,OPType_t op){
    if (is_int12(number)){
        if (op == PLUS_t){printf("addi %s, %s, %d\n",reg1,reg2,number);return;}
        if (op == LT_t){printf("lti %s, %s, %d\n",reg1,reg2,number);return;}
    }
    printf("li t0, %d\n",number);
    operation_register_and_register(reg1,reg2,"t0",op);
}
void operation_register(const char* reg1,const char* reg2,OPType_t op){
    switch (op){
        case SUB_t:printf("neg %s, %s\n",reg1,reg2);break;
        case NOT_t:printf("seqz %s, %s\n",reg1,reg2);break;
        default:printf("operation_register: error\n");exit(0);
    }
}
#ifdef YYDEBUG
#undef YYDEBUG
#endif
#define YYDEBUG 1
%}

%locations
%union{
    int number;
    const char* string;
    class OP_t* op;
    int empty;
}

%token IF RETURN GOTO CALL PARAM END LOAD STORE LOADADDR MALLOC LSQUARE RSQUARE COLON ASSIGN
%token<number> NUMBER
%token<string> LABEL VARIABLE FUNCTION REGISTER
%token<op> LOGICOP OP

%type<op> BINOP
%type<empty> Programs Program GlobalVarDecl FunctionDef FunctionHeader FunctionEnd Expression

%start Programs
%%
Programs: Programs Program {}
        | Program {}
        ;

Program: FunctionDef {}
       | GlobalVarDecl {}
       ;

GlobalVarDecl: VARIABLE ASSIGN NUMBER {printf(".global %s\n.section .sdata\n.align 2\n.type %s, @object\n.size %s, 4\n%s:\n.word %d\n\n",$1,$1,$1,$1,$3);}
             | VARIABLE ASSIGN MALLOC NUMBER {printf(".comm %s, %d, 4\n\n",$1,$4);}
             ;

FunctionDef: FunctionHeader Expressions FunctionEnd {}
           ;

FunctionHeader: FUNCTION LSQUARE NUMBER RSQUARE LSQUARE NUMBER RSQUARE {
        STK = ($6/4+1)*16;
        printf(".text\n.align 2\n\n.global %s\n.type %s, @function\n%s:\n",$1+2,$1+2,$1+2);
        if (is_int12(-STK))printf("addi sp, sp, %d\n",-STK);
        else{
            printf("li t0, %d\n",-STK);
            printf("add sp, sp, t0\n");
        }
        if (is_int12(STK-4))printf("sw ra, %d(sp)\n\n",STK-4);
        else{
            printf("li t0, %d\n",STK-4);
            printf("add t0, t0, sp\n");
            printf("sw ra, 0(t0)\n");
        }
}
              ;

Expressions: Expressions Expression {}
           | Expression {}

FunctionEnd: END FUNCTION {printf(".size %s, .-%s\n\n",$2+2,$2+2);};

Expression: REGISTER ASSIGN REGISTER BINOP REGISTER {operation_register_and_register($1,$3,$5,($4)->op);}
          | REGISTER ASSIGN REGISTER BINOP NUMBER {operation_register_and_number($1,$3,$5,($4)->op);}
          | REGISTER ASSIGN OP REGISTER {operation_register($1,$4,($3)->op);}
          | REGISTER ASSIGN REGISTER {printf("mv %s, %s\n",$1,$3);}
          | REGISTER ASSIGN NUMBER {printf("li %s, %d\n",$1,$3);}
          | REGISTER LSQUARE NUMBER RSQUARE ASSIGN REGISTER {
              if (is_int12($3))printf("sw %s, %d(%s)\n",$6,$3,$1);
              else {
                  printf("li t0, %d\n",$3);
                  printf("add t0, t0, %s\n",$1);
                  printf("sw %s, 0(t0)\n",$6);
              }
          }
          | REGISTER ASSIGN REGISTER LSQUARE NUMBER RSQUARE {
              if (is_int12($5))printf("lw %s, %d(%s)\n",$1,$5,$3);
              else {
                  printf("li t0, %d\n",$5);
                  printf("add t0, t0, %s\n",$3);
                  printf("lw %s ,0(t0)\n",$1);
              }
          }
          | IF REGISTER LOGICOP REGISTER GOTO LABEL {
              switch(($3)->op){
                  case LT_t:printf("blt %s, %s, .%s\n",$2,$4,$6);break;
                  case GT_t:printf("bgt %s, %s, .%s\n",$2,$4,$6);break;
                  case LE_t:printf("ble %s, %s, .%s\n",$2,$4,$6);break;
                  case GE_t:printf("bge %s, %s, .%s\n",$2,$4,$6);break;
                  case NE_t:printf("bne %s, %s, .%s\n",$2,$4,$6);break;
                  case EQ_t:printf("beq %s, %s, .%s\n",$2,$4,$6);break;
                  default:printf("LOGICOP: error\n");exit(0);
              }
          }
          | GOTO LABEL {printf("j .%s\n",$2);}
          | LABEL COLON {printf(".%s:\n",$1);}
          | CALL FUNCTION {printf("call %s\n",$2+2);}
          | RETURN {
              if (is_int12(STK-4))printf("lw ra, %d(sp)\n",STK-4);
              else {
                  printf("li t0, %d\n",STK-4);
                  printf("add t0, t0, sp\n");
                  printf("lw ra, 0(t0)\n");
              }
              if (is_int12(STK))printf("addi sp, sp, %d\n",STK);
              else {
                  printf("li t0, %d\n",STK);
                  printf("add sp, sp, t0\n");
              }
              printf("ret\n");
          }
          | STORE REGISTER NUMBER {
              if (is_int10($3))printf("sw %s, %d(sp)\n",$2,4*$3);
              else {
                  printf("li t0, %d\n",4*$3);
                  printf("add t0, t0, sp\n");
                  printf("sw %s, 0(t0)\n",$2);
              }
          }
          | LOAD NUMBER REGISTER {
              if (is_int10($2))printf("lw %s, %d(sp)\n",$3,4*$2);
              else{
                  printf("li t0, %d\n",4*$2);
                  printf("add t0, t0, sp\n");
                  printf("lw %s, 0(t0)\n",$3);
              }
          }
          | LOAD VARIABLE REGISTER {
              printf("lui %s, %%hi(%s)\n",$3,$2);
              printf("lw %s, %%lo(%s)(%s)\n",$3,$2,$3);
          }
          | LOADADDR NUMBER REGISTER {
              if (is_int10($2))printf("addi %s, sp, %d\n",$3,4*$2);
              else {
                  printf("li t0, %d\n",4*$2);
                  printf("add %s, t0, sp\n",$3);
              }
          }
          | LOADADDR VARIABLE REGISTER {
              printf("la %s, %s\n",$3,$2);
          }
          ;

BINOP: LOGICOP {$$ = $1;}
     | OP {$$ = $1;}
     ;
