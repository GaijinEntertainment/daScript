options gen2

module linq_das

require strings
require daslib/ast
require daslib/ast_boost
require daslib/templates_boost
require daslib/linq_boost public

// ===== C#-style LINQ query syntax (linq_das) — from/where/orderby/join/select|group over any _fold source =====
//
// The `%linq! … %%` inline reader macro rewrites a C#-like query into a `_fold(...)` chain:
//
//     %linq! from c in cars where c.price > 100 orderby c.price select c.name %%
//        →   ( _fold( each(cars) |> _where($(c) => c.price > 100) |> _order_by($(c) => c.price) |> _select($(c) => c.name) |> to_array() ) )
//     %linq! from c in cars group c by c.brand %%
//        →   ( _fold( each(cars) |> _group_by_lazy($(c) => c.brand) |> to_array() ) )
//     %linq! from c in cars join d in dealers on c.brand equals d.brand select (N = c.name, City = d.city) %%
//        →   ( _fold( each(cars) |> _join(each(dealers), $(c, d) => c.brand == d.brand, $(c, d) => (N = c.name, City = d.city)) |> to_array() ) )
//
// An untyped `from c in <arr>` is an array source. A typed `from c : Row in <src>` selects a non-array
// source: `decs` (a keyword marker → `from_decs_template`), or any value whose type the `from_in` call
// macro dispatches (a SQL runner → `select_from`, an XML node → `from_xml_node`). The range variable is
// spliced verbatim as the block parameter. `orderby <expr> [descending]` is a single sort key. `group c
// by <key>` is a terminal yielding `tuple<key; array<elem>>` per bucket — IGrouping, in-memory sources
// only (over SQL it errors: SQL needs an aggregating projection). A trailing `iterator` keyword yields an
// `iterator<T>` over the materialized (fused) result (via `to_sequence()`) instead of the default `array<T>`.
//
// `join <d> [: RowB] in <B> on <keyA> equals <keyB>` is a single inner equi-join (see transpile_join). A
// `where` before `join` filters the left source (single range var, pushes down to SQL). A plain `join …
// select <proj>` is select-terminal — the `select` becomes the join's result row, both `c` and `d` in
// scope. A scalar or named-tuple projection pushes down to SQL; a whole-row `select c` is in-memory only
// (over SQL it has no column form). Post-join `where`/`orderby`, and a `group` terminal, carry both `c`
// and `d` past the join as a `(c, d)` pair (transparent identifier) and run in-memory (over SQL the
// carried whole-row tuple errors — project columns in a select-terminal join).

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

// Source builder for a clause: array (untyped) → each(...); decs (keyword marker) → from_decs_template;
// any other typed value source → from_in (dispatches by the source value's type).
def private build_src(rowType : string; srcText : string) : string {
    if (rowType == "") { return "each({srcText})" }
    if (srcText == "decs") { return "from_decs_template(type<{rowType}>)" }
    return "from_in({srcText}, type<{rowType}>)"
}

// Rewrite whole-word identifiers named in `names` to `prefix.<name>` (the transparent-identifier
// substitution after a join). A word immediately following `.` (a member access) is left alone, and
// depth is irrelevant — refs at any paren depth are rewritten. Plain string content is copied verbatim,
// but a string INTERPOLATION (`"…{expr}…"`) is daslang code, so its `{…}` body is rewritten too (with
// one level of nested string literal inside the interpolation copied verbatim). Refs at any paren depth.
def private rewrite_idents(text : string; names : array<string>; prefix : string) : string {
    return build_string() $(var w) {
        peek_data(text) $(arr) {
            let n = length(arr)
            var i = 0
            var inStr = false   // inside a "…" literal (plain content portion)
            var interp = 0      // brace depth inside a string interpolation (>0 ⇒ code mode within the string)
            var prevSig = 0     // last significant (non-whitespace) byte, 0 = none
            while (i < n) {
                let c = int(arr[i])
                // Plain string content (not in an interpolation): copy verbatim; `{` opens interpolation, `"` closes the string.
                if (inStr && interp == 0) {
                    if (c == '\\' && i + 1 < n) { w |> write(slice(text, i, i + 2)); i += 2; continue }
                    w |> write(slice(text, i, i + 1))
                    if (c == '"') { inStr = false; prevSig = c }
                    elif (c == '{') { interp = 1; prevSig = 0 }
                    i++
                    continue
                }
                // Code mode — top-level, or inside a string interpolation.
                if (c == '"') {
                    if (interp > 0) {   // nested string inside interpolation — copy verbatim, no rewrite
                        w |> write(slice(text, i, i + 1)); i++
                        while (i < n) {
                            let cc = int(arr[i])
                            if (cc == '\\' && i + 1 < n) { w |> write(slice(text, i, i + 2)); i += 2; continue }
                            w |> write(slice(text, i, i + 1)); i++
                            if (cc == '"') break
                        }
                        prevSig = '"'
                        continue
                    }
                    inStr = true; prevSig = c; w |> write(slice(text, i, i + 1)); i++
                    continue
                }
                if (interp > 0 && c == '{') { interp ++; w |> write(slice(text, i, i + 1)); prevSig = c; i++; continue }
                if (interp > 0 && c == '}') {
                    interp --
                    w |> write(slice(text, i, i + 1))
                    prevSig = interp == 0 ? '"' : c   // back to plain string content when the interpolation closes
                    i++
                    continue
                }
                let atWordStart = is_ident_byte(c) && (i == 0 || !is_ident_byte(int(arr[i - 1])))
                if (atWordStart) {
                    var j = i
                    while (j < n && is_ident_byte(int(arr[j]))) { j++ }
                    let word = slice(text, i, j)
                    var hit = false
                    if (prevSig != '.') {
                        for (nm in names) {
                            if (nm == word) { hit = true; break }
                        }
                    }
                    w |> write(hit ? "{prefix}.{word}" : word)
                    prevSig = int(arr[j - 1])
                    i = j
                    continue
                }
                w |> write(slice(text, i, i + 1))
                if (!is_white_space(c)) { prevSig = c }
                i++
            }
        }
    }
}

// Synthetic single parameter that carries the joined `(c, d)` pair past the join (transparent
// identifier). Post-join clauses are rewritten to address the carried fields `linq_join_ti.c` /
// `linq_join_ti.d`. Reserved — a range variable may not use this name (`__`-prefixed names are
// reserved by the compiler, so this stays single-token).
let JOIN_TI = "linq_join_ti"

// Emit the rewrite for a query that contains a `join … on … equals …` clause:
//   from <c> [: RowA] in <A> [where <pred>] join <d> [: RowB] in <B> on <keyA> equals <keyB>
//      [where <pred>] [orderby <key> [descending]] ( select <proj> | group <c> by <key> ) [iterator]
// Single inner equi-join. A `where` before `join` filters the left source (single range var `c`, no
// rewrite). Two emit shapes, decided by the post-join clauses (the reader transpiles before type
// inference and cannot know the source, so it picks textually):
//   - select-terminal (no post-join where/orderby, terminal = select): the `select` projection IS the
//     `_join` result selector — both `c`,`d` in scope, spliced verbatim. Works on all 4 sources
//     (incl. SQL pushdown).
//   - transparent-identifier (post-join where/orderby, or a group terminal): the join carries
//     `(c = c, d = d)`; post-join clauses rewrite `c`/`d` to the carried fields and stream/sort/group
//     the pairs directly. In-memory only (over a SQL source the carried whole-row tuple trips `_sql`'s
//     column-ref check — the documented boundary). group yields `(key, array<pair>)` per bucket.
def private transpile_join(q : string; nq : int; varName : string; rowType : string;
                           srcStart : int; joinPos : int; prog : ProgramPtr; at : LineInfo) : string {
    if (find_kw_depth0(q, "join", joinPos + 4) >= 0) {
        macro_error(prog, at, "linq: only one 'join' per query is supported (nested joins are not yet supported)")
        return "(0)"
    }
    if (varName == JOIN_TI) {
        macro_error(prog, at, "linq: range variable name '{JOIN_TI}' is reserved")
        return "(0)"
    }
    // At most one `where` — its position (before `join` vs after `equals`) decides pre- vs post-join.
    let firstWhere = find_kw_depth0(q, "where", srcStart)
    if (firstWhere >= 0 && find_kw_depth0(q, "where", firstWhere + 5) >= 0) {
        macro_error(prog, at, "linq: at most one 'where' per query is supported")
        return "(0)"
    }
    // Join clause: join <d> [: RowB] in <B> on <keyA> equals <keyB>
    let v2 = read_ident(q, joinPos + 4)
    let var2Name = v2._0
    if (var2Name == "") {
        macro_error(prog, at, "linq: expected a range variable after 'join'")
        return "(0)"
    }
    if (var2Name == JOIN_TI || var2Name == varName) {
        macro_error(prog, at, "linq: join range variable '{var2Name}' must differ from '{varName}' and the reserved '{JOIN_TI}'")
        return "(0)"
    }
    let in2 = find_kw_depth0(q, "in", v2._1)
    if (in2 < 0) {
        macro_error(prog, at, "linq: expected 'in' after the join range variable '{var2Name}'")
        return "(0)"
    }
    let between2 = strip(slice(q, v2._1, in2))
    var rowType2 = ""
    if (starts_with(between2, ":")) {
        rowType2 = strip(slice(between2, 1, length(between2)))
        if (rowType2 == "") {
            macro_error(prog, at, "linq: expected a row type after ':' in the join clause (e.g. `join d : Dealer in db`)")
            return "(0)"
        }
    } elif (between2 != "") {
        macro_error(prog, at, "linq: unexpected '{between2}' between the join range variable and 'in'")
        return "(0)"
    }
    let onPos = find_kw_depth0(q, "on", in2 + 2)
    if (onPos < 0) {
        macro_error(prog, at, "linq: join requires an 'on <keyA> equals <keyB>' clause")
        return "(0)"
    }
    let src2Text = strip(slice(q, in2 + 2, onPos))
    if (src2Text == "") {
        macro_error(prog, at, "linq: empty join source after 'in'")
        return "(0)"
    }
    let equalsPos = find_kw_depth0(q, "equals", onPos + 2)
    if (equalsPos < 0) {
        macro_error(prog, at, "linq: join 'on' requires 'equals' (e.g. `on c.id equals d.cid`)")
        return "(0)"
    }
    // `join … into g` (group-join) and `select … into` continuations are not yet supported.
    if (find_kw_depth0(q, "into", equalsPos + 6) >= 0) {
        macro_error(prog, at, "linq: 'join … into' (group-join) and 'into' continuations are not yet supported")
        return "(0)"
    }
    let orderByPos = find_kw_depth0(q, "orderby", equalsPos + 6)
    let selectPos = find_kw_depth0(q, "select", equalsPos + 6)
    let groupPos = find_kw_depth0(q, "group", equalsPos + 6)
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
    if (isGroup && orderByPos >= 0) {
        macro_error(prog, at, "linq: 'orderby' before 'group' is not supported — order the groups after a 'select', or drop the 'orderby'")
        return "(0)"
    }
    // Single equi-key only — a second depth-0 `equals` before the terminal is a composite key.
    let equals2 = find_kw_depth0(q, "equals", equalsPos + 6)
    if (equals2 >= 0 && equals2 < termPos) {
        macro_error(prog, at, "linq: join 'on' supports a single equi-key only — composite keys (`a equals b && c equals d`) need raw SQL")
        return "(0)"
    }
    // A `where` is valid before `join` (filters c, single var, no rewrite — pushes to SQL) or after
    // `equals` (filters the carried pair, transparent-id rewrite); inside the join clause is a grammar
    // error. Position decides pre- vs post-join emit.
    let hasWhere = firstWhere >= 0
    let preWhere = hasWhere && firstWhere < joinPos
    if (hasWhere && !preWhere && firstWhere < equalsPos) {
        macro_error(prog, at, "linq: 'where' cannot appear inside the join clause (between 'join' and 'equals')")
        return "(0)"
    }
    // keyB ends at the first post-join clause keyword after `equals` (a post-join where, or orderby/term).
    let postWhere = (hasWhere && firstWhere > equalsPos) ? firstWhere : -1
    var keyBEnd = termPos
    if (orderByPos >= 0 && orderByPos < keyBEnd) { keyBEnd = orderByPos }
    if (postWhere >= 0 && postWhere < keyBEnd) { keyBEnd = postWhere }
    let keyA = strip(slice(q, onPos + 2, equalsPos))
    let keyB = strip(slice(q, equalsPos + 6, keyBEnd))
    if (keyA == "" || keyB == "") {
        macro_error(prog, at, "linq: empty join key — expected `on <keyA> equals <keyB>`")
        return "(0)"
    }
    let srcAEnd = preWhere ? firstWhere : joinPos
    let srcAText = strip(slice(q, srcStart, srcAEnd))
    if (srcAText == "") {
        macro_error(prog, at, "linq: empty source expression after 'in'")
        return "(0)"
    }
    var wherePred = ""
    if (hasWhere) {
        let wEnd = preWhere ? joinPos : (orderByPos >= 0 ? orderByPos : termPos)
        wherePred = strip(slice(q, firstWhere + 5, wEnd))
        if (wherePred == "") {
            macro_error(prog, at, "linq: empty 'where' predicate")
            return "(0)"
        }
    }
    var orderKey = ""
    var orderDesc = false
    if (orderByPos >= 0) {
        let rawOrder = strip(slice(q, orderByPos + 7, termPos))
        let od = strip_trailing_keyword(rawOrder, "descending")
        if (od._1) {
            orderKey = od._0; orderDesc = true
        } else {
            orderKey = strip_trailing_keyword(rawOrder, "ascending")._0
        }
        if (orderKey == "") {
            macro_error(prog, at, "linq: empty 'orderby' expression")
            return "(0)"
        }
    }
    var projText = ""
    var groupKey = ""
    var wantIterator = false
    if (isGroup) {
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
    }
    let srcA = build_src(rowType, srcAText)
    let srcB = build_src(rowType2, src2Text)
    let onLambda = "$({varName}, {var2Name}) => {keyA} == {keyB}"
    let carryLambda = "$({varName}, {var2Name}) => ({varName} = {varName}, {var2Name} = {var2Name})"
    let names <- [varName, var2Name]
    let hasOrderBy = orderByPos >= 0
    // A pre-join `where` (single range var `c`) filters the left source — emitted before `_join`, no
    // rewrite; pushes down to SQL. Only a POST-join where/orderby/group forces the transparent-id carry.
    let preWhereClause = preWhere ? " |> _where($({varName}) => {wherePred})" : ""
    let useTI = postWhere >= 0 || hasOrderBy || isGroup
    return build_string() $(var w) {
        w |> write("( _fold( {srcA}{preWhereClause}")
        if (!useTI) {
            // Select-terminal: the `select` projection IS the `_join` result selector (both c, d in scope).
            w |> write(" |> _join({srcB}, {onLambda}, $({varName}, {var2Name}) => {projText})")
        } else {
            // Transparent-id: carry (c, d); post-join clauses rewrite c/d to the carried fields and
            // stream/sort/group the pairs directly (single stage — no pre-materialize).
            w |> write(" |> _join({srcB}, {onLambda}, {carryLambda})")
            if (postWhere >= 0) { w |> write(" |> _where($({JOIN_TI}) => {rewrite_idents(wherePred, names, JOIN_TI)})") }
            if (hasOrderBy) { w |> write(" |> {orderDesc ? "_order_by_descending" : "_order_by"}($({JOIN_TI}) => {rewrite_idents(orderKey, names, JOIN_TI)})") }
            if (isGroup) {
                w |> write(" |> _group_by_lazy($({JOIN_TI}) => {rewrite_idents(groupKey, names, JOIN_TI)})")
            } else {
                w |> write(" |> _select($({JOIN_TI}) => {rewrite_idents(projText, names, JOIN_TI)})")
            }
        }
        w |> write(" |> to_array() )")
        if (wantIterator) { w |> write(" |> to_sequence()") }
        w |> write(" )")
    }
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
    // A `join` clause introduces a second range variable — its own grammar and emit (see transpile_join).
    let joinPos = find_kw_depth0(q, "join", srcStart)
    if (joinPos >= 0) {
        return transpile_join(q, nq, varName, rowType, srcStart, joinPos, prog, at)
    }
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
    let srcBuild = build_src(rowType, srcText)
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
    //! ``%linq! from c [: Row] in src [where <pred>] [join d [: RowB] in B on <kA> equals <kB>] [orderby <expr> [descending]] ( select <proj> | group c by <key> ) [iterator] %%``
    //! rewrites to a ``_fold(...)`` chain. Sources: array (untyped `from c in arr`), or a typed
    //! ``from c : Row in src`` over decs / SQL / XML. ``orderby`` is a single sort key; ``group c by <key>``
    //! is a terminal yielding ``tuple<key; array<elem>>`` buckets (in-memory sources only). ``join`` is a
    //! single inner equi-join; a select-terminal ``join … select <proj>`` projects the join's result row
    //! (scalar or named-tuple pushes down to SQL; whole-row ``select c`` is in-memory only), while a
    //! post-join ``where``/``orderby`` or a ``group`` terminal carries the ``(c, d)`` pair (in-memory). A
    //! trailing ``iterator`` keyword yields an ``iterator<T>`` over the materialized result instead of the
    //! ``array<T>``.
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
