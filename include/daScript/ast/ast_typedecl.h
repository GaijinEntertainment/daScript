#pragma once

namespace das {

    struct TypeDecl;
    typedef shared_ptr<TypeDecl> TypeDeclPtr;

    class Structure;
    typedef shared_ptr<Structure> StructurePtr;

    class Enumeration;
    typedef shared_ptr<Enumeration> EnumerationPtr;

    struct Annotation;
    typedef shared_ptr<Annotation> AnnotationPtr;

    struct TypeAnnotation;
    typedef shared_ptr<TypeAnnotation> TypeAnnotationPtr;

    struct Expression;
    typedef shared_ptr<Expression> ExpressionPtr;

    class Visitor;

    class Module;
    class ModuleLibrary;
    class ModuleGroup;

    struct TypeDecl : enable_shared_from_this<TypeDecl> {
        enum {
            dimAuto = -1,
            dimConst = -2,
        };
        TypeDecl() = default;
        TypeDecl(const TypeDecl & decl);
        TypeDecl & operator = (const TypeDecl & decl) = delete;
        TypeDecl(Type tt) : baseType(tt) {}
        TypeDecl(const StructurePtr & sp) : baseType(Type::tStructure), structType(sp.get()) {}
        TypeDecl(const EnumerationPtr & ep) : baseType(Type::tEnumeration), enumType(ep) {}
        TypeDeclPtr visit ( Visitor & vis );
        friend TextWriter& operator<< (TextWriter& stream, const TypeDecl & decl);
        string getMangledName() const;
        bool canAot() const;
        bool canAot( set<Structure *> & recAot ) const;
        bool isSameType ( const TypeDecl & decl, bool refMatters = true, bool constMatters = true, bool topLevel = true ) const;
        bool isExprType() const;
        bool isIteratorType ( const TypeDecl & decl ) const;
        bool isSimpleType () const;
        bool isSimpleType ( Type typ ) const;
        bool isArray() const;
        bool isGoodIteratorType() const;
        bool isGoodArrayType() const;
        bool isGoodTableType() const;
        bool isGoodBlockType() const;
        bool isGoodFunctionType() const;
        bool isGoodLambdaType() const;
        bool isGoodTupleType() const;
        bool isVoid() const;
        bool isRef() const;
        bool isRefType() const;
        bool isIndex() const;
        bool isPointer() const;
        bool isEnum() const;
        bool isHandle() const;
        bool isStructure() const;
        bool isTuple() const;
        int getSizeOf() const;
        int getCountOf() const;
        int getAlignOf() const;
        int getBaseSizeOf() const;
        int getStride() const;
        int getTupleSize() const;
        int getTupleAlign() const;
        int getTupleFieldOffset ( int index ) const;
        string describe ( bool extra = true, bool contracts = true ) const;
        bool canCopy() const;
        bool canMove() const;
        bool canClone() const;
        bool canDelete() const;
        bool isPod() const;
        bool isRawPod() const;
        bool isWorkhorseType() const; // we can return this, or pass this
        bool isPolicyType() const;
        bool isVecPolicyType() const;
        bool isReturnType() const;
        bool isCtorType() const;
        bool isRange() const;
        bool isConst() const;
        bool isFoldable() const;
        bool isAlias() const;
        bool isAuto() const;
        bool isVectorType() const;
        Type getVectorBaseType() const;
        int getVectorDim() const;
        bool canInitWithZero() const;
        static Type getVectorType ( Type baseType, int dim );
        static int getMaskFieldIndex ( char ch );
        static bool isSequencialMask ( const vector<uint8_t> & fields );
        static bool buildSwizzleMask ( const string & mask, int dim, vector<uint8_t> & fields );
        static TypeDeclPtr inferAutoType ( TypeDeclPtr autoT, TypeDeclPtr initT );
        static void applyAutoContracts ( TypeDeclPtr TT, TypeDeclPtr autoT );
        Type getRangeBaseType() const;
        const TypeDecl * findAlias ( const string & name, bool allowAuto = false ) const;
        Type                    baseType = Type::tVoid;
        Structure *             structType = nullptr;
        EnumerationPtr          enumType;
        TypeAnnotationPtr       annotation;
        ExpressionPtr           declTypeExpr;
        TypeDeclPtr             firstType;      // map.first or array, or pointer
        TypeDeclPtr             secondType;     // map.second
        vector<TypeDeclPtr>     argTypes;       // block arguments
        vector<int32_t>         dim;
        vector<ExpressionPtr>   dimExpr;
        union {
            struct {
                bool    ref : 1 ;
                bool    constant : 1;
                bool    removeRef : 1;
                bool    removeConstant : 1;
                bool    aotAlias : 1;
            };
            uint32_t flags = 0;
        };
        string              alias;
        LineInfo            at;
        Module *            module = nullptr;
    };

    template <typename TT> struct ToBasicType;
    template<> struct ToBasicType<EnumStub>     { enum { type = Type::tEnumeration }; };
    template<> struct ToBasicType<Iterator *>   { enum { type = Type::tIterator }; };
    template<> struct ToBasicType<void *>       { enum { type = Type::tPointer }; };
    template<> struct ToBasicType<char *>       { enum { type = Type::tString }; };
    template<> struct ToBasicType<bool>         { enum { type = Type::tBool }; };
    template<> struct ToBasicType<int64_t>      { enum { type = Type::tInt64 }; };
    template<> struct ToBasicType<uint64_t>     { enum { type = Type::tUInt64 }; };
    template<> struct ToBasicType<int8_t>       { enum { type = Type::tInt8 }; };
    template<> struct ToBasicType<uint8_t>      { enum { type = Type::tUInt8 }; };
    template<> struct ToBasicType<int16_t>      { enum { type = Type::tInt16 }; };
    template<> struct ToBasicType<uint16_t>     { enum { type = Type::tUInt16 }; };
    template<> struct ToBasicType<int32_t>      { enum { type = Type::tInt }; };
    template<> struct ToBasicType<uint32_t>     { enum { type = Type::tUInt }; };
    template<> struct ToBasicType<float>        { enum { type = Type::tFloat }; };
    template<> struct ToBasicType<void>         { enum { type = Type::tVoid }; };
    template<> struct ToBasicType<float2>       { enum { type = Type::tFloat2 }; };
    template<> struct ToBasicType<float3>       { enum { type = Type::tFloat3 }; };
    template<> struct ToBasicType<float4>       { enum { type = Type::tFloat4 }; };
    template<> struct ToBasicType<int2>         { enum { type = Type::tInt2 }; };
    template<> struct ToBasicType<int3>         { enum { type = Type::tInt3 }; };
    template<> struct ToBasicType<int4>         { enum { type = Type::tInt4 }; };
    template<> struct ToBasicType<uint2>        { enum { type = Type::tUInt2 }; };
    template<> struct ToBasicType<uint3>        { enum { type = Type::tUInt3 }; };
    template<> struct ToBasicType<uint4>        { enum { type = Type::tUInt4 }; };
    template<> struct ToBasicType<double>       { enum { type = Type::tDouble }; };
    template<> struct ToBasicType<range>        { enum { type = Type::tRange }; };
    template<> struct ToBasicType<urange>       { enum { type = Type::tURange }; };
    template<> struct ToBasicType<Array *>      { enum { type = Type::tArray }; };
    template<> struct ToBasicType<Table *>      { enum { type = Type::tTable }; };
    template<> struct ToBasicType<Array>        { enum { type = Type::tArray }; };
    template<> struct ToBasicType<Table>        { enum { type = Type::tTable }; };
    template<> struct ToBasicType<Block>        { enum { type = Type::tBlock }; };
    template<> struct ToBasicType<Func>         { enum { type = Type::tFunction }; };
    template<> struct ToBasicType<Lambda>       { enum { type = Type::tLambda }; };
    template<> struct ToBasicType<Tuple>        { enum { type = Type::tTuple }; };
    template<> struct ToBasicType<Context *>    { enum { type = Type::fakeContext }; };
    template<> struct ToBasicType<vec4f>        { enum { type = Type::anyArgument }; };

    template <typename TT>
    struct typeFactory {
        static TypeDeclPtr make(const ModuleLibrary &) {
            auto t = make_shared<TypeDecl>();
            t->baseType = Type( ToBasicType<TT>::type );
            t->constant = is_const<TT>::value;
            return t;
        }
    };

    template <>
    struct typeFactory<char *> {
        static TypeDeclPtr make(const ModuleLibrary &) {
            auto t = make_shared<TypeDecl>(Type::tString);
            return t;
        }
    };

    template <>
    struct typeFactory<const char *> {
        static TypeDeclPtr make(const ModuleLibrary &) {
            auto t = make_shared<TypeDecl>(Type::tString);
            t->constant = true;
            return t;
        }
    };

    template <>
    struct typeFactory<Array *> {
        static TypeDeclPtr make(const ModuleLibrary &) {
            auto t = make_shared<TypeDecl>(Type::tArray);
            return t;
        }
    };

    template <>
    struct typeFactory<Iterator *> {
        static TypeDeclPtr make(const ModuleLibrary &) {
            auto t = make_shared<TypeDecl>(Type::tIterator);
            return t;
        }
    };

    template <>
    struct typeFactory<Table *> {
        static TypeDeclPtr make(const ModuleLibrary &) {
            auto t = make_shared<TypeDecl>(Type::tTable);
            return t;
        }
    };

    template <>
    struct typeFactory<Context *> {
        static TypeDeclPtr make(const ModuleLibrary &) {
            return make_shared<TypeDecl>(Type::fakeContext);
        }
    };

    template <>
    struct typeFactory<Block *> {
        static TypeDeclPtr make(const ModuleLibrary &) {
            auto t = make_shared<TypeDecl>(Type::tBlock);
            return t;
        }
    };

    template <typename TT, int dim>
    struct typeFactory<TT[dim]> {
        static TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = typeFactory<TT>::make(lib);
            t->dim.push_back(dim);
            t->ref = false;
            return t;
        }
    };

    template <typename TT>
    struct typeFactory<TT *> {
        static TypeDeclPtr make(const ModuleLibrary & lib) {
            auto pt = make_shared<TypeDecl>(Type::tPointer);
            if ( !is_void<TT>::value ) {
                pt->firstType = typeFactory<TT>::make(lib);
            }
            return pt;
        }
    };

    template <typename TT>
    struct typeFactory<const TT *> {
        static TypeDeclPtr make(const ModuleLibrary & lib) {
            auto pt = make_shared<TypeDecl>(Type::tPointer);
            if ( !is_void<TT>::value ) {
                pt->firstType = typeFactory<TT>::make(lib);
                pt->firstType->constant = true;
            }
            pt->constant = true;
            return pt;
        }
    };

    template <typename TT>
    struct typeFactory<TT &> {
        static TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = typeFactory<TT>::make(lib);
            t->ref = true;
            return t;
        }
    };

    template <typename TT>
    struct typeFactory<const TT &> {
        static TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = typeFactory<TT>::make(lib);
            t->ref = true;
            t->constant = true;
            return t;
        }
    };

    template <typename TT>
    struct typeFactory<const TT> {
        static TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = typeFactory<TT>::make(lib);
            t->constant = true;
            return t;
        }
    };

    template <typename TT>
    inline TypeDeclPtr makeType(const ModuleLibrary & ctx) {
        return typeFactory<TT>::make(ctx);
    }

    das::TypeDeclPtr makeHandleType(const das::ModuleLibrary & library, const char * typeName);

#define MAKE_TYPE_FACTORY(TYPE,CTYPE)                                       \
    template <>                                                             \
    struct das::typeFactory<CTYPE> {                                        \
        static das::TypeDeclPtr make(const das::ModuleLibrary & library ) { \
            return makeHandleType(library,#TYPE);                           \
        }                                                                   \
    };                                                                      \
    template <>                                                             \
    struct das::typeName<CTYPE> {                                           \
        static string name() { return #CTYPE; }                             \
    };

    bool splitTypeName ( const string & name, string & moduleName, string & funcName );

    string describeCppType(const TypeDeclPtr & type, bool substituteRef = false, bool skipRef = false, bool skipConst = false);

}
