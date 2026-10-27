#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_expressions.h"
#include "daScript/ast/ast_const_visitor.h"

namespace das {

    static void constVisitType ( ConstVisitor & vis, const TypeDecl * td ) {
        if ( td->baseType==Type::typeDecl || td->baseType==Type::typeMacro ) {
            for ( size_t i=0, is=td->typeMacroExpr.size(); i!=is; ++i ) {
                if ( td->typeMacroExpr[i] ) {
                    td->typeMacroExpr[i]->visit(vis);
                }
            }
        } else if ( td->baseType==Type::tFixedArray ) {
            if ( td->fixedDim==TypeDecl::dimConst && td->fixedDimExpr ) {
                td->fixedDimExpr->visit(vis);
            }
        }
        if ( td->firstType ) {
            vis.preVisit(td->firstType);
            constVisitType(vis, td->firstType);
        }
        if ( td->secondType ) {
            vis.preVisit(td->secondType);
            constVisitType(vis, td->secondType);
        }
        for ( auto argType : td->argTypes ) {
            vis.preVisit(argType);
            constVisitType(vis, argType);
        }
    }

    static void constVisitTypeFull ( ConstVisitor & vis, const TypeDecl * td ) {
        vis.preVisit(td);
        constVisitType(vis, td);
        vis.visit(td);
    }

    void Expression::visit ( ConstVisitor & ) const {
        DAS_ASSERTF(0, "visit is not implemented for %s", __rtti);
    }

    void ExprReader::visit ( ConstVisitor & vis ) const {
        if ( !vis.canVisitReader(this) ) return;
        vis.preVisit(this);
        vis.visit(this);
    }

    void ExprLabel::visit ( ConstVisitor & vis ) const {
        if ( !vis.canVisitLabel(this) ) return;
        vis.preVisit(this);
        vis.visit(this);
    }

    void ExprGoto::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        if ( subexpr ) {
            subexpr->visit(vis);
        }
        vis.visit(this);
    }

    void ExprRef2Value::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        subexpr->visit(vis);
        vis.visit(this);
    }

    void ExprRef2Ptr::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        subexpr->visit(vis);
        vis.visit(this);
    }

    void ExprPtr2Ref::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        subexpr->visit(vis);
        vis.visit(this);
    }

    void ExprAddr::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        if ( funcType ) {
            constVisitTypeFull(vis, funcType);
        }
        vis.visit(this);
    }

    void ExprNullCoalescing::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        subexpr->visit(vis);
        vis.preVisitNullCoaelescingDefault(this, defaultValue);
        defaultValue->visit(vis);
        vis.visit(this);
    }

    void ExprConstEnumeration::visit ( ConstVisitor & vis ) const {
        vis.preVisit((const ExprConst *)this);
        vis.preVisit(this);
        vis.visit(this);
        vis.visit((const ExprConst *)this);
    }

    void ExprConstString::visit ( ConstVisitor & vis ) const {
        vis.preVisit((const ExprConst *)this);
        vis.preVisit(this);
        vis.visit(this);
        vis.visit((const ExprConst *)this);
    }

    void ExprQuote::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        if ( vis.canVisitQuoteSubexpression(this) ) {
            for ( auto arg : arguments ) {
                vis.preVisitLooksLikeCallArg(this, arg, arg==arguments.back());
                arg->visit(vis);
                vis.visitLooksLikeCallArg(this, arg, arg==arguments.back());
            }
        }
        vis.visit(this);
    }

    void ExprMakeGenerator::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        if ( iterType ) {
            constVisitTypeFull(vis, iterType);
        }
        ExprLooksLikeCall::visit(vis);
        vis.visit(this);
    }

    void ExprYield::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        subexpr->visit(vis);
        vis.visit(this);
    }

    void ExprMakeBlock::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        if ( vis.canVisitMakeBlockBody(this) ) block->visit(vis);
        vis.visit(this);
    }

    void ExprIs::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        subexpr->visit(vis);
        vis.preVisitType(this, typeexpr);
        constVisitTypeFull(vis, typeexpr);
        vis.visit(this);
    }

    void ExprTypeDecl::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        if ( typeexpr ) {
            constVisitTypeFull(vis, typeexpr);
        }
        vis.visit(this);
    }

    void ExprTypeInfo::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        if ( typeexpr ) {
            constVisitTypeFull(vis, typeexpr);
        }
        if ( vis.canVisitExpr(this,subexpr) ) {
            if ( subexpr ) {
                subexpr->visit(vis);
            }
        }
        vis.visit(this);
    }

    void ExprDelete::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        if ( subexpr ) {
            subexpr->visit(vis);
        }
        if ( sizeexpr ) {
            vis.preVisitDeleteSizeExpression(this, sizeexpr);
            sizeexpr->visit(vis);
        }
        vis.visit(this);
    }

    void ExprCast::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        if ( castType ) {
            constVisitTypeFull(vis, castType);
        }
        subexpr->visit(vis);
        vis.visit(this);
    }

    void ExprAscend::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        subexpr->visit(vis);
        vis.visit(this);
    }

    void ExprNew::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        if ( typeexpr ) {
            constVisitTypeFull(vis, typeexpr);
        }
        for ( auto arg : arguments ) {
            vis.preVisitNewArg(this, arg, arg==arguments.back());
            arg->visit(vis);
            vis.visitNewArg(this, arg, arg==arguments.back());
        }
        vis.visit(this);
    }

    void ExprAt::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        subexpr->visit(vis);
        vis.preVisitAtIndex(this, index);
        index->visit(vis);
        vis.visit(this);
    }

    void ExprSafeAt::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        subexpr->visit(vis);
        vis.preVisitSafeAtIndex(this, index);
        index->visit(vis);
        vis.visit(this);
    }

    static void constVisitFinally ( ConstVisitor & vis, const ExprBlock * blk ) {
        if ( !blk->finalList.empty() && !blk->finallyDisabled ) {
            vis.preVisitBlockFinal(blk);
            for ( auto subexpr : blk->finalList ) {
                vis.preVisitBlockFinalExpression(blk, subexpr);
                if ( subexpr ) {
                    subexpr->visit(vis);
                    vis.visitBlockFinalExpression(blk, subexpr);
                }
            }
            vis.visitBlockFinal(blk);
        }
    }

    void ExprBlock::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        if ( isClosure ) {
            for ( auto arg : arguments ) {
                vis.preVisitBlockArgument(this, arg, arg==arguments.back());
                if ( arg->type ) {
                    constVisitTypeFull(vis, arg->type);
                }
                if ( arg->init ) {
                    vis.preVisitBlockArgumentInit(this, arg, arg->init);
                    arg->init->visit(vis);
                    vis.visitBlockArgumentInit(this, arg, arg->init);
                }
                vis.visitBlockArgument(this, arg, arg==arguments.back());
            }
            if ( returnType ) {
                constVisitTypeFull(vis, returnType);
            }
        }
        if ( finallyBeforeBody ) {
            constVisitFinally(vis, this);
        }
        for ( auto subexpr : list ) {
            vis.preVisitBlockExpression(this, subexpr);
            if ( subexpr ) {
                subexpr->visit(vis);
                vis.visitBlockExpression(this, subexpr);
            }
        }
        if ( !finallyBeforeBody ) {
            constVisitFinally(vis, this);
        }
        vis.visit(this);
    }

    void ExprSwizzle::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        value->visit(vis);
        vis.visit(this);
    }

    void ExprField::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        value->visit(vis);
        vis.visit(this);
    }

    void ExprIsVariant::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        value->visit(vis);
        vis.visit(this);
    }

    void ExprAsVariant::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        value->visit(vis);
        vis.visit(this);
    }

    void ExprSafeAsVariant::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        value->visit(vis);
        vis.visit(this);
    }

    void ExprSafeField::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        value->visit(vis);
        vis.visit(this);
    }

    void ExprStringBuilder::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        for ( auto elem : elements ) {
            vis.preVisitStringBuilderElement(this, elem, elem==elements.back());
            elem->visit(vis);
            vis.visitStringBuilderElement(this, elem, elem==elements.back());
        }
        vis.visit(this);
    }

    void ExprVar::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        vis.visit(this);
    }

    void ExprTag::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        if ( subexpr ) {
            subexpr->visit(vis);
        }
        if ( value ) {
            vis.preVisitTagValue(this, value);
            value->visit(vis);
        }
        vis.visit(this);
    }

    void ExprOp1::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        subexpr->visit(vis);
        vis.visit(this);
    }

    void ExprOp2::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        left->visit(vis);
        vis.preVisitRight(this, right);
        right->visit(vis);
        vis.visit(this);
    }

    void ExprOp3::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        subexpr->visit(vis);
        vis.preVisitLeft(this, left);
        left->visit(vis);
        vis.preVisitRight(this, right);
        right->visit(vis);
        vis.visit(this);
    }

    template <typename TT>
    static void constVisitLeftRight ( ConstVisitor & vis, const TT * expr ) {
        vis.preVisit(expr);
        if ( vis.isRightFirst(expr) ) {
            vis.preVisitRight(expr, expr->right);
            expr->right->visit(vis);
            expr->left->visit(vis);
        } else {
            expr->left->visit(vis);
            vis.preVisitRight(expr, expr->right);
            expr->right->visit(vis);
        }
        vis.visit(expr);
    }

    void ExprMove::visit ( ConstVisitor & vis ) const {
        constVisitLeftRight(vis, this);
    }

    void ExprClone::visit ( ConstVisitor & vis ) const {
        constVisitLeftRight(vis, this);
    }

    void ExprCopy::visit ( ConstVisitor & vis ) const {
        constVisitLeftRight(vis, this);
    }

    void ExprTryCatch::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        if ( vis.canVisitTryCatchBody(this) ) {
            try_block->visit(vis);
            vis.preVisitCatch(this,catch_block);
            catch_block->visit(vis);
        }
        vis.visit(this);
    }

    void ExprReturn::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        if ( subexpr ) {
            subexpr->visit(vis);
        }
        vis.visit(this);
    }

    void ExprBreak::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        vis.visit(this);
    }

    void ExprContinue::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        vis.visit(this);
    }

    void ExprIfThenElse::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        cond->visit(vis);
        if ( vis.canVisitIfSubexpr(this) ) {
            vis.preVisitIfBlock(this, if_true);
            if_true->visit(vis);
            if ( if_false ) {
                vis.preVisitElseBlock(this, if_false);
                if_false->visit(vis);
            }
        }
        vis.visit(this);
    }

    void ExprWith::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        if ( with ) with->visit(vis);
        vis.preVisitWithBody(this, body);
        body->visit(vis);
        vis.visit(this);
    }

    void ExprAssume::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        if ( vis.canVisitWithAliasSubexpression(this) ) {
            if ( subexpr ) subexpr->visit(vis);
            if ( assumeType ) constVisitType(vis, assumeType);
        }
        vis.visit(this);
    }

    void ExprUnsafe::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        body->visit(vis);
        vis.visit(this);
    }

    void ExprWhile::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        cond->visit(vis);
        vis.preVisitWhileBody(this, body);
        body->visit(vis);
        vis.visit(this);
    }

    void ExprFor::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        for ( auto var : iteratorVariables ) {
            vis.preVisitFor(this, var, var==iteratorVariables.back());
            vis.visitFor(this, var, var==iteratorVariables.back());
        }
        for ( auto src : sources ) {
            vis.preVisitForSource(this, src, src==sources.back());
            src->visit(vis);
            vis.visitForSource(this, src, src==sources.back());
        }
        vis.preVisitForStack(this);
        if ( body ) {
            vis.preVisitForBody(this, body);
            body->visit(vis);
        }
        vis.visit(this);
    }

    void ExprLet::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        for ( auto var : variables ) {
            vis.preVisitLet(this, var, var==variables.back());
            if ( var->type ) {
                constVisitTypeFull(vis, var->type);
            }
            if ( var->init ) {
                vis.preVisitLetInit(this, var, var->init);
                var->init->visit(vis);
                vis.visitLetInit(this, var, var->init);
            }
            vis.visitLet(this, var, var==variables.back());
        }
        vis.visit(this);
    }

    void ExprCallMacro::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        int index = 0;
        for ( auto arg : arguments ) {
            if ( !macro || macro->canVisitArguments(const_cast<ExprCallMacro *>(this),index) ) {
                vis.preVisitLooksLikeCallArg(this, arg, arg==arguments.back());
                arg->visit(vis);
                vis.visitLooksLikeCallArg(this, arg, arg==arguments.back());
            }
            index ++;
        }
        vis.visit(this);
    }

    void ExprLooksLikeCall::visit ( ConstVisitor & vis ) const {
        if ( vis.canVisitLooksLikeCall(this) ) {
            vis.preVisit(this);
            for ( auto arg : arguments ) {
                if ( vis.canVisitLooksLikeCallArg(this, arg, arg==arguments.back()) ) {
                    vis.preVisitLooksLikeCallArg(this, arg, arg==arguments.back());
                    arg->visit(vis);
                    vis.visitLooksLikeCallArg(this, arg, arg==arguments.back());
                }
            }
            vis.visit(this);
        }
    }

    void ExprCall::visit ( ConstVisitor & vis ) const {
        if ( vis.canVisitCall(this) ) {
            vis.preVisit(this);
            for ( auto arg : arguments ) {
                vis.preVisitCallArg(this, arg, arg==arguments.back());
                arg->visit(vis);
                vis.visitCallArg(this, arg, arg==arguments.back());
            }
            vis.visit(this);
        }
    }

    void ExprNamedCall::visit ( ConstVisitor & vis ) const {
        if ( vis.canVisitNamedCall(this) ) {
            vis.preVisit(this);
            if ( nonNamedArguments.size() > 0 ) {
                ExprCall dummy;
                dummy.gc_unlink();
                for ( auto arg : nonNamedArguments ) {
                    vis.preVisitCallArg(&dummy, arg, arg == nonNamedArguments.back());
                    arg->visit(vis);
                    vis.visitCallArg(&dummy, arg, arg == nonNamedArguments.back());
                }
            }
            if ( arguments ) for ( auto arg : *arguments ) {
                vis.preVisitNamedCallArg(this, arg, arg == arguments->back());
                arg->value->visit(vis);
                vis.visitNamedCallArg(this, arg, arg==arguments->back());
            }
            vis.visit(this);
        }
    }

    void ExprMakeStruct::visit ( ConstVisitor & vis ) const {
        if ( vis.canVisitMakeStructure(this) ) {
            vis.preVisit(this);
            if ( makeType ) {
                constVisitTypeFull(vis, makeType);
            }
            if ( vis.canVisitMakeStructureBody(this) ) {
                for ( int index=0; index != int(structs.size()); ++index ) {
                    vis.preVisitMakeStructureIndex(this, index, index==int(structs.size()-1));
                    auto fields = structs[index];
                    for ( auto field : *fields ) {
                        vis.preVisitMakeStructureField(this, index, field, field==fields->back());
                        field->value->visit(vis);
                        vis.visitMakeStructureField(this, index, field, field==fields->back());
                    }
                    vis.visitMakeStructureIndex(this, index, index==int(structs.size()-1));
                }
            }
            if ( block && vis.canVisitMakeStructureBlock(this, block) ) {
                vis.preVisitMakeStructureBlock(this, block);
                block->visit(vis);
                vis.visitMakeStructureBlock(this, block);
            }
            vis.visit(this);
        }
    }

    void ExprMakeVariant::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        if ( makeType ) {
            constVisitTypeFull(vis, makeType);
        }
        int index = 0;
        for ( auto field : variants ) {
            vis.preVisitMakeVariantField(this, index, field, field==variants.back());
            if ( field->value ) field->value->visit(vis);
            vis.visitMakeVariantField(this, index, field, field==variants.back());
            index ++;
        }
        vis.visit(this);
    }

    void ExprMakeArray::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        if ( makeType ) {
            constVisitTypeFull(vis, makeType);
        }
        if ( vis.canVisitMakeArray(this) ) {
            int index = 0;
            for ( auto value : values ) {
                bool last = index==int(values.size()-1);
                vis.preVisitMakeArrayIndex(this, index, value, last);
                if ( vis.canVisitMakeArrayIndex(this, index, value, last) ) {
                    value->visit(vis);
                }
                vis.visitMakeArrayIndex(this, index, value, last);
                index ++;
            }
        }
        vis.visit(this);
    }

    void ExprMakeTuple::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        if ( makeType ) {
            constVisitTypeFull(vis, makeType);
        }
        int index = 0;
        for ( auto value : values ) {
            vis.preVisitMakeTupleIndex(this, index, value, index==int(values.size()-1));
            value->visit(vis);
            vis.visitMakeTupleIndex(this, index, value, index==int(values.size()-1));
            index ++;
        }
        vis.visit(this);
    }

    void ExprArrayComprehension::visit ( ConstVisitor & vis ) const {
        vis.preVisit(this);
        exprFor->visit(vis);
        vis.preVisitArrayComprehensionSubexpr(this, subexpr);
        subexpr->visit(vis);
        if ( exprWhere ) {
            vis.preVisitArrayComprehensionWhere(this, exprWhere);
            exprWhere->visit(vis);
        }
        vis.visit(this);
    }
}
