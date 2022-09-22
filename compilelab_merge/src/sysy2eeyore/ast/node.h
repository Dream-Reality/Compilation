#include<bits/stdc++.h>
using namespace std;

class BaseNode;
class IdentNode;
class CompUnitNode;
class CompUnitsNode;
class DeclsNode;
class DeclNode;
class Decl_OneNode;
class Decl_ArrayNameNode;
class Decl_ArrayNode;
class Decl_ArrayValueNode;
class FuncDefNode;
class ExpNode;
class UnaryExpNode;
class PrimaryExpNode;
class FunctionCallNode;
class FuncRParamsNode;
class LValNode;
class ArrayIdentNode;
class FuncFParamsNode;
class FuncFParamNode;
class FuncFParamParam_Node;
class FuncFParamArray_Node;
class BlockItemsNode;
class BlockItemNode;
class StmtNode;
class AssignStmtNode;
class EmptyStmtNode;
class ExpStmtNode;
class WhileStmtNode;
class BreakStmtNode;
class ContinueStmtNode;
class ReturnStmtNode;
class IfElseStmtNode;
class OperationNode;
class NumberNode;
class BTypeNode;



#define TODO
enum NodeType_t{
    BaseNode_t,IdentNode_t,CompUnitNode_t,CompUnitsNode_t,DeclsNode_t,DeclNode_t,Decl_OneNode_t,Decl_ArrayNameNode_t,Decl_ArrayNode_t,Decl_ArrayValueNode_t,FuncDefNode_t,ExpNode_t,UnaryExpNode_t,PrimaryExpNode_t,FunctionCallNode_t,FuncRParamsNode_t,LValNode_t,ArrayIdentNode_t,FuncFParamsNode_t,FuncFParamNode_t,FuncFParamParam_Node_t,FuncFParamArray_Node_t,BlockItemsNode_t,BlockItemNode_t,StmtNode_t,AssignStmtNode_t,EmptyStmtNode_t,ExpStmtNode_t,WhileStmtNode_t,BreakStmtNode_t,ContinueStmtNode_t,ReturnStmtNode_t,IfElseStmtNode_t,OperationNode_t,NumberNode_t,BTypeNode_t
};
enum UnitType_t{
    Decl_t,FuncDef_t
};
enum DeclType_t{
    Const_t,Variable_t
};
enum DeclNumType_t{
    One_t,Array_t
};
enum Type_t{
    INT_t
};

class BaseNode{
public:
    BaseNode* parent;
    NodeType_t nodetype;
    BaseNode();
    ~BaseNode(){}
};

class IdentNode:public BaseNode{
public:
    IdentNode();
    ~IdentNode(){}
    string* name;
    IdentNode(const char* name);
    void printInfo();
};

class CompUnitsNode:public BaseNode{
public:
    CompUnitsNode();
    ~CompUnitsNode(){}
    vector<CompUnitNode*> list;

    void push_back(CompUnitNode* item);
    void printInfo();
};

class CompUnitNode:public BaseNode{
public:
    CompUnitNode();
    ~CompUnitNode(){}
    UnitType_t UnitType;
    union{
        DeclsNode* Decls;
        FuncDefNode* FuncDef;
    }UnitItem;
    CompUnitNode(DeclsNode* Decls);
    CompUnitNode(FuncDefNode* FuncDef);
    void printInfo();
};

class DeclsNode:public BaseNode{
public:
    DeclsNode();
    ~DeclsNode(){}
    vector<DeclNode*> list;
    DeclType_t DeclType;
    Type_t ValueType;
    void push_back(DeclNode* Item);
    DeclsNode(DeclType_t dec,Type_t val);
    void printInfo();
};

class DeclNode:public BaseNode{
public:
    DeclNode();
    ~DeclNode(){}
    DeclNumType_t DeclNumType;
    union{
        Decl_OneNode* Decl_One;
        Decl_ArrayNode* Decl_Array;
    }DeclItem;
    DeclNode(Decl_OneNode* Decl_One);
    DeclNode(Decl_ArrayNode* Decl_Array);
    void printInfo();
};

class Decl_OneNode:public BaseNode{
public:
    Decl_OneNode();
    ~Decl_OneNode(){}
    DeclType_t DeclType;
    IdentNode* ident;
    ExpNode* exp;
    Decl_OneNode(DeclType_t t,IdentNode* ident,ExpNode* exp);
    Decl_OneNode(DeclType_t t,IdentNode* ident);
    void printInfo();
};

class Decl_ArrayNameNode:public BaseNode{
public:
    Decl_ArrayNameNode();
    ~Decl_ArrayNameNode(){}
    IdentNode* Ident;
    vector<ExpNode*> dim;
    Decl_ArrayNameNode(IdentNode* ident);
    void push_back(ExpNode* exp);
    void printInfo();
};

class Decl_ArrayNode:public BaseNode{
public:
    Decl_ArrayNode();
    ~Decl_ArrayNode(){}
    DeclType_t DeclType;
    Decl_ArrayNameNode* name;
    Decl_ArrayValueNode* value;
    Decl_ArrayNode(DeclType_t t,Decl_ArrayNameNode* name,Decl_ArrayValueNode* value);
    Decl_ArrayNode(DeclType_t t,Decl_ArrayNameNode* name);
    void printInfo();
};

enum ItemType_t{
    Item_Array_t,Item_Exp_t
};
class Decl_ArrayValueNode:public BaseNode{
public:
    ~Decl_ArrayValueNode();
    ItemType_t ItemType;
    vector<Decl_ArrayValueNode*> vec;
    ExpNode* exp;
    int ir_dep;
    Decl_ArrayValueNode(ExpNode*exp);
    // Decl_ArrayValueNode();
    Decl_ArrayValueNode(Decl_ArrayValueNode* item);
    // Decl_ArrayValueNode(const Decl_ArrayValueNode&a);
    void push_back(Decl_ArrayValueNode* item);
    void printInfo();
};

enum ReturnValueType{
    RINT_t,RVOID_t
};
class FuncDefNode:public BaseNode{
public:
    FuncDefNode();
    ~FuncDefNode(){}
    ReturnValueType ReturnValue;
    IdentNode* Ident;
    FuncFParamsNode* Params;
    BlockItemsNode* Block;
    FuncDefNode(ReturnValueType returnvalue,IdentNode* ident,FuncFParamsNode* params,BlockItemsNode* block);
    void printInfo();
};

enum ExpType_t{
    Exp_unary_t,Exp_op_t
};
class ExpNode:public BaseNode{
public:
    union{
        pair<pair<ExpNode*,ExpNode*>,OperationNode*>* ExpArgs;
        UnaryExpNode* UnaryExpArgs;
    }args;
    ExpType_t ExpType;
    ExpNode();
    ~ExpNode(){}
    ExpNode(OperationNode* op,ExpNode* arg1,ExpNode* arg2);
    ExpNode(UnaryExpNode* arg);
    void printInfo();
};

enum UnaryExpType_t{
    Unary_Primary_t,Unary_FuncCall_t,Unary_Exp_t
};
class UnaryExpNode:public BaseNode{
public:
    union{
        PrimaryExpNode* PrimaryExp;
        FunctionCallNode* FunctionCall;
        pair<OperationNode*,UnaryExpNode*>* UnaryExp;
    }node;
    UnaryExpType_t UnaryExpType;
    UnaryExpNode();
    ~UnaryExpNode(){}
    UnaryExpNode(PrimaryExpNode* PrimaryExp);
    UnaryExpNode(FunctionCallNode* FunctionCall);
    UnaryExpNode(OperationNode* op,UnaryExpNode* unary);
    void printInfo();
};

enum PrimaryExpType_t{
    Primary_LVal_t,Primary_Number_t,Primary_Exp_t
};
class PrimaryExpNode:public BaseNode{
public:
    PrimaryExpNode();
    ~PrimaryExpNode(){}
    PrimaryExpType_t PrimaryExpType;  
    union{
        LValNode* LVal;
        NumberNode* Number;
        ExpNode* Exp;
    }node;
    PrimaryExpNode(LValNode* lval);
    PrimaryExpNode(NumberNode* number);
    PrimaryExpNode(ExpNode* exp);
    void printInfo();
};

class FunctionCallNode:public BaseNode{
public:
    FunctionCallNode();
    ~FunctionCallNode(){}
    IdentNode* Ident;
    FuncRParamsNode* Param;
    FunctionCallNode(IdentNode* ident,FuncRParamsNode* param);
    void printInfo();
};

class FuncRParamsNode:public BaseNode{
public:
    FuncRParamsNode();
    ~FuncRParamsNode(){}
    vector<ExpNode*> list;
    void push_back(ExpNode*item);
    void printInfo();
};

enum LValType_t{
    LVal_Ident_t,LVal_ArrayIdent_t
};
class LValNode:public BaseNode{
public:
    LValNode();
    ~LValNode(){}
    union{
        IdentNode* Ident;
        ArrayIdentNode* ArrayIdent;
    }node;
    LValType_t LValType;
    LValNode(IdentNode* ident);
    LValNode(ArrayIdentNode* arrayident);
    void printInfo();
};

class ArrayIdentNode:public BaseNode{
public:
    ArrayIdentNode();
    ~ArrayIdentNode(){}
    IdentNode* Ident;
    vector<ExpNode*> dim;
    ArrayIdentNode(IdentNode* ident);
    void push_back(ExpNode*exp);
    void printInfo();
};

class FuncFParamsNode:public BaseNode{
public:
    FuncFParamsNode();
    ~FuncFParamsNode(){}
    vector<FuncFParamNode*> list;
    void push_back(FuncFParamNode* param);
    void printInfo();
};

enum FuncFParamType_t{
    FuncFParamParam_t,FuncFParamArray_t
};
class FuncFParamNode:public BaseNode{
public:
    FuncFParamNode();
    ~FuncFParamNode(){}
    FuncFParamType_t FuncFParamType;
    union{
        FuncFParamParam_Node* param;
        FuncFParamArray_Node* array;
    }node;
    FuncFParamNode(FuncFParamParam_Node*param);
    FuncFParamNode(FuncFParamArray_Node*array);
    void printInfo();
};

class FuncFParamParam_Node:public BaseNode{
public:
    FuncFParamParam_Node();
    ~FuncFParamParam_Node(){}
    BTypeNode* Type;
    IdentNode* Ident;
    FuncFParamParam_Node(BTypeNode* type,IdentNode* ident);
    void printInfo();
};

class FuncFParamArray_Node:public BaseNode{
public:
    FuncFParamArray_Node();
    ~FuncFParamArray_Node(){}
    BTypeNode* Type;
    IdentNode* Ident;
    vector<ExpNode*> dim;
    FuncFParamArray_Node(BTypeNode* type,IdentNode* ident);
    void push_back(ExpNode*item);
    void printInfo();
};

class BlockItemsNode:public BaseNode{
public:
    BlockItemsNode();
    ~BlockItemsNode(){}
    vector<BlockItemNode*> list;
    void push_back(BlockItemNode*item);
    void printInfo();
};

enum BlockItemType_t{
    Block_Decl_t,Block_Stmt_t
};
class BlockItemNode:public BaseNode{
public:
    BlockItemNode();
    ~BlockItemNode(){}
    BlockItemType_t BlockItemType;
    union{
        DeclsNode* Decl;
        StmtNode* Stmt;
    }node;
    BlockItemNode(DeclsNode* decl);
    BlockItemNode(StmtNode* stmt);
    void printInfo();
};

enum StmtType_t{
    Stmt_Assign_t,Stmt_Empty_t,Stmt_Exp_t,Stmt_While_t,Stmt_Break_t,Stmt_Continue_t,Stmt_Return_t,Stmt_Block_t,Stmt_IfElse_t
};
class StmtNode:public BaseNode{
public:
    StmtNode();
    ~StmtNode(){}
    StmtType_t StmtType;
    union{
        AssignStmtNode* AssignStmt;
        EmptyStmtNode* EmptyStmt;
        ExpStmtNode* ExpStmt;
        WhileStmtNode* WhileStmt;
        BreakStmtNode* BreakStmt;
        ContinueStmtNode* ContinueStmt;
        ReturnStmtNode* ReturnStmt;
        BlockItemsNode* BlockItems;
        IfElseStmtNode* IfElseStmt;
    }node;
    StmtNode(AssignStmtNode* assignstmt);
    StmtNode(EmptyStmtNode* emptystmt);
    StmtNode(ExpStmtNode* expstmt);
    StmtNode(WhileStmtNode* whilestmt);
    StmtNode(BreakStmtNode* breakstmt);
    StmtNode(ContinueStmtNode* continuestmt);
    StmtNode(ReturnStmtNode* returnstmt);
    StmtNode(BlockItemsNode* blockitems);
    StmtNode(IfElseStmtNode* ifelsestmt);
    void printInfo();
};

class AssignStmtNode:public BaseNode{
public:
    AssignStmtNode();
    ~AssignStmtNode(){}
    LValNode* LVal;
    ExpNode* Exp;
    AssignStmtNode(LValNode* lval,ExpNode* exp);
    void printInfo();
};

class EmptyStmtNode:public BaseNode{
public:
    EmptyStmtNode();
    ~EmptyStmtNode(){}
    void printInfo();
};

class ExpStmtNode:public BaseNode{
public:
    ExpStmtNode();
    ~ExpStmtNode(){}
    ExpNode* Exp;
    ExpStmtNode(ExpNode* exp);
    void printInfo();
};

class WhileStmtNode:public BaseNode{
public:
    WhileStmtNode();
    ~WhileStmtNode(){}
    ExpNode* Cond;
    StmtNode* Stmt;
    int ir_label_begin,ir_label_end;
    WhileStmtNode(ExpNode*cond, StmtNode*stmt);
    void printInfo();
};

class BreakStmtNode:public BaseNode{
public:
    BreakStmtNode();
    ~BreakStmtNode(){}
    void printInfo();
};

class ContinueStmtNode:public BaseNode{
public:
    ContinueStmtNode();
    ~ContinueStmtNode(){}
    void printInfo();
};

class ReturnStmtNode:public BaseNode{
public:
    ReturnStmtNode();
    ~ReturnStmtNode(){}
    ExpNode* ReturnValue;
    ReturnStmtNode(ExpNode* returnvalue);
    void printInfo();
};

enum ElseType_t{
    Else_Complete_t,Else_Incomplete_t,Else_Empty_t
};
class IfElseStmtNode:public BaseNode{
public:
    IfElseStmtNode();
    ~IfElseStmtNode(){}
    ExpNode* Cond;
    StmtNode* IfStmt;
    union{
        StmtNode* Complete;
        IfElseStmtNode* Incomplete;
    }ElseStmt;
    ElseType_t ElseType;
    IfElseStmtNode(ExpNode* cond,StmtNode* ifstmt);
    IfElseStmtNode(ExpNode* cond,StmtNode* ifstmt,StmtNode* elsestmt);
    IfElseStmtNode(ExpNode* cond,StmtNode* ifstmt,IfElseStmtNode* elsestmt);
    void printInfo();
};

enum Operation_t{
  PLUS_t,MINUS_t,MUL_t,DIV_t,MOD_t,NOT_t,GE_t,GT_t,LE_t,LT_t,OR_t,AND_t,NE_t,EQ_t
};
class OperationNode:public BaseNode{
public:
    OperationNode();
    ~OperationNode(){}
    Operation_t op;
    OperationNode(Operation_t op);
    void printInfo();
};

class NumberNode:public BaseNode{
public:
    NumberNode();
    ~NumberNode(){}
    int value;
    NumberNode(int value);
    void printInfo();
};

class BTypeNode:public BaseNode{
public:
    BTypeNode();
    ~BTypeNode(){}
    Type_t type;
    BTypeNode(Type_t t);
    void printInfo();
};

//CompUnitsNode* sysy2eeyore_root;
void GetGlobalVariable();
void analyze();