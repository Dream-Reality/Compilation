#include<vector>
#include<string>
#include<map>
using namespace std;

class BaseNode;
class ProgramsNode;
class ProgramNode;
class DeclarationNode;
class InitializationNode;
class FunctionDefNode;
class FunctionHeaderNode;
class StatementsNode;
class StatementNode;
class ExpressionNode;
class RightValueNode;
class OpNode;

class AssignExpNode;
class AssignUnaryExpNode;
class AssignNode;
class ArrayAssignNode;
class AssignArrayNode;
class IfNode;
class GotoNode;
class ParamNode;
class CallNode;
class CallReturnNode;
class ReturnValueNode;
class ReturnNode;
class Symbol;

class BaseNode{
public:
};
class ProgramsNode{
public:
    vector<ProgramNode*> *program;
    ProgramsNode();
    ~ProgramsNode();
    void push_back(ProgramNode* program);
    void printInfo();
    void analyze();
};
enum ProgramNodeType{
    Program_Declaration_t,Program_Initialization_t,Program_FunctionDef_t
};
class ProgramNode:public BaseNode{
public:
    enum ProgramNodeType ProgramType;
    union {
        DeclarationNode* decl;
        InitializationNode* init;
        FunctionDefNode* func;
    }ProgramItem;
    ProgramNode(DeclarationNode* decl_);
    ProgramNode(InitializationNode* init_);
    ProgramNode(FunctionDefNode* func);
    ~ProgramNode();
    void printInfo();
    void analyze();
};
enum Type_t{
    LVAL_GLOBAL_t,LVAL_LOCAL_t,LVAL_PARAM_t,LVAL_NUMBER_t
};
class DeclarationNode:public BaseNode{
public:
    const char* symbol;
    int ArrayLength;
    DeclarationNode(const char* s);
    DeclarationNode(const char* s,int l);
    ~DeclarationNode();
    void printInfo();
    void analyze(Type_t type);
};
class InitializationNode:public BaseNode{
public:
    const char* symbol;
    int ArrayIndex,value;
    InitializationNode(const char* symbol,int value);
    InitializationNode(const char* symbol,int index,int value);
    ~InitializationNode();
    void printInfo();
    void analyze();
};
class FunctionDefNode:public BaseNode{
public:
    FunctionHeaderNode* Header;
    StatementsNode* Statement;
    FunctionDefNode(FunctionHeaderNode* FunctionHeader,StatementsNode*States);
    ~FunctionDefNode();
    void printInfo();
    void analyze();
};
class FunctionHeaderNode:public BaseNode{
public:
    const char* func_name;
    int func_space;
    FunctionHeaderNode(const char* name,int space);
    ~FunctionHeaderNode();
    void printInfo();
    void analyze();
};
class StatementsNode:public BaseNode{
public:
    vector<StatementNode*> *statement;
    StatementsNode();
    void push_back(StatementNode* statement);
    ~StatementsNode();
    void printInfo();
    void analyze();
};
enum StatementNodeType{
    State_Expression_t,State_Declaration_t
};
class StatementNode:public BaseNode{
public:
    enum StatementNodeType StatementType;
    union{
        ExpressionNode *exp;
        DeclarationNode *decl;
    }StatementItem;
    StatementNode(ExpressionNode* exp_);
    StatementNode(DeclarationNode* decl);
    ~StatementNode();
    void printInfo();
    void analyze();
};
enum ExpressionNodeType{
    AssignExp_t,AssignUnaryExp_t,Assign_t,AssignArray_t,ArrayAssign_t,
    If_t,GOTO_t,LABEL_t,
    PARAM_t,CALL_t,CALLRETURN_t,RETURNVALUE_t,RETURN_t
};
class ExpressionNode:public BaseNode{
public:
    enum ExpressionNodeType ExpressionType;
    union{
        AssignExpNode* ae;
        AssignUnaryExpNode* aue;
        AssignNode* a;
        AssignArrayNode* aa;      // 1[2] = 3;
        ArrayAssignNode* aa_;
        IfNode* i;
        GotoNode* gt;
        const char* l;
        ParamNode* p;
        CallNode* c;
        CallReturnNode* cr;
        ReturnValueNode* rv;
        ReturnNode* r;
    }ExpressionItem;
    ExpressionNode(AssignExpNode* ae);
    ExpressionNode(AssignUnaryExpNode* aue);
    ExpressionNode(AssignNode* a);
    ExpressionNode(AssignArrayNode* aa);
    ExpressionNode(ArrayAssignNode* aa_);
    ExpressionNode(IfNode* i);
    ExpressionNode(GotoNode* gt);
    ExpressionNode(const char* l);
    ExpressionNode(ParamNode* p);
    ExpressionNode(CallNode* c);
    ExpressionNode(CallReturnNode* cr);
    ExpressionNode(ReturnValueNode* rv);
    ExpressionNode(ReturnNode* r);
    ~ExpressionNode();
    void printInfo();
    void analyze();
};
enum RightValueNodeType{
    Symbol_t,Number_t
};
class RightValueNode:public BaseNode{
public:
    enum RightValueNodeType RightValueType;
    union{
        const char* symbol;
        int number;
    }RightValueItem;
    RightValueNode(const char* s);
    RightValueNode(int number);
    ~RightValueNode();
    void printInfo();
    Symbol* analyze();
};
enum OpNodeType{
    LE_t,LT_t,GE_t,GT_t,NE_t,EQ_t,AND_t,OR_t,PLUS_t,SUB_t,MUL_t,DIV_t,REM_t,NOT_t
};
class OpNode:public BaseNode{
public:
    OpNodeType OpType;
    OpNode(OpNodeType t);
    ~OpNode();
    void printInfo();
    const char* analyze();
};
class AssignExpNode:public BaseNode{
public:
    const char* LVal;
    RightValueNode* value1,* value2;
    OpNode* op;
    AssignExpNode(const char*LVal,RightValueNode* v1,RightValueNode* v2,OpNode* op);
    ~AssignExpNode();
    void printInfo();
    void analyze();
};
class AssignUnaryExpNode:public BaseNode{
public:
    const char* LVal;
    RightValueNode* value;
    OpNode* op;
    AssignUnaryExpNode(const char*LVal,RightValueNode* v,OpNode* op);
    ~AssignUnaryExpNode();
    void printInfo();
    void analyze();
};
class AssignNode:public BaseNode{
public:
    const char* LVal;
    RightValueNode* value;
    AssignNode(const char* LVal,RightValueNode* value);
    ~AssignNode();
    void printInfo();
    void analyze();
};
class AssignArrayNode:public BaseNode{
public:
    const char* LVal;
    RightValueNode* value1,*value2;
    AssignArrayNode(const char* LVal,RightValueNode* v1,RightValueNode* v2);
    ~AssignArrayNode();
    void printInfo();
    void analyze();
};
class ArrayAssignNode:public BaseNode{
public:
    const char* LVal1,*LVal2;
    RightValueNode* value;
    ArrayAssignNode(const char* LVal1,const char* LVal2,RightValueNode* v);
    ~ArrayAssignNode();
    void printInfo();
    void analyze();
};
class IfNode:public BaseNode{
public:
    RightValueNode* value1,* value2;
    OpNode* op;
    const char* label;
    IfNode(RightValueNode*v1,RightValueNode* v2,OpNode* op, const char* l);
    ~IfNode();
    void printInfo();
    void analyze();
};
class GotoNode:public BaseNode{
public:
    const char* label;
    GotoNode(const char* l);
    ~GotoNode();
    void printInfo();
    void analyze();
};
class ParamNode:public BaseNode{
public:
    RightValueNode* param;
    ParamNode(RightValueNode* param);
    ~ParamNode();
    void printInfo();
    void analyze();
};
class CallNode:public BaseNode{
public:
    const char* func;
    CallNode(const char* func_);
    ~CallNode();
    void printInfo();
    void analyze();
};
class CallReturnNode:public BaseNode{
public:
    const char* LVal,*func;
    CallReturnNode(const char* LVal,const char* func_);
    ~CallReturnNode();
    void printInfo();
    void analyze();
};
class ReturnValueNode:public BaseNode{
public:
    RightValueNode* ReturnValue;
    ReturnValueNode(RightValueNode* ReturnValue);
    ~ReturnValueNode();
    void printInfo();
    void analyze();
};
class ReturnNode:public BaseNode{
public: 
    ReturnNode();
    ~ReturnNode();
    void printInfo();
    void analyze();
};
enum TypeNum_t{
    One_t,Array_t
};

class Symbol{
public:
    string name;
    enum Type_t type;
    enum TypeNum_t TypeNum;
    int stk;
    Symbol (string name_,Type_t type_,TypeNum_t TypeNum_);
    void printInfo(){
        printf("Symbol: (name %s) (Type: %d) (TypeNum %d) (Stack %d)\n",name.c_str(),type,TypeNum,stk);
    }
};

void Store(Symbol* s,int reg);
void Load(Symbol* s,int reg);
void MyOut(const char*s1,int l1,char* s2,int&l2);