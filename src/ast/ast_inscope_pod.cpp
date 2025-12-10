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
        virtual VariablePtr visitLet ( ExprLet * expr, const VariablePtr & var, bool last ) override {
            if ( func && var->podDelete && !var->podDeleteGen ) {
                anyWork = true;
                var->podDeleteGen = true;
                expr->inScope = true;
                if ( logs ) {
                    *logs << "In-scope POD applied to variable '" << var->name << "' in function '" << func->module->name << "::" << func->name << "'\n";
                }
            }
            return Visitor::visitLet(expr,var,last);
        }
    protected:
        Function * func = nullptr;
        TextWriter * logs = nullptr;
    };

    bool Program::inScopePodAnalysis(TextWriter & logs) {
        auto forceInscopePod = options.getBoolOption("force_inscope_pod", policies.force_inscope_pod);
        if ( !forceInscopePod ) return false;
        auto logInscopePod = options.getBoolOption("log_inscope_pod", false);
        PodVisitor pv(logInscopePod ? &logs : nullptr);
        visit(pv);
        return pv.anyWork;
    }

}
