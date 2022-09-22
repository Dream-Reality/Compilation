#include "../../sysy2eeyore/ast/node.h"
namespace ir{
enum DeclNum_t{
    Decl_One_t,Decl_Array_t
};
enum DeclType_t{
    Decl_Const_t,Decl_Variable_t
};

class Variable_int{
public:
    bool is_fix;
    int value;
    Variable_int(){
        is_fix = false;
        value = 0;
    }
    Variable_int(int x){
        is_fix = true;
        value = x;
    }
    Variable_int(const Variable_int&c){
        is_fix = c.is_fix;
        value = c.value;
    }
    void operator = (int x){
        is_fix = true;
        value = x;
    }
};
class Array{
public:
    Array(){}
    ~Array(){}
    int total;
    std::vector<int> dim;
    std::vector<Variable_int> value;
    void printInfo();
};
class Variable{
public:
    std::string name;
    DeclNum_t DeclNum;
    DeclType_t DeclType;
    union VariableValue{
        Variable_int* value;
        Array* array;
    }value;
    bool IsDelete;
    Variable(){
        IsDelete=false;
    }
    void printInfo();
};
class Param{
public:
    std::string name;
    DeclNum_t ParamNum;
    union ParamValue{
        int e;
        vector<int> *dim;
    }value;
    Param(string name_);
    Param(string name_,vector<ExpNode*> vec_);
};
class Global{
public:
    std::vector<Variable> GlobalVariable;
    int label_count;
};
class Instruct{
public:
    vector<string> instruct;
};
class Local{
    public:
    std::vector<Variable> LocalVariable;
    std::vector<Param> FuncParam;
};
int EvalConstIdentValue(IdentNode*ident);
int EvalConstIdentArrayValue(ArrayIdentNode*array);
int EvalConstLVal(LValNode*exp);
int EvalConstPrimaryExp(PrimaryExpNode*exp);
int EvalConstUnaryExp(UnaryExpNode*exp);
int EvalConst(ExpNode*exp);
void GlobalVariableArrayAssign(Array*array,Decl_ArrayValueNode*initarray);
void GlobalVariableAppend(DeclsNode*decl);
void GetGlobalVariable(CompUnitsNode*root);
void analyze_FuncDefNode(FuncDefNode*node);
void analyze(CompUnitsNode*root);
void generat_ir(CompUnitsNode*root);
void LocalVariableArrayAssign(Array*array,Decl_ArrayValueNode*initarray);
void analyze_DeclsNode(DeclsNode*node);
void analyze_StmtNode(StmtNode*node);
void analyze_BlockItemsNode(BlockItemsNode*node);
enum TagType_t{
    Primordial_t,Temporary_t,Param_t,Number_t
};

class Tag{
public:
    TagType_t TagType;
    union{
        pair<int,int>* primordial;
        int temporary;
        pair<int,int>* param;
        int number;
    }value;
    Tag(){}
    void print();
};
Tag Number2Tag(int x);
Tag analyze_ExpNode(ExpNode*exp);
Tag analyze_UnaryExpNode(UnaryExpNode*exp);
Tag analyze_PrimaryExpNode(PrimaryExpNode*exp);
enum InstructionType_t{
    OP_t,ASSIGN_t,PARAM_t,CALL_t,IFELSE_t,LABEL_t,GOTO_t,RETURN_t,FUNCTION_BEGIN,FUNCTION_END,STRING_t,VAR_T,ASSIGN_ARRAY_INDEX_t
};
void InsertInstruction(InstructionType_t type,Tag a,Tag b,Operation_t op,Tag c);
void InsertInstruction(InstructionType_t type,Tag a,Operation_t op,Tag b);
void InsertInstruction(InstructionType_t type,Tag a,Tag b);
void InsertInstruction(InstructionType_t type,Tag a,IdentNode*node);
void InsertInstruction(InstructionType_t type,IdentNode*node);
void InsertInstruction(InstructionType_t type,Tag a);
void InsertInstruction(InstructionType_t type,Tag a,int l);
void InsertInstruction(InstructionType_t type,int l);
void InsertInstruction(InstructionType_t type);
void InsertInstruction(InstructionType_t type,FuncDefNode*node);
void InsertInstruction(InstructionType_t type,const char* s);
void InsertInstruction(InstructionType_t type,int index,Variable v);
}

