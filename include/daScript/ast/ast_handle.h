#pragma once

#include "daScript/ast/ast.h"

namespace das
{
    using namespace std;
    
    template <typename OT>
    struct ManagedStructureAnnotation  : TypeAnnotation {
        struct StructureField {
            string      name;
            TypeDeclPtr decl;
            uint32_t    offset;
        };
        ManagedStructureAnnotation (const string & n) : TypeAnnotation(n) {}
        virtual bool rtti_isHandledTypeAnnotation() const override { return true; }
        virtual size_t getSizeOf() const override { return sizeof(OT); }
        virtual size_t getAlignOf() const override { return alignof(OT); }
        virtual bool isRefType() const override { return true; }
        virtual bool isNewable() const override { return true; }
        virtual bool isLocal() const override { return true; }
        virtual TypeDeclPtr makeFieldType ( const string & na ) const override {
            if ( auto t = makeSafeFieldType(na) ) {
                t->ref = true;
                return t;
            } else {
                return nullptr;
            }
        }
        virtual TypeDeclPtr makeSafeFieldType ( const string & na ) const override {
            auto it = fields.find(na);
            return it!=fields.end() ? make_shared<TypeDecl>(*it->second.decl) : nullptr;
        }
        virtual SimNode * simulateGetField ( const string & na, Context & context, const LineInfo & at, SimNode * value ) const override {
            auto it = fields.find(na);
            if ( it!=fields.end() ) {
                return context.code.makeNode<SimNode_FieldDeref>(at,value,it->second.offset);
            } else {
                return nullptr;
            }
        }
        virtual SimNode * simulateGetFieldR2V ( const string & na, Context & context, const LineInfo & at, SimNode * value ) const override {
            auto it = fields.find(na);
            if ( it!=fields.end() ) {
                auto itT = it->second.decl;
                return context.code.makeValueNode<SimNode_FieldDerefR2V>(itT->baseType,at,value,it->second.offset);
            } else {
                return nullptr;
            }
        }
        virtual SimNode * simulateGetNew ( Context & context, const LineInfo & at ) const override {
            return context.code.makeNode<SimNode_New>(at,int32_t(sizeof(OT)));
        }
        virtual SimNode * simulateSafeGetField ( const string & na, Context & context, const LineInfo & at, SimNode * value ) const override {
            auto it = fields.find(na);
            if ( it!=fields.end() ) {
                return context.code.makeNode<SimNode_SafeFieldDeref>(at,value,it->second.offset);
            } else {
                return nullptr;
            }
        };
        virtual SimNode * simulateSafeGetFieldPtr ( const string & na, Context & context, const LineInfo & at, SimNode * value ) const override {
            auto it = fields.find(na);
            if ( it!=fields.end() ) {
                return context.code.makeNode<SimNode_SafeFieldDerefPtr>(at,value,it->second.offset);
            } else {
                return nullptr;
            }
        };
        void addField ( const string & na, off_t offset, TypeDeclPtr pT ) {
            auto & field = fields[na]; assert(!field.decl && "field already exist");
            field.offset = offset;
            field.decl = pT;
        }
        map<string,StructureField> fields;
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
                DAS_INT_EXCEPTION_POINT;
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
                DAS_PTR_EXCEPTION_POINT;
                uint32_t idx = cast<uint32_t>::to(index->eval(context));
                DAS_PTR_EXCEPTION_POINT;
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
                DAS_ITERATOR_EXCEPTION_POINT;
                VectorType * pArray = cast<VectorType *>::to(ll);
                DAS_ITERATOR_EXCEPTION_POINT;
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
        ManagedVectorAnnotation(const string & n, const TypeDeclPtr & d)
            : TypeAnnotation(n), vecType(d) {
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
        virtual SimNode * simulateGetAt ( Context & context, const LineInfo & at, SimNode * rv, SimNode * idx ) const override {
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
        virtual void debug ( stringstream & ss, void * data, PrintFlags ) const override {
            ss << (* (OT*)data);
        }
        virtual void debug ( stringstream & ss, vec4f data, PrintFlags ) const override {
            ss << cast<OT>::to(data);
        }
    };
}

