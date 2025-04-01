#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require rtti
#include "daScript/simulate/aot_builtin_rtti.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_handle.h"
 // require strings
#include "daScript/simulate/aot_builtin_string.h"
 // require ast
#include "daScript/ast/ast.h"
#include "daScript/simulate/aot_builtin_ast.h"
#include "daScript/ast/ast_generate.h"
 // require math
#include "daScript/simulate/aot_builtin_math.h"
 // require strings_boost
 // require ast_boost
 // require regex
 // require regex_boost
 // require random
 // require templates
 // require contracts
 // require array_boost
 // require algorithm
 // require templates_boost
 // require static_let
 // require macro_boost
 // require defer
 // require math_bits
 // require faker
 // require constant_expression
 // require testing
 // require testing_boost
 // require fuzzer

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4100)   // unreferenced formal parameter
#pragma warning(disable:4189)   // local variable is initialized but not referenced
#pragma warning(disable:4244)   // conversion from 'int32_t' to 'float', possible loss of data
#pragma warning(disable:4114)   // same qualifier more than once
#pragma warning(disable:4623)   // default constructor was implicitly defined as deleted
#pragma warning(disable:4946)   // reinterpret_cast used between related classes
#pragma warning(disable:4269)   // 'const' automatic data initialized with compiler generated default constructor produces unreliable results
#pragma warning(disable:4555)   // result of expression not used
#endif
#if defined(__EDG__)
#pragma diag_suppress 826
#elif defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wreturn-local-addr"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wsubobject-linkage"
#endif
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wwritable-strings"
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#pragma clang diagnostic ignored "-Wunsequenced"
#pragma clang diagnostic ignored "-Wunused-function"
#endif

namespace das {
namespace _anon_14412364654671388840 {

namespace ast { struct AstFunctionAnnotation; };
namespace ast { struct AstBlockAnnotation; };
namespace ast { struct AstStructureAnnotation; };
namespace ast { struct AstPassMacro; };
namespace ast { struct AstVariantMacro; };
namespace ast { struct AstForLoopMacro; };
namespace ast { struct AstCaptureMacro; };
namespace ast { struct AstTypeMacro; };
namespace ast { struct AstSimulateMacro; };
namespace ast { struct AstReaderMacro; };
namespace ast { struct AstCommentReader; };
namespace ast { struct AstCallMacro; };
namespace ast { struct AstTypeInfoMacro; };
namespace ast { struct AstEnumerationAnnotation; };
namespace ast { struct AstVisitor; };
namespace ast_boost { struct MacroMacro; };
namespace ast_boost { struct TagFunctionAnnotation; };
namespace ast_boost { struct TagStructureAnnotation; };
namespace ast_boost { struct SetupAnyAnnotation; };
namespace ast_boost { struct SetupFunctionAnnotation; };
namespace ast_boost { struct SetupBlockAnnotation; };
namespace ast_boost { struct SetupStructureAnnotation; };
namespace ast_boost { struct SetupEnumerationAnnotation; };
namespace ast_boost { struct SetupContractAnnotation; };
namespace ast_boost { struct SetupReaderMacro; };
namespace ast_boost { struct SetupCommentReader; };
namespace ast_boost { struct SetupVariantMacro; };
namespace ast_boost { struct SetupForLoopMacro; };
namespace ast_boost { struct SetupCaptureMacro; };
namespace ast_boost { struct SetupTypeMacro; };
namespace ast_boost { struct SetupSimulateMacro; };
namespace ast_boost { struct SetupCallMacro; };
namespace ast_boost { struct SetupTypeInfoMacro; };
namespace ast_boost { struct SetupInferMacro; };
namespace ast_boost { struct SetupDirtyInferMacro; };
namespace ast_boost { struct SetupLintMacro; };
namespace ast_boost { struct SetupGlobalLintMacro; };
namespace ast_boost { struct SetupOptimizationMacro; };
namespace ast_boost { struct TagFunctionMacro; };
namespace ast_boost { struct BetterRttiVisitor; };
namespace regex { struct ReNode; };
namespace regex { struct Regex; };
namespace regex_boost { struct RegexReader; };
namespace random { struct _lambda_random_111_1; };
namespace templates { struct DecltypeMacro; };
namespace templates { struct DecltypeNoRefMacro; };
namespace templates { struct TemplateMacro; };
namespace contracts { struct IsAnyType; };
namespace contracts { struct IsAnyArrayMacro; };
namespace contracts { struct IsAnyEnumMacro; };
namespace contracts { struct IsAnyBitfieldMacro; };
namespace contracts { struct IsAnyVectorType; };
namespace contracts { struct IsAnyStructMacro; };
namespace contracts { struct IsAnyNumericMacro; };
namespace contracts { struct IsAnyWorkhorse; };
namespace contracts { struct IsAnyWorkhorseNonPtrMacro; };
namespace contracts { struct IsAnyTupleNonPtrMacro; };
namespace contracts { struct IsAnyVariantNonPtrMacro; };
namespace contracts { struct IsAnyFunctionNonPtrMacro; };
namespace contracts { struct IsAnyLambdaMacro; };
namespace contracts { struct IsRefMacro; };
namespace contracts { struct IsPointer; };
namespace contracts { struct IsClass; };
namespace contracts { struct IsValueHandle; };
namespace templates_boost { struct Template; };
namespace templates_boost { struct TemplateVisitor; };
namespace templates_boost { struct RemoveDerefVisitor; };
namespace templates_boost { struct QRulesVisitor; };
namespace templates_boost { struct AstQCallMacro; };
namespace templates_boost { struct QMacro; };
namespace templates_boost { struct QBlockMacro; };
namespace templates_boost { struct QBlockToArrayMacro; };
namespace templates_boost { struct QBlockExprMacro; };
namespace templates_boost { struct QTypeMacro; };
namespace templates_boost { struct AstQNamedMacro; };
namespace templates_boost { struct AstQFunctionMacro; };
namespace templates_boost { struct AstQVariableMacro; };
namespace templates_boost { struct AstQNamedClassMacro; };
namespace templates_boost { struct AstQMethodMacro; };
namespace static_let { struct StaticLetMacro; };
namespace macro_boost { struct MacroVerifyMacro; };
namespace macro_boost { struct CaptureBlock; };
namespace macro_boost { struct CapturedVariable; };
namespace macro_boost { struct ColletFinally; };
namespace macro_boost { struct ColletLabels; };
namespace macro_boost { struct ReturnSkipLockcheck; };
namespace defer { struct DeferMacro; };
namespace defer { struct DeferDeleteMacro; };
namespace faker { struct MonthNameAndDay; };
namespace faker { struct Faker; };
namespace constant_expression { struct ConstExprAnnotation; };
namespace constant_expression { struct ConstantExpressionMacro; };
namespace testing { struct T; };
namespace testing_boost { struct TestFunctionAnnotation; };
// unused enumeration CompilationError
// unused enumeration ConstMatters
// unused enumeration RefMatters
// unused enumeration TemporaryMatters
// unused enumeration Type
// unused enumeration ConversionResult
// unused enumeration CaptureMode
// unused enumeration SideEffects
// unused structure AstFunctionAnnotation
// unused structure AstBlockAnnotation
// unused structure AstStructureAnnotation
// unused structure AstPassMacro
// unused structure AstVariantMacro
// unused structure AstForLoopMacro
// unused structure AstCaptureMacro
// unused structure AstTypeMacro
// unused structure AstSimulateMacro
// unused structure AstReaderMacro
// unused structure AstCommentReader
// unused structure AstCallMacro
// unused structure AstTypeInfoMacro
// unused structure AstEnumerationAnnotation
// unused structure AstVisitor
// unused structure MacroMacro
// unused structure TagFunctionAnnotation
// unused structure TagStructureAnnotation
// unused structure SetupAnyAnnotation
// unused structure SetupFunctionAnnotation
// unused structure SetupBlockAnnotation
// unused structure SetupStructureAnnotation
// unused structure SetupEnumerationAnnotation
// unused structure SetupContractAnnotation
// unused structure SetupReaderMacro
// unused structure SetupCommentReader
// unused structure SetupVariantMacro
// unused structure SetupForLoopMacro
// unused structure SetupCaptureMacro
// unused structure SetupTypeMacro
// unused structure SetupSimulateMacro
// unused structure SetupCallMacro
// unused structure SetupTypeInfoMacro
// unused structure SetupInferMacro
// unused structure SetupDirtyInferMacro
// unused structure SetupLintMacro
// unused structure SetupGlobalLintMacro
// unused structure SetupOptimizationMacro
// unused structure TagFunctionMacro
// unused structure BetterRttiVisitor
// unused enumeration ReOp
// unused structure ReNode
// unused structure Regex
// unused structure RegexReader
// unused structure _lambda_random_111_1
// unused structure DecltypeMacro
// unused structure DecltypeNoRefMacro
// unused structure TemplateMacro
// unused structure IsAnyType
// unused structure IsAnyArrayMacro
// unused structure IsAnyEnumMacro
// unused structure IsAnyBitfieldMacro
// unused structure IsAnyVectorType
// unused structure IsAnyStructMacro
// unused structure IsAnyNumericMacro
// unused structure IsAnyWorkhorse
// unused structure IsAnyWorkhorseNonPtrMacro
// unused structure IsAnyTupleNonPtrMacro
// unused structure IsAnyVariantNonPtrMacro
// unused structure IsAnyFunctionNonPtrMacro
// unused structure IsAnyLambdaMacro
// unused structure IsRefMacro
// unused structure IsPointer
// unused structure IsClass
// unused structure IsValueHandle
// unused structure Template
// unused structure TemplateVisitor
// unused structure RemoveDerefVisitor
// unused structure QRulesVisitor
// unused structure AstQCallMacro
// unused structure QMacro
// unused structure QBlockMacro
// unused structure QBlockToArrayMacro
// unused structure QBlockExprMacro
// unused structure QTypeMacro
// unused structure AstQNamedMacro
// unused structure AstQFunctionMacro
// unused structure AstQVariableMacro
// unused structure AstQNamedClassMacro
// unused structure AstQMethodMacro
// unused structure StaticLetMacro
// unused structure MacroVerifyMacro
// unused structure CaptureBlock
// unused structure CapturedVariable
// unused structure ColletFinally
// unused structure ColletLabels
// unused structure ReturnSkipLockcheck
// unused structure DeferMacro
// unused structure DeferDeleteMacro
// unused structure MonthNameAndDay
namespace faker {

struct Faker {
    uint32_t min_year;
    uint32_t total_years;
    Sequence DAS_COMMENT((uint32_t)) rnd;
    uint32_t max_long_string;
};
}
// unused structure ConstExprAnnotation
// unused structure ConstantExpressionMacro
namespace testing {

struct T {
    void * __rtti;
    Func DAS_COMMENT((void,testing::T)) __finalize;
    char * name;
    int64_t startTick;
    bool verbose;
    Lambda DAS_COMMENT((void,char * const ,LineInfo const )) onLog;
    Lambda DAS_COMMENT((void,bool)) onFail;
    Lambda DAS_COMMENT((void)) onSkipNow;
    Lambda DAS_COMMENT((void,char * const ,AutoVariant<Lambda DAS_COMMENT((void,testing::T * const )),Func DAS_COMMENT((void,testing::T * const ))> const )) onRun;
    bool failed;
    bool skipped;
    Func DAS_COMMENT((void,testing::T const )) failNow;
    Func DAS_COMMENT((void,testing::T const )) fail;
    Func DAS_COMMENT((void,testing::T const ,char * const )) error;
    Func DAS_COMMENT((void,testing::T const ,char * const ,LineInfo const )) errorAt;
    Func DAS_COMMENT((void,testing::T const ,char * const )) fatal;
    Func DAS_COMMENT((void,testing::T const ,char * const ,LineInfo const )) fatalAt;
    Func DAS_COMMENT((void,testing::T const ,char * const )) log;
    Func DAS_COMMENT((void,testing::T const ,char * const ,LineInfo const )) logAt;
    Func DAS_COMMENT((void,testing::T const )) skipNow;
    Func DAS_COMMENT((void,testing::T const ,char * const )) skip;
    Func DAS_COMMENT((void,testing::T const ,char * const ,LineInfo const )) skipAt;
    Func DAS_COMMENT((int32_t,testing::T const )) getTimeUsec;
    Func DAS_COMMENT((double,testing::T const )) getTimeSec;
};
}
// unused structure TestFunctionAnnotation


static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}


inline void fuzz_b284ca1047c9135d ( Context * __context__, Block DAS_COMMENT((void)) const  & __blk_rename_at_15_0 );
inline void fuzz_26c215c38d1dfd1a ( Context * __context__, int32_t __fuzz_count_rename_at_27_2, Block DAS_COMMENT((void)) const  & __blk_rename_at_27_3 );
inline void fuzz_debug_93358114940f0b68 ( Context * __context__, Block DAS_COMMENT((void)) const  & __blk_rename_at_39_5 );
inline void fuzz_debug_3b504f8008b955ef ( Context * __context__, int32_t __fuzz_count_rename_at_48_7, Block DAS_COMMENT((void)) const  & __blk_rename_at_48_8 );
inline void fuzz_numeric_and_vector_op1_48ce713c34be8575 ( Context * __context__, testing::T * const  __t_rename_at_58_10, faker::Faker & __fake_rename_at_58_11, char * const  __funcname_rename_at_58_12 );
inline void fuzz_numeric_and_vector_signed_op1_cf02787541ef8a1f ( Context * __context__, testing::T * const  __t_rename_at_77_13, faker::Faker & __fake_rename_at_77_14, char * const  __funcname_rename_at_77_15 );
inline void fuzz_numeric_op1_7e39ee71d5eea21b ( Context * __context__, testing::T * const  __t_rename_at_92_16, faker::Faker & __fake_rename_at_92_17, char * const  __funcname_rename_at_92_18 );
inline void fuzz_numeric_and_storage_op1_14e5fe2cc526dd1f ( Context * __context__, testing::T * const  __t_rename_at_102_19, faker::Faker & __fake_rename_at_102_20, char * const  __funcname_rename_at_102_21 );
inline void fuzz_all_ints_op1_e805e6460a35229d ( Context * __context__, testing::T * const  __t_rename_at_118_22, faker::Faker & __fake_rename_at_118_23, char * const  __funcname_rename_at_118_24 );
inline void fuzz_all_unsigned_ints_op1_f9f52b078e024909 ( Context * __context__, testing::T * const  __t_rename_at_128_25, faker::Faker & __fake_rename_at_128_26, char * const  __funcname_rename_at_128_27 );
inline void fuzz_float_double_or_float_vec_op1_3652ef8150f5c24d ( Context * __context__, testing::T * const  __t_rename_at_136_28, faker::Faker & __fake_rename_at_136_29, char * const  __funcname_rename_at_136_30 );
inline void fuzz_float_or_float_vec_op1_78b3a33e9c015725 ( Context * __context__, testing::T * const  __t_rename_at_147_31, faker::Faker & __fake_rename_at_147_32, char * const  __funcname_rename_at_147_33 );
inline void fuzz_float_or_float_vec_op2_ae83307b5cfedd1 ( Context * __context__, testing::T * const  __t_rename_at_157_34, faker::Faker & __fake_rename_at_157_35, char * const  __funcname_rename_at_157_36 );
inline void fuzz_float_double_or_float_vec_op2_a264e3ee00149f0b ( Context * __context__, testing::T * const  __t_rename_at_167_37, faker::Faker & __fake_rename_at_167_38, char * const  __funcname_rename_at_167_39 );
inline void fuzz_numeric_and_vector_op2_f57314e6b07c00d3 ( Context * __context__, testing::T * const  __t_rename_at_178_40, faker::Faker & __fake_rename_at_178_41, char * const  __funcname_rename_at_178_42 );
inline void fuzz_numeric_and_vector_op2_no_unint_vec_f9fe354762f0d437 ( Context * __context__, testing::T * const  __t_rename_at_197_43, faker::Faker & __fake_rename_at_197_44, char * const  __funcname_rename_at_197_45 );
inline void fuzz_numeric_op2_92b03af12a4daff9 ( Context * __context__, testing::T * const  __t_rename_at_214_46, faker::Faker & __fake_rename_at_214_47, char * const  __funcname_rename_at_214_48 );
inline void fuzz_compareable_op2_87dd0992261931a1 ( Context * __context__, testing::T * const  __t_rename_at_224_49, faker::Faker & __fake_rename_at_224_50, char * const  __funcname_rename_at_224_51 );
inline void fuzz_eq_neq_op2_97177b59c300d73d ( Context * __context__, testing::T * const  __t_rename_at_237_52, faker::Faker & __fake_rename_at_237_53, char * const  __funcname_rename_at_237_54 );
inline void fuzz_numeric_vec_scal_op2_52ad44f3d93412e3 ( Context * __context__, testing::T * const  __t_rename_at_259_55, faker::Faker & __fake_rename_at_259_56, char * const  __funcname_rename_at_259_57 );
inline void fuzz_numeric_scal_vec_op2_fd0677cf42c35c93 ( Context * __context__, testing::T * const  __t_rename_at_274_58, faker::Faker & __fake_rename_at_274_59, char * const  __funcname_rename_at_274_60 );
inline void fuzz_int_vector_op2_dc7f59e64eb3fb95 ( Context * __context__, testing::T * const  __t_rename_at_289_61, faker::Faker & __fake_rename_at_289_62, char * const  __funcname_rename_at_289_63 );
inline void fuzz_shift_op2_11a978107d3af929 ( Context * __context__, testing::T * const  __t_rename_at_303_64, faker::Faker & __fake_rename_at_303_65, char * const  __funcname_rename_at_303_66 );
inline void fuzz_rotate_op2_e76ebde963d165d3 ( Context * __context__, testing::T * const  __t_rename_at_317_67, faker::Faker & __fake_rename_at_317_68, char * const  __funcname_rename_at_317_69 );
inline void fuzz_numeric_op3_c254164d54c65bef ( Context * __context__, testing::T * const  __t_rename_at_325_70, faker::Faker & __fake_rename_at_325_71, char * const  __funcname_rename_at_325_72 );
inline void fuzz_vec_op3_b8b2d31bb7eb72a3 ( Context * __context__, testing::T * const  __t_rename_at_335_73, faker::Faker & __fake_rename_at_335_74, char * const  __funcname_rename_at_335_75 );
inline void fuzz_vec_mad_op3_92bb9a6e84ac3c0b ( Context * __context__, testing::T * const  __t_rename_at_350_76, faker::Faker & __fake_rename_at_350_77, char * const  __funcname_rename_at_350_78 );
inline void fuzz_float_double_or_float_vec_op3_4985cd878555494d ( Context * __context__, testing::T * const  __t_rename_at_365_79, faker::Faker & __fake_rename_at_365_80, char * const  __funcname_rename_at_365_81 );
inline void fuzz_numeric_op4_16e37608179824b9 ( Context * __context__, testing::T * const  __t_rename_at_377_82, faker::Faker & __fake_rename_at_377_83, char * const  __funcname_rename_at_377_84 );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global<int32_t,0xbd9f757637c922d6>(__context__) = 1000;/*total_fuzz_count*/
}

inline void fuzz_b284ca1047c9135d ( Context * __context__, Block DAS_COMMENT((void)) const  &  __blk_rename_at_15_0 )
{
    {
        bool __need_loop_18 = true;
        // i: int const
        das_iterator<range> __i_iterator(mk_range(das_global<int32_t,0xbd9f757637c922d6>(__context__) /*total_fuzz_count*/));
        int32_t __i_rename_at_18_1;
        __need_loop_18 = __i_iterator.first(__context__,(__i_rename_at_18_1)) && __need_loop_18;
        for ( ; __need_loop_18 ; __need_loop_18 = __i_iterator.next(__context__,(__i_rename_at_18_1)) )
        {
            das_try_recover(__context__, [&]()
            {
                das_invoke<void>::invoke(__context__,nullptr,__blk_rename_at_15_0);
            }, [&]()
            {
            });
        }
        __i_iterator.close(__context__,(__i_rename_at_18_1));
    };
}

inline void fuzz_26c215c38d1dfd1a ( Context * __context__, int32_t __fuzz_count_rename_at_27_2, Block DAS_COMMENT((void)) const  &  __blk_rename_at_27_3 )
{
    {
        bool __need_loop_30 = true;
        // i: int const
        das_iterator<range> __i_iterator(mk_range(__fuzz_count_rename_at_27_2));
        int32_t __i_rename_at_30_4;
        __need_loop_30 = __i_iterator.first(__context__,(__i_rename_at_30_4)) && __need_loop_30;
        for ( ; __need_loop_30 ; __need_loop_30 = __i_iterator.next(__context__,(__i_rename_at_30_4)) )
        {
            das_try_recover(__context__, [&]()
            {
                das_invoke<void>::invoke(__context__,nullptr,__blk_rename_at_27_3);
            }, [&]()
            {
            });
        }
        __i_iterator.close(__context__,(__i_rename_at_30_4));
    };
}

inline void fuzz_debug_93358114940f0b68 ( Context * __context__, Block DAS_COMMENT((void)) const  &  __blk_rename_at_39_5 )
{
    {
        bool __need_loop_43 = true;
        // i: int const
        das_iterator<range> __i_iterator(mk_range(das_global<int32_t,0xbd9f757637c922d6>(__context__) /*total_fuzz_count*/));
        int32_t __i_rename_at_43_6;
        __need_loop_43 = __i_iterator.first(__context__,(__i_rename_at_43_6)) && __need_loop_43;
        for ( ; __need_loop_43 ; __need_loop_43 = __i_iterator.next(__context__,(__i_rename_at_43_6)) )
        {
            das_invoke<void>::invoke(__context__,nullptr,__blk_rename_at_39_5);
        }
        __i_iterator.close(__context__,(__i_rename_at_43_6));
    };
}

inline void fuzz_debug_3b504f8008b955ef ( Context * __context__, int32_t __fuzz_count_rename_at_48_7, Block DAS_COMMENT((void)) const  &  __blk_rename_at_48_8 )
{
    {
        bool __need_loop_52 = true;
        // i: int const
        das_iterator<range> __i_iterator(mk_range(__fuzz_count_rename_at_48_7));
        int32_t __i_rename_at_52_9;
        __need_loop_52 = __i_iterator.first(__context__,(__i_rename_at_52_9)) && __need_loop_52;
        for ( ; __need_loop_52 ; __need_loop_52 = __i_iterator.next(__context__,(__i_rename_at_52_9)) )
        {
            das_invoke<void>::invoke(__context__,nullptr,__blk_rename_at_48_8);
        }
        __i_iterator.close(__context__,(__i_rename_at_52_9));
    };
}

inline void fuzz_numeric_and_vector_op1_48ce713c34be8575 ( Context * __context__, testing::T * const  __t_rename_at_58_10, faker::Faker &  __fake_rename_at_58_11, char * const  __funcname_rename_at_58_12 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int32_t>(__context__,nullptr,__funcname_rename_at_58_12,__t_rename_at_58_10,das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_58_11)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint32_t>(__context__,nullptr,__funcname_rename_at_58_12,__t_rename_at_58_10,das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_58_11)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float>(__context__,nullptr,__funcname_rename_at_58_12,__t_rename_at_58_10,das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_58_11)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,double>(__context__,nullptr,__funcname_rename_at_58_12,__t_rename_at_58_10,das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_58_11)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int2>(__context__,nullptr,__funcname_rename_at_58_12,__t_rename_at_58_10,das_invoke_function<int2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int2 S<faker::Faker>*/ 0x4e735764aa61d3b5)),das_arg<faker::Faker>::pass(__fake_rename_at_58_11)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int3>(__context__,nullptr,__funcname_rename_at_58_12,__t_rename_at_58_10,das_invoke_function<int3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int3 S<faker::Faker>*/ 0x2c26493dc762dfc2)),das_arg<faker::Faker>::pass(__fake_rename_at_58_11)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int4>(__context__,nullptr,__funcname_rename_at_58_12,__t_rename_at_58_10,das_invoke_function<int4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int4 S<faker::Faker>*/ 0x1d5187bee4c6af97)),das_arg<faker::Faker>::pass(__fake_rename_at_58_11)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint2>(__context__,nullptr,__funcname_rename_at_58_12,__t_rename_at_58_10,das_invoke_function<uint2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint2 S<faker::Faker>*/ 0xceadb567a9b81609)),das_arg<faker::Faker>::pass(__fake_rename_at_58_11)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint3>(__context__,nullptr,__funcname_rename_at_58_12,__t_rename_at_58_10,das_invoke_function<uint3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint3 S<faker::Faker>*/ 0xdc9cf75c88c69b09)),das_arg<faker::Faker>::pass(__fake_rename_at_58_11)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint4>(__context__,nullptr,__funcname_rename_at_58_12,__t_rename_at_58_10,das_invoke_function<uint4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint4 S<faker::Faker>*/ 0xe2d38a5eaa391409)),das_arg<faker::Faker>::pass(__fake_rename_at_58_11)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float2>(__context__,nullptr,__funcname_rename_at_58_12,__t_rename_at_58_10,das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_58_11)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float3>(__context__,nullptr,__funcname_rename_at_58_12,__t_rename_at_58_10,das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_58_11)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float4>(__context__,nullptr,__funcname_rename_at_58_12,__t_rename_at_58_10,das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_58_11)));
}

inline void fuzz_numeric_and_vector_signed_op1_cf02787541ef8a1f ( Context * __context__, testing::T * const  __t_rename_at_77_13, faker::Faker &  __fake_rename_at_77_14, char * const  __funcname_rename_at_77_15 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int32_t>(__context__,nullptr,__funcname_rename_at_77_15,__t_rename_at_77_13,das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_77_14)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float>(__context__,nullptr,__funcname_rename_at_77_15,__t_rename_at_77_13,das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_77_14)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,double>(__context__,nullptr,__funcname_rename_at_77_15,__t_rename_at_77_13,das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_77_14)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int2>(__context__,nullptr,__funcname_rename_at_77_15,__t_rename_at_77_13,das_invoke_function<int2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int2 S<faker::Faker>*/ 0x4e735764aa61d3b5)),das_arg<faker::Faker>::pass(__fake_rename_at_77_14)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int3>(__context__,nullptr,__funcname_rename_at_77_15,__t_rename_at_77_13,das_invoke_function<int3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int3 S<faker::Faker>*/ 0x2c26493dc762dfc2)),das_arg<faker::Faker>::pass(__fake_rename_at_77_14)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int4>(__context__,nullptr,__funcname_rename_at_77_15,__t_rename_at_77_13,das_invoke_function<int4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int4 S<faker::Faker>*/ 0x1d5187bee4c6af97)),das_arg<faker::Faker>::pass(__fake_rename_at_77_14)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float2>(__context__,nullptr,__funcname_rename_at_77_15,__t_rename_at_77_13,das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_77_14)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float3>(__context__,nullptr,__funcname_rename_at_77_15,__t_rename_at_77_13,das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_77_14)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float4>(__context__,nullptr,__funcname_rename_at_77_15,__t_rename_at_77_13,das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_77_14)));
}

inline void fuzz_numeric_op1_7e39ee71d5eea21b ( Context * __context__, testing::T * const  __t_rename_at_92_16, faker::Faker &  __fake_rename_at_92_17, char * const  __funcname_rename_at_92_18 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int32_t>(__context__,nullptr,__funcname_rename_at_92_18,__t_rename_at_92_16,das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_92_17)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint32_t>(__context__,nullptr,__funcname_rename_at_92_18,__t_rename_at_92_16,das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_92_17)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float>(__context__,nullptr,__funcname_rename_at_92_18,__t_rename_at_92_16,das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_92_17)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,double>(__context__,nullptr,__funcname_rename_at_92_18,__t_rename_at_92_16,das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_92_17)));
}

inline void fuzz_numeric_and_storage_op1_14e5fe2cc526dd1f ( Context * __context__, testing::T * const  __t_rename_at_102_19, faker::Faker &  __fake_rename_at_102_20, char * const  __funcname_rename_at_102_21 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int32_t>(__context__,nullptr,__funcname_rename_at_102_21,__t_rename_at_102_19,das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_102_20)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint32_t>(__context__,nullptr,__funcname_rename_at_102_21,__t_rename_at_102_19,das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_102_20)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int8_t>(__context__,nullptr,__funcname_rename_at_102_21,__t_rename_at_102_19,das_invoke_function<int8_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int8 S<faker::Faker>*/ 0x9ef42a78d1c6aa5b)),das_arg<faker::Faker>::pass(__fake_rename_at_102_20)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint8_t>(__context__,nullptr,__funcname_rename_at_102_21,__t_rename_at_102_19,das_invoke_function<uint8_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint8 S<faker::Faker>*/ 0xf29c392a7e535009)),das_arg<faker::Faker>::pass(__fake_rename_at_102_20)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int16_t>(__context__,nullptr,__funcname_rename_at_102_21,__t_rename_at_102_19,das_invoke_function<int16_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int16 S<faker::Faker>*/ 0xd7da655a1d36bef5)),das_arg<faker::Faker>::pass(__fake_rename_at_102_20)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint16_t>(__context__,nullptr,__funcname_rename_at_102_21,__t_rename_at_102_19,das_invoke_function<uint16_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint16 S<faker::Faker>*/ 0xe6b011e400cf52b6)),das_arg<faker::Faker>::pass(__fake_rename_at_102_20)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int64_t>(__context__,nullptr,__funcname_rename_at_102_21,__t_rename_at_102_19,das_invoke_function<int64_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int64 S<faker::Faker>*/ 0x48db743deede88d0)),das_arg<faker::Faker>::pass(__fake_rename_at_102_20)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint64_t>(__context__,nullptr,__funcname_rename_at_102_21,__t_rename_at_102_19,das_invoke_function<uint64_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint64 S<faker::Faker>*/ 0x8d24a4146201a784)),das_arg<faker::Faker>::pass(__fake_rename_at_102_20)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float>(__context__,nullptr,__funcname_rename_at_102_21,__t_rename_at_102_19,das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_102_20)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,double>(__context__,nullptr,__funcname_rename_at_102_21,__t_rename_at_102_19,das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_102_20)));
}

inline void fuzz_all_ints_op1_e805e6460a35229d ( Context * __context__, testing::T * const  __t_rename_at_118_22, faker::Faker &  __fake_rename_at_118_23, char * const  __funcname_rename_at_118_24 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int32_t>(__context__,nullptr,__funcname_rename_at_118_24,__t_rename_at_118_22,das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_118_23)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint32_t>(__context__,nullptr,__funcname_rename_at_118_24,__t_rename_at_118_22,das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_118_23)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int64_t>(__context__,nullptr,__funcname_rename_at_118_24,__t_rename_at_118_22,das_invoke_function<int64_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int64 S<faker::Faker>*/ 0x48db743deede88d0)),das_arg<faker::Faker>::pass(__fake_rename_at_118_23)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint64_t>(__context__,nullptr,__funcname_rename_at_118_24,__t_rename_at_118_22,das_invoke_function<uint64_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint64 S<faker::Faker>*/ 0x8d24a4146201a784)),das_arg<faker::Faker>::pass(__fake_rename_at_118_23)));
}

inline void fuzz_all_unsigned_ints_op1_f9f52b078e024909 ( Context * __context__, testing::T * const  __t_rename_at_128_25, faker::Faker &  __fake_rename_at_128_26, char * const  __funcname_rename_at_128_27 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint32_t>(__context__,nullptr,__funcname_rename_at_128_27,__t_rename_at_128_25,das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_128_26)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint64_t>(__context__,nullptr,__funcname_rename_at_128_27,__t_rename_at_128_25,das_invoke_function<uint64_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint64 S<faker::Faker>*/ 0x8d24a4146201a784)),das_arg<faker::Faker>::pass(__fake_rename_at_128_26)));
}

inline void fuzz_float_double_or_float_vec_op1_3652ef8150f5c24d ( Context * __context__, testing::T * const  __t_rename_at_136_28, faker::Faker &  __fake_rename_at_136_29, char * const  __funcname_rename_at_136_30 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float>(__context__,nullptr,__funcname_rename_at_136_30,__t_rename_at_136_28,das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_136_29)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,double>(__context__,nullptr,__funcname_rename_at_136_30,__t_rename_at_136_28,das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_136_29)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float2>(__context__,nullptr,__funcname_rename_at_136_30,__t_rename_at_136_28,das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_136_29)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float3>(__context__,nullptr,__funcname_rename_at_136_30,__t_rename_at_136_28,das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_136_29)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float4>(__context__,nullptr,__funcname_rename_at_136_30,__t_rename_at_136_28,das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_136_29)));
}

inline void fuzz_float_or_float_vec_op1_78b3a33e9c015725 ( Context * __context__, testing::T * const  __t_rename_at_147_31, faker::Faker &  __fake_rename_at_147_32, char * const  __funcname_rename_at_147_33 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float>(__context__,nullptr,__funcname_rename_at_147_33,__t_rename_at_147_31,das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_147_32)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float2>(__context__,nullptr,__funcname_rename_at_147_33,__t_rename_at_147_31,das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_147_32)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float3>(__context__,nullptr,__funcname_rename_at_147_33,__t_rename_at_147_31,das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_147_32)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float4>(__context__,nullptr,__funcname_rename_at_147_33,__t_rename_at_147_31,das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_147_32)));
}

inline void fuzz_float_or_float_vec_op2_ae83307b5cfedd1 ( Context * __context__, testing::T * const  __t_rename_at_157_34, faker::Faker &  __fake_rename_at_157_35, char * const  __funcname_rename_at_157_36 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float,float>(__context__,nullptr,__funcname_rename_at_157_36,__t_rename_at_157_34,das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_157_35)),das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_157_35)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float2,float2>(__context__,nullptr,__funcname_rename_at_157_36,__t_rename_at_157_34,das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_157_35)),das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_157_35)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float3,float3>(__context__,nullptr,__funcname_rename_at_157_36,__t_rename_at_157_34,das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_157_35)),das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_157_35)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float4,float4>(__context__,nullptr,__funcname_rename_at_157_36,__t_rename_at_157_34,das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_157_35)),das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_157_35)));
}

inline void fuzz_float_double_or_float_vec_op2_a264e3ee00149f0b ( Context * __context__, testing::T * const  __t_rename_at_167_37, faker::Faker &  __fake_rename_at_167_38, char * const  __funcname_rename_at_167_39 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float,float>(__context__,nullptr,__funcname_rename_at_167_39,__t_rename_at_167_37,das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_167_38)),das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_167_38)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,double,double>(__context__,nullptr,__funcname_rename_at_167_39,__t_rename_at_167_37,das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_167_38)),das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_167_38)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float2,float2>(__context__,nullptr,__funcname_rename_at_167_39,__t_rename_at_167_37,das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_167_38)),das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_167_38)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float3,float3>(__context__,nullptr,__funcname_rename_at_167_39,__t_rename_at_167_37,das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_167_38)),das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_167_38)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float4,float4>(__context__,nullptr,__funcname_rename_at_167_39,__t_rename_at_167_37,das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_167_38)),das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_167_38)));
}

inline void fuzz_numeric_and_vector_op2_f57314e6b07c00d3 ( Context * __context__, testing::T * const  __t_rename_at_178_40, faker::Faker &  __fake_rename_at_178_41, char * const  __funcname_rename_at_178_42 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int32_t,int32_t>(__context__,nullptr,__funcname_rename_at_178_42,__t_rename_at_178_40,das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint32_t,uint32_t>(__context__,nullptr,__funcname_rename_at_178_42,__t_rename_at_178_40,das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)),das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float,float>(__context__,nullptr,__funcname_rename_at_178_42,__t_rename_at_178_40,das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)),das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,double,double>(__context__,nullptr,__funcname_rename_at_178_42,__t_rename_at_178_40,das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)),das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int2,int2>(__context__,nullptr,__funcname_rename_at_178_42,__t_rename_at_178_40,das_invoke_function<int2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int2 S<faker::Faker>*/ 0x4e735764aa61d3b5)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)),das_invoke_function<int2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int2 S<faker::Faker>*/ 0x4e735764aa61d3b5)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int3,int3>(__context__,nullptr,__funcname_rename_at_178_42,__t_rename_at_178_40,das_invoke_function<int3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int3 S<faker::Faker>*/ 0x2c26493dc762dfc2)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)),das_invoke_function<int3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int3 S<faker::Faker>*/ 0x2c26493dc762dfc2)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int4,int4>(__context__,nullptr,__funcname_rename_at_178_42,__t_rename_at_178_40,das_invoke_function<int4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int4 S<faker::Faker>*/ 0x1d5187bee4c6af97)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)),das_invoke_function<int4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int4 S<faker::Faker>*/ 0x1d5187bee4c6af97)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint2,uint2>(__context__,nullptr,__funcname_rename_at_178_42,__t_rename_at_178_40,das_invoke_function<uint2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint2 S<faker::Faker>*/ 0xceadb567a9b81609)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)),das_invoke_function<uint2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint2 S<faker::Faker>*/ 0xceadb567a9b81609)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint3,uint3>(__context__,nullptr,__funcname_rename_at_178_42,__t_rename_at_178_40,das_invoke_function<uint3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint3 S<faker::Faker>*/ 0xdc9cf75c88c69b09)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)),das_invoke_function<uint3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint3 S<faker::Faker>*/ 0xdc9cf75c88c69b09)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint4,uint4>(__context__,nullptr,__funcname_rename_at_178_42,__t_rename_at_178_40,das_invoke_function<uint4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint4 S<faker::Faker>*/ 0xe2d38a5eaa391409)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)),das_invoke_function<uint4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint4 S<faker::Faker>*/ 0xe2d38a5eaa391409)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float2,float2>(__context__,nullptr,__funcname_rename_at_178_42,__t_rename_at_178_40,das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)),das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float3,float3>(__context__,nullptr,__funcname_rename_at_178_42,__t_rename_at_178_40,das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)),das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float4,float4>(__context__,nullptr,__funcname_rename_at_178_42,__t_rename_at_178_40,das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)),das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_178_41)));
}

inline void fuzz_numeric_and_vector_op2_no_unint_vec_f9fe354762f0d437 ( Context * __context__, testing::T * const  __t_rename_at_197_43, faker::Faker &  __fake_rename_at_197_44, char * const  __funcname_rename_at_197_45 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int32_t,int32_t>(__context__,nullptr,__funcname_rename_at_197_45,__t_rename_at_197_43,das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_197_44)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_197_44)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint32_t,uint32_t>(__context__,nullptr,__funcname_rename_at_197_45,__t_rename_at_197_43,das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_197_44)),das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_197_44)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float,float>(__context__,nullptr,__funcname_rename_at_197_45,__t_rename_at_197_43,das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_197_44)),das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_197_44)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,double,double>(__context__,nullptr,__funcname_rename_at_197_45,__t_rename_at_197_43,das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_197_44)),das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_197_44)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int2,int2>(__context__,nullptr,__funcname_rename_at_197_45,__t_rename_at_197_43,das_invoke_function<int2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int2 S<faker::Faker>*/ 0x4e735764aa61d3b5)),das_arg<faker::Faker>::pass(__fake_rename_at_197_44)),das_invoke_function<int2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int2 S<faker::Faker>*/ 0x4e735764aa61d3b5)),das_arg<faker::Faker>::pass(__fake_rename_at_197_44)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int3,int3>(__context__,nullptr,__funcname_rename_at_197_45,__t_rename_at_197_43,das_invoke_function<int3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int3 S<faker::Faker>*/ 0x2c26493dc762dfc2)),das_arg<faker::Faker>::pass(__fake_rename_at_197_44)),das_invoke_function<int3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int3 S<faker::Faker>*/ 0x2c26493dc762dfc2)),das_arg<faker::Faker>::pass(__fake_rename_at_197_44)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int4,int4>(__context__,nullptr,__funcname_rename_at_197_45,__t_rename_at_197_43,das_invoke_function<int4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int4 S<faker::Faker>*/ 0x1d5187bee4c6af97)),das_arg<faker::Faker>::pass(__fake_rename_at_197_44)),das_invoke_function<int4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int4 S<faker::Faker>*/ 0x1d5187bee4c6af97)),das_arg<faker::Faker>::pass(__fake_rename_at_197_44)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float2,float2>(__context__,nullptr,__funcname_rename_at_197_45,__t_rename_at_197_43,das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_197_44)),das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_197_44)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float3,float3>(__context__,nullptr,__funcname_rename_at_197_45,__t_rename_at_197_43,das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_197_44)),das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_197_44)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float4,float4>(__context__,nullptr,__funcname_rename_at_197_45,__t_rename_at_197_43,das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_197_44)),das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_197_44)));
}

inline void fuzz_numeric_op2_92b03af12a4daff9 ( Context * __context__, testing::T * const  __t_rename_at_214_46, faker::Faker &  __fake_rename_at_214_47, char * const  __funcname_rename_at_214_48 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int32_t,int32_t>(__context__,nullptr,__funcname_rename_at_214_48,__t_rename_at_214_46,das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_214_47)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_214_47)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint32_t,uint32_t>(__context__,nullptr,__funcname_rename_at_214_48,__t_rename_at_214_46,das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_214_47)),das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_214_47)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float,float>(__context__,nullptr,__funcname_rename_at_214_48,__t_rename_at_214_46,das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_214_47)),das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_214_47)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,double,double>(__context__,nullptr,__funcname_rename_at_214_48,__t_rename_at_214_46,das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_214_47)),das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_214_47)));
}

inline void fuzz_compareable_op2_87dd0992261931a1 ( Context * __context__, testing::T * const  __t_rename_at_224_49, faker::Faker &  __fake_rename_at_224_50, char * const  __funcname_rename_at_224_51 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int32_t,int32_t>(__context__,nullptr,__funcname_rename_at_224_51,__t_rename_at_224_49,das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_224_50)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_224_50)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint32_t,uint32_t>(__context__,nullptr,__funcname_rename_at_224_51,__t_rename_at_224_49,das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_224_50)),das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_224_50)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int64_t,int64_t>(__context__,nullptr,__funcname_rename_at_224_51,__t_rename_at_224_49,das_invoke_function<int64_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int64 S<faker::Faker>*/ 0x48db743deede88d0)),das_arg<faker::Faker>::pass(__fake_rename_at_224_50)),das_invoke_function<int64_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int64 S<faker::Faker>*/ 0x48db743deede88d0)),das_arg<faker::Faker>::pass(__fake_rename_at_224_50)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint64_t,uint64_t>(__context__,nullptr,__funcname_rename_at_224_51,__t_rename_at_224_49,das_invoke_function<uint64_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint64 S<faker::Faker>*/ 0x8d24a4146201a784)),das_arg<faker::Faker>::pass(__fake_rename_at_224_50)),das_invoke_function<uint64_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint64 S<faker::Faker>*/ 0x8d24a4146201a784)),das_arg<faker::Faker>::pass(__fake_rename_at_224_50)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float,float>(__context__,nullptr,__funcname_rename_at_224_51,__t_rename_at_224_49,das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_224_50)),das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_224_50)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,double,double>(__context__,nullptr,__funcname_rename_at_224_51,__t_rename_at_224_49,das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_224_50)),das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_224_50)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,char * const ,char * const >(__context__,nullptr,__funcname_rename_at_224_51,__t_rename_at_224_49,das_invoke_function<char * const >::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::any_string S<faker::Faker>*/ 0x5b048519673aaa43)),das_arg<faker::Faker>::pass(__fake_rename_at_224_50)),das_invoke_function<char * const >::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::any_string S<faker::Faker>*/ 0x5b048519673aaa43)),das_arg<faker::Faker>::pass(__fake_rename_at_224_50)));
}

inline void fuzz_eq_neq_op2_97177b59c300d73d ( Context * __context__, testing::T * const  __t_rename_at_237_52, faker::Faker &  __fake_rename_at_237_53, char * const  __funcname_rename_at_237_54 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int32_t,int32_t>(__context__,nullptr,__funcname_rename_at_237_54,__t_rename_at_237_52,das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint32_t,uint32_t>(__context__,nullptr,__funcname_rename_at_237_54,__t_rename_at_237_52,das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)),das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int64_t,int64_t>(__context__,nullptr,__funcname_rename_at_237_54,__t_rename_at_237_52,das_invoke_function<int64_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int64 S<faker::Faker>*/ 0x48db743deede88d0)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)),das_invoke_function<int64_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int64 S<faker::Faker>*/ 0x48db743deede88d0)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint64_t,uint64_t>(__context__,nullptr,__funcname_rename_at_237_54,__t_rename_at_237_52,das_invoke_function<uint64_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint64 S<faker::Faker>*/ 0x8d24a4146201a784)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)),das_invoke_function<uint64_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint64 S<faker::Faker>*/ 0x8d24a4146201a784)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float,float>(__context__,nullptr,__funcname_rename_at_237_54,__t_rename_at_237_52,das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)),das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,double,double>(__context__,nullptr,__funcname_rename_at_237_54,__t_rename_at_237_52,das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)),das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,char * const ,char * const >(__context__,nullptr,__funcname_rename_at_237_54,__t_rename_at_237_52,das_invoke_function<char * const >::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::any_string S<faker::Faker>*/ 0x5b048519673aaa43)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)),das_invoke_function<char * const >::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::any_string S<faker::Faker>*/ 0x5b048519673aaa43)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int2,int2>(__context__,nullptr,__funcname_rename_at_237_54,__t_rename_at_237_52,das_invoke_function<int2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int2 S<faker::Faker>*/ 0x4e735764aa61d3b5)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)),das_invoke_function<int2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int2 S<faker::Faker>*/ 0x4e735764aa61d3b5)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int3,int3>(__context__,nullptr,__funcname_rename_at_237_54,__t_rename_at_237_52,das_invoke_function<int3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int3 S<faker::Faker>*/ 0x2c26493dc762dfc2)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)),das_invoke_function<int3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int3 S<faker::Faker>*/ 0x2c26493dc762dfc2)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int4,int4>(__context__,nullptr,__funcname_rename_at_237_54,__t_rename_at_237_52,das_invoke_function<int4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int4 S<faker::Faker>*/ 0x1d5187bee4c6af97)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)),das_invoke_function<int4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int4 S<faker::Faker>*/ 0x1d5187bee4c6af97)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint2,uint2>(__context__,nullptr,__funcname_rename_at_237_54,__t_rename_at_237_52,das_invoke_function<uint2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint2 S<faker::Faker>*/ 0xceadb567a9b81609)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)),das_invoke_function<uint2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint2 S<faker::Faker>*/ 0xceadb567a9b81609)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint3,uint3>(__context__,nullptr,__funcname_rename_at_237_54,__t_rename_at_237_52,das_invoke_function<uint3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint3 S<faker::Faker>*/ 0xdc9cf75c88c69b09)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)),das_invoke_function<uint3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint3 S<faker::Faker>*/ 0xdc9cf75c88c69b09)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint4,uint4>(__context__,nullptr,__funcname_rename_at_237_54,__t_rename_at_237_52,das_invoke_function<uint4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint4 S<faker::Faker>*/ 0xe2d38a5eaa391409)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)),das_invoke_function<uint4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint4 S<faker::Faker>*/ 0xe2d38a5eaa391409)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float2,float2>(__context__,nullptr,__funcname_rename_at_237_54,__t_rename_at_237_52,das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)),das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float3,float3>(__context__,nullptr,__funcname_rename_at_237_54,__t_rename_at_237_52,das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)),das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float4,float4>(__context__,nullptr,__funcname_rename_at_237_54,__t_rename_at_237_52,das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)),das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_237_53)));
}

inline void fuzz_numeric_vec_scal_op2_52ad44f3d93412e3 ( Context * __context__, testing::T * const  __t_rename_at_259_55, faker::Faker &  __fake_rename_at_259_56, char * const  __funcname_rename_at_259_57 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int2,int32_t>(__context__,nullptr,__funcname_rename_at_259_57,__t_rename_at_259_55,das_invoke_function<int2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int2 S<faker::Faker>*/ 0x4e735764aa61d3b5)),das_arg<faker::Faker>::pass(__fake_rename_at_259_56)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_259_56)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int3,int32_t>(__context__,nullptr,__funcname_rename_at_259_57,__t_rename_at_259_55,das_invoke_function<int3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int3 S<faker::Faker>*/ 0x2c26493dc762dfc2)),das_arg<faker::Faker>::pass(__fake_rename_at_259_56)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_259_56)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int4,int32_t>(__context__,nullptr,__funcname_rename_at_259_57,__t_rename_at_259_55,das_invoke_function<int4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int4 S<faker::Faker>*/ 0x1d5187bee4c6af97)),das_arg<faker::Faker>::pass(__fake_rename_at_259_56)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_259_56)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint2,uint32_t>(__context__,nullptr,__funcname_rename_at_259_57,__t_rename_at_259_55,das_invoke_function<uint2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint2 S<faker::Faker>*/ 0xceadb567a9b81609)),das_arg<faker::Faker>::pass(__fake_rename_at_259_56)),das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_259_56)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint3,uint32_t>(__context__,nullptr,__funcname_rename_at_259_57,__t_rename_at_259_55,das_invoke_function<uint3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint3 S<faker::Faker>*/ 0xdc9cf75c88c69b09)),das_arg<faker::Faker>::pass(__fake_rename_at_259_56)),das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_259_56)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint4,uint32_t>(__context__,nullptr,__funcname_rename_at_259_57,__t_rename_at_259_55,das_invoke_function<uint4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint4 S<faker::Faker>*/ 0xe2d38a5eaa391409)),das_arg<faker::Faker>::pass(__fake_rename_at_259_56)),das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_259_56)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float2,float>(__context__,nullptr,__funcname_rename_at_259_57,__t_rename_at_259_55,das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_259_56)),das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_259_56)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float3,float>(__context__,nullptr,__funcname_rename_at_259_57,__t_rename_at_259_55,das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_259_56)),das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_259_56)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float4,float>(__context__,nullptr,__funcname_rename_at_259_57,__t_rename_at_259_55,das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_259_56)),das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_259_56)));
}

inline void fuzz_numeric_scal_vec_op2_fd0677cf42c35c93 ( Context * __context__, testing::T * const  __t_rename_at_274_58, faker::Faker &  __fake_rename_at_274_59, char * const  __funcname_rename_at_274_60 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int32_t,int2>(__context__,nullptr,__funcname_rename_at_274_60,__t_rename_at_274_58,das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_274_59)),das_invoke_function<int2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int2 S<faker::Faker>*/ 0x4e735764aa61d3b5)),das_arg<faker::Faker>::pass(__fake_rename_at_274_59)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int32_t,int3>(__context__,nullptr,__funcname_rename_at_274_60,__t_rename_at_274_58,das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_274_59)),das_invoke_function<int3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int3 S<faker::Faker>*/ 0x2c26493dc762dfc2)),das_arg<faker::Faker>::pass(__fake_rename_at_274_59)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int32_t,int4>(__context__,nullptr,__funcname_rename_at_274_60,__t_rename_at_274_58,das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_274_59)),das_invoke_function<int4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int4 S<faker::Faker>*/ 0x1d5187bee4c6af97)),das_arg<faker::Faker>::pass(__fake_rename_at_274_59)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint32_t,uint2>(__context__,nullptr,__funcname_rename_at_274_60,__t_rename_at_274_58,das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_274_59)),das_invoke_function<uint2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint2 S<faker::Faker>*/ 0xceadb567a9b81609)),das_arg<faker::Faker>::pass(__fake_rename_at_274_59)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint32_t,uint3>(__context__,nullptr,__funcname_rename_at_274_60,__t_rename_at_274_58,das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_274_59)),das_invoke_function<uint3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint3 S<faker::Faker>*/ 0xdc9cf75c88c69b09)),das_arg<faker::Faker>::pass(__fake_rename_at_274_59)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint32_t,uint4>(__context__,nullptr,__funcname_rename_at_274_60,__t_rename_at_274_58,das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_274_59)),das_invoke_function<uint4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint4 S<faker::Faker>*/ 0xe2d38a5eaa391409)),das_arg<faker::Faker>::pass(__fake_rename_at_274_59)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float,float2>(__context__,nullptr,__funcname_rename_at_274_60,__t_rename_at_274_58,das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_274_59)),das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_274_59)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float,float3>(__context__,nullptr,__funcname_rename_at_274_60,__t_rename_at_274_58,das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_274_59)),das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_274_59)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float,float4>(__context__,nullptr,__funcname_rename_at_274_60,__t_rename_at_274_58,das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_274_59)),das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_274_59)));
}

inline void fuzz_int_vector_op2_dc7f59e64eb3fb95 ( Context * __context__, testing::T * const  __t_rename_at_289_61, faker::Faker &  __fake_rename_at_289_62, char * const  __funcname_rename_at_289_63 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int32_t,int32_t>(__context__,nullptr,__funcname_rename_at_289_63,__t_rename_at_289_61,das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_289_62)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_289_62)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint32_t,uint32_t>(__context__,nullptr,__funcname_rename_at_289_63,__t_rename_at_289_61,das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_289_62)),das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_289_62)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int2,int2>(__context__,nullptr,__funcname_rename_at_289_63,__t_rename_at_289_61,das_invoke_function<int2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int2 S<faker::Faker>*/ 0x4e735764aa61d3b5)),das_arg<faker::Faker>::pass(__fake_rename_at_289_62)),das_invoke_function<int2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int2 S<faker::Faker>*/ 0x4e735764aa61d3b5)),das_arg<faker::Faker>::pass(__fake_rename_at_289_62)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int3,int3>(__context__,nullptr,__funcname_rename_at_289_63,__t_rename_at_289_61,das_invoke_function<int3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int3 S<faker::Faker>*/ 0x2c26493dc762dfc2)),das_arg<faker::Faker>::pass(__fake_rename_at_289_62)),das_invoke_function<int3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int3 S<faker::Faker>*/ 0x2c26493dc762dfc2)),das_arg<faker::Faker>::pass(__fake_rename_at_289_62)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int4,int4>(__context__,nullptr,__funcname_rename_at_289_63,__t_rename_at_289_61,das_invoke_function<int4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int4 S<faker::Faker>*/ 0x1d5187bee4c6af97)),das_arg<faker::Faker>::pass(__fake_rename_at_289_62)),das_invoke_function<int4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int4 S<faker::Faker>*/ 0x1d5187bee4c6af97)),das_arg<faker::Faker>::pass(__fake_rename_at_289_62)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint2,uint2>(__context__,nullptr,__funcname_rename_at_289_63,__t_rename_at_289_61,das_invoke_function<uint2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint2 S<faker::Faker>*/ 0xceadb567a9b81609)),das_arg<faker::Faker>::pass(__fake_rename_at_289_62)),das_invoke_function<uint2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint2 S<faker::Faker>*/ 0xceadb567a9b81609)),das_arg<faker::Faker>::pass(__fake_rename_at_289_62)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint3,uint3>(__context__,nullptr,__funcname_rename_at_289_63,__t_rename_at_289_61,das_invoke_function<uint3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint3 S<faker::Faker>*/ 0xdc9cf75c88c69b09)),das_arg<faker::Faker>::pass(__fake_rename_at_289_62)),das_invoke_function<uint3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint3 S<faker::Faker>*/ 0xdc9cf75c88c69b09)),das_arg<faker::Faker>::pass(__fake_rename_at_289_62)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint4,uint4>(__context__,nullptr,__funcname_rename_at_289_63,__t_rename_at_289_61,das_invoke_function<uint4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint4 S<faker::Faker>*/ 0xe2d38a5eaa391409)),das_arg<faker::Faker>::pass(__fake_rename_at_289_62)),das_invoke_function<uint4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint4 S<faker::Faker>*/ 0xe2d38a5eaa391409)),das_arg<faker::Faker>::pass(__fake_rename_at_289_62)));
}

inline void fuzz_shift_op2_11a978107d3af929 ( Context * __context__, testing::T * const  __t_rename_at_303_64, faker::Faker &  __fake_rename_at_303_65, char * const  __funcname_rename_at_303_66 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int32_t,int32_t>(__context__,nullptr,__funcname_rename_at_303_66,__t_rename_at_303_64,das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_303_65)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_303_65)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint32_t,uint32_t>(__context__,nullptr,__funcname_rename_at_303_66,__t_rename_at_303_64,das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_303_65)),das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_303_65)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int2,int32_t>(__context__,nullptr,__funcname_rename_at_303_66,__t_rename_at_303_64,das_invoke_function<int2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int2 S<faker::Faker>*/ 0x4e735764aa61d3b5)),das_arg<faker::Faker>::pass(__fake_rename_at_303_65)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_303_65)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int3,int32_t>(__context__,nullptr,__funcname_rename_at_303_66,__t_rename_at_303_64,das_invoke_function<int3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int3 S<faker::Faker>*/ 0x2c26493dc762dfc2)),das_arg<faker::Faker>::pass(__fake_rename_at_303_65)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_303_65)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int4,int32_t>(__context__,nullptr,__funcname_rename_at_303_66,__t_rename_at_303_64,das_invoke_function<int4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int4 S<faker::Faker>*/ 0x1d5187bee4c6af97)),das_arg<faker::Faker>::pass(__fake_rename_at_303_65)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_303_65)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint2,int32_t>(__context__,nullptr,__funcname_rename_at_303_66,__t_rename_at_303_64,das_invoke_function<uint2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint2 S<faker::Faker>*/ 0xceadb567a9b81609)),das_arg<faker::Faker>::pass(__fake_rename_at_303_65)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_303_65)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint3,int32_t>(__context__,nullptr,__funcname_rename_at_303_66,__t_rename_at_303_64,das_invoke_function<uint3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint3 S<faker::Faker>*/ 0xdc9cf75c88c69b09)),das_arg<faker::Faker>::pass(__fake_rename_at_303_65)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_303_65)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint4,int32_t>(__context__,nullptr,__funcname_rename_at_303_66,__t_rename_at_303_64,das_invoke_function<uint4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint4 S<faker::Faker>*/ 0xe2d38a5eaa391409)),das_arg<faker::Faker>::pass(__fake_rename_at_303_65)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_303_65)));
}

inline void fuzz_rotate_op2_e76ebde963d165d3 ( Context * __context__, testing::T * const  __t_rename_at_317_67, faker::Faker &  __fake_rename_at_317_68, char * const  __funcname_rename_at_317_69 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int32_t,int32_t>(__context__,nullptr,__funcname_rename_at_317_69,__t_rename_at_317_67,das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_317_68)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_317_68)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint32_t,uint32_t>(__context__,nullptr,__funcname_rename_at_317_69,__t_rename_at_317_67,das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_317_68)),das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_317_68)));
}

inline void fuzz_numeric_op3_c254164d54c65bef ( Context * __context__, testing::T * const  __t_rename_at_325_70, faker::Faker &  __fake_rename_at_325_71, char * const  __funcname_rename_at_325_72 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int32_t,int32_t,int32_t>(__context__,nullptr,__funcname_rename_at_325_72,__t_rename_at_325_70,das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_325_71)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_325_71)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_325_71)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint32_t,uint32_t,uint32_t>(__context__,nullptr,__funcname_rename_at_325_72,__t_rename_at_325_70,das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_325_71)),das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_325_71)),das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_325_71)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float,float,float>(__context__,nullptr,__funcname_rename_at_325_72,__t_rename_at_325_70,das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_325_71)),das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_325_71)),das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_325_71)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,double,double,double>(__context__,nullptr,__funcname_rename_at_325_72,__t_rename_at_325_70,das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_325_71)),das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_325_71)),das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_325_71)));
}

inline void fuzz_vec_op3_b8b2d31bb7eb72a3 ( Context * __context__, testing::T * const  __t_rename_at_335_73, faker::Faker &  __fake_rename_at_335_74, char * const  __funcname_rename_at_335_75 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float2,float2,float2>(__context__,nullptr,__funcname_rename_at_335_75,__t_rename_at_335_73,das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)),das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)),das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float3,float3,float3>(__context__,nullptr,__funcname_rename_at_335_75,__t_rename_at_335_73,das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)),das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)),das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float4,float4,float4>(__context__,nullptr,__funcname_rename_at_335_75,__t_rename_at_335_73,das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)),das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)),das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int2,int2,int2>(__context__,nullptr,__funcname_rename_at_335_75,__t_rename_at_335_73,das_invoke_function<int2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int2 S<faker::Faker>*/ 0x4e735764aa61d3b5)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)),das_invoke_function<int2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int2 S<faker::Faker>*/ 0x4e735764aa61d3b5)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)),das_invoke_function<int2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int2 S<faker::Faker>*/ 0x4e735764aa61d3b5)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int3,int3,int3>(__context__,nullptr,__funcname_rename_at_335_75,__t_rename_at_335_73,das_invoke_function<int3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int3 S<faker::Faker>*/ 0x2c26493dc762dfc2)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)),das_invoke_function<int3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int3 S<faker::Faker>*/ 0x2c26493dc762dfc2)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)),das_invoke_function<int3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int3 S<faker::Faker>*/ 0x2c26493dc762dfc2)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int4,int4,int4>(__context__,nullptr,__funcname_rename_at_335_75,__t_rename_at_335_73,das_invoke_function<int4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int4 S<faker::Faker>*/ 0x1d5187bee4c6af97)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)),das_invoke_function<int4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int4 S<faker::Faker>*/ 0x1d5187bee4c6af97)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)),das_invoke_function<int4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int4 S<faker::Faker>*/ 0x1d5187bee4c6af97)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint2,uint2,uint2>(__context__,nullptr,__funcname_rename_at_335_75,__t_rename_at_335_73,das_invoke_function<uint2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint2 S<faker::Faker>*/ 0xceadb567a9b81609)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)),das_invoke_function<uint2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint2 S<faker::Faker>*/ 0xceadb567a9b81609)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)),das_invoke_function<uint2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint2 S<faker::Faker>*/ 0xceadb567a9b81609)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint3,uint3,uint3>(__context__,nullptr,__funcname_rename_at_335_75,__t_rename_at_335_73,das_invoke_function<uint3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint3 S<faker::Faker>*/ 0xdc9cf75c88c69b09)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)),das_invoke_function<uint3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint3 S<faker::Faker>*/ 0xdc9cf75c88c69b09)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)),das_invoke_function<uint3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint3 S<faker::Faker>*/ 0xdc9cf75c88c69b09)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint4,uint4,uint4>(__context__,nullptr,__funcname_rename_at_335_75,__t_rename_at_335_73,das_invoke_function<uint4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint4 S<faker::Faker>*/ 0xe2d38a5eaa391409)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)),das_invoke_function<uint4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint4 S<faker::Faker>*/ 0xe2d38a5eaa391409)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)),das_invoke_function<uint4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint4 S<faker::Faker>*/ 0xe2d38a5eaa391409)),das_arg<faker::Faker>::pass(__fake_rename_at_335_74)));
}

inline void fuzz_vec_mad_op3_92bb9a6e84ac3c0b ( Context * __context__, testing::T * const  __t_rename_at_350_76, faker::Faker &  __fake_rename_at_350_77, char * const  __funcname_rename_at_350_78 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float2,float,float2>(__context__,nullptr,__funcname_rename_at_350_78,__t_rename_at_350_76,das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)),das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)),das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float3,float,float3>(__context__,nullptr,__funcname_rename_at_350_78,__t_rename_at_350_76,das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)),das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)),das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float4,float,float4>(__context__,nullptr,__funcname_rename_at_350_78,__t_rename_at_350_76,das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)),das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)),das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int2,int32_t,int2>(__context__,nullptr,__funcname_rename_at_350_78,__t_rename_at_350_76,das_invoke_function<int2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int2 S<faker::Faker>*/ 0x4e735764aa61d3b5)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)),das_invoke_function<int2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int2 S<faker::Faker>*/ 0x4e735764aa61d3b5)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int3,int32_t,int3>(__context__,nullptr,__funcname_rename_at_350_78,__t_rename_at_350_76,das_invoke_function<int3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int3 S<faker::Faker>*/ 0x2c26493dc762dfc2)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)),das_invoke_function<int3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int3 S<faker::Faker>*/ 0x2c26493dc762dfc2)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int4,int32_t,int4>(__context__,nullptr,__funcname_rename_at_350_78,__t_rename_at_350_76,das_invoke_function<int4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int4 S<faker::Faker>*/ 0x1d5187bee4c6af97)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)),das_invoke_function<int4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int4 S<faker::Faker>*/ 0x1d5187bee4c6af97)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint2,uint32_t,uint2>(__context__,nullptr,__funcname_rename_at_350_78,__t_rename_at_350_76,das_invoke_function<uint2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint2 S<faker::Faker>*/ 0xceadb567a9b81609)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)),das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)),das_invoke_function<uint2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint2 S<faker::Faker>*/ 0xceadb567a9b81609)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint3,uint32_t,uint3>(__context__,nullptr,__funcname_rename_at_350_78,__t_rename_at_350_76,das_invoke_function<uint3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint3 S<faker::Faker>*/ 0xdc9cf75c88c69b09)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)),das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)),das_invoke_function<uint3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint3 S<faker::Faker>*/ 0xdc9cf75c88c69b09)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint4,uint32_t,uint4>(__context__,nullptr,__funcname_rename_at_350_78,__t_rename_at_350_76,das_invoke_function<uint4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint4 S<faker::Faker>*/ 0xe2d38a5eaa391409)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)),das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)),das_invoke_function<uint4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint4 S<faker::Faker>*/ 0xe2d38a5eaa391409)),das_arg<faker::Faker>::pass(__fake_rename_at_350_77)));
}

inline void fuzz_float_double_or_float_vec_op3_4985cd878555494d ( Context * __context__, testing::T * const  __t_rename_at_365_79, faker::Faker &  __fake_rename_at_365_80, char * const  __funcname_rename_at_365_81 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float,float,float>(__context__,nullptr,__funcname_rename_at_365_81,__t_rename_at_365_79,das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_365_80)),das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_365_80)),das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_365_80)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,double,double,double>(__context__,nullptr,__funcname_rename_at_365_81,__t_rename_at_365_79,das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_365_80)),das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_365_80)),das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_365_80)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float2,float2,float2>(__context__,nullptr,__funcname_rename_at_365_81,__t_rename_at_365_79,das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_365_80)),das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_365_80)),das_invoke_function<float2>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float2 S<faker::Faker>*/ 0xb12590d5c18e85b8)),das_arg<faker::Faker>::pass(__fake_rename_at_365_80)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float3,float3,float3>(__context__,nullptr,__funcname_rename_at_365_81,__t_rename_at_365_79,das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_365_80)),das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_365_80)),das_invoke_function<float3>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float3 S<faker::Faker>*/ 0x3fec14acf97aa66b)),das_arg<faker::Faker>::pass(__fake_rename_at_365_80)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float4,float4,float4>(__context__,nullptr,__funcname_rename_at_365_81,__t_rename_at_365_79,das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_365_80)),das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_365_80)),das_invoke_function<float4>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float4 S<faker::Faker>*/ 0x9e0d82ee17b5540e)),das_arg<faker::Faker>::pass(__fake_rename_at_365_80)));
}

inline void fuzz_numeric_op4_16e37608179824b9 ( Context * __context__, testing::T * const  __t_rename_at_377_82, faker::Faker &  __fake_rename_at_377_83, char * const  __funcname_rename_at_377_84 )
{
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,int32_t,int32_t,int32_t,int32_t>(__context__,nullptr,__funcname_rename_at_377_84,__t_rename_at_377_82,das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_377_83)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_377_83)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_377_83)),das_invoke_function<int32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_int S<faker::Faker>*/ 0x55ff764b8725971c)),das_arg<faker::Faker>::pass(__fake_rename_at_377_83)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,uint32_t,uint32_t,uint32_t,uint32_t>(__context__,nullptr,__funcname_rename_at_377_84,__t_rename_at_377_82,das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_377_83)),das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_377_83)),das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_377_83)),das_invoke_function<uint32_t>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_uint S<faker::Faker>*/ 0xb79665e62110de44)),das_arg<faker::Faker>::pass(__fake_rename_at_377_83)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,float,float,float,float>(__context__,nullptr,__funcname_rename_at_377_84,__t_rename_at_377_82,das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_377_83)),das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_377_83)),das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_377_83)),das_invoke_function<float>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_float S<faker::Faker>*/ 0x8a548e2978b69d8f)),das_arg<faker::Faker>::pass(__fake_rename_at_377_83)));
    das_invoke_function_by_name<void>::invoke<testing::T * const  &,double,double,double,double>(__context__,nullptr,__funcname_rename_at_377_84,__t_rename_at_377_82,das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_377_83)),das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_377_83)),das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_377_83)),das_invoke_function<double>::invoke<faker::Faker &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@faker::random_double S<faker::Faker>*/ 0xc10504c7632ea553)),das_arg<faker::Faker>::pass(__fake_rename_at_377_83)));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x2eb8d27740a137d5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_b284ca1047c9135d>>();
    };
    aotLib[0xc38d362f7f7784c5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_26c215c38d1dfd1a>>();
    };
    aotLib[0x10a00d2c416dd160] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_debug_93358114940f0b68>>();
    };
    aotLib[0x18bb6cb48f9a66a5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_debug_3b504f8008b955ef>>();
    };
    aotLib[0x780b8e70ed847415] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_numeric_and_vector_op1_48ce713c34be8575>>();
    };
    aotLib[0x61eb1fd7b1045ad4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_numeric_and_vector_signed_op1_cf02787541ef8a1f>>();
    };
    aotLib[0x59548ca1defe16f6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_numeric_op1_7e39ee71d5eea21b>>();
    };
    aotLib[0x6d7979c6d31d4371] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_numeric_and_storage_op1_14e5fe2cc526dd1f>>();
    };
    aotLib[0x7666ef3e5524521] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_all_ints_op1_e805e6460a35229d>>();
    };
    aotLib[0x8d05c17309c52e08] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_all_unsigned_ints_op1_f9f52b078e024909>>();
    };
    aotLib[0x15f37b41c1b0fce2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_float_double_or_float_vec_op1_3652ef8150f5c24d>>();
    };
    aotLib[0x530f76cf54d23275] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_float_or_float_vec_op1_78b3a33e9c015725>>();
    };
    aotLib[0x24a846741252cede] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_float_or_float_vec_op2_ae83307b5cfedd1>>();
    };
    aotLib[0x1018d9bc4286a4e4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_float_double_or_float_vec_op2_a264e3ee00149f0b>>();
    };
    aotLib[0x72d1813fe6e956ae] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_numeric_and_vector_op2_f57314e6b07c00d3>>();
    };
    aotLib[0x878fb70d15061de] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_numeric_and_vector_op2_no_unint_vec_f9fe354762f0d437>>();
    };
    aotLib[0x9473824addbd1324] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_numeric_op2_92b03af12a4daff9>>();
    };
    aotLib[0xf7378cf35659ab1c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_compareable_op2_87dd0992261931a1>>();
    };
    aotLib[0x468069dbd119ef3a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_eq_neq_op2_97177b59c300d73d>>();
    };
    aotLib[0x6d83b150a7e1c3ae] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_numeric_vec_scal_op2_52ad44f3d93412e3>>();
    };
    aotLib[0x2c246fecf261efc9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_numeric_scal_vec_op2_fd0677cf42c35c93>>();
    };
    aotLib[0x15a8c6e9fe4ce25f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_int_vector_op2_dc7f59e64eb3fb95>>();
    };
    aotLib[0x9d8764779c4a8fc2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_shift_op2_11a978107d3af929>>();
    };
    aotLib[0xf492e9c3b70ac051] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_rotate_op2_e76ebde963d165d3>>();
    };
    aotLib[0x31c2adea3087c8aa] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_numeric_op3_c254164d54c65bef>>();
    };
    aotLib[0xc696668b8b8904c6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_vec_op3_b8b2d31bb7eb72a3>>();
    };
    aotLib[0xdea5f22560d6f906] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_vec_mad_op3_92bb9a6e84ac3c0b>>();
    };
    aotLib[0xb74f8bc5ac43b49a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_float_double_or_float_vec_op3_4985cd878555494d>>();
    };
    aotLib[0xf2daee70032abf86] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fuzz_numeric_op4_16e37608179824b9>>();
    };
    // [[ init script ]]
    aotLib[0xad44ac4628907d9b] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
	resolveTypeInfoAnnotations();
}

} // namespace _anon_14412364654671388840
AotListBase impl_aot_fuzzer(_anon_14412364654671388840::registerAotFunctions);
} // namespace das

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
#if defined(__EDG__)
#pragma diag_default 826
#elif defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
