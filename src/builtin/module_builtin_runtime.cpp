#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/ast/ast_interop.h"

#include "daScript/simulate/runtime_array.h"
#include "daScript/simulate/runtime_table.h"
#include "daScript/simulate/runtime_profile.h"

namespace das
{
	struct ExportFunctionAnnotation : FunctionAnnotation {
		ExportFunctionAnnotation() : FunctionAnnotation("export") { }
		virtual bool apply(ExprBlock *, const AnnotationArgumentList &, string & err) override {
			err = "can't export block";
			return false;
		}
		virtual bool finalize(ExprBlock *, const AnnotationArgumentList &, string &) override {
			return true;
		}
		virtual bool apply(const FunctionPtr &, const AnnotationArgumentList &, string &) override {
			return true;
		};
		virtual bool finalize(const FunctionPtr & func, const AnnotationArgumentList &, string &) override {
			func->exports = true;
			return true;
		}
	};

    // core functions
    
    void builtin_throw ( char * text, Context * context ) {
        context->throw_error(text);
    }
    
    void builtin_print ( char * text, Context * context ) {
        context->to_out(text);
    }
    
    vec4f builtin_breakpoint ( Context & context, SimNode_CallBase * call, vec4f * ) {
        context.breakPoint(call->debug.column, call->debug.line);
        return v_zero();
    }
    
    void builtin_stackwalk ( Context * context) {
        context->stackWalk();
    }
    
    void builtin_terminate ( Context * context ) {
        context->stopFlags |= EvalFlags::stopForTerminate;
    }
    
    int builtin_table_size ( const Table * arr ) {
        return arr->size;
    }
    
    int builtin_table_capacity ( const Table * arr ) {
        return arr->capacity;
    }
    
    void builtin_table_clear ( Table * arr, Context * context ) {
        table_clear(*context, *arr);
    }
    
    void Module_BuiltIn::addRuntime(ModuleLibrary & lib) {
		// function annotations
		addAnnotation(make_shared<ExportFunctionAnnotation>());
        // functions
        addExtern<decltype(&builtin_throw),builtin_throw>         (*this, lib, "throw");
        addExtern<decltype(&builtin_print),builtin_print>         (*this, lib, "print");
        addExtern<decltype(&builtin_terminate),builtin_terminate> (*this, lib, "terminate");
        addExtern<decltype(&builtin_stackwalk),builtin_stackwalk> (*this, lib, "stackwalk");
        addInterop<builtin_breakpoint,void>     (*this, lib, "breakpoint");
        // function-like expresions
        addCall<ExprAssert>         ("assert");
        addCall<ExprStaticAssert>   ("static_assert");
        addCall<ExprDebug>          ("debug");
        addCall<ExprHash>           ("hash");
        // table functions
        addExtern<decltype(&builtin_table_clear), builtin_table_clear>(*this, lib, "clear", true);
        addExtern<decltype(&builtin_table_size), builtin_table_size>(*this, lib, "length", false);
        addExtern<decltype(&builtin_table_capacity), builtin_table_capacity>(*this, lib, "capacity", false);
        // table expressions
        addCall<ExprErase>("__builtin_table_erase");
        addCall<ExprFind>("__builtin_table_find");
        addCall<ExprKeys>("keys");
        addCall<ExprValues>("values");
        // blocks
        addCall<ExprInvoke>("invoke");
        // profile
        addExtern<decltype(&builtin_profile),builtin_profile>(*this,lib,"profile");
    }
}
