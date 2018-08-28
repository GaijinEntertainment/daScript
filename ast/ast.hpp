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
        tUInt,
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
    
    class Program
    {
    public:
        friend ostream& operator<< (ostream& stream, const Program & program);
        
    public:
        map<string, StructurePtr>    structures;
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
