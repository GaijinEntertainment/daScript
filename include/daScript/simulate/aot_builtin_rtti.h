#pragma once

#include "daScript/simulate/bind_enum.h"
#include "daScript/simulate/aot.h"
#include "daScript/misc/arraytype.h"

DAS_BIND_ENUM_CAST(RefMatters);
DAS_BIND_ENUM_CAST(ConstMatters);
DAS_BIND_ENUM_CAST(TemporaryMatters);

DAS_BIND_ENUM_CAST(Type);

namespace das {
    class ModuleGroup;
    struct AnnotationArgumentList;
}

MAKE_EXTERNAL_TYPE_FACTORY(ModuleGroup, das::ModuleGroup)

namespace das {

    class Program;
    typedef smart_ptr<Program> ProgramPtr;

    class Module;
    struct Annotation;

    using RttiValue = TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>;
    static_assert(sizeof(RttiValue)==32,"sizeof RttiValue must be 32");

    template <typename TT, typename PD>
    struct das_rtti_iterator {
        using TTA = typename std::conditional_t<std::is_const<PD>::value, const TT, TT>;
        __forceinline das_rtti_iterator(const PD & r) {
            array_start = r.fields;
            array_end = array_start + r.count;
        }
        __forceinline bool first ( Context *, TTA * & i ) {
            at = array_start;
            if ( at != array_end ) {
                i = *at;
                return true;
            } else {
                return false;
            }
        }
        __forceinline bool next  ( Context *, TTA * & i ) {
            at++;
            if ( at != array_end ) {
                i = *at;
                return true;
            } else {
                return false;
            }
        }
        __forceinline void close ( Context *, TTA * & i ) {
            i = nullptr;
        }
        TT ** at;
        TT ** array_start;
        TT ** array_end;
    };

    template<typename T, typename... Ts>
    constexpr bool is_one_of = (std::is_same_v<T, Ts> || ...);

    template <typename T>
    struct das_iterator<T, enable_if_t<is_one_of<T, EnumInfo, const EnumInfo>>> :
        das_rtti_iterator<EnumValueInfo, T> {
        das_iterator(T & info) : das_rtti_iterator<EnumValueInfo, T>(info) {}
    };

    template <class T>
    struct das_iterator<T, enable_if_t<is_one_of<T, FuncInfo, const FuncInfo>>> :
        das_rtti_iterator<VarInfo, T> {
        das_iterator(T & info) : das_rtti_iterator<VarInfo, T>(info) {}
    };

    template <typename T>
    struct das_iterator<T, enable_if_t<is_one_of<T, StructInfo, const StructInfo>>> :
        das_rtti_iterator<VarInfo, T> {
        das_iterator(T & info) : das_rtti_iterator<VarInfo, T>(info) {}
    };

    char * rtti_get_das_type_name(Type tt, Context * context, LineInfoArg * at);
    int rtti_add_annotation_argument(AnnotationArgumentList& list, const char* name);

    vec4f rtti_contextFunctionInfo ( Context & context, SimNode_CallBase *, vec4f * );
    vec4f rtti_contextVariableInfo ( Context & context, SimNode_CallBase *, vec4f * );
    int32_t rtti_contextTotalFunctions(Context & context);
    int32_t rtti_contextTotalVariables(Context & context);

    __forceinline Context  & thisContext ( Context * context ) { return *context; }

    RttiValue rtti_builtin_variable_value(const VarInfo & info);

    struct AnnotationArgument;
    RttiValue rtti_builtin_argument_value(const AnnotationArgument & info, Context * context, LineInfoArg * at);

    int32_t rtti_getDimTypeInfo(const TypeInfo & ti, int32_t index, Context * context, LineInfoArg * at);
    int32_t rtti_getDimVarInfo(const VarInfo & ti, int32_t index, Context * context, LineInfoArg * at);

    char * builtin_das_root ( Context * context, LineInfoArg * at );
    smart_ptr<FileAccess> makeFileAccess( char * pak, Context * context, LineInfoArg * at );
    bool introduceFile ( smart_ptr_raw<FileAccess> access, char * fname, char * str, Context * context, LineInfoArg * );
    bool rtti_add_file_access_root ( smart_ptr<FileAccess> access, const char * mod, const char * path );

    struct CodeOfPolicies;
    void rtti_builtin_compile(char * modName, char * str, const CodeOfPolicies & cop,
        const TBlock<void, bool, smart_ptr<Program>, const string> & block, Context * context, LineInfoArg * lineinfo);
    void rtti_builtin_compile_ex ( char * modName, char * str, const CodeOfPolicies & cop, bool exportAll,
            const TBlock<void,bool,smart_ptr<Program>,const string> & block, Context * context, LineInfoArg * at );
    void rtti_builtin_compile_file(char * modName, smart_ptr<FileAccess> access, ModuleGroup* module_group, const CodeOfPolicies & cop,
        const TBlock<void, bool, smart_ptr<Program>, const string> & block, Context * context, LineInfoArg * lineinfo);

    void rtti_builtin_simulate ( const smart_ptr<Program> & program,
        const TBlock<void,bool,smart_ptr_raw<Context>,string> & block, Context * context, LineInfoArg * lineinfo );

    void rtti_builtin_program_for_each_module(smart_ptr_raw<Program> prog, const TBlock<void, Module *> & block, Context * context, LineInfoArg * lineinfo);
    void rtti_builtin_program_for_each_registered_module(const TBlock<void, Module *> & block, Context * context, LineInfoArg * lineinfo);

    void rtti_builtin_module_for_each_dependency ( Module * module, const TBlock<void,Module *,bool> & block, Context * context, LineInfoArg * at );

    Module * rtti_get_this_module(smart_ptr_raw<Program> prog);
    Module * rtti_get_builtin_module(const char * name);

    void rtti_builtin_module_for_each_enumeration(Module * module, const TBlock<void, const EnumInfo> & block, Context * context, LineInfoArg * lineinfo);
    void rtti_builtin_module_for_each_structure(Module * module, const TBlock<void, const StructInfo> & block, Context * context, LineInfoArg * lineinfo);
    void rtti_builtin_module_for_each_function(Module * module, const TBlock<void, const FuncInfo> & block, Context * context, LineInfoArg * lineinfo);
    void rtti_builtin_module_for_each_generic(Module * module, const TBlock<void, const FuncInfo> & block, Context * context, LineInfoArg * lineinfo);
    void rtti_builtin_module_for_each_global(Module * module, const TBlock<void, const VarInfo> & block, Context * context, LineInfoArg * lineinfo);
    void rtti_builtin_module_for_each_annotation(Module * module, const TBlock<void, const Annotation> & block, Context * context, LineInfoArg * lineinfo);

    // note: this one is not TBlock, so that we don't have to include ast.h
    void rtti_builtin_structure_for_each_annotation(const StructInfo & info, const Block & block, Context * context, LineInfoArg * lineinfo);

    // if we are in the module, compiling macros
    bool is_compiling_macros_in_module ( char * name );

    struct BasicStructureAnnotation;
    void rtti_builtin_basic_struct_for_each_field(const BasicStructureAnnotation & ann,
        const TBlock<void, char *, char*, const TypeInfo, uint32_t> & block, Context * context, LineInfoArg * lineinfo);
    void rtti_builtin_basic_struct_for_each_parent ( const BasicStructureAnnotation & ann,
        const TBlock<void,Annotation *> & block, Context * context, LineInfoArg * at );

    LineInfo getCurrentLineInfo( LineInfoArg * lineInfo );
    void builtin_expected_errors ( ProgramPtr prog, const TBlock<void,CompilationError,int> & block, Context * context, LineInfoArg * lineInfo );
    void builtin_list_require ( ProgramPtr prog, const TBlock<void,Module *,TTemporary<const char *>,TTemporary<const char *>,bool,const LineInfo &> & block,
        Context * context, LineInfoArg * lineInfo );

    Func builtin_getFunctionByMnh ( uint64_t MNH, Context * context );
    Func builtin_getFunctionByMnh_inContext ( uint64_t MNH, Context & context );
    uint64_t builtin_getFunctionMnh ( Func func, Context * context );
    uint64_t das_get_SimFunction_by_MNH ( uint64_t MNH, Context & context );
    int32_t rtti_getTablePtr ( void * _table, vec4f key, Type baseType, int valueTypeSize, Context * context, LineInfoArg * at );

    void lockThisContext ( const TBlock<void> & block, Context * context, LineInfoArg * lineInfo );
    void lockAnyContext ( Context & ctx, const TBlock<void> & block, Context * context, LineInfoArg * lineInfo );
    void lockAnyMutex ( recursive_mutex & rm, const TBlock<void> & block, Context * context, LineInfoArg * lineInfo );

    TSequence<VarInfo&> each_FuncInfo ( FuncInfo & st, Context * context, LineInfoArg * at );
    TSequence<const VarInfo&> each_const_FuncInfo ( const FuncInfo & st, Context * context, LineInfoArg * at );
    TSequence<VarInfo&> each_StructInfo ( StructInfo & st, Context * context, LineInfoArg * at );
    TSequence<const VarInfo&> each_const_StructInfo ( const StructInfo & st, Context * context, LineInfoArg * at );
    TSequence<EnumValueInfo&> each_EnumInfo ( EnumInfo & st, Context * context, LineInfoArg * at );
    TSequence<const EnumValueInfo&> each_const_EnumInfo ( const EnumInfo & st, Context * context, LineInfoArg * at );
}

