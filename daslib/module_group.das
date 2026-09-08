options gen2
options indenting = 4
options strict_smart_pointers

module module_group shared private

//! Calls into every member of a module group.

require daslib/ast_boost
require daslib/templates_boost
require daslib/fio
require rtti

def public module_group_module_name(member : string) : string {
    //! The module a group member's require path names: its last segment.
    return base_name(member)
}

[call_macro(name="call_module_group")]
class CallModuleGroup : AstCallMacro {
    //! `call_module_group("group", "entry", args...)` expands to one `member::entry(args...)`
    //! call per module registered under the group, sorted by member path; a group nothing
    //! joined expands to nothing.
    def override visit(prog : ProgramPtr; mod : Module?; expr : ExprCallMacro?) : ExpressionPtr {
        if (length(expr.arguments) < 2 || !(expr.arguments[0] is ExprConstString) || !(expr.arguments[1] is ExprConstString)) {
            macro_error(prog, expr.at, "call_module_group expects (\"group\", \"entry\", args...)")
            return <- default<ExpressionPtr>
        }
        let group = string((expr.arguments[0] as ExprConstString).value)
        let entry = string((expr.arguments[1] as ExprConstString).value)
        var res <- qmacro_block() {
        }
        var blk = res as ExprBlock
        module_group_for_each_member(group) $(member : string) {
            var call <- make_call(expr.at, "{module_group_module_name(member)}::{entry}")
            for (i in range(2, length(expr.arguments))) {
                (call as ExprCall).arguments |> emplace_new(clone_expression(expr.arguments[i]))
            }
            blk.list |> emplace_new(call)
        }
        return <- res
    }
}
