options gen2

module linq_das

require strings
require daslib/ast
require daslib/ast_boost
require daslib/linq_boost public

// ===== C#-style LINQ query syntax (linq_das) — v1: array source, from/where/select =====
//
// The `%linq! … %%` inline reader macro rewrites a C#-like query into a `_fold(...)` chain:
//
//     %linq! from c in cars where c.price > 100 select c.name %%
//        →   ( _fold( each(cars) |> _where(_.price > 100) |> _select(_.name) |> to_array() ) )
//
// v1 covers `from`/`where`/`select` over an array source. The range variable is spliced verbatim as the
// block parameter (`_where($(c) => ...)`), so the predicate/projection text needs no rewriting; a typed
// source `from c : T in db` and the `orderby`/`group by`/`join` clauses are not yet supported.

def private is_ident_byte(c : int) : bool {
    return is_alnum(c) || c == '_'
}

// Byte offset of the next whole-word `kw` at paren/bracket/brace depth 0 in q[from..], or -1.
// String literals are skipped so a keyword inside `"..."` never matches, and a word immediately
// following `.` (a member access like `c.where`) is not treated as a clause keyword.
def private find_kw_depth0(q : string; kw : string; from : int) : int {
    var found = -1
    let m = length(kw)
    peek_data(q) $(arr) {
        let n = length(arr)
        var depth = 0
        var inStr = false
        var prevSig = 0   // last significant (non-whitespace) byte, 0 = none
        var i = from
        while (i < n && found < 0) {
            let c = int(arr[i])
            if (inStr) {
                if (c == '\\') { i += 2; continue }
                if (c == '"') { inStr = false; prevSig = c }
                i++
                continue
            }
            if (c == '"') { inStr = true; prevSig = c; i++; continue }
            if (c == '(' || c == '[' || c == '{') { depth++; prevSig = c; i++; continue }
            if (c == ')' || c == ']' || c == '}') { depth--; prevSig = c; i++; continue }
            // a clause keyword is a depth-0 whole word NOT preceded (ignoring whitespace) by `.`
            let atWordStart = is_ident_byte(c) && ((i == 0) || !is_ident_byte(int(arr[i - 1]))) && prevSig != '.'
            if (depth == 0 && atWordStart && i + m <= n && slice(q, i, i + m) == kw) {
                if (i + m == n || !is_ident_byte(int(arr[i + m]))) {
                    found = i
                }
            }
            if (!is_white_space(c)) { prevSig = c }
            i++
        }
    }
    return found
}

// Read the identifier at/after `from` (skipping leading whitespace); returns (name, endOffset).
// name is "" when no identifier is present.
def private read_ident(q : string; from : int) : tuple<string; int> {
    var s = -1
    var e = -1
    peek_data(q) $(arr) {
        let n = length(arr)
        var i = from
        while (i < n && !is_ident_byte(int(arr[i]))) { i++ }
        s = i
        while (i < n && is_ident_byte(int(arr[i]))) { i++ }
        e = i
    }
    if (s < 0 || e <= s) { return ("", -1) }
    return (slice(q, s, e), e)
}

// Parse the query and emit the `_fold(...)` rewrite text. On a malformed query, register a macro error
// and emit a benign `(0)` so the surrounding statement still parses (the error is the real signal).
def private transpile_query(query : string; prog : ProgramPtr; at : LineInfo) : string {
    let q = strip(query)
    let nq = length(q)
    let fromWord = read_ident(q, 0)
    if (fromWord._0 != "from") {
        macro_error(prog, at, "linq: query must start with the 'from' keyword (got: {q})")
        return "(0)"
    }
    let vp = read_ident(q, fromWord._1)
    let varName = vp._0
    let afterVar = vp._1
    if (varName == "") {
        macro_error(prog, at, "linq: expected a range variable after 'from'")
        return "(0)"
    }
    let inPos = find_kw_depth0(q, "in", afterVar)
    if (inPos < 0) {
        macro_error(prog, at, "linq: expected 'in' after range variable '{varName}'")
        return "(0)"
    }
    let between = strip(slice(q, afterVar, inPos))
    if (starts_with(between, ":")) {
        macro_error(prog, at, "linq: typed sources ('from c : Type in ...') are not supported yet (v1: array only)")
        return "(0)"
    }
    if (between != "") {
        macro_error(prog, at, "linq: unexpected '{between}' between range variable and 'in'")
        return "(0)"
    }
    let srcStart = inPos + 2
    let wherePos = find_kw_depth0(q, "where", srcStart)
    let selectPos = find_kw_depth0(q, "select", wherePos >= 0 ? wherePos + 5 : srcStart)
    if (selectPos < 0) {
        macro_error(prog, at, "linq: query must end with a 'select' clause (v1)")
        return "(0)"
    }
    let srcEnd = wherePos >= 0 ? wherePos : selectPos
    let srcText = strip(slice(q, srcStart, srcEnd))
    if (srcText == "") {
        macro_error(prog, at, "linq: empty source expression after 'in'")
        return "(0)"
    }
    let hasWhere = wherePos >= 0
    var predText = ""
    if (hasWhere) {
        predText = strip(slice(q, wherePos + 5, selectPos))
        if (predText == "") {
            macro_error(prog, at, "linq: empty 'where' predicate")
            return "(0)"
        }
    }
    let projText = strip(slice(q, selectPos + 6, nq))
    if (projText == "") {
        macro_error(prog, at, "linq: empty 'select' projection")
        return "(0)"
    }
    // `select c` (the range variable alone) is the identity projection — no `_select` is emitted.
    let identitySelect = (projText == varName)
    return build_string() $(var writer) {
        writer |> write("( _fold( each(")
        writer |> write(srcText)
        writer |> write(")")
        if (hasWhere) {
            writer |> write(" |> _where($(")
            writer |> write(varName)
            writer |> write(") => ")
            writer |> write(predText)
            writer |> write(")")
        }
        if (!identitySelect) {
            writer |> write(" |> _select($(")
            writer |> write(varName)
            writer |> write(") => ")
            writer |> write(projText)
            writer |> write(")")
        }
        writer |> write(" |> to_array() ) )")
    }
}

[reader_macro(name=linq)]
class LinqDasReader : AstReaderMacro {
    //! C#-style LINQ query reader macro. ``%linq! from c in src where <pred> select <proj> %%`` rewrites
    //! to a ``_fold(...)`` chain over an array source. v1: ``from``/``where``/``select`` only, single
    //! range variable (spliced verbatim as the block parameter), array source.
    def override accept(prog : ProgramPtr; mod : Module?; var expr : ExprReader?; ch : int; info : LineInfo) : bool {
        append(expr.sequence, ch)
        if (ends_with(expr.sequence, "%%")) {
            resize(expr.sequence, length(expr.sequence) - 2)
            return false
        }
        return true
    }
    def override suffix(prog : ProgramPtr; mod : Module?; var expr : ExprReader?; info : LineInfo;
                        var outLine : int&; var outFile : FileInfo?&) : string {
        return transpile_query(string(expr.sequence), prog, info)
    }
}
