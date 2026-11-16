options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_lever shared public

require dasllama/dasllama_lint public
require daslib/ast
require daslib/ast_boost
require daslib/templates_boost

//! `[lever]` on a knob's getter - a function of no arguments and a declared result `T` whose body reads the knob. The
//! getter answers the in-process pin where one stands, its own body where none does, and the module gains, for a lever
//! named `X` (the getter's name, or `name = X`): `set_X(v : T)`, `reset_X()`, `X_pin() : tuple<pinned : bool; value : T>`,
//! `restore_X(pin)` and `with_X(v : T) { ... }`, each as visible as the getter.
[function_macro(name = "lever")]
class private LeverMacro : AstFunctionAnnotation {
    [arch(at = "../ARCHITECTURE_ENGINE.md#levers")]
    def override apply(var func : FunctionPtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        var blk = func.body as ExprBlock
        if (blk == null) {
            errors := "lever: the getter `{func.name}` needs a block body"
            return false
        }
        if (length(func.arguments) != 0) {
            errors := "lever: the getter `{func.name}` takes no arguments"
            return false
        }
        if (func.result == null || func.result.baseType == Type.autoinfer || func.result.baseType == Type.tVoid) {
            errors := "lever: the getter `{func.name}` declares its result type - the pin holds a value of it"
            return false
        }
        let name = find_arg(args, "name") ?as tString ?? "{func.name}"
        let pinned = "g_lever`{name}`pinned"
        let value = "g_lever`{name}`value"
        var vt = clone_type(func.result)
        vt.flags.constant = false
        vt.flags.ref = false
        let mod = compiling_module()
        if (!add_global_var(mod, pinned, new TypeDecl(at = func.at, baseType = Type.tBool), func.at, true)
            || !add_global_var(mod, value, clone_type(vt), func.at, true)) {
            errors := "lever: the module already holds a lever named `{name}`"
            return false
        }
        var guard = qmacro_expr(${ if ($i(pinned)) { return $i(value); } })
        force_generated(guard, true)
        blk.list |> emplace(guard, 0)
        let pin_name = "{name}_pin"
        var fset <- qmacro_function("set_{name}") $(v : $t(vt)) {
            $i(pinned) = true
            $i(value) = v
        }
        var freset <- qmacro_function("reset_{name}") $() {
            $i(pinned) = false
            $i(value) = default<$t(vt)>
        }
        var fpin <- qmacro_function(pin_name) $() : tuple<pinned : bool; value : $t(vt)> {
            return (pinned = $i(pinned), value = $i(value))
        }
        var frestore <- qmacro_function("restore_{name}") $(pin : tuple<pinned : bool; value : $t(vt)>) {
            $i(pinned) = pin.pinned
            $i(value) = pin.value
        }
        var fwith <- qmacro_function("with_{name}") $(v : $t(vt); blk : block) {
            let was_pinned = $i(pinned)
            let was = $i(value)
            $i(pinned) = true
            $i(value) = v
            invoke(blk)
            $i(pinned) = was_pinned
            $i(value) = was
        }
        lever_add(mod, fset, func)
        lever_add(mod, freset, func)
        lever_add(mod, fpin, func)
        lever_add(mod, frestore, func)
        lever_add(mod, fwith, func)
        return true
    }
}

def private lever_add(mod : Module?; var fn : FunctionPtr; getter : FunctionPtr) {
    fn.flags.generated = true
    fn.flags.privateFunction = getter.flags.privateFunction
    fn.at = getter.at
    mod |> add_function(fn)
}
