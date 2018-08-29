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
    
    enum class Type
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
    
    class TypeDecl
    {
    public:
        friend ostream& operator<< (ostream& stream, const TypeDecl & decl);
        string getMangledName() const;
        
    public:
        Type                baseType;
        Structure *         structType = nullptr;
        vector<uint64_t>    dim;
        bool                constant = false;
    };
    typedef shared_ptr<TypeDecl> TypeDeclPtr;
    
    class Structure
    {
    public:
        struct FieldDeclaration
        {
            string      name;
            TypeDeclPtr type;
        };
        
        Structure ( const string & n ) : name(n) {}
        const FieldDeclaration * findField ( const string & name ) const;
        friend ostream& operator<< (ostream& stream, const Structure & structure);
        
    public:
        string                      name;
        vector<FieldDeclaration>    fields;
    };
    typedef shared_ptr<Structure> StructurePtr;
    
    class Variable
    {
    public:
        friend ostream& operator<< (ostream& stream, const Variable & var);
        
    public:
        string      name;
        TypeDeclPtr type;
        bool        constant = false;
    };
    typedef shared_ptr<Variable> VariablePtr;
    
    class Expression
    {
    public:
        friend ostream& operator<< (ostream& stream, const Expression & func);
    public:
        virtual void log(ostream& stream, int depth) const = 0;
    };
    typedef shared_ptr<Expression> ExpressionPtr;
    
    class ExprBlock : public Expression
    {
    public:
        vector<ExpressionPtr>   list;
    public:
        virtual void log(ostream& stream, int depth) const;
    };
    
    class ExprVar : public Expression
    {
    public:
        string  name;
    public:
        virtual void log(ostream& stream, int depth) const;
    };
    
    class ExprOp : public Expression
    {
    public:
        Operator        op;
    };
    
    class ExprOp1 : public ExprOp   // unary    !subexpr
    {
    public:
        ExpressionPtr   subexpr;
    public:
        virtual void log(ostream& stream, int depth) const;
    };
    
    class ExprOp2 : public ExprOp   // binary   left < right
    {
    public:
        ExpressionPtr   left, right;
    public:
        virtual void log(ostream& stream, int depth) const;
    };
    
    class ExprOp3 : public ExprOp   // trinary  subexpr ? left : right
    {
    public:
        ExpressionPtr   subexpr, left, right;
    public:
        virtual void log(ostream& stream, int depth) const;
    };
    
    class ExprReturn : public Expression
    {
    public:
        ExpressionPtr   subexpr;
    public:
        virtual void log(ostream& stream, int depth) const;
    };
    
    class ExprConst : public Expression
    {
    public:
    };

    class ExprConstInt : public ExprConst
    {
    public:
        ExprConstInt(int64_t i) : value(i) {}
    public:
        int64_t value;
    public:
        virtual void log(ostream& stream, int depth) const;
    };
    
    class ExprConstUInt : public ExprConst
    {
    public:
        ExprConstUInt(uint64_t i) : value(i) {}
    public:
        uint64_t value;
    public:
        virtual void log(ostream& stream, int depth) const;
    };

    class ExprConstDouble : public ExprConst
    {
    public:
        ExprConstDouble(double i) : value(i) {}
    public:
        double value;
    public:
        virtual void log(ostream& stream, int depth) const;
    };
    
    class ExprLet : public Expression
    {
    public:
        Variable * find ( const string & name ) const;
        virtual void log(ostream& stream, int depth) const;
    public:
        vector<VariablePtr>     variables;
        ExpressionPtr           subexpr;
    };
    
    class ExprCall : public Expression
    {
    public:
        virtual void log(ostream& stream, int depth) const;
    public:
        string                  name;
        vector<ExpressionPtr>   arguments;
    };
    
    class Function
    {
    public:
        struct Argument
        {
            string      name;
            TypeDeclPtr type;
        };

        friend ostream& operator<< (ostream& stream, const Function & func);
        string getMangledName() const;
        Argument * findArgument(const string & name);
        
    public:
        string              name;
        vector<Argument>    arguments;
        TypeDeclPtr         result;
        ExpressionPtr       body;
    };
    typedef shared_ptr<Function> FunctionPtr;
    
    class Program
    {
    public:
        friend ostream& operator<< (ostream& stream, const Program & program);
        VariablePtr findVariable ( const string & name ) const;
        FunctionPtr findFunction ( const string & mangledName ) const;
        
    public:
        map<string, StructurePtr>   structures;
        map<string, VariablePtr>    globals;
        map<string, VariablePtr>    constants;
        map<string, FunctionPtr>    functions;      // mangled name 2 function name
    };
    typedef shared_ptr<Program> ProgramPtr;

    class parse_error : public runtime_error
    {
    public:
        parse_error ( const string & message, const NodePtr & error_at ) : runtime_error(message), at(error_at.get()) {}
        Node * at;
    };
    
    ProgramPtr parse ( const NodePtr & root );
}

#endif /* ast_hpp */
