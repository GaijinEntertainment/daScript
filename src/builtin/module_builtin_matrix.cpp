#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"

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
        int GetField ( const string & na ) const {
            if ( na.length()!=1 )
                return -1;
            int field = TypeDecl::getMaskFieldIndex(na[0]);
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
        virtual size_t getAlignOf() const override {
            return alignof(vec4f);
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
                return context.makeNode<SimNode_FieldDeref>(at,value,uint32_t(field*sizeof(VecT)));
            } else {
                return nullptr;
            }
        }
        virtual SimNode * simulateGetFieldR2V ( const string & na, Context & context, const LineInfo & at, SimNode * value ) const override {
            int field = GetField(na);
            if ( field!=-1 ) {
                auto bt = TypeDecl::getVectorType(Type::tFloat, ColC);
                return context.makeValueNode<SimNode_FieldDerefR2V>(bt,at,value,uint32_t(field*sizeof(VecT)));
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
                return context.makeNode<SimNode_SafeFieldDeref>(at,value,uint32_t(field*sizeof(VecT)));
            } else {
                return nullptr;
            }
        };
        virtual SimNode * simulateSafeGetFieldPtr ( const string & na, Context & context, const LineInfo & at, SimNode * value ) const override {
            int field = GetField(na);
            if ( field!=-1 ) {
                return context.makeNode<SimNode_SafeFieldDerefPtr>(at,value,uint32_t(field*sizeof(VecT)));
            } else {
                return nullptr;
            }
        };
        virtual SimNode * simulateGetAt ( Context & context, const LineInfo & at, SimNode * rv, SimNode * idx ) const override {
            return context.makeNode<SimNode_At>(at, rv, idx, uint32_t(sizeof(float)*ColC), RowC);
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
    
    template <typename MatT>
    struct SimNode_MatrixCtor : SimNode_CallBase {
        SimNode_MatrixCtor(const LineInfo & at) : SimNode_CallBase(at) {}
        virtual vec4f eval(Context & context) override {
            assert(stackTop && "copy on return memory not allocated");
            auto cmres = context.stackTop + stackTop;
            memset ( cmres, 0, sizeof(MatT) );
            return cast<void *>::from(cmres);
        }
    };

    template <int r, int c>
    __forceinline void matrix_identity ( float * mat ) {
        for ( int y=0; y!=r; ++y ) {
            for ( int x=0; x!=c; ++x ) {
                *mat++ = x==y ? 1.0f : 0.0f;
            }
        }
    }

    void float4x4_identity ( float4x4 & mat ) {
        matrix_identity<4,4>((float*)&mat);
    }
    
    void float3x4_identity ( float3x4 & mat ) {
        matrix_identity<4,3>((float*)&mat);
    }
    
	float4x4 float_4x4_translation(float3 xyz) {
		float4x4 mat;
		matrix_identity<4,4>((float*)&mat);
		mat.m[3].x = xyz.x;
		mat.m[3].y = xyz.y;
		mat.m[3].z = xyz.z;
		return mat;
	}

    void Module_BuiltIn::addMatrixTypes(ModuleLibrary & lib) {
        // structure annotations
        addAnnotation(make_shared<float4x4_ann>());
        addAnnotation(make_shared<float3x4_ann>());
        // c-tor
        addFunction ( make_shared< BuiltInFn< SimNode_MatrixCtor<float3x4>,float3x4 > >("float3x4",lib) );
        addFunction ( make_shared< BuiltInFn< SimNode_MatrixCtor<float4x4>,float4x4 > >("float4x4",lib) );
        // 4x4
        addExtern<decltype(float4x4_identity),float4x4_identity>(*this, lib, "identity");
		addExtern<decltype(float_4x4_translation), float_4x4_translation, SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "translation");
        // 3x4
        addExtern<decltype(float3x4_identity),float3x4_identity>(*this, lib, "identity");
    }
}
