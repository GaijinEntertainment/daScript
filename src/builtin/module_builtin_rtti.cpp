#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate_nodes.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/simulate/bind_enum.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/fs_file_info.h"

#include "daScript/simulate/aot.h"
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
    tEnumeration8,  tEnumeration16, tVariant
)

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
MAKE_TYPE_FACTORY(Module,Module)

DAS_BASE_BIND_ENUM(RefMatters,   RefMatters,   no, yes)
DAS_BASE_BIND_ENUM(ConstMatters, ConstMatters, no, yes)
DAS_BASE_BIND_ENUM(TemporaryMatters, TemporaryMatters, no, yes)

namespace das {

    template <>
    struct typeFactory<RttiValue> {
        static TypeDeclPtr make(const ModuleLibrary & library ) {
            auto vtype = make_shared<TypeDecl>(Type::tVariant);
            vtype->alias = "RttiValue";
            vtype->aotAlias = true;
            vtype->addVariant("tBool",   typeFactory<decltype(RttiValue::bValue  )>::make(library));
            vtype->addVariant("tInt",    typeFactory<decltype(RttiValue::iValue  )>::make(library));
            vtype->addVariant("tUInt",   typeFactory<decltype(RttiValue::uValue  )>::make(library));
            vtype->addVariant("tInt64",  typeFactory<decltype(RttiValue::i64Value)>::make(library));
            vtype->addVariant("tUInt64", typeFactory<decltype(RttiValue::u64Value)>::make(library));
            vtype->addVariant("tFloat",  typeFactory<decltype(RttiValue::fValue  )>::make(library));
            vtype->addVariant("tDouble", typeFactory<decltype(RttiValue::dfValue )>::make(library));
            vtype->addVariant("tString", typeFactory<decltype(RttiValue::sValue  )>::make(library));
            vtype->addVariant("nothing", typeFactory<decltype(RttiValue::nothing )>::make(library));
            // optional validation
            DAS_ASSERT(sizeof(RttiValue) == vtype->getSizeOf());
            DAS_ASSERT(alignof(RttiValue) == vtype->getAlignOf());
            DAS_ASSERT(offsetof(RttiValue, bValue  ) == vtype->getVariantFieldOffset(0));
            DAS_ASSERT(offsetof(RttiValue, iValue  ) == vtype->getVariantFieldOffset(1));
            DAS_ASSERT(offsetof(RttiValue, uValue  ) == vtype->getVariantFieldOffset(2));
            DAS_ASSERT(offsetof(RttiValue, i64Value) == vtype->getVariantFieldOffset(3));
            DAS_ASSERT(offsetof(RttiValue, u64Value) == vtype->getVariantFieldOffset(4));
            DAS_ASSERT(offsetof(RttiValue, fValue  ) == vtype->getVariantFieldOffset(5));
            DAS_ASSERT(offsetof(RttiValue, dfValue ) == vtype->getVariantFieldOffset(6));
            DAS_ASSERT(offsetof(RttiValue, sValue  ) == vtype->getVariantFieldOffset(7));
            DAS_ASSERT(offsetof(RttiValue, nothing ) == vtype->getVariantFieldOffset(8));
            return vtype;
        }
    };

    struct ModuleAnnotation : ManagedStructureAnnotation<Module,false> {
        ModuleAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("Module", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
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
    struct DebugInfoIterator : PointerDimIterator {
        DebugInfoIterator  ( ST * ar ) 
            : PointerDimIterator((char **)ar->fields, ar->count, sizeof(DebugInfoIterator<VT,ST>)) {}
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
            this->template addField<DAS_BIND_MANAGED_FIELD(argCount)>("argCount");
            this->template addField<DAS_BIND_MANAGED_FIELD(hash)>("hash");
            this->template addProperty<DAS_BIND_MANAGED_PROP(isRef)>("isRef");
            this->template addProperty<DAS_BIND_MANAGED_PROP(isRefType)>("isRefType");
            this->template addProperty<DAS_BIND_MANAGED_PROP(isRefValue)>("isRefValue");
            this->template addProperty<DAS_BIND_MANAGED_PROP(canCopy)>("canCopy");
            this->template addProperty<DAS_BIND_MANAGED_PROP(isPod)>("isPod");
            this->template addProperty<DAS_BIND_MANAGED_PROP(isRawPod)>("isRawPod");
            this->template addProperty<DAS_BIND_MANAGED_PROP(isConst)>("isConst");
            this->template addField<DAS_BIND_MANAGED_FIELD(argNames)>("argNames");
        }
        void init() {
            // this needs to be initialized separately
            // reason is recursive type
            using ManagedType = TT;
            this->template addField<DAS_BIND_MANAGED_FIELD(structType)>("structType");
            this->template addField<DAS_BIND_MANAGED_FIELD(firstType)>("firstType");
            this->template addField<DAS_BIND_MANAGED_FIELD(secondType)>("secondType");
            this->template addField<DAS_BIND_MANAGED_FIELD(argTypes)>("argTypes");
        }
    };

    struct TypeInfoAnnotation : ManagedTypeInfoAnnotation <TypeInfo> {
        TypeInfoAnnotation(ModuleLibrary & ml) : ManagedTypeInfoAnnotation ("TypeInfo", ml) {
        }
    };

    void builtin_make_arg_names_iterator ( Sequence & result, const TypeInfo & info, Context * context ) {
        char * iter = context->heap.allocate(sizeof(PointerDimIterator));
        new (iter) PointerDimIterator(info.argNames, info.argCount, sizeof(PointerDimIterator));
        result = { (Iterator *) iter };
    }

    void builtin_make_arg_types_iterator ( Sequence & result, const TypeInfo & info, Context * context ) {
        char * iter = context->heap.allocate(sizeof(PointerDimIterator));
        new (iter) PointerDimIterator((char **)info.argTypes, info.argCount, sizeof(PointerDimIterator));
        result = { (Iterator *) iter };
    }

    void builtin_make_var_arg_names_iterator ( Sequence & result, const VarInfo & info, Context * context ) {
        char * iter = context->heap.allocate(sizeof(PointerDimIterator));
        new (iter) PointerDimIterator(info.argNames, info.argCount, sizeof(PointerDimIterator));
        result = { (Iterator *) iter };
    }

    void builtin_make_var_arg_types_iterator ( Sequence & result, const VarInfo & info, Context * context ) {
        char * iter = context->heap.allocate(sizeof(PointerDimIterator));
        new (iter) PointerDimIterator((char **)info.argTypes, info.argCount, sizeof(PointerDimIterator));
        result = { (Iterator *) iter };
    }

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
            addField<DAS_BIND_MANAGED_FIELD(uValue)>("uValue");
            addField<DAS_BIND_MANAGED_FIELD(i64Value)>("i64Value");
            addField<DAS_BIND_MANAGED_FIELD(u64Value)>("u64Value");
            addField<DAS_BIND_MANAGED_FIELD(fValue)>("fValue");
            addField<DAS_BIND_MANAGED_FIELD(dfValue)>("dfValue");
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

    void rtti_builtin_compile ( char * modName, char * str, const TBlock<void,bool,const RttiProgram,const string> & block, Context * context ) {
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
                    issues << reportError(err.at, err.what, err.extra, err.fixme, err.cerr);
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

    void rtti_builtin_program_for_each_module ( const RttiProgram & prog, const TBlock<void,const Module *> & block, Context * context ) {
        prog.program->library.foreach([&](Module * pm) -> bool {
            vec4f args[1] = { cast<Module *>::from(pm) };
            context->invoke(block, args, nullptr);
            return true;
        }, "*");
    }

    void rtti_builtin_program_for_each_registered_module ( const TBlock<void,const Module *> & block, Context * context ) {
        Module::foreach([&](Module * pm) -> bool {
            vec4f args[1] = { cast<Module *>::from(pm) };
            context->invoke(block, args, nullptr);
            return true;
        });
    }

    void rtti_builtin_module_for_each_enumeration ( const Module * module, const TBlock<void,const EnumInfo> & block, Context * context ) {
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

    RttiValue rtti_builtin_argument_value(const AnnotationArgument & info, Context * context ) {
        RttiValue nada;
        nada._variant = 8;  // nothing
        nada.nothing = v_zero();
        switch (info.type) {
        case Type::tBool:   
            nada._variant = 0; 
            nada.bValue = info.bValue; 
            break;
        case Type::tInt:    
            nada._variant = 1; 
            nada.iValue = info.iValue; 
            break;
        case Type::tFloat:  
            nada._variant = 5; 
            nada.fValue = info.fValue; 
            break;
        case Type::tString: 
            nada._variant = 7; 
            nada.sValue = context->stringHeap.allocateName(info.sValue); 
            break;
        }
        return nada;
    }


    RttiValue rtti_builtin_variable_value(const VarInfo & info) {
        RttiValue nada;
        nada._variant = 8;  // nothing
        nada.nothing = v_zero();
        if (info.dimSize == 0 && (info.flags & TypeInfo::flag_hasInitValue)!=0 ) {
            switch (info.type) {
            case Type::tBool:   nada._variant = 0; break;
            case Type::tInt:    nada._variant = 1; break;
            case Type::tUInt:   nada._variant = 2; break;
            case Type::tInt64:  nada._variant = 3; break;
            case Type::tUInt64: nada._variant = 4; break;
            case Type::tFloat:  nada._variant = 5; break;
            case Type::tDouble: nada._variant = 6; break;
            case Type::tString: nada._variant = 7; break;
            }
            if (nada._variant != 8) {
                if (nada._variant != 7) {
                    nada.nothing = info.value;
                } else {
                    nada.sValue = info.sValue;
                }
            }
        }
        return nada;
    }

    void rtti_builtin_module_for_each_structure ( const Module * module, const TBlock<void,const StructInfo> & block, Context * context ) {
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

    void rtti_builtin_module_for_each_function ( const Module * module, const TBlock<void,const FuncInfo> & block, Context * context ) {
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

    void rtti_builtin_module_for_each_generic ( const Module * module, const TBlock<void,const FuncInfo> & block, Context * context ) {
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

    void rtti_builtin_module_for_each_global ( const Module * module, const TBlock<void,const VarInfo> & block, Context * context ) {
        DebugInfoHelper helper;
        helper.rtti = true;
        for ( auto & it : module->globals ) {
            auto varName = it.first;
            VarInfo * info = helper.makeVariableDebugInfo(*it.second);
            vec4f args[1] = {
                cast<VarInfo *>::from(info)
            };
            context->invoke(block, args, nullptr);
        }
    }

    char * rtti_get_das_type_name(Type tt, Context * context) {
        string str = das_to_string(tt);
        return context->stringHeap.allocateName(str);
    }

#if !DAS_NO_FILEIO

    void rtti_builtin_compile_file ( char * modName, const TBlock<void,bool,const RttiProgram,const string> & block, Context * context ) {
        TextWriter issues;
        auto access = make_shared<FsFileAccess>();
        ModuleGroup dummyLibGroup;
        auto program = compileDaScript(modName, access, issues, dummyLibGroup, true);
        if ( program ) {
            if (program->failed()) {
                for (auto & err : program->errors) {
                    issues << reportError(err.at, err.what, err.extra, err.fixme, err.cerr);
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
            addAnnotation(make_shared<ManagedVectorAnnotation<AnnotationArguments>>("AnnotationArguments",lib));
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
            // RttiValue
            addAlias(typeFactory<RttiValue>::make(lib));
            // func info flags
            addConstant<uint32_t>(*this, "FUNCINFO_INIT", uint32_t(FuncInfo::flag_init));
            addConstant<uint32_t>(*this, "FUNCINFO_BUILTIN", uint32_t(FuncInfo::flag_builtin));
            // functions
            //      all the stuff is only resolved after debug info is built
            //      hence SideEffects::modifyExternal is essencial for it to not be optimized out
            addInterop<rtti_getTypeInfo,const TypeInfo &,vec4f>(*this, lib, "get_type_info",
                SideEffects::modifyExternal,"rtti_getTypeInfo");
            addExtern<DAS_BIND_FUN(rtti_getDimTypeInfo)>(*this, lib, "get_dim", 
                SideEffects::modifyExternal, "rtti_getDimTypeInfo");
            addExtern<DAS_BIND_FUN(rtti_getDimVarInfo)>(*this, lib, "get_dim", 
                SideEffects::modifyExternal, "rtti_getDimVarInfo");
            addExtern<DAS_BIND_FUN(rtti_contextTotalFunctions)>(*this, lib, "get_total_functions", 
                SideEffects::modifyExternal, "rtti_contextTotalFunctions");
            addExtern<DAS_BIND_FUN(rtti_contextTotalVariables)>(*this, lib, "get_total_variables", 
                SideEffects::modifyExternal, "rtti_contextTotalVariables");
            addInterop<rtti_contextFunctionInfo,const FuncInfo &,int32_t>(*this, lib, "get_function_info",
                SideEffects::modifyExternal, "rtti_contextFunctionInfo");
            addInterop<rtti_contextVariableInfo,const VarInfo &,int32_t>(*this, lib, "get_variable_info",
                SideEffects::modifyExternal, "rtti_contextVariableInfo");
            addExtern<DAS_BIND_FUN(rtti_get_this_module)>(*this, lib, "get_this_module",
                SideEffects::modifyExternal, "rtti_get_this_module");
            addExtern<DAS_BIND_FUN(rtti_get_builtin_module)>(*this, lib, "get_module",
                SideEffects::modifyExternal, "rtti_get_builtin_module");
            addExtern<DAS_BIND_FUN(rtti_builtin_compile)>(*this, lib, "compile", 
                SideEffects::modifyExternal, "rtti_builtin_compile");
            addExtern<DAS_BIND_FUN(rtti_builtin_compile_file)>(*this, lib, "compile_file", 
                SideEffects::modifyExternal, "rtti_builtin_compile_file");
            addExtern<DAS_BIND_FUN(rtti_builtin_program_for_each_module)>(*this, lib, "program_for_each_module", 
                SideEffects::modifyExternal, "rtti_builtin_program_for_each_module");
            addExtern<DAS_BIND_FUN(rtti_builtin_program_for_each_registered_module)>(*this, lib, "program_for_each_registered_module", 
                SideEffects::modifyExternal, "rtti_builtin_program_for_each_registered_module");
            addExtern<DAS_BIND_FUN(rtti_builtin_module_for_each_structure)>(*this, lib, "module_for_each_structure", 
                SideEffects::modifyExternal, "rtti_builtin_module_for_each_structure");
            addExtern<DAS_BIND_FUN(rtti_builtin_variable_value),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "get_variable_value", 
                SideEffects::modifyExternal, "rtti_builtin_variable_value");
            addExtern<DAS_BIND_FUN(rtti_builtin_argument_value),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "get_annotation_argument_value", 
                SideEffects::modifyExternal, "rtti_builtin_argument_value");
            addExtern<DAS_BIND_FUN(rtti_builtin_module_for_each_enumeration)>(*this, lib, "module_for_each_enumeration", 
                SideEffects::modifyExternal, "rtti_builtin_module_for_each_enumeration");
            addExtern<DAS_BIND_FUN(rtti_builtin_module_for_each_function)>(*this, lib, "module_for_each_function", 
                SideEffects::modifyExternal, "rtti_builtin_module_for_each_function");
            addExtern<DAS_BIND_FUN(rtti_builtin_module_for_each_generic)>(*this, lib, "module_for_each_generic", 
                SideEffects::modifyExternal, "rtti_builtin_module_for_each_generic");
            addExtern<DAS_BIND_FUN(rtti_builtin_module_for_each_global)>(*this, lib, "module_for_each_global", 
                SideEffects::modifyExternal, "rtti_builtin_module_for_each_global");
            addExtern<DAS_BIND_FUN(rtti_builtin_structure_for_each_annotation)>(*this, lib, "rtti_builtin_structure_for_each_annotation", 
                SideEffects::modifyExternal, "rtti_builtin_structure_for_each_annotation");
            addExtern<DAS_BIND_FUN(isSameType)>(*this, lib, "builtin_is_same_type", 
                SideEffects::modifyExternal, "isSameType");
            addExtern<DAS_BIND_FUN(isCompatibleCast)>(*this, lib, "builtin_is_compatible_cast", 
                SideEffects::modifyExternal, "isCompatibleCast");
            addExtern<DAS_BIND_FUN(builtin_make_arg_names_iterator)>(*this, lib,  "_builtin_make_arg_names_iterator",
                SideEffects::modifyArgumentAndExternal, "builtin_make_arg_names_iterator");
            addExtern<DAS_BIND_FUN(builtin_make_arg_types_iterator)>(*this, lib,  "_builtin_make_arg_types_iterator",
                SideEffects::modifyArgumentAndExternal, "builtin_make_arg_types_iterator");
            addExtern<DAS_BIND_FUN(builtin_make_var_arg_names_iterator)>(*this, lib,  "_builtin_make_arg_names_iterator",
                SideEffects::modifyArgumentAndExternal, "builtin_make_var_arg_names_iterator");
            addExtern<DAS_BIND_FUN(builtin_make_var_arg_types_iterator)>(*this, lib,  "_builtin_make_arg_types_iterator",
                SideEffects::modifyArgumentAndExternal, "builtin_make_var_arg_types_iterator");
            addExtern<DAS_BIND_FUN(rtti_get_das_type_name)>(*this, lib,  "get_das_type_name",
                SideEffects::none, "rtti_get_das_type_name");
            // add builtin module
            compileBuiltinModule("rtti.das",rtti_das, sizeof(rtti_das));
            // lets make sure its all aot ready
            verifyAotReady();
        }
        virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
            tw << "#include \"daScript/simulate/aot_builtin_rtti.h\"\n";
            tw << "#include \"daScript/ast/ast.h\"\n";
            return ModuleAotType::hybrid;
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_Rtti,das);
