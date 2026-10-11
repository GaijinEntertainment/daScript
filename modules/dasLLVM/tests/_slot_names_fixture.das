options gen2
options remove_unused_symbols = false

require daslib/ast_boost

struct Point {
    x : int = 1
}

def by_var(f : FunctionPtr) : bool {
    var blk = f.body as ExprBlock   // nolint:LINT003 - the var is the case: it types blk.list[0]'s at slot without const
    return blk.list[0] is ExprReturn
}

def by_let(f : FunctionPtr) : bool {
    let blk = f.body as ExprBlock
    return blk.list[0] is ExprReturn
}

def other_slots(tab : table<string; int>) : int {
    let p = Point()
    let fn = @@by_var
    return p.x + (tab?["a"] ?? 0) + (fn != null ? 1 : 0)
}
