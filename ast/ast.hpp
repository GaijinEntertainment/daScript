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
        tStructure
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
    
    class TypeDecl
    {
    public:
        TypeDecl() = default;
        TypeDecl(const TypeDecl &) = default;
        TypeDecl(Type tt) : baseType(tt) {}
        friend ostream& operator<< (ostream& stream, const TypeDecl & decl);
        string getMangledName() const;
        bool isSameType ( const TypeDecl & decl, bool rvalueMatters = true ) const;
        bool isSimpleType ( Type tp ) const;
        bool isArray() const;
        bool isVoid() const;
        bool isRValue() const;
        bool isIndex() const;
    public:
        Type                baseType = Type::tVoid;
        Structure *         structType = nullptr;
        vector<uint64_t>    dim;
        bool                rvalue = false;
        Node *              at = nullptr;
    };
    typedef shared_ptr<TypeDecl> TypeDeclPtr;
    
    class Structure
    {
    public:
        struct FieldDeclaration
        {
            string      name;
            TypeDeclPtr type;
            Node *      at = nullptr;
        };
    public:
        Structure ( const string & n ) : name(n) {}
        const FieldDeclaration * findField ( const string & name ) const;
        friend ostream& operator<< (ostream& stream, const Structure & structure);
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
    };
    
    class Expression
    {
    public:
        struct InferTypeContext
        {
            ProgramPtr          program;
            FunctionPtr         func;
            vector<VariablePtr> local;
        };
    public:
        friend ostream& operator<< (ostream& stream, const Expression & func);
        virtual void log(ostream& stream, int depth) const = 0;
        virtual void inferType(InferTypeContext & context) = 0;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const;
        void logType(ostream& stream) const;
        static ExpressionPtr autoDereference ( const ExpressionPtr & expr );
    public:
        TypeDeclPtr type;
        Node *      at = nullptr;
    };
    
    class ExprR2L : public Expression
    {
    public:
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
    public:
        ExpressionPtr   subexpr;
    };
    
    class ExprAt : public Expression
    {
    public:
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
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
    public:
        vector<ExpressionPtr>   list;
    };
    
    class ExprVar : public Expression
    {
    public:
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
    public:
        string      name;
        VariablePtr variable;
        bool        local = false;
        bool        argument = false;
    };
    
    class ExprField : public Expression
    {
    public:
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
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
        FunctionPtr     func;
    };
    
    class ExprOp1 : public ExprOp   // unary    !subexpr
    {
    public:
        virtual void inferType(InferTypeContext & context) override;
        virtual void log(ostream& stream, int depth) const override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
    public:
        ExpressionPtr   subexpr;
    };
    
    class ExprOp2 : public ExprOp   // binary   left < right
    {
    public:
        virtual void inferType(InferTypeContext & context) override;
        virtual void log(ostream& stream, int depth) const override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
    public:
        ExpressionPtr   left, right;
    };
    
    class ExprOp3 : public ExprOp   // trinary  subexpr ? left : right
    {
    public:
        virtual void inferType(InferTypeContext & context) override;
        virtual void log(ostream& stream, int depth) const override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
    public:
        ExpressionPtr   subexpr, left, right;
    };
    
    class ExprReturn : public Expression
    {
    public:
        virtual void inferType(InferTypeContext & context) override;
        virtual void log(ostream& stream, int depth) const override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
    public:
        ExpressionPtr   subexpr;
    };
    
    class ExprConst : public Expression
    {
    public:
    };

    class ExprConstInt : public ExprConst
    {
    public:
        ExprConstInt(int64_t i = 0) : value(i) {}
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
    public:
        int64_t value;
    };
    
    class ExprConstUInt : public ExprConst
    {
    public:
        ExprConstUInt(uint64_t i = 0) : value(i) {}
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
    public:
        uint64_t value;
    };

    class ExprConstDouble : public ExprConst
    {
    public:
        ExprConstDouble(double i = 0.0) : value(i) {}
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
    public:
        double value;
    };
    
    class ExprLet : public Expression
    {
    public:
        Variable * find ( const string & name ) const;
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
    public:
        vector<VariablePtr>     variables;
        ExpressionPtr           subexpr;
    };
    
    class ExprCall : public Expression
    {
    public:
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
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
    public:
        ExpressionPtr   cond, if_true, if_false;
    };
    
    class ExprWhile : public Expression
    {
    public:
        virtual void log(ostream& stream, int depth) const override;
        virtual void inferType(InferTypeContext & context) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
    public:
        ExpressionPtr   cond, body;
    };
    
    class Function
    {
    public:
        friend ostream& operator<< (ostream& stream, const Function & func);
        string getMangledName() const;
        VariablePtr findArgument(const string & name);
    public:
        string              name;
        vector<VariablePtr> arguments;
        TypeDeclPtr         result;
        ExpressionPtr       body;
        bool                builtIn = false;
    };
    
    class BuiltInFunction : public Function
    {
    public:
        BuiltInFunction ( const string & fn );
    };
    
    class Program : public enable_shared_from_this<Program>
    {
    public:
        friend ostream& operator<< (ostream& stream, const Program & program);
        VariablePtr findVariable ( const string & name ) const;
        FunctionPtr findFunction ( const string & mangledName ) const;
        void addBuiltIn(FunctionPtr && fn);
        void inferTypes();
        void addBuiltinOperators();
        vector<FunctionPtr> findMatchingFunctions ( const string & name, const vector<TypeDeclPtr> & types ) const;
    public:
        map<string, StructurePtr>   structures;
        map<string, VariablePtr>    globals;
        map<string, FunctionPtr>    functions;      // mangled name 2 function name
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
    
    ProgramPtr parse ( const NodePtr & root );
}

#endif /* ast_hpp */
