//
//  ast.cpp
//  yzg
//
//  Created by Boris Batkin on 8/26/18.
//  Copyright © 2018 Boris Batkin. All rights reserved.
//

#include "ast.hpp"
#include "enums.h"

#include <sstream>

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
    
    string TypeDecl::getMangledName() const
    {
        stringstream ss;
        if ( baseType==Type::tStructure ) {
            ss << structType->name;
        } else {
            ss << g_typeTable.find(baseType);
        }
        if ( constant )
            ss << "#const";
        if ( dim.size() ) {
            for ( auto d : dim ) {
                ss << "#" << d;
            }
        }
        return ss.str();
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
    
    // function
    
    ostream& operator<< (ostream& stream, const Function & func)
    {
        stream << "(defun (" << *func.result << " " << func.name << ")\n"; // //" << func.getMangledName() << "\n";
        for ( auto & decl : func.arguments ) {
            stream << "\t(" << *decl.type << " " << decl.name << ")\n";
        }
        stream << "\t" << *func.body;
        stream << ")\n";
        return stream;
    }
    
    string Function::getMangledName() const
    {
        stringstream ss;
        ss << name;
        for ( auto & arg : arguments ) {
            ss << " " << arg.type->getMangledName();
        }
        ss << "->" << result->getMangledName();
        return ss.str();
    }
    
    Function::Argument * Function::findArgument(const string & name) 
    {
        for ( auto & arg : arguments ) {
            if ( arg.name==name ) {
                return &arg;
            }
        }
        return nullptr;
    }
    
    // expression
    
    ostream& operator<< (ostream& stream, const Expression & func)
    {
        func.log(stream, 1);
        return stream;
    }

    void ExprBlock::log(ostream& stream, int depth) const
    {
        stream << "(";
        for ( int i = 0; i!=list.size(); ++i ) {
            if ( i )
                stream << string(depth, '\t');
            list[i]->log(stream, depth+1);
            if ( i != list.size()-1 )
                stream << "\n";
        }
        stream << ")\n";
    }
    
    void ExprVar::log(ostream& stream, int depth) const
    {
        stream << name;
    }
    
    void ExprOp1::log(ostream& stream, int depth) const
    {
        stream << "(" << to_string(op) << " ";
        subexpr->log(stream, depth);
        stream << ")";
    }
    
    void ExprOp2::log(ostream& stream, int depth) const
    {
        stream << "(" << to_string(op) << " ";
        left->log(stream, depth);
        stream << " ";
        right->log(stream, depth);
        stream << ")";
    }
    
    void ExprOp3::log(ostream& stream, int depth) const
    {
        stream << "(" << to_string(op) << " ";
        subexpr->log(stream, depth);
        stream << " ";
        left->log(stream, depth);
        stream << " ";
        right->log(stream, depth);
        stream << ")";
    }
    
    void ExprReturn::log(ostream& stream, int depth) const
    {
        if ( subexpr ) {
            stream << "(return\n" << string(depth+1, '\t');
            subexpr->log(stream, depth+1);
            stream << ")\n";
        } else {
            stream << "(return)\n";
        }
    }
    
    void ExprConstInt::log(ostream& stream, int depth) const
    {
        stream << value;
    }
    
    void ExprConstUInt::log(ostream& stream, int depth) const
    {
        stream << "0x" << hex << value;
    }
    
    void ExprConstDouble::log(ostream& stream, int depth) const
    {
        stream << to_string_ex(value);
    }

    // ExprLst

    Variable * ExprLet::find(const string & name) const
    {
        for ( auto & v : variables ) {
            if ( v->name==name ) {
                return v.get();
            }
        }
        return nullptr;
    }
    
    void ExprLet::log(ostream& stream, int depth) const
    {
        stream << "(let\n";
        for ( auto & var : variables ) {
            stream << string(depth+1, '\t') << "(" << *var << ")\n";
        }
        stream << string(depth+1, '\t');
        subexpr->log(stream, depth+1);
        stream << ")\n";
    }
    
    // ExprCall
    
    void ExprCall::log(ostream& stream, int depth) const
    {
        stream << "(" << name;
        for ( auto & arg : arguments ) {
            stream << " ";
            arg->log(stream, depth);
        }
        stream << ")";
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
    
    FunctionPtr Program::findFunction ( const string & mangledName ) const
    {
        auto it = functions.find(mangledName);
        if ( it != functions.end() )
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
        for ( const auto & st : program.functions ) {
            stream << *st.second << "\n";
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
    
    /*
     (let
        (Ball g_variable))
     
     (let const
        (bool ext_constant))
     */
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
    
    /*
     */
    ExpressionPtr parseExpression ( const NodePtr & decl, const ProgramPtr & program )
    {
        if ( decl->isList() ) {
            if ( decl->list.size()==0 )
                throw parse_error("empty list is not a valid expression", decl);
            auto & head = decl->list[0];
            if ( head->isList() ) {    // if first element is list, its an expression block
                auto block = make_shared<ExprBlock>();
                for ( auto & subExpr : decl->list ) {
                    if ( !subExpr->isList() )
                        throw parse_error("expression block must contain full expressions", subExpr);
                    auto se = parseExpression(subExpr, program);
                    block->list.emplace_back(se);
                }
                return block;
            } else if ( head->isOperator() ) {
                auto nOp = decl->list.size() - 1;
                if ( nOp==0 )
                    throw parse_error("naked operator", decl);
                if ( nOp==1 ) {
                    if ( !isUnaryOperator(head->op) )
                        throw parse_error("only unary operators can have 1 argument", decl);
                    auto pOp = make_shared<ExprOp1>();
                    pOp->op = head->op;
                    pOp->subexpr = parseExpression(decl->list[1], program);
                    return pOp;
                } else if ( nOp==2 ) {
                    if ( !isBinaryOperator(head->op) )
                        throw parse_error("only binary operators can have 2 arguments", decl);
                    auto pOp = make_shared<ExprOp2>();
                    pOp->op = head->op;
                    pOp->left = parseExpression(decl->list[1], program);
                    pOp->right = parseExpression(decl->list[2], program);
                    return pOp;
                } else if ( nOp==3 ) {
                    if ( !isTrinaryOperator(head->op) )
                        throw parse_error("only trinary operators can have 3 arguments", decl);
                    auto pOp = make_shared<ExprOp3>();
                    pOp->op = head->op;
                    pOp->subexpr = parseExpression(decl->list[1], program);
                    pOp->left = parseExpression(decl->list[2], program);
                    pOp->right = parseExpression(decl->list[3], program);
                    return pOp;
                } else {
                    throw parse_error("operator has too many arguments", decl);
                }
            } else if ( head->isName("return") ) {
                auto pRet = make_shared<ExprReturn>();
                auto nArg = decl->list.size() -  1;
                if ( nArg == 1 ) {
                    pRet->subexpr = parseExpression(decl->list[1], program);
                } else if ( nArg == 0 ) {
                    // nothing to return
                } else {
                    throw parse_error("return has too many operands", decl);
                }
                return pRet;
            } else if ( head->isName("let") ) {
                if ( !decl->isListOfAtLeastSize(3) )
                    throw parse_error("needs at least one variable declaration and expression", decl);
                auto let = make_shared<ExprLet>();
                for ( int iVar = 1; iVar != decl->list.size()-1; ++iVar ) {
                    auto & vdecl = decl->list[iVar];
                    auto pVar = parseVariable(vdecl, program);
                    if ( let->find (pVar->name) )
                        throw parse_error("variable already declared", decl);
                    let->variables.push_back(pVar);
                }
                let->subexpr = parseExpression(decl->list.back(), program);
                return let;
            } else if ( head->isName() ) {
                // function call
                auto call = make_shared<ExprCall>();
                call->name = head->text;
                for ( int i = 1; i != decl->list.size(); ++i ) {
                    auto arg = parseExpression(decl->list[i], program);
                    call->arguments.emplace_back(arg);
                }
                return call;
            } else {
                throw parse_error("unrecognized expression", decl);
            }
        } else if ( decl->isName() ) {
            auto pVar = make_shared<ExprVar>();
            pVar->name = decl->text;
            return pVar;
        } else if ( decl->isNumericConstant() ) {
            if ( decl->type==NodeType::dnumber ) {
                return make_shared<ExprConstDouble>(decl->dnum);
            } else if ( decl->type==NodeType::inumber ) {
                return make_shared<ExprConstInt>(decl->inum);
            } else if ( decl->type==NodeType::unumber ) {
                return make_shared<ExprConstUInt>(decl->unum);
            } else {
                throw parse_error("undefined constant type", decl);
            }
        } else {
            throw parse_error("unrecognized expression", decl);
        }
        return nullptr;
    }
    
    /*
     (defun (int mad)
        (int a)
        (int b)
        (int c)
        (return
                (+ (* a b) c))
     */
    FunctionPtr parseFunction ( const NodePtr & decl, const ProgramPtr & program )
    {
        if ( !decl->isListOfAtLeastSize(3) )
            throw parse_error("function needs name, return type, and body", decl);
        auto func = make_shared<Function>();
        func->name = decl->list[1]->getTailName();
        if ( func->name.empty() )
            throw parse_error("function must have name", decl);
        func->result = parseTypeDeclaratoin(decl->list[1], program);
        for ( int ai = 2; ai < decl->list.size()-1; ++ai ) {
            auto & arg = decl->list[ai];
            auto argName = arg->getTailName();
            if ( argName.empty() )
                throw parse_error("function argument must have name", arg);
            if ( func->findArgument(argName) )
                throw parse_error("function already has argument with this name", arg);
            auto argType = parseTypeDeclaratoin(arg, program);
            func->arguments.push_back({argName, argType});
            // TODO: context and function body
            func->body = parseExpression(decl->list.back(), program);
        }
        // TODO: parse body
        return func;
    }
    
    void parseFunctionDeclarations ( const NodePtr & root, const ProgramPtr & program )
    {
        for ( auto & expr : root->list ) {
            if ( expr->getName(0)=="defun"  ) {
                auto func = parseFunction(expr, program);
                auto mangledName = func->getMangledName();
                if ( program->findFunction(mangledName) )
                    throw parse_error("function already defined", expr);
                program->functions[mangledName] = func;
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
        parseFunctionDeclarations(root, program);
        return program;
    }
    
}
