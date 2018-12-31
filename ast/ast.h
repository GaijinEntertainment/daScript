#pragma once

#include "simulate.h"
#include "vectypes.h"
#include "arraytype.h"
#include "rangetype.h"
#include "function_traits.h"
#include "interop.h"
#include "debug_info.h"
#include "compilation_errors.h"
#include "runtime_table.h"
#include "runtime_array.h"

namespace yzg
{
    using namespace std;
    
    class Structure;
    typedef shared_ptr<Structure> StructurePtr;
    
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
    
    struct TypeDecl : enable_shared_from_this<TypeDecl> {
        TypeDecl() = default;
        TypeDecl(const TypeDecl & decl);
        TypeDecl & operator = (const TypeDecl & decl) = delete;
        TypeDecl(Type tt) : baseType(tt) {}
        friend ostream& operator<< (ostream& stream, const TypeDecl & decl);
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
        bool isVoid() const;
        bool isRef() const;
        bool isRefType() const;
        bool isIndex() const;
        bool isPointer() const;
        bool isHandle() const;
        int getSizeOf() const;
        int getBaseSizeOf() const;
        int getStride() const;
        string describe() const { stringstream ss; ss << *this; return ss.str(); }
        bool canCopy() const;
        bool canMove() const;
        bool isPod() const;
        bool isWorkhorseType() const; // we can return this, or pass this
        bool isRange() const;
        bool isConst() const;
        bool isFoldable() const;
        Type getRangeBaseType() const;
        Type                baseType = Type::tVoid;
        StructurePtr        structType;
        TypeAnnotationPtr   annotation;
        TypeDeclPtr         firstType;      // map.first or array, or pointer
        TypeDeclPtr         secondType;     // map.second
        vector<TypeDeclPtr> argTypes;        // block arguments
        vector<uint32_t>    dim;
        bool                ref = false;
        bool                constant = false;
        LineInfo            at;
    };
    
    template <typename TT> struct ToBasicType;
    template <typename TT> struct ToBasicType<const TT *> : ToBasicType<TT *> {};
    template <typename TT> struct ToBasicType<TT &> : ToBasicType<TT> {};
    template <typename TT> struct ToBasicType<const TT &> : ToBasicType<TT> {};
    template<> struct ToBasicType<Iterator *>   { enum { type = Type::tIterator }; };
    template<> struct ToBasicType<void *>       { enum { type = Type::tPointer }; };
    template<> struct ToBasicType<char *>       { enum { type = Type::tString }; };
    template<> struct ToBasicType<string>       { enum { type = Type::tString }; };
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
    template<> struct ToBasicType<range>        { enum { type = Type::tRange }; };
    template<> struct ToBasicType<urange>       { enum { type = Type::tURange }; };
    template<> struct ToBasicType<Array *>      { enum { type = Type::tArray }; };
    template<> struct ToBasicType<Table *>      { enum { type = Type::tTable }; };
    template<> struct ToBasicType<Block>        { enum { type = Type::tBlock }; };
    template<> struct ToBasicType<Context *>    { enum { type = Type::fakeContext }; };

    template <typename TT>
    struct typeFactory {
        static TypeDeclPtr make(const ModuleLibrary &) {
            auto t = make_shared<TypeDecl>();
            t->baseType = Type( ToBasicType<typename remove_const<TT>::type>::type );
            t->constant = is_const<TT>::value;
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
    struct typeFactory<TT &> {
        static TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = typeFactory<TT>::make(lib);
            t->ref = true;
            return t;
        }
    };

    
    template <typename TT>
    __forceinline TypeDeclPtr makeType(const ModuleLibrary & ctx) {
        return typeFactory<TT>::make(ctx);
    }
    
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
        AnnotationArgument () : type(Type::tVoid) {}
        AnnotationArgument ( const string & n, const string & s ) : type(Type::tString), name(n), sValue(s) {}
        AnnotationArgument ( const string & n, bool  b ) : type(Type::tBool), name(n), bValue(b) {}
        AnnotationArgument ( const string & n, int   i ) : type(Type::tInt), name(n), iValue(i) {}
        AnnotationArgument ( const string & n, float f ) : type(Type::tFloat), name(n), fValue(f) {}
    };
    
    struct AnnotationArgumentList {
        const AnnotationArgument * find ( const string & name, Type type ) const;
		bool getOption(const string & name, bool def = false) const;
        vector<AnnotationArgument>  arguments;
    };
    
    struct Annotation : enable_shared_from_this<Annotation> {
        Annotation ( const string & n ) : name(n) {}
        virtual ~Annotation() {}
        virtual bool rtti_isHandledTypeAnnotation() const { return false; }
        virtual bool rtti_isStructureAnnotation() const { return false; }
        virtual bool rtti_isFunctionAnnotation() const { return false; }
        string describe() const { return name; }
        string      name;
        Module *    module = nullptr;
    };
    
    struct AnnotationDeclaration : enable_shared_from_this<AnnotationDeclaration> {
        AnnotationPtr           annotation;
        AnnotationArgumentList  arguments;
    };
    typedef shared_ptr<AnnotationDeclaration> AnnotationDeclarationPtr;
    
    typedef vector<AnnotationDeclarationPtr> AnnotationList;
    
    class Structure : public enable_shared_from_this<Structure> {
    public:
		struct FieldDeclaration {
			string      name;
			TypeDeclPtr type;
			LineInfo    at;
			int         offset = 0;
			FieldDeclaration() = default;
			FieldDeclaration(const string & n, const TypeDeclPtr & t, const LineInfo & a) : name(n), type(t), at(a) {}
        };
    public:
        Structure ( const string & n ) : name(n) {}
        const FieldDeclaration * findField ( const string & name ) const;
        friend ostream& operator<< (ostream& stream, const Structure & structure);
        int getSizeOf() const;
        bool canCopy() const;
        bool isPod() const;
        string describe() const { return name; }
    public:
        string                      name;
        vector<FieldDeclaration>    fields;
        LineInfo                    at;
        Module *                    module = nullptr;
    };
    
    struct Variable : public enable_shared_from_this<Variable> {
        friend ostream& operator<< (ostream& stream, const Variable & var);
        VariablePtr clone() const;
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
                bool    access_extern : 1;
                bool    access_get : 1;
                bool    access_ref : 1;
                bool    access_init : 1;
            };
            uint32_t flags = 0;
        };
    };
    
    struct FunctionAnnotation : Annotation {
        FunctionAnnotation ( const string & n ) : Annotation(n) {}
        virtual bool rtti_isFunctionAnnotation() const override { return true; }
        virtual bool apply ( const FunctionPtr & func, const AnnotationArgumentList & args, string & err ) = 0;
        virtual bool finalize ( const FunctionPtr & func, const AnnotationArgumentList & args, string & err ) = 0;
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
        virtual bool isNewable() const { return false; }
        virtual bool isIndexable ( const TypeDeclPtr & indexType ) const { return false; }
        virtual bool isIterable ( ) const { return false; }
        virtual size_t getSizeOf() const { return sizeof(void *); }
        virtual TypeDeclPtr makeFieldType ( const string & ) const { return nullptr; }
        virtual TypeDeclPtr makeSafeFieldType ( const string & ) const { return nullptr; }
        virtual TypeDeclPtr makeIndexType ( TypeDeclPtr & ) const { return nullptr; }
        virtual TypeDeclPtr makeIteratorType () const { return nullptr; }
        virtual SimNode * simulateCopy ( Context & context, const LineInfo & at, SimNode * l, SimNode * r ) const { return nullptr; }
        virtual SimNode * simulateRef2Value ( Context & context, const LineInfo & at, SimNode * l ) const { return nullptr; }
        virtual SimNode * simulateGetField ( const string & name, Context &, const LineInfo &, SimNode * ) const { return nullptr; }
        virtual SimNode * simulateSafeGetField ( const string & name, Context &, const LineInfo &, SimNode * ) const { return nullptr; }
        virtual SimNode * simulateSafeGetFieldPtr ( const string & name, Context &, const LineInfo &, SimNode * ) const { return nullptr; }
        virtual SimNode * simulateGetNew ( Context &, const LineInfo & ) const { return nullptr; }
        virtual SimNode * simulateGetAt ( Context &, const LineInfo &, SimNode *, SimNode * ) const { return nullptr; }
        virtual SimNode * simulateGetIterator ( Context &, const LineInfo &, SimNode * ) const { return nullptr; }
        virtual void debug ( stringstream & ss, void * data ) const { ss << "handle<" << name << ">"; }
    };
    
    // annotated structure
    //  needs to override
    //      create,clone, simulateGetField, SafeGetField, and SafeGetFieldPtr
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
        friend ostream& operator<< (ostream& stream, const Expression & func);
        virtual ExpressionPtr visit(Visitor & vis) = 0;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const;
        static ExpressionPtr autoDereference ( const ExpressionPtr & expr );
        virtual SimNode * simulate (Context & context) const = 0;
        virtual bool rtti_isSequence() const { return false; }
        virtual bool rtti_isConstant() const { return false; }
        virtual bool rtti_isStringConstant() const { return false; }
        virtual bool rtti_isCall() const { return false; }
		virtual bool rtti_isLet() const { return false; }
        virtual bool rtti_isReturn() const { return false; }
        virtual bool rtti_isBreak() const { return false; }
        virtual bool rtti_isBlock() const { return false; }
        virtual bool rtti_isVar() const { return false; }
        virtual bool rtti_isField() const { return false; }
        virtual bool rtti_isSafeField() const { return false; }
        virtual bool rtti_isAt() const { return false; }
        virtual bool rtti_isOp3() const { return false; }
        virtual bool rtti_isNullCoalescing() const { return false; }
        virtual bool rtti_isValues() const { return false; }
        virtual Expression * tail() { return this; }
        virtual uint32_t getEvalFlags() const { return 0; }
        LineInfo    at;
        TypeDeclPtr type;
        union {
            struct {
                bool    constexpression : 1;
                bool    noSideEffects : 1;
                bool    topLevel :  1;
                bool    argLevel : 1;
                bool    bottomLevel : 1;
            };
            uint32_t    flags = 0;
        };
    };
    
    template <typename ExprType>
    __forceinline shared_ptr<ExprType> clonePtr ( const ExpressionPtr & expr ) {
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
    
    struct ExprNew : Expression {
        ExprNew() = default;
        ExprNew ( const LineInfo & a, TypeDeclPtr t ) : Expression(a), typeexpr(t) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        TypeDeclPtr     typeexpr;
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
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isBlock() const override { return true; }
        VariablePtr findArgument(const string & name);
        vector<ExpressionPtr>   list;
        bool                    isClosure = false;
        TypeDeclPtr             returnType;
        vector<VariablePtr>     arguments;
        uint32_t                stackTop = 0;
    };
    
    struct ExprVar : Expression {
        ExprVar () = default;
        ExprVar ( const LineInfo & a, const string & n ) : Expression(a), name(n) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
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
        FunctionPtr func;   // always built-in function
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
        virtual bool rtti_isOp3() const override { return true; };
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
    
    struct ExprConst : Expression {
        ExprConst ( Type t ) : baseType(t) {}
        ExprConst ( const LineInfo & a, Type t ) : Expression(a), baseType(t) {}
        virtual SimNode * simulate (Context & context) const override;
        virtual bool rtti_isConstant() const override { return true; }
        __m128  value;
        Type    baseType;
    };
    
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
        ExpressionPtr           filter;
    };
    
    struct ExprLooksLikeCall : Expression {
        ExprLooksLikeCall () = default;
        ExprLooksLikeCall ( const LineInfo & a, const string & n ) : Expression(a), name(n) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        void autoDereference();
        virtual SimNode * simulate (Context & context) const override { return nullptr; }
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
        ExpressionPtr block;
    };
    
    struct ExprInvoke : ExprLikeCall<ExprInvoke> {
        ExprInvoke () = default;
        ExprInvoke ( const LineInfo & a, const string & name ) : ExprLikeCall<ExprInvoke>(a,name) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
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
    
    struct ExprHash : ExprLikeCall<ExprHash> {
        ExprHash () = default;
        ExprHash ( const LineInfo & a, const string & name ) : ExprLikeCall<ExprHash>(a, name) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };
    
    struct ExprArrayPush : ExprLikeCall<ExprArrayPush> {
        ExprArrayPush() = default;
        ExprArrayPush ( const LineInfo & a, const string & name ) : ExprLikeCall<ExprArrayPush>(a, name) {}
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
            return context.makeNode<SimNodeT>(at,subexpr,stride);
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
            return cexpr;
        }
        virtual SimNode * simulate (Context & context) const override {
            auto arr = arguments[0]->simulate(context);
            auto newSize = arguments[1]->simulate(context);
            auto size = arguments[0]->type->firstType->getSizeOf();
            return context.makeNode<SimNodeT>(at,arr,newSize,size);
        }
        virtual ExpressionPtr visit ( Visitor & vis ) override;
    };
    
    struct ExprArrayResize : ExprArrayCallWithSizeOrIndex<ExprArrayResize, SimNode_ArrayResize> {
        ExprArrayResize() = default;
        ExprArrayResize ( const LineInfo & a, const string & n )
            : ExprArrayCallWithSizeOrIndex<ExprArrayResize, SimNode_ArrayResize>(a,n) {}
    };
    
    struct ExprArrayReserve : ExprArrayCallWithSizeOrIndex<ExprArrayReserve, SimNode_ArrayReserve> {
        ExprArrayReserve() = default;
        ExprArrayReserve ( const LineInfo & a, const string & n )
            : ExprArrayCallWithSizeOrIndex<ExprArrayReserve, SimNode_ArrayReserve>(a,n) {}
    };
    
    struct ExprErase : ExprLikeCall<ExprErase> {
        ExprErase() = default;
        ExprErase ( const LineInfo & a, const string & name ) : ExprLikeCall<ExprErase>(a, "erase") {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };
    
    struct ExprFind : ExprLikeCall<ExprFind> {
        ExprFind() = default;
        ExprFind ( const LineInfo & a, const string & name ) : ExprLikeCall<ExprFind>(a, "find") {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };
    
    struct ExprSizeOf : Expression {
        ExprSizeOf () = default;
        ExprSizeOf ( const LineInfo & a, const ExpressionPtr & s )
            : Expression(a), subexpr(s) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        ExpressionPtr   subexpr;
        TypeDeclPtr     typeexpr;
    };

    struct ExprCall : ExprLooksLikeCall {
        ExprCall () = default;
        ExprCall ( const LineInfo & a, const string & n ) : ExprLooksLikeCall(a,n) { }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        FunctionPtr             func;
    };
    
    struct ExprIfThenElse : Expression {
        ExprIfThenElse () = default;
        ExprIfThenElse ( const LineInfo & a, const ExpressionPtr & c,
                        const ExpressionPtr & ift, const ExpressionPtr & iff )
            : Expression(a), cond(c), if_true(ift), if_false(iff) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        ExpressionPtr   cond, if_true, if_false;
    };
    
    struct ExprWhile : Expression {
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual uint32_t getEvalFlags() const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        ExpressionPtr   cond, body;
    };
    
    class Function : public enable_shared_from_this<Function> {
    public:
        virtual ~Function() {}
        friend ostream& operator<< (ostream& stream, const Function & func);
        string getMangledName() const;
        VariablePtr findArgument(const string & name);
        SimNode * simulate (Context & context) const;
        virtual SimNode * makeSimNode ( Context & context ) { return context.makeNode<SimNode_Call>(at); }
        string describe() const { return getMangledName(); }
        virtual FunctionPtr visit(Visitor & vis);
        FunctionPtr sideEffects ( bool hasSideEffects ) { noSideEffects = !hasSideEffects; return shared_from_this(); }
    public:
        AnnotationList      annotations;
        void *              annotationData = nullptr;   // to be filled with annotation
        string              name;
        vector<VariablePtr> arguments;
        TypeDeclPtr         result;
        ExpressionPtr       body;
        int                 index = -1;
        uint32_t            totalStackSize = 0;
        LineInfo            at;
        Module *            module = nullptr;
        union {
            struct {
                bool    builtIn : 1;
                bool    noSideEffects : 1;
            };
            uint32_t flags = 0;
        };
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
        bool addVariable ( const VariablePtr & var );
        bool addStructure ( const StructurePtr & st );
        bool addFunction ( const FunctionPtr & fn );
        bool addAnnotation ( const AnnotationPtr & ptr );
        VariablePtr findVariable ( const string & name ) const;
        FunctionPtr findFunction ( const string & mangledName ) const;
        StructurePtr findStructure ( const string & name ) const;
        AnnotationPtr findAnnotation ( const string & name ) const;
        ExprCallFactory * findCall ( const string & name ) const;
        static Module * require ( const string & name );
        static void Shutdown();
    public:
        template <typename TT>
        __forceinline void addCall ( const string & fnName ) {
            callThis[fnName] = [fnName](const LineInfo & at) { return new TT(at, fnName); };
        }
    public:
        map<string, AnnotationPtr>              handleTypes;
        map<string, StructurePtr>               structures;
        map<string, VariablePtr>                globals;
        map<string, FunctionPtr>                functions;          // mangled name 2 function name
        map<string, vector<FunctionPtr>>        functionsByName;    // all functions of the same name
        mutable map<string, ExprCallFactory>    callThis;
        string name;
    public:
        static intptr_t Karma;
    private:
        Module * next = nullptr;
        static Module * modules;
    };
    
    #define REGISTER_MODULE(ClassName) \
        yzg::Module * register_##ClassName () { \
            static ClassName * module_##ClassName = new ClassName(); \
            return module_##ClassName; \
        }
    
    #define REGISTER_MODULE_IN_NAMESPACE(ClassName,Namespace) \
        yzg::Module * register_##ClassName () { \
            static Namespace::ClassName * module_##ClassName = new Namespace::ClassName(); \
            return module_##ClassName; \
        }
    
    #define NEED_MODULE(ClassName) \
        extern yzg::Module * register_##ClassName (); \
        Module::Karma += intptr_t(register_##ClassName());
    
    class ModuleLibrary {
        friend class Module;
        friend class Program;
    public:
        void addBuiltInModule ();
        void addModule ( Module * module );
        void foreach ( function<bool (Module * module)> && func, const string & name ) const;
        vector<AnnotationPtr> findAnnotation ( const string & name ) const;
        vector<StructurePtr> findStructure ( const string & name ) const;
        TypeDeclPtr makeStructureType ( const string & name ) const;
        TypeDeclPtr makeHandleType ( const string & name ) const;
    protected:
        vector<Module *>                modules;
    };
    
    class Program : public enable_shared_from_this<Program> {
    public:
        Program();
        friend ostream& operator<< (ostream& stream, const Program & program);
        VariablePtr findVariable ( const string & name ) const;
        vector<StructurePtr> findStructure ( const string & name ) const;
        vector<AnnotationPtr> findAnnotation ( const string & name ) const;
        bool addVariable ( const VariablePtr & var );
        bool addStructure ( const StructurePtr & st );
        bool addStructureHandle ( const StructurePtr & st, const TypeAnnotationPtr & ann, const AnnotationArgumentList & arg );
        bool addFunction ( const FunctionPtr & fn );
        void addModule ( Module * pm );
        void finalizeAnnotations();
        void inferTypes();
        bool optimizationRefFolding();
        bool optimizationConstFolding();
        bool optimizationBlockFolding();
        bool optimizationUnused();
        bool staticAsserts();
        void optimize();
        void allocateStack();
		string dotGraph();
        vector<FunctionPtr> findMatchingFunctions ( const string & name, const vector<TypeDeclPtr> & types ) const;
        vector<FunctionPtr> findCandidates ( const string & name, const vector<TypeDeclPtr> & types ) const;
        bool simulate ( Context & context );
        void error ( const string & str, const LineInfo & at, CompilationError cerr = CompilationError::unspecified );
        bool failed() const { return failToCompile; }
        static ExpressionPtr makeConst ( const LineInfo & at, const TypeDeclPtr & type, __m128 value );
        ExprLooksLikeCall * makeCall ( const LineInfo & at, const string & name );
        TypeDecl * makeTypeDeclaration ( const LineInfo & at, const string & name );
        void visit(Visitor & vis);
    public:
        void makeTypeInfo ( TypeInfo * info, Context & context, const TypeDeclPtr & type );
        VarInfo * makeVariableDebugInfo ( Context & context, const Variable & var );
        StructInfo * makeStructureDebugInfo ( Context & context, const Structure & st );
        FuncInfo * makeFunctionDebugInfo ( Context & context, const Function & fn );
    public:
        template <typename TT>
        string describeCandidates ( const vector<TT> & result, bool needHeader = true ) const {
            if ( !result.size() ) return "";
            stringstream ss;
            if ( needHeader ) ss << "candidates are:";
            for ( auto & fn : result ) {
                ss << "\n\t";
                if ( fn->module && !fn->module->name.empty() )
                    ss << fn->module->name << "::";
                ss << fn->describe();
            }
            return ss.str();
        }
    public:
        map<string,StructInfo *>    sdebug;
    public:
        unique_ptr<Module>          thisModule;
        ModuleLibrary               library;
        int                         totalFunctions = 0;
        vector<Error>               errors;
        bool                        failToCompile = false;
    public:
        map<CompilationError,int>   expectErrors;
	public:
		AnnotationArgumentList		options;
    };
         
    ProgramPtr parseDaScript ( const char * script );
    
    class Visitor {
    public:
        // STRUCTURE
        virtual void preVisit ( Structure * var ) { }
        virtual void preVisitStructureField ( Structure * var, Structure::FieldDeclaration & decl, bool last ) {}
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
        virtual void preVisitBlockExpression ( ExprBlock * block, Expression * expr ) {}
        virtual ExpressionPtr visitBlockExpression (  ExprBlock * block, Expression * expr ) { return expr->shared_from_this(); }
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
        virtual void preVisitForFilter ( ExprFor *, Expression * ) {}
        virtual void preVisitForBody ( ExprFor *, Expression * ) {}
        virtual void preVisitForSource ( ExprFor *, Expression *, bool ) {}
        virtual ExpressionPtr visitForSource ( ExprFor *, Expression * that , bool ) { return that->shared_from_this(); }
        // EXPRESSIONS
#define VISIT_EXPR(ExprType) \
        virtual void preVisit ( ExprType * that ) { preVisitExpression(that); } \
        virtual ExpressionPtr visit ( ExprType * that ) { return visitExpression(that); }
        // all visitable expressions
        VISIT_EXPR(ExprRef2Value)
        VISIT_EXPR(ExprPtr2Ref)
        VISIT_EXPR(ExprNullCoalescing)
        VISIT_EXPR(ExprAssert)
        VISIT_EXPR(ExprStaticAssert)
        VISIT_EXPR(ExprDebug)
        VISIT_EXPR(ExprInvoke)
        VISIT_EXPR(ExprHash)
        VISIT_EXPR(ExprKeys)
        VISIT_EXPR(ExprValues)
        VISIT_EXPR(ExprArrayPush)
        VISIT_EXPR(ExprArrayResize)
        VISIT_EXPR(ExprArrayReserve)
        VISIT_EXPR(ExprErase)
        VISIT_EXPR(ExprFind)
        VISIT_EXPR(ExprNew)
        VISIT_EXPR(ExprAt)
        VISIT_EXPR(ExprBlock)
        VISIT_EXPR(ExprVar)
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
        VISIT_EXPR(ExprLet)
        VISIT_EXPR(ExprFor)
        VISIT_EXPR(ExprLooksLikeCall)
        VISIT_EXPR(ExprMakeBlock)
        VISIT_EXPR(ExprSizeOf)
        VISIT_EXPR(ExprCall)
        VISIT_EXPR(ExprIfThenElse)
        VISIT_EXPR(ExprWhile)
#undef VISIT_EXPR
    };
    
    class OptVisitor : public Visitor {
    public:
        bool didAnything () const { return anyFolding; }
    protected:
        void reportFolding() { anyFolding = true; }
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


