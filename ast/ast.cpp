#include "precomp.h"

#include "ast.h"
#include "enums.h"

void yybegin(const char * str);
int yyparse();

namespace yzg
{
    bool g_logTypes = false;
    
    // operator goo
    
    Enum<Operator> g_opTable2 = {
        {   Operator::r2l,          "=>"    },
        {   Operator::p2r,          "->"    },
        {   Operator::addEqu,       "+="    },
        {   Operator::subEqu,       "-="    },
        {   Operator::divEqu,       "/="    },
        {   Operator::mulEqu,       "*="    },
        {   Operator::modEqu,       "%="    },
        {   Operator::eqEq,         "=="    },
        {   Operator::lessEqu,      "<="    },
        {   Operator::greaterEqu,   ">="    },
        {   Operator::notEqu,       "!="    },
        {   Operator::binNotEqu,    "~="    },
        {   Operator::andEqu,       "&="    },
        {   Operator::orEqu,        "|="    },
        {   Operator::xorEqu,       "^="    },
        {   Operator::inc,          "++"    },
        {   Operator::dec,          "--"    },
        {   Operator::postInc,      "+++"    },
        {   Operator::postDec,      "---"    },
    };
    
    Enum<Operator> g_opTable1 = {
        {   Operator::at,       "@"    },
        {   Operator::dot,      "."    },
        {   Operator::add,      "+"    },
        {   Operator::sub,      "-"    },
        {   Operator::div,      "/"    },
        {   Operator::mul,      "*"    },
        {   Operator::mod,      "%"    },
        {   Operator::eq,       "="    },
        {   Operator::is,       "?"    },
        {   Operator::boolNot,  "!"    },
        {   Operator::binNot,   "~"    },
        {   Operator::less,     "<"    },
        {   Operator::greater,  ">"    },
        {   Operator::binand,   "&"    },
        {   Operator::binor,    "|"    },
        {   Operator::binxor,   "^"    },
    };
    
    string to_string ( Operator o ) {
        string t = g_opTable2.find(o);
        if ( t.empty() )
            t = g_opTable1.find(o);
        return t;
    }
    
    bool isUnaryOperator ( Operator op )
    {
        return
            (op==Operator::add)
        ||  (op==Operator::sub)
        ||  (op==Operator::boolNot)
        ||  (op==Operator::binNot)
        ||  (op==Operator::inc)
        ||  (op==Operator::dec)
        ||  (op==Operator::postInc)
        ||  (op==Operator::postDec)
        ;
    }
    
    bool isBinaryOperator ( Operator op )
    {
        return
            (op!=Operator::is)
        &&  (op!=Operator::boolNot)
        &&  (op!=Operator::binNot)
        &&  (op==Operator::inc)
        &&  (op==Operator::dec)
        &&  (op==Operator::postInc)
        &&  (op==Operator::postDec)
        ;
    }
    
    bool isTrinaryOperator ( Operator op )
    {
        return (op==Operator::is);
    }
    
    // TypeDecl
    
    ostream& operator<< (ostream& stream, const TypeDecl & decl)
    {
        if ( decl.baseType==Type::tStructure || decl.baseType==Type::tPointer ) {
            stream << decl.structType->name;
        } else {
            stream << to_string(decl.baseType);
        }
        if ( decl.baseType==Type::tPointer )
            stream << " *";
        for ( auto d : decl.dim ) {
            stream << " " << d;
        }
        if ( decl.ref )
            stream << " &";
        return stream;
    }
    
    TypeDecl::TypeDecl(const TypeDecl & decl)
    {
        baseType = decl.baseType;
        structType = decl.structType;
        dim = decl.dim;
        ref = decl.ref;
        at = decl.at;
    }
    
    string TypeDecl::getMangledName() const
    {
        stringstream ss;
        if ( baseType==Type::tStructure ) {
            ss << structType->name;
        } else {
            ss << to_string(baseType);
        }
        if ( ref )
            ss << "#ref";
        if ( dim.size() ) {
            for ( auto d : dim ) {
                ss << "#" << d;
            }
        }
        return ss.str();
    }
    
    bool TypeDecl::isSameType ( const TypeDecl & decl, bool refMatters ) const
    {
        if ( baseType!=decl.baseType )
            return false;
        if ( baseType==Type::tStructure && structType!=decl.structType )
            return false;
        if ( baseType==Type::tPointer ) {
            if ( structType && decl.structType && structType!=decl.structType ) {
                return false;
            }
        }
        if ( dim!=decl.dim )
            return false;
        if ( refMatters )
            if ( ref!=decl.ref )
                return false;
        return true;
    }
    
    bool TypeDecl::isIteratorType ( const TypeDecl & decl ) const
    {
        if ( baseType!=decl.baseType )
            return false;
        if ( baseType==Type::tStructure && structType!=decl.structType )
            return false;
        if ( decl.dim.size() )
            return false;
        if ( !decl.isRef() )
            return false;
        return true;
    }
    
    bool TypeDecl::isVoid() const
    {
        return (baseType==Type::tVoid) && (dim.size()==0);
    }
    
    bool TypeDecl::isPointer() const
    {
        return (baseType==Type::tPointer) && (dim.size()==0);
    }
    
    bool TypeDecl::isSimpleType() const
    {
        if (    baseType==Type::none
            ||  baseType==Type::tVoid
            ||  baseType==Type::tStructure
            ||  baseType==Type::tPointer )
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
    
    bool TypeDecl::isRef() const
    {
        return ref || baseType==Type::tStructure || dim.size();
    }
    
    bool TypeDecl::isIndex() const
    {
        return (baseType==Type::tInt || baseType==Type::tUInt) && dim.size()==0;
    }
    
    int TypeDecl::getBaseSizeOf() const
    {
        return baseType==Type::tStructure ? structType->getSizeOf() : getTypeBaseSize(baseType);
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
    
    void Expression::InferTypeContext::error ( const string & err, const LineInfo & at )
    {
        program->error(err,at);
    }
    
    ExpressionPtr Expression::clone( const ExpressionPtr & expr ) const
    {
        if ( !expr )
            throw runtime_error("unsupported expression");
        expr->at = at;
        expr->type = type ? make_shared<TypeDecl>(*type) : nullptr;
        return expr;
    }
    
    ExpressionPtr Expression::autoDereference ( const ExpressionPtr & expr )
    {
        if ( expr->type && expr->type->isRef() ) {
            auto ar2l = make_shared<ExprRef2Value>();
            ar2l->subexpr = expr;
            ar2l->at = expr->at;
            ar2l->type = make_shared<TypeDecl>(*expr->type);
            ar2l->type->ref = false;
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
    
    // ExprRef2Value
    
    ExpressionPtr ExprRef2Value::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprRef2Value>(expr);
        cexpr->subexpr = subexpr->clone();
        return cexpr;
    }
    
    void ExprRef2Value::log(ostream& stream, int depth) const
    {
        stream << "(-> " << *subexpr << ")";
    }
    
    void ExprRef2Value::inferType(InferTypeContext & context)
    {
        subexpr->inferType(context);
        if ( !subexpr->type->isRef() ) {
            context.error("can only dereference ref", at);
        } else if ( !subexpr->type->isSimpleType() ) {
            context.error("can only dereference a simple type", at);
        } else {
            type = make_shared<TypeDecl>(*subexpr->type);
            type->ref = false;
        }
    }
    
    SimNode * ExprRef2Value::simulate (Context & context) const
    {
        switch ( type->baseType ) {
            case Type::tInt:        return context.makeNode<SimNode_Ref2Value<int32_t>>(at,subexpr->simulate(context));
            case Type::tUInt:       return context.makeNode<SimNode_Ref2Value<uint32_t>>(at,subexpr->simulate(context));
            case Type::tBool:       return context.makeNode<SimNode_Ref2Value<bool>>(at,subexpr->simulate(context));
            case Type::tString:     return context.makeNode<SimNode_Ref2Value<char *>>(at,subexpr->simulate(context));
            case Type::tPointer:    return context.makeNode<SimNode_Ref2Value<void *>>(at,subexpr->simulate(context));
            case Type::tFloat:      return context.makeNode<SimNode_Ref2Value<float>>(at,subexpr->simulate(context));
            case Type::tFloat2:     return context.makeNode<SimNode_Ref2Value<float2>>(at,subexpr->simulate(context));
            case Type::tFloat3:     return context.makeNode<SimNode_Ref2Value<float3>>(at,subexpr->simulate(context));
            case Type::tFloat4:     return context.makeNode<SimNode_Ref2Value<float4>>(at,subexpr->simulate(context));
            case Type::tInt2:       return context.makeNode<SimNode_Ref2Value<int2>>(at,subexpr->simulate(context));
            case Type::tInt3:       return context.makeNode<SimNode_Ref2Value<int3>>(at,subexpr->simulate(context));
            case Type::tInt4:       return context.makeNode<SimNode_Ref2Value<int4>>(at,subexpr->simulate(context));
            case Type::tUInt2:      return context.makeNode<SimNode_Ref2Value<uint2>>(at,subexpr->simulate(context));
            case Type::tUInt3:      return context.makeNode<SimNode_Ref2Value<uint3>>(at,subexpr->simulate(context));
            case Type::tUInt4:      return context.makeNode<SimNode_Ref2Value<uint4>>(at,subexpr->simulate(context));
            default:                throw runtime_error("can't dereference type");
        }
    }
    
    // ExprPtr2Ref
    
    ExpressionPtr ExprPtr2Ref::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprPtr2Ref>(expr);
        cexpr->subexpr = subexpr->clone();
        return cexpr;
    }
    
    void ExprPtr2Ref::log(ostream& stream, int depth) const
    {
        stream << "(=> " << *subexpr << ")";
    }
    
    void ExprPtr2Ref::inferType(InferTypeContext & context)
    {
        subexpr->inferType(context);
        subexpr = autoDereference(subexpr);
        if ( !subexpr->type->isPointer() ) {
            context.error("can only dereference pointer", at);
        } else {
            type = make_shared<TypeDecl>(*subexpr->type);
            type->baseType = Type::tStructure;
            type->ref = true;
        }
    }
    
    SimNode * ExprPtr2Ref::simulate (Context & context) const
    {
        return context.makeNode<SimNode_Ptr2Ref>(at,subexpr->simulate(context));
    }

    // ExprAssert
    
    ExpressionPtr ExprAssert::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprAssert>(expr);
        cexpr->subexpr = subexpr->clone();
        cexpr->message = message;
        return cexpr;
    }
    
    void ExprAssert::log(ostream& stream, int depth) const
    {
        stream << "(assert ";
        stream << *subexpr;
        if ( !message.empty() )
            stream << " \"" << message << "\"";
        stream << ")";
    }
    
    void ExprAssert::inferType(InferTypeContext & context)
    {
        subexpr->inferType(context);
        if ( !subexpr->type ) return;
        subexpr = autoDereference(subexpr);
        if ( !subexpr->type->isSimpleType(Type::tBool) )
            context.error("assert condition must be boolean", at);
        type = make_shared<TypeDecl>(Type::tVoid);
    }
    
    SimNode * ExprAssert::simulate (Context & context) const
    {
        return context.makeNode<SimNode_Assert>(at,subexpr->simulate(context),context.allocateName(message));
    }
    
    // ExprDebug
    
    ExpressionPtr ExprDebug::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprDebug>(expr);
        cexpr->subexpr = subexpr->clone();
        cexpr->message = message;
        return cexpr;
    }
    
    void ExprDebug::log(ostream& stream, int depth) const
    {
        stream << "(debug ";
        stream << *subexpr;
        if ( !message.empty() )
            stream << " \"" << message << "\"";
        stream << ")";
    }
    
    void ExprDebug::inferType(InferTypeContext & context)
    {
        thisProgram = context.program.get();
        subexpr->inferType(context);
        type = make_shared<TypeDecl>(*subexpr->type);
    }
    
    SimNode * ExprDebug::simulate (Context & context) const
    {
        TypeInfo * pTypeInfo = context.makeNode<TypeInfo>();
        thisProgram->makeTypeInfo(pTypeInfo, context, subexpr->type);
        return context.makeNode<SimNode_Debug>(at,
                                               subexpr->simulate(context),
                                               pTypeInfo,
                                               context.allocateName(message));
    }
    
    // ExprSizeOf
    
    ExpressionPtr ExprSizeOf::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprSizeOf>(expr);
        if ( subexpr )
            cexpr->subexpr = subexpr->clone();
        if ( typeexpr )
            cexpr->typeexpr = typeexpr;
        return cexpr;
    }
    
    void ExprSizeOf::log(ostream& stream, int depth) const
    {
        stream << "(sizeof ";
        if ( subexpr )
            stream << *subexpr;
        else
            stream << "(" << *typeexpr << ")";
        stream << ")";
    }
    
    void ExprSizeOf::inferType(InferTypeContext & context)
    {
        if ( subexpr ) {
            subexpr->inferType(context);
            if ( subexpr->type )
                typeexpr = make_shared<TypeDecl>(*subexpr->type);
        }
        type = make_shared<TypeDecl>(Type::tInt);
    }
    
    SimNode * ExprSizeOf::simulate (Context & context) const
    {
        int32_t size = typeexpr->getSizeOf();
        return context.makeNode<SimNode_ConstValue<int32_t>>(at,size);
    }
    
    // ExprNew
    
    ExpressionPtr ExprNew::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprNew>(expr);
        cexpr->typeexpr = typeexpr;
        return cexpr;
    }
    
    void ExprNew::log(ostream& stream, int depth) const
    {
        stream << "(new (" << *typeexpr << "))";
    }
    
    void ExprNew::inferType(InferTypeContext & context)
    {
        if ( typeexpr->baseType != Type::tStructure ) {
            context.error("can only new structures (for now)", typeexpr->at );
        } else if ( typeexpr->ref ) {
            context.error("can't new a ref", typeexpr->at);
        } else if ( typeexpr->dim.size() ) {
            context.error("can only new single object", typeexpr->at );
        } else {
            type = make_shared<TypeDecl>(Type::tPointer);
            type->structType = typeexpr->structType;
        }
    }
    
    SimNode * ExprNew::simulate (Context & context) const
    {
        int32_t bytes = typeexpr->getSizeOf();
        return context.makeNode<SimNode_New>(at,bytes);
    }

    // ExprAt
    
    void ExprAt::log(ostream& stream, int depth) const
    {
        stream << "(@ " << *subexpr << " " << *index << ")";
    }
    
    void ExprAt::inferType(InferTypeContext & context)
    {
        subexpr->inferType(context);
        if ( !subexpr->type->isRef() ) {
            context.error("can only index ref", subexpr->at);
        } else if ( !subexpr->type->dim.size() ) {
            context.error("can only index arrays", subexpr->at);
        } else {
            index->inferType(context);
            index = autoDereference(index);
            if ( !index->type->isIndex() ) {
                context.error("index is int or uint", index->at);
            } else {
                type = make_shared<TypeDecl>(*subexpr->type);
                type->ref = true;
                type->dim.pop_back();
            }
        }
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
        uint32_t stride = subexpr->type->getStride();
        uint32_t range = subexpr->type->dim.back();
        return context.makeNode<SimNode_At>(at, prv, pidx, stride, range);
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
        // TODO: what if list size is 0?
        if ( list.size()!=1 ) {
            auto block = context.makeNode<SimNode_Block>(at);
            block->total = int(list.size());
            block->list = (SimNode **) context.allocate(sizeof(SimNode *)*block->total);
            for ( int i = 0; i != block->total; ++i )
                block->list[i] = list[i]->simulate(context);
            return block;
        } else {
            return list[0]->simulate(context);
        }
    }
    
    // ExprField
    
    ExpressionPtr ExprField::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprField>(expr);
        cexpr->name = name;
        cexpr->value = value->clone();
        cexpr->field = field;
        return cexpr;
    }
    
    void ExprField::log(ostream& stream, int depth) const
    {
        logType(stream);
        stream << "(. ";
        value->log(stream,depth+1);
        stream << " " << name << ")";
    }
    
    void ExprField::inferType(InferTypeContext & context)
    {
        value->inferType(context);
        if ( !value->type ) return;
        if ( value->type->baseType==Type::tPointer )
            value = autoDereference(value);
        if ( value->type->baseType!=Type::tStructure && value->type->baseType!=Type::tPointer ) {
            context.error("expecting structure or pointer", at);
        } else if ( value->type->isArray() ) {
            context.error("can't get field of array", at);
        } else {
            field = value->type->structType->findField(name);
            if ( !field ) {
                context.error("field " + name + " not found", at);
            } else {
                type = make_shared<TypeDecl>(*field->type);
                type->ref = true;
            }
        }
    }
    
    SimNode * ExprField::simulate (Context & context) const
    {
        if ( value->type->baseType==Type::tStructure )
            return context.makeNode<SimNode_Field>(at,value->simulate(context),field->offset);
        else
            return context.makeNode<SimNode_PtrField>(at,value->simulate(context),field->offset);
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
                type->ref = true;
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
                type->ref = arg->type->ref;
                return;
            }
            argumentIndex ++;
        }
        // global
        auto var = context.program->findVariable(name);
        if ( !var ) {
            context.error("can't locate variable " + name, at);
        } else {
            variable = var;
            type = make_shared<TypeDecl>(*var->type);
            type->ref = true;
        }
    }
    
    SimNode * ExprVar::simulate (Context & context) const
    {
        if ( local ) {
            return context.makeNode<SimNode_GetLocal>(at, variable->stackTop);
        } else if ( argument) {
            return context.makeNode<SimNode_GetArgument>(at, argumentIndex);
        } else {
            return context.makeNode<SimNode_GetGlobal>(at, variable->index);
        }
    }

    
    // ExprOp
    
    ExpressionPtr ExprOp::clone( const ExpressionPtr & expr ) const
    {
        if ( !expr )
            throw runtime_error("can't clone ExprOp");
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
        if ( functions.size()==0 ) {
            context.error("no matching function " + to_string(op) + " (" + subexpr->type->describe() + ")", at);
        } else if ( functions.size()>1 ) {
            context.error("too many matching functions", at);
        } else {
        func = functions[0];
            if ( !func->builtIn ) {
                context.error("operator must point to built-in function every time", at);
            } else {
                type = make_shared<TypeDecl>(*func->result);
                if ( !func->arguments[0]->type->isRef() )
                    subexpr = autoDereference(subexpr);
            }
        }
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
        if ( !left->type ) return;
        if ( !right->type ) return;
        vector<TypeDeclPtr> types = { left->type, right->type };
        auto functions = context.program->findMatchingFunctions(to_string(op), types);
        if ( functions.size()==0 ) {
            context.error("no matching function " + to_string(op)
                + " (" + left->type->describe() + ", " + right->type->describe() + ")", at);
        } else if ( functions.size()>1 ) {
            context.error("too many matching functions", at);
        } else {
            func = functions[0];
            if ( !func->builtIn ) {
                context.error("operator must point to built-in function every time", at);
            } else {
                type = make_shared<TypeDecl>(*func->result);
                if ( !func->arguments[0]->type->isRef() )
                    left = autoDereference(left);
                if ( !func->arguments[1]->type->isRef() )
                    right = autoDereference(right);
            }
        }
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
        if ( !subexpr->type->isSimpleType(Type::tBool) ) {
            context.error("cond operator condition must be boolean", at);
        } else {
            left->inferType(context);
            right->inferType(context);
            vector<TypeDeclPtr> types = { subexpr->type, left->type, right->type };
            auto functions = context.program->findMatchingFunctions(to_string(op), types);
            if ( functions.size()==0 ) {
                context.error("no matching function " + to_string(op)
                    + " (" + subexpr->type->describe() + ", "
                        + left->type->describe() + ", " + right->type->describe() + ")", at);
            } else if ( functions.size()>1 ) {
                context.error("too many matching functions", at);
            } else {
                func = functions[0];
                type = make_shared<TypeDecl>(*func->result);
                if ( !func->arguments[0]->type->isRef() )
                    subexpr = autoDereference(subexpr);
                if ( !func->arguments[1]->type->isRef() )
                    left = autoDereference(left);
                if ( !func->arguments[2]->type->isRef() )
                    right = autoDereference(right);
            }
        }
    }
    
    SimNode * ExprOp3::simulate (Context & context) const
    {
        assert(0 && "implement");
        return nullptr;
    }
    
    // ExprReturn
    
    void ExprReturn::log(ostream& stream, int depth) const
    {
        stream << "return";
    }
    
    void ExprReturn::inferType(InferTypeContext & context)
    {
        if ( context.func->result->isVoid() ) {
            context.error("void function has no return", at);
        } else {
            type = make_shared<TypeDecl>(*context.func->result);
            type->ref = true;
        }
    }
    
    SimNode * ExprReturn::simulate (Context & context) const
    {
        return context.makeNode<SimNode_Return>(at);
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
        if ( !cond->type->isSimpleType(Type::tBool) ) {
            context.error("if-then-else condition must be boolean", at);
        } else {
            if_true->inferType(context);
            if ( if_false )
                if_false->inferType(context);
            type = make_shared<TypeDecl>();
        }
    }
    
    SimNode * ExprIfThenElse::simulate (Context & context) const
    {
        return context.makeNode<SimNode_IfThenElse>(at, cond->simulate(context), if_true->simulate(context),
                                                    if_false ? if_false->simulate(context) : nullptr);
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
        if ( !cond->type->isSimpleType(Type::tBool) ) {
            context.error("while loop condition must be boolean", at);
        } else {
            body->inferType(context);
            type = make_shared<TypeDecl>();
        }
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
        return context.makeNode<SimNode_While>(at, cond->simulate(context),body->simulate(context));
    }
    
    // ExprForeach
    
    ExpressionPtr ExprForeach::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprForeach>(expr);
        cexpr->head = head->clone();
        cexpr->iter = iter->clone();
        cexpr->body = body->clone();
        return cexpr;
    }
    
    void ExprForeach::inferType(InferTypeContext & context)
    {
        head->inferType(context);
        iter->inferType(context);
        if ( !head->type || !iter->type ) return;
        if ( head->type->dim.size()!=1 ) {   // TODO: support multi-array
            context.error("can only iterate through a 1-d array", at);
        } else if ( !head->type->isIteratorType(*iter->type) ) {
            context.error("iterator type does not match", at);
        } else {
            body->inferType(context);
            type = make_shared<TypeDecl>();
        }
    }
    
    void ExprForeach::log(ostream& stream, int depth) const
    {
        stream << "(foreach\n"<< string(depth+1,'\t');
        head->log(stream, depth+1);
        stream << " " << *iter;
        stream << "\n" << string(depth+2,'\t');
        body->log(stream, depth+2);
        stream << ")";
    }
    
    SimNode * ExprForeach::simulate (Context & context) const
    {
        return context.makeNode<SimNode_Foreach>(at,
                                                 head->simulate(context),
                                                 iter->simulate(context),
                                                 body->simulate(context),
                                                 (int) head->type->dim[0],
                                                 iter->type->getSizeOf(),
                                                 iter->type->getSizeOf());
    }
    
    // ExprTryCatch
    
    ExpressionPtr ExprTryCatch::clone( const ExpressionPtr & expr ) const
    {
        auto cexpr = clonePtr<ExprTryCatch>(expr);
        cexpr->try_this = try_this->clone();
        cexpr->catch_that = catch_that->clone();
        return cexpr;
    }
    
    void ExprTryCatch::inferType(InferTypeContext & context)
    {
        try_this->inferType(context);
        catch_that->inferType(context);
        type = make_shared<TypeDecl>();
    }
    
    void ExprTryCatch::log(ostream& stream, int depth) const
    {
        stream << "(try\n"<< string(depth+1,'\t');
        try_this->log(stream, depth+1);
        stream << "\n" << string(depth+2,'\t');
        catch_that->log(stream, depth+2);
        stream << ")";
    }
    
    SimNode * ExprTryCatch::simulate (Context & context) const
    {
        return context.makeNode<SimNode_TryCatch>(at,try_this->simulate(context),catch_that->simulate(context));
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
            if ( var->init )
                stream << string(depth+1, '\t') << "(" << *var << " " << *var->init << ")\n";
            else
                stream << string(depth+1, '\t') << "(" << *var << ")\n";
        }
        stream << string(depth+2, '\t');
        subexpr->log(stream, depth+2);
        stream << ")";
    }
    
    void ExprLet::inferType(InferTypeContext & context)
    {
        auto sp = context.stackTop;
        auto sz = context.local.size();
        for ( auto & var : variables ) {
            context.local.push_back(var);
            if ( var->init ) {
                var->init->inferType(context);
                if ( !var->type->isSameType(*var->init->type,false) ) {
                    context.error("variable initialization type mismatch", var->at );
                } else if ( var->type->baseType==Type::tStructure ) {
                    context.error("can't initialize structures", var->at );
                } else {
                    var->init = autoDereference(var->init);
                }
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
        auto let = context.makeNode<SimNode_Let>(at);
        let->total = (uint32_t) variables.size();
        let->list = (SimNode **) context.allocate(let->total * sizeof(SimNode*));
        int vi = 0;
        for ( auto & var : variables ) {
            int size = var->type->getSizeOf();
            if ( var->init ) {
                SimNode * copy = nullptr;
                auto init = var->init->simulate(context);
                auto get = context.makeNode<SimNode_GetLocal>(at, var->stackTop);
                if ( var->init->type->isRef() ) {
                    copy = context.makeNode<SimNode_CopyRefValue>(at, get, init, size);
                } else {
                    switch ( var->type->baseType ) {
                        case Type::tBool:       copy = context.makeNode<SimNode_CopyValue<bool>>(at, get, init);       break;
                        case Type::tInt:        copy = context.makeNode<SimNode_CopyValue<int32_t>>(at, get, init);    break;
                        case Type::tUInt:       copy = context.makeNode<SimNode_CopyValue<uint32_t>>(at, get, init);   break;
                        case Type::tFloat:      copy = context.makeNode<SimNode_CopyValue<float>>(at, get, init);      break;
                        case Type::tString:     copy = context.makeNode<SimNode_CopyValue<char *>>(at, get, init);     break;
                        case Type::tPointer:    copy = context.makeNode<SimNode_CopyValue<void *>>(at, get, init);     break;
                        default:
                            throw runtime_error("unsupported? can't assign initial value");
                    }
                }
                let->list[vi++] = copy;
            } else {
                let->list[vi++] = context.makeNode<SimNode_InitLocal>(at, var->stackTop, var->type->getSizeOf());
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
    
    string ExprCall::describe() const
    {
        stringstream stream;
        stream << "(" << name;
        for ( auto & arg : arguments ) {
            if ( arg->type )
                stream << " " << *arg->type;
            else
                stream << " ???";
        }
        stream << ")";
        return stream.str();
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
        stackTop = context.stackTop;
        context.stackTop = (stackTop + arguments.size()*sizeof(__m128));
        context.func->totalStackSize = max(context.func->totalStackSize, context.stackTop);
        vector<TypeDeclPtr> types;
        types.reserve(arguments.size());
        for ( auto & ar : arguments ) {
            ar->inferType(context);
            types.push_back(ar->type);
        }
        auto functions = context.program->findMatchingFunctions(name, types);
        if ( functions.size()==0 ) {
            context.error("no matching function " + describe(), at);
        } else if ( functions.size()>1 ) {
            context.error("too many matching functions " + describe(), at);
        } else {
            func = functions[0];
            type = make_shared<TypeDecl>(*func->result);
            for ( size_t iT = arguments.size(); iT != func->arguments.size(); ++iT ) {
                auto newArg = func->arguments[iT]->init->clone();
                if ( !newArg->type )
                    newArg->inferType(context);
                arguments.push_back(newArg);
            }
            for ( size_t iA = 0; iA != arguments.size(); ++iA )
                if ( !func->arguments[iA]->type->isRef() )
                    arguments[iA] = autoDereference(arguments[iA]);
        }
        context.stackTop = stackTop;
    }
    
    SimNode * ExprCall::simulate (Context & context) const
    {
        SimNode_Call * pCall = static_cast<SimNode_Call *>(func->makeSimNode(context));
        pCall->debug = at;
        pCall->fnIndex = func->index;
        if ( int nArg = (int) arguments.size() ) {
            pCall->arguments = (SimNode **) context.allocate(nArg * sizeof(SimNode *));
            pCall->nArguments = nArg;
            pCall->stackTop = stackTop;
            for ( int a=0; a!=nArg; ++a ) {
                pCall->arguments[a] = arguments[a]->simulate(context);
            }
        } else {
            pCall->arguments = nullptr;
            pCall->nArguments = 0;
            pCall->stackTop = 0;
        }
        return pCall;
    }

    // program
    
    StructurePtr Program::findStructure ( const string & name ) const
    {
        auto it = structures.find(name);
        if ( it != structures.end() )
            return it->second;
        return nullptr;
    }
    
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
                context.func->totalStackSize = context.stackTop = sizeof(Prologue);
                context.func->index = totalFunctions ++;
                for ( auto & arg : context.func->arguments ) {
                    if ( arg->init ) {
                        arg->init->inferType(context);
                        if ( failed() )
                            return;
                        if ( !arg->type->isSameType(*arg->init->type, true) ) {
                            context.error("function argument default value type mismatch", arg->init->at);
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
        if ( findFunction(mangledName) ) {
            error("builtin function already defined " + mangledName, LineInfo());
        } else {
            functions[mangledName] = func;
            functionsByName[func->name].push_back(func);
        }
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
                    if ( passType && ((argType->isRef() && !passType->isRef()) || !argType->isSameType(*passType, false)) ) {
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
    
    FuncInfo * Program::makeFunctionDebugInfo ( Context & context, const Function & fn )
    {
        FuncInfo * fni = context.makeNode<FuncInfo>();
        fni->name = context.allocateName(fn.name);
        fni->stackSize = fn.totalStackSize;
        fni->argsSize = (uint32_t) fn.arguments.size();
        fni->args = (VarInfo **) context.allocate(sizeof(VarInfo *) * fni->argsSize);
        for ( uint32_t i=0; i!=fni->argsSize; ++i ) {
            fni->args[i] = makeVariableDebugInfo(context, *fn.arguments[i]);
        }
        return fni;
    }
    
    StructInfo * Program::makeStructureDebugInfo ( Context & context, const Structure & st )
    {
        StructInfo * sti = context.makeNode<StructInfo>();
        sti->name = context.allocateName(st.name);
        sti->fieldsSize = (uint32_t) st.fields.size();
        sti->fields = (VarInfo **) context.allocate( sizeof(VarInfo *) * sti->fieldsSize );
        for ( uint32_t i=0; i!=sti->fieldsSize; ++i ) {
            auto & var = st.fields[i];
            VarInfo * vi = context.makeNode<VarInfo>();
            makeTypeInfo(vi, context, var.type);
            vi->name = context.allocateName(var.name);
            sti->fields[i] = vi;
        }
        return sti;
    }
    
    void Program::makeTypeInfo ( TypeInfo * info, Context & context, const TypeDeclPtr & type )
    {
        info->type = type->baseType;
        info->dimSize = (uint32_t) type->dim.size();
        if ( info->dimSize ) {
            info->dim = (uint32_t *) context.allocate(sizeof(uint32_t) * info->dimSize );
            for ( uint32_t i=0; i != info->dimSize; ++i ) {
                info->dim[i] = type->dim[i];
            }
        }
        if ( type->baseType==Type::tStructure || type->baseType==Type::tPointer ) {
            auto st = sdebug.find(type->structType->name);
            if ( st==sdebug.end() ) {
                info->structType = makeStructureDebugInfo(context, *type->structType);
                sdebug[type->structType->name] = info->structType;
            } else {
                info->structType = st->second;
            }
        } else {
            info->structType = nullptr;
        }
        info->ref = type->ref;
    }

    VarInfo * Program::makeVariableDebugInfo ( Context & context, const Variable & var )
    {
        VarInfo * vi = context.makeNode<VarInfo>();
        makeTypeInfo(vi, context, var.type);
        vi->name = context.allocateName(var.name);
        return vi;
    }
    
    void Program::simulate ( Context & context )
    {
        context.globalVariables = (GlobalVariable *) context.allocate( uint32_t(globals.size()*sizeof(GlobalVariable)) );
        for ( auto & it : globals ) {
            auto pvar = it.second;
            auto & gvar = context.globalVariables[pvar->index];
            gvar.name = context.allocateName(pvar->name);
            gvar.size = pvar->type->getSizeOf();
            gvar.debug = makeVariableDebugInfo(context, *it.second);
            void * data = context.allocate(gvar.size);
            memset(data, 0, gvar.size);
            gvar.value = cast<void *>::from(data);
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
            gfun.debug = makeFunctionDebugInfo(context, *pfun);
        }
        context.linearAllocatorExecuteBase = context.linearAllocator;
        context.restart();
        sdebug.clear();
    }
    
    TypeDeclPtr Program::makeStructureType ( const string & name ) const
    {
        auto t = make_shared<TypeDecl>();
        t->baseType = Type::tStructure;
        t->structType = findStructure(name).get();
        if ( !t->structType )
            throw runtime_error("can't make structure type " + name);
        return t;
    }
    
    void Program::error ( const string & str, const LineInfo & at )
    {
        errors.emplace_back(str,at);
        failToCompile = true;
    }
    
    // PARSER
    
    ProgramPtr g_Program;
    
    ProgramPtr parseDaScript ( const char * script, function<void (const ProgramPtr & prg)> && defineContext )
    {
        int err;
        auto program = g_Program = make_shared<Program>();
        yybegin(script);
        err = yyparse();        // TODO: add mutex or make thread safe?
        g_Program = nullptr;
        if ( err || program->failed() ) {
            sort(program->errors.begin(),program->errors.end());
            return program;
        } else {
            program->addBuiltinOperators();
            program->addBuiltinFunctions();
            if ( defineContext )
                defineContext(program);
            program->inferTypes();
            return program;
        }
    }
    
}
