#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/ast/ast_generate.h"
#include "daScript/ast/ast_expressions.h"
#include "daScript/ast/ast_visitor.h"

namespace das {

    /*
        this is VisitorMacro, and not OptimizationMacro. here is why
            we replace one generic with another, which requires additional type inference (to instance new generic etc)
            re-doing type inference on optimized tree will put too much strain on optimization passes (block variable renaming etc)
            so for now this stays as infer-time pass, and would do its function replacement goo there
    */
    class InferBuiltinFunctions : public VisitorMacro {
    public:
        InferBuiltinFunctions ( Module * tm ) : VisitorMacro("InferBuiltinFunctions") , thisModule(tm) {}
    protected:
        /*
            find(tab, key) <| $ ( pvalue )
                if pvalue != null
                    ...
        to
            find_if_exists(tab, key) <| $ ( pvalue )
                ...
        */
        ExpressionPtr replaceFindWithFindIfExists ( ExprCall * call, const string & new_name ) {
            const auto & blockArg = call->arguments[2];
            if (blockArg->rtti_isMakeBlock()) {                                             // check if first arg is block
                auto mkblock = static_pointer_cast<ExprMakeBlock>(blockArg);
                auto block = static_pointer_cast<ExprBlock>(mkblock->block);
                if (block->list.size() == 1 && block->finalList.empty()) {                  // check if block only has one expression
                    if (block->list[0]->rtti_isIfThenElse()) {                              // check if that expression is if_then_else
                        auto ite = static_pointer_cast<ExprIfThenElse>(block->list[0]);
                        if ( !ite->if_false ) {                                             // check if 'else' section is empty
                            if (ite->cond->rtti_isOp2()) {                                  // check if conditon is Op2
                                auto itcond = static_pointer_cast<ExprOp2>(ite->cond);
                                if (itcond->name == "!=") {                                 // check if operator is !=
                                    auto valName = block->arguments[0]->name;
                                                                                            // check if var != null or null != var
                                    if ((isExpressionVariableDeref(itcond->left,valName) && isExpressionNull(itcond->right))
                                    ||  (isExpressionVariableDeref(itcond->right,valName) && isExpressionNull(itcond->left)) ) {
                                        reportFolding();                                    // report that optimization has occured
                                        call->name = new_name;                              // replace find with find_if_exists or find_for_edit with find_for_edit_if_exists
                                        block->list[0] = ite->if_true;                      // replace first expression in the block with if_true portion of the condition
                                        return call;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            return VisitorMacro::visit(call);
        }

        virtual ExpressionPtr visit ( ExprCall * call ) override {
            if (        call->func->fromGeneric                                         // find(tab, key, block<(pvalue?))
                &&      call->func->fromGeneric->module==thisModule                      
                &&      call->name=="find" 
                &&      call->arguments.size()==3
                &&      call->arguments[0]->type->isGoodTableType() ) {
                return replaceFindWithFindIfExists(call,"find_if_exists");
            } else if ( call->func->fromGeneric                                         // find_for_edit(tab, key, block<(pvalue?))
                &&      call->func->fromGeneric->module == thisModule
                &&      call->name == "find_for_edit"
                &&      call->arguments.size() == 3
                &&      call->arguments[0]->type->isGoodTableType()) {
                return replaceFindWithFindIfExists(call, "find_for_edit_if_exists");
            }
            return VisitorMacro::visit(call);
        }
    protected:
        Module * thisModule;
    };

    class OptimizeBuiltinFunctions : public OptimizationMacro {
    public:
        OptimizeBuiltinFunctions ( Module * tm ) : OptimizationMacro("OptimizeBuiltinFunctions") , thisModule(tm) {}
    protected:
        bool isFloat4x4 ( const TypeDeclPtr & td ) const {
            if (    td->isHandle()
                &&  td->annotation
                &&  td->annotation->module==thisModule
                &&  td->annotation->name=="float4x4" ) {
                return true;
            }
            return false;
        }
        bool isTranspose4x4 ( const ExpressionPtr & expr ) const {
            if ( expr->rtti_isCall() ) {
                auto call = static_pointer_cast<ExprCall>(expr);
                if (    call->func->module==thisModule
                &&      call->name=="transpose"
                &&      call->arguments.size()==1
                &&      isFloat4x4(call->arguments[0]->type) ) {
                    return true;
                }
            }
            return false;
        }
        virtual ExpressionPtr visit ( ExprCall * call ) override {
            /*
                float4x4
                    transpose(transpose(x)) = x
             */
            if ( isTranspose4x4(call) ) {
                if ( isTranspose4x4(call->arguments[0]) ) {
                    reportFolding();
                    auto tt = static_pointer_cast<ExprCall>(call->arguments[0]);
                    return tt->arguments[0];
                }
            }
            return OptimizationMacro::visit(call);
        }
        virtual ExpressionPtr visit ( ExprOp2 * op2 ) override {
            if ( !op2->left->noSideEffects && !op2->right->noSideEffects ) {    // side effects on both sides, can't reorder
                return OptimizationMacro::visit(op2);
            }
            /*
                transpose(b) * transpose(a) = transpose(a*b)
             */
            if (    op2->op=="*"
            &&      isTranspose4x4(op2->left)
            &&      isTranspose4x4(op2->right)) {
                reportFolding();
                auto cl = static_pointer_cast<ExprCall>(op2->left);
                auto cr = static_pointer_cast<ExprCall>(op2->right);
                auto b = cl->arguments[0];
                auto a = cr->arguments[0];
                auto rest = cl;
                rest->arguments[0] = op2;
                op2->left = a;
                op2->right = b;
                return rest;
            }
            return OptimizationMacro::visit(op2);
        }
    protected:
        Module * thisModule;
    };

    void Module_BuiltIn::addMacros(ModuleLibrary &) {
        macros.push_back(make_unique<InferBuiltinFunctions>(this));
        optimizationMacros.push_back(make_unique<OptimizeBuiltinFunctions>(this));
    }
}
