#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"

void yybegin(const char * str);
int yyparse();
int yylex_destroy();

namespace das
{
    // annotations

    string Annotation::getMangledName() const {
        return module ? module->name + "::" + name : name;
    }

    // enumeration

    string Enumeration::getMangledName() const {
        return module ? module->name+"::"+name : name;
    }

    pair<int,bool> Enumeration::find ( const string & na ) const {
        auto it = list.find(na);
        return it!=list.end() ? pair<int,bool>(it->second,true) : pair<int,bool>(0,false);
    }

    int Enumeration::find ( const string & na, int def ) const {
        auto it = list.find(na);
        return it!=list.end() ? it->second : def;
    }

    string Enumeration::find ( int va, const string & def ) const {
        auto it = listI.find(va);
        return it!=listI.end() ? it->second : def;
    }

    bool Enumeration::add ( const string & f ) {
        return add(f, lastOne);
    }

    bool Enumeration::add ( const string & f, int v ) {
        auto it = list.find(f);
        if ( it == list.end() ) {
            list[f] = v;
            listI[v] = f;
            lastOne = v + 1;
            return true;
        } else {
            return false;
        }
    }

    // structure

    bool Structure::hasAnyInitializers() const {
        for ( const auto & fd : fields ) {
            if ( fd.init ) return true;
        }
        return false;
    }

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
            int al = fd.type->getAlignOf() - 1;
            size = (size + al) & ~al;
            size += fd.type->getSizeOf();
        }
        int al = getAlignOf() - 1;
        size = (size + al) & ~al;
        return size;
    }

    int Structure::getAlignOf() const {
        int align = 1;
        for ( const auto & fd : fields ) {
            align = max ( fd.type->getAlignOf(), align );
        }
        return align;
    }

    const Structure::FieldDeclaration * Structure::findField ( const string & na ) const {
        for ( const auto & fd : fields ) {
            if ( fd.name==na ) {
                return &fd;
            }
        }
        return nullptr;
    }

    string Structure::getMangledName() const {
        return module ? module->name+"::"+name : name;
    }

    // variable

    VariablePtr Variable::clone() const {
        auto pVar = make_shared<Variable>();
        pVar->name = name;
        pVar->type = make_shared<TypeDecl>(*type);
        if ( init )
            pVar->init = init->clone();
        if ( source )
            pVar->source = source->clone();
        pVar->at = at;
        pVar->flags = flags;
        return pVar;
    }

    string Variable::getMangledName() const {
        string mn = module ? module->name+"::"+name : name;
        return mn + " " + type->getMangledName();
    }

    // function

    FunctionPtr Function::clone() const {
        auto cfun = make_shared<Function>();
        cfun->name = name;
        for ( const auto & arg : arguments ) {
            cfun->arguments.push_back(arg->clone());
        }
        cfun->annotations = annotations;
        cfun->result = make_shared<TypeDecl>(*result);
        cfun->body = body->clone();
        cfun->index = -1;
        cfun->totalStackSize = 0;
        cfun->at = at;
        cfun->module = nullptr;
        cfun->flags = flags;
        cfun->sideEffectFlags = sideEffectFlags;
        cfun->inferStack = inferStack;
        return cfun;
    }

    string Function::getLocationExtra() const {
        if ( !inferStack.size() ) {
            return "";
        }
        TextWriter ss;
        ss << "\nwhile compiling " << describe() << "\n";
        for ( const auto & ih : inferStack ) {
            ss << "instanced from " << ih.func->describe() << " at " << ih.at.describe() << "\n";
        }
        return ss.str();
    }

    string Function::describe() const {
        TextWriter ss;
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
        TextWriter ss;
        // TODO: module name?
        ss << name;
        for ( auto & arg : arguments ) {
            ss << " " << arg->type->getMangledName();
        }
        return ss.str();
    }

    VariablePtr Function::findArgument(const string & na) {
        for ( auto & arg : arguments ) {
            if ( arg->name==na ) {
                return arg;
            }
        }
        return nullptr;
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
        if ( expr->type && !expr->type->isAuto() && expr->type->isRef() && !expr->type->isRefType() ) {
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
            if ( type->isRefType() ) {
                return expr;
            } else {
                return context.code->makeValueNode<SimNode_Ref2Value>(type->baseType, at, expr);
            }
        }
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

    // Const

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

    // ExprStaticAssert

    ExpressionPtr ExprStaticAssert::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprStaticAssert>(expr);
        ExprLooksLikeCall::clone(cexpr);
        return cexpr;
    }

    // ExprAssert

    ExpressionPtr ExprAssert::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprAssert>(expr);
        ExprLooksLikeCall::clone(cexpr);
        return cexpr;
    }

    // ExprDebug

    ExpressionPtr ExprDebug::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprDebug>(expr);
        ExprLooksLikeCall::clone(cexpr);
        return cexpr;
    }

    // ExprMakeBlock

    ExpressionPtr ExprMakeBlock::visit(Visitor & vis) {
        vis.preVisit(this);
        block = block->visit(vis);
        return vis.visit(this);
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

    // ExprErase

    ExpressionPtr ExprErase::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprErase>(expr);
        ExprLooksLikeCall::clone(cexpr);
        return cexpr;
    }

    // ExprFind

    ExpressionPtr ExprFind::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprFind>(expr);
        ExprLooksLikeCall::clone(cexpr);
        return cexpr;
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

    // ExprDelete

    ExpressionPtr ExprDelete::visit(Visitor & vis) {
        vis.preVisit(this);
        if ( subexpr ) {
            subexpr = subexpr->visit(vis);
        }
        return vis.visit(this);
    }

    ExpressionPtr ExprDelete::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprDelete>(expr);
        Expression::clone(cexpr);
        cexpr->subexpr = subexpr->clone();
        return cexpr;
    }

    // ExprNew

    ExpressionPtr ExprNew::visit(Visitor & vis) {
        vis.preVisit(this);
        for ( auto & arg : arguments ) {
            vis.preVisitNewArg(this, arg.get(), arg==arguments.back());
            arg = arg->visit(vis);
            arg = vis.visitNewArg(this, arg.get(), arg==arguments.back());
        }
        return vis.visit(this);
    }

    ExpressionPtr ExprNew::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprNew>(expr);
        ExprLooksLikeCall::clone(cexpr);
        cexpr->typeexpr = typeexpr;
        cexpr->initializer = initializer;
        return cexpr;
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
        if ( !finalList.empty() ) {
            vis.preVisitBlockFinal(this);
            for ( auto it = finalList.begin(); it!=finalList.end(); ) {
                auto & subexpr = *it;
                vis.preVisitBlockFinalExpression(this, subexpr.get());
                subexpr = subexpr->visit(vis);
                if ( subexpr )
                    subexpr = vis.visitBlockFinalExpression(this, subexpr.get());
                if ( subexpr ) ++it; else it = finalList.erase(it);
            }
        }
        return vis.visit(this);
    }

    ExpressionPtr ExprBlock::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprBlock>(expr);
        Expression::clone(cexpr);
        cexpr->list.reserve(list.size());
        for ( auto & subexpr : list ) {
            cexpr->list.push_back(subexpr->clone());
        }
        cexpr->finalList.reserve(finalList.size());
        for ( auto & subexpr : finalList ) {
            cexpr->finalList.push_back(subexpr->clone());
        }
        cexpr->isClosure = isClosure;
        cexpr->inTheLoop = inTheLoop;
        if ( returnType )
            cexpr->returnType = make_shared<TypeDecl>(*returnType);
        for ( auto & arg : arguments ) {
            cexpr->arguments.push_back(arg->clone());
        }
        return cexpr;
    }

    uint32_t ExprBlock::getEvalFlags() const {
        uint32_t flg = getFinallyEvalFlags();
        for ( const auto & ex : list ) {
            flg |= ex->getEvalFlags();
        }
        return flg;
    }

    uint32_t ExprBlock::getFinallyEvalFlags() const {
        uint32_t flg = 0;
        for ( const auto & ex : finalList ) {
            flg |= ex->getEvalFlags();
        }
        return flg;
    }

    VariablePtr ExprBlock::findArgument(const string & name) {
        for ( auto & arg : arguments ) {
            if ( arg->name==name ) {
                return arg;
            }
        }
        return nullptr;
    }

    // ExprSwizzle

    ExpressionPtr ExprSwizzle::visit(Visitor & vis) {
        vis.preVisit(this);
        value = value->visit(vis);
        return vis.visit(this);
    }

    ExpressionPtr ExprSwizzle::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprSwizzle>(expr);
        Expression::clone(cexpr);
        cexpr->mask = mask;
        cexpr->value = value->clone();
        return cexpr;
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

    // ExprTryCatch

    ExpressionPtr ExprTryCatch::visit(Visitor & vis) {
        vis.preVisit(this);
        try_block = try_block->visit(vis);
        vis.preVisitCatch(this,catch_block.get());
        catch_block = catch_block->visit(vis);
        return vis.visit(this);
    }

    uint32_t ExprTryCatch::getEvalFlags() const {
        return try_block->getEvalFlags() | catch_block->getEvalFlags();
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
        cexpr->moveSemantics = moveSemantics;
        return cexpr;
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

    uint32_t ExprIfThenElse::getEvalFlags() const {
        auto flagsE = cond->getEvalFlags() | if_true->getEvalFlags();
        if (if_false) flagsE |= if_false->getEvalFlags();
        return flagsE;
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
        cexpr->inScope = inScope;
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
        TextWriter stream;
        stream << name << " ( ";
        for ( auto & arg : arguments ) {
            if ( arg->type )
                stream << *arg->type;
            else
                stream << "???";
            if (arg != arguments.back()) {
                stream << ", ";
            }
        }
        stream << " )";
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

    // make structure

    ExpressionPtr ExprMakeStructure::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprMakeStructure>(expr);
        Expression::clone(cexpr);
        cexpr->structs.reserve ( structs.size() );
        for ( auto & fields : structs ) {
            auto mfd = make_shared<MakeStruct>();
            mfd->reserve( fields->size() );
            for ( auto & fd : *fields ) {
                auto cfd = make_shared<MakeFieldDecl>();
                cfd->at = fd->at;
                cfd->name = fd->name;
                cfd->value = fd->value->clone();
                mfd->push_back(cfd);
            }
            cexpr->structs.push_back(mfd);
        }
        cexpr->makeType = make_shared<TypeDecl>(*makeType);
        return cexpr;
    }

    ExpressionPtr ExprMakeStructure::visit(Visitor & vis) {
        vis.preVisit(this);
        for ( int index=0; index != int(structs.size()); ++index ) {
            vis.preVisitMakeStructureIndex(this, index, index==int(structs.size()-1));
            auto & fields = structs[index];
            for ( auto it = fields->begin(); it != fields->end(); ) {
                auto & field = *it;
                vis.preVisitMakeStructureField(this, index, field.get(), field==fields->back());
                field->value = field->value->visit(vis);
                if ( field ) {
                    field = vis.visitMakeStructureField(this, index, field.get(), field==fields->back());
                }
                if ( field ) ++it; else it = fields->erase(it);
            }
            vis.visitMakeStructureIndex(this, index, index==int(structs.size()-1));
        }
        return vis.visit(this);
    }

    // make structure

    ExpressionPtr ExprMakeArray::clone( const ExpressionPtr & expr ) const {
        auto cexpr = clonePtr<ExprMakeArray>(expr);
        Expression::clone(cexpr);
        cexpr->values.reserve ( values.size() );
        for ( auto & val : values ) {
            cexpr->values.push_back(val->clone());
        }
        cexpr->makeType = make_shared<TypeDecl>(*makeType);
        return cexpr;
    }

    ExpressionPtr ExprMakeArray::visit(Visitor & vis) {
        vis.preVisit(this);
        int index = 0;
        for ( auto it = values.begin(); it != values.end(); ) {
            auto & value = *it;
            vis.preVisitMakeArrayIndex(this, index, value.get(), index==int(values.size()-1));
            value = value->visit(vis);
            if ( value ) {
                value = vis.visitMakeArrayIndex(this, index, value.get(), index==int(values.size()-1));
            }
            if ( value ) ++it; else it = values.erase(it);
            index ++;
        }
        return vis.visit(this);
    }

    // program

    vector<TypeDeclPtr> Program::findAlias ( const string & name ) const {
        return library.findAlias(name);
    }

    vector<EnumerationPtr> Program::findEnum ( const string & name ) const {
        return library.findEnum(name);
    }

    vector<AnnotationPtr> Program::findAnnotation ( const string & name ) const {
        return library.findAnnotation(name);
    }

    vector<StructurePtr> Program::findStructure ( const string & name ) const {
        return library.findStructure(name);
    }

    VariablePtr Program::findVariable ( const string & name ) const {
        return thisModule->findVariable(name);
    }

    void Program::error ( const string & str, const LineInfo & at, CompilationError cerr ) {
        errors.emplace_back(str,at,cerr);
        failToCompile = true;
    }

    void Program::addModule ( Module * pm ) {
        library.addModule(pm);
    }

    bool Program::addAlias ( const TypeDeclPtr & at ) {
        return thisModule->addAlias(at);
    }

    bool Program::addVariable ( const VariablePtr & var ) {
        return thisModule->addVariable(var);
    }

    bool Program::addStructure ( const StructurePtr & st ) {
        return thisModule->addStructure(st);
    }

    bool Program::addEnumeration ( const EnumerationPtr & st ) {
        return thisModule->addEnumeration(st);
    }

    FunctionPtr Program::findFunction(const string & mangledName) const {
        return thisModule->findFunction(mangledName);
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
        auto enums = findEnum(name);
        auto aliases = findAlias(name);
        if ( ((structs.size()!=0)+(handles.size()!=0)+(enums.size()!=0)+(aliases.size()!=0)) > 1 ) {
            string candidates = describeCandidates(structs);
            candidates += describeCandidates(handles, false);
            candidates += describeCandidates(enums, false);
            candidates += describeCandidates(aliases, false);
            error("undefined type "+name + "\n" + candidates,at,CompilationError::type_not_found);
            return nullptr;
        } else if ( structs.size() ) {
            if ( structs.size()==1 ) {
                auto pTD = new TypeDecl(structs.back());
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
        } else if ( enums.size() ) {
            if ( enums.size()==1 ) {
                auto pTD = new TypeDecl(enums.back());
                pTD->enumType = enums.back();
                pTD->at = at;
                return pTD;
            } else {
                string candidates = describeCandidates(enums);
                error("too many options for "+name + "\n" + candidates,at,CompilationError::enumeration_not_found);
                return nullptr;
            }
        } else if ( aliases.size() ) {
            if ( aliases.size()==1 ) {
                auto pTD = new TypeDecl(*aliases.back());
                return pTD;
            } else {
                string candidates = describeCandidates(aliases);
                error("too many options for "+name + "\n" + candidates,at,CompilationError::type_alias_not_found);
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

    ExpressionPtr Program::makeConst ( const LineInfo & at, const TypeDeclPtr & type, vec4f value ) {
        if ( type->dim.size() || type->ref ) return nullptr;
        switch ( type->baseType ) {
            case Type::tBool:           return make_shared<ExprConstBool>(at, cast<bool>::to(value));
            case Type::tInt64:          return make_shared<ExprConstInt64>(at, cast<int64_t>::to(value));
            case Type::tEnumeration:    return make_shared<ExprConstEnumeration>(at, cast<int32_t>::to(value), type);
            case Type::tInt:            return make_shared<ExprConstInt>(at, cast<int32_t>::to(value));
            case Type::tInt2:           return make_shared<ExprConstInt2>(at, cast<int2>::to(value));
            case Type::tInt3:           return make_shared<ExprConstInt3>(at, cast<int3>::to(value));
            case Type::tInt4:           return make_shared<ExprConstInt4>(at, cast<int4>::to(value));
            case Type::tUInt64:         return make_shared<ExprConstUInt64>(at, cast<uint64_t>::to(value));
            case Type::tUInt:           return make_shared<ExprConstUInt>(at, cast<uint32_t>::to(value));
            case Type::tUInt2:          return make_shared<ExprConstUInt2>(at, cast<uint2>::to(value));
            case Type::tUInt3:          return make_shared<ExprConstUInt3>(at, cast<uint3>::to(value));
            case Type::tUInt4:          return make_shared<ExprConstUInt4>(at, cast<uint4>::to(value));
            case Type::tFloat:          return make_shared<ExprConstFloat>(at, cast<float>::to(value));
            case Type::tFloat2:         return make_shared<ExprConstFloat2>(at, cast<float2>::to(value));
            case Type::tFloat3:         return make_shared<ExprConstFloat3>(at, cast<float3>::to(value));
            case Type::tFloat4:         return make_shared<ExprConstFloat4>(at, cast<float4>::to(value));
            case Type::tDouble:         return make_shared<ExprConstDouble>(at, cast<double>::to(value));
            default:                    assert(0 && "we should not even be here"); return nullptr;
        }
    }

    void Program::visit(Visitor & vis, bool visitGenerics ) {
        // enumerations
        for ( auto & ite : thisModule->enumerations ) {
            auto penum = ite.second.get();
            vis.preVisit(penum);
            size_t count = 0;
            size_t total = penum->list.size();
            for ( auto & itf : penum->list ) {
                vis.preVisitEnumerationValue(penum, itf.first, itf.second, count++==total);
            }
            ite.second = vis.visit(penum);
        }
        // structures
        for ( auto & ist : thisModule->structures ) {
            auto pst = ist.second.get();
            vis.preVisit(pst);
            for ( auto & fi : pst->fields ) {
                vis.preVisitStructureField(pst, fi, &fi==&pst->fields.back());
            }
            ist.second = vis.visit(pst);
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

    void Program::optimize(TextWriter & logs) {
        const bool log = options.getOption("logOptimizationPasses",false);
        bool any, last;
        if (log) {
            logs << *this << "\n";
        }
        do {
            if ( log ) logs << "OPTIMIZE:\n" << *this;
            any = false;
            last = optimizationRefFolding();    if ( failed() ) break;  any |= last;
            if ( log ) logs << "REF FOLDING: " << (last ? "optimized" : "nothing") << "\n" << *this;
            last = optimizationUnused(logs);    if ( failed() ) break;  any |= last;
            if ( log ) logs << "REMOVE UNUSED:" << (last ? "optimized" : "nothing") << "\n" << *this;
            last = optimizationConstFolding();  if ( failed() ) break;  any |= last;
            if ( log ) logs << "CONST FOLDING:" << (last ? "optimized" : "nothing") << "\n" << *this;
            last = optimizationCondFolding();  if ( failed() ) break;  any |= last;
            if ( log ) logs << "COND FOLDING:" << (last ? "optimized" : "nothing") << "\n" << *this;
            last = optimizationBlockFolding();  if ( failed() ) break;  any |= last;
            if ( log ) logs << "BLOCK FOLDING:" << (last ? "optimized" : "nothing") << "\n" << *this;
            // this is here again for a reason
            last = optimizationUnused(logs);    if ( failed() ) break;  any |= last;
            if ( log ) logs << "REMOVE UNUSED:" << (last ? "optimized" : "nothing") << "\n" << *this;
        } while ( any );
    }

    // PARSER

    ProgramPtr g_Program;
    vector<FileInfo *>  g_AccessStack;

    ProgramPtr parseDaScript ( const string & fileName, const FileAccessPtr & access, TextWriter & logs ) {
        int err;
        auto program = g_Program = make_shared<Program>();
        program->access = access;
        g_AccessStack.clear();
        if ( auto fi = g_Program->access->getFileInfo(fileName) ) {
            g_AccessStack.push_back(fi);
            yybegin(fi->source);
        } else {
            g_Program->error(fileName + " not found", LineInfo());
        }
        err = yyparse();        // TODO: add mutex or make thread safe?
        yylex_destroy();
        g_Program.reset();
        if ( err || program->failed() ) {
            sort(program->errors.begin(),program->errors.end());
            return program;
        } else {
            program->inferTypes(logs);
            if ( !program->failed() ) {
                if (program->options.getOption("optimize", true)) {
                    program->optimize(logs);
                } else {
                    program->buildAccessFlags(logs);
                }
                if (!program->failed())
                    program->verifyAndFoldContracts();
                if (!program->failed())
                    program->markOrRemoveUnusedSymbols();
                if (!program->failed())
                    program->allocateStack(logs);
                if (!program->failed())
                    program->finalizeAnnotations();
            }
            if (!program->failed()) {
                if (program->options.getOption("log")) {
                    logs << *program;
                }
                if (program->options.getOption("plot")) {
                    logs << "\n" << program->dotGraph() << "\n";
                }
            }
            sort(program->errors.begin(), program->errors.end());
            return program;
        }
    }
}
