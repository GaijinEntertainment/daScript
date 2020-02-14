#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate_nodes.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/simulate/bind_enum.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/fs_file_info.h"

#include "daScript/simulate/aot_builtin_rtti.h"

#include "daScript/simulate/simulate_visit_op.h"

using namespace das;
DAS_BASE_BIND_ENUM_98(Type, Type,
    none,           autoinfer,      alias,          fakeContext,
    anyArgument,    tVoid,          tBool,          tInt64,
    tUInt64,        tInt,           tInt2,          tInt3,
    tInt4,          tUInt,          tUInt2,         tUInt3,
    tUInt4,         tFloat,         tFloat2,        tFloat3,
    tFloat4,        tDouble,        tRange,         tURange,
    tString,        tStructure,     tHandle,        tEnumeration,
    tPointer,       tFunction,      tLambda,        tIterator,
    tArray,         tTable,         tBlock,         tInt8,
    tUInt8,         tInt16,         tUInt16,        tTuple,
    tEnumeration8,  tEnumeration16, tEnumeration64
)
namespace das {
    struct RttiProgram {
        ProgramPtr      program;
    };
}

MAKE_TYPE_FACTORY(Annotation,Annotation)
MAKE_TYPE_FACTORY(TypeAnnotation,TypeAnnotation)
MAKE_TYPE_FACTORY(StructInfo,StructInfo)
MAKE_TYPE_FACTORY(EnumInfo,EnumInfo)
MAKE_TYPE_FACTORY(EnumValueInfo,EnumValueInfo)
MAKE_TYPE_FACTORY(TypeInfo,TypeInfo)
MAKE_TYPE_FACTORY(VarInfo,VarInfo)
MAKE_TYPE_FACTORY(FuncInfo,FuncInfo)
MAKE_TYPE_FACTORY(AnnotationArgument,AnnotationArgument)
MAKE_TYPE_FACTORY(AnnotationArguments,AnnotationArguments)
MAKE_TYPE_FACTORY(RttiProgram,RttiProgram)
MAKE_TYPE_FACTORY(RttiModule,Module)

DAS_BASE_BIND_ENUM(RefMatters,   RefMatters,   no, yes)
DAS_BASE_BIND_ENUM(ConstMatters, ConstMatters, no, yes)
DAS_BASE_BIND_ENUM(TemporaryMatters, TemporaryMatters, no, yes)

namespace das {

    struct ModuleAnnotation : ManagedStructureAnnotation<Module,false> {
        ModuleAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("RttiModule", ml) {
        }
    };

    struct RttiProgramAnnotation : ManagedStructureAnnotation <RttiProgram,false> {
        RttiProgramAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("RttiProgram", ml) {
        }
    };

    struct AnnotationArgumentAnnotation : ManagedStructureAnnotation <AnnotationArgument,false> {
        AnnotationArgumentAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("AnnotationArgument", ml) {
            addFieldEx ( "basicType", "type", offsetof(AnnotationArgument, type), makeType<Type_DasProxy>(ml) );
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(sValue)>("sValue");
            addField<DAS_BIND_MANAGED_FIELD(bValue)>("bValue");
            addField<DAS_BIND_MANAGED_FIELD(iValue)>("iValue");
            addField<DAS_BIND_MANAGED_FIELD(fValue)>("fValue");
        }
    };

    struct TypeAnnotationAnnotation : ManagedStructureAnnotation <TypeAnnotation,false> {
        TypeAnnotationAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("TypeAnnotation", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(cppName)>("cppName");
        }
    };

    struct AnnotationAnnotation : ManagedStructureAnnotation <Annotation,false> {
        AnnotationAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("Annotation", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
        }
    };

    struct EnumValueInfoAnnotation : ManagedStructureAnnotation <EnumValueInfo,false> {
        EnumValueInfoAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("EnumValueInfo", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(value)>("value");
        }
    };

    template <typename ST>
    struct SimNode_DebugInfoAtField : SimNode_At {
        using TT = ST;
        DAS_PTR_NODE;
        SimNode_DebugInfoAtField ( const LineInfo & at, SimNode * rv, SimNode * idx, uint32_t ofs )
            : SimNode_At(at, rv, idx, 0, ofs, 0) {}
        __forceinline char * compute ( Context & context ) {
            DAS_PROFILE_NODE
            auto pValue = (ST *) value->evalPtr(context);
            uint32_t idx = cast<uint32_t>::to(index->eval(context));
            if ( idx >= pValue->count ) {
                context.throw_error_at(debugInfo,"field index out of range, %u of %u", idx, pValue->count);
                return nullptr;
            } else {
                return ((char *)(pValue->fields[idx])) + offset;
            }
        }
        virtual SimNode * visit ( SimVisitor & vis ) override {
            V_BEGIN();
            V_OP_TT(DebugInfoAtField);
            V_SUB(value);
            V_SUB(index);
            V_ARG(stride);
            V_ARG(offset);
            V_ARG(range);
            V_END();
        }
    };

    template <typename VT, typename ST>
    struct DebugInfoIterator : Iterator {
        DebugInfoIterator  ( ST * ar ) : info(ar) {}
        virtual bool first ( Context &, char * _value ) override {
            VT ** value = (VT **) _value;
            data = info->fields;
            data_end = info->fields + info->count;
            if ( data != data_end ) {
                *value = *data;
                return true;
            } else {
                return false;
            }
        }
        virtual bool next  ( Context &, char * _value ) override {
            VT ** value = (VT **) _value;
            if ( ++data != data_end ) {
                *value = *data;
                return true;
            } else {
                return false;
            }
        }
        virtual void close ( Context & context, char * _value ) override {
            VT ** value = (VT **) _value;
            *value = nullptr;
            context.heap.free((char *)this, sizeof(DebugInfoIterator<VT,ST>));
        }
        VT ** data;
        VT ** data_end;
        ST * info;
    };

    template <typename VT, typename ST>
    struct DebugInfoAnnotation : ManagedStructureAnnotation <ST,false> {
        DebugInfoAnnotation(const string & st, ModuleLibrary & ml)
            : ManagedStructureAnnotation<ST,false> (st,ml) {
            using ManagedType = ST;
            this->template addField<DAS_BIND_MANAGED_FIELD(count)>("count");
        }
        virtual bool isIndexable ( const TypeDeclPtr & indexType ) const override {
            return indexType->isIndex();
        }
        virtual TypeDeclPtr makeIndexType ( const ExpressionPtr &, const ExpressionPtr & ) const override {
            return make_shared<TypeDecl>(*fieldType);
        }
        virtual SimNode * simulateGetAt ( Context & context, const LineInfo & at, const TypeDeclPtr &,
                                         const ExpressionPtr & rv, const ExpressionPtr & idx, uint32_t ofs ) const override {
            return context.code->makeNode<SimNode_DebugInfoAtField<ST>>(at,
                                                                                rv->simulate(context),
                                                                                idx->simulate(context),
                                                                                ofs);
        }
        virtual bool isIterable ( ) const override {
            return true;
        }
        virtual TypeDeclPtr makeIteratorType ( const ExpressionPtr & ) const override {
            return make_shared<TypeDecl>(*fieldType);
        }
        virtual SimNode * simulateGetIterator ( Context & context, const LineInfo & at, const ExpressionPtr & src ) const override {
            auto rv = src->simulate(context);
            return context.code->makeNode<SimNode_AnyIterator<ST,DebugInfoIterator<VT,ST>>>(at, rv);
        }
        TypeDeclPtr fieldType;
    };

    struct EnumInfoAnnotation : DebugInfoAnnotation<EnumValueInfo,EnumInfo> {
        EnumInfoAnnotation(ModuleLibrary & ml) : DebugInfoAnnotation ("EnumInfo", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(hash)>("hash");
            fieldType = makeType<EnumValueInfo>(*mlib);
            fieldType->ref = true;
        }
    };

    struct StructInfoAnnotation : DebugInfoAnnotation<VarInfo,StructInfo> {
        StructInfoAnnotation(ModuleLibrary & ml) : DebugInfoAnnotation ("StructInfo", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(size)>("size");
            addField<DAS_BIND_MANAGED_FIELD(initializer)>("initializer");
            addField<DAS_BIND_MANAGED_FIELD(hash)>("hash");
        }
        void init () {
            fieldType = makeType<VarInfo>(*mlib);
            fieldType->ref = true;
        }
    };

    template <typename TT>
    struct ManagedTypeInfoAnnotation : ManagedStructureAnnotation <TT,false> {
        ManagedTypeInfoAnnotation ( const string & st, ModuleLibrary & ml )
            : ManagedStructureAnnotation<TT,false> (st, ml) {
            using ManagedType = TT;
            this->addFieldEx ( "basicType", "type", offsetof(TypeInfo, type), makeType<Type_DasProxy>(ml) );
            this->template addField<DAS_BIND_MANAGED_FIELD(annotation_or_name)>("annotation_or_name");
            this->template addField<DAS_BIND_MANAGED_FIELD(enumType)>("enumType");
            this->template addField<DAS_BIND_MANAGED_FIELD(dimSize)>("dimSize");
            this->template addField<DAS_BIND_MANAGED_FIELD(hash)>("hash");
            this->template addProperty<DAS_BIND_MANAGED_PROP(isRef)>("isRef");
            this->template addProperty<DAS_BIND_MANAGED_PROP(isRefType)>("isRefType");
            this->template addProperty<DAS_BIND_MANAGED_PROP(canCopy)>("canCopy");
            this->template addProperty<DAS_BIND_MANAGED_PROP(isPod)>("isPod");
            this->template addProperty<DAS_BIND_MANAGED_PROP(isRawPod)>("isRawPod");
            this->template addProperty<DAS_BIND_MANAGED_PROP(isConst)>("isConst");
        }
        void init() {
            // this needs to be initialized separately
            // reason is recursive type
            using ManagedType = TT;
            this->template addField<DAS_BIND_MANAGED_FIELD(structType)>("structType");
            this->template addField<DAS_BIND_MANAGED_FIELD(firstType)>("firstType");
            this->template addField<DAS_BIND_MANAGED_FIELD(secondType)>("secondType");
        }
    };

    struct TypeInfoAnnotation : ManagedTypeInfoAnnotation <TypeInfo> {
        TypeInfoAnnotation(ModuleLibrary & ml) : ManagedTypeInfoAnnotation ("TypeInfo", ml) {
        }
    };

    struct VarInfoAnnotation : ManagedTypeInfoAnnotation <VarInfo> {
        VarInfoAnnotation(ModuleLibrary & ml) : ManagedTypeInfoAnnotation ("VarInfo", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(offset)>("offset");
            addFieldEx ( "annotation_arguments", "annotation_arguments",
                        offsetof(VarInfo, annotation_arguments), makeType<const AnnotationArguments *>(ml) );
            // default values
            addField<DAS_BIND_MANAGED_FIELD(sValue)>("sValue");
            addField<DAS_BIND_MANAGED_FIELD(bValue)>("bValue");
            addField<DAS_BIND_MANAGED_FIELD(iValue)>("iValue");
            addField<DAS_BIND_MANAGED_FIELD(fValue)>("fValue");
        }
    };

    struct FuncInfoAnnotation : DebugInfoAnnotation<VarInfo,FuncInfo> {
        FuncInfoAnnotation(ModuleLibrary & ml) : DebugInfoAnnotation ("FuncInfo", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(cppName)>("cppName");
            addField<DAS_BIND_MANAGED_FIELD(stackSize)>("stackSize");
            addField<DAS_BIND_MANAGED_FIELD(result)>("result");
            addField<DAS_BIND_MANAGED_FIELD(hash)>("hash");
            addField<DAS_BIND_MANAGED_FIELD(flags)>("flags");
            fieldType = makeType<VarInfo>(*mlib);
            fieldType->ref = true;
        }
    };

    vec4f rtti_getTypeInfo ( Context & , SimNode_CallBase * call, vec4f * ) {
        DAS_ASSERTF(call->types[0], "missing type info somehow");
        return cast<TypeInfo *>::from(call->types[0]);
    }

    template <typename TT>
    int32_t rtti_getDim ( const TT & ti, int32_t _index, Context * context ) {
        uint32_t index = _index;
        if ( ti.dimSize==0 ) {
            context->throw_error("type is not an array");
        }
        if ( index>=ti.dimSize ) {
            context->throw_error_ex("dim index out of range, %u of %u", index, uint32_t(ti.dimSize));
        }
        return ti.dim[index];
    }

    int32_t rtti_getDimTypeInfo ( const TypeInfo & ti, int32_t index, Context * context ) {
        return rtti_getDim(ti, index, context);
    }

    int32_t rtti_getDimVarInfo ( const VarInfo & ti, int32_t index, Context * context ) {
        return rtti_getDim(ti, index, context);
    }

    int32_t rtti_contextTotalFunctions ( Context * context ) {
        return context->getTotalFunctions();
    }

    int32_t rtti_contextTotalVariables ( Context * context ) {
        return context->getTotalVariables();
    }

    vec4f rtti_contextFunctionInfo ( Context & context, SimNode_CallBase *, vec4f * args ) {
        int32_t tf = context.getTotalFunctions();
        int32_t index = cast<int32_t>::to(args[0]);
        if ( index<0 || index>=tf ) {
            context.throw_error_ex("function index out of range, %i of %i", index, tf);
        }
        FuncInfo * fi = context.getFunction(index)->debugInfo;
        return cast<FuncInfo *>::from(fi);
    }

    vec4f rtti_contextVariableInfo ( Context & context, SimNode_CallBase *, vec4f * args ) {
        int32_t tf = context.getTotalVariables();
        int32_t index = cast<int32_t>::to(args[0]);
        if ( index<0 || index>=tf ) {
            context.throw_error_ex("variable index out of range, %i of %i", index, tf);
        }
        return cast<VarInfo *>::from(context.getVariableInfo(index));
    }

    void rtti_builtin_compile ( char * modName, char * str, const Block & block, Context * context ) {
        TextWriter issues;
        uint32_t str_len = stringLengthSafe(*context, str);
        auto access = make_shared<FileAccess>();
        auto fileInfo = make_unique<FileInfo>((char *) str, uint32_t(str_len));
        access->setFileInfo(modName, move(fileInfo));
        ModuleGroup dummyLibGroup;
        auto program = parseDaScript(modName, access, issues, dummyLibGroup, true);
        if ( program ) {
            if (program->failed()) {
                for (auto & err : program->errors) {
                    issues << reportError(err.at, err.what, err.cerr);
                }
                string istr = issues.str();
                vec4f args[3] = {
                    cast<bool>::from(false),
                    cast<char *>::from(nullptr),
                    cast<string *>::from(&istr)
                };
                context->invoke(block, args, nullptr);
            } else {
                RttiProgram rtp;
                rtp.program = program;
                string istr = issues.str();
                vec4f args[3] = {
                    cast<bool>::from(true),
                    cast<RttiProgram *>::from(&rtp),
                    cast<string *>::from(&istr)
                };
                context->invoke(block, args, nullptr);
            }
        } else {
            context->throw_error("rtti_compile internal error, something went wrong");
        }
    }

    const Module * rtti_get_this_module ( const RttiProgram & prog ) {
        return prog.program->thisModule.get();
    }

    const Module * rtti_get_builtin_module ( const char * name ) {
        return Module::require(name);
    }

    void rtti_builtin_program_for_each_module ( const RttiProgram & prog, const Block & block, Context * context ) {
        prog.program->library.foreach([&](Module * pm) -> bool {
            vec4f args[1] = { cast<Module *>::from(pm) };
            context->invoke(block, args, nullptr);
            return true;
        }, "*");
    }

    void rtti_builtin_module_for_each_enumeration ( const Module * module, const Block & block, Context * context ) {
        DebugInfoHelper helper;
        helper.rtti = true;
        for ( auto & it : module->enumerations ) {
            auto structName = it.first;
            EnumInfo * info = helper.makeEnumDebugInfo(*it.second);
            vec4f args[1] = {
                cast<EnumInfo *>::from(info)
            };
            context->invoke(block, args, nullptr);
        }
    }

    void rtti_builtin_module_for_each_structure ( const Module * module, const Block & block, Context * context ) {
        DebugInfoHelper helper;
        helper.rtti = true;
        for ( auto & it : module->structures ) {
            auto structName = it.first;
            StructInfo * info = helper.makeStructureDebugInfo(*it.second);
            vec4f args[1] = {
                cast<StructInfo *>::from(info)
            };
            context->invoke(block, args, nullptr);
        }
    }

    void rtti_builtin_structure_for_each_annotation ( const StructInfo & info, const Block & block, Context * context ) {
        if ( info.annotation_list ) {
            auto al = (const AnnotationList *) info.annotation_list;
            for ( const auto & adp : *al ) {
                vec4f args[2] = {
                    cast<Annotation *>::from(adp->annotation.get()),
                    cast<AnnotationArgumentList *>::from(&adp->arguments)
                };
                context->invoke(block, args, nullptr);
            }
        }
    }

    void rtti_builtin_module_for_each_function ( const Module * module, const Block & block, Context * context ) {
        DebugInfoHelper helper;
        helper.rtti = true;
        for ( auto & it : module->functions ) {
            auto funcName = it.first;
            FuncInfo * info = helper.makeFunctionDebugInfo(*it.second);
            vec4f args[1] = {
                cast<FuncInfo *>::from(info)
            };
            context->invoke(block, args, nullptr);
        }
    }

    void rtti_builtin_module_for_each_generic ( const Module * module, const Block & block, Context * context ) {
        DebugInfoHelper helper;
        helper.rtti = true;
        for ( auto & it : module->generics ) {
            auto funcName = it.first;
            FuncInfo * info = helper.makeFunctionDebugInfo(*it.second);
            vec4f args[1] = {
                cast<FuncInfo *>::from(info)
            };
            context->invoke(block, args, nullptr);
        }
    }

#if !DAS_NO_FILEIO

    void rtti_builtin_compile_file ( char * modName, const Block & block, Context * context ) {
        TextWriter issues;
        auto access = make_shared<FsFileAccess>();
        ModuleGroup dummyLibGroup;
        auto program = compileDaScript(modName, access, issues, dummyLibGroup, true);
        if ( program ) {
            if (program->failed()) {
                for (auto & err : program->errors) {
                    issues << reportError(err.at, err.what, err.cerr);
                }
                string istr = issues.str();
                vec4f args[3] = {
                    cast<bool>::from(false),
                    cast<char *>::from(nullptr),
                    cast<string *>::from(&istr)
                };
                context->invoke(block, args, nullptr);
            } else {
                RttiProgram rtp;
                rtp.program = program;
                string istr = issues.str();
                vec4f args[3] = {
                    cast<bool>::from(true),
                    cast<RttiProgram *>::from(&rtp),
                    cast<string *>::from(&istr)
                };
                context->invoke(block, args, nullptr);
            }
        } else {
            context->throw_error("rtti_compile internal error, something went wrong");
        }
    }

#else
    void rtti_builtin_compile_file ( char *, const Block &, Context * context ) {
        context->throw_error("not supported with DAS_NO_FILEIO");
    }
#endif

    #include "rtti.das.inc"

    class Module_Rtti : public Module {
    public:
        template <typename RecAnn>
        void initRecAnnotation ( const shared_ptr<RecAnn> & rec, ModuleLibrary & lib ) {
            rec->mlib = &lib;
            rec->init();
            rec->mlib = nullptr;
        }
        template <typename RecAnn>
        void addRecAnnotation ( ModuleLibrary & lib ) {
            auto rec = make_shared<RecAnn>(lib);
            addAnnotation(rec);
            initRecAnnotation(rec, lib);
        }
        Module_Rtti() : Module("rtti") {
            ModuleLibrary lib;
            lib.addModule(this);
            lib.addBuiltInModule();
            // type annotations
            addAnnotation(make_shared<ModuleAnnotation>(lib));
            addAnnotation(make_shared<RttiProgramAnnotation>(lib));
            addEnumeration(make_shared<EnumerationType>());
            addAnnotation(make_shared<AnnotationArgumentAnnotation>(lib));
            addAnnotation(make_shared<ManagedVectorAnnotation<AnnotationArgument>>("AnnotationArguments",lib));
            addAnnotation(make_shared<AnnotationAnnotation>(lib));
            addAnnotation(make_shared<TypeAnnotationAnnotation>(lib));
            addAnnotation(make_shared<EnumValueInfoAnnotation>(lib));
            addAnnotation(make_shared<EnumInfoAnnotation>(lib));
            addEnumeration(make_shared<EnumerationRefMatters>());
            addEnumeration(make_shared<EnumerationConstMatters>());
            addEnumeration(make_shared<EnumerationTemporaryMatters>());
            auto sia = make_shared<StructInfoAnnotation>(lib);              // this is type forward decl
            addAnnotation(sia);
            addRecAnnotation<TypeInfoAnnotation>(lib);
            addRecAnnotation<VarInfoAnnotation>(lib);
            initRecAnnotation(sia, lib);
            addAnnotation(make_shared<FuncInfoAnnotation>(lib));
            // func info flags
            addConstant<uint32_t>(*this, "FUNCINFO_INIT", uint32_t(FuncInfo::flag_init));
            addConstant<uint32_t>(*this, "FUNCINFO_BUILTIN", uint32_t(FuncInfo::flag_builtin));
            // functions
            //      all the stuff is only resolved after debug info is built
            //      hence SideEffects::modifyExternal is essencial for it to not be optimized out
            addInterop<rtti_getTypeInfo,const TypeInfo &,vec4f>(*this, lib, "get_type_info",
                SideEffects::modifyExternal,"rtti_getTypeInfo");
            addExtern<DAS_BIND_FUN(rtti_getDimTypeInfo)>(*this, lib, "get_dim", SideEffects::modifyExternal);
            addExtern<DAS_BIND_FUN(rtti_getDimVarInfo)>(*this, lib, "get_dim", SideEffects::modifyExternal);
            addExtern<DAS_BIND_FUN(rtti_contextTotalFunctions)>(*this, lib, "get_total_functions", SideEffects::modifyExternal);
            addExtern<DAS_BIND_FUN(rtti_contextTotalVariables)>(*this, lib, "get_total_variables", SideEffects::modifyExternal);
            addInterop<rtti_contextFunctionInfo,const FuncInfo &,int32_t>(*this, lib, "get_function_info",
                SideEffects::modifyExternal, "rtti_contextFunctionInfo");
            addInterop<rtti_contextVariableInfo,const VarInfo &,int32_t>(*this, lib, "get_variable_info",
                SideEffects::modifyExternal, "rtti_contextVariableInfo");
            addExtern<DAS_BIND_FUN(rtti_builtin_compile)>(*this, lib, "rtti_builtin_compile", SideEffects::modifyExternal);
            addExtern<DAS_BIND_FUN(rtti_builtin_compile_file)>(*this, lib, "rtti_builtin_compile_file", SideEffects::modifyExternal);
            addExtern<DAS_BIND_FUN(rtti_get_this_module)>(*this, lib, "get_this_module",
                SideEffects::modifyExternal, "rtti_get_this_module");
            addExtern<DAS_BIND_FUN(rtti_get_builtin_module)>(*this, lib, "get_module",
                SideEffects::modifyExternal, "rtti_get_builtin_module");
            addExtern<DAS_BIND_FUN(rtti_builtin_program_for_each_module)>(*this, lib, "rtti_builtin_program_for_each_module", SideEffects::modifyExternal);
            addExtern<DAS_BIND_FUN(rtti_builtin_module_for_each_structure)>(*this, lib, "rtti_builtin_module_for_each_structure", SideEffects::modifyExternal);
            addExtern<DAS_BIND_FUN(rtti_builtin_module_for_each_enumeration)>(*this, lib, "rtti_builtin_module_for_each_enumeration", SideEffects::modifyExternal);
            addExtern<DAS_BIND_FUN(rtti_builtin_module_for_each_function)>(*this, lib, "rtti_builtin_module_for_each_function", SideEffects::modifyExternal);
            addExtern<DAS_BIND_FUN(rtti_builtin_module_for_each_generic)>(*this, lib, "rtti_builtin_module_for_each_generic", SideEffects::modifyExternal);
            addExtern<DAS_BIND_FUN(rtti_builtin_structure_for_each_annotation)>(*this, lib, "rtti_builtin_structure_for_each_annotation", SideEffects::modifyExternal);
            addExtern<DAS_BIND_FUN(isSameType)>(*this, lib, "builtin_is_same_type", SideEffects::modifyExternal);
            addExtern<DAS_BIND_FUN(isCompatibleCast)>(*this, lib, "builtin_is_compatible_cast", SideEffects::modifyExternal);
            // add builtin module
            compileBuiltinModule("rtti.das",rtti_das, sizeof(rtti_das));
        }
        virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
            tw << "#include \"daScript/simulate/aot_builtin_rtti.h\"\n";
            return ModuleAotType::hybrid;
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_Rtti,das);
