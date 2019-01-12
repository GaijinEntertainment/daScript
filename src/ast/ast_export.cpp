#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"

namespace das {

	class MarkSymbolUse : public Visitor {
	public:
		void propagateFunctionUse(const FunctionPtr & fn) {
			if (fn->used) return;
			fn->used = true;
			for (const auto & gv : fn->useGlobalVariables) {
				gv->used = true;
			}
			for (const auto & it : fn->useFunctions) {
				propagateFunctionUse(it);
			}
		}
		void markUsedFunctions( Module & thisModule ){
			for (const auto & it : thisModule.functions) {
				auto fn = it.second;
				if (fn->exports) {
					propagateFunctionUse(fn);
				}
			}
		}
        void RemoveUnusedSymbols ( Module & mod ) {
            map<string,FunctionPtr> functions;
            map<string,VariablePtr> globals;
            swap(functions,mod.functions);
            swap(globals,mod.globals);
            mod.functionsByName.clear();
            for ( auto & fn : functions ) {
                if ( fn.second->used ) {
                    mod.addFunction(fn.second);
                }
            }
            for ( auto & var : globals ) {
                if ( var.second->used ) {
                    mod.addVariable(var.second);
                }
            }
        }
	protected:
		ProgramPtr              program;
		FunctionPtr             func;
	protected:
		// global variable declaration
		virtual void preVisitGlobalLet(const VariablePtr & var) override {
			Visitor::preVisitGlobalLet(var);
			var->used = false;
		}
		// function
		virtual void preVisit(Function * f) override {
			Visitor::preVisit(f);
			func = f->shared_from_this();
			func->useFunctions.clear();
			func->useGlobalVariables.clear();
		}
		virtual FunctionPtr visit(Function * that) override {
			func.reset();
			return Visitor::visit(that);
		}
		// variable
		virtual void preVisit(ExprVar * expr) override {
			Visitor::preVisit(expr);
			if (!expr->local && !expr->argument && !expr->block) {
				func->useGlobalVariables.insert(expr->variable);
			}
		}
		// function call
		virtual void preVisit(ExprCall * call) override {
			Visitor::preVisit(call);
			if (!call->func->builtIn) {
				func->useFunctions.insert(call->func);
			}
		}
		// Op1
		virtual void preVisit(ExprOp1 * expr) override {
			Visitor::preVisit(expr);
			if (!expr->func->builtIn) {
				func->useFunctions.insert(expr->func);
			}
		}
		// Op2
		virtual void preVisit(ExprOp2 * expr) override {
			Visitor::preVisit(expr);
			if (!expr->func->builtIn) {
				func->useFunctions.insert(expr->func);
			}
		}
		// Op3
		virtual void preVisit(ExprOp3 * expr) override {
			Visitor::preVisit(expr);
			if (!expr->func->builtIn) {
				func->useFunctions.insert(expr->func);
			}
		}
	};

	void Program::markOrRemoveUnusedSymbols() {
		MarkSymbolUse vis;
		visit(vis);
		vis.markUsedFunctions(*thisModule);
        if ( options.getOption("removeUnusedSymbols",true) ) {
            vis.RemoveUnusedSymbols(*thisModule);
        }
	}
}
