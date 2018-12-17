#pragma once

#include "ast.h"

namespace yzg
{
    using namespace std;
    
    template <typename OT>
    struct StructureTypeAnnotation : TypeAnnotation {
        struct StructureField {
            string      name;
            TypeDeclPtr decl;
            uint32_t    offset;
        };
        StructureTypeAnnotation(const string & n) : TypeAnnotation(n) {}
        virtual size_t getSizeOf() const override { return sizeof(OT); }
        virtual bool isRefType() const override { return true; }
        virtual bool isNullable() const override { return true; }
        virtual bool isNewable() const override { return true; }
        virtual bool isLocal() const override { return true; }
        virtual TypeDecl * getField ( const string & name ) const override {
            auto it = fields.find(name);
            return it!=fields.end() ? it->second.decl.get() : nullptr;
        }
        virtual SimNode * simulateGetField ( const string & name, Context & context, const LineInfo & at, SimNode * value ) const override {
            auto it = fields.find(name);
            if ( it!=fields.end() ) {
                return context.makeNode<SimNode_FieldDeref>(at,value,it->second.offset);
            } else {
                return nullptr;
            }
        }
        virtual SimNode * simulateGetNew ( Context & context, const LineInfo & at ) const override {
            return context.makeNode<SimNode_New>(at,sizeof(OT));
        }
        virtual SimNode * simulateSafeGetField ( const string & name, Context & context, const LineInfo & at, SimNode * value ) const override {
            auto it = fields.find(name);
            if ( it!=fields.end() ) {
                return context.makeNode<SimNode_SafeFieldDeref>(at,value,it->second.offset);
            } else {
                return nullptr;
            }
        };
        virtual SimNode * simulateSafeGetFieldPtr ( const string & name, Context & context, const LineInfo & at, SimNode * value ) const override {
            auto it = fields.find(name);
            if ( it!=fields.end() ) {
                return context.makeNode<SimNode_SafeFieldDerefPtr>(at,value,it->second.offset);
            } else {
                return nullptr;
            }
        };
        void addField ( const string & name, off_t offset, TypeDeclPtr pT ) {
            auto & field = fields[name]; assert(!field.decl && "field already exist");
            field.offset = offset;
            field.decl = pT;
        }
        map<string,StructureField> fields;
    };
    
    template <typename OT>
    struct VectorTypeAnnotation : TypeAnnotation {
        typedef vector<OT> VectorType;
        struct SimNode_AtVector : SimNode_At {
            SimNode_AtVector ( const LineInfo & at, SimNode * rv, SimNode * idx )
            : SimNode_At(at, rv, idx, 0, 0) {}
            virtual __m128 eval ( Context & context ) override {
                VectorType * pValue = cast<VectorType *>::to(value->eval(context));
                YZG_EXCEPTION_POINT;
                uint32_t idx = cast<uint32_t>::to(index->eval(context));
                YZG_EXCEPTION_POINT;
                if ( idx >= pValue->size() ) {
                    context.throw_error("index out of range");
                    return _mm_setzero_ps();
                } else {
                    return cast<char *>::from((char *)(pValue->data() + idx));
                }
            }
        };
        struct VectorIterator : Iterator {
            virtual bool first ( Context & context, IteratorContext & itc ) override {
                __m128 ll = source->eval(context);
                YZG_ITERATOR_EXCEPTION_POINT;
                VectorType * pArray = cast<VectorType *>::to(ll);
                YZG_ITERATOR_EXCEPTION_POINT;
                char * data    = (char *) pArray->data();
                uint32_t size = pArray->size();
                itc.value      = cast<char *>::from(data);
                itc.array_end  = data + size * sizeof(OT);
                itc.array      = nullptr;
                return size != 0;
            }
            virtual bool next  ( Context & context, IteratorContext & itc ) override {
                char * data = cast<char *>::to(itc.value) + sizeof(OT);
                itc.value = cast<char *>::from(data);
                return data != itc.array_end;
            }
            virtual void close ( Context & context, IteratorContext & itc ) override {
            }
            SimNode *   source;
        };
        struct SimNode_VectorIterator : SimNode, VectorIterator {
            SimNode_VectorIterator ( const LineInfo & at, SimNode * s )
            : SimNode(at) { VectorIterator::source = s;}
            virtual __m128 eval ( Context & context ) override {
                return cast<Iterator *>::from(static_cast<VectorIterator *>(this));
            }
        };
        VectorTypeAnnotation(const string & n, const TypeDeclPtr & d)
        : TypeAnnotation(n), vecType(d) {
            vecType->ref = true;
        }
        virtual size_t getSizeOf() const override { return sizeof(VectorType); }
        virtual bool isRefType() const override { return true; }
        virtual bool isNullable() const override { return true; }
        virtual bool isIndexable ( TypeDecl * indexType ) const override { return indexType->isIndex(); }
        virtual bool isIterable ( ) const override { return true; }
        virtual TypeDecl * getIndex ( TypeDecl * ) const override { return vecType.get(); }
        virtual TypeDecl * getIterator () const override { return vecType.get(); }
        virtual TypeDecl * getField ( const string & ) const override { return vecType.get(); }
        virtual SimNode * simulateGetAt ( Context & context, const LineInfo & at, SimNode * rv, SimNode * idx ) const override {
            return context.makeNode<SimNode_AtVector>(at, rv, idx);
        }
        virtual SimNode * simulateGetIterator ( Context & context, const LineInfo & at, SimNode * rv ) const override {
            return context.makeNode<SimNode_VectorIterator>(at, rv);
        }
        TypeDeclPtr vecType;
    };
}

