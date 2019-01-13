#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/ast/ast_interop.h"

namespace das {
    
    template <typename VecT, int rowC>
    struct Matrix {
        VecT    m[rowC];
    };

    template <typename VecT, int RowC>
    class MatrixAnnotation : public TypeAnnotation {
        enum { ColC = sizeof(VecT) / sizeof(float) };
        typedef MatrixAnnotation<VecT,RowC> ThisAnnotation;
        typedef Matrix<VecT,RowC> ThisMatrix;
    protected:
        int GetField ( const string & name ) const {
            if ( name.length()!=1 )
                return -1;
            int field = TypeDecl::getMaskFieldIndex(name[0]);
            if ( field<0 || field>=RowC )
                return -1;
            return field;
        }
    public:
        MatrixAnnotation()
            : TypeAnnotation( "float" + std::to_string(ColC) + "x" + std::to_string(RowC) ) {}
        virtual TypeAnnotationPtr clone ( const TypeAnnotationPtr & p = nullptr ) const override {
            shared_ptr<ThisAnnotation> cp =  p ? static_pointer_cast<ThisAnnotation>(p) : make_shared<ThisAnnotation>();
            return TypeAnnotation::clone(cp);
        }
        virtual bool rtti_isHandledTypeAnnotation() const override {
            return true;
        }
        virtual bool isRefType() const override {
            return true;
        }
        virtual bool isLocal() const override {
            return true;
        }
        virtual bool isNewable() const override {
            return true;
        }
        virtual bool isIndexable ( const TypeDeclPtr & decl ) const override {
            return decl->isIndex();
        }
        virtual size_t getSizeOf() const override {
            return sizeof(ThisMatrix);
        }
        virtual TypeDeclPtr makeFieldType ( const string & na ) const override {
            if ( auto ft = makeSafeFieldType(na) ) {
                ft->ref = true;
                return ft;
            } else {
                return nullptr;
            }
        }
        virtual TypeDeclPtr makeSafeFieldType ( const string & na ) const override {
            int field = GetField(na);
            if ( field<0  )
                return nullptr;
            auto bt = TypeDecl::getVectorType(Type::tFloat, ColC);
            return make_shared<TypeDecl>(bt);
        }
        virtual TypeDeclPtr makeIndexType ( TypeDeclPtr & decl ) const override {
            if ( !decl->isIndex() ) return nullptr;
            auto bt = TypeDecl::getVectorType(Type::tFloat, ColC);
            auto pt = make_shared<TypeDecl>(bt);
            pt->ref = true;
            return pt;
        }
        virtual SimNode * simulateCopy ( Context & context, const LineInfo & at, SimNode * l, SimNode * r ) const override {
            return context.makeNode<SimNode_CopyValue<ThisMatrix>>(at, l, r);
        }
        virtual SimNode * simulateGetField ( const string & na, Context & context, const LineInfo & at, SimNode * value ) const override {
            int field = GetField(na);
            if ( field!=-1 ) {
                return context.makeNode<SimNode_FieldDeref>(at,value,field*sizeof(VecT));
            } else {
                return nullptr;
            }
        }
        virtual SimNode * simulateGetFieldR2V ( const string & na, Context & context, const LineInfo & at, SimNode * value ) const override {
            int field = GetField(na);
            if ( field!=-1 ) {
                auto bt = TypeDecl::getVectorType(Type::tFloat, ColC);
                return context.makeValueNode<SimNode_FieldDerefR2V>(bt,at,value,field*sizeof(VecT));
            } else {
                return nullptr;
            }
        }
        virtual SimNode * simulateGetNew ( Context & context, const LineInfo & at ) const override {
            return context.makeNode<SimNode_New>(at,int32_t(sizeof(ThisMatrix)));
        }
        virtual SimNode * simulateSafeGetField ( const string & na, Context & context, const LineInfo & at, SimNode * value ) const override {
            int field = GetField(na);
            if ( field!=-1 ) {
                return context.makeNode<SimNode_SafeFieldDeref>(at,value,field*sizeof(VecT));
            } else {
                return nullptr;
            }
        };
        virtual SimNode * simulateSafeGetFieldPtr ( const string & na, Context & context, const LineInfo & at, SimNode * value ) const override {
            int field = GetField(na);
            if ( field!=-1 ) {
                return context.makeNode<SimNode_SafeFieldDerefPtr>(at,value,field*sizeof(VecT));
            } else {
                return nullptr;
            }
        };
        virtual SimNode * simulateGetAt ( Context & context, const LineInfo & at, SimNode * rv, SimNode * idx ) const override {
            return context.makeNode<SimNode_At>(at, rv, idx, sizeof(float)*ColC, RowC);
        }
        virtual void debug ( stringstream & ss, void * data, PrintFlags ) const override {
            auto pM = (ThisMatrix *) data;
            for ( int i=0; i!=RowC; ++i ) {
                if ( i ) ss << ", ";
                ss << pM->m[i];
            }
        }
        virtual void debug ( stringstream & ss, vec4f pV, PrintFlags flags ) const override {
            debug(ss,cast<void *>::to(pV),flags);
        }
    };
    
    typedef Matrix<float4,4> float4x4;
    typedef Matrix<float3,4> float3x4;
    
    typedef MatrixAnnotation<float4,4> float4x4_ann;
    typedef MatrixAnnotation<float3,4> float3x4_ann;
    
    MAKE_TYPE_FACTORY(float4x4, float4x4)
    MAKE_TYPE_FACTORY(float3x4, float3x4)

    __forceinline void matrix_identity ( float * mat, int r, int c ) {
        for ( int y=0; y!=r; ++y ) {
            for ( int x=0; x!=c; ++x ) {
                *mat++ = x==y ? 1.0f : 0.0f;
            }
        }
    }

    void float4x4_identity ( float4x4 & mat ) {
        matrix_identity((float*)&mat, 4, 4);
    }
    
    void float3x4_identity ( float3x4 & mat ) {
        matrix_identity((float*)&mat, 4, 3);
    }
    
    void Module_BuiltIn::addMatrixTypes(ModuleLibrary & lib) {
        // structure annotations
        addAnnotation(make_shared<float4x4_ann>());
        addAnnotation(make_shared<float3x4_ann>());
        // identity
        addExtern<decltype(float4x4_identity),float4x4_identity>(*this, lib, "identity");
        addExtern<decltype(float3x4_identity),float3x4_identity>(*this, lib, "identity");
    }
}
