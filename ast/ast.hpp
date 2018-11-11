//
//  ast.hpp
//  yzg
//
//  Created by Boris Batkin on 8/26/18.
//  Copyright © 2018 Boris Batkin. All rights reserved.
//

#ifndef ast_hpp
#define ast_hpp

#include "reader.hpp"
#include "simulate.hpp"
#include "vectypes.h"
#include "function_traits.h"
#include "interop.h"

#include <memory>
#include <vector>
#include <map>
#include <string>

namespace yzg
{
    using namespace std;
    
    enum Type
    {
        none,
        tNull,
        tVoid,
        tBool,
        tInt,
        tInt2,
        tInt3,
        tInt4,
        tUInt,
        tUInt2,
        tUInt3,
        tUInt4,
        tFloat,
        tFloat2,
        tFloat3,
        tFloat4,
        tString,
        tStructure,
        tPointer
    };
    
    string to_string ( Type t );
    Type nameToBasicType(const string & name);
    
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
    
    class TypeDecl
    {
    public:
        TypeDecl() = default;
        TypeDecl(const TypeDecl &) = default;
        TypeDecl(Type tt) : baseType(tt) {}
        friend ostream& operator<< (ostream& stream, const TypeDecl & decl);
        string getMangledName() const;
        bool isSameType ( const TypeDecl & decl, bool rvalueMatters = true ) const;
        bool isIteratorType ( const TypeDecl & decl ) const;
        bool isSimpleType () const;
        bool isSimpleType ( Type typ ) const;
        bool isArray() const;
        bool isVoid() const;
        bool isRValue() const;
        bool isIndex() const;
        bool isPointer() const;
        int getSizeOf() const;
        int getBaseSizeOf() const;
        int getStride() const;
    public:
        Type                baseType = Type::tVoid;
        Structure *         structType = nullptr;
        vector<uint32_t>    dim;
        bool                rvalue = false;
        Node *              at = nullptr;
        TypeDeclPtr         ptrType;
    };
    
    template <typename TT>  struct ToBasicType;
    template <typename QQ> struct ToBasicType<QQ &> : ToBasicType<QQ> {};
    template <typename QQ> struct ToBasicType<const QQ &> : ToBasicType<QQ> {};
    template<> struct ToBasicType<void *>   { enum { type = Type::tPointer }; };
    template<> struct ToBasicType<char *>   { enum { type = Type::tString }; };
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

    template <typename TT>
    struct typeFactory {
        static TypeDeclPtr make(const Program &) {
            auto t = make_shared<TypeDecl>();
            t->baseType = Type(ToBasicType<TT>::type);
            t->rvalue = is_reference<TT>::value;
            return t;
        }
    };
    
    template <typename TT>
    __forceinline TypeDeclPtr makeType(const Program & ctx)
    {
        return typeFactory<TT>::make(ctx);
    }
    
    class Structure
    {
    public:
        struct FieldDeclaration
        {
            string      name;
            TypeDeclPtr type;
            Node *      at = nullptr;
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
        Node *                      at = nullptr;
    };
    
    class Variable
    {
    public:
        friend ostream& operator<< (ostream& stream, const Variable & var);
    public:
        string          name;
        TypeDeclPtr     type;
        ExpressionPtr   init;
        Node *          at = nullptr;
        int             index = -1;
        size_t          stackTop = 0;
    };
    
    class Expression
    {
    public:
        struct InferTypeContext
        {
            ProgramPtr          program;
            FunctionPtr         func;
            vector<VariablePtr> local;
            size_t              stackTop = 0;
        };
    public:
        friend ostream& operator<< (ostream& stream, const Expression & func);
        virtual void log(ostream& stream, int depth) const = 0;
        virtual void inferType(InferTypeContext & context) = 0;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const;
        void logType(ostream& stream) const;
        static ExpressionPtr autoDereference ( const ExpressionPtr & expr );
        virtual SimNode * simulate (Context & context) const = 0;
    public:
        TypeDeclPtr type;
        Node *      at = nullptr;
    };
    
    template <typename ExprType, typename SuperType = Expression>
    __forceinline shared_ptr<ExprType> clonePtr ( const ExpressionPtr & expr ) {
        auto cexpr =  expr ? static_pointer_cast<ExprType>(expr) : make_shared<ExprType>();
        (*cexpr).SuperType::clone(cexpr);
        return cexpr;
    }
    
    class ExprR2L : public Expression   // rvalue to lvalue
    {
    public:
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        ExpressionPtr   subexpr;
    };
    
    class ExprP2R : public Expression   // pointer to rvalue
    {
    public:
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        ExpressionPtr   subexpr;
    };
    
    class ExprSizeOf : public Expression
    {
    public:
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        ExpressionPtr   subexpr;
        TypeDeclPtr     typeexpr;
    };
    
    class ExprAt : public Expression
    {
    public:
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
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        string          name;
        ExpressionPtr   rvalue;
        const Structure::FieldDeclaration * field = nullptr;
    };
    
    class ExprOp : public Expression
    {
    public:
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
    public:
        Operator        op;
        FunctionPtr     func;   // always built-in function
    };
    
    class ExprOp1 : public ExprOp   // unary    !subexpr
    {
    public:
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
        virtual void inferType(InferTypeContext & context) override;
        virtual void log(ostream& stream, int depth) const override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        ExpressionPtr   left, right;
    };
    
    class ExprOp3 : public ExprOp   // trinary  subexpr ? left : right
    {
    public:
        virtual void inferType(InferTypeContext & context) override;
        virtual void log(ostream& stream, int depth) const override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        ExpressionPtr   subexpr, left, right;
    };
    
    class ExprReturn : public Expression
    {
    public:
        virtual void inferType(InferTypeContext & context) override;
        virtual void log(ostream& stream, int depth) const override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        ExpressionPtr   subexpr;
    };
    
    template <typename TT, typename ExprConstExt>
    class ExprConst : public Expression
    {
    public:
        ExprConst(TT val = TT()) : value(val) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr ) const override {
            auto cexpr = clonePtr<ExprConstExt,ExprConst<TT,ExprConstExt>>(expr);
            cexpr->value = value;
            return cexpr;
        }
        virtual void inferType(InferTypeContext & context) override {
            type = make_shared<TypeDecl>((Type)ToBasicType<TT>::type);
        }
        virtual SimNode * simulate (Context & context) const override {
            return context.makeNode<SimNode_ConstValue<TT>>(value);
        }
        virtual void log(ostream& stream, int depth) const override {
            stream << value;
        }
    protected:
        TT  value;
    };

    class ExprConstInt : public ExprConst<int32_t,ExprConstInt>
    {
    public:
        ExprConstInt(int32_t i = 0)  : ExprConst(i) {}
    };
    
    class ExprConstUInt : public ExprConst<uint32_t,ExprConstUInt>
    {
    public:
        ExprConstUInt(uint32_t i = 0) : ExprConst(i) {}
        virtual void log(ostream& stream, int depth) const override {  stream << "0x" << hex << value << dec; }
    };
    
    class ExprConstBool : public ExprConst<bool,ExprConstBool>
    {
    public:
        ExprConstBool(bool i = false) : ExprConst(i) {}
        virtual void log(ostream& stream, int depth) const override {  stream << (value ? "true" : "false"); }
    };

    class ExprConstFloat : public ExprConst<float,ExprConstFloat>
    {
    public:
        ExprConstFloat(float i = 0.0f) : ExprConst(i) {}
        virtual void log(ostream& stream, int depth) const override { stream << to_string_ex(value); }
    };
    
    class ExprConstString : public ExprConst<string,ExprConstString>
    {
    public:
        ExprConstString(const string & str = string()) : ExprConst(unescapeString(str)) {}
        virtual SimNode * simulate (Context & context) const override {
            char * str = context.allocateName(value);
            return context.makeNode<SimNode_ConstValue<char *>>(str);
        }
        virtual void log(ostream& stream, int depth) const override {  stream << "\"" << value << "\""; }   // todo: escape string
    };
    
    class ExprLet : public Expression
    {
    public:
        Variable * find ( const string & name ) const;
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        vector<VariablePtr>     variables;
        ExpressionPtr           subexpr;
        int                     totalInit = 0;
    };
    
    class ExprCall : public Expression
    {
    public:
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    protected:
        string describe() const;
    public:
        string                  name;
        vector<ExpressionPtr>   arguments;
        FunctionPtr             func;
    };
    
    class ExprIfThenElse : public Expression
    {
    public:
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
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        ExpressionPtr   head, iter, body;
    };
    
    class ExprTryCatch : public Expression
    {
    public:
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    public:
        ExpressionPtr   try_this, catch_that;
    };
    
    class Function
    {
    public:
        friend ostream& operator<< (ostream& stream, const Function & func);
        string getMangledName() const;
        VariablePtr findArgument(const string & name);
        SimNode * simulate (Context & context) const;
        virtual SimNode * makeSimNode ( Context & context ) { return context.makeNode<SimNode_Call>(); }
    public:
        string              name;
        vector<VariablePtr> arguments;
        TypeDeclPtr         result;
        ExpressionPtr       body;
        bool                builtIn = false;
        int                 index = -1;
        size_t              totalStackSize = 0;
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
        BuiltInFn(const string & fn, const Program & prg) : BuiltInFunction(fn)
        {
            this->result = makeType<RetT>(prg);
            vector<TypeDeclPtr> args = { makeType<Args>(prg)... };
            for ( int argi=0; argi != args.size(); ++argi ) {
                auto arg = make_shared<Variable>();
                arg->name = "arg" + std::to_string(argi);
                arg->type = args[argi];
                this->arguments.push_back(arg);
            }
        }
        virtual SimNode * makeSimNode ( Context & context ) override {
            return context.makeNode<SimT>();
        }
    };
    
    template  <typename FuncT, FuncT fn>
    class ExternalFn : public BuiltInFunction
    {
        template <typename ArgumentsType, size_t... I>
        __forceinline vector<TypeDeclPtr> makeArgs ( const Program & prg, index_sequence<I...> ) {
            return { makeType< typename tuple_element<I, ArgumentsType>::type>(prg)... };
        }
    public:
        ExternalFn(const string & name, const Program & prg) : BuiltInFunction(name)
        {
            using FunctionTrait = function_traits<FuncT>;
            const int nargs = tuple_size<typename FunctionTrait::arguments>::value;
            using Indices = make_index_sequence<nargs>;
            using Arguments = typename FunctionTrait::arguments;
            using Result  = typename FunctionTrait::return_type;
            auto args = makeArgs<Arguments>(prg, Indices());
            for ( int argi=0; argi!=nargs; ++argi ) {
                auto arg = make_shared<Variable>();
                arg->name = "arg" + std::to_string(argi);
                arg->type = args[argi];
                this->arguments.push_back(arg);
            }
            this->result = makeType<Result>(prg);
        }
        virtual SimNode * makeSimNode ( Context & context ) override {
            return context.makeNode<SimNode_ExtFuncCall<FuncT,fn>>();
        }
    };
    
    class Program : public enable_shared_from_this<Program>
    {
    public:
        friend ostream& operator<< (ostream& stream, const Program & program);
        VariablePtr findVariable ( const string & name ) const;
        FunctionPtr findFunction ( const string & mangledName ) const;
        StructurePtr findStructure ( const string & name ) const;
        TypeDeclPtr makeStructureType ( const string & name ) const;
        void addBuiltIn(FunctionPtr && fn);
        void inferTypes();
        void addBuiltinOperators();
        void addBuiltinFunctions();
        vector<FunctionPtr> findMatchingFunctions ( const string & name, const vector<TypeDeclPtr> & types ) const;
        void simulate ( Context & context );
        
        template <typename FuncT, FuncT fn>
        void addExtern ( const string & name ) {
            addBuiltIn(make_shared<ExternalFn<FuncT,fn>>(name, *this));
        }
        
    public:
        map<string, StructurePtr>   structures;
        map<string, VariablePtr>    globals;
        map<string, FunctionPtr>    functions;                  // mangled name 2 function name
        map<string, vector<FunctionPtr>>    functionsByName;    // all functions of the same name
        int                         totalFunctions = 0;
    };

    class parse_error : public runtime_error
    {
    public:
        parse_error ( const string & message, const NodePtr & error_at ) : runtime_error(message), at(error_at.get()) {}
        Node * at = nullptr;
    };
    
    class semantic_error : public runtime_error
    {
    public:
        semantic_error ( const string & message, Node * error_at ) : runtime_error(message), at(error_at) {}
        Node * at = nullptr;
    };
    
    ProgramPtr parse ( const NodePtr & root, function<void (const ProgramPtr & prg)> && defineContext );
}

#endif /* ast_hpp */
