options gen2
options indenting = 4
options strict_smart_pointers = true

module only_nttp shared private

require daslib/ast
require daslib/ast_boost
require daslib/strings_boost

//! ``[only_nttp]`` — benchmark-kernel guard. Every call-based extern invoked
//! directly from the annotated function must be the NTTP binding flavor
//! (``addExternInline``); a member-flavor extern in a fastpath kernel fails
//! the compile naming the call, so a silently-downgraded bind can't skew a
//! measurement. Policy-node builtins and interop functions are exempt.

class OnlyNttpVisitor : AstVisitor {
    offenders : array<string>
    def override visitExprCall(var expr : ExprCall?) : ExpressionPtr {
        if (expr.func != null && expr.func.flags.builtIn && expr.func.flags.callBased && !expr.func.flags.interopFn) {
            if (!expr.func.moreFlags2.nttp) {
                offenders |> push("{expr.func.name} (line {int(expr.at.line)})")
            }
        }
        return expr
    }
}

[function_macro(name="only_nttp")]
class public OnlyNttpAnnotation : AstFunctionAnnotation {
    def override patch(var func : FunctionPtr; var group : ModuleGroup; args, progArgs : AnnotationArgumentList; var errors : das_string; var astChanged : bool&) : bool {
        var scan = new OnlyNttpVisitor()
        make_visitor(*scan) $(adapter) {
            visit(func, adapter)
        }
        var failed = false
        if (!empty(scan.offenders)) {
            errors := build_string() $(writer) {
                writer |> write("only_nttp: slow-path (member-flavor) extern call(s) inside fastpath kernel '{func.name}':")
                for (off in scan.offenders) {
                    writer |> write("\n    {off}")
                }
                writer |> write("\n  bind these with addExternInline, or drop [only_nttp]")
            }
            failed = true
        }
        unsafe {
            delete scan
        }
        return !failed
    }
}
