#pragma once

#include "daScript/ast/ast_typefactory.h"
#include "daScript/misc/free_list.h"

namespace das {

    struct TypeDecl;
    typedef smart_ptr<TypeDecl> TypeDeclPtr;

    class Structure;
    typedef smart_ptr<Structure> StructurePtr;

    class Enumeration;
    typedef smart_ptr<Enumeration> EnumerationPtr;

    struct Annotation;
    typedef smart_ptr<Annotation> AnnotationPtr;

    struct TypeAnnotation;
    typedef smart_ptr<TypeAnnotation> TypeAnnotationPtr;

    struct Expression;
    typedef smart_ptr<Expression> ExpressionPtr;

    typedef das_map<string,TypeDeclPtr> AliasMap;
    typedef das_map<TypeDecl *,int> OptionsMap;

    class Visitor;

    class Module;
    class ModuleLibrary;
    class ModuleGroup;

    struct TypeDecl : ptr_ref_count {
        enum {
            dimAuto = -1,
            dimConst = -2,
        };
        enum class DescribeExtra     { no, yes };
        enum class DescribeContracts { no, yes };
        enum class DescribeModule    { no, yes };
        enum {
            gcFlag_heap = (1<<0),
            gcFlag_stringHeap = (1<<1)
        };
        TypeDecl() = default;
        TypeDecl(const TypeDecl & decl);
        TypeDecl & operator = (const TypeDecl & decl) = delete;
        TypeDecl(Type tt) : baseType(tt) {}
        TypeDecl(Structure * sp) : baseType(Type::tStructure), structType(sp) {}
        TypeDecl(const StructurePtr & sp) : baseType(Type::tStructure), structType(sp.get()) {}
        TypeDecl(const EnumerationPtr & ep);
        TypeDeclPtr visit ( Visitor & vis );
        friend TextWriter& operator<< (TextWriter& stream, const TypeDecl & decl);
        string getMangledName ( bool fullName=false ) const;
        void getMangledName ( FixedBufferTextWriter & tw, bool fullName=false ) const;
        bool canAot() const;
        bool canAot( das_set<Structure *> & recAot ) const;
        bool isSameType ( const TypeDecl & decl, RefMatters refMatters, ConstMatters constMatters,
            TemporaryMatters temporaryMatters, AllowSubstitute allowSubstitute = AllowSubstitute::no, bool topLevel = true, bool isPassType = false ) const;
        bool isSameExactType ( const TypeDecl & decl ) const;
        void sanitize();
        bool isExprType() const;
        bool isExprTypeAnywhere() const;
        bool isExprTypeAnywhere(das_set<Structure*> & dep) const;
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
        bool isGoodVariantType() const;
        bool isVoid() const;
        bool isRef() const;
        bool isRefType() const;
        bool isRefOrPointer() const { return isRef() || isPointer(); }
        bool canWrite() const;
        bool isTemp( bool topLevel = true, bool refMatters = true) const;
        bool isTemp(bool topLevel, bool refMatters, das_set<Structure*> & dep) const;
        bool isTempType(bool refMatters = true) const;
        bool isFullyInferred(das_set<Structure*> & dep) const;
        bool isFullyInferred() const;
        bool isShareable(das_set<Structure*> & dep) const;
        bool isShareable() const;
        bool isIndex() const;
        bool isBool() const;
        bool isInteger() const;
        bool isSignedInteger() const;
        bool isUnsignedInteger() const;
        bool isSignedIntegerOrIntVec() const;
        bool isUnsignedIntegerOrIntVec() const;
        bool isFloatOrDouble() const;
        bool isNumeric() const;
        bool isNumericStorage() const;
        bool isNumericComparable() const;
        bool isPointer() const;
        bool isVoidPointer() const;
        bool isIterator() const;
        bool isLambda() const;
        bool isEnum() const;
        bool isEnumT() const;
        bool isHandle() const;
        bool isStructure() const;
        bool isClass() const;
        bool isFunction() const;
        bool isTuple() const;
        bool isVariant() const;
        bool isMoveableValue() const;
        int getSizeOf() const;
        int getCountOf() const;
        int getAlignOf() const;
        int getBaseSizeOf() const;
        int getStride() const;
        int getTupleSize() const;
        int getTupleAlign() const;
        int getTupleFieldOffset ( int index ) const;
        int getVariantSize() const;
        int getVariantAlign() const;
        int getVariantFieldOffset ( int index ) const;
        int getVariantUniqueFieldIndex ( const TypeDeclPtr & uniqueType ) const;
        string describe ( DescribeExtra extra = DescribeExtra::yes, DescribeContracts contracts = DescribeContracts::yes, DescribeModule module = DescribeModule::yes) const;
        __forceinline bool canCopy() const { return canCopy(false); }
        bool canCopy(bool tempMatters) const;
        bool canMove() const;
        bool canClone() const;
        bool canNew() const;
        bool canDeletePtr() const;
        bool canDelete() const;
        bool needDelete() const;
        bool isPod() const;
        bool isRawPod() const;
        bool isNoHeapType() const;
        bool isWorkhorseType() const; // we can return this, or pass this
        bool isPolicyType() const;
        bool isVecPolicyType() const;
        bool isReturnType() const;
        bool isCtorType() const;
        bool isRange() const;
        bool isString() const;
        bool isConst() const;
        bool isFoldable() const;
        void collectAliasList(vector<string> & aliases) const;
        bool isAutoArrayResolved() const;
        bool isAuto() const;
        bool isAutoOrAlias() const;
        bool isAotAlias () const;
        bool isAlias() const;
        bool isAliasOrExpr() const;
        bool isVectorType() const;
        bool isBitfield() const;
        bool isLocal() const;
        bool isLocal( das_set<Structure*> & dep ) const;
        bool hasClasses() const;
        bool hasClasses( das_set<Structure*> & dep ) const;
        int32_t gcFlags() const;
        int32_t gcFlags( das_set<Structure *> & dep, das_set<Annotation *> & depA ) const;
        bool lockCheck() const;
        bool lockCheck( das_set<Structure *> & dep ) const;
        bool hasNonTrivialCtor() const;
        bool hasNonTrivialCtor( das_set<Structure*> & dep ) const;
        bool hasNonTrivialDtor() const;
        bool hasNonTrivialDtor( das_set<Structure*> & dep ) const;
        bool hasNonTrivialCopy() const;
        bool hasNonTrivialCopy( das_set<Structure*> & dep ) const;
        bool canBePlacedInContainer() const;
        bool canBePlacedInContainer( das_set<Structure*> & dep ) const;
        Annotation * isPointerToAnnotation() const;
        Type getVectorBaseType() const;
        int getVectorDim() const;
        bool canInitWithZero() const;
        static Type getVectorType ( Type baseType, int dim );
        static int getMaskFieldIndex ( char ch );
        static bool isSequencialMask ( const vector<uint8_t> & fields );
        static bool buildSwizzleMask ( const string & mask, int dim, vector<uint8_t> & fields );
        static TypeDeclPtr inferGenericType ( TypeDeclPtr autoT, TypeDeclPtr initT, bool topLevel = false, OptionsMap * options = nullptr );
        static TypeDeclPtr inferGenericInitType ( TypeDeclPtr autoT, TypeDeclPtr initT );
        static void applyAutoContracts ( TypeDeclPtr TT, TypeDeclPtr autoT );
        static void updateAliasMap ( const TypeDeclPtr & decl, const TypeDeclPtr & pass, AliasMap & aliases, OptionsMap & options );
        Type getRangeBaseType() const;
        TypeDecl * findAlias ( const string & name, bool allowAuto = false );
        int findArgumentIndex(const string & name) const;
        void addVariant(const string & name, const TypeDeclPtr & tt);
        string findBitfieldName ( uint32_t value ) const;
        Type                    baseType = Type::tVoid;
        Structure *             structType = nullptr;
        Enumeration *           enumType = nullptr;
        TypeAnnotation *        annotation = nullptr;
        TypeDeclPtr             firstType;      // map.first or array, or pointer
        TypeDeclPtr             secondType;     // map.second
        vector<TypeDeclPtr>     argTypes;       // block arguments
        vector<string>          argNames;
        vector<int32_t>         dim;
        vector<ExpressionPtr>   dimExpr;
        union {
            struct {
                bool    ref : 1 ;
                bool    constant : 1;
                bool    temporary : 1;
                bool    implicit : 1;
                bool    removeRef : 1;
                bool    removeConstant : 1;
                bool    removeDim : 1;
                bool    removeTemporary : 1;
                bool    explicitConst : 1;
                bool    aotAlias : 1;
                bool    smartPtr : 1;
                bool    smartPtrNative : 1;
                bool    isExplicit : 1;
                bool    isNativeDim : 1;
                bool    isTag: 1;
            };
            uint32_t flags = 0;
        };
        string              alias;
        LineInfo            at;
        Module *            module = nullptr;
    };

    template <typename TT> struct ToBasicType {
        enum { type = Type::none };
        static_assert( int(type)!=int(Type::none),"This type is not supported or not bound. For the bound type missing or not included are "
            "MAKE_TYPE_FACTORY or MAKE_EXTERNAL_TYPE_FACTORY as well as addAnnotation in the appropriate module.");
    };

    template<> struct ToBasicType<Bitfield>     { enum { type = Type::tBitfield }; };
    template<> struct ToBasicType<EnumStub>     { enum { type = Type::tEnumeration }; };
    template<> struct ToBasicType<EnumStub8>    { enum { type = Type::tEnumeration8 }; };
    template<> struct ToBasicType<EnumStub16>   { enum { type = Type::tEnumeration16 }; };
    template<> struct ToBasicType<Sequence>     { enum { type = Type::tIterator }; };
    template<> struct ToBasicType<Sequence *>   { enum { type = Type::tIterator }; };
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
    template<> struct ToBasicType<char>         { enum { type = Type::tInt8 }; };
#if defined(_MSC_VER)
    template<> struct ToBasicType<long>             { enum { type = Type::tInt }; };
    template<> struct ToBasicType<unsigned long>    { enum { type = Type::tUInt }; };
    template<> struct ToBasicType<long double>      { enum { type = Type::tDouble }; };
    template<> struct ToBasicType<wchar_t>          { enum { type = Type::tUInt16 }; };
#endif
#if defined(__APPLE__)
    // note - under MSVC size_t is unsigned __int64 (or 32) accordingly
    template<> struct ToBasicType<size_t>           { enum { type = sizeof(size_t)==8 ? Type::tUInt64 : Type::tUInt }; };
    template<> struct ToBasicType<long>             { enum { type = Type::tInt }; };
    template<> struct ToBasicType<long double>      { enum { type = Type::tDouble }; };
    template<> struct ToBasicType<wchar_t>          { enum { type = Type::tUInt16 }; };
#endif
#if defined(__linux__)
    template<> struct ToBasicType<long long int>      { enum { type = Type::tInt64 }; };
    template<> struct ToBasicType<unsigned long long int>     { enum { type = Type::tUInt64 }; };
#endif
#ifdef _EMSCRIPTEN_VER
    template<> struct ToBasicType<unsigned long>    { enum { type = Type::tUInt }; };
#endif
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
    template<> struct ToBasicType<Variant>      { enum { type = Type::tVariant }; };
    template<> struct ToBasicType<Context *>    { enum { type = Type::fakeContext }; };
    template<> struct ToBasicType<LineInfoArg *>{ enum { type = Type::fakeLineInfo }; };
    template<> struct ToBasicType<vec4f>        { enum { type = Type::anyArgument }; };

    template <typename TT>
    struct typeFactory {
        static ___noinline TypeDeclPtr make(const ModuleLibrary &) {
            auto t = make_smart<TypeDecl>();
            t->baseType = Type( ToBasicType<TT>::type );
            t->constant = is_const<TT>::value;
            return t;
        }
    };

    template <>
    struct typeFactory<char *> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary &) {
            auto t = make_smart<TypeDecl>(Type::tString);
            return t;
        }
    };

    template <>
    struct typeFactory<const char *> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary &) {
            auto t = make_smart<TypeDecl>(Type::tString);
            t->constant = true;
            return t;
        }
    };

    template <typename TT>
    struct typeFactory<smart_ptr<TT>> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = make_smart<TypeDecl>(Type::tPointer);
            t->firstType = typeFactory<TT>::make(lib);
            t->smartPtr = true;
            t->smartPtrNative = true;
            return t;
        }
    };

    template <typename TT>
    struct typeFactory<smart_ptr_raw<TT>> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = make_smart<TypeDecl>(Type::tPointer);
            t->firstType = typeFactory<TT>::make(lib);
            t->smartPtr = true;
            return t;
        }
    };

    template <>
    struct typeFactory<Array *> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary &) {
            auto t = make_smart<TypeDecl>(Type::tArray);
            return t;
        }
    };

    template <>
    struct typeFactory<Iterator *> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary &) {
            auto t = make_smart<TypeDecl>(Type::tIterator);
            return t;
        }
    };

    template <>
    struct typeFactory<const Iterator *> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary &) {
            auto t = make_smart<TypeDecl>(Type::tIterator);
            t->constant = true;
            return t;
        }
    };

    template <>
    struct typeFactory<Table *> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary &) {
            auto t = make_smart<TypeDecl>(Type::tTable);
            return t;
        }
    };

    template <>
    struct typeFactory<Context *> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary &) {
            return make_smart<TypeDecl>(Type::fakeContext);
        }
    };

    template <>
    struct typeFactory<LineInfoArg *> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary &) {
            return make_smart<TypeDecl>(Type::fakeLineInfo);
        }
    };

    template <typename ResultType, typename ...Args>
    struct typeFactory<TBlock<ResultType,Args...>> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = make_smart<TypeDecl>(Type::tBlock);
            t->firstType = typeFactory<ResultType>::make(lib);
            t->argTypes = { typeFactory<Args>::make(lib)... };
            return t;
        }
    };
    template <typename ResultType, typename ...Args>
    struct typeFactory<TFunc<ResultType,Args...>> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = make_smart<TypeDecl>(Type::tFunction);
            t->firstType = typeFactory<ResultType>::make(lib);
            t->argTypes = { typeFactory<Args>::make(lib)... };
            return t;
        }
    };

    template <typename ResultType, typename ...Args>
    struct typeFactory<TLambda<ResultType,Args...>> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = make_smart<TypeDecl>(Type::tLambda);
            t->firstType = typeFactory<ResultType>::make(lib);
            t->argTypes = { typeFactory<Args>::make(lib)... };
            return t;
        }
    };

    template <typename TT>
    struct typeFactory<TTemporary<TT>> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = typeFactory<TT>::make(lib);
            t->temporary = true;
            return t;
        }
    };

    template <typename TT>
    struct typeFactory<TExplicit<TT>> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = typeFactory<TT>::make(lib);
            t->isExplicit = true;
            return t;
        }
    };

    template <typename TT>
    struct TImplicit {};

    template <typename TT>
    struct typeFactory<TImplicit<TT>> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = typeFactory<TT>::make(lib);
            t->implicit = true;
            return t;
        }
    };

    template <typename TT>
    struct TArray;

    template <typename TT>
    struct typeFactory<TArray<TT>> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = make_smart<TypeDecl>(Type::tArray);
            t->firstType = typeFactory<TT>::make(lib);
            return t;
        }
    };

    template <typename TT, int size>
    struct TDim;

    template <typename TT, int size>
    struct typeFactory<TDim<TT,size>> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = typeFactory<TT>::make(lib);
            t->dim.push_back(size);
            return t;
        }
    };

    template <typename TK, typename TV>
    struct TTable;

    template <typename TK, typename TV>
    struct typeFactory<TTable<TK,TV>> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = make_smart<TypeDecl>(Type::tTable);
            t->firstType = typeFactory<TK>::make(lib);
            t->secondType = typeFactory<TV>::make(lib);
            return t;
        }
    };

    template <typename TT>
    struct TSequence;

    template <typename TT>
    struct typeFactory<TSequence<TT>> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = make_smart<TypeDecl>(Type::tIterator);
            t->firstType = typeFactory<TT>::make(lib);
            return t;
        }
    };


    template <typename TT, int dim>
    struct typeFactory<TT[dim]> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = typeFactory<TT>::make(lib);
            t->dim.push_back(dim);
            t->ref = false;
            t->isNativeDim = true;
            return t;
        }
    };

    template <typename TT>
    struct typeFactory<TT *> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary & lib) {
            auto pt = make_smart<TypeDecl>(Type::tPointer);
            if ( !is_void<TT>::value ) {
                pt->firstType = typeFactory<TT>::make(lib);
            }
            return pt;
        }
    };

    template <typename TT>
    struct typeFactory<const TT *> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary & lib) {
            auto pt = make_smart<TypeDecl>(Type::tPointer);
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
        static ___noinline TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = typeFactory<TT>::make(lib);
            t->ref = true;
            return t;
        }
    };

    template <typename TT>
    struct typeFactory<const TT &> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = typeFactory<TT>::make(lib);
            t->ref = true;
            t->constant = true;
            return t;
        }
    };

    template <typename TT>
    struct typeFactory<const TT> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = typeFactory<TT>::make(lib);
            t->constant = true;
            return t;
        }
    };

    template <typename FT, typename ST>
    struct typeFactory<pair<FT,ST>> {
        static ___noinline TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = make_smart<TypeDecl>(Type::tTuple);
            t->argTypes.push_back(typeFactory<FT>::make(lib));
            t->argTypes.push_back(typeFactory<ST>::make(lib));
            return t;
        }
    };

    template <typename TT>
    __forceinline TypeDeclPtr makeType(const ModuleLibrary & ctx) {
        return typeFactory<TT>::make(ctx);
    }

    template <typename TT>
    ___noinline TypeDeclPtr makeArgumentType(const ModuleLibrary & ctx) {
        auto tt = typeFactory<TT>::make(ctx);
        if (tt->isRefType()) {
            tt->ref = false;
        } else if (!tt->isRef()) {
            // note:
            //  C ++ does not differenciate between void foo ( Foo ); and void foo ( const Foo );
            //  DAS differenciates for pointers
            tt->constant = true;
        }
        return tt;
    }

    das::TypeDeclPtr makeHandleType(const das::ModuleLibrary & library, const char * typeName);

    bool splitTypeName ( const string & name, string & moduleName, string & funcName );

    bool isCircularType ( const TypeDeclPtr & type );
    bool hasImplicit ( const TypeDeclPtr & type );

    enum class CpptSubstitureRef { no, yes };
    enum class CpptSkipRef { no, yes };
    enum class CpptSkipConst { no, yes };
    enum class CpptRedundantConst { no, yes };

    string describeCppType(const TypeDeclPtr & type,
                           CpptSubstitureRef substituteRef = CpptSubstitureRef::no,
                           CpptSkipRef skipRef = CpptSkipRef::no,
                           CpptSkipConst skipConst = CpptSkipConst::no,
                           CpptRedundantConst redundantConst = CpptRedundantConst::yes );

    class MangledNameParser {
    protected:
        virtual void error ( const string &, const char * );
        string parseAnyName ( const char * & ch, bool allowModule );
        string parseAnyNameInBrackets ( const char * & ch, bool allowModule );
    public:
        TypeDeclPtr parseTypeFromMangledName ( const char * & ch, const ModuleLibrary & library, Module * thisModule );
    };
}

