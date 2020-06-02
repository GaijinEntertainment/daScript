#pragma once

#include "daScript/simulate/bind_enum.h"
#include "daScript/misc/arraytype.h"

DAS_BIND_ENUM_CAST(RefMatters);
DAS_BIND_ENUM_CAST(ConstMatters);
DAS_BIND_ENUM_CAST(TemporaryMatters);

DAS_BIND_ENUM_CAST(Type);

namespace das {

    class Program;
    typedef smart_ptr<Program> ProgramPtr;

    class Module;
    struct Annotation;

    struct RttiValue {
        int32_t         _variant;
        union {
            bool        bValue;         // 0
            int32_t     iValue;         // 1
            uint32_t    uValue;         // 2
            int64_t     i64Value;       // 3
            uint64_t    u64Value;       // 4
            float       fValue;         // 5
            double      dfValue;        // 6
            char *      sValue;         // 7
            vec4f       nothing;        // 8
        };
    };

    template <> struct das_alias<RttiValue>
        : das_alias_ref<RttiValue,TVariant<sizeof(RttiValue),bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>> {};

    template <typename TT, typename PD>
    struct das_rtti_iterator {
        __forceinline das_rtti_iterator(const PD & r) {
            array_start = r.fields;
            array_end = array_start + r.count;
        }
        __forceinline bool first ( Context *, const TT * & i ) {
            at = array_start;
            if ( at != array_end ) {
                i = *at;
                return true;
            } else {
                return false;
            }
        }
        __forceinline bool next  ( Context *, const TT * & i ) {
            at++;
            if ( at != array_end ) {
                i = *at;
                return true;
            } else {
                return false;
            }
        }
        __forceinline void close ( Context *, const TT * & i ) {
            i = nullptr;
        }
        TT ** at;
        TT ** array_start;
        TT ** array_end;
    };

    template <>
    struct das_iterator<EnumInfo const> :
        das_rtti_iterator<EnumValueInfo, EnumInfo> {
        das_iterator(EnumInfo const & info) : das_rtti_iterator<EnumValueInfo, EnumInfo>(info) {}
    };

    template <>
    struct das_iterator<FuncInfo const> :
        das_rtti_iterator<VarInfo, FuncInfo> {
        das_iterator(FuncInfo const & info) : das_rtti_iterator<VarInfo, FuncInfo>(info) {}
    };

    template <>
    struct das_iterator<StructInfo const> :
        das_rtti_iterator<VarInfo, StructInfo> {
        das_iterator(StructInfo const & info) : das_rtti_iterator<VarInfo, StructInfo>(info) {}
    };

    char * rtti_get_das_type_name(Type tt, Context * context);

    vec4f rtti_contextFunctionInfo ( Context & context, SimNode_CallBase *, vec4f * );
    vec4f rtti_contextVariableInfo ( Context & context, SimNode_CallBase *, vec4f * );
    int32_t rtti_contextTotalFunctions(Context * context);
    int32_t rtti_contextTotalVariables(Context * context);

    RttiValue rtti_builtin_variable_value(const VarInfo & info);

    struct AnnotationArgument;
    RttiValue rtti_builtin_argument_value(const AnnotationArgument & info, Context * context);

    int32_t rtti_getDimTypeInfo(const TypeInfo & ti, int32_t index, Context * context);
    int32_t rtti_getDimVarInfo(const VarInfo & ti, int32_t index, Context * context);

    smart_ptr<FileAccess> makeFileAccess( char * pak );
    bool introduceFile ( smart_ptr_raw<FileAccess> access, char * fname, char * str, Context * context );
    void rtti_builtin_compile(char * modName, char * str, const TBlock<void, bool, smart_ptr<Program>, const string> & block, Context * context);
    void rtti_builtin_compile_file(char * modName, smart_ptr<FileAccess> access, const TBlock<void, bool, smart_ptr<Program>, const string> & block, Context * context);

    void rtti_builtin_program_for_each_module(smart_ptr_raw<Program> prog, const TBlock<void, const Module *> & block, Context * context);
    void rtti_builtin_program_for_each_registered_module(const TBlock<void, const Module *> & block, Context * context);

    const Module * rtti_get_this_module(smart_ptr_raw<Program> prog);
    const Module * rtti_get_builtin_module(const char * name);

    void rtti_builtin_module_for_each_enumeration(const Module * module, const TBlock<void, const EnumInfo> & block, Context * context);
    void rtti_builtin_module_for_each_structure(const Module * module, const TBlock<void, const StructInfo> & block, Context * context);
    void rtti_builtin_module_for_each_function(const Module * module, const TBlock<void, const FuncInfo> & block, Context * context);
    void rtti_builtin_module_for_each_generic(const Module * module, const TBlock<void, const FuncInfo> & block, Context * context);
    void rtti_builtin_module_for_each_global(const Module * module, const TBlock<void, const VarInfo> & block, Context * context);
    void rtti_builtin_module_for_each_annotation(const Module * module, const TBlock<void, const Annotation> & block, Context * context);

    // note: this one is not TBlock, so that we don't have to include ast.h
    void rtti_builtin_structure_for_each_annotation(const StructInfo & info, const Block & block, Context * context);

    struct BasicStructureAnnotation;
    void rtti_builtin_basic_struct_for_each_field(const BasicStructureAnnotation & ann,
        const TBlock<void, char *, char*, const TypeInfo, uint32_t> & block, Context * context);
}

