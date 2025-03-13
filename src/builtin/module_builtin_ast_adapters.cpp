#include "daScript/misc/platform.h"

#include "module_builtin_rtti.h"

#include "daScript/simulate/simulate_visit_op.h"
#include "daScript/ast/ast_policy_types.h"
#include "daScript/ast/ast_expressions.h"
#include "daScript/ast/ast_generate.h"
#include "daScript/ast/ast_visitor.h"
#include "daScript/simulate/aot_builtin_ast.h"
#include "daScript/simulate/aot_builtin_string.h"
#include "daScript/misc/performance_time.h"

#include "module_builtin_ast.h"

namespace das {

    void runMacroFunction ( Context * context, const string & message, const callable<void()> & subexpr ) {
        auto timeM = ref_time_ticks();
        if ( !context->runWithCatch(subexpr) ) {
            DAS_ASSERTF(daScriptEnvironment::bound->g_Program, "calling macros while not compiling a program");
            daScriptEnvironment::bound->g_Program->error(
                "macro caused exception during " + message,
                context->getException(), "",
                context->exceptionAt,
                CompilationError::exception_during_macro
            );
            daScriptEnvironment::bound->g_Program->macroException = true;
        }
        daScriptEnvironment::bound->macroTimeTicks += ref_time_ticks() - timeM;
    }

    VisitorAdapter::VisitorAdapter(char *pClass, const StructInfo *info, Context *ctx)
        : AstVisitor_Adapter(info), classPtr(pClass), context(ctx) {}

    bool VisitorAdapter::canVisitStructure(Structure *st) {
        if ( auto fnCanVisit = get_canVisitStructure(classPtr) ) {
            bool result = true;
            runMacroFunction(context, "canVisitStructure", [&]() {
                result = invoke_canVisitStructure(context,fnCanVisit,classPtr,st);
            });
            return result;
        } else {
            return true;
        }
    }

    bool VisitorAdapter::canVisitGlobalVariable(Variable *fun) {
        if ( auto fnCanVisit = get_canVisitGlobalVariable(classPtr) ) {
            bool result = true;
            runMacroFunction(context, "canVisitGlobalVariable", [&]() {
                result = invoke_canVisitGlobalVariable(context,fnCanVisit,classPtr,fun);
            });
            return result;
        } else {
            return true;
        }
    }

    bool VisitorAdapter::canVisitFunction(Function *fun) {
        if ( auto fnCanVisit = get_canVisitFunction(classPtr) ) {
            bool result = true;
            runMacroFunction(context, "canVisitFunction", [&]() {
                result = invoke_canVisitFunction(context,fnCanVisit,classPtr,fun);
            });
            return result;
        } else {
            return true;
        }
    }

    bool VisitorAdapter::canVisitEnumeration(Enumeration *en) {
        if ( auto fnCanVisit = get_canVisitEnumeration(classPtr) ) {
            bool result = true;
            runMacroFunction(context, "canVisitEnumeration", [&]() {
                result = invoke_canVisitEnumeration(context,fnCanVisit,classPtr,en);
            });
            return result;
        } else {
            return true;
        }
    }

    bool VisitorAdapter::canVisitMakeStructureBlock(ExprMakeStruct *expr, Expression *blk) {
        if ( auto fnCanVisit = get_canVisitMakeStructBlock(classPtr) ) {
            bool result = true;
            runMacroFunction(context, "canVisitMakeStructureBlock", [&]() {
                result = invoke_canVisitMakeStructBlock(context,fnCanVisit,classPtr,expr,blk);
            });
            return result;
        } else {
            return true;
        }
    }

    bool VisitorAdapter::canVisitMakeStructureBody(ExprMakeStruct *expr) {
        if ( auto fnCanVisit = get_canVisitMakeStructBody(classPtr) ) {
            bool result = true;
            runMacroFunction(context, "canVisitMakeStructureBody", [&]() {
                result = invoke_canVisitMakeStructBody(context,fnCanVisit,classPtr,expr);
            });
            return result;
        } else {
            return true;
        }
    }

    bool VisitorAdapter::canVisitArgumentInit(Function *fun, const VariablePtr &var, Expression *init) {
        if ( auto fnCanVisit = get_canVisitFunctionArgumentInit(classPtr) ) {
            bool result = true;
            runMacroFunction(context, "canVisitArgumentInit", [&]() {
                result = invoke_canVisitFunctionArgumentInit(context,fnCanVisit,classPtr,fun,var,init);
            });
            return result;
        } else {
            return true;
        }
    }

    bool VisitorAdapter::canVisitWithAliasSubexpression(ExprAssume *expr) {
        if ( auto fnCanVisit = get_canVisitWithAliasSubexpression(classPtr) ) {
            bool result = true;
            runMacroFunction(context, "canVisitWithAliasSubexpression", [&]() {
                result = invoke_canVisitWithAliasSubexpression(context,fnCanVisit,classPtr,expr);
            });
            return result;
        } else {
            return true;
        }
    }

    bool VisitorAdapter::canVisitMakeBlockBody(ExprMakeBlock *expr) {
        if ( auto fnCanVisit = get_canVisitMakeBlockBody(classPtr) ) {
            bool result = true;
            runMacroFunction(context, "canVisitMakeBlockBody", [&]() {
                result = invoke_canVisitMakeBlockBody(context,fnCanVisit,classPtr,expr);
            });
            return result;
        } else {
            return true;
        }
    }

    bool VisitorAdapter::canVisitCall(ExprCall *expr) {
        if ( auto fnCanVisit = get_canVisitCall(classPtr) ) {
            bool result = true;
            runMacroFunction(context, "canVisitCall", [&]() {
                result = invoke_canVisitCall(context,fnCanVisit,classPtr,expr);
            });
            return result;
        } else {
            return true;
        }
    }

    void VisitorAdapter::preVisitProgram(Program *prog) {
        if ( auto fnPreVisit = get_preVisitProgram(classPtr) ) {
            runMacroFunction(context, "preVisitProgram", [&]() {
                invoke_preVisitProgram(context,fnPreVisit,classPtr,prog);
            });
        }
    }

    void VisitorAdapter::visitProgram(Program *prog) {
        if ( auto fnVisit = get_visitProgram(classPtr) ) {
            runMacroFunction(context, "visitProgram", [&]() {
                invoke_visitProgram(context,fnVisit,classPtr,prog);
            });
        }
    }

    void VisitorAdapter::preVisitModule(Module *mod) {
        if ( auto fnPreVisit = get_preVisitModule(classPtr) ) {
            runMacroFunction(context, "preVisitModule", [&]() {
                invoke_preVisitModule(context,fnPreVisit,classPtr,mod);
            });
        }
    }

    void VisitorAdapter::visitModule(Module *mod) {
        if ( auto fnVisit = get_visitModule(classPtr) ) {
            runMacroFunction(context, "visitModule", [&]() {
                invoke_visitModule(context,fnVisit,classPtr,mod);
            });
        }
    }

    void VisitorAdapter::preVisit(TypeDecl *td) {
        if ( auto fnPreVisit = get_preVisitTypeDecl(classPtr) ) {
            runMacroFunction(context, "preVisit", [&]() {
                invoke_preVisitTypeDecl(context,fnPreVisit,classPtr,td);
            });
        }
    }

    TypeDeclPtr VisitorAdapter::visit(TypeDecl *td) {
        if ( auto fnVisit = get_visitTypeDecl(classPtr) ) {
            TypeDeclPtr result;
            runMacroFunction(context, "visit", [&]() {
                result = invoke_visitTypeDecl(context,fnVisit,classPtr,td);
            });
            return return_smart(result,td);
        } else {
            return td;
        }
    }

    void VisitorAdapter::preVisitAlias(TypeDecl *td, const string &name) {
        if ( auto fnPreVisit = get_preVisitAlias(classPtr) ) {
            runMacroFunction(context, "preVisitAlias", [&]() {
                invoke_preVisitAlias(context,fnPreVisit,classPtr,td,name);
            });
        }
    }

    TypeDeclPtr VisitorAdapter::visitAlias(TypeDecl *td, const string &name) {
        if ( auto fnVisit = get_visitAlias(classPtr) ) {
            TypeDeclPtr result;
            runMacroFunction(context, "visitAlias", [&]() {
                result = invoke_visitAlias(context,fnVisit,classPtr,td,name);
            });
            return return_smart(result,td);
        } else {
            return td;
        }
    }

    void VisitorAdapter::preVisit(Enumeration *enu) {
        if ( auto fnPreVisit = get_preVisitEnumeration(classPtr) ) {
            runMacroFunction(context, "preVisit", [&]() {
                invoke_preVisitEnumeration(context,fnPreVisit,classPtr,enu);
            });
        }
    }

    void VisitorAdapter::preVisitEnumerationValue(Enumeration *enu, const string &name, Expression *value, bool last) {
        if ( auto fnPreVisit = get_preVisitEnumerationValue(classPtr) ) {
            runMacroFunction(context, "preVisitEnumerationValue", [&]() {
                invoke_preVisitEnumerationValue(context,fnPreVisit,classPtr,enu,name,value,last);
            });
        }
    }

    ExpressionPtr VisitorAdapter::visitEnumerationValue(Enumeration *enu, const string &name, Expression *value, bool last) {
        if ( auto fnVisit = get_visitEnumerationValue(classPtr) ) {
            ExpressionPtr result;
            runMacroFunction(context, "visitEnumerationValue", [&]() {
                result = invoke_visitEnumerationValue(context,fnVisit,classPtr,enu,name,value,last);
            });
            return return_smart(result,value);
        } else {
            return value;
        }
    }

    void VisitorAdapter::preVisit(Structure *var) {
        if ( auto fnPreVisit = get_preVisitStructure(classPtr) ) {
            runMacroFunction(context, "preVisit", [&]() {
                invoke_preVisitStructure(context,fnPreVisit,classPtr,var);
            });
        }
    }

    EnumerationPtr VisitorAdapter::visit(Enumeration *enu) {
        if ( auto fnVisit = get_visitEnumeration(classPtr) ) {
            EnumerationPtr result;
            runMacroFunction(context, "visit", [&]() {
                result = invoke_visitEnumeration(context,fnVisit,classPtr,enu);
            });
            return return_smart(result,enu);
        } else {
            return enu;
        }
    }

    void VisitorAdapter::preVisitStructureField(Structure *var, Structure::FieldDeclaration &decl, bool last) {
        if ( auto fnPreVisit = get_preVisitStructureField(classPtr) ) {
            runMacroFunction(context, "preVisitStructureField", [&]() {
                invoke_preVisitStructureField(context,fnPreVisit,classPtr,var,decl,last);
            });
        }
    }

    void VisitorAdapter::visitStructureField(Structure *var, Structure::FieldDeclaration &decl, bool last) {
        if ( auto fnVisit = get_visitStructureField(classPtr) ) {
            runMacroFunction(context, "visitStructureField", [&]() {
                invoke_visitStructureField(context,fnVisit,classPtr,var,decl,last);
            });
        }
    }

    StructurePtr VisitorAdapter::visit(Structure *var) {
        if ( auto fnVisit = get_visitStructure(classPtr) ) {
            StructurePtr result;
            runMacroFunction(context, "visit", [&]() {
                result = invoke_visitStructure(context,fnVisit,classPtr,var);
            });
            return return_smart(result,var);
        } else {
            return var;
        }
    }

    void VisitorAdapter::preVisitProgramBody(Program *prog, Module *mod) {
        if ( auto fnPreVisit = get_preVisitProgramBody(classPtr) ) {
            runMacroFunction(context, "preVisitProgramBody", [&]() {
                invoke_preVisitProgramBody(context,fnPreVisit,classPtr,prog,mod);
            });
        }
    }
    void VisitorAdapter::preVisit(Function *that) {
        if ( auto fnPreVisit = get_preVisitFunction(classPtr) ) {
            runMacroFunction(context, "preVisit", [&]() {
                invoke_preVisitFunction(context,fnPreVisit,classPtr,that);
            });
        }
    }

    FunctionPtr VisitorAdapter::visit(Function *that) {
        if ( auto fnVisit = get_visitFunction(classPtr) ) {
            FunctionPtr result;
            runMacroFunction(context, "visit", [&]() {
                result = invoke_visitFunction(context,fnVisit,classPtr,that);
            });
            return return_smart(result,that);
        } else {
            return that;
        }
    }

    void VisitorAdapter::preVisitArgument(Function *fn, const VariablePtr &var, bool lastArg) {
        if ( auto fnPreVisit = get_preVisitFunctionArgument(classPtr) ) {
            runMacroFunction(context, "preVisitArgument", [&]() {
                invoke_preVisitFunctionArgument(context,fnPreVisit,classPtr,fn,var,lastArg);
            });
        }
    }

    VariablePtr VisitorAdapter::visitArgument(Function *fn, const VariablePtr &that, bool lastArg) {
        if ( auto fnVisit = get_visitFunctionArgument(classPtr) ) {
            VariablePtr result;
            runMacroFunction(context, "visitArgument", [&]() {
                result = invoke_visitFunctionArgument(context,fnVisit,classPtr,fn,that,lastArg);
            });
            return return_smart(result,that.get());
        } else {
            return that;
        }
    }

    void VisitorAdapter::preVisitArgumentInit(Function *fn, const VariablePtr &var, Expression *init) {
        if ( auto fnPreVisit = get_preVisitFunctionArgumentInit(classPtr) ) {
            runMacroFunction(context, "preVisitArgumentInit", [&]() {
                invoke_preVisitFunctionArgumentInit(context,fnPreVisit,classPtr,fn,var,init);
            });
        }
    }

    ExpressionPtr VisitorAdapter::visitArgumentInit(Function *fn, const VariablePtr &var, Expression *that) {
        if ( auto fnVisit = get_visitFunctionArgumentInit(classPtr) ) {
            ExpressionPtr result;
            runMacroFunction(context, "visitArgumentInit", [&]() {
                result = invoke_visitFunctionArgumentInit(context,fnVisit,classPtr,fn,var,that);
            });
            return return_smart(result,that);
        } else {
            return that;
        }
    }

    void VisitorAdapter::preVisitFunctionBody(Function *fn, Expression *that) {
        if ( auto fnPreVisit = get_preVisitFunctionBody(classPtr) ) {
            runMacroFunction(context, "preVisitFunctionBody", [&]() {
                invoke_preVisitFunctionBody(context,fnPreVisit,classPtr,fn,that);
            });
        }
    }

    ExpressionPtr VisitorAdapter::visitFunctionBody(Function *fn, Expression *that) {
        if ( auto fnVisit = get_visitFunctionBody(classPtr) ) {
            ExpressionPtr result;
            runMacroFunction(context, "visitFunctionBody", [&]() {
                result = invoke_visitFunctionBody(context,fnVisit,classPtr,fn,that);
            });
            return return_smart(result,that);
        } else {
            return that;
        }
    }

    void VisitorAdapter::preVisitExpression(Expression *expr) {
        if ( auto fnPreVisit = get_preVisitExpression(classPtr) ) {
            runMacroFunction(context, "preVisitExpression", [&]() {
                invoke_preVisitExpression(context,fnPreVisit,classPtr,expr);
            });
        }
    }

    ExpressionPtr VisitorAdapter::visitExpression(Expression *expr) {
        if ( auto fnVisit = get_visitExpression(classPtr) ) {
            ExpressionPtr result;
            runMacroFunction(context, "visitExpression", [&]() {
                result = invoke_visitExpression(context,fnVisit,classPtr,expr);
            });
            return return_smart(result,expr);
        } else {
            return expr;
        }
    }

    void VisitorAdapter::preVisitBlockArgument(ExprBlock *block, const VariablePtr &var, bool lastArg) {
        if ( auto fnPreVisit = get_preVisitExprBlockArgument(classPtr) ) {
            runMacroFunction(context, "preVisitBlockArgument", [&]() {
                invoke_preVisitExprBlockArgument(context,fnPreVisit,classPtr,block,var,lastArg);
            });
        }
    }

    VariablePtr VisitorAdapter::visitBlockArgument(ExprBlock *block, const VariablePtr &var, bool lastArg) {
        if ( auto fnVisit = get_visitExprBlockArgument(classPtr) ) {
            VariablePtr result;
            runMacroFunction(context, "visitBlockArgument", [&]() {
                result = invoke_visitExprBlockArgument(context,fnVisit,classPtr,block,var,lastArg);
            });
            return return_smart(result,var.get());
        } else {
            return var;
        }
    }

    void VisitorAdapter::preVisitBlockArgumentInit(ExprBlock *block, const VariablePtr &var, Expression *init) {
        if ( auto fnPreVisit = get_preVisitExprBlockArgumentInit(classPtr) ) {
            runMacroFunction(context, "preVisitBlockArgumentInit", [&]() {
                invoke_preVisitExprBlockArgumentInit(context,fnPreVisit,classPtr,block,var,init);
            });
        }
    }

    ExpressionPtr VisitorAdapter::visitBlockArgumentInit(ExprBlock *block, const VariablePtr &var, Expression *that) {
        if ( auto fnVisit = get_visitExprBlockArgumentInit(classPtr) ) {
            ExpressionPtr result;
            runMacroFunction(context, "visitBlockArgumentInit", [&]() {
                result = invoke_visitExprBlockArgumentInit(context,fnVisit,classPtr,block,var,that);
            });
            return return_smart(result,that);
        } else {
            return that;
        }
    }

    void VisitorAdapter::preVisitBlockExpression(ExprBlock *block, Expression *expr) {
        if ( auto fnPreVisit = get_preVisitExprBlockExpression(classPtr) ) {
            runMacroFunction(context, "preVisitBlockExpression", [&]() {
                invoke_preVisitExprBlockExpression(context,fnPreVisit,classPtr,block,expr);
            });
        }
    }

    ExpressionPtr VisitorAdapter::visitBlockExpression(ExprBlock *block, Expression *expr) {
        if ( auto fnVisit = get_visitExprBlockExpression(classPtr) ) {
            ExpressionPtr result;
            runMacroFunction(context, "visitBlockExpression", [&]() {
                result = invoke_visitExprBlockExpression(context,fnVisit,classPtr,block,expr);
            });
            return return_smart(result,expr);
        } else {
            return expr;
        }
    }

    void VisitorAdapter::preVisitBlockFinal(ExprBlock *block) {
        if ( auto fnPreVisit = get_preVisitExprBlockFinal(classPtr) ) {
            runMacroFunction(context, "preVisitBlockFinal", [&]() {
                invoke_preVisitExprBlockFinal(context,fnPreVisit,classPtr,block);
            });
        }
    }

    void VisitorAdapter::visitBlockFinal(ExprBlock *block) {
        if ( auto fnVisit = get_visitExprBlockFinal(classPtr) ) {
            runMacroFunction(context, "visitBlockFinal", [&]() {
                invoke_visitExprBlockFinal(context,fnVisit,classPtr,block);
            });
        }
    }

    void VisitorAdapter::preVisitBlockFinalExpression(ExprBlock *block, Expression *expr) {
        if ( auto fnPreVisit = get_preVisitExprBlockFinalExpression(classPtr) ) {
            runMacroFunction(context, "preVisitBlockFinalExpression", [&]() {
                invoke_preVisitExprBlockFinalExpression(context,fnPreVisit,classPtr,block,expr);
            });
        }
    }

    ExpressionPtr VisitorAdapter::visitBlockFinalExpression(ExprBlock *block, Expression *expr) {
        if ( auto fnVisit = get_visitExprBlockFinalExpression(classPtr) ) {
            ExpressionPtr result;
            runMacroFunction(context, "visitBlockFinalExpression", [&]() {
                result = invoke_visitExprBlockFinalExpression(context,fnVisit,classPtr,block,expr);
            });
            return return_smart(result,expr);
        } else {
            return expr;
        }
    }

    void VisitorAdapter::preVisitLet(ExprLet *let, const VariablePtr &var, bool last) {
        if ( auto fnPreVisit = get_preVisitExprLetVariable(classPtr) ) {
            runMacroFunction(context, "preVisitLet", [&]() {
                invoke_preVisitExprLetVariable(context,fnPreVisit,classPtr,let,var,last);
            });
        }
    }

    VariablePtr VisitorAdapter::visitLet(ExprLet *let, const VariablePtr &var, bool last) {
        if ( auto fnVisit = get_visitExprLetVariable(classPtr) ) {
            VariablePtr result;
            runMacroFunction(context, "visitLet", [&]() {
                result = invoke_visitExprLetVariable(context,fnVisit,classPtr,let,var,last);
            });
            return return_smart(result,var.get());
        } else {
            return var;
        }
    }

    void VisitorAdapter::preVisitLetInit(ExprLet *let, const VariablePtr &var, Expression *init) {
        if ( auto fnPreVisit = get_preVisitExprLetVariableInit(classPtr) ) {
            runMacroFunction(context, "preVisitLetInit", [&]() {
                invoke_preVisitExprLetVariableInit(context,fnPreVisit,classPtr,let,var,init);
            });
        }
    }

    ExpressionPtr VisitorAdapter::visitLetInit(ExprLet *let, const VariablePtr &var, Expression *that) {
        if ( auto fnVisit = get_visitExprLetVariableInit(classPtr) ) {
            ExpressionPtr result;
            runMacroFunction(context, "visitLetInit", [&]() {
                result = invoke_visitExprLetVariableInit(context,fnVisit,classPtr,let,var,that);
            });
            return return_smart(result,that);
        } else {
            return that;
        }
    }

    void VisitorAdapter::preVisitGlobalLetBody(Program *prog) {
        if ( auto fnPreVisit = get_preVisitGlobalLet(classPtr) ) {
            runMacroFunction(context, "preVisitGlobalLetBody", [&]() {
                invoke_preVisitGlobalLet(context,fnPreVisit,classPtr,prog);
            });
        }
    }

    void VisitorAdapter::visitGlobalLetBody(Program *prog) {
        if ( auto fnVisit = get_visitGlobalLet(classPtr) ) {
            runMacroFunction(context, "visitGlobalLetBody", [&]() {
                invoke_visitGlobalLet(context,fnVisit,classPtr,prog);
            });
        }
    }

    void VisitorAdapter::preVisitGlobalLet(const VariablePtr &var) {
        if ( auto fnPreVisit = get_preVisitGlobalLetVariable(classPtr) ) {
            runMacroFunction(context, "preVisitGlobalLet", [&]() {
                invoke_preVisitGlobalLetVariable(context,fnPreVisit,classPtr,var,false); // TODO: remove 'last'
            });
        }
    }

    VariablePtr VisitorAdapter::visitGlobalLet(const VariablePtr &var) {
        if ( auto fnVisit = get_visitGlobalLetVariable(classPtr) ) {
            VariablePtr result;
            runMacroFunction(context, "visitGlobalLet", [&]() {
                result = invoke_visitGlobalLetVariable(context,fnVisit,classPtr,var,false); // TODO: remove 'last'
            });
            return return_smart(result,var.get());
        } else {
            return var;
        }
    }

    void VisitorAdapter::preVisitGlobalLetInit(const VariablePtr &var, Expression *that) {
        if ( auto fnPreVisit = get_preVisitGlobalLetVariableInit(classPtr) ) {
            runMacroFunction(context, "preVisitGlobalLetInit", [&]() {
                invoke_preVisitGlobalLetVariableInit(context,fnPreVisit,classPtr,var,that);
            });
        }
    }

    ExpressionPtr VisitorAdapter::visitGlobalLetInit(const VariablePtr &var, Expression *that) {
        if ( auto fnVisit = get_visitGlobalLetVariableInit(classPtr) ) {
            ExpressionPtr result;
            runMacroFunction(context, "visitGlobalLetInit", [&]() {
                result = invoke_visitGlobalLetVariableInit(context,fnVisit,classPtr,var,that);
            });
            return return_smart(result,that);
        } else {
            return that;
        }
    }

    void VisitorAdapter::preVisit(ExprStringBuilder *expr) {
        if ( auto fnPreVisit = get_preVisitExprStringBuilder(classPtr) ) {
            runMacroFunction(context, "preVisit", [&]() {
                invoke_preVisitExprStringBuilder(context,fnPreVisit,classPtr,expr);
            });
        }
    }

    void VisitorAdapter::preVisitStringBuilderElement(ExprStringBuilder *sb, Expression *expr, bool last) {
        if ( auto fnPreVisit = get_preVisitExprStringBuilderElement(classPtr) ) {
            runMacroFunction(context, "preVisitStringBuilderElement", [&]() {
                invoke_preVisitExprStringBuilderElement(context,fnPreVisit,classPtr,sb,expr,last);
            });
        }
    }

    ExpressionPtr VisitorAdapter::visitStringBuilderElement(ExprStringBuilder *sb, Expression *expr, bool last) {
        if ( auto fnVisit = get_visitExprStringBuilderElement(classPtr) ) {
            ExpressionPtr result;
            runMacroFunction(context, "visitStringBuilderElement", [&]() {
                result = invoke_visitExprStringBuilderElement(context,fnVisit,classPtr,sb,expr,last);
            });
            return return_smart(result,expr);
        } else {
            return expr;
        }
    }

    void VisitorAdapter::preVisitNewArg(ExprNew *call, Expression *arg, bool last) {
        if ( auto fnPreVisit = get_preVisitExprNewArgument(classPtr) ) {
            runMacroFunction(context, "preVisitNewArg", [&]() {
                invoke_preVisitExprNewArgument(context,fnPreVisit,classPtr,call,arg,last);
            });
        }
    }

    ExpressionPtr VisitorAdapter::visit(ExprStringBuilder *expr) {
        if ( auto fnVisit = get_visitExprStringBuilder(classPtr) ) {
            ExpressionPtr result;
            runMacroFunction(context, "visit", [&]() {
                result = invoke_visitExprStringBuilder(context,fnVisit,classPtr,expr);
            });
            return return_smart(result,expr);
        } else {
            return expr;
        }
    }

    ExpressionPtr VisitorAdapter::visitNewArg(ExprNew *call, Expression *arg, bool last) {
        if ( auto fnVisit = get_visitExprNewArgument(classPtr) ) {
            ExpressionPtr result;
            runMacroFunction(context, "visitNewArg", [&]() {
                result = invoke_visitExprNewArgument(context,fnVisit,classPtr,call,arg,last);
            });
            return return_smart(result,arg);
        } else {
            return arg;
        }
    }

    void VisitorAdapter::preVisitNamedCallArg(ExprNamedCall *call, MakeFieldDecl *arg, bool last) {
        if ( auto fnPreVisit = get_preVisitExprNamedCallArgument(classPtr) ) {
            runMacroFunction(context, "preVisitNamedCallArg", [&]() {
                invoke_preVisitExprNamedCallArgument(context,fnPreVisit,classPtr,call,arg,last);
            });
        }
    }

    MakeFieldDeclPtr VisitorAdapter::visitNamedCallArg(ExprNamedCall *call, MakeFieldDecl *arg, bool last) {
        if ( auto fnVisit = get_visitExprNamedCallArgument(classPtr) ) {
            MakeFieldDeclPtr result;
            runMacroFunction(context, "visitNamedCallArg", [&]() {
                result = invoke_visitExprNamedCallArgument(context,fnVisit,classPtr,call,arg,last);
            });
            return return_smart(result,arg);
        } else {
            return arg;
        }
    }

    void VisitorAdapter::preVisitCallArg(ExprCall *call, Expression *arg, bool last) {
        if ( auto fnPreVisit = get_preVisitExprCallArgument(classPtr) ) {
            runMacroFunction(context, "preVisitCallArg", [&]() {
                invoke_preVisitExprCallArgument(context,fnPreVisit,classPtr,call,arg,last);
            });
        }
    }

    ExpressionPtr VisitorAdapter::visitCallArg(ExprCall *call, Expression *arg, bool last) {
        if ( auto fnVisit = get_visitExprCallArgument(classPtr) ) {
            ExpressionPtr result;
            runMacroFunction(context, "visitCallArg", [&]() {
                result = invoke_visitExprCallArgument(context,fnVisit,classPtr,call,arg,last);
            });
            return return_smart(result,arg);
        } else {
            return arg;
        }
    }

    bool VisitorAdapter::canVisitLooksLikeCallArg(ExprLooksLikeCall *call, Expression *arg, bool last) {
        if ( auto fnCanVisit = get_canVisitLooksLikeCallArgument(classPtr) ) {
            bool result = true;
            runMacroFunction(context, "canVisitLooksLikeCallArg", [&]() {
                result = invoke_canVisitLooksLikeCallArgument(context,fnCanVisit,classPtr,call,arg,last);
            });
            return result;
        } else {
            return true;
        }
    }

    void VisitorAdapter::preVisitLooksLikeCallArg(ExprLooksLikeCall *call, Expression *arg, bool last) {
        if ( auto fnPreVisit = get_preVisitExprLooksLikeCallArgument(classPtr) ) {
            runMacroFunction(context, "preVisitLooksLikeCallArg", [&]() {
                invoke_preVisitExprLooksLikeCallArgument(context,fnPreVisit,classPtr,call,arg,last);
            });
        }
    }

    ExpressionPtr VisitorAdapter::visitLooksLikeCallArg(ExprLooksLikeCall *call, Expression *arg, bool last) {
        if ( auto fnVisit = get_visitExprLooksLikeCallArgument(classPtr) ) {
            ExpressionPtr result;
            runMacroFunction(context, "visitLooksLikeCallArg", [&]() {
                result = invoke_visitExprLooksLikeCallArgument(context,fnVisit,classPtr,call,arg,last);
            });
            return return_smart(result,arg);
        } else {
            return arg;
        }
    }

    void VisitorAdapter::preVisitNullCoaelescingDefault(ExprNullCoalescing *expr, Expression *def) {
        if ( auto fnPreVisit = get_preVisitExprNullCoalescingDefault(classPtr) ) {
            runMacroFunction(context, "preVisitNullCoaelescingDefault", [&]() {
                invoke_preVisitExprNullCoalescingDefault(context,fnPreVisit,classPtr,expr,def);
            });
        }
    }

    void VisitorAdapter::preVisitTagValue(ExprTag *expr, Expression *val) {
        if ( auto fnPreVisit = get_preVisitExprTagValue(classPtr) ) {
            runMacroFunction(context, "preVisitTagValue", [&]() {
                invoke_preVisitExprTagValue(context,fnPreVisit,classPtr,expr,val);
            });
        }
    }

    void VisitorAdapter::preVisitAtIndex(ExprAt *expr, Expression *index) {
        if ( auto fnPreVisit = get_preVisitExprAtIndex(classPtr) ) {
            runMacroFunction(context, "preVisitAtIndex", [&]() {
                invoke_preVisitExprAtIndex(context,fnPreVisit,classPtr,expr,index);
            });
        }
    }

    void VisitorAdapter::preVisitSafeAtIndex(ExprSafeAt *expr, Expression *index) {
        if ( auto fnPreVisit = get_preVisitExprSafeAtIndex(classPtr) ) {
            runMacroFunction(context, "preVisitSafeAtIndex", [&]() {
                invoke_preVisitExprSafeAtIndex(context,fnPreVisit,classPtr,expr,index);
            });
        }
    }

    void VisitorAdapter::preVisitType(ExprIs *expr, TypeDecl *val) {
        if ( auto fnPreVisit = get_preVisitExprIsType(classPtr) ) {
            runMacroFunction(context, "preVisitType", [&]() {
                invoke_preVisitExprIsType(context,fnPreVisit,classPtr,expr,val);
            });
        }
    }

    void VisitorAdapter::preVisitRight(ExprOp2 *expr, Expression *right) {
        if ( auto fnPreVisit = get_preVisitExprOp2Right(classPtr) ) {
            runMacroFunction(context, "preVisitRight", [&]() {
                invoke_preVisitExprOp2Right(context,fnPreVisit,classPtr,expr,right);
            });
        }
    }

    void VisitorAdapter::preVisitLeft(ExprOp3 *expr, Expression *left) {
        if ( auto fnPreVisit = get_preVisitExprOp3Left(classPtr) ) {
            runMacroFunction(context, "preVisitLeft", [&]() {
                invoke_preVisitExprOp3Left(context,fnPreVisit,classPtr,expr,left);
            });
        }
    }

    bool VisitorAdapter::isRightFirst(ExprCopy *expr) {
        if ( auto fnIsFirst = get_isRightFirstExprCopy(classPtr) ) {
            bool result = true;
            runMacroFunction(context, "isRightFirst", [&]() {
                result = invoke_isRightFirstExprCopy(context,fnIsFirst,classPtr,expr);
            });
            return result;
        } else {
            return false;
        }
    }

    void VisitorAdapter::preVisitWithBody(ExprWith *expr, Expression *body) {
        if ( auto fnPreVisit = get_preVisitExprWithBody(classPtr) ) {
            runMacroFunction(context, "preVisitWithBody", [&]() {
                invoke_preVisitExprWithBody(context,fnPreVisit,classPtr,expr,body);
            });
        }
    }

    void VisitorAdapter::preVisitRight(ExprOp3 *expr, Expression *right) {
        if ( auto fnPreVisit = get_preVisitExprOp3Right(classPtr) ) {
            runMacroFunction(context, "preVisitRight", [&]() {
                invoke_preVisitExprOp3Right(context,fnPreVisit,classPtr,expr,right);
            });
        }
    }

    void VisitorAdapter::preVisitRight(ExprCopy *expr, Expression *right) {
        if ( auto fnPreVisit = get_preVisitExprCopyRight(classPtr) ) {
            runMacroFunction(context, "preVisitRight", [&]() {
                invoke_preVisitExprCopyRight(context,fnPreVisit,classPtr,expr,right);
            });
        }
    }

    bool VisitorAdapter::isRightFirst(ExprMove *expr) {
        if ( auto fnIsFirst = get_isRightFirstExprMove(classPtr) ) {
            bool result = true;
            runMacroFunction(context, "isRightFirst", [&]() {
                result = invoke_isRightFirstExprMove(context,fnIsFirst,classPtr,expr);
            });
            return result;
        } else {
            return false;
        }
    }

    void VisitorAdapter::preVisitRight(ExprMove *expr, Expression *right) {
        if ( auto fnPreVisit = get_preVisitExprMoveRight(classPtr) ) {
            runMacroFunction(context, "preVisitRight", [&]() {
                invoke_preVisitExprMoveRight(context,fnPreVisit,classPtr,expr,right);
            });
        }
    }

    bool VisitorAdapter::isRightFirst(ExprClone *expr) {
        if ( auto fnIsFirst = get_isRightFirstExprClone(classPtr) ) {
            bool result = true;
            runMacroFunction(context, "isRightFirst", [&]() {
                result = invoke_isRightFirstExprClone(context,fnIsFirst,classPtr,expr);
            });
            return result;
        } else {
            return false;
        }
    }

    void VisitorAdapter::preVisitRight(ExprClone *expr, Expression *right) {
        if ( auto fnPreVisit = get_preVisitExprCloneRight(classPtr) ) {
            runMacroFunction(context, "preVisitRight", [&]() {
                invoke_preVisitExprCloneRight(context,fnPreVisit,classPtr,expr,right);
            });
        }
    }

    void VisitorAdapter::preVisitWhileBody(ExprWhile *expr, Expression *right) {
        if ( auto fnPreVisit = get_preVisitExprWhileBody(classPtr) ) {
            runMacroFunction(context, "preVisitWhileBody", [&]() {
                invoke_preVisitExprWhileBody(context,fnPreVisit,classPtr,expr,right);
            });
        }
    }

    void VisitorAdapter::preVisitCatch(ExprTryCatch *expr, Expression *that) {
        if ( auto fnPreVisit = get_preVisitExprTryCatchCatch(classPtr) ) {
            runMacroFunction(context, "preVisitCatch", [&]() {
                invoke_preVisitExprTryCatchCatch(context,fnPreVisit,classPtr,expr,that);
            });
        }
    }

    void VisitorAdapter::preVisitIfBlock(ExprIfThenElse *expr, Expression *that) {
        if ( auto fnPreVisit = get_preVisitExprIfThenElseIfBlock(classPtr) ) {
            runMacroFunction(context, "preVisitIfBlock", [&]() {
                invoke_preVisitExprIfThenElseIfBlock(context,fnPreVisit,classPtr,expr,that);
            });
        }
    }

    void VisitorAdapter::preVisitElseBlock(ExprIfThenElse *expr, Expression *that) {
        if ( auto fnPreVisit = get_preVisitExprIfThenElseElseBlock(classPtr) ) {
            runMacroFunction(context, "preVisitElseBlock", [&]() {
                invoke_preVisitExprIfThenElseElseBlock(context,fnPreVisit,classPtr,expr,that);
            });
        }
    }

    void VisitorAdapter::preVisitFor(ExprFor *expr, const VariablePtr &var, bool last) {
        if ( auto fnPreVisit = get_preVisitExprForVariable(classPtr) ) {
            runMacroFunction(context, "preVisitFor", [&]() {
                invoke_preVisitExprForVariable(context,fnPreVisit,classPtr,expr,var,last);
            });
        }
    }

    VariablePtr VisitorAdapter::visitFor(ExprFor *expr, const VariablePtr &var, bool last) {
        if ( auto fnVisit = get_visitExprForVariable(classPtr) ) {
            VariablePtr result;
            runMacroFunction(context, "visitFor", [&]() {
                result = invoke_visitExprForVariable(context,fnVisit,classPtr,expr,var,last);
            });
            return return_smart(result,var.get());
        } else {
            return var;
        }
    }

    void VisitorAdapter::preVisitForStack(ExprFor *expr) {
        if ( auto fnPreVisit = get_preVisitExprForStack(classPtr) ) {
            runMacroFunction(context, "preVisitForStack", [&]() {
                invoke_preVisitExprForStack(context,fnPreVisit,classPtr,expr);
            });
        }
    }

    void VisitorAdapter::preVisitForBody(ExprFor *expr, Expression *) {
        if ( auto fnPreVisit = get_preVisitExprForBody(classPtr) ) {
            runMacroFunction(context, "preVisitForBody", [&]() {
                invoke_preVisitExprForBody(context,fnPreVisit,classPtr,expr); // TODO: pass that
            });
        }
    }

    void VisitorAdapter::preVisitForSource(ExprFor *expr, Expression *that, bool last) {
        if ( auto fnPreVisit = get_preVisitExprForSource(classPtr) ) {
            runMacroFunction(context, "preVisitForSource", [&]() {
                invoke_preVisitExprForSource(context,fnPreVisit,classPtr,expr,that,last);
            });
        }
    }

    ExpressionPtr VisitorAdapter::visitForSource(ExprFor *expr, Expression *that, bool last) {
        if ( auto fnVisit = get_visitExprForSource(classPtr) ) {
            ExpressionPtr result;
            runMacroFunction(context, "visitForSource", [&]() {
                result = invoke_visitExprForSource(context,fnVisit,classPtr,expr,that,last);
            });
            return return_smart(result,that);
        } else {
            return that;
        }
    }

    void VisitorAdapter::preVisitMakeVariantField(ExprMakeVariant *expr, int index, MakeFieldDecl *decl, bool lastField) {
        if ( auto fnPreVisit = get_preVisitExprMakeVariantField(classPtr) ) {
            runMacroFunction(context, "preVisitMakeVariantField", [&]() {
                invoke_preVisitExprMakeVariantField(context,fnPreVisit,classPtr,expr,index,decl,lastField);
            });
        }
    }

    MakeFieldDeclPtr
    VisitorAdapter::visitMakeVariantField(ExprMakeVariant *expr, int index, MakeFieldDecl *decl, bool lastField) {
        if ( auto fnVisit = get_visitExprMakeVariantField(classPtr) ) {
            MakeFieldDeclPtr result;
            runMacroFunction(context, "visitMakeVariantField", [&]() {
                result = invoke_visitExprMakeVariantField(context,fnVisit,classPtr,expr,index,decl,lastField);
            });
            return return_smart(result,decl);
        } else {
            return decl;
        }
    }

    void VisitorAdapter::preVisitMakeStructureIndex(ExprMakeStruct *expr, int index, bool lastIndex) {
        if ( auto fnPreVisit = get_preVisitExprMakeStructIndex(classPtr) ) {
            runMacroFunction(context, "preVisitMakeStructureIndex", [&]() {
                invoke_preVisitExprMakeStructIndex(context,fnPreVisit,classPtr,expr,index,lastIndex);
            });
        }
    }

    void VisitorAdapter::visitMakeStructureIndex(ExprMakeStruct *expr, int index, bool lastField) {
        if ( auto fnVisit = get_visitExprMakeStructIndex(classPtr) ) {
            runMacroFunction(context, "visitMakeStructureIndex", [&]() {
                invoke_visitExprMakeStructIndex(context,fnVisit,classPtr,expr,index,lastField);
            });
        }
    }

    void VisitorAdapter::preVisitMakeStructureField(ExprMakeStruct *expr, int index, MakeFieldDecl *decl, bool lastField) {
        if ( auto fnPreVisit = get_preVisitExprMakeStructField(classPtr) ) {
            runMacroFunction(context, "preVisitMakeStructureField", [&]() {
                invoke_preVisitExprMakeStructField(context,fnPreVisit,classPtr,expr,index,decl,lastField);
            });
        }
    }

    MakeFieldDeclPtr
    VisitorAdapter::visitMakeStructureField(ExprMakeStruct *expr, int index, MakeFieldDecl *decl, bool lastField) {
        if ( auto fnVisit = get_visitExprMakeStructField(classPtr) ) {
            MakeFieldDeclPtr result;
            runMacroFunction(context, "visitMakeStructureField", [&]() {
                result = invoke_visitExprMakeStructField(context,fnVisit,classPtr,expr,index,decl,lastField);
            });
            return return_smart(result,decl);
        } else {
            return decl;
        }
    }

    void VisitorAdapter::preVisitMakeStructureBlock(ExprMakeStruct *expr, Expression *blk) {
        if ( auto fnPreVisit = get_preVisitMakeStructureBlock(classPtr) ) {
            runMacroFunction(context, "preVisitMakeStructureBlock", [&]() {
                invoke_preVisitMakeStructureBlock(context,fnPreVisit,classPtr,expr,blk);
            });
        }
    }

    ExpressionPtr VisitorAdapter::visitMakeStructureBlock(ExprMakeStruct *expr, Expression *blk) {
        if ( auto fnVisit = get_visitMakeStructureBlock(classPtr) ) {
            ExpressionPtr result;
            runMacroFunction(context, "visitMakeStructureBlock", [&]() {
                result = invoke_visitMakeStructureBlock(context,fnVisit,classPtr,expr,blk);
            });
            return return_smart(result,blk);
        } else {
            return blk;
        }
    }

    void VisitorAdapter::preVisitMakeArrayIndex(ExprMakeArray *expr, int index, Expression *init, bool lastIndex) {
        if ( auto fnPreVisit = get_preVisitExprMakeArrayIndex(classPtr) ) {
            runMacroFunction(context, "preVisitMakeArrayIndex", [&]() {
                invoke_preVisitExprMakeArrayIndex(context,fnPreVisit,classPtr,expr,index,init,lastIndex);
            });
        }
    }

    ExpressionPtr VisitorAdapter::visitMakeArrayIndex(ExprMakeArray *expr, int index, Expression *init, bool lastField) {
        if ( auto fnVisit = get_visitExprMakeArrayIndex(classPtr) ) {
            ExpressionPtr result;
            runMacroFunction(context, "visitMakeArrayIndex", [&]() {
                result = invoke_visitExprMakeArrayIndex(context,fnVisit,classPtr,expr,index,init,lastField);
            });
            return return_smart(result,init);
        } else {
            return init;
        }
    }

    void VisitorAdapter::preVisitMakeTupleIndex(ExprMakeTuple *expr, int index, Expression *init, bool lastIndex) {
        if ( auto fnPreVisit = get_preVisitExprMakeTupleIndex(classPtr) ) {
            runMacroFunction(context, "preVisitMakeTupleIndex", [&]() {
                invoke_preVisitExprMakeTupleIndex(context,fnPreVisit,classPtr,expr,index,init,lastIndex);
            });
        }
    }

    ExpressionPtr VisitorAdapter::visitMakeTupleIndex(ExprMakeTuple *expr, int index, Expression *init, bool lastField) {
        if ( auto fnVisit = get_visitExprMakeTupleIndex(classPtr) ) {
            ExpressionPtr result;
            runMacroFunction(context, "visitMakeTupleIndex", [&]() {
                result = invoke_visitExprMakeTupleIndex(context,fnVisit,classPtr,expr,index,init,lastField);
            });
            return return_smart(result,init);
        } else {
            return init;
        }
    }

    void VisitorAdapter::preVisitArrayComprehensionSubexpr(ExprArrayComprehension *expr, Expression *subexpr) {
        if ( auto fnPreVisit = get_preVisitExprArrayComprehensionSubexpr(classPtr) ) {
            runMacroFunction(context, "preVisitArrayComprehensionSubexpr", [&]() {
                invoke_preVisitExprArrayComprehensionSubexpr(context,fnPreVisit,classPtr,expr,subexpr);
            });
        }
    }

    void VisitorAdapter::preVisitArrayComprehensionWhere(ExprArrayComprehension *expr, Expression *where) {
        if ( auto fnPreVisit = get_preVisitExprArrayComprehensionWhere(classPtr) ) {
            runMacroFunction(context, "preVisitArrayComprehensionWhere", [&]() {
                invoke_preVisitExprArrayComprehensionWhere(context,fnPreVisit,classPtr,expr,where);
            });
        }
    }


    struct AstVisitorAdapterAnnotation : ManagedStructureAnnotation<VisitorAdapter,false,true> {
        AstVisitorAdapterAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("VisitorAdapter", ml) {
        }
    };

    class BlockAnnotationAdapter : public FunctionAnnotation, AstBlockAnnotation_Adapter {
    public:
        BlockAnnotationAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
        : FunctionAnnotation(n), AstBlockAnnotation_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual bool apply ( const FunctionPtr &, ModuleGroup &,
                            const AnnotationArgumentList &, string & err ) override {
            err = "not a function annotation";
            return false;
        }
        virtual bool finalize ( const FunctionPtr &, ModuleGroup &,
                               const AnnotationArgumentList &,
                               const AnnotationArgumentList &, string & err ) override {
            err = "not a function annotation";
            return false;
        }
        virtual bool apply ( ExprBlock * blk, ModuleGroup & group,
                            const AnnotationArgumentList & args, string & errors ) override {
            if ( auto fnApply = get_apply(classPtr) ) {
                bool result = true;
                runMacroFunction(context, "apply", [&]() {
                    result = invoke_apply(context,fnApply,classPtr,blk,group,args,errors);
                });
                return result;
            } else {
                return true;
            }
        }
        virtual bool finalize ( ExprBlock * blk, ModuleGroup & group,
                               const AnnotationArgumentList & args,
                               const AnnotationArgumentList & progArgs, string & errors ) override {
            if ( auto fnFinish = get_finish(classPtr) ) {
                bool result = true;
                runMacroFunction(context, "finish", [&]() {
                    result = invoke_finish(context,fnFinish,classPtr,blk,group,args,progArgs,errors);
                });
                return result;
            } else {
                return true;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    class FunctionAnnotationAdapter : public FunctionAnnotation, AstFunctionAnnotation_Adapter {
    public:
        FunctionAnnotationAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
        : FunctionAnnotation(n), AstFunctionAnnotation_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual bool apply ( const FunctionPtr & func, ModuleGroup & group,
                            const AnnotationArgumentList & args, string & errors ) override {
            if ( auto fnApply = get_apply(classPtr) ) {
                bool result = true;
                runMacroFunction(context, "apply", [&]() {
                    result = invoke_apply(context,fnApply,classPtr,func,group,args,errors);
                });
                return result;
            } else {
                return true;
            }
        }
        virtual bool generic_apply ( const FunctionPtr & func, ModuleGroup & group,
                            const AnnotationArgumentList & args, string & errors ) override {
            if ( auto fnApply = get_generic_apply(classPtr) ) {
                bool result = true;
                runMacroFunction(context, "generic_apply", [&]() {
                    result = invoke_generic_apply(context,fnApply,classPtr,func,group,args,errors);
                });
                return result;
            } else {
                return true;
            }
        }
        virtual bool finalize ( const FunctionPtr & func, ModuleGroup & group,
                               const AnnotationArgumentList & args,
                               const AnnotationArgumentList & progArgs, string & errors ) override {
            if ( auto fnFinish = get_finish(classPtr) ) {
                bool result = true;
                runMacroFunction(context, "finish", [&]() {
                    result = invoke_finish(context,fnFinish,classPtr,func,group,args,progArgs,errors);
                });
                return result;
            } else {
                return true;
            }
        }
        virtual bool lint ( const FunctionPtr & func, ModuleGroup & group,
                               const AnnotationArgumentList & args,
                               const AnnotationArgumentList & progArgs, string & errors ) override {
            if ( auto fnLint = get_lint(classPtr) ) {
                bool result = true;
                runMacroFunction(context, "lint", [&]() {
                    result = invoke_lint(context,fnLint,classPtr,func,group,args,progArgs,errors);
                });
                return result;
            } else {
                return true;
            }
        }
        virtual bool patch ( const FunctionPtr & func, ModuleGroup & group,
                               const AnnotationArgumentList & args,
                               const AnnotationArgumentList & progArgs, string & errors, bool & astChanged ) override {
            if ( auto fnPatch = get_patch(classPtr) ) {
                bool result = true;
                runMacroFunction(context, "patch", [&]() {
                    result = invoke_patch(context,fnPatch,classPtr,func,group,args,progArgs,errors,astChanged);
                });
                return result;
            } else {
                return true;
            }
        }
        virtual bool fixup ( const FunctionPtr & func, ModuleGroup & group,
                               const AnnotationArgumentList & args,
                               const AnnotationArgumentList & progArgs, string & errors ) override {
            if ( auto fnFixup = get_fixup(classPtr) ) {
                bool result = true;
                runMacroFunction(context, "fixup", [&]() {
                    result = invoke_fixup(context,fnFixup,classPtr,func,group,args,progArgs,errors);
                });
                return result;
            } else {
                return true;
            }
        }
        virtual bool apply ( ExprBlock *, ModuleGroup &,
                            const AnnotationArgumentList &, string & err ) override {
            err = "not a block annotation";
            return false;
        }
        virtual bool finalize ( ExprBlock *, ModuleGroup &,
                               const AnnotationArgumentList &,
                               const AnnotationArgumentList &, string & err ) override {
            err = "not a block annotation";
            return false;
        }
        virtual ExpressionPtr transformCall ( ExprCallFunc * call, string & err ) override {
            if ( auto fnTransform = get_transform(classPtr) ) {
                ExpressionPtr result;
                runMacroFunction(context, "transformCall", [&]() {
                    result = invoke_transform(context,fnTransform,classPtr,call,err);
                });
                return result;
            } else {
                return nullptr;
            }
        }
        virtual bool verifyCall ( ExprCallFunc * call, const AnnotationArgumentList & args,
                const AnnotationArgumentList & progArgs, string & err ) override {
            if ( auto fnTransform = get_verifyCall(classPtr) ) {
                bool result = true;
                runMacroFunction(context, "verifyCall", [&]() {
                    result = invoke_verifyCall(context,fnTransform,classPtr,call,args,progArgs,err);
                });
                return result;
            } else {
                return true;
            }
        }
        virtual bool isSpecialized () const override {
            if ( auto fnIsSpecialized = get_isSpecialized(classPtr) ) {
                bool result = false;
                runMacroFunction(context, "isSpecialized", [&]() {
                    result = invoke_isSpecialized(context,fnIsSpecialized,classPtr);
                });
                return result;
            } else {
                return false;
            }
        }
        virtual bool isCompatible ( const FunctionPtr & fn, const vector<TypeDeclPtr> & types,
            const AnnotationDeclaration & decl, string & err  ) const override {
            if ( auto fnIsCompatible = get_isCompatible(classPtr) ) {
                bool result = true;
                runMacroFunction(context, "isCompatible", [&]() {
                    result = invoke_isCompatible(context,fnIsCompatible,classPtr,fn,const_cast<vector<TypeDeclPtr> &>(types),decl,err);
                });
                return result;
            } else {
                return true;
            }
        }
        virtual void  complete (  Context * ctx, const FunctionPtr & fnp ) override {
            if ( auto fnComplete = get_complete(classPtr) ) {
                runMacroFunction(context, "complete", [&]() {
                    invoke_complete(context,fnComplete,classPtr,fnp,ctx);
                });
            }
        }
        virtual void appendToMangledName( const FunctionPtr & fnp, const AnnotationDeclaration & decl, string & mangledName ) const override {
            if ( auto fnAppend = get_appendToMangledName(classPtr) ) {
                runMacroFunction(context, "appendToMangledName", [&]() {
                    invoke_appendToMangledName(context,fnAppend,classPtr,fnp,decl,mangledName);
                });
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    struct AstFunctionAnnotationAnnotation : ManagedStructureAnnotation<FunctionAnnotation,false,true> {
        AstFunctionAnnotationAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("FunctionAnnotation", ml) {
        }
    };

    struct StructureAnnotationAdapter : StructureAnnotation, AstStructureAnnotation_Adapter {
        StructureAnnotationAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
            : StructureAnnotation(n), AstStructureAnnotation_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual bool touch ( const StructurePtr & st, ModuleGroup & group,
                            const AnnotationArgumentList & args, string & errors ) override {
            if ( auto fnApply = get_apply(classPtr) ) {
                bool result = true;
                runMacroFunction(context, "apply", [&]() {
                    result = invoke_apply(context,fnApply,classPtr,st,group,args,errors);
                });
                return result;
            } else {
                return true;
            }
        }
        virtual bool look (const StructurePtr & st, ModuleGroup & group,
            const AnnotationArgumentList & args, string & errors ) override {
            if ( auto fnFinish = get_finish(classPtr) ) {
                bool result = true;
                runMacroFunction(context, "finish", [&]() {
                    result = invoke_finish(context,fnFinish,classPtr,st,group,args,errors);
                });
                return result;
            } else {
                return true;
            }
        }
        virtual bool patch (const StructurePtr & st, ModuleGroup & group,
            const AnnotationArgumentList & args, string & errors, bool & astChanged ) override {
            if ( auto fnPatch = get_patch(classPtr) ) {
                bool result = true;
                runMacroFunction(context, "patch", [&]() {
                    result = invoke_patch(context,fnPatch,classPtr,st,group,args,errors,astChanged);
                });
                return result;
            } else {
                return true;
            }
        }
        virtual void  complete (  Context * ctx, const StructurePtr & stp ) override {
            if ( auto fnComplete = get_complete(classPtr) ) {
                runMacroFunction(context, "complete", [&]() {
                    invoke_complete(context,fnComplete,classPtr,stp,ctx);
                });
            }
        }
        virtual void aotPrefix ( const StructurePtr & st, const AnnotationArgumentList & args,TextWriter & writer ) override {
            if ( auto fnAotPrefix = get_aotPrefix(classPtr) ) {
                runMacroFunction(context, "aotPrefix", [&]() {
                    invoke_aotPrefix(context,fnAotPrefix,classPtr,st,args,reinterpret_cast<StringBuilderWriter&>(writer));
                });
            }
        }
        virtual void aotBody  ( const StructurePtr & st, const AnnotationArgumentList & args, TextWriter & writer ) override {
            if ( auto fnAotBody = get_aotBody(classPtr) ) {
                runMacroFunction(context, "aotBody", [&]() {
                    invoke_aotBody(context,fnAotBody,classPtr,st,args,reinterpret_cast<StringBuilderWriter&>(writer));
                });
            }

        }
        virtual void aotSuffix ( const StructurePtr & st, const AnnotationArgumentList & args,TextWriter & writer ) override {
            if ( auto fnAotSuffix = get_aotSuffix(classPtr) ) {
                runMacroFunction(context, "aotSuffix", [&]() {
                    invoke_aotSuffix(context,fnAotSuffix,classPtr,st,args,reinterpret_cast<StringBuilderWriter&>(writer));
                });
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    struct AstStructureAnnotationAnnotation : ManagedStructureAnnotation<StructureAnnotation,false,true> {
        AstStructureAnnotationAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("StructureAnnotation", ml) {
        }
    };

    struct EnumerationAnnotationAdapter : EnumerationAnnotation, AstEnumerationAnnotation_Adapter {
        EnumerationAnnotationAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
            : EnumerationAnnotation(n), AstEnumerationAnnotation_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual bool touch ( const EnumerationPtr & st, ModuleGroup & group,
                            const AnnotationArgumentList & args, string & errors ) override {
            if ( auto fnApply = get_apply(classPtr) ) {
                bool result = true;
                runMacroFunction(context, "apply", [&]() {
                    result = invoke_apply(context,fnApply,classPtr,st,group,args,errors);
                });
                return result;
            } else {
                return true;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    struct AstEnumerationAnnotationAnnotation : ManagedStructureAnnotation<EnumerationAnnotation,false,true> {
        AstEnumerationAnnotationAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("EnumerationAnnotation", ml) {
        }
    };

    struct PassMacroAdapter : PassMacro, AstPassMacro_Adapter {
        PassMacroAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
            : PassMacro(n), AstPassMacro_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual bool apply ( Program * prog, Module * mod ) override {
            if ( auto fnApply = get_apply(classPtr) ) {
                bool result = false;
                runMacroFunction(context, "apply", [&]() {
                    result = invoke_apply(context,fnApply,classPtr,prog,mod);
                });
                return result;
            } else {
                return false;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    struct AstPassMacroAnnotation : ManagedStructureAnnotation<PassMacro,false,true> {
        AstPassMacroAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("PassMacro", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
        }
    };

    struct VariantMacroAdapter : VariantMacro, AstVarianMacro_Adapter {
        VariantMacroAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
            : VariantMacro(n), AstVarianMacro_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual ExpressionPtr visitIs ( Program * prog, Module * mod, ExprIsVariant * expr ) override {
            if ( auto fnVisitIs = get_visitExprIsVariant(classPtr) ) {
                ExpressionPtr result;
                runMacroFunction(context, "visitExprIsVariant", [&]() {
                    result = invoke_visitExprIsVariant(context,fnVisitIs,classPtr,prog,mod,expr);
                });
                return result;
            } else {
                return nullptr;
            }
        }
        virtual ExpressionPtr visitAs ( Program * prog, Module * mod, ExprAsVariant * expr ) override {
            if ( auto fnVisitAs = get_visitExprAsVariant(classPtr) ) {
                ExpressionPtr result;
                runMacroFunction(context, "visitExprAsVariant", [&]() {
                    result = invoke_visitExprAsVariant(context,fnVisitAs,classPtr,prog,mod,expr);
                });
                return result;
            } else {
                return nullptr;
            }
        }
        virtual ExpressionPtr visitSafeAs ( Program * prog, Module * mod, ExprSafeAsVariant * expr ) override {
            if ( auto fnVisitSafeAs = get_visitExprSafeAsVariant(classPtr) ) {
                ExpressionPtr result;
                runMacroFunction(context, "visitExprSafeAsVariant", [&]() {
                    result = invoke_visitExprSafeAsVariant(context,fnVisitSafeAs,classPtr,prog,mod,expr);
                });
                return result;
            } else {
                return nullptr;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    struct AstVariantMacroAnnotation : ManagedStructureAnnotation<VariantMacro,false,true> {
        AstVariantMacroAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("VariantMacro", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
        }
    };

    struct ForLoopMacroAdapter : ForLoopMacro, AstForLoopMacro_Adapter {
        ForLoopMacroAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
            : ForLoopMacro(n), AstForLoopMacro_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual ExpressionPtr visit ( Program * prog, Module * mod, ExprFor * loop ) override {
            if ( auto fnVisit = get_visitExprFor(classPtr) ) {
                ExpressionPtr result;
                runMacroFunction(context, "visitExprFor", [&]() {
                    result = invoke_visitExprFor(context,fnVisit,classPtr,prog,mod,loop);
                });
                return result;
            } else {
                return nullptr;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    struct AstForLoopMacroAnnotation : ManagedStructureAnnotation<ForLoopMacro,false,true> {
        AstForLoopMacroAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("ForLoopMacro", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
        }
    };

    struct CaptureMacroAdapter : CaptureMacro, AstCaptureMacro_Adapter {
        CaptureMacroAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
            : CaptureMacro(n), AstCaptureMacro_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual ExpressionPtr captureExpression ( Program * prog, Module * mod, Expression * expr, TypeDecl * typ ) override {
            if ( auto fnCaptureExpression = get_captureExpression(classPtr) ) {
                ExpressionPtr result;
                runMacroFunction(context, "captureExpression", [&]() {
                    result = invoke_captureExpression(context,fnCaptureExpression,classPtr,prog,mod,expr,typ);
                });
                return result;
            } else {
                return nullptr;
            }
        }
        virtual void captureFunction ( Program * prog, Module * mod, Structure * lcs, Function * fun ) override {
            if ( auto fnCaptureFunction = get_captureFunction(classPtr) ) {
                runMacroFunction(context, "captureFunction", [&]() {
                    invoke_captureFunction(context,fnCaptureFunction,classPtr,prog,mod,lcs,fun);
                });
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    struct TypeMacroAdapter : TypeMacro, AstTypeMacro_Adapter {
        TypeMacroAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
            : TypeMacro(n), AstTypeMacro_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual TypeDeclPtr visit ( Program * prog, Module * mod, const TypeDeclPtr & typ, const TypeDeclPtr & passT ) override {
            if ( auto fnVisit = get_visit(classPtr) ) {
                TypeDeclPtr result;
                runMacroFunction(context, "visit", [&]() {
                    result = invoke_visit(context,fnVisit,classPtr,prog,mod,typ,passT);
                });
                return result;
            } else {
                return nullptr;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    struct AstCaptureMacroAnnotation : ManagedStructureAnnotation<CaptureMacro,false,true> {
        AstCaptureMacroAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("CaptureMacro", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
        }
    };

    struct AstTypeMacroAnnotation : ManagedStructureAnnotation<TypeMacro,false,true> {
        AstTypeMacroAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("TypeMacro", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
        }
    };

    struct SimulateMacroAdapter : SimulateMacro, AstSimulateMacro_Adapter {
        SimulateMacroAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
            : SimulateMacro(n), AstSimulateMacro_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual bool preSimulate ( Program * prog, Context * ctx ) override {
            if ( auto fnPreSimulate = get_preSimulate(classPtr) ) {
                bool result;
                runMacroFunction(context, "preSimulate", [&]() {
                    result = invoke_preSimulate(context,fnPreSimulate,classPtr,prog,ctx);
                });
                return result;
            } else {
                return true;
            }
        }
        virtual bool simulate ( Program * prog, Context * ctx ) override {
            if ( auto fnSimulate = get_simulate(classPtr) ) {
                bool result;
                runMacroFunction(context, "simulate", [&]() {
                    result = invoke_simulate(context,fnSimulate,classPtr,prog,ctx);
                });
                return result;
            } else {
                return true;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    struct AstSimulateMacroAnnotation : ManagedStructureAnnotation<SimulateMacro,false,true> {
        AstSimulateMacroAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("SimulateMacro", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
        }
    };

    struct ReaderMacroAdapter : ReaderMacro, AstReaderMacro_Adapter {
        ReaderMacroAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
            : ReaderMacro(n), AstReaderMacro_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual char * suffix ( Program * prog, Module * mod, ExprReader * expr, int & nextLine, FileInfo * & finfo, const LineInfo & info ) override {
            finfo = nullptr;
            if ( auto fnSuffix = get_suffix(classPtr) ) {
                char * result = nullptr;
                runMacroFunction(context, "suffix", [&]() {
                    result = invoke_suffix(context,fnSuffix,classPtr,prog,mod,expr,info,nextLine,finfo);
                });
                return result;
            } else {
                return nullptr;
            }
        }
        virtual bool accept ( Program * prog, Module * mod, ExprReader * expr, int Ch, const LineInfo & info ) override {
            if ( auto fnAccept = get_accept(classPtr) ) {
                bool result = false;
                runMacroFunction(context, "accept", [&]() {
                    result = invoke_accept(context,fnAccept,classPtr,prog,mod,expr,Ch,info);
                });
                return result;
            } else {
                return false;
            }
        }
        virtual ExpressionPtr visit (  Program * prog, Module * mod, ExprReader * expr ) override {
            if ( auto fnVisit = get_visit(classPtr) ) {
                ExpressionPtr result;
                runMacroFunction(context, "visit", [&]() {
                    result = invoke_visit(context,fnVisit,classPtr,prog,mod,expr);
                });
                return result;
            } else {
                return nullptr;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    struct AstReaderMacroAnnotation : ManagedStructureAnnotation<ReaderMacro,false,true> {
        AstReaderMacroAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("ReaderMacro", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(module)>("_module", "module");
        }
    };

    struct CommentReaderAdapter : CommentReader, AstCommentReader_Adapter {
        CommentReaderAdapter ( char * pClass, const StructInfo * info, Context * ctx )
            : AstCommentReader_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual void open ( bool cppStyle, const LineInfo & info ) override {
            if ( auto fnOpen = get_open(classPtr) ) {
                runMacroFunction(context, "open", [&]() {
                    invoke_open(context,fnOpen,classPtr,
                        daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            cppStyle,info);
                });
            }
        }
        virtual void accept ( int Ch, const LineInfo & info ) override {
            if ( auto fnAccept = get_accept(classPtr) ) {
                runMacroFunction(context, "accept", [&]() {
                    invoke_accept(context,fnAccept,classPtr,
                        daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            Ch,info);
                });
            }
        }
        virtual void close ( const LineInfo & info ) override {
            if ( auto fnClose = get_close(classPtr) ) {
                runMacroFunction(context, "close", [&]() {
                    invoke_close(context,fnClose,classPtr,
                        daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void beforeStructure ( const LineInfo & info ) override {
            if ( auto fnBeforeStructure = get_beforeStructure(classPtr) ) {
                runMacroFunction(context, "beforeStructure", [&]() {
                    invoke_beforeStructure(context,fnBeforeStructure,classPtr,
                        daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void afterStructure ( Structure * st, const LineInfo & info ) override {
            if ( auto fnAfterStructure = get_afterStructure(classPtr) ) {
                runMacroFunction(context, "afterStructure", [&]() {
                    invoke_afterStructure(context,fnAfterStructure,classPtr,
                        st, daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }

        }
        virtual void beforeFunction ( const LineInfo & info ) override {
            if ( auto fnBeforeFunction = get_beforeFunction(classPtr) ) {
                runMacroFunction(context, "beforeFunction", [&]() {
                    invoke_beforeFunction(context,fnBeforeFunction,classPtr,
                        daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void afterFunction ( Function * fn, const LineInfo & info ) override {
            if ( auto fnAfterFunction = get_afterFunction(classPtr) ) {
                runMacroFunction(context, "afterFunction", [&]() {
                    invoke_afterFunction(context,fnAfterFunction,classPtr,
                        fn, daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void beforeStructureFields ( const LineInfo & info ) override {
            if ( auto fnBeforeStructureFields = get_beforeStructureFields(classPtr) ) {
                runMacroFunction(context, "beforeStructureFields", [&]() {
                    invoke_beforeStructureFields(context,fnBeforeStructureFields,classPtr,
                        daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void afterStructureField ( const char * name, const LineInfo & info ) override {
            if ( auto fnAfterStructureField = get_afterStructureField(classPtr) ) {
                runMacroFunction(context, "afterStructureField", [&]() {
                    invoke_afterStructureField(context,fnAfterStructureField,classPtr,
                        (char *) name, daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void afterStructureFields ( const LineInfo & info ) override {
            if ( auto fnAfterStructureFields = get_afterStructureFields(classPtr) ) {
                runMacroFunction(context, "afterStructureFields", [&]() {
                    invoke_afterStructureFields(context,fnAfterStructureFields,classPtr,
                        daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void beforeGlobalVariables ( const LineInfo & info ) override {
            if ( auto fnGlobalVariables = get_beforeGlobalVariables(classPtr) ) {
                runMacroFunction(context, "beforeGlobalVariables", [&]() {
                    invoke_beforeGlobalVariables(context,fnGlobalVariables,classPtr,
                        daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void afterGlobalVariable ( const char * name, const LineInfo & info ) override {
            if ( auto fnGlobalVariable = get_afterGlobalVariable(classPtr) ) {
                runMacroFunction(context, "afterGlobalVariable", [&]() {
                    invoke_afterGlobalVariable(context,fnGlobalVariable,classPtr,
                        (char *) name, daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void afterGlobalVariables ( const LineInfo & info ) override {
            if ( auto fnGlobalVariables = get_afterGlobalVariables(classPtr) ) {
                runMacroFunction(context, "afterGlobalVariables", [&]() {
                    invoke_afterGlobalVariables(context,fnGlobalVariables,classPtr,
                        daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void beforeTuple ( const LineInfo & info ) override {
            if ( auto fnTuple = get_beforeTuple(classPtr) ) {
                runMacroFunction(context, "beforeTuple", [&]() {
                    invoke_beforeTuple(context,fnTuple,classPtr,
                        daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void beforeTupleEntries ( const LineInfo & info ) override {
            if ( auto fnTuple = get_beforeTupleEntries(classPtr) ) {
                runMacroFunction(context, "beforeTupleEntries", [&]() {
                    invoke_beforeTupleEntries(context,fnTuple,classPtr,
                        daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void afterTupleEntry ( const char * name, const LineInfo & info ) override {
            if ( auto fnTuple = get_afterTupleEntry(classPtr) ) {
                runMacroFunction(context, "afterTupleEntry", [&]() {
                    invoke_afterTupleEntry(context,fnTuple,classPtr,
                        (char *) name, daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void afterTupleEntries ( const LineInfo & info ) override {
            if ( auto fnTuple = get_afterTupleEntries(classPtr) ) {
                runMacroFunction(context, "afterTupleEntries", [&]() {
                    invoke_afterTupleEntries(context,fnTuple,classPtr,
                        daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void afterTuple ( const char * name, const LineInfo & info ) override {
            if ( auto fnTuple = get_afterTuple(classPtr) ) {
                runMacroFunction(context, "afterTuple", [&]() {
                    invoke_afterTuple(context,fnTuple,classPtr,
                        (char *) name, daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void beforeVariant ( const LineInfo & info ) override {
            if ( auto fnVariant = get_beforeVariant(classPtr) ) {
                runMacroFunction(context, "beforeVariant", [&]() {
                    invoke_beforeVariant(context,fnVariant,classPtr,
                        daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void beforeVariantEntries ( const LineInfo & info ) override {
            if ( auto fnVariant = get_beforeVariantEntries(classPtr) ) {
                runMacroFunction(context, "beforeVariantEntries", [&]() {
                    invoke_beforeVariantEntries(context,fnVariant,classPtr,
                        daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void afterVariantEntry ( const char * name, const LineInfo & info ) override {
            if ( auto fnVariant = get_afterVariantEntry(classPtr) ) {
                runMacroFunction(context, "afterVariantEntry", [&]() {
                    invoke_afterVariantEntry(context,fnVariant,classPtr,
                        (char *) name, daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void afterVariantEntries ( const LineInfo & info ) override {
            if ( auto fnVariant = get_afterVariantEntries(classPtr) ) {
                runMacroFunction(context, "afterVariantEntries", [&]() {
                    invoke_afterVariantEntries(context,fnVariant,classPtr,
                        daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void afterVariant ( const char * name, const LineInfo & info ) override {
            if ( auto fnVariant = get_afterVariant(classPtr) ) {
                runMacroFunction(context, "afterVariant", [&]() {
                    invoke_afterVariant(context,fnVariant,classPtr,
                        (char *) name, daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void beforeBitfield ( const LineInfo & info ) override {
            if ( auto fnBitfield = get_beforeBitfield(classPtr) ) {
                runMacroFunction(context, "beforeBitfield", [&]() {
                    invoke_beforeBitfield(context,fnBitfield,classPtr,
                        daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void beforeBitfieldEntries ( const LineInfo & info ) override {
            if ( auto fnBitfield = get_beforeBitfieldEntries(classPtr) ) {
                runMacroFunction(context, "beforeBitfieldEntries", [&]() {
                    invoke_beforeBitfieldEntries(context,fnBitfield,classPtr,
                        daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void afterBitfieldEntry ( const char * name, const LineInfo & info ) override {
            if ( auto fnBitfield = get_afterBitfieldEntry(classPtr) ) {
                runMacroFunction(context, "afterBitfieldEntry", [&]() {
                    invoke_afterBitfieldEntry(context,fnBitfield,classPtr,
                        (char *) name, daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void afterBitfieldEntries ( const LineInfo & info ) override {
            if ( auto fnBitfield = get_afterBitfieldEntries(classPtr) ) {
                runMacroFunction(context, "afterBitfieldEntries", [&]() {
                    invoke_afterBitfieldEntries(context,fnBitfield,classPtr,
                        daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void afterBitfield ( const char * name, const LineInfo & info ) override {
            if ( auto fnBitfield = get_afterBitfield(classPtr) ) {
                runMacroFunction(context, "afterBitfield", [&]() {
                    invoke_afterBitfield(context,fnBitfield,classPtr,
                        (char *) name, daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void beforeEnumeration ( const LineInfo & info ) override {
            if ( auto fnEnum = get_beforeEnumeration(classPtr) ) {
                runMacroFunction(context, "beforeEnumeration", [&]() {
                    invoke_beforeEnumeration(context,fnEnum,classPtr,
                        daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void beforeEnumerationEntries ( const LineInfo & info ) override {
            if ( auto fnEnum = get_beforeEnumerationEntries(classPtr) ) {
                runMacroFunction(context, "beforeEnumerationEntries", [&]() {
                    invoke_beforeEnumerationEntries(context,fnEnum,classPtr,
                        daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void afterEnumerationEntry ( const char * name, const LineInfo & info ) override {
            if ( auto fnEnum = get_afterEnumerationEntry(classPtr) ) {
                runMacroFunction(context, "afterEnumerationEntry", [&]() {
                    invoke_afterEnumerationEntry(context,fnEnum,classPtr,
                        (char *) name, daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void afterEnumerationEntries ( const LineInfo & info ) override {
            if ( auto fnEnum = get_afterEnumerationEntries(classPtr) ) {
                runMacroFunction(context, "afterEnumerationEntries", [&]() {
                    invoke_afterEnumerationEntries(context,fnEnum,classPtr,
                        daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void afterEnumeration ( const char * name, const LineInfo & info ) override {
            if ( auto fnEnum = get_afterEnumeration(classPtr) ) {
                runMacroFunction(context, "afterEnumeration", [&]() {
                    invoke_afterEnumeration(context,fnEnum,classPtr,
                        (char *) name, daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void beforeAlias ( const LineInfo & info ) override {
            if ( auto fnAlias = get_beforeAlias(classPtr) ) {
                runMacroFunction(context, "beforeAlias", [&]() {
                    invoke_beforeAlias(context,fnAlias,classPtr,
                        daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
        virtual void afterAlias ( const char * name, const LineInfo & info ) override {
            if ( auto fnAlias = get_afterAlias(classPtr) ) {
                runMacroFunction(context, "afterAlias", [&]() {
                    invoke_afterAlias(context,fnAlias,classPtr,
                        (char *) name, daScriptEnvironment::bound->g_Program,
                        daScriptEnvironment::bound->g_Program->thisModule.get(),
                            info);
                });
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    struct AstCommentReaderAnnotation : ManagedStructureAnnotation<CommentReader,false,true> {
        AstCommentReaderAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("CommentReader", ml) {
        }
    };

    struct CallMacroAdapter : CallMacro, AstCallMacro_Adapter {
        CallMacroAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
            : CallMacro(n), AstCallMacro_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual void preVisit (  Program * prog, Module * mod, ExprCallMacro * expr ) override {
            if ( auto fnPreVisit = get_preVisit(classPtr) ) {
                runMacroFunction(context, "preVisit", [&]() {
                    invoke_preVisit(context,fnPreVisit,classPtr,prog,mod,expr);
                });
            }
        }
        virtual ExpressionPtr visit (  Program * prog, Module * mod, ExprCallMacro * expr ) override {
            if ( auto fnVisit = get_visit(classPtr) ) {
                ExpressionPtr result;
                runMacroFunction(context, "visit", [&]() {
                    result = invoke_visit(context,fnVisit,classPtr,prog,mod,expr);
                });
                return result;
            } else {
                return nullptr;
            }
        }
        virtual bool canVisitArguments ( ExprCallMacro * expr, int index ) override {
            if ( auto fncanVisitArguments = get_canVisitArgument(classPtr) ) {
                bool result = true;
                runMacroFunction(context, "canVisitArguments", [&]() {
                    result = invoke_canVisitArgument(context,fncanVisitArguments,classPtr,expr,index);
                });
                return result;
            } else {
                return true;
            }
        }
        virtual bool canFoldReturnResult ( ExprCallMacro * expr ) override {
            if ( auto fnCanFoldResult = get_canFoldReturnResult(classPtr) ) {
                bool result = true;
                runMacroFunction(context, "canFoldReturnResult", [&]() {
                    result = invoke_canFoldReturnResult(context,fnCanFoldResult,classPtr,expr);
                });
                return result;
            } else {
                return true;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    struct AstCallMacroAnnotation : ManagedStructureAnnotation<CallMacro,false,true> {
        AstCallMacroAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("CallMacro", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(module)>("_module", "module");
        }
    };
    struct TypeInfoMacroAdapter : TypeInfoMacro, AstTypeInfoMacro_Adapter {
        TypeInfoMacroAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
            : TypeInfoMacro(n), AstTypeInfoMacro_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual ExpressionPtr getAstChange ( const ExpressionPtr & expr, string & err ) override {
            if ( auto fnGetAstChange = get_getAstChange(classPtr) ) {
                ExpressionPtr result;
                runMacroFunction(context, "getAstChange", [&]() {
                    auto tinfo = static_pointer_cast<ExprTypeInfo>(expr);
                    result = invoke_getAstChange(context,fnGetAstChange,classPtr,tinfo,err);
                });
                return result;
            } else {
                return nullptr;
            }
        }
        virtual TypeDeclPtr getAstType ( ModuleLibrary & lib, const ExpressionPtr & expr, string & err ) override {
            if ( auto fnGetAstType = get_getAstType(classPtr) ) {
                TypeDeclPtr result;
                runMacroFunction(context, "getAstType", [&]() {
                    auto tinfo = static_pointer_cast<ExprTypeInfo>(expr);
                    result = invoke_getAstType(context,fnGetAstType,classPtr,lib,tinfo,err);
                });
                return result;
            } else {
                return nullptr;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    ReaderMacroPtr makeReaderMacro ( const char * name, const void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<ReaderMacroAdapter>(name,(char *)pClass,info,context);
    }

    void addModuleReaderMacro ( Module * module, ReaderMacroPtr & _newM, Context * context, LineInfoArg * at ) {
        ReaderMacroPtr newM = das::move(_newM);
        if ( !module->addReaderMacro(newM, true) ) {
            context->throw_error_at(at, "can't add reader macro %s to module %s", newM->name.c_str(), module->name.c_str());
        }
    }

    CommentReaderPtr makeCommentReader ( const void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<CommentReaderAdapter>((char *)pClass,info,context);
    }

    void addModuleCommentReader ( Module * module, CommentReaderPtr & _newM, Context * context, LineInfoArg * at ) {
        CommentReaderPtr newM = das::move(_newM);
        if ( !module->addCommentReader(newM, true) ) {
            context->throw_error_at(at, "can't add comment reader to module %s", module->name.c_str());
        }
    }

    CallMacroPtr makeCallMacro ( const char * name, const void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<CallMacroAdapter>(name,(char *)pClass,info,context);
    }

    void addModuleCallMacro ( Module * module, CallMacroPtr & _newM, Context * context, LineInfoArg * at ) {
        CallMacroPtr newM = das::move(_newM);
        if ( ! module->addCallMacro(newM->name, [=](const LineInfo & at) -> ExprLooksLikeCall * {
            auto ecm = new ExprCallMacro(at, newM->name);
            ecm->macro = newM.get();
            newM->module = module;
            return ecm;
        }) ) {
            context->throw_error_at(at, "can't add call macro %s to module %s", newM->name.c_str(), module->name.c_str());
        }
    }

    TypeInfoMacroPtr makeTypeInfoMacro ( const char * name, const void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<TypeInfoMacroAdapter>(name,(char *)pClass,info,context);
    }

    void addModuleTypeInfoMacro ( Module * module, TypeInfoMacroPtr & _newM, Context * context, LineInfoArg * at ) {
        TypeInfoMacroPtr newM = das::move(_newM);
        if ( ! module->addTypeInfoMacro(newM,true) ) {
            context->throw_error_at(at, "can't add type info macro %s to module %s", newM->name.c_str(), module->name.c_str());
        }
    }

    smart_ptr<VisitorAdapter> makeVisitor ( const void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<VisitorAdapter>((char *)pClass,info,context);
    }

    PassMacroPtr makePassMacro ( const char * name, const void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<PassMacroAdapter>(name,(char *)pClass,info,context);
    }

    VariantMacroPtr makeVariantMacro ( const char * name, const void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<VariantMacroAdapter>(name,(char *)pClass,info,context);
    }

    void addModuleVariantMacro ( Module * module, VariantMacroPtr & _newM, Context * ) {
        VariantMacroPtr newM = das::move(_newM);
        module->variantMacros.push_back(newM);
    }

    ForLoopMacroPtr makeForLoopMacro ( const char * name, const void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<ForLoopMacroAdapter>(name,(char *)pClass,info,context);
    }

    void addModuleForLoopMacro ( Module * module, ForLoopMacroPtr & _newM, Context * ) {
        ForLoopMacroPtr newM = das::move(_newM);
        module->forLoopMacros.push_back(newM);
    }

    CaptureMacroPtr makeCaptureMacro ( const char * name, const void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<CaptureMacroAdapter>(name,(char *)pClass,info,context);
    }

    void addModuleCaptureMacro ( Module * module, CaptureMacroPtr & _newM, Context * ) {
        CaptureMacroPtr newM = das::move(_newM);
        module->captureMacros.push_back(newM);
    }

    TypeMacroPtr makeTypeMacro ( const char * name, const void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<TypeMacroAdapter>(name,(char *)pClass,info,context);
    }

    void addModuleTypeMacro ( Module * module, TypeMacroPtr & _newM, Context * ctx, LineInfoArg * at ) {
        auto it = module->typeMacros.find(_newM->name);
        if ( it != module->typeMacros.end() ) {
            ctx->throw_error_at(at, "type macro %s already exists in module %s", _newM->name.c_str(), module->name.c_str());
        }
        string name = _newM->name;
        module->typeMacros[name] = das::move(_newM);
    }

    SimulateMacroPtr makeSimulateMacro ( const char * name, const void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<SimulateMacroAdapter>(name,(char *)pClass,info,context);
    }

    void addModuleSimulateMacro ( Module * module, SimulateMacroPtr & _newM, Context * ) {
        SimulateMacroPtr newM = das::move(_newM);
        module->simulateMacros.push_back(newM);
    }

    void addModuleInferMacro ( Module * module, PassMacroPtr & _newM, Context * ) {
        PassMacroPtr newM = das::move(_newM);
        module->macros.push_back(newM);
    }

    void addModuleInferDirtyMacro ( Module * module, PassMacroPtr & _newM, Context * ) {
        PassMacroPtr newM = das::move(_newM);
        module->inferMacros.push_back(newM);
    }

    void addModuleLintMacro ( Module * module, PassMacroPtr & _newM, Context * ) {
        PassMacroPtr newM = das::move(_newM);
        module->lintMacros.push_back(newM);
    }

    void addModuleGlobalLintMacro ( Module * module, PassMacroPtr & _newM, Context * ) {
        PassMacroPtr newM = das::move(_newM);
        module->globalLintMacros.push_back(newM);
    }

    void addModuleOptimizationMacro ( Module * module, PassMacroPtr & _newM, Context * ) {
        PassMacroPtr newM = das::move(_newM);
        module->optimizationMacros.push_back(newM);
    }

    EnumerationAnnotationPtr makeEnumerationAnnotation ( const char * name, void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<EnumerationAnnotationAdapter>(name,(char *)pClass,info,context);
    }

    void addModuleEnumerationAnnotation ( Module * module, EnumerationAnnotationPtr & _ann, Context * context, LineInfoArg * at ) {
        EnumerationAnnotationPtr ann = das::move(_ann);
        if ( !module->addAnnotation(ann, true) ) {
            context->throw_error_at(at, "can't add enumeration annotation %s to module %s",
                ann->name.c_str(), module->name.c_str());
        }
    }

    int addEnumerationEntry ( smart_ptr<Enumeration> enu, const char* name ) {
        if ( !name ) return -1;
        for ( auto & ee : enu->list ) {
            if ( ee.name==name ) {
                return -1;
            }
        }
        enu->list.push_back(Enumeration::EnumEntry());
        int index = (int)enu->list.size() - 1;
        enu->list[index].name = name;
        return index;
    }

    StructureAnnotationPtr makeStructureAnnotation ( const char * name, void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<StructureAnnotationAdapter>(name,(char *)pClass,info,context);
    }

    void addModuleStructureAnnotation ( Module * module, StructureAnnotationPtr & _ann, Context * context, LineInfoArg * at ) {
        StructureAnnotationPtr ann = das::move(_ann);
        if ( !module->addAnnotation(ann, true) ) {
            context->throw_error_at(at, "can't add structure annotation %s to module %s",
                ann->name.c_str(), module->name.c_str());
        }
    }

    void addStructureStructureAnnotation ( smart_ptr_raw<Structure> st, StructureAnnotationPtr & _ann, Context * context, LineInfoArg * at ) {
        StructureAnnotationPtr ann = das::move(_ann);
        string err;
        ModuleGroup dummy;
        if ( !ann->touch(st, dummy, AnnotationArgumentList(), err) ) {
            context->throw_error_at(at, "annotation %s failed to apply to structure %s",
                ann->name.c_str(), st->name.c_str());
        }
        auto annDecl = make_smart<AnnotationDeclaration>();
        annDecl->annotation = ann;
        st->annotations.push_back(annDecl);
    }

    FunctionAnnotationPtr makeBlockAnnotation ( const char * name, void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<BlockAnnotationAdapter>(name,(char *)pClass,info,context);
    }

    FunctionAnnotationPtr makeFunctionAnnotation ( const char * name, void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<FunctionAnnotationAdapter>(name,(char *)pClass,info,context);
    }

    void addModuleFunctionAnnotation ( Module * module, FunctionAnnotationPtr & _ann, Context * context, LineInfoArg * at ) {
        FunctionAnnotationPtr ann = das::move(_ann);
        if ( !module->addAnnotation(ann, true) ) {
            context->throw_error_at(at, "can't add function annotation %s to module %s",
                ann->name.c_str(), module->name.c_str());
        }
    }

    void addFunctionFunctionAnnotation ( smart_ptr_raw<Function> func, FunctionAnnotationPtr & _ann, Context * context, LineInfoArg * at ) {
        FunctionAnnotationPtr ann = das::move(_ann);
        string err;
        ModuleGroup dummy;
        if ( !ann->apply(func, dummy, AnnotationArgumentList(), err) ) {
            context->throw_error_at(at, "annotation %s failed to apply to function %s",
                ann->name.c_str(), func->name.c_str());
        }
        auto annDecl = make_smart<AnnotationDeclaration>();
        annDecl->annotation = ann;
        func->annotations.push_back(annDecl);
    }

    void addBlockBlockAnnotation ( smart_ptr_raw<ExprBlock> blk, FunctionAnnotationPtr & _ann, Context * context, LineInfoArg * at ) {
        FunctionAnnotationPtr ann = das::move(_ann);
        string err;
        ModuleGroup dummy;
        if ( !ann->apply(blk.ptr, dummy, AnnotationArgumentList(), err) ) {
            context->throw_error_at(at, "annotation %s failed to apply to block %s",
                ann->name.c_str(), blk->at.describe().c_str());
        }
        auto annDecl = make_smart<AnnotationDeclaration>();
        annDecl->annotation = ann;
        blk->annotations.push_back(annDecl);
    }

    void addAndApplyFunctionAnnotation ( smart_ptr_raw<Function> func, smart_ptr_raw<AnnotationDeclaration> & ann, Context * context, LineInfoArg * at ) {
        string err;
        if (!ann->annotation->rtti_isFunctionAnnotation()) {
            context->throw_error_at(at, "annotation %s failed to apply to function %s, not a FunctionAnnotation",
                ann->annotation->name.c_str(), func->name.c_str());
        }
        auto fAnn = (FunctionAnnotation*)ann->annotation.get();
        auto program = daScriptEnvironment::bound->g_Program;
        if ( !fAnn->apply(func, *program->thisModuleGroup, ann->arguments, err) ) {
            context->throw_error_at(at, "annotation %s failed to apply to function %s",
                ann->annotation->name.c_str(), func->name.c_str());
        }
        func->annotations.push_back(ann);
    }

    void addAndApplyBlockAnnotation ( smart_ptr_raw<ExprBlock> blk, smart_ptr_raw<AnnotationDeclaration> & ann, Context * context, LineInfoArg * at ) {
        string err;
        if (!ann->annotation->rtti_isFunctionAnnotation()) {
            context->throw_error_at(at, "annotation %s failed to apply to block %s, not a FunctionAnnotation",
                ann->annotation->name.c_str(), blk->at.describe().c_str());
        }
        auto fAnn = (FunctionAnnotation*)ann->annotation.get();
        auto program = daScriptEnvironment::bound->g_Program;
        if ( !fAnn->apply(blk.ptr, *program->thisModuleGroup, ann->arguments, err) ) {
            context->throw_error_at(at, "annotation %s failed to apply to block %s",
                ann->annotation->name.c_str(), blk->at.describe().c_str());
        }
        blk->annotations.push_back(ann);
    }

    void addAndApplyStructAnnotation ( smart_ptr_raw<Structure> st, smart_ptr_raw<AnnotationDeclaration> & ann, Context * context, LineInfoArg * at ) {
        string err;
        if (!ann->annotation->rtti_isStructureAnnotation()) {
            context->throw_error_at(at, "annotation %s failed to apply to struct %s, not a StructureAnnotation",
                ann->annotation->name.c_str(), st->name.c_str());
        }
        auto stAnn = (StructureAnnotation*)ann->annotation.get();
        auto program = daScriptEnvironment::bound->g_Program;
        if ( !stAnn->touch(st, *program->thisModuleGroup, ann->arguments, err) ) {
            context->throw_error_at(at, "annotation %s failed to apply to struct %s",
                ann->annotation->name.c_str(), st->name.c_str());
        }
        st->annotations.push_back(ann);
    }

    void astVisit ( smart_ptr_raw<Program> program, smart_ptr_raw<VisitorAdapter> adapter, Context * context, LineInfoArg * line_info ) {
        if (!adapter)
            context->throw_error_at(line_info, "adapter is required");
        if (!program)
            context->throw_error_at(line_info, "program is required");
        program->visit(*adapter);
    }

    void astVisitModulesInOrder ( smart_ptr_raw<Program> program, smart_ptr_raw<VisitorAdapter> adapter, Context * context, LineInfoArg * line_info ) {
        if (!adapter)
            context->throw_error_at(line_info, "adapter is required");
        if (!program)
            context->throw_error_at(line_info, "program is required");
        program->visitModulesInOrder(*adapter);
    }

    void astVisitFunction ( smart_ptr_raw<Function> func, smart_ptr_raw<VisitorAdapter> adapter, Context * context, LineInfoArg * line_info ) {
        if (!adapter)
            context->throw_error_at(line_info, "adapter is required");
        if (!func)
            context->throw_error_at(line_info, "func is required");
        func->visit(*adapter);
    }

    smart_ptr_raw<TypeDecl> astVisitTypeDecl ( smart_ptr_raw<TypeDecl> expr, smart_ptr_raw<VisitorAdapter> adapter, Context * context, LineInfoArg * line_info ) {
        if (!adapter)
            context->throw_error_at(line_info, "adapter is required");
        if (!expr)
            context->throw_error_at(line_info, "expr is required");
        adapter->preVisit(expr.get());
        expr->visit(*adapter);
        smart_ptr<TypeDecl> res = adapter->visit(expr.get());
        if ( res.get()!=expr.get() ) {
            DAS_VERIFYF(res->use_count()==1,"visitor returns new value, refcount must be 1 or else there will be a leak");
            res->addRef();
        }
        return res;
    }

    smart_ptr_raw<Expression> astVisitExpression ( smart_ptr_raw<Expression> expr, smart_ptr_raw<VisitorAdapter> adapter, Context * context, LineInfoArg * line_info ) {
        if (!adapter)
            context->throw_error_at(line_info, "adapter is required");
        if (!expr)
            context->throw_error_at(line_info, "expr is required");
        smart_ptr<Expression> res = expr->visit(*adapter);
        if ( res.get()!=expr.get() ) {
            DAS_VERIFYF(res->use_count()==1,"visitor returns new value, refcount must be 1 or else there will be a leak");
            res->addRef();
        }
        return res;
    }

    void astVisitBlockFinally ( smart_ptr_raw<ExprBlock> expr, smart_ptr_raw<VisitorAdapter> adapter, Context * context, LineInfoArg * line_info ) {
        if (!adapter)
            context->throw_error_at(line_info, "adapter is required");
        if (!expr)
            context->throw_error_at(line_info, "expr is required");
        expr->visitFinally(*adapter);
    }

    void Module_Ast::registerAdapterAnnotations(ModuleLibrary & lib) {
        // visitor
        addAnnotation(make_smart<AstVisitorAdapterAnnotation>(lib));
        addExtern<DAS_BIND_FUN(makeVisitor)>(*this, lib,  "make_visitor",
            SideEffects::modifyExternal, "makeVisitor")
                ->args({"class","info","context"});
        addExtern<DAS_BIND_FUN(astVisit)>(*this, lib,  "visit",
            SideEffects::accessExternal, "astVisit")
                ->args({"program","adapter","context","line"});
        addExtern<DAS_BIND_FUN(astVisitModulesInOrder)>(*this, lib,  "visit_modules",
            SideEffects::accessExternal, "astVisitModules")
                ->args({"program","adapter","context","line"});
        addExtern<DAS_BIND_FUN(astVisitFunction)>(*this, lib,  "visit",
            SideEffects::accessExternal, "astVisitFunction")
                ->args({"function","adapter","context","line"});
        addExtern<DAS_BIND_FUN(astVisitExpression)>(*this, lib,  "visit",
            SideEffects::accessExternal, "astVisitExpression")
                ->args({"expression","adapter","context","line"});
        addExtern<DAS_BIND_FUN(astVisitTypeDecl)>(*this, lib,  "visit",
            SideEffects::accessExternal, "astVisitTypeDecl")
                ->args({"expression","adapter","context","line"});
        addExtern<DAS_BIND_FUN(astVisitBlockFinally)>(*this, lib,  "visit_finally",
            SideEffects::accessExternal, "astVisitBlockFinally")
                ->args({"expression","adapter","context","line"});
        // function annotation
        addAnnotation(make_smart<AstFunctionAnnotationAnnotation>(lib));
        addExtern<DAS_BIND_FUN(makeFunctionAnnotation)>(*this, lib,  "make_function_annotation",
            SideEffects::modifyExternal, "makeFunctionAnnotation")
                ->args({"name","class","info","context"});
        addExtern<DAS_BIND_FUN(makeBlockAnnotation)>(*this, lib,  "make_block_annotation",
            SideEffects::modifyExternal, "makeBlockAnnotation")
                ->args({"name","class","info","context"});
        addExtern<DAS_BIND_FUN(addModuleFunctionAnnotation)>(*this, lib,  "add_function_annotation",
            SideEffects::modifyExternal, "addModuleFunctionAnnotation")
                ->args({"module","annotation","context","at"});
        addExtern<DAS_BIND_FUN(addFunctionFunctionAnnotation)>(*this, lib,  "add_function_annotation",
            SideEffects::modifyExternal, "addFunctionFunctionAnnotation")
                ->args({"function","annotation","context","at"});
        addExtern<DAS_BIND_FUN(addAndApplyFunctionAnnotation)>(*this, lib,  "add_function_annotation",
            SideEffects::modifyExternal, "addAndApplyFunctionAnnotation")
                ->args({"function","annotation","context","at"});
        // block annotation
        addExtern<DAS_BIND_FUN(addBlockBlockAnnotation)>(*this, lib,  "add_block_annotation",
            SideEffects::modifyExternal, "addBlockBlockAnnotation")
                ->args({"block","annotation","context","at"});
        addExtern<DAS_BIND_FUN(addAndApplyBlockAnnotation)>(*this, lib,  "add_block_annotation",
            SideEffects::modifyExternal, "addAndApplyBlockAnnotation")
                ->args({"block","annotation","context","at"});
        // structure annotation
        addAnnotation(make_smart<AstStructureAnnotationAnnotation>(lib));
        addExtern<DAS_BIND_FUN(makeStructureAnnotation)>(*this, lib,  "make_structure_annotation",
            SideEffects::modifyExternal, "makeStructureAnnotation")
                ->args({"name","class","info","context"});
        addExtern<DAS_BIND_FUN(addModuleStructureAnnotation)>(*this, lib,  "add_structure_annotation",
            SideEffects::modifyExternal, "addModuleStructureAnnotation")
                ->args({"module","annotation","context","at"});
        addExtern<DAS_BIND_FUN(addStructureStructureAnnotation)>(*this, lib,  "add_structure_annotation",
            SideEffects::modifyExternal, "addStructureStructureAnnotation")
                ->args({"structure","annotation","context","at"});
        addExtern<DAS_BIND_FUN(addAndApplyStructAnnotation)>(*this, lib,  "add_structure_annotation",
            SideEffects::modifyExternal, "addAndApplyStructAnnotation")
                ->args({"structure","annotation","context","at"});
        // enumeration annotation
        addAnnotation(make_smart<AstEnumerationAnnotationAnnotation>(lib));
        addExtern<DAS_BIND_FUN(makeEnumerationAnnotation)>(*this, lib,  "make_enumeration_annotation",
            SideEffects::modifyExternal, "makeEnumerationAnnotation")
                ->args({"name","class","info","context"});
        addExtern<DAS_BIND_FUN(addModuleEnumerationAnnotation)>(*this, lib,  "add_enumeration_annotation",
            SideEffects::modifyExternal, "addModuleEnumerationAnnotation")
                ->args({"module","annotation","context","at"});
        addExtern<DAS_BIND_FUN(addEnumerationEntry)>(*this, lib,  "add_enumeration_entry",
            SideEffects::modifyExternal, "addEnumerationEntry")
                ->args({"enum","name"});
        // pass macro
        addAnnotation(make_smart<AstPassMacroAnnotation>(lib));
        addExtern<DAS_BIND_FUN(makePassMacro)>(*this, lib,  "make_pass_macro",
            SideEffects::modifyExternal, "makePassMacro")
                ->args({"name","class","info","context"});
        addExtern<DAS_BIND_FUN(addModuleInferMacro)>(*this, lib,  "add_infer_macro",
            SideEffects::modifyExternal, "addModuleInferMacro")
                ->args({"module","annotation","context"});
        addExtern<DAS_BIND_FUN(addModuleInferDirtyMacro)>(*this, lib,  "add_dirty_infer_macro",
            SideEffects::modifyExternal, "addModuleInferDirtyMacro")
                ->args({"module","annotation","context"});
        // lint macro
        addExtern<DAS_BIND_FUN(addModuleLintMacro)>(*this, lib,  "add_lint_macro",
            SideEffects::modifyExternal, "addModuleLintMacro")
                ->args({"module","annotation","context"});
        addExtern<DAS_BIND_FUN(addModuleGlobalLintMacro)>(*this, lib,  "add_global_lint_macro",
            SideEffects::modifyExternal, "addModuleGlobalLintMacro")
                ->args({"module","annotation","context"});
        // optimization macro
        addExtern<DAS_BIND_FUN(addModuleOptimizationMacro)>(*this, lib,  "add_optimization_macro",
            SideEffects::modifyExternal, "addModuleOptimizationMacro")
                ->args({"module","annotation","context"});
        // reader macro
        addAnnotation(make_smart<AstReaderMacroAnnotation>(lib));
        addExtern<DAS_BIND_FUN(makeReaderMacro)>(*this, lib,  "make_reader_macro",
            SideEffects::modifyExternal, "makeReaderMacro")
                ->args({"name","class","info","context"});
        addExtern<DAS_BIND_FUN(addModuleReaderMacro)>(*this, lib,  "add_reader_macro",
            SideEffects::modifyExternal, "addModuleReaderMacro")
                ->args({"module","annotation","context","at"});
        // comment reader
        addAnnotation(make_smart<AstCommentReaderAnnotation>(lib));
        addExtern<DAS_BIND_FUN(makeCommentReader)>(*this, lib,  "make_comment_reader",
            SideEffects::modifyExternal, "makeCommentReader")
                ->args({"class","info","context"});
        addExtern<DAS_BIND_FUN(addModuleCommentReader)>(*this, lib,  "add_comment_reader",
            SideEffects::modifyExternal, "addModuleCommentReader")
                ->args({"module","reader","context","at"});
        // call macro
        addAnnotation(make_smart<AstCallMacroAnnotation>(lib));
        addExtern<DAS_BIND_FUN(makeCallMacro)>(*this, lib,  "make_call_macro",
            SideEffects::modifyExternal, "makeCallMacro")
                ->args({"name","class","info","context"});
        addExtern<DAS_BIND_FUN(addModuleCallMacro)>(*this, lib,  "add_call_macro",
            SideEffects::modifyExternal, "addModuleCallMacro")
                ->args({"module","annotation","context","at"});
        // type info macro
        addExtern<DAS_BIND_FUN(makeTypeInfoMacro)>(*this, lib,  "make_typeinfo_macro",
            SideEffects::modifyExternal, "makeTypeInfoMacro")
                ->args({"name","class","info","context"});
        addExtern<DAS_BIND_FUN(addModuleTypeInfoMacro)>(*this, lib,  "add_typeinfo_macro",
            SideEffects::modifyExternal, "addModuleTypeInfoMacro")
                ->args({"module","annotation","context","at"});
        // variant macro
        addAnnotation(make_smart<AstVariantMacroAnnotation>(lib));
        addExtern<DAS_BIND_FUN(makeVariantMacro)>(*this, lib,  "make_variant_macro",
            SideEffects::modifyExternal, "makeVariantMacro")
                ->args({"name","class","info","context"});
        addExtern<DAS_BIND_FUN(addModuleVariantMacro)>(*this, lib,  "add_variant_macro",
            SideEffects::modifyExternal, "addModuleVariantMacro")
                ->args({"module","annotation","context"});
        // for loop macro
        addAnnotation(make_smart<AstForLoopMacroAnnotation>(lib));
        addExtern<DAS_BIND_FUN(makeForLoopMacro)>(*this, lib,  "make_for_loop_macro",
            SideEffects::modifyExternal, "makeForLoopMacro")
                ->args({"name","class","info","context"});
        addExtern<DAS_BIND_FUN(addModuleForLoopMacro)>(*this, lib,  "add_for_loop_macro",
            SideEffects::modifyExternal, "addModuleForLoopMacro")
                ->args({"module","annotation","context"});
        // capture macro
        addAnnotation(make_smart<AstCaptureMacroAnnotation>(lib));
        addExtern<DAS_BIND_FUN(makeCaptureMacro)>(*this, lib,  "make_capture_macro",
            SideEffects::modifyExternal, "makeCaptureMacro")
                ->args({"name","class","info","context"});
        addExtern<DAS_BIND_FUN(addModuleCaptureMacro)>(*this, lib,  "add_capture_macro",
            SideEffects::modifyExternal, "addModuleCaptureMacro")
                ->args({"module","annotation","context"});
        // type macro
        addAnnotation(make_smart<AstTypeMacroAnnotation>(lib));
        addExtern<DAS_BIND_FUN(makeTypeMacro)>(*this, lib,  "make_type_macro",
            SideEffects::modifyExternal, "makeTypeMacro")
                ->args({"name","class","info","context"});
        addExtern<DAS_BIND_FUN(addModuleTypeMacro)>(*this, lib,  "add_type_macro",
            SideEffects::modifyExternal, "addModuleTypeMacro")
                ->args({"module","annotation","context","at"});
        // simulate macro macro
        addAnnotation(make_smart<AstSimulateMacroAnnotation>(lib));
        addExtern<DAS_BIND_FUN(makeSimulateMacro)>(*this, lib,  "make_simulate_macro",
            SideEffects::modifyExternal, "makeSimulateMacro")
                ->args({"name","class","info","context"});
        addExtern<DAS_BIND_FUN(addModuleSimulateMacro)>(*this, lib,  "add_simulate_macro",
            SideEffects::modifyExternal, "addModuleSimulateMacro")
                ->args({"module","annotation","context"});
    }
}
