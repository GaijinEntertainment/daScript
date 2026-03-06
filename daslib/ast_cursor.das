options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers = true

module ast_cursor shared public

require daslib/ast_boost
require strings

// A single hit at the cursor position — one AST expression node.
// Innermost (most specific) expressions come first in the result array.
struct public CursorHit {
    expr : Expression const?    // raw ptr to expression node (valid while program lives)
    func : Function const?      // enclosing function, or null for global scope
    name : string               // extracted name for named nodes (variable, call, field, op)
    rtti : string               // expression type: "ExprVar", "ExprCall", etc.
}

// Check whether point (line, col) is inside the span described by `at`.
// Follows the same logic as C++ LineInfo::inside.
def public cursor_inside(line, col : int; at : LineInfo) : bool {
    if (int(at.line) == 0 && int(at.last_line) == 0) {
        return false
    }
    let tl = int(at.line)
    let tc = int(at.column)
    let tll = int(at.last_line)
    let tlc = int(at.last_column)
    if (line < tl || line > tll) {
        return false
    }
    // single-line span
    if (line == tl && line == tll) {
        return col >= tc && col < tlc
    }
    // first line of multi-line span
    if (line == tl) {
        return col >= tc
    }
    // last line of multi-line span
    if (line == tll) {
        return col < tlc
    }
    // middle lines
    return true
}

// Extract a human-readable name from an expression based on its __rtti.
def expression_name(expr : Expression const?) : string {
    let rtti = expr.__rtti
    if (rtti == "ExprVar") {
        unsafe {
            let v = reinterpret<ExprVar const?> expr
            return string(v.name)
        }
    }
    if (rtti == "ExprCall") {
        unsafe {
            let c = reinterpret<ExprCall const?> expr
            return string(c.name)
        }
    }
    if (rtti == "ExprField" || rtti == "ExprSafeField") {
        unsafe {
            let f = reinterpret<ExprField const?> expr
            return string(f.name)
        }
    }
    if (rtti == "ExprOp1") {
        unsafe {
            let o = reinterpret<ExprOp1 const?> expr
            return string(o.op)
        }
    }
    if (rtti == "ExprOp2") {
        unsafe {
            let o = reinterpret<ExprOp2 const?> expr
            return string(o.op)
        }
    }
    if (rtti == "ExprOp3") {
        return "?"
    }
    if (rtti == "ExprLooksLikeCall") {
        unsafe {
            let c = reinterpret<ExprLooksLikeCall const?> expr
            return string(c.name)
        }
    }
    if (rtti == "ExprNamedCall") {
        unsafe {
            let c = reinterpret<ExprNamedCall const?> expr
            return string(c.name)
        }
    }
    return ""
}

class CursorVisitor : AstVisitor {
    targetLine : int
    targetCol : int
    targetFile : string
    hits : array<CursorHit>
    currentFunc : Function const?

    def CursorVisitor(file : string; line, col : int) {
        targetFile = file
        targetLine = line
        targetCol = col
        currentFunc = null
    }

    def override preVisitFunction(fun : FunctionPtr) : void {
        currentFunc = get_ptr(fun)
    }

    def override visitFunction(var fun : FunctionPtr) : FunctionPtr {
        currentFunc = null
        return <- fun
    }

    def override preVisitExpression(expr : ExpressionPtr) : void {
        let e = get_ptr(expr)
        // filter by file if specified
        if (!empty(targetFile) && e.at.fileInfo != null) {
            let fname = string(e.at.fileInfo.name)
            if (find(fname, targetFile) < 0) {
                return
            }
        }
        if (cursor_inside(targetLine, targetCol, e.at)) {
            hits |> emplace(CursorHit(
                expr = e,
                func = currentFunc,
                name = expression_name(e),
                rtti = string(e.__rtti)
            ))
        }
    }
}

def reverse_hits(var src : array<CursorHit>) : array<CursorHit> {
    var result : array<CursorHit>
    var i = length(src) - 1
    while (i >= 0) {
        result |> emplace <| src[i]
        i--
    }
    return <- result
}

// Find all expression nodes at the given cursor position, innermost first.
// `file` is a substring matched against FileInfo.name (use "" to match any file).
def public find_at_cursor(program : smart_ptr<Program>; file : string; line, col : int) : array<CursorHit> {
    var visitor = new CursorVisitor(file, line, col)
    var inscope adapter <- make_visitor(*visitor)
    visit(program, adapter)
    // visitor collects parent-first (depth-first pre-order), reverse for innermost-first
    var result <- reverse_hits(visitor.hits)
    unsafe {
        delete visitor
    }
    return <- result
}

// Convenience: find at cursor in a single function's body.
def public find_at_cursor_in_function(func : FunctionPtr; file : string; line, col : int) : array<CursorHit> {
    var visitor = new CursorVisitor(file, line, col)
    visitor.currentFunc = get_ptr(func)
    var inscope adapter <- make_visitor(*visitor)
    visit(func, adapter)
    var result <- reverse_hits(visitor.hits)
    unsafe {
        delete visitor
    }
    return <- result
}

// Pretty-print a CursorHit for debugging.
def public describe(hit : CursorHit) : string {
    return build_string() <| $(var w) {
        write(w, hit.rtti)
        if (!empty(hit.name)) {
            write(w, "({hit.name})")
        }
        if (hit.expr != null && hit.expr._type != null) {
            write(w, " : {describe(hit.expr._type)}")
        }
        write(w, " at {int(hit.expr.at.line)}:{int(hit.expr.at.column)}-{int(hit.expr.at.last_line)}:{int(hit.expr.at.last_column)}")
        if (hit.func != null) {
            write(w, " in {hit.func.name}")
        }
    }
}
