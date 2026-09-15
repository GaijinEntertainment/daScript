options gen2
options indenting = 4
options strict_smart_pointers = true

//! ast_cursor provides position-based AST queries — given a file, line, and column,
//! it finds all expression nodes at that cursor position. Results are returned
//! innermost-first, with enclosing function context and extracted names.
//!
//! Typical use is implementing IDE features like goto-definition, type-of, and
//! find-references on top of the compiled AST.

module ast_cursor shared public

require daslib/ast_boost
require strings

//! A single hit at the cursor position — one AST expression node, or a variable declaration when asked for.
//! Innermost (most specific) hits come first in the result array.
struct public CursorHit {
    //! Raw pointer to the expression node (valid while the program lives); null for a variable hit.
    expr : Expression const?
    //! The variable whose declaration the cursor is on - a `for` loop variable, a `let` variable,
    //! a function, lambda or block argument - when the search asked for declarations; else null.
    variable : Variable const?
    //! Enclosing function, or null for expressions in global scope.
    func : Function const?
    //! Extracted name for named nodes (variable name, call name, field name, operator).
    name : string
    //! Expression RTTI type string: "ExprVar", "ExprCall", "ExprField", etc.
    rtti : string
}

//! Check whether a point (line, col) is inside the span described by `at`.
//! Follows the same logic as C++ ``LineInfo::inside``.
def public cursor_inside(line, col : int; at : LineInfo) : bool {
    if (int(at.line) == 0 && int(at.last_line) == 0) return false
    let tl = int(at.line)
    let tc = int(at.column)
    let tll = int(at.last_line)
    let tlc = int(at.last_column)
    if (line < tl || line > tll) return false
    if (line == tl && line == tll) return col >= tc && col < tlc
    if (line == tl) return col >= tc
    if (line == tll) return col < tlc
    return true
}

def private expression_name(expr : ExpressionPtr) : string {
    if (expr is ExprVar) return string((expr as ExprVar).name)
    if (expr is ExprCall) return string((expr as ExprCall).name)
    if (expr is ExprField) return string((expr as ExprField).name)
    if (expr is ExprSafeField) return string((expr as ExprSafeField).name)
    if (expr is ExprOp1) return string((expr as ExprOp1).op)
    if (expr is ExprOp2) return string((expr as ExprOp2).op)
    if (expr is ExprOp3) return "?"
    if (expr is ExprLooksLikeCall) return string((expr as ExprLooksLikeCall).name)
    if (expr is ExprNamedCall) return string((expr as ExprNamedCall).name)
    return ""
}

class CursorVisitor : AstVisitor {
    targetLine : int
    targetCol : int
    targetFile : string
    hits : array<CursorHit>
    currentFunc : Function const?
    withVariables : bool

    def CursorVisitor(file : string; line, col : int) {
        targetFile = file
        targetLine = line
        targetCol = col
        currentFunc = null
    }

    def in_target_file(at : LineInfo) : bool {
        return true if (empty(targetFile) || at.fileInfo == null)
        return find(string(at.fileInfo.name), targetFile) >= 0
    }

    // a variable's position is its name token; a position with no width is sized by the name
    def cursor_on_variable(v : Variable const?) : bool {
        let at = v.at
        return false if (int(at.line) != targetLine)
        if (int(at.last_line) != int(at.line) || int(at.last_column) > int(at.column)) {
            return cursor_inside(targetLine, targetCol, at)
        }
        let c = int(at.column)
        return targetCol >= c && targetCol < c + length(string(v.name))
    }

    def add_variable(v : Variable const?) : void {
        return if (!withVariables || v == null || v.flags.generated || !in_target_file(v.at) || !cursor_on_variable(v))
        hits |> emplace(CursorHit(
            variable = v,
            func = currentFunc,
            name = string(v.name),
            rtti = "Variable"
        ))
    }

    def override preVisitFunction(fun : FunctionPtr) : void {
        currentFunc = fun
    }

    def override preVisitFunctionArgument(fun : FunctionPtr; arg : VariablePtr; lastArg : bool) : void {
        add_variable(arg)
    }

    def override preVisitExprBlockArgument(blk : ExprBlock?; arg : VariablePtr; lastArg : bool) : void {
        add_variable(arg)
    }

    def override preVisitExprLetVariable(expr : ExprLet?; arg : VariablePtr; lastArg : bool) : void {
        add_variable(arg)
    }

    def override preVisitExprForVariable(expr : ExprFor?; svar : VariablePtr; last : bool) : void {
        add_variable(svar)
    }

    def override visitFunction(var fun : FunctionPtr) : FunctionPtr {
        currentFunc = null
        return fun
    }

    def override preVisitExpression(expr : ExpressionPtr) : void {
        let e = expr
        return if (!in_target_file(e.at))
        if (cursor_inside(targetLine, targetCol, e.at)) {
            hits |> emplace(CursorHit(
                expr = e,
                func = currentFunc,
                name = expression_name(expr),
                rtti = e.__rtti
            ))
        }
    }
}

//! the size of an expression's span, lines first: a nested expression always has the
//! smaller one, whichever function the visitor met first - a lambda or generator body is a
//! function of its own, visited before the function whose source it sits in
def private span_size(hit : CursorHit) : int2 {
    if (hit.expr == null) {
        return int2(0, length(hit.name))
    }
    let at = hit.expr.at
    return int2(int(at.last_line) - int(at.line), int(at.last_column) - int(at.column))
}

//! hits innermost first: by span, and within one span the later-visited (deeper) node first
def private reverse_hits(var src : array<CursorHit>) : array<CursorHit> {
    var order : array<int>
    order |> reserve(length(src))
    for (i in range(length(src))) {
        order |> push(i)
    }
    order |> sort() $(a, b) {
        let sa = span_size(src[a])
        let sb = span_size(src[b])
        if (sa.x != sb.x) return sa.x < sb.x
        if (sa.y != sb.y) return sa.y < sb.y
        return a > b
    }
    var result : array<CursorHit>
    result |> reserve(length(src))
    for (i in order) {
        result |> emplace <| src[i]
    }
    return <- result
}

//! Find all expression nodes at the given cursor position, innermost first; `file` is a substring
//! matched against ``FileInfo.name`` ("" matches any). With `declarations`, a cursor on the name of
//! a `for`/`let` variable or a function, lambda or block argument yields a `variable` hit with no `expr`.
def public find_at_cursor(program : smart_ptr<Program>; file : string; line, col : int; declarations : bool = false) : array<CursorHit> {
    var visitor = new CursorVisitor(file, line, col)
    visitor.withVariables = declarations
    make_visitor(*visitor) $(adapter) {
        visit(program, adapter)
    }
    var result <- reverse_hits(visitor.hits)
    unsafe {
        delete visitor
    }
    return <- result
}

//! Find all expression nodes at the given cursor position within a single function body.
//! `file` is a substring matched against ``FileInfo.name`` (use "" to match any file).
def public find_at_cursor_in_function(func : FunctionPtr; file : string; line, col : int; declarations : bool = false) : array<CursorHit> {
    var visitor = new CursorVisitor(file, line, col)
    visitor.currentFunc = func
    visitor.withVariables = declarations
    make_visitor(*visitor) $(adapter) {
        visit(func, adapter)
    }
    var result <- reverse_hits(visitor.hits)
    unsafe {
        delete visitor
    }
    return <- result
}

//! Pretty-print a CursorHit for debugging. Includes RTTI type, name, resolved type, source location, and enclosing function.
def public describe(hit : CursorHit) : string {
    return build_string() $(var w) {
        write(w, hit.rtti)
        if (!empty(hit.name)) {
            write(w, "({hit.name})")
        }
        if (hit.variable != null && hit.variable._type != null) {
            write(w, " : {describe(hit.variable._type)}")
        } elif (hit.expr != null && hit.expr._type != null) {
            write(w, " : {describe(hit.expr._type)}")
        }
        let at = hit.expr != null ? hit.expr.at : hit.variable.at
        write(w, " at {int(at.line)}:{int(at.column)}-{int(at.last_line)}:{int(at.last_column)}")
        if (hit.func != null) {
            write(w, " in {hit.func.name}")
        }
    }
}
