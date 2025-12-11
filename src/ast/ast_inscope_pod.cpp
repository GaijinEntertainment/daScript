#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_visitor.h"
#include "daScript/ast/ast_generate.h"

namespace das {

    class PodVisitor : public Visitor {
    public:
        bool anyWork = false;
        PodVisitor ( TextWriter * logs_ = nullptr ) : logs(logs_) {}
    protected:
        virtual bool canVisitFunction ( Function * fun ) override {
            if ( fun->fromGeneric && fun->fromGeneric->module->name=="$" ) return false;
            return !fun->stub && !fun->isTemplate;    // we don't do a thing with templates
        }
        virtual void preVisit ( Function * fun ) override {
            Visitor::preVisit(fun);
            func = fun;
        }
        virtual FunctionPtr visit ( Function * fun ) override {
            func = nullptr;
            return Visitor::visit(fun);
        }
        virtual void preVisit ( ExprBlock * block ) override {
            Visitor::preVisit(block);
            blocks.push_back(block);
        }
        virtual ExpressionPtr visit ( ExprBlock * block ) override {
            blocks.pop_back();
            return Visitor::visit(block);
        }
        virtual VariablePtr visitLet ( ExprLet * expr, const VariablePtr & var, bool last ) override {
            if ( func && var->pod_delete && !var->pod_delete_gen ) {
                anyWork = true;
                var->pod_delete_gen = true;
                if ( var->single_return_via_move ) {
                    // we silently do nothing. because this pod is returned via move, no need to collect it
                } else if ( func && (func->generated || func->generator || func->lambda || func->hasTryRecover || func->hasUnsafe || !func->module->allowPodInscope || blocks.back()->inTheLoop) ) {
                    if ( logs ) {
                        if ( !var->at.empty() && var->at.fileInfo ) {
                            *logs << var->at.fileInfo->name << ":" << var->at.line << ":" << var->at.column << "\n";
                        }
                        *logs << "warning: POD optimization failed for " << var->name << "' in function '" << func->module->name << "::" << func->name << "'\n";
                        if ( func->generated ) *logs << "\tfunction is generated\n";
                        if ( func->generator ) *logs << "\tfunction is generator\n";
                        if ( func->lambda ) *logs << "\tfunction is lambda\n";
                        if ( func->hasTryRecover ) *logs << "\tfunction has try-recover\n";
                        if ( func->hasUnsafe ) *logs << "\tfunction has unsafe\n";
                        if ( !func->module->allowPodInscope ) *logs << "\tmodule " << func->module->name << " does not allow in-scope POD\n";
                        if ( blocks.back()->inTheLoop ) *logs << "\tblock is in the loop, which does not have separate 'finally' scope. you can add 'if ( true )' to create scope\n";
                    }
                } else {
                    func->notInferred();
                    auto CallCollectLocal = make_smart<ExprCall>(expr->at,"_::builtin_collect_local");
                    CallCollectLocal->arguments.push_back( make_smart<ExprVar>(expr->at, var->name) );
                    CallCollectLocal->alwaysSafe = true;
                    blocks.back()->finalList.push_back(CallCollectLocal);
                    if ( logs ) {
                        if ( !var->at.empty() && var->at.fileInfo ) {
                            *logs << var->at.fileInfo->name << ":" << var->at.line << ":" << var->at.column << "\n";
                        }
                        *logs << "In-scope POD applied to variable '" << var->name << "' in function '" << func->module->name << "::" << func->name << "'\n";
                    }
                }
            }
            return Visitor::visitLet(expr,var,last);
        }
    protected:
        Function * func = nullptr;
        TextWriter * logs = nullptr;
        vector<ExprBlock *> blocks;
    };

    bool Program::inScopePodAnalysis(TextWriter & logs) {
        auto forceInscopePod = options.getBoolOption("force_inscope_pod", policies.force_inscope_pod);
        if ( !forceInscopePod ) return false;
        auto logInscopePod = options.getBoolOption("log_inscope_pod", policies.log_inscope_pod);
        PodVisitor pv(logInscopePod ? &logs : nullptr);
        visit(pv);
        return pv.anyWork;
    }

}
