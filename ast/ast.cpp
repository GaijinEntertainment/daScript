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
    
    bool TypeDecl::isSimpleType() const
    {
        if (    baseType==Type::none
            ||  baseType==Type::tVoid
            ||  baseType==Type::tStructure )
            return false;
        if ( dim.size() )
            return false;
        return true;
    }
    
    bool TypeDecl::isSimpleType(Type typ) const
    {
        return baseType==typ && isSimpleType();
    }
    
    bool TypeDecl::isArray() const
    {
        return dim.size() != 0;
    }
    
    bool TypeDecl::isRValue() const
    {
        return rvalue || baseType==Type::tStructure || dim.size();
    }
    
    bool TypeDecl::isIndex() const
    {
        return (baseType==Type::tInt || baseType==Type::tUInt) && dim.size()==0;
    }
    
    int TypeDecl::getBaseSizeOf() const
    {
        switch ( baseType ) {
            case tBool:     return sizeof(bool);
            case tInt:      return sizeof(int);
            case tInt2:     return sizeof(int) * 2;
            case tInt3:     return sizeof(int) * 3;
            case tInt4:     return sizeof(int) * 4;
            case tUInt:     return sizeof(uint);
            case tUInt2:    return sizeof(uint) * 2;
            case tUInt3:    return sizeof(uint) * 3;
            case tUInt4:    return sizeof(uint) * 4;
            case tFloat:    return sizeof(float);
            case tFloat2:   return sizeof(float) * 2;
            case tFloat3:   return sizeof(float) * 3;
            case tFloat4:   return sizeof(float) * 4;
            case tStructure:
                return structType->getSizeOf();
            default:
                return 0;
        }
    }
    
    int TypeDecl::getSizeOf() const
    {
        int size = 1;
        for ( auto i : dim )
            size *= i;
        return getBaseSizeOf() * size;
    }
    
    int TypeDecl::getStride() const
    {
        int size = 1;
        if ( dim.size() > 1 ) {
            for ( size_t i=0; i!=dim.size()-1; ++i )
                size *= dim[i];
        }
        return getBaseSizeOf() * size;
    }
    
    // structure
    
    int Structure::getSizeOf() const
    {
        int size = 0;
        for ( const auto & fd : fields ) {
            size += fd.type->getSizeOf();
        }
        return size;
    }
    
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
    
    SimNode * Function::simulate (Context & context) const
    {
        if ( builtIn )
            throw runtime_error("can only simulate non built-in function");
        return body->simulate(context);
    }
    
    // built-in function
    
    BuiltInFunction::BuiltInFunction ( const string & fn )
    {
        builtIn = true;
        name = fn;
    }
    
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
        if ( !subexpr->type->isSimpleType() )
            throw semantic_error("can only dereference an simple type", at);
        type = make_shared<TypeDecl>(*subexpr->type);
        type->rvalue = false;
    }
    
    SimNode * ExprR2L::simulate (Context & context) const
    {
        switch ( type->baseType ) {
            case Type::tInt:    return context.makeNode<SimNode_R2L<int64_t>>(subexpr->simulate(context));
            case Type::tUInt:   return context.makeNode<SimNode_R2L<uint64_t>>(subexpr->simulate(context));
            case Type::tBool:   return context.makeNode<SimNode_R2L<bool>>(subexpr->simulate(context));
            case Type::tFloat:  return context.makeNode<SimNode_R2L<float>>(subexpr->simulate(context));
            default:            throw runtime_error("can't dereference type");
        }
    }

    // ExprAt
    
    void ExprAt::log(ostream& stream, int depth) const
    {
        stream << "(@ " << *subexpr << " " << *index << ")";
    }
    
    void ExprAt::inferType(InferTypeContext & context)
    {
        subexpr->inferType(context);
        if ( !subexpr->type->isRValue() )
            throw semantic_error("can only index rvalue", subexpr->at);
        if ( !subexpr->type->dim.size() )
            throw semantic_error("can only index arrays", subexpr->at);
        index->inferType(context);
        index = autoDereference(index);
        if ( !index->type->isIndex() )
            throw semantic_error("index is int or uint", index->at);
        type = make_shared<TypeDecl>(*subexpr->type);
        type->rvalue = true;
        type->dim.pop_back();
    }
    
    ExpressionPtr ExprAt::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprAt>(expr);
        cexpr->subexpr = subexpr->clone();
        cexpr->index = index->clone();
        return cexpr;
    }

    SimNode * ExprAt::simulate (Context & context) const
    {
        auto prv = subexpr->simulate(context);
        auto pidx = index->simulate(context);
        int stride = subexpr->type->getStride();
        return context.makeNode<SimNode_At>(prv, pidx, stride);
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
    
    SimNode * ExprBlock::simulate (Context & context) const
    {
        auto block = context.makeNode<SimNode_Block>();
        block->total = int(list.size());
        block->list = (SimNode **) context.allocate(sizeof(SimNode *)*block->total);
        for ( int i = 0; i != block->total; ++i )
            block->list[i] = list[i]->simulate(context);
        return block;
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
    
    SimNode * ExprField::simulate (Context & context) const
    {
        return context.makeNode<SimNode_Field>(rvalue->simulate(context), field->offset);
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
        argumentIndex = 0;
        for ( auto & arg : context.func->arguments ) {
            if ( arg->name==name ) {
                variable = arg;
                argument = true;
                type = make_shared<TypeDecl>(*arg->type);
                return;
            }
            argumentIndex ++;
        }
        // global
        auto var = context.program->findVariable(name);
        if ( !var )
            throw semantic_error("can't locate variable " + name, at);
        variable = var;
        type = make_shared<TypeDecl>(*var->type);
        type->rvalue = true;
    }
    
    SimNode * ExprVar::simulate (Context & context) const
    {
        if ( local ) {
            return context.makeNode<SimNode_GetLocal>(variable->stackTop);
        } else if ( argument) {
            return context.makeNode<SimNode_GetArgument>(argumentIndex);
        } else {
            return context.makeNode<SimNode_GetGlobal>(variable->index);
        }
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
            throw semantic_error("no matching function " + to_string(op) + " " + subexpr->type->getMangledName(), at);
        if ( functions.size()>1 )
            throw semantic_error("too many matching functions", at);
        func = functions[0];
        if ( !func->builtIn )
            throw semantic_error("operator must point to built-in function every time", at);
        type = make_shared<TypeDecl>(*func->result);
        if ( !func->arguments[0]->type->isRValue() )
            subexpr = autoDereference(subexpr);
    }
    
    SimNode * ExprOp1::simulate (Context & context) const
    {
        auto pSimOp1 = static_cast<SimNode_Op1 *>(func->makeSimNode(context));
        pSimOp1->x = subexpr->simulate(context);
        return pSimOp1;
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
            throw semantic_error("no matching function " + to_string(op)
                + " " + left->type->getMangledName() + " " + right->type->getMangledName(), at);
        if ( functions.size()>1 )
            throw semantic_error("too many matching functions", at);
        func = functions[0];
        if ( !func->builtIn )
            throw semantic_error("operator must point to built-in function every time", at);
        type = make_shared<TypeDecl>(*func->result);
        if ( !func->arguments[0]->type->isRValue() )
            left = autoDereference(left);
        if ( !func->arguments[1]->type->isRValue() )
            right = autoDereference(right);
    }
    
    SimNode * ExprOp2::simulate (Context & context) const
    {
        auto pSimOp2 = static_cast<SimNode_Op2 *>(func->makeSimNode(context));
        pSimOp2->l = left->simulate(context);
        pSimOp2->r = right->simulate(context);
        return pSimOp2;
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
            throw semantic_error("no matching function " + to_string(op)
                + " " + subexpr->type->getMangledName() + " "
                    + left->type->getMangledName() + " " + right->type->getMangledName(), at);
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
    
    SimNode * ExprOp3::simulate (Context & context) const
    {
        assert(0 && "implement");
        return nullptr;
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
    
    SimNode * ExprReturn::simulate (Context & context) const
    {
        assert(0 && "implement");
        return nullptr;
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
    
    SimNode * ExprConstInt::simulate (Context & context) const
    {
        return context.makeNode<SimNode_ConstValue<int64_t>>(value);
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
    
    SimNode * ExprConstUInt::simulate (Context & context) const
    {
        return context.makeNode<SimNode_ConstValue<uint64_t>>(value);
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
    
    SimNode * ExprConstDouble::simulate (Context & context) const
    {
        return context.makeNode<SimNode_ConstValue<float>>(value);

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
    
    SimNode * ExprIfThenElse::simulate (Context & context) const
    {
        if ( if_false )
            return context.makeNode<SimNode_IfThenElse>(cond->simulate(context), if_true->simulate(context), if_false->simulate(context));
        else
            return context.makeNode<SimNode_IfThen>(cond->simulate(context), if_true->simulate(context));
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

    SimNode * ExprWhile::simulate (Context & context) const
    {
        return context.makeNode<SimNode_While>(cond->simulate(context),body->simulate(context));
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
        auto sp = context.stackTop;
        auto sz = context.local.size();
        totalInit = 0;
        for ( auto & var : variables ) {
            context.local.push_back(var);
            if ( var->init ) {
                var->init->inferType(context);
                if ( !var->type->isSameType(*var->init->type,false) )
                    throw semantic_error("variable initialization type mismatch", var->at );
                var->init = autoDereference(var->init);
                totalInit ++;
            }
            var->stackTop = context.stackTop;
            context.stackTop += (var->type->getSizeOf() + 0xf) & ~0xf;
        }
        subexpr->inferType(context);
        context.func->totalStackSize = max(context.func->totalStackSize, context.stackTop);
        context.stackTop = sp;
        context.local.resize(sz);
        type = make_shared<TypeDecl>();
    }
    
    
    SimNode * ExprLet::simulate (Context & context) const
    {
        auto let = context.makeNode<SimNode_Let>();
        let->total = totalInit;
        let->list = (SimNode **) context.allocate(let->total * sizeof(SimNode*));
        int vi = 0;
        for ( auto & var : variables ) {
            if ( var->init ) {
                SimNode * copy = nullptr;
                auto init = var->init->simulate(context);
                auto get = context.makeNode<SimNode_GetLocal>(var->stackTop);
                int size = var->type->getSizeOf();
                if ( var->init->type->isRValue() ) {
                    copy = context.makeNode<SimNode_CopyRValue>(get, init, size);
                } else {
                    switch ( var->type->baseType ) {
                        case Type::tInt:    copy = context.makeNode<SimNode_CopyLValue<int64_t>>(get, init);    break;
                        case Type::tUInt:   copy = context.makeNode<SimNode_CopyLValue<uint64_t>>(get, init);   break;
                        case Type::tFloat:  copy = context.makeNode<SimNode_CopyLValue<float>>(get, init);      break;
                        default:
                            throw runtime_error("unsupported? can't assign initial value");
                    }
                }
                let->list[vi++] = copy;
            }
        }
        let->subexpr = subexpr->simulate(context);
        return let;
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
        for ( size_t iT = arguments.size(); iT != func->arguments.size(); ++iT ) {
            auto newArg = func->arguments[iT]->init->clone();
            if ( !newArg->type )
                newArg->inferType(context);
            arguments.push_back(newArg);
        }
        for ( size_t iA = 0; iA != arguments.size(); ++iA )
            if ( !func->arguments[iA]->type->isRValue() )
                arguments[iA] = autoDereference(arguments[iA]);
    }
    
    SimNode * ExprCall::simulate (Context & context) const
    {
        SimNode_Call * pCall = static_cast<SimNode_Call *>(func->makeSimNode(context));
        pCall->fnIndex = func->index;
        if ( int nArg = (int) arguments.size() ) {
            pCall->arguments = (SimNode **) context.allocate(nArg * sizeof(SimNode *));
            pCall->nArguments = nArg;
            pCall->argValues = (__m128 *) context.allocate(nArg * sizeof(__m128));
            for ( int a=0; a!=nArg; ++a ) {
                pCall->arguments[a] = arguments[a]->simulate(context);
            }
        } else {
            pCall->arguments = nullptr;
            pCall->nArguments = 0;
            pCall->argValues = nullptr;
        }
        return pCall;
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
        // structure declarations (precompute offsets of fields)
        for ( auto & ist : structures ) {
            auto & st = ist.second;
            int offset = 0;
            for ( auto & fi : st->fields ) {
                fi.offset = offset;
                offset += fi.type->getSizeOf();
            }
        }
        // global variables
        int gvi = 0;
        for ( auto & it : globals ) {
            auto pvar = it.second;
            pvar->index = gvi ++;
        }
        // functions
        totalFunctions = 0;
        for ( auto & fit : functions ) {
            Expression::InferTypeContext context;
            context.program = shared_from_this();
            context.func = fit.second;
            if ( !context.func->builtIn ) {
                context.func->index = totalFunctions ++;
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
        functionsByName[func->name].push_back(func);
    }
    
    // basic operations
    template <typename TT, typename SimPolicy_TT>
    void addBuiltInBasic(Program & prg)
    {
        //                                    policy                        ret   arg1 arg2    name
        prg.addBuiltIn( make_shared<BuiltInFn<SimNode_Debug<TT>,            TT,   TT>       >("debug") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_Set<SimPolicy_TT>,        TT&,  TT&, TT>  >("=") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_Equ<SimPolicy_TT>,        bool, TT,  TT>  >("==") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_NotEqu<SimPolicy_TT>,     bool, TT,  TT>  >("!=") );
    }
    
    // numeric types
    template <typename TT, typename SimPolicy_TT>
    void addBuiltInNumeric(Program & prg)
    {
        //                                    policy                        ret   arg1 arg2    name
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_Unp<SimPolicy_TT>,        TT,   TT>       >("+") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_Unm<SimPolicy_TT>,        TT,   TT>       >("-") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_Add<SimPolicy_TT>,        TT,   TT,  TT>  >("+") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_Sub<SimPolicy_TT>,        TT,   TT,  TT>  >("-") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_Mul<SimPolicy_TT>,        TT,   TT,  TT>  >("*") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_Div<SimPolicy_TT>,        TT,   TT,  TT>  >("/") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_GtEqu<SimPolicy_TT>,      bool, TT,  TT>  >(">=") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_LessEqu<SimPolicy_TT>,    bool, TT,  TT>  >("<=") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_Gt<SimPolicy_TT>,         bool, TT,  TT>  >(">") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_Less<SimPolicy_TT>,       bool, TT,  TT>  >("<") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_SetAdd<SimPolicy_TT>,     TT&,  TT&, TT>  >("+=") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_SetSub<SimPolicy_TT>,     TT&,  TT&, TT>  >("-=") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_SetMul<SimPolicy_TT>,     TT&,  TT&, TT>  >("*=") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_SetDiv<SimPolicy_TT>,     TT&,  TT&, TT>  >("/=") );
    }
    
    // built-in numeric types
    template <typename TT, typename SimPolicy_TT>
    void addBuiltInBit(Program & prg)
    {
        //                                    policy                        ret   arg1 arg2    name
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_BinNot<SimPolicy_TT>,     TT,   TT>       >("~") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_BinAnd<SimPolicy_TT>,     TT,   TT,  TT>  >("&") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_BinOr<SimPolicy_TT>,      TT,   TT,  TT>  >("|") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_BinXor<SimPolicy_TT>,     TT,   TT,  TT>  >("^") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_SetBinAnd<SimPolicy_TT>,  TT&,  TT,  TT&> >("&=") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_SetBinOr<SimPolicy_TT>,   TT&,  TT,  TT&> >("|=") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_SetBinXor<SimPolicy_TT>,  TT&,  TT,  TT&> >("^=") );
    }
    
    // built-in boolean types
    template <typename TT, typename SimPolicy_TT>
    void addBuiltInBoolean(Program & prg)
    {
        //                                    policy                        ret   arg1 arg2    name
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_BoolNot<SimPolicy_Bool>,  TT,   TT>       >("!") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_BoolAnd<SimPolicy_Bool>,  TT,   TT,  TT>  >("&") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_BoolOr<SimPolicy_Bool>,   TT,   TT,  TT>  >("|") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_BoolXor<SimPolicy_Bool>,  TT,   TT,  TT>  >("^") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_SetBoolAnd<SimPolicy_Bool>,TT&, TT&, TT>  >("&=") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_SetBoolOr<SimPolicy_Bool>, TT&, TT&, TT>  >("|=") );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_SetBoolXor<SimPolicy_Bool>,TT&, TT&, TT>  >("^=") );
    }
    
    void Program::addBuiltinOperators()
    {
        // boolean
        addBuiltInBasic<bool, SimPolicy_Bool>(*this);
        addBuiltInBoolean<bool, SimPolicy_Bool>(*this);
        // int64
        addBuiltInBasic<int64_t, SimPolicy_Int>(*this);
        addBuiltInNumeric<int64_t, SimPolicy_Int>(*this);
        addBuiltInBit<int64_t, SimPolicy_Int>(*this);
        addBuiltIn ( make_shared<BuiltInFn<SimNode_Cast<int64_t,float>,int64_t,float>>("int") );
        addBuiltIn ( make_shared<BuiltInFn<SimNode_Cast<int64_t,uint64_t>,int64_t,uint64_t>>("int") );
        // uint64
        addBuiltInBasic<uint64_t, SimPolicy_UInt>(*this);
        addBuiltInNumeric<uint64_t, SimPolicy_UInt>(*this);
        addBuiltInBit<uint64_t, SimPolicy_UInt>(*this);
        addBuiltIn ( make_shared<BuiltInFn<SimNode_Cast<uint64_t,float>,uint64_t,float>>("uint") );
        addBuiltIn ( make_shared<BuiltInFn<SimNode_Cast<uint64_t,int64_t>,uint64_t,int64_t>>("uint") );
        // float
        addBuiltInBasic<float, SimPolicy_Float>(*this);
        addBuiltInNumeric<float, SimPolicy_Float>(*this);
        addBuiltIn ( make_shared<BuiltInFn<SimNode_Cast<float,int64_t>,float,int64_t>>("float") );
        addBuiltIn ( make_shared<BuiltInFn<SimNode_Cast<float,uint64_t>,float,uint64_t>>("float") );
    }
    
    vector<FunctionPtr> Program::findMatchingFunctions ( const string & name, const vector<TypeDeclPtr> & types ) const
    {
        /*
         TODO:
            arguments by name?
         */
        vector<FunctionPtr> result;
        auto itFnList = functionsByName.find(name);
        if ( itFnList == functionsByName.end() )
            return result;
        auto & goodFunctions = itFnList->second;
        for ( auto & pFn : goodFunctions ) {
            if ( pFn->arguments.size() >= types.size() ) {
                bool typesCompatible = true;
                for ( auto ai = 0; ai != types.size(); ++ai ) {
                    auto & argType = pFn->arguments[ai]->type;
                    auto & passType = types[ai];
                    if ( (argType->isRValue() && !passType->isRValue()) || !argType->isSameType(*passType, false) ) {
                        typesCompatible = false;
                        break;
                    }
                }
                bool tailCompatible = true;
                for ( auto ti = types.size(); ti != pFn->arguments.size(); ++ti ) {
                    if ( !pFn->arguments[ti]->init ) {
                        tailCompatible = false;
                    }
                }
                if ( typesCompatible && tailCompatible ) {
                    result.push_back(pFn);
                }
            }
        }
        return result;
    }
    
    void Program::simulate ( Context & context )
    {
        context.globalVariables = (GlobalVariable *) context.allocate( globals.size()*sizeof(GlobalVariable) );
        for ( auto & it : globals ) {
            auto pvar = it.second;
            auto & gvar = context.globalVariables[pvar->index];
            gvar.name = context.allocateName(pvar->name);
            gvar.size = pvar->type->getSizeOf();
            void * data = context.allocate(gvar.size);
            gvar.value = ptr_cast_from(data);
        }
        context.totalVariables = (int) globals.size();
        context.functions = (SimFunction *) context.allocate( totalFunctions*sizeof(SimFunction) );
        context.totalFunctions = totalFunctions;
        for ( auto & it : functions ) {
            auto pfun = it.second;
            if ( pfun->index==-1 )
                continue;
            auto & gfun = context.functions[pfun->index];
            gfun.name = context.allocateName(pfun->name);
            gfun.code = pfun->simulate(context);
            gfun.stackSize = pfun->totalStackSize;
        }
        context.linearAllocatorExecuteBase = context.linearAllocator;
    }
    
    // PARSER
    
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
                    if ( head->op==Operator::at ) {
                        auto pAt = make_shared<ExprAt>();
                        pAt->subexpr = parseExpression(decl->list[1], program);
                        pAt->index = parseExpression(decl->list[2], program);
                        return pAt;
                    } else if ( head->op==Operator::dot) {
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
                program->functionsByName[func->name].push_back(func);
            }
        }
    }
    
    ProgramPtr parse ( const NodePtr & root, function<void (const ProgramPtr & prg)> && defineContext )
    {
        if ( !root->isList() )
            throw parse_error("expecting a list", root);
        auto program = make_shared<Program>();
        parseStructureDeclarations(root, program);
        parseVariableDeclarations(root, program);
        parseFunctionDeclarations(root, program);
        program->addBuiltinOperators();
        defineContext(program);
        program->inferTypes();
        return program;
    }
    
}
