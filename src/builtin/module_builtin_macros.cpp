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
    class OptimizeBuiltinFunctions : public VisitorMacro {
    public:
        OptimizeBuiltinFunctions ( Module * tm ) : VisitorMacro("OptimizeBuiltinFunctions") , thisModule(tm) {}
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
                                        return call->shared_from_this();
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


    void Module_BuiltIn::addMacros(ModuleLibrary &) {
        macros.push_back(make_shared<OptimizeBuiltinFunctions>(this));
    }
}
