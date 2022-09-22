#include "../../sysy2eeyore/ir/ir.h"
extern stringstream eeyore_code;
namespace ir{

int label_count;
Global global;
Local local;
Instruct instruct;

vector<pair<string,int> > FuncIsReturn;

char buf[10000000];
char temp[10000000];
int len = 0;
void myprintf(char*buf){
    eeyore_code<<buf;
}
void buf_store(){
    // printf("store:%s\n",temp);
    int l = strlen(temp);
    for (int i=0;i<l;i++)
        buf[len+i]=temp[i];
    len+=l;
}
void buf_flush(){
    buf[len]='\0';
    myprintf(buf);
    len = 0;
}

void Tag::print(){

    switch (TagType){
        case (Primordial_t):{
            if (value.primordial->second == -1){
                sprintf(temp,"T%d",value.primordial->first);
                buf_store();
            }
            else {
                sprintf(temp,"T%d[t%d]",value.primordial->first,value.primordial->second);
                buf_store();
            }
            break;
        }
        case (Temporary_t):{
            sprintf(temp,"t%d",value.temporary);
            buf_store();
            break;
        }
        case (Param_t):{
            if (value.param->second == -1){
                sprintf(temp,"p%d",value.param->first);
                buf_store();
            }
            else {
                sprintf(temp,"p%d[t%d]",value.param->first,value.param->second);
                buf_store();
            }
        }break;
        case (Number_t):{
            sprintf(temp,"%d",value.number);
            buf_store();
            break;
        }
    }
}

bool is_array(Tag t){
    return (t.TagType == Primordial_t && t.value.primordial->second != -1) || (t.TagType == Param_t && t.value.param->second != -1);
}

int tag_count=0,lable_count=0;
int newlabel(){
    return label_count++;
}

Tag newtag(){
    Tag tag;
    tag.TagType = Temporary_t;
    tag.value.temporary = tag_count;
    tag_count++;
    return tag;
}

void InsertInstruction(InstructionType_t type,Tag a,Tag b,Operation_t op,Tag c){
    int isarray_b = is_array(b);
    int isarray_c = is_array(c);
    if (isarray_b){
        Tag tmp = newtag();
        InsertInstruction(ASSIGN_t,tmp,b);
        b = tmp;
    }
    if (isarray_c){
        Tag tmp = newtag();
        InsertInstruction(ASSIGN_t,tmp,c);
        c = tmp;
    }
    switch (type){
        case OP_t:{
            a.print();
            sprintf(temp," = ");buf_store();
            b.print();
            sprintf(temp," ");buf_store();
            switch (op){
                case PLUS_t:sprintf(temp,"+");buf_store();break;
                case MINUS_t:sprintf(temp,"-");buf_store();break;
                case MUL_t:sprintf(temp,"*");buf_store();break;
                case DIV_t:sprintf(temp,"/");buf_store();break;
                case MOD_t:sprintf(temp,"%%");buf_store();break;
                case GE_t:sprintf(temp,">=");buf_store();break;
                case GT_t:sprintf(temp,">");buf_store();break;
                case LE_t:sprintf(temp,"<=");buf_store();break;
                case LT_t:sprintf(temp,"<");buf_store();break;
                case OR_t:sprintf(temp,"||");buf_store();break;
                case AND_t:sprintf(temp,"&&");buf_store();break;
                case NE_t:sprintf(temp,"!=");buf_store();break;
                case EQ_t:sprintf(temp,"==");buf_store();break;
                default:{
                    printf("InsertInstruction: (Tag Tag op Tag) error\n");
                    exit(0);
                }
            }
            sprintf(temp," ");buf_store();
            c.print();
            sprintf(temp,"\n");buf_store();
            break;
        }
        default:{
            printf("InsertInstruction: (Tag Tag op Tag) error\n");
            exit(0);
        }
    }
}

void InsertInstruction(InstructionType_t type,Tag a,Operation_t op,Tag b){
    if (is_array(b)){
        Tag tag = newtag();
        InsertInstruction(ASSIGN_t,tag,b);
        b = tag;
    }
    switch (type){
        case OP_t:{
            a.print();
            sprintf(temp," = ");buf_store();
            switch (op){
                case PLUS_t:sprintf(temp," ");buf_store();break;
                case MINUS_t:sprintf(temp,"-");buf_store();break;
                case NOT_t:sprintf(temp,"!");buf_store();break;
                default:{
                    printf("InsertInstruction: (Tag op Tag) error\n");
                    exit(0);
                }
            }
            sprintf(temp," ");buf_store();
            b.print();
            sprintf(temp,"\n");buf_store();
            break;
        }
        default:{
            printf("InsertInstruction: (Tag op Tag) error\n");
            exit(0);
        }
    }
}

void InsertInstruction(InstructionType_t type,Tag a,Tag b){
    int isarray_a = is_array(a);
    int isarray_b = is_array(b);
    if (isarray_a && isarray_b){
        Tag tag = newtag();
        InsertInstruction(ASSIGN_t,tag,b);
        b = tag;
    }
    switch (type){
        case ASSIGN_t:{
            a.print();
            sprintf(temp," = ");buf_store();
            b.print();
            sprintf(temp,"\n");buf_store();
            break;
        }
        case ASSIGN_ARRAY_INDEX_t:{
            if (a.TagType == Primordial_t)
                sprintf(temp,"T%d[%d] = ",a.value.primordial->first,4*a.value.primordial->second);
            else sprintf(temp,"p%d[%d] = ",a.value.param->first,4*a.value.param->second);
            buf_store();
            b.print();
            sprintf(temp,"\n");buf_store();
            break;
        }
        default:{
            printf("InsertInstruction: (Tag Tag) error\n");
            exit(0);
        }
    }
}
void InsertInstruction(InstructionType_t type,Tag a,IdentNode*node){
    switch (type){
        case CALL_t:{
            a.print();
            sprintf(temp," = call ");buf_store();
            sprintf(temp,"f_%s\n",node->name->c_str());
            buf_store();
            break;
        }
        default:{
            printf("InsertInstruction: (tag IdentNode*) error\n");
            exit(0);
        }
    }
}
void InsertInstruction(InstructionType_t type,IdentNode*node){
    switch (type){
        case CALL_t:{
            sprintf(temp,"call ");buf_store();
            sprintf(temp,"f_%s\n",node->name->c_str());buf_store();
            break;
        }
        default:{
            printf("InsertInstruction: (IdentNode*) error\n");
            exit(0);
        }
    }
}
void InsertInstruction(InstructionType_t type,Tag a){
    int isarray_a = is_array(a);
    if (isarray_a){
        Tag tag = newtag();
        InsertInstruction(ASSIGN_t,tag,a);
        a = tag;
    }
    switch (type){
        case PARAM_t:sprintf(temp,"param ");buf_store();a.print();sprintf(temp,"\n");buf_store();break;
        case RETURN_t:sprintf(temp,"return ");buf_store();a.print();sprintf(temp,"\n");buf_store();break;
        default:{
            printf("InsertInstruction: (Tag) error\n");
            exit(0);
        }
    }
}
void InsertInstruction(InstructionType_t type,Tag a,int l){
    switch (type){
        case IFELSE_t:{
            if (is_array(a)){
                Tag tag = newtag();
                InsertInstruction(ASSIGN_t,tag,a);
                a = tag;
            }
            sprintf(temp,"if ");buf_store();a.print();sprintf(temp," == 0 goto l%d\n",l);buf_store();
            break;
        }
        case ASSIGN_ARRAY_INDEX_t:{
            sprintf(temp,"T%d[%d] = %d\n",a.value.primordial->first,4 * a.value.primordial->second,l);buf_store();
            break;
        }
        default:{
            printf("InsertInstruction: (Tag label) error\n");
            exit(0);
        }
    }
}
void InsertInstruction(InstructionType_t type,int l){
    switch (type){
        case GOTO_t:{
            sprintf(temp,"goto l%d\n",l);buf_store();break;
        }
        case LABEL_t:{
            sprintf(temp,"l%d:\n",l);buf_store();break;
        }
        default:{
            printf("InsertInstruction: (label) error\n");
            exit(0);
        }
    }
}
void InsertInstruction(InstructionType_t type){
    switch (type){
        case RETURN_t:{
            sprintf(temp,"return\n");buf_store();break;
        }
        default:{
            printf("InsertInstruction: () error\n");
            exit(0);
        }
    }
}
void InsertInstruction(InstructionType_t type,FuncDefNode*node){
    switch(type){
        case FUNCTION_BEGIN:
            sprintf(temp,"f_%s [%lu]\n",node->Ident->name->c_str(),(node->Params == nullptr)?0:node->Params->list.size());buf_store();
        break;
        case FUNCTION_END:
            sprintf(temp,"end f_%s\n",node->Ident->name->c_str());buf_store();
        break;

        default:{
            printf("InsertInstruction: (FuncDefNode*) error\n");
            exit(0);
        }
    }
}
void InsertInstruction(InstructionType_t type,const char* s){
    switch (type){
        case STRING_t:sprintf(temp,"%s",s);buf_store();break;
        default:
            printf("InsertInstruction: (char*) error\n");
            exit(0);
    }
}
void InsertInstruction(InstructionType_t type,int index,Variable v){
    switch (type){
        case VAR_T:
            if (v.DeclNum == Decl_One_t){
                sprintf(temp,"var T%d\n",index);buf_store();
            }
            else if (v.DeclNum == Decl_Array_t){
                sprintf(temp,"var %d T%d\n",4*v.value.array->total,index);buf_store();
            }
            break;
        default:
            printf("InsertInstruction: (Variable) error\n");
            exit(0);
    }
}
void InsertInstruction(InstructionType_t type,int name,int index,Tag tag){
    int isarray_tag = is_array(tag);
    if (isarray_tag){
        Tag m = newtag();
        InsertInstruction(ASSIGN_t,m,tag);
        tag = m;
    }
    sprintf(temp,"T%d[%d] = ",name,4*index);buf_store();
    tag.print();
    sprintf(temp,"\n");buf_store();
}

int EvalConstIdentValue(IdentNode*ident){
    for (vector<Variable>::reverse_iterator it = local.LocalVariable.rbegin();it!=local.LocalVariable.rend();it++){
        if (!(*it).IsDelete && (*it).DeclNum == Decl_One_t && (*it).name == (*ident->name)){
            return (*it).value.value->value;
        }
    }
    for (vector<Variable>::iterator it = global.GlobalVariable.begin();it!=global.GlobalVariable.end();it++){
        if ((*it).DeclType == Decl_Const_t && (*it).DeclNum == Decl_One_t && (*it).name == (*ident->name)){
            return (*it).value.value->value;
        }
    }
    printf("EvalConstIdentValue: No Ident\n");
    exit(0);
}
int EvalConstIdentArrayValue(ArrayIdentNode*array){
    for (vector<Variable>::reverse_iterator it = local.LocalVariable.rbegin();it!=local.LocalVariable.rend();it++){
        if (!(*it).IsDelete && (*it).DeclNum == Decl_Array_t && (*it).name == (*array->Ident->name)){
            if ((*it).value.array->dim.size() != array->dim.size()){
                printf("EvalConstIdentArrayValue: diffierent size\n");
                exit(0);
            }
            int index = 0;
            int size = array->dim.size();
            for (int i = 0;i<size;i++){
                index = index * (*it).value.array->dim[i] + EvalConst(array->dim[i]);
            }
            return (*it).value.array->value[index].value;
        }
    }
    for (vector<Variable>::iterator it = global.GlobalVariable.begin();it!=global.GlobalVariable.end();it++){
        if ((*it).DeclType == Decl_Const_t && (*it).DeclNum == Decl_Array_t && (*it).name == (*array->Ident->name)){
            if ((*it).value.array->dim.size() != array->dim.size()){
                printf("EvalConstIdentArrayValue: diffierent size\n");
                exit(0);
            }
            int index = 0;
            int size = array->dim.size();
            for (int i = 0;i<size;i++){
                index = index * (*it).value.array->dim[i] + EvalConst(array->dim[i]);
            }
            return (*it).value.array->value[index].value;
        }
    }
    printf("EvalConstIdentArrayValue: No ArrayIdent\n");
    exit(0);
}
int EvalConstLVal(LValNode*exp){
    if (exp->LValType == LVal_Ident_t){
        return EvalConstIdentValue(exp->node.Ident);
    }
    else return EvalConstIdentArrayValue(exp->node.ArrayIdent);
}
int EvalConstPrimaryExp(PrimaryExpNode*exp){
    if (exp->PrimaryExpType == Primary_Exp_t){
        return EvalConst(exp->node.Exp);
    }
    else if (exp->PrimaryExpType == Primary_Number_t){
        return exp->node.Number->value;
    }
    else if (exp->PrimaryExpType == Primary_LVal_t){
        return EvalConstLVal(exp->node.LVal);
    }
    printf("EvalConstPrimaryExp: No such case\n");
    exit(0);
}
int EvalConstUnaryExp(UnaryExpNode*exp){
    if (exp->UnaryExpType == Unary_Exp_t){
        Operation_t op = exp->node.UnaryExp->first->op;
        switch (op){
            case NOT_t:{
                int l = EvalConstUnaryExp(exp->node.UnaryExp->second);
                if (l)return 0;else return 1;
            }
            case MINUS_t:{
                int l = EvalConstUnaryExp(exp->node.UnaryExp->second);
                return -l;
            }
            case PLUS_t:{
                int l = EvalConstUnaryExp(exp->node.UnaryExp->second);
                return l;
            }
            default:{
                printf("EvalConstUnaryExp: wrong op\n");
                exit(0);
            }
        }
    }
    else if (exp->UnaryExpType == Unary_FuncCall_t){
        printf("EvalConstUnaryExp:undefine\n");
        exit(0);
    }
    else if (exp->UnaryExpType == Unary_Primary_t){
        return EvalConstPrimaryExp(exp->node.PrimaryExp);
    }
    printf("EvalConstUnaryExp: No such case\n");
    exit(0);
}
int EvalConst(ExpNode*exp){
    if (exp->ExpType == Exp_op_t){
        Operation_t op = exp->args.ExpArgs->second->op;
        switch (op){
            case PLUS_t:{
                int l = EvalConst(exp->args.ExpArgs->first.first),r = EvalConst(exp->args.ExpArgs->first.second);
                return l+r;
            }
            case MINUS_t:{
                int l = EvalConst(exp->args.ExpArgs->first.first),r = EvalConst(exp->args.ExpArgs->first.second);
                return l - r;
            }
            case MUL_t:{
                int l = EvalConst(exp->args.ExpArgs->first.first),r = EvalConst(exp->args.ExpArgs->first.second);
                return l*r;
            }
            case DIV_t:{
                int l = EvalConst(exp->args.ExpArgs->first.first),r = EvalConst(exp->args.ExpArgs->first.second);
                return l/r;
            }
            case MOD_t:{
                int l = EvalConst(exp->args.ExpArgs->first.first),r = EvalConst(exp->args.ExpArgs->first.second);
                return l%r;
            }
            case GE_t:{
                int l = EvalConst(exp->args.ExpArgs->first.first),r = EvalConst(exp->args.ExpArgs->first.second);
                if (l>=r)return 1;else return 0;
            }
            case GT_t:{
                int l = EvalConst(exp->args.ExpArgs->first.first),r = EvalConst(exp->args.ExpArgs->first.second);
                if (l>r) return 1;else return 0;
            }
            case LE_t:{
                int l = EvalConst(exp->args.ExpArgs->first.first),r = EvalConst(exp->args.ExpArgs->first.second);
                if (l<=r) return 1;else return 0;
            }
            case LT_t:{
                int l = EvalConst(exp->args.ExpArgs->first.first),r = EvalConst(exp->args.ExpArgs->first.second);
                if (l<r) return 1;else return 0;
            }
            case OR_t:{
                int l = EvalConst(exp->args.ExpArgs->first.first);
                if (l)return 1;
                else{
                    int r = EvalConst(exp->args.ExpArgs->first.second);
                    if (r)return 1;else return 0;
                }
            }
            case AND_t:{
                int l = EvalConst(exp->args.ExpArgs->first.first);
                if (l){
                    int r = EvalConst(exp->args.ExpArgs->first.second);
                    if (r)return 1;else return 0;
                }
                else return 0;
            }
            case NE_t:{
                int l = EvalConst(exp->args.ExpArgs->first.first),r = EvalConst(exp->args.ExpArgs->first.second);
                if (l!=r)return 1;else return 0;
            }
            case EQ_t:{
                int l = EvalConst(exp->args.ExpArgs->first.first),r = EvalConst(exp->args.ExpArgs->first.second);
                if (l==r)return 1;else return 0;
            }
            default:{
                printf("EvalConst: wrong op\n");
                exit(0);
            }
        }
    }
    else if (exp->ExpType == Exp_unary_t){
        return EvalConstUnaryExp(exp->args.UnaryExpArgs);
    }
    else {
        printf("EvalConst: No such case\n");
        exit(0);
    }
}
void GlobalVariableArrayAssign(Array*array,Decl_ArrayValueNode*initarray,int align,int dep){
    //printf("%d %d\n",align,dep);
    if (initarray == nullptr){
        for (int i=0;i<array->total;i++)array->value.push_back(0);
        return;
    }else{
        if (initarray->ItemType == Item_Array_t){
            int s = array->value.size();
            while (s%align!=0){array->value.push_back(0);s++;}
            if (initarray->vec.size()==0){
                for (int i = 0;i < align;i++){
                    array->value.push_back(0);
                }
            }
            else {
                for (vector<Decl_ArrayValueNode*>::iterator it = (initarray->vec).begin();it != (initarray->vec).end();it++)
                    GlobalVariableArrayAssign(array,*it,align/array->dim[dep],dep+1);
                int size = (int)array->value.size();
                while (size%align!=0){array->value.push_back(0);size++;}
            }
        }
        else array->value.push_back(EvalConst(initarray->exp));
    }
}

void LocalVariableArrayAssign(int index,Array*array,Decl_ArrayValueNode*initarray,int align,int dep){
    // printf("local: %d %d\n",align,dep);
    Variable_int temp(0);
    if (initarray == nullptr){
        for (int i=0;i<array->total;i++){
            Tag item;
            Tag tag = Number2Tag(array->value.size());
            item.TagType = Primordial_t;
            item.value.primordial = new pair<int,int>;
            item.value.primordial->first = index;
            item.value.primordial->second = tag.value.number;
            InsertInstruction(ASSIGN_ARRAY_INDEX_t,item,0);
            array->value.push_back(temp);
        }
        return;
    }else{
        if (initarray->ItemType == Item_Array_t){
            int s = array->value.size();
            while (s%align!=0){
                Tag item;
                Tag tag = Number2Tag(array->value.size());
                item.TagType = Primordial_t;
                item.value.primordial = new pair<int,int>;
                item.value.primordial->first = index;
                item.value.primordial->second = tag.value.number;
                InsertInstruction(ASSIGN_ARRAY_INDEX_t,item,0);
                array->value.push_back(temp);
                s++;
            }
            if (initarray->vec.size() == 0){
                for (int i=0;i<align;i++){
                    Tag item;
                    Tag tag = Number2Tag(array->value.size());
                    item.TagType = Primordial_t;
                    item.value.primordial = new pair<int,int>;
                    item.value.primordial->first = index;
                    item.value.primordial->second = tag.value.number;
                    InsertInstruction(ASSIGN_ARRAY_INDEX_t,item,0);
                    array->value.push_back(temp);
                }
            }
            else{
                for (vector<Decl_ArrayValueNode*>::iterator it = (initarray->vec).begin();it!=(initarray->vec).end();it++)
                    LocalVariableArrayAssign(index,array,*it,align/array->dim[dep],dep+1);
                int size = (int)array->value.size();
                while (size%align!=0){
                    Tag item;
                    Tag tag = Number2Tag(array->value.size());
                    item.TagType = Primordial_t;
                    item.value.primordial = new pair<int,int>;
                    item.value.primordial->first = index;
                    item.value.primordial->second = tag.value.number;
                    InsertInstruction(ASSIGN_ARRAY_INDEX_t,item,0);
                    array->value.push_back(temp);size++;
                }
            }
        }
        else{
            Tag item;
            Tag tag = Number2Tag(array->value.size());
            item.TagType = Primordial_t;
            item.value.primordial = new pair<int,int>;
            item.value.primordial->first = index;
            item.value.primordial->second = tag.value.number;
            InsertInstruction(ASSIGN_ARRAY_INDEX_t,item,analyze_ExpNode(initarray->exp));
            //TODO
            array->value.push_back(temp);
        }
    }
}
void LocalVariableArrayAssignConst(int index,Array*array,Decl_ArrayValueNode*initarray,int align,int dep){
    Variable_int temp(0);
    if (initarray == nullptr){
        for (int i=0;i<array->total;i++){
            Tag item;
            Tag tag = Number2Tag(array->value.size());
            item.TagType = Primordial_t;
            item.value.primordial = new pair<int,int>;
            item.value.primordial->first = index;
            item.value.primordial->second = tag.value.number;
            InsertInstruction(ASSIGN_ARRAY_INDEX_t,item,0);
            array->value.push_back(temp);
        }
        return;
    }else{
        if (initarray->ItemType == Item_Array_t){
            int s = array->value.size();
            while (s%align!=0){
                Tag item;
                Tag tag = Number2Tag(array->value.size());
                item.TagType = Primordial_t;
                item.value.primordial = new pair<int,int>;
                item.value.primordial->first = index;
                item.value.primordial->second = tag.value.number;
                InsertInstruction(ASSIGN_ARRAY_INDEX_t,item,0);
                array->value.push_back(temp);
                s++;
            }
            if (initarray->vec.size()==0){
                Tag item;
                Tag tag = Number2Tag(array->value.size());
                item.TagType = Primordial_t;
                item.value.primordial = new pair<int,int>;
                item.value.primordial->first = index;
                item.value.primordial->second = tag.value.number;
                InsertInstruction(ASSIGN_ARRAY_INDEX_t,item,0);
                array->value.push_back(temp);
            }
            else{
                for (vector<Decl_ArrayValueNode*>::iterator it = (initarray->vec).begin();it!=(initarray->vec).end();it++)
                    LocalVariableArrayAssignConst(index,array,*it,align/array->dim[dep],dep+1);
                int size = (int)array->value.size();
                while (size%align!=0){
                    Tag item;
                    Tag tag = Number2Tag(array->value.size());
                    item.TagType = Primordial_t;
                    item.value.primordial = new pair<int,int>;
                    item.value.primordial->first = index;
                    item.value.primordial->second = tag.value.number;
                    InsertInstruction(ASSIGN_ARRAY_INDEX_t,item,0);
                    array->value.push_back(temp);size++;
                }
            }
        }
        else{
            Tag item;
            Tag tag = Number2Tag(array->value.size());
            item.TagType = Primordial_t;
            item.value.primordial = new pair<int,int>;
            item.value.primordial->first = index;
            item.value.primordial->second = tag.value.number;
            InsertInstruction(ASSIGN_ARRAY_INDEX_t,item,analyze_ExpNode(initarray->exp));
            //TODO
            array->value.push_back(temp);
        }
    }

}

void GlobalVariableAppend(DeclsNode*decl){
     #ifdef Debug_Analyze
        printf("GlobalVariableAppend\n");
    #endif
    for (vector<DeclNode*>::iterator it = decl->list.begin(); it != decl->list.end();it++){
        Variable item;
        if (decl->DeclType == Const_t)item.DeclType = Decl_Const_t;else item.DeclType = Decl_Variable_t;
        if ((*it)->DeclNumType == One_t)item.DeclNum = Decl_One_t;else item.DeclNum = Decl_Array_t;
        if ((*it)->DeclNumType == One_t){
            Decl_OneNode* now = (*it)->DeclItem.Decl_One;
            item.name = (*now->ident->name);
            if (now->exp == nullptr)item.value.value = new Variable_int(0);
            else item.value.value = new Variable_int(EvalConst(now->exp));
            global.GlobalVariable.push_back(item);
        }
        else{
            Decl_ArrayNode* now = (*it)->DeclItem.Decl_Array;
            item.name = (*now->name->Ident->name);
            item.value.array = new Array();
            item.value.array->total = 1;
            for (vector<ExpNode*>::iterator it=now->name->dim.begin();it!=now->name->dim.end();it++){
                int v = EvalConst(*it);
                item.value.array->dim.push_back(v);
                item.value.array->total *= v;
            }
            if (now->value != nullptr){
                int dep = 0;
                int align = 1;
                //printf("global: %d %d %d\n",item.value.array->dim.size(),now->value->ir_dep,align);
                //exit(0);
                for (int i =dep;i<(int)item.value.array->dim.size(); i++)
                    align *= item.value.array->dim[i];
                GlobalVariableArrayAssign(item.value.array,now->value,align,dep);
                int s = item.value.array->value.size();
                while (s<item.value.array->total){
                    item.value.array->value.push_back(0);
                    s++;
                }
            }
            else{
                int s = item.value.array->value.size();
                while (s<item.value.array->total){
                    item.value.array->value.push_back(0);
                    s++;
                }
            }
            global.GlobalVariable.push_back(item);
        }
    }
}

void GetGlobalVariable(CompUnitsNode*root){
    #ifdef Debug_Analyze
        printf("GetGlobalVariable\n");
    #endif
    FuncIsReturn.push_back(make_pair((string)"getint",1));
    FuncIsReturn.push_back(make_pair((string)"getch",1));
    FuncIsReturn.push_back(make_pair((string)"getarray",1));
    FuncIsReturn.push_back(make_pair((string)"putint",0));
    FuncIsReturn.push_back(make_pair((string)"putch",0));
    FuncIsReturn.push_back(make_pair((string)"putarray",0));
    FuncIsReturn.push_back(make_pair((string)"_sysy_starttime",0));
    FuncIsReturn.push_back(make_pair((string)"_sysy_stoptime",0));

    for (vector<CompUnitNode*>::iterator it = root->list.begin();it!=root->list.end();it++){
        if ((*it)->UnitType == Decl_t)GlobalVariableAppend((*it)->UnitItem.Decls);
        else {
            if ((*it)->UnitItem.FuncDef->ReturnValue == RINT_t){
                FuncIsReturn.push_back(make_pair(*(*it)->UnitItem.FuncDef->Ident->name,1));
            }
            else FuncIsReturn.push_back(make_pair(*(*it)->UnitItem.FuncDef->Ident->name,0));
        }
    }
    for (vector<ir::Variable>::iterator it = global.GlobalVariable.begin();it!=global.GlobalVariable.end();it++){
        //(*it).printInfo();
        InsertInstruction(VAR_T,it - global.GlobalVariable.begin(),*it);
    }
    for (vector<ir::Variable>::iterator it = global.GlobalVariable.begin();it!=global.GlobalVariable.end();it++){
        //(*it).printInfo();
        if ((*it).DeclNum == Decl_One_t){
            if ((*it).value.value->is_fix&&(*it).value.value!=0){
                Tag item;
                item.TagType = Primordial_t;
                item.value.primordial = new pair<int,int>;
                item.value.primordial->first = it - (global.GlobalVariable.begin());
                item.value.primordial->second = -1;
                InsertInstruction(ASSIGN_t,item,Number2Tag((*it).value.value->value));
            }
        }
        else if ((*it).DeclNum == Decl_Array_t){
            for (vector<Variable_int>::iterator it_ = (*it).value.array->value.begin();it_ != (*it).value.array->value.end();it_++){
                if ((*it_).is_fix&&(*it_).value!=0){
                    Tag item;
                    item.TagType = Primordial_t;
                    item.value.primordial = new pair<int,int>;
                    item.value.primordial->first = it - (global.GlobalVariable.begin());
                    item.value.primordial->second = it_ - (*it).value.array->value.begin();
                    // InsertInstruction(ASSIGN_t,item,Number2Tag((*it_).value));
                    InsertInstruction(ASSIGN_ARRAY_INDEX_t,item,(*it_).value);
                    //TODO
                }
            }
        }
    }
    buf_flush();
}

Tag Number2Tag(int x){
    #ifdef Debug_Analyze
        printf("Number2Tag:\n");
    #endif
    Tag tag;
    tag.TagType = Number_t;
    tag.value.number = x;
    return tag;
}

Tag LVal2Tag(LValNode*node){
    #ifdef Debug_Analyze
        printf("LVal2Tag:\n");
    #endif
    Tag res;
    // printf("%d %d %s\n",node->LValType,LVal_ArrayIdent_t,node->node.Ident->name->c_str());
    res.TagType = Primordial_t;
    for (vector<Variable>::reverse_iterator it = local.LocalVariable.rbegin();it!=local.LocalVariable.rend();it++){
        if ((*it).IsDelete)continue;
        if (node->LValType==LVal_Ident_t && (*it).name == *node->node.Ident->name){
            if ((*it).DeclNum == Decl_One_t){
                int index = (local.LocalVariable.rend() - it - 1) + global.GlobalVariable.size();
                res.value.primordial = new pair<int,int>;
                res.value.primordial->first = index;
                res.value.primordial->second = -1;
                return res;
            }
            else if ((*it).DeclNum == Decl_Array_t){
                int index = (local.LocalVariable.rend() - it - 1) + global.GlobalVariable.size();
                res.value.primordial = new pair<int,int>;
                res.value.primordial->first = index;
                res.value.primordial->second = -1;
                return res;
            }
        }
        else if (node->LValType == LVal_ArrayIdent_t && (*it).name == *node->node.ArrayIdent->Ident->name){
            int index = (local.LocalVariable.rend() - it - 1) + global.GlobalVariable.size();
            if (node->node.ArrayIdent->dim.size() != (*it).value.array->dim.size()){
                //printf("LVal2Tag: error\n");exit(0);
                int size = (*it).value.array->dim.size();
                Tag now = analyze_ExpNode(node->node.ArrayIdent->dim[0]);
                for (int i = 1;i<size;i++){
                    Tag out1 = newtag();
                    InsertInstruction(OP_t,out1,now,MUL_t,Number2Tag((*it).value.array->dim[i]));
                    if (i < (int)node->node.ArrayIdent->dim.size()){
                        Tag out2 = newtag();
                        InsertInstruction(OP_t,out2,out1,PLUS_t,analyze_ExpNode(node->node.ArrayIdent->dim[i]));
                        now = out2;
                    }else now = out1;
                }
                Tag array_tag;
                array_tag.TagType = Primordial_t;
                array_tag.value.primordial = new pair<int,int>;
                array_tag.value.primordial->first = index;
                array_tag.value.primordial->second = -1;
                Tag tag1 = newtag(),tag2 = newtag();
                InsertInstruction(OP_t,tag1,now,MUL_t,Number2Tag(4));
                InsertInstruction(OP_t,tag2,array_tag,PLUS_t,tag1);
                return tag2;
            }
            else {
                int size = node->node.ArrayIdent->dim.size();
                Tag now = analyze_ExpNode(node->node.ArrayIdent->dim[0]);
                for (int i = 1;i<size;i++){
                    Tag out1 = newtag();
                    Tag out2 = newtag();
                    InsertInstruction(OP_t,out1,now,MUL_t,Number2Tag((*it).value.array->dim[i]));
                    InsertInstruction(OP_t,out2,out1,PLUS_t,analyze_ExpNode(node->node.ArrayIdent->dim[i]));
                    now = out2;
                }
                res.value.primordial = new pair<int,int>;
                res.value.primordial->first = index;
                Tag tag1 = newtag();
                InsertInstruction(OP_t,tag1,now,MUL_t,Number2Tag(4));
                res.value.primordial->second = tag1.value.temporary;
                return res;
            }
        }
    }
    for (vector<Param>::iterator it = local.FuncParam.begin();it != local.FuncParam.end();it++){
        if (node->LValType == LVal_Ident_t && (*it).name == *node->node.Ident->name){
            int index = (it - local.FuncParam.begin());
            res.TagType = Param_t;
            res.value.param = new pair<int,int>;
            res.value.param->first = index;
            res.value.param->second = -1;
            return res;
        }
        else if (node->LValType == LVal_ArrayIdent_t && (*it).name == *node->node.ArrayIdent->Ident->name){
            int index = (it - local.FuncParam.begin());
            res.TagType = Param_t;
            if (node->node.ArrayIdent->dim.size() != (*it).value.dim->size()){
                int size  = (*it).value.dim->size();
                Tag now = analyze_ExpNode(node->node.ArrayIdent->dim[0]);
                for (int i=1;i<size;i++){
                    Tag out1 = newtag();
                    InsertInstruction(OP_t,out1,now,MUL_t,Number2Tag((*(*it).value.dim)[i]));
                    if (i< (int)node->node.ArrayIdent->dim.size()){
                        Tag out2 = newtag();
                        InsertInstruction(OP_t,out2,out1,PLUS_t,analyze_ExpNode(node->node.ArrayIdent->dim[i]));
                        now = out2;
                    }else now = out1;
                }
                Tag array_tag;
                array_tag.TagType = Param_t;
                array_tag.value.param = new pair<int,int>;
                array_tag.value.param->first = index;
                array_tag.value.param->second = -1;
                Tag tag1 = newtag(), tag2 = newtag();
                InsertInstruction(OP_t,tag1,now,MUL_t,Number2Tag(4));
                InsertInstruction(OP_t,tag2,array_tag,PLUS_t,tag1);
                return tag2;
            }
            else{
                int size = node->node.ArrayIdent->dim.size();
                Tag now = analyze_ExpNode(node->node.ArrayIdent->dim[0]);
                for (int i = 1;i<size;i++){
                    Tag out1 = newtag();
                    Tag out2 = newtag();
                    InsertInstruction(OP_t,out1,now,MUL_t,Number2Tag((*(*it).value.dim)[i]));
                    InsertInstruction(OP_t,out2,out1,PLUS_t,analyze_ExpNode(node->node.ArrayIdent->dim[i]));
                    now = out2;
                }
                Tag tag1 = newtag();
                InsertInstruction(OP_t,tag1,now,MUL_t,Number2Tag(4));
                res.TagType = Param_t;
                res.value.param = new pair<int,int>;
                res.value.param->first = index;
                res.value.param->second = tag1.value.temporary;
                return res;
            }
        }
    }
    for (vector<Variable>::iterator it = global.GlobalVariable.begin();it!=global.GlobalVariable.end();it++){
        if (node->LValType==LVal_Ident_t && (*it).name == *node->node.Ident->name){
            int index = (it - global.GlobalVariable.begin());
            res.value.primordial = new pair<int,int>;
            res.value.primordial->first = index;
            res.value.primordial->second = -1;
            return res;
        }
        else if (node->LValType == LVal_ArrayIdent_t && (*it).name == *node->node.ArrayIdent->Ident->name){
            int index = (it - global.GlobalVariable.begin());
            if (node->node.ArrayIdent->dim.size() != (*it).value.array->dim.size()){
                int size = (*it).value.array->dim.size();
                Tag now = analyze_ExpNode(node->node.ArrayIdent->dim[0]);
                for (int i=1;i<size;i++){
                    Tag out1 = newtag();
                    InsertInstruction(OP_t,out1,now,MUL_t,Number2Tag((*it).value.array->dim[i]));
                    if (i<(int)node->node.ArrayIdent->dim.size()){
                        Tag out2 = newtag();
                        InsertInstruction(OP_t,out2,out1,PLUS_t,analyze_ExpNode(node->node.ArrayIdent->dim[i]));
                        now = out2;
                    }else now =out1;
                }
                Tag array_tag;
                array_tag.TagType = Primordial_t;
                array_tag.value.primordial = new pair<int,int>;
                array_tag.value.primordial->first = index;
                array_tag.value.primordial->second = -1;
                Tag tag1 = newtag(),tag2 = newtag();
                InsertInstruction(OP_t,tag1,now,MUL_t,Number2Tag(4));
                InsertInstruction(OP_t,tag2,array_tag,PLUS_t,tag1);
                return tag2;
            }
            else{
                int size = node->node.ArrayIdent->dim.size();
                Tag now = analyze_ExpNode(node->node.ArrayIdent->dim[0]);
                for (int i=1;i< size ;i++){
                    Tag out1 = newtag();
                    Tag out2 = newtag();
                    InsertInstruction(OP_t,out1,now,MUL_t,Number2Tag((*it).value.array->dim[i]));
                    InsertInstruction(OP_t,out2,out1,PLUS_t,analyze_ExpNode(node->node.ArrayIdent->dim[i]));
                    now = out2;
                }
                res.TagType = Primordial_t;
                res.value.primordial = new pair<int,int>;
                res.value.primordial->first = index;
                Tag tag1 = newtag();
                InsertInstruction(OP_t,tag1,now,MUL_t,Number2Tag(4));
                res.value.primordial->second = tag1.value.temporary;
                // res.value.primordial->second = now.value.temporary;
                return res;
            }
        }
    }
    printf("LVal2Tag: error\n");
    exit(0);
}

Tag analyze_PrimaryExpNode(PrimaryExpNode*exp){
    #ifdef Debug_Analyze
        printf("analyze_PrimaryExpNode:\n");
    #endif
    if (exp->PrimaryExpType == Primary_LVal_t){
        return LVal2Tag(exp->node.LVal);
    }
    else if (exp->PrimaryExpType == Primary_Number_t){
        return Number2Tag(exp->node.Number->value);
    }
    else if (exp->PrimaryExpType == Primary_Exp_t){
        return analyze_ExpNode(exp->node.Exp);
    }
    else{
        printf("analyze_PrimaryExpNode: error\n");
        exit(0);
    }
}

bool IsReturn(IdentNode*node){
    for (vector<pair<string,int> >::iterator it = FuncIsReturn.begin();it!=FuncIsReturn.end();it++){
        // printf("%s\n",(*it).first->c_str());
        if (*node->name == (*it).first)return (*it).second;
    }
    printf("IsReturn: %s\n",node->name->c_str());
    exit(0);
}

Tag analyze_UnaryExpNode(UnaryExpNode*exp){
    #ifdef Debug_Analyze
        printf("analyze_UnaryExpNode:\n");
    #endif
    if (exp->UnaryExpType == Unary_Exp_t){
        Tag S = analyze_UnaryExpNode(exp->node.UnaryExp->second);
        Tag tag = newtag();
        InsertInstruction(OP_t,tag,exp->node.UnaryExp->first->op,S);
        return tag;
    }
    else if (exp->UnaryExpType == Unary_FuncCall_t){
        FunctionCallNode* now=exp->node.FunctionCall;
        if (now->Param!=nullptr){
            vector<Tag> vec;vec.clear();
            for (vector<ExpNode*>::iterator it = now->Param->list.begin();it!=now->Param->list.end();it++){
                Tag t = analyze_ExpNode(*it);
                vec.push_back(t);//InsertInstruction(PARAM_t,t);
            }
            for (vector<Tag>::iterator it = vec.begin();it!=vec.end();it++){
                InsertInstruction(PARAM_t,(*it));
            }
        }
        // printf("%s\n",now->Ident->name->c_str());
        if (IsReturn(now->Ident)){
            Tag res = newtag();
            InsertInstruction(CALL_t,res,now->Ident);
            return res;
        }
        else{
            InsertInstruction(CALL_t,now->Ident);
            return Tag();
        }
    }
    else if (exp->UnaryExpType == Unary_Primary_t){
        return analyze_PrimaryExpNode(exp->node.PrimaryExp);
    }
    else {
        printf("analyze_UnaryExpNode: error\n");
        exit(0);
    }
}

Tag analyze_ExpNode(ExpNode*exp){
    #ifdef Debug_Analyze
        printf("analyze_ExpNode:\n");
    #endif
    if (exp->ExpType == Exp_op_t){
        Operation_t op = exp->args.ExpArgs->second->op;
        if (op == OR_t){
            Tag L = analyze_ExpNode(exp->args.ExpArgs->first.first);
            Tag tag = newtag();
            int l1 = newlabel(),l2 = newlabel();
            InsertInstruction(IFELSE_t,L,l1);
            InsertInstruction(ASSIGN_t,tag,Number2Tag(1));
            InsertInstruction(GOTO_t,l2);
            InsertInstruction(LABEL_t,l1);
            InsertInstruction(ASSIGN_t,tag,analyze_ExpNode(exp->args.ExpArgs->first.second));
            InsertInstruction(LABEL_t,l2);
            return tag;
        }
        else if (op==AND_t){
            Tag L = analyze_ExpNode(exp->args.ExpArgs->first.first);
            Tag tag = newtag();
            int l1 = newlabel(),l2 = newlabel();
            InsertInstruction(IFELSE_t,L,l1);
            InsertInstruction(ASSIGN_t,tag,analyze_ExpNode(exp->args.ExpArgs->first.second));
            InsertInstruction(GOTO_t,l2);
            InsertInstruction(LABEL_t,l1);
            InsertInstruction(ASSIGN_t,tag,Number2Tag(0));
            InsertInstruction(LABEL_t,l2);
            return tag;
        }
        else{
            Tag tag = newtag();
            Tag L = analyze_ExpNode(exp->args.ExpArgs->first.first);
            Tag R = analyze_ExpNode(exp->args.ExpArgs->first.second);
            InsertInstruction(OP_t,tag,L,op,R);
            return tag;
        }
    }
    else if (exp->ExpType == Exp_unary_t){
        return analyze_UnaryExpNode(exp->args.UnaryExpArgs);
    }
    else{
        printf("analyze_ExpNode: error\n");
        exit(0);
    }
}

void analyze_DeclsNode(DeclsNode*node){
    #ifdef Debug_Analyze
        printf("analyze_DeclsNode:\n");
    #endif
    for (vector<DeclNode*>::iterator it = node->list.begin();it!= node->list.end();it++){
        Variable item;
        if (node->DeclType == Const_t)item.DeclType = Decl_Const_t;else item.DeclType = Decl_Variable_t;
        if ((*it)->DeclNumType == One_t)item.DeclNum = Decl_One_t;else item.DeclNum = Decl_Array_t;
        if ((*it)->DeclNumType == One_t){
            Decl_OneNode* now = (*it)->DeclItem.Decl_One;
            item.name = *now->ident->name;
            if (now->exp == nullptr) item.value.value = new Variable_int();
            else {
                if (item.DeclType == Decl_Const_t){
                    int value = EvalConst(now->exp);
                    Tag tag;
                    tag.TagType = Primordial_t;
                    tag.value.primordial = new pair<int,int>;
                    tag.value.primordial->first = local.LocalVariable.size() + global.GlobalVariable.size();
                    tag.value.primordial->second = -1;
                    InsertInstruction(ASSIGN_t,tag,Number2Tag(value));
                    item.value.value = new Variable_int(value);
                }
                else {
                    Tag tag;
                    tag.TagType = Primordial_t;
                    tag.value.primordial = new pair<int,int>;
                    tag.value.primordial->first = local.LocalVariable.size() + global.GlobalVariable.size();
                    tag.value.primordial->second = -1;
                    InsertInstruction(ASSIGN_t,tag,analyze_ExpNode(now->exp));
                    item.value.value = new Variable_int();
                }
            }
            local.LocalVariable.push_back(item);
        }
        else if ((*it)->DeclNumType == Array_t){
            Decl_ArrayNode* now = (*it)->DeclItem.Decl_Array;
            item.name  = *now->name->Ident->name;
            item.value.array = new Array();
            item.value.array->total = 1;
            for (vector<ExpNode*>::iterator it=now->name->dim.begin();it!=now->name->dim.end();it++){
                int v = EvalConst(*it);
                item.value.array->dim.push_back(v);
                item.value.array->total *= v;
            }
            if (now->value != nullptr){
                if (item.DeclType == Decl_Variable_t){
                    int index = local.LocalVariable.size() + global.GlobalVariable.size();
                    int dep = 0;
                    int align = 1;
                    for (int i = dep;i<(int)item.value.array->dim.size();i++)
                        align *= item.value.array->dim[i];
                    LocalVariableArrayAssign(index,item.value.array,now->value,align,dep);
                    int s = item.value.array->value.size();
                    while (s < item.value.array->total){
                        Variable_int temp(0);
                        Tag res;
                        Tag tag = Number2Tag(item.value.array->value.size());
                        res.TagType = Primordial_t;
                        res.value.primordial = new pair<int,int>;
                        res.value.primordial->first = index;
                        res.value.primordial->second = tag.value.number;
                        InsertInstruction(ASSIGN_ARRAY_INDEX_t,res,0);
                        item.value.array->value.push_back(temp);
                        s++;
                    }
                }else if (item.DeclType == Decl_Const_t){
                    int index = local.LocalVariable.size() + global.GlobalVariable.size();
                    int dep = 0;
                    int align = 1;
                    for (int i = dep;i<(int)item.value.array->dim.size();i++)
                        align *= item.value.array->dim[i];
                    LocalVariableArrayAssignConst(index,item.value.array,now->value,align,dep);
                    
                    int s = item.value.array->value.size();
                    while (s < item.value.array->total){
                        Variable_int temp(0);
                        Tag res;
                        Tag tag = Number2Tag(item.value.array->value.size());
                        res.value.primordial = new pair<int,int>;
                        res.value.primordial->first = index;
                        res.value.primordial->second = tag.value.number;
                        InsertInstruction(ASSIGN_ARRAY_INDEX_t,res,0);
                        item.value.array->value.push_back(temp);
                        s++;
                    }
                }
            }
            else {
                int s = item.value.array->value.size();
                while (s<item.value.array->total){
                    Variable_int temp;
                    item.value.array->value.push_back(temp);
                    s++;
                }
            }
            local.LocalVariable.push_back(item);
        }
    }
    /*
    for (vector<ir::Variable>::iterator it = local.LocalVariable.begin();it!=local.LocalVariable.end();it++){
        (*it).printInfo();
    }*/
}
void analyze_IfElseStmtNode(IfElseStmtNode*node){
    
    if (node->ElseType == Else_Empty_t){
        int l1 = newlabel();
        Tag t1 = analyze_ExpNode(node->Cond);
        InsertInstruction(IFELSE_t,t1,l1);
        analyze_StmtNode(node->IfStmt);
        InsertInstruction(LABEL_t,l1);
    }
    else if (node->ElseType == Else_Incomplete_t){
        int l1 = newlabel(),l2=newlabel();
        Tag t1 = analyze_ExpNode(node->Cond);
        InsertInstruction(IFELSE_t,t1,l1);
        analyze_StmtNode(node->IfStmt);
        InsertInstruction(GOTO_t,l2);
        InsertInstruction(LABEL_t,l1);
        analyze_IfElseStmtNode(node->ElseStmt.Incomplete);
        InsertInstruction(LABEL_t,l2);
    }
    else if (node->ElseType == Else_Complete_t){
        int l1 = newlabel(),l2=newlabel();
        Tag t1 = analyze_ExpNode(node->Cond);
        InsertInstruction(IFELSE_t,t1,l1);
        analyze_StmtNode(node->IfStmt);
        InsertInstruction(GOTO_t,l2);
        InsertInstruction(LABEL_t,l1);
        analyze_StmtNode(node->ElseStmt.Complete);
        InsertInstruction(LABEL_t,l2);
    }
}
void analyze_StmtNode(StmtNode*node){
    #ifdef Debug_Analyze
        printf("analyze_StmtNode:\n");
    #endif
    switch (node->StmtType){
        case Stmt_Assign_t:{
            Tag t1 = LVal2Tag(node->node.AssignStmt->LVal);
            Tag t2 = analyze_ExpNode(node->node.AssignStmt->Exp);
            InsertInstruction(ASSIGN_t,t1,t2);
            break;
        }
        case Stmt_Empty_t:break;
        case Stmt_Exp_t:{
            analyze_ExpNode(node->node.ExpStmt->Exp);
            break;
        }
        case Stmt_While_t:{
            node->node.WhileStmt->ir_label_begin = newlabel();
            node->node.WhileStmt->ir_label_end = newlabel();
            InsertInstruction(LABEL_t,node->node.WhileStmt->ir_label_begin);
            Tag t1 = analyze_ExpNode(node->node.WhileStmt->Cond);
            InsertInstruction(IFELSE_t,t1,node->node.WhileStmt->ir_label_end);
            analyze_StmtNode(node->node.WhileStmt->Stmt);
            InsertInstruction(GOTO_t,node->node.WhileStmt->ir_label_begin);
            InsertInstruction(LABEL_t,node->node.WhileStmt->ir_label_end);

            break;
        }
        case Stmt_Break_t:{
            BaseNode* now = node->node.BreakStmt;
            while (true){
                now = now->parent;
                if (now->nodetype == WhileStmtNode_t){
                    WhileStmtNode*temp = (WhileStmtNode*)now;
                    InsertInstruction(GOTO_t,temp->ir_label_end);
                    break;
                }
            }
            break;
        }
        case Stmt_Continue_t:{
            BaseNode* now = node->node.ContinueStmt;
            while (true){
                now = now->parent;
                if (now->nodetype == WhileStmtNode_t){
                    WhileStmtNode*temp = (WhileStmtNode*)now;
                    InsertInstruction(GOTO_t,temp->ir_label_begin);
                    break;
                }
            }
            break;
        }
        case Stmt_Return_t:
            if (node->node.ReturnStmt->ReturnValue == nullptr)
            InsertInstruction(RETURN_t);
            else {
                Tag t = analyze_ExpNode(node->node.ReturnStmt->ReturnValue);
                InsertInstruction(RETURN_t,t);
            }
            break;
        case Stmt_Block_t:
            analyze_BlockItemsNode(node->node.BlockItems);
            break;
        case Stmt_IfElse_t:
            analyze_IfElseStmtNode(node->node.IfElseStmt);
            break;
            

    }
}

void analyze_BlockItemsNode(BlockItemsNode*node){
    #ifdef Debug_Analyze
        printf("analyze_BlockItemsNode:\n");
    #endif
    int top = local.LocalVariable.size();
    for (vector<BlockItemNode*>::iterator it = node->list.begin();it!=node->list.end();it++){
        if ((*it)->BlockItemType == Block_Decl_t)analyze_DeclsNode((*it)->node.Decl);
        else if ((*it)->BlockItemType == Block_Stmt_t)analyze_StmtNode((*it)->node.Stmt);
        else{
            printf("analyze_BlockItemsNode: error\n");
            exit(0);
        }
    }
    //while (local.LocalVariable.size()>top)local.LocalVariable.pop_back();
    for (int i = top;i<(int)local.LocalVariable.size();i++)
        local.LocalVariable[i].IsDelete = true;
}

void analyze_FuncDefNode(FuncDefNode*node){
    #ifdef Debug_Analyze
        printf("analyze_FuncDefNode:\n");
    #endif
    local.FuncParam.clear();
    local.LocalVariable.clear();
    if (node->Params != nullptr){
        for (vector<FuncFParamNode*>::iterator it = node->Params->list.begin();it != node->Params->list.end();it++){
            if ((*it)->FuncFParamType == FuncFParamParam_t){
                local.FuncParam.push_back(Param(*(*it)->node.param->Ident->name));
            }
            else if ((*it)->FuncFParamType == FuncFParamArray_t){
                local.FuncParam.push_back(Param(*(*it)->node.array->Ident->name,(*it)->node.array->dim));
            }
            else {
                printf("analyze_FuncDefNode: errer\n");
                exit(0);
            }
        }
    }
    analyze_BlockItemsNode(node->Block);

    if (IsReturn(node->Ident)){
        InsertInstruction(RETURN_t,Number2Tag(0));
    }
    else InsertInstruction(RETURN_t);
    InsertInstruction(FUNCTION_END,node);
    InsertInstruction(STRING_t,"\n");

    // InsertInstruction(FUNCTION_BEGIN,node);
    char function_temp[1000];
    sprintf(function_temp,"f_%s [%lu]\n",node->Ident->name->c_str(),(node->Params == nullptr)?0:node->Params->list.size());
    myprintf(function_temp);
    
    for (vector<Variable>::reverse_iterator it = local.LocalVariable.rbegin();it != local.LocalVariable.rend();it++){
        int index = local.LocalVariable.rend() - it - 1 + global.GlobalVariable.size();
        if ((*it).DeclNum == Decl_One_t){
            sprintf(function_temp,"var T%d\n",index);
            myprintf(function_temp);
        }
        else if ((*it).DeclNum == Decl_Array_t){
            sprintf(function_temp,"var %d T%d\n",4*(*it).value.array->total,index);
            myprintf(function_temp);
        }
    }

    for (int i = 0;i<tag_count;i++){
        sprintf(function_temp,"var t%d\n",i);
        myprintf(function_temp);
    }
    
    tag_count = 0;
    buf_flush();
}


void analyze(CompUnitsNode*root){
    #ifdef Debug_Analyze
        printf("analyze:\n");
    #endif
    for (vector<CompUnitNode*>::iterator it = root->list.begin();it!=root->list.end();it++){
        if ((*it)->UnitType == FuncDef_t)analyze_FuncDefNode((*it)->UnitItem.FuncDef);
    }
}

void generat_ir(CompUnitsNode*root){
    GetGlobalVariable(root);
    analyze(root);
}

void Variable::printInfo(){
    printf("Variable:");
    printf("(name: %s)",name.c_str());
    if (DeclNum == Decl_One_t)printf("(DeclNum_t: Decl_One_t)");else printf("(DeclNum_t: Decl_Array_t)");
    if (DeclType == Decl_Const_t)printf("(DeclType: Decl_Const_t)");else printf("(DeclType:Decl_Variable_t)");
    if (DeclNum == Decl_One_t)printf("(value: %d %d)\n",value.value->is_fix,value.value->value);
    else{
        value.array->printInfo();printf("\n");
    }
}
void Array::printInfo(){
    printf("(total: %d)",total);
    printf("(dim:");
    for (std::vector<int>::iterator it = dim.begin();it!=dim.end();it++)
        printf(" %d",*it);
    printf(")");
    printf("(value:");
    for (std::vector<Variable_int>::iterator it = value.begin();it!=value.end();it++)
        printf(" (%d %d)",(*it).is_fix,(*it).value);
    printf(")");
}

Param::Param(string name_){
    name = name_;
    ParamNum = Decl_One_t;
    value.e = 0;
}
Param::Param(string name_,vector<ExpNode*> dim_){
    name = name_;
    ParamNum = Decl_Array_t;
    value.dim = new vector<int>;
    value.dim->push_back(1);
    for (vector<ExpNode*>::iterator it = dim_.begin();it!=dim_.end();it++)
        value.dim->push_back(EvalConst(*it));
}
}

