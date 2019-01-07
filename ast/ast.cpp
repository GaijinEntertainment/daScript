#include "precomp.h"

#include "ast.h"
#include "enums.h"

#include "runtime_array.h"
#include "runtime_table.h"
#include "runtime_range.h"
#include "hash.h"

void yybegin(const char * str);
int yyparse();
int yylex_destroy();

namespace yzg
{
    // TypeDecl
    
    string TypeDecl::describe ( bool extra ) const {
        stringstream stream;
        if ( baseType==Type::alias ) {
            stream << alias;
        } else if ( baseType==Type::autoinfer ) {
            stream << "auto";
            if ( !alias.empty() ) {
                stream << "(" << alias << ")";
            }
        } else if ( baseType==Type::tHandle ) {
            stream << annotation->name;
        } else if ( baseType==Type::tArray ) {
            if ( firstType ) {
                stream << "array<" << firstType->describe(extra) << ">";
            } else {
                stream << "array";
            }
        } else if ( baseType==Type::tTable ) {
            if ( firstType && secondType ) {
                stream << "table<" << firstType->describe(extra) << "," << secondType->describe(extra) << ">";
            } else {
                stream << "table";
            }
        } else if ( baseType==Type::tStructure ) {
            if ( structType ) {
                stream << structType->name;
            } else {
                stream << "unspecified";
            }
        } else if ( baseType==Type::tPointer ) {
            if ( firstType ) {
                stream << firstType->describe(extra) << "?";
            } else {
                stream << "void ?";
            }
        } else if ( baseType==Type::tIterator ) {
            if ( firstType ) {
                stream << "iterator<" << firstType->describe(extra) << ">";
            } else {
                stream << "iterator";
            }
        } else if ( baseType==Type::tBlock ) {
            stream << "block<";
            if ( argTypes.size() ) {
                stream << "(";
                for ( const auto & arg : argTypes ) {
                    stream << arg->describe(extra);
                    if ( arg != argTypes.back() ) {
                        stream << ";";
                    }
                }
                stream << ")";
            }
            if ( firstType ) {
                if ( argTypes.size() ) {
                    stream << ":";
                }
                stream << firstType->describe(extra);
            }
            stream << ">";
        } else {
            stream << to_string(baseType);
        }
        if ( extra && baseType!=Type::autoinfer && baseType!=Type::alias && !alias.empty() ) {
            stream << " aka " << alias;
        }
        if ( constant ) {
            stream << " const";
        }
        for ( auto d : dim ) {
            stream << "[" << d << "]";
        }
        if ( ref )
            stream << "&";
        return stream.str();
    }

    ostream& operator<< (ostream& stream, const TypeDecl & decl) {
        stream << decl.describe();
        return stream;
    }
    
    TypeDecl::TypeDecl(const TypeDecl & decl) {
        baseType = decl.baseType;
        structType = decl.structType;
        annotation = decl.annotation;
        dim = decl.dim;
        flags = decl.flags;
        alias = decl.alias;
        at = decl.at;
        if ( decl.firstType )
            firstType = make_shared<TypeDecl>(*decl.firstType);
        if ( decl.secondType )
            secondType = make_shared<TypeDecl>(*decl.secondType);
        for ( const auto & arg : decl.argTypes ) {
            argTypes.push_back(make_shared<TypeDecl>(*arg) );
        }
    }
    
    const TypeDecl * TypeDecl::findAlias ( const string & name, bool allowAuto ) const {
        if ( baseType==Type::tPointer ) {
            return firstType ? firstType->findAlias(name,allowAuto) : nullptr;
        } else if ( baseType==Type::tArray ) {
            return firstType ? firstType->findAlias(name,allowAuto) : nullptr;
        } else if ( baseType==Type::tTable ) {
            if ( firstType ) {
                if ( auto k = firstType->findAlias(name,allowAuto) ) {
                    return k;
                }
            }
            return secondType ? secondType->findAlias(name,allowAuto) : nullptr;
        } else if ( baseType==Type::tBlock ) {
            for ( auto & arg : argTypes ) {
                if ( auto at = arg->findAlias(name,allowAuto) ) {
                    return at;
                }
            }
            return firstType->findAlias(name,allowAuto);
        } else if ( baseType==Type::alias ) {
            return nullptr; // if it is another alias, can't find it
        } else if ( baseType==Type::autoinfer && !allowAuto) {
            return nullptr; // if it has not been infered yet, can't find it
        } else {
            return alias==name ? this : nullptr;
        }
    }
    
    bool TypeDecl::canMove() const {
        if ( baseType==Type::tHandle )
            return annotation->canMove();
        if ( baseType==Type::tBlock )
            return false;
        return true;
    }
    
    bool TypeDecl::canCopy() const {
        if ( baseType==Type::tHandle )
            return annotation->canCopy();
        if ( baseType==Type::tArray || baseType==Type::tTable || baseType==Type::tBlock )
            return false;
        if ( baseType==Type::tStructure && structType )
            return structType->canCopy();
        return true;
    }
    
    bool TypeDecl::isPod() const {
        if ( baseType==Type::tArray || baseType==Type::tTable || baseType==Type::tString || baseType==Type::tBlock )
            return false;
        if ( baseType==Type::tStructure && structType )
            return structType->isPod();
        if ( baseType==Type::tHandle )
            return annotation->isPod();
        return true;
    }
    
    string TypeDecl::getMangledName() const {
        stringstream ss;
        if ( constant ) {
            ss << "#const";
        }
        if ( baseType==Type::tHandle ) {
            ss << "#handle#" << annotation->name;
        } else if ( baseType==Type::tArray ) {
            ss << "#array";
            if ( firstType ) {
                ss << "#" << firstType->getMangledName();
            }
        } else if ( baseType==Type::tTable ) {
            ss << "#table";
            if ( firstType ) {
                ss << "#" << firstType->getMangledName();
            }
            if ( secondType ) {
                ss << "#" << secondType->getMangledName();
            }
        } else if ( baseType==Type::tPointer ) {
            ss << "#ptr";
            if ( firstType ) {
                ss << "#" << firstType->getMangledName();
            }
        } else if ( baseType==Type::tIterator ) {
            ss << "#iterator";
            if ( firstType ) {
                ss << "#" << firstType->getMangledName();
            }
        } else if ( baseType==Type::tBlock ) {
            ss << "#block";
            for ( auto & arg : argTypes ) {
                ss << "#" << arg->getMangledName();
            }
            if ( firstType ) {
                ss << "#:" << firstType->getMangledName();
            }
        } else if ( baseType==Type::tStructure ) {
            if ( structType ) {
                ss << structType->name;
            } else {
                ss << "structue?";
            }
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
    
    bool TypeDecl::isConst() const
    {
        if ( constant )
            return true;
        if ( baseType==Type::tPointer )
            if ( firstType && firstType->constant )
                return true;
        return false;
    }
    
    bool TypeDecl::isSameType ( const TypeDecl & decl, bool refMatters, bool constMatters ) const {
        if ( baseType!=decl.baseType )
            return false;
        if ( baseType==Type::tHandle && annotation!=decl.annotation )
            return false;
        if ( baseType==Type::tStructure && structType!=decl.structType )
            return false;
        if ( baseType==Type::tPointer || baseType==Type::tIterator ) {
            if ( firstType && decl.firstType && !firstType->isSameType(*decl.firstType) ) {
                return false;
            }
        }
        if ( baseType==Type::tArray ) {
            if ( firstType && decl.firstType && !firstType->isSameType(*decl.firstType) ) {
                return false;
            }
        }
        if ( baseType==Type::tTable ) {
            if ( firstType && decl.firstType && !firstType->isSameType(*decl.firstType) ) {
                return false;
            }
            if ( secondType && decl.secondType && !secondType->isSameType(*decl.secondType) ) {
                return false;
            }
        }
        if ( baseType==Type::tBlock ) {
            if ( firstType && decl.firstType && !firstType->isSameType(*decl.firstType) ) {
                return false;
            }
            if ( firstType && argTypes.size()==0 ) {    // if not any block
                if ( argTypes.size() != decl.argTypes.size() ) {
                    return false;
                }
                for ( size_t i=0; i != argTypes.size(); ++i ) {
                    const auto & arg = argTypes[i];
                    const auto & declArg = decl.argTypes[i];
                    if ( !arg->isSameType(*declArg) ) {
                        return false;
                    }
                }
            }
        }
        if ( dim!=decl.dim )
            return false;
        if ( refMatters )
            if ( ref!=decl.ref )
                return false;
        if ( constMatters )
            if ( constant!=decl.constant )
                return false;
        return true;
    }
    
    bool TypeDecl::isGoodIteratorType() const {
        return baseType==Type::tIterator && dim.size()==0 && firstType;
    }
    
    bool TypeDecl::isGoodBlockType() const {
        return baseType==Type::tBlock && dim.size()==0;
    }
    
    bool TypeDecl::isGoodArrayType() const {
        return baseType==Type::tArray && dim.size()==0 && firstType;
    }
    
    bool TypeDecl::isGoodTableType() const {
        return baseType==Type::tTable && dim.size()==0 && firstType && secondType;
    }
    
    bool TypeDecl::isIteratorType ( const TypeDecl & decl ) const {
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
    
    bool TypeDecl::isVoid() const {
        return (baseType==Type::tVoid) && (dim.size()==0);
    }
    
    bool TypeDecl::isPointer() const {
        return (baseType==Type::tPointer) && (dim.size()==0);
    }

    bool TypeDecl::isAlias() const {
        // auto is auto.... or auto....?
        if ( baseType==Type::alias ) {
            return true;
        } else  if ( baseType==Type::tPointer ) {
            if ( firstType )
                return firstType->isAlias();
        } else if ( baseType==Type::tArray ) {
            if ( firstType )
                return firstType->isAlias();
        } else if ( baseType==Type::tTable ) {
            bool any = false;
            if ( firstType )
                any |= firstType->isAlias();
            if ( secondType )
                any |= secondType->isAlias();
            return any;
        } else if ( baseType==Type::tBlock ) {
            bool any = false;
            if ( firstType )
                any |= firstType->isAlias();
            for ( auto & arg : argTypes )
                any |= arg->isAlias();
            return any;
        }
        return false;
    }
    
    bool TypeDecl::isAuto() const {
        // auto is auto.... or auto....?
        if ( baseType==Type::autoinfer ) {
            return true;
        } else  if ( baseType==Type::tPointer ) {
            if ( firstType )
                return firstType->isAuto();
        } else if ( baseType==Type::tArray ) {
            if ( firstType )
                return firstType->isAuto();
        } else if ( baseType==Type::tTable ) {
            bool any = false;
            if ( firstType )
                any |= firstType->isAuto();
            if ( secondType )
                any |= secondType->isAuto();
            return any;
        } else if ( baseType==Type::tBlock ) {
            bool any = false;
            if ( firstType )
                any |= firstType->isAuto();
            for ( auto & arg : argTypes )
                any |= arg->isAuto();
            return any;
        }
        return false;
    }
    
    bool TypeDecl::isFoldable() const {
        if ( dim.size() || ref )
            return false;
        switch ( baseType ) {
            case Type::tBool:
            case Type::tInt:
            case Type::tInt2:
            case Type::tInt3:
            case Type::tInt4:
            case Type::tUInt64:
            case Type::tUInt:
            case Type::tUInt2:
            case Type::tUInt3:
            case Type::tUInt4:
            case Type::tFloat:
            case Type::tFloat2:
            case Type::tFloat3:
            case Type::tFloat4:
                return true;
            default:
                return false;
        }
    }
    
    bool TypeDecl::isHandle() const {
        return (baseType==Type::tHandle) && (dim.size()==0);
    }
    
    bool TypeDecl::isSimpleType() const {
        if (    baseType==Type::none
            ||  baseType==Type::tVoid
            ||  baseType==Type::tStructure
            ||  baseType==Type::tPointer)
            return false;
        if ( dim.size() )
            return false;
        return true;
    }
    
    bool TypeDecl::isCtorType() const {
        if ( dim.size() )
            return false;
        switch ( baseType ) {
            // case Type::tBool:
            case Type::tInt64:
            case Type::tUInt64:
            case Type::tInt:
            case Type::tInt2:
            case Type::tInt3:
            case Type::tInt4:
            case Type::tUInt:
            case Type::tUInt2:
            case Type::tUInt3:
            case Type::tUInt4:
            case Type::tFloat:
            case Type::tFloat2:
            case Type::tFloat3:
            case Type::tFloat4:
            case Type::tRange:
            case Type::tURange:
            case Type::tString:
            // case Type::tPointer:
                return true;
            default:
                return false;
        }
    }
    
    bool TypeDecl::isWorkhorseType() const {
        if ( dim.size() )
            return false;
        switch ( baseType ) {
            case Type::tBool:
            case Type::tInt64:
            case Type::tUInt64:
            case Type::tInt:
            case Type::tInt2:
            case Type::tInt3:
            case Type::tInt4:
            case Type::tUInt:
            case Type::tUInt2:
            case Type::tUInt3:
            case Type::tUInt4:
            case Type::tFloat:
            case Type::tFloat2:
            case Type::tFloat3:
            case Type::tFloat4:
            case Type::tRange:
            case Type::tURange:
            case Type::tString:
            case Type::tPointer:
                return true;
            default:
                return false;
        }
    }
    
    Type TypeDecl::getRangeBaseType() const
    {
        switch ( baseType ) {
            case Type::tRange:  return Type::tInt;
            case Type::tURange: return Type::tUInt;
            default:
                assert(0 && "we should not even be here");
                return Type::none;
        }
    }
    
    bool TypeDecl::isRange() const
    {
        return (baseType==Type::tRange || baseType==Type::tURange) && dim.size()==0;
    }
    
    bool TypeDecl::isSimpleType(Type typ) const {
        return baseType==typ && isSimpleType();
    }
    
    bool TypeDecl::isArray() const {
        return dim.size() != 0;
    }
    
    bool TypeDecl::isRef() const {
        return ref || isRefType();
    }
    
    bool TypeDecl::isRefType() const {
        if ( baseType==Type::tHandle ) {
            return annotation->isRefType();
        }
        return baseType==Type::tStructure || baseType==Type::tArray || baseType==Type::tTable || dim.size();
    }
    
    bool TypeDecl::isIndex() const {
        return (baseType==Type::tInt || baseType==Type::tUInt) && dim.size()==0;
    }
    
    int TypeDecl::getBaseSizeOf() const {
        if ( baseType==Type::tHandle ) {
            return int(annotation->getSizeOf());
        }
        return baseType==Type::tStructure ? structType->getSizeOf() : getTypeBaseSize(baseType);
    }
    
    int TypeDecl::getSizeOf() const {
        int size = 1;
        for ( auto i : dim )
            size *= i;
        return getBaseSizeOf() * size;
    }
    
    int TypeDecl::getStride() const {
        int size = 1;
        if ( dim.size() > 1 ) {
            for ( size_t i=0; i!=dim.size()-1; ++i )
                size *= dim[i];
        }
        return getBaseSizeOf() * size;
    }
    
    // structure
    
    bool Structure::canCopy() const {
        for ( const auto & fd : fields ) {
            if ( !fd.type->canCopy() )
                return false;
        }
        return true;
    }
    
    bool Structure::isPod() const {
        for ( const auto & fd : fields ) {
            if ( !fd.type->isPod() )
                return false;
        }
        return true;
    }
    
    int Structure::getSizeOf() const {
        int size = 0;
        for ( const auto & fd : fields ) {
            size += fd.type->getSizeOf();
        }
        return size;
    }
    
    const Structure::FieldDeclaration * Structure::findField ( const string & name ) const {
        for ( const auto & fd : fields ) {
            if ( fd.name==name ) {
                return &fd;
            }
        }
        return nullptr;
    }
    
    ostream& operator<< (ostream& stream, const Structure & structure) {
        stream << "(struct " << structure.name << "\n";
        for ( auto & decl : structure.fields ) {
            stream << "\t(" << *decl.type << " " << decl.name << ")\n";
        }
        stream << ")";
        return stream;
    }

    // variable
    
    ostream& operator<< (ostream& stream, const Variable & var) {
        stream << *var.type << " " << var.name;
        return stream;
    }
    
    VariablePtr Variable::clone() const {
        auto pVar = make_shared<Variable>();
        pVar->name = name;
        pVar->type = make_shared<TypeDecl>(*type);
        if ( init )
            pVar->init = init->clone();
        if ( source )
            pVar->source = source->clone();
        pVar->at = at;
        return pVar;
    }
    
    // function
    
    FunctionPtr Function::clone() const {
        auto cfun = make_shared<Function>();
        // TODO:
        //  what do we do with annotations?
        cfun->name = name;
        for ( const auto & arg : arguments ) {
            cfun->arguments.push_back(arg->clone());
        }
        cfun->result = make_shared<TypeDecl>(*result);
        cfun->body = body->clone();
        cfun->index = -1;
        cfun->totalStackSize = 0;
        cfun->at = at;
        cfun->module = nullptr;
        cfun->flags = 0;
        cfun->inferStack = inferStack;
        return cfun;
    }
    
    string Function::getLocationExtra() const {
        if ( !inferStack.size() ) {
            return "";
        }
        stringstream ss;
        ss << "\nwhile compiling " << describe() << "\n";
        for ( const auto & ih : inferStack ) {
            ss << "instanced from " << ih.func->describe() << " at " << ih.at.describe() << "\n";
        }
        return ss.str();
    }
    
    string Function::describe() const {
        stringstream ss;
        if ( !isalpha(name[0]) && name[0]!='_' ) {
            ss << "operator ";
        }
        ss << name;
        if ( arguments.size() ) {
            ss << " ( ";
            for ( auto & arg : arguments ) {
                ss << arg->name << " : " << *arg->type;
                if ( arg != arguments.back() ) ss << "; ";
            }
            ss << " )";
        }
        ss << " : " << result->describe();
        return ss.str();
    }
    
    string Function::getMangledName() const {
        stringstream ss;
        ss << name;
        for ( auto & arg : arguments ) {
            ss << " " << arg->type->getMangledName();
        }
        // ss << "->" << result->getMangledName();
        return ss.str();
    }
    
    VariablePtr Function::findArgument(const string & name) {
        for ( auto & arg : arguments ) {
            if ( arg->name==name ) {
                return arg;
            }
        }
        return nullptr;
    }
    
    SimNode * Function::simulate (Context & context) const {
        if ( builtIn ) {
            assert(0 && "can only simulate non built-in function");
            return nullptr;
        }
        return body->simulate(context);
    }
    
    FunctionPtr Function::visit(Visitor & vis) {
        vis.preVisit(this);
        for ( auto & arg : arguments ) {
            vis.preVisitArgument(this, arg, arg==arguments.back() );
            if ( arg->init ) {
                vis.preVisitArgumentInit(this, arg, arg->init.get());
                arg->init = arg->init->visit(vis);
                if ( arg->init ) {
                    arg->init = vis.visitArgumentInit(this, arg, arg->init.get());
                }
            }
            arg = vis.visitArgument(this, arg, arg==arguments.back() );
        }
        vis.preVisitFunctionBody(this, body.get());
        body = body->visit(vis);
        body = vis.visitFunctionBody(this, body.get());
        return vis.visit(this);
    }
    
    bool Function::isGeneric() const {
        for ( auto & arg : arguments ) {
            if ( arg->type->isAuto() ) {
                return true;
            }
        }
        return false;
    }
    
    // built-in function
    
    BuiltInFunction::BuiltInFunction ( const string & fn ) {
        builtIn = true;
        name = fn;
    }
    
    // type annotation
    
    void debugType ( TypeAnnotation * ta, stringstream & ss , void * data, PrintFlags flags ) {
        ta->debug(ss, data);
    }
    
    // expression
    
    ExpressionPtr Expression::clone( const ExpressionPtr & expr ) const {
        if ( !expr ) {
            assert(0 && "unsupported expression");
            return nullptr;
        }
        expr->at = at;
        expr->type = type ? make_shared<TypeDecl>(*type) : nullptr;
        return expr;
    }
    
    ExpressionPtr Expression::autoDereference ( const ExpressionPtr & expr ) {
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
    
    // ExprRef2Value
    
    ExpressionPtr ExprRef2Value::visit(Visitor & vis) {
        vis.preVisit(this);
        subexpr = subexpr->visit(vis);
        return vis.visit(this);
    }
    
    ExpressionPtr ExprRef2Value::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprRef2Value>(expr);
        Expression::clone(cexpr);
        cexpr->subexpr = subexpr->clone();
        return cexpr;
    }
    
    SimNode * ExprRef2Value::GetR2V ( Context & context, const LineInfo & at, const TypeDeclPtr & type, SimNode * expr ) {
        if ( type->isHandle() ) {
            return type->annotation->simulateRef2Value(context, at, expr);
        } else {
            return context.makeValueNode<SimNode_Ref2Value>(type->baseType, at, expr);
        }
    }
    
    SimNode * ExprRef2Value::simulate (Context & context) const {
        return GetR2V(context, at, type, subexpr->simulate(context));
    }
    
    // ExprPtr2Ref
    
    ExpressionPtr ExprPtr2Ref::visit(Visitor & vis) {
        vis.preVisit(this);
        subexpr = subexpr->visit(vis);
        return vis.visit(this);
    }
    
    ExpressionPtr ExprPtr2Ref::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprPtr2Ref>(expr);
        Expression::clone(cexpr);
        cexpr->subexpr = subexpr->clone();
        return cexpr;
    }
    
    SimNode * ExprPtr2Ref::simulate (Context & context) const {
        return context.makeNode<SimNode_Ptr2Ref>(at,subexpr->simulate(context));
    }

    // ExprNullCoalescing
    
    ExpressionPtr ExprNullCoalescing::visit(Visitor & vis) {
        vis.preVisit(this);
        subexpr = subexpr->visit(vis);
        vis.preVisitNullCoaelescingDefault(this, defaultValue.get());
        defaultValue = defaultValue->visit(vis);
        return vis.visit(this);
    }
    
    ExpressionPtr ExprNullCoalescing::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprNullCoalescing>(expr);
        ExprPtr2Ref::clone(cexpr);
        cexpr->defaultValue = defaultValue->clone();
        return cexpr;
    }
    
    SimNode * ExprNullCoalescing::simulate (Context & context) const {
        if ( type->ref ) {
            return context.makeNode<SimNode_NullCoalescingRef>(at,subexpr->simulate(context),defaultValue->simulate(context));
        } else {
            return context.makeValueNode<SimNode_NullCoalescing>(type->baseType,at,subexpr->simulate(context),defaultValue->simulate(context));
        }
    }
    
    // Const
    
    SimNode * ExprConst::simulate (Context & context) const {
        return context.makeNode<SimNode_ConstValue>(at,value);
    }
    
    ExpressionPtr ExprConstString::visit(Visitor & vis) {
        vis.preVisit((ExprConst *)this);
        vis.preVisit(this);
        auto res = vis.visit(this);
        if ( res.get() != this )
            return res;
        return vis.visit((ExprConst *)this);
    }
    
    ExpressionPtr ExprConstString::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprConstString>(expr);
        Expression::clone(cexpr);
        cexpr->value = value;
        cexpr->text = text;
        return cexpr;
    }
    
    SimNode * ExprConstString::simulate (Context & context) const {
        char * str = context.allocateName(text);
        return context.makeNode<SimNode_ConstValue>(at,cast<char *>::from(str));
    }

    // ExprStaticAssert
    
    ExpressionPtr ExprStaticAssert::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprStaticAssert>(expr);
        ExprLooksLikeCall::clone(cexpr);
        return cexpr;
    }
    
    SimNode * ExprStaticAssert::simulate (Context & context) const {
        return nullptr;
    }
    
    // ExprAssert
    
    ExpressionPtr ExprAssert::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprAssert>(expr);
        ExprLooksLikeCall::clone(cexpr);
        return cexpr;
    }
    
    SimNode * ExprAssert::simulate (Context & context) const {
        string message;
        if ( arguments.size()==2 && arguments[1]->rtti_isStringConstant() )
            message = static_pointer_cast<ExprConstString>(arguments[1])->getValue();
        return context.makeNode<SimNode_Assert>(at,arguments[0]->simulate(context),context.allocateName(message));
    }
    
    // ExprDebug
    
    ExpressionPtr ExprDebug::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprDebug>(expr);
        ExprLooksLikeCall::clone(cexpr);
        return cexpr;
    }
    
    SimNode * ExprDebug::simulate (Context & context) const {
        TypeInfo * pTypeInfo = context.makeNode<TypeInfo>();
        context.thisProgram->makeTypeInfo(pTypeInfo, context, arguments[0]->type);
        string message;
        if ( arguments.size()==2 && arguments[1]->rtti_isStringConstant() )
            message = static_pointer_cast<ExprConstString>(arguments[1])->getValue();
        return context.makeNode<SimNode_Debug>(at,
                                               arguments[0]->simulate(context),
                                               pTypeInfo,
                                               context.allocateName(message));
    }

    // ExprMakeBlock
    
    ExpressionPtr ExprMakeBlock::visit(Visitor & vis) {
        vis.preVisit(this);
        block = block->visit(vis);
        return vis.visit(this);
    }
    
    SimNode * ExprMakeBlock::simulate (Context & context) const {
        uint32_t argSp = static_pointer_cast<ExprBlock>(block)->stackTop;
        return context.makeNode<SimNode_MakeBlock>(at,block->simulate(context),argSp);
    }
    
    ExpressionPtr ExprMakeBlock::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprMakeBlock>(expr);
        cexpr->block = block->clone();
        return cexpr;
    }
    
    // ExprInvoke
    
    ExpressionPtr ExprInvoke::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprInvoke>(expr);
        ExprLooksLikeCall::clone(cexpr);
        return cexpr;
    }
    
    SimNode * ExprInvoke::simulate (Context & context) const {
        
        SimNode_Invoke * pInvoke = context.makeNode<SimNode_Invoke>(at);
        pInvoke->debug = at;
        if ( int nArg = (int) arguments.size() ) {
            pInvoke->arguments = (SimNode **) context.allocate(nArg * sizeof(SimNode *));
            pInvoke->nArguments = nArg;
            for ( int a=0; a!=nArg; ++a ) {
                pInvoke->arguments[a] = arguments[a]->simulate(context);
            }
        } else {
            pInvoke->arguments = nullptr;
            pInvoke->nArguments = 0;
        }
        return pInvoke;
    }
    
    // ExprHash

    ExpressionPtr ExprHash::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprHash>(expr);
        ExprLooksLikeCall::clone(cexpr);
        return cexpr;
    }
    
    SimNode * ExprHash::simulate (Context & context) const {
        auto val = arguments[0]->simulate(context);
        if ( !arguments[0]->type->isRef() ) {
            return context.makeValueNode<SimNode_HashOfValue>(arguments[0]->type->baseType, at, val);
        } else if ( arguments[0]->type->isPod() ) {
            return context.makeNode<SimNode_HashOfRef>(at, val, arguments[0]->type->getSizeOf());
        } else {
            auto typeInfo = context.makeNode<TypeInfo>();
            context.thisProgram->makeTypeInfo(typeInfo, context, arguments[0]->type);
            return context.makeNode<SimNode_HashOfMixedType>(at, val, typeInfo);
        }
    }

    // ExprArrayPush
    
    ExpressionPtr ExprArrayPush::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprArrayPush>(expr);
        ExprLooksLikeCall::clone(cexpr);
        return cexpr;
    }
    
    SimNode * ExprArrayPush::simulate (Context & context) const {
        auto arr = arguments[0]->simulate(context);
        auto val = arguments[1]->simulate(context);
        auto idx = arguments.size()==3 ? arguments[2]->simulate(context) : nullptr;
        if ( arguments[1]->type->isRef() ) {
            return context.makeNode<SimNode_ArrayPushRefValue>(at, arr, val, idx, arguments[0]->type->firstType->getSizeOf());
        } else {
            return context.makeValueNode<SimNode_ArrayPushValue>(arguments[1]->type->baseType, at, arr, val, idx);
        }
    }
    
    // ExprErase
    
    ExpressionPtr ExprErase::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprErase>(expr);
        ExprLooksLikeCall::clone(cexpr);
        return cexpr;
    }
    
    SimNode * ExprErase::simulate (Context & context) const {
        auto cont = arguments[0]->simulate(context);
        auto val = arguments[1]->simulate(context);
        if ( arguments[0]->type->isGoodArrayType() ) {
            auto size = arguments[0]->type->firstType->getSizeOf();
            return context.makeNode<SimNode_ArrayErase>(at,cont,val,size);
        } else if ( arguments[0]->type->isGoodTableType() ) {
            uint32_t valueTypeSize = arguments[0]->type->secondType->getSizeOf();
            return context.makeValueNode<SimNode_TableErase>(arguments[0]->type->firstType->baseType, at, cont, val, valueTypeSize);
        } else {
            assert(0 && "we should not even be here");
            return nullptr;
        }
    }
    
    // ExprFind
    
    ExpressionPtr ExprFind::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprFind>(expr);
        ExprLooksLikeCall::clone(cexpr);
        return cexpr;
    }
    
    SimNode * ExprFind::simulate (Context & context) const {
        auto cont = arguments[0]->simulate(context);
        auto val = arguments[1]->simulate(context);
        if ( arguments[0]->type->isGoodArrayType() ) {
            assert(0);
            return nullptr;
            // auto size = arguments[0]->type->firstType->getSizeOf();
            // return context.makeNode<SimNode_ArrayErase>(at,cont,val,size);
        } else if ( arguments[0]->type->isGoodTableType() ) {
            uint32_t valueTypeSize = arguments[0]->type->secondType->getSizeOf();
            return context.makeValueNode<SimNode_TableFind>(arguments[0]->type->firstType->baseType, at, cont, val, valueTypeSize);
        } else {
            assert(0 && "we should not even be here");
            return nullptr;
        }
    }

    // ExprSizeOf
    
    ExpressionPtr ExprSizeOf::visit(Visitor & vis) {
        vis.preVisit(this);
        if ( subexpr ) {
            subexpr = subexpr->visit(vis);
        }
        return vis.visit(this);
    }
    
    ExpressionPtr ExprSizeOf::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprSizeOf>(expr);
        Expression::clone(cexpr);
        if ( subexpr )
            cexpr->subexpr = subexpr->clone();
        if ( typeexpr )
            cexpr->typeexpr = typeexpr;
        return cexpr;
    }
    
    SimNode * ExprSizeOf::simulate (Context & context) const {
        uint32_t size = typeexpr->getSizeOf();
        return context.makeNode<SimNode_ConstValue>(at,cast<uint32_t>::from(size));
    }
    
    // ExprTypeName
    
    ExpressionPtr ExprTypeName::visit(Visitor & vis) {
        vis.preVisit(this);
        if ( subexpr ) {
            subexpr = subexpr->visit(vis);
        }
        return vis.visit(this);
    }
    
    ExpressionPtr ExprTypeName::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprTypeName>(expr);
        Expression::clone(cexpr);
        if ( subexpr )
            cexpr->subexpr = subexpr->clone();
        if ( typeexpr )
            cexpr->typeexpr = typeexpr;
        return cexpr;
    }
    
    SimNode * ExprTypeName::simulate (Context & context) const {
        auto pName = context.allocateName(typeexpr->describe(false));
        return context.makeNode<SimNode_ConstValue>(at,cast<char *>::from(pName));
    }
    
    // ExprNew
    
    ExpressionPtr ExprNew::visit(Visitor & vis) {
        vis.preVisit(this);
        return vis.visit(this);
    }
    
    ExpressionPtr ExprNew::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprNew>(expr);
        Expression::clone(cexpr);
        cexpr->typeexpr = typeexpr;
        return cexpr;
    }
    
    SimNode * ExprNew::simulate (Context & context) const {
        if ( typeexpr->isHandle() ) {
            return typeexpr->annotation->simulateGetNew(context, at);
        } else {
            int32_t bytes = typeexpr->getSizeOf();
            return context.makeNode<SimNode_New>(at,bytes);
        }
    }

    // ExprAt
    
    ExpressionPtr ExprAt::visit(Visitor & vis) {
        vis.preVisit(this);
        subexpr = subexpr->visit(vis);
        vis.preVisitAtIndex(this, index.get());
        index = index->visit(vis);
        return vis.visit(this);
    }
    
    ExpressionPtr ExprAt::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprAt>(expr);
        Expression::clone(cexpr);
        cexpr->subexpr = subexpr->clone();
        cexpr->index = index->clone();
        return cexpr;
    }

    SimNode * ExprAt::simulate (Context & context) const {
        auto prv = subexpr->simulate(context);
        auto pidx = index->simulate(context);
        SimNode * result;
        if ( subexpr->type->isGoodTableType() ) {
            uint32_t valueTypeSize = subexpr->type->secondType->getSizeOf();
            result = context.makeValueNode<SimNode_TableIndex>(subexpr->type->firstType->baseType, at, prv, pidx, valueTypeSize);
        } else if ( subexpr->type->isGoodArrayType() ) {
            uint32_t stride = subexpr->type->firstType->getSizeOf();
            result = context.makeNode<SimNode_ArrayAt>(at, prv, pidx, stride);
        } else if ( subexpr->type->isHandle() ) {
            result = subexpr->type->annotation->simulateGetAt(context, at, prv, pidx);
        } else {
            uint32_t stride = subexpr->type->getStride();
            uint32_t range = subexpr->type->dim.back();
            result = context.makeNode<SimNode_At>(at, prv, pidx, stride, range);
        }
        if ( r2v ) {
            return ExprRef2Value::GetR2V(context, at, type, result);
        } else {
            return result;
        }
    }

    // ExprBlock
    
    ExpressionPtr ExprBlock::visit(Visitor & vis) {
        vis.preVisit(this);
        for ( auto it = arguments.begin(); it != arguments.end(); ) {
            auto & arg = *it;
            vis.preVisitBlockArgument(this, arg, arg==arguments.back());
            if ( arg->init ) {
                vis.preVisitBlockArgumentInit(this, arg, arg->init.get());
                arg->init = arg->init->visit(vis);
                if ( arg->init ) {
                    arg->init = vis.visitBlockArgumentInit(this, arg, arg->init.get());
                }
            }
            arg = vis.visitBlockArgument(this, arg, arg==arguments.back());
            if ( arg ) ++it; else it = arguments.erase(it);
        }
        for ( auto it = list.begin(); it!=list.end(); ) {
            auto & subexpr = *it;
            vis.preVisitBlockExpression(this, subexpr.get());
            subexpr = subexpr->visit(vis);
            if ( subexpr )
                subexpr = vis.visitBlockExpression(this, subexpr.get());
            if ( subexpr ) ++it; else it = list.erase(it);
        }
        return vis.visit(this);
    }
    
    ExpressionPtr ExprBlock::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprBlock>(expr);
        Expression::clone(cexpr);
        for ( auto & subexpr : list ) {
            cexpr->list.push_back(subexpr->clone());
        }
        cexpr->isClosure = isClosure;
        if ( returnType )
            cexpr->returnType = make_shared<TypeDecl>(*returnType);
        for ( auto & arg : arguments ) {
            cexpr->arguments.push_back(arg->clone());
        }
        return cexpr;
    }
    
    uint32_t ExprBlock::getEvalFlags() const {
        uint32_t flags = 0;
        for ( const auto & ex : list ) {
            flags |= ex->getEvalFlags();
        }
        return flags;
    }
    
    SimNode * ExprBlock::simulate (Context & context) const {
        vector<SimNode *> simlist;
        for ( auto & node : list ) {
			if ( node->rtti_isLet()) {
				shared_ptr<ExprLet> pLet = static_pointer_cast<ExprLet>(node);
				if (!pLet->subexpr) {
					auto letInit = ExprLet::simulateInit(context, pLet.get());
					simlist.insert(simlist.end(), letInit.begin(), letInit.end());
					continue;
				}
			}
            if ( auto simE = node->simulate(context) ) {
                simlist.push_back(simE);
            }
        }
        // TODO: what if list size is 0?
        if ( simlist.size()!=1 || isClosure ) {
            auto block = isClosure ? context.makeNode<SimNode_ClosureBlock>(at, type!=nullptr && type->baseType!=Type::tVoid, annotationData)
                : context.makeNode<SimNode_Block>(at);
            block->total = int(simlist.size());
            block->list = (SimNode **) context.allocate(sizeof(SimNode *)*block->total);
            for ( int i = 0; i != block->total; ++i )
                block->list[i] = simlist[i];
            return block;
        } else {
            return simlist[0];
        }
    }
    
    VariablePtr ExprBlock::findArgument(const string & name) {
        for ( auto & arg : arguments ) {
            if ( arg->name==name ) {
                return arg;
            }
        }
        return nullptr;
    }
    
    // ExprField
    
    ExpressionPtr ExprField::visit(Visitor & vis) {
        vis.preVisit(this);
        value = value->visit(vis);
        return vis.visit(this);
    }
    
    ExpressionPtr ExprField::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprField>(expr);
        Expression::clone(cexpr);
        cexpr->name = name;
        cexpr->value = value->clone();
        cexpr->field = field;
        return cexpr;
    }
    
    SimNode * ExprField::simulate (Context & context) const {
        if ( !field ) {
            auto result = annotation->simulateGetField(name, context, at, value->simulate(context));
            if ( r2v ) {
                result = ExprRef2Value::GetR2V(context, at, type, result);
            }
            return result;
        } else {
			if (type->isPointer()) {
				if (r2v)
					return context.makeValueNode<SimNode_PtrFieldDerefR2V>(type->baseType, at, value->simulate(context), field->offset);
				else
					return context.makeNode<SimNode_PtrFieldDeref>(at, value->simulate(context), field->offset);
			} else {
				if (r2v)
					return context.makeValueNode<SimNode_FieldDerefR2V>(type->baseType, at, value->simulate(context), field->offset);
				else
					return context.makeNode<SimNode_FieldDeref>(at, value->simulate(context), field->offset);
			}
        }
    }
    
    // ExprSafeField
    
    ExpressionPtr ExprSafeField::visit(Visitor & vis) {
        vis.preVisit(this);
        value = value->visit(vis);
        return vis.visit(this);
    }
    
    ExpressionPtr ExprSafeField::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprSafeField>(expr);
        ExprField::clone(cexpr);
        return cexpr;
    }
    
    SimNode * ExprSafeField::simulate (Context & context) const {
        if ( skipQQ ) {
            if ( annotation ) {
                return annotation->simulateSafeGetFieldPtr(name, context, at, value->simulate(context));
            } else {
                return context.makeNode<SimNode_SafeFieldDerefPtr>(at,value->simulate(context),field->offset);
            }
        } else {
            if ( annotation ) {
                return annotation->simulateSafeGetField(name, context, at, value->simulate(context));
            } else {
                return context.makeNode<SimNode_SafeFieldDeref>(at,value->simulate(context),field->offset);
            }
        }
    }
    
    // ExprStringBuilder
    
    ExpressionPtr ExprStringBuilder::visit(Visitor & vis) {
        vis.preVisit(this);
        for ( auto it = elements.begin(); it!=elements.end(); ) {
            auto & elem = *it;
            vis.preVisitStringBuilderElement(this, elem.get(), elem==elements.back());
            elem = elem->visit(vis);
            if ( elem ) elem = vis.visitStringBuilderElement(this, elem.get(), elem==elements.back());
            if ( elem ) ++ it; else it = elements.erase(it);
        }
        return vis.visit(this);
    }
    
    ExpressionPtr ExprStringBuilder::clone( const ExpressionPtr & expr  ) const {
        auto cexpr = clonePtr<ExprStringBuilder>(expr);
        Expression::clone(cexpr);
        cexpr->elements.reserve(elements.size());
        for ( auto & elem : elements ) {
            cexpr->elements.push_back(elem->clone());
        }
        return cexpr;
    }
    
    SimNode * ExprStringBuilder::simulate (Context & context) const {
        SimNode_StringBuilder * pSB = context.makeNode<SimNode_StringBuilder>(at);
        if ( int nArg = (int) elements.size() ) {
            pSB->arguments = (SimNode **) context.allocate(nArg * sizeof(SimNode *));
            pSB->types = (TypeInfo **) context.allocate(nArg * sizeof(TypeInfo *));
            pSB->nArguments = nArg;
            for ( int a=0; a!=nArg; ++a ) {
                pSB->arguments[a] = elements[a]->simulate(context);
                pSB->types[a] = context.makeNode<TypeInfo>();
                context.thisProgram->makeTypeInfo(pSB->types[a], context, elements[a]->type);
            }
        } else {
            pSB->arguments = nullptr;
            pSB->types = nullptr;
            pSB->nArguments = 0;
        }
        return pSB;
    }
    
    // ExprVar
    
    ExpressionPtr ExprVar::visit(Visitor & vis) {
        vis.preVisit(this);
        return vis.visit(this);
    }
    
    ExpressionPtr ExprVar::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprVar>(expr);
        Expression::clone(cexpr);
        cexpr->name = name;
        cexpr->variable = variable; // todo: lookup again?
        cexpr->local = local;
        cexpr->block = block;
        cexpr->pBlock = pBlock;
        cexpr->argument = argument;
        cexpr->argumentIndex = argumentIndex;
        return cexpr;
    }
    
    SimNode * ExprVar::simulate (Context & context) const {
        if ( block ) {
            auto blk = pBlock.lock();
            
            if (variable->type->isRef()) {
                if (r2v) {
                    return context.makeValueNode<SimNode_GetBlockArgumentR2V>(type->baseType, at, argumentIndex, blk->stackTop);
                } else {
                    return context.makeNode<SimNode_GetBlockArgument>(at, argumentIndex, blk->stackTop);
                }
            } else {
                if (r2v) {
                    return context.makeNode<SimNode_GetBlockArgument>(at, argumentIndex, blk->stackTop);
                }
                else {
                    return context.makeNode<SimNode_GetBlockArgumentRef>(at, argumentIndex, blk->stackTop);
                }
            }
        } else if ( local ) {
            if ( variable->type->ref ) {
                if ( r2v ) {
                    return context.makeValueNode<SimNode_GetLocalRefR2V>(type->baseType, at, variable->stackTop);
                } else {
                    return context.makeNode<SimNode_GetLocalRef>(at, variable->stackTop);
                }
            } else {
                if ( r2v ) {
                    return context.makeValueNode<SimNode_GetLocalR2V>(type->baseType, at, variable->stackTop);
                } else {
                    return context.makeNode<SimNode_GetLocal>(at, variable->stackTop);
                }
            }
        } else if ( argument) {
			if (variable->type->isRef()) {
				if (r2v) {
					return context.makeValueNode<SimNode_GetArgumentR2V>(type->baseType, at, argumentIndex);
				} else {
					return context.makeNode<SimNode_GetArgument>(at, argumentIndex);
				}
			} else {
				if (r2v) {
					return context.makeNode<SimNode_GetArgument>(at, argumentIndex);
				}
				else {
					return context.makeNode<SimNode_GetArgumentRef>(at, argumentIndex);
				}
            }
        } else {
            if ( r2v ) {
                return context.makeValueNode<SimNode_GetGlobalR2V>(type->baseType, at, variable->index);
            } else {
                return context.makeNode<SimNode_GetGlobal>(at, variable->index);
            }
        }
    }

    // ExprOp
    
    ExpressionPtr ExprOp::clone( const ExpressionPtr & expr ) const {
        if ( !expr ) {
            assert(0 && "can't clone ExprOp");
            return nullptr;
        }
        auto cexpr = static_pointer_cast<ExprOp>(expr);
        cexpr->op = op;
        cexpr->func = func;
        cexpr->at = at;
        return cexpr;
    }
    
    // ExprOp1
    
    ExpressionPtr ExprOp1::visit(Visitor & vis) {
        vis.preVisit(this);
        subexpr = subexpr->visit(vis);
        return vis.visit(this);
    }
    
    ExpressionPtr ExprOp1::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprOp1>(expr);
        ExprOp::clone(cexpr);
        cexpr->subexpr = subexpr->clone();
        return cexpr;
    }
    
    SimNode * ExprOp1::simulate (Context & context) const {
        if ( func->builtIn ) {
            auto pSimOp1 = static_cast<SimNode_Op1 *>(func->makeSimNode(context));
            pSimOp1->x = subexpr->simulate(context);
            return pSimOp1;
        } else {
            SimNode_Call * pCall = static_cast<SimNode_Call *>(func->makeSimNode(context));
            pCall->debug = at;
            pCall->fnIndex = func->index;
            pCall->arguments = (SimNode **) context.allocate(1 * sizeof(SimNode *));
            pCall->nArguments = 1;
            pCall->arguments[0] = subexpr->simulate(context);
            return pCall;
        }
    }
    
    // ExprOp2
    
    ExpressionPtr ExprOp2::visit(Visitor & vis) {
        vis.preVisit(this);
        left = left->visit(vis);
        vis.preVisitRight(this, right.get());
        right = right->visit(vis);
        return vis.visit(this);
    }
    
    ExpressionPtr ExprOp2::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprOp2>(expr);
        ExprOp::clone(cexpr);
        cexpr->left = left->clone();
        cexpr->right = right->clone();
        return cexpr;
    }
    
    SimNode * ExprOp2::simulate (Context & context) const {
        if ( func->builtIn ) {
            auto pSimOp2 = static_cast<SimNode_Op2 *>(func->makeSimNode(context));
            pSimOp2->l = left->simulate(context);
            pSimOp2->r = right->simulate(context);
            return pSimOp2;
        } else {
            SimNode_Call * pCall = static_cast<SimNode_Call *>(func->makeSimNode(context));
            pCall->debug = at;
            pCall->fnIndex = func->index;
            pCall->arguments = (SimNode **) context.allocate(2 * sizeof(SimNode *));
            pCall->nArguments = 2;
            pCall->arguments[0] = left->simulate(context);
            pCall->arguments[1] = right->simulate(context);
            return pCall;
        }
    }

    // ExprOp3
    
    ExpressionPtr ExprOp3::visit(Visitor & vis) {
        vis.preVisit(this);
        subexpr = subexpr->visit(vis);
        vis.preVisitLeft(this, left.get());
        left = left->visit(vis);
        vis.preVisitRight(this, right.get());
        right = right->visit(vis);
        return vis.visit(this);
    }
    
    ExpressionPtr ExprOp3::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprOp3>(expr);
        ExprOp::clone(cexpr);
        cexpr->subexpr = subexpr->clone();
        cexpr->left = left->clone();
        cexpr->right = right->clone();
        return cexpr;
    }
    
    SimNode * ExprOp3::simulate (Context & context) const {
        return context.makeNode<SimNode_IfThenElse>(at,
                                                    subexpr->simulate(context),
                                                    left->simulate(context),
                                                    right->simulate(context));
    }
    
    // common for move and copy
    
    SimNode * makeCopy(const LineInfo & at, Context & context, const ExpressionPtr & lE, const ExpressionPtr & rE ) {
        const auto & rightType = *rE->type;
        assert ( rightType.canCopy() && "should check above" );
        // TODO:
        //  expand to (while managing combinatorics explosion, policy?)
        //      1. local ref variables (iterator results etc)
        //      2. global variables
        //      3. arguments
        //      4. blocks
        //      5. common cross-copy scenarios
        if ( rightType.isWorkhorseType() && lE->rtti_isVar() ) {
            auto var = static_pointer_cast<ExprVar>(lE);
            if ( var->local && !var->variable->type->ref ) {
                if ( rE->rtti_isVar() ) {
                    auto rvar = static_pointer_cast<ExprVar>(rE);
                    if ( rvar->local && !rvar->variable->type->ref ) {
                        return context.makeValueNode<SimNode_CopyLocal2LocalT>(rightType.baseType,
                                at, var->variable->stackTop, rvar->variable->stackTop);
                    }
                }
                auto right = rE->simulate(context);
                if ( rightType.ref ) {
                    return context.makeValueNode<SimNode_SetLocalRefT>(rightType.baseType,
                                at, right, var->variable->stackTop);
                } else {
                    return context.makeValueNode<SimNode_SetLocalValueT>(rightType.baseType,
                                at, right, var->variable->stackTop);
                }
            }
        }
        // now, to the regular copy
        auto left = lE->simulate(context);
        auto right = rE->simulate(context);
        if ( rightType.isRef() ) {
            if ( rightType.isWorkhorseType() ) {
                return context.makeValueNode<SimNode_CopyRefValueT>(rightType.baseType, at, left, right);
            } else {
                return context.makeNode<SimNode_CopyRefValue>(at, left, right, rightType.getSizeOf());
            }
        } else if ( rightType.isHandle() ) {
            return rightType.annotation->simulateCopy(context, at, left, right);
        } else {
            return context.makeValueNode<SimNode_CopyValue>(rightType.baseType, at, left, right);
        }
    }
    
    SimNode * makeMove (const LineInfo & at, Context & context, const TypeDecl & rightType, SimNode * left, SimNode * right ) {
        assert ( !rightType.canCopy() && "should check above" );
        if ( rightType.ref ) {
            return context.makeNode<SimNode_MoveRefValue>(at, left, right, rightType.getSizeOf());
        } else if ( rightType.isGoodArrayType() ) {
            return context.makeNode<SimNode_CopyValue<Array>>(at, left, right);
        } else {
            assert(0 && "we should not be here");
            return nullptr;
        }
    }
    
    // ExprMove
    
    ExpressionPtr ExprMove::visit(Visitor & vis) {
        vis.preVisit(this);
        left = left->visit(vis);
        vis.preVisitRight(this, right.get());
        right = right->visit(vis);
        return vis.visit(this);
    }
    
    ExpressionPtr ExprMove::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprMove>(expr);
        ExprOp2::clone(cexpr);
        return cexpr;
    }
    
    SimNode * ExprMove::simulate (Context & context) const {
        return makeMove(at,
                        context,
                        *right->type,
                        left->simulate(context),
                        right->simulate(context));
    }
    
    // ExprCopy
    
    ExpressionPtr ExprCopy::visit(Visitor & vis) {
        vis.preVisit(this);
        left = left->visit(vis);
        vis.preVisitRight(this, right.get());
        right = right->visit(vis);
        return vis.visit(this);
    }
    
    ExpressionPtr ExprCopy::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprCopy>(expr);
        ExprOp2::clone(cexpr);
        return cexpr;
    }
    
    SimNode * ExprCopy::simulate (Context & context) const {
        return makeCopy(at, context, left, right);
    }
    
    // ExprTryCatch
    
    ExpressionPtr ExprTryCatch::visit(Visitor & vis) {
        vis.preVisit(this);
        try_block = try_block->visit(vis);
        vis.preVisitCatch(this,catch_block.get());
        catch_block = catch_block->visit(vis);
        return vis.visit(this);
    }
    
    uint32_t ExprTryCatch::getEvalFlags() const {
        return (try_block->getEvalFlags() | catch_block->getEvalFlags()) & ~EvalFlags::stopForThrow;
    }

    SimNode * ExprTryCatch::simulate (Context & context) const {
        return context.makeNode<SimNode_TryCatch>(at,
                                                  try_block->simulate(context),
                                                  catch_block->simulate(context));
    }
    
    ExpressionPtr ExprTryCatch::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprTryCatch>(expr);
        Expression::clone(cexpr);
        cexpr->try_block = try_block->clone();
        cexpr->catch_block = catch_block->clone();
        return cexpr;
    }
    
    // ExprReturn
    
    ExpressionPtr ExprReturn::visit(Visitor & vis) {
        vis.preVisit(this);
        if ( subexpr ) {
            subexpr = subexpr->visit(vis);
        }
        return vis.visit(this);
    }
    
    ExpressionPtr ExprReturn::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprReturn>(expr);
        Expression::clone(cexpr);
        if ( subexpr )
            cexpr->subexpr = subexpr->clone();
        return cexpr;
    }

    SimNode * ExprReturn::simulate (Context & context) const {
        return context.makeNode<SimNode_Return>(at, subexpr ? subexpr->simulate(context) : nullptr);
    }
    
    // ExprBreak
    
    ExpressionPtr ExprBreak::visit(Visitor & vis) {
        vis.preVisit(this);
        return vis.visit(this);
    }
    
    ExpressionPtr ExprBreak::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprBreak>(expr);
        Expression::clone(cexpr);
        return cexpr;
    }
    
    SimNode * ExprBreak::simulate (Context & context) const {
        return context.makeNode<SimNode_Break>(at);
    }

    // ExprIfThenElse
    
    ExpressionPtr ExprIfThenElse::visit(Visitor & vis) {
        vis.preVisit(this);
        cond = cond->visit(vis);
        vis.preVisitIfBlock(this, if_true.get());
        if_true = if_true->visit(vis);
        if ( if_false ) {
            vis.preVisitElseBlock(this, if_false.get());
            if_false = if_false->visit(vis);
        }
        return vis.visit(this);
    }
    
    ExpressionPtr ExprIfThenElse::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprIfThenElse>(expr);
        Expression::clone(cexpr);
        cexpr->cond = cond->clone();
        cexpr->if_true = if_true->clone();
        if ( if_false )
            cexpr->if_false = if_false->clone();
        return cexpr;
    }
    
    SimNode * ExprIfThenElse::simulate (Context & context) const {
        return context.makeNode<SimNode_IfThenElse>(at, cond->simulate(context), if_true->simulate(context),
                                                    if_false ? if_false->simulate(context) : nullptr);
    }

    // ExprWhile
    
    ExpressionPtr ExprWhile::visit(Visitor & vis) {
        vis.preVisit(this);
        cond = cond->visit(vis);
        vis.preVisitWhileBody(this, body.get());
        body = body->visit(vis);
        return vis.visit(this);
    }
    
    ExpressionPtr ExprWhile::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprWhile>(expr);
        Expression::clone(cexpr);
        cexpr->cond = cond->clone();
        cexpr->body = body->clone();
        return cexpr;
    }
    
    uint32_t ExprWhile::getEvalFlags() const {
        return body->getEvalFlags() & ~EvalFlags::stopForBreak;
    }

    SimNode * ExprWhile::simulate (Context & context) const {
        return context.makeNode<SimNode_While>(at, cond->simulate(context),body->simulate(context));
    }
    
    // ExprFor

    ExpressionPtr ExprFor::visit(Visitor & vis) {
        vis.preVisit(this);
        for ( auto & var : iteratorVariables ) {
            vis.preVisitFor(this, var, var==iteratorVariables.back());
            var = vis.visitFor(this, var, var==iteratorVariables.back());
        }
        for ( auto & src : sources ) {
            vis.preVisitForSource(this, src.get(), src==sources.back());
            src = src->visit(vis);
            src = vis.visitForSource(this, src.get(), src==sources.back());
        }
        vis.preVisitForStack(this);
        vis.preVisitForBody(this, subexpr.get());
        subexpr = subexpr->visit(vis);
        return vis.visit(this);
    }
    
    ExpressionPtr ExprFor::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprFor>(expr);
        Expression::clone(cexpr);
        cexpr->iterators = iterators;
        for ( auto & src : sources )
            cexpr->sources.push_back(src->clone());
        for ( auto & var : iteratorVariables )
            cexpr->iteratorVariables.push_back(var->clone());
        cexpr->subexpr = subexpr->clone();
        return cexpr;
    }
    
    Variable * ExprFor::findIterator(const string & name) const {
        for ( auto & v : iteratorVariables ) {
            if ( v->name==name ) {
                return v.get();
            }
        }
        return nullptr;
    }
    
    uint32_t ExprFor::getEvalFlags() const {
        return subexpr->getEvalFlags() & ~EvalFlags::stopForBreak;
    }
    
    SimNode * ExprFor::simulate (Context & context) const {
        // determine iteration types
        bool nativeIterators = false;
        bool fixedArrays = false;
        bool dynamicArrays = false;
        bool rangeBase = false;
        uint32_t fixedSize = UINT16_MAX;
        for ( auto & src : sources ) {
            if ( !src->type ) continue;
            if ( src->type->isArray() ) {
                fixedSize = min(fixedSize, src->type->dim.back());
                fixedArrays = true;
            } else if ( src->type->isGoodArrayType() ) {
                dynamicArrays = true;
            } else if ( src->type->isGoodIteratorType() ) {
                nativeIterators = true;
            } else if ( src->type->isHandle() ) {
                nativeIterators = true;
            } else if ( src->type->isRange() ) {
                rangeBase = true;
            }
        }
        // create loops based on
        int  total = int(sources.size());
        int  sourceTypes = int(dynamicArrays) + int(fixedArrays) + int(rangeBase);
        bool hybridRange = rangeBase && (total>1);
        if ( (sourceTypes>1) || hybridRange || nativeIterators ) {
            SimNode_ForWithIteratorBase * result = (SimNode_ForWithIteratorBase *)
                context.makeNodeUnroll<SimNode_ForWithIterator>(total, at);
            for ( int t=0; t!=total; ++t ) {
                if ( sources[t]->type->isGoodIteratorType() ) {
                    result->source_iterators[t] = sources[t]->simulate(context);
                } else if ( sources[t]->type->isGoodArrayType() ) {
                    result->source_iterators[t] = context.makeNode<SimNode_GoodArrayIterator>(
                        sources[t]->at,
                        sources[t]->simulate(context),
                        sources[t]->type->firstType->getStride());
                } else if ( sources[t]->type->isRange() ) {
                    result->source_iterators[t] = context.makeNode<SimNode_RangeIterator>(
                        sources[t]->at,
                        sources[t]->simulate(context));
                } else if ( sources[t]->type->isHandle() ) {
                    result->source_iterators[t] = sources[t]->type->annotation->simulateGetIterator(
                         context,
                         sources[t]->at,
                         sources[t]->simulate(context)
                    );
                } else if ( sources[t]->type->dim.size() ) {
                    result->source_iterators[t] = context.makeNode<SimNode_FixedArrayIterator>(
                        sources[t]->at,
                        sources[t]->simulate(context),
                        sources[t]->type->dim.back(),
                        sources[t]->type->getStride());
                } else {
                    assert(0 && "we should not be here yet");
                    return nullptr;
                }
                result->stackTop[t] = iteratorVariables[t]->stackTop;
            }
            result->body = subexpr->simulate(context);
            return result;
        } else {
            SimNode_ForBase * result;
            if ( dynamicArrays ) {
                result = (SimNode_ForBase *) context.makeNodeUnroll<SimNode_ForGoodArray>(total, at);
            } else if ( fixedArrays ) {
                result = (SimNode_ForBase *) context.makeNodeUnroll<SimNode_ForFixedArray>(total, at);
            } else if ( rangeBase ) {
                assert(total==1 && "simple range on 1 loop only");
                result = context.makeNode<SimNode_ForRange>(at);
            } else {
                assert(0 && "we should not be here yet");
                return nullptr;
            }
            for ( int t=0; t!=total; ++t ) {
                result->sources[t] = sources[t]->simulate(context);
                if ( sources[t]->type->isGoodArrayType() ) {
                    result->strides[t] = sources[t]->type->firstType->getStride();
                } else {
                    result->strides[t] = sources[t]->type->getStride();
                }
                result->stackTop[t] = iteratorVariables[t]->stackTop;
            }
            result->size = fixedSize;
            result->body = subexpr->simulate(context);
            return result;
        }
    }
    
    // ExprLet
    
    ExpressionPtr ExprLet::visit(Visitor & vis) {
        vis.preVisit(this);
        for ( auto it = variables.begin(); it!=variables.end(); ) {
            auto & var = *it;
            vis.preVisitLet(this, var, var==variables.back());
            if ( var->init ) {
                vis.preVisitLetInit(this, var, var->init.get());
                var->init = var->init->visit(vis);
                var->init = vis.visitLetInit(this, var, var->init.get());
            }
            var = vis.visitLet(this, var, var==variables.back());
            if ( var ) ++it; else it = variables.erase(it);
        }
        vis.preVisitLetStack(this);
        if ( subexpr )
            subexpr = subexpr->visit(vis);
        return vis.visit(this);
    }
    
    ExpressionPtr ExprLet::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprLet>(expr);
        Expression::clone(cexpr);
        for ( auto & var : variables )
            cexpr->variables.push_back(var->clone());
        if ( subexpr )
            cexpr->subexpr = subexpr->clone();
        cexpr->scoped = scoped;
        return cexpr;
    }

    Variable * ExprLet::find(const string & name) const {
        for ( auto & v : variables ) {
            if ( v->name==name ) {
                return v.get();
            }
        }
        return nullptr;
    }
    
    uint32_t ExprLet::getEvalFlags() const {
        return subexpr ? subexpr->getEvalFlags() : 0;
    }
    
	vector<SimNode *> ExprLet::simulateInit(Context & context, const ExprLet * pLet) {
		vector<SimNode *> simlist;
		simlist.reserve(pLet->variables.size());
		for (auto & var : pLet->variables) {
			SimNode * init;
			if (var->init) {
				init = ExprLet::simulateInit(context, var, true);
			} else {
				init = context.makeNode<SimNode_InitLocal>(pLet->at, var->stackTop, var->type->getSizeOf());
			}
			if (init)
				simlist.push_back(init);
		}
		return simlist;
	}

    SimNode * ExprLet::simulateInit(Context & context, const VariablePtr & var, bool local) {
        SimNode * init = var->init->simulate(context);
        SimNode * get;
        if ( local )
            get = context.makeNode<SimNode_GetLocal>(var->init->at, var->stackTop);
        else
            get = context.makeNode<SimNode_GetGlobal>(var->init->at, var->index);
        if ( var->type->canCopy() ) {
            auto varExpr = make_shared<ExprVar>(var->at, var->name);
            varExpr->variable = var;
            varExpr->local = local;
            varExpr->type = make_shared<TypeDecl>(*var->type);
            return makeCopy(var->init->at, context, varExpr, var->init);
        }
        else if ( var->type->canMove() )
            return makeMove(var->init->at, context, *var->init->type, get, init);
        else {
            assert(0 && "we should not be here");
            return nullptr;
        }
    }
    
    SimNode * ExprLet::simulate (Context & context) const {
        auto let = context.makeNode<SimNode_Let>(at);
        let->total = (uint32_t) variables.size();
        let->list = (SimNode **) context.allocate(let->total * sizeof(SimNode*));
		auto simList = ExprLet::simulateInit(context, this);
		std::copy(simList.data(), simList.data() + simList.size(), let->list);
        let->subexpr = subexpr ? subexpr->simulate(context) : nullptr;
        return let;
    }
    
    // ExprLooksLikeCall
    
    ExpressionPtr ExprLooksLikeCall::visit(Visitor & vis) {
        vis.preVisit(this);
        for ( auto & arg : arguments ) {
            vis.preVisitLooksLikeCallArg(this, arg.get(), arg==arguments.back());
            arg = arg->visit(vis);
            arg = vis.visitLooksLikeCallArg(this, arg.get(), arg==arguments.back());
        }
        return vis.visit(this);
    }
    
    ExpressionPtr ExprLooksLikeCall::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprLooksLikeCall>(expr);
        Expression::clone(cexpr);
        cexpr->name = name;
        for ( auto & arg : arguments ) {
            cexpr->arguments.push_back(arg->clone());
        }
        return cexpr;
    }
    
    string ExprLooksLikeCall::describe() const {
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
    
    void ExprLooksLikeCall::autoDereference() {
        for ( size_t iA = 0; iA != arguments.size(); ++iA ) {
            arguments[iA] = Expression::autoDereference(arguments[iA]);
        }
    }
    
    // ExprCall
    
    ExpressionPtr ExprCall::visit(Visitor & vis) {
        vis.preVisit(this);
        for ( auto & arg : arguments ) {
            vis.preVisitCallArg(this, arg.get(), arg==arguments.back());
            arg = arg->visit(vis);
            arg = vis.visitCallArg(this, arg.get(), arg==arguments.back());
        }
        return vis.visit(this);
    }
    
    ExpressionPtr ExprCall::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprCall>(expr);
        ExprLooksLikeCall::clone(cexpr);
        cexpr->func = func;
        return cexpr;
    }
    
    SimNode * ExprCall::simulate (Context & context) const {
        SimNode_Call * pCall = static_cast<SimNode_Call *>(func->makeSimNode(context));
        pCall->debug = at;
        pCall->fnIndex = func->index;
        if ( int nArg = (int) arguments.size() ) {
            pCall->arguments = (SimNode **) context.allocate(nArg * sizeof(SimNode *));
            pCall->nArguments = nArg;
            for ( int a=0; a!=nArg; ++a ) {
                pCall->arguments[a] = arguments[a]->simulate(context);
            }
        } else {
            pCall->arguments = nullptr;
            pCall->nArguments = 0;
        }
        return pCall;
    }

    // program
    
    vector<AnnotationPtr> Program::findAnnotation ( const string & name ) const {
        return library.findAnnotation(name);
    }
    
    vector<StructurePtr> Program::findStructure ( const string & name ) const {
        return library.findStructure(name);
    }
    
    VariablePtr Program::findVariable ( const string & name ) const {
        return thisModule->findVariable(name);
    }
    
    FuncInfo * Program::makeFunctionDebugInfo ( Context & context, const Function & fn ) {
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
    
    StructInfo * Program::makeStructureDebugInfo ( Context & context, const Structure & st ) {
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
    
    void Program::makeTypeInfo ( TypeInfo * info, Context & context, const TypeDeclPtr & type ) {
        info->type = type->baseType;
        info->dimSize = (uint32_t) type->dim.size();
        info->annotation = type->annotation.get();
        if ( info->dimSize ) {
            info->dim = (uint32_t *) context.allocate(sizeof(uint32_t) * info->dimSize );
            for ( uint32_t i=0; i != info->dimSize; ++i ) {
                info->dim[i] = type->dim[i];
            }
        }
        if ( type->baseType==Type::tStructure  ) {
            auto st = sdebug.find(type->structType->name);
            if ( st==sdebug.end() ) {
                info->structType = makeStructureDebugInfo(context, *type->structType);
                sdebug[type->structType->name] = info->structType;
            } else {
                info->structType = st->second;
            }
        }
        info->ref = type->ref;
        if ( type->isRefType() )
            info->ref = false;
        info->canCopy = type->canCopy();
        info->isPod = type->isPod();
        if ( type->firstType ) {
            info->firstType = context.makeNode<TypeInfo>();
            makeTypeInfo(info->firstType, context, type->firstType);
        } else {
            info->firstType = nullptr;
        }
        if ( type->secondType ) {
            info->secondType = context.makeNode<TypeInfo>();
            makeTypeInfo(info->secondType , context, type->secondType);
        } else {
            info->secondType = nullptr;
        }
    }

    VarInfo * Program::makeVariableDebugInfo ( Context & context, const Variable & var ) {
        VarInfo * vi = context.makeNode<VarInfo>();
        makeTypeInfo(vi, context, var.type);
        vi->name = context.allocateName(var.name);
        return vi;
    }
    
    bool Program::simulate ( Context & context ) {
        context.thisProgram = this;
        context.globalVariables = (GlobalVariable *) context.allocate( uint32_t(thisModule->globals.size()*sizeof(GlobalVariable)) );
        for ( auto & it : thisModule->globals ) {
            auto pvar = it.second;
            auto & gvar = context.globalVariables[pvar->index];
            gvar.name = context.allocateName(pvar->name);
            gvar.size = pvar->type->getSizeOf();
            gvar.debug = makeVariableDebugInfo(context, *it.second);
            gvar.value = cast<char *>::from((char *)context.allocate(gvar.size));
            gvar.init = pvar->init ? ExprLet::simulateInit(context, pvar, false) : nullptr;
        }
        context.totalVariables = (int) thisModule->globals.size();
        context.functions = (SimFunction *) context.allocate( totalFunctions*sizeof(SimFunction) );
        context.totalFunctions = totalFunctions;
        for ( auto & it : thisModule->functions ) {
            auto pfun = it.second;
            if ( pfun->index==-1 )
                continue;
            auto & gfun = context.functions[pfun->index];
            gfun.name = context.allocateName(pfun->name);
            gfun.code = pfun->simulate(context);
            gfun.stackSize = pfun->totalStackSize;
            gfun.debug = makeFunctionDebugInfo(context, *pfun);
        }
        sdebug.clear();
        context.simEnd();
        context.restart();
        context.runInitScript();
        context.restart();
        return errors.size() == 0;
    }
    
    void Program::error ( const string & str, const LineInfo & at, CompilationError cerr ) {
        // cout << "ERROR: " << str << ", at " << at.describe() << "\n";
        errors.emplace_back(str,at,cerr);
        failToCompile = true;
    }
    
    void Program::addModule ( Module * pm ) {
        library.addModule(pm);
    }
    
    bool Program::addVariable ( const VariablePtr & var ) {
        return thisModule->addVariable(var);
    }
    
    bool Program::addStructure ( const StructurePtr & st ) {
        return thisModule->addStructure(st);
    }
    
    bool Program::addFunction ( const FunctionPtr & fn ) {
        return thisModule->addFunction(fn);
    }
    
    bool Program::addGeneric ( const FunctionPtr & fn ) {
        return thisModule->addGeneric(fn);
    }
    
    bool Program::addStructureHandle ( const StructurePtr & st, const TypeAnnotationPtr & ann, const AnnotationArgumentList & arg ) {
        if ( ann->rtti_isStructureAnnotation() ) {
            auto annotation = static_pointer_cast<StructureTypeAnnotation>(ann->clone());
            annotation->name = st->name;
            string err;
            if ( annotation->create(st,arg,err) ) {
                thisModule->addAnnotation(annotation);
                return true;
            } else {
                error("can't create structure handle "+ann->name + "\n" + err,st->at,CompilationError::invalid_annotation);
                return false;
            }
        } else {
            error("not a structure annotation "+ann->name,st->at,CompilationError::invalid_annotation);
            return false;
        }
    }
    
    Program::Program() {
        thisModule = make_unique<Module>();
        library.addBuiltInModule();
        library.addModule(thisModule.get());
    }
    
    TypeDecl * Program::makeTypeDeclaration(const LineInfo &at, const string &name) {
        auto structs = findStructure(name);
        auto handles = findAnnotation(name);
        if ( structs.size() && handles.size() ) {
            string candidates = describeCandidates(structs);
            candidates += describeCandidates(handles, false);
            error("undefined type "+name + "\n" + candidates,at,CompilationError::type_not_found);
            return nullptr;
        } else if ( structs.size() ) {
            if ( structs.size()==1 ) {
                auto pTD = new TypeDecl(Type::tStructure);
                pTD->structType = structs.back();
                pTD->at = at;
                return pTD;
            } else {
                string candidates = describeCandidates(structs);
                error("too many options for "+name + "\n" + candidates,at,CompilationError::structure_not_found);
                return nullptr;
            }
        } else if ( handles.size() ) {
            if ( handles.size()==1 ) {
                if ( handles.back()->rtti_isHandledTypeAnnotation() ) {
                    auto pTD = new TypeDecl(Type::tHandle);
                    pTD->annotation = static_pointer_cast<TypeAnnotation>(handles.back());
                    pTD->at = at;
                    return pTD;
                } else {
                    error("not a handled type annotation "+name,at,CompilationError::handle_not_found);
                    return nullptr;
                }
            } else {
                string candidates = describeCandidates(handles);
                error("too many options for "+name + "\n" + candidates,at,CompilationError::handle_not_found);
                return nullptr;
            }
        } else {
            auto tt = new TypeDecl(Type::alias);
            tt->alias = name;
            return tt;
        }
    }
    
    ExprLooksLikeCall * Program::makeCall ( const LineInfo & at, const string & name ) {
        vector<ExprCallFactory *> ptr;
        string moduleName, funcName;
        splitTypeName(name, moduleName, funcName);
        library.foreach([&](Module * pm) -> bool {
            if ( auto pp = pm->findCall(funcName) )
                ptr.push_back(pp);
            return false;
        }, moduleName);
        if ( ptr.size()==1 ) {
            return (*ptr.back())(at);
        } else if ( ptr.size()==0 ) {
            return new ExprCall(at,name);
        } else {
            error("too many options for " + name, at, CompilationError::function_not_found);
            return new ExprCall(at,name);
        }
    }
    
    ExpressionPtr Program::makeConst ( const LineInfo & at, const TypeDeclPtr & type, __m128 value ) {
        if ( type->dim.size() || type->ref ) return nullptr;
        switch ( type->baseType ) {
            case Type::tBool:       return make_shared<ExprConstBool>(at, cast<bool>::to(value));
            case Type::tInt:        return make_shared<ExprConstInt>(at, cast<int32_t>::to(value));
            case Type::tInt2:       return make_shared<ExprConstInt2>(at, cast<int2>::to(value));
            case Type::tInt3:       return make_shared<ExprConstInt3>(at, cast<int3>::to(value));
            case Type::tInt4:       return make_shared<ExprConstInt4>(at, cast<int4>::to(value));
            case Type::tUInt64:     return make_shared<ExprConstUInt64>(at, cast<uint64_t>::to(value));
            case Type::tUInt:       return make_shared<ExprConstUInt>(at, cast<uint32_t>::to(value));
            case Type::tUInt2:      return make_shared<ExprConstUInt2>(at, cast<uint2>::to(value));
            case Type::tUInt3:      return make_shared<ExprConstUInt3>(at, cast<uint3>::to(value));
            case Type::tUInt4:      return make_shared<ExprConstUInt4>(at, cast<uint4>::to(value));
            case Type::tFloat:      return make_shared<ExprConstFloat>(at, cast<float>::to(value));
            case Type::tFloat2:     return make_shared<ExprConstFloat2>(at, cast<float2>::to(value));
            case Type::tFloat3:     return make_shared<ExprConstFloat3>(at, cast<float3>::to(value));
            case Type::tFloat4:     return make_shared<ExprConstFloat4>(at, cast<float4>::to(value));
            default:                assert(0 && "we should not even be here"); return nullptr;
        }
    }
    
    void Program::visit(Visitor & vis, bool visitGenerics ) {
        // structures
        for ( auto & ist : thisModule->structures ) {
            vis.preVisit(ist.second.get());
            for ( auto & fi : ist.second->fields ) {
                vis.preVisitStructureField(ist.second.get(), fi, &fi==&ist.second->fields.back());
            }
            ist.second = vis.visit(ist.second.get());
        }
        // globals
        for ( auto & it : thisModule->globals ) {
            auto & var = it.second;
            vis.preVisitGlobalLet(var);
            if ( var->init ) {
                vis.preVisitGlobalLetInit(var, var->init.get());
                var->init = var->init->visit(vis);
                var->init = vis.visitGlobalLetInit(var, var->init.get());
            }
            var = vis.visitGlobalLet(var);
        }
        // generics
        if ( visitGenerics ) {
            for ( auto & fn : thisModule->generics ) {
                if ( !fn.second->builtIn ) {
                    fn.second = fn.second->visit(vis);
                }
            }
        }
        // functions
        for ( auto & fn : thisModule->functions ) {
            if ( !fn.second->builtIn ) {
                fn.second = fn.second->visit(vis);
            }
        }
    }
    
    void Program::optimize() {
        const bool log = options.getOption("logOptimizationPasses",false);
        bool any, last;
        if (log) {
            cout << *this << "\n";
            cout.flush();
        }
        do {
            if ( log ) cout << "OPTIMIZE:\n" << *this;
            any = false;
            last = optimizationRefFolding();    if ( failed() ) break;  any |= last;
            if ( log ) cout << "REF FOLDING: " << (last ? "optimized" : "nothing") << "\n" << *this;
            last = optimizationConstFolding();  if ( failed() ) break;  any |= last;
            if ( log ) cout << "CONST FOLDING:" << (last ? "optimized" : "nothing") << "\n" << *this;
            last = optimizationBlockFolding();  if ( failed() ) break;  any |= last;
            if ( log ) cout << "BLOCK FOLDING:" << (last ? "optimized" : "nothing") << "\n" << *this;
            last = optimizationUnused();        if ( failed() ) break;  any |= last;
            if ( log ) cout << "REMOVE UNUSED:" << (last ? "optimized" : "nothing") << "\n" << *this;
            if ( log ) cout.flush();
        } while ( any );
    }

    // PARSER
    
    ProgramPtr g_Program;
    
    ProgramPtr parseDaScript ( const char * script ) {
        int err;
        auto program = g_Program = make_shared<Program>();
        yybegin(script);
        err = yyparse();        // TODO: add mutex or make thread safe?
        yylex_destroy();
        g_Program.reset();
        if ( err || program->failed() ) {
            sort(program->errors.begin(),program->errors.end());
            return program;
        } else {
            program->inferTypes();
            if ( !program->failed() ) {
				if (program->options.getOption("optimize", true)) {
					program->optimize();
				}
                if (!program->failed())
					program->staticAsserts();
                if (!program->failed())
                    program->allocateStack();
                if ( !program->failed() ) {
                    program->finalizeAnnotations();
                }
            }
			if (!program->failed()) {
				if (program->options.getOption("log")) {
					cout << *program;
				}
				if (program->options.getOption("plot")) {
					if (auto pf = program->options.find("plotFile", Type::tString)) {
						ofstream of(pf->sValue);
						if (of.is_open()) {
							of << program->dotGraph() << "\n";
							of.close();
						}
					} else {
						cout << "\n" << program->dotGraph() << "\n";
					}
				}
			}
			sort(program->errors.begin(), program->errors.end());
            return program;
        }
    }
}
