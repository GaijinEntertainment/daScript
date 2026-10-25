options gen2
options indenting = 4
options strict_smart_pointers = true

module match_solver shared private

//! A coverage solver: which arms no value can reach, which values no
//! arm matches, and which arms share a value. It knows nothing of the
//! language - the caller phrases its tests as rows of cells over
//! columns of named values; ``daslib/match`` and STYLE048 do.
//! ``solve_coverage`` computes everything; the other public functions
//! only read its result.
//!
//! Every arm adds a row per alternative, and each cell says what its
//! test covers of a column: every value, a set of named values,
//! integer spans, values the caller can not name, or the intersection
//! of other cells. A column of an enum, bool, variant or pointer has
//! a finite domain. A row is useful when some value reaches it that
//! no earlier row covers; an arm with no useful row is unreachable. A
//! guarded arm covers nothing, except arms whose guards ask the same
//! question and together cover every outcome.

require daslib/strings_boost
require daslib/option
require daslib/result
require math

bitfield private SolverGuardCover {
    //! The outcomes a guard covers: of a comparison between two values,
    //! or of a bool.
    less
    equal
    greater
    holds
    fails
}

struct private SolverTable {
    labels : array<string>
    rows : array<array<SolverCell>>
    arm_rows : array<array<int>>
    domains : array<array<string>>
}

def private span_cuts(spans : array<range64>) : array<int64> {
    //! Where integer spans split a column - at every span end - so a
    //! range and a constant name the same pieces; empty when every span
    //! is one value, and constants then name themselves.
    let ranged = has_value([for (s in spans); s.y - s.x != 1l], true)
    var cuts : array<int64>
    for (s in spans) {
        cuts |> push_from([s.x, s.y])
    }
    sort(cuts)
    return <- [for (v, i in cuts, count()); v; where ranged && (i == 0 || cuts[i - 1] != v)]
}

def private span_keys(span : range64; cuts : array<int64>) : array<string> {
    //! The pieces between ``cuts`` that ``span`` covers, named ``lo..hi``
    //! (``v`` for one value).
    return <- [for (i in range(length(cuts) - 1));
               cuts[i + 1] == cuts[i] + 1l ? "{cuts[i]}" : "{cuts[i]}..{cuts[i + 1]}";
        where cuts[i] >= span.x && cuts[i + 1] <= span.y]
}

def private column_domain(col : SolverColumn; c : int;
                          rows : array<array<SolverCell>>) : array<string> {
    //! A finite column's values; for any other column, the values the
    //! rows name plus "anything else".
    return <- clone_to_move(col.domain as some) if (col.domain is some)
    var values : array<string>
    for (r in rows) {
        values |> push_from([for (k in r[c].keys); k; where !has_value(values, k)])
    }
    values |> push(SOLVER_ANY_OTHER)
    return <- values
}

def private solver_table(columns : array<SolverColumn>; arms : array<SolverArm>) : SolverTable {
    var tab <- SolverTable(labels <- [for (c in columns); c.name])
    var total = 0
    for (arm in arms) {
        total += length(arm.rows)
    }
    tab.rows |> reserve(total)
    tab.arm_rows |> reserve(length(arms))
    for (arm in arms) {
        let first = length(tab.rows)
        tab.arm_rows |> emplace([for (i in range(first, first + length(arm.rows))); i])
        tab.rows |> push_clone_from(arm.rows)
    }
    for (c in range(length(columns))) {
        var inscope spans : array<range64>
        for (r in tab.rows) {
            spans |> push_from(r[c].spans)
        }
        continue if (empty(spans))
        var inscope cuts <- span_cuts(spans)
        for (r in tab.rows) {
            for (sp in r[c].spans) {
                r[c].keys |> push_from(empty(cuts) ? ["{sp.x}"] : span_keys(sp, cuts))
            }
            r[c].spans |> clear()
        }
    }
    tab.domains <- [for (col, ci in columns, count()); column_domain(col, ci, tab.rows)]
    return <- tab
}

def private cell_covers(cell : SolverCell; value : string) : bool =>
    cell.any || cell.keys |> has_value(value)

def private is_useful(tab : SolverTable; live : array<int>; cand : array<SolverCell>;
                      col : int) : bool {
    //! True when ``cand`` matches some value none of the ``live`` rows
    //! matches.
    return empty(live) if (col == length(cand))
    assume cell = cand[col]
    for (v in cell.any || cell.opaque ? tab.domains[col] : cell.keys) {
        var inscope sub <- [for (ri in live); ri; where cell_covers(tab.rows[ri][col], v)]
        return true if (is_useful(tab, sub, cand, col + 1))
    }
    return false
}

def private uncovered(tab : SolverTable; live : array<int>; col : int; limit : int;
                      path : array<string>) : array<array<string>> {
    //! Value combinations no ``live`` row matches, one value per column -
    //! one more than a message shows, so it can say there are more.
    var found : array<array<string>>
    if (empty(live)) {
        var full := path
        full |> push_from([for (_c in range(col, length(tab.domains))); SOLVER_ANY_OTHER])
        found |> emplace(full)
        return <- found
    }
    return <- found if (col == length(tab.domains))
    let all_any = !has_value([for (ri in live); tab.rows[ri][col].any], false)
    for (v in all_any ? [SOLVER_ANY_OTHER] : tab.domains[col]) {
        var inscope next := path
        next |> push(v)
        var inscope sub <- [for (ri in live); ri; where cell_covers(tab.rows[ri][col], v)]
        found |> push_clone_from(uncovered(tab, sub, col + 1, limit, next))
        break if (length(found) > limit)
    }
    return <- found
}

def private describe_row(row : array<SolverCell>) : string {
    let cells <- [for (c in row); c.any || c.opaque || empty(c.keys) ? "_" : join(c.keys, " | ")]
    return length(cells) == 1 ? cells[0] : "({join(cells, ", ")})"
}

def private dead_keys(tab : SolverTable; prev : array<int>;
                      row : array<SolverCell>) : array<string> {
    //! In a single-column row, the named alternatives an earlier arm
    //! already matches.
    var dead : array<string>
    return <- dead if (length(row) != 1 || row[0].any || row[0].opaque || length(row[0].keys) < 2)
    dead <- [for (k in row[0].keys); k; where !is_useful(tab, prev, [SolverCell(keys <- [k])], 0)]
    return <- dead
}

let private SOLVER_SHOWN_MISSING = 3

struct private Analysis {
    //! The walk's facts, before any is spelled for a message. Per row:
    //! none when no value reaches it, else the names an earlier arm
    //! takes (only a single-column row of several names has any).
    rows : array<Option<array<string>>>
    //! The value combinations no covering row takes.
    uncovered : array<array<string>>
}

struct private GuardAnswer {
    //! What the exact backend reads of a guard: one question - two terms
    //! compared, or one bool term - and the outcomes the guard takes.
    left : SolverTerm
    right : SolverTerm
    truth : bool
    cover : SolverGuardCover
}

struct private AskedQuestion {
    answer : GuardAnswer
    rows : array<array<SolverCell>>
}

let private SOLVER_ALL_ORDER = (SolverGuardCover.less | SolverGuardCover.equal |
    SolverGuardCover.greater)
let private SOLVER_ALL_TRUTH = SolverGuardCover.holds | SolverGuardCover.fails

def private compare_cover(op : SolverCompare) : SolverGuardCover {
    if (op == SolverCompare.less) return SolverGuardCover.less
    if (op == SolverCompare.less_equal) return SolverGuardCover.less | SolverGuardCover.equal
    if (op == SolverCompare.equal) return SolverGuardCover.equal
    if (op == SolverCompare.not_equal) return SolverGuardCover.less | SolverGuardCover.greater
    if (op == SolverCompare.greater_equal) return SolverGuardCover.greater | SolverGuardCover.equal
    return SolverGuardCover.greater
}

def private mirrored(cover : SolverGuardCover) : SolverGuardCover {
    var res = cover
    res.less = cover.greater
    res.greater = cover.less
    return res
}

def private guard_answer(fact : SolverFact) : Option<GuardAnswer> {
    //! The facts this backend decides: two terms compared, a value tested
    //! bare, or ``none_of`` one of those.
    assume n = fact.node
    if (n is compare) {
        let c = n as compare
        return some(GuardAnswer(left = c.left, right = c.right, cover = compare_cover(c.op)))
    } elif (n is truth) {
        return none(type<GuardAnswer>) if (!(n as truth is value))
        return some(
                    GuardAnswer(left = n as truth, right = SolverTerm(int_const = 0l), truth = true,
                                cover = SolverGuardCover.holds))
    } elif (n is none_of && length(n as none_of) == 1) {
        var inner = guard_answer((n as none_of)[0])
        return none(type<GuardAnswer>) if (inner is none)
        var a <- inner as some
        a.cover = (a.truth ? SOLVER_ALL_TRUTH : SOLVER_ALL_ORDER) & ~a.cover
        return some(a)
    }
    return none(type<GuardAnswer>)
}

def private same_term(a, b : SolverTerm) : bool {
    if (a is value) return b is value && a as value == b as value
    if (a is int_const) return b is int_const && a as int_const == b as int_const
    return b is string_const && a as string_const == b as string_const
}

def private same_cell(a, b : SolverCell) : bool {
    return false if (a.any != b.any || a.opaque != b.opaque || length(a.keys) != length(b.keys)
                     || length(a.spans) != length(b.spans) || length(a.all_of) != length(b.all_of))
    return (!has_value([for (x, y in a.keys, b.keys); x == y], false)
            && !has_value([for (x, y in a.spans, b.spans); x == y], false)
        && !has_value([for (x, y in a.all_of, b.all_of); same_cell(x, y)], false))
}

def private same_rows(a, b : array<array<SolverCell>>) : bool {
    return false if (length(a) != length(b))
    for (ra, rb in a, b) {
        return false if (length(ra) != length(rb)
                         || has_value([for (x, y in ra, rb); same_cell(x, y)], false))
    }
    return true
}

def private joined_cover(var asked : array<AskedQuestion>; var answer : GuardAnswer;
                         rows : array<array<SolverCell>>) : SolverGuardCover {
    //! The outcomes every arm asking this question over these rows takes
    //! so far; the same two terms in the other order are the same
    //! question, its outcomes mirrored.
    for (q in asked) {
        continue if (q.answer.truth != answer.truth || !same_rows(q.rows, rows))
        if (same_term(q.answer.left, answer.left) && same_term(q.answer.right, answer.right)) {
            q.answer.cover = q.answer.cover | answer.cover
            return q.answer.cover
        } elif (!answer.truth && same_term(q.answer.left, answer.right)
                && same_term(q.answer.right, answer.left)) {
            q.answer.cover = q.answer.cover | mirrored(answer.cover)
            return q.answer.cover
        }
    }
    let cover = answer.cover
    asked |> emplace(AskedQuestion(answer <- answer, rows := rows))
    return cover
}

def private analyze(tab : SolverTable; arms : array<SolverArm>;
                    var answers : array<Option<GuardAnswer>>) : Analysis {
    //! One walk over the arms in order: each row is useful when some
    //! value reaches it past the rows before it, and an arm's rows cover
    //! what they match once the arm is unguarded or its guards complete.
    var res : Analysis
    res.rows |> resize(length(tab.rows))
    var inscope prev : array<int>
    var inscope asked : array<AskedQuestion>
    for (arm, ai in arms, count()) {
        continue if (empty(arm.rows))
        for (ri in tab.arm_rows[ai]) {
            if (is_useful(tab, prev, tab.rows[ri], 0)) {
                var taken <- dead_keys(tab, prev, tab.rows[ri])
                res.rows[ri] <- move_some(taken)
            }
        }
        var covered = arm.guard is unguarded
        if (answers[ai] is some) {
            var answer <- answers[ai] as some
            let full = answer.truth ? SOLVER_ALL_TRUTH : SOLVER_ALL_ORDER
            covered = joined_cover(asked, answer, arm.rows) == full
        }
        prev |> push_from(tab.arm_rows[ai]) if (covered)
    }
    res.uncovered <- uncovered(tab, prev, 0, SOLVER_SHOWN_MISSING, default<array<string>>)
    return <- res
}

def private report(tab : SolverTable; columns : array<SolverColumn>; arms : array<SolverArm>;
                   facts : Analysis) : SolverCoverage {
    //! Every answer the walk's facts give, spelled: arms no value
    //! reaches, dead alternatives, and the missing values a message
    //! names.
    var res : SolverCoverage
    res.all_finite = !has_value([for (c in columns); c.domain is none], true)
    for (arm, ai in arms, count()) {
        continue if (empty(arm.rows))
        res.has_guards ||= !(arm.guard is unguarded)
        var inscope dead : array<string>
        var useful = false
        for (ri in tab.arm_rows[ai]) {
            if (facts.rows[ri] is some) {
                useful = true
                dead |> push_from(facts.rows[ri] as some)
            } else {
                dead |> push(describe_row(tab.rows[ri]))
            }
        }
        if (useful) {
            res.dead_alternatives |> push_from([for (d in dead); (arm = ai, what = d)])
        } else {
            res.unreachable |> push(ai)
        }
    }
    var inscope found <- [for (values in facts.uncovered); witness(values, tab.labels)]
    if (empty(found)) {
        res.missing = SolverMissing(covered = true)
    } elif (is_unnamed(found[0])) {
        res.missing = SolverMissing(unnamed = true)
    } else {
        var inscope shown <- [for (w in found); w; where !is_unnamed(w)]
        let more = length(found) > SOLVER_SHOWN_MISSING
        shown |> resize(min(length(shown), SOLVER_SHOWN_MISSING))
        let text = join([for (w in shown); "`{w.text}`"], ", ") + (more ? " and more" : "")
        res.missing = SolverMissing(named = (shown <- shown, more = more, text = text))
    }
    return <- res
}

let private SOLVER_ANY_OTHER = "_"

def private witness(values : array<string>; labels : array<string>) : SolverWitness {
    var named_values <- [for (v in values);
        v == SOLVER_ANY_OTHER ? SolverValue(other = true) : SolverValue(named = v)]
    var w <- SolverWitness(values <- named_values, labels := labels)
    w.text = witness_text(w)
    return <- w
}

def private is_unnamed(w : SolverWitness) : bool {
    //! A witness that names nothing: every value is "any other", and no
    //! field labels place them.
    let all_other = !has_value([for (v in w.values); v is named], true)
    return all_other && (length(w.values) == 1 || !has_value([for (l in w.labels); empty(l)], true))
}

struct public SolverCell {
    //! What a pattern covers of a column: every value, named ``keys``,
    //! half-open integer ``spans``, or unnamed values (``opaque``). Spans
    //! are cut to the pieces every row names, so they compare as values.
    any : bool
    opaque : bool
    keys : array<string>
    spans : array<range64>
    all_of : array<SolverCell>
}

struct public SolverColumn {
    //! One matched value; labels the column in a missing-value witness,
    //! "" for a top-level value.
    name : string
    //! Every value of a finite column (an enum, a bool, a variant); none
    //! for an open one, whose values are the rows' keys plus the rest.
    domain : Option<array<string>>
}

enum public SolverCompare {
    less
    less_equal
    equal
    not_equal
    greater_equal
    greater
}

variant public SolverTerm {
    //! One side of a fact: a value the caller numbers - the same number
    //! is the same value in every arm - or a constant.
    value : int
    int_const : int64
    string_const : string
}

variant public SolverFactNode {
    //! What a fact states. A ``compare`` asserts its two sides are
    //! totally ordered (no NaN); ``none_of`` holds when none of its facts
    //! does.
    compare : tuple<op : SolverCompare; left : SolverTerm; right : SolverTerm>
    truth : SolverTerm
    all_of : array<SolverFact>
    any_of : array<SolverFact>
    none_of : array<SolverFact>
}

struct public SolverFact {
    //! A condition a guard states; a struct, so a fact can hold facts.
    node : SolverFactNode
}

variant public SolverGuard {
    //! An arm's guard: none, a fact the caller describes, or opaque
    //! (covers nothing). Arms asking one question over the same rows
    //! join outcomes; once those are every outcome, they cover the rows.
    unguarded
    opaque
    fact : SolverFact
}

struct public SolverUnsupported {
    //! A guard fact the backend can not decide, and the arm holding it.
    arm : int
    fact : SolverFact
}

struct public SolverArm {
    //! One arm: a row of cells per alternative, one cell per column. An
    //! arm with no rows is skipped and keeps its index.
    rows : array<array<SolverCell>>
    guard : SolverGuard
}

variant public SolverValue {
    //! One column's value in a missing combination: a value the column
    //! names, or any value it does not.
    named : string
    other : bool
}

struct public SolverWitness {
    //! One combination no arm matches: a value per column, the columns'
    //! field labels ("" for a top-level value), and the combination as a
    //! message spells it.
    values : array<SolverValue>
    labels : array<string>
    text : string
}

variant public SolverMissing {
    //! The values no arm matches: none, some the columns can not name (an
    //! open column's "any other"), or the first few combinations and
    //! whether there are more.
    covered : bool
    unnamed : bool
    named : tuple<shown : array<SolverWitness>; more : bool; text : string>
}

struct public SolverCoverage {
    //! Arms (and alternatives of single-column arms) no value reaches,
    //! and the values no arm matches.
    unreachable : array<int>
    dead_alternatives : array<tuple<arm : int; what : string>>
    all_finite : bool
    has_guards : bool
    missing : SolverMissing
    overlaps : array<array<int>>
}

def private value_text(v : SolverValue) : string => v is named ? v as named : SOLVER_ANY_OTHER

def private witness_text(w : SolverWitness) : string {
    //! One missing combination: positional ``(a, b)``, or
    //! ``(field = value, ...)`` once a column was split into parts.
    if (!has_value([for (l in w.labels); empty(l)], false)) {
        return value_text(w.values[0]) if (length(w.values) == 1)
        return "({join([for (v in w.values); value_text(v)], ", ")})"
    }
    let items <- [for (v, l in w.values, w.labels);
                  empty(l) ? value_text(v) : "{l} = {value_text(v)}"; where v is named]
    return empty(items) ? SOLVER_ANY_OTHER : "({join(items, ", ")})"
}

def private cells_overlap(a, b : array<SolverCell>) : bool {
    //! Some value matches both rows: in every column a cell covers
    //! everything, can not name its values, or shares a key or an integer
    //! with the other.
    for (x, y in a, b) {
        continue if (x.any || y.any || x.opaque || y.opaque)
        let shared_key = has_value([for (k in x.keys); has_value(y.keys, k)], true)
        let shared_int = has_value([for (u in x.spans);
            has_value([for (v in y.spans); u.x < v.y && v.x < u.y], true)], true)
        return false if (!shared_key && !shared_int)
    }
    return true
}

def private cell_intersection(var l : SolverCell; var r : SolverCell) : SolverCell {
    //! ``p1 & p2`` covers what both sides cover.
    return <- r if (l.any)
    return <- l if (r.any)
    return <- SolverCell(opaque = true) if (l.opaque || r.opaque)
    var spans : array<range64>
    for (x in l.spans) {
        for (y in r.spans) {
            let lo = max(x.x, y.x)
            let hi = min(x.y, y.y)
            spans |> push(interval(lo, hi)) if (lo < hi)
        }
    }
    return <- SolverCell(keys <- [for (k in l.keys); k; where has_value(r.keys, k)], spans <- spans)
}


def private resolved(cell : SolverCell) : SolverCell {
    //! A cell with ``all_of`` covers what every one of those cells
    //! covers.
    return <- clone_to_move(cell) if (empty(cell.all_of))
    return <- intersect_from(cell.all_of, 0)
}

def private intersect_from(cells : array<SolverCell>; i : int) : SolverCell {
    return <- SolverCell(any = true) if (i == length(cells))
    return <- cell_intersection(resolved(cells[i]), intersect_from(cells, i + 1))
}

def private arm_overlaps(arms : array<SolverArm>) : array<array<int>> {
    //! Per arm, the earlier arms some value of it also matches.
    return <- [for (arm, ai in arms, count()); [for (aj in range(ai)); aj;
        where has_value([for (ra in arm.rows);
            has_value([for (rb in arms[aj].rows); cells_overlap(ra, rb)], true)], true)]]
}

def public solve_coverage(columns : array<SolverColumn>;
                          arms : array<SolverArm>) : Result<SolverCoverage; SolverUnsupported> {
    //! Which arms or alternatives no value can reach, which values no arm
    //! matches, and which arms share a value; ``arms`` in source order.
    //! An error names the first guard fact this backend can not decide.
    var inscope answers : array<Option<GuardAnswer>>
    for (arm, ai in arms, count()) {
        if (arm.guard is fact) {
            var answer <- guard_answer(arm.guard as fact)
            if (answer is none) {
                return <- move_err(SolverUnsupported(arm = ai, fact := arm.guard as fact),
                                   type<SolverCoverage>)
            }
            answers |> emplace(answer)
        } else {
            answers |> emplace(none(type<GuardAnswer>))
        }
    }
    var inscope plain <- [for (arm in arms);
                          SolverArm(rows <- [for (row in arm.rows); [for (c in row); resolved(c)]],
                                    guard := arm.guard)]
    let tab <- solver_table(columns, plain)
    var res <- report(tab, columns, plain, analyze(tab, plain, answers))
    res.overlaps <- arm_overlaps(plain)
    return <- move_ok(res, type<SolverUnsupported>)
}

def public all_reachable(cov : SolverCoverage) : bool =>
    empty(cov.unreachable) && empty(cov.dead_alternatives)

def public is_reachable(cov : SolverCoverage; arm : int) : bool => !has_value(cov.unreachable, arm)

def public is_exhaustive(cov : SolverCoverage) : bool => cov.missing is covered

def public arms_overlap(cov : SolverCoverage; a, b : int) : bool =>
    a != b && has_value(cov.overlaps[max(a, b)], min(a, b))

def public missing_fully_listed(cov : SolverCoverage) : bool =>
    cov.missing is named && !(cov.missing as named).more
