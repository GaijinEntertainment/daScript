#pragma once

#include "simulate.h"
#include "vectypes.h"
#include "arraytype.h"
#include "function_traits.h"
#include "interop.h"
#include "debug_info.h"

namespace yzg
{
    using namespace std;
    
    class Structure;
    typedef shared_ptr<Structure> StructurePtr;
    
    class Function;
    typedef shared_ptr<Function> FunctionPtr;
    
    class Variable;
    typedef shared_ptr<Variable> VariablePtr;
    
    class Expression;
    typedef shared_ptr<Expression> ExpressionPtr;
    
    class Program;
    typedef shared_ptr<Program> ProgramPtr;
    
    class TypeDecl;
    typedef shared_ptr<TypeDecl> TypeDeclPtr;
    
    class Module;
    typedef shared_ptr<Module> ModulePtr;
    
    class ModuleLibrary;
    
    enum class Operator
    {
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
        eq,
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
    
    class TypeDecl : public enable_shared_from_this<TypeDecl>
    {
    public:
        TypeDecl() = default;
        TypeDecl(const TypeDecl & decl);
        TypeDecl & operator = (const TypeDecl & decl) = delete;
        TypeDecl(Type tt) : baseType(tt) {}
        friend ostream& operator<< (ostream& stream, const TypeDecl & decl);
        string getMangledName() const;
        bool isSameType ( const TypeDecl & decl, bool refMatters = true ) const;
        bool isIteratorType ( const TypeDecl & decl ) const;
        bool isSimpleType () const;
        bool isSimpleType ( Type typ ) const;
        bool isArray() const;
        bool isGoodArrayType() const;
        bool isVoid() const;
        bool isRef() const;
        bool isIndex() const;
        bool isPointer() const;
        int getSizeOf() const;
        int getBaseSizeOf() const;
        int getStride() const;
        string describe() const { stringstream ss; ss << *this; return ss.str(); }
    public:
        Type                baseType = Type::tVoid;
        Structure *         structType = nullptr;
        TypeDeclPtr         firstType;      // map.first or array
        TypeDeclPtr         secondType;     // map.second
        vector<uint32_t>    dim;
        bool                ref = false;
        LineInfo            at;
    };
    
    template <typename TT>  struct ToBasicType;
    template <typename QQ> struct ToBasicType<QQ &> : ToBasicType<QQ> {};
    template <typename QQ> struct ToBasicType<const QQ &> : ToBasicType<QQ> {};
    template<> struct ToBasicType<void *>   { enum { type = Type::tPointer }; };
    template<> struct ToBasicType<char *>   { enum { type = Type::tString }; };
    template<> struct ToBasicType<const char *> { enum { type = Type::tString }; };
    template<> struct ToBasicType<string>   { enum { type = Type::tString }; };
    template<> struct ToBasicType<bool>     { enum { type = Type::tBool }; };
    template<> struct ToBasicType<int32_t>  { enum { type = Type::tInt }; };
    template<> struct ToBasicType<uint32_t> { enum { type = Type::tUInt }; };
    template<> struct ToBasicType<float>    { enum { type = Type::tFloat }; };
    template<> struct ToBasicType<void>     { enum { type = Type::tVoid }; };
    template<> struct ToBasicType<float2>   { enum { type = Type::tFloat2 }; };
    template<> struct ToBasicType<float3>   { enum { type = Type::tFloat3 }; };
    template<> struct ToBasicType<float4>   { enum { type = Type::tFloat4 }; };
    template<> struct ToBasicType<int2>   { enum { type = Type::tInt2 }; };
    template<> struct ToBasicType<int3>   { enum { type = Type::tInt3 }; };
    template<> struct ToBasicType<int4>   { enum { type = Type::tInt4 }; };
    template<> struct ToBasicType<uint2>   { enum { type = Type::tUInt2 }; };
    template<> struct ToBasicType<uint3>   { enum { type = Type::tUInt3 }; };
    template<> struct ToBasicType<uint4>   { enum { type = Type::tUInt4 }; };
    template<> struct ToBasicType<Array *> { enum { type = Type::tArray }; };

    template <typename TT>
    struct typeFactory {
        static TypeDeclPtr make(const ModuleLibrary &) {
            auto t = make_shared<TypeDecl>();
            t->baseType = Type(ToBasicType<TT>::type);
            t->ref = is_reference<TT>::value;
            return t;
        }
    };
    
    template <typename TT>
    __forceinline TypeDeclPtr makeType(const ModuleLibrary & ctx)
    {
        return typeFactory<TT>::make(ctx);
    }
    
    class Structure : public enable_shared_from_this<Structure>
    {
    public:
        struct FieldDeclaration
        {
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
    public:
        string                      name;
        vector<FieldDeclaration>    fields;
        LineInfo                    at;
    };
    
    class Variable
    {
    public:
        friend ostream& operator<< (ostream& stream, const Variable & var);
        VariablePtr clone() const;
    public:
        string          name;
        TypeDeclPtr     type;
        ExpressionPtr   init;
        LineInfo        at;
        int             index = -1;
        uint32_t        stackTop = 0;
    };
    
    class Expression : public enable_shared_from_this<Expression>
    {
    public:
        struct InferTypeContext
        {
            ProgramPtr              program;
            FunctionPtr             func;
            vector<VariablePtr>     local;
            vector<ExpressionPtr>   loop;
            uint32_t                stackTop = 0;
            void error ( const string & err, const LineInfo & at );
        };
    public:
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
    public:
        LineInfo    at;
        TypeDeclPtr type;
    };
    
    template <typename ExprType>
    __forceinline shared_ptr<ExprType> clonePtr ( const ExpressionPtr & expr ) {
        return expr ? static_pointer_cast<ExprType>(expr) : make_shared<ExprType>();
    }
    
    class ExprRef2Value : public Expression   // &value to value
    {
    public:
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        ExpressionPtr   subexpr;
    };
    
    class ExprPtr2Ref : public Expression   // pointer to &value
    {
    public:
        ExprPtr2Ref () = default;
        ExprPtr2Ref ( const LineInfo & a, ExpressionPtr s ) : Expression(a), subexpr(s) {}
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        ExpressionPtr   subexpr;
    };
    
    class ExprNew : public Expression
    {
    public:
        ExprNew() = default;
        ExprNew ( const LineInfo & a, TypeDeclPtr t ) : Expression(a), typeexpr(t) {}
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        TypeDeclPtr     typeexpr;
    };
    
    class ExprAt : public Expression
    {
    public:
        ExprAt() = default;
        ExprAt ( const LineInfo & a, ExpressionPtr s, ExpressionPtr i ) : Expression(a), subexpr(s), index(i) {}
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        ExpressionPtr   subexpr;
        ExpressionPtr   index;
    };

    class ExprBlock : public Expression
    {
    public:
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        vector<ExpressionPtr>   list;
    };
    
    class ExprVar : public Expression
    {
    public:
        ExprVar () = default;
        ExprVar ( const LineInfo & a, const string & n ) : Expression(a), name(n) {}
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        string      name;
        VariablePtr variable;
        bool        local = false;
        bool        argument = false;
        int         argumentIndex = -1;
    };
    
    class ExprField : public Expression
    {
    public:
        ExprField () = default;
        ExprField ( const LineInfo & a, ExpressionPtr val, const string & n ) : Expression(a), value(val), name(n) {}
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        ExpressionPtr   value;
        string          name;
        const Structure::FieldDeclaration * field = nullptr;
    };
    
    class ExprOp : public Expression
    {
    public:
        ExprOp () = default;
        ExprOp ( const LineInfo & a, Operator o ) : Expression(a), op(o) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
    public:
        Operator        op;
        FunctionPtr     func;   // always built-in function
    };
    
    class ExprOp1 : public ExprOp   // unary    !subexpr
    {
    public:
        ExprOp1 () = default;
        ExprOp1 ( const LineInfo & a, Operator o, ExpressionPtr s ) : ExprOp(a,o), subexpr(s) {}
        virtual void inferType(InferTypeContext & context) override;
        virtual void log(ostream& stream, int depth) const override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        ExpressionPtr   subexpr;
    };
    
    class ExprOp2 : public ExprOp   // binary   left < right
    {
    public:
        ExprOp2 () = default;
        ExprOp2 ( const LineInfo & a, Operator o, ExpressionPtr l, ExpressionPtr r ) : ExprOp(a,o), left(l), right(r) {}
        virtual void inferType(InferTypeContext & context) override;
        virtual void log(ostream& stream, int depth) const override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        ExpressionPtr   left, right;
    };
    
    // this copies one object to the other
    class ExprCopy : public ExprOp2
    {
    public:
        ExprCopy () = default;
        ExprCopy ( const LineInfo & a, ExpressionPtr l, ExpressionPtr r ) : ExprOp2(a, Operator::none, l, r) {};
        virtual void inferType(InferTypeContext & context) override;
        virtual void log(ostream& stream, int depth) const override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };
    
    // this only exists during parsing, and can't be
    class ExprSequence : public ExprOp2
    {
    public:
        ExprSequence ( const LineInfo & a, ExpressionPtr l, ExpressionPtr r ) : ExprOp2(a, Operator::none, l, r) {}
        virtual bool isSequence() const override { return true; }
    };
    
    class ExprOp3 : public ExprOp   // trinary  subexpr ? left : right
    {
    public:
        ExprOp3 () = default;
        ExprOp3 ( const LineInfo & a, Operator o, ExpressionPtr s, ExpressionPtr l, ExpressionPtr r )
            : ExprOp(a,o), subexpr(s), left(l), right(r) {}
        virtual void inferType(InferTypeContext & context) override;
        virtual void log(ostream& stream, int depth) const override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        ExpressionPtr   subexpr, left, right;
    };
    
    class ExprTryCatch : public Expression
    {
    public:
        ExprTryCatch() = default;
        ExprTryCatch ( const LineInfo & a, ExpressionPtr t, ExpressionPtr c ) : Expression(a), try_block(t), catch_block(c) {}
        virtual void inferType(InferTypeContext & context) override;
        virtual void log(ostream& stream, int depth) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        ExpressionPtr try_block, catch_block;
    };
    
    class ExprReturn : public Expression
    {
    public:
        ExprReturn() = default;
        ExprReturn ( const LineInfo & a, ExpressionPtr s ) : Expression(a), subexpr(s) {}
        virtual void inferType(InferTypeContext & context) override;
        virtual void log(ostream& stream, int depth) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        ExpressionPtr subexpr;
    };
    
    class ExprBreak : public Expression
    {
    public:
        ExprBreak() = default;
        ExprBreak ( const LineInfo & a ) : Expression(a) {}
        virtual void inferType(InferTypeContext & context) override;
        virtual void log(ostream& stream, int depth) const override;
        virtual SimNode * simulate (Context & context) const override;
    };
    
    template <typename TT, typename ExprConstExt>
    class ExprConst : public Expression
    {
    public:
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
        }
        virtual SimNode * simulate (Context & context) const override {
            return context.makeNode<SimNode_ConstValue<TT>>(at,value);
        }
        virtual void log(ostream& stream, int depth) const override {
            stream << value;
        }
        TT getValue() const { return value; }
    protected:
        TT  value;
    };
    
    class ExprConstPtr : public ExprConst<void *,ExprConstPtr>
    {
    public:
        ExprConstPtr(void * ptr = nullptr) : ExprConst(ptr) {}
        ExprConstPtr(const LineInfo & a, void * ptr = nullptr) : ExprConst(a,ptr) {}
        virtual void log(ostream& stream, int depth) const override {
            if ( value ) stream << hex << "*0x" << intptr_t(value) << dec; else stream << "nil";
        }
    };

    class ExprConstInt : public ExprConst<int32_t,ExprConstInt>
    {
    public:
        ExprConstInt(int32_t i = 0)  : ExprConst(i) {}
        ExprConstInt(const LineInfo & a, int32_t i = 0)  : ExprConst(a, i) {}
    };
    
    class ExprConstUInt : public ExprConst<uint32_t,ExprConstUInt>
    {
    public:
        ExprConstUInt(uint32_t i = 0) : ExprConst(i) {}
        ExprConstUInt(const LineInfo & a, uint32_t i = 0) : ExprConst(a,i) {}
        virtual void log(ostream& stream, int depth) const override {  stream << "0x" << hex << value << dec; }
    };
    
    class ExprConstBool : public ExprConst<bool,ExprConstBool>
    {
    public:
        ExprConstBool(bool i = false) : ExprConst(i) {}
        ExprConstBool(const LineInfo & a, bool i = false) : ExprConst(a,i) {}
        virtual void log(ostream& stream, int depth) const override {  stream << (value ? "true" : "false"); }
    };

    class ExprConstFloat : public ExprConst<float,ExprConstFloat>
    {
    public:
        ExprConstFloat(float i = 0.0f) : ExprConst(i) {}
        ExprConstFloat(const LineInfo & a, float i = 0.0f) : ExprConst(a,i) {}
        virtual void log(ostream& stream, int depth) const override { stream << to_string_ex(value); }
    };
    
    class ExprConstString : public ExprConst<string,ExprConstString>
    {
    public:
        ExprConstString(const string & str = string()) : ExprConst(unescapeString(str)) {}
        ExprConstString(const LineInfo & a, const string & str = string()) : ExprConst(a,unescapeString(str)) {}
        virtual SimNode * simulate (Context & context) const override {
            char * str = context.allocateName(value);
            return context.makeNode<SimNode_ConstValue<char *>>(at,str);
        }
        virtual void log(ostream& stream, int depth) const override {  stream << "\"" << escapeString(value) << "\""; }
        virtual bool isStringConstant() const override { return true; }
    };
    
    class ExprLet : public Expression
    {
    public:
        Variable * find ( const string & name ) const;
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        static SimNode * simulateInit(Context & context, const VariablePtr & var, bool local);
    public:
        vector<VariablePtr>     variables;
        ExpressionPtr           subexpr;
    };
    
    class ExprLooksLikeCall : public Expression
    {
    public:
        ExprLooksLikeCall () = default;
        ExprLooksLikeCall ( const LineInfo & a, const string & n ) : Expression(a), name(n) {}
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        void autoDereference();
    protected:
        string describe() const;
    public:
        string                  name;
        vector<ExpressionPtr>   arguments;
    };
    typedef function<ExprLooksLikeCall * (const LineInfo & info)> ExprCallFactory;
    
    class ExprAssert : public ExprLooksLikeCall
    {
    public:
        ExprAssert () = default;
        ExprAssert ( const LineInfo & a, const string & name ) : ExprLooksLikeCall(a,name) {}
        virtual void inferType(InferTypeContext & context) override;
        virtual SimNode * simulate (Context & context) const override;
    };
    
    class ExprDebug : public ExprLooksLikeCall
    {
    public:
        ExprDebug () = default;
        ExprDebug ( const LineInfo & a, const string & name ) : ExprLooksLikeCall(a, name) {}
        virtual void inferType(InferTypeContext & context) override;
        virtual SimNode * simulate (Context & context) const override;
    };
    
    class ExprArrayPush : public ExprLooksLikeCall
    {
    public:
        ExprArrayPush() = default;
        ExprArrayPush ( const LineInfo & a, const string & name ) : ExprLooksLikeCall(a, name) {}
        virtual void inferType(InferTypeContext & context) override;
        virtual SimNode * simulate (Context & context) const override;
    };
    
    template <typename SimNodeT>
    class ExprArrayResizeOrReserve : public ExprLooksLikeCall
    {
    public:
        ExprArrayResizeOrReserve() = default;
        ExprArrayResizeOrReserve ( const LineInfo & a, const string & name ) : ExprLooksLikeCall(a, name) {}
        
        void inferType(InferTypeContext & context)
        {
            if ( arguments.size()!=2 ) {
                context.error("expecting (array,size)", at);
            }
            ExprLooksLikeCall::inferType(context);
            auto arrayType = arguments[0]->type;
            auto valueType = arguments[1]->type;
            if ( !arrayType || !valueType ) return;
            if ( !arrayType->isSimpleType(Type::tArray) || !arrayType->firstType ) {
                context.error("first argument must be fully qualified array", at);
                return;
            }
            if ( !valueType->isIndex() )
                context.error("size must be int or uint", at);
            arguments[1] = Expression::autoDereference(arguments[1]);
            type = make_shared<TypeDecl>(Type::tVoid);
        }
        
        SimNode * simulate (Context & context) const
        {
            auto arr = arguments[0]->simulate(context);
            auto newSize = arguments[1]->simulate(context);
            auto size = arguments[0]->type->firstType->getSizeOf();
            return context.makeNode<SimNodeT>(at,arr,newSize,size);
        }
    };

    
    class ExprSizeOf : public Expression
    {
    public:
        ExprSizeOf () = default;
        ExprSizeOf ( const LineInfo & a, ExpressionPtr s ) : Expression(a), subexpr(s) {}
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        ExpressionPtr   subexpr;
        TypeDeclPtr     typeexpr;
    };

    class ExprCall : public ExprLooksLikeCall
    {
    public:
        ExprCall () = default;
        ExprCall ( const LineInfo & a, const string & n ) : ExprLooksLikeCall(a,n) {}
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        FunctionPtr             func;
        uint32_t                stackTop = 0;
    };
    
    class ExprIfThenElse : public Expression
    {
    public:
        ExprIfThenElse () = default;
        ExprIfThenElse ( const LineInfo & a, ExpressionPtr c, ExpressionPtr ift, ExpressionPtr iff )
            : Expression(a), cond(c), if_true(ift), if_false(iff) {}
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        ExpressionPtr   cond, if_true, if_false;
    };
    
    class ExprWhile : public Expression
    {
    public:
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        ExpressionPtr   cond, body;
    };
    
    class ExprForeach : public Expression
    {
    public:
        ExprForeach () = default;
        ExprForeach ( const LineInfo & a ) : Expression(a) {}
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        ExpressionPtr   head, iter, body;
    };
    
    class Function
    {
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
    
    class BuiltInFunction : public Function
    {
    public:
        BuiltInFunction ( const string & fn );
    };
    
    template  <typename SimT, typename RetT, typename ...Args>
    class BuiltInFn : public BuiltInFunction
    {
    public:
        BuiltInFn(const string & fn, const ModuleLibrary & lib) : BuiltInFunction(fn)
        {
            this->result = makeType<RetT>(lib);
            vector<TypeDeclPtr> args = { makeType<Args>(lib)... };
            for ( int argi=0; argi != args.size(); ++argi ) {
                auto arg = make_shared<Variable>();
                arg->name = "arg" + std::to_string(argi);
                arg->type = args[argi];
                this->arguments.push_back(arg);
            }
        }
        virtual SimNode * makeSimNode ( Context & context ) override {
            return context.makeNode<SimT>(at);
        }
    };
    
    struct Error
    {
        Error ( const string & w, LineInfo a ) : what(w), at(a) {}
        __forceinline bool operator < ( const Error & err ) const { return at==err.at ? what < err.what : at<err.at; };
        string      what;
        LineInfo    at;
    };
    
    class Module : public enable_shared_from_this<Module>
    {
    public:
        bool addVariable ( const VariablePtr & var );
        bool addStructure ( const StructurePtr & st );
        bool addFunction ( const FunctionPtr & fn );
        VariablePtr findVariable ( const string & name ) const;
        FunctionPtr findFunction ( const string & mangledName ) const;
        StructurePtr findStructure ( const string & name ) const;
    public:
        map<string, StructurePtr>           structures;
        map<string, VariablePtr>            globals;
        map<string, FunctionPtr>            functions;                  // mangled name 2 function name
        map<string, vector<FunctionPtr>>    functionsByName;    // all functions of the same name
    };
    
    class Module_BuiltIn : public Module
    {
        friend class Program;
    public:
        Module_BuiltIn();
    protected:
        template <typename TT>
        __forceinline void addCall ( const string & fnName ) {
            callThis[fnName] = [fnName](const LineInfo & at) { return new TT(at, fnName); };
        }
    protected:
        map<string,ExprCallFactory> callThis;
    };
    
    class ModuleLibrary
    {
    public:
        void addModule ( const ModulePtr & module ) { modules.push_back(module); }
        void foreach ( function<bool (const ModulePtr & module)> && func ) const;
        VariablePtr findVariable ( const string & name ) const;
        FunctionPtr findFunction ( const string & mangledName ) const;
        StructurePtr findStructure ( const string & name ) const;
        TypeDeclPtr makeStructureType ( const string & name ) const;
    protected:
        vector<ModulePtr>   modules;
    };
    
    class Program : public enable_shared_from_this<Program>
    {
    public:
        Program();
        friend ostream& operator<< (ostream& stream, const Program & program);
        VariablePtr findVariable ( const string & name ) const;
        FunctionPtr findFunction ( const string & mangledName ) const;
        StructurePtr findStructure ( const string & name ) const;
        bool addVariable ( const VariablePtr & var );
        bool addStructure ( const StructurePtr & st );
        bool addFunction ( const FunctionPtr & fn );
        void addModule ( const ModulePtr & pm );
        void inferTypes();
        vector<FunctionPtr> findMatchingFunctions ( const string & name, const vector<TypeDeclPtr> & types ) const;
        void simulate ( Context & context );
        void error ( const string & str, const LineInfo & at );
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
        static ModulePtr            builtInModule;
        ModulePtr                   thisModule;
        ModuleLibrary               library;
        int                         totalFunctions = 0;
        vector<Error>               errors;
        bool                        failToCompile = false;
    };
    
    template  <typename FuncT, FuncT fn>
    class ExternalFn : public BuiltInFunction
    {
        template <typename ArgumentsType, size_t... I>
        __forceinline vector<TypeDeclPtr> makeArgs ( const ModuleLibrary & lib, index_sequence<I...> ) {
            return { makeType< typename tuple_element<I, ArgumentsType>::type>(lib)... };
        }
    public:
        ExternalFn(const string & name, const ModuleLibrary & lib) : BuiltInFunction(name)
        {
            using FunctionTrait = function_traits<FuncT>;
            const int nargs = tuple_size<typename FunctionTrait::arguments>::value;
            using Indices = make_index_sequence<nargs>;
            using Arguments = typename FunctionTrait::arguments;
            using Result  = typename FunctionTrait::return_type;
            auto args = makeArgs<Arguments>(lib, Indices());
            for ( int argi=0; argi!=nargs; ++argi ) {
                auto arg = make_shared<Variable>();
                arg->name = "arg" + std::to_string(argi);
                arg->type = args[argi];
                this->arguments.push_back(arg);
            }
            this->result = makeType<Result>(lib);
            this->totalStackSize = sizeof(Prologue);
        }
        virtual SimNode * makeSimNode ( Context & context ) override {
            auto pCall = context.makeNode<SimNode_ExtFuncCall<FuncT,fn>>(at);
            pCall->info = context.thisProgram->makeFunctionDebugInfo(context, *this);
            return pCall;
        }
    };
    
    template  <InteropFunction func, typename RetT, typename ...Args>
    class InteropFn : public BuiltInFunction
    {
    public:
        InteropFn(const string & name, const ModuleLibrary & lib) : BuiltInFunction(name)
        {
            vector<TypeDeclPtr> args = { makeType<Args>(lib)... };
            for ( int argi=0; argi!=args.size(); ++argi ) {
                auto arg = make_shared<Variable>();
                arg->name = "arg" + std::to_string(argi);
                arg->type = args[argi];
                this->arguments.push_back(arg);
            }
            this->result = makeType<RetT>(lib);
            this->totalStackSize = sizeof(Prologue);
        }
        virtual SimNode * makeSimNode ( Context & context ) override {
            auto pCall = context.makeNode<SimNode_InteropFuncCall<func>>(at);
            pCall->info = context.thisProgram->makeFunctionDebugInfo(context, *this);
            return pCall;
        }
    };
    
    template <typename FuncT, FuncT fn>
    __forceinline void addExtern ( Module & mod, const ModuleLibrary & lib, const string & name ) {
        mod.addFunction(make_shared<ExternalFn<FuncT,fn>>(name,lib));
    }
    
    template <InteropFunction func, typename RetT, typename ...Args>
    __forceinline void addInterop ( Module & mod, const ModuleLibrary & lib, const string & name ) {
        mod.addFunction(make_shared<InteropFn<func,RetT,Args...>>(name,lib));
    }
     
    ProgramPtr parseDaScript ( const char * script, function<void (const ProgramPtr & prg)> && defineContext );
}


