enum OPType_t{
    PLUS_t,SUB_t,MUL_t,DIV_t,REM_t,LT_t,GT_t,LE_t,GE_t,AND_t,OR_t,NE_t,EQ_t,NOT_t
};
class OP_t{
public:
    OPType_t op;
    OP_t(OPType_t op):op(op){}
};