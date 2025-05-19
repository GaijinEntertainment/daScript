#include "daScript/simulate/standalone_ctx_utils.h"
 // require builtin
 // require strings
#include "daScript/simulate/aot_builtin_string.h"
 // require fio
#include "daScript/misc/performance_time.h"
#include "daScript/simulate/aot_builtin_fio.h"
 // require rtti
#include "daScript/simulate/aot_builtin_rtti.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_handle.h"
 // require ast
#include "daScript/ast/ast.h"
#include "daScript/simulate/aot_builtin_ast.h"
#include "daScript/ast/ast_generate.h"
 // require math
#include "daScript/simulate/aot_builtin_math.h"
 // require strings_boost
 // require ast_boost
 // require templates
 // require contracts
 // require array_boost
 // require algorithm
 // require templates_boost
 // require macro_boost
 // require defer
 // require match
 // require functional
 // require printer_flags_visitor
 // require aot_constants
 // require ast_aot_cpp
 // require standalone_contexts
#include "main.das.h"

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
namespace _anon_6563252783937865275 {
namespace fio { struct df_header; };
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
namespace macro_boost { struct MacroVerifyMacro; };
namespace macro_boost { struct CaptureBlock; };
namespace macro_boost { struct CapturedVariable; };
namespace macro_boost { struct ColletFinally; };
namespace macro_boost { struct ColletLabels; };
namespace macro_boost { struct ReturnSkipLockcheck; };
namespace defer { struct DeferMacro; };
namespace defer { struct DeferDeleteMacro; };
namespace match { struct MatchError; };
namespace match { struct MatchTo; };
namespace match { struct MatchMacro; };
namespace match { struct StaticMatchMacro; };
namespace match { struct MultiMatchMacro; };
namespace match { struct StaticMultiMatchMacro; };
namespace match { struct MatchAsIs; };
namespace match { struct MatchCopy; };
namespace printer_flags_visitor { struct SetPrinterFlags; };
namespace ast_aot_cpp { struct DescribeConfig; };
namespace ast_aot_cpp { struct NoAotMarker; };
namespace ast_aot_cpp { struct PrologueMarker; };
namespace ast_aot_cpp { struct UseTypeMarker; };
namespace ast_aot_cpp { struct AotDebugInfoHelper; };
namespace ast_aot_cpp { struct BlockVariableCollector; };
namespace ast_aot_cpp { struct CppAot; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_508_1; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_516_2; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_524_3; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_613_4; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_642_5; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_665_6; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_1550_7; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_1783_8; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_281_9; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_291_10; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_355_11; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_363_12; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_923_13; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_2307_14; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_2352_15; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_2954_16; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_2828_17; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_43_18; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_43_19; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_43_20; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_43_21; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_43_22; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_43_23; };
namespace ast_aot_cpp { struct _lambda_ast_aot_cpp_43_24; };
namespace standalone_contexts { struct StandaloneContextCfg; };
namespace standalone_contexts { struct StandaloneContextGen; };
namespace standalone_contexts { struct _lambda_standalone_contexts_86_1; };
namespace standalone_contexts { struct _lambda_standalone_contexts_43_2; };
// unused enumeration ConversionResult
// unused structure df_header
// unused enumeration CompilationError
// unused enumeration ConstMatters
// unused enumeration RefMatters
// unused enumeration TemporaryMatters
// unused enumeration Type
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
// unused structure MacroVerifyMacro
// unused structure CaptureBlock
// unused structure CapturedVariable
// unused structure ColletFinally
// unused structure ColletLabels
// unused structure ReturnSkipLockcheck
// unused structure DeferMacro
// unused structure DeferDeleteMacro
// unused enumeration MatchType
// unused structure MatchError
// unused structure MatchTo
// unused structure MatchMacro
// unused structure StaticMatchMacro
// unused structure MultiMatchMacro
// unused structure StaticMultiMatchMacro
// unused structure MatchAsIs
// unused structure MatchCopy
// unused structure SetPrinterFlags
// unused enumeration CpptRedundantConst
// unused enumeration CpptSkipConst
// unused enumeration CpptSkipRef
// unused enumeration CpptSubstitureRef
// unused enumeration CpptUseAlias
// unused structure DescribeConfig
// unused structure NoAotMarker
// unused structure PrologueMarker
// unused structure UseTypeMarker
// unused structure AotDebugInfoHelper
// unused structure BlockVariableCollector
// unused structure CppAot
// unused structure _lambda_ast_aot_cpp_508_1
// unused structure _lambda_ast_aot_cpp_516_2
// unused structure _lambda_ast_aot_cpp_524_3
// unused structure _lambda_ast_aot_cpp_613_4
// unused structure _lambda_ast_aot_cpp_642_5
// unused structure _lambda_ast_aot_cpp_665_6
// unused structure _lambda_ast_aot_cpp_1550_7
// unused structure _lambda_ast_aot_cpp_1783_8
// unused structure _lambda_ast_aot_cpp_281_9
// unused structure _lambda_ast_aot_cpp_291_10
// unused structure _lambda_ast_aot_cpp_355_11
// unused structure _lambda_ast_aot_cpp_363_12
// unused structure _lambda_ast_aot_cpp_923_13
// unused structure _lambda_ast_aot_cpp_2307_14
// unused structure _lambda_ast_aot_cpp_2352_15
// unused structure _lambda_ast_aot_cpp_2954_16
// unused structure _lambda_ast_aot_cpp_2828_17
// unused structure _lambda_ast_aot_cpp_43_18
// unused structure _lambda_ast_aot_cpp_43_19
// unused structure _lambda_ast_aot_cpp_43_20
// unused structure _lambda_ast_aot_cpp_43_21
// unused structure _lambda_ast_aot_cpp_43_22
// unused structure _lambda_ast_aot_cpp_43_23
// unused structure _lambda_ast_aot_cpp_43_24
// unused structure StandaloneContextCfg
// unused structure StandaloneContextGen
// unused structure _lambda_standalone_contexts_86_1
// unused structure _lambda_standalone_contexts_43_2

extern TypeInfo __type_info__12393804d99ce574;
extern TypeInfo __type_info__16d0aa3dd6b69257;
extern TypeInfo __type_info__52c096950f9c0766;
extern TypeInfo __type_info__af81fe4c86352052;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__af63ee4c86020b22;
extern TypeInfo __type_info__af63eb4c86020609;
extern VarInfo __var_info__1cc58ac20f6548da;
extern VarInfo __var_info__c7300f53c4a80931;
extern VarInfo __var_info__d4cad117337cc462;
extern VarInfo __var_info__59a0b05855762103;
extern VarInfo __var_info__fcf65cc491eb77de;
extern VarInfo __var_info__13cb32390474ab3d;
extern VarInfo __var_info__839dbeaf015a785d;
extern FuncInfo __func_info__c508c4a3fccd2cca;
extern FuncInfo __func_info__1474780a87ee3908;
extern FuncInfo __func_info__9edf2eff375e5b54;
extern FuncInfo __func_info__aec066351adb341f;
extern FuncInfo __func_info__793c57c1b60ae74a;
extern FuncInfo __func_info__723153080f205d43;

FuncInfo __func_info__c508c4a3fccd2cca = {"builtin`get_command_line_arguments`14327939727965569528", "", nullptr, 0, 32, &__type_info__12393804d99ce574, nullptr,0,UINT64_C(0xc508c4a3fccd2cca), 0x4 };
VarInfo __func_info__1474780a87ee3908_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57378, 24, UINT64_C(0x1cc58ac20f6548da), "args", 0, 0 };
VarInfo __func_info__1474780a87ee3908_field_1 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xfcf65cc491eb77de), "key", 0, 0 };
VarInfo * __func_info__1474780a87ee3908_fields[2] =  { &__func_info__1474780a87ee3908_field_0, &__func_info__1474780a87ee3908_field_1 };
FuncInfo __func_info__1474780a87ee3908 = {"find_argument", "", __func_info__1474780a87ee3908_fields, 2, 48, &__type_info__af90fe4c864e9d52, nullptr,0,UINT64_C(0x1474780a87ee3908), 0x0 };
VarInfo __func_info__9edf2eff375e5b54_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57378, 24, UINT64_C(0x1cc58ac20f6548da), "args", 0, 0 };
VarInfo __func_info__9edf2eff375e5b54_field_1 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xfcf65cc491eb77de), "key", 0, 0 };
VarInfo __func_info__9edf2eff375e5b54_field_2 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xd4cad117337cc462), "def_val", 0, 0 };
VarInfo * __func_info__9edf2eff375e5b54_fields[3] =  { &__func_info__9edf2eff375e5b54_field_0, &__func_info__9edf2eff375e5b54_field_1, &__func_info__9edf2eff375e5b54_field_2 };
FuncInfo __func_info__9edf2eff375e5b54 = {"find_argument_or", "", __func_info__9edf2eff375e5b54_fields, 3, 48, &__type_info__af90fe4c864e9d52, nullptr,0,UINT64_C(0x9edf2eff375e5b54), 0x0 };
VarInfo __func_info__aec066351adb341f_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0x839dbeaf015a785d), "name", 0, 0 };
VarInfo __func_info__aec066351adb341f_field_1 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0x13cb32390474ab3d), "mode", 0, 0 };
TypeInfo * __type_info__c7300f53c4a80931_arg_types_var_12592176936397779999[1] = { &__type_info__52c096950f9c0766 };
const char * __type_info__c7300f53c4a80931_arg_names_var_12592176936397779999[1] = { "f" };
VarInfo __func_info__aec066351adb341f_field_2 =  { Type::tBlock, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__c7300f53c4a80931_arg_types_var_12592176936397779999, __type_info__c7300f53c4a80931_arg_names_var_12592176936397779999, 1, 0, nullptr, 34, 48, UINT64_C(0xc7300f53c4a80931), "blk", 0, 0 };
VarInfo * __func_info__aec066351adb341f_fields[3] =  { &__func_info__aec066351adb341f_field_0, &__func_info__aec066351adb341f_field_1, &__func_info__aec066351adb341f_field_2 };
FuncInfo __func_info__aec066351adb341f = {"fio`fopen`3937565566638487747", "", __func_info__aec066351adb341f_fields, 3, 48, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0xaec066351adb341f), 0x4 };
VarInfo __func_info__793c57c1b60ae74a_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__16d0aa3dd6b69257, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, 8, UINT64_C(0x59a0b05855762103), "fw", 0, 0 };
VarInfo * __func_info__793c57c1b60ae74a_fields[1] =  { &__func_info__793c57c1b60ae74a_field_0 };
FuncInfo __func_info__793c57c1b60ae74a = {"invoke block<(fw:fio::FILE const? const):void> const", "", __func_info__793c57c1b60ae74a_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x793c57c1b60ae74a), 0x0 };
FuncInfo __func_info__723153080f205d43 = {"main", "", nullptr, 0, 336, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x723153080f205d43), 0x0 };
TypeInfo __type_info__12393804d99ce574 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x12393804d99ce574) };
TypeInfo __type_info__16d0aa3dd6b69257 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~fio::FILE"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 34, 16, UINT64_C(0x16d0aa3dd6b69257) };
TypeInfo __type_info__52c096950f9c0766 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__16d0aa3dd6b69257, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, 8, UINT64_C(0x52c096950f9c0766) };
TypeInfo __type_info__af81fe4c86352052 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 1, UINT64_C(0xaf81fe4c86352052) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__af63ee4c86020b22 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xaf63ee4c86020b22) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__16d0aa3dd6b69257, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_1[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_2[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_3[7] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af81fe4c86352052, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_4[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_5[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };

inline void _FuncfioTickfopenTick3937565566638487747_3225c697e37c1bfa ( Context * __context__, char * const  __name_rename_at_12_0, char * const  __mode_rename_at_12_1, Block DAS_COMMENT((void,FILE const  * const )) const  & __blk_rename_at_12_2 );
inline TArray<char *> _FuncbuiltinTickget_command_line_argumentsTick14327939727965569528_4c51dd8b3477a086 ( Context * __context__ );
inline char * find_argument_or_14bb48686f57685c ( Context * __context__, TArray<char *> const  & __args_rename_at_8_5, char * const  __key_rename_at_8_6, char * const  __def_val_rename_at_8_7 );
inline char * find_argument_6e5fe233b4a580ad ( Context * __context__, TArray<char *> const  & __args_rename_at_17_9, char * const  __key_rename_at_17_10 );
inline void main_99f7022db5f9f6d ( Context * __context__ );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncfioTickfopenTick3937565566638487747_3225c697e37c1bfa ( Context * __context__, char * const  __name_rename_at_12_0, char * const  __mode_rename_at_12_1, Block DAS_COMMENT((void,FILE const  * const )) const  &  __blk_rename_at_12_2 )
{
    FILE const  * __f_rename_at_13_3 = ((FILE const  *)builtin_fopen(__name_rename_at_12_0,__mode_rename_at_12_1));
    das_invoke<void>::invoke<FILE const  * const >(__context__,nullptr,__blk_rename_at_12_2,__f_rename_at_13_3);
    if ( __f_rename_at_13_3 != nullptr )
    {
        builtin_fclose(__f_rename_at_13_3,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline TArray<char *> _FuncbuiltinTickget_command_line_argumentsTick14327939727965569528_4c51dd8b3477a086 ( Context * __context__ )
{
    TArray<char *> __args_rename_at_1753_4; das_zero(__args_rename_at_1753_4);
    getCommandLineArguments(das_arg<TArray<char *>>::pass(__args_rename_at_1753_4));
    return /* <- */ das_auto_cast_move<TArray<char *>>::cast(__args_rename_at_1753_4);
}

inline char * find_argument_or_14bb48686f57685c ( Context * __context__, TArray<char *> const  &  __args_rename_at_8_5, char * const  __key_rename_at_8_6, char * const  __def_val_rename_at_8_7 )
{
    {
        bool __need_loop_9 = true;
        // i: int const
        das_iterator<range> __i_iterator(mk_range(builtin_array_size(__args_rename_at_8_5) - 1));
        int32_t __i_rename_at_9_8;
        __need_loop_9 = __i_iterator.first(__context__,(__i_rename_at_9_8)) && __need_loop_9;
        for ( ; __need_loop_9 ; __need_loop_9 = __i_iterator.next(__context__,(__i_rename_at_9_8)) )
        {
            if ( SimPolicy<char *>::Equ(cast<char *>::from(__args_rename_at_8_5(__i_rename_at_9_8,__context__)),cast<char *>::from(__key_rename_at_8_6),*__context__,nullptr) )
            {
                return das_auto_cast<char *>::cast(__args_rename_at_8_5((__i_rename_at_9_8 + 1),__context__));
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_9_8));
    };
    return das_auto_cast<char *>::cast(__def_val_rename_at_8_7);
}

inline char * find_argument_6e5fe233b4a580ad ( Context * __context__, TArray<char *> const  &  __args_rename_at_17_9, char * const  __key_rename_at_17_10 )
{
    {
        bool __need_loop_18 = true;
        // i: int const
        das_iterator<range> __i_iterator(mk_range(builtin_array_size(__args_rename_at_17_9) - 1));
        int32_t __i_rename_at_18_11;
        __need_loop_18 = __i_iterator.first(__context__,(__i_rename_at_18_11)) && __need_loop_18;
        for ( ; __need_loop_18 ; __need_loop_18 = __i_iterator.next(__context__,(__i_rename_at_18_11)) )
        {
            if ( SimPolicy<char *>::Equ(cast<char *>::from(__args_rename_at_17_9(__i_rename_at_18_11,__context__)),cast<char *>::from(__key_rename_at_17_10),*__context__,nullptr) )
            {
                return das_auto_cast<char *>::cast(__args_rename_at_17_9((__i_rename_at_18_11 + 1),__context__));
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_18_11));
    };
    builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_0, cast<char * const >::from(((char *) "expected ")), cast<char * const >::from(__key_rename_at_17_10), cast<char * const >::from(((char *) " argument!")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return das_auto_cast<char *>::cast(nullptr);
}

inline void main_99f7022db5f9f6d ( Context * __context__ ) { das_stack_prologue __prologue(__context__,336,"main " DAS_FILE_LINE);
{
    TArray<char *> __args_rename_at_29_12_ConstRef; das_zero(__args_rename_at_29_12_ConstRef); das_move(__args_rename_at_29_12_ConstRef, ((TArray<char *>)_FuncbuiltinTickget_command_line_argumentsTick14327939727965569528_4c51dd8b3477a086(__context__)));
    TArray<char *> const  & __args_rename_at_29_12 = __args_rename_at_29_12_ConstRef; ;
    char * __mode_rename_at_31_13 = ((char *)(char *)(find_argument_6e5fe233b4a580ad(__context__,__args_rename_at_29_12,((char *) "--mode"))));
    if ( SimPolicy<char *>::Equ(cast<char *>::from(__mode_rename_at_31_13),cast<char *>::from(((char *) "aot")),*__context__,nullptr) )
    {
        char * __input_rename_at_33_14 = ((char *)(char *)(find_argument_6e5fe233b4a580ad(__context__,__args_rename_at_29_12,((char *) "--input"))));
        char * __output_rename_at_34_15 = ((char *)(char *)(find_argument_6e5fe233b4a580ad(__context__,__args_rename_at_29_12,((char *) "--output"))));
        bool __cross_platform_rename_at_35_16 = ((bool)(SimPolicy<char *>::Equ(cast<char *>::from(find_argument_or_14bb48686f57685c(__context__,__args_rename_at_29_12,((char *) "--cross_platform"),((char *) "false"))),cast<char *>::from(((char *) "true")),*__context__,nullptr)));
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_1, cast<char * const >::from(((char *) "aot ")), cast<char * const >::from(__input_rename_at_33_14), cast<char * const >::from(((char *) " to ")), cast<char * const >::from(__output_rename_at_34_15), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        char * __res_rename_at_37_17 = ((char *)(char *)(das_invoke_function<char * const >::invoke<char * const ,bool,bool,bool>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_aot_cpp::aot Cs Cb Cb Cb*/ 18133740268605143206u)),__input_rename_at_33_14,true,false,__cross_platform_rename_at_35_16)));
        if ( builtin_string_length(__res_rename_at_37_17,__context__) > 0 )
        {
            _FuncfioTickfopenTick3937565566638487747_3225c697e37c1bfa(__context__,__output_rename_at_34_15,((char *) "wb"),das_make_block<void,FILE const  * const >(__context__,192,0,&__func_info__793c57c1b60ae74a,[&](FILE const  * const  __fw_rename_at_39_18) -> void{
                if ( __fw_rename_at_39_18 != nullptr )
                {
                    builtin_fwrite(__fw_rename_at_39_18,__res_rename_at_37_17,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                } else {
                    builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_2, cast<char * const >::from(((char *) "Couldn't create output file ")), cast<char * const >::from(__output_rename_at_34_15))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                };
            }));
        };
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__mode_rename_at_31_13),cast<char *>::from(((char *) "standalone_ctx")),*__context__,nullptr) )
    {
        char * __input_rename_at_48_19 = ((char *)(char *)(find_argument_6e5fe233b4a580ad(__context__,__args_rename_at_29_12,((char *) "--input"))));
        char * __output_rename_at_49_20 = ((char *)(char *)(find_argument_6e5fe233b4a580ad(__context__,__args_rename_at_29_12,((char *) "--output"))));
        bool __cross_platform_rename_at_50_21 = ((bool)(SimPolicy<char *>::Equ(cast<char *>::from(find_argument_or_14bb48686f57685c(__context__,__args_rename_at_29_12,((char *) "--cross_platform"),((char *) "false"))),cast<char *>::from(((char *) "true")),*__context__,nullptr)));
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<7>(__tinfo_3, cast<char * const >::from(((char *) "standalone ctx ")), cast<char * const >::from(__input_rename_at_48_19), cast<char * const >::from(((char *) " to ")), cast<char * const >::from(__output_rename_at_49_20), cast<char * const >::from(((char *) " cross_platform=")), cast<bool>::from(__cross_platform_rename_at_50_21), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        char * __res_rename_at_52_22 = ((char *)(char *)(das_invoke_function<char * const >::invoke<char * const ,char * const ,bool,bool,bool>(__context__,nullptr,Func(__context__->fnByMangledName(/*@standalone_contexts::standalone_aot Cs Cs Cb Cb Cb*/ 3447988833651899129u)),__input_rename_at_48_19,__output_rename_at_49_20,false,__cross_platform_rename_at_50_21,false)));
        if ( builtin_string_length(__res_rename_at_52_22,__context__) > 0 )
        {
            _FuncfioTickfopenTick3937565566638487747_3225c697e37c1bfa(__context__,__output_rename_at_49_20,((char *) "wb"),das_make_block<void,FILE const  * const >(__context__,320,0,&__func_info__793c57c1b60ae74a,[&](FILE const  * const  __fw_rename_at_54_23) -> void{
                if ( __fw_rename_at_54_23 != nullptr )
                {
                    builtin_fwrite(__fw_rename_at_54_23,__res_rename_at_52_22,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                } else {
                    builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_4, cast<char * const >::from(((char *) "Couldn't create output file ")), cast<char * const >::from(__output_rename_at_49_20))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                };
            }));
        };
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__mode_rename_at_31_13),cast<char *>::from(((char *) "ast_print")),*__context__,nullptr) )
    {
        builtin_throw(((char *) "unimplemented"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    } else {
        builtin_throw(das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_5, cast<char * const >::from(((char *) "Unknown mode ")), cast<char * const >::from(__mode_rename_at_31_13))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}}
} // namespace _anon_6563252783937865275
using namespace _anon_6563252783937865275;

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xdef39bed9b8754ea] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncfioTickfopenTick3937565566638487747_3225c697e37c1bfa>>();
    };
    aotLib[0x62afc600b58de125] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickget_command_line_argumentsTick14327939727965569528_4c51dd8b3477a086>>();
    };
    aotLib[0x1e916c0972089f0c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&find_argument_or_14bb48686f57685c>>();
    };
    aotLib[0x9dac7ab38b2daadd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&find_argument_6e5fe233b4a580ad>>();
    };
    aotLib[0xf81d59e2085cb407] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&main_99f7022db5f9f6d>>();
    };
    resolveTypeInfoAnnotations();
};

AotListBase impl(registerAotFunctions);
namespace main {
auto Standalone::main ( ) -> void {
    return main_99f7022db5f9f6d(this);
}

Standalone::Standalone() {
    auto & context = *this;
    CodeOfPolicies policies;    policies.debugger = false /*policies.debugger*/;
    policies.persistent_heap = false;
    policies.heap_size_hint = 65536;
    policies.string_heap_size_hint = 65536;
    context.setup(0/*totalVariables*/, 14508 /*globalStringHeapSize*/, policies, {});
     // start totalVariables
     // end totalVariables

    context.globals = (char *) das_aligned_alloc16(context.globalsSize);
    context.shared = (char *) das_aligned_alloc16(context.sharedSize);
    context.sharedOwner = true;
    context.totalVariables = 0/*totalVariables*/;
    context.functions = (SimFunction *) context.code->allocate( 592/*totalFunctions*/*sizeof(SimFunction) );
    context.totalFunctions = 592/*totalFunctions*/;
    bool anyPInvoke = false;
    if ( anyPInvoke || false/*(policies.threadlock_context || policies.debugger)*/ ) {
        context.contextMutex = new recursive_mutex;
    }
    context.tabMnLookup = make_shared<das_hash_map<uint64_t,SimFunction *>>();
    context.tabMnLookup->clear();
     // start totalFunctions
    auto initFunctions = {
        std::make_tuple(0, FunctionInfo("fio`fopen`3937565566638487747", "fio`fopen`3937565566638487747 Cs Cs CN<f>0<CY<file>1<CH<fio::FILE>>?>1<v>$", 0xaec066351adb341f, 0xdef39bed9b8754ea, 48, false, false, false, false, false, false), &__func_info__aec066351adb341f),
        std::make_tuple(1, FunctionInfo("builtin`get_command_line_arguments`14327939727965569528", "builtin`get_command_line_arguments`14327939727965569528", 0xc508c4a3fccd2cca, 0x62afc600b58de125, 32, false, false, false, false, true, false), &__func_info__c508c4a3fccd2cca),
        std::make_tuple(2, FunctionInfo("find_argument_or", "find_argument_or C1<s>A Cs Cs", 0x9edf2eff375e5b54, 0x1e916c0972089f0c, 48, false, false, false, false, false, false), &__func_info__9edf2eff375e5b54),
        std::make_tuple(3, FunctionInfo("find_argument", "find_argument C1<s>A Cs", 0x1474780a87ee3908, 0x9dac7ab38b2daadd, 48, false, false, false, false, false, false), &__func_info__1474780a87ee3908),
        std::make_tuple(4, FunctionInfo("main", "main", 0x723153080f205d43, 0xf81d59e2085cb407, 336, false, false, false, false, false, false), &__func_info__723153080f205d43),
    };
    // end totalFunctions
    vector<pair<uint64_t, SimFunction*>> id_to_funcs;
    for (const auto& [index, func_info, debug_info]: initFunctions) {
        InitAotFunction(context, &context.functions[index], func_info);
        context.functions[index].debugInfo = debug_info;
        (*context.tabMnLookup)[func_info.mnh] = context.functions + index;
        id_to_funcs.emplace_back(index, &context.functions[index]);
        anyPInvoke |= func_info.pinvoke;
    }
    context.tabGMnLookup = make_shared<das_hash_map<uint64_t,uint32_t>>();
    context.tabGMnLookup->clear();
    for ( int i=0, is=context.totalVariables; i!=is; ++i ) {
        auto mnh = context.globalVariables[i].mangledNameHash;
        (*context.tabGMnLookup)[mnh] = context.globalVariables[i].offset;
    }
    context.tabAdLookup = make_shared<das_hash_map<uint64_t,uint64_t>>();
    FillFunction(context, getGlobalAotLibrary(), move(id_to_funcs));
    context.runInitScript();
}
#ifdef STANDALONE_CONTEXT_TESTS
static Context * registerStandaloneTest ( ) {
    auto ctx = new StandaloneContext();
    return ctx;
}
StandaloneContextNode node(registerStandaloneTest);
#endif
} // namespace main
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
