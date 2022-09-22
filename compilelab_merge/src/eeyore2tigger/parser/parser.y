%define api.prefix {eeyore2tigger_}
%{
#include<string.h>
#include"../../eeyore2tigger/parser/parser.tab.hpp"
#include"../../eeyore2tigger/ast/node.h"
extern int yydebug;
extern int yylex();
ProgramsNode* root;
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
    int value;
    const char* string;
    class ProgramsNode* programs;
    class ProgramNode* program;
    class DeclarationNode* declaration;
    class InitializationNode* initialization;
    class FunctionDefNode* functiondef;
    class FunctionHeaderNode* functionheader;
    class StatementsNode* statements;
    class StatementNode* statement;
    class ExpressionNode* expression;
    class RightValueNode* rightvalue;
    class OpNode* op;
}

%token IF VAR RETURN GOTO CALL PARAM END LSQUARE RSQUARE COLON ASSIGN 
%token<value> NUMBER
%token<string> IDENT FUNC SYMBOL FUNCTION LABEL
%token<op> LOGICOP OP

%type<programs> Programs
%type<program> Program
%type<declaration> Declaration
%type<initialization> Initialization
%type<functiondef> FunctionDef
%type<functionheader> FunctionHeader;
%type<statements> Statements
%type<statement> Statement
%type<expression> Expression
%type<rightvalue> RightValue
%type<op> BinOp
%start Programs
%%
Programs: Programs Program {$$->push_back($2);}
        | Program {$$ = new ProgramsNode();root = $$;$$->push_back($1);}
        ;

Program: Declaration {$$ = new ProgramNode($1);}
       | Initialization {$$ = new ProgramNode($1);}
       | FunctionDef {$$ = new ProgramNode($1);}
       ;

Declaration: VAR SYMBOL {$$ = new DeclarationNode($2);}
           | VAR NUMBER SYMBOL {$$ = new DeclarationNode($3,$2);}
           ;

Initialization: SYMBOL ASSIGN NUMBER {$$ = new InitializationNode($1,$3);}
              | SYMBOL LSQUARE NUMBER RSQUARE ASSIGN NUMBER {$$ = new InitializationNode($1,$3,$6);}
              ;

FunctionDef: FunctionHeader Statements FunctionEnd {$$ = new FunctionDefNode($1,$2);}
           ;

FunctionHeader: FUNCTION LSQUARE NUMBER RSQUARE {$$ = new FunctionHeaderNode($1,$3);}
              ;

Statements: Statements Statement {$$->push_back($2);}
          | Statement {$$ = new StatementsNode();$$->push_back($1);}
          ;

FunctionEnd: END FUNCTION {}
           ;

Statement: Expression {$$ = new StatementNode($1);}
         | Declaration {$$ = new StatementNode($1);}
         ;

Expression: SYMBOL ASSIGN RightValue BinOp RightValue {$$ = new ExpressionNode(new AssignExpNode($1,$3,$5,$4));}
          | SYMBOL ASSIGN OP RightValue {$$ = new ExpressionNode(new AssignUnaryExpNode($1,$4,$3));}
          | SYMBOL ASSIGN RightValue {$$ = new ExpressionNode(new AssignNode($1,$3));}
          | SYMBOL LSQUARE RightValue RSQUARE ASSIGN RightValue {$$ = new ExpressionNode(new AssignArrayNode($1,$3,$6));}
          | SYMBOL ASSIGN SYMBOL LSQUARE RightValue RSQUARE {$$ = new ExpressionNode(new ArrayAssignNode($1,$3,$5));}
          | IF RightValue LOGICOP RightValue GOTO LABEL {$$ = new ExpressionNode(new IfNode($2,$4,$3,$6));}
          | GOTO LABEL {$$ = new ExpressionNode(new GotoNode($2));}
          | LABEL COLON {$$ = new ExpressionNode($1);}
          | PARAM RightValue {$$ =new ExpressionNode(new ParamNode($2));}
          | CALL FUNCTION {$$ = new ExpressionNode(new CallNode($2));}
          | SYMBOL ASSIGN CALL FUNCTION {$$ = new ExpressionNode(new CallReturnNode($1,$4));}
          | RETURN RightValue {$$ = new ExpressionNode(new ReturnValueNode($2));}
          | RETURN {$$ = new ExpressionNode(new ReturnNode());}

RightValue: SYMBOL {$$ = new RightValueNode($1);}
          | NUMBER {$$ = new RightValueNode($1);}

BinOp: OP {$$ = $1;}
     | LOGICOP {$$ = $1;}
