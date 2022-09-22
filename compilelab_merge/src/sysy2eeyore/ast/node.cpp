#include "../../sysy2eeyore/ast/node.h"
//#include "../ir/ir.h"
void IdentNode::printInfo(){
    printf("Node: IdentNode:");
    printf("(name: %s)\n",name->c_str());
}
void CompUnitsNode::printInfo(){
    printf("Node CompUnitsNode\n");
    for (vector<CompUnitNode*>::iterator it = list.begin();it!=list.end();it++)
        (*it)->printInfo();
}
void CompUnitNode::printInfo(){
    printf("Node CompUnitNode:");
    if (UnitType == Decl_t){printf("(UnitType: Decl_t)\n");UnitItem.Decls->printInfo();}
    else {printf("(UnitType: FuncDef_t)\n");UnitItem.FuncDef->printInfo();}
}
void DeclsNode::printInfo(){
    printf("DeclsNode:");
    if (DeclType == Const_t)printf("(DeclType: Const_t)");
    else printf("(DeclType_t): Variable_t");
    if (ValueType == INT_t)printf("(ValueType: INT_t)\n");
    for (vector<DeclNode*>::iterator it = list.begin(); it!=list.end();it++)
        (*it)->printInfo();
}
void DeclNode::printInfo(){
    printf("DeclNode:");
    if (DeclNumType == One_t){
        printf("(DeclNumType: One_t)\n");
        DeclItem.Decl_One->printInfo();
    }
    else {
        printf("(DeclNumType: Array_t)\n");
        DeclItem.Decl_Array->printInfo();
    }
}
void Decl_OneNode::printInfo(){
    printf("Decl_OneNode:");
    if (DeclType == Const_t)printf("(DeclType: Const_t)\n");else printf("(DeclType: Variable_t)\n");
    ident->printInfo();
    if (exp!=nullptr)exp->printInfo();
}
void Decl_ArrayNameNode::printInfo(){
    printf("Decl_ArrayNameNode:");
    Ident->printInfo();
    for (vector<ExpNode*>::iterator it = dim.begin();it!=dim.end(); it++)
        (*it)->printInfo();
}
void Decl_ArrayNode::printInfo(){
    printf("Decl_ArrayNode:");
    if (DeclType == Const_t)printf("(DeclType: Const_t)\n");else printf("(DeclType: Variable_t)\n");
    name->printInfo();
    if (value != nullptr)value->printInfo();
}
void Decl_ArrayValueNode::printInfo(){
    printf("Decl_ArrayValueNode:");
    printf("(ir_dep: %d)",ir_dep);
    if (ItemType == Item_Exp_t){
        printf("(ItemType: Item_Exp_t)\n");
        exp->printInfo();
    }
    else{
        printf("(ItemType: Item_Array_t)\n");
        for (vector<Decl_ArrayValueNode*>::iterator it = vec.begin(); it!=vec.end(); it++)
            (*it)->printInfo();
    }
}
void FuncDefNode::printInfo(){
    printf("FuncDefNode:");
    if (ReturnValue == RINT_t)printf("(ReturnValue: RINT_t)\n");else printf("(ReturnValue: RVOID_t)\n");
    Ident->printInfo();
    if (Params!=nullptr)Params->printInfo();
    Block->printInfo();
}
void ExpNode::printInfo(){
    printf("ExpNode:");
    if (ExpType == Exp_op_t){
        printf("(ExpType: Exp_op_t)\n");
        args.ExpArgs->second->printInfo();
        args.ExpArgs->first.first->printInfo();
        args.ExpArgs->first.second->printInfo();
    }else {
        printf("(ExpType: Exp_unary_t)\n");
        args.UnaryExpArgs->printInfo();
    }
}    
void UnaryExpNode::printInfo(){
    printf("UnaryExpNode:");
    if (UnaryExpType == Unary_Primary_t){
        printf("(UnaryExpType: Unary_Primary_t)\n");
        node.PrimaryExp->printInfo();
    }
    else if (UnaryExpType == Unary_FuncCall_t){
        printf("(UnaryExpType: Unary_FuncCall_t)\n");
        node.FunctionCall->printInfo();
    }
    else if (UnaryExpType == Unary_Exp_t){
        printf("(UnaryExpType: Unary_Exp_t)\n");
        node.UnaryExp->first->printInfo();
        node.UnaryExp->second->printInfo();
    }
}
    void PrimaryExpNode::printInfo(){
    if (PrimaryExpType == Primary_LVal_t){
        printf("(PrimaryExpType: Primary_LVal_t)\n");
        node.LVal->printInfo();
    }
    else if (PrimaryExpType == Primary_Number_t){
        printf("(PrimaryExpType: Primary_Number_t)\n");
        node.Number->printInfo();
    }
    else if (PrimaryExpType == Primary_Exp_t){
        printf("(PrimaryExpType: Primary_Exp_t)\n");
        node.Exp->printInfo();
    }
}
void FunctionCallNode::printInfo(){
    printf("FunctionCallNode:\n");
    Ident->printInfo();
    if (Param != nullptr)Param->printInfo();
}
void FuncRParamsNode::printInfo(){
    printf("FuncRParamsNode:\n");
    for (vector<ExpNode*>::iterator it = list.begin(); it !=list.end(); it++ )
        (*it)->printInfo();
}
void LValNode::printInfo(){
    printf("LValNode:");
    if (LValType == LVal_Ident_t){
        printf("(LValType: LVal_Ident_t)\n");
        node.Ident->printInfo();
    }
    else{
        printf("(LValType: LVal_ArrayIdent_t)\n");
        node.ArrayIdent->printInfo();
    }
}
void ArrayIdentNode::printInfo(){
    printf("ArrayIdentNode:\n");
    Ident->printInfo();
    for (vector<ExpNode*>::iterator it = dim.begin(); it != dim.end(); it++)
        (*it)->printInfo();
}
void FuncFParamsNode::printInfo(){
    printf("FuncFParamsNode:\n");
    for (vector<FuncFParamNode*>::iterator it = list.begin(); it != list.end(); it++)
        (*it)->printInfo();
}
void FuncFParamNode::printInfo(){
    printf("FuncFParamType_t:");
    if (FuncFParamType == FuncFParamParam_t){
        printf("(FuncFParamType: FuncFParamParam_t)\n");
        node.param->printInfo();
    }
    else{
        printf("(FuncFParamType: FuncFParamArray_t)\n");
        node.array->printInfo();
    }
}    
void FuncFParamParam_Node::printInfo(){
    printf("FuncFParamParam_Node:\n");
    Type->printInfo();
    Ident->printInfo();
}
void FuncFParamArray_Node::printInfo(){
    printf("FuncFParamArray_Node:\n");
    Type->printInfo();
    Ident->printInfo();
    for (vector<ExpNode*>::iterator it = dim.begin(); it != dim.end(); it++)
        (*it)->printInfo();
}
void BlockItemsNode::printInfo(){
    printf("BlockItemsNode:\n");
    for (vector<BlockItemNode*>::iterator it = list.begin(); it != list.end(); it++)
        (*it)->printInfo();
}
void BlockItemNode::printInfo(){
    printf("BlockItemNode:");
    if (BlockItemType == Block_Decl_t){
        printf("(BlockItemType: Block_Decl_t)\n");
        node.Decl->printInfo();
    }
    else{
        printf("(BlockItemType: Block_Stmt_t)\n");
        node.Stmt->printInfo();
    }
}
void StmtNode::printInfo(){
    printf("StmtNode:");
    switch (StmtType){
        case Stmt_Assign_t:printf("(StmtType: Stmt_Assign_t)\n");node.AssignStmt->printInfo();break;
        case Stmt_Empty_t:printf("(StmtType: Stmt_Empty_t)\n");node.EmptyStmt->printInfo();break;
        case Stmt_Exp_t:printf("(StmtType: Stmt_Exp_t)\n");node.ExpStmt->printInfo();break;
        case Stmt_While_t:printf("(StmtType: Stmt_While_t)\n");node.WhileStmt->printInfo();break;
        case Stmt_Break_t:printf("(StmtType: Stmt_Break_t)\n");node.BreakStmt->printInfo();break;
        case Stmt_Continue_t:printf("(StmtType: Stmt_Continue_t)\n");node.ContinueStmt->printInfo();break;
        case Stmt_Return_t:printf("(StmtType: Stmt_Return_t)\n");node.ReturnStmt->printInfo();break;
        case Stmt_Block_t:printf("(StmtType: Stmt_Block_t)\n");node.BlockItems->printInfo();break;
        case Stmt_IfElse_t:printf("(StmtType: Stmt_IfElse_t)\n");node.IfElseStmt->printInfo();break;
    }
}
void AssignStmtNode::printInfo(){
    printf("AssignStmtNode:\n");
    LVal->printInfo();
    Exp->printInfo();
}
void EmptyStmtNode::printInfo(){
    printf("EmptyStmtNode\n");
}
    void ExpStmtNode::printInfo(){
    printf("ExpStmtNode:\n");
    Exp->printInfo();
}
void WhileStmtNode::printInfo(){
    printf("WhileStmtNode:\n");
    Cond->printInfo();
    Stmt->printInfo();
}
void BreakStmtNode::printInfo(){
    printf("BreakStmtNode:\n");
}
void ContinueStmtNode::printInfo(){
    printf("ContinueStmtNode:\n");
}
void ReturnStmtNode::printInfo(){
    printf("ReturnStmtNode:\n");
    if (ReturnValue!=nullptr)ReturnValue->printInfo();
}
void IfElseStmtNode::printInfo(){
    printf("IfElseStmtNode:\n");
    switch (ElseType){
        case Else_Complete_t:printf("(ElseType: Else_Complete_t)\n");Cond->printInfo();IfStmt->printInfo();ElseStmt.Complete->printInfo();break;
        case Else_Incomplete_t:printf("(ElseType: Else_Incomplete_t)\n");Cond->printInfo();IfStmt->printInfo();ElseStmt.Incomplete->printInfo();break;
        case Else_Empty_t:printf("(ElseType: Else_Empty_t)\n");Cond->printInfo();IfStmt->printInfo();break;
    }
}
void OperationNode::printInfo(){
    printf("OperationNode:");
    switch (op){
        case PLUS_t:printf("(op: PLUS_t)\n");break;
        case MINUS_t:printf("(op: MINUS_t)\n");break;
        case MUL_t:printf("(op: MUL_t)\n");break;
        case DIV_t:printf("(op: DIV_t)\n");break;
        case MOD_t:printf("(op: MOD_t)\n");break;
        case NOT_t:printf("(op: NOT_t)\n");break;
        case GE_t:printf("(op: GE_t)\n");break;
        case GT_t:printf("(op: GT_t)\n");break;
        case LE_t:printf("(op: LE_t)\n");break;
        case LT_t:printf("(op: LT_t)\n");break;
        case OR_t:printf("(op: OR_t)\n");break;
        case AND_t:printf("(op: AND_t)\n");break;
        case NE_t:printf("(op: NE_t)\n");break;
        case EQ_t:printf("(op: EQ_t)\n");break;
    }
}
void NumberNode::printInfo(){
    printf("NumberNode:(value: %d)\n",value);
}
void BTypeNode::printInfo(){
    printf("BTypeNode:");
    switch (type){
        case INT_t:printf("(type: INT_t)\n");break;
    }
}

BaseNode::BaseNode(){
    nodetype = BaseNode_t;
    parent = nullptr;
}
IdentNode::IdentNode(){
    nodetype = IdentNode_t;
    parent = nullptr;
}
IdentNode::IdentNode(const char* name_){
    name = new string(name_);
    nodetype = IdentNode_t;
    parent = nullptr;
}
CompUnitsNode::CompUnitsNode(){
    nodetype = CompUnitsNode_t;
    parent =nullptr;
}
void CompUnitsNode::push_back(CompUnitNode* item){
    list.push_back(item);
    item->parent = this;
}
CompUnitNode::CompUnitNode(){
    nodetype = CompUnitNode_t;
    parent = nullptr;
}
CompUnitNode::CompUnitNode(DeclsNode* Decls){
    UnitItem.Decls = Decls;
    UnitType = Decl_t;
    nodetype = CompUnitNode_t;
    parent = nullptr;
    Decls->parent = this;
}
CompUnitNode::CompUnitNode(FuncDefNode* FuncDef){
    UnitItem.FuncDef = FuncDef;
    UnitType = FuncDef_t;
    nodetype = CompUnitNode_t;
    parent = nullptr;
    FuncDef->parent = this;
}
DeclsNode::DeclsNode(){
    nodetype = DeclsNode_t;
    parent = nullptr;
}
void DeclsNode::push_back(DeclNode* Item){
    list.push_back(Item);
    Item->parent = this;
}
DeclsNode::DeclsNode(DeclType_t dec,Type_t val){
    nodetype = DeclsNode_t;
    parent = nullptr;
    DeclType = dec;
    ValueType = val;
}

DeclNode::DeclNode(){
    nodetype = DeclNode_t;
    parent = nullptr;
}
DeclNode::DeclNode(Decl_OneNode* Decl_One){
    DeclItem.Decl_One = Decl_One;
    DeclNumType = One_t;
    nodetype = DeclNode_t;
    parent = nullptr;
    Decl_One->parent = this;
}
DeclNode::DeclNode(Decl_ArrayNode* Decl_Array){
    DeclItem.Decl_Array = Decl_Array;
    DeclNumType = Array_t;
    nodetype = DeclNode_t;
    parent = nullptr;
    Decl_Array->parent = this;
}
Decl_OneNode::Decl_OneNode(){
    nodetype = Decl_OneNode_t;
    parent = nullptr;
}
Decl_OneNode::Decl_OneNode(DeclType_t t,IdentNode* ident,ExpNode* exp):DeclType(t),ident(ident),exp(exp){
    nodetype = Decl_OneNode_t;
    parent = nullptr;
    ident->parent = this;
    if (exp != nullptr)exp->parent = this;
}
Decl_OneNode::Decl_OneNode(DeclType_t t,IdentNode* ident):DeclType(t),ident(ident),exp(nullptr){
    nodetype = Decl_OneNode_t;
    parent = nullptr;
    ident->parent = this;
}
Decl_ArrayNameNode::Decl_ArrayNameNode(){
    nodetype = Decl_ArrayNameNode_t;
    parent = nullptr;
}
Decl_ArrayNameNode::Decl_ArrayNameNode(IdentNode* ident):Ident(ident){
    nodetype = Decl_ArrayNameNode_t;
    parent = nullptr;
    ident->parent = this;
}
void Decl_ArrayNameNode::push_back(ExpNode* exp){
    //exp->parent = this;
    dim.push_back(exp);
    exp->parent = this;
}
Decl_ArrayNode::Decl_ArrayNode(){
    nodetype = Decl_ArrayNode_t;
    parent = nullptr;
}
Decl_ArrayNode::Decl_ArrayNode(DeclType_t t,Decl_ArrayNameNode* name,Decl_ArrayValueNode* value):DeclType(t),name(name),value(value){
    nodetype = Decl_ArrayNode_t;
    parent = nullptr;
    name->parent = this;
    value->parent = this;
}
Decl_ArrayNode::Decl_ArrayNode(DeclType_t t,Decl_ArrayNameNode* name):DeclType(t),name(name),value(nullptr){
    nodetype = Decl_ArrayNode_t;
    parent = nullptr;
    name->parent = this;
}
Decl_ArrayValueNode::Decl_ArrayValueNode(ExpNode*_exp){
    //printf("Node\n");
    ir_dep = 0;
    exp = _exp;
    ItemType = Item_Exp_t;
    nodetype = Decl_ArrayValueNode_t;
    parent = nullptr;
    _exp->parent = this;
}
/*
Decl_ArrayValueNode::Decl_ArrayValueNode(){
    ir_dep = -1;
    ItemType = Item_Array_t;
    vec.clear();
    nodetype = Decl_ArrayValueNode_t;
    parent = nullptr;
}*/

Decl_ArrayValueNode::Decl_ArrayValueNode(Decl_ArrayValueNode* item){
    //printf("Array\n");
    if (item == nullptr){
        ir_dep = 0;
        vec.clear();
    }
    else {
        ir_dep = max(ir_dep,item->ir_dep+1);
        //for (vector<Decl_ArrayValueNode*>::iterator it = vec.begin();it!=vec.end();it++)
        //    printf("%p\n",*it);
        vec.clear();
        vec.push_back(item);
        item->parent = this;
    }
    ItemType = Item_Array_t;
    nodetype = Decl_ArrayValueNode_t;
    parent = nullptr;
}
/*
Decl_ArrayValueNode::Decl_ArrayValueNode(const Decl_ArrayValueNode&a){
    printf("important\n");
}*/
void Decl_ArrayValueNode::push_back(Decl_ArrayValueNode* item){
    ir_dep = max(ir_dep,item->ir_dep+1);
    vec.push_back(item);
    item->parent = this;
}
FuncDefNode::FuncDefNode(){
    nodetype = FuncDefNode_t;
    parent = nullptr;
}
FuncDefNode::FuncDefNode(ReturnValueType returnvalue,IdentNode* ident,FuncFParamsNode* params,BlockItemsNode* block):ReturnValue(returnvalue),Ident(ident),Params(params),Block(block){
    nodetype = FuncDefNode_t;
    parent = nullptr;
    ident->parent = this;
    if (params != nullptr)params->parent = this;
    block->parent = this;
}
ExpNode::ExpNode(){
    nodetype = ExpNode_t;
    parent = nullptr;
}
ExpNode::ExpNode(OperationNode* op,ExpNode* arg1,ExpNode* arg2){
    args.ExpArgs = new pair<pair<ExpNode*,ExpNode*>,OperationNode*>;
    args.ExpArgs->first = make_pair(arg1,arg2);
    args.ExpArgs->second = op;
    ExpType = Exp_op_t;
    nodetype = ExpNode_t;
    parent = nullptr;
    op->parent = this;
    arg1->parent = this;
    arg2->parent = this;
}
ExpNode::ExpNode(UnaryExpNode* arg){
    args.UnaryExpArgs = arg;
    ExpType = Exp_unary_t;
    nodetype = ExpNode_t;
    parent = nullptr;
    arg->parent = this;
}
UnaryExpNode::UnaryExpNode(){
    nodetype = UnaryExpNode_t;
    parent = nullptr;
}
UnaryExpNode::UnaryExpNode(PrimaryExpNode* PrimaryExp){
    node.PrimaryExp = PrimaryExp;
    UnaryExpType = Unary_Primary_t;
    nodetype = UnaryExpNode_t;
    parent = nullptr;
    PrimaryExp->parent = this;
}
UnaryExpNode::UnaryExpNode(FunctionCallNode* FunctionCall){
    node.FunctionCall = FunctionCall;
    UnaryExpType = Unary_FuncCall_t;
    nodetype = UnaryExpNode_t;
    parent = nullptr;
    FunctionCall->parent = this;
}
UnaryExpNode::UnaryExpNode(OperationNode* op,UnaryExpNode* unary){
    this->node.UnaryExp = new pair<OperationNode*,UnaryExpNode*>;
    this->node.UnaryExp->first = op;
    this->node.UnaryExp->second = unary;
    UnaryExpType = Unary_Exp_t;
    nodetype = UnaryExpNode_t;
    parent = nullptr;
    op->parent = this;
    unary->parent = this;
}
PrimaryExpNode::PrimaryExpNode(){
    nodetype = PrimaryExpNode_t;
    parent = nullptr;
}
PrimaryExpNode::PrimaryExpNode(LValNode* lval){
    node.LVal=lval;
    PrimaryExpType=Primary_LVal_t;
    nodetype = PrimaryExpNode_t;
    parent = nullptr;
    lval->parent = this;
}
PrimaryExpNode::PrimaryExpNode(NumberNode* number){
    node.Number = number;
    PrimaryExpType=Primary_Number_t;
    nodetype = PrimaryExpNode_t;
    parent = nullptr;
    number->parent = this;
}
PrimaryExpNode::PrimaryExpNode(ExpNode* exp){
    node.Exp = exp;
    PrimaryExpType=Primary_Exp_t;
    nodetype = PrimaryExpNode_t;
    parent = nullptr;
    exp->parent = this;
}
FunctionCallNode::FunctionCallNode(){
    nodetype = FunctionCallNode_t;
    parent = nullptr;
}
FunctionCallNode::FunctionCallNode(IdentNode* ident,FuncRParamsNode* param):Ident(ident),Param(param){
    nodetype = FunctionCallNode_t;
    parent = nullptr;
    ident->parent = this;
    if (param != nullptr)param->parent = this;
}
FuncRParamsNode::FuncRParamsNode(){
    nodetype = FuncRParamsNode_t;
    parent = nullptr;
}
void FuncRParamsNode::push_back(ExpNode*item){
    list.push_back(item);
    item->parent = this;
}
LValNode::LValNode(){
    nodetype = LValNode_t;
    parent = nullptr;
}
LValNode::LValNode(IdentNode* ident){
    node.Ident = ident;
    LValType = LVal_Ident_t;
    nodetype = LValNode_t;
    parent = nullptr;
    ident->parent = this;
}
LValNode::LValNode(ArrayIdentNode* arrayident){
    node.ArrayIdent = arrayident;
    LValType = LVal_ArrayIdent_t;
    nodetype = LValNode_t;
    parent = nullptr;
    arrayident->parent = this;
}
ArrayIdentNode::ArrayIdentNode(){
    nodetype = ArrayIdentNode_t;
    parent = nullptr;
}
ArrayIdentNode::ArrayIdentNode(IdentNode* ident):Ident(ident){
    nodetype = ArrayIdentNode_t;
    parent = nullptr;
    ident->parent = this;
}
void ArrayIdentNode::push_back(ExpNode*exp){
    dim.push_back(exp);
    exp->parent = this;
}
FuncFParamsNode::FuncFParamsNode(){
    nodetype = FuncFParamsNode_t;
    parent = nullptr;
}
void FuncFParamsNode::push_back(FuncFParamNode* param){
    list.push_back(param);
    param->parent = this;
}
FuncFParamNode::FuncFParamNode(){
    nodetype = FuncFParamNode_t;
    parent = nullptr;
}
FuncFParamNode::FuncFParamNode(FuncFParamParam_Node*param){
    node.param = param;
    FuncFParamType = FuncFParamParam_t;
    nodetype = FuncFParamNode_t;
    parent = nullptr;
    param->parent = this;
}
FuncFParamNode::FuncFParamNode(FuncFParamArray_Node*array){
    node.array = array;
    FuncFParamType = FuncFParamArray_t;
    nodetype = FuncFParamNode_t;
    parent = nullptr;
    array->parent = this;
}
FuncFParamParam_Node::FuncFParamParam_Node(){
    nodetype = FuncFParamParam_Node_t;
    parent = nullptr;
}
FuncFParamParam_Node::FuncFParamParam_Node(BTypeNode* type,IdentNode* ident):Type(type),Ident(ident){
    nodetype = FuncFParamParam_Node_t;
    parent = nullptr;
    type->parent = this;
    ident->parent = this;
}
FuncFParamArray_Node::FuncFParamArray_Node(){
    nodetype = FuncFParamArray_Node_t;
    parent = nullptr;
}
FuncFParamArray_Node::FuncFParamArray_Node(BTypeNode* type,IdentNode* ident):Type(type),Ident(ident){
    nodetype = FuncFParamArray_Node_t;
    parent = nullptr;
    type->parent = this;
    ident->parent = this;
}
void FuncFParamArray_Node::push_back(ExpNode*item){
    dim.push_back(item);
    item->parent = this;
}
BlockItemsNode::BlockItemsNode(){
    nodetype = BlockItemsNode_t;
    parent = nullptr;
}
void BlockItemsNode::push_back(BlockItemNode*item){
    list.push_back(item);
    item->parent = this;
}
BlockItemNode::BlockItemNode(){
    nodetype = BlockItemNode_t;
    parent = nullptr;
}
BlockItemNode::BlockItemNode(DeclsNode* decl){
    node.Decl=decl;
    BlockItemType = Block_Decl_t;
    nodetype = BlockItemNode_t;
    parent = nullptr;
    decl->parent = this;
}
BlockItemNode::BlockItemNode(StmtNode* stmt){
    node.Stmt = stmt;
    BlockItemType = Block_Stmt_t;
    nodetype = BlockItemNode_t;
    parent = nullptr;
    stmt->parent = this;
}
StmtNode::StmtNode(){
    nodetype = StmtNode_t;
    parent = nullptr;
}
StmtNode::StmtNode(AssignStmtNode* assignstmt){
    node.AssignStmt = assignstmt;
    StmtType = Stmt_Assign_t;
    nodetype = StmtNode_t;
    parent = nullptr;
    assignstmt->parent = this;
}
StmtNode::StmtNode(EmptyStmtNode* emptystmt){
    node.EmptyStmt = emptystmt;
    StmtType = Stmt_Empty_t;
    nodetype = StmtNode_t;
    parent = nullptr;
    emptystmt->parent = this;
}
StmtNode::StmtNode(ExpStmtNode* expstmt){
    node.ExpStmt = expstmt;
    StmtType = Stmt_Exp_t;
    nodetype = StmtNode_t;
    parent = nullptr;
    expstmt->parent = this;
}
StmtNode::StmtNode(WhileStmtNode* whilestmt){
    node.WhileStmt = whilestmt;
    StmtType = Stmt_While_t;
    nodetype = StmtNode_t;
    parent = nullptr;
    whilestmt->parent = this;
}
StmtNode::StmtNode(BreakStmtNode* breakstmt){
    node.BreakStmt = breakstmt;
    StmtType = Stmt_Break_t;
    nodetype = StmtNode_t;
    parent = nullptr;
    breakstmt->parent = this;
}
StmtNode::StmtNode(ContinueStmtNode* continuestmt){
    node.ContinueStmt = continuestmt;
    StmtType = Stmt_Continue_t;
    nodetype = StmtNode_t;
    parent = nullptr;
    continuestmt->parent = this;
}
StmtNode::StmtNode(ReturnStmtNode* returnstmt){
    node.ReturnStmt = returnstmt;
    StmtType = Stmt_Return_t;
    nodetype = StmtNode_t;
    parent = nullptr;
    returnstmt->parent = this;
}
StmtNode::StmtNode(BlockItemsNode* blockitems){
    node.BlockItems = blockitems;
    StmtType = Stmt_Block_t;
    nodetype = StmtNode_t;
    parent = nullptr;
    blockitems->parent = this;
}
StmtNode::StmtNode(IfElseStmtNode* ifelsestmt){
    node.IfElseStmt = ifelsestmt;
    StmtType = Stmt_IfElse_t;
    nodetype = StmtNode_t;
    parent = nullptr;
    ifelsestmt->parent = this;
}
AssignStmtNode::AssignStmtNode(){
    nodetype = AssignStmtNode_t;
    parent = nullptr;
}
AssignStmtNode::AssignStmtNode(LValNode* lval,ExpNode* exp):LVal(lval),Exp(exp){
    nodetype = AssignStmtNode_t;
    parent = nullptr;
    lval->parent = this;
    exp->parent = this;
}
EmptyStmtNode::EmptyStmtNode(){
    nodetype = EmptyStmtNode_t;
    parent = nullptr;
}
ExpStmtNode::ExpStmtNode(){
    nodetype = ExpStmtNode_t;
    parent = nullptr;
}
ExpStmtNode::ExpStmtNode(ExpNode* exp):Exp(exp){
    nodetype = ExpStmtNode_t;
    parent = nullptr;
    exp->parent = this;
}
WhileStmtNode::WhileStmtNode(){
    nodetype = WhileStmtNode_t;
    parent = nullptr;
}
WhileStmtNode::WhileStmtNode(ExpNode*cond, StmtNode*stmt):Cond(cond),Stmt(stmt){
    nodetype = WhileStmtNode_t;
    parent = nullptr;
    cond->parent = this;
    stmt->parent = this;
}
BreakStmtNode::BreakStmtNode(){
    nodetype = BreakStmtNode_t;
    parent = nullptr;
}
ContinueStmtNode::ContinueStmtNode(){
    nodetype = ContinueStmtNode_t;
    parent = nullptr;
}
ReturnStmtNode::ReturnStmtNode(){
    nodetype = ReturnStmtNode_t;
    parent = nullptr;   
}
ReturnStmtNode::ReturnStmtNode(ExpNode* returnvalue):ReturnValue(returnvalue){
    nodetype = ReturnStmtNode_t;
    parent = nullptr;   
    returnvalue->parent = this;
}
IfElseStmtNode::IfElseStmtNode(){
    nodetype = IfElseStmtNode_t;
    parent = nullptr;
}
IfElseStmtNode::IfElseStmtNode(ExpNode* cond,StmtNode* ifstmt):Cond(cond),IfStmt(ifstmt){
    ElseStmt.Complete = nullptr;
    ElseType = Else_Empty_t;
    nodetype = IfElseStmtNode_t;
    parent = nullptr;
    cond->parent = this;
    ifstmt->parent = this;
}
IfElseStmtNode::IfElseStmtNode(ExpNode* cond,StmtNode* ifstmt,StmtNode* elsestmt):Cond(cond),IfStmt(ifstmt){
    ElseStmt.Complete = elsestmt;
    ElseType = Else_Complete_t;
    nodetype = IfElseStmtNode_t;
    parent = nullptr;
    cond->parent = this;
    ifstmt->parent = this;
    if (elsestmt!=nullptr)
        elsestmt->parent = this;
}
IfElseStmtNode::IfElseStmtNode(ExpNode* cond,StmtNode* ifstmt,IfElseStmtNode* elsestmt):Cond(cond),IfStmt(ifstmt){
    ElseStmt.Incomplete = elsestmt;
    ElseType = Else_Incomplete_t;
    nodetype = IfElseStmtNode_t;
    parent = nullptr;
    cond->parent = this;
    ifstmt->parent = this;
    if (elsestmt!=nullptr)
        elsestmt->parent = this;
}
OperationNode::OperationNode(){
    nodetype = OperationNode_t;
    parent = nullptr;
}
OperationNode::OperationNode(Operation_t op):op(op){
    nodetype = OperationNode_t;
    parent = nullptr;
}
NumberNode::NumberNode(){
    nodetype = NumberNode_t;
    parent = nullptr;
}
NumberNode::NumberNode(int value):value(value){
    nodetype = NumberNode_t;
    parent = nullptr;
}
BTypeNode::BTypeNode(){
    nodetype = BTypeNode_t;
    parent = nullptr;
}
BTypeNode::BTypeNode(Type_t t):type(t){
    nodetype = BTypeNode_t;
    parent = nullptr;
}

