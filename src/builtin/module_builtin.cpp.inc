namespace das
{
    template <> struct SimPolicy<bool> : SimPolicy_Bool {};
    template <> struct SimPolicy<int32_t> : SimPolicy_Bin<int32_t> {};
    template <> struct SimPolicy<uint32_t> : SimPolicy_Bin<uint32_t> {};
    template <> struct SimPolicy<int64_t> : SimPolicy_Bin<int64_t> {};
    template <> struct SimPolicy<uint64_t> : SimPolicy_Bin<uint64_t> {};
    template <> struct SimPolicy<float> : SimPolicy_Float {};
	template <> struct SimPolicy<double> : SimPolicy_Double {};
    template <> struct SimPolicy<void *> : SimPolicy_Pointer {};

    // unary
    DEFINE_OP1_NUMERIC(Unp);
    DEFINE_OP1_NUMERIC(Unm);
    DEFINE_OP1_SET_NUMERIC(Inc);
    DEFINE_OP1_SET_NUMERIC(Dec);
    DEFINE_OP1_SET_NUMERIC(IncPost);
    DEFINE_OP1_SET_NUMERIC(DecPost);
    DEFINE_OP1_NUMERIC_INTEGER(BinNot);
    DEFINE_POLICY(BoolNot);
    IMPLEMENT_OP1_POLICY(BoolNot, Bool, bool);
    // binary
    // +,-,*,/,%
    DEFINE_OP2_NUMERIC(Add);
    DEFINE_OP2_NUMERIC(Sub);
    DEFINE_OP2_NUMERIC(Mul);
    DEFINE_OP2_NUMERIC(Div);
    DEFINE_OP2_NUMERIC(Mod);
    DEFINE_OP2_SET_NUMERIC(SetAdd);
    DEFINE_OP2_SET_NUMERIC(SetSub);
    DEFINE_OP2_SET_NUMERIC(SetMul);
    DEFINE_OP2_SET_NUMERIC(SetDiv);
    DEFINE_OP2_SET_NUMERIC(SetMod);
    // comparisons
    DEFINE_OP2_BOOL_NUMERIC(Equ);
    DEFINE_OP2_BOOL_NUMERIC(NotEqu);
    DEFINE_OP2_BOOL_NUMERIC(LessEqu);
    DEFINE_OP2_BOOL_NUMERIC(GtEqu);
    DEFINE_OP2_BOOL_NUMERIC(Less);
    DEFINE_OP2_BOOL_NUMERIC(Gt);
    DEFINE_OP2_FUNCTION_NUMERIC(Min);
    DEFINE_OP2_FUNCTION_NUMERIC(Max);
    DEFINE_OP2_BASIC_POLICY(Bool,bool);
    DEFINE_OP2_BASIC_POLICY(Ptr,void *);
    // binary and, or, xor
    DEFINE_OP2_NUMERIC_INTEGER(BinAnd);
    DEFINE_OP2_NUMERIC_INTEGER(BinOr);
    DEFINE_OP2_NUMERIC_INTEGER(BinXor);
    DEFINE_OP2_SET_NUMERIC_INTEGER(SetBinAnd);
    DEFINE_OP2_SET_NUMERIC_INTEGER(SetBinOr);
    DEFINE_OP2_SET_NUMERIC_INTEGER(SetBinXor);
    // boolean and, or, xor
    DEFINE_POLICY(SetBoolAnd);
    IMPLEMENT_OP2_SET_POLICY(SetBoolAnd, Bool, bool);
    DEFINE_POLICY(SetBoolOr);
    IMPLEMENT_OP2_SET_POLICY(SetBoolOr, Bool, bool);
    DEFINE_POLICY(SetBoolXor);
    IMPLEMENT_OP2_SET_POLICY(SetBoolXor, Bool, bool);
    DEFINE_POLICY(BoolXor);
    IMPLEMENT_OP2_POLICY(BoolXor, Bool, bool);

#define ADD_NUMERIC_CASTS(TYPE,CTYPE)                                                                   \
    addFunction ( make_shared<BuiltInFn<SimNode_Zero,CTYPE>>(#TYPE,lib) );                              \
    addFunction ( make_shared<BuiltInFn<SimNode_Cast<CTYPE,float>,CTYPE,float>>(#TYPE,lib) );           \
    addFunction ( make_shared<BuiltInFn<SimNode_Cast<CTYPE,double>,CTYPE,double>>(#TYPE,lib) );         \
    addFunction ( make_shared<BuiltInFn<SimNode_Cast<CTYPE,int32_t>,CTYPE,int32_t>>(#TYPE,lib) );       \
    addFunction ( make_shared<BuiltInFn<SimNode_Cast<CTYPE,uint32_t>,CTYPE,uint32_t>>(#TYPE,lib) );     \
    addFunction ( make_shared<BuiltInFn<SimNode_Cast<CTYPE,int64_t>,CTYPE,int64_t>>(#TYPE,lib) );       \
    addFunction ( make_shared<BuiltInFn<SimNode_Cast<CTYPE,uint64_t>,CTYPE,uint64_t>>(#TYPE,lib) );
}