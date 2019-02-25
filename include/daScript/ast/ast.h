#pragma once

#include "daScript/simulate/simulate.h"
#include "daScript/misc/string_writer.h"
#include "daScript/misc/vectypes.h"
#include "daScript/misc/arraytype.h"
#include "daScript/misc/rangetype.h"
#include "daScript/misc/function_traits.h"
#include "daScript/simulate/interop.h"
#include "daScript/simulate/data_walker.h"
#include "daScript/simulate/debug_info.h"
#include "daScript/ast/compilation_errors.h"
#include "daScript/simulate/runtime_table.h"
#include "daScript/simulate/runtime_array.h"

namespace das
{
    class Structure;
    typedef shared_ptr<Structure> StructurePtr;

    class Enumeration;
    typedef shared_ptr<Enumeration> EnumerationPtr;

    class Function;
    typedef shared_ptr<Function> FunctionPtr;

    struct Variable;
    typedef shared_ptr<Variable> VariablePtr;

    struct Expression;
    typedef shared_ptr<Expression> ExpressionPtr;

    class Program;
    typedef shared_ptr<Program> ProgramPtr;

    struct TypeDecl;
    typedef shared_ptr<TypeDecl> TypeDeclPtr;

    struct Annotation;
    typedef shared_ptr<Annotation> AnnotationPtr;

    struct TypeAnnotation;
    typedef shared_ptr<TypeAnnotation> TypeAnnotationPtr;

    struct FunctionAnnotation;
    typedef shared_ptr<FunctionAnnotation> FunctionAnnotationPtr;

    class Visitor;

    class Module;
    class ModuleLibrary;
    class ModuleGroup;

    struct TypeDecl : enable_shared_from_this<TypeDecl> {
        TypeDecl() = default;
        TypeDecl(const TypeDecl & decl);
        TypeDecl & operator = (const TypeDecl & decl) = delete;
        TypeDecl(Type tt) : baseType(tt) {}
        TypeDecl(const StructurePtr & sp) : baseType(Type::tStructure), structType(sp.get()) {}
        TypeDecl(const EnumerationPtr & ep) : baseType(Type::tEnumeration), enumType(ep) {}
        friend TextWriter& operator<< (TextWriter& stream, const TypeDecl & decl);
        string getMangledName() const;
        bool isSameType ( const TypeDecl & decl, bool refMatters = true, bool constMatters = true ) const;
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
        bool isVoid() const;
        bool isRef() const;
        bool isRefType() const;
        bool isIndex() const;
        bool isPointer() const;
        bool isEnum() const;
        bool isHandle() const;
        bool isStructure() const;
        int getSizeOf() const;
        int getCountOf() const;
        int getAlignOf() const;
        int getBaseSizeOf() const;
        int getStride() const;
        string describe ( bool extra = true ) const;
        bool canCopy() const;
        bool canMove() const;
        bool canDelete() const;
        bool isPod() const;
        bool isWorkhorseType() const; // we can return this, or pass this
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
        static Type getVectorType ( Type baseType, int dim );
        static int getMaskFieldIndex ( char ch );
        static bool isSequencialMask ( const vector<uint8_t> & fields );
        static bool buildSwizzleMask ( const string & mask, int dim, vector<uint8_t> & fields );
        static TypeDeclPtr inferAutoType ( TypeDeclPtr autoT, TypeDeclPtr initT );
        static void applyAutoContracts ( TypeDeclPtr TT, TypeDeclPtr autoT );
        Type getRangeBaseType() const;
        const TypeDecl * findAlias ( const string & name, bool allowAuto = false ) const;
        Type                baseType = Type::tVoid;
        Structure *         structType = nullptr;
        EnumerationPtr      enumType;
        TypeAnnotationPtr   annotation;
        TypeDeclPtr         firstType;      // map.first or array, or pointer
        TypeDeclPtr         secondType;     // map.second
        vector<TypeDeclPtr> argTypes;        // block arguments
        vector<uint32_t>    dim;
        union {
            struct {
                bool    ref : 1 ;
                bool    constant : 1;
                bool    removeRef : 1;
                bool    removeConstant : 1;
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
    template<> struct ToBasicType<Block>        { enum { type = Type::tBlock }; };
    template<> struct ToBasicType<Func>         { enum { type = Type::tFunction }; };
    template<> struct ToBasicType<Lambda>       { enum { type = Type::tLambda }; };
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
    struct typeFactory<const Array *> {
        static TypeDeclPtr make(const ModuleLibrary &) {
            auto t = make_shared<TypeDecl>(Type::tArray);
            t->constant = true;
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
    struct typeFactory<const Table *> {
        static TypeDeclPtr make(const ModuleLibrary &) {
            auto t = make_shared<TypeDecl>(Type::tTable);
            t->constant = true;
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
    struct typeFactory<TT &> {
        static TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = typeFactory<TT>::make(lib);
            t->ref = true;
            return t;
        }
    };

    template <typename TT>
    struct typeFactory<const TT *> {
        static TypeDeclPtr make(const ModuleLibrary & lib) {
            auto pt = make_shared<TypeDecl>(Type::tPointer);
            if ( !is_void<TT>::value ) {
                pt->firstType = typeFactory<TT>::make(lib);
                pt->constant = true;
            }
            return pt;
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
    inline TypeDeclPtr makeType(const ModuleLibrary & ctx) {
        return typeFactory<TT>::make(ctx);
    }

#define MAKE_TYPE_FACTORY(TYPE,CTYPE)                                        \
    template <>                                                                \
    struct das::typeFactory<CTYPE> {                                        \
        static das::TypeDeclPtr make(const das::ModuleLibrary & library ) {    \
            return library.makeHandleType(#TYPE);                            \
        }                                                                    \
    };

    bool splitTypeName ( const string & name, string & moduleName, string & funcName );

        //      [annotation (value,value,...,value)]
    //  or  [annotation (key=value,key,value,...,key=value)]
    struct AnnotationArgument {
        Type    type;       // only tInt, tFloat, tBool, and tString are allowed
        string  name;
        string  sValue;
        union {
            bool    bValue;
            int     iValue;
            float   fValue;
        };
        AnnotationArgument () : type(Type::tVoid), iValue(0) {}
        //explicit copy is required to avoid copying union as float and cause FPE
        AnnotationArgument (const AnnotationArgument&a)
            : type(a.type), name(a.name), sValue(a.sValue), iValue(a.iValue) {}
        AnnotationArgument ( const string & n, const string & s )
            : type(Type::tString), name(n), sValue(s), iValue(0) {}
        AnnotationArgument ( const string & n, bool  b )
            : type(Type::tBool), name(n), bValue(b) {}
        AnnotationArgument ( const string & n, int   i )
            : type(Type::tInt), name(n), iValue(i) {}
        AnnotationArgument ( const string & n, float f )
            : type(Type::tFloat), name(n), fValue(f) {}
    };

    struct AnnotationArgumentList {
        const AnnotationArgument * find ( const string & name, Type type ) const;
        bool getOption(const string & name, bool def = false) const;
        vector<AnnotationArgument>  arguments;
    };

    struct Annotation : BasicAnnotation, enable_shared_from_this<Annotation> {
        Annotation ( const string & n ) : BasicAnnotation(n) {}
        virtual ~Annotation() {}
        virtual void seal( Module * m ) { module = m; }
        virtual bool rtti_isHandledTypeAnnotation() const { return false; }
        virtual bool rtti_isStructureAnnotation() const { return false; }
        virtual bool rtti_isFunctionAnnotation() const { return false; }
        string describe() const { return name; }
        string getMangledName() const;
        Module *    module = nullptr;
    };

    struct AnnotationDeclaration : enable_shared_from_this<AnnotationDeclaration> {
        AnnotationPtr           annotation;
        AnnotationArgumentList  arguments;
    };
    typedef shared_ptr<AnnotationDeclaration> AnnotationDeclarationPtr;

    typedef vector<AnnotationDeclarationPtr> AnnotationList;

    class Enumeration : public enable_shared_from_this<Enumeration> {
    public:
        Enumeration() = default;
        Enumeration( const string & na ) : name(na) {}
        bool add ( const string & f );
        bool add ( const string & f, int v );
        string describe() const { return name; }
        string getMangledName() const;
        int find ( const string & na, int def ) const;
        string find ( int va, const string & def ) const;
        pair<int,bool> find ( const string & f ) const;
    public:
        string          name;
        LineInfo        at;
        map<string,int> list;
        map<int,string> listI;
        int             lastOne = 0;
        Module *        module = nullptr;
    };

    class Structure : public enable_shared_from_this<Structure> {
    public:
        struct FieldDeclaration {
            string          name;
            TypeDeclPtr     type;
            ExpressionPtr   init;
            bool            moveSemantic;
            LineInfo        at;
            int             offset = 0;
            FieldDeclaration() = default;
            FieldDeclaration(const string & n, const TypeDeclPtr & t,  const ExpressionPtr & i, bool ms, const LineInfo & a )
                : name(n), type(t), init(i), moveSemantic(ms), at(a) {}
        };
    public:
        Structure ( const string & n ) : name(n) {}
        const FieldDeclaration * findField ( const string & name ) const;
        int getSizeOf() const;
        int getAlignOf() const;
        bool canCopy() const;
        bool isPod() const;
        string describe() const { return name; }
        string getMangledName() const;
        bool hasAnyInitializers() const;
    public:
        string                      name;
        vector<FieldDeclaration>    fields;
        LineInfo                    at;
        Module *                    module = nullptr;
        bool                        genCtor = false;
    };

    struct Variable : public enable_shared_from_this<Variable> {
        VariablePtr clone() const;
        string getMangledName() const;
        string          name;
        TypeDeclPtr     type;
        ExpressionPtr   init;
        ExpressionPtr   source;     // if its interator variable, this is where the source is
        LineInfo        at;
        int             index = -1;
        uint32_t        stackTop = 0;
        Module *        module = nullptr;
        union {
            struct {
                bool    move_to_init : 1;
                bool    used : 1;
            };
            uint32_t flags = 0;
        };
        union {
            struct {
                bool    access_extern : 1;
                bool    access_get : 1;
                bool    access_ref : 1;
                bool    access_init : 1;
                bool    access_pass : 1;
            };
            uint32_t access_flags = 0;
        };
    };

    struct ExprBlock;

    struct FunctionAnnotation : Annotation {
        FunctionAnnotation ( const string & n ) : Annotation(n) {}
        virtual bool rtti_isFunctionAnnotation() const override { return true; }
        virtual bool apply ( const FunctionPtr & func, ModuleGroup & libGroup,
                            const AnnotationArgumentList & args, string & err ) = 0;
        virtual bool finalize ( const FunctionPtr & func, ModuleGroup & libGroup,
                               const AnnotationArgumentList & args,
                               const AnnotationArgumentList & progArgs, string & err ) = 0;
        virtual bool apply ( ExprBlock * block, ModuleGroup & libGroup,
                            const AnnotationArgumentList & args, string & err ) = 0;
        virtual bool finalize ( ExprBlock * block, ModuleGroup & libGroup,
                               const AnnotationArgumentList & args,
                               const AnnotationArgumentList & progArgs, string & err ) = 0;
    };

    struct TypeAnnotation : Annotation {
        TypeAnnotation ( const string & n ) : Annotation(n) {}
        virtual TypeAnnotationPtr clone ( const TypeAnnotationPtr & p = nullptr ) const {
            assert(p && "can only clone real type");
            p->name = name;
            return p;
        }
        virtual bool canMove() const { return true; }
        virtual bool canCopy() const { return true; }
        virtual bool isPod() const { return true; }
        virtual bool isRefType() const { return false; }
        virtual bool isLocal() const { return false; }
        virtual bool canNew() const { return false; }
        virtual bool canDelete() const { return false; }
        virtual bool canDeletePtr() const { return false; }
        virtual bool isIndexable ( const TypeDeclPtr & ) const { return false; }
        virtual bool isIterable ( ) const { return false; }
        virtual size_t getSizeOf() const { return sizeof(void *); }
        virtual size_t getAlignOf() const { return 1; }
        virtual TypeDeclPtr makeFieldType ( const string & ) const { return nullptr; }
        virtual TypeDeclPtr makeSafeFieldType ( const string & ) const { return nullptr; }
        virtual TypeDeclPtr makeIndexType ( TypeDeclPtr & ) const { return nullptr; }
        virtual TypeDeclPtr makeIteratorType () const { return nullptr; }
        virtual SimNode * simulateDelete ( Context &, const LineInfo &, SimNode *, uint32_t ) const { return nullptr; }
        virtual SimNode * simulateDeletePtr ( Context &, const LineInfo &, SimNode *, uint32_t ) const { return nullptr; }
        virtual SimNode * simulateCopy ( Context &, const LineInfo &, SimNode *, SimNode * ) const { return nullptr; }
        virtual SimNode * simulateRef2Value ( Context &, const LineInfo &, SimNode * ) const { return nullptr; }
        virtual SimNode * simulateGetField ( const string &, Context &, const LineInfo &, SimNode * ) const { return nullptr; }
        virtual SimNode * simulateGetFieldR2V ( const string &, Context &, const LineInfo &, SimNode * ) const { return nullptr; }
        virtual SimNode * simulateSafeGetField ( const string &, Context &, const LineInfo &, SimNode * ) const { return nullptr; }
        virtual SimNode * simulateSafeGetFieldPtr ( const string &, Context &, const LineInfo &, SimNode * ) const { return nullptr; }
        virtual SimNode * simulateGetNew ( Context &, const LineInfo & ) const { return nullptr; }
        virtual SimNode * simulateGetAt ( Context &, const LineInfo &, const TypeDeclPtr &, SimNode *, SimNode * ) const { return nullptr; }
        virtual SimNode * simulateGetIterator ( Context &, const LineInfo &, SimNode * ) const { return nullptr; }
        virtual void walk ( DataWalker &, void * ) { }
    };

    // annotated structure
    //  needs to override
    //      create,clone, simulateGetField, simulateGetFieldR2V, SafeGetField, and SafeGetFieldPtr
    struct StructureTypeAnnotation : TypeAnnotation {
        StructureTypeAnnotation ( const string & n ) : TypeAnnotation(n) {}
        virtual bool rtti_isStructureAnnotation() const override { return true; }
        virtual bool rtti_isHandledTypeAnnotation() const override { return true; }
        virtual bool canCopy() const override { return false; }
        virtual bool isPod() const override { return false; }
        virtual bool isRefType() const override { return false; }
        virtual bool create ( const shared_ptr<Structure> & st, const AnnotationArgumentList & /*args*/, string & /*err*/ ) {
            structureType = st;
            return true;
        }
        virtual TypeAnnotationPtr clone ( const TypeAnnotationPtr & p = nullptr ) const override {
            shared_ptr<StructureTypeAnnotation> cp =  p ? static_pointer_cast<StructureTypeAnnotation>(p) : make_shared<StructureTypeAnnotation>(name);
            cp->structureType = structureType;
            return TypeAnnotation::clone(cp);
        }
        shared_ptr<Structure>   structureType;
    };

    struct Expression : enable_shared_from_this<Expression> {
        Expression() = default;
        Expression(const LineInfo & a) : at(a) {}
        virtual ~Expression() {}
        friend TextWriter& operator<< (TextWriter& stream, const Expression & func);
        virtual ExpressionPtr visit(Visitor & vis) = 0;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const;
        static ExpressionPtr autoDereference ( const ExpressionPtr & expr );
        virtual SimNode * simulate (Context & context) const = 0;
        virtual SimNode * trySimulate (Context & context, uint32_t extraOffset, Type r2vType ) const;
        virtual bool rtti_isSequence() const { return false; }
        virtual bool rtti_isConstant() const { return false; }
        virtual bool rtti_isStringConstant() const { return false; }
        virtual bool rtti_isCall() const { return false; }
        virtual bool rtti_isLet() const { return false; }
        virtual bool rtti_isReturn() const { return false; }
        virtual bool rtti_isBreak() const { return false; }
        virtual bool rtti_isBlock() const { return false; }
        virtual bool rtti_isWith() const { return false; }
        virtual bool rtti_isVar() const { return false; }
        virtual bool rtti_isField() const { return false; }
        virtual bool rtti_isSwizzle() const { return false; }
        virtual bool rtti_isSafeField() const { return false; }
        virtual bool rtti_isAt() const { return false; }
        virtual bool rtti_isOp1() const { return false; }
        virtual bool rtti_isOp2() const { return false; }
        virtual bool rtti_isOp3() const { return false; }
        virtual bool rtti_isNullCoalescing() const { return false; }
        virtual bool rtti_isValues() const { return false; }
        virtual bool rtti_isMakeBlock() const { return false; }
        virtual bool rtti_isIfThenElse() const { return false; }
        virtual bool rtti_isAddr() const { return false; }
        virtual Expression * tail() { return this; }
        virtual uint32_t getEvalFlags() const { return 0; }
        LineInfo    at;
        TypeDeclPtr type;
        union {
            struct {
                bool    constexpression : 1;
                bool    noSideEffects : 1;
            };
            uint32_t    flags = 0;
        };
        union {
            struct {
                bool    topLevel :  1;
                bool    argLevel : 1;
                bool    bottomLevel : 1;
            };
            uint32_t    printFlags = 0;
        };
    };

    template <typename ExprType>
    inline shared_ptr<ExprType> clonePtr ( const ExpressionPtr & expr ) {
        return expr ? static_pointer_cast<ExprType>(expr) : make_shared<ExprType>();
    }

    struct ExprRef2Value : Expression {
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        static SimNode * GetR2V ( Context & context, const LineInfo & a, const TypeDeclPtr & type, SimNode * expr );
        ExpressionPtr   subexpr;
    };

    struct ExprPtr2Ref : Expression {
        ExprPtr2Ref () = default;
        ExprPtr2Ref ( const LineInfo & a, const ExpressionPtr & s ) : Expression(a), subexpr(s) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        ExpressionPtr   subexpr;
    };

    struct ExprAddr : Expression {
        ExprAddr () = default;
        ExprAddr ( const LineInfo & a, const string & n ) : Expression(a), target(n) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isAddr() const override { return true; }
        string target;
        Function * func = nullptr;
    };

    struct ExprNullCoalescing : ExprPtr2Ref {
        ExprNullCoalescing () = default;
        ExprNullCoalescing ( const LineInfo & a, const ExpressionPtr & s, const ExpressionPtr & defVal )
            : ExprPtr2Ref(a,s), defaultValue(defVal) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isNullCoalescing() const override { return true; }
        ExpressionPtr   defaultValue;
    };

    struct ExprDelete : Expression {
        ExprDelete() = default;
        ExprDelete ( const LineInfo & a, const ExpressionPtr & s )
            : Expression(a), subexpr(s) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        ExpressionPtr subexpr;
    };

    struct ExprAt : Expression {
        ExprAt() = default;
        ExprAt ( const LineInfo & a, const ExpressionPtr & s, const ExpressionPtr & i )
            : Expression(a), subexpr(s), index(i) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isAt() const override { return true; }
        ExpressionPtr   subexpr, index;
        union {
            struct {
                bool        r2v : 1;
                bool        r2cr : 1;
                bool        write : 1;
            };
            uint32_t atFlags = 0;
        };
    };

    struct ExprBlock : Expression {
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual uint32_t getEvalFlags() const override;
        uint32_t getFinallyEvalFlags() const;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isBlock() const override { return true; }
        VariablePtr findArgument(const string & name);
        vector<SimNode *> collectExpressions ( Context & context, const vector<ExpressionPtr> & list ) const;
        void simulateFinal ( Context & context, SimNode_Final * sim ) const;
        void simulateBlock ( Context & context, SimNode_Block * sim ) const;
        TypeDeclPtr makeBlockType () const;
        vector<ExpressionPtr>   list;
        vector<ExpressionPtr>   finalList;
        TypeDeclPtr             returnType;
        vector<VariablePtr>     arguments;
        uint32_t                stackTop = 0;
        AnnotationList          annotations;
        uint64_t                annotationData = 0;   // to be filled with annotation
        union {
            struct {
                bool            isClosure : 1;
                bool            hasReturn : 1;
                bool            copyOnReturn : 1;
                bool            moveOnReturn : 1;
                bool            inTheLoop : 1;
            };
            uint32_t            blockFlags = 0;
        };
    };

    struct ExprVar : Expression {
        ExprVar () = default;
        ExprVar ( const LineInfo & a, const string & n ) : Expression(a), name(n) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual SimNode * trySimulate (Context & context, uint32_t extraOffset, Type r2vType ) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isVar() const override { return true; }
        string              name;
        VariablePtr         variable;
        weak_ptr<ExprBlock> pBlock;
        int                 argumentIndex = -1;
        union {
            struct {
                bool        local : 1;
                bool        argument : 1;
                bool        block : 1;
                bool        r2v  : 1;       // built-in ref2value   (read-only)
                bool        r2cr : 1;       // built-in ref2contref (read-only, but stay ref)
                bool        write : 1;
            };
            uint32_t varFlags = 0;
        };
    };

    struct ExprField : Expression {
        ExprField () = default;
        ExprField ( const LineInfo & a, const ExpressionPtr & val, const string & n )
            : Expression(a), value(val), name(n) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual SimNode * trySimulate (Context & context, uint32_t extraOffset, Type r2vType ) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isField() const override { return true; }
        ExpressionPtr   value;
        string          name;
        const Structure::FieldDeclaration * field = nullptr;
        TypeAnnotationPtr annotation;
        union {
            struct {
                bool        r2v : 1;
                bool        r2cr : 1;
                bool        write : 1;
            };
            uint32_t fieldFlags = 0;
        };
    };

    struct ExprSwizzle : Expression {
        ExprSwizzle () = default;
        ExprSwizzle ( const LineInfo & a, const ExpressionPtr & val, const string & n )
            : Expression(a), value(val), mask(n) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual SimNode * trySimulate (Context & context, uint32_t extraOffset, Type r2vType ) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isSwizzle() const override { return true; }
        ExpressionPtr   value;
        string          mask;
        vector<uint8_t> fields;
        union {
            struct {
                bool        r2v : 1;
                bool        r2cr : 1;
                bool        write : 1;
            };
            uint32_t fieldFlags = 0;
        };
    };

    struct ExprSafeField : ExprField {
        ExprSafeField () = default;
        ExprSafeField ( const LineInfo & a, const ExpressionPtr & val, const string & n )
            : ExprField(a,val,n) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
         virtual bool rtti_isField() const override { return false; }
         virtual bool rtti_isSafeField() const override { return true; }
        bool skipQQ = false;
    };

    struct ExprOp : Expression {
        ExprOp () = default;
        ExprOp ( const LineInfo & a, const string & o ) : Expression(a), op(o) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        string      op;
        Function *  func = nullptr;   // always built-in function
        uint32_t    stackTop = 0;
    };

    // unary    !subexpr
    struct ExprOp1 : ExprOp {
        ExprOp1 () = default;
        ExprOp1 ( const LineInfo & a, const string & o, const ExpressionPtr & s )
            : ExprOp(a,o), subexpr(s) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual Expression * tail() override { return subexpr->tail(); }
        virtual bool rtti_isOp1() const override { return true; }
        ExpressionPtr   subexpr;
    };

    // binary   left < right
    struct ExprOp2 : ExprOp {
        ExprOp2 () = default;
        ExprOp2 ( const LineInfo & a, const string & o, const ExpressionPtr & l, const ExpressionPtr & r )
            : ExprOp(a,o), left(l), right(r) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual Expression * tail() override { return right->tail(); }
        virtual bool rtti_isOp2() const override { return true; }
        ExpressionPtr   left, right;
    };

    // this copies one object to the other
    struct ExprCopy : ExprOp2 {
        ExprCopy () = default;
        ExprCopy ( const LineInfo & a, const ExpressionPtr & l, const ExpressionPtr & r )
            : ExprOp2(a, "=", l, r) {};
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
    };

    // this moves one object to the other
    struct ExprMove : ExprOp2 {
        ExprMove () = default;
        ExprMove ( const LineInfo & a, const ExpressionPtr & l, const ExpressionPtr & r )
            : ExprOp2(a, "<-", l, r) {};
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
    };

    // this only exists during parsing, and can't be
    // and this is why it does not have CLONE
    struct ExprSequence : ExprOp2 {
        ExprSequence ( const LineInfo & a, const ExpressionPtr & l, const ExpressionPtr & r )
            : ExprOp2(a, ",", l, r) {}
        virtual bool rtti_isSequence() const override { return true; }
    };

    // trinary  subexpr ? left : right
    struct ExprOp3 : ExprOp {
        ExprOp3 () = default;
        ExprOp3 ( const LineInfo & a, const string & o, const ExpressionPtr & s,
                 const ExpressionPtr & l, const ExpressionPtr & r )
            : ExprOp(a,o), subexpr(s), left(l), right(r) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual Expression * tail() override { return right->tail(); }
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isOp3() const override { return true; }
        ExpressionPtr   subexpr, left, right;
    };

    struct ExprTryCatch : Expression {
        ExprTryCatch() = default;
        ExprTryCatch ( const LineInfo & a, const ExpressionPtr & t, const ExpressionPtr & c )
            : Expression(a), try_block(t), catch_block(c) {}
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual uint32_t getEvalFlags() const override;
        ExpressionPtr try_block, catch_block;
    };

    struct ExprReturn : Expression {
        ExprReturn() = default;
        ExprReturn ( const LineInfo & a, const ExpressionPtr & s )
            : Expression(a), subexpr(s) {}
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual uint32_t getEvalFlags() const override { return EvalFlags::stopForReturn; }
        virtual bool rtti_isReturn() const override { return true; }
        ExpressionPtr subexpr;
        union {
            struct {
                bool moveSemantics   : 1;
                bool returnReference : 1;
                bool returnInBlock   : 1;
            };
            uint32_t    returnFlags = 0;
        };
        uint32_t                stackTop = 0;
        Function *              func = nullptr;
        ExprBlock *             block = nullptr;
    };

    struct ExprBreak : Expression {
        ExprBreak() = default;
        ExprBreak ( const LineInfo & a ) : Expression(a) {}
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual uint32_t getEvalFlags() const override { return EvalFlags::stopForBreak; }
        virtual bool rtti_isBreak() const override { return true; }
    };

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4324)
#endif
    struct ExprConst : Expression {
        ExprConst ( Type t ) : baseType(t) {}
        ExprConst ( const LineInfo & a, Type t ) : Expression(a), baseType(t) {}
        virtual SimNode * simulate (Context & context) const override;
        virtual bool rtti_isConstant() const override { return true; }
        Type    baseType;
        vec4f  value;
      };
#ifdef _MSC_VER
#pragma warning(pop)
#endif

    template <typename TT, typename ExprConstExt>
    struct ExprConstT : ExprConst {
        ExprConstT ( TT val, Type bt ) : ExprConst(bt) { value = cast<TT>::from(val); }
        ExprConstT ( const LineInfo & a, TT val, Type bt ) : ExprConst(a,bt) { value = cast<TT>::from(val); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr ) const override {
            auto cexpr = clonePtr<ExprConstExt>(expr);
            Expression::clone(cexpr);
            cexpr->value = value;
            return cexpr;
        }
        virtual ExpressionPtr visit(Visitor & vis) override;
        TT getValue() const { return cast<TT>::to(value); }
    };

    struct ExprFakeContext : ExprConstT<void *, ExprFakeContext> {
        ExprFakeContext(void * ptr = nullptr) : ExprConstT(ptr, Type::fakeContext) {}
        ExprFakeContext(const LineInfo & a, void * ptr = nullptr) : ExprConstT(a, ptr, Type::fakeContext) {}
    };

    struct ExprConstPtr : ExprConstT<void *,ExprConstPtr> {
        ExprConstPtr(void * ptr = nullptr) : ExprConstT(ptr,Type::tPointer) {}
        ExprConstPtr(const LineInfo & a, void * ptr = nullptr) : ExprConstT(a,ptr,Type::tPointer) {}
    };

    struct ExprConstInt : ExprConstT<int32_t,ExprConstInt> {
        ExprConstInt(int32_t i = 0)  : ExprConstT(i,Type::tInt) {}
        ExprConstInt(const LineInfo & a, int32_t i = 0)  : ExprConstT(a,i,Type::tInt) {}
    };
    
    struct ExprConstEnumeration : ExprConstT<int32_t,ExprConstEnumeration> {
        ExprConstEnumeration(int32_t i = 0, const TypeDeclPtr & td = nullptr)
            : ExprConstT(i,Type::tEnumeration) {
                if ( td ) {
                    enumType = td->enumType;
                }
        }
        ExprConstEnumeration(const LineInfo & a, int32_t i, const TypeDeclPtr & td)
            : ExprConstT(a,i,Type::tEnumeration) {
            enumType = td->enumType;
        }
        virtual ExpressionPtr clone( const ExpressionPtr & expr ) const override {
            auto cexpr = clonePtr<ExprConstEnumeration>(expr);
            ExprConstT<int32_t,ExprConstEnumeration> ::clone(cexpr);
            cexpr->enumType = enumType;
            return cexpr;
        }
        EnumerationPtr enumType;
    };

    struct ExprConstInt64 : ExprConstT<int64_t,ExprConstInt64> {
        ExprConstInt64(int64_t i = 0)  : ExprConstT(i,Type::tInt64) {}
        ExprConstInt64(const LineInfo & a, int64_t i = 0)  : ExprConstT(a,i,Type::tInt64) {}
    };

    struct ExprConstInt2 : ExprConstT<int2,ExprConstInt2> {
        ExprConstInt2(int2 i = int2())  : ExprConstT(i,Type::tInt2) {}
        ExprConstInt2(const LineInfo & a, int2 i)  : ExprConstT(a,i,Type::tInt2) {}
    };

    struct ExprConstInt3 : ExprConstT<int3,ExprConstInt3> {
        ExprConstInt3(int3 i = int3())  : ExprConstT(i,Type::tInt3) {}
        ExprConstInt3(const LineInfo & a, int3 i)  : ExprConstT(a,i,Type::tInt3) {}
    };

    struct ExprConstInt4 : ExprConstT<int4,ExprConstInt4> {
        ExprConstInt4(int4 i = int4())  : ExprConstT(i,Type::tInt4) {}
        ExprConstInt4(const LineInfo & a, int4 i)  : ExprConstT(a,i,Type::tInt4) {}
    };

    struct ExprConstUInt64 : ExprConstT<uint64_t,ExprConstUInt64> {
        ExprConstUInt64(uint64_t i = 0) : ExprConstT(i,Type::tUInt64) {}
        ExprConstUInt64(const LineInfo & a, uint64_t i = 0) : ExprConstT(a,i,Type::tUInt64) {}
    };

    struct ExprConstUInt : ExprConstT<uint32_t,ExprConstUInt> {
        ExprConstUInt(uint32_t i = 0) : ExprConstT(i,Type::tUInt) {}
        ExprConstUInt(const LineInfo & a, uint32_t i = 0) : ExprConstT(a,i,Type::tUInt) {}
    };

    struct ExprConstUInt2 : ExprConstT<uint2,ExprConstUInt2> {
        ExprConstUInt2(uint2 i = uint2())  : ExprConstT(i,Type::tUInt2) {}
        ExprConstUInt2(const LineInfo & a, uint2 i)  : ExprConstT(a,i,Type::tUInt2) {}
    };

    struct ExprConstUInt3 : ExprConstT<uint3,ExprConstUInt3> {
        ExprConstUInt3(uint3 i = uint3())  : ExprConstT(i,Type::tUInt3) {}
        ExprConstUInt3(const LineInfo & a, uint3 i)  : ExprConstT(a,i,Type::tUInt3) {}
    };

    struct ExprConstUInt4 : ExprConstT<uint4,ExprConstUInt4> {
        ExprConstUInt4(uint4 i = uint4())  : ExprConstT(i,Type::tUInt4) {}
        ExprConstUInt4(const LineInfo & a, uint4 i)  : ExprConstT(a,i,Type::tUInt4) {}
    };

    struct ExprConstBool : ExprConstT<bool,ExprConstBool> {
        ExprConstBool(bool i = false) : ExprConstT(i,Type::tBool) {}
        ExprConstBool(const LineInfo & a, bool i = false) : ExprConstT(a,i,Type::tBool) {}
    };

    struct ExprConstFloat : ExprConstT<float,ExprConstFloat> {
        ExprConstFloat(float i = 0.0f) : ExprConstT(i,Type::tFloat) {}
        ExprConstFloat(const LineInfo & a, float i = 0.0f) : ExprConstT(a,i,Type::tFloat) {}
    };

    struct ExprConstDouble : ExprConstT<double,ExprConstDouble> {
        ExprConstDouble(double i = 0.0) : ExprConstT(i,Type::tDouble) {}
        ExprConstDouble(const LineInfo & a, double i = 0.0) : ExprConstT(a,i,Type::tDouble) {}
    };

    struct ExprConstFloat2 : ExprConstT<float2,ExprConstFloat2> {
        ExprConstFloat2(float2 i = float2())  : ExprConstT(i,Type::tFloat2) {}
        ExprConstFloat2(const LineInfo & a, float2 i)  : ExprConstT(a,i,Type::tFloat2) {}
    };

    struct ExprConstFloat3 : ExprConstT<float3,ExprConstFloat3> {
        ExprConstFloat3(float3 i = float3())  : ExprConstT(i,Type::tFloat) {}
        ExprConstFloat3(const LineInfo & a, float3 i)  : ExprConstT(a,i,Type::tFloat3) {}
    };

    struct ExprConstFloat4 : ExprConstT<float4,ExprConstFloat4> {
        ExprConstFloat4(float4 i = float4())  : ExprConstT(i,Type::tFloat4) {}
        ExprConstFloat4(const LineInfo & a, float4 i)  : ExprConstT(a,i,Type::tFloat4) {}
    };

    struct ExprConstString : ExprConst {
        ExprConstString(const string & str = string())
            : ExprConst(Type::tString), text(unescapeString(str)) {}
        ExprConstString(const LineInfo & a, const string & str = string())
            : ExprConst(a,Type::tString), text(unescapeString(str)) {}
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        const string & getValue() const { return text; }
        virtual bool rtti_isStringConstant() const override { return true; }
        string text;
    };

    struct ExprStringBuilder : Expression {
        ExprStringBuilder() = default;
        ExprStringBuilder(const LineInfo & a) : Expression(a) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        vector<ExpressionPtr>   elements;
    };

    struct ExprLet : Expression {
        Variable * find ( const string & name ) const;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        static SimNode * simulateInit(Context & context, const VariablePtr & var, bool local);
        static vector<SimNode *> simulateInit(Context & context, const ExprLet * pLet);
        virtual uint32_t getEvalFlags() const override;
        virtual bool rtti_isLet() const override { return true; }
        vector<VariablePtr>     variables;
        ExpressionPtr           subexpr;
        bool                    scoped = true;
        bool                    inScope = false;
    };

    // for a,b in foo,bar where a>b ...
    struct ExprFor : Expression {
        ExprFor () = default;
        ExprFor ( const LineInfo & a ) : Expression(a) {}
        Variable * findIterator ( const string & name ) const;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual uint32_t getEvalFlags() const override;
        vector<string>          iterators;
        vector<VariablePtr>     iteratorVariables;
        vector<ExpressionPtr>   sources;
        ExpressionPtr           subexpr;
    };

    struct ExprWhile : Expression {
        ExprWhile() = default;
        ExprWhile(const LineInfo & a) : Expression(a) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual uint32_t getEvalFlags() const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        static void simulateFinal ( Context & context, const ExpressionPtr & bod, SimNode_Block * blk );
        ExpressionPtr   cond, body;
    };

    struct ExprWith : Expression {
        ExprWith() = default;
        ExprWith(const LineInfo & a) : Expression(a) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isWith() const override { return true; }
        ExpressionPtr   with, body;
    };

    struct ExprLooksLikeCall : Expression {
        ExprLooksLikeCall () = default;
        ExprLooksLikeCall ( const LineInfo & a, const string & n ) : Expression(a), name(n) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        void autoDereference();
        virtual SimNode * simulate (Context &) const override { return nullptr; }
        virtual ExpressionPtr visit(Visitor & vis) override;
        string describe() const;
        virtual bool rtti_isCall() const override { return true; }
        string                  name;
        vector<ExpressionPtr>   arguments;
        bool                    argumentsFailedToInfer = false;
    };
    typedef function<ExprLooksLikeCall * (const LineInfo & info)> ExprCallFactory;

    template <typename TT>
    struct ExprLikeCall : ExprLooksLikeCall {
        ExprLikeCall () = default;
        ExprLikeCall ( const LineInfo & a, const string & n ) : ExprLooksLikeCall(a,n) {}
        virtual ExpressionPtr visit ( Visitor & vis ) override;
    };

    struct ExprMakeBlock : Expression {
        ExprMakeBlock () = default;
        ExprMakeBlock ( const LineInfo & a, const ExpressionPtr & b )
        : Expression(a), block(b) { b->at = a; static_pointer_cast<ExprBlock>(b)->isClosure = true; }
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual bool rtti_isMakeBlock() const override { return true; }
        ExpressionPtr block;
        uint32_t stackTop = 0;
    };

    struct ExprMakeLambda : ExprLooksLikeCall {
        ExprMakeLambda () = default;
        ExprMakeLambda ( const LineInfo & a, const ExpressionPtr & b = nullptr );
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    struct ExprInvoke : ExprLikeCall<ExprInvoke> {
        ExprInvoke () = default;
        ExprInvoke ( const LineInfo & a, const string & name ) : ExprLikeCall<ExprInvoke>(a,name) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        uint32_t    stackTop = 0;
    };

    struct ExprAssert : ExprLikeCall<ExprAssert> {
        ExprAssert () = default;
        ExprAssert ( const LineInfo & a, const string & name ) : ExprLikeCall<ExprAssert>(a,name) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    struct ExprStaticAssert : ExprLikeCall<ExprStaticAssert> {
        ExprStaticAssert () = default;
        ExprStaticAssert ( const LineInfo & a, const string & name ) : ExprLikeCall<ExprStaticAssert>(a,name) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    struct ExprDebug : ExprLikeCall<ExprDebug> {
        ExprDebug () = default;
        ExprDebug ( const LineInfo & a, const string & name ) : ExprLikeCall<ExprDebug>(a, name) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    template <typename It, typename SimNodeT, bool first>
    struct ExprTableKeysOrValues : ExprLooksLikeCall {
        ExprTableKeysOrValues() = default;
        ExprTableKeysOrValues ( const LineInfo & a, const string & name ) : ExprLooksLikeCall(a, name) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override {
            auto cexpr = clonePtr<ExprTableKeysOrValues<It,SimNodeT,first>>(expr);
            return cexpr;
        }
        virtual SimNode * simulate (Context & context) const override {
            auto subexpr = arguments[0]->simulate(context);
            auto tableType = arguments[0]->type;
            auto iterType = first ? tableType->firstType : tableType->secondType;
            auto stride = iterType->getSizeOf();
            return context.code->makeNode<SimNodeT>(at,subexpr,stride);
        }
        virtual ExpressionPtr visit ( Visitor & vis ) override;
    };

    struct ExprKeys : ExprTableKeysOrValues<ExprKeys,SimNode_TableIterator<TableKeysIterator>,true> {
        ExprKeys() = default;
        ExprKeys ( const LineInfo & a, const string & n )
            : ExprTableKeysOrValues<ExprKeys,SimNode_TableIterator<TableKeysIterator>,true>(a, n) {}
    };

    struct ExprValues : ExprTableKeysOrValues<ExprValues,SimNode_TableIterator<TableValuesIterator>,false> {
        ExprValues() = default;
        ExprValues ( const LineInfo & a, const string & n )
            : ExprTableKeysOrValues<ExprValues,SimNode_TableIterator<TableValuesIterator>,false>(a, n) {}
        virtual bool rtti_isValues() const override { return true; }
    };

    template <typename It, typename SimNodeT>
    struct ExprArrayCallWithSizeOrIndex : ExprLooksLikeCall {
        ExprArrayCallWithSizeOrIndex() = default;
        ExprArrayCallWithSizeOrIndex ( const LineInfo & a, const string & name ) : ExprLooksLikeCall(a, name) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override {
            auto cexpr = clonePtr<ExprArrayCallWithSizeOrIndex<It,SimNodeT>>(expr);
            ExprLooksLikeCall::clone(cexpr);
            return cexpr;
        }
        virtual SimNode * simulate (Context & context) const override {
            auto arr = arguments[0]->simulate(context);
            auto newSize = arguments[1]->simulate(context);
            auto size = arguments[0]->type->firstType->getSizeOf();
            return context.code->makeNode<SimNodeT>(at,arr,newSize,size);
        }
        virtual ExpressionPtr visit ( Visitor & vis ) override;
    };

    struct ExprErase : ExprLikeCall<ExprErase> {
        ExprErase() = default;
        ExprErase ( const LineInfo & a, const string & ) : ExprLikeCall<ExprErase>(a, "erase") {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    struct ExprFind : ExprLikeCall<ExprFind> {
        ExprFind() = default;
        ExprFind ( const LineInfo & a, const string & ) : ExprLikeCall<ExprFind>(a, "find") {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    struct ExprSizeOf : Expression {
        ExprSizeOf () = default;
        ExprSizeOf ( const LineInfo & a, const ExpressionPtr & s )
            : Expression(a), subexpr(s) {}
        ExprSizeOf ( const LineInfo & a, const TypeDeclPtr & d )
            : Expression(a), typeexpr(d) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        ExpressionPtr   subexpr;
        TypeDeclPtr     typeexpr;
    };

    struct ExprTypeName : Expression {
        ExprTypeName () = default;
        ExprTypeName ( const LineInfo & a, const ExpressionPtr & s )
            : Expression(a), subexpr(s) {}
        ExprTypeName ( const LineInfo & a, const TypeDeclPtr & d )
            : Expression(a), typeexpr(d) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        ExpressionPtr   subexpr;
        TypeDeclPtr     typeexpr;
    };

    struct ExprAscend : Expression {
        ExprAscend() = default;
        ExprAscend( const LineInfo & a, const ExpressionPtr & se, const TypeDeclPtr & as = nullptr )
            : Expression(a), subexpr(se), ascType(as) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        ExpressionPtr   subexpr;
        TypeDeclPtr     ascType;
    };

    struct ExprNew : ExprLooksLikeCall {
        ExprNew() = default;
        ExprNew ( const LineInfo & a, TypeDeclPtr t, bool ini )
            : ExprLooksLikeCall(a,"new"), typeexpr(t), initializer(ini) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        Function *      func = nullptr;
        TypeDeclPtr     typeexpr;
        uint32_t        stackTop = 0;
        bool            initializer = false;
    };

    struct ExprCall : ExprLooksLikeCall {
        ExprCall () = default;
        ExprCall ( const LineInfo & a, const string & n ) : ExprLooksLikeCall(a,n) { }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        static SimNode_CallBase * simulateCall (const FunctionPtr & func, const ExprLooksLikeCall * expr,
            Context & context, SimNode_CallBase * pCall);
        Function *      func = nullptr;
        uint32_t        stackTop = 0;
    };

    struct ExprIfThenElse : Expression {
        ExprIfThenElse () = default;
        ExprIfThenElse ( const LineInfo & a, const ExpressionPtr & c,
                        const ExpressionPtr & ift, const ExpressionPtr & iff )
            : Expression(a), cond(c), if_true(ift), if_false(iff) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isIfThenElse() const override { return true; }
        virtual uint32_t getEvalFlags() const override;
        ExpressionPtr   cond, if_true, if_false;
    };

    struct MakeFieldDecl : enable_shared_from_this<MakeFieldDecl> {
        LineInfo        at;
        string          name;
        ExpressionPtr   value;
        bool            moveSemantic;
        MakeFieldDecl () = default;
        MakeFieldDecl ( const LineInfo & a, const string & n, const ExpressionPtr & e, bool mv )
            : at(a), name(n), value(e), moveSemantic(mv) {}
    };
    typedef shared_ptr<MakeFieldDecl>   MakeFieldDeclPtr;
    typedef vector<MakeFieldDeclPtr>    MakeStruct;
    typedef shared_ptr<MakeStruct>      MakeStructPtr;

    struct ExprMakeStructure : Expression {
        ExprMakeStructure() = default;
        ExprMakeStructure ( const LineInfo & at ) : Expression(at) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        TypeDeclPtr                 makeType;
        vector<MakeStructPtr>       structs;
        uint32_t                    stackTop = 0;
        bool                        useInitializer = false;
    };

    struct ExprMakeArray : Expression {
        ExprMakeArray() = default;
        ExprMakeArray ( const LineInfo & at ) : Expression(at) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        TypeDeclPtr                 makeType;
        TypeDeclPtr                 recordType;
        vector<ExpressionPtr>       values;
        uint32_t                    stackTop = 0;
    };

    enum class SideEffects : uint32_t {
        none =              0
    ,   userScenario =      (1<<0)
    ,   modifyExternal =    (1<<1)
    ,   modifyArgument =    (1<<2)
    ,   accessGlobal =      (1<<3)
    ,   invoke =            (1<<4)

    ,   inferedSideEffects = uint32_t(SideEffects::modifyArgument) | uint32_t(SideEffects::accessGlobal) | uint32_t(SideEffects::invoke)
    };

    class Function : public enable_shared_from_this<Function> {
    public:
        virtual ~Function() {}
        friend TextWriter& operator<< (TextWriter& stream, const Function & func);
        string getMangledName() const;
        VariablePtr findArgument(const string & name);
        SimNode * simulate (Context & context) const;
        virtual SimNode * makeSimNode ( Context & context ) {
            if ( copyOnReturn || moveOnReturn ) {
                return context.code->makeNodeUnroll<SimNode_CallAndCopyOrMove>(int(arguments.size()), at);
            } else if ( fastCall ) {
                return context.code->makeNodeUnroll<SimNode_FastCall>(int(arguments.size()), at);
            } else {
                return context.code->makeNodeUnroll<SimNode_Call>(int(arguments.size()), at);
            }
        }
        string describe() const;
        virtual FunctionPtr visit(Visitor & vis);
        FunctionPtr setSideEffects ( SideEffects seFlags ) { sideEffectFlags = (uint32_t) seFlags; return shared_from_this(); }
        bool isGeneric() const;
        FunctionPtr clone() const;
        string getLocationExtra() const;
    public:
        AnnotationList      annotations;
        string              name;
        vector<VariablePtr> arguments;
        TypeDeclPtr         result;
        ExpressionPtr       body;
        int                 index = -1;
        uint32_t            totalStackSize = 0;
        LineInfo            at;
        Module *            module = nullptr;
        set<Function *>     useFunctions;
        set<Variable *>     useGlobalVariables;
        union {
            struct {
                bool    builtIn : 1;
                bool    callBased : 1;
                bool    hasReturn: 1;
                bool    copyOnReturn : 1;
                bool    moveOnReturn : 1;
                bool    exports : 1;
                bool    addr : 1;
                bool    used : 1;
                bool    fastCall : 1;
                bool    knownSideEffects : 1;
                bool    hasToRunAtCompileTime : 1;
            };
            uint32_t flags = 0;
        };
        uint32_t    sideEffectFlags = 0;
        struct InferHistory {
            LineInfo    at;
            Function *  func = nullptr;
            InferHistory() = default;
            InferHistory(const LineInfo & a, const FunctionPtr & p) : at(a), func(p.get()) {}
        };
        vector<InferHistory> inferStack;
    };

    class BuiltInFunction : public Function {
    public:
        BuiltInFunction ( const string & fn );
    };

    struct Error {
        Error ( const string & w, LineInfo a, CompilationError ce ) : what(w), at(a), cerr(ce)  {}
        __forceinline bool operator < ( const Error & err ) const { return at==err.at ? what < err.what : at<err.at; };
        string              what;
        LineInfo            at;
        CompilationError    cerr;
    };

    class Module {
    public:
        Module ( const string & n = "" );
        virtual ~Module();
        bool addAlias ( const TypeDeclPtr & at, bool canFail = false );
        bool addVariable ( const VariablePtr & var, bool canFail = false );
        bool addStructure ( const StructurePtr & st, bool canFail = false );
        bool removeStructure ( const StructurePtr & st );
        bool addEnumeration ( const EnumerationPtr & st, bool canFail = false );
        bool addFunction ( const FunctionPtr & fn, bool canFail = false );
        bool addGeneric ( const FunctionPtr & fn, bool canFail = false );
        bool addAnnotation ( const AnnotationPtr & ptr, bool canFail = false );
        TypeDeclPtr findAlias ( const string & name ) const;
        VariablePtr findVariable ( const string & name ) const;
        FunctionPtr findFunction ( const string & mangledName ) const;
        StructurePtr findStructure ( const string & name ) const;
        AnnotationPtr findAnnotation ( const string & name ) const;
        EnumerationPtr findEnum ( const string & name ) const;
        ExprCallFactory * findCall ( const string & name ) const;
        bool compileBuiltinModule ( const string & name, unsigned char * str, unsigned int str_len );//will replace last symbol to 0
        static Module * require ( const string & name );
        static void Shutdown();
    public:
        template <typename TT>
        __forceinline void addCall ( const string & fnName ) {
            if ( callThis.find(fnName)!=callThis.end() ) {
                DAS_FATAL_LOG("addCall(%s) failed. duplicate call\n", fnName.c_str());
                DAS_FATAL_ERROR;
            }
            callThis[fnName] = [fnName](const LineInfo & at) {
                return new TT(at, fnName);
            };
        }
    public:
        map<string, TypeDeclPtr>                aliasTypes;
        map<string, AnnotationPtr>              handleTypes;
        map<string, StructurePtr>               structures;
        map<string, EnumerationPtr>             enumerations;
        map<string, VariablePtr>                globals;
        map<string, FunctionPtr>                functions;          // mangled name 2 function name
        map<string, vector<FunctionPtr>>        functionsByName;    // all functions of the same name
        map<string, FunctionPtr>                generics;           // mangled name 2 generic name
        map<string, vector<FunctionPtr>>        genericsByName;     // all generics of the same name
        mutable map<string, ExprCallFactory>    callThis;
        string  name;
        bool    builtIn = false;
    public:
        static intptr_t Karma;
    private:
        Module * next = nullptr;
        static Module * modules;
    };

    #define REGISTER_MODULE(ClassName) \
        das::Module * register_##ClassName () { \
            static ClassName * module_##ClassName = new ClassName(); \
            return module_##ClassName; \
        }

    #define REGISTER_MODULE_IN_NAMESPACE(ClassName,Namespace) \
        das::Module * register_##ClassName () { \
            static Namespace::ClassName * module_##ClassName = new Namespace::ClassName(); \
            return module_##ClassName; \
        }

    #define NEED_MODULE(ClassName) \
        extern das::Module * register_##ClassName (); \
        das::Module::Karma += intptr_t(register_##ClassName());

    class ModuleLibrary {
        friend class Module;
        friend class Program;
    public:
        virtual ~ModuleLibrary() {};
        void addBuiltInModule ();
        void addModule ( Module * module );
        void foreach ( function<bool (Module * module)> && func, const string & name ) const;
        vector<TypeDeclPtr> findAlias ( const string & name ) const;
        vector<AnnotationPtr> findAnnotation ( const string & name ) const;
        vector<EnumerationPtr> findEnum ( const string & name ) const;
        vector<StructurePtr> findStructure ( const string & name ) const;
        Module * findModule ( const string & name ) const;
        TypeDeclPtr makeStructureType ( const string & name ) const;
        TypeDeclPtr makeHandleType ( const string & name ) const;
        TypeDeclPtr makeEnumType ( const string & name ) const;
    protected:
        vector<Module *>                modules;
    };

    struct ModuleGroupUserData {
        ModuleGroupUserData ( const string & n ) : name(n) {}
        virtual ~ModuleGroupUserData() {}
        string name;
    };
    typedef unique_ptr<ModuleGroupUserData> ModuleGroupUserDataPtr;

    class ModuleGroup : public ModuleLibrary {
    public:
        virtual ~ModuleGroup();
        ModuleGroupUserData * getUserData ( const string & dataName ) const;
        bool setUserData ( ModuleGroupUserData * data );
    protected:
        map<string,ModuleGroupUserDataPtr>  userData;
    };

    class DebugInfoHelper {
    public:
        DebugInfoHelper () { debugInfo = make_shared<NodeAllocator>(); }
        DebugInfoHelper ( const shared_ptr<NodeAllocator> & di ) : debugInfo(di) {}
    public:
        TypeInfo * makeTypeInfo ( TypeInfo * info, const TypeDeclPtr & type );
        VarInfo * makeVariableDebugInfo ( const Variable & var );
        StructInfo * makeStructureDebugInfo ( const Structure & st );
        FuncInfo * makeFunctionDebugInfo ( const Function & fn );
        EnumInfo * makeEnumDebugInfo ( const Enumeration & en );
    public:
        shared_ptr<NodeAllocator>   debugInfo;
    protected:
        map<string,StructInfo *>    smn2s;
        map<string,TypeInfo *>      tmn2t;
        map<string,VarInfo *>       vmn2v;
        map<string,FuncInfo *>      fmn2f;
        map<string,EnumInfo *>      emn2e;
    };

    class Program : public enable_shared_from_this<Program> {
    public:
        Program();
        friend TextWriter& operator<< (TextWriter& stream, const Program & program);
        VariablePtr findVariable ( const string & name ) const;
        vector<TypeDeclPtr> findAlias ( const string & name ) const;
        vector<StructurePtr> findStructure ( const string & name ) const;
        vector<AnnotationPtr> findAnnotation ( const string & name ) const;
        vector<EnumerationPtr> findEnum ( const string & name ) const;
        bool addAlias ( const TypeDeclPtr & at );
        bool addVariable ( const VariablePtr & var );
        bool addStructure ( const StructurePtr & st );
        bool addEnumeration ( const EnumerationPtr & st );
        bool addStructureHandle ( const StructurePtr & st, const TypeAnnotationPtr & ann, const AnnotationArgumentList & arg );
        bool addFunction ( const FunctionPtr & fn );
        FunctionPtr findFunction(const string & mangledName) const;
        bool addGeneric ( const FunctionPtr & fn );
        bool addModule ( const string & name );
        void finalizeAnnotations();
        void inferTypes(TextWriter & logs);
        bool optimizationRefFolding();
        bool optimizationConstFolding();
        bool optimizationBlockFolding();
        bool optimizationCondFolding();
        bool optimizationUnused(TextWriter & logs);
        void buildAccessFlags(TextWriter & logs);
        bool verifyAndFoldContracts();
        void optimize(TextWriter & logs);
        void markSymbolUse(bool builtInSym);
        void clearSymbolUse();
        void markOrRemoveUnusedSymbols(bool forceAll = false);
        void allocateStack(TextWriter & logs);
        string dotGraph();
        bool simulate ( Context & context, TextWriter & logs );
        void error ( const string & str, const LineInfo & at, CompilationError cerr = CompilationError::unspecified );
        bool failed() const { return failToCompile; }
        static ExpressionPtr makeConst ( const LineInfo & at, const TypeDeclPtr & type, vec4f value );
        ExprLooksLikeCall * makeCall ( const LineInfo & at, const string & name );
        TypeDecl * makeTypeDeclaration ( const LineInfo & at, const string & name );
        void visit(Visitor & vis, bool visitGenerics = false);
    public:
        template <typename TT>
        string describeCandidates ( const vector<TT> & result, bool needHeader = true ) const {
            if ( !result.size() ) return "";
            TextWriter ss;
            if ( needHeader ) ss << "candidates are:";
            for ( auto & fn : result ) {
                ss << "\n\t";
                if ( fn->module && !fn->module->name.empty() && !(fn->module->name=="$") )
                    ss << fn->module->name << "::";
                ss << fn->describe();
            }
            return ss.str();
        }
    public:
        unique_ptr<Module>          thisModule;
        ModuleLibrary               library;
        ModuleGroup *               thisModuleGroup;
        int                         totalFunctions = 0;
        int                         totalVariables = 0;
        vector<Error>               errors;
        bool                        failToCompile = false;
        uint32_t                    globalInitStackSize = 0;
    public:
        map<CompilationError,int>   expectErrors;
    public:
        AnnotationArgumentList      options;
    };

    // this one works for single module only
    ProgramPtr parseDaScript ( const string & fileName, const FileAccessPtr & access, TextWriter & logs, ModuleGroup & libGroup );

    // this one collectes dependencies and compiles with modules
    ProgramPtr compileDaScript ( const string & fileName, const FileAccessPtr & access, TextWriter & logs, ModuleGroup & libGroup );

    // NOTE: parameters here are unreferenced for a reason
    //            the idea is you copy the function defintion, and paste to your code
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4100)    // unreferenced formal parameter
#elif defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif
    class Visitor {
    protected:
      virtual ~Visitor() {}

    public:
        // ENUMERATOIN
        virtual void preVisit ( Enumeration * enu ) { }
        virtual void preVisitEnumerationValue ( Enumeration * enu, const string & name, int value, bool last ) { }
        virtual EnumerationPtr visit ( Enumeration * enu ) { return enu->shared_from_this(); }
        // STRUCTURE
        virtual void preVisit ( Structure * var ) { }
        virtual void preVisitStructureField ( Structure * var, Structure::FieldDeclaration & decl, bool last ) {}
        virtual void visitStructureField ( Structure * var, Structure::FieldDeclaration & decl, bool last ) {}
        virtual StructurePtr visit ( Structure * var ) { return var->shared_from_this(); }
        // FUNCTON
        virtual void preVisit ( Function * ) {}
        virtual FunctionPtr visit ( Function * that ) { return that->shared_from_this(); }
        virtual void preVisitArgument ( Function * fn, const VariablePtr & var, bool lastArg ) {}
        virtual VariablePtr visitArgument ( Function *, const VariablePtr & that, bool lastArg ) { return that; }
        virtual void preVisitArgumentInit ( Function * fn, const VariablePtr & var, Expression * init ) {}
        virtual ExpressionPtr visitArgumentInit ( Function *, const VariablePtr &, Expression * that ) { return that->shared_from_this(); }
        virtual void preVisitFunctionBody ( Function *,Expression * ) {}
        virtual ExpressionPtr visitFunctionBody ( Function *, Expression * that ) { return that->shared_from_this(); }
        // ANY
        virtual void preVisitExpression ( Expression * expr ) {}
        virtual ExpressionPtr visitExpression ( Expression * expr ) { return expr->shared_from_this(); }
        // BLOCK
        virtual void preVisitBlockArgument ( ExprBlock * block, const VariablePtr & var, bool lastArg ) {}
        virtual VariablePtr visitBlockArgument ( ExprBlock * block, const VariablePtr & var, bool lastArg ) { return var; }
        virtual void preVisitBlockArgumentInit ( ExprBlock * block, const VariablePtr & var, Expression * init ) {}
        virtual ExpressionPtr visitBlockArgumentInit ( ExprBlock * block, const VariablePtr &, Expression * that ) { return that->shared_from_this(); }
        virtual void preVisitBlockExpression ( ExprBlock * block, Expression * expr ) {}
        virtual ExpressionPtr visitBlockExpression (  ExprBlock * block, Expression * expr ) { return expr->shared_from_this(); }
        virtual void preVisitBlockFinal ( ExprBlock * block ) {}
        virtual void preVisitBlockFinalExpression ( ExprBlock * block, Expression * expr ) {}
        virtual ExpressionPtr visitBlockFinalExpression (  ExprBlock * block, Expression * expr ) { return expr->shared_from_this(); }
        // LET
        virtual void preVisitLetStack ( ExprLet * ) {}
        virtual void preVisitLet ( ExprLet * let, const VariablePtr & var, bool last ) {}
        virtual VariablePtr visitLet ( ExprLet * let, const VariablePtr & var, bool last ) { return var; }
        virtual void preVisitLetInit ( ExprLet * let, const VariablePtr & var, Expression * init ) {}
        virtual ExpressionPtr visitLetInit ( ExprLet *, const VariablePtr & var, Expression * that ) { return that->shared_from_this(); }
        // GLOBAL LET
        virtual void preVisitGlobalLet ( const VariablePtr & ) {}
        virtual VariablePtr visitGlobalLet ( const VariablePtr & var ) { return var; }
        virtual void preVisitGlobalLetInit ( const VariablePtr & var, Expression * ) {}
        virtual ExpressionPtr visitGlobalLetInit ( const VariablePtr & var, Expression * that ) { return that->shared_from_this(); }
        // STRING BUILDER
        virtual void preVisit ( ExprStringBuilder * expr ) {}
        virtual void preVisitStringBuilderElement ( ExprStringBuilder * sb, Expression * expr, bool last ) {}
        virtual ExpressionPtr visitStringBuilderElement ( ExprStringBuilder * sb, Expression * expr, bool last ) { return expr->shared_from_this(); }
        virtual ExpressionPtr visit ( ExprStringBuilder * expr ) { return expr->shared_from_this(); }
        // NEW
        virtual void preVisitNewArg ( ExprNew * call, Expression * arg, bool last ) {}
        virtual ExpressionPtr visitNewArg ( ExprNew * call, Expression * arg , bool last ) { return arg->shared_from_this(); }
        // CALL
        virtual void preVisitCallArg ( ExprCall * call, Expression * arg, bool last ) {}
        virtual ExpressionPtr visitCallArg ( ExprCall * call, Expression * arg , bool last ) { return arg->shared_from_this(); }
        // CALL
        virtual void preVisitLooksLikeCallArg ( ExprLooksLikeCall * call, Expression * arg, bool last ) {}
        virtual ExpressionPtr visitLooksLikeCallArg ( ExprLooksLikeCall * call, Expression * arg , bool last ) { return arg->shared_from_this(); }
        // NULL COAELESCING
        virtual void preVisitNullCoaelescingDefault ( ExprNullCoalescing * , Expression * ) {}
        // AT
        virtual void preVisitAtIndex ( ExprAt *, Expression * ) {}
        // OP2
        virtual void preVisitRight ( ExprOp2 *, Expression * ) {}
        // OP3
        virtual void preVisitLeft  ( ExprOp3 *, Expression * ) {}
        virtual void preVisitRight ( ExprOp3 *, Expression * ) {}
        // COPY
        virtual void preVisitRight ( ExprCopy *, Expression * ) {}
        // MOVE
        virtual void preVisitRight ( ExprMove *, Expression * ) {}
        // WITH
        virtual void preVisitWithBody ( ExprWith *, Expression * ) {}
        // WHILE
        virtual void preVisitWhileBody ( ExprWhile *, Expression * ) {}
        // TRY-CATCH
        virtual void preVisitCatch ( ExprTryCatch *, Expression * ) {}
        // IF-THEN-ELSE
        virtual void preVisitIfBlock ( ExprIfThenElse *, Expression * ) {}
        virtual void preVisitElseBlock ( ExprIfThenElse *, Expression * ) {}
        // FOR
        virtual void preVisitFor ( ExprFor * expr, const VariablePtr & var, bool last ) {}
        virtual VariablePtr visitFor ( ExprFor * expr, const VariablePtr & var, bool last ) { return var; }
        virtual void preVisitForStack ( ExprFor * ) {}
        virtual void preVisitForBody ( ExprFor *, Expression * ) {}
        virtual void preVisitForSource ( ExprFor *, Expression *, bool ) {}
        virtual ExpressionPtr visitForSource ( ExprFor *, Expression * that , bool ) { return that->shared_from_this(); }
        // MAKE STRUCTURE
        virtual void preVisitMakeStructureIndex ( ExprMakeStructure * expr, int index, bool lastIndex ) {}
        virtual void visitMakeStructureIndex ( ExprMakeStructure * expr, int index, bool lastField ) {}
        virtual void preVisitMakeStructureField ( ExprMakeStructure * expr, int index, MakeFieldDecl * decl, bool lastField ) {}
        virtual MakeFieldDeclPtr visitMakeStructureField ( ExprMakeStructure * expr, int index, MakeFieldDecl * decl, bool lastField ) {
            return decl->shared_from_this(); }
        // MAKE ARRAY
        virtual void preVisitMakeArrayIndex ( ExprMakeArray * expr, int index, Expression * init, bool lastIndex ) {}
        virtual ExpressionPtr visitMakeArrayIndex ( ExprMakeArray * expr, int index, Expression * init, bool lastField ) {
            return init->shared_from_this();
        }
        // EXPRESSIONS
#define VISIT_EXPR(ExprType) \
        virtual void preVisit ( ExprType * that ) { preVisitExpression(that); } \
        virtual ExpressionPtr visit ( ExprType * that ) { return visitExpression(that); }
        // all visitable expressions
        VISIT_EXPR(ExprRef2Value)
        VISIT_EXPR(ExprPtr2Ref)
        VISIT_EXPR(ExprAddr)
        VISIT_EXPR(ExprNullCoalescing)
        VISIT_EXPR(ExprAssert)
        VISIT_EXPR(ExprStaticAssert)
        VISIT_EXPR(ExprDebug)
        VISIT_EXPR(ExprInvoke)
        VISIT_EXPR(ExprKeys)
        VISIT_EXPR(ExprValues)
        VISIT_EXPR(ExprErase)
        VISIT_EXPR(ExprFind)
        VISIT_EXPR(ExprAscend)
        VISIT_EXPR(ExprNew)
        VISIT_EXPR(ExprDelete)
        VISIT_EXPR(ExprAt)
        VISIT_EXPR(ExprBlock)
        VISIT_EXPR(ExprVar)
        VISIT_EXPR(ExprSwizzle)
        VISIT_EXPR(ExprField)
        VISIT_EXPR(ExprSafeField)
        VISIT_EXPR(ExprOp1)
        VISIT_EXPR(ExprOp2)
        VISIT_EXPR(ExprOp3)
        VISIT_EXPR(ExprCopy)
        VISIT_EXPR(ExprMove)
        VISIT_EXPR(ExprTryCatch)
        VISIT_EXPR(ExprReturn)
        VISIT_EXPR(ExprBreak)
        VISIT_EXPR(ExprConst)
        VISIT_EXPR(ExprFakeContext)
        VISIT_EXPR(ExprConstPtr)
        VISIT_EXPR(ExprConstEnumeration)
        VISIT_EXPR(ExprConstInt64)
        VISIT_EXPR(ExprConstInt)
        VISIT_EXPR(ExprConstInt2)
        VISIT_EXPR(ExprConstInt3)
        VISIT_EXPR(ExprConstInt4)
        VISIT_EXPR(ExprConstUInt64)
        VISIT_EXPR(ExprConstUInt)
        VISIT_EXPR(ExprConstUInt2)
        VISIT_EXPR(ExprConstUInt3)
        VISIT_EXPR(ExprConstUInt4)
        VISIT_EXPR(ExprConstBool)
        VISIT_EXPR(ExprConstFloat)
        VISIT_EXPR(ExprConstFloat2)
        VISIT_EXPR(ExprConstFloat3)
        VISIT_EXPR(ExprConstFloat4)
        VISIT_EXPR(ExprConstString)
        VISIT_EXPR(ExprConstDouble)
        VISIT_EXPR(ExprLet)
        VISIT_EXPR(ExprFor)
        VISIT_EXPR(ExprLooksLikeCall)
        VISIT_EXPR(ExprMakeBlock)
        VISIT_EXPR(ExprMakeLambda)
        VISIT_EXPR(ExprSizeOf)
        VISIT_EXPR(ExprTypeName)
        VISIT_EXPR(ExprCall)
        VISIT_EXPR(ExprIfThenElse)
        VISIT_EXPR(ExprWith)
        VISIT_EXPR(ExprWhile)
        VISIT_EXPR(ExprMakeStructure)
        VISIT_EXPR(ExprMakeArray)
#undef VISIT_EXPR
    };
#if defined(_MSC_VER)
#pragma warning(pop)
#elif defined(__clang__)
#pragma clang diagnostic pop
#endif


    class OptVisitor : public Visitor {
    public:
        bool didAnything () const { return anyFolding; }
    protected:
        void reportFolding();
    private:
        bool anyFolding = false;
    };

    template <typename TT>
    ExpressionPtr ExprLikeCall<TT>::visit(Visitor & vis) {
        vis.preVisit(static_cast<TT *>(this));
        auto llk = ExprLooksLikeCall::visit(vis);
        return llk.get()==this ? vis.visit(static_cast<TT *>(this)) : llk;
    }

    template <typename It, typename SimNodeT, bool first>
    ExpressionPtr ExprTableKeysOrValues<It,SimNodeT,first>::visit(Visitor & vis) {
        vis.preVisit(static_cast<It *>(this));
        auto llk = ExprLooksLikeCall::visit(vis);
        return llk.get()==this ? vis.visit(static_cast<It *>(this)) : llk;
    }

    template <typename It, typename SimNodeT>
    ExpressionPtr ExprArrayCallWithSizeOrIndex<It,SimNodeT>::visit(Visitor & vis) {
        vis.preVisit(static_cast<It *>(this));
        auto llk = ExprLooksLikeCall::visit(vis);
        return llk.get()==this ? vis.visit(static_cast<It *>(this)) : llk;
    }

    template <typename TT, typename ExprConstExt>
    ExpressionPtr ExprConstT<TT,ExprConstExt>::visit(Visitor & vis) {
        vis.preVisit((ExprConst*)this);
        vis.preVisit((ExprConstExt*)this);
        auto res = vis.visit((ExprConstExt*)this);
        if ( res.get()!=this ) return res;
        return vis.visit((ExprConst *)this);
    }
}


