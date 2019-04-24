#include "daScript/misc/platform.h"

#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/simulate/bind_enum.h"
#include "daScript/simulate/sim_policy.h"

namespace das {

    DAS_BASE_BIND_ENUM_98(Type, Type,
        none,           autoinfer,      alias,          fakeContext,
        anyArgument,    tVoid,          tBool,          tInt64,
        tUInt64,        tInt,           tInt2,          tInt3,
        tInt4,          tUInt,          tUInt2,         tUInt3,
        tUInt4,         tFloat,         tFloat2,        tFloat3,
        tFloat4,        tDouble,        tRange,         tURange,
        tString,        tStructure,     tHandle,        tEnumeration,
        tPointer,       tFunction,      tLambda,        tIterator,
        tArray,         tTable,         tBlock
    )

    MAKE_TYPE_FACTORY(TypeAnnotation,TypeAnnotation)
    MAKE_TYPE_FACTORY(StructInfo,StructInfo)
    MAKE_TYPE_FACTORY(EnumInfo,EnumInfo)
    MAKE_TYPE_FACTORY(EnumValueInfo,EnumValueInfo)
    MAKE_TYPE_FACTORY(TypeInfo,TypeInfo)
    MAKE_TYPE_FACTORY(VarInfo,VarInfo)

    struct TypeAnnotationAnnotation : ManagedStructureAnnotation <TypeAnnotation,false> {
        TypeAnnotationAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("TypeAnnotation", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(name)>("cppName");
        }
    };

    struct EnumValueInfoAnnotation : ManagedStructureAnnotation <EnumValueInfo,false> {
        EnumValueInfoAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("EnumValueInfo", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(value)>("value");
        }
    };

    /*
    struct EnumInfo {
        EnumValueInfo **    values;
    };
    */

    struct EnumInfoAnnotation : ManagedStructureAnnotation <EnumInfo,false> {
        EnumInfoAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("EnumInfo", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(totalValues)>("totalValues");
            addField<DAS_BIND_MANAGED_FIELD(hash)>("hash");
        }
    };

    /*
     struct StructInfo {
     VarInfo **  fields;
     };
     */

    struct StructInfoAnnotation : ManagedStructureAnnotation <StructInfo,false> {
        StructInfoAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("StructInfo", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(fieldsSize)>("fieldsSize");
            addField<DAS_BIND_MANAGED_FIELD(size)>("size");
            addField<DAS_BIND_MANAGED_FIELD(initializer)>("initializer");
            addField<DAS_BIND_MANAGED_FIELD(hash)>("hash");
        }
        void init () {
            fieldType = makeType<VarInfo>(*mlib);
        }
        struct SimNode_AtSIField : SimNode_At {
            DAS_PTR_NODE;
            SimNode_AtSIField ( const LineInfo & at, SimNode * rv, SimNode * idx, uint32_t ofs )
            : SimNode_At(at, rv, idx, 0, ofs, 0) {}
            __forceinline char * compute ( Context & context ) {
                auto pValue = (StructInfo *) value->evalPtr(context);
                uint32_t idx = cast<uint32_t>::to(index->eval(context));
                if ( idx >= pValue->fieldsSize ) {
                    context.throw_error_at(debugInfo,"StructInfo field index out of range");
                    return nullptr;
                } else {
                    return ((char *)(pValue->fields[idx])) + offset;
                }
            }
        };
        struct SIIterator : Iterator {
            SIIterator  ( StructInfo * ar ) : info(ar) {}
            virtual bool first ( Context &, char * _value ) override {
                VarInfo ** value = (VarInfo **) _value;
                if ( info->fieldsSize ) {
                    *value      = info->fields[0];
                    return true;
                } else {
                    *value      = nullptr;
                    return false;
                }
            }
            virtual bool next  ( Context &, char * _value ) override {
                // note - this iterator is N^2, but that way we can return field itself,
                // not pointer to pointer to field
                VarInfo ** value = (VarInfo **) _value;
                uint32_t idx = 0;
                for ( ; idx != info->fieldsSize; ++idx ) {
                    if ( info->fields[idx]==*value ) {
                        idx ++;
                        if ( idx != info->fieldsSize ) {
                            * value = info->fields[idx];
                            return true;
                        } else {
                            return false;
                        }
                    }
                }
                return false;
            }
            virtual void close ( Context &, char * _value ) override {
                VarInfo ** value = (VarInfo **) _value;
                *value = nullptr;
            }
            StructInfo * info;
        };
        virtual bool isIndexable ( const TypeDeclPtr & indexType ) const override {
            return indexType->isIndex();
        }
        virtual TypeDeclPtr makeIndexType ( const ExpressionPtr &, const ExpressionPtr & ) const override {
            return make_shared<TypeDecl>(*fieldType);
        }
        virtual SimNode * simulateGetAt ( Context & context, const LineInfo & at, const TypeDeclPtr &,
                                         const ExpressionPtr & rv, const ExpressionPtr & idx, uint32_t ofs ) const override {
            return context.code->makeNode<SimNode_AtSIField>(at,
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
            return context.code->makeNode<SimNode_AnyIterator<StructInfo,SIIterator>>(at, rv);
        }
        TypeDeclPtr fieldType;
    };

    template <typename TT>
    struct ManagedTypeInfoAnnotation : ManagedStructureAnnotation <TT,false> {
        ManagedTypeInfoAnnotation ( const string & st, ModuleLibrary & ml )
            : ManagedStructureAnnotation<TT,false> (st, ml) {
            using ManagedType = TT;
            this->addFieldEx ( "type", "type", offsetof(TypeInfo, type), makeType<Type_DasProxy>(ml) );
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
            context->throw_error("dim index out of range");
        }
        return ti.dim[index];
    }

    int32_t rtti_getDimTypeInfo ( const TypeInfo & ti, int32_t index, Context * context ) {
        return rtti_getDim(ti, index, context);
    }

    int32_t rtti_getDimVarInfo ( const VarInfo & ti, int32_t index, Context * context ) {
        return rtti_getDim(ti, index, context);
    }

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
            addEnumeration(make_shared<EnumerationType>());
            addAnnotation(make_shared<TypeAnnotationAnnotation>(lib));
            addAnnotation(make_shared<EnumValueInfoAnnotation>(lib));
            addAnnotation(make_shared<EnumInfoAnnotation>(lib));
            auto sia = make_shared<StructInfoAnnotation>(lib);
            addAnnotation(sia);
            addRecAnnotation<TypeInfoAnnotation>(lib);
            addRecAnnotation<VarInfoAnnotation>(lib);
            initRecAnnotation(sia, lib);
            addInterop<rtti_getTypeInfo,const TypeInfo &,vec4f>(*this, lib, "getTypeInfo", SideEffects::none);
            addExtern<DAS_BIND_FUN(rtti_getDimTypeInfo)>(*this, lib, "getDim", SideEffects::none);
            addExtern<DAS_BIND_FUN(rtti_getDimVarInfo)>(*this, lib, "getDim", SideEffects::none);
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_Rtti,das);
