options gen2

module linq_das

require strings
require daslib/ast
require daslib/ast_boost
require daslib/templates_boost
require daslib/linq_boost public

// ===== C#-style LINQ query syntax (linq_das) — from/where/orderby/select|group over any _fold source =====
//
// The `%linq! … %%` inline reader macro rewrites a C#-like query into a `_fold(...)` chain:
//
//     %linq! from c in cars where c.price > 100 orderby c.price select c.name %%
//        →   ( _fold( each(cars) |> _where($(c) => c.price > 100) |> _order_by($(c) => c.price) |> _select($(c) => c.name) |> to_array() ) )
//     %linq! from c in cars group c by c.brand %%
//        →   ( _fold( each(cars) |> _group_by_lazy($(c) => c.brand) |> to_array() ) )
//
// An untyped `from c in <arr>` is an array source. A typed `from c : Row in <src>` selects a non-array
// source: `decs` (a keyword marker → `from_decs_template`), or any value whose type the `from_in` call
// macro dispatches (a SQL runner → `select_from`, an XML node → `from_xml_node`). The range variable is
// spliced verbatim as the block parameter. `orderby <expr> [descending]` is a single sort key. `group c
// by <key>` is a terminal yielding `tuple<key; array<elem>>` per bucket — IGrouping, in-memory sources
// only (over SQL it errors: SQL needs an aggregating projection). A trailing `iterator` keyword yields an
// `iterator<T>` over the materialized (fused) result (via `to_sequence()`) instead of the default `array<T>`.

def private is_ident_byte(c : int) : bool {
    return is_alnum(c) || c == '_'
}

// Replace `//` line and `/* */` block comments with spaces (newlines preserved, so the spliced text's
// line/column stay accurate), skipping comment markers inside string literals.
def private strip_query_comments(query : string) : string {
    return modify_data(query) $(var arr) {
        let n = length(arr)
        var i = 0
        var inStr = false
        while (i < n) {
            let c = int(arr[i])
            if (inStr) {
                if (c == '\\') { i += 2; continue }
                if (c == '"') { inStr = false }
                i++
                continue
            }
            if (c == '"') { inStr = true; i++; continue }
            if (c == '/' && i + 1 < n && int(arr[i + 1]) == '/') {
                while (i < n && int(arr[i]) != '\n' && int(arr[i]) != '\r') { arr[i] = uint8(' '); i++ }
                continue
            }
            if (c == '/' && i + 1 < n && int(arr[i + 1]) == '*') {
                arr[i] = uint8(' '); arr[i + 1] = uint8(' '); i += 2
                while (i < n) {
                    if (int(arr[i]) == '*' && i + 1 < n && int(arr[i + 1]) == '/') {
                        arr[i] = uint8(' '); arr[i + 1] = uint8(' '); i += 2
                        break
                    }
                    if (int(arr[i]) != '\n' && int(arr[i]) != '\r') { arr[i] = uint8(' ') }
                    i++
                }
                continue
            }
            i++
        }
    }
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

// If `text` ends with a standalone `kw` keyword (preceded by whitespace, so a member access like
// `c.iterator` / `c.descending` is not matched), strip it and return (remainder, true).
def private strip_trailing_keyword(text : string; kw : string) : tuple<string; bool> {
    let p = strip(text)
    let m = length(kw)
    let n = length(p)
    if (n <= m || !ends_with(p, kw)) { return (p, false) }
    var wsBefore = false
    peek_data(p) $(arr) {
        wsBefore = is_white_space(int(arr[n - m - 1]))
    }
    if (wsBefore) { return (strip(slice(p, 0, n - m)), true) }
    return (p, false)
}

// Parse the query and emit the `_fold(...)` rewrite text. On a malformed query, register a macro error
// and emit a benign `(0)` so the surrounding statement still parses (the error is the real signal).
def private transpile_query(query : string; prog : ProgramPtr; at : LineInfo) : string {
    let q = strip(strip_query_comments(query))
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
    // Optional row-type annotation `: Row` between the range variable and `in` (selects a typed source).
    let between = strip(slice(q, afterVar, inPos))
    var rowType = ""
    if (starts_with(between, ":")) {
        rowType = strip(slice(between, 1, length(between)))
        if (rowType == "") {
            macro_error(prog, at, "linq: expected a row type after ':' (e.g. `from c : Car in db`)")
            return "(0)"
        }
    } elif (between != "") {
        macro_error(prog, at, "linq: unexpected '{between}' between range variable and 'in'")
        return "(0)"
    }
    // Clauses in C# order, each searched after the previous so positions stay ordered:
    //   from … [where <pred>] [orderby <expr> [descending]] ( select <proj> | group <var> by <key> ) [iterator]
    let srcStart = inPos + 2
    let wherePos = find_kw_depth0(q, "where", srcStart)
    let afterWhere = wherePos >= 0 ? wherePos + 5 : srcStart
    let orderByPos = find_kw_depth0(q, "orderby", afterWhere)
    let afterOrderBy = orderByPos >= 0 ? orderByPos + 7 : afterWhere
    let selectPos = find_kw_depth0(q, "select", afterOrderBy)
    let groupPos = find_kw_depth0(q, "group", afterOrderBy)
    if (selectPos < 0 && groupPos < 0) {
        macro_error(prog, at, "linq: query must end with a 'select' or 'group … by …' clause")
        return "(0)"
    }
    if (selectPos >= 0 && groupPos >= 0) {
        macro_error(prog, at, "linq: query cannot have both 'select' and 'group' — they are alternative terminals")
        return "(0)"
    }
    let isGroup = groupPos >= 0
    let termPos = isGroup ? groupPos : selectPos
    // `orderby` before `group` would emit `_order_by |> _group_by_lazy`, which `_fold` does not fuse.
    if (isGroup && orderByPos >= 0) {
        macro_error(prog, at, "linq: 'orderby' before 'group' is not supported — order the groups after a 'select', or drop the 'orderby'")
        return "(0)"
    }
    let srcEnd = wherePos >= 0 ? wherePos : (orderByPos >= 0 ? orderByPos : termPos)
    let srcText = strip(slice(q, srcStart, srcEnd))
    if (srcText == "") {
        macro_error(prog, at, "linq: empty source expression after 'in'")
        return "(0)"
    }
    let hasWhere = wherePos >= 0
    var predText = ""
    if (hasWhere) {
        predText = strip(slice(q, wherePos + 5, orderByPos >= 0 ? orderByPos : termPos))
        if (predText == "") {
            macro_error(prog, at, "linq: empty 'where' predicate")
            return "(0)"
        }
    }
    // orderby clause (select-terminated only): `<expr> [descending|ascending]`.
    let hasOrderBy = orderByPos >= 0
    var orderKey = ""
    var orderDesc = false
    if (hasOrderBy) {
        let rawOrder = strip(slice(q, orderByPos + 7, termPos))
        let od = strip_trailing_keyword(rawOrder, "descending")
        if (od._1) {
            orderKey = od._0; orderDesc = true
        } else {
            // a trailing `ascending` is the default — strip it if present
            orderKey = strip_trailing_keyword(rawOrder, "ascending")._0
        }
        if (orderKey == "") {
            macro_error(prog, at, "linq: empty 'orderby' expression")
            return "(0)"
        }
    }
    // Terminal: select projection, or group element/key.
    var projText = ""
    var identitySelect = false
    var groupKey = ""
    var wantIterator = false
    if (isGroup) {
        // `group <elem> by <key> [iterator]` — element must be the range variable (identity; element
        // selectors are deferred). The group yields `tuple<key; array<elem>>` per bucket (IGrouping).
        let groupBody = strip(slice(q, termPos + 5, nq))
        let byPos = find_kw_depth0(groupBody, "by", 0)
        if (byPos < 0) {
            macro_error(prog, at, "linq: 'group' requires a 'by' clause (e.g. `group {varName} by {varName}.field`)")
            return "(0)"
        }
        let elem = strip(slice(groupBody, 0, byPos))
        if (elem != varName) {
            macro_error(prog, at, "linq: 'group' element must be the range variable '{varName}' — element selectors are not yet supported (write `group {varName} by …`)")
            return "(0)"
        }
        let gk = strip_trailing_keyword(strip(slice(groupBody, byPos + 2, length(groupBody))), "iterator")
        groupKey = gk._0
        wantIterator = gk._1
        if (groupKey == "") {
            macro_error(prog, at, "linq: empty 'group … by' key expression")
            return "(0)"
        }
    } else {
        let pj = strip_trailing_keyword(strip(slice(q, termPos + 6, nq)), "iterator")
        projText = pj._0
        wantIterator = pj._1
        if (projText == "") {
            macro_error(prog, at, "linq: empty 'select' projection")
            return "(0)"
        }
        // `select c` (the range variable alone) is the identity projection — no `_select` is emitted.
        identitySelect = (projText == varName)
    }
    // Source builder: array (untyped) → each(...); decs (keyword marker) → from_decs_template;
    // any other typed value source → from_in (dispatches by the source value's type).
    var srcBuild = ""
    if (rowType == "") {
        srcBuild = "each({srcText})"
    } elif (srcText == "decs") {
        srcBuild = "from_decs_template(type<{rowType}>)"
    } else {
        srcBuild = "from_in({srcText}, type<{rowType}>)"
    }
    return build_string() $(var writer) {
        writer |> write("( _fold( {srcBuild}")
        if (hasWhere) {
            writer |> write(" |> _where($({varName}) => {predText})")
        }
        if (isGroup) {
            writer |> write(" |> _group_by_lazy($({varName}) => {groupKey})")
        } else {
            if (hasOrderBy) {
                writer |> write(" |> {orderDesc ? "_order_by_descending" : "_order_by"}($({varName}) => {orderKey})")
            }
            if (!identitySelect) {
                writer |> write(" |> _select($({varName}) => {projText})")
            }
        }
        writer |> write(" |> to_array() )")
        if (wantIterator) {
            writer |> write(" |> to_sequence()")
        }
        writer |> write(" )")
    }
}

[call_macro(name="from_in")]
class FromInMacro : AstCallMacro {
    //! Typed-source dispatcher for the C# query form `from c : Row in <src>`. Rewrites
    //! `from_in(src, type<Row>)` to the concrete `_fold` source builder by `src`'s type — a SQL runner
    //! → `select_from`, an XML node → `from_xml_node`. Must be a call macro: a plain function would leave
    //! `from_in(...)` at the chain head, which `_fold`'s name-based source detection cannot route.
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : ExpressionPtr {
        if (length(call.arguments) != 2) {
            macro_error(prog, call.at, "from_in(src, type<Row>): expected 2 arguments")
            return call
        }
        let srcT = call.arguments[0]._type
        if (srcT == null) {
            macro_error(prog, call.at, "from_in: source type is not inferred")
            return call
        }
        // SQL: db is a sqlite_boost::SqlRunner → select_from(db, type<Row>)
        if (srcT.structType != null && srcT.structType.name == "SqlRunner"
                && srcT.structType._module != null && srcT.structType._module.name == "sqlite_boost") {
            return qmacro($c("select_from")($e(call.arguments[0]), $e(call.arguments[1])))
        }
        // XML: node is a pugixml `xml_node` (handled type) → unsafe(from_xml_node(node, type<Row>)).
        // from_xml_node is [unsafe_outside_of_for]; the unsafe wrap is required in a `_fold` chain.
        if (srcT.isHandle && srcT.annotation != null && srcT.annotation.name == "xml_node") {
            return qmacro(unsafe($c("from_xml_node")($e(call.arguments[0]), $e(call.arguments[1]))))
        }
        macro_error(prog, call.at, "linq: unsupported source for `from c : Row in <src>` — expected a SQL runner or an XML node (use `in decs` for decs, or an array for the untyped `from c in <arr>` form)")
        return call
    }
}

[reader_macro(name=linq)]
class LinqDasReader : AstReaderMacro {
    //! C#-style LINQ query reader macro.
    //! ``%linq! from c [: Row] in src [where <pred>] [orderby <expr> [descending]] ( select <proj> | group c by <key> ) [iterator] %%``
    //! rewrites to a ``_fold(...)`` chain. Sources: array (untyped `from c in arr`), or a typed
    //! ``from c : Row in src`` over decs / SQL / XML. ``orderby`` is a single sort key; ``group c by <key>``
    //! is a terminal yielding ``tuple<key; array<elem>>`` buckets (in-memory sources only). A trailing
    //! ``iterator`` keyword yields an ``iterator<T>`` over the materialized result instead of the
    //! ``array<T>``. Single range variable, spliced verbatim.
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
