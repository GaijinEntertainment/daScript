options gen2
options indenting = 4
options strict_smart_pointers = true

module match_solver shared private

//! Compile-time coverage analysis for ``daslib/match``: which arms no value can reach, and which
//! values no arm matches.
//!
//! The arms become a table. Every value matched is a column; a struct or tuple column splits into
//! one column per field. Every arm adds a row - an arm with alternatives adds one row per
//! alternative - and each cell classifies what its pattern covers of the column: every value
//! (``_``, a name, an irrefutable shape), a set of named values (constants, enum values, variant
//! alternatives, range pieces, ``non-null``), or values the analysis can not name.
//! A column of an enum, bool, variant or pointer has a finite domain of named values.
//! A row is useful when some value reaches it that no earlier row covers; an arm with no useful row
//! is unreachable. Values the rows leave uncovered are reported, at most three, as the missing
//! cases. A guarded arm covers nothing, except arms that guard one pattern with comparisons whose
//! outcomes together are complete.
//! Also holds the pattern-shape helpers the ``match`` macro shares with the analysis.

require daslib/ast_boost
require daslib/strings_boost
require daslib/option

[macro_function]
def public peel_r2v(var e : Expression?) : Expression? {
    //! Strip post-typer ``ExprRef2Value`` wrappers: a typed pattern or value reads as the source spelled it.
    while (e != null && e is ExprRef2Value) {
        e = (e as ExprRef2Value).subexpr
    }
    return e
}

[macro_function]
def public peel_r2v(e : Expression const?) : Expression const? {
    //! Pointee-const flavor — same peel, preserves the caller's const view.
    var c : Expression const? = e
    while (c != null && c is ExprRef2Value) {
        c = (c as ExprRef2Value).subexpr
    }
    return c
}

[macro_function]
def public is_rest(p : Expression const?) : bool {
    let x = peel_r2v(p)
    return x is ExprTag && (x as ExprTag).name == "..."
}

[macro_function]
def public is_arrow_arm(e : Expression?) : bool => e is ExprMakeTuple && length((e as ExprMakeTuple).values) == 2

[macro_function]
def public is_binder(p : Expression?) : bool => p is ExprVar || (p is ExprTag && (p as ExprTag).name == "v")

[macro_function]
def public shape_part(var cell : Expression?) : Expression? {
    //! The destructuring side of a cell: ``n & Point(...)`` is ``Point(...)``; a binder or ``_`` is null (matches anything).
    var p = peel_r2v(cell)
    return null if (p == null || is_binder(p))
    if (p is ExprOp2 && (p as ExprOp2).op == "&") {
        var op2 = p as ExprOp2
        return shape_part(op2.right) if (is_binder(peel_r2v(op2.left)))
        return shape_part(op2.left) if (is_binder(peel_r2v(op2.right)))
    }
    return p
}

[macro_function]
def public op_sides(var e : Expression?; op : string) : array<Expression?> {
    //! The operands of a chain of one binary operator, left to right.
    return <- [e] if (e == null || !(e is ExprOp2) || (e as ExprOp2).op != op)
    var sides <- op_sides((e as ExprOp2).left, op)
    sides |> push_from(op_sides((e as ExprOp2).right, op))
    return <- sides
}

[macro_function]
def public is_range_call(pat : Expression?) : bool => pat is ExprCall && (pat as ExprCall).name == "interval" && length((pat as ExprCall).arguments) == 2

[macro_function]
def public is_range_const(pat : Expression?) : bool => pat is ExprConstRange || pat is ExprConstURange || pat is ExprConstRange64 || pat is ExprConstURange64

let public MATCH_ANY_OTHER = "_"
let private MATCH_NON_NULL = "non-null"

struct private MatchCell {
    any : bool
    opaque : bool
    keys : array<string>
}

struct private MatchDomain {
    finite : bool
    values : array<string>
}

struct private PatternRow {
    arm : int
    @do_not_delete cells : array<Expression?>
}

struct private MatchTable {
    columns : array<TypeDeclPtr>
    labels : array<string>
    rows : array<array<MatchCell>>
    arm_rows : array<array<int>>
    domains : array<MatchDomain>
}

struct public MatchCoverage {
    unreachable : array<int>
    dead_alternatives : array<tuple<arm : int; what : string>>
    all_finite : bool
    has_guards : bool
    missing : Option<string>
}

[macro_function]
def private constant_key(p : Expression?) : string {
    if (p is ExprConstEnumeration) {
        let ce = p as ExprConstEnumeration
        return "{ce.enumType.name}.{ce.value}"
    }
    return p.__rtti |> starts_with("ExprConst") && !is_range_const(p) ? describe(p) : ""
}

[macro_function]
def private int_value(var e : Expression?) : Option<int64> {
    let p = peel_r2v(e)
    return some(int64((p as ExprConstInt).value)) if (p is ExprConstInt)
    return some(int64((p as ExprConstUInt).value)) if (p is ExprConstUInt)
    return some((p as ExprConstInt64).value) if (p is ExprConstInt64)
    return none(type<int64>)
}

[macro_function]
def private constant_span(var e : Expression?) : Option<range64> {
    //! The half-open span of integers a constant range - or a single integer constant - matches.
    var p = peel_r2v(e)
    if (p is ExprConstRange) {
        let r = (p as ExprConstRange).value
        return some(interval(int64(r.x), int64(r.y)))
    } elif (p is ExprConstURange) {
        let r = (p as ExprConstURange).value
        return some(interval(int64(r.x), int64(r.y)))
    } elif (p is ExprConstRange64) {
        return some((p as ExprConstRange64).value)
    } elif (is_range_call(p)) {
        let lo = int_value((p as ExprCall).arguments[0])
        let hi = int_value((p as ExprCall).arguments[1])
        return is_some(lo) && is_some(hi) ? some(interval(unwrap(lo), unwrap(hi))) : none(type<range64>)
    }
    let v = int_value(p)
    return is_some(v) ? some(interval(unwrap(v), unwrap(v) + 1l)) : none(type<range64>)
}

[macro_function]
def private cell_spans(var e : Expression?) : array<range64> {
    //! The constant spans in a cell - a constant range or an integer constant each - through ``|`` and ``&``.
    var p = peel_r2v(e)
    if (p != null && p is ExprOp2 && ((p as ExprOp2).op == "||" || (p as ExprOp2).op == "&")) {
        var spans <- cell_spans((p as ExprOp2).left)
        spans |> push_from(cell_spans((p as ExprOp2).right))
        return <- spans
    }
    let span = p == null ? none(type<range64>) : constant_span(p)
    return <- [for (s in [span]); unwrap(s); where is_some(s)]
}

[macro_function]
def private column_cuts(var rows : array<PatternRow>; c : int) : array<int64> {
    //! A column with constant ranges splits the integers at every constant end, so a range and a constant name the same pieces; empty without ranges.
    var spans : array<range64>
    for (r in rows) {
        spans |> push_from(cell_spans(r.cells[c]))
    }
    let ranged = has_value([for (s in spans); s.y - s.x != 1l], true)
    var cuts : array<int64>
    for (s in spans) {
        cuts |> push_from([s.x, s.y])
    }
    sort(cuts)
    return <- [for (v, i in cuts, count()); v; where ranged && (i == 0 || cuts[i - 1] != v)]
}

[macro_function]
def private span_keys(var p : Expression?; cuts : array<int64>) : array<string> {
    //! The pieces between ``cuts`` a constant span covers, named ``lo..hi`` (``v`` for one value).
    let span = constant_span(p)
    return <- [for (i in range(length(cuts) - 1)); cuts[i + 1] == cuts[i] + 1l ? "{cuts[i]}" : "{cuts[i]}..{cuts[i + 1]}";
        where is_some(span) && cuts[i] >= unwrap(span).x && cuts[i + 1] <= unwrap(span).y]
}

[macro_function]
def private variant_name(typ : TypeDeclPtr) : string {
    //! The name a variant is declared with: its alias, or the ``aka`` name ``describe`` prints for a copied type.
    return string(typ.alias) if (!empty(typ.alias))
    let d = describe(typ)
    let aka_pos = find(d, " aka ")
    return "variant" if (aka_pos < 0)
    let name = slice(d, aka_pos + 5)
    let stop = find(name, " ")
    return stop < 0 ? name : slice(name, 0, stop)
}

[macro_function]
def private variant_key(typ : TypeDeclPtr; alt : das_string) : string => "{variant_name(typ)}({alt} = _)"

[macro_function]
def private is_struct_of(typ : TypeDeclPtr; p : Expression?) : bool {
    //! ``p`` is a one-element struct pattern of exactly ``typ``.
    return false if (typ == null || !typ.isStructure || !(p is ExprMakeStruct))
    let ms = p as ExprMakeStruct
    return (length(ms.structs) == 1 && ms.makeType != null && ms.makeType.isStructure
        && is_same_type(typ, ms.makeType, RefMatters.no, ConstMatters.no, TemporaryMatters.no))
}

[macro_function]
def private is_irrefutable(typ : TypeDeclPtr; var pat : Expression?) : bool {
    //! Irrefutable: the pattern can not fail on any value of ``typ`` (on a pointer, any non-null one) - ``x``, ``_``, ``(a, _)``, ``Point(x = a, y = _)``. Such an arm covers its whole column.
    var p = peel_r2v(pat)
    return true if (is_binder(p))
    if (p is ExprOp2 && (p as ExprOp2).op == "&") return is_irrefutable(typ, (p as ExprOp2).left) && is_irrefutable(typ, (p as ExprOp2).right)
    if (p is ExprMakeTuple) {
        for (v in (p as ExprMakeTuple).values) {
            return false if (!is_rest(v) && !is_irrefutable(null, v))
        }
        return true
    }
    return false if (typ == null || !is_struct_of(typ.isPointer ? typ.firstType : typ, p))
    for (mf in *(p as ExprMakeStruct).structs[0]) {
        return false if (!is_irrefutable(null, mf.value))
    }
    return true
}

[macro_function]
def private either(var l : MatchCell; var r : MatchCell) : MatchCell {
    //! ``p1 | p2`` covers what either side covers.
    l.any ||= r.any
    l.opaque ||= r.opaque
    l.keys |> push_from(r.keys)
    return <- l
}

[macro_function]
def private both(var l : MatchCell; var r : MatchCell) : MatchCell {
    //! ``p1 & p2`` covers what both sides cover.
    return <- r if (l.any)
    return <- l if (r.any)
    return <- MatchCell(opaque = true) if (l.opaque || r.opaque)
    return <- MatchCell(keys <- [for (k in l.keys); k; where has_value(r.keys, k)])
}

[macro_function]
def private classify_cell(typ : TypeDeclPtr; var pat : Expression?; cuts : array<int64>) : MatchCell {
    //! What one pattern covers of one column: everything, named values, or values the analysis can't name.
    var p = peel_r2v(pat)
    return <- MatchCell(any = true) if (is_binder(p))
    if (p is ExprOp2) {
        var op2 = p as ExprOp2
        return <- either(classify_cell(typ, op2.left, cuts), classify_cell(typ, op2.right, cuts)) if (op2.op == "||")
        return <- both(classify_cell(typ, op2.left, cuts), classify_cell(typ, op2.right, cuts)) if (op2.op == "&")
        return <- MatchCell(opaque = true)
    }
    return <- MatchCell(keys <- span_keys(p, cuts)) if (!empty(cuts) && is_some(constant_span(p)))
    let key = constant_key(p)
    return <- MatchCell(keys <- [key]) if (!empty(key))
    if (typ != null && typ.isVariant && p is ExprMakeVariant && length((p as ExprMakeVariant).variants) == 1
            && is_irrefutable(null, (p as ExprMakeVariant).variants[0].value)) {
        return <- MatchCell(keys <- [variant_key(typ, (p as ExprMakeVariant).variants[0].name)])
    }
    if (typ != null && typ.isVariant && p is ExprAsVariant && is_irrefutable(null, (p as ExprAsVariant).value)) {
        return <- MatchCell(keys <- [variant_key(typ, (p as ExprAsVariant).name)])
    }
    return <- MatchCell(opaque = true) if (!is_irrefutable(typ, p))
    return <- MatchCell(keys <- [MATCH_NON_NULL]) if (typ != null && typ.isPointer)
    return <- MatchCell(any = true)
}

[macro_function]
def private finite_domain(typ : TypeDeclPtr) : MatchDomain {
    if (typ != null && typ.isEnum && typ.enumType != null) {
        return <- MatchDomain(finite = true, values <- [for (e in typ.enumType.list); "{typ.enumType.name}.{e.name}"])
    } elif (typ != null && typ.baseType == Type.tBool) {
        return <- MatchDomain(finite = true, values <- ["true", "false"])
    } elif (typ != null && typ.isVariant) {
        return <- MatchDomain(finite = true, values <- [for (n in typ.argNames); variant_key(typ, n)])
    } elif (typ != null && typ.isPointer && typ.firstType != null && typ.firstType.isStructure) {
        return <- MatchDomain(finite = true, values <- ["null", MATCH_NON_NULL])
    }
    return <- MatchDomain()
}

[macro_function]
def private column_domain(col : TypeDeclPtr; c : int; rows : array<array<MatchCell>>) : MatchDomain {
    //! A finite type's values; for any other type, the values the arms name plus "anything else".
    var dom <- finite_domain(col)
    return <- dom if (dom.finite)
    for (r in rows) {
        for (k in r[c].keys) {
            dom.values |> push(k) if (!has_value(dom.values, k))
        }
    }
    dom.values |> push(MATCH_ANY_OTHER)
    return <- dom
}

[macro_function]
def private cell_covers(cell : MatchCell; value : string) : bool => cell.any || cell.keys |> has_value(value)

[macro_function]
def private is_useful(tab : MatchTable; live : array<int>; cand : array<MatchCell>; col : int) : bool {
    //! True when ``cand`` matches some value none of the ``live`` rows matches.
    return empty(live) if (col == length(cand))
    assume cell = cand[col]
    for (v in cell.any || cell.opaque ? tab.domains[col].values : cell.keys) {
        var inscope sub <- [for (ri in live); ri; where cell_covers(tab.rows[ri][col], v)]
        return true if (is_useful(tab, sub, cand, col + 1))
    }
    return false
}

[macro_function]
def private format_witness(values : array<string>; labels : array<string>) : string {
    //! One missing combination: positional ``(a, b)``, or ``(field = value, ...)`` once a column was split into parts.
    if (!has_value([for (l in labels); empty(l)], false)) {
        return length(values) == 1 ? values[0] : "({join(values, ", ")})"
    }
    let items <- [for (v, l in values, labels); empty(l) ? v : "{l} = {v}"; where v != MATCH_ANY_OTHER]
    return empty(items) ? MATCH_ANY_OTHER : "({join(items, ", ")})"
}

[macro_function]
def private missing_values(tab : MatchTable; live : array<int>; col : int; limit : int; path : array<string>) : array<string> {
    //! Value combinations no ``live`` row matches - one more than are shown, so the message can say there are more.
    var found : array<string>
    if (empty(live)) {
        var full := path
        full |> push_from([for (_c in range(col, length(tab.domains))); MATCH_ANY_OTHER])
        found |> push(format_witness(full, tab.labels))
        return <- found
    }
    return <- found if (col == length(tab.domains))
    let all_any = !has_value([for (ri in live); tab.rows[ri][col].any], false)
    for (v in all_any ? [MATCH_ANY_OTHER] : tab.domains[col].values) {
        var inscope next := path
        next |> push(v)
        found |> push_from(missing_values(tab, [for (ri in live); ri; where cell_covers(tab.rows[ri][col], v)], col + 1, limit, next))
        break if (length(found) > limit)
    }
    return <- found
}

[macro_function]
def private pattern_part(var pat : Expression?) : Expression? {
    //! The pattern of an arm without its top-level ``&&`` guard.
    var p = peel_r2v(pat)
    while (p is ExprOp2 && (p as ExprOp2).op == "&&") {
        p = peel_r2v((p as ExprOp2).left)
    }
    return p
}

[macro_function]
def private is_guarded(var pat : Expression?) : bool {
    let p = peel_r2v(pat)
    return p is ExprOp2 && (p as ExprOp2).op == "&&"
}

[macro_function]
def private describe_row(row : array<MatchCell>) : string {
    let cells <- [for (c in row); c.any || c.opaque || empty(c.keys) ? "_" : join(c.keys, " | ")]
    return length(cells) == 1 ? cells[0] : "({join(cells, ", ")})"
}

[macro_function]
def private dead_keys(tab : MatchTable; prev : array<int>; row : array<MatchCell>) : array<string> {
    //! In a single-column row, the named alternatives an earlier arm already matches.
    var dead : array<string>
    return <- dead if (length(row) != 1 || row[0].any || row[0].opaque || length(row[0].keys) < 2)
    dead <- [for (k in row[0].keys); k; where !is_useful(tab, prev, [MatchCell(keys <- [k])], 0)]
    return <- dead
}

[macro_function]
def private alternatives_of(var cell : Expression?) : array<Expression?> {
    //! A cell's top-level ``|`` alternatives, each without a binding ``n &``; null for one that matches anything.
    return <- [for (a in op_sides(shape_part(cell), "||")); shape_part(a)]
}

[macro_function]
def private column_parts(var typ : TypeDeclPtr) : array<tuple<name : string; typ : TypeDeclPtr>> {
    //! The parts a tuple or struct column splits into; none for any other type.
    var parts : array<tuple<name : string; typ : TypeDeclPtr>>
    if (typ != null && typ.baseType == Type.tTuple) {
        parts <- [for (t, i in typ.argTypes, count()); (name = "_{i}", typ = t)]
    } elif (typ != null && typ.isStructure && !typ.isPointer && typ.structType != null) {
        parts <- [for (f in typ.structType.fields); (name = string(f.name), typ = f._type)]
    }
    return <- parts
}

[macro_function]
def private splits(typ : TypeDeclPtr; alt : Expression?) : bool {
    //! The cell spells the whole value part by part: a struct pattern of exactly the column type, or a full tuple.
    return true if (alt == null || is_struct_of(typ, alt))
    return false if (!(alt is ExprMakeTuple) || typ.baseType != Type.tTuple || length((alt as ExprMakeTuple).values) != length(typ.argTypes))
    return !has_value([for (v in (alt as ExprMakeTuple).values); is_rest(v)], true)
}

[macro_function]
def private part_of(var alt : Expression?; idx : int; name : string) : Expression? {
    return null if (alt == null)
    return (alt as ExprMakeTuple).values[idx] if (alt is ExprMakeTuple)
    for (mf in *(alt as ExprMakeStruct).structs[0]) {
        return mf.value if (mf.name == name)
    }
    return null
}

[macro_function]
def private split_column(var tab : MatchTable; var rows : array<PatternRow>; c : int) : bool {
    //! Splits column ``c`` into its parts when every arm spells it part by part; an alternative becomes one row each.
    var parts <- column_parts(tab.columns[c])
    return false if (empty(parts))
    var alts <- [for (r in rows); alternatives_of(r.cells[c])]
    for (ra in alts) {
        return false if (has_value([for (a in ra); splits(tab.columns[c], a)], false))
    }
    var split : array<PatternRow>
    split |> reserve(length(rows))
    for (r, ra in rows, alts) {
        for (a in ra) {
            var nr <- PatternRow(arm = r.arm, cells := r.cells)
            nr.cells |> erase(c)
            nr.cells |> reserve(length(nr.cells) + length(parts))
            for (p, i in parts, count()) {
                nr.cells |> push(part_of(a, i, p.name), c + i)
            }
            split |> emplace(nr)
        }
    }
    delete rows
    rows <- split
    let parent = tab.labels[c]
    tab.columns |> erase(c)
    tab.labels |> erase(c)
    for (p, i in parts, count()) {
        tab.columns |> push(p.typ, c + i)
        tab.labels |> push(empty(parent) ? p.name : "{parent}.{p.name}", c + i)
    }
    return true
}

[macro_function]
def private pattern_rows(var types : array<TypeDeclPtr>; var patterns : array<Expression?>) : array<PatternRow> {
    //! One row of cells per arm, per top-level alternative of a several-value match; a dropped arm has none.
    var rows : array<PatternRow>
    rows |> reserve(length(patterns))
    for (pat, ai in patterns, count()) {
        continue if (pat == null)
        var part = pattern_part(pat)
        var alts <- length(types) > 1 ? alternatives_of(part) : [part]
        for (a in alts) {
            var r <- PatternRow(arm = ai)
            if (length(types) > 1 && a != null && a is ExprMakeTuple && length((a as ExprMakeTuple).values) == length(types)) {
                r.cells <- [for (v in (a as ExprMakeTuple).values); v]
            } else {
                r.cells <- [for (_t in types); a]
            }
            rows |> emplace(r)
        }
    }
    return <- rows
}

[macro_function]
def private coverage_table(var types : array<TypeDeclPtr>; var patterns : array<Expression?>) : MatchTable {
    //! Columns (struct and tuple columns split into their parts), the rows every arm contributes (by index), and each column's value domain.
    var tab <- MatchTable(columns := types, labels <- [for (_t in types); ""])
    var rows <- pattern_rows(types, patterns)
    var c = 0
    while (c < length(tab.columns)) {
        c += split_column(tab, rows, c) ? 0 : 1
    }
    tab.arm_rows <- [for (ai in range(length(patterns))); [for (r, ri in rows, count()); ri; where r.arm == ai]]
    let cuts <- [for (ci in range(length(tab.columns))); column_cuts(rows, ci)]
    tab.rows <- [for (r in rows); [for (cell, ci in r.cells, count()); cell == null ? MatchCell(any = true) : classify_cell(tab.columns[ci], cell, cuts[ci])]]
    tab.domains <- [for (col, ci in tab.columns, count()); column_domain(col, ci, tab.rows)]
    return <- tab
}

bitfield private GuardCover {
    //! The outcomes a guard covers: of a comparison between two values, or of a bool.
    less
    equal
    greater
    holds
    fails
}

struct private GuardFact {
    key : string
    mask : GuardCover
}

[macro_function]
def private is_ordered_type(typ : TypeDeclPtr) : bool {
    //! Comparisons over these types are total: exactly one of ``<``, ``==``, ``>`` holds. Floats are not (NaN).
    return false if (typ == null)
    return (typ.isEnum || typ.baseType == Type.tString || typ.baseType == Type.tInt || typ.baseType == Type.tUInt
        || typ.baseType == Type.tInt8 || typ.baseType == Type.tUInt8 || typ.baseType == Type.tInt16
        || typ.baseType == Type.tUInt16 || typ.baseType == Type.tInt64 || typ.baseType == Type.tUInt64)
}

[macro_function]
def private operand_type(var e : Expression?; var names : table<string; TypeDeclPtr>) : TypeDeclPtr {
    //! The type of a guard operand: a name the pattern binds at a column, or an already typed variable or constant.
    var x = peel_r2v(e)
    return null if (x == null || !(x is ExprVar || x.__rtti |> starts_with("ExprConst")))
    if (x is ExprVar) {
        let n = string((x as ExprVar).name)
        return names[n] if (names |> key_exists(n))
    }
    return x._type
}

[macro_function]
def private guard_fact(var guard : Expression?; pattern_key : string; var names : table<string; TypeDeclPtr>) : Option<GuardFact> {
    //! What a single comparison guard covers of the relation between its two operands; none when it can't tell.
    var g = peel_r2v(guard)
    let negated = g is ExprOp1 && (g as ExprOp1).op == "!"
    g = negated ? peel_r2v((g as ExprOp1).subexpr) : g
    if (g is ExprOp2) {
        var op2 = g as ExprOp2
        let masks <- { "<" => GuardCover.less, "<=" => GuardCover.less | GuardCover.equal, "==" => GuardCover.equal,
            "!=" => GuardCover.less | GuardCover.greater, ">=" => GuardCover.greater | GuardCover.equal, ">" => GuardCover.greater }
        var mask = masks?[string(op2.op)] ?? default<GuardCover>
        if (mask != default<GuardCover> && is_ordered_type(operand_type(op2.left, names)) && is_ordered_type(operand_type(op2.right, names))) {
            var l = describe(peel_r2v(op2.left))
            var r = describe(peel_r2v(op2.right))
            if (l > r) {
                swap(l, r)
                let less = mask.less
                mask.less = mask.greater
                mask.greater = less
            }
            let full = GuardCover.less | GuardCover.equal | GuardCover.greater
            return some(GuardFact(key = "{pattern_key} | {l} ? {r}", mask = negated ? full & ~mask : mask))
        }
    }
    let gt = operand_type(g, names)
    return some(GuardFact(key = "{pattern_key} | {describe(g)}", mask = negated ? GuardCover.fails : GuardCover.holds)) if (gt != null && gt.baseType == Type.tBool)
    return none(type<GuardFact>)
}

[macro_function]
def private bound_column_types(var columns : array<TypeDeclPtr>; var pat : Expression?) : table<string; TypeDeclPtr> {
    //! Names the pattern binds directly at a column, with that column's type; a single tuple column binds per element.
    var names : table<string; TypeDeclPtr>
    var p = peel_r2v(pat)
    if (length(columns) == 1 && p is ExprVar) {
        names[string((p as ExprVar).name)] = columns[0]
    }
    let per_element = length(columns) == 1 && columns[0].baseType == Type.tTuple
    var cols <- per_element ? [for (t in columns[0].argTypes); t] : [for (t in columns); t]
    return <- names if (!(p is ExprMakeTuple) || length((p as ExprMakeTuple).values) != length(cols))
    var vars <- [for (v in (p as ExprMakeTuple).values); peel_r2v(v)]
    return <- {for (v, col in vars, cols); string((v as ExprVar).name) => col; where v is ExprVar}
}

[macro_function]
def private guards_complete(var types : array<TypeDeclPtr>; var pat : Expression?; var guard_masks : table<string; GuardCover>) : bool {
    //! Arms with one pattern and comparison guards over the same operands cover that pattern once their guards cover every outcome.
    var p = peel_r2v(pat)
    return false if (is_guarded((p as ExprOp2).left))
    var part = pattern_part(p)
    var names <- bound_column_types(types, part)
    let found = guard_fact((p as ExprOp2).right, describe(part), names)
    return false if (is_none(found))
    let fact = unwrap(found)
    let mask = (guard_masks?[fact.key] ?? default<GuardCover>) | fact.mask
    guard_masks[fact.key] = mask
    return mask == (fact.mask.holds || fact.mask.fails ? GuardCover.holds | GuardCover.fails : GuardCover.less | GuardCover.equal | GuardCover.greater)
}

[macro_function]
def public match_coverage(var types : array<TypeDeclPtr>; var patterns : array<Expression?>) : MatchCoverage {
    //! Which arms or alternatives no value can reach, and whether every value reaches some arm; ``patterns`` is in source order, null for an arm that was dropped.
    var res : MatchCoverage
    var tab <- coverage_table(types, patterns)
    res.all_finite = !has_value([for (d in tab.domains); d.finite], false)
    var inscope prev : array<int>
    var inscope guard_masks : table<string; GuardCover>
    for (pat, ai in patterns, count()) {
        continue if (pat == null)
        let guarded = is_guarded(pat)
        res.has_guards ||= guarded
        var useful = false
        var inscope dead : array<string>
        for (ri in tab.arm_rows[ai]) {
            if (is_useful(tab, prev, tab.rows[ri], 0)) {
                useful = true
                dead |> push_from(dead_keys(tab, prev, tab.rows[ri]))
            } else {
                dead |> push(describe_row(tab.rows[ri]))
            }
        }
        if (useful) {
            res.dead_alternatives |> push_from([for (d in dead); (arm = ai, what = d)])
        } else {
            res.unreachable |> push(ai)
        }
        prev |> push_from(tab.arm_rows[ai]) if (!guarded || guards_complete(types, pat, guard_masks))
    }
    let shown_limit = 3
    let found <- missing_values(tab, prev, 0, shown_limit, default<array<string>>)
    let shown <- [for (f in found); "`{f}`"; where f != MATCH_ANY_OTHER]
    res.missing = (empty(found) ? none(type<string>) : some(found[0] == MATCH_ANY_OTHER ? MATCH_ANY_OTHER
        : join([for (f, i in shown, count()); f; where i < shown_limit], ", ") + (length(found) > shown_limit ? " and more" : "")))
    return <- res
}
