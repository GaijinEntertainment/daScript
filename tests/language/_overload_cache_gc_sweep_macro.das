options gen2

module _overload_cache_gc_sweep_macro

require daslib/ast
require daslib/ast_boost
require daslib/templates
require daslib/macro_boost
require daslib/templates_boost

var private g_step = 0

def private find_structure(var mod : Module?; name : string) : Structure? {
    var found : Structure?
    mod |> for_each_structure() $(s) {
        if (s.name == name) {
            found = s
        }
    }
    return found
}

def private set_probe_assignment(var mod : Module?; var st : Structure?) {
    var fn = find_unique_function(mod, "probe_site", true)
    var blk = fn.body as ExprBlock
    blk.list |> clear()
    var sT <- new TypeDecl(baseType = Type.tStructure, structType = st)
    var pT <- new TypeDecl(baseType = Type.tPointer, firstType <- sT)
    blk.list |> push(qmacro_expr(${ t = unsafe(reinterpret<$t(pT)>(null)); }))
}

[infer_macro]
class ChurnStructures : AstPassMacro {
    def override apply(prog : ProgramPtr; var mod : Module?) : bool {
        let churn = 30
        let other = find_structure(mod, "Other")
        var base = find_structure(mod, "Base")
        let probe = find_unique_function(mod, "probe_site", true)
        if (other == null || base == null || probe == null) return false
        if (g_step < churn) {
            var st <- clone_structure(other)
            st.name := "Churned{g_step}"
            set_probe_assignment(mod, st)
            g_step ++
            return true
        } elif (g_step == churn) {
            var derived <- clone_structure(base)
            derived.name := "Derived"
            derived.parent = base
            var derivedPtr = derived
            mod |> add_structure(derived)
            set_probe_assignment(mod, derivedPtr)
            g_step ++
            return true
        }
        return false
    }
}
