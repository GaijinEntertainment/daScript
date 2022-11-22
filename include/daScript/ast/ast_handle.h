#pragma once

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/simulate/interop.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/simulate_nodes.h"
#include "daScript/simulate/simulate_visit_op.h"

namespace das
{
    #define DAS_BIND_MANAGED_FIELD(FIELDNAME)   DAS_BIND_FIELD(ManagedType,FIELDNAME)
    #define DAS_BIND_MANAGED_PROP(FIELDNAME)    DAS_BIND_PROP(ManagedType,FIELDNAME)

    struct DummyTypeAnnotation : TypeAnnotation {
        DummyTypeAnnotation(const string & name, const string & cppName, size_t sz, size_t al)
            : TypeAnnotation(name,cppName), dummySize(sz), dummyAlignment(al) {
        }
        virtual bool rtti_isHandledTypeAnnotation() const override { return true; }
        virtual bool isRefType() const override { return true; }
        virtual bool isLocal() const override { return false; }
        virtual bool canCopy() const override { return false; }
        virtual bool canMove() const override { return false; }
        virtual bool canClone() const override { return false; }
        virtual size_t getAlignOf() const override { return dummyAlignment;}
        virtual size_t getSizeOf() const override { return dummySize;}
        size_t dummySize;
        size_t dummyAlignment;
    };

    struct DasStringTypeAnnotation : TypeAnnotation {
        DasStringTypeAnnotation() : TypeAnnotation("das_string","das::string") {}
        virtual bool rtti_isHandledTypeAnnotation() const override { return true; }
        virtual bool isRefType() const override { return true; }
        virtual bool isLocal() const override { return false; }
        virtual void walk ( DataWalker & dw, void * p ) override {
            auto pstr = (string *)p;
            if (dw.reading) {
                char * pss = nullptr;
                dw.String(pss);
                *pstr = pss;
            } else {
                char * pss = (char *) pstr->c_str();
                dw.String(pss);
            }
        }
        virtual bool canCopy() const override { return false; }
        virtual bool canMove() const override { return false; }
        virtual bool canClone() const override { return true; }
        virtual size_t getAlignOf() const override { return alignof(string);}
        virtual size_t getSizeOf() const override { return sizeof(string);}
        virtual SimNode * simulateClone ( Context & context, const LineInfo & at, SimNode * l, SimNode * r ) const override {
            return context.code->makeNode<SimNode_CloneRefValueT<string>>(at, l, r);
        }
    };

    template <typename OT>
    struct ManagedStructureAlignof {static constexpr size_t alignment = alignof(OT); } ;//we use due to MSVC inability to work with abstarct classes

    template <typename OT, bool abstract>
    struct ManagedStructureAlignofAuto {static constexpr size_t alignment = ManagedStructureAlignof<OT>::alignment; } ;//we use due to MSVC inability to work with abstarct classes

    template <typename OT>
    struct ManagedStructureAlignofAuto<OT, true> {static constexpr size_t alignment = sizeof(void*); } ;//we use due to MSVC inability to work with abstarct classes

    template <typename OT,
        bool canNew = is_default_constructible<OT>::value,
        bool canDelete = canNew && is_destructible<OT>::value
    > struct ManagedStructureAnnotation ;

    struct BasicStructureAnnotation : TypeAnnotation {
        enum class FactoryNodeType {
            getField
        ,   getFieldR2V
        ,   getFieldPtr
        ,   getFieldPtrR2V
        ,   safeGetField
        ,   safeGetFieldPtr
        };
        struct StructureField {
            string      name;
            string      cppName;
            string      aotPrefix;
            string      aotPostfix;
            TypeDeclPtr decl;
            TypeDeclPtr constDecl;
            uint32_t    offset;
            function<SimNode * (FactoryNodeType,Context &,const LineInfo &, const ExpressionPtr &)>   factory;
            __forceinline void adjustAot ( const char * pref, const char * postf ) { aotPrefix=pref; aotPostfix=postf; }
        };
        BasicStructureAnnotation(const string & n, const string & cpn, ModuleLibrary * l)
            : TypeAnnotation(n,cpn), mlib(l) {
        }
        virtual string getSmartAnnotationCloneFunction() const override { return "smart_ptr_clone"; }
        virtual void seal(Module * m) override;
        virtual bool rtti_isHandledTypeAnnotation() const override { return true; }
        virtual bool rtti_isBasicStructureAnnotation() const override { return true; }
        virtual bool isRefType() const override { return true; }
        virtual int32_t getGcFlags(das_set<Structure *> &, das_set<Annotation *> &) const override;
        virtual TypeDeclPtr makeFieldType(const string & na, bool isConst) const override;
        virtual TypeDeclPtr makeSafeFieldType(const string & na, bool isConst) const override;
        virtual SimNode * simulateGetField(const string & na, Context & context,
            const LineInfo & at, const ExpressionPtr & value) const override;
        virtual SimNode * simulateGetFieldR2V(const string & na, Context & context,
            const LineInfo & at, const ExpressionPtr & value) const override;
        virtual SimNode * simulateSafeGetField(const string & na, Context & context,
            const LineInfo & at, const ExpressionPtr & value) const override;
        virtual SimNode * simulateSafeGetFieldPtr(const string & na, Context & context,
            const LineInfo & at, const ExpressionPtr & value) const override;
        virtual void aotPreVisitGetField ( TextWriter &, const string & ) override;
        virtual void aotPreVisitGetFieldPtr ( TextWriter &, const string & ) override;
        virtual void aotVisitGetField(TextWriter & ss, const string & fieldName) override;
        virtual void aotVisitGetFieldPtr(TextWriter & ss, const string & fieldName) override;
        virtual bool canSubstitute(TypeAnnotation * ann) const override;
        StructureField & addFieldEx(const string & na, const string & cppNa, off_t offset, TypeDeclPtr pT);
        virtual void walk(DataWalker & walker, void * data) override;
        int32_t fieldCount() const { return int32_t(fields.size()); }
        void from(BasicStructureAnnotation * ann);
        void from(const char* parentName);
        das_map<string,StructureField> fields;
        vector<string>                 fieldsInOrder;
        DebugInfoHelper            helpA;
        StructInfo *               sti = nullptr;
        ModuleLibrary *            mlib = nullptr;
        vector<TypeAnnotation*> parents;
        bool validationNeverFails = false;
        recursive_mutex walkMutex;
    };

    template <typename TT, bool canCopy = isCloneable<TT>::value>
    struct GenCloneNode;

    template <typename TT>
    struct GenCloneNode<TT,true> {
        static __forceinline SimNode * simulateClone ( Context & context, const LineInfo & at, SimNode * l, SimNode * r ) {
            return context.code->makeNode<SimNode_CloneRefValueT<TT>>(at, l, r);
        }
    };

    template <typename TT>
    struct GenCloneNode<TT,false> {
        static __forceinline SimNode * simulateClone ( Context &, const LineInfo &, SimNode *, SimNode * ) {
            return nullptr;
        }
    };

    template <typename OT>
    struct ManagedStructureAnnotation<OT,false,false> : BasicStructureAnnotation {
        typedef OT ManagedType;
        ManagedStructureAnnotation (const string & n, ModuleLibrary & ml, const string & cpn = "" )
            : BasicStructureAnnotation(n,cpn,&ml) { }
        virtual size_t getSizeOf() const override { return sizeof(ManagedType); }
        virtual size_t getAlignOf() const override { return ManagedStructureAlignofAuto<ManagedType, is_abstract<ManagedType>::value>::alignment; }
        virtual bool isSmart() const override { return is_base_of<ptr_ref_count,OT>::value; }
        virtual bool hasNonTrivialCtor() const override {
            return !is_trivially_constructible<OT>::value;
        }
        virtual bool hasNonTrivialDtor() const override {
            return !is_trivially_destructible<OT>::value;
        }
        virtual bool hasNonTrivialCopy() const override {
            return  !is_trivially_copyable<OT>::value
                ||  !is_trivially_copy_constructible<OT>::value;
        }
        virtual bool isPod() const override {
            return is_pod<OT>::value;
        }
        virtual bool isRawPod() const override {
            return false;   // can we detect this?
        }
        virtual bool canClone() const override {
            return isCloneable<OT>::value;
        }
        template <typename FunT, FunT PROP>
        void addProperty ( const string & na, const string & cppNa="" ) {
            auto & field = fields[na];
            if ( field.decl ) {
                DAS_FATAL_ERROR("structure field %s already exist in structure %s\n", na.c_str(), name.c_str() );
            }
            using resultType = decltype((((ManagedType *)0)->*PROP)());
            field.cppName = (cppNa.empty() ? na : cppNa) + "()";
            field.decl = makeType<resultType>(*mlib);
            DAS_ASSERTF ( !(field.decl->isRefType() && !field.decl->ref), "property can't be CMRES, in %s", field.decl->describe().c_str() );
            field.offset = -1U;
            auto sft = make_smart<TypeDecl>(*field.decl);
            field.factory = [sft](FactoryNodeType nt,Context & context,const LineInfo & at, const ExpressionPtr & value) -> SimNode * {
                switch ( nt ) {
                    case FactoryNodeType::getField:
                        return context.code->makeNode<SimNode_Property<ManagedType,FunT,PROP,false>>(at, value->simulate(context));
                    case FactoryNodeType::getFieldPtr:
                        return context.code->makeNode<SimNode_Property<ManagedType,FunT,PROP,true>>(at, value->simulate(context));
                    case FactoryNodeType::getFieldR2V: {
                        auto getFieldNode = context.code->makeNode<SimNode_Property<ManagedType,FunT,PROP,true>>(at, value->simulate(context));
                        return ExprRef2Value::GetR2V(context, at, sft, getFieldNode);
                    }
                    case FactoryNodeType::safeGetField:
                    case FactoryNodeType::safeGetFieldPtr:
                    case FactoryNodeType::getFieldPtrR2V:
                        DAS_ASSERTF(0, "property requested property type, which is meaningless for the non-ref"
                                    "we should not be here, since property can't have ref type"
                                    "daScript compiler will later report missing node error");
                    default:
                        return nullptr;
                }
            };
        }
        template <typename FunT, FunT PROP, typename FunTConst, FunTConst PROP_CONST>
        void addPropertyExtConst ( const string & na, const string & cppNa="" ) {
            auto & field = fields[na];
            if ( field.decl ) {
                DAS_FATAL_ERROR("structure field %s already exist in structure %s\n", na.c_str(), name.c_str() );
            }
            using resultType = decltype((((ManagedType *)0)->*PROP)());
            field.cppName = (cppNa.empty() ? na : cppNa) + "()";
            field.decl = makeType<resultType>(*mlib);
            DAS_ASSERTF ( !(field.decl->isRefType() && !field.decl->ref), "property can't be CMRES, in %s", field.decl->describe().c_str() );
            using constResultType = decltype((((ManagedType *)0)->*PROP_CONST)());
            field.constDecl = makeType<constResultType>(*mlib);
            DAS_ASSERTF ( !(field.constDecl->isRefType() && !field.constDecl->ref), "property can't be CMRES, in %s", field.constDecl->describe().c_str() );
            field.offset = -1U;
            auto sft = make_smart<TypeDecl>(*field.decl);
            auto sftc = make_smart<TypeDecl>(*field.constDecl);
            field.factory = [sft,sftc](FactoryNodeType nt,Context & context,const LineInfo & at, const ExpressionPtr & value) -> SimNode * {
                switch ( nt ) {
                    case FactoryNodeType::getField:
                        if ( value->type->constant ) {
                            return context.code->makeNode<SimNode_Property<ManagedType,FunTConst,PROP_CONST,false>>(at, value->simulate(context));
                        } else {
                            return context.code->makeNode<SimNode_Property<ManagedType,FunT,PROP,false>>(at, value->simulate(context));
                        }
                    case FactoryNodeType::getFieldPtr:
                        if ( value->type->constant ) {
                            return context.code->makeNode<SimNode_Property<ManagedType,FunTConst,PROP_CONST,true>>(at, value->simulate(context));
                        } else {
                            return context.code->makeNode<SimNode_Property<ManagedType,FunT,PROP,true>>(at, value->simulate(context));
                        }
                    case FactoryNodeType::getFieldR2V:
                        if ( value->type->constant ) {
                            auto getFieldNode =  context.code->makeNode<SimNode_Property<ManagedType,FunTConst,PROP_CONST,false>>(at, value->simulate(context));
                            return ExprRef2Value::GetR2V(context, at, sftc, getFieldNode);
                        } else {
                            auto getFieldNode =  context.code->makeNode<SimNode_Property<ManagedType,FunT,PROP,false>>(at, value->simulate(context));
                            return ExprRef2Value::GetR2V(context, at, sft, getFieldNode);
                        }
                    case FactoryNodeType::safeGetField:
                    case FactoryNodeType::safeGetFieldPtr:
                    case FactoryNodeType::getFieldPtrR2V:
                        DAS_ASSERTF(0, "property requested property type, which is meaningless for the non-ref"
                                    "we should not be here, since property can't have ref type"
                                    "daScript compiler will later report missing node error");
                    default:
                        return nullptr;
                }
            };
        }
        template <typename TT, off_t off>
        __forceinline StructureField & addField ( const string & na, const string & cppNa = "" ) {
            return addFieldEx ( na, cppNa.empty() ? na : cppNa, off, makeType<TT>(*mlib) );
        }
        virtual SimNode * simulateCopy ( Context & context, const LineInfo & at, SimNode * l, SimNode * r ) const override {
            return context.code->makeNode<SimNode_CopyRefValue>(at, l, r, uint32_t(sizeof(OT)));
        }
        virtual SimNode * simulateClone ( Context & context, const LineInfo & at, SimNode * l, SimNode * r ) const override {
            return GenCloneNode<OT>::simulateClone(context,at,l,r);
        }
    };

    template <typename OT>
    struct ManagedStructureAnnotation<OT, true, false> : public ManagedStructureAnnotation<OT, false, false> {
        typedef OT ManagedType;
        enum { is_smart = is_base_of<ptr_ref_count, OT>::value };
        ManagedStructureAnnotation(const string& n, ModuleLibrary& ml, const string& cpn = "")
            : ManagedStructureAnnotation<OT, false,false>(n, ml, cpn) { }
        virtual bool canNew() const override { return true; }
        virtual SimNode* simulateGetNew(Context& context, const LineInfo& at) const override {
            return context.code->makeNode<SimNode_NewHandle<ManagedType, is_smart>>(at);
        }
    };

    template <typename OT>
    struct ManagedStructureAnnotation<OT, false, true> : public ManagedStructureAnnotation<OT, false, false> {
        typedef OT ManagedType;
        enum { is_smart = is_base_of<ptr_ref_count, OT>::value };
        ManagedStructureAnnotation(const string& n, ModuleLibrary& ml, const string& cpn = "")
            : ManagedStructureAnnotation<OT, false,false>(n, ml, cpn) { }
        virtual bool canDeletePtr() const override { return true; }
        virtual SimNode* simulateDeletePtr(Context& context, const LineInfo& at, SimNode* sube, uint32_t count) const override {
            return context.code->makeNode<SimNode_DeleteHandlePtr<ManagedType, is_smart>>(at, sube, count);
        }
    };

    template <typename OT>
    struct ManagedStructureAnnotation<OT,true,true> : public ManagedStructureAnnotation<OT,false,false> {
        typedef OT ManagedType;
        enum { is_smart = is_base_of<ptr_ref_count,OT>::value };
        ManagedStructureAnnotation (const string & n, ModuleLibrary & ml, const string & cpn = "" )
            : ManagedStructureAnnotation<OT,false,false>(n,ml,cpn) { }
        virtual bool canNew() const override { return true; }
        virtual bool canDeletePtr() const override { return true; }
        virtual SimNode * simulateGetNew ( Context & context, const LineInfo & at ) const override {
            return context.code->makeNode<SimNode_NewHandle<ManagedType,is_smart>>(at);
        }
        virtual SimNode * simulateDeletePtr ( Context & context, const LineInfo & at, SimNode * sube, uint32_t count ) const override {
            return context.code->makeNode<SimNode_DeleteHandlePtr<ManagedType,is_smart>>(at,sube,count);
        }
    };

    template <typename VectorType>
    struct ManagedVectorAnnotation : TypeAnnotation {
        using OT = typename VectorType::value_type;
        struct SimNode_VectorLength : SimNode {
            using TT = OT;
            DAS_INT_NODE;
            SimNode_VectorLength ( const LineInfo & at, SimNode * rv )
                : SimNode(at), value(rv) {}
            __forceinline int32_t compute ( Context & context ) {
                DAS_PROFILE_NODE
                auto pValue = (VectorType *) value->evalPtr(context);
                return int32_t(pValue->size());
            }
            virtual SimNode * visit ( SimVisitor & vis ) override {
                V_BEGIN();
                V_OP_TT(StdVectorLength);
                V_SUB(value);
                V_END();
            }
            SimNode * value;
        };
        struct SimNode_AtStdVector : SimNode_At {
            using TT = OT;
            DAS_PTR_NODE;
            SimNode_AtStdVector ( const LineInfo & at, SimNode * rv, SimNode * idx, uint32_t ofs )
                : SimNode_At(at, rv, idx, 0, ofs, 0) {}
            virtual SimNode * visit ( SimVisitor & vis ) override {
                V_BEGIN();
                V_OP_TT(AtStdVector);
                V_SUB_THIS(value);
                V_SUB_THIS(index);
                V_ARG_THIS(stride);
                V_ARG_THIS(offset);
                V_ARG_THIS(range);
                V_END();
            }
            ___noinline char * compute ( Context & context ) {
                DAS_PROFILE_NODE
                auto pValue = (VectorType *) value->evalPtr(context);
                uint32_t idx = cast<uint32_t>::to(index->eval(context));
                if ( idx >= uint32_t(pValue->size()) ) {
                    context.throw_error_at(debugInfo,"std::vector index out of range, %u of %u", idx, uint32_t(pValue->size()));
                    return nullptr;
                } else {
                    return ((char *)(pValue->data() + idx)) + offset;
                }
            }
        };
        template <typename OOT>
        struct SimNode_AtStdVectorR2V : SimNode_AtStdVector {
            using TT = OOT;
            SimNode_AtStdVectorR2V ( const LineInfo & at, SimNode * rv, SimNode * idx, uint32_t ofs )
                : SimNode_AtStdVector(at, rv, idx, ofs) {}
            virtual SimNode * visit ( SimVisitor & vis ) override {
                V_BEGIN();
                V_OP_TT(AtStdVectorR2V);
                V_SUB_THIS(value);
                V_SUB_THIS(index);
                V_ARG_THIS(stride);
                V_ARG_THIS(offset);
                V_ARG_THIS(range);
                V_END();
            }
            virtual vec4f DAS_EVAL_ABI eval ( Context & context ) override {
                OOT * pR = (OOT *) SimNode_AtStdVector::compute(context);
                DAS_ASSERT(pR);
                return cast<OOT>::from(*pR);
            }
#define EVAL_NODE(TYPE,CTYPE)                                           \
            virtual CTYPE eval##TYPE ( Context & context ) override {   \
                return *(CTYPE *) SimNode_AtStdVector::compute(context);    \
            }
            DAS_EVAL_NODE
#undef EVAL_NODE
        };
        ManagedVectorAnnotation(const string & n, ModuleLibrary & lib)
            : TypeAnnotation(n) {
                vecType = makeType<OT>(lib);
                vecType->ref = true;
        }
        virtual bool rtti_isHandledTypeAnnotation() const override { return true; }
        virtual int32_t getGcFlags(das_set<Structure *> & dep, das_set<Annotation *> & depA) const override {
            return vecType->gcFlags(dep,depA);
        }
        virtual size_t getSizeOf() const override { return sizeof(VectorType); }
        virtual size_t getAlignOf() const override { return alignof(VectorType); }
        virtual bool isRefType() const override { return true; }
        virtual bool isIndexable ( const TypeDeclPtr & indexType ) const override { return indexType->isIndex(); }
        virtual bool isIterable ( ) const override { return true; }
        virtual bool canMove() const override { return false; }
        virtual bool canCopy() const override { return false; }
        virtual bool isLocal() const override { return false; }
        virtual TypeDeclPtr makeFieldType ( const string & na, bool ) const override {
            if ( na=="length" ) return make_smart<TypeDecl>(Type::tInt);
            return nullptr;
        }
        virtual void aotVisitGetField ( TextWriter & ss, const string & fieldName ) override {
            if ( fieldName=="length" ) {
                ss << ".size()";
            } else {
                ss << "." << fieldName << " /*undefined */";
            }
        }
        virtual void aotVisitGetFieldPtr ( TextWriter & ss, const string & fieldName ) override {
            if ( fieldName=="length" ) {
                ss << "->size()";
            } else {
                ss << "." << fieldName << " /*undefined */";
            }
        }
        virtual TypeDeclPtr makeIndexType ( const ExpressionPtr &, const ExpressionPtr & ) const override {
            return make_smart<TypeDecl>(*vecType);
        }
        virtual TypeDeclPtr makeIteratorType ( const ExpressionPtr & ) const override {
            return make_smart<TypeDecl>(*vecType);
        }
        virtual SimNode * simulateGetAt ( Context & context, const LineInfo & at, const TypeDeclPtr &,
                                         const ExpressionPtr & rv, const ExpressionPtr & idx, uint32_t ofs ) const override {
            return context.code->makeNode<SimNode_AtStdVector>(at,
                                                               rv->simulate(context),
                                                               idx->simulate(context),
                                                               ofs);
        }
        virtual SimNode * simulateGetAtR2V ( Context & context, const LineInfo & at, const TypeDeclPtr & type,
                                            const ExpressionPtr & rv, const ExpressionPtr & idx, uint32_t ofs ) const override {
            if ( type->isHandle() ) {
                auto expr = context.code->makeNode<SimNode_AtStdVector>(at,
                                                                rv->simulate(context),
                                                                idx->simulate(context),
                                                                ofs);
                return ExprRef2Value::GetR2V(context, at, type, expr);
            } else {
                return context.code->makeValueNode<SimNode_AtStdVectorR2V>(type->baseType,
                                                                at,
                                                                rv->simulate(context),
                                                                idx->simulate(context),
                                                                ofs);
            }
        }

        virtual SimNode * simulateGetIterator ( Context & context, const LineInfo & at, const ExpressionPtr & src ) const override {
            auto rv = src->simulate(context);
            return context.code->makeNode<SimNode_AnyIterator<VectorType,StdVectorIterator<VectorType>>>(at, rv);
        }
        virtual SimNode * simulateGetField ( const string & na, Context & context,
                                            const LineInfo & at, const ExpressionPtr & value ) const override {
            if ( na=="length" ) return context.code->makeNode<SimNode_VectorLength>(at,value->simulate(context));
            return nullptr;
        }
        virtual void walk ( DataWalker & walker, void * vec ) override {
            {
                lock_guard<recursive_mutex> guard(walkMutex);
                if ( !ati ) {
                    auto dimType = make_smart<TypeDecl>(*vecType);
                    dimType->ref = 0;
                    dimType->dim.push_back(1);
                    ati = helpA.makeTypeInfo(nullptr, dimType);
                    ati->flags |= TypeInfo::flag_isHandled;
                }
            }
            auto pVec = (VectorType *)vec;
            auto atit = *ati;
            atit.dim[atit.dimSize - 1] = uint32_t(pVec->size());
            atit.size = int(ati->size * pVec->size());
            walker.walk_dim((char *)pVec->data(), &atit);
        }
        virtual bool isYetAnotherVectorTemplate() const override { return true; }
        TypeDeclPtr                vecType;
        DebugInfoHelper            helpA;
        TypeInfo *                 ati = nullptr;
        recursive_mutex            walkMutex;
    };

    template <typename TT>
    struct typeName<vector<TT>> {
        static string name() {
            return string("dasvector`") + typeName<TT>::name(); // TODO: compilation time concat
        }
    };

    template <typename TT, bool byValue = has_cast<typename TT::value_type>::value >
    struct registerVectorFunctions;


    template <typename TT>
    struct registerVectorFunctions<TT,false> {
        static void init ( Module * mod, const ModuleLibrary & lib, bool canCopy, bool canMove ) {
            if ( canMove ) {
                addExtern<DAS_BIND_FUN((das_vector_emplace<TT>)),SimNode_ExtFuncCall,permanentArgFn>(*mod, lib, "emplace",
                    SideEffects::modifyArgument, "das_vector_emplace")->generated = true;
                addExtern<DAS_BIND_FUN((das_vector_emplace_back<TT>)),SimNode_ExtFuncCall,permanentArgFn>(*mod, lib, "emplace",
                    SideEffects::modifyArgument, "das_vector_emplace_back")->generated = true;
            }
            if ( canCopy ) {
                addExtern<DAS_BIND_FUN((das_vector_push<TT>)),SimNode_ExtFuncCall,permanentArgFn>(*mod, lib, "push",
                    SideEffects::modifyArgument, "das_vector_push")->generated = true;
                addExtern<DAS_BIND_FUN((das_vector_push_back<TT>)),SimNode_ExtFuncCall,permanentArgFn>(*mod, lib, "push",
                    SideEffects::modifyArgument, "das_vector_push_back")->generated = true;
            }
            if constexpr ( std::is_default_constructible_v<typename TT::value_type> ) {
                addExtern<DAS_BIND_FUN((das_vector_push_empty<TT>)),SimNode_ExtFuncCall,permanentArgFn>(*mod, lib, "push_empty",
                    SideEffects::modifyArgument, "das_vector_push_empty")->generated = true;
                addExtern<DAS_BIND_FUN((das_vector_push_back_empty<TT>)),SimNode_ExtFuncCall,permanentArgFn>(*mod, lib, "push_empty",
                    SideEffects::modifyArgument, "das_vector_push_back_empty")->generated = true;
            }
            addExtern<DAS_BIND_FUN(das_vector_pop<TT>)>(*mod, lib, "pop",
                SideEffects::modifyArgument, "das_vector_pop")->generated = true;
            addExtern<DAS_BIND_FUN(das_vector_clear<TT>)>(*mod, lib, "clear",
                SideEffects::modifyArgument, "das_vector_clear")->generated = true;
            addExtern<DAS_BIND_FUN(das_vector_resize<TT>)>(*mod, lib, "resize",
                SideEffects::modifyArgument, "das_vector_resize")->generated = true;
            addExtern<DAS_BIND_FUN(das_vector_erase<TT>)>(*mod, lib, "erase",
                SideEffects::modifyArgument, "das_vector_erase")->generated = true;
            addExtern<DAS_BIND_FUN(das_vector_erase_range<TT>)>(*mod, lib, "erase",
                SideEffects::modifyArgument, "das_vector_erase_range")->generated = true;
            addExtern<DAS_BIND_FUN(das_vector_each<TT>),SimNode_ExtFuncCallAndCopyOrMove,explicitConstArgFn>(*mod, lib, "each",
                SideEffects::none, "das_vector_each")->generated = true;
            addExtern<DAS_BIND_FUN(das_vector_each_const<TT>),SimNode_ExtFuncCallAndCopyOrMove,explicitConstArgFn>(*mod, lib, "each",
                SideEffects::none, "das_vector_each_const")->generated = true;
        }
    };

    template <typename TT>
    struct registerVectorFunctions<TT,true> {
        static void init ( Module * mod, const ModuleLibrary & lib, bool canCopy, bool canMove ) {
            if ( canMove ) {
                addExtern<DAS_BIND_FUN((das_vector_emplace<TT>))>(*mod, lib, "emplace",
                    SideEffects::modifyArgument, "das_vector_emplace")
                        ->args({"vec","value","at"})->generated = true;
                addExtern<DAS_BIND_FUN((das_vector_emplace_back<TT>))>(*mod, lib, "emplace",
                    SideEffects::modifyArgument, "das_vector_emplace_back")
                        ->args({"vec","value"})->generated = true;
            }
            if ( canCopy ) {
                addExtern<DAS_BIND_FUN((das_vector_push_value<TT>))>(*mod, lib, "push",
                    SideEffects::modifyArgument, "das_vector_push_value")
                        ->args({"vec","value","at","context"})->generated = true;
                addExtern<DAS_BIND_FUN((das_vector_push_back_value<TT>))>(*mod, lib, "push",
                    SideEffects::modifyArgument, "das_vector_push_back_value")
                        ->args({"vec","value"})->generated = true;
            }
            if constexpr ( std::is_default_constructible_v<typename TT::value_type> ) {
              addExtern<DAS_BIND_FUN((das_vector_push_empty<TT>))>(*mod, lib, "push_empty",
                  SideEffects::modifyArgument, "das_vector_push_empty")
                      ->args({"vec","at","context"})->generated = true;
              addExtern<DAS_BIND_FUN((das_vector_push_back_empty<TT>))>(*mod, lib, "push_empty",
                    SideEffects::modifyArgument, "das_vector_push_back_empty")
                        ->args({"vec"})->generated = true;
            }
            addExtern<DAS_BIND_FUN(das_vector_pop<TT>)>(*mod, lib, "pop",
                SideEffects::modifyArgument, "das_vector_pop")
                    ->arg("vec")->generated = true;
            addExtern<DAS_BIND_FUN(das_vector_clear<TT>)>(*mod, lib, "clear",
                SideEffects::modifyArgument, "das_vector_clear")
                    ->arg("vec")->generated = true;
            addExtern<DAS_BIND_FUN(das_vector_resize<TT>)>(*mod, lib, "resize",
                SideEffects::modifyArgument, "das_vector_resize")
                    ->args({"vec","newSize"})->generated = true;
            addExtern<DAS_BIND_FUN(das_vector_erase<TT>)>(*mod, lib, "erase",
                SideEffects::modifyArgument, "das_vector_erase")
                    ->args({"vec","index","context"})->generated = true;
            addExtern<DAS_BIND_FUN(das_vector_erase_range<TT>)>(*mod, lib, "erase",
                SideEffects::modifyArgument, "das_vector_erase_range")
                    ->args({"vec","index","count","context"})->generated = true;
            addExtern<DAS_BIND_FUN(das_vector_each<TT>),SimNode_ExtFuncCallAndCopyOrMove,explicitConstArgFn>(*mod, lib, "each",
                SideEffects::none, "das_vector_each")
                    ->args({"vec","context"})->generated = true;
            addExtern<DAS_BIND_FUN(das_vector_each_const<TT>),SimNode_ExtFuncCallAndCopyOrMove,explicitConstArgFn>(*mod, lib, "each",
                SideEffects::none, "das_vector_each_const")
                    ->args({"vec","context"})->generated = true;
        }
    };

    template <typename TT>
    struct typeFactory<vector<TT>> {
        using VT = vector<TT>;
        static TypeDeclPtr make(const ModuleLibrary & library ) {
            string declN = typeName<VT>::name();
            if ( library.findAnnotation(declN,nullptr).size()==0 ) {
                auto declT = makeType<TT>(library);
                auto ann = make_smart<ManagedVectorAnnotation<VT>>(declN,const_cast<ModuleLibrary &>(library));
                ann->cppName = "das::vector<" + describeCppType(declT) + ">";
                auto mod = library.front();
                mod->addAnnotation(ann);
                registerVectorFunctions<vector<TT>>::init(mod,library,
                    declT->canCopy(),
                    declT->canMove()
                );
            }
            return makeHandleType(library,declN.c_str());
        }
    };

    template <typename OT>
    struct ManagedValueAnnotation : TypeAnnotation {
        static_assert(sizeof(OT)<=sizeof(vec4f), "value types have to fit in ABI");
        ManagedValueAnnotation(const string & n, const string & cpn = string()) : TypeAnnotation(n,cpn) {}
        virtual bool rtti_isHandledTypeAnnotation() const override { return true; }
        virtual bool canMove() const override { return true; }
        virtual bool canCopy() const override { return true; }
        virtual bool isLocal() const override { return true; }
        virtual bool hasNonTrivialCtor() const override {
            return !is_trivially_constructible<OT>::value;
        }
        virtual bool hasNonTrivialDtor() const override {
            return !is_trivially_destructible<OT>::value;
        }
        virtual bool hasNonTrivialCopy() const override {
            return  !is_trivially_copyable<OT>::value
                ||  !is_trivially_copy_constructible<OT>::value;
        }
        virtual bool isPod() const override { return true; }
        virtual bool isRawPod() const override { return true; }
        virtual size_t getSizeOf() const override { return sizeof(OT); }
        virtual size_t getAlignOf() const override { return alignof(OT); }
        virtual bool isRefType() const override { return false; }
        virtual SimNode * simulateCopy ( Context & context, const LineInfo & at, SimNode * l, SimNode * r ) const override {
            return context.code->makeNode<SimNode_Set<OT>>(at, l, r);
        }
        virtual SimNode * simulateRef2Value ( Context & context, const LineInfo & at, SimNode * l ) const override {
            return context.code->makeNode<SimNode_Ref2Value<OT>>(at, l);
        }
        virtual SimNode * simulateNullCoalescing ( Context & context, const LineInfo & at, SimNode * s, SimNode * dv ) const override {
            return context.code->makeNode<SimNode_NullCoalescing<OT>>(at,s,dv);
        }
    };

    template <typename TT>
    void addConstant ( Module & mod, const string & name, const TT & value ) {
        VariablePtr pVar = make_smart<Variable>();
        pVar->name = name;
        pVar->type = make_smart<TypeDecl>((Type)ToBasicType<TT>::type);
        pVar->type->constant = true;
        pVar->init = Program::makeConst(LineInfo(),pVar->type,cast<TT>::from(value));
        pVar->init->type = make_smart<TypeDecl>(*pVar->type);
        pVar->init->constexpression = true;
        pVar->initStackSize = sizeof(Prologue);
        if ( !mod.addVariable(pVar) ) {
            DAS_FATAL_ERROR("addVariable(%s) failed in module %s\n", name.c_str(), mod.name.c_str());
        }
    }
    __forceinline void addConstant ( Module & mod, const string & name, const string & value ) {
        VariablePtr pVar = make_smart<Variable>();
        pVar->name = name;
        pVar->type = make_smart<TypeDecl>(Type::tString);
        pVar->type->constant = true;
        pVar->init = make_smart<ExprConstString>(LineInfo(),value);
        pVar->init->type = make_smart<TypeDecl>(*pVar->type);
        pVar->init->constexpression = true;
        pVar->initStackSize = sizeof(Prologue);
        if ( !mod.addVariable(pVar) ) {
            DAS_FATAL_ERROR("addVariable(%s) failed in module %s\n", name.c_str(), mod.name.c_str());
        }
    }
    __forceinline void addConstant ( Module & mod, const string & name, const char * value ) { addConstant(mod, name, string(value)); }
    __forceinline void addConstant ( Module & mod, const string & name, char * value ) { addConstant(mod, name, string(value)); }

    template <typename TT>
    void addEquNeq(Module & mod, const ModuleLibrary & lib) {
        addExtern<decltype(&das_equ<TT>),  das_equ<TT>> (mod, lib, "==", SideEffects::none, "das_equ");
        addExtern<decltype(&das_nequ<TT>), das_nequ<TT>>(mod, lib, "!=", SideEffects::none, "das_nequ");
    }

    template <typename TT>
    void addEquNeqVal(Module & mod, const ModuleLibrary & lib) {
        addExtern<decltype(&das_equ_val<TT,TT>),  das_equ_val<TT,TT>> (mod, lib, "==", SideEffects::none, "das_equ_val");
        addExtern<decltype(&das_nequ_val<TT,TT>), das_nequ_val<TT,TT>>(mod, lib, "!=", SideEffects::none, "das_nequ_val");
    }

    void setParents ( Module * mod, const char * child, const std::initializer_list<const char *> & parents );
}

MAKE_TYPE_FACTORY(das_string, das::string);

#include "daScript/simulate/simulate_visit_op_undef.h"

