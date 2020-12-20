#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/simulate/simulate_nodes.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/aot_builtin_math.h"
#include "daScript/simulate/simulate_visit_op.h"
#include "daScript/misc/performance_time.h"

namespace das {
#define MATH_FUN_OP1(fun)\
      DEFINE_POLICY(fun);\
      IMPLEMENT_OP1_FUNCTION_POLICY(fun,Float,float);\
      IMPLEMENT_OP1_EVAL_FUNCTION_POLICY(fun, float2);     \
      IMPLEMENT_OP1_EVAL_FUNCTION_POLICY(fun, float3);     \
      IMPLEMENT_OP1_EVAL_FUNCTION_POLICY(fun, float4);

#define MATH_FUN_OP1_INT(fun)\
      DEFINE_POLICY(fun);\
      IMPLEMENT_OP1_FUNCTION_POLICY_EX(fun,Int,int32_t,Float,float);\
      IMPLEMENT_OP1_EVAL_FUNCTION_POLICY(fun, float2);     \
      IMPLEMENT_OP1_EVAL_FUNCTION_POLICY(fun, float3);     \
      IMPLEMENT_OP1_EVAL_FUNCTION_POLICY(fun, float4);

#define MATH_FUN_OP2(fun)\
      DEFINE_POLICY(fun);\
      IMPLEMENT_OP2_FUNCTION_POLICY(fun,Float,float);\
      IMPLEMENT_OP2_EVAL_FUNCTION_POLICY(fun, float2);     \
      IMPLEMENT_OP2_EVAL_FUNCTION_POLICY(fun, float3);     \
      IMPLEMENT_OP2_EVAL_FUNCTION_POLICY(fun, float4);

#define MATH_FUN_OP2I(fun)\
      IMPLEMENT_OP2_FUNCTION_POLICY(fun,Int,int32_t);\
      IMPLEMENT_OP2_EVAL_FUNCTION_POLICY(fun, int2);     \
      IMPLEMENT_OP2_EVAL_FUNCTION_POLICY(fun, int3);     \
      IMPLEMENT_OP2_EVAL_FUNCTION_POLICY(fun, int4);

#define MATH_FUN_OP3I(fun)\
      IMPLEMENT_OP3_FUNCTION_POLICY(fun,Int,int32_t);\
      IMPLEMENT_OP3_EVAL_FUNCTION_POLICY(fun, int2);     \
      IMPLEMENT_OP3_EVAL_FUNCTION_POLICY(fun, int3);     \
      IMPLEMENT_OP3_EVAL_FUNCTION_POLICY(fun, int4);

#define MATH_FUN_OP3(fun)\
      DEFINE_POLICY(fun);\
      IMPLEMENT_OP3_FUNCTION_POLICY(fun,Float,float);\
      IMPLEMENT_OP3_EVAL_FUNCTION_POLICY(fun, float2);     \
      IMPLEMENT_OP3_EVAL_FUNCTION_POLICY(fun, float3);     \
      IMPLEMENT_OP3_EVAL_FUNCTION_POLICY(fun, float4);

#define MATH_FUN_OP2A(fun)                              \
    MATH_FUN_OP2(fun);                                  \
    MATH_FUN_OP2I(fun);                                 \
    IMPLEMENT_OP2_FUNCTION_POLICY(fun,UInt,uint32_t);   \
    IMPLEMENT_OP2_FUNCTION_POLICY(fun,Int64,int64_t);   \
    IMPLEMENT_OP2_FUNCTION_POLICY(fun,UInt64,uint64_t); \
    IMPLEMENT_OP2_FUNCTION_POLICY(fun,Double,double);

#define MATH_FUN_OP3A(fun)                              \
    MATH_FUN_OP3(fun);                                  \
    MATH_FUN_OP3I(fun);                                 \
    IMPLEMENT_OP3_FUNCTION_POLICY(fun,UInt,uint32_t);   \
    IMPLEMENT_OP3_FUNCTION_POLICY(fun,Int64,int64_t);   \
    IMPLEMENT_OP3_FUNCTION_POLICY(fun,UInt64,uint64_t); \
    IMPLEMENT_OP3_FUNCTION_POLICY(fun,Double,double);

    // everything
    MATH_FUN_OP2A(Min)
    MATH_FUN_OP2A(Max)
    MATH_FUN_OP3A(Clamp)

    //common
    MATH_FUN_OP1(Abs)
    MATH_FUN_OP1(Floor)
    MATH_FUN_OP1(Ceil)
    MATH_FUN_OP1(Sqrt)
    MATH_FUN_OP1(RSqrt)
    MATH_FUN_OP1(RSqrtEst)
    MATH_FUN_OP1(Sat)
    MATH_FUN_OP3(Mad)
    MATH_FUN_OP3(Lerp)

    MATH_FUN_OP1_INT(Trunci)
    MATH_FUN_OP1_INT(Floori)
    MATH_FUN_OP1_INT(Ceili)
    MATH_FUN_OP1_INT(Roundi)

    //exp
    MATH_FUN_OP1(Exp)
    MATH_FUN_OP1(Log)
    MATH_FUN_OP1(Exp2)
    MATH_FUN_OP1(Log2)
    MATH_FUN_OP2(Pow)
    MATH_FUN_OP1(Rcp)
    MATH_FUN_OP1(RcpEst)

    //trig
    MATH_FUN_OP1(Sin)
    MATH_FUN_OP1(Cos)
    MATH_FUN_OP1(Tan)
    MATH_FUN_OP1(ATan)
    MATH_FUN_OP1(ASin)
    MATH_FUN_OP1(ACos)

    MATH_FUN_OP2(ATan2)
    MATH_FUN_OP2(ATan2_est)

    DEFINE_POLICY(MadS)     // vector_a*scalar_b + vector_c
    IMPLEMENT_OP3_EVAL_FUNCTION_POLICY(MadS,float2);
    IMPLEMENT_OP3_EVAL_FUNCTION_POLICY(MadS,float3);
    IMPLEMENT_OP3_EVAL_FUNCTION_POLICY(MadS,float4);

    // trig types
    template <typename TT>
    void addFunctionTrig(Module & mod, const ModuleLibrary & lib) {
        //                                     policy              ret   arg1 arg2     name
        mod.addFunction( make_smart<BuiltInFn<Sim_Sin<TT>,        TT,   TT>        >("sin",       lib, "Sin")->arg("x") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Cos<TT>,        TT,   TT>        >("cos",       lib, "Cos")->arg("x") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Tan<TT>,        TT,   TT>        >("tan",       lib, "Tan")->arg("x") );
        mod.addFunction( make_smart<BuiltInFn<Sim_ATan<TT>,       TT,   TT>        >("atan",      lib, "ATan")->arg("x") );
        mod.addFunction( make_smart<BuiltInFn<Sim_ASin<TT>,       TT,   TT>        >("asin",      lib, "ASin")->arg("x") );
        mod.addFunction( make_smart<BuiltInFn<Sim_ACos<TT>,       TT,   TT>        >("acos",      lib, "ACos")->arg("x") );
        mod.addFunction( make_smart<BuiltInFn<Sim_ATan2<TT>,      TT,   TT,  TT>   >("atan2",     lib, "ATan2")->args({"y","x"}) );
        mod.addFunction( make_smart<BuiltInFn<Sim_ATan2_est<TT>,  TT,   TT,  TT>   >("atan2_est", lib, "ATan2_est")->args({"y","x"}) );
    }

    template <typename TT>
    void addFunctionCommonTyped(Module & mod, const ModuleLibrary & lib) {
        mod.addFunction( make_smart<BuiltInFn<Sim_Min <TT>, TT,   TT,   TT>      >("min",   lib, "Min")->args({"x","y"}) );
        mod.addFunction( make_smart<BuiltInFn<Sim_Max <TT>, TT,   TT,   TT>      >("max",   lib, "Max")->args({"x","y"}) );
        mod.addFunction( make_smart<BuiltInFn<Sim_Clamp<TT>,TT,   TT,   TT,  TT> >("clamp", lib, "Clamp")->args({"t","a","b"}) );
    }

    template <typename TT>
    void addFunctionCommon(Module & mod, const ModuleLibrary & lib) {
        //                                     policy            ret   arg1     name
        mod.addFunction( make_smart<BuiltInFn<Sim_Abs<TT>,      TT,   TT>   >("abs",         lib, "Abs")->arg("x") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Floor<TT>,    TT,   TT>   >("floor",       lib, "Floor")->arg("x") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Ceil<TT>,     TT,   TT>   >("ceil",        lib, "Ceil")->arg("x") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Sqrt<TT>,     TT,   TT>   >("sqrt",        lib, "Sqrt")->arg("x") );
        mod.addFunction( make_smart<BuiltInFn<Sim_RSqrt<TT>,    TT,   TT>   >("rsqrt",       lib, "RSqrt")->arg("x") );
        mod.addFunction( make_smart<BuiltInFn<Sim_RSqrtEst<TT>, TT,   TT>   >("rsqrt_est",   lib, "RSqrtEst")->arg("x") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Sat<TT>,      TT,   TT>   >("saturate",    lib, "Sat")->arg("x") );
    }
    template <typename Ret, typename TT>
    void addFunctionCommonConversion(Module & mod, const ModuleLibrary & lib) {
        //                                     policy          ret    arg1     name
        mod.addFunction( make_smart<BuiltInFn<Sim_Floori<TT>, Ret,   TT>   >("floori",  lib, "Floori")->arg("x") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Ceili <TT>, Ret,   TT>   >("ceili",   lib, "Ceili")->arg("x") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Roundi<TT>, Ret,   TT>   >("roundi",  lib, "Roundi")->arg("x") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Trunci<TT>, Ret,   TT>   >("trunci",  lib, "Trunci")->arg("x") );
    }

    template <typename TT>
    void addFunctionPow(Module & mod, const ModuleLibrary & lib) {
        //                                     policy           ret   arg1   name
        mod.addFunction( make_smart<BuiltInFn<Sim_Exp<TT>,     TT,   TT> >("exp",      lib, "Exp")->arg("x") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Log<TT>,     TT,   TT> >("log",      lib, "Log")->arg("x") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Exp2<TT>,    TT,   TT> >("exp2",     lib, "Exp2")->arg("x") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Log2<TT>,    TT,   TT> >("log2",     lib, "Log2")->arg("x") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Rcp<TT>,     TT,   TT> >("rcp",      lib, "Rcp")->arg("x") );
        mod.addFunction( make_smart<BuiltInFn<Sim_RcpEst<TT>,  TT,   TT> >("rcp_est",  lib, "RcpEst")->arg("x") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Pow<TT>,  TT,   TT,   TT> >("pow",   lib, "Pow")->args({"x","y"}) );
    }

    template <typename TT>
    void addFunctionOp3(Module & mod, const ModuleLibrary & lib) {
        //                                     policy         ret arg1 arg2 arg3   name
        mod.addFunction( make_smart<BuiltInFn<Sim_Mad<TT>,   TT, TT,  TT,  TT> >("mad",   lib, "Mad")->args({"a","b","c"}) );
        mod.addFunction( make_smart<BuiltInFn<Sim_Lerp<TT>,  TT, TT,  TT,  TT> >("lerp",  lib, "Lerp")->args({"a","b","t"}) );
    }

    template <typename VecT, int RowC>
    class MatrixAnnotation : public TypeAnnotation {
        enum { ColC = sizeof(VecT) / sizeof(float) };
        typedef MatrixAnnotation<VecT,RowC> ThisAnnotation;
        typedef Matrix<VecT,RowC> ThisMatrix;
    protected:
        DebugInfoHelper            helpA;
        TypeInfo *                 matrixTypeInfo;
    protected:
        int GetField ( const string & na ) const {
            if ( na.length()!=1 )
                return -1;
            int field = TypeDecl::getMaskFieldIndex(na[0]);
            if ( field<0 || field>=RowC )
                return -1;
            return field;
        }
    public:
        MatrixAnnotation() : TypeAnnotation( "float" + to_string(ColC) + "x" + to_string(RowC) ) {
            matrixTypeInfo = helpA.debugInfo->makeNode<TypeInfo>();
            auto bt = ToBasicType<VecT>::type;
            auto tt = make_smart<TypeDecl>(Type(bt));
            tt->dim.push_back(RowC);
            helpA.makeTypeInfo(matrixTypeInfo, tt);
        }
        virtual TypeAnnotationPtr clone ( const TypeAnnotationPtr & p = nullptr ) const override {
            smart_ptr<ThisAnnotation> cp =  p ? static_pointer_cast<ThisAnnotation>(p) : make_smart<ThisAnnotation>();
            return TypeAnnotation::clone(cp);
        }
        virtual bool rtti_isHandledTypeAnnotation() const override {
            return true;
        }
        virtual bool isRefType() const override { return true; }
        virtual bool isLocal() const override { return true; }
        virtual bool canBePlacedInContainer() const override { return true; }
        virtual bool hasNonTrivialCtor() const override { return false; }
        virtual bool hasNonTrivialDtor() const override { return false; }
        virtual bool canMove() const override { return true; }
        virtual bool canCopy() const override { return true; }
        virtual bool canClone() const override { return true; }
        virtual bool canNew() const override { return true; }
        virtual bool isPod() const override { return true; }
        virtual bool isRawPod() const override { return true; }
        virtual bool isIndexable ( const TypeDeclPtr & decl ) const override {
            return decl->isIndex();
        }
        virtual size_t getAlignOf() const override {
            return alignof(VecT);
        }
        virtual size_t getSizeOf() const override {
            return sizeof(ThisMatrix);
        }
        virtual TypeDeclPtr makeFieldType ( const string & na, bool isConst ) const override {
            if ( auto ft = makeSafeFieldType(na, isConst) ) {
                ft->ref = true;
                return ft;
            } else {
                return nullptr;
            }
        }
        virtual TypeDeclPtr makeSafeFieldType ( const string & na, bool ) const override {
            int field = GetField(na);
            if ( field<0  )
                return nullptr;
            auto bt = TypeDecl::getVectorType(Type::tFloat, ColC);
            return make_smart<TypeDecl>(bt);
        }
        virtual TypeDeclPtr makeIndexType ( const ExpressionPtr &, const ExpressionPtr & idx ) const override {
            auto decl = idx->type;
            if ( !decl->isIndex() ) return nullptr;
            auto bt = TypeDecl::getVectorType(Type::tFloat, ColC);
            auto pt = make_smart<TypeDecl>(bt);
            pt->ref = true;
            return pt;
        }
        // aot
        virtual void aotVisitGetField ( TextWriter & ss, const string & fieldName ) override {
            ss << ".m[" << TypeDecl::getMaskFieldIndex(fieldName[0]) << "]";
        }
        virtual void aotVisitGetFieldPtr ( TextWriter & ss, const string & fieldName ) override {
            ss << "->m[" << TypeDecl::getMaskFieldIndex(fieldName[0]) << "]";
        }
        // simulate
        virtual SimNode * simulateCopy ( Context & context, const LineInfo & at, SimNode * l, SimNode * r ) const override {
            return context.code->makeNode<SimNode_CopyRefValue>(at, l, r, uint32_t(sizeof(ThisMatrix)));
        }
        virtual SimNode * simulateClone ( Context & context, const LineInfo & at, SimNode * l, SimNode * r ) const override {
            return context.code->makeNode<SimNode_CopyRefValue>(at, l, r, uint32_t(sizeof(ThisMatrix)));
        }
        virtual SimNode * simulateGetField ( const string & na, Context & context,
                                            const LineInfo & at, const ExpressionPtr & value ) const override {
            int field = GetField(na);
            if ( field!=-1 ) {
                if ( !value->type->isPointer() ) {
                    auto tnode = value->trySimulate(context, field*sizeof(VecT), make_smart<TypeDecl>(Type::none));
                    if ( tnode ) {
                        return tnode;
                    }
                }
                return context.code->makeNode<SimNode_FieldDeref>(at,
                                                                  value->simulate(context),
                                                                  uint32_t(field*sizeof(VecT)));
            } else {
                return nullptr;
            }
        }
        virtual SimNode * simulateGetFieldR2V ( const string & na, Context & context,
                                               const LineInfo & at, const ExpressionPtr & value ) const override {
            int field = GetField(na);
            if ( field!=-1 ) {
                auto bt = TypeDecl::getVectorType(Type::tFloat, ColC);
                if ( !value->type->isPointer() ) {
                    auto tnode = value->trySimulate(context, field*sizeof(VecT), make_smart<TypeDecl>(bt));
                    if ( tnode ) {
                        return tnode;
                    }
                }
                return context.code->makeValueNode<SimNode_FieldDerefR2V>(bt,
                                                                          at,
                                                                          value->simulate(context),
                                                                          uint32_t(field*sizeof(VecT)));
            } else {
                return nullptr;
            }
        }
        virtual SimNode * simulateGetNew ( Context & context, const LineInfo & at ) const override {
            return context.code->makeNode<SimNode_New>(at,int32_t(sizeof(ThisMatrix)),false);
        }
        virtual SimNode * simulateDeletePtr ( Context & context, const LineInfo & at, SimNode * sube, uint32_t count ) const override {
            uint32_t ms = uint32_t(sizeof(ThisMatrix));
            return context.code->makeNode<SimNode_DeleteStructPtr>(at,sube,count,ms, false, false);
        }
        virtual SimNode * simulateSafeGetField ( const string & na, Context & context,
                                                const LineInfo & at, const ExpressionPtr & value ) const override {
            int field = GetField(na);
            if ( field!=-1 ) {
                return context.code->makeNode<SimNode_SafeFieldDeref>(at,
                                                                      value->simulate(context),
                                                                      uint32_t(field*sizeof(VecT)));
            } else {
                return nullptr;
            }
        };
        virtual SimNode * simulateSafeGetFieldPtr ( const string & na, Context & context,
                                                   const LineInfo & at, const ExpressionPtr & value ) const override {
            int field = GetField(na);
            if ( field!=-1 ) {
                return context.code->makeNode<SimNode_SafeFieldDerefPtr>(at,
                                                                         value->simulate(context),
                                                                         uint32_t(field*sizeof(VecT)));
            } else {
                return nullptr;
            }
        };
        SimNode * trySimulate ( Context & context, const ExpressionPtr & subexpr, const ExpressionPtr & index,
                               const TypeDeclPtr & r2vType, uint32_t ofs ) const {
            if ( index->rtti_isConstant() ) {
                // if its constant index, like a[3]..., we try to let node bellow simulate
                auto idxCE = static_pointer_cast<ExprConst>(index);
                uint32_t idxC = cast<uint32_t>::to(idxCE->value);
                if ( idxC >= RowC ) {
                    context.thisProgram->error("matrix index out of range", "", "",
                        subexpr->at, CompilationError::index_out_of_range);
                    return nullptr;
                }
                uint32_t stride = sizeof(float)*ColC;
                auto tnode = subexpr->trySimulate(context, idxC*stride + ofs, r2vType);
                if ( tnode ) {
                    return tnode;
                }
            }
            return nullptr;
        }
        virtual SimNode * simulateGetAt ( Context & context, const LineInfo & at, const TypeDeclPtr &,
                                         const ExpressionPtr & rv, const ExpressionPtr & idx, uint32_t ofs ) const override {
            if ( auto tnode = trySimulate(context, rv, idx, make_smart<TypeDecl>(Type::none), ofs) ) {
                return tnode;
            } else {
                return context.code->makeNode<SimNode_At>(at,
                                                          rv->simulate(context),
                                                          idx->simulate(context),
                                                          uint32_t(sizeof(float)*ColC), ofs, RowC);
            }
        }
        virtual SimNode * simulateGetAtR2V ( Context & context, const LineInfo & at, const TypeDeclPtr &,
                                            const ExpressionPtr & rv, const ExpressionPtr & idx, uint32_t ofs ) const override {
            Type r2vType = (Type) ToBasicType<VecT>::type;
            if ( auto tnode = trySimulate(context, rv, idx, make_smart<TypeDecl>(r2vType), ofs) ) {
                return tnode;
            } else {
                return context.code->makeValueNode<SimNode_AtR2V>(  r2vType, at,
                                                                    rv->simulate(context),
                                                                    idx->simulate(context),
                                                                    uint32_t(sizeof(float)*ColC), ofs, RowC);
            }
        }
        virtual void walk ( DataWalker & walker, void * data ) override {
            walker.walk((char *)data, matrixTypeInfo);
        }
    };

    typedef MatrixAnnotation<float4,4> float4x4_ann;
    typedef MatrixAnnotation<float3,4> float3x4_ann;

    template <typename MatT>
    struct SimNode_MatrixCtor : SimNode_CallBase {
        SimNode_MatrixCtor(const LineInfo & at) : SimNode_CallBase(at) {}
        virtual SimNode * visit ( SimVisitor & vis ) override {
            V_BEGIN();
            V_OP(MatrixCtor);
            V_CALL();
            V_END();
        }
        virtual vec4f eval(Context & context) override {
            auto cmres = cmresEval->evalPtr(context);
            memset ( cmres, 0, sizeof(MatT) );
            return cast<void *>::from(cmres);
        }
    };

    template <int r, int c>
    __forceinline void matrix_identity ( float * mat ) {
        for ( int y=0; y!=r; ++y ) {
            for ( int x=0; x!=c; ++x ) {
                *mat++ = x==y ? 1.0f : 0.0f;
            }
        }
    }

    void float4x4_identity ( float4x4 & mat ) {
        matrix_identity<4,4>((float*)&mat);
    }

    void float3x4_identity ( float3x4 & mat ) {
        matrix_identity<4,3>((float*)&mat);
    }

    float4x4 float4x4_translation(float3 xyz) {
        float4x4 mat;
        matrix_identity<4,4>((float*)&mat);
        mat.m[3].x = xyz.x;
        mat.m[3].y = xyz.y;
        mat.m[3].z = xyz.z;
        return mat;
    }

    float3x4 float3x4_mul(const float3x4 &a, const float3x4 &b) {
        //not working yet!
        mat44f va,vb,vc;
        v_mat44_make_from_43cu(va, &a.m[0].x);
        v_mat44_make_from_43cu(vb, &b.m[0].x);
        v_mat44_mul43(vc, va, vb);
        alignas(16) float3x4 ret;
        v_mat_43ca_from_mat44(&ret.m[0].x, vc);
        return ret;
    }

    float4x4 float4x4_mul(const float4x4 &a, const float4x4 &b) {
        mat44f va,vb,res;
        memcpy(&va,&a,sizeof(float4x4));
        memcpy(&vb,&b,sizeof(float4x4));
        v_mat44_mul(res,va,vb);
        return reinterpret_cast<float4x4&>(res);;
    }

    float4x4 float4x4_transpose ( const float4x4 & src ) {
        mat44f res;
        memcpy ( &res, &src, sizeof(float4x4) );
        v_mat44_transpose(res.col0, res.col1, res.col2, res.col3);
        return reinterpret_cast<float4x4&>(res);
    }

    float3x4 float3x4_inverse ( const float3x4 & src ) {
        mat44f mat, invMat;
        v_mat44_make_from_43cu(mat, &src.m[0].x);
        v_mat44_inverse43(invMat, mat);
        alignas(16) float3x4 ret;
        v_mat_43ca_from_mat44(&ret.m[0].x, invMat);
        return ret;
    }

    class Module_Math : public Module {
    public:
        Module_Math() : Module("math") {
            DAS_PROFILE_SECTION("Module_Math");
            ModuleLibrary lib;
            lib.addModule(this);
            lib.addBuiltInModule();
            // constants
            addConstant(*this,"PI",(float)M_PI);
            addConstant(*this,"FLT_EPSILON",FLT_EPSILON);
            // trigonometry functions
            addFunctionTrig<float>(*this, lib);
            addFunctionTrig<float2>(*this,lib);
            addFunctionTrig<float3>(*this,lib);
            addFunctionTrig<float4>(*this,lib);
            // exp functions
            addFunctionPow<float>(*this, lib);
            addFunctionPow<float2>(*this,lib);
            addFunctionPow<float3>(*this,lib);
            addFunctionPow<float4>(*this,lib);
            // op3
            addFunctionOp3<float >(*this,lib);
            addFunctionOp3<float2>(*this,lib);
            addFunctionOp3<float3>(*this,lib);
            addFunctionOp3<float4>(*this,lib);
            addFunction( make_smart<BuiltInFn<Sim_MadS<float2>,   float2, float2,  float,  float2> >("mad", lib, "MadS")->args({"a","b","c"}) );
            addFunction( make_smart<BuiltInFn<Sim_MadS<float3>,   float3, float3,  float,  float3> >("mad", lib, "MadS")->args({"a","b","c"}) );
            addFunction( make_smart<BuiltInFn<Sim_MadS<float4>,   float4, float4,  float,  float4> >("mad", lib, "MadS")->args({"a","b","c"}) );
            //common
            addFunctionCommon<float>(*this, lib);
            addFunctionCommon<float2>(*this,lib);
            addFunctionCommon<float3>(*this,lib);
            addFunctionCommon<float4>(*this,lib);
            addFunctionCommonTyped<int32_t>(*this, lib);
            addFunctionCommonTyped<int2>(*this, lib);
            addFunctionCommonTyped<int3>(*this, lib);
            addFunctionCommonTyped<int4>(*this, lib);
            addFunctionCommonTyped<uint32_t>(*this, lib);
            //addFunctionCommonTyped<uint2>(*this, lib);
            //addFunctionCommonTyped<uint3>(*this, lib);
            //addFunctionCommonTyped<uint4>(*this, lib);
            addFunctionCommonTyped<float>(*this, lib);
            addFunctionCommonTyped<float2>(*this, lib);
            addFunctionCommonTyped<float3>(*this, lib);
            addFunctionCommonTyped<float4>(*this, lib);
            addFunctionCommonTyped<double>(*this, lib);
            addFunctionCommonTyped<int64_t>(*this, lib);
            addFunctionCommonTyped<uint64_t>(*this, lib);
            addExtern<DAS_BIND_FUN(uint32_hash)>(*this, lib, "uint32_hash", SideEffects::none, "uint32_hash")->arg("seed");
            addExtern<DAS_BIND_FUN(uint_noise1D)>(*this, lib, "uint_noise_1D", SideEffects::none, "uint_noise1D")->args({"position","seed"});
            addExtern<DAS_BIND_FUN(uint_noise2D_int2)>(*this, lib, "uint_noise_2D", SideEffects::none, "uint_noise2D_int2")->args({"position","seed"});
            addExtern<DAS_BIND_FUN(uint_noise3D_int3)>(*this, lib, "uint_noise_3D", SideEffects::none, "uint_noise3D_int3")->args({"position","seed"});
            addExternEx<float(float2,float2),DAS_BIND_FUN(dot2)>(*this, lib, "dot", SideEffects::none, "dot2")->args({"x","y"});
            addExternEx<float(float3,float3),DAS_BIND_FUN(dot3)>(*this, lib, "dot", SideEffects::none, "dot3")->args({"x","y"});
            addExternEx<float(float4,float4),DAS_BIND_FUN(dot4)>(*this, lib, "dot", SideEffects::none, "dot4")->args({"x","y"});
            addExternEx<float3(float3,float3),DAS_BIND_FUN(cross3)>(*this, lib, "cross", SideEffects::none, "cross3")->args({"x","y"});
            addExternEx<float2(float2),DAS_BIND_FUN(normalize2)>(*this, lib, "fast_normalize", SideEffects::none, "normalize2")->arg("x");
            addExternEx<float3(float3),DAS_BIND_FUN(normalize3)>(*this, lib, "fast_normalize", SideEffects::none, "normalize3")->arg("x");
            addExternEx<float4(float4),DAS_BIND_FUN(normalize4)>(*this, lib, "fast_normalize", SideEffects::none, "normalize4")->arg("x");
            addExternEx<float2(float2),DAS_BIND_FUN(safe_normalize2)>(*this, lib, "normalize", SideEffects::none, "safe_normalize2")->arg("x");
            addExternEx<float3(float3),DAS_BIND_FUN(safe_normalize3)>(*this, lib, "normalize", SideEffects::none, "safe_normalize3")->arg("x");
            addExternEx<float4(float4),DAS_BIND_FUN(safe_normalize4)>(*this, lib, "normalize", SideEffects::none, "safe_normalize4")->arg("x");
            addExternEx<float(float2),DAS_BIND_FUN(length2)>(*this, lib, "length", SideEffects::none, "length2")->arg("x");
            addExternEx<float(float3),DAS_BIND_FUN(length3)>(*this, lib, "length", SideEffects::none, "length3")->arg("x");
            addExternEx<float(float4),DAS_BIND_FUN(length4)>(*this, lib, "length", SideEffects::none, "length4")->arg("x");
            addExternEx<float(float2),DAS_BIND_FUN(invlength2)>(*this, lib, "inv_length", SideEffects::none,"invlength2")->arg("x");
            addExternEx<float(float3),DAS_BIND_FUN(invlength3)>(*this, lib, "inv_length", SideEffects::none,"invlength3")->arg("x");
            addExternEx<float(float4),DAS_BIND_FUN(invlength4)>(*this, lib, "inv_length", SideEffects::none,"invlength4")->arg("x");
            addExternEx<float(float2),DAS_BIND_FUN(invlengthSq2)>(*this, lib, "inv_length_sq", SideEffects::none, "invlengthSq2")->arg("x");
            addExternEx<float(float3),DAS_BIND_FUN(invlengthSq3)>(*this, lib, "inv_length_sq", SideEffects::none, "invlengthSq3")->arg("x");
            addExternEx<float(float4),DAS_BIND_FUN(invlengthSq4)>(*this, lib, "inv_length_sq", SideEffects::none, "invlengthSq4")->arg("x");
            addExternEx<float(float2),DAS_BIND_FUN(lengthSq2)>(*this, lib, "length_sq", SideEffects::none, "lengthSq2")->arg("x");
            addExternEx<float(float3),DAS_BIND_FUN(lengthSq3)>(*this, lib, "length_sq", SideEffects::none, "lengthSq3")->arg("x");
            addExternEx<float(float4),DAS_BIND_FUN(lengthSq4)>(*this, lib, "length_sq", SideEffects::none, "lengthSq4")->arg("x");
            addExternEx<float(float3,float3),DAS_BIND_FUN(distance3)>(*this, lib, "distance", SideEffects::none, "distance3")->args({"x","y"});
            addExternEx<float(float3,float3),DAS_BIND_FUN(distanceSq3)>(*this, lib, "distance_sq", SideEffects::none, "distanceSq3")->args({"x","y"});
            addExternEx<float(float3,float3),DAS_BIND_FUN(invdistance3)>(*this, lib, "inv_distance", SideEffects::none, "invdistance3")->args({"x","y"});
            addExternEx<float(float3,float3),DAS_BIND_FUN(invdistanceSq3)>(*this, lib, "inv_distance_sq", SideEffects::none, "invdistanceSq3")->args({"x","y"});
            // unique float functions
            addExtern<DAS_BIND_FUN(fisnan)>(*this, lib, "is_nan",SideEffects::none, "fisnan")->arg("x");
            //double functions
            addExtern<DAS_BIND_FUN(disnan)>(*this, lib, "is_nan",SideEffects::none, "disnan")->arg("x");
            addExtern<DAS_BIND_FUN(dabs)>(*this, lib, "abs",     SideEffects::none, "dabs")->arg("x");
            addExtern<DAS_BIND_FUN(dsqrt)>(*this, lib, "sqrt",   SideEffects::none, "dsqrt")->arg("x");
            addExtern<DAS_BIND_FUN(dexp)>(*this, lib, "exp",     SideEffects::none, "dexp")->arg("x");
            addExtern<DAS_BIND_FUN(drcp)>(*this, lib, "rcp",     SideEffects::none, "drcp")->arg("x");
            addExtern<DAS_BIND_FUN(dlog)>(*this, lib, "log",     SideEffects::none, "dlog")->arg("x");
            addExtern<DAS_BIND_FUN(dpow)>(*this, lib, "pow",     SideEffects::none, "dpow")->args({"x","y"});
            addExtern<DAS_BIND_FUN(dexp2)>(*this, lib, "exp2",   SideEffects::none, "dexp2")->arg("x");
            addExtern<DAS_BIND_FUN(dlog2)>(*this, lib, "log2",   SideEffects::none, "dlog2")->arg("x");
            addExtern<DAS_BIND_FUN(dsin)>(*this, lib, "sin",     SideEffects::none, "dsin")->arg("x");
            addExtern<DAS_BIND_FUN(dcos)>(*this, lib, "cos",     SideEffects::none, "dcos")->arg("x");
            addExtern<DAS_BIND_FUN(dasin)>(*this, lib, "asin",   SideEffects::none, "dasin")->arg("x");
            addExtern<DAS_BIND_FUN(dacos)>(*this, lib, "acos",   SideEffects::none, "dacos")->arg("x");
            addExtern<DAS_BIND_FUN(dtan)>(*this, lib, "tan",     SideEffects::none, "dtan")->arg("x");
            addExtern<DAS_BIND_FUN(datan)>(*this, lib, "atan",   SideEffects::none, "datan")->arg("x");
            addExtern<DAS_BIND_FUN(datan2)>(*this, lib, "atan2", SideEffects::none, "datan2")->args({"y","x"});
            addExtern<DAS_BIND_FUN(sincosF)>(*this, lib, "sincos", SideEffects::modifyArgument, "sincosF")->args({"x","s","c"});
            addExtern<DAS_BIND_FUN(sincosD)>(*this, lib, "sincos", SideEffects::modifyArgument, "sincosD")->args({"x","s","c"});
            addExternEx<float3(float3,float3),DAS_BIND_FUN(reflect)>(*this, lib, "reflect",
                SideEffects::none, "reflect")->args({"v","n"});
            addExternEx<bool(float3,float3,float,float3&),DAS_BIND_FUN(refract)>(*this, lib, "refract",
                SideEffects::modifyArgument, "refract")->args({"v","n","nint","outRefracted"});
            addFunctionCommonConversion<int, float>  (*this, lib);
            addFunctionCommonConversion<int2, float2>(*this,lib);
            addFunctionCommonConversion<int3, float3>(*this,lib);
            addFunctionCommonConversion<int4, float4>(*this,lib);
            // structure annotations
            addAnnotation(make_smart<float4x4_ann>());
            addAnnotation(make_smart<float3x4_ann>());
            // c-tor
            addFunction ( make_smart< BuiltInFn< SimNode_MatrixCtor<float3x4>,float3x4 > >("float3x4",lib) );
            addFunction ( make_smart< BuiltInFn< SimNode_MatrixCtor<float4x4>,float4x4 > >("float4x4",lib) );
            // 4x4
            addExtern<DAS_BIND_FUN(float4x4_identity)>(*this, lib, "identity",
                SideEffects::modifyArgument, "float4x4_identity")->arg("x");
            addExtern<DAS_BIND_FUN(float4x4_translation), SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "translation",
                 SideEffects::none, "float_4x4_translation")->arg("xyz");
            addExtern<DAS_BIND_FUN(float4x4_transpose), SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "transpose",
                SideEffects::none, "float4x4_transpose")->arg("x");
            addExtern<DAS_BIND_FUN(float4x4_mul), SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "*",
                SideEffects::none,"float4x4_mul")->args({"x", "y"});
            addExtern<DAS_BIND_FUN(float4x4_equ)>(*this, lib, "==",
                SideEffects::none, "float4x4_equ")->args({"x","y"});
            addExtern<DAS_BIND_FUN(float4x4_nequ)>(*this, lib, "!=",
                SideEffects::none, "float4x4_nequ")->args({"x","y"});
            // 3x4
            addExtern<DAS_BIND_FUN(float3x4_identity)>(*this, lib, "identity",
                SideEffects::modifyArgument,"float3x4_identity")->arg("x");
            addExtern<DAS_BIND_FUN(float3x4_mul), SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "*",
                SideEffects::none,"float3x4_mul")->args({"x","y"});
            addExtern<DAS_BIND_FUN(float3x4_mul_vec3p), SimNode_ExtFuncCall>(*this, lib, "*",
                SideEffects::none,"float3x4_mul_vec3p")->args({"x","y"});
            addExtern<DAS_BIND_FUN(float3x4_inverse), SimNode_ExtFuncCallAndCopyOrMove>(*this, lib,
                "inverse", SideEffects::none, "float3x4_inverse")->arg("x");
            addExtern<DAS_BIND_FUN(rotate)>(*this, lib, "rotate",
                SideEffects::none, "rotate")->args({"x","y"});
            // packing
            addExtern<DAS_BIND_FUN(pack_float_to_byte)>(*this, lib, "pack_float_to_byte",
                SideEffects::none,"pack_float_to_byte")->arg("x");
            addExtern<DAS_BIND_FUN(unpack_byte_to_float)>(*this, lib, "unpack_byte_to_float",
                SideEffects::none,"unpack_byte_to_float")->arg("x");
            // lets make sure its all aot ready
            verifyAotReady();
        }
        virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
            tw << "#include \"daScript/simulate/aot_builtin_math.h\"\n";
            return ModuleAotType::cpp;
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_Math,das);

IMPLEMENT_EXTERNAL_TYPE_FACTORY(float4x4, das::float4x4)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(float3x4, das::float3x4)
