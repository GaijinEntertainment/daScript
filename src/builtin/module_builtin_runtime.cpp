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
        virtual bool apply(const FunctionPtr & func, const AnnotationArgumentList &, string &) override {
            func->exports = true;
            return true;
        };
        virtual bool finalize(const FunctionPtr &, const AnnotationArgumentList &, string &) override {
            return true;
        }
    };
    
    struct SideEffectsFunctionAnnotation : FunctionAnnotation {
        SideEffectsFunctionAnnotation() : FunctionAnnotation("sideeffects") { }
        virtual bool apply(ExprBlock *, const AnnotationArgumentList &, string & err) override {
            err = "can't have side-effects of a block";
            return false;
        }
        virtual bool finalize(ExprBlock *, const AnnotationArgumentList &, string &) override {
            return true;
        }
        virtual bool apply(const FunctionPtr & func, const AnnotationArgumentList &, string &) override {
            func->sideEffectFlags |= uint32_t(SideEffects::userScenario);
            return true;
        };
        virtual bool finalize(const FunctionPtr &, const AnnotationArgumentList &, string &) override {
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
        addAnnotation(make_shared<SideEffectsFunctionAnnotation>());
        // functions
        addExtern<DAS_BIND_FUN(builtin_throw)>         (*this, lib, "throw", SideEffects::modifyExternal);
        addExtern<DAS_BIND_FUN(builtin_print)>         (*this, lib, "print", SideEffects::modifyExternal);
        addExtern<DAS_BIND_FUN(builtin_terminate)> (*this, lib, "terminate", SideEffects::modifyExternal);
        addExtern<DAS_BIND_FUN(builtin_stackwalk)> (*this, lib, "stackwalk", SideEffects::modifyExternal);
        addInterop<builtin_breakpoint,void>     (*this, lib, "breakpoint", SideEffects::modifyExternal);
        // function-like expresions
        addCall<ExprAssert>         ("assert");
        addCall<ExprStaticAssert>   ("static_assert");
        addCall<ExprDebug>          ("debug");
        addCall<ExprHash>           ("hash");
        // table functions
        addExtern<DAS_BIND_FUN(builtin_table_clear)>(*this, lib, "clear", SideEffects::modifyArgument);
        addExtern<DAS_BIND_FUN(builtin_table_size)>(*this, lib, "length", SideEffects::none);
        addExtern<DAS_BIND_FUN(builtin_table_capacity)>(*this, lib, "capacity", SideEffects::none);
        // table expressions
        addCall<ExprErase>("__builtin_table_erase");
        addCall<ExprFind>("__builtin_table_find");
        addCall<ExprKeys>("keys");
        addCall<ExprValues>("values");
        // blocks
        addCall<ExprInvoke>("invoke");
        // profile
        addExtern<DAS_BIND_FUN(builtin_profile)>(*this,lib,"profile", SideEffects::modifyExternal);
        addString(lib);
    }
}
