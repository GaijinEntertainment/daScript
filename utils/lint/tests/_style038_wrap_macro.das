options gen2

module _style038_wrap_macro shared

require daslib/ast_boost
require daslib/templates_boost

// `[wraps]` replaces a function's body with a block built here, so the new body's
// location points into this file - past line 10, the fixture's length limit.
//
// padding 1: keeps the block below past line 40
// padding 2: keeps the block below past line 40
// padding 3: keeps the block below past line 40
// padding 4: keeps the block below past line 40
// padding 5: keeps the block below past line 40
// padding 6: keeps the block below past line 40
// padding 7: keeps the block below past line 40
// padding 8: keeps the block below past line 40
// padding 9: keeps the block below past line 40
// padding 10: keeps the block below past line 40
// padding 11: keeps the block below past line 40
// padding 12: keeps the block below past line 40
// padding 13: keeps the block below past line 40
// padding 14: keeps the block below past line 40
// padding 15: keeps the block below past line 40
// padding 16: keeps the block below past line 40
// padding 17: keeps the block below past line 40
// padding 18: keeps the block below past line 40
// padding 19: keeps the block below past line 40
// padding 20: keeps the block below past line 40

[function_macro(name = "wraps")]
class WrapsAnnotation : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup; var args : AnnotationArgumentList; var errors : das_string) : bool {
        var wrapped = qmacro_block() {
            let entered = true
            $e(func.body)
        }
        func.body = unsafe(reinterpret<ExprBlock?>(wrapped))
        return true
    }
}
