#include "precomp.h"

#include "ast.h"
#include "enums.h"

#include "runtime_array.h"
#include "runtime_table.h"
#include "runtime_range.h"
#include "hash.h"

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
        {   Operator::postInc,      "+++"   },
        {   Operator::postDec,      "---"   },
    };
    
    Enum<Operator> g_opTable1 = {
        {   Operator::at,       "@"    },
        {   Operator::dot,      "."    },
        {   Operator::add,      "+"    },
        {   Operator::sub,      "-"    },
        {   Operator::div,      "/"    },
        {   Operator::mul,      "*"    },
        {   Operator::mod,      "%"    },
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
    
    bool isUnaryOperator ( Operator op ) {
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
    
    bool isBinaryOperator ( Operator op ) {
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
    
    bool isTrinaryOperator ( Operator op ) {
        return (op==Operator::is);
    }
    
    // TypeDecl
    
    ostream& operator<< (ostream& stream, const TypeDecl & decl) {
        if ( decl.constant ) {
            stream << "const ";
        }
        if ( decl.baseType==Type::tHandle ) {
            stream << decl.annotation->name;
        } else if ( decl.baseType==Type::tArray ) {
            if ( decl.firstType ) {
                stream << "array (" << *decl.firstType << ")";
            } else {
                stream << "array";
            }
        } else if ( decl.baseType==Type::tTable ) {
            if ( decl.firstType && decl.secondType ) {
                stream << "table (" << *decl.firstType << "," << *decl.secondType << ")";
            } else {
                stream << "table";
            }
        } else if ( decl.baseType==Type::tStructure ) {
            if ( decl.structType ) {
                stream << decl.structType->name;
            } else {
                stream << "unspecified";
            }
        } else if ( decl.baseType==Type::tPointer ) {
            if ( decl.firstType ) {
                stream << *decl.firstType << " ?";
            } else {
                stream << "void ?";
            }
        } else if ( decl.baseType==Type::tIterator ) {
            if ( decl.firstType ) {
                stream << "iterator<" << *decl.firstType << ">";
            } else {
                stream << "iterator";
            }
        } else if ( decl.baseType==Type::tBlock ) {
            if ( decl.firstType ) {
                stream << "block<" << *decl.firstType << ">";
            } else {
                stream << "block";
            }
        } else {
            stream << to_string(decl.baseType);
        }
        for ( auto d : decl.dim ) {
            stream << " " << d;
        }
        if ( decl.ref )
            stream << " &";
        return stream;
    }
    
    TypeDecl::TypeDecl(const TypeDecl & decl) {
        baseType = decl.baseType;
        structType = decl.structType;
        annotation = decl.annotation;
        dim = decl.dim;
        ref = decl.ref;
        constant = decl.constant;
        at = decl.at;
        if ( decl.firstType )
            firstType = make_shared<TypeDecl>(*decl.firstType);
        if ( decl.secondType )
            secondType = make_shared<TypeDecl>(*decl.secondType);
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
            if ( firstType ) {
                ss << "#" << firstType->getMangledName();
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
    
    bool TypeDecl::isHandle() const {
        return (baseType==Type::tHandle) && (dim.size()==0);
    }
    
    bool TypeDecl::isSimpleType() const {
        if (    baseType==Type::none
            ||  baseType==Type::tVoid
            ||  baseType==Type::tStructure
            ||  baseType==Type::tPointer )
            return false;
        if ( dim.size() )
            return false;
        return true;
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
            return annotation->getSizeOf();
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
        pVar->at = at;
        return pVar;
    }
    
    // function
    
    ostream& operator<< (ostream& stream, const Function & func) {
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
    
    // built-in function
    
    BuiltInFunction::BuiltInFunction ( const string & fn ) {
        builtIn = true;
        name = fn;
    }
    
    // expression
    
    void Expression::InferTypeContext::error ( const string & err, const LineInfo & at, CompilationError cerr ) {
        program->error(err,at,cerr);
    }
    
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
    
    void Expression::logType(ostream& stream) const {
        if ( g_logTypes )
            stream << "$ (" << *type << ") ";
    }
    
    ostream& operator<< (ostream& stream, const Expression & expr) {
        expr.log(stream, 1);
        return stream;
    }
    
    // ExprRef2Value
    
    ExpressionPtr ExprRef2Value::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprRef2Value>(expr);
        Expression::clone(cexpr);
        cexpr->subexpr = subexpr->clone();
        return cexpr;
    }
    
    void ExprRef2Value::log(ostream& stream, int depth) const {
        stream << "(=> " << *subexpr << ")";
    }
    
    void ExprRef2Value::inferType(InferTypeContext & context) {
        type.reset();
        subexpr->inferType(context);
        if ( !subexpr->type ) return;
        // infer
        if ( !subexpr->type->isRef() ) {
            context.error("can only dereference ref", at);
        } else if ( !subexpr->type->isSimpleType() ) {
            context.error("can only dereference a simple type", at);
        } if ( !subexpr->type->canCopy() ) {
            context.error("can't dereference non-copyable type", at);
        } else {
            type = make_shared<TypeDecl>(*subexpr->type);
            type->ref = false;
        }
    }
    
    SimNode * ExprRef2Value::simulate (Context & context) const {
        return context.makeValueNode<SimNode_Ref2Value>(type->baseType, at, subexpr->simulate(context));
    }
    
    // ExprPtr2Ref
    
    ExpressionPtr ExprPtr2Ref::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprPtr2Ref>(expr);
        Expression::clone(cexpr);
        cexpr->subexpr = subexpr->clone();
        return cexpr;
    }
    
    void ExprPtr2Ref::log(ostream& stream, int depth) const {
        stream << "(-> " << *subexpr << ")";
    }
    
    void ExprPtr2Ref::inferType(InferTypeContext & context) {
        type.reset();
        subexpr->inferType(context);
        if ( !subexpr->type ) return;
        // infer
        subexpr = autoDereference(subexpr);
        if ( !subexpr->type->isPointer() ) {
            context.error("can only dereference pointer", at, CompilationError::cant_dereference);
        } else if ( !subexpr->type->firstType || subexpr->type->firstType->isVoid() ) {
            context.error("can only dereference pointer to something", at, CompilationError::cant_dereference);
        } else {
            type = make_shared<TypeDecl>(*subexpr->type->firstType);
            type->ref = true;
            type->constant |= subexpr->type->constant;
        }
    }
    
    SimNode * ExprPtr2Ref::simulate (Context & context) const {
        return context.makeNode<SimNode_Ptr2Ref>(at,subexpr->simulate(context));
    }

    // ExprPtr2Ref
    
    ExpressionPtr ExprNullCoalescing::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprNullCoalescing>(expr);
        ExprPtr2Ref::clone(cexpr);
        cexpr->defaultValue = defaultValue->clone();
        return cexpr;
    }
    
    void ExprNullCoalescing::log(ostream& stream, int depth) const {
        stream << "(?? " << *subexpr << " " << *defaultValue << ")";
    }
    
    void ExprNullCoalescing::inferType(InferTypeContext & context) {
        subexpr->inferType(context);
        defaultValue->inferType(context);
        if ( !subexpr->type || !defaultValue->type ) return;
        // infer
        subexpr = autoDereference(subexpr);
        auto seT = subexpr->type;
        auto dvT = defaultValue->type;
        if ( !seT->isPointer() ) {
            context.error("can only dereference pointer", at, CompilationError::cant_dereference);
        } else if ( !seT->firstType || seT->firstType->isVoid() ) {
            context.error("can only dereference pointer to something", at, CompilationError::cant_dereference);
        } else if ( !seT->firstType->isSameType(*dvT,false,false) ) {
            context.error("default value type mismatch in " + seT->firstType->describe() + " vs " + dvT->describe(),
                          at, CompilationError::cant_dereference);
        } else if ( !seT->isConst() && dvT->isConst() ) {
            context.error("default value type mismatch, constant matters in " + seT->describe() + " vs " + dvT->describe(),
                          at, CompilationError::cant_dereference);
        } else {
            type = make_shared<TypeDecl>(*dvT);
            type->constant |= subexpr->type->constant;
        }
    }
    
    SimNode * ExprNullCoalescing::simulate (Context & context) const {
        if ( type->ref ) {
            return context.makeNode<SimNode_NullCoalescingRef>(at,subexpr->simulate(context),defaultValue->simulate(context));
        } else {
            return context.makeValueNode<SimNode_NullCoalescing>(type->baseType,at,subexpr->simulate(context),defaultValue->simulate(context));
        }
    }

    
    // ExprAssert
    
    ExpressionPtr ExprAssert::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprAssert>(expr);
        ExprLooksLikeCall::clone(cexpr);
        return cexpr;
    }
    
    void ExprAssert::inferType(InferTypeContext & context) {
        type.reset();
        ExprLooksLikeCall::inferType(context);
        if ( arguments.size()<1 || arguments.size()>2 ) {
            context.error("assert(expr) or assert(expr,string)", at, CompilationError::invalid_argument_count);
            return;
        }
        if ( argumentsFailedToInfer ) return;
        // infer
        autoDereference();
        if ( !arguments[0]->type->isSimpleType(Type::tBool) )
            context.error("assert condition must be boolean", at, CompilationError::invalid_argument_type);
        if ( arguments.size()==2 && !arguments[1]->isStringConstant() )
            context.error("assert comment must be string constant", at, CompilationError::invalid_argument_type);
        type = make_shared<TypeDecl>(Type::tVoid);
    }
    
    SimNode * ExprAssert::simulate (Context & context) const {
        string message;
        if ( arguments.size()==2 && arguments[1]->isStringConstant() )
            message = static_pointer_cast<ExprConstString>(arguments[1])->getValue();
        return context.makeNode<SimNode_Assert>(at,arguments[0]->simulate(context),context.allocateName(message));
    }
    
    // ExprDebug
    
    ExpressionPtr ExprDebug::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprDebug>(expr);
        ExprLooksLikeCall::clone(cexpr);
        return cexpr;
    }
    
    void ExprDebug::inferType(InferTypeContext & context) {
        type.reset();
        if ( arguments.size()<1 || arguments.size()>2 ) {
            context.error("debug(expr) or debug(expr,string)", at, CompilationError::invalid_argument_count);
            return;
        }
        ExprLooksLikeCall::inferType(context);
        if ( argumentsFailedToInfer ) return;
        // infer
        if ( arguments.size()==2 && !arguments[1]->isStringConstant() )
            context.error("debug comment must be string constant", at, CompilationError::invalid_argument_type);
        type = make_shared<TypeDecl>(*arguments[0]->type);
    }
    
    SimNode * ExprDebug::simulate (Context & context) const {
        TypeInfo * pTypeInfo = context.makeNode<TypeInfo>();
        context.thisProgram->makeTypeInfo(pTypeInfo, context, arguments[0]->type);
        string message;
        if ( arguments.size()==2 && arguments[1]->isStringConstant() )
            message = static_pointer_cast<ExprConstString>(arguments[1])->getValue();
        return context.makeNode<SimNode_Debug>(at,
                                               arguments[0]->simulate(context),
                                               pTypeInfo,
                                               context.allocateName(message));
    }

    // ExprMakeBlock
    
    void ExprMakeBlock::inferType(InferTypeContext & context) {
        type.reset();
        block->inferType(context);
        // infer
        type = make_shared<TypeDecl>(Type::tBlock);
        if ( block->type ) {
            type->firstType = make_shared<TypeDecl>(*block->type);
        }
    }
    
    void ExprMakeBlock::log(ostream& stream, int depth) const {
        stream << "(make_block\n";
        stream << string(depth, '\t');
        block->log(stream, depth+1);
        stream << ")";
    }
    
    SimNode * ExprMakeBlock::simulate (Context & context) const {
        return context.makeNode<SimNode_MakeBlock>(at,block->simulate(context));
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
    
    void ExprInvoke::inferType(InferTypeContext & context) {
        type.reset();
        if ( arguments.size()!=1 ) {
            context.error("invoke(block)", at, CompilationError::invalid_argument_count);
            return;
        }
        ExprLooksLikeCall::inferType(context);
        if ( argumentsFailedToInfer ) return;
        // infer
        arguments[0] = Expression::autoDereference(arguments[0]);
        auto blockT = arguments[0]->type;
        if ( !blockT->isGoodBlockType() ) {
            context.error("expecting block", at, CompilationError::invalid_argument_type);
        }
        if ( blockT->firstType ) {
            type = make_shared<TypeDecl>(*blockT->firstType);
        } else {
            type = make_shared<TypeDecl>();
        }
    }
    
    SimNode * ExprInvoke::simulate (Context & context) const {
        return context.makeNode<SimNode_Invoke>(at,arguments[0]->simulate(context));
    }
    
    // ExprHash
    
    ExpressionPtr ExprHash::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprHash>(expr);
        ExprLooksLikeCall::clone(cexpr);
        return cexpr;
    }
    
    void ExprHash::inferType(InferTypeContext & context) {
        type.reset();
        if ( arguments.size()!=1 ) {
            context.error("hash(expr)", at, CompilationError::invalid_argument_count);
            return;
        }
        ExprLooksLikeCall::inferType(context);
        if ( argumentsFailedToInfer ) return;
        // infer
        type = make_shared<TypeDecl>(Type::tUInt64);
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
    
    void ExprArrayPush::inferType(InferTypeContext & context) {
        type.reset();
        if ( arguments.size()!=2 && arguments.size()!=3 ) {
            context.error("push(array,value) or push(array,value,at)", at, CompilationError::invalid_argument_count);
            return;
        }
        ExprLooksLikeCall::inferType(context);
        if ( argumentsFailedToInfer ) return;
        // infer
        auto arrayType = arguments[0]->type;
        auto valueType = arguments[1]->type;
        if ( !arrayType->isGoodArrayType() ) {
            context.error("push first argument must be fully qualified array", at, CompilationError::invalid_argument_type);
            return;
        }
        if ( !arrayType->firstType->isSameType(*valueType,false) )
            context.error("can't push value of different type", at, CompilationError::invalid_argument_type);
        if ( arguments.size()==3 && !arguments[2]->type->isIndex() )
            context.error("push at must be an index", at, CompilationError::invalid_argument_type);
        type = make_shared<TypeDecl>(Type::tVoid);
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
    
    void ExprErase::inferType(InferTypeContext & context) {
        type.reset();
        if ( arguments.size()!=2 ) {
            context.error("erase(table,key) or erase(array,index)", at, CompilationError::invalid_argument_count);
            return;
        }
        ExprLooksLikeCall::inferType(context);
        if ( argumentsFailedToInfer ) return;
        // infer
        arguments[1] = Expression::autoDereference(arguments[1]);
        auto containerType = arguments[0]->type;
        auto valueType = arguments[1]->type;
        if ( containerType->isGoodArrayType() ) {
            if ( !valueType->isIndex() )
                context.error("size must be int or uint", at, CompilationError::invalid_argument_type);
            type = make_shared<TypeDecl>(Type::tVoid);
        } else if ( containerType->isGoodTableType() ) {
            if ( !containerType->firstType->isSameType(*valueType,false) )
                context.error("key must be of the same type as table<key,...>", at, CompilationError::invalid_argument_type);
            type = make_shared<TypeDecl>(Type::tBool);
        } else {
            context.error("first argument must be fully qualified array or table", at, CompilationError::invalid_argument_type);
        }
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
    
    void ExprFind::inferType(InferTypeContext & context) {
        type.reset();
        if ( arguments.size()!=2 ) {
            context.error("find(table,key) or find(array,value)", at, CompilationError::invalid_argument_count);
            return;
        }
        ExprLooksLikeCall::inferType(context);
        if ( argumentsFailedToInfer ) return;
        // infer
        arguments[1] = Expression::autoDereference(arguments[1]);
        auto containerType = arguments[0]->type;
        auto valueType = arguments[1]->type;
        if ( containerType->isGoodArrayType() ) {
            if ( !valueType->isSameType(*containerType->firstType) )
                context.error("value must be of the same type as array<value>", at, CompilationError::invalid_argument_type);
        } else if ( containerType->isGoodTableType() ) {
            if ( !containerType->firstType->isSameType(*valueType,false) )
                context.error("key must be of the same type as table<key,...>", at, CompilationError::invalid_argument_type);
            type = make_shared<TypeDecl>(Type::tPointer);
            type->firstType = make_shared<TypeDecl>(*containerType->secondType);
        } else {
            context.error("first argument must be fully qualified array or table", at, CompilationError::invalid_argument_type);
        }
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
    
    ExpressionPtr ExprSizeOf::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprSizeOf>(expr);
        Expression::clone(cexpr);
        if ( subexpr )
            cexpr->subexpr = subexpr->clone();
        if ( typeexpr )
            cexpr->typeexpr = typeexpr;
        return cexpr;
    }
    
    void ExprSizeOf::log(ostream& stream, int depth) const {
        stream << "(sizeof ";
        if ( subexpr )
            stream << *subexpr;
        else
            stream << "(" << *typeexpr << ")";
        stream << ")";
    }
    
    void ExprSizeOf::inferType(InferTypeContext & context) {
        type.reset();
        subexpr->inferType(context);
        if ( !subexpr->type ) return;
        // infer
        typeexpr = make_shared<TypeDecl>(*subexpr->type);
        type = make_shared<TypeDecl>(Type::tInt);
    }
    
    SimNode * ExprSizeOf::simulate (Context & context) const {
        int32_t size = typeexpr->getSizeOf();
        return context.makeNode<SimNode_ConstValue<int32_t>>(at,size);
    }
    
    // ExprNew
    
    ExpressionPtr ExprNew::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprNew>(expr);
        Expression::clone(cexpr);
        cexpr->typeexpr = typeexpr;
        return cexpr;
    }
    
    void ExprNew::log(ostream& stream, int depth) const {
        stream << "(new (" << *typeexpr << "))";
    }
    
    // TODO:
    //  this would need proper testing, but only afrer parser is modified
    //  curently none of the errors bellow can even be parsed
    void ExprNew::inferType(InferTypeContext & context) {
        type.reset();
        // infer
        if ( typeexpr->ref ) {
            context.error("can't new a ref", typeexpr->at, CompilationError::invalid_new_type);
        } else if ( typeexpr->dim.size() ) {
            context.error("can only new single object", typeexpr->at, CompilationError::invalid_new_type);
        } else if ( typeexpr->baseType==Type::tStructure || typeexpr->isHandle() ) {
            type = make_shared<TypeDecl>(Type::tPointer);
            type->firstType = make_shared<TypeDecl>(*typeexpr);
        } else {
            context.error("can only new structures or handles", typeexpr->at, CompilationError::invalid_new_type);
        }
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
    
    void ExprAt::log(ostream& stream, int depth) const {
        stream << "(@ " << *subexpr << " " << *index << ")";
    }
    
    void ExprAt::inferType(InferTypeContext & context) {
        type.reset();
        subexpr->inferType(context);
        index->inferType(context);
        if ( !subexpr->type || !index->type ) return;
        // infer
        index = autoDereference(index);
        if ( subexpr->type->isGoodTableType() ) {
            if ( !subexpr->type->firstType->isSameType(*index->type) ) {
                context.error("table index type mismatch", index->at, CompilationError::invalid_index_type);
                return;
            }
            type = make_shared<TypeDecl>(*subexpr->type->secondType);
            type->ref = true;
            type->constant |= subexpr->type->constant;
        } else {
            if ( !index->type->isIndex() ) {
                context.error("index is int or uint", index->at, CompilationError::invalid_index_type);
                return;
            }
            if ( subexpr->type->isGoodArrayType() ) {
                type = make_shared<TypeDecl>(*subexpr->type->firstType);
                type->ref = true;
                type->constant |= subexpr->type->constant;
            } else if ( !subexpr->type->isRef() ) {
                context.error("can only index ref", subexpr->at, CompilationError::cant_index);
            } else if ( !subexpr->type->dim.size() ) {
                context.error("can only index arrays", subexpr->at, CompilationError::cant_index);
            } else {
                type = make_shared<TypeDecl>(*subexpr->type);
                type->ref = true;
                type->dim.pop_back();
            }
        }
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
        if ( subexpr->type->isGoodTableType() ) {
            uint32_t valueTypeSize = subexpr->type->secondType->getSizeOf();
            return context.makeValueNode<SimNode_TableIndex>(subexpr->type->firstType->baseType, at, prv, pidx, valueTypeSize);
        } else if ( subexpr->type->isGoodArrayType() ) {
            uint32_t stride = subexpr->type->firstType->getSizeOf();
            return context.makeNode<SimNode_ArrayAt>(at, prv, pidx, stride);
        } else {
            uint32_t stride = subexpr->type->getStride();
            uint32_t range = subexpr->type->dim.back();
            return context.makeNode<SimNode_At>(at, prv, pidx, stride, range);
        }
    }

    // ExprBlock
    
    ExpressionPtr ExprBlock::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprBlock>(expr);
        Expression::clone(cexpr);
        for ( auto & subexpr : list ) {
            cexpr->list.push_back(subexpr->clone());
        }
        return cexpr;
    }
    
    void ExprBlock::log(ostream& stream, int depth) const {
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
    
    void ExprBlock::setBlockReturnsValue() {
        returnsValue = true;
        if ( list.size() ) {
            list.back()->setBlockReturnsValue();
        }
    }
    
    uint32_t ExprBlock::getEvalFlags() const {
        uint32_t flags = 0;
        for ( const auto & ex : list ) {
            flags |= ex->getEvalFlags();
        }
        return flags;
    }
    
    void ExprBlock::inferType(InferTypeContext & context) {
        type.reset();
        // infer
        for ( auto & ex : list ) {
            ex->inferType(context);
        }
        // block type
        if ( returnsValue && list.size() ) {
            uint32_t flags = getEvalFlags();
            if ( flags & EvalFlags::stopForReturn ) {
                context.error("captured block can't return outside of the block", at, CompilationError::invalid_block);
            } else if ( flags & EvalFlags::stopForBreak ) {
                context.error("captured block can't break outside of the block", at, CompilationError::invalid_block);
            } else {
                auto & tail = list.back();
                if ( tail->type ) {
                    tail = autoDereference(tail);
                    type = make_shared<TypeDecl>(*tail->type);
                }
            }
        }
    }
    
    SimNode * ExprBlock::simulate (Context & context) const {
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
    
    ExpressionPtr ExprField::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprField>(expr);
        Expression::clone(cexpr);
        cexpr->name = name;
        cexpr->value = value->clone();
        cexpr->field = field;
        return cexpr;
    }
    
    void ExprField::log(ostream& stream, int depth) const {
        logType(stream);
        stream << "(. ";
        value->log(stream,depth+1);
        stream << " " << name << ")";
    }
    
    void ExprField::inferType(InferTypeContext & context) {
        type.reset();
        value->inferType(context);
        if ( !value->type ) return;
        // infer
        TypeDecl * fieldType = nullptr;
        auto valT = value->type;
        if ( valT->isHandle() ) {
            annotation = valT->annotation;
            fieldType = annotation->getField(name);
        } else {
            if ( valT->isPointer() ) {
                value = autoDereference(value);
            }
            if ( valT->isArray() ) {
                context.error("can't get field of array", at, CompilationError::cant_get_field);
                return;
            }
            if ( valT->baseType==Type::tStructure ) {
                field = valT->structType->findField(name);
            } else if ( valT->isPointer() ) {
                if ( valT->firstType->baseType==Type::tStructure ) {
                    field = valT->firstType->structType->findField(name);
                } else if ( valT->firstType->isHandle() ) {
                    annotation = valT->firstType->annotation;
                    fieldType = annotation->getField(name);
                }
            }
        }
        // handle
        fieldType = field ? field->type.get() : fieldType;
        if ( !fieldType ) {
            context.error("field " + name + " not found", at, CompilationError::cant_get_field);
        } else {
            type = make_shared<TypeDecl>(*fieldType);
            type->ref = true;
            type->constant |= valT->constant;
        }
    }
    
    SimNode * ExprField::simulate (Context & context) const {
        if ( !field ) {
            return annotation->simulateGetField(name, context, at, value->simulate(context));
        } else {
            return context.makeNode<SimNode_FieldDeref>(at,value->simulate(context),field->offset);
        }
    }
    
    // ExprSafeField
    
    ExpressionPtr ExprSafeField::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprSafeField>(expr);
        ExprField::clone(cexpr);
        return cexpr;
    }
    
    void ExprSafeField::log(ostream& stream, int depth) const {
        logType(stream);
        stream << "(?. ";
        value->log(stream,depth+1);
        stream << " " << name << ")";
    }
    
    void ExprSafeField::inferType(InferTypeContext & context) {
        type.reset();
        value->inferType(context);
        if ( !value->type ) return;
        // infer
        TypeDecl * fieldType = nullptr;
        auto valT = value->type;
        if ( !valT->isPointer() || !valT->firstType ) {
            context.error("can only safe dereference a pointer to a structure or handle " + valT->describe(),
                          at, CompilationError::cant_get_field);
            return;
        }
        value = autoDereference(value);
        if ( valT->firstType->structType ) {
            field = valT->firstType->structType->findField(name);
            fieldType = field->type.get();
        } else if ( valT->firstType->isHandle() ) {
            fieldType = valT->firstType->annotation->getField(name);
            if ( !fieldType ) {
                context.error("can't get field " + name,
                          at, CompilationError::cant_get_field);
            }
        }
        if ( !fieldType ) {
            context.error("can only safe dereference a pointer to a structure or handle " + valT->describe(),
                          at, CompilationError::cant_get_field);
            return;
        }
        if ( fieldType->isPointer() ) {
            skipQQ = true;
            type = make_shared<TypeDecl>(*fieldType);
            type->constant |= valT->constant;
        } else {
            skipQQ = false;
            type = make_shared<TypeDecl>(Type::tPointer);
            type->firstType = make_shared<TypeDecl>(*fieldType);
            type->constant |= valT->constant;
        }
    }
    
    SimNode * ExprSafeField::simulate (Context & context) const {
        auto valFT = value->type->firstType;
        if ( skipQQ ) {
            if ( valFT->isHandle() ) {
                return valFT->annotation->simulateSafeGetFieldPtr(name, context, at, value->simulate(context));
            } else {
                return context.makeNode<SimNode_SafeFieldDerefPtr>(at,value->simulate(context),field->offset);
            }
        } else {
            if ( valFT->isHandle() ) {
                return valFT->annotation->simulateSafeGetField(name, context, at, value->simulate(context));
            } else {
                return context.makeNode<SimNode_SafeFieldDeref>(at,value->simulate(context),field->offset);
            }
        }
    }
    
    // ExprVar
    
    ExpressionPtr ExprVar::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprVar>(expr);
        Expression::clone(cexpr);
        cexpr->name = name;
        cexpr->variable = variable; // todo: lookup again?
        cexpr->local = local;
        cexpr->argument = argument;
        return cexpr;
    }
    
    void ExprVar::log(ostream& stream, int depth) const {
        logType(stream);
        stream << name;
    }
    
    void ExprVar::inferType(InferTypeContext & context) {
        type.reset();
        // infer
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
    
    SimNode * ExprVar::simulate (Context & context) const {
        if ( local ) {
            if ( variable->type->ref ) {
                return context.makeNode<SimNode_GetLocalRef>(at, variable->stackTop);
            } else {
                return context.makeNode<SimNode_GetLocal>(at, variable->stackTop);
            }
        } else if ( argument) {
            return context.makeNode<SimNode_GetArgument>(at, argumentIndex);
        } else {
            return context.makeNode<SimNode_GetGlobal>(at, variable->index);
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
        return cexpr;
    }
    
    // ExprOp1
    
    ExpressionPtr ExprOp1::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprOp1>(expr);
        ExprOp::clone(cexpr);
        cexpr->subexpr = subexpr->clone();
        return cexpr;
    }
    
    void ExprOp1::log(ostream& stream, int depth) const {
        logType(stream);
        stream << "(" << to_string(op) << " ";
        subexpr->log(stream, depth);
        stream << ")";
    }
    
    void ExprOp1::inferType(InferTypeContext & context) {
        type.reset();
        subexpr->inferType(context);
        if ( !subexpr->type ) return;
        // infer
        vector<TypeDeclPtr> types = { subexpr->type };
        auto functions = context.program->findMatchingFunctions(to_string(op), types);
        if ( functions.size()==0 ) {
            context.error("no matching operator '" + to_string(op)
                          + "' with argument (" + subexpr->type->describe() + ")", at, CompilationError::operator_not_found);
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
        constexpression = subexpr->constexpression;
    }
    
    SimNode * ExprOp1::simulate (Context & context) const {
        auto pSimOp1 = static_cast<SimNode_Op1 *>(func->makeSimNode(context));
        pSimOp1->x = subexpr->simulate(context);
        return pSimOp1;
    }
    
    // ExprOp2
    
    ExpressionPtr ExprOp2::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprOp2>(expr);
        ExprOp::clone(cexpr);
        cexpr->left = left->clone();
        cexpr->right = right->clone();
        return cexpr;
    }
    
    void ExprOp2::log(ostream& stream, int depth) const {
        logType(stream);
        stream << "(" << to_string(op) << " ";
        left->log(stream, depth);
        stream << " ";
        right->log(stream, depth);
        stream << ")";
    }
    
    void ExprOp2::inferType(InferTypeContext & context) {
        type.reset();
        left->inferType(context);
        right->inferType(context);
        if ( !left->type || !right->type ) return;
        // infer
        if ( left->type->isPointer() && right->type->isPointer() )
            if ( !left->type->isSameType(*right->type,false) )
                context.error("operations on incompatible pointers are prohibited", at);
        vector<TypeDeclPtr> types = { left->type, right->type };
        auto functions = context.program->findMatchingFunctions(to_string(op), types);
        if ( functions.size()==0 ) {
            context.error("no matching operator '" + to_string(op)
                + "' with arguments (" + left->type->describe() + ", " + right->type->describe() + ")", at, CompilationError::operator_not_found);
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
        constexpression = left->constexpression && right->constexpression;
    }
    
    SimNode * ExprOp2::simulate (Context & context) const {
        auto pSimOp2 = static_cast<SimNode_Op2 *>(func->makeSimNode(context));
        pSimOp2->l = left->simulate(context);
        pSimOp2->r = right->simulate(context);
        return pSimOp2;
    }

    // ExprOp3
    
    ExpressionPtr ExprOp3::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprOp3>(expr);
        ExprOp::clone(cexpr);
        cexpr->subexpr = subexpr->clone();
        cexpr->left = left->clone();
        cexpr->right = right->clone();
        return cexpr;
    }
    
    void ExprOp3::log(ostream& stream, int depth) const {
        logType(stream);
        stream << "(" << to_string(op) << " ";
        subexpr->log(stream, depth);
        stream << " ";
        left->log(stream, depth);
        stream << " ";
        right->log(stream, depth);
        stream << ")";
    }
    
    void ExprOp3::inferType(InferTypeContext & context) {
        type.reset();
        subexpr->inferType(context);
        left->inferType(context);
        right->inferType(context);
        if ( !subexpr->type || !left->type || !right->type ) return;
        // infer
        if ( op!=Operator::is ) {
            context.error("Op3 currently only supports 'is'", at, CompilationError::operator_not_found);
            return;
        }
        subexpr = autoDereference(subexpr);
        if ( !subexpr->type->isSimpleType(Type::tBool) ) {
            context.error("cond operator condition must be boolean", at, CompilationError::condition_must_be_bool);
        } else if ( !left->type->isSameType(*right->type,false,false) ) {
            context.error("cond operator must return same types on both sides", at, CompilationError::operator_not_found);
        } else {
            if ( left->type->ref ^ right->type->ref ) { // if either one is not ref
                left = autoDereference(left);
                right = autoDereference(right);
            }
            type = make_shared<TypeDecl>(*left->type);
            type->constant |= right->type->constant;
        }
        constexpression = subexpr->constexpression && left->constexpression && right->constexpression;
    }
    
    SimNode * ExprOp3::simulate (Context & context) const {
        return context.makeNode<SimNode_IfThenElse>(at,
                                                    subexpr->simulate(context),
                                                    left->simulate(context),
                                                    right->simulate(context));
    }
    
    // common for move and copy
    
    SimNode * makeCopy(const LineInfo & at, Context & context, const TypeDecl & rightType, SimNode * left, SimNode * right ) {
        assert ( rightType.canCopy() && "should check above" );
        if ( rightType.isRef() ) {
            return context.makeNode<SimNode_CopyRefValue>(at, left, right, rightType.getSizeOf());
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
    
    void ExprMove::log(ostream& stream, int depth) const {
        logType(stream);
        stream << "(<- ";
        left->log(stream, depth);
        stream << " ";
        right->log(stream, depth);
        stream << ")";
    }
    
    void ExprMove::inferType(InferTypeContext & context) {
        type.reset();
        left->inferType(context);
        right->inferType(context);
        if ( !left->type || !right->type ) return;
        // infer
        if ( !left->type->isSameType(*right->type,false,false) ) {
            context.error("can only move same type", at, CompilationError::operator_not_found);
        } else if ( !left->type->isRef() ) {
            context.error("can only move to reference", at, CompilationError::cant_write_to_non_reference);
        } else if ( left->type->constant ) {
            context.error("can't move to constant value", at, CompilationError::cant_move_to_const);
        } else if ( !left->type->canMove() ) {
            context.error("this type can't be moved", at, CompilationError::cant_move);
        } else if ( left->type->canCopy() ) {
            context.error("this type can be copied, use = instead", at, CompilationError::cant_move);
        }
        type = make_shared<TypeDecl>(*left->type);  // we return left
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
    
    void ExprCopy::log(ostream& stream, int depth) const {
        logType(stream);
        stream << "(= ";
        left->log(stream, depth);
        stream << " ";
        right->log(stream, depth);
        stream << ")";
    }
    
    void ExprCopy::inferType(InferTypeContext & context) {
        type.reset();
        left->inferType(context);
        right->inferType(context);
        if ( !left->type || !right->type ) return;
        // infer
        if ( !left->type->isSameType(*right->type,false,false) ) {
            context.error("can only copy same type " + left->type->describe() + " vs " + right->type->describe(),
                          at, CompilationError::operator_not_found);
        } else if ( !left->type->isRef() ) {
            context.error("can only copy to reference", at, CompilationError::cant_write_to_non_reference);
        } else if ( left->type->constant ) {
            context.error("can't write to constant value", at, CompilationError::cant_write_to_const);
        }
        if ( !left->type->canCopy() ) {
            context.error("this type can't be copied, use <- instead", at, CompilationError::cant_copy);
        }
        type = make_shared<TypeDecl>(*left->type);  // we return left
    }
    
    ExpressionPtr ExprCopy::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprCopy>(expr);
        ExprOp2::clone(cexpr);
        return cexpr;
    }
    
    SimNode * ExprCopy::simulate (Context & context) const {
        return makeCopy(at, context, *right->type, left->simulate(context), right->simulate(context));
    }
    
    // ExprTryCatch
    
    void ExprTryCatch::log(ostream& stream, int depth) const {
        stream << "(try\n";
        stream << string(depth+2,'\t');
        try_block->log(stream, depth+1);
        stream << "\n" << string(depth+2,'\t');
        catch_block->log(stream, depth+1);
        stream << ")";
    }
    
    uint32_t ExprTryCatch::getEvalFlags() const {
        return (try_block->getEvalFlags() | catch_block->getEvalFlags()) & ~EvalFlags::stopForThrow;
    }
    
    void ExprTryCatch::inferType(InferTypeContext & context) {
        type.reset();
        // infer
        try_block->inferType(context);
        catch_block->inferType(context);
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
    
    ExpressionPtr ExprReturn::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprReturn>(expr);
        Expression::clone(cexpr);
        if ( subexpr )
            cexpr->subexpr = subexpr->clone();
        return cexpr;
    }
    
    void ExprReturn::log(ostream& stream, int depth) const {
        if ( subexpr ) {
            stream << "(return ";
            subexpr->log(stream, depth);
            stream << ")";
        } else {
            stream << "return";
        }
    }
    
    void ExprReturn::inferType(InferTypeContext & context) {
        type.reset();
        if ( subexpr ) {
            subexpr->inferType(context);
            if ( !subexpr->type ) return;
            subexpr = autoDereference(subexpr);
        }
        // infer
        auto resType = context.func->result;
        if ( resType->isVoid() ) {
            if ( subexpr ) {
                context.error("void function has no return", at);
            }
        } else {
            if ( !subexpr ) {
                context.error("must return value", at);
            } else {
                if ( !resType->isSameType(*subexpr->type,true,false) ) {
                    context.error("incompatible return type, expecting "
                                  + resType->describe() + " vs " + subexpr->type->describe(),
                                  at, CompilationError::invalid_return_type);
                }
                if ( resType->isPointer() && !resType->isConst() && subexpr->type->isConst() ) {
                    context.error("incompatible return type, constant matters. expecting "
                                  + resType->describe() + " vs " + subexpr->type->describe(),
                                  at, CompilationError::invalid_return_type);
                }
                type = make_shared<TypeDecl>(*context.func->result);
                type->ref = true;   // we return func-result &
            }
        }
    }
    
    SimNode * ExprReturn::simulate (Context & context) const {
        return context.makeNode<SimNode_Return>(at, subexpr ? subexpr->simulate(context) : nullptr);
    }
    
    // ExprBreak
    
    ExpressionPtr ExprBreak::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprBreak>(expr);
        Expression::clone(cexpr);
        return cexpr;
    }
    
    void ExprBreak::log(ostream& stream, int depth) const {
        stream << "break";
    }
    
    void ExprBreak::inferType(InferTypeContext & context) {
        type.reset();
        // infer
        if ( !context.loop.size() )
            context.error("break without loop", at);
    }
    
    SimNode * ExprBreak::simulate (Context & context) const {
        return context.makeNode<SimNode_Break>(at);
    }

    // ExprIfThenElse
    
    ExpressionPtr ExprIfThenElse::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprIfThenElse>(expr);
        Expression::clone(cexpr);
        cexpr->cond = cond->clone();
        cexpr->if_true = if_true->clone();
        if ( if_false )
            cexpr->if_false = if_false->clone();
        return cexpr;
    }
    
    void ExprIfThenElse::log(ostream& stream, int depth) const {
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
    
    void ExprIfThenElse::inferType(InferTypeContext & context) {
        type.reset();
        cond->inferType(context);
        if_true->inferType(context);
        if ( if_false )
            if_false->inferType(context);
        if ( !cond->type ) return;
        // infer
        if ( !cond->type->isSimpleType(Type::tBool) ) {
            context.error("if-then-else condition must be boolean", at, CompilationError::condition_must_be_bool);
        }
    }
    
    SimNode * ExprIfThenElse::simulate (Context & context) const {
        return context.makeNode<SimNode_IfThenElse>(at, cond->simulate(context), if_true->simulate(context),
                                                    if_false ? if_false->simulate(context) : nullptr);
    }

    // ExprWhile
    
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
    
    void ExprWhile::inferType(InferTypeContext & context) {
        type.reset();
        cond->inferType(context);
        if ( !cond->type ) return;
        // infer
        if ( !cond->type->isSimpleType(Type::tBool) ) {
            context.error("while loop condition must be boolean", at);
        } else {
            context.loop.push_back(shared_from_this());
            body->inferType(context);
            context.loop.pop_back();        }
    }
    
    void ExprWhile::log(ostream& stream, int depth) const {
        stream << "(while\n"<< string(depth+1,'\t');
        cond->log(stream, depth+1);
        stream << "\n" << string(depth+2,'\t');
        body->log(stream, depth+2);
        stream << ")";
    }

    SimNode * ExprWhile::simulate (Context & context) const {
        return context.makeNode<SimNode_While>(at, cond->simulate(context),body->simulate(context));
    }
    
    // ExprFor

    ExpressionPtr ExprFor::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprFor>(expr);
        Expression::clone(cexpr);
        cexpr->iterators = iterators;
        for ( auto & src : sources )
            cexpr->sources.push_back(src->clone());
        for ( auto & var : iteratorVariables )
            cexpr->iteratorVariables.push_back(var->clone());
        cexpr->subexpr = subexpr->clone();
        if ( filter )
            cexpr->filter = filter->clone();
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
    
    void ExprFor::log(ostream& stream, int depth) const {
        stream << "(for (";
        for ( int i=0; i!=iterators.size(); ++i ) {
            if ( i ) stream << " ";
            stream << iterators[i];
        }
        stream << ")\n";
        for ( auto & src : sources ) {
            stream << string(depth+2, '\t') << "(";
            src->log(stream, depth+2);
            stream << ")\n";
        }
        if ( filter ) {
            stream << string(depth+2, '\t') << "(";
            filter->log(stream, depth+2);
            stream << ")\n";
        }
        stream << string(depth+2, '\t');
        subexpr->log(stream, depth+2);
        stream << ")";
    }
    
    uint32_t ExprFor::getEvalFlags() const {
        return subexpr->getEvalFlags() & ~EvalFlags::stopForBreak;
    }
    
    void ExprFor::inferType(InferTypeContext & context) {
        type.reset();
        // infer
        if ( !iterators.size() ) {
            context.error("for needs at least one iterator", at);
            return;
        } else if ( iterators.size() != sources.size() ) {
            context.error("for needs as many iterators as there are sources", at);
            return;
        } else if ( sources.size()>MAX_FOR_ITERATORS ) {
            context.error("too many sources for now", at);
            return;
        }
        auto sp = context.stackTop;
        auto sz = context.local.size();
        // determine iteration types
        fixedArrays = false;
        dynamicArrays = false;
        fixedSize = UINT16_MAX;
        for ( auto & src : sources ) {
            src->inferType(context);
            if ( !src->type ) return;
            if ( src->type->isArray() ) {
                fixedSize = min(fixedSize, src->type->dim.back());
                fixedArrays = true;
            } else if ( src->type->isGoodArrayType() ) {
                dynamicArrays = true;
            } else if ( src->type->isGoodIteratorType() ) {
                nativeIterators = true;
            } else if ( src->type->isRange() ) {
                rangeBase = true;
            }
        }
        int idx = 0;
        for ( auto & src : sources ) {
            if ( !src->type ) return;
            auto pVar = make_shared<Variable>();
            pVar->name = iterators[idx];
            pVar->at = at;
            if ( src->type->dim.size() ) {
                pVar->type = make_shared<TypeDecl>(*src->type);
                pVar->type->ref = true;
                pVar->type->dim.pop_back();
            } else if ( src->type->isGoodIteratorType() ) {
                pVar->type = make_shared<TypeDecl>(*src->type->firstType);
            } else if ( src->type->isGoodArrayType() ) {
                pVar->type = make_shared<TypeDecl>(*src->type->firstType);
                pVar->type->ref = true;
            } else if ( src->type->isRange() ) {
                pVar->type = make_shared<TypeDecl>(src->type->getRangeBaseType());
                pVar->type->ref = false;
            } else {
                context.error("unsupported iteration type for " + pVar->name, at);
                return;
            }
            context.local.push_back(pVar);
            pVar->stackTop = context.stackTop;
            context.stackTop += (pVar->type->getSizeOf() + 0xf) & ~0xf;
            iteratorVariables.push_back(pVar);
            ++ idx;
        }
        if ( filter ) {
            filter->inferType(context);
            if ( !filter->type ) return;
            if ( !filter->type->isSimpleType(Type::tBool) ) {
                context.error("where clause must be boolean", at);
            }
        }
        subexpr->inferType(context);
        context.func->totalStackSize = max(context.func->totalStackSize, context.stackTop);
        context.stackTop = sp;
        context.local.resize(sz);
    }

    SimNode * ExprFor::simulate (Context & context) const {
        int  total = sources.size();
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
            result->filter = filter ? filter->simulate(context) : nullptr;
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
            result->filter = filter ? filter->simulate(context) : nullptr;
            return result;
        }
    }
    
    // ExprLet
    
    ExpressionPtr ExprLet::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprLet>(expr);
        Expression::clone(cexpr);
        for ( auto & var : variables )
            cexpr->variables.push_back(var->clone());
        cexpr->subexpr = subexpr->clone();
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

    void ExprLet::log(ostream& stream, int depth) const {
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
    
    void ExprLet::setBlockReturnsValue() {
        returnsValue = true;
        subexpr->setBlockReturnsValue();
    }
    
    uint32_t ExprLet::getEvalFlags() const {
        return subexpr->getEvalFlags();
    }
    
    void ExprLet::inferType(InferTypeContext & context) {
        type.reset();
        // infer
        auto sp = context.stackTop;
        auto sz = context.local.size();
        for ( auto & var : variables ) {
            if ( var->type->ref )
                context.error("local variable can't be reference", var->at, CompilationError::invalid_variable_type);
            if ( var->type->isVoid() )
                context.error("local variable can't be reference", var->at, CompilationError::invalid_variable_type);
            context.local.push_back(var);
            if ( var->init ) {
                var->init->inferType(context);
                if ( !var->init->type ) {
                    var->init->inferType(context);
                    return;
                }
                if ( !var->type->isSameType(*var->init->type,false) ) {
                    context.error("variable initialization type mismatch, "
                        + var->type->describe() + " = " + var->init->type->describe(), var->at );
                } else if ( var->type->baseType==Type::tStructure ) {
                    context.error("can't initialize structures", var->at );
                } else if ( !var->init->type->canCopy() && !var->init->type->canMove() ) {
                    context.error("this variable can't be initialized at all", var->at);
                }
            }
            var->stackTop = context.stackTop;
            context.stackTop += (var->type->getSizeOf() + 0xf) & ~0xf;
        }
        subexpr->inferType(context);
        // block type
        if ( returnsValue && subexpr->type ) {
            subexpr = autoDereference(subexpr);
            type = make_shared<TypeDecl>(*subexpr->type);
        }
        context.func->totalStackSize = max(context.func->totalStackSize, context.stackTop);
        context.stackTop = sp;
        context.local.resize(sz);
    }
    
    SimNode * ExprLet::simulateInit(Context & context, const VariablePtr & var, bool local) {
        SimNode * init = var->init->simulate(context);
        SimNode * get;
        if ( local )
            get = context.makeNode<SimNode_GetLocal>(var->init->at, var->stackTop);
        else
            get = context.makeNode<SimNode_GetGlobal>(var->init->at, var->index);
        if ( var->type->canCopy() )
            return makeCopy(var->init->at, context, *var->init->type, get, init);
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
        int vi = 0;
        for ( auto & var : variables ) {
            if ( var->init ) {
                let->list[vi++] = simulateInit(context, var, true);
            } else {
                let->list[vi++] = context.makeNode<SimNode_InitLocal>(at, var->stackTop, var->type->getSizeOf());
            }
        }
        let->subexpr = subexpr->simulate(context);
        return let;
    }
    
    // ExprLooksLikeCall
    
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
    
    void ExprLooksLikeCall::log(ostream& stream, int depth) const {
        logType(stream);
        stream << "(" << name;
        for ( auto & arg : arguments ) {
            stream << " ";
            arg->log(stream, depth);
        }
        stream << ")";
    }
    
    void ExprLooksLikeCall::inferType(InferTypeContext & context) {
        argumentsFailedToInfer = false;
        for ( auto & ar : arguments ) {
            ar->inferType(context);
            if ( !ar->type ) argumentsFailedToInfer = true;
        }
    }
    
    void ExprLooksLikeCall::autoDereference() {
        for ( size_t iA = 0; iA != arguments.size(); ++iA )
            arguments[iA] = Expression::autoDereference(arguments[iA]);
    }
    
    // ExprCall
    
    ExpressionPtr ExprCall::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprCall>(expr);
        ExprLooksLikeCall::clone(cexpr);
        cexpr->func = func;
        return cexpr;
    }
    
    void ExprCall::inferType(InferTypeContext & context) {
        type.reset();
        ExprLooksLikeCall::inferType(context);
        if ( argumentsFailedToInfer ) return;
        // infer
        vector<TypeDeclPtr> types;
        types.reserve(arguments.size());
        for ( auto & ar : arguments ) {
            types.push_back(ar->type);
        }
        auto functions = context.program->findMatchingFunctions(name, types);
        if ( functions.size()==0 ) {
            string candidates = context.program->describeCandidates(context.program->findCandidates(name,types));
            context.error("no matching function " + describe() + "\n" + candidates, at, CompilationError::function_not_found);
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
                    arguments[iA] = Expression::autoDereference(arguments[iA]);
        }
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
    
    TypeAnnotation * Program::findHandle ( const string & name ) const {
        return library.findHandle(name);
    }
    
    StructurePtr Program::findStructure ( const string & name ) const {
        return library.findStructure(name);
    }
    
    VariablePtr Program::findVariable ( const string & name ) const {
        return library.findVariable(name);
    }
    
    FunctionPtr Program::findFunction ( const string & mangledName ) const {
        return library.findFunction(mangledName);
    }
    
    ostream& operator<< (ostream& stream, const Program & program) {
        for ( const auto & st : program.thisModule->structures ) {
            stream << *st.second << "\n";
        }
        if ( program.thisModule->globals.size() ) {
            stream << "(let\n";
            for ( auto & pv : program.thisModule->globals ) {
                stream << "\t(" <<  *pv.second << ")\n";
            }
            stream << ")\n\n";
        }
        for ( const auto & st : program.thisModule->functions ) {
            if ( !st.second->builtIn )
                stream << *st.second << "\n";
        }
        return stream;
    }
    
    void Program::inferTypes() {
        // structure declarations (precompute offsets of fields)
        for ( auto & ist : thisModule->structures ) {
            auto & st = ist.second;
            int offset = 0;
            for ( auto & fi : st->fields ) {
                fi.offset = offset;
                offset += fi.type->getSizeOf();
            }
        }
        // global variables
        int gvi = 0;
        for ( auto & it : thisModule->globals ) {
            auto pvar = it.second;
            pvar->index = gvi ++;
            if ( pvar->init ) {
                Expression::InferTypeContext context;
                pvar->init->inferType(context);
                if ( failed() )
                    return;
            }
        }
        // functions
        totalFunctions = 0;
        for ( auto & fit : thisModule->functions ) {
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
    
    string Program::describeCandidates ( vector<FunctionPtr> result ) const {
        if ( !result.size() ) {
            return "";
        }
        stringstream ss;
        ss << "candidates are:";
        for ( auto & fn : result ) {
            ss << "\n\t" << fn->getMangledName();
        }
        return ss.str();
    }
    
    vector<FunctionPtr> Program::findCandidates ( const string & name, const vector<TypeDeclPtr> & types ) const {
        /*
         TODO:
            arguments by name?
         */
        vector<FunctionPtr> result;
        library.foreach([&](Module * mod) -> bool {
            auto itFnList = mod->functionsByName.find(name);
            if ( itFnList != mod->functionsByName.end() ) {
                auto & goodFunctions = itFnList->second;
                for ( auto & pFn : goodFunctions ) {
                    if ( pFn->arguments.size() >= types.size() ) {
                        result.push_back(pFn);
                    }
                }
            }
            return true;
        });
        return result;
    }
        
    vector<FunctionPtr> Program::findMatchingFunctions ( const string & name, const vector<TypeDeclPtr> & types ) const {
        /*
         TODO:
            arguments by name?
         */
        vector<FunctionPtr> result;
        library.foreach([&](Module * mod) -> bool {
            auto itFnList = mod->functionsByName.find(name);
            if ( itFnList != mod->functionsByName.end() ) {
                auto & goodFunctions = itFnList->second;
                for ( auto & pFn : goodFunctions ) {
                    if ( pFn->arguments.size() >= types.size() ) {
                        bool typesCompatible = true;
                        for ( auto ai = 0; ai != types.size(); ++ai ) {
                            auto & argType = pFn->arguments[ai]->type;
                            auto & passType = types[ai];
                            if ( passType && ((argType->isRef() && !passType->isRef()) || !argType->isSameType(*passType, false, false)) ) {
                                typesCompatible = false;
                                break;
                            }
                            // ref types can only add constness
                            if ( argType->isRef() && !argType->constant && passType->constant ) {
                                typesCompatible = false;
                                break;
                            }
                            // pointer types can only add constant
                            if ( argType->isPointer() && !argType->constant && passType->constant ) {
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
            }
            return true;
        });
        return result;
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
        info->annotation = type->annotation;
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
    
    void Program::simulate ( Context & context ) {
        context.thisProgram = this;
        context.globalVariables = (GlobalVariable *) context.allocate( uint32_t(thisModule->globals.size()*sizeof(GlobalVariable)) );
        for ( auto & it : thisModule->globals ) {
            auto pvar = it.second;
            auto & gvar = context.globalVariables[pvar->index];
            gvar.name = context.allocateName(pvar->name);
            gvar.size = pvar->type->getSizeOf();
            gvar.debug = makeVariableDebugInfo(context, *it.second);
            gvar.value = cast<void *>::from(context.allocate(gvar.size));
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
        context.thisProgram = nullptr;
        context.linearAllocatorExecuteBase = context.linearAllocator;
        context.restart();
        context.runInitScript();
        context.restart();
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
    
    Program::Program() {
        thisModule = make_unique<Module>();
        library.addBuiltInModule();
        library.addModule(thisModule.get());
    }
    
    ExprLooksLikeCall * Program::makeCall ( const LineInfo & at, const string & name ) {
        auto builtIn = static_cast<Module_BuiltIn *>(ModuleLibrary::builtInModule.get());
        auto it = builtIn->callThis.find(name);
        if ( it != builtIn->callThis.end() ) {
            return (it->second)(at);
        } else {
            return new ExprCall(at,name);
        }
    }

    
    // MODULE
    
    Module::Module ( const string & n ) : name(n) {
        if ( !name.empty() ) {
            ModuleLibrary::requireModules[name] = this;
        }
    }
    
    bool Module::addHandle ( unique_ptr<TypeAnnotation> && ptr ) {
        return handleTypes.insert(make_pair(ptr->name, move(ptr))).second;
    }
    
    bool Module::addVariable ( const VariablePtr & var ) {
        return globals.insert(make_pair(var->name, var)).second;
    }
    
    bool Module::addStructure ( const StructurePtr & st ) {
        return structures.insert(make_pair(st->name, st)).second;
    }
    
    bool Module::addFunction ( const FunctionPtr & fn ) {
        auto mangledName = fn->getMangledName();
        if ( functions.insert(make_pair(mangledName, fn)).second ) {
            functionsByName[fn->name].push_back(fn);
            return true;
        } else {
            return false;
        }
    }
    
    VariablePtr Module::findVariable ( const string & name ) const {
        auto it = globals.find(name);
        return it != globals.end() ? it->second : VariablePtr();
    }
    
    FunctionPtr Module::findFunction ( const string & mangledName ) const {
        auto it = functions.find(mangledName);
        return it != functions.end() ? it->second : FunctionPtr();
    }
    
    StructurePtr Module::findStructure ( const string & name ) const {
        auto it = structures.find(name);
        return it != structures.end() ? it->second : StructurePtr();
    }
    
    TypeAnnotation * Module::findHandle ( const string & name ) const {
        auto it = handleTypes.find(name);
        return it != handleTypes.end() ? it->second.get() : nullptr;
    }

    // MODULE LIBRARY
    
    unique_ptr<Module> ModuleLibrary::builtInModule;
    map<string,Module *> ModuleLibrary::requireModules;
    
    void ModuleLibrary::addBuiltInModule () {
        if ( !builtInModule ) {
            builtInModule = make_unique<Module_BuiltIn>();
        }
        addModule(builtInModule.get());
    }
    
    Module * ModuleLibrary::require ( const string & name ) {
        auto it = requireModules.find(name);
        if ( it == requireModules.end() )
            return nullptr;
        return it->second;
    }
    
    void ModuleLibrary::foreach ( function<bool (Module * module)> && func ) const {
        for ( auto pm : modules ) {
            if ( !func(pm) ) break;
        }
    }
    
    TypeAnnotation * ModuleLibrary::findHandle ( const string & name ) const {
        TypeAnnotation * ptr = nullptr;
        foreach([&](Module * pm) -> bool {
            ptr = pm->findHandle(name);
            return !ptr;
        });
        return ptr;
    }
    
    VariablePtr ModuleLibrary::findVariable ( const string & name ) const {
        VariablePtr ptr;
        foreach([&](Module * pm) -> bool {
            ptr = pm->findVariable(name);
            return !ptr;
        });
        return ptr;
    }
    
    FunctionPtr ModuleLibrary::findFunction ( const string & mangledName ) const {
        FunctionPtr ptr;
        foreach([&](Module * pm) -> bool {
            ptr = pm->findFunction(mangledName);
            return !ptr;
        });
        return ptr;
    }
    
    StructurePtr ModuleLibrary::findStructure ( const string & name ) const {
        StructurePtr ptr;
        foreach([&](Module * pm) -> bool {
            ptr = pm->findStructure(name);
            return !ptr;
        });
        return ptr;
    }
    
    TypeDeclPtr ModuleLibrary::makeStructureType ( const string & name ) const {
        auto t = make_shared<TypeDecl>(Type::tStructure);
        t->structType = findStructure(name).get();
        if ( !t->structType ) {
            assert(0 && "can't make structure type");
        }
        return t;
    }
    
    TypeDeclPtr ModuleLibrary::makeHandleType ( const string & name ) const {
        auto t = make_shared<TypeDecl>(Type::tHandle);
        t->annotation = findHandle(name);
        if ( !t->annotation ) {
            assert(0 && "can't make hanlde type");
        }
        return t;
    }
    
    // PARSER
    
    ProgramPtr g_Program;
    
    ProgramPtr parseDaScript ( const char * script ) {
        int err;
        auto program = g_Program = make_shared<Program>();
        yybegin(script);
        err = yyparse();        // TODO: add mutex or make thread safe?
        g_Program = nullptr;
        if ( err || program->failed() ) {
            sort(program->errors.begin(),program->errors.end());
            return program;
        } else {
            program->inferTypes();
            sort(program->errors.begin(),program->errors.end());
            return program;
        }
    }
    
}
