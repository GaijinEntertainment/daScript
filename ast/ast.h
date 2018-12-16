#pragma once

#include "simulate.h"
#include "vectypes.h"
#include "arraytype.h"
#include "rangetype.h"
#include "function_traits.h"
#include "interop.h"
#include "debug_info.h"
#include "type_annotation.h"
#include "compilation_errors.h"

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
    
    class Module;
    
    class ModuleLibrary;
    
    enum class Operator {
        none,
        // 2-char
        r2l,
        p2r,
        addEqu,
        subEqu,
        divEqu,
        mulEqu,
        modEqu,
        andEqu,
        orEqu,
        xorEqu,
        eqEq,
        lessEqu,
        greaterEqu,
        notEqu,
        binNotEqu,
        inc,
        dec,
        postInc,
        postDec,
        // 1-char
        at,         // @
        dot,        // .
        binand,
        binor,
        binxor,
        add,
        sub,
        div,
        mul,
        mod,
        is,         // ?
        boolNot,    // !
        binNot,     // ~
        less,
        greater
    };
    
    string to_string ( Operator op );
    bool isUnaryOperator ( Operator op );
    bool isBinaryOperator ( Operator op );
    bool isTrinaryOperator ( Operator op );
    
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
        Type getRangeBaseType() const;
        Type                baseType = Type::tVoid;
        Structure *         structType = nullptr;
        TypeAnnotation *    annotation = nullptr;
        TypeDeclPtr         firstType;      // map.first or array
        TypeDeclPtr         secondType;     // map.second
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
            t->baseType = Type( ToBasicType<TT>::type );
            t->ref = is_reference<TT>::value;
            t->constant = is_const<TT>::value;
            return t;
        }
    };
    
    template <typename TT, int dim>
    struct typeFactory<TT[dim]> {
        static TypeDeclPtr make(const ModuleLibrary & lib) {
            auto t = typeFactory<TT *>::make(lib);
            t->dim.push_back(dim);
            t->ref = false;
            return t;
        }
    };
    
    template <typename TT>
    __forceinline TypeDeclPtr makeType(const ModuleLibrary & ctx) {
        return typeFactory<TT>::make(ctx);
    }
    
    class Structure : public enable_shared_from_this<Structure> {
    public:
        struct FieldDeclaration {
            string      name;
            TypeDeclPtr type;
            LineInfo    at;
            int         offset = 0;
        };
    public:
        Structure ( const string & n ) : name(n) {}
        const FieldDeclaration * findField ( const string & name ) const;
        friend ostream& operator<< (ostream& stream, const Structure & structure);
        int getSizeOf() const;
        bool canCopy() const;
        bool isPod() const;
    public:
        string                      name;
        vector<FieldDeclaration>    fields;
        LineInfo                    at;
    };
    
    struct Variable {
        friend ostream& operator<< (ostream& stream, const Variable & var);
        VariablePtr clone() const;
        string          name;
        TypeDeclPtr     type;
        ExpressionPtr   init;
        LineInfo        at;
        int             index = -1;
        uint32_t        stackTop = 0;
    };
    
    struct Expression : enable_shared_from_this<Expression> {
        struct InferTypeContext {
            ProgramPtr              program;
            FunctionPtr             func;
            vector<VariablePtr>     local;
            vector<ExpressionPtr>   loop;
            uint32_t                stackTop = 0;
            void error ( const string & err, const LineInfo & at, CompilationError cerr = CompilationError::unspecified );
        };
        Expression() = default;
        Expression(const LineInfo & a) : at(a) {}
        virtual ~Expression() {}
        friend ostream& operator<< (ostream& stream, const Expression & func);
        virtual void log(ostream& stream, int depth) const = 0;
        virtual void inferType(InferTypeContext & context) = 0;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const;
        void logType(ostream& stream) const;
        static ExpressionPtr autoDereference ( const ExpressionPtr & expr );
        virtual SimNode * simulate (Context & context) const = 0;
        virtual bool isSequence() const { return false; }
        virtual bool isStringConstant() const { return false; }
        virtual bool isCall() const { return false; }
        virtual Expression * tail() { return this; }
        virtual void setBlockReturnsValue() {}
        virtual uint32_t getEvalFlags() const { return 0; }
        LineInfo    at;
        TypeDeclPtr type;
        bool        constexpression = false;
    };

    template <typename ExprType>
    __forceinline shared_ptr<ExprType> clonePtr ( const ExpressionPtr & expr ) {
        return expr ? static_pointer_cast<ExprType>(expr) : make_shared<ExprType>();
    }
    
    struct ExprRef2Value : Expression {
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        ExpressionPtr   subexpr;
    };
    
    struct ExprPtr2Ref : Expression {
        ExprPtr2Ref () = default;
        ExprPtr2Ref ( const LineInfo & a, ExpressionPtr s ) : Expression(a), subexpr(s) {}
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        ExpressionPtr   subexpr;
    };
    
    struct ExprNullCoalescing : ExprPtr2Ref {
        ExprNullCoalescing () = default;
        ExprNullCoalescing ( const LineInfo & a, ExpressionPtr s, ExpressionPtr defVal )
            : ExprPtr2Ref(a,s), defaultValue(defVal) {}
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        ExpressionPtr   defaultValue;
    };
    
    struct ExprNew : Expression {
        ExprNew() = default;
        ExprNew ( const LineInfo & a, TypeDeclPtr t ) : Expression(a), typeexpr(t) {}
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        TypeDeclPtr     typeexpr;
    };
    
    struct ExprAt : Expression {
        ExprAt() = default;
        ExprAt ( const LineInfo & a, ExpressionPtr s, ExpressionPtr i ) : Expression(a), subexpr(s), index(i) {}
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        ExpressionPtr   subexpr, index;
    };

    struct ExprBlock : Expression {
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual void setBlockReturnsValue() override;
        virtual uint32_t getEvalFlags() const override;
        vector<ExpressionPtr>   list;
        bool                    returnsValue = false;
    };
    
    struct ExprVar : Expression {
        ExprVar () = default;
        ExprVar ( const LineInfo & a, const string & n ) : Expression(a), name(n) {}
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        string      name;
        VariablePtr variable;
        bool        local = false;
        bool        argument = false;
        int         argumentIndex = -1;
    };
    
    struct ExprField : Expression {
        ExprField () = default;
        ExprField ( const LineInfo & a, ExpressionPtr val, const string & n ) : Expression(a), value(val), name(n) {}
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        ExpressionPtr   value;
        string          name;
        const Structure::FieldDeclaration * field = nullptr;
    };
    
    struct ExprSafeField : ExprField {
        ExprSafeField () = default;
        ExprSafeField ( const LineInfo & a, ExpressionPtr val, const string & n ) : ExprField(a,val,n) {}
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        bool skipQQ = false;
    };
    
    struct ExprOp : Expression {
        ExprOp () = default;
        ExprOp ( const LineInfo & a, Operator o ) : Expression(a), op(o) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        Operator        op;
        FunctionPtr     func;   // always built-in function
    };
    
    // unary    !subexpr
    struct ExprOp1 : ExprOp {
        ExprOp1 () = default;
        ExprOp1 ( const LineInfo & a, Operator o, ExpressionPtr s ) : ExprOp(a,o), subexpr(s) {}
        virtual void inferType(InferTypeContext & context) override;
        virtual void log(ostream& stream, int depth) const override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual Expression * tail() override { return subexpr->tail(); }
        ExpressionPtr   subexpr;
    };
    
    // binary   left < right
    struct ExprOp2 : ExprOp {
        ExprOp2 () = default;
        ExprOp2 ( const LineInfo & a, Operator o, ExpressionPtr l, ExpressionPtr r ) : ExprOp(a,o), left(l), right(r) {}
        virtual void inferType(InferTypeContext & context) override;
        virtual void log(ostream& stream, int depth) const override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual Expression * tail() override { return right->tail(); }
        ExpressionPtr   left, right;
    };
    
    // this copies one object to the other
    struct ExprCopy : ExprOp2 {
        ExprCopy () = default;
        ExprCopy ( const LineInfo & a, ExpressionPtr l, ExpressionPtr r ) : ExprOp2(a, Operator::none, l, r) {};
        virtual void inferType(InferTypeContext & context) override;
        virtual void log(ostream& stream, int depth) const override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };
    
    // this moves one object to the other
    struct ExprMove : ExprOp2 {
        ExprMove () = default;
        ExprMove ( const LineInfo & a, ExpressionPtr l, ExpressionPtr r ) : ExprOp2(a, Operator::none, l, r) {};
        virtual void inferType(InferTypeContext & context) override;
        virtual void log(ostream& stream, int depth) const override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };
    
    // this only exists during parsing, and can't be
    // and this is why it does not have CLONE
    struct ExprSequence : ExprOp2 {
        ExprSequence ( const LineInfo & a, ExpressionPtr l, ExpressionPtr r ) : ExprOp2(a, Operator::none, l, r) {}
        virtual bool isSequence() const override { return true; }
    };
    
    // trinary  subexpr ? left : right
    struct ExprOp3 : ExprOp {
        ExprOp3 () = default;
        ExprOp3 ( const LineInfo & a, Operator o, ExpressionPtr s, ExpressionPtr l, ExpressionPtr r )
            : ExprOp(a,o), subexpr(s), left(l), right(r) {}
        virtual void inferType(InferTypeContext & context) override;
        virtual void log(ostream& stream, int depth) const override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual Expression * tail() override { return right->tail(); }
        ExpressionPtr   subexpr, left, right;
    };
    
    struct ExprTryCatch : Expression {
        ExprTryCatch() = default;
        ExprTryCatch ( const LineInfo & a, ExpressionPtr t, ExpressionPtr c ) : Expression(a), try_block(t), catch_block(c) {}
        virtual void inferType(InferTypeContext & context) override;
        virtual void log(ostream& stream, int depth) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual uint32_t getEvalFlags() const override;
        ExpressionPtr try_block, catch_block;
    };
    
    struct ExprReturn : Expression {
        ExprReturn() = default;
        ExprReturn ( const LineInfo & a, ExpressionPtr s ) : Expression(a), subexpr(s) {}
        virtual void inferType(InferTypeContext & context) override;
        virtual void log(ostream& stream, int depth) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual uint32_t getEvalFlags() const override { return EvalFlags::stopForReturn; }
        ExpressionPtr subexpr;
    };
    
    struct ExprBreak : Expression {
        ExprBreak() = default;
        ExprBreak ( const LineInfo & a ) : Expression(a) {}
        virtual void inferType(InferTypeContext & context) override;
        virtual void log(ostream& stream, int depth) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual uint32_t getEvalFlags() const override { return EvalFlags::stopForBreak; }
    };
    
    template <typename TT, typename ExprConstExt>
    struct ExprConst : Expression {
        ExprConst(TT val = TT()) : value(val) {}
        ExprConst(const LineInfo & a, TT val = TT()) : Expression(a), value(val) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr ) const override {
            auto cexpr = clonePtr<ExprConstExt>(expr);
            Expression::clone(cexpr);
            cexpr->value = value;
            return cexpr;
        }
        virtual void inferType(InferTypeContext & context) override {
            type = make_shared<TypeDecl>((Type)ToBasicType<TT>::type);
            constexpression = true;
        }
        virtual SimNode * simulate (Context & context) const override {
            return context.makeNode<SimNode_ConstValue<TT>>(at,value);
        }
        virtual void log(ostream& stream, int depth) const override {
            stream << value;
        }
        TT getValue() const { return value; }
        TT  value;
    };
    
    struct ExprConstPtr : ExprConst<void *,ExprConstPtr> {
        ExprConstPtr(void * ptr = nullptr) : ExprConst(ptr) {}
        ExprConstPtr(const LineInfo & a, void * ptr = nullptr) : ExprConst(a,ptr) {}
        virtual void log(ostream& stream, int depth) const override {
            if ( value ) stream << hex << "*0x" << intptr_t(value) << dec; else stream << "null";
        }
    };

    struct ExprConstInt : ExprConst<int32_t,ExprConstInt> {
        ExprConstInt(int32_t i = 0)  : ExprConst(i) {}
        ExprConstInt(const LineInfo & a, int32_t i = 0)  : ExprConst(a, i) {}
    };
    
    struct ExprConstUInt : ExprConst<uint32_t,ExprConstUInt> {
        ExprConstUInt(uint32_t i = 0) : ExprConst(i) {}
        ExprConstUInt(const LineInfo & a, uint32_t i = 0) : ExprConst(a,i) {}
        virtual void log(ostream& stream, int depth) const override {  stream << "0x" << hex << value << dec; }
    };
    
    struct ExprConstBool : ExprConst<bool,ExprConstBool> {
        ExprConstBool(bool i = false) : ExprConst(i) {}
        ExprConstBool(const LineInfo & a, bool i = false) : ExprConst(a,i) {}
        virtual void log(ostream& stream, int depth) const override {  stream << (value ? "true" : "false"); }
    };

    struct ExprConstFloat : ExprConst<float,ExprConstFloat> {
        ExprConstFloat(float i = 0.0f) : ExprConst(i) {}
        ExprConstFloat(const LineInfo & a, float i = 0.0f) : ExprConst(a,i) {}
        virtual void log(ostream& stream, int depth) const override { stream << to_string_ex(value); }
    };
    
    struct ExprConstString : ExprConst<string,ExprConstString> {
        ExprConstString(const string & str = string()) : ExprConst(unescapeString(str)) {}
        ExprConstString(const LineInfo & a, const string & str = string()) : ExprConst(a,unescapeString(str)) {}
        virtual SimNode * simulate (Context & context) const override {
            char * str = context.allocateName(value);
            return context.makeNode<SimNode_ConstValue<char *>>(at,str);
        }
        virtual void log(ostream& stream, int depth) const override {  stream << "\"" << escapeString(value) << "\""; }
        virtual bool isStringConstant() const override { return true; }
    };
    
    struct ExprLet : Expression {
        Variable * find ( const string & name ) const;
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        static SimNode * simulateInit(Context & context, const VariablePtr & var, bool local);
        virtual void setBlockReturnsValue() override;
        virtual uint32_t getEvalFlags() const override;
        vector<VariablePtr>     variables;
        ExpressionPtr           subexpr;
        bool                    returnsValue = false;
    };
    
    // for a,b in foo,bar where a>b ...
    struct ExprFor : Expression {
        ExprFor () = default;
        ExprFor ( const LineInfo & a ) : Expression(a) {}
        Variable * findIterator ( const string & name ) const;
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual uint32_t getEvalFlags() const override;
        vector<string>          iterators;
        vector<VariablePtr>     iteratorVariables;
        vector<ExpressionPtr>   sources;
        ExpressionPtr           subexpr;
        ExpressionPtr           filter;
        uint32_t                fixedSize = 0;
        bool                    dynamicArrays = false;
        bool                    fixedArrays = false;
        bool                    nativeIterators = false;
        bool                    rangeBase = false;
    };
    
    struct ExprLooksLikeCall : Expression {
        ExprLooksLikeCall () = default;
        ExprLooksLikeCall ( const LineInfo & a, const string & n ) : Expression(a), name(n) {}
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        void autoDereference();
        virtual SimNode * simulate (Context & context) const override { return nullptr; }
        string describe() const;
        virtual bool isCall() const override { return true; }
        string                  name;
        vector<ExpressionPtr>   arguments;
        bool                    argumentsFailedToInfer = false;
    };
    typedef function<ExprLooksLikeCall * (const LineInfo & info)> ExprCallFactory;
    
    struct ExprMakeBlock : Expression {
        ExprMakeBlock () = default;
        ExprMakeBlock ( const LineInfo & a, ExpressionPtr b )
            : Expression(a), block(b) { b->at = a; b->setBlockReturnsValue(); }
        virtual void inferType(InferTypeContext & context) override;
        virtual void log(ostream& stream, int depth) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        ExpressionPtr block;
    };
    
    struct ExprInvoke : ExprLooksLikeCall {
        ExprInvoke () = default;
        ExprInvoke ( const LineInfo & a, const string & name ) : ExprLooksLikeCall(a,name) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual SimNode * simulate (Context & context) const override;
    };
    
    struct ExprAssert : ExprLooksLikeCall {
        ExprAssert () = default;
        ExprAssert ( const LineInfo & a, const string & name ) : ExprLooksLikeCall(a,name) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual SimNode * simulate (Context & context) const override;
    };
    
    struct ExprDebug : ExprLooksLikeCall {
        ExprDebug () = default;
        ExprDebug ( const LineInfo & a, const string & name ) : ExprLooksLikeCall(a, name) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual SimNode * simulate (Context & context) const override;
    };
    
    struct ExprHash : ExprLooksLikeCall {
        ExprHash () = default;
        ExprHash ( const LineInfo & a, const string & name ) : ExprLooksLikeCall(a, name) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual SimNode * simulate (Context & context) const override;
    };
    
    struct ExprArrayPush : ExprLooksLikeCall {
        ExprArrayPush() = default;
        ExprArrayPush ( const LineInfo & a, const string & name ) : ExprLooksLikeCall(a, name) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual SimNode * simulate (Context & context) const override;
    };
    
    template <typename SimNodeT, bool first>
    struct ExprTableKeysOrValues : ExprLooksLikeCall {
        ExprTableKeysOrValues() = default;
        ExprTableKeysOrValues ( const LineInfo & a, const string & name ) : ExprLooksLikeCall(a, name) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override {
            auto cexpr = clonePtr<ExprTableKeysOrValues<SimNodeT,first>>(expr);
            return cexpr;
        }
        virtual void inferType(InferTypeContext & context) override {
            type.reset();
            if ( arguments.size()!=1 ) {
                context.error("expecting " + name + "(table)", at);
            }
            ExprLooksLikeCall::inferType(context);
            if ( argumentsFailedToInfer ) return;
            // infer
            auto tableType = arguments[0]->type;
            if ( !tableType->isGoodTableType() ) {
                context.error("first argument must be fully qualified table", at);
                return;
            }
            auto iterType = first ? tableType->firstType : tableType->secondType;
            type = make_shared<TypeDecl>(Type::tIterator);
            type->firstType = make_shared<TypeDecl>(*iterType);
            type->firstType->ref = true;
            type->firstType->constant = first;
        }
        virtual SimNode * simulate (Context & context) const override {
            auto subexpr = arguments[0]->simulate(context);
            auto tableType = arguments[0]->type;
            auto iterType = first ? tableType->firstType : tableType->secondType;
            auto stride = iterType->getSizeOf();
            return context.makeNode<SimNodeT>(at,subexpr,stride);
        }
    };
    
    template <typename SimNodeT>
    struct ExprArrayCallWithSizeOrIndex : ExprLooksLikeCall {
        ExprArrayCallWithSizeOrIndex() = default;
        ExprArrayCallWithSizeOrIndex ( const LineInfo & a, const string & name ) : ExprLooksLikeCall(a, name) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override {
            auto cexpr = clonePtr<ExprArrayCallWithSizeOrIndex<SimNodeT>>(expr);
            return cexpr;
        }
        virtual void inferType(InferTypeContext & context) override {
            type.reset();
            if ( arguments.size()!=2 ) {
                context.error("expecting array and size or index", at);
            }
            ExprLooksLikeCall::inferType(context);
            if ( argumentsFailedToInfer ) return;
            // infer
            auto arrayType = arguments[0]->type;
            auto valueType = arguments[1]->type;
            if ( !arrayType->isGoodArrayType() ) {
                context.error("first argument must be fully qualified array", at);
                return;
            }
            if ( !valueType->isIndex() )
                context.error("size must be int or uint", at);
            arguments[1] = Expression::autoDereference(arguments[1]);
            type = make_shared<TypeDecl>(Type::tVoid);
        }
        virtual SimNode * simulate (Context & context) const override {
            auto arr = arguments[0]->simulate(context);
            auto newSize = arguments[1]->simulate(context);
            auto size = arguments[0]->type->firstType->getSizeOf();
            return context.makeNode<SimNodeT>(at,arr,newSize,size);
        }
    };
    
    struct ExprErase : ExprLooksLikeCall {
        ExprErase() = default;
        ExprErase ( const LineInfo & a, const string & name ) : ExprLooksLikeCall(a, "erase") {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual SimNode * simulate (Context & context) const override;
    };
    
    struct ExprFind : ExprLooksLikeCall {
        ExprFind() = default;
        ExprFind ( const LineInfo & a, const string & name ) : ExprLooksLikeCall(a, "find") {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual SimNode * simulate (Context & context) const override;
    };
    
    struct ExprSizeOf : Expression {
        ExprSizeOf () = default;
        ExprSizeOf ( const LineInfo & a, ExpressionPtr s ) : Expression(a), subexpr(s) {}
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        ExpressionPtr   subexpr;
        TypeDeclPtr     typeexpr;
    };

    struct ExprCall : ExprLooksLikeCall {
        ExprCall () = default;
        ExprCall ( const LineInfo & a, const string & n ) : ExprLooksLikeCall(a,n) { }
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        FunctionPtr             func;
        uint32_t                stackTop = 0;
    };
    
    struct ExprIfThenElse : Expression {
        ExprIfThenElse () = default;
        ExprIfThenElse ( const LineInfo & a, ExpressionPtr c, ExpressionPtr ift, ExpressionPtr iff )
            : Expression(a), cond(c), if_true(ift), if_false(iff) {}
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        ExpressionPtr   cond, if_true, if_false;
    };
    
    struct ExprWhile : Expression {
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual uint32_t getEvalFlags() const override;
        ExpressionPtr   cond, body;
    };
    
    class Function {
    public:
        virtual ~Function() {}
        friend ostream& operator<< (ostream& stream, const Function & func);
        string getMangledName() const;
        VariablePtr findArgument(const string & name);
        SimNode * simulate (Context & context) const;
        virtual SimNode * makeSimNode ( Context & context ) { return context.makeNode<SimNode_Call>(at); }
    public:
        string              name;
        vector<VariablePtr> arguments;
        TypeDeclPtr         result;
        ExpressionPtr       body;
        bool                builtIn = false;
        int                 index = -1;
        uint32_t            totalStackSize = 0;
        LineInfo            at;
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
        bool addVariable ( const VariablePtr & var );
        bool addStructure ( const StructurePtr & st );
        bool addFunction ( const FunctionPtr & fn );
        bool addHandle ( unique_ptr<TypeAnnotation> && ptr );
        VariablePtr findVariable ( const string & name ) const;
        FunctionPtr findFunction ( const string & mangledName ) const;
        StructurePtr findStructure ( const string & name ) const;
        TypeAnnotation * findHandle ( const string & name ) const;
    public:
        map<string, unique_ptr<TypeAnnotation>>  handleTypes;
        map<string, StructurePtr>           structures;
        map<string, VariablePtr>            globals;
        map<string, FunctionPtr>            functions;          // mangled name 2 function name
        map<string, vector<FunctionPtr>>    functionsByName;    // all functions of the same name
        string name;
    };
    
    class Module_BuiltIn : public Module {
        friend class Program;
    public:
        Module_BuiltIn();
    protected:
        template <typename TT>
        __forceinline void addCall ( const string & fnName ) {
            callThis[fnName] = [fnName](const LineInfo & at) { return new TT(at, fnName); };
        }
        void addRuntime(ModuleLibrary & lib);
        map<string,ExprCallFactory> callThis;
    };
    
    class ModuleLibrary {
        friend class Module;
        friend class Program;
    public:
        void addBuiltInModule ();
        void addModule ( Module * module ) { modules.push_back(module); }
        void foreach ( function<bool (Module * module)> && func ) const;
        TypeAnnotation * findHandle ( const string & name ) const;
        VariablePtr findVariable ( const string & name ) const;
        FunctionPtr findFunction ( const string & mangledName ) const;
        StructurePtr findStructure ( const string & name ) const;
        TypeDeclPtr makeStructureType ( const string & name ) const;
        TypeDeclPtr makeHandleType ( const string & name ) const;
        static Module * require ( const string & name );
    protected:
        vector<Module *>                modules;
        static unique_ptr<Module>       builtInModule;
        static map<string,Module *>     requireModules;
    };
    
    class Program : public enable_shared_from_this<Program> {
    public:
        Program();
        friend ostream& operator<< (ostream& stream, const Program & program);
        VariablePtr findVariable ( const string & name ) const;
        FunctionPtr findFunction ( const string & mangledName ) const;
        StructurePtr findStructure ( const string & name ) const;
        TypeAnnotation * findHandle ( const string & name ) const;
        bool addVariable ( const VariablePtr & var );
        bool addStructure ( const StructurePtr & st );
        bool addFunction ( const FunctionPtr & fn );
        void addModule ( Module * pm );
        void inferTypes();
        vector<FunctionPtr> findMatchingFunctions ( const string & name, const vector<TypeDeclPtr> & types ) const;
        vector<FunctionPtr> findCandidates ( const string & name, const vector<TypeDeclPtr> & types ) const;
        string describeCandidates ( vector<FunctionPtr> vec ) const;
        void simulate ( Context & context );
        void error ( const string & str, const LineInfo & at, CompilationError cerr = CompilationError::unspecified );
        bool failed() const { return failToCompile; }
        ExprLooksLikeCall * makeCall ( const LineInfo & at, const string & name );
    public:
        void makeTypeInfo ( TypeInfo * info, Context & context, const TypeDeclPtr & type );
        VarInfo * makeVariableDebugInfo ( Context & context, const Variable & var );
        StructInfo * makeStructureDebugInfo ( Context & context, const Structure & st );
        FuncInfo * makeFunctionDebugInfo ( Context & context, const Function & fn );
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
    };
         
    ProgramPtr parseDaScript ( const char * script, function<void (const ProgramPtr & prg)> && defineContext );
}


