// Stage 0 coverage for the tFixedArray rework (FIXED_ARRAY_REWORK.md).
// Raw AstTypeMacro exercising the typeMacro argument payload that Stage 1b moves
// from TypeDecl.dimExpr to the dedicated typeMacroExpr field:
//   dimExpr[0] — ExprConstString with the macro name
//   dimExpr[1] — type argument (ExprTypeDecl)
//   dimExpr[2] — ExprConstInt (size)
//   dimExpr[3] — ExprConstBool (wrap into an array or not)
//   dimExpr[4] — ExprConstString (mode tag, "arr" expected when wrapping)
// NOTE: this module reads .dimExpr and writes .dim directly — it is itself a
// consumer the rework migrates (kept on the old API until the compat property
// lands in Stage 1, ported to typeMacroExpr in Stage 5).
options gen2
options no_aot

module _typemacro_mod shared private

require daslib/ast
require daslib/ast_boost

// resolves tm_make(type<T>, N, true, "arr") -> T[N]; tm_make(type<T>, N, false, "plain") -> T
[type_macro(name="tm_make")]
class TmMakeTypeMacro : AstTypeMacro {
    def override visit(prog : ProgramPtr; mod : Module?; td : TypeDeclPtr; passT : TypeDeclPtr) : TypeDeclPtr {
        if (length(td.dimExpr) != 5) {
            macro_error(compiling_program(), td.at, "tm_make expects 4 arguments: type, size, wrap, tag")
            return <- TypeDeclPtr()
        }
        if (!(td.dimExpr[2] is ExprConstInt)) {
            macro_error(compiling_program(), td.at, "tm_make: size must be a constant integer")
            return <- TypeDeclPtr()
        }
        if (!(td.dimExpr[3] is ExprConstBool)) {
            macro_error(compiling_program(), td.at, "tm_make: wrap must be a constant bool")
            return <- TypeDeclPtr()
        }
        if (!(td.dimExpr[4] is ExprConstString)) {
            macro_error(compiling_program(), td.at, "tm_make: tag must be a constant string")
            return <- TypeDeclPtr()
        }
        let count = (td.dimExpr[2] as ExprConstInt).value
        let wrap = (td.dimExpr[3] as ExprConstBool).value
        if (wrap && (td.dimExpr[4] as ExprConstString).value != "arr") {
            macro_error(compiling_program(), td.at, "tm_make: wrapping requires the 'arr' tag, got '{(td.dimExpr[4] as ExprConstString).value}'")
            return <- TypeDeclPtr()
        }
        // generic path: type argument not inferred yet — return auto so the compiler retries
        if (td.dimExpr[1]._type == null) {
            var auto_type : TypeDeclPtr
            if (td.dimExpr[1] is ExprTypeDecl) {
                auto_type = clone_type((td.dimExpr[1] as ExprTypeDecl).typeexpr)
            } else {
                auto_type = new TypeDecl(baseType = Type.autoinfer)
            }
            if (wrap) {
                auto_type.dim |> push(count)
            }
            return <- auto_type
        }
        var final_type = clone_type(td.dimExpr[1]._type)
        if (wrap) {
            final_type.dim |> push(count)
        }
        return <- final_type
    }
}
