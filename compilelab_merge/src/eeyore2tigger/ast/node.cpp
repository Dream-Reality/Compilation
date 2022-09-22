#include"../../eeyore2tigger/ast/node.h"
#include<string.h>
#include<iostream>
#include<sstream>
extern stringstream tigger_code;
#define DebugAnalyze 0
int LocalStackTop,NumberParam,Global_Symbol;
char temp[10000000],buf[10000000],FunctionBody[10000000],globalinit[10000000],tigger[10000000];
int lbuf = 0, lFunctionBody = 0,lglobalinit = 0,ltigger = 0;
map<string,Symbol*>name2symbol;
#define Debug_Delete_Node 1


void MyOut(const char* s1,int l1,char* s2,int&l2){
    if (s2!=tigger){
        for (int i=0;i<l1;i++)s2[i+l2]=s1[i];
        l2+=l1;s2[l2] = '\0';
    }
    else{
        tigger_code<<s1;
    }
}
ProgramsNode::ProgramsNode(){
    program = new vector<ProgramNode*>;
}
ProgramsNode::~ProgramsNode(){
    if (Debug_Delete_Node){
        printf("delete ProgramsNode\n");
    }
    for (vector<ProgramNode*>::iterator it = program->begin();it!=program->end();it++)
        delete *it;
    delete program;
}
void ProgramsNode::push_back(ProgramNode*program_){
    program->push_back(program_);
}
void ProgramsNode::printInfo(){
    printf("ProgramsNode: \n");
    for (vector<ProgramNode*>::iterator it = program->begin();it!=program->end();it++)
        (*it)->printInfo();
}
void ProgramsNode::analyze(){
    if (DebugAnalyze)printf("ProgramsNode\n");
    for (vector<ProgramNode*>::iterator it = program->begin();it!=program->end();it++){
        // (*it)->analyze();
        if ((*it)->ProgramType == Program_Declaration_t)(*it)->analyze();
    }
    for (vector<ProgramNode*>::iterator it = program->begin();it!=program->end();it++){
        // (*it)->analyze();
        if ((*it)->ProgramType == Program_Initialization_t)(*it)->analyze();
    }
    for (vector<ProgramNode*>::iterator it = program->begin();it!=program->end();it++){
        // (*it)->analyze();
        if ((*it)->ProgramType == Program_FunctionDef_t)(*it)->analyze();
    }
}

ProgramNode::ProgramNode(DeclarationNode* decl_){
    ProgramType = Program_Declaration_t;
    ProgramItem.decl = decl_;
}
ProgramNode::ProgramNode(InitializationNode* init_){
    ProgramType = Program_Initialization_t;
    ProgramItem.init = init_;
}
ProgramNode::ProgramNode(FunctionDefNode* func_){
    ProgramType = Program_FunctionDef_t;
    ProgramItem.func = func_;
}
ProgramNode::~ProgramNode(){
    switch (ProgramType){
        case Program_Declaration_t:delete ProgramItem.decl;break;
        case Program_Initialization_t:delete ProgramItem.init;break;
        case Program_FunctionDef_t:delete ProgramItem.func;break;
        default:printf("~ProgramNode: error\n");exit(0);
    }
}
void ProgramNode::printInfo(){
    printf("ProgramNode: (ProgramType: %d)\n",ProgramType);
    switch (ProgramType){
        case Program_Declaration_t:ProgramItem.decl->printInfo();break;
        case Program_Initialization_t:ProgramItem.init->printInfo();break;
        case Program_FunctionDef_t:ProgramItem.func->printInfo();break;
        default:printf("ProgramNode_printInfo: error\n");exit(0);
    }
}
void ProgramNode::analyze(){
    if (DebugAnalyze)printf("ProgramNode\n");
    switch (ProgramType){
        case Program_Declaration_t:ProgramItem.decl->analyze(LVAL_GLOBAL_t);break;
        case Program_Initialization_t:ProgramItem.init->analyze();break;
        case Program_FunctionDef_t:ProgramItem.func->analyze();break;
        default:printf("ProgramNode_analyze: error\n");exit(0);
    }
}

DeclarationNode::DeclarationNode(const char* s){
    symbol = s;
    ArrayLength = 0;
}
DeclarationNode::DeclarationNode(const char* s,int l){
    symbol = s;
    ArrayLength = l;
}
DeclarationNode::~DeclarationNode(){}
void DeclarationNode::printInfo(){
    printf("DeclarationNode: (symbol: %s) (length: %d)\n",symbol,ArrayLength);
}
void DeclarationNode::analyze(Type_t type){
    if (DebugAnalyze)printf("DeclarationNode\n");
    string name = (string)this->symbol;
    Symbol* s = new Symbol(name,type,(this->ArrayLength == 0)?One_t:Array_t);
    if (name2symbol.find(name)!=name2symbol.end()){
        delete name2symbol[name];
    }
    name2symbol[name] = s;
    if (type == LVAL_GLOBAL_t){
        if (ArrayLength)sprintf(temp,"%s = malloc %d\n",s->name.c_str(),ArrayLength);
        else sprintf(temp,"%s = 0\n",s->name.c_str());
        MyOut(temp,strlen(temp),tigger,ltigger);
    }
    else{
        s->stk = LocalStackTop;
        if (ArrayLength)LocalStackTop+=ArrayLength/4;
        else LocalStackTop++;
    }
}

InitializationNode::InitializationNode(const char* symbol_,int value_){
    symbol = symbol_;value = value_;ArrayIndex = 0;
}
InitializationNode::InitializationNode(const char* symbol_,int index,int value_){
    symbol = symbol_;value = value_;ArrayIndex = index;
}
InitializationNode::~InitializationNode(){}
void InitializationNode::analyze(){
    if (DebugAnalyze)printf("InitializationNode\n");
    sprintf(temp,"loadaddr %s s4\n",name2symbol[this->symbol]->name.c_str());
    MyOut(temp,strlen(temp),globalinit,lglobalinit);
    sprintf(temp,"s5 = %d\n",this->value);
    MyOut(temp,strlen(temp),globalinit,lglobalinit);
    sprintf(temp,"s4[%d] = s5\n",this->ArrayIndex);
    MyOut(temp,strlen(temp),globalinit,lglobalinit);
}
void InitializationNode::printInfo(){
    printf("InitializationNode: (symbol: %s) (ArrayIndex: %d) (value: %d)\n",symbol,ArrayIndex,ArrayIndex);
}
FunctionDefNode::FunctionDefNode(FunctionHeaderNode* FunctionHeader,StatementsNode*States){
    Header = FunctionHeader;
    Statement = States;
}
FunctionDefNode::~FunctionDefNode(){
    delete Header;
    delete Statement;
}
void FunctionDefNode::printInfo(){
    printf("FunctionDefNode: \n");
    Header->printInfo();
    Statement->printInfo();
}
void FunctionDefNode::analyze(){
    if (DebugAnalyze)printf("FunctionDefNode\n");
    Header->analyze();
    Statement->analyze();
    sprintf(temp,"%s [%d] [%d]\n",Header->func_name,Header->func_space,LocalStackTop);
    MyOut(temp,strlen(temp),tigger,ltigger);

    if (Header->func_name == (string)"f_main"){
        MyOut(globalinit,lglobalinit,tigger,ltigger);
    }

    MyOut(FunctionBody,lFunctionBody,tigger,ltigger);
    lFunctionBody = 0;

    sprintf(temp,"end %s\n",Header->func_name);
    MyOut(temp,strlen(temp),tigger,ltigger);
}
FunctionHeaderNode::FunctionHeaderNode(const char* name,int space){
    func_name = name; func_space = space;
}
FunctionHeaderNode::~FunctionHeaderNode(){}
void FunctionHeaderNode::printInfo(){
    printf("FunctionHeaderNode: (name: %s) (space: %d)\n",func_name,func_space);
}
void FunctionHeaderNode::analyze(){
    if (DebugAnalyze)printf("FunctionHeaderNode\n");
    for (int i = 0;i<func_space;i++){
        Symbol* s = new Symbol(((string)"p"+to_string(i)).c_str(),LVAL_PARAM_t,One_t);
        name2symbol[(string)"p"+to_string(i)] = s;
        s->stk = i;
        Store(s,i+12);
    }
    LocalStackTop = func_space;
}

StatementsNode::StatementsNode(){
    statement = new vector<StatementNode*>;
}
StatementsNode::~StatementsNode(){
    for (vector<StatementNode*>::iterator it = statement->begin();it!=statement->end();it++){
        delete (*it);
    }
    delete statement;
}
void StatementsNode::push_back(StatementNode* statement_){
    statement->push_back(statement_);
}
void StatementsNode::printInfo(){
    printf("StatementsNode: \n");
    for (vector<StatementNode*>::iterator it = statement->begin();it!=statement->end();it++){
        (*it)->printInfo();
    }
}
void StatementsNode::analyze(){
    if (DebugAnalyze)printf("StatementsNode\n");
    for (vector<StatementNode*>::iterator it = statement->begin();it!=statement->end();it++){
        if ((*it)->StatementType == State_Declaration_t)
            (*it)->analyze();
    }
    for (vector<StatementNode*>::iterator it = statement->begin();it!=statement->end();it++){
        if ((*it)->StatementType == State_Expression_t)
            (*it)->analyze();
    }
}

StatementNode::StatementNode(ExpressionNode* exp_){
    StatementType = State_Expression_t;
    StatementItem.exp = exp_;
}
StatementNode::StatementNode(DeclarationNode* decl_){
    StatementType = State_Declaration_t;
    StatementItem.decl = decl_;
}
StatementNode::~StatementNode(){
    switch (StatementType){
        case State_Expression_t:delete StatementItem.exp;break;
        case State_Declaration_t:delete StatementItem.decl;break;
        default:printf("~StatementNode: error\n");exit(0);
    }
}
void StatementNode::printInfo(){
    switch (StatementType){
        case State_Expression_t:StatementItem.exp->printInfo();break;
        case State_Declaration_t:StatementItem.decl->printInfo();break;
        default:printf("StatementNode_printInfo: error\n");exit(0);
    }
}
void StatementNode::analyze(){
    if (DebugAnalyze)printf("StatementNode\n");
    switch (StatementType){
        case State_Expression_t:StatementItem.exp->analyze();break;
        case State_Declaration_t:StatementItem.decl->analyze(LVAL_LOCAL_t);break;
        default:printf("StatementNode_analyze: error\n");exit(0);
    }
}

ExpressionNode::ExpressionNode(AssignExpNode* item){
    ExpressionType = AssignExp_t;
    ExpressionItem.ae = item;
}
ExpressionNode::ExpressionNode(AssignUnaryExpNode* item){
    ExpressionType = AssignUnaryExp_t;
    ExpressionItem.aue = item;
}
ExpressionNode::ExpressionNode(AssignNode* item){
    ExpressionType = Assign_t;
    ExpressionItem.a = item;
}
ExpressionNode::ExpressionNode(AssignArrayNode* item){
    ExpressionType = AssignArray_t;
    ExpressionItem.aa = item;
}
ExpressionNode::ExpressionNode(ArrayAssignNode* item){
    ExpressionType = ArrayAssign_t;
    ExpressionItem.aa_ = item;
}
ExpressionNode::ExpressionNode(IfNode* item){
    ExpressionType = If_t;
    ExpressionItem.i = item;
}
ExpressionNode::ExpressionNode(GotoNode* item){
    ExpressionType = GOTO_t;
    ExpressionItem.gt = item;
}
ExpressionNode::ExpressionNode(const char* item){
    ExpressionType = LABEL_t;
    ExpressionItem.l = item;
}
ExpressionNode::ExpressionNode(ParamNode* item){
    ExpressionType = PARAM_t;
    ExpressionItem.p = item;
}
ExpressionNode::ExpressionNode(CallNode* item){
    ExpressionType = CALL_t;
    ExpressionItem.c = item;
}
ExpressionNode::ExpressionNode(CallReturnNode* item){
    ExpressionType = CALLRETURN_t;
    ExpressionItem.cr = item;
}
ExpressionNode::ExpressionNode(ReturnValueNode* item){
    ExpressionType = RETURNVALUE_t;
    ExpressionItem.rv = item;
}
ExpressionNode::ExpressionNode(ReturnNode* item){
    ExpressionType = RETURN_t;
    ExpressionItem.r = item;
}
ExpressionNode::~ExpressionNode(){
    switch (ExpressionType){
        case AssignExp_t:delete ExpressionItem.ae;break;
        case AssignUnaryExp_t:delete ExpressionItem.aue;break;
        case Assign_t:delete ExpressionItem.a;break;
        case AssignArray_t:delete ExpressionItem.aa;break;
        case ArrayAssign_t:delete ExpressionItem.aa_;break;
        case If_t:delete ExpressionItem.i;break;
        case GOTO_t:delete ExpressionItem.gt;break;
        case LABEL_t:break;
        case PARAM_t:delete ExpressionItem.p;break;
        case CALL_t:delete ExpressionItem.c;break;
        case CALLRETURN_t:delete ExpressionItem.cr;break;
        case RETURNVALUE_t:delete ExpressionItem.rv;break;
        case RETURN_t:delete ExpressionItem.r;break;
        default:printf("~ExpressionNode: error\n");exit(0);
    }
}
void ExpressionNode::printInfo(){
    printf("ExpressionNode: (ExpressionType: %d)\n",ExpressionType);
    switch (ExpressionType){
        case AssignExp_t:ExpressionItem.ae->printInfo();break;
        case AssignUnaryExp_t:ExpressionItem.aue->printInfo();break;
        case Assign_t:ExpressionItem.a->printInfo();break;
        case AssignArray_t:ExpressionItem.aa->printInfo();break;
        case ArrayAssign_t:ExpressionItem.aa_->printInfo();break;
        case If_t:ExpressionItem.i->printInfo();break;
        case GOTO_t:ExpressionItem.gt->printInfo();break;
        case LABEL_t:printf(temp,"Label: (label:\n",ExpressionItem.l);break;
        case PARAM_t:ExpressionItem.p->printInfo();break;
        case CALL_t:ExpressionItem.c->printInfo();break;
        case CALLRETURN_t:ExpressionItem.cr->printInfo();break;
        case RETURNVALUE_t:ExpressionItem.rv->printInfo();break;
        case RETURN_t:ExpressionItem.r->printInfo();break;
        default:printf("ExpressionNode_printInfo: error\n");exit(0);
    }
}
void ExpressionNode::analyze(){
    if (DebugAnalyze)printf("ExpressionNode\n");
    switch (ExpressionType){
        case AssignExp_t:ExpressionItem.ae->analyze();break;
        case AssignUnaryExp_t:ExpressionItem.aue->analyze();break;
        case Assign_t:ExpressionItem.a->analyze();break;
        case AssignArray_t:ExpressionItem.aa->analyze();break;
        case ArrayAssign_t:ExpressionItem.aa_->analyze();break;
        case If_t:ExpressionItem.i->analyze();break;
        case GOTO_t:ExpressionItem.gt->analyze();break;
        case LABEL_t:sprintf(temp,"%s:\n",ExpressionItem.l);MyOut(temp,strlen(temp),FunctionBody,lFunctionBody);break;
        case PARAM_t:ExpressionItem.p->analyze();break;
        case CALL_t:ExpressionItem.c->analyze();break;
        case CALLRETURN_t:ExpressionItem.cr->analyze();break;
        case RETURNVALUE_t:ExpressionItem.rv->analyze();break;
        case RETURN_t:ExpressionItem.r->analyze();break;
        default:printf("ExpressionNode_analyze: error\n");exit(0);
    }
}

RightValueNode::RightValueNode(const char* s){
    RightValueType = Symbol_t;
    RightValueItem.symbol = s;
}
RightValueNode::RightValueNode(int number){
    RightValueType = Number_t;
    RightValueItem.number = number;
}
RightValueNode::~RightValueNode(){}
void RightValueNode::printInfo(){
    printf("RightValueNode: (RightValueType %d)",RightValueType);
    switch (RightValueType){
        case Symbol_t:printf("(symbol: %s)\n",RightValueItem.symbol);break;
        case Number_t:printf("(number: %d)\n",RightValueItem.number);break;
        default:printf("RightValueNode_printInfo: error\n");exit(0);
    }
}
Symbol* RightValueNode::analyze(){
    if (DebugAnalyze)printf("RightValueNode\n");
    if (RightValueType == Number_t){
        return new Symbol(to_string(RightValueItem.number),LVAL_NUMBER_t,One_t);
    }else return name2symbol[(string)RightValueItem.symbol];
}

OpNode::OpNode(OpNodeType t){
    OpType = t;
}
OpNode::~OpNode(){}
void OpNode::printInfo(){
    printf("OpNode: (type: %d)\n",OpType);
}
const char* OpNode::analyze(){
    if (DebugAnalyze)printf("OpNode\n");
    switch (OpType){
        case LE_t:return "<=";
        case LT_t:return "<";
        case GE_t:return ">=";
        case GT_t:return ">";
        case NE_t:return "!=";
        case EQ_t:return "==";
        case AND_t:return "&&";
        case OR_t:return "||";
        case PLUS_t:return "+";
        case SUB_t:return "-";
        case MUL_t:return "*";
        case DIV_t:return "/";
        case REM_t:return "%";
        case NOT_t:return "!";
        default:printf("OpNode_analyze: error\n");exit(0);
    }
}

AssignExpNode::AssignExpNode(const char*symbol,RightValueNode* v1,RightValueNode* v2,OpNode* op_){
    LVal = symbol;
    value1 = v1;
    value2 = v2;
    op = op_;
}
AssignExpNode::~AssignExpNode(){
    delete value1;
    delete value2;
    delete op;
}
void AssignExpNode::printInfo(){
    printf("AssignExpNode: (name: %s)\n",LVal);
    value1->printInfo();
    value2->printInfo();
    op->printInfo();
}
void AssignExpNode::analyze(){
    if (DebugAnalyze)printf("AssignExpNode\n");
    Symbol* v1 = value1->analyze();
    Symbol* v2 = value2->analyze();
    Load(v1,0);
    Load(v2,1);
    sprintf(temp,"s2 = s0 %s s1\n",this->op->analyze());
    MyOut(temp,strlen(temp),FunctionBody,lFunctionBody);
    Store(name2symbol[LVal],2);
    sprintf(temp,"\n");MyOut(temp,strlen(temp),FunctionBody,lFunctionBody);
}

AssignUnaryExpNode::AssignUnaryExpNode(const char*name,RightValueNode* v,OpNode* op_){
    LVal = name;value = v;op = op_;
}
AssignUnaryExpNode::~AssignUnaryExpNode(){
    delete value;
    delete op;
}
void AssignUnaryExpNode::printInfo(){
    printf("AssignUnaryExpNode: (name: %s)\n",LVal);
    value->printInfo();
    op->printInfo();
}
void AssignUnaryExpNode::analyze(){
    if (DebugAnalyze)printf("AssignUnaryExpNode\n");
    Symbol* v1 = value->analyze();
    Load(v1,0);
    sprintf(temp,"s2 = %s s0\n",this->op->analyze());
    MyOut(temp,strlen(temp),FunctionBody,lFunctionBody);
    Store(name2symbol[LVal],2);
}

AssignNode::AssignNode(const char*name,RightValueNode*v){
    LVal = name;value = v;
}
AssignNode::~AssignNode(){
    delete value;
}
void AssignNode::printInfo(){
    printf("AssignNode: (name: %s)\n",LVal);
    value->printInfo();
}
void AssignNode::analyze(){
    if (DebugAnalyze)printf("AssignNode\n");
    Symbol* v1 = value->analyze();
    Load(v1,0);
    Store(name2symbol[LVal],0);
    sprintf(temp,"\n");MyOut(temp,strlen(temp),FunctionBody,lFunctionBody);
}

AssignArrayNode::AssignArrayNode(const char* name,RightValueNode*v1,RightValueNode*v2){
    LVal = name;
    value1 = v1;
    value2 = v2;
}
AssignArrayNode::~AssignArrayNode(){
    delete value1;
    delete value2;
}
void AssignArrayNode::printInfo(){
    printf("AssignArrayNode: (name: %s)\n",LVal);
    value1->printInfo();value2->printInfo();
}
void AssignArrayNode::analyze(){
    if (DebugAnalyze)printf("AssignArrayNode\n");
    Symbol* v1 = value1->analyze();
    Symbol* v2 = value2->analyze();
    Load(v1,0);
    Load(v2,1);
    // std::cerr << v1->name << " " << v2->name<< std::endl;
    Symbol* res = name2symbol[LVal];
    switch (res->type){
        case LVAL_GLOBAL_t:sprintf(temp,"loadaddr %s s2\n",res->name.c_str());MyOut(temp,strlen(temp),FunctionBody,lFunctionBody);break;
        case LVAL_LOCAL_t:{
            if (res->TypeNum == One_t)
                sprintf(temp,"load %d s2\n",res->stk);
            else sprintf(temp,"loadaddr %d s2\n",res->stk);    
            MyOut(temp,strlen(temp),FunctionBody,lFunctionBody);
            break;
        }
        case LVAL_PARAM_t:sprintf(temp,"load %d s2\n",res->stk);MyOut(temp,strlen(temp),FunctionBody,lFunctionBody);break;
        default:printf("AssignArrayNode_analyze: error\n");
    }
    sprintf(temp,"s2 = s2 + s0\ns2[0] = s1\n");MyOut(temp,strlen(temp),FunctionBody,lFunctionBody);
    sprintf(temp,"\n");MyOut(temp,strlen(temp),FunctionBody,lFunctionBody);
}

ArrayAssignNode::ArrayAssignNode(const char*name1,const char*name2,RightValueNode*v){
    LVal1 = name1;
    LVal2 = name2;
    value = v;
}
ArrayAssignNode::~ArrayAssignNode(){
    delete value;
}
void ArrayAssignNode::printInfo(){
    printf("ArrayAssignNode: (name1: %s) (name2: %s)\n",LVal1,LVal2);
    value->printInfo();
}
void ArrayAssignNode::analyze(){
    if (DebugAnalyze)printf("ArrayAssignNode\n");
    Symbol* v = value->analyze();
    Load(v,0);
    Symbol* res = name2symbol[LVal2];
    Symbol* assignres = name2symbol[LVal1];
    // std::cerr << v->name << " " << res->name<< std::endl;
    switch (res->type){
        case LVAL_GLOBAL_t:sprintf(temp,"loadaddr %s s1\n",res->name.c_str());MyOut(temp,strlen(temp),FunctionBody,lFunctionBody);break;
        case LVAL_LOCAL_t:{
            if (res->TypeNum == One_t)
                sprintf(temp,"load %d s1\n",res->stk);
            else sprintf(temp,"loadaddr %d s1\n",res->stk);   
            //sprintf(temp,"load %d s1\n",res->stk);
            MyOut(temp,strlen(temp),FunctionBody,lFunctionBody);break;
        }
        case LVAL_PARAM_t:sprintf(temp,"load %d s1\n",res->stk);MyOut(temp,strlen(temp),FunctionBody,lFunctionBody);break;
        default:printf("ArrayAssignNode_analyze: error\n");exit(0);
    }
    sprintf(temp,"s1 = s1 + s0\ns2 = s1[0]\n");
    MyOut(temp,strlen(temp),FunctionBody,lFunctionBody);
    Store(assignres,2);
    sprintf(temp,"\n");MyOut(temp,strlen(temp),FunctionBody,lFunctionBody);
}

IfNode::IfNode(RightValueNode*v1,RightValueNode*v2,OpNode*op_,const char*l){
    value1 = v1;value2 = v2;
    op = op_; label = l;
}
IfNode::~IfNode(){
    delete value1;
    delete value2;
    delete op;
}
void IfNode::printInfo(){
    printf("IfNode: (label %s)\n",label);
    value1->printInfo();
    value2->printInfo();
    op->printInfo();
}
void IfNode::analyze(){
    if (DebugAnalyze)printf("IfNode\n");
    Symbol* v1 = value1->analyze();
    Symbol* v2 = value2->analyze();
    Load(v1,0);
    Load(v2,1);
    sprintf(temp,"if s0 %s s1 goto %s\n",op->analyze(),label);
    MyOut(temp,strlen(temp),FunctionBody,lFunctionBody);
}

GotoNode::GotoNode(const char*l){
    label = l;
}
GotoNode::~GotoNode(){}
void GotoNode::printInfo(){
    printf("GotoNode: (label %s)\n",label);
}
void GotoNode::analyze(){
    if (DebugAnalyze)printf("GotoNode\n");
    sprintf(temp,"goto %s\n",label);
    MyOut(temp,strlen(temp),FunctionBody,lFunctionBody);
}

ParamNode::ParamNode(RightValueNode* param_){
    param = param_;
}
ParamNode::~ParamNode(){
    delete param;
}
void ParamNode::printInfo(){
    printf("ParamNode: \n");
    param->printInfo();
}
void ParamNode::analyze(){
    if (DebugAnalyze)printf("ParamNode\n");
    Symbol* v = param->analyze();
    Load(v, 12+NumberParam);
    NumberParam++;
}

CallNode::CallNode(const char* func_){func = func_;}
CallNode::~CallNode(){}
void CallNode::printInfo(){printf("CallNode (func %s)\n",func);}
void CallNode::analyze(){
    if (DebugAnalyze)printf("CallNode\n");
    NumberParam=0;
    sprintf(temp,"call %s\n",func);
    MyOut(temp,strlen(temp),FunctionBody,lFunctionBody);
}

CallReturnNode::CallReturnNode(const char* name,const char* func_){
    LVal = name; func = func_;
}
CallReturnNode::~CallReturnNode(){}
void CallReturnNode::printInfo(){
    printf("CallReturnNode: (name: %s) (func: %s)\n",LVal,func);
}
void CallReturnNode::analyze(){
    if (DebugAnalyze)printf("CallReturnNode\n");
    NumberParam=0;
    sprintf(temp,"call %s\n",func);
    MyOut(temp,strlen(temp),FunctionBody,lFunctionBody);
    Store(name2symbol[LVal],12);
}


ReturnValueNode::ReturnValueNode(RightValueNode* rv){
    ReturnValue = rv;
}
ReturnValueNode::~ReturnValueNode(){
    delete ReturnValue;
}
void ReturnValueNode::printInfo(){
    printf("ReturnValueNode: \n");
    ReturnValue->printInfo();
}
void ReturnValueNode::analyze(){
    if (DebugAnalyze)printf("ReturnValueNode\n");
    Symbol* rv = ReturnValue->analyze();
    Load(rv,12);
    sprintf(temp,"return\n");
    MyOut(temp,strlen(temp),FunctionBody,lFunctionBody);
}

ReturnNode::ReturnNode(){}
ReturnNode::~ReturnNode(){}
void ReturnNode::printInfo(){
    printf("ReturnNode \n");
}
void ReturnNode::analyze(){
    if (DebugAnalyze)printf("ReturnNode\n");
    sprintf(temp,"return\n");
    MyOut(temp,strlen(temp),FunctionBody,lFunctionBody);
}

void Load(Symbol* s,int reg){
    char regtype = (reg<12)?'s':'a';reg %= 12;
    switch (s->type){
        case LVAL_GLOBAL_t:{
            if (s->TypeNum == One_t)sprintf(temp,"load %s %c%d\n",s->name.c_str(),regtype,reg);
            else sprintf(temp,"loadaddr %s %c%d\n",s->name.c_str(),regtype,reg);
            break;
        }
        case LVAL_LOCAL_t:{
            if (s->TypeNum == One_t){
                if (s->stk!=-1)
                    sprintf(temp,"load %d %c%d\n",s->stk,regtype,reg);
                else {
                    sprintf(temp,"%c%d = 0",regtype,reg);
                    printf("Load: error\n");
                    exit(0);
                }
            }
            else sprintf(temp,"loadaddr %d %c%d\n",s->stk,regtype,reg);
            break;
        }
        case LVAL_PARAM_t:{
            sprintf(temp,"load %d %c%d\n",s->stk,regtype,reg);break;
        }
        case LVAL_NUMBER_t:{
            sprintf(temp,"%c%d = %s\n",regtype,reg,s->name.c_str());break;
        }
        default:printf("Load: error\n");exit(0);
    }
    MyOut(temp,strlen(temp),FunctionBody,lFunctionBody);
}

void Store(Symbol* s,int reg){
    char regtype = (reg<12)?'s':'a';reg %= 12;
    switch (s->type){
        case LVAL_GLOBAL_t:sprintf(temp,"loadaddr %s s3\ns3[0] = %c%d\n",s->name.c_str(),regtype,reg);break;
        case LVAL_LOCAL_t:sprintf(temp,"store %c%d %d\n",regtype,reg,s->stk);break;
        case LVAL_PARAM_t:sprintf(temp,"store %c%d %d\n",regtype,reg,s->stk);break;
        default:printf("Store: error\n");exit(0);
    }
    MyOut(temp,strlen(temp),FunctionBody,lFunctionBody);
}

Symbol::Symbol(string name_,Type_t type_,TypeNum_t TypeNum_){
    type = type_;
    name = (type == LVAL_GLOBAL_t)?("v"+to_string(Global_Symbol++)):name_;
    TypeNum = TypeNum_;
    stk = -1;
}