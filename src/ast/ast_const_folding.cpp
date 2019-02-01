#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/simulate/debug_print.h"

namespace das {

    void OptVisitor::reportFolding() {
        anyFolding = true;
    }

    class SetSideEffectVisitor : public Visitor {
        // any expression
        virtual void preVisitExpression ( Expression * expr ) override {
            Visitor::preVisitExpression(expr);
            expr->noSideEffects = false;
        }
    };

    class NoSideEffectVisitor : public Visitor {
    protected:
    // const
        virtual void preVisit ( ExprConst * expr ) override {
            Visitor::preVisit(expr);
            expr->noSideEffects = true;
        }
    // sizeof
        virtual void preVisit ( ExprSizeOf * expr ) override {
            Visitor::preVisit(expr);
            expr->noSideEffects = true;
        }
    // typename
        virtual void preVisit ( ExprTypeName * expr ) override {
            Visitor::preVisit(expr);
            expr->noSideEffects = true;
        }
    // find
        virtual void preVisit ( ExprFind * expr ) override {
            Visitor::preVisit(expr);
            expr->noSideEffects = true;
        }
    // hash
        virtual void preVisit ( ExprHash * expr ) override {
            Visitor::preVisit(expr);
            expr->noSideEffects = true;
        }
    // keys
        virtual void preVisit ( ExprKeys * expr ) override {
            Visitor::preVisit(expr);
            expr->noSideEffects = true;
        }
    // values
        virtual void preVisit ( ExprValues * expr ) override {
            Visitor::preVisit(expr);
            expr->noSideEffects = true;
        }
    // variable
        virtual void preVisit ( ExprVar * expr ) override {
            Visitor::preVisit(expr);
            expr->noSideEffects = !expr->write;
        }
    // swizzle
        virtual void preVisit ( ExprSwizzle * expr ) override {
            Visitor::preVisit(expr);
            expr->noSideEffects = !expr->write;
        }
    // field
        virtual void preVisit ( ExprField * expr ) override {
            Visitor::preVisit(expr);
            expr->noSideEffects = !expr->write;
        }
    // safe-field
        virtual void preVisit ( ExprSafeField * expr ) override {
            Visitor::preVisit(expr);
            expr->noSideEffects = !expr->write;
        }
    // at
        virtual void preVisit ( ExprAt * expr ) override {
            Visitor::preVisit(expr);
            expr->noSideEffects = !expr->write;
        }
    // op1
        virtual ExpressionPtr visit ( ExprOp1 * expr ) override {
            expr->noSideEffects = expr->subexpr->noSideEffects && expr->func->noSideEffects;
            return Visitor::visit(expr);
        }
    // op2
        virtual ExpressionPtr visit ( ExprOp2 * expr ) override {
            expr->noSideEffects = expr->left->noSideEffects && expr->right->noSideEffects && expr->func->noSideEffects;
            return Visitor::visit(expr);
        }
    // op3
        virtual ExpressionPtr visit ( ExprOp3 * expr ) override {
            expr->noSideEffects = expr->subexpr->noSideEffects && expr->left->noSideEffects && expr->right->noSideEffects;
            return Visitor::visit(expr);
        }
    // call
        virtual ExpressionPtr visit ( ExprCall * expr ) override {
            expr->noSideEffects = expr->func->noSideEffects;
            if ( expr->noSideEffects ) {
                for ( auto & arg : expr->arguments ) {
                    expr->noSideEffects &= arg->noSideEffects;
                }
            }
            return Visitor::visit(expr);
        }
    };

    class FoldingVisitor : public OptVisitor {
    public:
        FoldingVisitor( const ProgramPtr & prog ) : ctx(nullptr) {
            program = prog;
        }
    protected:
        Context         ctx;
        ProgramPtr      program;
    protected:
        vec4f eval ( Expression * expr, bool & failed ) {
            ctx.simEnd();
            ctx.restart();
            auto node = expr->simulate(ctx);
            ctx.simEnd();
            ctx.restart();
            vec4f result = ctx.evalWithCatch(node);
            if ( ctx.getException() ) {
                failed = true;
                return v_zero();
            } else {
                failed = false;
                return result;
            }
        }
        ExpressionPtr evalAndFold ( Expression * expr ) {
            if ( expr->type->baseType == Type::tString ) return evalAndFoldString(expr);
            if ( expr->rtti_isConstant() ) return expr->shared_from_this();
            bool failed;
            vec4f value = eval(expr, failed);
            if ( !failed ) {
                auto sim = Program::makeConst(expr->at, expr->type, value);
                sim->type = make_shared<TypeDecl>(*expr->type);
                sim->constexpression = true;
                reportFolding();
                return sim;
            } else {
                return expr->shared_from_this();
            }
        }
        ExpressionPtr evalAndFoldString ( Expression * expr ) {
            if ( expr->rtti_isStringConstant() ) return expr->shared_from_this();
            bool failed;
            vec4f value = eval(expr, failed);
            if ( !failed ) {
                DebugInfoHelper helper(ctx.debugInfo);
                auto pTypeInfo = helper.makeTypeInfo(nullptr,expr->type);
                auto res = debug_value(value, pTypeInfo, PrintFlags::string_builder);
                auto sim = make_shared<ExprConstString>(expr->at, res);
                sim->type = make_shared<TypeDecl>(Type::tString);
                sim->constexpression = true;
                reportFolding();
                return sim;
            } else {
                return expr->shared_from_this();
            }
        }
        bool isSameFoldValue ( const TypeDeclPtr & t, vec4f a, vec4f b ) const {
            return memcmp(&a,&b,t->getSizeOf()) == 0;
        }
    };

    /*
        op1 constexpr = op1(constexpr)
        op2 constexpr,constexpr = op2(constexpr,constexpr)
        op3 constexpr,a,b = constexpr ? a or b
        op3 cond,a,a = a
        if ( constexpr ) a; else b;    =   constexpr ? a : b
        constexpr; = nop
        assert(true,...) = nop
        hash(x) = ...
        sizeof(...) -> const
        typename(...) -> const
        no_side_effec_builtin(const...) -> const
        stringbuilder(const1,const2,...) -> stringbuilder(const12,...)
        stringbuilder(const) -> const
        def FN return const -> const
     */
    class ConstFolding : public FoldingVisitor {
    public:
        ConstFolding( const ProgramPtr & prog ) : FoldingVisitor(prog) {}
        bool needRun() const { return runMe; }
    protected:
        bool runMe = false;
    protected:
        // function which is fully a nop
        bool isNop ( const FunctionPtr & func ) {
            if ( func->builtIn ) return false;
            if ( func->body->rtti_isBlock() ) {
                auto block = static_pointer_cast<ExprBlock>(func->body);
                if ( block->list.size()==0 ) {
                    return true;
                }
            }
            return false;
        }
        // function which is 'return const'
        ExpressionPtr getSimpleConst ( const FunctionPtr & func ) {
            if ( func->builtIn ) return nullptr;
            if ( func->body->rtti_isBlock() ) {
                auto block = static_pointer_cast<ExprBlock>(func->body);
                if ( block->list.size()==1 ) {
                    if ( block->list.back()->rtti_isReturn() ) {
                        auto ret = static_pointer_cast<ExprReturn>(block->list.back());
                        if ( ret->subexpr && ret->subexpr->rtti_isConstant() ) {
                            return ret->subexpr->clone();
                        }
                    }
                }
            }
            return nullptr;
        }
    protected:
    // swizzle
        virtual ExpressionPtr visit ( ExprSwizzle * expr ) override {
            if ( expr->type->baseType == expr->value->type->baseType ) {
                if ( expr->fields[0]==0 && TypeDecl::isSequencialMask(expr->fields) ) {
                    if ( !expr->r2v ) {
                        return expr->value;
                    }
                }
            }
            return Visitor::visit(expr);
        }
    // op1
        virtual ExpressionPtr visit ( ExprOp1 * expr ) override {
            if ( expr->type->isFoldable() && expr->subexpr->constexpression ) {
                return evalAndFold(expr);
            }
            return Visitor::visit(expr);
        }
    // op2
        virtual ExpressionPtr visit ( ExprOp2 * expr ) override {
            if ( expr->left->constexpression && expr->right->constexpression ) {
                if ( expr->type->isFoldable() ) {
                    return evalAndFold(expr);
                }
            }
            return Visitor::visit(expr);
        }
    // op3
        virtual ExpressionPtr visit ( ExprOp3 * expr ) override {
            if ( expr->type->isFoldable() && expr->subexpr->constexpression && expr->left->constexpression && expr->right->constexpression ) {
                return evalAndFold(expr);
            } else if ( expr->type->isFoldable() && expr->subexpr->noSideEffects && expr->left->constexpression && expr->right->constexpression ) {
                bool failed;
                vec4f left = eval(expr->left.get(), failed);
                if ( failed ) return Visitor::visit(expr);
                vec4f right = eval(expr->right.get(), failed);
                if ( failed ) return Visitor::visit(expr);
                if ( isSameFoldValue(expr->type, left, right) ) {
                    reportFolding();
                    return expr->left->clone();
                }
            } else if ( expr->subexpr->constexpression ) {
                bool failed;
                vec4f resB = eval(expr->subexpr.get(), failed);
                if ( failed ) return Visitor::visit(expr);
                bool res = cast<bool>::to(resB);
                reportFolding();
                return res ? expr->left : expr->right;
            }
            return Visitor::visit(expr);
        }
    // ExprIfThenElse
        virtual ExpressionPtr visit ( ExprIfThenElse * expr ) override {
            if ( expr->cond->constexpression ) {
                bool failed;
                vec4f resB = eval(expr->cond.get(), failed);
                if ( failed ) return Visitor::visit(expr);
                bool res = cast<bool>::to(resB);
                reportFolding();
                return res ? expr->if_true : expr->if_false;
            }
            if ( expr->cond->noSideEffects ) {
                if ( expr->if_false ) {
                    auto ifeb = static_pointer_cast<ExprBlock>(expr->if_false);
                    if ( !ifeb->list.size() ) {
                        expr->if_false = nullptr;
                        reportFolding();
                    }
                    auto ifb = static_pointer_cast<ExprBlock>(expr->if_true);
                    if ( !ifb->list.size() && !ifeb->list.size() ) {
                        reportFolding();
                        return nullptr;
                    }
                }
            }
            return Visitor::visit(expr);
        }
    // block
        virtual ExpressionPtr visitBlockExpression (  ExprBlock * block, Expression * expr ) override {
            if ( expr->constexpression ) {  // top level constant expresson like 4;
                reportFolding();
                return nullptr;
            }
            if ( expr->noSideEffects ) {    // top level expressions like a + 5;
                reportFolding();
                return nullptr;
            }
            return Visitor::visitBlockExpression(block, expr);
        }
        virtual ExpressionPtr visit ( ExprFor * expr ) override {
            if ( expr->subexpr->rtti_isBlock()) {
                auto block = static_pointer_cast<ExprBlock>(expr->subexpr);
                if ( !block->list.size() ) {
                    bool noSideEffects = true;
                    for ( auto & src : expr->sources ) {
                        noSideEffects &= src->noSideEffects;
                    }
                    if ( noSideEffects ) {
                        reportFolding();
                        return nullptr;
                    }
                }
            }
            return Visitor::visit(expr);
        }
    // const
        virtual ExpressionPtr visit ( ExprConst * c ) override {
            c->constexpression = true;
            return Visitor::visit(c);
        }
    // assert
        virtual ExpressionPtr visit ( ExprAssert * expr ) override {
            if ( expr->arguments[0]->constexpression ) {
                bool failed;
                vec4f resB = eval(expr->arguments[0].get(), failed);
                if ( failed ) return Visitor::visit(expr);
                bool res = cast<bool>::to(resB);
                if ( res ) {
                    reportFolding();
                    return nullptr;
                }
            }
            return Visitor::visit(expr);
        }
    // hash
        virtual ExpressionPtr visit ( ExprHash * expr ) override {
            if ( expr->arguments[0]->constexpression ) {
                return evalAndFold(expr);
            }
            return Visitor::visit(expr);
        }
    // sizeof
        virtual ExpressionPtr visit ( ExprSizeOf * expr ) override {
            reportFolding();
            auto ci = make_shared<ExprConstInt>(expr->at, expr->typeexpr->getSizeOf());
            ci->type = make_shared<TypeDecl>(Type::tInt);
            return ci;
        }
    // typename
        virtual ExpressionPtr visit ( ExprTypeName * expr ) override {
            reportFolding();
            auto cs = make_shared<ExprConstString>(expr->at, expr->typeexpr->describe(false));
            cs->type = make_shared<TypeDecl>(Type::tString);
            return cs;
        }
    // ExprCall
        virtual ExpressionPtr visit ( ExprCall * expr ) override {
            bool allNoSideEffects = true;
            for ( auto & arg : expr->arguments ) {
                if ( arg->type->baseType!=Type::fakeContext )
                    allNoSideEffects &= arg->noSideEffects;
            }
            if ( allNoSideEffects ) {
                if ( isNop(expr->func) ) {
                    reportFolding();
                    return nullptr;
                }
                if ( auto sc = getSimpleConst(expr->func) ) {
                    reportFolding();
                    return sc;
                }
            }
            if ( expr->func->result->isFoldable() && expr->func->noSideEffects ) {
                auto allConst = true;
                for ( auto & arg : expr->arguments ) {
                    if ( arg->type->baseType!=Type::fakeContext )
                        allConst &= arg->constexpression;
                }
                if ( allConst ) {
                    if ( expr->func->builtIn ) {
                        return evalAndFold(expr);
                    } else {
                        runMe = true;
                    }
                }
            }
            return Visitor::visit(expr);
        }
    // ExprStringBuilder
        virtual ExpressionPtr visitStringBuilderElement ( ExprStringBuilder * sb, Expression * expr, bool last ) override {
            if ( expr->constexpression ) {
                return evalAndFoldString(expr);
            }
            return Visitor::visitStringBuilderElement(sb, expr, last);
        }
        virtual ExpressionPtr visit ( ExprStringBuilder * expr ) override {
            // concatinate all constant strings, which are close together
            shared_ptr<ExprConstString> str;
            for ( auto it = expr->elements.begin(); it != expr->elements.end(); ) {
                const auto & elem = *it;
                if ( elem->rtti_isStringConstant() ) {
                    auto selem = static_pointer_cast<ExprConstString>(elem);
                    if ( str ) {
                        str->text += selem->text;
                        it = expr->elements.erase(it);
                        reportFolding();
                    } else {
                        str = selem;
                        ++ it;
                    }
                } else {
                    str.reset();
                    ++ it;
                }
            }
            // check if we are no longer a builder
            if ( expr->elements.size()==0 ) {
                // empty string builder is "" string
                auto estr = make_shared<ExprConstString>(expr->at,"");
                estr->type = make_shared<TypeDecl>(Type::tString);
                estr->constexpression = true;
                return estr;
            } else if ( expr->elements.size()==1 && expr->elements[0]->rtti_isStringConstant() ) {
                // string builder with one string constant is that constant
                return expr->elements[0];
            } else {
                return Visitor::visit(expr);
            }
        }
    };

    //  turn static-assert into nop
    //  fail if condition is not there
    class StaticAssertFolding : public FoldingVisitor {
    public:
        StaticAssertFolding( const ProgramPtr & prog ) : FoldingVisitor(prog) {}
    protected:
        FunctionPtr             func;
    protected:
        virtual void preVisit ( Function * f ) override {
            Visitor::preVisit(f);
            func = f->shared_from_this();
        }
        virtual FunctionPtr visit ( Function * that ) override {
            func.reset();
            return Visitor::visit(that);
        }
        virtual ExpressionPtr visit(ExprStaticAssert * expr) override {
            auto cond = expr->arguments[0];
            if (!cond->constexpression && !cond->rtti_isConstant()) {
                program->error("static assert condition is not constexpr or const", expr->at);
                return nullptr;
            }
            bool result = false;
            if (cond->constexpression) {
                bool failed;
                vec4f resB = eval(cond.get(), failed);
                if ( failed ) {
                    program->error("exception while computing static assert condition", expr->at);
                }
                result = cast<bool>::to(resB);
            } else {
                result = ((ExprConstBool *)cond.get())->getValue();
            }
            if ( !result ) {
                string message;
                if ( expr->arguments.size()==2 ) {
                    bool failed;
                    vec4f resM = eval(expr->arguments[1].get(), failed);
                    if ( failed ) {
                        program->error("exception while computing static assert message", expr->at);
                        message = "";
                    } else {
                        message = cast<char *>::to(resM);
                    }
                } else {
                    message = "static assert failed";
                }
                if ( func ) {
                    message += func->getLocationExtra();
                }
                program->error(message, expr->at, CompilationError::static_assert_failed);
            }
            return cond->constexpression ? nullptr : Visitor::visit(expr);
        }
    };
    
    class RunFolding : public FoldingVisitor {
    public:
        RunFolding( const ProgramPtr & prog ) : FoldingVisitor(prog) {
            TextWriter dummy;
            program->markOrRemoveUnusedSymbols(true);
            assert ( !program->failed() && "internal error while folding (remove unused)?" );
            program->allocateStack(dummy);
            assert ( !program->failed() && "internal error while folding (allocate stack)?" );
            program->simulate(ctx, dummy);
            assert ( !program->failed() && "internal error while folding (simulate)?" );
        }
    protected:
        // ExprCall
        virtual ExpressionPtr visit ( ExprCall * expr ) override {
            bool allNoSideEffects = true;
            for ( auto & arg : expr->arguments ) {
                if ( arg->type->baseType!=Type::fakeContext )
                    allNoSideEffects &= arg->noSideEffects;
            }
            if ( expr->func->result->isFoldable() && expr->func->noSideEffects && !expr->func->builtIn ) {
                auto allConst = true;
                for ( auto & arg : expr->arguments ) {
                    if ( arg->type->baseType!=Type::fakeContext )
                        allConst &= arg->constexpression;
                }
                if ( allConst ) {
                    assert ( expr->func->index!=-1 );
                    return evalAndFold(expr);
                }
            }
            return Visitor::visit(expr);
        }
    };

    // program

    bool Program::optimizationConstFolding() {
        SetSideEffectVisitor sse;
        visit(sse);
        NoSideEffectVisitor nse;
        visit(nse);
        ConstFolding cfe(shared_from_this());
        visit(cfe);
        bool any = cfe.didAnything();
        if ( cfe.needRun() ) {
            RunFolding rfe(shared_from_this());
            visit(rfe);
            any |= rfe.didAnything();
        }
        return any;
    }

    bool Program::staticAsserts() {
        StaticAssertFolding context(shared_from_this());
        visit(context);
        return context.didAnything();
    }
}

