%define api.prefix {sysy2eeyore_}
%{
#include<stdio.h>
#include<string.h>
#include"../../sysy2eeyore/parser/parser.tab.hpp"
#include"../../sysy2eeyore/ast/node.h"
extern int yydebug;

extern int yylex();
CompUnitsNode* sysy2eeyore_root;

#define YYERROR_VERBOSE true
void yyerror(const char *s) {
    printf("%s\n",s);
}
#ifdef YYDEBUG
#undef YYDEBUG
#endif
#define YYDEBUG 1

%}

%locations
%union{
    int token;
    const char* string;
    int number;
    class IdentNode* ident_t;
    class CompUnitNode* compunit_t;
    class CompUnitsNode* compunits_t;
    class DeclsNode* decls_t;
    class DeclNode* decl_t;
    class Decl_OneNode* decl_one_t;
    class Decl_ArrayNameNode* decl_arrayname_t;
    class Decl_ArrayNode* decl_array_t;
    class Decl_ArrayValueNode* decl_arrayvalue_t;
    class FuncDefNode* funcdef_t;
    class ExpNode* exp_t;
    class UnaryExpNode* unaryexp_t;
    class PrimaryExpNode* primaryexp_t;
    class FunctionCallNode* functioncall_t;
    class FuncRParamsNode* funcrparams_t;
    class LValNode* lval_t;
    class ArrayIdentNode* arrayident_t;
    class FuncFParamsNode* funcfparams_t;
    class FuncFParamNode* funcfparam_t;
    class FuncFParamParam_Node* funcfparamparam_t;
    class FuncFParamArray_Node* funcfparamarray_t;
    class BlockItemsNode* blockitems_t;
    class BlockItemNode* blockitem_t;
    class StmtNode* stmt_t;
    class AssignStmtNode* assignstmt_t;
    class EmptyStmtNode* emptystmt_t;
    class ExpStmtNode* expstmt_t;
    class WhileStmtNode* whilestmt_t;
    class BreakStmtNode* breakstmt_t;
    class ContinueStmtNode* continuestmt_t;
    class ReturnStmtNode* returnstmt_t;
    class IfElseStmtNode* ifelsestmt_t;
    class OperationNode* op_t;
    class NumberNode* number_t;
    class BTypeNode* btype_t;
}
%token <number> INTEGER_VALUE_INT
%token <string> INTEGER_VALUE_BASE_8 INTEGER_VALUE_BASE_10 INTEGER_VALUE_BASE_16 IDENTIFIER
%token <token> IF ELSE WHILE FOR
%token <token> BREAK CONTINUE RETURN
%token <token> CONST INT VOID
%token <token> ASSIGN EQ NE LT LE GT GE
%token <token> AND OR
%token <token> LPAREN RPAREN LBRACE RBRACE LSQUARE RSQUARE

%token <token> DOT COMMA SEMI
%token <token> PLUSPLUS MINUSMINUS
%token <token> PLUS MINUS MUL DIV MOD NOT

%left COMMA
%left ASSIGN
%left EQ NE LT LE GT GE
%left PLUS MINUS
%left MUL DIV MOD
%left NOT

%type <op_t> AddOp MulOp RelOp UnaryOp OrOp AndOp EqOp NeOp
%type <btype_t> BType
%type <compunits_t> CompUnits
%type <compunit_t> CompUnit NormDecl
%type <decls_t> Decl ConstDecl VarDecl ConstDefs VarDefs
%type <decl_t> ConstDef VarDef
%type <decl_one_t> ConstDef_Const VarDef_Variable
%type <decl_array_t> ConstDef_Array VarDef_Array
%type <funcdef_t> VoidFuncDef BTypeFuncDef
%type <exp_t> Const_InitVal_Const ConstExp Varable_InitVal_Var VariableExp Exp LOrExp LAndExp EqExp RelExp AddExp MulExp Cond FuncRParam Unary2MulExp
%type <decl_arrayname_t> ArrayName
%type <decl_arrayvalue_t> Const_InitVal_Array Const_InitVal_Array_Inner Variable_InitVal_Array Variable_InitVal_Array_Inner 
%type <unaryexp_t> UnaryExp
%type <functioncall_t> FunctionCall
%type <funcrparams_t> FuncRParams
%type <primaryexp_t> PrimaryExp
%type <lval_t> LVal
%type <arrayident_t> ArrayIdent
%type <funcfparams_t> FuncFParams
%type <funcfparam_t> FuncFParam
%type <funcfparamparam_t> FuncFParam_Param
%type <funcfparamarray_t> FuncFParam_Array
%type <blockitems_t> Block BlockItems
%type <blockitem_t> BlockItem
%type <stmt_t> Stmt Complete_Stmt OtherStmt
%type <assignstmt_t> AssignStmt
%type <emptystmt_t> EmptyStmt
%type <expstmt_t> ExpStmt
%type <whilestmt_t> WhileStmt
%type <breakstmt_t> BreakStmt
%type <continuestmt_t> ContinueStmt
%type <returnstmt_t> ReturnStmt
%type <ifelsestmt_t> Incomplete_IfElseStmt Complete_IfElseStmt
%type <ident_t> Ident
%type <number_t> Number



%start CompUnits

%%
CompUnits: CompUnits CompUnit { $$->push_back($2); }
         | CompUnit {sysy2eeyore_root = new CompUnitsNode(); $$ = sysy2eeyore_root; $$->push_back($1);}
         ;

CompUnit: ConstDecl {$$ = new CompUnitNode($1);} 
        | VoidFuncDef {$$ = new CompUnitNode($1);}
        | BType NormDecl {$$ = $2;}
        ;

NormDecl: VarDefs SEMI {$$ = new CompUnitNode($1);}
        | BTypeFuncDef {$$ = new CompUnitNode($1);}
        ;

BTypeFuncDef: Ident LPAREN FuncFParams RPAREN Block {$$ = new FuncDefNode(RINT_t,$1,$3,$5);}
            | Ident LPAREN RPAREN Block {$$ = new FuncDefNode(RINT_t,$1,nullptr,$4);}
            ;

VoidFuncDef: VOID Ident LPAREN FuncFParams RPAREN Block {$$ = new FuncDefNode(RVOID_t,$2,$4,$6);}
           | VOID Ident LPAREN RPAREN Block {$$ = new FuncDefNode(RVOID_t,$2,nullptr,$5);}
           ;

Decl: ConstDecl {$$ = $1;}
    | VarDecl {$$ = $1;}
    ;

ConstDecl: CONST BType ConstDefs SEMI {$$ = $3;$$->ValueType = $2->type;}
         ;

ConstDefs: ConstDefs COMMA ConstDef {$$->push_back($3);}
         | ConstDef {$$ = new DeclsNode(Const_t,INT_t);$$->push_back($1);}
         ;

ConstDef: ConstDef_Const {$$ = new DeclNode($1);}
        | ConstDef_Array {$$ = new DeclNode($1);}
        ;

ConstDef_Const: Ident ASSIGN Const_InitVal_Const {$$ = new Decl_OneNode(Const_t,$1,$3);}
              ;

Const_InitVal_Const: ConstExp {$$ = $1;}
                   ;

ConstExp: AddExp {$$ = $1;}
        ;

ConstDef_Array: ArrayName ASSIGN Const_InitVal_Array {$$ = new Decl_ArrayNode(Const_t,$1,$3);}
              ;


ArrayName: Ident LSQUARE ConstExp RSQUARE {$$ = new Decl_ArrayNameNode($1);$$->dim.push_back($3);}
         | ArrayName LSQUARE ConstExp RSQUARE {$$->dim.push_back($3);}
         ;


Const_InitVal_Array: ConstExp {$$ = new Decl_ArrayValueNode($1);}
                   | LBRACE RBRACE {$$ = new Decl_ArrayValueNode((Decl_ArrayValueNode*)nullptr);}
                   | LBRACE Const_InitVal_Array_Inner RBRACE {$$ = $2;}
                   ;


Const_InitVal_Array_Inner: Const_InitVal_Array {$$ = new Decl_ArrayValueNode($1);}
                         | Const_InitVal_Array_Inner COMMA Const_InitVal_Array {$$->push_back($3);}
                         ;

VarDecl: BType VarDefs SEMI {$$ = $2;$$->ValueType = $1->type;}
       ;

VarDefs: VarDefs COMMA VarDef {$$->push_back($3);}
       | VarDef {$$ = new DeclsNode(Variable_t,INT_t);$$->push_back($1);}
       ;

VarDef: VarDef_Variable {$$ = new DeclNode($1);}
      | VarDef_Array {$$ = new DeclNode($1);}
      ;

VarDef_Variable: Ident ASSIGN Varable_InitVal_Var {$$ = new Decl_OneNode(Variable_t,$1,$3);}
               | Ident {$$ = new Decl_OneNode(Variable_t,$1);}
               ;

Varable_InitVal_Var: VariableExp {$$ = $1;}
                   ;

VariableExp: Exp {$$ = $1;}
           ;

VarDef_Array: ArrayName ASSIGN Variable_InitVal_Array {$$ = new Decl_ArrayNode(Variable_t,$1,$3);}
            | ArrayName {$$ = new Decl_ArrayNode(Variable_t,$1);}
            ;

Variable_InitVal_Array: VariableExp {$$ = new Decl_ArrayValueNode($1);}
                      | LBRACE RBRACE {$$ = new Decl_ArrayValueNode((Decl_ArrayValueNode*)nullptr);}
                      | LBRACE Variable_InitVal_Array_Inner RBRACE {$$ = $2;}
                      ;

Variable_InitVal_Array_Inner: Variable_InitVal_Array {$$ = new Decl_ArrayValueNode($1);}
                            | Variable_InitVal_Array_Inner COMMA Variable_InitVal_Array {$$->push_back($3);}
                            ;

Exp: AddExp {$$ = $1;}
   ;

LOrExp: LAndExp {$$ = $1;}
      | LOrExp OrOp LAndExp {$$ = new ExpNode($2,$1,$3);}
      ;

LAndExp: EqExp {$$ = $1;}
       | LAndExp AndOp EqExp {$$ = new ExpNode($2,$1,$3);}
       ;

EqExp: RelExp {$$ = $1;}
     | EqExp EqOp RelExp {$$ = new ExpNode($2,$1,$3);}
     | EqExp NeOp RelExp {$$ = new ExpNode($2,$1,$3);}
     ;

RelExp: AddExp {$$ = $1;}
      | RelExp RelOp AddExp {$$ = new ExpNode($2,$1,$3);}
      ;

AddExp: MulExp {$$ = $1;}
      | AddExp AddOp MulExp {$$ = new ExpNode($2,$1,$3);}
      ;

MulExp: Unary2MulExp {$$ = $1;}
      | MulExp MulOp Unary2MulExp {$$ = new ExpNode($2,$1,$3);}
      ;

Unary2MulExp: UnaryExp {$$ = new ExpNode($1);}
            ;

UnaryExp: PrimaryExp {$$ = new UnaryExpNode($1);}
        | FunctionCall {$$ = new UnaryExpNode($1);}
        | UnaryOp UnaryExp {$$ = new UnaryExpNode($1,$2);}
        ;

FunctionCall: Ident LPAREN FuncRParams RPAREN {$$ = new FunctionCallNode($1,$3);}
            | Ident LPAREN RPAREN {$$ = new FunctionCallNode($1,nullptr);}
            ;

FuncRParams: FuncRParam {$$ = new FuncRParamsNode();$$->push_back($1);}
           | FuncRParams COMMA FuncRParam {$$->push_back($3);}
           ;

FuncRParam: Exp {$$ = $1;}
          ;

PrimaryExp: LVal {$$ = new PrimaryExpNode($1);}
          | Number {$$ = new PrimaryExpNode($1);}
          | LPAREN Exp RPAREN {$$ = new PrimaryExpNode($2);}
          ;

LVal: Ident {$$ = new LValNode($1);}
    | ArrayIdent {$$ = new LValNode($1);}
    ;
    
ArrayIdent: Ident LSQUARE Exp RSQUARE {$$ = new ArrayIdentNode($1);$$->push_back($3);}
          | ArrayIdent LSQUARE Exp RSQUARE {$$->push_back($3);}
          ;

FuncFParams: FuncFParam {$$ = new FuncFParamsNode();$$->push_back($1);}
           | FuncFParams COMMA FuncFParam {$$->push_back($3);}
           ;

FuncFParam: FuncFParam_Param {$$ = new FuncFParamNode($1);}
          | FuncFParam_Array {$$ = new FuncFParamNode($1);}
          ;

FuncFParam_Param: BType Ident {$$ = new FuncFParamParam_Node($1,$2);}
                ;

FuncFParam_Array: BType Ident LSQUARE RSQUARE {$$ = new FuncFParamArray_Node($1,$2);}
                | FuncFParam_Array LSQUARE Exp RSQUARE {$$->dim.push_back($3);}
                ;

Block: LBRACE BlockItems RBRACE {$$ = $2;}
     | LBRACE RBRACE {$$ = new BlockItemsNode();}
     ;

BlockItems: BlockItem {$$ = new BlockItemsNode(); $$->push_back($1);}
          | BlockItems BlockItem {$$->push_back($2);}
          ;

BlockItem: Decl {$$ = new BlockItemNode($1);}
         | Stmt {$$ = new BlockItemNode($1);}
         ;

Stmt: Complete_Stmt {$$ = $1;}
    | Incomplete_IfElseStmt {$$ = new StmtNode($1);}
    ;

Complete_Stmt: Complete_IfElseStmt {$$ = new StmtNode($1);}
             | OtherStmt {$$ = $1;}
             ;

OtherStmt: AssignStmt {$$ = new StmtNode($1);}
         | EmptyStmt {$$ = new StmtNode($1);}
         | ExpStmt {$$ = new StmtNode($1);}
         | WhileStmt {$$ = new StmtNode($1);}
         | BreakStmt {$$ = new StmtNode($1);}
         | ContinueStmt {$$ = new StmtNode($1);}
         | ReturnStmt {$$ = new StmtNode($1);}
         | Block {$$ = new StmtNode($1);}
         ;

AssignStmt: LVal ASSIGN Exp SEMI {$$ = new AssignStmtNode($1,$3);}
          ;

EmptyStmt: SEMI {$$ = new EmptyStmtNode();}
         ;

ExpStmt: Exp SEMI {$$ = new ExpStmtNode($1);}
       ;

Complete_IfElseStmt: IF LPAREN Cond RPAREN Complete_Stmt ELSE Complete_Stmt {$$ = new IfElseStmtNode($3,$5,$7);}
                   ;

Incomplete_IfElseStmt: IF LPAREN Cond RPAREN Stmt {$$ = new IfElseStmtNode($3,$5);}
                     | IF LPAREN Cond RPAREN Complete_Stmt ELSE Incomplete_IfElseStmt {$$ = new IfElseStmtNode($3,$5,$7);}
                     ;

WhileStmt: WHILE LPAREN Cond RPAREN Stmt {$$ = new WhileStmtNode($3,$5);}
         ;

BreakStmt: BREAK SEMI {$$ =new BreakStmtNode();}
         ;

ContinueStmt: CONTINUE SEMI {$$ = new ContinueStmtNode();}
            ;

ReturnStmt: RETURN Exp SEMI {$$ = new ReturnStmtNode($2);}
          | RETURN SEMI {$$ = new ReturnStmtNode();}
          ;

Cond: LOrExp {$$ = $1;}
    ;

AddOp: PLUS {$$ = new OperationNode(PLUS_t);}
     | MINUS {$$ = new OperationNode(MINUS_t);}
     ;

MulOp: MUL {$$ = new OperationNode(MUL_t);}
     | DIV {$$ = new OperationNode(DIV_t);}
     | MOD {$$ = new OperationNode(MOD_t);}
     ;

UnaryOp: PLUS {$$ = new OperationNode(PLUS_t);}
       | MINUS {$$ = new OperationNode(MINUS_t);}
       | NOT {$$ = new OperationNode(NOT_t);}
       ;

RelOp: GE {$$ = new OperationNode(GE_t);}
     | GT {$$ = new OperationNode(GT_t);}
     | LE {$$ = new OperationNode(LE_t);}
     | LT {$$ = new OperationNode(LT_t);}
     ;

OrOp: OR {$$ = new OperationNode(OR_t);}
    ;
AndOp: AND {$$ = new OperationNode(AND_t);}
     ;
EqOp: EQ {$$ = new OperationNode(EQ_t);}
    ;
NeOp: NE {$$ = new OperationNode(NE_t);}
    ;

Ident: IDENTIFIER {$$ = new IdentNode($1);}
     ;
     
Number: INTEGER_VALUE_BASE_10 {$$ = new NumberNode(strtol($1,nullptr,10));}
      | INTEGER_VALUE_BASE_16 {$$ = new NumberNode(strtol($1,nullptr,16));}
      | INTEGER_VALUE_BASE_8 {$$ = new NumberNode(strtol($1,nullptr,8));}
      | INTEGER_VALUE_INT {$$ = new NumberNode($1);}
      ;

BType: INT {$$ = new BTypeNode(INT_t);}
     ;
