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
#include <iostream>

namespace yzg
{
    bool g_logTypes = false;
    
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
        for ( auto d : decl.dim ) {
            stream << " " << d;
        }
        if ( decl.rvalue )
            stream << " &";
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
        if ( rvalue )
            ss << "#rvalue";
        if ( dim.size() ) {
            for ( auto d : dim ) {
                ss << "#" << d;
            }
        }
        return ss.str();
    }
    
    bool TypeDecl::isSameType ( const TypeDecl & decl, bool rvalueMatters ) const
    {
        if ( baseType!=decl.baseType )
            return false;
        if ( baseType==Type::tStructure && structType!=decl.structType )
            return false;
        if ( dim!=decl.dim )
            return false;
        if ( rvalueMatters )
            if ( rvalue!=decl.rvalue )
                return false;
        return true;
    }
    
    bool TypeDecl::isVoid() const
    {
        return (baseType==Type::tVoid) && (dim.size()==0);
    }
    
    bool TypeDecl::isSimpleType ( Type tp ) const
    {
        if (    baseType==Type::none
            ||  baseType==Type::tVoid
            ||  baseType==Type::tStructure )
            return false;
        if ( dim.size() )
            return false;
        return true;
    }
    
    bool TypeDecl::isArray() const
    {
        return dim.size() != 0;
    }
    
    bool TypeDecl::isRValue() const
    {
        return rvalue || baseType==Type::tStructure;
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
        stream << ")";
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
            stream << "\t(" << *decl->type << " " << decl->name;
            if ( decl->init )
                stream << " " << *decl->init;
            stream << ")\n";
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
            ss << " " << arg->type->getMangledName();
        }
        // ss << "->" << result->getMangledName();
        return ss.str();
    }
    
    VariablePtr Function::findArgument(const string & name)
    {
        for ( auto & arg : arguments ) {
            if ( arg->name==name ) {
                return arg;
            }
        }
        return nullptr;
    }
    
    // built-in function
    
    BuiltInFunction::BuiltInFunction ( const string & fn )
    {
        builtIn = true;
        name = fn;
    }
    
    template <typename TT>  struct ToBasicType;
    template <typename QQ> struct ToBasicType<QQ &> : ToBasicType<QQ> {};
    template<> struct ToBasicType<bool>     { enum { type = Type::tBool }; };
    template<> struct ToBasicType<int>      { enum { type = Type::tInt }; };
    template<> struct ToBasicType<float>    { enum { type = Type::tFloat }; };
    
    template <typename TT>
    TypeDeclPtr makeType()
    {
        auto t = make_shared<TypeDecl>();
        t->baseType = Type(ToBasicType<TT>::type);
        t->rvalue = is_reference<TT>::value;
        return t;
    }
    
    template <typename ArgT, typename RetT>
    class BuiltInOp1 : public BuiltInFunction
    {
    public:
        BuiltInOp1(const string & fn) : BuiltInFunction(fn)
        {
            result = makeType<RetT>();
            auto arg = make_shared<Variable>();
            arg->name = "arg";
            arg->type = makeType<ArgT>();
            arguments.push_back(arg);
        }
    };
    
    template <typename ArgT1, typename ArgT2, typename RetT>
    class BuiltInOp2 : public BuiltInFunction
    {
    public:
        BuiltInOp2(const string & fn) : BuiltInFunction(fn)
        {
            result = makeType<RetT>();
            auto arg1 = make_shared<Variable>();
            arg1->name = "arg1";
            arg1->type = makeType<ArgT1>();
            arguments.push_back(arg1);
            auto arg2 = make_shared<Variable>();
            arg2->name = "arg2";
            arg2->type = makeType<ArgT2>();
            arguments.push_back(arg2);
        }
    };
    
    // expression
    
    template <typename ExprType, typename SuperType = Expression>
    shared_ptr<ExprType> clonePtr ( const ExpressionPtr & expr )
    {
        auto cexpr =  expr ? static_pointer_cast<ExprType>(expr) : make_shared<ExprType>();
        (*cexpr).SuperType::clone(cexpr);
        return cexpr;
    }
    
    ExpressionPtr Expression::clone( const ExpressionPtr & expr ) const
    {
        if ( !expr )
            throw semantic_error("unsupported expression", at);
        expr->at = at;
        expr->type = type ? make_shared<TypeDecl>(*type) : nullptr;
        return expr;
    }
    
    ExpressionPtr Expression::autoDereference ( const ExpressionPtr & expr )
    {
        if ( expr->type->isRValue() ) {
            auto ar2l = make_shared<ExprR2L>();
            ar2l->subexpr = expr;
            ar2l->at = expr->at;
            ar2l->type = make_shared<TypeDecl>(*expr->type);
            ar2l->type->rvalue = false;
            return ar2l;
        } else {
            return expr;
        }
    }
    
    void Expression::logType(ostream& stream) const
    {
        if ( g_logTypes )
            stream << "$ (" << *type << ") ";
    }
    
    ostream& operator<< (ostream& stream, const Expression & expr)
    {
        expr.log(stream, 1);
        return stream;
    }
    
    // ExprR2L
    
    ExpressionPtr ExprR2L::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprR2L>(expr);
        cexpr->subexpr = subexpr->clone();
        return cexpr;
    }
    
    void ExprR2L::log(ostream& stream, int depth) const
    {
        stream << "(-> " << *subexpr << ")";
    }
    
    void ExprR2L::inferType(InferTypeContext & context)
    {
        subexpr->inferType(context);
        if ( !subexpr->type->isRValue() )
            throw semantic_error("can only dereference rvalue", at);
        type = make_shared<TypeDecl>(*subexpr->type);
        type->rvalue = false;
    }

    // ExprBlock
    
    ExpressionPtr ExprBlock::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprBlock>(expr);
        for ( auto & subexpr : list ) {
            cexpr->list.push_back(subexpr->clone());
        }
        return cexpr;
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
        stream << ")";
    }
    
    void ExprBlock::inferType(InferTypeContext & context)
    {
        type = make_shared<TypeDecl>();
        for ( auto & ex : list ) {
            ex->inferType(context);
        }
    }
    
    // ExprField
    
    ExpressionPtr ExprField::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprField>(expr);
        cexpr->name = name;
        cexpr->rvalue = rvalue->clone();
        cexpr->field = field;
        return cexpr;
    }
    
    void ExprField::log(ostream& stream, int depth) const
    {
        logType(stream);
        stream << "(. ";
        rvalue->log(stream,depth+1);
        stream << " " << name << ")";
    }
    
    void ExprField::inferType(InferTypeContext & context)
    {
        rvalue->inferType(context);
        if ( rvalue->type->baseType!=Type::tStructure )
            throw semantic_error("expecting structure", at);
        if ( rvalue->type->isArray() )
            throw semantic_error("can't get field of array", at);
        field = rvalue->type->structType->findField(name);
        if ( !field )
            throw semantic_error("field " + name + " not found", at);
        type = make_shared<TypeDecl>(*field->type);
        type->rvalue = rvalue->type->isRValue();
    }
    
    // ExprVar
    
    ExpressionPtr ExprVar::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprVar>(expr);
        cexpr->name = name;
        cexpr->variable = variable; // todo: lookup again?
        cexpr->local = local;
        cexpr->argument = argument;
        return cexpr;
    }
    
    void ExprVar::log(ostream& stream, int depth) const
    {
        logType(stream);
        stream << name;
    }
    
    void ExprVar::inferType(InferTypeContext & context)
    {
        // local (that on the stack)
        for ( auto it = context.local.rbegin(); it!=context.local.rend(); ++it ) {
            auto var = *it;
            if ( var->name==name ) {
                variable = var;
                local = true;
                type = make_shared<TypeDecl>(*var->type);
                type->rvalue = true;
                return;
            }
        }
        // function argument
        for ( auto & arg : context.func->arguments ) {
            if ( arg->name==name ) {
                variable = arg;
                argument = true;
                type = make_shared<TypeDecl>(*arg->type);
                return;
            }
        }
        // global
        auto var = context.program->findVariable(name);
        if ( !var )
            throw semantic_error("can't locate variable " + name, at);
        variable = var;
        type = make_shared<TypeDecl>(*var->type);
        type->rvalue = true;
    }
    
    // ExprOp
    
    ExpressionPtr ExprOp::clone( const ExpressionPtr & expr ) const
    {
        if ( !expr )
            throw semantic_error("can't clone ExprOp", at);
        auto cexpr = static_pointer_cast<ExprOp>(expr);
        cexpr->op = op;
        cexpr->func = func;
        return cexpr;
    }
    
    // ExprOp1
    
    ExpressionPtr ExprOp1::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprOp1,ExprOp>(expr);
        cexpr->subexpr = subexpr->clone();
        return cexpr;
    }
    
    void ExprOp1::log(ostream& stream, int depth) const
    {
        logType(stream);
        stream << "(" << to_string(op) << " ";
        subexpr->log(stream, depth);
        stream << ")";
    }
    
    void ExprOp1::inferType(InferTypeContext & context)
    {
        subexpr->inferType(context);
        vector<TypeDeclPtr> types = { subexpr->type };
        auto functions = context.program->findMatchingFunctions(to_string(op), types);
        if ( functions.size()==0 )
            throw semantic_error("no matching function", at);
        if ( functions.size()>1 )
            throw semantic_error("too many matching functions", at);
        func = functions[0];
        type = make_shared<TypeDecl>(*func->result);
        if ( !func->arguments[0]->type->isRValue() )
            subexpr = autoDereference(subexpr);
    }
    
    // ExprOp2
    
    ExpressionPtr ExprOp2::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprOp2,ExprOp>(expr);
        cexpr->left = left->clone();
        cexpr->right = right->clone();
        return cexpr;
    }
    
    void ExprOp2::log(ostream& stream, int depth) const
    {
        logType(stream);
        stream << "(" << to_string(op) << " ";
        left->log(stream, depth);
        stream << " ";
        right->log(stream, depth);
        stream << ")";
    }
    
    void ExprOp2::inferType(InferTypeContext & context)
    {
        left->inferType(context);
        right->inferType(context);
        vector<TypeDeclPtr> types = { left->type, right->type };
        auto functions = context.program->findMatchingFunctions(to_string(op), types);
        if ( functions.size()==0 )
            throw semantic_error("no matching function", at);
        if ( functions.size()>1 )
            throw semantic_error("too many matching functions", at);
        func = functions[0];
        type = make_shared<TypeDecl>(*func->result);
        if ( !func->arguments[0]->type->isRValue() )
            left = autoDereference(left);
        if ( !func->arguments[1]->type->isRValue() )
            right = autoDereference(right);
    }
    
    // ExprOp3
    
    ExpressionPtr ExprOp3::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprOp3,ExprOp>(expr);
        cexpr->subexpr = subexpr->clone();
        cexpr->left = left->clone();
        cexpr->right = right->clone();
        return cexpr;
    }
    
    void ExprOp3::log(ostream& stream, int depth) const
    {
        logType(stream);
        stream << "(" << to_string(op) << " ";
        subexpr->log(stream, depth);
        stream << " ";
        left->log(stream, depth);
        stream << " ";
        right->log(stream, depth);
        stream << ")";
    }
    
    void ExprOp3::inferType(InferTypeContext & context)
    {
        subexpr->inferType(context);
        if ( !subexpr->type->isSimpleType(Type::tBool) )
            throw semantic_error("cond operator condition must be boolean", at);
        left->inferType(context);
        right->inferType(context);
        vector<TypeDeclPtr> types = { subexpr->type, left->type, right->type };
        auto functions = context.program->findMatchingFunctions(to_string(op), types);
        if ( functions.size()==0 )
            throw semantic_error("no matching function", at);
        if ( functions.size()>1 )
            throw semantic_error("too many matching functions", at);
        func = functions[0];
        type = make_shared<TypeDecl>(*func->result);
        if ( !func->arguments[0]->type->isRValue() )
            subexpr = autoDereference(subexpr);
        if ( !func->arguments[1]->type->isRValue() )
            left = autoDereference(left);
        if ( !func->arguments[2]->type->isRValue() )
            right = autoDereference(right);
    }
    
    // ExprReturn
    
    ExpressionPtr ExprReturn::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprReturn>(expr);
        cexpr->subexpr = subexpr->clone();
        return cexpr;
    }
    
    void ExprReturn::log(ostream& stream, int depth) const
    {
        if ( subexpr ) {
            stream << "(return\n" << string(depth+1, '\t');
            subexpr->log(stream, depth+1);
            stream << ")";
        } else {
            stream << "(return)";
        }
    }
    
    void ExprReturn::inferType(InferTypeContext & context)
    {
        if ( subexpr ) {
            subexpr->inferType(context);
            if ( context.func->result->isVoid() )
                throw semantic_error("return subexpression of void function must be empty", at);
            if ( !subexpr->type->isSameType(*context.func->result, false) )
                throw semantic_error("return subexpression type must match function return type", at);
        } else {
            if ( !context.func->result->isVoid() )
                throw semantic_error("only void functions can skip return subexpression", at);
        }
        type = make_shared<TypeDecl>();
        if ( subexpr && !context.func->result->isRValue() )
            subexpr = autoDereference(subexpr);
    }
    
    // ExprConstInt
    
    ExpressionPtr ExprConstInt::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprConstInt,ExprConst>(expr);
        cexpr->value = value;
        return cexpr;
    }
    
    void ExprConstInt::log(ostream& stream, int depth) const
    {
        stream << value;
    }
    
    void ExprConstInt::inferType(InferTypeContext & context)
    {
        type = make_shared<TypeDecl>(Type::tInt);
    }
    
    // ExprConstUInt
    
    ExpressionPtr ExprConstUInt::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprConstUInt,ExprConst>(expr);
        cexpr->value = value;
        return cexpr;
    }
    
    void ExprConstUInt::log(ostream& stream, int depth) const
    {
        stream << "0x" << hex << value << dec;
    }
    
    void ExprConstUInt::inferType(InferTypeContext & context)
    {
        type = make_shared<TypeDecl>(Type::tUInt);
    }
    
    // ExprConstDouble
    
    ExpressionPtr ExprConstDouble::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprConstDouble,ExprConst>(expr);
        cexpr->value = value;
        return cexpr;
    }
    
    void ExprConstDouble::log(ostream& stream, int depth) const
    {
        stream << to_string_ex(value);
    }
    
    void ExprConstDouble::inferType(InferTypeContext & context)
    {
        type = make_shared<TypeDecl>(Type::tFloat);
    }
    
    // ExprIfThenElse
    
    ExpressionPtr ExprIfThenElse::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprIfThenElse>(expr);
        cexpr->cond = cond->clone();
        cexpr->if_true = if_true->clone();
        if ( if_false )
            cexpr->if_false = if_false->clone();
        return cexpr;
    }
    
    void ExprIfThenElse::log(ostream& stream, int depth) const
    {
        stream << "(if\n" << string(depth+1,'\t');
        cond->log(stream, depth+1);
        stream << "\n" << string(depth+2,'\t');
        if_true->log(stream, depth+2);
        if ( if_false ) {
            stream << "\n" << string(depth+2,'\t');
            if_false->log(stream, depth+2);
        }
        stream << ")";
    }
    
    void ExprIfThenElse::inferType(InferTypeContext & context)
    {
        cond->inferType(context);
        if ( !cond->type->isSimpleType(Type::tBool) )
            throw semantic_error("if-then-else condition must be boolean", at);
        if_true->inferType(context);
        if ( if_false )
            if_false->inferType(context);
        type = make_shared<TypeDecl>();
    }
    
    // ExprWhile
    
    ExpressionPtr ExprWhile::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprWhile>(expr);
        cexpr->cond = cond->clone();
        cexpr->body = body->clone();
        return cexpr;
    }
    
    void ExprWhile::inferType(InferTypeContext & context)
    {
        cond->inferType(context);
        if ( !cond->type->isSimpleType(Type::tBool) )
            throw semantic_error("while loop condition must be boolean", at);
        body->inferType(context);
        type = make_shared<TypeDecl>();
    }
    
    void ExprWhile::log(ostream& stream, int depth) const
    {
        stream << "(while\n"<< string(depth+1,'\t');
        cond->log(stream, depth+1);
        stream << "\n" << string(depth+2,'\t');
        body->log(stream, depth+2);
        stream << ")";
    }

    // ExprLet
    
    ExpressionPtr ExprLet::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprLet>(expr);
        for ( auto & var : variables )
            cexpr->variables.push_back(var);    // TODO: clone variable???
        cexpr->subexpr = subexpr->clone();
        return cexpr;
    }

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
            stream << string(depth+1, '\t') << "(" << *var << " " << *var->init << ")\n";
        }
        stream << string(depth+2, '\t');
        subexpr->log(stream, depth+2);
        stream << ")";
    }
    
    void ExprLet::inferType(InferTypeContext & context)
    {
        auto sz = context.local.size();
        for ( auto & var : variables ) {
            context.local.push_back(var);
            var->init->inferType(context);
            if ( !var->type->isSameType(*var->init->type,false) )
                throw semantic_error("variable initialization type mismatch", var->at );
            var->init = autoDereference(var->init);
        }
        subexpr->inferType(context);
        context.local.resize(sz);
        type = make_shared<TypeDecl>();
    }
    
    // ExprCall
    
    ExpressionPtr ExprCall::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprCall>(expr);
        cexpr->name = name;
        for ( auto & arg : arguments ) {
            cexpr->arguments.push_back(arg->clone());
        }
        cexpr->func = func;
        return cexpr;
    }
    
    void ExprCall::log(ostream& stream, int depth) const
    {
        logType(stream);
        stream << "(" << name;
        for ( auto & arg : arguments ) {
            stream << " ";
            arg->log(stream, depth);
        }
        stream << ")";
    }
    
    void ExprCall::inferType(InferTypeContext & context)
    {
        vector<TypeDeclPtr> types;
        types.reserve(arguments.size());
        for ( auto & ar : arguments ) {
            ar->inferType(context);
            types.push_back(ar->type);
        }
        auto functions = context.program->findMatchingFunctions(name, types);
        if ( functions.size()==0 )
            throw semantic_error("no matching function", at);
        if ( functions.size()>1 )
            throw semantic_error("too many matching functions", at);
        func = functions[0];
        type = make_shared<TypeDecl>(*func->result);
        for ( int iT = arguments.size(); iT != func->arguments.size(); ++iT ) {
            auto newArg = func->arguments[iT]->init->clone();
            if ( !newArg->type )
                newArg->inferType(context);
            arguments.push_back(newArg);
        }
        for ( int iA = 0; iA != arguments.size(); ++iA )
            if ( !func->arguments[iA]->type->isRValue() )
                arguments[iA] = autoDereference(arguments[iA]);
    }
    
    // program
    
    VariablePtr Program::findVariable ( const string & name ) const
    {
        auto it = globals.find(name);
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
        if ( program.globals.size() ) {
            stream << "(let\n";
            for ( auto & pv : program.globals ) {
                stream << "\t(" <<  *pv.second << ")\n";
            }
            stream << ")\n\n";
        }
        for ( const auto & st : program.functions ) {
            if ( !st.second->builtIn ) {
                stream << *st.second << "\n";
            }
        }
        return stream;
    }
    
    void Program::inferTypes()
    {
        for ( auto & fit : functions ) {
            Expression::InferTypeContext context;
            context.program = shared_from_this();
            context.func = fit.second;
            if ( !context.func->builtIn ) {
                for ( auto & arg : context.func->arguments ) {
                    if ( arg->init ) {
                        arg->init->inferType(context);
                        if ( !arg->type->isSameType(*arg->init->type, true) ) {
                            throw semantic_error("function argument default value type mismatch", arg->init->at);
                        }
                    }
                }
                context.func->body->inferType(context);
            }
        }
    }
    
    void Program::addBuiltIn(FunctionPtr && func)
    {
        auto mangledName = func->getMangledName();
        if ( findFunction(mangledName) )
            throw parse_error("builtin function already defined " + mangledName, nullptr);
        functions[mangledName] = func;
    }
    
    
    void Program::addBuiltinOperators()
    {
        // boolean
        addBuiltIn( make_shared<BuiltInOp1<bool, bool>>("!") );   // unary !
        addBuiltIn( make_shared<BuiltInOp2<bool, bool, bool>>("==") );
        addBuiltIn( make_shared<BuiltInOp2<bool, bool, bool>>("!=") );
        addBuiltIn( make_shared<BuiltInOp2<bool, bool, bool>>("&") );
        addBuiltIn( make_shared<BuiltInOp2<bool, bool, bool>>("|") );
        addBuiltIn( make_shared<BuiltInOp2<bool, bool, bool>>("^") );
        addBuiltIn( make_shared<BuiltInOp2<bool&, bool, bool&>>("=") );
        addBuiltIn( make_shared<BuiltInOp2<bool&, bool, bool&>>("&=") );
        addBuiltIn( make_shared<BuiltInOp2<bool&, bool, bool&>>("|=") );
        addBuiltIn( make_shared<BuiltInOp2<bool&, bool, bool&>>("^=") );
        // integer
        addBuiltIn( make_shared<BuiltInOp1<int, int>>("+") );   // unary +
        addBuiltIn( make_shared<BuiltInOp1<int, int>>("-") );   // unary -
        addBuiltIn( make_shared<BuiltInOp1<int, int>>("~") );   // unary ~
        addBuiltIn( make_shared<BuiltInOp2<int, int, int>>("+") );
        addBuiltIn( make_shared<BuiltInOp2<int, int, int>>("-") );
        addBuiltIn( make_shared<BuiltInOp2<int, int, int>>("*") );
        addBuiltIn( make_shared<BuiltInOp2<int, int, int>>("/") );
        addBuiltIn( make_shared<BuiltInOp2<int, int, int>>("&") );
        addBuiltIn( make_shared<BuiltInOp2<int, int, int>>("|") );
        addBuiltIn( make_shared<BuiltInOp2<int, int, int>>("^") );
        addBuiltIn( make_shared<BuiltInOp2<bool, int, int>>("==") );
        addBuiltIn( make_shared<BuiltInOp2<bool, int, int>>("!=") );
        addBuiltIn( make_shared<BuiltInOp2<bool, int, int>>(">=") );
        addBuiltIn( make_shared<BuiltInOp2<bool, int, int>>("<=") );
        addBuiltIn( make_shared<BuiltInOp2<bool, int, int>>(">") );
        addBuiltIn( make_shared<BuiltInOp2<bool, int, int>>("<") );
        addBuiltIn( make_shared<BuiltInOp2<int&, int, int&>>("=") );
        addBuiltIn( make_shared<BuiltInOp2<int&, int, int&>>("+=") );
        addBuiltIn( make_shared<BuiltInOp2<int&, int, int&>>("-=") );
        addBuiltIn( make_shared<BuiltInOp2<int&, int, int&>>("*=") );
        addBuiltIn( make_shared<BuiltInOp2<int&, int, int&>>("/=") );
        addBuiltIn( make_shared<BuiltInOp2<int&, int, int&>>("~=") );
        addBuiltIn( make_shared<BuiltInOp2<int&, int, int&>>("&=") );
        addBuiltIn( make_shared<BuiltInOp2<int&, int, int&>>("|=") );
        addBuiltIn( make_shared<BuiltInOp2<int&, int, int&>>("^=") );
        // float
        addBuiltIn( make_shared<BuiltInOp1<float, float>>("+") );   // unary +
        addBuiltIn( make_shared<BuiltInOp1<float, float>>("-") );   // unary -
        addBuiltIn( make_shared<BuiltInOp2<float, float, float>>("+") );
        addBuiltIn( make_shared<BuiltInOp2<float, float, float>>("-") );
        addBuiltIn( make_shared<BuiltInOp2<float, float, float>>("*") );
        addBuiltIn( make_shared<BuiltInOp2<float, float, float>>("/") );
        addBuiltIn( make_shared<BuiltInOp2<bool, float, float>>("==") );
        addBuiltIn( make_shared<BuiltInOp2<bool, float, float>>("!=") );
        addBuiltIn( make_shared<BuiltInOp2<bool, float, float>>(">=") );
        addBuiltIn( make_shared<BuiltInOp2<bool, float, float>>("<=") );
        addBuiltIn( make_shared<BuiltInOp2<bool, float, float>>(">") );
        addBuiltIn( make_shared<BuiltInOp2<bool, float, float>>("<") );
        addBuiltIn( make_shared<BuiltInOp2<float&, float, float&>>("=") );
        addBuiltIn( make_shared<BuiltInOp2<float&, float, float&>>("+=") );
        addBuiltIn( make_shared<BuiltInOp2<float&, float, float&>>("-=") );
        addBuiltIn( make_shared<BuiltInOp2<float&, float, float&>>("*=") );
        addBuiltIn( make_shared<BuiltInOp2<float&, float, float&>>("/=") );
    }
    
    vector<FunctionPtr> Program::findMatchingFunctions ( const string & name, const vector<TypeDeclPtr> & types ) const
    {
        /*
         TODO:
            arguments by name?
         */
        vector<FunctionPtr> result;
        for ( auto & it : functions ) {
            if ( it.second->name == name ) {
                auto & pFn = it.second;
                if ( pFn->arguments.size() >= types.size() ) {
                    bool typesCompatible = true;
                    for ( int ai = 0; ai != types.size(); ++ai ) {
                        auto & argType = pFn->arguments[ai]->type;
                        auto & passType = types[ai];
                        if ( (argType->isRValue() && !passType->isRValue()) || !argType->isSameType(*passType, false) ) {
                            typesCompatible = false;
                            break;
                        }
                    }
                    bool tailCompatible = true;
                    for ( int ti = types.size(); ti != pFn->arguments.size(); ++ti ) {
                        if ( !pFn->arguments[ti]->init ) {
                            tailCompatible = false;
                        }
                    }
                    if ( typesCompatible && tailCompatible ) {
                        result.push_back(pFn);
                    }
                }
            }
        }
        return result;
    }
    
    // expression parser forward declaration
    ExpressionPtr parseExpression ( const NodePtr & decl, const ProgramPtr & program );
    
    /*
        (int ...)               // integer
        (float 2 ...)           // float[2]
        (blah ..)               // struct blah
        (uint const 3 4 ...)    // const uint [3][4]
        (Sphere ...)
     */
    TypeDeclPtr parseTypeDeclaratoin ( const NodePtr & decl, const ProgramPtr & program, int nFields = 0 )
    {
        // cout << *decl << endl;
        auto tdecl = make_shared<TypeDecl>();
        tdecl->at = decl.get();
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
        if ( decl->list.size()==(3+nFields) && decl->list[1]->isOperator(Operator::binand) ) {
            tdecl->rvalue = true;
        } else {
            for ( int iDim = 1; iDim != decl->list.size() - 1 - nFields; ++iDim ) {
                uint64_t dim = decl->getUnsigned(iDim);
                if ( dim == -1U )
                    throw parse_error("expecting dimension", decl);
                tdecl->dim.push_back(dim);
            }
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
                auto st = make_shared<Structure>(name);
                st->at = expr.get();
                program->structures[name] = st;
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
                    if ( typeDecl->baseType==Type::tVoid )
                        throw parse_error("structure field can't be void", field);
                    if ( typeDecl->rvalue )
                        throw parse_error("structure field can't be reference", field);
                    decl->fields.push_back({name, typeDecl, field.get()});
                }
            }
        }
    }
    
    VariablePtr parseVariable ( const NodePtr & decl, const ProgramPtr & program, bool needInit, bool canHaveInit )
    {
        auto pVar = make_shared<Variable>();
        pVar->name = decl->getTailName();
        if ( pVar->name.empty() ) {
            if ( !canHaveInit )
                throw parse_error("variable can't have initializer", decl);
            pVar->name = decl->getTailName(1);
            if ( pVar->name.empty() )
                throw parse_error("variable must have a name", decl);
            pVar->init = parseExpression(decl->list.back(), program);
            pVar->type = parseTypeDeclaratoin(decl, program, 1);
        } else {
            if ( needInit )
                throw parse_error("variable must be initialized", decl);
            pVar->type = parseTypeDeclaratoin(decl, program);
        }
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
                for ( int iVar = 1; iVar != expr->list.size(); ++iVar ) {
                    auto & vdecl = expr->list[iVar];
                    auto pVar = parseVariable(vdecl, program, false, false);    // can global be initialized?
                    if ( program->findVariable(pVar->name) )
                        throw parse_error("variable already declared", vdecl);
                    program->globals[pVar->name] = pVar;
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
                block->at = decl.get();
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
                    if ( head->op==Operator::r2l ) {
                        auto pR2L = make_shared<ExprR2L>();
                        pR2L->at = decl.get();
                        pR2L->subexpr = parseExpression(decl->list[1], program);
                        return pR2L;
                    } else {
                        if ( !isUnaryOperator(head->op) )
                            throw parse_error("only unary operators can have 1 argument", decl);
                        auto pOp = make_shared<ExprOp1>();
                        pOp->at = decl.get();
                        pOp->op = head->op;
                        pOp->subexpr = parseExpression(decl->list[1], program);
                        return pOp;
                    }
                } else if ( nOp==2 ) {
                    if ( !isBinaryOperator(head->op) )
                        throw parse_error("only binary operators can have 2 arguments", decl);
                    if ( head->op==Operator::dot) {
                        if ( !decl->list[2]->isName() )
                            throw parse_error("field needs to be specified as a name", decl);
                        auto pDot = make_shared<ExprField>();
                        pDot->at = decl.get();
                        pDot->rvalue = parseExpression(decl->list[1], program);
                        pDot->name = decl->list[2]->text;
                        return pDot;
                    } else {
                        auto pOp = make_shared<ExprOp2>();
                        pOp->at = decl.get();
                        pOp->op = head->op;
                        pOp->left = parseExpression(decl->list[1], program);
                        pOp->right = parseExpression(decl->list[2], program);
                        return pOp;
                    }
                } else if ( nOp==3 ) {
                    if ( !isTrinaryOperator(head->op) )
                        throw parse_error("only trinary operators can have 3 arguments", decl);
                    auto pOp = make_shared<ExprOp3>();
                    pOp->at = decl.get();
                    pOp->op = head->op;
                    pOp->subexpr = parseExpression(decl->list[1], program);
                    pOp->left = parseExpression(decl->list[2], program);
                    pOp->right = parseExpression(decl->list[3], program);
                    return pOp;
                } else {
                    throw parse_error("operator has too many arguments", decl);
                }
            } else if ( head->isName("while") ) {
                if ( !decl->isListOfAtLeastSize(3) )
                    throw parse_error("only (while cond body) is allowed", decl);
                auto pWhile = make_shared<ExprWhile>();
                pWhile->at = decl.get();
                pWhile->cond = parseExpression(decl->list[1], program);
                pWhile->body = parseExpression(decl->list[2], program);
                return pWhile;
            } else if ( head->isName("if") ) {
                if ( !decl->isList() && !(decl->list.size()==3 || decl->list.size()==4) )
                    throw parse_error("only (if cond if_true) or (if cond if_true if_false) are allowed", decl);
                auto pIfThenElse = make_shared<ExprIfThenElse>();
                pIfThenElse->at = decl.get();
                pIfThenElse->cond = parseExpression(decl->list[1], program);
                pIfThenElse->if_true = parseExpression(decl->list[2], program);
                if ( decl->list.size()==4 )
                    pIfThenElse->if_false = parseExpression(decl->list[3], program);
                return pIfThenElse;
            } else if ( head->isName("return") ) {
                auto pRet = make_shared<ExprReturn>();
                pRet->at = decl.get();
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
                let->at = decl.get();
                for ( int iVar = 1; iVar != decl->list.size()-1; ++iVar ) {
                    auto & vdecl = decl->list[iVar];
                    auto pVar = parseVariable(vdecl, program, true, true);
                    if ( let->find (pVar->name) )
                        throw parse_error("variable already declared", decl);
                    if ( pVar->type->rvalue )
                        throw parse_error("local variable can't be reference", decl);
                    let->variables.push_back(pVar);
                }
                let->subexpr = parseExpression(decl->list.back(), program);
                return let;
            } else if ( head->isName() ) {
                // function call
                auto call = make_shared<ExprCall>();
                call->at = decl.get();
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
            pVar->at = decl.get();
            pVar->name = decl->text;
            return pVar;
        } else if ( decl->isNumericConstant() ) {
            ExpressionPtr pconst;
            if ( decl->type==NodeType::dnumber ) {
                pconst = make_shared<ExprConstDouble>(decl->dnum);
            } else if ( decl->type==NodeType::inumber ) {
                pconst = make_shared<ExprConstInt>(decl->inum);
            } else if ( decl->type==NodeType::unumber ) {
                pconst = make_shared<ExprConstUInt>(decl->unum);
            } else {
                throw parse_error("undefined constant type", decl);
            }
            pconst->at = decl.get();
            return pconst;
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
            int hasDefault = 1;
            auto & arg = decl->list[ai];
            auto argp = make_shared<Variable>();
            argp->name = arg->getTailName(1);
            if ( argp->name.empty() || arg->list.size()==2 ) {
                hasDefault = 0;
                argp->name = arg->getTailName();
                if ( argp->name.empty() )
                    throw parse_error("function argument must have name", arg);
            }
            if ( func->findArgument(argp->name) )
                throw parse_error("function already has argument with this name", arg);
            argp->type = parseTypeDeclaratoin(arg, program, hasDefault);
            if ( hasDefault )
                argp->init = parseExpression(arg->list.back(), program);
            func->arguments.push_back(argp);
        }
        func->body = parseExpression(decl->list.back(), program);
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
            throw parse_error("expecting a list", root);
        auto program = make_shared<Program>();
        parseStructureDeclarations(root, program);
        parseVariableDeclarations(root, program);
        parseFunctionDeclarations(root, program);
        program->addBuiltinOperators();
        program->inferTypes();
        return program;
    }
    
}
