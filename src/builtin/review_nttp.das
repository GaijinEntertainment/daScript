options gen2
module review_nttp private

require daslib/ast
require daslib/ast_boost
require daslib/macro_boost
require math
require strings
require daslib/fio
require daslib/rtti

// Compile-time half of the src/builtin gate: in builtin ($), math and strings
// every plain value-returning extern bind is NTTP (addExternInline); every
// other module stays member. Exempt by construction: policy-node builtins
// (not callBased), interop, cmres (non-ref reference-type results) and
// ref-returning binds. Violations surface as compile errors.

def private is_flavor_checked(func : FunctionPtr) : bool {
    if (!func.flags.builtIn || !func.flags.callBased || func.flags.interopFn) {
        return false
    }
    if (func.result == null || func.result.isRefType || func.result.flags.ref) {
        return false
    }
    return true
}

[_macro]
def check_nttp_policy {
    let trio <- {"$", "math", "strings"}
    program_for_each_module(compiling_program()) $(mod) {
        let inTrio = trio |> key_exists("{mod.name}")
        for_each_function(mod, "") $(func) {
            return if (!is_flavor_checked(func))
            // ast_handle's generic container/equality helpers are NTTP in every module
            // they instantiate into — hottest plumbing everywhere, one call site serves all
            let cppName = "{(func as BuiltInFunction).cppName}"
            return if (cppName |> starts_with("das_vector_"))
            return if (cppName |> starts_with("das_equ") || cppName |> starts_with("das_nequ"))
            return if (cppName |> starts_with("das_handle_equ") || cppName |> starts_with("das_handle_nequ"))
            if (inTrio && !func.moreFlags2.nttp) {
                macro_error(compiling_program(), func.at, "REVIEW policy: {mod.name}::{func.name} must bind via addExternInline (trio is 100% NTTP)")
            } elif (!inTrio && func.moreFlags2.nttp) {
                macro_error(compiling_program(), func.at, "REVIEW policy: {mod.name}::{func.name} must not bind via addExternInline (only builtin/math/strings are NTTP)")
            }
        }
    }
}
