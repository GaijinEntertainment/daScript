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

    template <typename OT, bool canNew = true, bool canDelete = canNew>
    struct ManagedStructureAnnotation ;

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
            TypeDeclPtr decl;
            uint32_t    offset;
            function<SimNode * (FactoryNodeType,Context &,const LineInfo &, const ExpressionPtr &)>   factory;
        };
        BasicStructureAnnotation(const string & n, const string & cpn, ModuleLibrary * l)
            : TypeAnnotation(n,cpn), mlib(l) {
        }
        virtual void seal(Module * m) override;
        virtual bool rtti_isHandledTypeAnnotation() const override { return true; }
        virtual bool rtti_isBasicStructureAnnotation() const override { return true; }
        virtual bool isRefType() const override { return true; }
        virtual TypeDeclPtr makeFieldType(const string & na) const override;
        virtual TypeDeclPtr makeSafeFieldType(const string & na) const override;
        virtual SimNode * simulateGetField(const string & na, Context & context,
            const LineInfo & at, const ExpressionPtr & value) const override;
        virtual SimNode * simulateGetFieldR2V(const string & na, Context & context,
            const LineInfo & at, const ExpressionPtr & value) const override;
        virtual SimNode * simulateSafeGetField(const string & na, Context & context,
            const LineInfo & at, const ExpressionPtr & value) const override;
        virtual SimNode * simulateSafeGetFieldPtr(const string & na, Context & context,
            const LineInfo & at, const ExpressionPtr & value) const override;
        virtual void aotVisitGetField(TextWriter & ss, const string & fieldName) override;
        virtual void aotVisitGetFieldPtr(TextWriter & ss, const string & fieldName) override;
        void addFieldEx(const string & na, const string & cppNa, off_t offset, TypeDeclPtr pT);
        virtual void walk(DataWalker & walker, void * data) override;
        int32_t fieldCount() const { return int32_t(fields.size()); }
        das_map<string,StructureField> fields;
        DebugInfoHelper            helpA;
        StructInfo *               sti = nullptr;
        ModuleLibrary *            mlib = nullptr;
    };

    template <typename OT>
    struct ManagedStructureAnnotation<OT,false,false> : BasicStructureAnnotation {
        typedef OT ManagedType;
        ManagedStructureAnnotation (const string & n, ModuleLibrary & ml, const string & cpn = "" )
            : BasicStructureAnnotation(n,cpn,&ml) { }
        virtual size_t getSizeOf() const override { return sizeof(ManagedType); }
        virtual size_t getAlignOf() const override { return ManagedStructureAlignofAuto<ManagedType, is_abstract<ManagedType>::value>::alignment; }
        virtual bool isSmart() const override { return is_base_of<ptr_ref_count,OT>::value; }
        virtual bool canMove() const override { return false; }
        virtual bool canCopy() const override { return false; }
        virtual bool isLocal() const override { return false; }
        template <typename FunT, FunT PROP>
        void addProperty ( const string & na, const string & cppNa="" ) {
            auto & field = fields[na];
            if ( field.decl ) {
                DAS_FATAL_LOG("structure field %s already exist in structure %s\n", na.c_str(), name.c_str() );
                DAS_FATAL_ERROR;
            }
            using resultType = decltype((((ManagedType *)0)->*PROP)());
            field.cppName = (cppNa.empty() ? na : cppNa) + "()";
            field.decl = makeType<resultType>(*mlib);
            DAS_ASSERTF ( !(field.decl->isRefType() && !field.decl->ref), "property can't be CMRES, in %s", field.decl->describe().c_str() );
            field.offset = -1U;
            field.factory = [](FactoryNodeType nt,Context & context,const LineInfo & at, const ExpressionPtr & value) -> SimNode * {
                switch ( nt ) {
                    case FactoryNodeType::getField:
                        return context.code->makeNode<SimNode_Property<ManagedType,FunT,PROP,false>>(at, value->simulate(context));
                    case FactoryNodeType::getFieldPtr:
                        return context.code->makeNode<SimNode_Property<ManagedType,FunT,PROP,true>>(at, value->simulate(context));
                    case FactoryNodeType::safeGetField:
                    case FactoryNodeType::safeGetFieldPtr:
                    case FactoryNodeType::getFieldR2V:
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
        __forceinline void addField ( const string & na, const string & cppNa = "" ) {
            addFieldEx ( na, cppNa.empty() ? na : cppNa, off, makeType<TT>(*mlib) );
        }
    };

    template <typename OT>
    struct ManagedStructureAnnotation<OT, true, false> : public ManagedStructureAnnotation<OT, false, false> {
        typedef OT ManagedType;
        enum { is_smart = is_base_of<ptr_ref_count, OT>::value };
        ManagedStructureAnnotation(const string& n, ModuleLibrary& ml, const string& cpn = "")
            : ManagedStructureAnnotation<OT, false,false>(n, ml, cpn) { }
        virtual bool canNew() const override { return true; }
        virtual string getSmartAnnotationCloneFunction() const override { return "smart_ptr_clone"; }
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
        virtual string getSmartAnnotationCloneFunction() const override { return "smart_ptr_clone"; }
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
        virtual string getSmartAnnotationCloneFunction () const override { return "smart_ptr_clone"; }
        virtual SimNode * simulateGetNew ( Context & context, const LineInfo & at ) const override {
            return context.code->makeNode<SimNode_NewHandle<ManagedType,is_smart>>(at);
        }
        virtual SimNode * simulateDeletePtr ( Context & context, const LineInfo & at, SimNode * sube, uint32_t count ) const override {
            return context.code->makeNode<SimNode_DeleteHandlePtr<ManagedType,is_smart>>(at,sube,count);
        }
    };

    template <typename OT, bool r2v=has_cast<typename OT::value_type>::value>
    struct ManagedVectorAnnotation;

    template <typename VectorType>
    struct ManagedVectorAnnotation<VectorType,false> : TypeAnnotation {
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
                V_SUB(value);
                V_SUB(index);
                V_ARG(stride);
                V_ARG(offset);
                V_ARG(range);
                V_END();
            }
            __forceinline char * compute ( Context & context ) {
                DAS_PROFILE_NODE
                auto pValue = (VectorType *) value->evalPtr(context);
                uint32_t idx = cast<uint32_t>::to(index->eval(context));
                if ( idx >= pValue->size() ) {
                    context.throw_error_at(debugInfo,"std::vector index out of range, %u of %u", idx, uint32_t(pValue->size()));
                    return nullptr;
                } else {
                    return ((char *)(pValue->data() + idx)) + offset;
                }
            }
        };
        struct VectorIterator : Iterator {
            VectorIterator  ( VectorType * ar ) : array(ar) {}
            virtual bool first ( Context &, char * _value ) override {
                char ** value = (char **) _value;
                char * data     = (char *) array->data();
                uint32_t size   = (uint32_t) array->size();
                *value          = data;
                array_end       = data + size * sizeof(OT);
                return (bool) size;
            }
            virtual bool next  ( Context &, char * _value ) override {
                char ** value = (char **) _value;
                char * data = *value + sizeof(OT);
                *value = data;
                return data != array_end;
            }
            virtual void close ( Context & context, char * _value ) override {
                if ( _value ) {
                    char ** value = (char **) _value;
                    *value = nullptr;
                }
                context.heap->free((char *)this, sizeof(VectorIterator));
            }
            VectorType * array;
            char * array_end = nullptr;
        };
        ManagedVectorAnnotation(const string & n, ModuleLibrary & lib)
            : TypeAnnotation(n) {
                vecType = makeType<OT>(lib);
                vecType->ref = true;
        }
        virtual bool rtti_isHandledTypeAnnotation() const override { return true; }
        virtual size_t getSizeOf() const override { return sizeof(VectorType); }
        virtual size_t getAlignOf() const override { return alignof(VectorType); }
        virtual bool isRefType() const override { return true; }
        virtual bool isIndexable ( const TypeDeclPtr & indexType ) const override { return indexType->isIndex(); }
        virtual bool isIterable ( ) const override { return true; }
        virtual bool canMove() const override { return false; }
        virtual bool canCopy() const override { return false; }
        virtual bool isLocal() const override { return false; }
        virtual TypeDeclPtr makeFieldType ( const string & na ) const override {
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
        virtual SimNode * simulateGetIterator ( Context & context, const LineInfo & at, const ExpressionPtr & src ) const override {
            auto rv = src->simulate(context);
            return context.code->makeNode<SimNode_AnyIterator<VectorType,VectorIterator>>(at, rv);
        }
        virtual SimNode * simulateGetField ( const string & na, Context & context,
                                            const LineInfo & at, const ExpressionPtr & value ) const override {
            if ( na=="length" ) return context.code->makeNode<SimNode_VectorLength>(at,value->simulate(context));
            return nullptr;
        }
        virtual void walk ( DataWalker & walker, void * vec ) override {
            if ( !ati ) {
                auto dimType = make_smart<TypeDecl>(*vecType);
                dimType->ref = 0;
                dimType->dim.push_back(1234);
                ati = helpA.makeTypeInfo(nullptr, dimType);
            }
            auto pVec = (VectorType *)vec;
            ati->dim[ati->dimSize - 1] = uint32_t(pVec->size());
            walker.walk_dim((char *)pVec->data(), ati);
        }
        TypeDeclPtr                vecType;
        DebugInfoHelper            helpA;
        TypeInfo *                 ati = nullptr;
    };

    template <typename VectorType>
    struct ManagedVectorAnnotation<VectorType,true> : ManagedVectorAnnotation<VectorType,false> {
        using OT = typename VectorType::value_type;
        ManagedVectorAnnotation(const string & n, ModuleLibrary & lib) :
            ManagedVectorAnnotation<VectorType, false>(n, lib) {
        }
        struct SimNode_AtStdVectorR2V : ManagedVectorAnnotation<VectorType,false>::SimNode_AtStdVector {
            SimNode_AtStdVectorR2V ( const LineInfo & at, SimNode * rv, SimNode * idx, uint32_t ofs )
                : ManagedVectorAnnotation<VectorType,false>::SimNode_AtStdVector(at, rv, idx, ofs) {}
            virtual vec4f eval ( Context & context ) override {
                DAS_PROFILE_NODE
                OT * pR = (OT *) ManagedVectorAnnotation<VectorType,false>::SimNode_AtStdVector::compute(context);
                return cast<OT>::from(*pR);
            }
#define EVAL_NODE(TYPE,CTYPE)                                           \
            virtual CTYPE eval##TYPE ( Context & context ) override {   \
                DAS_PROFILE_NODE \
                return *(CTYPE *)ManagedVectorAnnotation<VectorType,false>::SimNode_AtStdVector::compute(context);    \
            }
            DAS_EVAL_NODE
#undef EVAL_NODE
        };
        virtual SimNode * simulateGetAtR2V ( Context & context, const LineInfo & at, const TypeDeclPtr &,
                                            const ExpressionPtr & rv, const ExpressionPtr & idx, uint32_t ofs ) const override {
            return context.code->makeNode<SimNode_AtStdVectorR2V>(at,
                                                                  rv->simulate(context),
                                                                  idx->simulate(context),
                                                                  ofs);
        }
    };

    template <typename TT>
    struct typeName<vector<TT>> {
        static string name() {
            return "dasvector`" + typeName<TT>::name();
        }
    };

    template <typename TT, bool byValue = has_cast<TT>::value >
    struct registerVectorFunctions;

    template <typename TT>
    struct registerVectorFunctions<TT,false> {
        static void init ( Module * mod, const ModuleLibrary & lib ) {
            addExtern<DAS_BIND_FUN((das_vector_push<TT,TT>))>(*mod, lib, "push",
                SideEffects::modifyArgument, "das_vector_push")->generated = true;
            addExtern<DAS_BIND_FUN(das_vector_pop<TT>)>(*mod, lib, "pop",
                SideEffects::modifyArgument, "das_vector_pop")->generated = true;
            addExtern<DAS_BIND_FUN(das_vector_clear<TT>)>(*mod, lib, "clear",
                SideEffects::modifyArgument, "das_vector_clear")->generated = true;
            addExtern<DAS_BIND_FUN(das_vector_resize<TT>)>(*mod, lib, "resize",
                SideEffects::modifyArgument, "das_vector_resize")->generated = true;
        }
    };

    template <typename TT>
    struct registerVectorFunctions<TT,true> {
        static void init ( Module * mod, const ModuleLibrary & lib ) {
            addExtern<DAS_BIND_FUN((das_vector_push_value<TT,TT>))>(*mod, lib, "push",
                SideEffects::modifyArgument, "das_vector_push_value")->generated = true;
            addExtern<DAS_BIND_FUN(das_vector_pop<TT>)>(*mod, lib, "pop",
                SideEffects::modifyArgument, "das_vector_pop")->generated = true;
            addExtern<DAS_BIND_FUN(das_vector_clear<TT>)>(*mod, lib, "clear",
                SideEffects::modifyArgument, "das_vector_clear")->generated = true;
            addExtern<DAS_BIND_FUN(das_vector_resize<TT>)>(*mod, lib, "resize",
                SideEffects::modifyArgument, "das_vector_resize")->generated = true;
        }
    };

    template <typename TT>
    struct typeFactory<vector<TT>> {
        using VT = vector<TT>;
        static TypeDeclPtr make(const ModuleLibrary & library ) {
            auto declN = typeName<VT>::name();
            if ( library.findAnnotation(declN,nullptr).size()==0 ) {
                auto declT = makeType<TT>(library);
                auto ann = make_smart<ManagedVectorAnnotation<VT>>(declN,const_cast<ModuleLibrary &>(library));
                ann->cppName = "das::vector<" + describeCppType(declT) + ">";
                auto mod = library.front();
                mod->addAnnotation(ann);
                registerVectorFunctions<TT>::init(mod,library);
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
            DAS_FATAL_LOG("addVariable(%s) failed in module %s\n", name.c_str(), mod.name.c_str());
            DAS_FATAL_ERROR;
        }
    }

    template <typename TT>
    void addEquNeq(Module & mod, const ModuleLibrary & lib) {
        addExtern<decltype(&das_equ<TT>),  das_equ<TT>> (mod, lib, "==", SideEffects::none, "das_equ");
        addExtern<decltype(&das_nequ<TT>), das_nequ<TT>>(mod, lib, "!=", SideEffects::none, "das_nequ");
    }

    template <typename TT>
    void addEquNeqVal(Module & mod, const ModuleLibrary & lib) {
        addExtern<decltype(&das_equ_val<TT>),  das_equ<TT>> (mod, lib, "==", SideEffects::none, "das_equ_val");
        addExtern<decltype(&das_nequ_val<TT>), das_nequ<TT>>(mod, lib, "!=", SideEffects::none, "das_nequ_val");
    }
}

MAKE_TYPE_FACTORY(das_string, das::string);

#include "daScript/simulate/simulate_visit_op_undef.h"

