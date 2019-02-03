#pragma once

#include "daScript/ast/ast.h"

namespace das
{
    #define DAS_BIND_MANAGED_FIELD(FIELDNAME)   DAS_BIND_FIELD(ManagedType,FIELDNAME)
    #define DAS_BIND_MANAGED_PROP(FIELDNAME)    DAS_BIND_PROP(ManagedType,FIELDNAME)

    template <typename OT>
    struct ManagedStructureAnnotation  : TypeAnnotation {
        typedef OT ManagedType;
        enum class FactoryNodeType {
            getField
        ,   getFieldR2V
        ,   safeGetField
        ,   safeGetFieldPtr
        };
        struct StructureField {
            string      name;
            TypeDeclPtr decl;
            uint32_t    offset;
            function<SimNode * (FactoryNodeType,Context &,const LineInfo &, SimNode *)>   factory;
        };
        ManagedStructureAnnotation (const string & n, ModuleLibrary & ml )
            : TypeAnnotation(n), debugInfo(2048), helpA(debugInfo), mlib(&ml) {}
        virtual void seal( Module * m ) override {
            TypeAnnotation::seal(m);
            mlib = nullptr;
        }
        virtual bool rtti_isHandledTypeAnnotation() const override { return true; }
        virtual size_t getSizeOf() const override { return sizeof(ManagedType); }
        virtual size_t getAlignOf() const override { return alignof(ManagedType); }
        virtual bool isRefType() const override { return true; }
        virtual bool isNewable() const override { return true; }
        virtual bool isLocal() const override { return true; }
        virtual TypeDeclPtr makeFieldType ( const string & na ) const override {
            auto it = fields.find(na);
            if ( it!=fields.end() ) {
                auto t = make_shared<TypeDecl>(*it->second.decl);
                if ( it->second.offset != -1U ) {
                    t->ref = true;
                }
                return t;
            } else {
                return nullptr;
            }
        }
        virtual TypeDeclPtr makeSafeFieldType ( const string & na ) const override {
            auto it = fields.find(na);
            if ( it!=fields.end() && it->second.offset!=-1U ) {
                return make_shared<TypeDecl>(*it->second.decl);
            } else {
                return nullptr;
            }
        }
        virtual SimNode * simulateGetField ( const string & na, Context & context, const LineInfo & at, SimNode * value ) const override {
            auto it = fields.find(na);
            if ( it!=fields.end() ) {
                return it->second.factory(FactoryNodeType::getField,context,at,value);
            } else {
                return nullptr;
            }
        }
        virtual SimNode * simulateGetFieldR2V ( const string & na, Context & context, const LineInfo & at, SimNode * value ) const override {
            auto it = fields.find(na);
            if ( it!=fields.end() ) {
                auto itT = it->second.decl;
                return it->second.factory(FactoryNodeType::getFieldR2V,context,at,value);
            } else {
                return nullptr;
            }
        }
        virtual SimNode * simulateGetNew ( Context & context, const LineInfo & at ) const override {
            return context.code.makeNode<SimNode_New>(at,int32_t(sizeof(ManagedType)));
        }
        virtual SimNode * simulateSafeGetField ( const string & na, Context & context, const LineInfo & at, SimNode * value ) const override {
            auto it = fields.find(na);
            if ( it!=fields.end() ) {
                return it->second.factory(FactoryNodeType::safeGetField,context,at,value);
            } else {
                return nullptr;
            }
        };
        virtual SimNode * simulateSafeGetFieldPtr ( const string & na, Context & context, const LineInfo & at, SimNode * value ) const override {
            auto it = fields.find(na);
            if ( it!=fields.end() ) {
                return it->second.factory(FactoryNodeType::safeGetFieldPtr,context,at,value);
            } else {
                return nullptr;
            }
        };
        template <typename FunT, FunT PROP>
        void addProperty ( const string & na ) {
            auto & field = fields[na]; assert(!field.decl && "field already exist");
            using resultType = decltype((((ManagedType *)0)->*PROP)());
            field.decl = makeType<resultType>(*mlib);
            field.offset = -1U;
            field.factory = [](FactoryNodeType nt,Context & context,const LineInfo & at, SimNode * value) -> SimNode * {
                switch ( nt ) {
                    case FactoryNodeType::getField:
                        return context.code.makeNode<SimNode_Property<ManagedType,FunT,PROP,false>>(at, value);
                    case FactoryNodeType::safeGetField:
                    case FactoryNodeType::safeGetFieldPtr:
                    case FactoryNodeType::getFieldR2V:
                        assert(0 && "we should not be here");
                    default:
                        return nullptr;
                }
            };
        }
        void addFieldEx ( const string & na, off_t offset, TypeDeclPtr pT ) {
            auto & field = fields[na]; assert(!field.decl && "field already exist");
            field.decl = pT;
            field.offset = offset;
            auto baseType = field.decl->baseType;
            field.factory = [offset,baseType](FactoryNodeType nt,Context & context,const LineInfo & at, SimNode * value) -> SimNode * {
                switch ( nt ) {
                    case FactoryNodeType::getField:
                        return context.code.makeNode<SimNode_FieldDeref>(at,value,offset);
                    case FactoryNodeType::getFieldR2V:
                        return context.code.makeValueNode<SimNode_FieldDerefR2V>(baseType,at,value,offset);
                    case FactoryNodeType::safeGetField:
                        return context.code.makeNode<SimNode_SafeFieldDeref>(at,value,offset);
                    case FactoryNodeType::safeGetFieldPtr:
                        return context.code.makeNode<SimNode_SafeFieldDerefPtr>(at,value,offset);
                    default:
                        return nullptr;
                }
            };
        }
        template <typename TT, off_t off>
        __forceinline void addField ( const string & na ) {
            addFieldEx ( na, off, makeType<TT>(*mlib) );
        }
        virtual void walk ( DataWalker & walker, void * data ) override {
            if ( !sti ) {
                debugInfo.reset();
                sti = debugInfo.makeNode<StructInfo>();
                sti->name = debugInfo.allocateName(name);
                // count fields
                sti->fieldsSize = 0;
                for ( auto & fi : fields ) {
                    auto & var = fi.second;
                    if ( var.offset != -1U ) {
                        sti->fieldsSize ++;
                    }
                }
                // and allocate
                sti->size = (uint32_t) getSizeOf();
                sti->fields = (VarInfo **) debugInfo.allocate( sizeof(VarInfo *) * sti->fieldsSize );
                int i = 0;
                for ( auto & fi : fields ) {
                    auto & var = fi.second;
                    if ( var.offset != -1U ) {
                        VarInfo * vi = debugInfo.makeNode<VarInfo>();
                        helpA.makeTypeInfo(vi, var.decl);
                        vi->name = debugInfo.allocateName(fi.first);
                        vi->offset = var.offset;
                        sti->fields[i++] = vi;
                    }
                }
            }
            walker.walk_struct((char *)data, sti);
        }
        map<string,StructureField> fields;
        NodeAllocator              debugInfo;
        DebugInfoHelper            helpA;
        StructInfo *               sti = nullptr;
        ModuleLibrary *            mlib = nullptr;
    };

    template <typename OT>
    struct ManagedVectorAnnotation : TypeAnnotation {
        typedef vector<OT> VectorType;
        struct SimNode_VectorLength : SimNode {
            DAS_INT_NODE;
            SimNode_VectorLength ( const LineInfo & at, SimNode * rv )
                : SimNode(at), value(rv) {}
            __forceinline int32_t compute ( Context & context ) {
                auto pValue = (VectorType *) value->evalPtr(context);
                return int32_t(pValue->size());
            }
            SimNode * value;
        };
        struct SimNode_AtVector : SimNode_At {
            DAS_PTR_NODE;
            SimNode_AtVector ( const LineInfo & at, SimNode * rv, SimNode * idx )
                : SimNode_At(at, rv, idx, 0, 0) {}
            __forceinline char * compute ( Context & context ) {
                auto pValue = (VectorType *) value->evalPtr(context);
                uint32_t idx = cast<uint32_t>::to(index->eval(context));
                if ( idx >= pValue->size() ) {
                    context.throw_error("index out of range");
                    return nullptr;
                } else {
                    return (char *)(pValue->data() + idx);
                }
            }
        };
        struct VectorIterator : Iterator {
            virtual bool first ( Context & context, IteratorContext & itc ) override {
                vec4f ll = source->eval(context);
                VectorType * pArray = cast<VectorType *>::to(ll);
                char * data    = (char *) pArray->data();
                uint32_t size = (uint32_t) pArray->size();
                itc.value      = cast<char *>::from(data);
                itc.array_end  = data + size * sizeof(OT);
                itc.array      = nullptr;
                return (bool) size;
            }
            virtual bool next  ( Context &, IteratorContext & itc ) override {
                char * data = cast<char *>::to(itc.value) + sizeof(OT);
                itc.value = cast<char *>::from(data);
                return data != itc.array_end;
            }
            virtual void close ( Context &, IteratorContext & ) override {
            }
            SimNode *   source;
        };
        struct SimNode_VectorIterator : SimNode, VectorIterator {
            SimNode_VectorIterator ( const LineInfo & at, SimNode * s )
                : SimNode(at) { VectorIterator::source = s;}
            virtual vec4f eval ( Context & ) override {
                return cast<Iterator *>::from(static_cast<VectorIterator *>(this));
            }
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
        virtual TypeDeclPtr makeFieldType ( const string & na ) const override {
            if ( na=="length" ) return make_shared<TypeDecl>(Type::tInt);
            return nullptr;
        }
        virtual TypeDeclPtr makeIndexType ( TypeDeclPtr & ) const override { return make_shared<TypeDecl>(*vecType); }
        virtual TypeDeclPtr makeIteratorType () const override { return make_shared<TypeDecl>(*vecType); }
        virtual SimNode * simulateGetAt ( Context & context, const LineInfo & at, const TypeDeclPtr &, SimNode * rv, SimNode * idx ) const override {
            return context.code.makeNode<SimNode_AtVector>(at, rv, idx);
        }
        virtual SimNode * simulateGetIterator ( Context & context, const LineInfo & at, SimNode * rv ) const override {
            return context.code.makeNode<SimNode_VectorIterator>(at, rv);
        }
        virtual SimNode * simulateGetField ( const string & na, Context & context, const LineInfo & at, SimNode * value ) const override {
            if ( na=="length" ) return context.code.makeNode<SimNode_VectorLength>(at,value);
            return nullptr;
        }
        TypeDeclPtr vecType;
    };

    template <typename OT>
    struct ManagedValueAnnotation : TypeAnnotation {
        static_assert(sizeof(OT)<=sizeof(vec4f), "value types have to fit in ABI");
        ManagedValueAnnotation(const string & n) : TypeAnnotation(n) {}
        virtual bool rtti_isHandledTypeAnnotation() const override { return true; }
        virtual bool isLocal() const override { return true; }
        virtual size_t getSizeOf() const override { return sizeof(OT); }
        virtual bool isRefType() const override { return false; }
        virtual SimNode * simulateCopy ( Context & context, const LineInfo & at, SimNode * l, SimNode * r ) const override {
            return context.code.makeNode<SimNode_CopyValue<OT>>(at, l, r);
        }
        virtual SimNode * simulateRef2Value ( Context & context, const LineInfo & at, SimNode * l ) const override {
            return context.code.makeNode<SimNode_Ref2Value<OT>>(at, l);
        }
    };
}

