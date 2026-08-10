options gen2

module alias_breaker shared

// Self-test fixture: points two argument slots at ONE TypeDecl. The generator
// builds every node fresh, so an aliased node is the one shape it cannot reach.

require daslib/ast
require daslib/ast_boost
require daslib/ast_verify

class Aliaser : AstVisitor {
    def override visitFunction(var fun : FunctionPtr) : FunctionPtr {
        if (fun.name == "aliased" && length(fun.arguments) == 2) {
            fun.arguments[1]._type = fun.arguments[0]._type
        }
        return fun
    }
}

[infer_macro]
class AliasMacro : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        if (mod == null || !empty(mod.name)) {
            return false
        }
        var v = new Aliaser()
        make_visitor(*v) $(adapter) {
            visit_module(prog, adapter, mod)
        }
        unsafe {
            delete v
        }
        verify_module(prog, mod)
        return false
    }
}
