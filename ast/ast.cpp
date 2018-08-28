//
//  ast.cpp
//  yzg
//
//  Created by Boris Batkin on 8/26/18.
//  Copyright © 2018 Boris Batkin. All rights reserved.
//

#include "ast.hpp"
#include "enums.h"

namespace yzg
{
    Enum<Type> g_typeTable = {
        {   Type::tVoid,        "void"  },
        {   Type::tBool,        "bool"  },
        {   Type::tInt,         "int"   },
        {   Type::tInt2,        "int2"  },
        {   Type::tInt3,        "int3"  },
        {   Type::tInt4,        "int4"  },
        {   Type::tUInt,        "uint"  },
        {   Type::tUInt2,       "uint2" },
        {   Type::tUInt3,       "uint3" },
        {   Type::tUInt4,       "uint4" },
        {   Type::tFloat,       "float" },
        {   Type::tFloat2,      "float2"},
        {   Type::tFloat3,      "float3"},
        {   Type::tFloat4,      "float4"}
    };

    string to_string ( Type t )
    {
        return g_typeTable.find(t);
    }
    
    Type nameToBasicType(const string & name)
    {
        return g_typeTable.find(name, Type::none);
    }
    
    // TypeDecl
    
    ostream& operator<< (ostream& stream, const TypeDecl & decl)
    {
        if ( decl.baseType==Type::tStructure ) {
            stream << decl.structType->name;
        } else {
            stream << to_string(decl.baseType);
        }
        if ( decl.constant )
            stream << " const";
        for ( auto d : decl.dim ) {
            stream << " " << d;
        }
        return stream;
    }
    
    // structure
    
    const Structure::FieldDeclaration * Structure::findField ( const string & name ) const
    {
        for ( const auto & fd : fields ) {
            if ( fd.name==name ) {
                return &fd;
            }
        }
        return nullptr;
    }
    
    ostream& operator<< (ostream& stream, const Structure & structure)
    {
        stream << "(struct " << structure.name << "\n";
        for ( auto & decl : structure.fields ) {
            stream << "\t(" << *decl.type << " " << decl.name << ")\n";
        }
        stream << ")\n";
        return stream;
    }

    // variable
    
    ostream& operator<< (ostream& stream, const Variable & var)
    {
        stream << *var.type << " " << var.name;
        return stream;
    }
    
    // program
    
    VariablePtr Program::findVariable ( const string & name ) const
    {
        auto it = constants.find(name);
        if ( it != constants.end() )
            return it->second;
        it = globals.find(name);
        if ( it != globals.end() )
            return it->second;
        return nullptr;
    }
    
    ostream& operator<< (ostream& stream, const Program & program)
    {
        for ( const auto & st : program.structures ) {
            stream << *st.second << "\n";
        }
        if ( program.constants.size() ) {
            stream << "(let const\n";
            for ( auto & pv : program.constants ) {
                stream << "\t(" <<  *pv.second << ")\n";
            }
            stream << ")\n\n";
        }
        if ( program.globals.size() ) {
            stream << "(let\n";
            for ( auto & pv : program.globals ) {
                stream << "\t(" <<  *pv.second << ")\n";
            }
            stream << ")\n\n";
        }
        return stream;
    }
    
    /*
        (int ...)               // integer
        (float 2 ...)           // float[2]
        (blah ..)               // struct blah
        (uint const 3 4 ...)    // const uint [3][4]
        (Sphere ...)
     */
    TypeDeclPtr parseTypeDeclaratoin ( const NodePtr & decl, const ProgramPtr & program )
    {
        auto tdecl = make_shared<TypeDecl>();
        auto typeName = decl->getName(0);
        if ( typeName.empty() )
            throw parse_error("expecting basic type", decl);
        tdecl->baseType = nameToBasicType(typeName);
        if ( tdecl->baseType == Type::none ) {
            auto it = program->structures.find(typeName);
            if ( it == program->structures.end() )
                throw parse_error("expecting type or structure name", decl);
            tdecl->baseType = Type::tStructure;
            tdecl->structType = it->second.get();
        }
        int iDim = 1;
        if ( decl->getName(1)=="const" ) {
            tdecl->constant = true;
            iDim ++;
        }
        for ( ; iDim != decl->list.size()-1; ++iDim ) {
            uint64_t dim = decl->getUnsigned(iDim);
            if ( dim == -1U )
                throw parse_error("expecting dimension", decl);
            tdecl->dim.push_back(dim);
        }
        return tdecl;
    }
    
    /*
     (struct Sphere
        (float3 xyz)
        (float radius))
     
     (struct Ball
        (Sphere sph)
        (float3 color))
     */
    void parseStructureDeclarations ( const NodePtr & root, const ProgramPtr & program )
    {
        // go through all structure declarations, make placeholders in the program
        for ( auto & expr : root->list ) {
            if ( expr->getName(0)=="struct"  ) {
                auto name = expr->getName(1);
                if ( name.empty() )
                    throw parse_error("structure must have a name", expr);
                if ( program->structures.find(name) != program->structures.end() )
                    throw parse_error("structure is already declared", expr);
                program->structures[name] = make_shared<Structure>(name);
            }
        }
        // go through all structures, parse fields
        for ( auto & expr : root->list ) {
            if ( expr->getName(0)=="struct"  ) {
                StructurePtr decl = program->structures.find(expr->getName(1))->second;
                for ( int i = 2; i != expr->list.size(); ++i ) {
                    auto & field = expr->list[i];
                    if ( !field->isListOfAtLeastSize(2) )
                        throw parse_error("structure fields must be lists", field);
                    auto name = field->getTailName();
                    if ( name.empty() )
                        throw parse_error("structure field must have a name", field);
                    if ( decl->findField(name) )
                        throw parse_error("structure field already declared", field);
                    auto typeDecl = parseTypeDeclaratoin(field, program);
                    if ( typeDecl->constant )
                        throw parse_error("structure field can't be constant", field);
                    if ( typeDecl->baseType==Type::tVoid )
                        throw parse_error("structure field can't be void", field);
                    decl->fields.push_back({name, typeDecl});
                }
            }
        }
    }
    
    VariablePtr parseVariable ( const NodePtr & decl, const ProgramPtr & program )
    {
        auto pVar = make_shared<Variable>();
        pVar->type = parseTypeDeclaratoin(decl, program);
        pVar->name = decl->getTailName();
        if ( pVar->name.empty() )
            throw parse_error("variable must have a name", decl);
        return pVar;
    }
    
    void parseVariableDeclarations ( const NodePtr & root, const ProgramPtr & program )
    {
        for ( auto & expr : root->list ) {
            if ( expr->getName(0)=="let"  ) {
                int iVar = 1;
                bool isConst = false;
                if ( expr->getName(1)=="const" ) {
                    iVar ++;
                    isConst = true;
                }
                for ( ; iVar != expr->list.size(); ++iVar ) {
                    auto & vdecl = expr->list[iVar];
                    auto pVar = parseVariable(vdecl, program);
                    pVar->constant = isConst;
                    if ( program->findVariable(pVar->name) )
                        throw parse_error("variable already declared", vdecl);
                    auto & cmap = isConst ? program->constants : program->globals;
                    cmap[pVar->name] = pVar;
                }
            }
        }
    }
    
    
    ProgramPtr parse ( const NodePtr & root )
    {
        if ( !root->isList() )
            return nullptr;
        auto program = make_shared<Program>();
        parseStructureDeclarations(root, program);
        parseVariableDeclarations(root, program);
        return program;
    }
    
}
