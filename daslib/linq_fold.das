options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
// Intentionally opt in for user-facing docs: keep comments concise and actionable.
options _comment_hygiene = true

module linq_fold shared public

//! LINQ ``_fold`` macro — fuse-time-aware pipeline rewriting.
//!
//! ``_fold(expr)`` is a three-tier cascade:
//!
//! 1. **Splice** — when the chain matches a recognized pattern, emit a single
//!    fused for-loop with predicates and projections inlined (best perf,
//!    no intermediate iterators or buffers).
//! 2. **``fold_linq_default``** — when splice can't fire, emit an
//!    array-shape pipeline (``call → array → call → array``) with
//!    ``_inplace`` variants reusing the same buffer and explicit ``delete``
//!    on the previous stage. One buffer at a time, no iterator overhead.
//! 3. **``clone_expression``** — raw passthrough when the chain has no
//!    recognized linq operators (``flatten_linq`` returns empty).
//!
//! All three tiers preserve semantics — ``_fold(chain)`` is observationally
//! equivalent to ``chain``, just faster when patterns match. The macro is
//! always safe to apply.
//!
//! Requires the ``linq`` module for the backing operator functions.

require daslib/linq public

require daslib/ast_boost
require daslib/ast_match
require daslib/templates_boost
require daslib/macro_boost
require strings

// LinqCall — registry record describing an operator name (where_/select/distinct/...) and its fold disposition.
struct private LinqCall {
    name : string
    moduleName : string = "linq"
    skip : bool = false             // if specified, expression is skipped in folding
    inplace : bool = false          // if specified, expression is inplace and does not create new variable
    noToArrayVariant : bool = false // if specified, no `_to_array` variant exists (function already returns array regardless of input shape)
    recursive : array<int>          // indices of arguments to apply fold_linq_default on
}

// ===== Pattern-table refactor kernel =====
// See daslib/linq_fold.md for masterplan.

// Hashed equi-join payload — feeds the per-pair `let resBindName = result(tupA,tupB)` bind used by both GroupBy's decs-join arm and `emit_decs_join`. The 2 consumers differ on whether the per-pair loop is the right granularity (GroupBy: yes; emit_decs_join keeps a bucket-length fast path for count-no-where).
struct private DecsJoinShape {
    bridgeA, bridgeB : DecsBridgeShape?
    keyaLam, keybLam, resultLam : Expression?
    keyType, tupBType, resultType : TypeDeclPtr
    resBindName : string
}

// 2-source lockstep zip payload — feeds `for (itA, itB in srcA, srcB) { body }` and the 2-source outer invoke.
struct private ZipShape {
    srcAExpr, srcBExpr : Expression?
    srcAName, srcBName : string
    srcAType, srcBType : TypeDeclPtr
}

variant private SourceAdapter {
    Array    : tuple<Expression?; string>               // (top, srcName) — PR A
    Decs     : tuple<DecsBridgeShape?; string>          // (bridge, tupName) — PR C
    DecsJoin : DecsJoinShape                            // PR D2
    Zip      : ZipShape                                 // PR D2
}

variant SlotMatcher {
    literal : string                       // exact name match
    one_of  : array<string>                // any-of name set
    alias   : string                       // looked up in alias_table
}

variant SlotCardinality {
    one      : void?                       // required, exactly 1
    optional : void?                       // 0 or 1
    chain    : void?                       // 0 or more (greedy); captures as array<ExprCall?> via Captures.many
}

struct Slot {
    matcher      : SlotMatcher
    cardinality  : SlotCardinality
    capture_name : string = ""             // "" = don't capture
    arity        : int = -1                // -1 = any; positive = require N args on the matched call
}

// Captures: `single` for c_one/c_opt (ExprCall), `single_name` parallel post-normalize LinqCall.name (load-bearing for plan_order_family normalize_order_reverse swap), `many` for c_chain.
struct Captures {
    single      : table<string; ExprCall?>
    single_name : table<string; string>
    many        : table<string; array<ExprCall?>>
}

variant private MatchResult {
    no_match : void?                       // daslang-idiomatic Option<Captures>
    matched  : Captures
}

typedef private RequiresPredicate = function<(var c : Captures; var top : Expression?) : bool>

// Fold-time context passed to every emit archetype. Carries the peeled source expression, source adapter, and the outer
// `_fold(...)` expression's iterator-ness (drives `buffer_return` wrap so iterator-typed contexts wrap survivors with
// `.to_sequence_move()`; array-typed contexts return the buffer directly).
struct EmitCtx {
    top              : Expression?         // peel_each'd; stubs pre-clone per invoke
    src              : SourceAdapter
    expr_is_iterator : bool
}

typedef EmitFn = function<(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression?>

struct SplicePattern {
    name     : string                      // debug / lint diagnostics
    chain    : array<Slot>
    requires : array<RequiresPredicate>
    emit     : EmitFn
}

// Slot construction helpers (variant arms need named-field syntax; helpers keep pattern rows compact)
def m_literal(s : string) : SlotMatcher {
    return SlotMatcher(literal = s)
}
def m_alias(s : string) : SlotMatcher {
    return SlotMatcher(alias = s)
}
def c_one() : SlotCardinality {
    return SlotCardinality(one = null)
}
def c_opt() : SlotCardinality {
    return SlotCardinality(optional = null)
}
def c_chain() : SlotCardinality {
    return SlotCardinality(chain = null)
}

// Convenience: c_chain + m_one_of always pair (head shape "0+ contiguous calls whose name is in `names`, captured as `cap`"). `names` is consumed (moved into the slot matcher).
def slot_chain_of(var names : array<string>; cap : string) : Slot {
    return Slot(matcher = SlotMatcher(one_of <- names), cardinality = c_chain(), capture_name = cap)
}

// Prefix-conflict lint: pattern A shadows pattern B (B unreachable) when A's chain is a structural prefix of B's chain.
// PR A scope: simple structural check (matcher + cardinality + arity equality, capture_name ignored). Doesn't catch all subsumption cases (e.g. opt slots interacting with required slots) — exhaustive subsumption deferred to a later PR.
def private slot_matchers_equal(a : SlotMatcher; b : SlotMatcher) : bool {
    if (a is literal && b is literal) return (a as literal) == (b as literal)
    if (a is alias && b is alias) return (a as alias) == (b as alias)
    if (a is one_of && b is one_of) {
        // `one_of` is set-membership — equality is order-insensitive (same length + every element in a is in b).
        let la & = unsafe(a as one_of)
        let lb & = unsafe(b as one_of)
        if (length(la) != length(lb)) return false
        for (name in la) {
            var found = false
            for (other in lb) {
                if (other == name) { found = true; break; }
            }
            if (!found) return false
        }
        return true
    }
    return false
}

def private slots_structurally_match(a : Slot; b : Slot) : bool {
    return (slot_matchers_equal(a.matcher, b.matcher)
        && (a.cardinality is one)      == (b.cardinality is one)
        && (a.cardinality is optional) == (b.cardinality is optional)
        && (a.cardinality is chain)    == (b.cardinality is chain)
        && a.arity == b.arity)
}

// Strict prefix: a must be SHORTER than b. Two patterns with identical chains but different `requires`
// predicates are both legitimately reachable (the walker falls through to the second when the first's
// requires fail), so equal-length is not a shadowing relation.
def chain_prefix_of(a : array<Slot>; b : array<Slot>) : bool {
    if (length(a) >= length(b)) return false
    for (i in 0 .. length(a)) {
        if (!slots_structurally_match(a[i], b[i])) return false
    }
    return true
}

// Walk a pattern table; return true iff every pattern is reachable (no earlier pattern is its strict prefix).
// Silent — return value carries the verdict. Tests assert on the bool; future compile-time hook can add diagnostics.
def check_pattern_table_reachable(name : string; patterns : array<SplicePattern>) : bool {
    for (i in 0 .. length(patterns)) {
        for (j in i + 1 .. length(patterns)) {
            if (chain_prefix_of(patterns[i].chain, patterns[j].chain)) return false
        }
    }
    return true
}

// Normalize an ExprCall's name through linqCalls (e.g. "distinct_by_to_array" → "distinct_by").
// Walks func.fromGeneric chain — ExprCall.name itself is the mangled generic-instance name (`__::linq\`distinct_by\`<hash>`); the original user-facing name lives at the root of the fromGeneric chain.
def private call_norm_name(call : ExprCall?) : string {
    var topFunc = call.func
    while (topFunc.fromGeneric != null) {
        topFunc = topFunc.fromGeneric
    }
    let raw = string(topFunc.name)
    if (linqCalls |> key_exists(raw)) return linqCalls[raw].name
    return raw
}

var alias_table : table<string; array<string>> <- {
    "distinct_family"            => ["distinct", "distinct_by"],
    "first_family"               => ["first", "first_or_default"],
    "count_family"               => ["count", "long_count"],
    // Narrow to the terminators emit_hashtable_dedup actually handles — first[_or_default] would be over-matched then cascade.
    "distinct_terminator_family" => ["count", "long_count", "sum"],
    // PR B additions:
    "order_family"               => ["order", "order_descending", "order_by", "order_by_descending"],
    "range_op_family"            => ["skip", "skip_while", "take_while", "take"],
    // accum_family: all reducing terminators routed via emit_accumulator_lane in plan_loop_or_count.
    "accum_family"               => ["sum", "min", "max", "average", "aggregate",
                                     "min_by", "max_by", "min_max", "min_max_by",
                                     "min_max_average", "min_max_average_by", "long_count"],
    "early_exit_family"          => ["any", "all", "contains", "first", "first_or_default"],
    // loop_terminator_family: union used by plan_loop_or_count's single pattern row's optional terminator slot.
    "loop_terminator_family"     => ["count", "long_count", "sum", "min", "max", "average", "aggregate",
                                     "min_by", "max_by", "min_max", "min_max_by",
                                     "min_max_average", "min_max_average_by",
                                     "any", "all", "contains", "first", "first_or_default",
                                     "last", "last_or_default", "single", "single_or_default",
                                     "element_at", "element_at_or_default"],
    // PR D1 — order-by-with-key (excludes bare `order` / `order_descending` which lack a key arg).
    // Used by plan_group_by's trailing_order slot.
    "order_by_family"            => ["order_by", "order_by_descending"]
}

// Walker — see daslib/linq_fold.md Step 3 contract. Returns MatchResult by move;
// caller binds with `var r <- match_pattern(...)` and reads via `if (r is matched) { let c & = r as matched; ... }`.

def private slot_matches_call(slot : Slot; call : ExprCall?; name : string; pattern_name : string) : bool {
    var in_set = false
    if (slot.matcher is literal) {
        in_set = (slot.matcher as literal) == name
    } elif (slot.matcher is one_of) {
        for (n in slot.matcher as one_of) {
            if (n == name) { in_set = true; break; }
        }
    } elif (slot.matcher is alias) {
        let key = slot.matcher as alias
        if (alias_table |> key_exists(key)) {
            for (n in alias_table[key]) {
                if (n == name) { in_set = true; break; }
            }
        } else {
            panic("match_pattern: unknown alias '{key}' in pattern '{pattern_name}'")
        }
    }
    return in_set && (slot.arity == -1 || length(call.arguments) == slot.arity)
}

def private match_pattern(p : SplicePattern;
                          var calls : array<tuple<ExprCall?; LinqCall?>>;
                          var top : Expression?) : MatchResult {
    var captures : Captures
    var slot_i = 0
    var call_i = 0
    while (slot_i < length(p.chain)) {
        let slot & = unsafe(p.chain[slot_i])
        if (slot.cardinality is chain) {
            // c_chain: greedy match-while-in-set. Always succeeds (0+); empty match still creates a Captures.many entry so emit fns can rely on the key existing.
            var captured : array<ExprCall?>
            while (call_i < length(calls) && slot_matches_call(slot, calls[call_i]._0, calls[call_i]._1.name, p.name)) {
                captured |> push(calls[call_i]._0)
                call_i ++
            }
            if (slot.capture_name != "") {
                captures.many[slot.capture_name] <- captured
            }
            slot_i ++
            continue
        }
        var matched_here = false
        if (call_i < length(calls)) {
            let cur & = unsafe(calls[call_i])
            matched_here = slot_matches_call(slot, cur._0, cur._1.name, p.name)
        }
        if (matched_here) {
            if (slot.capture_name != "") {
                captures.single |> insert(slot.capture_name, calls[call_i]._0)
                captures.single_name |> insert(slot.capture_name, calls[call_i]._1.name)
            }
            slot_i ++
            call_i ++
        } elif (slot.cardinality is one) {
            return MatchResult(no_match = null)
        } else {
            slot_i ++
        }
    }
    if (call_i < length(calls)) return MatchResult(no_match = null)
    for (pred in p.requires) {
        if (!invoke(pred, captures, top)) return MatchResult(no_match = null)
    }
    return MatchResult(matched <- captures)
}

// Predicate library — named module-level functions (named so JIT can take their address; inline `@@(...)` lambdas
// produce `_localfunction_*` symbols the JIT pass can't resolve). Pattern rows wrap each with `@@<RequiresPredicate>`.
// Starts minimal; grows with use per masterplan.

def private array_source(var c : Captures; var top : Expression?) : bool {
    // top is already peel_each'd by the calling stub; verify it carries an array type. Naturally false for decs sources (`from_decs_template(...)` is iterator-typed).
    return top != null && top._type != null && (top._type.isGoodArrayType || top._type.isArray)
}

def private decs_source(var c : Captures; var top : Expression?) : bool {
    // For rows that should only match decs adapter (PR C). extract_decs_bridge is cheap shape check.
    return extract_decs_bridge(top) != null
}

def private take_arg_is_int(var c : Captures; var top : Expression?) : bool {
    if (!(c.single |> key_exists("take"))) return true   // vacuous: no take to constrain
    let take = c.single["take"]
    return (take != null && length(take.arguments) >= 2 && take.arguments[1] != null
            && take.arguments[1]._type != null && take.arguments[1]._type.baseType == Type.tInt)
}

// `no_terminator` — chain ends bare (no `count` / `sum` / `first[_or_default]` / `to_array` captured).
// Return shape (array vs iterator) is decided by `ctx.expr_is_iterator` in the emit fn, not this predicate.
def private no_terminator(var c : Captures; var top : Expression?) : bool {
    return !(c.single |> key_exists("term"))
}

// True only for `order_by[_descending]` with an inline-splice-able key lambda. `order` / `order_descending` route elsewhere.
def private inline_cmp_available(var c : Captures; var top : Expression?) : bool {
    if (!(c.single |> key_exists("order"))) return false
    let orderCall = c.single["order"]
    if (orderCall == null || orderCall._type == null || orderCall._type.firstType == null
            || (orderCall.arguments |> length) < 2) return false
    let orderName = c.single_name["order"]    // post-normalize LinqCall.name; call_norm_name would lose the swap
    if (orderName != "order_by" && orderName != "order_by_descending") return false
    return try_make_inline_cmp(orderCall.arguments[1], orderName, orderCall._type.firstType, orderCall.at) != null
}

// `has_where_or_distinct` — used by order_fused_prefilter row to distinguish from bare buffer_helper_dispatch.
// At least one prefilter source must be present for the fused-loop path to make sense.
def private has_where_or_distinct(var c : Captures; var top : Expression?) : bool {
    return (c.single |> key_exists("where")) || (c.single |> key_exists("distinct"))
}

// v1 limit (imperative line 6011): upstream `join` requires empty head + no having + no trailing_where.
// Vacuous when no upstream_join captured.
def private decs_join_invariants(var c : Captures; var top : Expression?) : bool {
    if (!(c.single |> key_exists("upstream_join"))) return true
    return ((c.many["head"] |> empty)
            && !(c.single |> key_exists("having"))
            && !(c.single |> key_exists("trailing_where")))
}

// plan_group_by_core's count lane ignores orderName/orderKey — splicing this shape would silently drop
// the order_by key lambda (and its side effects). Cascade to tier-2 instead. Matches the pre-PR-D1
// imperative recognizer which only popped trailing order when no count terminator was present.
def private no_order_with_count(var c : Captures; var top : Expression?) : bool {
    return !((c.single |> key_exists("trailing_order")) && (c.single |> key_exists("term")))
}

// Matches pre-PR-D2 plan_decs_join: trailing_select is only popped when terminator is NOT count
// (count's emission ignores select entirely). Splicing `join+select+count` would silently drop
// select's side effects — cascade to tier-2 instead.
def private decs_join_no_select_with_count(var c : Captures; var top : Expression?) : bool {
    return !((c.single |> key_exists("trailing_select")) && (c.single |> key_exists("term")))
}

// Zip arity guard: 2-source (lockstep) or 3-source (result-selector form). Matches pre-PR-D2 plan_zip.
def private zip_arity_2_or_3(var c : Captures; var top : Expression?) : bool {
    if (!(c.single |> key_exists("zip"))) return true
    let n = c.single["zip"].arguments |> length
    return n == 2 || n == 3
}

// skip_while/take_while peel against the zip tuple `it`; an upstream select rebinds the element type
// so the while predicate would dangle. Matches pre-PR-D2 plan_zip chain-walk bail (lines 6494/6500).
def private zip_no_while_after_select(var c : Captures; var top : Expression?) : bool {
    if (!(c.single |> key_exists("skip_while")) && !(c.single |> key_exists("take_while"))) return true
    for (call in c.many["head"]) {
        if (call_norm_name(call) == "select") return false
    }
    return true
}

// Trailing reverse is identity for count/sum/min/max/average/any/all/contains but picks a different
// element for first/first_or_default. Matches pre-PR-D2 plan_zip post-walk bail (line 6644).
def private zip_reverse_compatible_with_term(var c : Captures; var top : Expression?) : bool {
    if (!(c.single |> key_exists("trailing_reverse")) || !(c.single |> key_exists("term"))) return true
    let name = c.single_name["term"]
    return name != "first" && name != "first_or_default"
}

// Master pattern table. Each register_*_rows below is `[_macro]` so it only runs while linq_fold itself is being macro-compiled; the per-family rows are appended in file declaration order on a fresh splice_patterns. First-match-wins per adapter pass (see LinqFold.visit cascade).

var private splice_patterns : array<SplicePattern>

// ===== End of pattern-table kernel =====

var private linqCalls = {
// filtering data
    "where_" => LinqCall(name = "where_"),
    "where_to_array" => LinqCall(name = "where_"),
// transform operations
    "select" => LinqCall(name = "select"),
    "select_to_array" => LinqCall(name = "select"),
    "select_many" => LinqCall(name = "select_many"),
    "select_many_to_array" => LinqCall(name = "select_many"),
    "zip" => LinqCall(name = "zip", recursive = [1, 2]),
    "zip_to_array" => LinqCall(name = "zip", recursive = [1, 2]),
// sorting data
    "reverse" => LinqCall(name = "reverse", inplace = true),
    "reverse_to_array" => LinqCall(name = "reverse", inplace = true),
    "order" => LinqCall(name = "order", inplace = true),
    "order_to_array" => LinqCall(name = "order", inplace = true),
    "order_descending" => LinqCall(name = "order_descending", inplace = true),
    "order_descending_to_array" => LinqCall(name = "order_descending", inplace = true),
    "order_by" => LinqCall(name = "order_by", inplace = true),
    "order_by_to_array" => LinqCall(name = "order_by", inplace = true),
    "order_by_descending" => LinqCall(name = "order_by_descending", inplace = true),
    "order_by_descending_to_array" => LinqCall(name = "order_by_descending", inplace = true),
// top-N terminal forms — always return array regardless of input shape, so no
    "top_n" => LinqCall(name = "top_n", noToArrayVariant = true),
    "top_n_by" => LinqCall(name = "top_n_by", noToArrayVariant = true),
    "top_n_descending" => LinqCall(name = "top_n_descending", noToArrayVariant = true),
    "top_n_by_descending" => LinqCall(name = "top_n_by_descending", noToArrayVariant = true),
    "top_n_by_with_cmp" => LinqCall(name = "top_n_by_with_cmp", noToArrayVariant = true),
// aggregate
    "count" => LinqCall(name = "count"),
    "long_count" => LinqCall(name = "long_count"),
    "average" => LinqCall(name = "average"),
    "min" => LinqCall(name = "min"),
    "min_by" => LinqCall(name = "min_by"),
    "max" => LinqCall(name = "max"),
    "max_by" => LinqCall(name = "max_by"),
    "min_max" => LinqCall(name = "min_max"),
    "min_max_by" => LinqCall(name = "min_max_by"),
    "min_max_average" => LinqCall(name = "min_max_average"),
    "min_max_average_by" => LinqCall(name = "min_max_average_by"),
    "aggregate" => LinqCall(name = "aggregate"),
    "sum" => LinqCall(name = "sum"),
// ranges
    "skip" => LinqCall(name = "skip", inplace = true),
    "skip_to_array" => LinqCall(name = "skip", inplace = true),
    "skip_while" => LinqCall(name = "skip_while"),
    "skip_while_to_array" => LinqCall(name = "skip_while"),
    "take" => LinqCall(name = "take", inplace = true),
    "take_to_array" => LinqCall(name = "take", inplace = true),
    "take_while" => LinqCall(name = "take_while"),
    "take_while_to_array" => LinqCall(name = "take_while"),
    "skip_last" => LinqCall(name = "skip_last", inplace = true),
    "skip_last_to_array" => LinqCall(name = "skip_last", inplace = true),
    "take_last" => LinqCall(name = "take_last", inplace = true),
    "take_last_to_array" => LinqCall(name = "take_last", inplace = true),
    "chunk" => LinqCall(name = "chunk"),
    "chunk_to_array" => LinqCall(name = "chunk"),
// concatenation operations
    "concat" => LinqCall(name = "concat", recursive = [1], inplace = true),
    "concat_to_array" => LinqCall(name = "concat", recursive = [1], inplace = true),
    "prepend" => LinqCall(name = "prepend", inplace = true),
    "prepend_to_array" => LinqCall(name = "prepend", inplace = true),
    "append" => LinqCall(name = "append", inplace = true),
    "append_to_array" => LinqCall(name = "append", inplace = true),
// set operations
    "distinct" => LinqCall(name = "distinct", inplace = true),
    "distinct_to_array" => LinqCall(name = "distinct", inplace = true),
    "distinct_by" => LinqCall(name = "distinct_by", inplace = true),
    "distinct_by_to_array" => LinqCall(name = "distinct_by", inplace = true),
    "union" => LinqCall(name = "union", recursive = [1]),
    "union_to_array" => LinqCall(name = "union", recursive = [1]),
    "union_by" => LinqCall(name = "union_by", recursive = [1]),
    "union_by_to_array" => LinqCall(name = "union_by", recursive = [1]),
    "except" => LinqCall(name = "except", recursive = [1]),
    "except_to_array" => LinqCall(name = "except", recursive = [1]),
    "except_by" => LinqCall(name = "except_by", recursive = [1]),
    "except_by_to_array" => LinqCall(name = "except_by", recursive = [1]),
    "intersect" => LinqCall(name = "intersect", recursive = [1]),
    "intersect_to_array" => LinqCall(name = "intersect", recursive = [1]),
    "intersect_by" => LinqCall(name = "intersect_by", recursive = [1]),
    "intersect_by_to_array" => LinqCall(name = "intersect_by", recursive = [1]),
    "unique" => LinqCall(name = "unique", inplace = true),
    "unique_to_array" => LinqCall(name = "unique", inplace = true),
    "unique_by" => LinqCall(name = "unique_by", inplace = true),
    "unique_by_to_array" => LinqCall(name = "unique_by", inplace = true),
// element operations
    "element_at" => LinqCall(name = "element_at"),
    "element_at_or_default" => LinqCall(name = "element_at_or_default"),
    "first" => LinqCall(name = "first"),
    "first_or_default" => LinqCall(name = "first_or_default"),
    "last" => LinqCall(name = "last"),
    "last_or_default" => LinqCall(name = "last_or_default"),
    "single" => LinqCall(name = "single"),
    "single_or_default" => LinqCall(name = "single_or_default"),
// join operations
    "join" => LinqCall(name = "join", recursive = [1]),
    "join_to_array" => LinqCall(name = "join", recursive = [1]),
    "left_join" => LinqCall(name = "left_join", recursive = [1]),
    "left_join_to_array" => LinqCall(name = "left_join", recursive = [1]),
    "group_join" => LinqCall(name = "group_join", recursive = [1]),
    "group_join_to_array" => LinqCall(name = "group_join", recursive = [1]),
    "group_by" => LinqCall(name = "group_by"),
    "group_by_to_array" => LinqCall(name = "group_by"),
    "group_by_lazy" => LinqCall(name = "group_by_lazy"),
    "group_by_lazy_to_array" => LinqCall(name = "group_by_lazy"),
    "having_" => LinqCall(name = "having_"),
    "having_to_array" => LinqCall(name = "having_"),
// querying data
    "any" => LinqCall(name = "any"),
    "all" => LinqCall(name = "all"),
    "contains" => LinqCall(name = "contains"),
// skippable
    "to_table" => LinqCall(name = "to_table"),
    "to_array" => LinqCall(name = "to_array", moduleName = "builtin", skip = true),
    "to_sequence" => LinqCall(name = "to_sequence", skip = true),
}

[macro_function]
def private is_linq_call(var expr : Expression?) : bool {
    if (expr is ExprCall) {
        var call = expr as ExprCall
        var funcName = string(call.func.name)
        while (call.func.fromGeneric != null) {
            call.func = call.func.fromGeneric
            funcName = string(call.func.name)
        }
        if (linqCalls.key_exists(funcName)) {
            let linqCall & = unsafe(linqCalls?[funcName] ?? default<LinqCall>)
            if (linqCall.moduleName == call.func._module.name) return true
        }
    }
    return false
}

// Theme 5: `_order_by(k).reverse()` → `_order_by_descending(k)` (and symmetric: order/order_descending, order_by/order_by_descending). Mutates `calls` in place by swapping the LinqCall metadata to the flipped registry entry and erasing the trailing reverse. The ExprCall arg list is identical between ascending/descending order variants, so no AST rewrite is needed. Runs once per planner that walks an order/reverse chain; idempotent (a fully-normalized chain produces no work). Closes audit 1b (order + reverse + take) and 2b (reverse over order) — after normalization, the existing plan_*order_family arms fire naturally.
[macro_function]
def private normalize_order_reverse(var calls : array<tuple<ExprCall?; LinqCall?>>) {
    var i = 0
    while (i + 1 < length(calls)) {
        let curName = calls[i]._1.name
        let nextName = calls[i + 1]._1.name
        var flipped = ""
        if (nextName == "reverse") {
            if (curName == "order_by") {
                flipped = "order_by_descending"
            } elif (curName == "order_by_descending") {
                flipped = "order_by"
            } elif (curName == "order") {
                flipped = "order_descending"
            } elif (curName == "order_descending") {
                flipped = "order"
            }
        }
        if (flipped != "" && linqCalls.key_exists(flipped)) {
            var linqCall & = unsafe(linqCalls?[flipped] ?? default<LinqCall>)
            calls[i] = (calls[i]._0, unsafe(addr(linqCall)))
            calls |> erase(i + 1)
            // Don't advance `i` — the newly flipped order call might itself be followed by another reverse (`order_by(k).reverse().reverse()` → `order_by_descending(k).reverse()` → `order_by(k)`).
        } else {
            i ++
        }
    }
}

// Theme 7: collapse consecutive `_select(f) |> _select(g)` calls into a single `_select(g(f(_)))`. Symmetric to how N consecutive `_where` compose via `&&`. Mutates `calls` in place by replacing the OUTER select's lambda with a composed lambda (built by cloning the INNER lambda structure so param TYPE is preserved, then renaming its param to a fresh name and overwriting the body with the composed expression) and erasing the inner entry. Gated on purity of the inner projection — for impure inner, collapsing would shift evaluation count (outer may reference its param zero or many times); leaving the chain unchanged preserves cascade semantics. Closes audit 7c (chained selects on plan_zip) and unblocks chained selects on plan_distinct / plan_order_family / plan_reverse / plan_decs_* / plan_decs_join.
[macro_function]
def private collapse_chained_selects(var calls : array<tuple<ExprCall?; LinqCall?>>) {
    // Match on LinqCall.name == "select" — this name is shared between `select` and `select_to_array` in the `linqCalls` registry (both denote the same projection semantic, just different input shapes: array vs iterator). The chain backlink rewire below may change the outer call's input shape (e.g. outer was `select` reading from inner's array<T>; after collapse it reads directly from an iter<T> source). That would normally require rebinding outer.func to `select_to_array`, BUT every downstream planner that consumes this `calls` array builds its emission from scratch using cll._1.name + cll._0.arguments[1] (the lambda) — none of them re-evaluate cll._0.func or instantiate a call to the original `select`/`select_to_array` function. The fold_linq_default fallback derives the actual call name (`select_to_array` vs `select`) at emission time from `top._type.isIterator`, not from cll._0.func. So the variant mismatch on the modified ExprCall is latent and never observed.
    var i = 0
    while (i + 1 < length(calls)) {
        if (calls[i]._1.name != "select" || calls[i + 1]._1.name != "select") {
            i ++
            continue
        }
        var innerLam = calls[i]._0.arguments[1]
        var outerLam = calls[i + 1]._0.arguments[1]
        if (innerLam == null || outerLam == null
                || !(innerLam is ExprMakeBlock) || !(outerLam is ExprMakeBlock)) {
            i ++
            continue
        }
        var innerMblk = innerLam as ExprMakeBlock
        var innerBlk = innerMblk._block as ExprBlock
        var outerMblk = outerLam as ExprMakeBlock
        var outerBlk = outerMblk._block as ExprBlock
        if (innerBlk == null || outerBlk == null
                || innerBlk.arguments |> length != 1 || outerBlk.arguments |> length != 1
                || innerBlk.list |> length != 1 || outerBlk.list |> length != 1
                || !(innerBlk.list[0] is ExprReturn) || !(outerBlk.list[0] is ExprReturn)) {
            i ++
            continue
        }
        var innerRet = innerBlk.list[0] as ExprReturn
        var outerRet = outerBlk.list[0] as ExprReturn
        if (innerRet.subexpr == null || outerRet.subexpr == null) {
            i ++
            continue
        }
        if (has_sideeffects(innerRet.subexpr)) {
            i ++
            continue
        }
        // Rename inner's param to a fresh name BEFORE composition. apply_template recursively walks substituted content — if outer's param name matches a binder name inside innerBody (both selects often share `_` from boost-side desugaring), substitution loops. Renaming inner's `_` to a unique name guarantees no collision when substituting outer's `_` with renamedInnerBody.
        let freshName = qn("cs", innerLam.at)
        var renamedInnerBody = peel_lambda_rename_var(innerLam, freshName)
        if (renamedInnerBody == null) {
            i ++
            continue
        }
        // Build new lambda: clone innerLam (preserves param TYPE), rename its bound param to freshName, then overwrite its body with composedBody (outer's body with outer's param substituted by renamedInnerBody).
        var newLam = clone_expression(innerLam)
        var newMblk = newLam as ExprMakeBlock
        var newBlk = newMblk._block as ExprBlock
        if (newBlk == null || newBlk.list |> length != 1 || !(newBlk.list[0] is ExprReturn)) {
            i ++
            continue
        }
        var newRet = newBlk.list[0] as ExprReturn
        newBlk.arguments[0].name := freshName
        var rules : Template
        rules |> replaceVariable(string(outerBlk.arguments[0].name), renamedInnerBody)
        newRet.subexpr = apply_template(rules, outerRet.subexpr.at, outerRet.subexpr)
        calls[i + 1]._0.arguments[1] = newLam
        // Rewire the chain backlink so the inner select is dropped from the AST too. Without this, a subsequent planner pass would re-flatten the original outer→inner→source chain and re-collapse — non-idempotent (substitution of outer's already-renamed param into inner's body would compound `._0` accessors).
        calls[i + 1]._0.arguments[0] = calls[i]._0.arguments[0]
        calls |> erase(i)
        // Don't advance `i` — the merged outer might still chain with another `_select` just before it.
    }
}

[macro_function]
def private collapse_chained_wheres(var calls : array<tuple<ExprCall?; LinqCall?>>) {
    // Mirror of collapse_chained_selects shape (find adjacent same-op pairs, rename to fresh param, rewire backlink, erase inner, stay at i) but composes via `pred1 && pred2` instead of function compose. Both predicates take the source element, so the composed body uses ONE fresh param shared by both halves. No has_sideeffects bail needed — composition doesn't duplicate either body (each runs at most once per element, same as the imperative chain with short-circuit `&&`).
    var i = 0
    while (i + 1 < length(calls)) {
        if (calls[i]._1.name != "where_" || calls[i + 1]._1.name != "where_") {
            i ++
            continue
        }
        var innerLam = calls[i]._0.arguments[1]
        var outerLam = calls[i + 1]._0.arguments[1]
        if (innerLam == null || outerLam == null
                || !(innerLam is ExprMakeBlock) || !(outerLam is ExprMakeBlock)) {
            i ++
            continue
        }
        var innerMblk = innerLam as ExprMakeBlock
        var innerBlk = innerMblk._block as ExprBlock
        var outerMblk = outerLam as ExprMakeBlock
        var outerBlk = outerMblk._block as ExprBlock
        if (innerBlk == null || outerBlk == null
                || innerBlk.arguments |> length != 1 || outerBlk.arguments |> length != 1
                || innerBlk.list |> length != 1 || outerBlk.list |> length != 1
                || !(innerBlk.list[0] is ExprReturn) || !(outerBlk.list[0] is ExprReturn)) {
            i ++
            continue
        }
        var innerRet = innerBlk.list[0] as ExprReturn
        var outerRet = outerBlk.list[0] as ExprReturn
        if (innerRet.subexpr == null || outerRet.subexpr == null) {
            i ++
            continue
        }
        // Rename both predicate params to the same fresh name — composed body references one shared bind.
        let freshName = qn("cw", innerLam.at)
        var innerBodyFresh = peel_lambda_rename_var(innerLam, freshName)
        var outerBodyFresh = peel_lambda_rename_var(outerLam, freshName)
        if (innerBodyFresh == null || outerBodyFresh == null) {
            i ++
            continue
        }
        var newLam = clone_expression(innerLam)
        var newMblk = newLam as ExprMakeBlock
        var newBlk = newMblk._block as ExprBlock
        if (newBlk == null || newBlk.list |> length != 1 || !(newBlk.list[0] is ExprReturn)) {
            i ++
            continue
        }
        var newRet = newBlk.list[0] as ExprReturn
        newBlk.arguments[0].name := freshName
        newRet.subexpr = merge_where_cond(innerBodyFresh, outerBodyFresh)
        calls[i + 1]._0.arguments[1] = newLam
        calls[i + 1]._0.arguments[0] = calls[i]._0.arguments[0]
        calls |> erase(i)
    }
}

[macro_function]
def private flatten_linq(var expr : Expression?)  {
    var top = expr
    var calls : array<tuple<ExprCall?; LinqCall?>>
    while (top != null) {
        if (top is ExprCall) {
            var topCall = top as ExprCall
            var topFunc = topCall.func
            while (topFunc.fromGeneric != null) {
                topFunc = topFunc.fromGeneric
            }
            let funcName = string(topFunc.name)
            if (!linqCalls.key_exists(funcName)) break
            var linqCall & = unsafe(linqCalls?[funcName] ?? default<LinqCall>)
            if (linqCall.moduleName != topFunc._module.name) break
            if (!linqCall.skip) {
                calls.push((topCall, unsafe(addr(linqCall))))
            }
            top = topCall.arguments[0]
        } else {
            break
        }
    }
    calls.reverse_inplace()
    // ok, at this point top is source
    return <- (top, calls)
}

[macro_function]
def private fold_linq_default(var expr : Expression?) : Expression? {
    var blk = new ExprBlock(at = expr.at)
    var (top, calls) = flatten_linq(expr)
    if (empty(calls)) return null
    var topExpr = clone_expression(top)
    var topExprType = clone_type(top._type)
    var topValue = qmacro(source)
    var argIndex = 0
    let argMax = length(calls)
    while (argIndex < argMax) {
        {
            var cll & = unsafe(calls[argIndex])
            let newArgName = "pass_{argIndex}"
            var callName = cll._1.name
            var inplace = false
            if (cll._0._type.isIterator || cll._0._type.isGoodArrayType) {
                // Names ending in `_` (e.g. `where_`) join the suffix without a connector — actual func is `where_to_array`, not `where__to_array`.
                let sep : string = ends_with(callName, "_") ? "" : "_"
                if (argIndex == 0 && top._type.isIterator && !cll._1.noToArrayVariant) {
                    callName = "{callName}{sep}to_array"
                } elif (argIndex != 0 && cll._1.inplace) {
                    callName = "{callName}{sep}inplace"
                    inplace = true
                }
            }
            for (i in cll._1.recursive) {   // recurse into sub-pipeline args (e.g. zip's second arg)
                if (i >= 1 && i < cll._0.arguments |> length && is_linq_call(cll._0.arguments[i])) {
                    var argExpr = make_call(cll._0.at, "_fold")
                    (argExpr as ExprCallMacro).arguments |> emplace <| cll._0.arguments[i]
                    cll._0.arguments[i] = argExpr
                }
            }
            var newCall = qmacro($c(callName)($e(topValue)))
            let numArgs = cll._0.arguments |> length
            for (i in 1..numArgs) {
                (newCall as ExprCall).arguments |> emplace_new <| clone_expression(cll._0.arguments[i])
            }
            if (inplace) {
                blk.list |> emplace(newCall)
            } else {
                // Macro emits `var pass_N = call` and a later `return <- pass_N`;
                blk.list |> emplace_new <| qmacro_expr() {
                    var $i(newArgName) = $e(newCall)  // nolint:PERF009
                }
                (blk.list.back() as ExprLet).variables[0].flags.can_shadow = true
                if (argIndex != 0) {
                    blk.list |> emplace_new <| qmacro_expr() {
                        delete $e(topValue)
                    }
                }
                topValue = qmacro($i(newArgName))
            }
            argIndex ++
        }
    }
    if (expr._type.isIterator) {
        blk.list |> emplace_new <| qmacro_expr() {
            return <- $e(topValue).to_sequence_move()
        }
    } else {
        blk.list |> emplace_new <| qmacro_expr() {
            return <- $e(topValue)
        }
    }
    topExpr.genFlags.alwaysSafe = true
    var res = qmacro(invoke($(source : $t(topExprType)) { $e(blk); }, $e(topExpr)))
    (((res as ExprInvoke).arguments[0] as ExprMakeBlock)._block as ExprBlock).arguments[0].flags.can_shadow = true
    res.force_generated(true)
    return res
}

[macro_function]
def private type_has_length(t : TypeDecl?) : bool {
    // Gate for macro-emitted `length(src)` — only types with a known overload (excludes lambdas / user iterables).
    if (t == null) return false
    return (t.isGoodArrayType || t.isGoodTableType || t.isString
        || t.isArray || t.isRange)
}

enum private LinqLane {
    UNKNOWN
    ARRAY
    COUNTER
    ACCUMULATOR
    EARLY_EXIT
}

[macro_function]
def private classify_terminator(name : string) : LinqLane {
    if (name == "count") return LinqLane.COUNTER
    // take/skip/take_while/skip_while trailing (after to_array strip) → ARRAY lane with implicit materialization.
    if (name == "where_" || name == "select" || name == "take" || name == "skip"
            || name == "take_while" || name == "skip_while") return LinqLane.ARRAY
    if (name == "sum" || name == "min" || name == "max" || name == "average" || name == "long_count") return LinqLane.ACCUMULATOR
    // EARLY_EXIT is also the dispatch lane for full-walk single-return terminators (last/single/element_at/aggregate) — same emit_early_exit_lane shape, different per-op state.
    if (name == "first" || name == "first_or_default" || name == "any" || name == "all" || name == "contains"
            || name == "last" || name == "last_or_default"
            || name == "single" || name == "single_or_default"
            || name == "element_at" || name == "element_at_or_default"
            || name == "aggregate") return LinqLane.EARLY_EXIT
    return LinqLane.UNKNOWN
}

[macro_function]
def private is_buffer_required_op(name : string) : bool {
    // Defense-in-depth marker: dedicated planners catch these first in the cascade; this arm only fires on shapes they don't recognize.
    return (name == "order_by" || name == "order" || name == "order_descending"
        || name == "order_by_descending"
        || name == "distinct" || name == "distinct_by"
        || name == "reverse"
        || name == "group_by" || name == "group_by_lazy"
        || name == "join" || name == "left_join" || name == "group_join")
}

[macro_function]
def private peel_each(var top : Expression?) : Expression? {
    // Unwrap `each(<arr>)` so downstream length/reserve hints fire; only safe when arg is a real array (iterator sources lack indexed length).
    if (!(top is ExprCall)) return top
    var topCall = top as ExprCall
    if (match_call_in_module(top, "each", "builtin") == null || topCall.arguments |> length != 1) return top
    let argExpr = topCall.arguments[0]
    if ((argExpr == null || argExpr._type == null)
            || (!argExpr._type.isGoodArrayType && !argExpr._type.isArray)) return top
    return clone_expression(argExpr)
}

[macro_function]
def private finalize_invoke(var res : Expression?; at : LineInfo) : Expression? {
    // Post-emit cleanup: stamp loc+generated for diagnostics; set can_shadow on every block arg so gensym sources coexist with user scope. Loop handles 1-source single planners and N-source zip emission uniformly.
    res.force_at(at)
    res.force_generated(true)
    let blk = (res as ExprInvoke).arguments[0] as ExprMakeBlock
    var blkBlock = blk._block as ExprBlock
    for (i in 0 .. length(blkBlock.arguments)) {
        blkBlock.arguments[i].flags.can_shadow = true
    }
    return res
}

[macro_function]
def private finalize_lane_emission(var topExprs : array<Expression?>; srcNames : array<string>;
                                   var bodyStmts : array<Expression?>; at : LineInfo) : Expression? {
    // Multi-source-aware invoke wrap used by emit_accumulator_lane / emit_early_exit_lane. 1-source clones top + derives param type + emits 1-arg invoke; 2-source (zip) does both sides + emits 2-arg invoke. Caller has already pushed the for-loop into bodyStmts.
    let nSrcs = length(srcNames)
    if (nSrcs != 1 && nSrcs != 2) panic("finalize_lane_emission: only 1- or 2-source supported (got {nSrcs}); higher-arity zip planners must extend this branch (or build the invoke wrap directly)")
    if (length(topExprs) != nSrcs) panic("finalize_lane_emission: topExprs length {length(topExprs)} != srcNames length {nSrcs}")
    if (nSrcs == 1) {
        var topExpr = clone_expression(topExprs[0])
        topExpr.genFlags.alwaysSafe = true
        var srcParamType = invoke_src_param_type(topExprs[0])
        var res = qmacro(invoke($($i(srcNames[0]) : $t(srcParamType)) {
            $b(bodyStmts)
        }, $e(topExpr)))
        return finalize_invoke(res, at)
    }
    var topAExpr = clone_expression(topExprs[0])
    topAExpr.genFlags.alwaysSafe = true
    var topBExpr = clone_expression(topExprs[1])
    topBExpr.genFlags.alwaysSafe = true
    var srcAType = invoke_src_param_type(topExprs[0])
    var srcBType = invoke_src_param_type(topExprs[1])
    var res = qmacro(invoke($($i(srcNames[0]) : $t(srcAType), $i(srcNames[1]) : $t(srcBType)) {
        $b(bodyStmts)
    }, $e(topAExpr), $e(topBExpr)))
    return finalize_invoke(res, at)
}

[clone(top), macro_function]
def private emit_length_shortcut(opName : string; var top : Expression?; srcName : string; at : LineInfo) : Expression? {
    // Count-shaped shortcut: emit `length(src)` (count, int) or `int64(length(src))` (long_count). length returns int, so the int() cast on count would be redundant — interp doesn't fold it and PERF020 fires.
    var topExpr = clone_expression(top)
    topExpr.genFlags.alwaysSafe = true
    var res : Expression?
    if (opName == "long_count") {
        res = qmacro(invoke($($i(srcName) : typedecl($e(topExpr))) {
            return int64(length($i(srcName)))
        }, $e(topExpr)))
    } else {
        res = qmacro(invoke($($i(srcName) : typedecl($e(topExpr))) {
            return length($i(srcName))
        }, $e(topExpr)))
    }
    return finalize_invoke(res, at)
}

[macro_function]
def private invoke_src_param_type(top : Expression?) : TypeDeclPtr {
    // Compute the invoke block's `$src` parameter type from the chain top:
    var srcParamType = clone_type(top._type)
    if (top._type.isIterator) {
        srcParamType.flags.constant = false
    }
    return srcParamType
}

[macro_function]
def private prepend_binds(var stmts : array<Expression?>; var intermediateBinds : array<Expression?>) {
    // Splice the chain's let-bindings (one per upstream `select`) ahead of the per-element
    if (empty(intermediateBinds)) return
    var prefixed : array<Expression?>
    prefixed |> reserve(long_length(intermediateBinds) + long_length(stmts))
    prefixed |> push_from(intermediateBinds)
    prefixed |> push_from(stmts)
    swap(stmts, prefixed)
}

[macro_function]
def private stmts_to_expr(var stmts : array<Expression?>) : Expression? {
    // Collapse a stmt list to a single expression: pass through when there's only one,
    if (length(stmts) == 1) return stmts[0]
    return <- qmacro_block() {
        $b(stmts)
    }
}

[macro_function]
def private wrap_with_condition(var body : Expression?; var cond : Expression?) : Expression? {
    // Wrap `body` in `if ($cond) { body }` when `cond` is non-null, else return `body` as-is.
    if (cond == null) return body
    return <- qmacro_expr() {
        if ($e(cond)) {
            $e(body)
        }
    }
}

[macro_function]
def private prepend_precond(var body : Expression?; var preCondStmts : array<Expression?>) : Expression? {
    // Splice per-element unconditional binds BEFORE the body. Used by the select+where
    if (empty(preCondStmts)) return body
    var stmts : array<Expression?>
    stmts |> reserve(long_length(preCondStmts) + 1_l)
    stmts |> push_from(preCondStmts)
    stmts |> push(body)
    return stmts_to_expr(stmts)
}

struct private RangeStateNames {
    skipName      : string
    takeCountName : string
    skippingName  : string
}

[macro_function]
def private make_range_names(at : LineInfo) : RangeStateNames {
    return <- RangeStateNames(
        skipName      = qn("skip", at),
        takeCountName = qn("tc", at),
        skippingName  = qn("skipping", at))
}

[macro_function]
def private append_ranges_prelude(var preludeStmts : array<Expression?>; var skipExpr, takeExpr, skipWhileCond : Expression?;
                                  names : RangeStateNames) {
    // Loop-level state for skip/take counters + skip_while flag (one-way, flips on first false-pred).
    let skipName      = names.skipName
    let takeCountName = names.takeCountName
    let skippingName  = names.skippingName
    if (skipExpr != null) {
        preludeStmts |> push <| qmacro_expr() {
            var $i(skipName) = $e(skipExpr)
        }
    }
    if (takeExpr != null) {
        preludeStmts |> push <| qmacro_expr() {
            var $i(takeCountName) = 0
        }
    }
    if (skipWhileCond != null) {
        preludeStmts |> push <| qmacro_expr() {
            var $i(skippingName) = true
        }
    }
}

[macro_function]
def private wrap_with_ranges(var stmts : array<Expression?>; var skipExpr, takeExpr, skipWhileCond, takeWhileCond : Expression?;
                             names : RangeStateNames) {
    // Per-match prefix order: take-guard → skip counter → skip_while flip → take_while break → takeCount++ → body. takeCount++ sits AFTER skip_while/take_while so gated-out elements don't eat the take(N) budget.
    if (skipExpr == null && takeExpr == null && skipWhileCond == null && takeWhileCond == null) return
    let skipName      = names.skipName
    let takeCountName = names.takeCountName
    let skippingName  = names.skippingName
    var prefixed : array<Expression?>
    prefixed |> reserve(length(stmts) + 5)
    if (takeExpr != null) {
        // `>=` (not `==`) so non-positive N short-circuits on the first iteration —
        prefixed |> push <| qmacro_expr() {
            if ($i(takeCountName) >= $e(takeExpr)) {
                break
            }
        }
    }
    if (skipExpr != null) {
        prefixed |> push <| qmacro_expr() {
            if ($i(skipName) > 0) {
                $i(skipName) --
                continue
            }
        }
    }
    if (skipWhileCond != null) {
        prefixed |> push <| qmacro_expr() {
            if ($i(skippingName)) {
                if ($e(skipWhileCond)) {
                    continue
                }
                $i(skippingName) = false
            }
        }
    }
    if (takeWhileCond != null) {
        prefixed |> push <| qmacro_expr() {
            if (!($e(takeWhileCond))) {
                break
            }
        }
    }
    if (takeExpr != null) {
        prefixed |> push <| qmacro_expr() {
            $i(takeCountName) ++
        }
    }
    prefixed |> push_from(stmts)
    swap(stmts, prefixed)
}

[macro_function]
def private min_max_compare(workhorse : bool; opName : string; valName, accName : string) : Expression? {
    // Workhorse types use direct `<` / `>` (single-instruction comparison) — this is the
    if (workhorse && opName == "min") return <- qmacro($i(valName) < $i(accName))
    if (workhorse) return <- qmacro($i(valName) > $i(accName))
    if (opName == "min") return <- qmacro(_::less($i(valName), $i(accName)))
    return <- qmacro(_::less($i(accName), $i(valName)))
}

[macro_function]
def private build_accumulator_perelement_stmts(opName : string;
                                               accName, valBindName, firstName, cntName : string;
                                               var valueExpr : Expression?;
                                               workhorse : bool;
                                               isDoubleAccType : bool) : array<Expression?> {
    // Per-element accumulator update for sum / min / max / average / count / long_count. Shared between emit_accumulator_lane (array side, valueExpr is the cloned projection or `it`) and emit_decs_accumulator (decs side, valueExpr wraps chainInfo.finalBind as a qmacro_expr). Preludes + return-expr stay caller-side because acc types and empty-source guards differ.
    var stmts : array<Expression?>
    if (opName == "count" || opName == "long_count") {
        stmts |> push <| qmacro_expr() {
            $i(accName) ++
        }
    } elif (opName == "sum") {
        stmts |> push <| qmacro_expr() {
            $i(accName) += $e(valueExpr)
        }
    } elif (opName == "average") {
        // double accumulator matches linq.das:1545 average() semantics. Already-double sources skip the cast (PERF020).
        if (isDoubleAccType) {
            stmts |> push_from <| qmacro_block_to_array() {
                $i(accName) += $e(valueExpr)
                $i(cntName) ++
            }
        } else {
            stmts |> push_from <| qmacro_block_to_array() {
                $i(accName) += double($e(valueExpr))
                $i(cntName) ++
            }
        }
    } elif (opName == "min" || opName == "max") {
        var compareExpr = min_max_compare(workhorse, opName, valBindName, accName)
        stmts |> push_from <| qmacro_block_to_array() {
            let $i(valBindName) = $e(valueExpr)
            if ($i(firstName)) {
                $i(accName) := $i(valBindName)
                $i(firstName) = false
            } elif ($e(compareExpr)) {
                $i(accName) := $i(valBindName)
            }
        }
    }
    return <- stmts
}

[clone(top), macro_function]
def private emit_counter_lane(var top : Expression?; srcName, accName, itName : string; names : RangeStateNames;
                              var skipExpr, takeExpr, skipWhileCond : Expression?;
                              var loopBody : Expression?; at : LineInfo) : Expression? {
    // Counter lane: `[skip/take init]; var acc = 0; for (it in src) { $loopBody }; return acc`
    var topExpr = clone_expression(top)
    topExpr.genFlags.alwaysSafe = true
    var srcParamType = invoke_src_param_type(top)
    var bodyStmts : array<Expression?>
    append_ranges_prelude(bodyStmts, skipExpr, takeExpr, skipWhileCond, names)
    bodyStmts |> push_from <| qmacro_block_to_array() {
        var $i(accName) = 0
        for ($i(itName) in $i(srcName)) {
            $e(loopBody)
        }
        return $i(accName)
    }
    var res = qmacro(invoke($($i(srcName) : $t(srcParamType)) {
        $b(bodyStmts)
    }, $e(topExpr)))
    return finalize_invoke(res, at)
}

[clone(top), macro_function]
def private emit_array_lane(var top : Expression?; isIter : bool; var loopBody : Expression?; var elementType : TypeDeclPtr;
                            srcName, accName, itName : string; names : RangeStateNames;
                            var skipExpr, takeExpr, skipWhileCond : Expression?;
                            at : LineInfo) : Expression? {
    // Array lane: `[skip/take init]; var acc : array<T>; [reserve]; for (it in src) { $loopBody }; return <- acc`
    let sourceHasLength = type_has_length(top._type)
    var topExpr = clone_expression(top)
    topExpr.genFlags.alwaysSafe = true
    var srcParamType = invoke_src_param_type(top)
    var bodyStmts : array<Expression?>
    append_ranges_prelude(bodyStmts, skipExpr, takeExpr, skipWhileCond, names)
    bodyStmts |> push <| qmacro_expr() {
        var $i(accName) : array<$t(elementType)>
    }
    if (sourceHasLength && !isIter) {
        if (takeExpr != null) {
            // Inline `min` so the emission doesn't depend on `require math` at the call site.
            bodyStmts |> push <| qmacro_expr() {
                $i(accName) |> reserve($e(takeExpr) < length($i(srcName)) ? $e(takeExpr) : length($i(srcName)))
            }
        } else {
            bodyStmts |> push <| qmacro_expr() {
                $i(accName) |> reserve(length($i(srcName)))
            }
        }
    }
    bodyStmts |> push <| qmacro_expr() {
        for ($i(itName) in $i(srcName)) {
            $e(loopBody)
        }
    }
    if (isIter) {
        bodyStmts |> push <| qmacro_expr() {
            return <- $i(accName).to_sequence_move()
        }
    } else {
        bodyStmts |> push <| qmacro_expr() {
            return <- $i(accName)
        }
    }
    var res = qmacro(invoke($($i(srcName) : $t(srcParamType)) {
        $b(bodyStmts)
    }, $e(topExpr)))
    return finalize_invoke(res, at)
}

[macro_function]
def private emit_accumulator_lane(
                                  opName : string;
                                  var topExprs : array<Expression?>;
                                  var projection : Expression?;
                                  var whereCond : Expression?;
                                  var postTakeWhereCond : Expression?;
                                  var intermediateBinds : array<Expression?>;
                                  var preCondStmts : array<Expression?>;
                                  var elementType : TypeDeclPtr;
                                  srcNames : array<string>;
                                  accName, itName : string;
                                  names : RangeStateNames;
                                  var skipExpr, takeExpr, skipWhileCond, takeWhileCond : Expression?;
                                  at : LineInfo
                                  ) : Expression? {
    // Ring 1 single-pass accumulator lane: sum / min / max / average / long_count.
    var valueExpr : Expression?
    if (projection != null) {
        valueExpr = clone_expression(projection)
    } else {
        valueExpr = qmacro_expr() {
            $i(itName)
        }
    }
    // Accumulator type: projection-typed if present, else source's element type. Strip
    var accType : TypeDeclPtr
    if (projection != null) {
        accType = clone_type(projection._type)
    } else {
        accType = clone_type(elementType)
    }
    if (accType != null) {
        accType.flags.constant = false
        accType.flags.ref = false
    }
    let valBindName = qn("val", at)
    let firstName = qn("first", at)
    let cntName = qn("cnt", at)
    let workhorse = ((projection != null && projection._type != null && projection._type.isWorkhorseType)
        || (projection == null && elementType != null && elementType.isWorkhorseType))
    let isDoubleAccType = accType != null && accType.baseType == Type.tDouble
    var preludeStmts : array<Expression?>
    var perMatchStmts : array<Expression?>
    var returnExpr : Expression?
    if (opName == "long_count") {
        preludeStmts <- qmacro_block_to_array() {
            var $i(accName) : int64 = 0l
        }
        // Value is unused; mirror counter-lane discipline and bind only when projection has side effects (keeps the AST visit but discards the value).
        if (projection != null && has_sideeffects(projection)) {
            let finalBindName = qn("vfinal", at)
            perMatchStmts |> push <| qmacro_expr() {
                var $i(finalBindName) = $e(projection)
            }
        }
        returnExpr = qmacro_expr() {
            $i(accName)
        }
    } elif (opName == "sum") {
        preludeStmts <- qmacro_block_to_array() {
            var $i(accName) : $t(accType) = default<$t(accType)>
        }
        returnExpr = qmacro_expr() {
            $i(accName)
        }
    } elif (opName == "average") {
        // Matches linq.das:1545 average(iterator) — always-double accumulator, uint64 counter, empty source returns 0.0lf instead of NaN.
        preludeStmts <- qmacro_block_to_array() {
            var $i(accName) : double = 0lf
            var $i(cntName) : uint64 = 0ul
        }
        returnExpr = qmacro_expr() {
            $i(cntName) != 0ul ? $i(accName) / double($i(cntName)) : 0lf
        }
    } elif (opName == "min" || opName == "max") {
        preludeStmts <- qmacro_block_to_array() {
            var $i(firstName) = true
            var $i(accName) : $t(accType)
        }
        returnExpr = qmacro_expr() {
            $i(accName)
        }
    } else {
        return null
    }
    perMatchStmts |> push_from <| build_accumulator_perelement_stmts(opName, accName, valBindName, firstName, cntName, valueExpr, workhorse, isDoubleAccType)
    prepend_binds(perMatchStmts, intermediateBinds)
    // Theme 2 5c: post-take where wraps the per-match work (acc++ / += / cmp+update) BEFORE the take cap so the cap still ticks unconditionally per iteration.
    if (postTakeWhereCond != null) {
        var gated = wrap_with_condition(stmts_to_expr(perMatchStmts), postTakeWhereCond)
        perMatchStmts |> clear
        perMatchStmts |> push(gated)
    }
    wrap_with_ranges(perMatchStmts, skipExpr, takeExpr, skipWhileCond, takeWhileCond, names)
    var loopBody = prepend_precond(wrap_with_condition(stmts_to_expr(perMatchStmts), whereCond), preCondStmts)
    // Collect all body statements into one list so they share scope when spliced via $b.
    var bodyStmts : array<Expression?>
    bodyStmts |> reserve(length(preludeStmts) + 4)
    append_ranges_prelude(bodyStmts, skipExpr, takeExpr, skipWhileCond, names)
    bodyStmts |> push_from(preludeStmts)
    // For-loop emission: single-source uses one iter var (itName); 2-source (zip) uses literal `itA, itB` parallel iter vars (qmacro for-loop iter-var position doesn't accept $i(...) splice). Caller (plan_zip) threads `let it = (itA, itB)` via preCondStmts so itName resolves inside the loop body.
    if (length(srcNames) == 1) {
        bodyStmts |> push <| qmacro_expr() {
            for ($i(itName) in $i(srcNames[0])) {
                $e(loopBody)
            }
        }
    } else {
        bodyStmts |> push <| qmacro_expr() {
            for (itA, itB in $i(srcNames[0]), $i(srcNames[1])) {
                $e(loopBody)
            }
        }
    }
    bodyStmts |> push <| qmacro_expr() {
        return $e(returnExpr)
    }
    return finalize_lane_emission(topExprs, srcNames, bodyStmts, at)
}

[macro_function]
def private emit_any_empty_shortcut(var top : Expression?; srcName : string; at : LineInfo) : Expression? {
    // any (no predicate) shortcut: emit `!empty(src)` directly. Caller has verified no
    var topExpr = clone_expression(top)
    topExpr.genFlags.alwaysSafe = true
    var res = qmacro(invoke($($i(srcName) : typedecl($e(topExpr))) {
        return !empty($i(srcName))
    }, $e(topExpr)))
    return finalize_invoke(res, at)
}

[macro_function]
def private emit_early_exit_lane(
                                 opName : string;
                                 var topExprs : array<Expression?>;
                                 var projection : Expression?;
                                 var whereCond : Expression?;
                                 var postTakeWhereCond : Expression?;
                                 var intermediateBinds : array<Expression?>;
                                 var preCondStmts : array<Expression?>;
                                 var elementType : TypeDeclPtr;
                                 terminatorCall : ExprCall?;
                                 srcNames : array<string>;
                                 itName : string;
                                 names : RangeStateNames;
                                 var skipExpr, takeExpr, skipWhileCond, takeWhileCond : Expression?;
                                 at : LineInfo
                                 ) : Expression? {
    // Ring 2 early-exit lane: first / first_or_default / any / all / contains.
    var perMatchStmts : array<Expression?>
    var valueName = itName
    let projBindName = qn("vproj", at)
    if (projection != null) {
        perMatchStmts |> push <| qmacro_expr() {
            let $i(projBindName) = $e(projection)
        }
        valueName = projBindName
    }
    // Per-op (preludeStmts, per-element work appended to perMatchStmts, tailStmts).
    var preludeStmts : array<Expression?>
    var tailStmts : array<Expression?>
    if (opName == "first") {
        // Return type for first: projection's type (if any) or source's element type.
        var retType : TypeDeclPtr
        if (projection != null) {
            retType = clone_type(projection._type)
        } else {
            retType = clone_type(elementType)
        }
        if (retType != null) {
            retType.flags.constant = false
            retType.flags.ref = false
        }
        perMatchStmts |> push <| qmacro_expr() {
            return $i(valueName)
        }
        // Tail: panic + unreachable default-return (matches linq.das line 2383).
        tailStmts <- qmacro_block_to_array() {
            panic("sequence contains no elements")
            return default<$t(retType)>
        }
    } elif (opName == "first_or_default") {
        // Bind `d` once at the top of the block (eager evaluation, matches linq.das line 2397).
        let defaultName = qn("dval", at)
        preludeStmts |> push <| qmacro_expr() {
            let $i(defaultName) = $e(terminatorCall.arguments[1])
        }
        perMatchStmts |> push <| qmacro_expr() {
            return $i(valueName)
        }
        tailStmts |> push <| qmacro_expr() {
            return $i(defaultName)
        }
    } elif (opName == "any") {
        let argCount = length(terminatorCall.arguments)
        if (argCount > 1) {
            var predExpr = peel_lambda_rename_var(terminatorCall.arguments[1], valueName)
            perMatchStmts |> push <| qmacro_expr() {
                if ($e(predExpr)) {
                    return true
                }
            }
        } else {
            perMatchStmts |> push <| qmacro_expr() {
                return true
            }
        }
        tailStmts |> push <| qmacro_expr() {
            return false
        }
    } elif (opName == "all") {
        var predExpr = peel_lambda_rename_var(terminatorCall.arguments[1], valueName)
        perMatchStmts |> push <| qmacro_expr() {
            if (!$e(predExpr)) {
                return false
            }
        }
        tailStmts |> push <| qmacro_expr() {
            return true
        }
    } elif (opName == "contains") {
        // `v` is bound once at the top of the block; per-element compare uses the bound name
        let containsValName = qn("cval", at)
        preludeStmts |> push <| qmacro_expr() {
            let $i(containsValName) = $e(terminatorCall.arguments[1])
        }
        perMatchStmts |> push <| qmacro_expr() {
            if ($i(valueName) == $i(containsValName)) {
                return true
            }
        }
        tailStmts |> push <| qmacro_expr() {
            return false
        }
    } elif (opName == "last" || opName == "last_or_default") {
        // Full walk; per-match overwrites a single bind; tail returns it after found-check (panic/default for empty).
        var retType : TypeDeclPtr
        if (projection != null) {
            retType = clone_type(projection._type)
        } else {
            retType = clone_type(elementType)
        }
        if (retType != null) {
            retType.flags.constant = false
            retType.flags.ref = false
        }
        let foundName = qn("found", at)
        let lastBindName = qn("lst", at)
        preludeStmts |> push_from <| qmacro_block_to_array() {
            var $i(foundName) = false
            var $i(lastBindName) : $t(retType)
        }
        perMatchStmts |> push_from <| qmacro_block_to_array() {
            $i(foundName) = true
            $i(lastBindName) := $i(valueName)
        }
        if (opName == "last") {
            tailStmts <- qmacro_block_to_array() {
                if (!$i(foundName)) {
                    panic("sequence contains no elements")
                }
                return <- $i(lastBindName)
            }
        } else {
            // Bind `d` once at the top (eager, matches linq.das line 2621); copy/move probe matches linq.das can_copy/clone_to_move branch.
            let defaultName = qn("dval", at)
            let dvCanCopy = (terminatorCall.arguments[1]._type != null && terminatorCall.arguments[1]._type.canCopy)
            preludeStmts |> push(emit_default_bind(defaultName, terminatorCall.arguments[1], dvCanCopy))
            tailStmts <- qmacro_block_to_array() {
                if (!$i(foundName)) {
                    $e(emit_default_return(defaultName, dvCanCopy))
                }
                return <- $i(lastBindName)
            }
        }
    } elif (opName == "single" || opName == "single_or_default") {
        // Full walk; on the SECOND match `single` panics and `single_or_default` returns the default; same for empty source.
        var retType : TypeDeclPtr
        if (projection != null) {
            retType = clone_type(projection._type)
        } else {
            retType = clone_type(elementType)
        }
        if (retType != null) {
            retType.flags.constant = false
            retType.flags.ref = false
        }
        let foundName = qn("found", at)
        let bindName = qn("sng", at)
        preludeStmts |> push_from <| qmacro_block_to_array() {
            var $i(foundName) = false
            var $i(bindName) : $t(retType)
        }
        if (opName == "single") {
            perMatchStmts |> push_from <| qmacro_block_to_array() {
                if ($i(foundName)) {
                    panic("sequence contains more than one element")
                }
                $i(foundName) = true
                $i(bindName) := $i(valueName)
            }
            tailStmts <- qmacro_block_to_array() {
                if (!$i(foundName)) {
                    panic("sequence contains no elements")
                }
                return <- $i(bindName)
            }
        } else {
            let defaultName = qn("dval", at)
            let dvCanCopy = (terminatorCall.arguments[1]._type != null && terminatorCall.arguments[1]._type.canCopy)
            preludeStmts |> push(emit_default_bind(defaultName, terminatorCall.arguments[1], dvCanCopy))
            // `single_or_default` early-returns default on the SECOND match (subsequent matches don't matter).
            perMatchStmts |> push_from <| qmacro_block_to_array() {
                if ($i(foundName)) {
                    $e(emit_default_return(defaultName, dvCanCopy))
                }
                $i(foundName) = true
                $i(bindName) := $i(valueName)
            }
            tailStmts <- qmacro_block_to_array() {
                if (!$i(foundName)) {
                    $e(emit_default_return(defaultName, dvCanCopy))
                }
                return <- $i(bindName)
            }
        }
    } elif (opName == "element_at" || opName == "element_at_or_default") {
        // Counter-style early-exit: return the N-th element that survives upstream where/select. Negative index panics or returns default pre-loop.
        var retType : TypeDeclPtr
        if (projection != null) {
            retType = clone_type(projection._type)
        } else {
            retType = clone_type(elementType)
        }
        if (retType != null) {
            retType.flags.constant = false
            retType.flags.ref = false
        }
        let idxName = qn("idx", at)
        let cntName = qn("ec", at)
        preludeStmts |> push <| qmacro_expr() {
            let $i(idxName) = $e(terminatorCall.arguments[1])
        }
        if (opName == "element_at") {
            preludeStmts |> push <| qmacro_expr() {
                if ($i(idxName) < 0) {
                    panic("element index out of range")
                }
            }
        } else {
            // `var t : TT; return <- t` (move-init) works for both copyable and non-copyable element types; matches linq.das line 2547.
            let tName = qn("emptyT", at)
            preludeStmts |> push_from <| qmacro_block_to_array() {
                if ($i(idxName) < 0) {
                    var $i(tName) : $t(retType)
                    return <- $i(tName)
                }
            }
        }
        preludeStmts |> push <| qmacro_expr() {
            var $i(cntName) = 0
        }
        perMatchStmts |> push_from <| qmacro_block_to_array() {
            if ($i(cntName) == $i(idxName)) {
                return $i(valueName)
            }
            $i(cntName) ++
        }
        if (opName == "element_at") {
            let tName = qn("emptyT", at)
            tailStmts <- qmacro_block_to_array() {
                panic("element index out of range")
                var $i(tName) : $t(retType)
                return <- $i(tName)
            }
        } else {
            let tName = qn("emptyT", at)
            tailStmts |> push_from <| qmacro_block_to_array() {
                var $i(tName) : $t(retType)
                return <- $i(tName)
            }
        }
    } elif (opName == "aggregate") {
        // Peel single-return `block<(acc, x):AGG>` body inline; fall back to invoke on multi-stmt. Workhorse seed picks `=` / `return`; non-workhorse uses `<-` / `return <-` (matches linq.das:1466 static_if).
        var seedExpr = clone_expression(terminatorCall.arguments[1])
        var aggFn = clone_expression(terminatorCall.arguments[2])
        var accType = clone_type(seedExpr._type)
        if (accType != null) {
            accType.flags.constant = false
            accType.flags.ref = false
        }
        let aggAccName = qn("agg", at)
        let aggIsWorkhorse = (seedExpr._type != null && seedExpr._type.isWorkhorseType)
        if (aggIsWorkhorse) {
            preludeStmts |> push <| qmacro_expr() {
                var $i(aggAccName) : $t(accType) = $e(seedExpr)
            }
        } else {
            preludeStmts |> push <| qmacro_expr() {
                var $i(aggAccName) : $t(accType) <- $e(seedExpr)
            }
        }
        var peeled = peel_lambda_rename_2vars(aggFn, aggAccName, valueName)
        if (peeled != null) {
            if (aggIsWorkhorse) {
                perMatchStmts |> push <| qmacro_expr() {
                    $i(aggAccName) = $e(peeled)
                }
            } else {
                perMatchStmts |> push <| qmacro_expr() {
                    $i(aggAccName) <- $e(peeled)
                }
            }
        } else {
            if (aggIsWorkhorse) {
                perMatchStmts |> push <| qmacro_expr() {
                    $i(aggAccName) = invoke($e(aggFn), $i(aggAccName), $i(valueName))
                }
            } else {
                perMatchStmts |> push <| qmacro_expr() {
                    $i(aggAccName) <- invoke($e(aggFn), $i(aggAccName), $i(valueName))
                }
            }
        }
        if (aggIsWorkhorse) {
            tailStmts |> push <| qmacro_expr() {
                return $i(aggAccName)
            }
        } else {
            tailStmts |> push <| qmacro_expr() {
                return <- $i(aggAccName)
            }
        }
    } else {
        return null
    }
    prepend_binds(perMatchStmts, intermediateBinds)
    // Theme 2 5c: post-take where wraps the per-match work BEFORE the take cap so the cap still ticks unconditionally per iteration.
    if (postTakeWhereCond != null) {
        var gated = wrap_with_condition(stmts_to_expr(perMatchStmts), postTakeWhereCond)
        perMatchStmts |> clear
        perMatchStmts |> push(gated)
    }
    wrap_with_ranges(perMatchStmts, skipExpr, takeExpr, skipWhileCond, takeWhileCond, names)
    var loopBody = prepend_precond(wrap_with_condition(stmts_to_expr(perMatchStmts), whereCond), preCondStmts)
    // Single-$b body so all stmts (skip/take counters + prelude + for + tail) share scope
    var bodyStmts : array<Expression?>
    bodyStmts |> reserve(length(preludeStmts) + length(tailStmts) + 3)
    append_ranges_prelude(bodyStmts, skipExpr, takeExpr, skipWhileCond, names)
    bodyStmts |> push_from(preludeStmts)
    // For-loop emission: 1-source uses itName; 2-source (zip) uses literal `itA, itB` (qmacro for-iter-var position doesn't accept $i splice). Caller (plan_zip) threads `let it = (itA, itB)` via preCondStmts.
    if (length(srcNames) == 1) {
        bodyStmts |> push <| qmacro_expr() {
            for ($i(itName) in $i(srcNames[0])) {
                $e(loopBody)
            }
        }
    } else {
        bodyStmts |> push <| qmacro_expr() {
            for (itA, itB in $i(srcNames[0]), $i(srcNames[1])) {
                $e(loopBody)
            }
        }
    }
    bodyStmts |> push_from(tailStmts)
    return finalize_lane_emission(topExprs, srcNames, bodyStmts, at)
}

[macro_function]
def private merge_where_cond(var curr : Expression?; var pred : Expression?) : Expression? {
    if (curr == null) return pred
    return <- qmacro($e(curr) && $e(pred))
}

[macro_function]
def private strip_const_ref(var t : TypeDeclPtr) : TypeDeclPtr {
    if (t != null) {
        t.flags.constant = false
        t.flags.ref = false
    }
    return t
}

[clone(top), macro_function]
def private finalize_emission_stmts(top : Expression?; srcName : string; at : LineInfo; var stmts : array<Expression?>) : Expression? {
    var srcParamType = invoke_src_param_type(top)
    var topExpr = clone_expression(top)
    topExpr.genFlags.alwaysSafe = true
    var emission : Expression? = qmacro(invoke($($i(srcName) : $t(srcParamType)) {
        $b(stmts)
    }, $e(topExpr)))
    return finalize_invoke(emission, at)
}

// ===== SourceAdapter helpers (PR C) — used by emit fns to dispatch source-loop + invoke wrap per adapter. =====

// The per-element bind name emit fns peel where/select/key/value lambdas against. Drives build_decs_inner_for_pruned's
// pattern matching, so the decs side MUST use `decs_tup`. DecsJoin carries the per-pair result-lam bind name in its payload.
[macro_function]
def private adapter_bind_name(adapter : SourceAdapter; at : LineInfo) : string {
    return (adapter as DecsJoin).resBindName if (adapter is DecsJoin)
    return qn("decs_tup", at) if (adapter is Decs)
    return qn("it", at)
}

// Source element type for state-decl construction. Returns a fresh clone so callers can pass it to strip_const_ref / $t() without aliasing.
// Array: top._type.firstType. Decs: bridge.elementType. DecsJoin: per-pair resultType from payload. Zip: defensive null
// (plan_group_by_core never reaches Zip; emit_zip computes element type inline from zip's _type.firstType + select rebind).
[macro_function]
def private adapter_element_type(adapter : SourceAdapter) : TypeDeclPtr {
    if (adapter is DecsJoin) return clone_type((adapter as DecsJoin).resultType)
    if (adapter is Zip) return null
    if (adapter is Decs) {
        let bridge = (adapter as Decs)._0
        if (bridge == null) return null
        return clone_type(bridge.elementType)
    }
    let top = (adapter as Array)._0
    if (top == null || top._type == null) return null
    return clone_type(top._type.firstType)
}

// Per-element source loop. Array: `for(bindName in srcName) { body }`. Decs: `for_each_archetype(...) { build_decs_inner_for_pruned(bridge, tupName, body) }`. DecsJoin (PR D2): hashB-collect + srcA-probe + per-pair result-lam bind (lifted from PR D1's GroupBy `adapter_emit_source_loop`; GroupBy is the only consumer — `emit_decs_join` keeps its own collect+probe inline to preserve the count-no-where bucket-length fast path). Zip (PR D2): plain `for (itA, itB in srcA, srcB) { body }` — the `let it = (itA, itB)` bind, when needed by chain ops, is prepended inside `body` by emit_zip (only emit_zip knows whether the body references `it`).
[macro_function]
def private adapter_wrap_source_loop(adapter : SourceAdapter; var body : Expression?; at : LineInfo) : Expression? {
    if (adapter is DecsJoin) {
        // GroupBy's decs-join is the only consumer (emit_decs_join keeps its own inline collect+probe per D2-A).
        let dj & = unsafe(adapter as DecsJoin)
        let jhashName = qn("djoin_jhash", at)
        let arrName   = qn("djoin_jarr", at)
        let tupAName  = qn("djoin_jta", at)
        let tupBName  = qn("djoin_jtb", at)
        let resName   = dj.resBindName
        var keyaBody   = peel_lambda_rename_var(dj.keyaLam, tupAName)
        var keybBody   = peel_lambda_rename_var(dj.keybLam, tupBName)
        var resultBody = peel_lambda_rename_2vars(dj.resultLam, tupAName, tupBName)
        if (keyaBody == null || keybBody == null || resultBody == null) return null
        var tupBindA = build_decs_tup_bind(dj.bridgeA, tupAName, at)
        var tupBindB = build_decs_tup_bind(dj.bridgeB, tupBName, at)
        if (tupBindA == null || tupBindB == null) return null
        var collectBody <- qmacro_block_to_array() {
            // nolint:PERF006 per-key bucket size unknown ahead of time
            $i(jhashName)[$e(keybBody)] |> push_clone($i(tupBName))
        }
        var collectInner = build_decs_inner_for(dj.bridgeB, tupBindB, stmts_to_expr(collectBody), at)
        var probeBody <- qmacro_block_to_array() {
            get($i(jhashName), $e(keyaBody), $(var $i(arrName) : array<$t(dj.tupBType)>) {
                for ($i(tupBName) in $i(arrName)) {
                    let $i(resName) : $t(dj.resultType) = $e(resultBody)
                    $e(body)
                }
            })
        }
        var probeInner = build_decs_inner_for(dj.bridgeA, tupBindA, stmts_to_expr(probeBody), at)
        return <- qmacro_block() {
            var $i(jhashName) : table<$t(dj.keyType); array<$t(dj.tupBType)>>
            for_each_archetype($e(dj.bridgeB.reqHashExpr), $e(dj.bridgeB.erqExpr), $($i(dj.bridgeB.archName) : Archetype) {
                $e(collectInner)
            })
            for_each_archetype($e(dj.bridgeA.reqHashExpr), $e(dj.bridgeA.erqExpr), $($i(dj.bridgeA.archName) : Archetype) {
                $e(probeInner)
            })
        }
    }
    if (adapter is Zip) {
        let zip & = unsafe(adapter as Zip)
        let srcAName = zip.srcAName
        let srcBName = zip.srcBName
        return <- qmacro_block() {
            for (itA, itB in $i(srcAName), $i(srcBName)) {
                $e(body)
            }
        }
    }
    if (adapter is Decs) {
        let bridge = (adapter as Decs)._0
        let tupName = (adapter as Decs)._1
        var forExprNode = build_decs_inner_for_pruned(bridge, tupName, body, at)
        let archName = bridge.archName
        return <- qmacro_expr() {
            for_each_archetype($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) {
                $e(forExprNode)
            })
        }
    }
    let srcName = (adapter as Array)._1
    let bindName = adapter_bind_name(adapter, at)
    return <- qmacro_block() {
        for ($i(bindName) in $i(srcName)) {
            $e(body)
        }
    }
}

// Wrap stmts into the adapter's outer invoke. Array binds top as source-arg; Decs / DecsJoin are zero-arg invokes (state
// hoisted); Zip is a 2-source invoke (force_at + force_generated + can_shadow on both arg flags). retType null = infer.
// wrapIter wraps the emission with .to_sequence_move() — caller drives via ctx.expr_is_iterator on array-returning emits.
[macro_function]
def private adapter_wrap_invoke(adapter : SourceAdapter; var stmts : array<Expression?>; retType : TypeDeclPtr; wrapIter : bool; at : LineInfo) : Expression? {
    var emission : Expression?
    if (adapter is Zip) {
        let zip & = unsafe(adapter as Zip)
        let srcAName = zip.srcAName
        let srcBName = zip.srcBName
        if (retType != null) {
            emission = qmacro(invoke($($i(srcAName) : $t(zip.srcAType), $i(srcBName) : $t(zip.srcBType)) : $t(retType) {
                $b(stmts)
            }, $e(zip.srcAExpr), $e(zip.srcBExpr)))
        } else {
            emission = qmacro(invoke($($i(srcAName) : $t(zip.srcAType), $i(srcBName) : $t(zip.srcBType)) {
                $b(stmts)
            }, $e(zip.srcAExpr), $e(zip.srcBExpr)))
        }
        emission.force_at(at)
        emission.force_generated(true)
        let blk = (emission as ExprInvoke).arguments[0] as ExprMakeBlock
        (blk._block as ExprBlock).arguments[0].flags.can_shadow = true
        (blk._block as ExprBlock).arguments[1].flags.can_shadow = true
        if (wrapIter) {
            emission = qmacro($e(emission).to_sequence_move())
            emission.force_generated(true)
        }
        return emission
    }
    if (adapter is Decs || adapter is DecsJoin) {
        if (retType != null) {
            emission = qmacro(invoke($() : $t(retType) {
                $b(stmts)
            }))
        } else {
            emission = qmacro(invoke($() {
                $b(stmts)
            }))
        }
        return finalize_decs_emission(emission, at, wrapIter)
    }
    let top = (adapter as Array)._0
    let srcName = (adapter as Array)._1
    var srcParamType = invoke_src_param_type(top)
    var topExpr = clone_expression(top)
    topExpr.genFlags.alwaysSafe = true
    if (retType != null) {
        emission = qmacro(invoke($($i(srcName) : $t(srcParamType)) : $t(retType) {
            $b(stmts)
        }, $e(topExpr)))
    } else {
        emission = qmacro(invoke($($i(srcName) : $t(srcParamType)) {
            $b(stmts)
        }, $e(topExpr)))
    }
    emission = finalize_invoke(emission, at)
    if (wrapIter) {
        emission = qmacro($e(emission).to_sequence_move())
        emission.force_generated(true)
    }
    return emission
}

[macro_function]
def private finalize_decs_emission(var emission : Expression?; at : LineInfo; wrapToIter : bool) : Expression? {
    emission.force_at(at)
    emission.force_generated(true)
    if (wrapToIter) {
        emission = qmacro($e(emission).to_sequence_move())
    }
    return emission
}

[macro_function]
def private buffer_return(bufName : string; isIterator : bool) : Expression? {
    if (isIterator) return <- qmacro_expr() {
        return <- $i(bufName).to_sequence_move()
    }
    return <- qmacro_expr() {
        return <- $i(bufName)
    }
}

[macro_function]
def private order_top_n_call_name(orderName : string) : string {
    if (orderName == "order") return "top_n"
    if (orderName == "order_descending") return "top_n_descending"
    if (orderName == "order_by") return "top_n_by"
    if (orderName == "order_by_descending") return "top_n_by_descending"
    return ""
}

def private order_min_call_name(orderName : string; hasKey : bool) : string {
    // `order + first` collapses to min/max — asc → min, desc → max. Keyed forms route to min_by/max_by.
    let isDesc = orderName == "order_descending" || orderName == "order_by_descending"
    if (hasKey) return isDesc ? "max_by" : "min_by"
    return isDesc ? "max" : "min"
}

[macro_function]
def private try_make_inline_cmp(orderKey : Expression?; orderName : string;
                                elemType : TypeDeclPtr; at : LineInfo) : Expression? {
    if (orderKey == null || !(orderKey is ExprMakeBlock)) return null
    var mblk = orderKey as ExprMakeBlock
    var blk = mblk._block as ExprBlock
    if (blk.arguments |> length != 1 || blk.list |> length != 1
            || !(blk.list[0] is ExprReturn)) return null
    var ret = blk.list[0] as ExprReturn
    if (ret.subexpr == null || has_sideeffects(ret.subexpr)) return null
    let argName = string(blk.arguments[0].name)
    var b1 = clone_expression(ret.subexpr)
    var b2 = clone_expression(ret.subexpr)
    var r1 : Template
    r1 |> renameVariable(argName, "v1")
    var r2 : Template
    r2 |> renameVariable(argName, "v2")
    apply_template(r1, b1.at, b1)
    apply_template(r2, b2.at, b2)
    var cmpExpr : Expression?
    if (orderName == "order_by_descending") {
        cmpExpr = qmacro(_::less($e(b2), $e(b1)))
    } else {
        cmpExpr = qmacro(_::less($e(b1), $e(b2)))
    }
    // Emit untyped block args (`$(v1, v2) { ... }`); typer infers v1/v2 types from the call site (top_n_by_with_cmp / order_inplace cmp-param signatures).
    return qmacro($(v1, v2) {
        return $e(cmpExpr)
    })
}

def private make_inline_less_call(orderKey : Expression?; orderName : string;
                                  var lhsExpr, rhsExpr : Expression?; at : LineInfo) : Expression? {
    // Direct-call mirror of try_make_inline_cmp: builds `less(KEY_BODY[lhsExpr], KEY_BODY[rhsExpr])` (or flipped for descending) for inline use without a block dispatch — needed for the bounded-heap less-test where invoking a block on a 100K-element hot path costs ~5ns/op extra.
    if (orderKey == null || !(orderKey is ExprMakeBlock)) return null
    var mblk = orderKey as ExprMakeBlock
    var blk = mblk._block as ExprBlock
    if (blk.arguments |> length != 1 || blk.list |> length != 1
            || !(blk.list[0] is ExprReturn)) return null
    var ret = blk.list[0] as ExprReturn
    if (ret.subexpr == null || has_sideeffects(ret.subexpr)) return null
    let argName = string(blk.arguments[0].name)
    var b1 = clone_expression(ret.subexpr)
    var b2 = clone_expression(ret.subexpr)
    var r1 : Template
    r1 |> replaceVariable(argName, lhsExpr)
    var r2 : Template
    r2 |> replaceVariable(argName, rhsExpr)
    // apply_template returns the new root expression when the visitor replaces it (e.g. body == ExprVar(argName) → root substitution for identity keys like `_order_by(_)`). For non-root replacements (ExprField, etc.) the root pointer is unchanged. Always reassign.
    b1 = apply_template(r1, b1.at, b1)
    b2 = apply_template(r2, b2.at, b2)
    return qmacro(_::less($e(b2), $e(b1))) if (orderName == "order_by_descending")
    return qmacro(_::less($e(b1), $e(b2)))
}


[_macro]
def private register_order_family_rows {
    if (!is_compiling_macros_in_module("linq_fold")) return
    // Row 1 — streaming_min: inline-cmp + first[_or_default]; no distinct (final imperative bail).
    splice_patterns |> emplace <| SplicePattern(
        name = "order_streaming_min",
        chain <- [
            Slot(matcher = m_literal("where_"),       cardinality = c_opt(), capture_name = "where"),
            Slot(matcher = m_alias("distinct_family"), cardinality = c_opt(), capture_name = "distinct"),
            Slot(matcher = m_alias("order_family"),    cardinality = c_one(), capture_name = "order"),
            Slot(matcher = m_alias("first_family"),    cardinality = c_one(), capture_name = "term"),
            Slot(matcher = m_literal("select"),        cardinality = c_opt(), capture_name = "termsel")
        ],
        requires <- [@@ < RequiresPredicate > inline_cmp_available],
        emit = @@ < EmitFn > emit_streaming_min
    )
    // Row 2 — bounded_heap: inline-cmp + take(N); distinct gate optional; no first.
    splice_patterns |> emplace <| SplicePattern(
        name = "order_bounded_heap",
        chain <- [
            Slot(matcher = m_literal("where_"),       cardinality = c_opt(), capture_name = "where"),
            Slot(matcher = m_alias("distinct_family"), cardinality = c_opt(), capture_name = "distinct"),
            Slot(matcher = m_alias("order_family"),    cardinality = c_one(), capture_name = "order"),
            Slot(matcher = m_literal("take"),          cardinality = c_one(), capture_name = "take"),
            Slot(matcher = m_literal("select"),        cardinality = c_opt(), capture_name = "termsel")
        ],
        requires <- [@@ < RequiresPredicate > inline_cmp_available, @@ < RequiresPredicate > take_arg_is_int],
        emit = @@ < EmitFn > emit_bounded_heap
    )
    // Row 3 — order_then_plain_distinct: plain distinct (NOT distinct_by — whole-tuple equality is position-invariant; distinct_by would pick an arbitrary K1 representative).
    splice_patterns |> emplace <| SplicePattern(
        name = "order_then_plain_distinct",
        chain <- [
            Slot(matcher = m_alias("order_family"), cardinality = c_one(), capture_name = "order"),
            Slot(matcher = m_literal("distinct"),   cardinality = c_one(), capture_name = "distinct_after"),
            Slot(matcher = m_literal("take"),       cardinality = c_opt(), capture_name = "take"),
            Slot(matcher = m_alias("first_family"), cardinality = c_opt(), capture_name = "term"),
            Slot(matcher = m_literal("select"),     cardinality = c_opt(), capture_name = "termsel")
        ],
        requires <- [@@ < RequiresPredicate > take_arg_is_int],
        emit = @@ < EmitFn > emit_fused_prefilter
    )
    // Row 4 — fused_prefilter: where_ or distinct_family upstream + order_family; optional take/first/terminal_select.
    splice_patterns |> emplace <| SplicePattern(
        name = "order_fused_prefilter",
        chain <- [
            Slot(matcher = m_literal("where_"),       cardinality = c_opt(), capture_name = "where"),
            Slot(matcher = m_alias("distinct_family"), cardinality = c_opt(), capture_name = "distinct"),
            Slot(matcher = m_alias("order_family"),    cardinality = c_one(), capture_name = "order"),
            Slot(matcher = m_literal("take"),          cardinality = c_opt(), capture_name = "take"),
            Slot(matcher = m_alias("first_family"),    cardinality = c_opt(), capture_name = "term"),
            Slot(matcher = m_literal("select"),        cardinality = c_opt(), capture_name = "termsel")
        ],
        requires <- [@@ < RequiresPredicate > has_where_or_distinct, @@ < RequiresPredicate > take_arg_is_int],
        emit = @@ < EmitFn > emit_fused_prefilter
    )
    // Row 5 — buffer_helper_dispatch: bare order_family + optional take/first → direct daslib helper call (order/top_n*/min_max). array_source gates to Array only — Decs has no `top` expr, cascades to Row 4 (fused_prefilter).
    splice_patterns |> emplace <| SplicePattern(
        name = "order_buffer_helper_dispatch",
        chain <- [
            Slot(matcher = m_alias("order_family"), cardinality = c_one(), capture_name = "order"),
            Slot(matcher = m_literal("take"),       cardinality = c_opt(), capture_name = "take"),
            Slot(matcher = m_alias("first_family"), cardinality = c_opt(), capture_name = "term")
        ],
        requires <- [@@ < RequiresPredicate > take_arg_is_int, @@ < RequiresPredicate > array_source],
        emit = @@ < EmitFn > emit_buffer_helper_dispatch
    )
}

// ===== plan_order_family migration (PR B2) — 4 emit archetypes + 5 pattern rows. Capture keys: where / distinct / distinct_after / order / take / term / termsel. =====

// Shared helper — every emit archetype starts with this state extraction.
struct private OrderCaptures {
    whereCond           : Expression?
    distinctName        : string             // "" / "distinct" / "distinct_by"
    distinctKey         : Expression?        // only when distinctName == "distinct_by"
    orderName           : string             // mandatory; one of order / order_descending / order_by / order_by_descending
    orderKey            : Expression?        // only when hasKey
    orderElemType       : TypeDeclPtr
    takeExpr            : Expression?
    firstName           : string             // "" / "first" / "first_or_default"
    firstDefaultExpr    : Expression?        // only for first_or_default
    selectLam           : Expression?
    selectElemType      : TypeDeclPtr
    hasKey              : bool               // orderName == "order_by" or "order_by_descending"
    ok                  : bool               // false on extraction failure (caller bails to cascade)
}

[macro_function]
def private extract_order_captures(var c : Captures; at : LineInfo; itName : string) : OrderCaptures {
    var oc : OrderCaptures
    oc.ok = false
    if (!(c.single |> key_exists("order"))) return oc
    let orderCall = c.single["order"]
    // single_name preserves the post-normalize_order_reverse LinqCall.name; call_norm_name reads the source ExprCall.func and would lose the swap.
    oc.orderName = c.single_name["order"]
    let argCount = orderCall.arguments |> length
    // order(arr, cmp) / order_descending(arr, cmp): user-supplied comparator can't splice through min/max/top_n helpers.
    if ((oc.orderName == "order" || oc.orderName == "order_descending") && argCount >= 2) return oc
    oc.hasKey = oc.orderName == "order_by" || oc.orderName == "order_by_descending"
    if (oc.hasKey && argCount >= 2) {
        oc.orderKey = clone_expression(orderCall.arguments[1])
    }
    if (orderCall._type == null || orderCall._type.firstType == null) return oc
    oc.orderElemType = clone_type(orderCall._type.firstType)
    if (c.single |> key_exists("where")) {
        oc.whereCond = peel_lambda_rename_var(c.single["where"].arguments[1], itName)
    }
    // distinct BEFORE order (Row 4 fused_prefilter, Row 2 bounded_heap with dset gate) OR distinct AFTER order plain only (Row 5).
    if (c.single |> key_exists("distinct")) {
        let distinctCall = c.single["distinct"]
        oc.distinctName = c.single_name["distinct"]
        if (oc.distinctName == "distinct_by") {
            if ((distinctCall.arguments |> length) < 2) return oc
            oc.distinctKey = clone_expression(distinctCall.arguments[1])
        }
    } elif (c.single |> key_exists("distinct_after")) {
        oc.distinctName = "distinct"   // Row 5 is m_literal("distinct"), distinct_by structurally excluded.
    }
    if (c.single |> key_exists("take")) {
        let takeArg = c.single["take"].arguments[1]
        if (takeArg == null || takeArg._type == null || takeArg._type.baseType != Type.tInt) return oc
        oc.takeExpr = clone_expression(takeArg)
    }
    if (c.single |> key_exists("term")) {
        let termCall = c.single["term"]
        oc.firstName = c.single_name["term"]
        if (oc.firstName == "first_or_default") {
            if ((termCall.arguments |> length) < 2) return oc
            oc.firstDefaultExpr = clone_expression(termCall.arguments[1])
        }
    }
    if (c.single |> key_exists("termsel")) {
        var termselCall = c.single["termsel"]
        oc.selectLam = termselCall.arguments[1]
        if (oc.selectLam == null || termselCall._type == null || termselCall._type.firstType == null) return oc
        oc.selectElemType = clone_type(termselCall._type.firstType)
    }
    oc.ok = true
    return oc
}

// emit_streaming_min — first[_or_default] with inline-cmp key. Per-walk single-best state, no buffer.
[macro_function]
def private emit_streaming_min(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
    let bindName = adapter_bind_name(ctx.src, at)
    var oc <- extract_order_captures(c, at, bindName)
    // Distinct without take but with first bails — streaming-min has no dset hook (mirrors imperative final bail).
    if (!oc.ok || oc.firstName == "" || oc.distinctName != "") return null
    var inlineCmp = oc.hasKey ? try_make_inline_cmp(oc.orderKey, oc.orderName, oc.orderElemType, at) : null
    if (inlineCmp == null) return null
    let bestName = qn("order_best", at)
    let seenName = qn("order_seen", at)
    var elemType = clone_type(oc.orderElemType)
    var lessTest = make_inline_less_call(oc.orderKey, oc.orderName,
        qmacro($i(bindName)), qmacro($i(bestName)), at)
    var perElement : Expression? = qmacro_expr() {
        if (!$i(seenName)) {
            $i(bestName) := $i(bindName)
            $i(seenName) = true
        } elif ($e(lessTest)) {
            $i(bestName) := $i(bindName)
        }
    }
    if (oc.whereCond != null) {
        perElement = qmacro_expr() {
            if ($e(oc.whereCond)) {
                $e(perElement)
            }
        }
    }
    let outElemType = (oc.selectLam != null) ? oc.selectElemType : elemType
    var stmts : array<Expression?>
    if (oc.firstName == "first") {
        var firstRetExpr : Expression?
        if (oc.selectLam != null) {
            firstRetExpr = peel_lambda_replace_var(oc.selectLam, qmacro($i(bestName)))
        } else {
            firstRetExpr = qmacro($i(bestName))
        }
        stmts |> push_from <| qmacro_block_to_array() {
            var $i(bestName) = default<$t(elemType)>
            var $i(seenName) = false
        }
        stmts |> push <| adapter_wrap_source_loop(ctx.src, perElement, at)
        stmts |> push_from <| qmacro_block_to_array() {
            panic("sequence contains no elements") if (!$i(seenName))
            return $e(firstRetExpr)
        }
    } else {
        let dBindName = qn("order_d", at)
        var bestRetExpr : Expression?
        var dRetExpr : Expression?
        if (oc.selectLam != null) {
            bestRetExpr = peel_lambda_replace_var(oc.selectLam, qmacro($i(bestName)))
            dRetExpr = peel_lambda_replace_var(oc.selectLam, qmacro($i(dBindName)))
        } else {
            bestRetExpr = qmacro($i(bestName))
            dRetExpr = qmacro($i(dBindName))
        }
        stmts |> push_from <| qmacro_block_to_array() {
            let $i(dBindName) = $e(oc.firstDefaultExpr)
            var $i(bestName) = default<$t(elemType)>
            var $i(seenName) = false
        }
        stmts |> push <| adapter_wrap_source_loop(ctx.src, perElement, at)
        stmts |> push_from <| qmacro_block_to_array() {
            return $e(bestRetExpr) if ($i(seenName))
            return $e(dRetExpr)
        }
    }
    return adapter_wrap_invoke(ctx.src, stmts, outElemType, false, at)
}

// emit_bounded_heap — take(N) with inline-cmp key. Heap of size N during walk; distinct gate variant + terminal _select.
[macro_function]
def private emit_bounded_heap(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
    let bindName = adapter_bind_name(ctx.src, at)
    var oc <- extract_order_captures(c, at, bindName)
    if (!oc.ok || oc.takeExpr == null || oc.firstName != "") return null
    var inlineCmp = oc.hasKey ? try_make_inline_cmp(oc.orderKey, oc.orderName, oc.orderElemType, at) : null
    if (inlineCmp == null) return null
    let takeNName = qn("order_take_n", at)
    let bhBufName = qn("order_buf", at)
    let dsetName = qn("order_dset", at)
    let dkeyName = qn("order_dkey", at)
    var bufElemType = clone_type(oc.orderElemType)
    var lessTest = make_inline_less_call(oc.orderKey, oc.orderName,
        qmacro($i(bindName)), qmacro($i(bhBufName)[0]), at)
    var perElement : Expression? = qmacro_expr() {
        if (length($i(bhBufName)) < $i(takeNName)) {
            $i(bhBufName) |> push_clone($i(bindName))
            _::spliced_push_heap($i(bhBufName), $e(inlineCmp))
        } elif ($e(lessTest)) {
            _::spliced_pop_heap($i(bhBufName), $e(inlineCmp))
            $i(bhBufName)[length($i(bhBufName)) - 1] := $i(bindName)
            _::spliced_push_heap($i(bhBufName), $e(inlineCmp))
        }
    }
    // Theme 3 Phase 3: leading distinct[_by] gates per-element push by set-insert on the distinct key.
    if (oc.distinctName != "") {
        var dkeyExpr : Expression?
        if (oc.distinctName == "distinct_by") {
            dkeyExpr = peel_lambda_rename_var(oc.distinctKey, bindName)
            if (dkeyExpr == null) return null
        } else {
            dkeyExpr = qmacro($i(bindName))
        }
        perElement = qmacro_block() {
            let $i(dkeyName) = _::unique_key($e(dkeyExpr))
            if (!$i(dsetName) |> key_exists($i(dkeyName))) {
                $i(dsetName) |> insert($i(dkeyName))
                $e(perElement)
            }
        }
    }
    if (oc.whereCond != null) {
        perElement = qmacro_expr() {
            if ($e(oc.whereCond)) {
                $e(perElement)
            }
        }
    }
    var stmts : array<Expression?>
    stmts |> push_from <| qmacro_block_to_array() {
        let $i(takeNName) = $e(oc.takeExpr)
        var $i(bhBufName) : array<$t(bufElemType)>
    }
    if (oc.distinctName != "") {
        if (oc.distinctName == "distinct_by") {
            stmts |> push <| qmacro_expr() {
                var inscope $i(dsetName) : table<typedecl(_::unique_key(invoke($e(oc.distinctKey), default<$t(bufElemType)>)))>
            }
        } else {
            stmts |> push <| qmacro_expr() {
                var inscope $i(dsetName) : table<typedecl(_::unique_key(default<$t(bufElemType)>))>
            }
        }
    }
    if (oc.selectLam != null) {
        let outBufName = qn("order_proj_buf", at)
        let elemName = qn("order_proj_e", at)
        var projBody = peel_lambda_replace_var(oc.selectLam, qmacro($i(elemName)))
        stmts |> push_from <| qmacro_block_to_array() {
            var $i(outBufName) : array<$t(oc.selectElemType)>
            return <- $i(outBufName) if ($i(takeNName) <= 0)
        }
        stmts |> push <| adapter_wrap_source_loop(ctx.src, perElement, at)
        stmts |> push_from <| qmacro_block_to_array() {
            _::order_inplace($i(bhBufName), $e(inlineCmp))
            $i(outBufName) |> reserve(length($i(bhBufName)))
            for ($i(elemName) in $i(bhBufName)) {
                $i(outBufName) |> push_clone($e(projBody))
            }
            return <- $i(outBufName)
        }
    } else {
        stmts |> push_from <| qmacro_block_to_array() {
            return <- $i(bhBufName) if ($i(takeNName) <= 0)
        }
        stmts |> push <| adapter_wrap_source_loop(ctx.src, perElement, at)
        stmts |> push_from <| qmacro_block_to_array() {
            _::order_inplace($i(bhBufName), $e(inlineCmp))
            return <- $i(bhBufName)
        }
    }
    return adapter_wrap_invoke(ctx.src, stmts, null, ctx.expr_is_iterator, at)
}

// emit_buffer_helper_dispatch — no where, no distinct. Direct call to daslib helpers (order / top_n* / min_max).
[macro_function]
def private emit_buffer_helper_dispatch(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
    let itName = qn("it", at)
    var oc <- extract_order_captures(c, at, itName)
    // Buffer-helper is the no-prefilter, no-terminal-_select path; anything richer routes to fused_prefilter or streaming/heap.
    if (!oc.ok || oc.whereCond != null || oc.distinctName != "" || oc.selectLam != null) return null
    var top = ctx.top
    var topExpr = clone_expression(top)
    topExpr.genFlags.alwaysSafe = true
    var inlineCmp = oc.hasKey ? try_make_inline_cmp(oc.orderKey, oc.orderName, oc.orderElemType, at) : null
    let topNName = order_top_n_call_name(oc.orderName)
    let minMaxName = order_min_call_name(oc.orderName, oc.hasKey)
    let needIterWrap = ctx.expr_is_iterator
    var emission : Expression?
    if (oc.firstName == "first") {
        // order + first → preserve eager panic-on-empty. For arrays: min/max on length-guarded src. For iterators: top_n*(_, 1) |> first().
        if (top._type.isGoodArrayType) {
            var srcParamType = invoke_src_param_type(top)
            let firstSrcName = qn("first_src", at)
            var minMaxCall : Expression?
            if (oc.hasKey) {
                minMaxCall = qmacro($c(minMaxName)($i(firstSrcName), $e(oc.orderKey)))
            } else {
                minMaxCall = qmacro($c(minMaxName)($i(firstSrcName)))
            }
            emission = qmacro(invoke($($i(firstSrcName) : $t(srcParamType)) {
                panic("sequence contains no elements") if (empty($i(firstSrcName)))
                return $e(minMaxCall)
            }, $e(topExpr)))
            emission = finalize_invoke(emission, at)
        } else {
            var topNCall : Expression?
            if (inlineCmp != null) {
                topNCall = qmacro(_::top_n_by_with_cmp($e(topExpr), 1, $e(inlineCmp)))
            } elif (oc.hasKey) {
                topNCall = qmacro($c(topNName)($e(topExpr), 1, $e(oc.orderKey)))
            } else {
                topNCall = qmacro($c(topNName)($e(topExpr), 1))
            }
            emission = qmacro(_::first($e(topNCall)))
        }
    } elif (oc.firstName == "first_or_default") {
        // No min_by_or_default; route through top_n*(_, 1) + first_or_default (returns array, then default if empty).
        var topNCall : Expression?
        if (inlineCmp != null) {
            topNCall = qmacro(_::top_n_by_with_cmp($e(topExpr), 1, $e(inlineCmp)))
        } elif (oc.hasKey) {
            topNCall = qmacro($c(topNName)($e(topExpr), 1, $e(oc.orderKey)))
        } else {
            topNCall = qmacro($c(topNName)($e(topExpr), 1))
        }
        emission = qmacro(_::first_or_default($e(topNCall), $e(oc.firstDefaultExpr)))
    } elif (oc.takeExpr == null) {
        // Bare order — direct call.
        if (inlineCmp != null) {
            emission = qmacro(_::order($e(topExpr), $e(inlineCmp)))
        } elif (oc.hasKey) {
            emission = qmacro($c(oc.orderName)($e(topExpr), $e(oc.orderKey)))
        } else {
            emission = qmacro($c(oc.orderName)($e(topExpr)))
        }
    } else {
        // order + take → top_n*.
        if (inlineCmp != null) {
            emission = qmacro(_::top_n_by_with_cmp($e(topExpr), $e(oc.takeExpr), $e(inlineCmp)))
        } elif (oc.hasKey) {
            emission = qmacro($c(topNName)($e(topExpr), $e(oc.takeExpr), $e(oc.orderKey)))
        } else {
            emission = qmacro($c(topNName)($e(topExpr), $e(oc.takeExpr)))
        }
    }
    // Wrap with to_sequence_move only when emission is array-shaped: take dispatches array; bare order/min on arrays returns array; min on iterator returns scalar (no wrap).
    let emissionIsArray = oc.takeExpr != null || (oc.firstName == "" && top._type.isGoodArrayType)
    if (needIterWrap && emissionIsArray) {
        emission = qmacro($e(emission).to_sequence_move())
    }
    emission.force_generated(true)
    return emission
}

// emit_fused_prefilter — where+order or distinct+order. Single fused loop: filter (and dset gate) into fresh buf, then sort/min/top_n on buf.
[macro_function]
def private emit_fused_prefilter(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
    let bindName = adapter_bind_name(ctx.src, at)
    var oc <- extract_order_captures(c, at, bindName)
    // Fused-prefilter requires where OR distinct upstream (buffer_helper covers the bare case); distinct+first without take bails (no per-element min-state).
    if (!oc.ok
            || (oc.whereCond == null && oc.distinctName == "")
            || (oc.distinctName != "" && oc.takeExpr == null && oc.firstName != "")) return null
    let bufName = qn("buf", at)
    let dsetName = qn("order_dset", at)
    let dkeyName = qn("order_dkey", at)
    var bufElemType = clone_type(oc.orderElemType)
    var inlineCmp = oc.hasKey ? try_make_inline_cmp(oc.orderKey, oc.orderName, oc.orderElemType, at) : null
    let topNName = order_top_n_call_name(oc.orderName)
    let inplaceName = "{oc.orderName}_inplace"
    let minMaxName = order_min_call_name(oc.orderName, oc.hasKey)
    var pushStmt : Expression? = qmacro_expr() {
        $i(bufName) |> push_clone($i(bindName))
    }
    // Theme 8 (audit 3b): distinct gate per-element push by set-insert on key. Single source pass, no full distinct materialization.
    if (oc.distinctName != "") {
        var dkeyExpr : Expression?
        if (oc.distinctName == "distinct_by") {
            dkeyExpr = peel_lambda_rename_var(oc.distinctKey, bindName)
            if (dkeyExpr == null) return null
        } else {
            dkeyExpr = qmacro($i(bindName))
        }
        pushStmt = qmacro_block() {
            let $i(dkeyName) = _::unique_key($e(dkeyExpr))
            if (!$i(dsetName) |> key_exists($i(dkeyName))) {
                $i(dsetName) |> insert($i(dkeyName))
                $e(pushStmt)
            }
        }
    }
    var loopBody : Expression?
    if (oc.whereCond != null) {
        loopBody = qmacro_expr() {
            if ($e(oc.whereCond)) {
                $e(pushStmt)
            }
        }
    } else {
        loopBody = pushStmt
    }
    var stmts : array<Expression?>
    stmts |> push <| qmacro_expr() {
        var $i(bufName) : array<$t(bufElemType)>
    }
    if (oc.distinctName != "") {
        if (oc.distinctName == "distinct_by") {
            stmts |> push <| qmacro_expr() {
                var inscope $i(dsetName) : table<typedecl(_::unique_key(invoke($e(oc.distinctKey), default<$t(bufElemType)>)))>
            }
        } else {
            stmts |> push <| qmacro_expr() {
                var inscope $i(dsetName) : table<typedecl(_::unique_key(default<$t(bufElemType)>))>
            }
        }
    }
    // Reserve hint: array source with no distinct gate has known length. Decs has no random-access length — skip.
    if (ctx.src is Array && oc.distinctName == "") {
        let top = (ctx.src as Array)._0
        let srcName = (ctx.src as Array)._1
        if (top != null && top._type != null && type_has_length(top._type)) {
            stmts |> push <| qmacro_expr() {
                $i(bufName) |> reserve(length($i(srcName)))
            }
        }
    }
    stmts |> push <| adapter_wrap_source_loop(ctx.src, loopBody, at)
    let elemName = qn("order_proj_e", at)
    let outBufName = qn("order_proj_buf", at)
    var projBody : Expression?
    if (oc.selectLam != null) {
        projBody = peel_lambda_replace_var(oc.selectLam, qmacro($i(elemName)))
    }
    if (oc.firstName == "first") {
        // where + order + first → min/max on prefilter buffer. Empty buf must panic to match eager first(); min/max return uninitialized refs on empty.
        stmts |> push <| qmacro_expr() {
            panic("sequence contains no elements") if (empty($i(bufName)))
        }
        var minMaxCall : Expression?
        if (oc.hasKey) {
            minMaxCall = qmacro($c(minMaxName)($i(bufName), $e(oc.orderKey)))
        } else {
            minMaxCall = qmacro($c(minMaxName)($i(bufName)))
        }
        if (oc.selectLam != null) {
            stmts |> push_from <| qmacro_block_to_array() {
                let $i(elemName) = $e(minMaxCall)
                return $e(projBody)
            }
        } else {
            stmts |> push <| qmacro_expr() {
                return $e(minMaxCall)
            }
        }
    } elif (oc.firstName == "first_or_default") {
        var topNCall : Expression?
        if (inlineCmp != null) {
            topNCall = qmacro(_::top_n_by_with_cmp($i(bufName), 1, $e(inlineCmp)))
        } elif (oc.hasKey) {
            topNCall = qmacro($c(topNName)($i(bufName), 1, $e(oc.orderKey)))
        } else {
            topNCall = qmacro($c(topNName)($i(bufName), 1))
        }
        if (oc.selectLam != null) {
            let dBindName = qn("order_d", at)
            stmts |> push_from <| qmacro_block_to_array() {
                let $i(dBindName) = $e(oc.firstDefaultExpr)
                let $i(elemName) = _::first_or_default($e(topNCall), $i(dBindName))
                return $e(projBody)
            }
        } else {
            stmts |> push <| qmacro_expr() {
                return _::first_or_default($e(topNCall), $e(oc.firstDefaultExpr))
            }
        }
    } elif (oc.takeExpr == null) {
        var sortCall : Expression?
        if (inlineCmp != null) {
            sortCall = qmacro(_::order_inplace($i(bufName), $e(inlineCmp)))
        } elif (oc.hasKey) {
            sortCall = qmacro($c(inplaceName)($i(bufName), $e(oc.orderKey)))
        } else {
            sortCall = qmacro($c(inplaceName)($i(bufName)))
        }
        stmts |> push(sortCall)
        if (oc.selectLam != null) {
            stmts |> push_from <| qmacro_block_to_array() {
                var $i(outBufName) : array<$t(oc.selectElemType)>
                $i(outBufName) |> reserve(length($i(bufName)))
                for ($i(elemName) in $i(bufName)) {
                    $i(outBufName) |> push_clone($e(projBody))
                }
                return <- $i(outBufName)
            }
        } else {
            stmts |> push <| qmacro_expr() {
                return <- $i(bufName)
            }
        }
    } else {
        var topNCall : Expression?
        if (inlineCmp != null) {
            topNCall = qmacro(_::top_n_by_with_cmp($i(bufName), $e(oc.takeExpr), $e(inlineCmp)))
        } elif (oc.hasKey) {
            topNCall = qmacro($c(topNName)($i(bufName), $e(oc.takeExpr), $e(oc.orderKey)))
        } else {
            topNCall = qmacro($c(topNName)($i(bufName), $e(oc.takeExpr)))
        }
        if (oc.selectLam != null) {
            let topResName = qn("order_top_res", at)
            stmts |> push_from <| qmacro_block_to_array() {
                var $i(topResName) <- $e(topNCall)
                var $i(outBufName) : array<$t(oc.selectElemType)>
                $i(outBufName) |> reserve(length($i(topResName)))
                for ($i(elemName) in $i(topResName)) {
                    $i(outBufName) |> push_clone($e(projBody))
                }
                return <- $i(outBufName)
            }
        } else {
            stmts |> push <| qmacro_expr() {
                return <- $e(topNCall)
            }
        }
    }
    return adapter_wrap_invoke(ctx.src, stmts, null, ctx.expr_is_iterator, at)
}

// ===== plan_loop_or_count migration (PR B) =====
// Single emit + single pattern row. c_chain captures where_/select head; canonical-order slots carry the rest.

// PR C Decs-arm: reconstructs a flatten_linq-shaped calls array from captures, runs extract_decs_ranges +
// compute_decs_chain_info (the same helpers plan_decs_unroll imperative uses), then dispatches to the
// existing emit_decs_* lane fns. State-hoist-above-for_each_archetype shape stays per-adapter (D1).
[macro_function]
def private emit_loop_or_count_lane_decs(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
    let bridge = (ctx.src as Decs)._0
    let tupName = (ctx.src as Decs)._1
    // post_take_where (Theme 2 5c on Array) has no decs lane fn yet; cascade so the chain doesn't silently lose the predicate.
    if (c.single |> key_exists("post_take_where")) return null
    let hasTerm = c.single |> key_exists("term")
    var termCall : ExprCall?
    var termName : string
    if (hasTerm) {
        termCall = c.single["term"]
        termName = c.single_name["term"]
    }
    // D5 fast path: bare `count()` (1-arg, no chain) → arch.size sum, no entity walk.
    let noRangeOps = (!(c.single |> key_exists("skip")) && !(c.single |> key_exists("skip_while"))
        && !(c.single |> key_exists("take_while")) && !(c.single |> key_exists("take")))
    if (hasTerm && termName == "count" && length(termCall.arguments) == 1
            && (c.many["head"] |> empty) && noRangeOps) return emit_decs_count_archsize(bridge, at)
    // Reconstruct calls = head + range ops (canonical order) + term, so existing helpers (compute_decs_chain_info, extract_decs_ranges) work unchanged. LinqCall records come from the linqCalls registry by call_norm_name; addr() to convert ref→pointer matches flatten_linq's pattern.
    var calls : array<tuple<ExprCall?; LinqCall?>>
    calls |> reserve(length(c.many["head"]) + 5)        // head + up to 4 range slots + term
    for (call in c.many["head"]) {
        let nm = call_norm_name(call)
        if (!(linqCalls |> key_exists(nm))) return null
        var lc & = unsafe(linqCalls?[nm] ?? default<LinqCall>)
        calls |> push((call, unsafe(addr(lc))))
    }
    let rangeSlots = ["skip", "skip_while", "take_while", "take"]
    for (sn in rangeSlots) {
        if (c.single |> key_exists(sn)) {
            let scall = c.single[sn]
            let snm = call_norm_name(scall)
            if (!(linqCalls |> key_exists(snm))) return null
            var lc & = unsafe(linqCalls?[snm] ?? default<LinqCall>)
            calls |> push((scall, unsafe(addr(lc))))
        }
    }
    if (hasTerm) {
        if (!(linqCalls |> key_exists(termName))) return null
        var lc & = unsafe(linqCalls?[termName] ?? default<LinqCall>)
        calls |> push((termCall, unsafe(addr(lc))))
    }
    let intermediateEnd = hasTerm ? length(calls) - 1 : length(calls)
    let rangeInfo = extract_decs_ranges(calls, intermediateEnd, tupName, at)
    if (rangeInfo == null) return null
    let chainInfo = compute_decs_chain_info(calls, rangeInfo.chainEnd, tupName, bridge, at)
    if (chainInfo == null) return null
    // Terminator classification mirrors plan_decs_unroll imperative (linq_fold.das:5781-5791); differs from classify_terminator's 4-lane split because decs has dedicated min_max_by / walk / element_at emit fns with hoisted state.
    let isAccum = (termName == "count" || termName == "long_count" || termName == "sum"
        || termName == "min" || termName == "max" || termName == "average")
    let isEarlyExit = (termName == "first" || termName == "first_or_default"
        || termName == "any" || termName == "all" || termName == "contains")
    let isMinMaxBy = termName == "min_by" || termName == "max_by"
    let isWalk = (termName == "last" || termName == "last_or_default"
        || termName == "single" || termName == "single_or_default"
        || termName == "aggregate")
    let isElementAt = termName == "element_at" || termName == "element_at_or_default"
    if (isAccum) {
        var accType : TypeDeclPtr
        if (termName == "sum" || termName == "min" || termName == "max" || termName == "average") {
            if (chainInfo.selectCount == 0 || chainInfo.finalType == null) return null
            accType = clone_type(chainInfo.finalType)
        }
        return emit_decs_accumulator(bridge, termName, chainInfo, rangeInfo, calls, rangeInfo.chainEnd, termCall, accType, at)
    }
    if (isEarlyExit) return emit_decs_early_exit(bridge, termName, chainInfo, rangeInfo, calls, rangeInfo.chainEnd, termCall, at)
    if (isMinMaxBy) return emit_decs_min_max_by(bridge, termName, chainInfo, rangeInfo, calls, rangeInfo.chainEnd, termCall, at)
    if (isWalk) return emit_decs_walk_lane(bridge, termName, chainInfo, rangeInfo, calls, rangeInfo.chainEnd, termCall, at)
    if (isElementAt) return emit_decs_element_at(bridge, termName, chainInfo, rangeInfo, calls, rangeInfo.chainEnd, termCall, at)
    // Implicit to_array: only when outer expr was array-typed (caller wrote `.to_array()`, peeled by linqCalls skip=true). Iterator-typed chains cascade to tier-2 here, matches imperative plan_decs_unroll line 5813.
    if (ctx.expr_is_iterator) return null
    return emit_decs_to_array(bridge, chainInfo, rangeInfo, calls, rangeInfo.chainEnd, at)
}

[macro_function]
def private emit_loop_or_count_lane(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
    // PR C Decs-arm: route to existing emit_decs_* lane fns. State hoists above for_each_archetype (different invoke shape from array side); unifying lane fns is out of scope per D1.
    if (ctx.src is Decs) return emit_loop_or_count_lane_decs(c, ctx, at)
    var top = ctx.top
    let srcName = (ctx.src as Array)._1
    let itName = qn("it", at)
    let accName = qn("acc", at)
    let names <- make_range_names(at)
    let hasTerminator = c.single |> key_exists("term")
    var lastName : string
    if (hasTerminator) {
        lastName = call_norm_name(c.single["term"])
    }
    let lane = hasTerminator ? classify_terminator(lastName) : LinqLane.ARRAY
    if (lane == LinqLane.UNKNOWN) return null
    let counterLane = lane == LinqLane.COUNTER
    if (top._type == null || top._type.firstType == null) return null
    var whereCond : Expression?
    var postTakeWhereCond : Expression?
    var projection : Expression?
    var intermediateBinds : array<Expression?>
    var preCondStmts : array<Expression?>
    var seenSelect = false
    var allProjectionsPure = true
    var elementType = clone_type(top._type.firstType)
    var lastBindName = itName
    for (call in c.many["head"]) {
        let opName = call_norm_name(call)
        if (opName == "where_") {
            var predicate : Expression?
            if (seenSelect) {
                // where-after-select: bind the running projection to a typed local so the
                if (has_sideeffects(projection)) return null
                if (lane != LinqLane.COUNTER) {
                    let wbName = "`vw`{at.line}`{at.column}`{length(preCondStmts)}"
                    var projType = clone_type(elementType)
                    preCondStmts |> push <| qmacro_expr() {
                        var $i(wbName) : $t(projType) := $e(projection)
                    }
                    var pvar = new ExprVar(at = at, name := wbName)
                    pvar._type = clone_type(elementType)
                    pvar._type.flags.ref = true
                    projection = pvar
                }
                predicate = peel_lambda_replace_var(call.arguments[1], projection)
            } else {
                predicate = peel_lambda_rename_var(call.arguments[1], itName)
            }
            if (whereCond == null) {
                whereCond = predicate
            } else {
                whereCond = qmacro($e(whereCond) && $e(predicate))
            }
        } elif (opName == "select") {
            if (projection != null) {
                if (has_sideeffects(projection)) {
                    allProjectionsPure = false
                }
                let bindName = "`v`{at.line}`{at.column}`{length(intermediateBinds)}"
                intermediateBinds |> push <| qmacro_expr() {
                    var $i(bindName) := $e(projection)
                }
                lastBindName = bindName
            }
            projection = peel_lambda_rename_var(call.arguments[1], lastBindName)
            elementType = clone_type(call._type.firstType)
            seenSelect = true
        } else {
            return null
        }
    }
    if (projection != null && has_sideeffects(projection)) {
        allProjectionsPure = false
    }
    var skipExpr : Expression?
    var skipWhileCond : Expression?
    var takeWhileCond : Expression?
    var takeExpr : Expression?
    if (c.single |> key_exists("skip")) {
        var skipArg = c.single["skip"].arguments[1]
        if (skipArg == null || skipArg._type == null || skipArg._type.baseType != Type.tInt) return null
        skipExpr = clone_expression(skipArg)
    }
    if (c.single |> key_exists("skip_while")) {
        if (seenSelect) return null
        var swArg = c.single["skip_while"].arguments[1]
        if (swArg == null) return null
        skipWhileCond = peel_lambda_rename_var(swArg, itName)
        if (skipWhileCond == null) return null
    }
    if (c.single |> key_exists("take_while")) {
        if (seenSelect) return null
        var twArg = c.single["take_while"].arguments[1]
        if (twArg == null) return null
        takeWhileCond = peel_lambda_rename_var(twArg, itName)
        if (takeWhileCond == null) return null
    }
    if (c.single |> key_exists("take")) {
        var takeArg = c.single["take"].arguments[1]
        if (takeArg == null || takeArg._type == null || takeArg._type.baseType != Type.tInt) return null
        takeExpr = clone_expression(takeArg)
    }
    if (c.single |> key_exists("post_take_where")) {
        var ptwArg = c.single["post_take_where"].arguments[1]
        if (ptwArg == null) return null
        if (seenSelect) {
            if (has_sideeffects(projection)) return null
            if (lane != LinqLane.COUNTER) {
                let wbName = "`vw`{at.line}`{at.column}`{length(preCondStmts)}"
                var projType = clone_type(elementType)
                preCondStmts |> push <| qmacro_expr() {
                    var $i(wbName) : $t(projType) := $e(projection)
                }
                var pvar = new ExprVar(at = at, name := wbName)
                pvar._type = clone_type(elementType)
                pvar._type.flags.ref = true
                projection = pvar
            }
            postTakeWhereCond = peel_lambda_replace_var(ptwArg, projection)
        } else {
            postTakeWhereCond = peel_lambda_rename_var(ptwArg, itName)
        }
    }
    let noLimits = skipExpr == null && takeExpr == null && skipWhileCond == null && takeWhileCond == null
    let isCountShaped = (lane == LinqLane.COUNTER
        || (lane == LinqLane.ACCUMULATOR && lastName == "long_count"))
    if (isCountShaped && whereCond == null && allProjectionsPure && noLimits
            && type_has_length(top._type))
        return emit_length_shortcut(lastName, top, srcName, at)
    if (lane == LinqLane.ACCUMULATOR) {
        var laneTops : array<Expression?>
        laneTops |> push(top)
        var laneSrcs : array<string>
        laneSrcs |> push(srcName)
        return emit_accumulator_lane(lastName, laneTops, projection, whereCond, postTakeWhereCond,
            intermediateBinds, preCondStmts, elementType, laneSrcs, accName, itName, names,
            skipExpr, takeExpr, skipWhileCond, takeWhileCond, at)
    }
    if (lane == LinqLane.EARLY_EXIT) {
        let terminatorCall = c.single["term"]
        let isAnyNoPred = lastName == "any" && length(terminatorCall.arguments) == 1
        if (isAnyNoPred && whereCond == null && allProjectionsPure && noLimits
                && type_has_length(top._type))
            return emit_any_empty_shortcut(top, srcName, at)
        var laneTops : array<Expression?>
        laneTops |> push(top)
        var laneSrcs : array<string>
        laneSrcs |> push(srcName)
        return emit_early_exit_lane(lastName, laneTops, projection, whereCond, postTakeWhereCond,
            intermediateBinds, preCondStmts, elementType, terminatorCall, laneSrcs, itName, names,
            skipExpr, takeExpr, skipWhileCond, takeWhileCond, at)
    }
    var loopBody : Expression?
    if (counterLane) {
        var stmts : array<Expression?>
        if (projection != null && has_sideeffects(projection)) {
            let finalBindName = qn("vfinal", at)
            stmts |> push <| qmacro_expr() {
                var $i(finalBindName) = $e(projection)
            }
        }
        var incExpr = qmacro_expr() {
            $i(accName) ++
        }
        stmts |> push(wrap_with_condition(incExpr, postTakeWhereCond))
        prepend_binds(stmts, intermediateBinds)
        wrap_with_ranges(stmts, skipExpr, takeExpr, skipWhileCond, takeWhileCond, names)
        loopBody = prepend_precond(wrap_with_condition(stmts_to_expr(stmts), whereCond), preCondStmts)
    } else {
        var stmts : array<Expression?>
        var pushExpr : Expression?
        if (projection != null) {
            pushExpr = qmacro_expr() {
                $i(accName) |> push_clone($e(projection))
            }
        } elif (whereCond != null || postTakeWhereCond != null || skipExpr != null || takeExpr != null
                || skipWhileCond != null || takeWhileCond != null) {
            pushExpr = qmacro_expr() {
                $i(accName) |> push_clone($i(itName))
            }
        } else {
            return null
        }
        stmts |> push(wrap_with_condition(pushExpr, postTakeWhereCond))
        prepend_binds(stmts, intermediateBinds)
        wrap_with_ranges(stmts, skipExpr, takeExpr, skipWhileCond, takeWhileCond, names)
        loopBody = prepend_precond(wrap_with_condition(stmts_to_expr(stmts), whereCond), preCondStmts)
    }
    if (counterLane) {
        return emit_counter_lane(top, srcName, accName, itName, names,
            skipExpr, takeExpr, skipWhileCond, loopBody, at)
    } else {
        return emit_array_lane(top, ctx.expr_is_iterator, loopBody, elementType, srcName, accName, itName,
            names, skipExpr, takeExpr, skipWhileCond, at)
    }
}

[_macro]
def private register_loop_or_count_rows {
    if (!is_compiling_macros_in_module("linq_fold")) return
    // Single row — c_chain head matches the where_/select run (0+ contiguous calls), then canonical-order range ops, optional post-take where, optional terminator from loop_terminator_family.
    splice_patterns |> emplace <| SplicePattern(
        name = "loop_or_count_general",
        chain <- [
            slot_chain_of(["where_", "select"], "head"),
            Slot(matcher = m_literal("skip"),       cardinality = c_opt(), capture_name = "skip"),
            Slot(matcher = m_literal("skip_while"), cardinality = c_opt(), capture_name = "skip_while"),
            Slot(matcher = m_literal("take_while"), cardinality = c_opt(), capture_name = "take_while"),
            Slot(matcher = m_literal("take"),       cardinality = c_opt(), capture_name = "take"),
            Slot(matcher = m_literal("where_"),     cardinality = c_opt(), capture_name = "post_take_where"),
            Slot(matcher = m_alias("loop_terminator_family"), cardinality = c_opt(), capture_name = "term")
        ],
        requires <- array<RequiresPredicate>(),
        emit = @@ < EmitFn > emit_loop_or_count_lane
    )
}

// ===== plan_reverse migration (PR A — pattern-table refactor) =====
// 5 emit archetypes + 5 pattern rows + stub. Each archetype lifted verbatim from
// the prior imperative plan_reverse branches (see git history before PR A).

// Ra — counter (reverse is identity for count). Side-effecting projection still fires per match.
[macro_function]
def private emit_reverse_counter(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
    let bindName = adapter_bind_name(ctx.src, at)
    let cntName = qn("cnt", at)
    var whereCond : Expression?
    if (c.single |> key_exists("where")) {
        whereCond = peel_lambda_rename_var(c.single["where"].arguments[1], bindName)
    }
    var projection : Expression?
    if (c.single |> key_exists("proj")) {
        projection = peel_lambda_rename_var(c.single["proj"].arguments[1], bindName)
    }
    var perElement : Expression?
    if (projection != null && has_sideeffects(projection)) {
        let vfinalName = qn("vfinal", at)
        perElement = qmacro_block() {
            var $i(vfinalName) = $e(projection)
            $i(cntName) ++
        }
    } else {
        perElement = qmacro_expr() {
            $i(cntName) ++
        }
    }
    perElement = wrap_with_condition(perElement, whereCond)
    var stmts <- qmacro_block_to_array() {
        var $i(cntName) = 0
    }
    stmts |> push <| adapter_wrap_source_loop(ctx.src, perElement, at)
    stmts |> push_from <| qmacro_block_to_array() {
        return $i(cntName)
    }
    return adapter_wrap_invoke(ctx.src, stmts, null, false, at)
}

// Rb — walk + overwrite-last scalar (terminator: first / first_or_default).
// "first of reversed" = LAST surviving element; walk source, overwrite `last` on each match. No buffer.
[macro_function]
def private emit_reverse_walk_overwrite_scalar(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
    let bindName = adapter_bind_name(ctx.src, at)
    let foundName = qn("found", at)
    let lastName = qn("last", at)
    let dBindName = qn("d", at)
    var whereCond : Expression?
    if (c.single |> key_exists("where")) {
        whereCond = peel_lambda_rename_var(c.single["where"].arguments[1], bindName)
    }
    var projection : Expression?
    if (c.single |> key_exists("proj")) {
        projection = peel_lambda_rename_var(c.single["proj"].arguments[1], bindName)
    }
    var terminalSelectLam : Expression?
    var terminalSelectElemType : TypeDeclPtr
    if (c.single |> key_exists("termsel")) {
        terminalSelectLam = c.single["termsel"].arguments[1]
        if (terminalSelectLam == null || c.single["termsel"]._type == null || c.single["termsel"]._type.firstType == null) return null
        terminalSelectElemType = clone_type(c.single["termsel"]._type.firstType)
    }
    if (!(c.single |> key_exists("term"))) return null
    let terminatorCall = c.single["term"]
    let terminatorName = call_norm_name(terminatorCall)
    // Bail to cascade if upstream typing is incomplete — `lastType` reads `projection._type` / source element type, either can be null on partially-typed exprs.
    var lastType : TypeDeclPtr
    if (projection != null) {
        if (projection._type == null) return null
        lastType = strip_const_ref(clone_type(projection._type))
    } else {
        var srcElemType = adapter_element_type(ctx.src)
        if (srcElemType == null) return null
        lastType = strip_const_ref(srcElemType)
    }
    // outElemType drives the invoke retType; terminal _select projects the survivor at return so the lane returns post-select shape.
    let outElemType = (terminalSelectLam != null) ? terminalSelectElemType : lastType
    var valueExpr : Expression?
    if (projection != null) {
        valueExpr = clone_expression(projection)
    } else {
        valueExpr = qmacro_expr() {
            $i(bindName)
        }
    }
    var matchBlock : Expression? = qmacro_block() {
        $i(lastName) := $e(valueExpr)
        $i(foundName) = true
    }
    var perElement = wrap_with_condition(matchBlock, whereCond)
    var lastRetExpr : Expression?
    if (terminalSelectLam != null) {
        lastRetExpr = peel_lambda_replace_var(terminalSelectLam, qmacro($i(lastName)))
    } else {
        lastRetExpr = qmacro($i(lastName))
    }
    // first_or_default's user default is already at post-termsel type — re-projecting through `termsel` double-applies.
    var dRetExpr : Expression? = qmacro($i(dBindName))
    var stmts : array<Expression?>
    if (terminatorName == "first_or_default") {
        stmts |> push_from <| qmacro_block_to_array() {
            let $i(dBindName) = $e(terminatorCall.arguments[1])
        }
    }
    stmts |> push_from <| qmacro_block_to_array() {
        var $i(foundName) = false
        var $i(lastName) : $t(lastType) = default<$t(lastType)>
    }
    stmts |> push <| adapter_wrap_source_loop(ctx.src, perElement, at)
    if (terminatorName == "first") {
        stmts |> push_from <| qmacro_block_to_array() {
            if (!$i(foundName)) {
                panic("sequence contains no elements")
            }
            return $e(lastRetExpr)
        }
    } else {
        stmts |> push_from <| qmacro_block_to_array() {
            return $i(foundName) ? $e(lastRetExpr) : $e(dRetExpr)
        }
    }
    return adapter_wrap_invoke(ctx.src, stmts, outElemType, false, at)
}

// R6 — backward index walk (bare reverse + take(N) + implicit to_array on array source).
// Visits only the last takeN indices — skips full-source push + O(length) reverse_inplace.
[macro_function]
def private emit_reverse_backward_index_walk(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
    var top = ctx.top
    let srcName = (ctx.src as Array)._1
    if (!(c.single |> key_exists("take")) || top._type == null || top._type.firstType == null) return null
    var bufElemType = strip_const_ref(clone_type(top._type.firstType))
    let bufName = qn("buf", at)
    let lenName = qn("rlen", at)
    let takeLimName = qn("rtakeLim", at)
    let takeNName = qn("rtn", at)
    let kName = qn("rk", at)
    var returnExpr = buffer_return(bufName, ctx.expr_is_iterator)
    // Bind `take` arg once — user expression may have side effects; matches normal call semantics.
    var body : Expression? = qmacro_block() {
        let $i(lenName) = length($i(srcName))
        let $i(takeLimName) = $e(c.single["take"].arguments[1])
        let $i(takeNName) = $i(takeLimName) <= 0 ? 0 : ($i(takeLimName) < $i(lenName) ? $i(takeLimName) : $i(lenName))
        var $i(bufName) : array<$t(bufElemType)>
        $i(bufName) |> reserve($i(takeNName))
        for ($i(kName) in 0 .. $i(takeNName)) {
            $i(bufName) |> push_clone($i(srcName)[$i(lenName) - 1 - $i(kName)])
        }
        $e(returnExpr)
    }
    var bodyStmts : array<Expression?>
    bodyStmts |> push_block_list(body)
    return finalize_emission_stmts(top, srcName, at, bodyStmts)
}

// R-2a — backward walk + dset gate (Theme 8 / audit 2a: reverse + distinct[_by] + implicit to_array).
// Single backward source walk with set-gated push — saves the cascade's reverse_to_array + distinct_by_inplace second walk.
[macro_function]
def private emit_reverse_backward_walk_dset_gate(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
    var top = ctx.top
    let srcName = (ctx.src as Array)._1
    if (!(c.single |> key_exists("dist"))) return null
    let distinctCall = c.single["dist"]
    let distinctName = call_norm_name(distinctCall)
    var distinctKey : Expression?
    if (distinctName == "distinct_by") {
        if ((distinctCall.arguments |> length) < 2) return null
        distinctKey = clone_expression(distinctCall.arguments[1])
    }
    if (top._type == null || top._type.firstType == null) return null
    var bufElemType = strip_const_ref(clone_type(top._type.firstType))
    let itName = qn("it", at)
    let bufName = qn("buf", at)
    let dkeyName = qn("rev_dkey", at)
    let dsetName = qn("rev_dset", at)
    let idxName = qn("rev_k", at)
    let rlenName = qn("rev_len", at)
    var dkeyExpr : Expression?
    if (distinctName == "distinct_by") {
        dkeyExpr = peel_lambda_rename_var(distinctKey, itName)
        if (dkeyExpr == null) return null
    } else {
        dkeyExpr = qmacro($i(itName))
    }
    var dsetDecl : Expression?
    if (distinctName == "distinct_by") {
        dsetDecl = qmacro_expr() {
            var inscope $i(dsetName) : table<typedecl(_::unique_key(invoke($e(distinctKey), default<$t(bufElemType)>)))>
        }
    } else {
        dsetDecl = qmacro_expr() {
            var inscope $i(dsetName) : table<typedecl(_::unique_key(default<$t(bufElemType)>))>
        }
    }
    var returnExpr = buffer_return(bufName, ctx.expr_is_iterator)
    var body : Expression? = qmacro_block() {
        let $i(rlenName) = length($i(srcName))
        var $i(bufName) : array<$t(bufElemType)>
        $e(dsetDecl)
        for ($i(idxName) in 0 .. $i(rlenName)) {
            let $i(itName) = $i(srcName)[$i(rlenName) - 1 - $i(idxName)]
            let $i(dkeyName) = _::unique_key($e(dkeyExpr))
            if (!$i(dsetName) |> key_exists($i(dkeyName))) {
                $i(dsetName) |> insert($i(dkeyName))
                $i(bufName) |> push_clone($i(itName))
            }
        }
        $e(returnExpr)
    }
    var bodyStmts : array<Expression?>
    bodyStmts |> push_block_list(body)
    return finalize_emission_stmts(top, srcName, at, bodyStmts)
}

// Decs `reverse |> take(N) |> to_array` skip-into-tail: 2-pass walk (arch.size sum, then for_each_archetype_find with whole-arch-skip + partial-arch skip-counter + early-exit). Preserves PR #2834's 5.2× gain.
[macro_function]
def private emit_decs_reverse_skip_into_tail(var takeExpr : Expression?; bridge : DecsBridgeShape?; tupName : string; needIterWrap : bool; at : LineInfo) : Expression? {
    let bufName = qn("buf", at)
    let takeNName = qn("take_n", at)
    let totalName = qn("decs_total", at)
    let actualName = qn("decs_actual", at)
    let skipName = qn("decs_skip", at)
    let seenName = qn("decs_seen", at)
    let skipsLeftName = qn("decs_skips", at)
    let archName = bridge.archName
    var bufElemType = strip_const_ref(clone_type(bridge.elementType))
    let tupBind = build_decs_tup_bind(bridge, tupName, at)
    var innerBody : Expression? = qmacro_block() {
        if ($i(skipsLeftName) > 0_l) {
            $i(skipsLeftName) --
            continue
        }
        $e(tupBind)
        $i(bufName) |> push_clone($i(tupName))
        if (int64(length($i(bufName))) >= $i(actualName)) {
            break
        }
    }
    var clonedForExpr = clone_expression(bridge.forExpr)
    var clonedFor = clonedForExpr as ExprFor
    var newForBody = new ExprBlock(at = at)
    newForBody.list |> push(innerBody)
    clonedFor.body = newForBody
    var emission : Expression? = qmacro(invoke($() : array<$t(bufElemType)> {
        var $i(totalName) = 0_l
        for_each_archetype($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) {
            $i(totalName) += $i(archName).size
        })
        let $i(takeNName) = $e(takeExpr)
        let $i(actualName) = ($i(takeNName) <= 0) ? 0_l : ((int64($i(takeNName)) < $i(totalName)) ? int64($i(takeNName)) : $i(totalName))
        let $i(skipName) = $i(totalName) - $i(actualName)
        var $i(bufName) : array<$t(bufElemType)>
        if ($i(actualName) == 0_l) {
            return <- $i(bufName)
        }
        $i(bufName) |> reserve(int($i(actualName)))
        var $i(seenName) = 0_l
        for_each_archetype_find($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) : bool {
            if ($i(seenName) + $i(archName).size <= $i(skipName)) {
                $i(seenName) += $i(archName).size
                return false
            }
            var $i(skipsLeftName) = ($i(skipName) > $i(seenName)) ? ($i(skipName) - $i(seenName)) : 0_l
            $e(clonedForExpr)
            $i(seenName) += $i(archName).size
            return int64(length($i(bufName))) >= $i(actualName)
        })
        _::reverse_inplace($i(bufName))
        return <- $i(bufName)
    }))
    return finalize_decs_emission(emission, at, needIterWrap)
}

// R1-R4 — catch-all buffer + reverse_inplace + optional resize + optional terminal _select.
[macro_function]
def private emit_reverse_buffer_inplace(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
    let bindName = adapter_bind_name(ctx.src, at)
    let bufName = qn("buf", at)
    let outBufName = qn("rev_proj_buf", at)
    let elemName = qn("rev_proj_e", at)
    var whereCond : Expression?
    if (c.single |> key_exists("where")) {
        whereCond = peel_lambda_rename_var(c.single["where"].arguments[1], bindName)
    }
    var projection : Expression?
    if (c.single |> key_exists("preproj")) {
        projection = peel_lambda_rename_var(c.single["preproj"].arguments[1], bindName)
    }
    var terminalSelectLam_check : Expression?
    if (c.single |> key_exists("termsel")) {
        terminalSelectLam_check = c.single["termsel"].arguments[1]
    }
    // Decs skip-into-tail fast path for `reverse |> take(N) |> to_array` (no where/select/termsel) — preserves PR #2834's 5.2× perf gain; otherwise the unified path walks all entities then resizes.
    if (ctx.src is Decs && (c.single |> key_exists("take")) && whereCond == null && projection == null && terminalSelectLam_check == null)
        return emit_decs_reverse_skip_into_tail(c.single["take"].arguments[1], (ctx.src as Decs)._0, (ctx.src as Decs)._1, ctx.expr_is_iterator, at)
    var takeExpr : Expression?
    if (c.single |> key_exists("take")) {
        takeExpr = clone_expression(c.single["take"].arguments[1])
    }
    var terminalSelectLam : Expression?
    var terminalSelectElemType : TypeDeclPtr
    if (c.single |> key_exists("termsel")) {
        terminalSelectLam = c.single["termsel"].arguments[1]
        if (terminalSelectLam == null || c.single["termsel"]._type == null || c.single["termsel"]._type.firstType == null) return null
        terminalSelectElemType = clone_type(c.single["termsel"]._type.firstType)
    }
    // Buffer element type: post-select if a pre-reverse projection narrowed the element shape; otherwise source type.
    var bufElemType : TypeDeclPtr
    if (projection != null) {
        if (projection._type == null) return null
        bufElemType = strip_const_ref(clone_type(projection._type))
    } else {
        var srcElemType = adapter_element_type(ctx.src)
        if (srcElemType == null) return null
        bufElemType = strip_const_ref(srcElemType)
    }
    var projBody : Expression?
    if (terminalSelectLam != null) {
        projBody = peel_lambda_replace_var(terminalSelectLam, qmacro($i(elemName)))
    }
    var pushExpr : Expression?
    if (projection != null) {
        pushExpr = qmacro_expr() {
            $i(bufName) |> push_clone($e(projection))
        }
    } else {
        pushExpr = qmacro_expr() {
            $i(bufName) |> push_clone($i(bindName))
        }
    }
    pushExpr = wrap_with_condition(pushExpr, whereCond)
    // Reserve hint: array source with no prefilter has known length. Decs has no random-access length — skip.
    var reserveStmts : array<Expression?>
    if (ctx.src is Array && whereCond == null) {
        let top = (ctx.src as Array)._0
        let srcName = (ctx.src as Array)._1
        if (top != null && top._type != null && type_has_length(top._type)) {
            reserveStmts |> push <| qmacro_expr() {
                $i(bufName) |> reserve(length($i(srcName)))
            }
        }
    }
    var resizeStmts : array<Expression?>
    if (takeExpr != null) {
        // Bind `take` arg once — user expression may have side effects; matches normal call semantics.
        let takeLimName = qn("rev_takeLim", at)
        resizeStmts |> push_from <| qmacro_block_to_array() {
            let $i(takeLimName) = $e(takeExpr)
            $i(bufName) |> resize($i(takeLimName) <= 0 ? 0 : ($i(takeLimName) < length($i(bufName)) ? $i(takeLimName) : length($i(bufName))))
        }
    }
    var stmts : array<Expression?>
    stmts |> push_from <| qmacro_block_to_array() {
        var $i(bufName) : array<$t(bufElemType)>
    }
    stmts |> push_from(reserveStmts)
    stmts |> push <| adapter_wrap_source_loop(ctx.src, pushExpr, at)
    stmts |> push_from <| qmacro_block_to_array() {
        _::reverse_inplace($i(bufName))
    }
    stmts |> push_from(resizeStmts)
    if (terminalSelectLam != null) {
        stmts |> push_from <| qmacro_block_to_array() {
            var $i(outBufName) : array<$t(terminalSelectElemType)>
            $i(outBufName) |> reserve(length($i(bufName)))
            for ($i(elemName) in $i(bufName)) {
                $i(outBufName) |> push_clone($e(projBody))
            }
            return <- $i(outBufName)
        }
    } else {
        stmts |> push_from <| qmacro_block_to_array() {
            return <- $i(bufName)
        }
    }
    return adapter_wrap_invoke(ctx.src, stmts, null, ctx.expr_is_iterator, at)
}

// [_macro] runs at macro-compile time and stays off the JIT runtime graph — the rows carry @@<EmitFn>
// addresses of [macro_function] emit fns whose quote() bodies the LLVM JIT can't lower.
[_macro]
def private register_reverse_rows {
    if (!is_compiling_macros_in_module("linq_fold")) return
    // R-2a (most specific) — Theme 8 / audit 2a — backward walk + dset gate
    splice_patterns |> emplace <| SplicePattern(
        name = "reverse_distinct_backward_walk",
        chain <- [
            Slot(matcher = m_literal("reverse"), cardinality = c_one()),
            Slot(matcher = m_alias("distinct_family"), cardinality = c_one(), capture_name = "dist")
        ],
        requires <- [@@ < RequiresPredicate > array_source, @@ < RequiresPredicate > no_terminator],
        emit = @@ < EmitFn > emit_reverse_backward_walk_dset_gate
    )
    // R6 — bare reverse + take + to_array, backward index walk
    splice_patterns |> emplace <| SplicePattern(
        name = "reverse_take_backward_index",
        chain <- [
            Slot(matcher = m_literal("reverse"), cardinality = c_one()),
            Slot(matcher = m_literal("take"), cardinality = c_one(), capture_name = "take")
        ],
        requires <- [@@ < RequiresPredicate > array_source, @@ < RequiresPredicate > take_arg_is_int, @@ < RequiresPredicate > no_terminator],
        emit = @@ < EmitFn > emit_reverse_backward_index_walk
    )
    // Ra — reverse + count (reverse is identity for count)
    splice_patterns |> emplace <| SplicePattern(
        name = "reverse_counter",
        chain <- [
            Slot(matcher = m_literal("where_"), cardinality = c_opt(), capture_name = "where"),
            Slot(matcher = m_literal("select"), cardinality = c_opt(), capture_name = "proj"),
            Slot(matcher = m_literal("reverse"), cardinality = c_one()),
            Slot(matcher = m_literal("count"), cardinality = c_one(), capture_name = "term", arity = 1)
        ],
        requires <- array<RequiresPredicate>(),     // Ra works on iterator and array sources (for-loop body, no indexed access)
        emit = @@ < EmitFn > emit_reverse_counter
    )
    // Rb — reverse + first[_or_default], walk-and-overwrite-last scalar; optional terminal _select
    splice_patterns |> emplace <| SplicePattern(
        name = "reverse_walk_overwrite",
        chain <- [
            Slot(matcher = m_literal("where_"), cardinality = c_opt(), capture_name = "where"),
            Slot(matcher = m_literal("select"), cardinality = c_opt(), capture_name = "proj"),
            Slot(matcher = m_literal("reverse"), cardinality = c_one()),
            Slot(matcher = m_literal("select"), cardinality = c_opt(), capture_name = "termsel"),
            Slot(matcher = m_alias("first_family"), cardinality = c_one(), capture_name = "term")
        ],
        requires <- array<RequiresPredicate>(),
        emit = @@ < EmitFn > emit_reverse_walk_overwrite_scalar
    )
    // R1-R4 — catch-all buffer + reverse_inplace + optional resize + optional terminal _select
    splice_patterns |> emplace <| SplicePattern(
        name = "reverse_buffer_inplace",
        chain <- [
            Slot(matcher = m_literal("where_"), cardinality = c_opt(), capture_name = "where"),
            Slot(matcher = m_literal("select"), cardinality = c_opt(), capture_name = "preproj"),
            Slot(matcher = m_literal("reverse"), cardinality = c_one()),
            Slot(matcher = m_literal("take"), cardinality = c_opt(), capture_name = "take"),
            Slot(matcher = m_literal("select"), cardinality = c_opt(), capture_name = "termsel")
        ],
        requires <- [@@ < RequiresPredicate > take_arg_is_int],
        emit = @@ < EmitFn > emit_reverse_buffer_inplace
    )
}

// ===== plan_distinct migration (PR A — pattern-table refactor) =====
// 1 emit archetype with internal terminator-shape dispatch + 2 pattern rows + stub.

// Hashtable dedup + per-fresh-key consume; terminator picks return wiring (count/long_count/sum/implicit to_array).
// take(N) bounds outer loop with cross-iteration break for true O(N)-source streaming exit.
[macro_function]
def private emit_hashtable_dedup(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
    let bindName = adapter_bind_name(ctx.src, at)
    let bufName = qn("buf", at)
    let seenName = qn("seen", at)
    let keyName = qn("k", at)
    let takenName = qn("taken", at)
    let accName = qn("acc", at)
    let takeLimName = qn("takeLim", at)
    let pvName = qn("pv", at)
    var whereCond : Expression?
    if (c.single |> key_exists("where")) {
        whereCond = peel_lambda_rename_var(c.single["where"].arguments[1], bindName)
    }
    var projection : Expression?
    if (c.single |> key_exists("proj")) {
        projection = peel_lambda_rename_var(c.single["proj"].arguments[1], bindName)
    }
    if (!(c.single |> key_exists("dist"))) return null
    let distinctCall = c.single["dist"]
    let isDistinctBy = call_norm_name(distinctCall) == "distinct_by"
    var distinctKeyBlock : Expression?
    if (isDistinctBy) {
        if ((distinctCall.arguments |> length) < 2) return null
        distinctKeyBlock = clone_expression(distinctCall.arguments[1])
    }
    var takeExpr : Expression?
    if (c.single |> key_exists("take")) {
        takeExpr = clone_expression(c.single["take"].arguments[1])
    }
    var terminatorName : string  = ""
    var countPred : Expression?
    if (c.single |> key_exists("term")) {
        let terminatorCall = c.single["term"]
        let termName = call_norm_name(terminatorCall)
        let termArgs = terminatorCall.arguments |> length
        // `sum` has no selector overload that interacts cleanly with distinct buffering — keep 1-arg only. count/long_count(predicate) (Theme 4): dedup runs UNCONDITIONALLY (distinct_by keeps the FIRST occurrence per key); a separate `acc` counter increments only when the predicate matches that first occurrence.
        if ((termName == "count" || termName == "sum" || termName == "long_count") && termArgs == 1) {
            terminatorName = termName
        } elif ((termName == "count" || termName == "long_count") && termArgs == 2) {
            if (terminatorCall.arguments[1] == null) return null
            countPred = clone_expression(terminatorCall.arguments[1])
            terminatorName = termName
        } else {
            return null
        }
    }
    if (takeExpr != null && terminatorName != "") return null
    var elemType : TypeDeclPtr
    if (projection != null) {
        if (projection._type == null) return null
        elemType = strip_const_ref(clone_type(projection._type))
    } else {
        var srcElemType = adapter_element_type(ctx.src)
        if (srcElemType == null) return null
        elemType = strip_const_ref(srcElemType)
    }
    var stmts : array<Expression?>
    if (isDistinctBy) {
        stmts |> push <| qmacro_expr() {
            var inscope $i(seenName) : table<typedecl(_::unique_key(invoke($e(distinctKeyBlock), default<$t(elemType)>)))>
        }
    } else {
        stmts |> push <| qmacro_expr() {
            var inscope $i(seenName) : table<typedecl(_::unique_key(default<$t(elemType)>))>
        }
    }
    let isCountTerminator = terminatorName == "count" || terminatorName == "long_count"
    let isSumTerminator = terminatorName == "sum"
    let needBuffer = !isCountTerminator && !isSumTerminator
    if (needBuffer) {
        stmts |> push <| qmacro_expr() {
            var $i(bufName) : array<$t(elemType)>
        }
    } elif (isSumTerminator) {
        stmts |> push <| qmacro_expr() {
            var $i(accName) : $t(elemType) = default<$t(elemType)>
        }
    } elif (countPred != null && terminatorName == "count") {
        stmts |> push <| qmacro_expr() {
            var $i(accName) = 0
        }
    } elif (countPred != null && terminatorName == "long_count") {
        stmts |> push <| qmacro_expr() {
            var $i(accName) = 0l
        }
    }
    if (takeExpr != null) {
        stmts |> push_from <| qmacro_block_to_array() {
            let $i(takeLimName) = $e(takeExpr)
            var $i(takenName) = 0
        }
    }
    let bindProjection = projection != null && has_sideeffects(projection)
    var pushExpr : Expression?
    if (projection != null) {
        if (bindProjection) {
            pushExpr = qmacro_expr() {
                $i(pvName)
            }
        } else {
            pushExpr = clone_expression(projection)
        }
    } else {
        pushExpr = qmacro_expr() {
            $i(bindName)
        }
    }
    var keyExpr : Expression?
    if (isDistinctBy) {
        keyExpr = peel_lambda_replace_var(distinctKeyBlock, pushExpr)
    } else {
        keyExpr = clone_expression(pushExpr)
    }
    if (countPred != null) {
        countPred = peel_lambda_replace_var(countPred, pushExpr)
        if (countPred == null) return null
    }
    var consumeStmts : array<Expression?>
    if (takeExpr != null) {
        consumeStmts |> push <| qmacro_expr() {
            $i(takenName) ++
        }
    }
    consumeStmts |> push <| qmacro_expr() {
        $i(seenName) |> insert($i(keyName))
    }
    if (needBuffer) {
        consumeStmts |> push <| qmacro_expr() {
            $i(bufName) |> push_clone($e(pushExpr))
        }
    } elif (isSumTerminator) {
        consumeStmts |> push <| qmacro_expr() {
            $i(accName) += $e(pushExpr)
        }
    }
    if (countPred != null && isCountTerminator) {
        consumeStmts |> push <| qmacro_expr() {
            if ($e(countPred)) {
                $i(accName) ++
            }
        }
    }
    var consumeBody = stmts_to_expr(consumeStmts)
    var ifNew : Expression? = qmacro_expr() {
        if (!$i(seenName) |> key_exists($i(keyName))) {
            $e(consumeBody)
        }
    }
    var perMatchStmts : array<Expression?>    // nolint:STYLE012 — array-literal-of-qmacro_expr is parse-fragile
    if (bindProjection) {
        perMatchStmts |> push <| qmacro_expr() {
            let $i(pvName) = $e(projection)
        }
    }
    perMatchStmts |> push <| qmacro_expr() {
        let $i(keyName) = _::unique_key($e(keyExpr))
    }
    perMatchStmts |> push(ifNew)
    var perElement = stmts_to_expr(perMatchStmts)
    perElement = wrap_with_condition(perElement, whereCond)
    if (takeExpr != null) {
        // take(N) early-exit shape diverges by adapter — Array uses break in for-loop; Decs uses for_each_archetype_find returning bool to stop the archetype walk (mirrors plan_decs_distinct imperative).
        if (ctx.src is Decs) {
            let bridge = (ctx.src as Decs)._0
            let tupName = (ctx.src as Decs)._1
            let archName = bridge.archName
            var innerBody : Expression? = qmacro_block() {
                break if ($i(takenName) >= $i(takeLimName))
                $e(perElement)
            }
            var prunedFor = build_decs_inner_for_pruned(bridge, tupName, innerBody, at)
            stmts |> push <| qmacro_expr() {
                for_each_archetype_find($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) : bool {
                    $e(prunedFor)
                    return $i(takenName) >= $i(takeLimName)
                })
            }
        } else {
            let srcName = (ctx.src as Array)._1
            stmts |> push <| qmacro_expr() {
                for ($i(bindName) in $i(srcName)) {
                    if ($i(takenName) >= $i(takeLimName)) {
                        break
                    }
                    $e(perElement)
                }
            }
        }
    } else {
        stmts |> push <| adapter_wrap_source_loop(ctx.src, perElement, at)
    }
    if (terminatorName == "count") {
        if (countPred != null) {
            stmts |> push <| qmacro_expr() {
                return $i(accName)
            }
        } else {
            stmts |> push <| qmacro_expr() {
                return length($i(seenName))
            }
        }
    } elif (terminatorName == "long_count") {
        if (countPred != null) {
            stmts |> push <| qmacro_expr() {
                return $i(accName)
            }
        } else {
            stmts |> push <| qmacro_expr() {
                return int64(length($i(seenName)))
            }
        }
    } elif (isSumTerminator) {
        stmts |> push <| qmacro_expr() {
            return $i(accName)
        }
    } else {
        stmts |> push_from <| qmacro_block_to_array() {
            return <- $i(bufName)
        }
    }
    let needIterWrap = ctx.expr_is_iterator && needBuffer
    return adapter_wrap_invoke(ctx.src, stmts, null, needIterWrap, at)
}

[_macro]
def private register_distinct_rows {
    if (!is_compiling_macros_in_module("linq_fold")) return
    // distinct_take — distinct[_by] |> take(N) [|> terminator]; inner-loop break on take cap
    splice_patterns |> emplace <| SplicePattern(
        name = "distinct_take",
        chain <- [
            Slot(matcher = m_literal("where_"), cardinality = c_opt(), capture_name = "where"),
            Slot(matcher = m_literal("select"), cardinality = c_opt(), capture_name = "proj"),
            Slot(matcher = m_alias("distinct_family"), cardinality = c_one(), capture_name = "dist"),
            Slot(matcher = m_literal("take"), cardinality = c_one(), capture_name = "take"),
            Slot(matcher = m_alias("distinct_terminator_family"), cardinality = c_opt(), capture_name = "term")
        ],
        requires <- [@@ < RequiresPredicate > take_arg_is_int],
        emit = @@ < EmitFn > emit_hashtable_dedup
    )
    // distinct_main — distinct[_by]; optional terminator (count / long_count / sum / implicit to_array)
    splice_patterns |> emplace <| SplicePattern(
        name = "distinct_main",
        chain <- [
            Slot(matcher = m_literal("where_"), cardinality = c_opt(), capture_name = "where"),
            Slot(matcher = m_literal("select"), cardinality = c_opt(), capture_name = "proj"),
            Slot(matcher = m_alias("distinct_family"), cardinality = c_one(), capture_name = "dist"),
            Slot(matcher = m_alias("distinct_terminator_family"), cardinality = c_opt(), capture_name = "term")
        ],
        requires <- array<RequiresPredicate>(),
        emit = @@ < EmitFn > emit_hashtable_dedup
    )
}

// ── Group-by helpers ──────────────────────────────────────────────────

[macro_function]
def private is_bucket_reducer_call(expr : Expression?; bindName : string) : tuple<bool; string; Expression?> {
    // Returns (matched, reducerName, innerLambda). innerLambda is non-null for *_inner_select forms.
    if (expr == null || !(expr is ExprCall)) return (false, "", null)
    let c = expr as ExprCall
    if (c.func == null) return (false, "", null)
    var fnName = string(c.func.name)
    if (c.func.fromGeneric != null) {
        fnName = string(c.func.fromGeneric.name)
    }
    if ((c.arguments |> length) != 1) return (false, "", null)
    // Bare reducer: `<reducer>(<bind>._1)`
    if (fnName == "length" || fnName == "count" || fnName == "long_count"
            || fnName == "sum" || fnName == "min" || fnName == "max" || fnName == "first" || fnName == "average") {
        if (peel_tuple_field_read(c.arguments[0], bindName, 1)) return (true, fnName, null)
    }
    // Inner-select reducer: `<reducer>(select(<bind>._1, <lambda>))`
    if (fnName == "sum" || fnName == "min" || fnName == "max" || fnName == "first" || fnName == "average") {
        let inner = c.arguments[0]
        if (inner != null && inner is ExprCall) {
            let innerCall = inner as ExprCall
            if (innerCall.func != null) {
                var innerName = string(innerCall.func.name)
                if (innerCall.func.fromGeneric != null) {
                    innerName = string(innerCall.func.fromGeneric.name)
                }
                if (innerName == "select"
                        && (innerCall.arguments |> length) == 2
                        && peel_tuple_field_read(innerCall.arguments[0], bindName, 1)) return (true, "{fnName}_inner_select", innerCall.arguments[1])
            }
        }
    }
    return (false, "", null)
}

struct private ReducerSpec {
    slot : int                  // target tuple slot: entry._{slot}
    name : string               // bare reducer or "<reducer>_inner_select"
    innerBody : Expression?     // peeled inner-select body, null for bare reducer
    accType : TypeDeclPtr       // type of entry._{slot} (for synth table type + workhorse check)
}

[macro_function]
def private is_average_spec(spec : ReducerSpec) : bool {
    return spec.name == "average" || spec.name == "average_inner_select"
}

[macro_function]
def private mk_avg_acc_type(at : LineInfo) : TypeDeclPtr {
    // tuple<double; uint64> — sum + count, lives in one entry slot for cache locality.
    var t = new TypeDecl(baseType = Type.tTuple, at = at)
    t.argTypes |> emplace_new <| new TypeDecl(baseType = Type.tDouble, at = at)
    t.argTypes |> emplace_new <| new TypeDecl(baseType = Type.tUInt64, at = at)
    return t
}

[macro_function]
def private slot_acc_type(bodyType : TypeDeclPtr; slot : int; isBareForm, isAverage : bool; at : LineInfo) : TypeDeclPtr {
    // average overrides the user-facing type (double) with the 2-slot tuple acc.
    if (isAverage) return mk_avg_acc_type(at)
    // Bare form: bodyType IS the reducer's return type. Named-tuple: drill into argTypes[slot].
    if (isBareForm) return strip_const_ref(clone_type(bodyType))
    if (bodyType == null || (bodyType.argTypes |> length) <= slot) return null
    return strip_const_ref(clone_type(bodyType.argTypes[slot]))
}

[macro_function]
def private recognize_reducer_specs(var groupProjBody : Expression?; bindName, itName : string) {
    // Returns (specs, usesNamedTuple). Empty specs = unrecognized → caller cascades.
    var specs : array<ReducerSpec>
    if (groupProjBody == null) return <- (<- specs, false)
    // Bare form: body is the reducer call directly.
    let (okBare, rnBare, innerBare) = is_bucket_reducer_call(groupProjBody, bindName)
    if (okBare) {
        var innerBody : Expression?
        if (innerBare != null) {
            innerBody = peel_lambda_rename_var(innerBare, itName)
            if (innerBody == null) return <- (<- specs, false)
        }
        let isAvgBare = rnBare == "average" || rnBare == "average_inner_select"
        var accType : TypeDeclPtr = slot_acc_type(groupProjBody._type, 1, true, isAvgBare, groupProjBody.at)
        if (accType == null) return <- (<- specs, false)
        var spec = ReducerSpec(slot = 1, name = rnBare, innerBody = innerBody, accType = accType)
        specs |> push(spec)
        return <- (<- specs, false)
    }
    // Named-tuple form: slot 0 = key (must be _._0), slots 1..N = reducers.
    if (!(groupProjBody is ExprMakeTuple)) return <- (<- specs, false)
    let mt = groupProjBody as ExprMakeTuple
    let n = mt.values |> length
    if (n < 2 || !peel_tuple_field_read(mt.values[0], bindName, 0)) return <- (<- specs, false)
    specs |> reserve(n - 1)
    for (i in range(1, n)) {
        let (ok, rn, innerLam) = is_bucket_reducer_call(mt.values[i], bindName)
        if (!ok) {
            specs |> clear
            return <- (<- specs, false)
        }
        var innerBody : Expression?
        if (innerLam != null) {
            innerBody = peel_lambda_rename_var(innerLam, itName)
            if (innerBody == null) {
                specs |> clear
                return <- (<- specs, false)
            }
        }
        let isAvg = rn == "average" || rn == "average_inner_select"
        var accType : TypeDeclPtr = slot_acc_type(groupProjBody._type, i, false, isAvg, groupProjBody.at)
        if (accType == null) {
            specs |> clear
            return <- (<- specs, false)
        }
        var spec = ReducerSpec(slot = i, name = rn, innerBody = innerBody, accType = accType)
        specs |> push(spec)
    }
    return <- (<- specs, true)
}

[macro_function]
def private mk_slot_ref(at : LineInfo; entryName : string; slot : int) : Expression? {
    // Programmatic `entry._{slot}` — qmacro has no dynamic-field-name splice.
    var ev = new ExprVar(at = at)
    ev.name := entryName
    var ef = new ExprField(at = at)
    ef.name := "_{slot}"
    ef.value = ev
    return ef
}

typedef private ReducerEmitterFn = function<(spec : ReducerSpec; at : LineInfo; entryName, itName : string) : tuple<Expression?; Expression?>>

// "value strictly preferred over cmp" for min (val<cmp) / max (val>cmp); workhorse uses native `<`/`>`, else `_::less`.
[macro_function]
def private mk_strictly_preferred(workhorse, isMin : bool; var valExpr : Expression?; var cmpExpr : Expression?) : Expression? {
    if (workhorse) {
        return qmacro($e(valExpr) < $e(cmpExpr)) if (isMin)
        return qmacro($e(valExpr) > $e(cmpExpr))
    }
    return qmacro(_::less($e(valExpr), $e(cmpExpr))) if (isMin)
    return qmacro(_::less($e(cmpExpr), $e(valExpr)))
}

[macro_function]
def private mk_reducer_count(spec : ReducerSpec; at : LineInfo; entryName, itName : string) : tuple<Expression?; Expression?> {
    var l1 = mk_slot_ref(at, entryName, spec.slot)
    var l2 = mk_slot_ref(at, entryName, spec.slot)
    let missInit = qmacro_expr() {
        $e(l1) = 1
    }
    let hitUpdate = qmacro_expr() {
        $e(l2) ++
    }
    return (missInit, hitUpdate)
}

[macro_function]
def private mk_reducer_long_count(spec : ReducerSpec; at : LineInfo; entryName, itName : string) : tuple<Expression?; Expression?> {
    var l1 = mk_slot_ref(at, entryName, spec.slot)
    var l2 = mk_slot_ref(at, entryName, spec.slot)
    let missInit = qmacro_expr() {
        $e(l1) = 1l
    }
    let hitUpdate = qmacro_expr() {
        $e(l2) ++
    }
    return (missInit, hitUpdate)
}

[macro_function]
def private mk_reducer_sum(spec : ReducerSpec; at : LineInfo; entryName, itName : string) : tuple<Expression?; Expression?> {
    var l1 = mk_slot_ref(at, entryName, spec.slot)
    var l2 = mk_slot_ref(at, entryName, spec.slot)
    let missInit = qmacro_expr() {
        $e(l1) = $i(itName)
    }
    let hitUpdate = qmacro_expr() {
        $e(l2) += $i(itName)
    }
    return (missInit, hitUpdate)
}

[macro_function]
def private mk_reducer_sum_inner(spec : ReducerSpec; at : LineInfo; entryName, itName : string) : tuple<Expression?; Expression?> {
    var l1 = mk_slot_ref(at, entryName, spec.slot)
    var l2 = mk_slot_ref(at, entryName, spec.slot)
    let missInit = qmacro_expr() {
        $e(l1) = $e(spec.innerBody)
    }
    let hitUpdate = qmacro_expr() {
        $e(l2) += $e(spec.innerBody)
    }
    return (missInit, hitUpdate)
}

[macro_function]
def private mk_reducer_min_max(spec : ReducerSpec; at : LineInfo; entryName, itName : string) : tuple<Expression?; Expression?> {
    let workhorse = spec.accType != null && spec.accType.isWorkhorseType
    let isMin = spec.name == "min"
    var lInit = mk_slot_ref(at, entryName, spec.slot)
    var lCmp  = mk_slot_ref(at, entryName, spec.slot)
    var lAsn  = mk_slot_ref(at, entryName, spec.slot)
    var predExpr = mk_strictly_preferred(workhorse, isMin, qmacro($i(itName)), lCmp)
    let missInit = qmacro_expr() {
        $e(lInit) := $i(itName)
    }
    let hitUpdate = qmacro_expr() {
        if ($e(predExpr)) {
            $e(lAsn) := $i(itName)
        }
    }
    return (missInit, hitUpdate)
}

[macro_function]
def private mk_reducer_min_max_inner(spec : ReducerSpec; at : LineInfo; entryName, itName : string) : tuple<Expression?; Expression?> {
    let workhorse = spec.accType != null && spec.accType.isWorkhorseType
    let isMin = spec.name == "min_inner_select"
    let tmp = "`vis`{at.line}`{at.column}`{spec.slot}"
    var lInit = mk_slot_ref(at, entryName, spec.slot)
    var lCmp  = mk_slot_ref(at, entryName, spec.slot)
    var lAsn  = mk_slot_ref(at, entryName, spec.slot)
    var predExpr = mk_strictly_preferred(workhorse, isMin, qmacro($i(tmp)), lCmp)
    let missInit = qmacro_expr() {
        $e(lInit) := $e(spec.innerBody)
    }
    let hitUpdate = qmacro_block() {
        let $i(tmp) = $e(spec.innerBody)
        if ($e(predExpr)) {
            $e(lAsn) := $i(tmp)
        }
    }
    return (missInit, hitUpdate)
}

[macro_function]
def private mk_reducer_first(spec : ReducerSpec; at : LineInfo; entryName, itName : string) : tuple<Expression?; Expression?> {
    var l1 = mk_slot_ref(at, entryName, spec.slot)
    let missInit = qmacro_expr() {
        $e(l1) := $i(itName)
    }
    return (missInit, null)
}

[macro_function]
def private mk_reducer_first_inner(spec : ReducerSpec; at : LineInfo; entryName, itName : string) : tuple<Expression?; Expression?> {
    var l1 = mk_slot_ref(at, entryName, spec.slot)
    let missInit = qmacro_expr() {
        $e(l1) := $e(spec.innerBody)
    }
    return (missInit, null)
}

// 2-slot acc: ._0 = sum (double), ._1 = count (uint64). Source cast to double once per side.
[macro_function]
def private mk_reducer_average(spec : ReducerSpec; at : LineInfo; entryName, itName : string) : tuple<Expression?; Expression?> {
    var lInit = mk_slot_ref(at, entryName, spec.slot)
    var lSum  = mk_slot_ref(at, entryName, spec.slot)
    var lCnt  = mk_slot_ref(at, entryName, spec.slot)
    let missInit = qmacro_expr() {
        $e(lInit) = (double($i(itName)), 1ul)
    }
    let hitUpdate = qmacro_block() {
        $e(lSum)._0 += double($i(itName))
        $e(lCnt)._1 ++
    }
    return (missInit, hitUpdate)
}

// Inner projection evaluated once into a temp per side — matches reference single-eval-per-element.
[macro_function]
def private mk_reducer_average_inner(spec : ReducerSpec; at : LineInfo; entryName, itName : string) : tuple<Expression?; Expression?> {
    let tmp = "`vavg`{at.line}`{at.column}`{spec.slot}"
    var lInit = mk_slot_ref(at, entryName, spec.slot)
    var lSum  = mk_slot_ref(at, entryName, spec.slot)
    var lCnt  = mk_slot_ref(at, entryName, spec.slot)
    let missInit = qmacro_block() {
        let $i(tmp) = $e(spec.innerBody)
        $e(lInit) = (double($i(tmp)), 1ul)
    }
    let hitUpdate = qmacro_block() {
        let $i(tmp) = $e(spec.innerBody)
        $e(lSum)._0 += double($i(tmp))
        $e(lCnt)._1 ++
    }
    return (missInit, hitUpdate)
}

var private reducer_emitters : table<string; ReducerEmitterFn> <- {
    "length"               => @@ < ReducerEmitterFn > mk_reducer_count,
    "count"                => @@ < ReducerEmitterFn > mk_reducer_count,
    "long_count"           => @@ < ReducerEmitterFn > mk_reducer_long_count,
    "sum"                  => @@ < ReducerEmitterFn > mk_reducer_sum,
    "sum_inner_select"     => @@ < ReducerEmitterFn > mk_reducer_sum_inner,
    "min"                  => @@ < ReducerEmitterFn > mk_reducer_min_max,
    "max"                  => @@ < ReducerEmitterFn > mk_reducer_min_max,
    "min_inner_select"     => @@ < ReducerEmitterFn > mk_reducer_min_max_inner,
    "max_inner_select"     => @@ < ReducerEmitterFn > mk_reducer_min_max_inner,
    "first"                => @@ < ReducerEmitterFn > mk_reducer_first,
    "first_inner_select"   => @@ < ReducerEmitterFn > mk_reducer_first_inner,
    "average"              => @@ < ReducerEmitterFn > mk_reducer_average,
    "average_inner_select" => @@ < ReducerEmitterFn > mk_reducer_average_inner
}

[macro_function]
def private emit_reducer_branches(spec : ReducerSpec; at : LineInfo; entryName, itName : string) : tuple<Expression?; Expression?> {
    // (missInit, hitUpdate); hitUpdate null for first* (subsequent same-key elements ignored).
    let emitter = reducer_emitters?[spec.name] ?? default<ReducerEmitterFn>
    return (null, null) if (emitter == null)
    return invoke(emitter, spec, at, entryName, itName)
}

[macro_function]
def private mk_avg_subfield(at : LineInfo; entryName : string; slot, sub : int) : Expression? {
    // entry._{slot}._{sub} — 2-level field access into a tuple<double; uint64> avg accumulator.
    var ev = new ExprVar(at = at)
    ev.name := entryName
    var ef1 = new ExprField(at = at)
    ef1.name := "_{slot}"
    ef1.value = ev
    var ef2 = new ExprField(at = at)
    ef2.name := "_{sub}"
    ef2.value = ef1
    return ef2
}

[macro_function]
def private mk_avg_divide_expr(at : LineInfo; entryName : string; slot : int) : Expression? {
    // entry._{slot}._0 / double(entry._{slot}._1) — average from the 2-slot per-key acc. Bucket is non-empty by construction (only keys with ≥1 source element enter the table), so count is always ≥1.
    var sumRef = mk_avg_subfield(at, entryName, slot, 0)
    var cntRef = mk_avg_subfield(at, entryName, slot, 1)
    return qmacro_expr() {
        $e(sumRef) / double($e(cntRef))
    }
}

[macro_function]
def private mk_slot_output_expr(at : LineInfo; entryName : string; spec : ReducerSpec) : Expression? {
    // Per-spec output value: divide for average slots, plain field ref otherwise.
    if (is_average_spec(spec)) return mk_avg_divide_expr(at, entryName, spec.slot)
    return mk_slot_ref(at, entryName, spec.slot)
}

[macro_function]
def private mk_hidden_acc_type(reducerName : string; isInnerSelect : bool;
                               bucketElemType, innerBodyType : TypeDeclPtr; at : LineInfo) : TypeDeclPtr {
    // Hidden-slot acc type: like slot_acc_type but takes source types directly (hidden slots have no group_proj body to drill into).
    if (reducerName == "length" || reducerName == "count") return new TypeDecl(baseType = Type.tInt, at = at)
    if (reducerName == "long_count") return new TypeDecl(baseType = Type.tInt64, at = at)
    if (reducerName == "average" || reducerName == "average_inner_select") return mk_avg_acc_type(at)
    let src = isInnerSelect ? innerBodyType : bucketElemType
    if (src == null) return null
    return strip_const_ref(clone_type(src))
}

[macro_function]
def private having_reducer_append_if_missing(var expr : Expression?; hbName, itName : string;
                                             var specs : array<ReducerSpec>; userVisibleSlotCount : int;
                                             bucketElemType : TypeDeclPtr; at : LineInfo) : bool {
    // Append a hidden ReducerSpec when `expr` is a bare/inner-select reducer on the having bind that has no name-match in `specs`. Returns false only on inner-select peel failure or on a second same-named hidden inner-select reducer (caller cascades — the splice can't distinguish two different inner lambdas, so we'd silently route both predicate terms to the first hidden slot).
    if (expr == null || !(expr is ExprCall)) return true
    let c = expr as ExprCall
    if (c.func == null || (c.arguments |> length) != 1) return true
    var fnName = string(c.func.name)
    if (c.func.fromGeneric != null) {
        fnName = string(c.func.fromGeneric.name)
    }
    let isReducer = (fnName == "length" || fnName == "count" || fnName == "long_count"
            || fnName == "sum" || fnName == "min" || fnName == "max"
            || fnName == "first" || fnName == "average")
    if (!isReducer) return true
    if (peel_tuple_field_read(c.arguments[0], hbName, 1)) {
        for (s in specs) {
            if (s.innerBody == null && s.name == fnName) return true
        }
        var accType = mk_hidden_acc_type(fnName, false, bucketElemType, null, at)
        if (accType == null) return true
        let newSlot = (specs |> length) + 1
        specs |> push <| ReducerSpec(slot = newSlot, name = fnName, innerBody = null, accType = accType)
        return true
    }
    let canInnerSelect = (fnName == "sum" || fnName == "min" || fnName == "max"
            || fnName == "first" || fnName == "average")
    if (!canInnerSelect) return true
    let inner = c.arguments[0]
    if (inner == null || !(inner is ExprCall)) return true
    let ic = inner as ExprCall
    if (ic.func == null || (ic.arguments |> length) != 2 || !peel_tuple_field_read(ic.arguments[0], hbName, 1)) return true
    var innerName = string(ic.func.name)
    if (ic.func.fromGeneric != null) {
        innerName = string(ic.func.fromGeneric.name)
    }
    if (innerName != "select") return true
    let comboName = "{fnName}_inner_select"
    for (s in specs) {
        // Match against a select-side spec (visible slot): v1 trusts the user wrote identical lambdas (pre-existing PR-D limitation, documented in LINQ.md). Match against an already-added hidden spec from this having scan would silently route two different inner lambdas to the same slot — bail to tier 2 instead, where each select() is evaluated separately.
        if (s.name == comboName) return s.slot <= userVisibleSlotCount
    }
    var innerBody = peel_lambda_rename_var(ic.arguments[1], itName)
    if (innerBody == null || innerBody._type == null) return false
    var accType = mk_hidden_acc_type(comboName, true, bucketElemType, innerBody._type, at)
    if (accType == null) return false
    let newSlot = (specs |> length) + 1
    specs |> push <| ReducerSpec(slot = newSlot, name = comboName, innerBody = innerBody, accType = accType)
    return true
}

[macro_function]
def private extend_specs_for_missing_having_reducers(var expr : Expression?; hbName, itName : string;
                                                     var specs : array<ReducerSpec>; userVisibleSlotCount : int;
                                                     bucketElemType : TypeDeclPtr; at : LineInfo) : bool {
    // Pre-rewrite walker that appends hidden ReducerSpecs for having-only reducers; unrecognized shapes leak to rewrite_having_pred (which bails on any surviving raw `<hb>`).
    if (expr == null) return true
    qm_peel_ref2value(expr)
    if (expr is ExprCall) {
        var c = expr as ExprCall
        if (!having_reducer_append_if_missing(expr, hbName, itName, specs, userVisibleSlotCount, bucketElemType, at)) return false
        for (i in 0 .. (c.arguments |> length)) {
            if (!extend_specs_for_missing_having_reducers(c.arguments[i], hbName, itName, specs, userVisibleSlotCount, bucketElemType, at)) return false
        }
        return true
    }
    if (expr is ExprField) {
        var f = expr as ExprField
        return extend_specs_for_missing_having_reducers(f.value, hbName, itName, specs, userVisibleSlotCount, bucketElemType, at)
    }
    if (expr is ExprOp1) {
        var op = expr as ExprOp1
        return extend_specs_for_missing_having_reducers(op.subexpr, hbName, itName, specs, userVisibleSlotCount, bucketElemType, at)
    }
    if (expr is ExprOp2) {
        var op = expr as ExprOp2
        if (!extend_specs_for_missing_having_reducers(op.left, hbName, itName, specs, userVisibleSlotCount, bucketElemType, at)) return false
        return extend_specs_for_missing_having_reducers(op.right, hbName, itName, specs, userVisibleSlotCount, bucketElemType, at)
    }
    if (expr is ExprOp3) {
        var op = expr as ExprOp3
        return (extend_specs_for_missing_having_reducers(op.subexpr, hbName, itName, specs, userVisibleSlotCount, bucketElemType, at)
                && extend_specs_for_missing_having_reducers(op.left, hbName, itName, specs, userVisibleSlotCount, bucketElemType, at)
                && extend_specs_for_missing_having_reducers(op.right, hbName, itName, specs, userVisibleSlotCount, bucketElemType, at))
    }
    return true
}

[macro_function]
def private rewrite_having_pred(var expr : Expression?; hbName, kvName : string; specs : array<ReducerSpec>) : Expression? {
    // Substitutes `<hb>._0` → `kv._0` and `<reducer>(<hb>._1)` / `<reducer>(select(<hb>._1, <lam>))` → `kv._{slot}` (or divide expr for average) against matching specs; any other use of `<hb>` returns null and cascades.
    if (expr == null) return expr
    qm_peel_ref2value(expr)
    if (expr is ExprCall) {
        var c = expr as ExprCall
        if (c.func != null && (c.arguments |> length) == 1) {
            var fnName = string(c.func.name)
            if (c.func.fromGeneric != null) {
                fnName = string(c.func.fromGeneric.name)
            }
            if (peel_tuple_field_read(c.arguments[0], hbName, 1)) {
                for (s in specs) {
                    if (s.innerBody == null && s.name == fnName) return mk_slot_output_expr(expr.at, kvName, s)
                }
                return null
            }
            let inner = c.arguments[0]
            if (inner != null && inner is ExprCall) {
                let ic = inner as ExprCall
                if (ic.func != null && (ic.arguments |> length) == 2 && peel_tuple_field_read(ic.arguments[0], hbName, 1)) {
                    var innerName = string(ic.func.name)
                    if (ic.func.fromGeneric != null) {
                        innerName = string(ic.func.fromGeneric.name)
                    }
                    if (innerName == "select") {
                        let comboName = "{fnName}_inner_select"
                        for (s in specs) {
                            if (s.name == comboName) return mk_slot_output_expr(expr.at, kvName, s)
                        }
                        return null
                    }
                }
            }
        }
        for (i in 0 .. (c.arguments |> length)) {
            var sub = rewrite_having_pred(c.arguments[i], hbName, kvName, specs)
            if (sub == null) return null
            c.arguments[i] = sub
        }
        return expr
    }
    if (expr is ExprField) {
        var f = expr as ExprField
        if (f.value != null && f.value is ExprVar) {
            let v = f.value as ExprVar
            if (v.name == hbName) {
                if (f.name == "_0") return mk_slot_ref(expr.at, kvName, 0)
                return null
            }
        }
        var sub = rewrite_having_pred(f.value, hbName, kvName, specs)
        if (sub == null) return null
        f.value = sub
        return expr
    }
    if (expr is ExprVar) {
        let v = expr as ExprVar
        if (v.name == hbName) return null
        return expr
    }
    if (expr is ExprOp1) {
        var op = expr as ExprOp1
        var sub = rewrite_having_pred(op.subexpr, hbName, kvName, specs)
        if (sub == null) return null
        op.subexpr = sub
        return expr
    }
    if (expr is ExprOp2) {
        var op = expr as ExprOp2
        var l = rewrite_having_pred(op.left, hbName, kvName, specs)
        if (l == null) return null
        op.left = l
        var r = rewrite_having_pred(op.right, hbName, kvName, specs)
        if (r == null) return null
        op.right = r
        return expr
    }
    if (expr is ExprOp3) {
        var op = expr as ExprOp3
        var s = rewrite_having_pred(op.subexpr, hbName, kvName, specs)
        if (s == null) return null
        op.subexpr = s
        var l = rewrite_having_pred(op.left, hbName, kvName, specs)
        if (l == null) return null
        op.left = l
        var r = rewrite_having_pred(op.right, hbName, kvName, specs)
        if (r == null) return null
        op.right = r
        return expr
    }
    return expr
}

class private HbScanner : AstVisitor {
    // Safety net for rewrite_having_pred — flags any ExprVar reference that survived
    // the targeted rewrite (e.g., because the predicate contained a node type the walker
    // doesn't recurse into, like ExprAt or ExprInvoke from a non-peelable lambda).
    vname : string
    hit : bool = false
    def HbScanner(n : string) {
        vname = n
    }
    def override visitExprVar(var expr : ExprVar?) : Expression? {
        if (expr.name == vname) {
            hit = true
        }
        return expr
    }
}

def private expr_uses_var(var e : Expression?; name : string) : bool {
    if (e == null) return false
    var astVisitor = new HbScanner(name)
    make_visitor(*astVisitor) $(astVisitorAdapter) {
        visit_expression(e, astVisitorAdapter)
    }
    let hit = astVisitor.hit
    unsafe {
        delete astVisitor
    }
    return hit
}

// ── group_by core (shared between array and decs sources) ──────────────────────

[macro_function]
def private plan_group_by_core(var calls : array<tuple<ExprCall?; LinqCall?>>;
                               var keyBlock : Expression?;
                               var groupProjCall : ExprCall?;
                               var havingCall : ExprCall?;
                               var trailingWhereCall : ExprCall?;
                               terminatorName : string;
                               orderName : string;
                               var orderKey : Expression?;
                               exprIsIterator : bool;
                               at : LineInfo;
                               var adapter : SourceAdapter) : Expression? {
    // Prefix keeps hoisted names per-source distinct ("" array / "decs_" decs / "djoin_" decs-join).
    let prefix = (adapter is Array) ? "" : ((adapter is DecsJoin) ? "djoin_" : "decs_")
    let tabName    = qn("{prefix}tab", at)
    let kName      = qn("{prefix}k", at)
    let ukName     = qn("{prefix}uk", at)
    let entryName  = qn("{prefix}entry", at)
    let kvName     = qn("{prefix}kv", at)
    let bufName    = qn("{prefix}buf", at)
    let bindName   = qn("{prefix}gpb", at)
    let cntName    = qn("{prefix}cnt", at)
    let dummyName  = qn("{prefix}dummy", at)
    let outName    = qn("{prefix}out", at)
    // Walk upstream where_/select* into segments. Each where guards everything AFTER it; a select after a where flushes a new segment so the projection bind lives inside the where's guard.
    var segBinds : array<array<Expression?>>
    var segWheres : array<Expression?>
    var currentBinds : array<Expression?>
    var currentWhere : Expression?
    var projection : Expression?
    var lastBindName = adapter_bind_name(adapter, at)
    var elemType = strip_const_ref(adapter_element_type(adapter))
    var totalBindCount = 0
    for (i in 0 .. (calls |> length)) {
        let opName = calls[i]._1.name
        if (opName == "where_") {
            if (projection != null) {
                let bn = "`{prefix}v`{at.line}`{at.column}`{totalBindCount}"
                totalBindCount ++
                currentBinds |> push <| qmacro_expr() {
                    var $i(bn) := $e(projection)
                }
                lastBindName = bn
                projection = null
            }
            var predicate = peel_lambda_rename_var(calls[i]._0.arguments[1], lastBindName)
            if (predicate == null) return null
            currentWhere = merge_where_cond(currentWhere, predicate)
        } elif (opName == "select") {
            if (currentWhere != null) {
                segBinds |> emplace(currentBinds)
                segWheres |> push(currentWhere)
                currentWhere = null
            }
            if (projection != null) {
                let bn = "`{prefix}v`{at.line}`{at.column}`{totalBindCount}"
                totalBindCount ++
                currentBinds |> push <| qmacro_expr() {
                    var $i(bn) := $e(projection)
                }
                lastBindName = bn
            }
            projection = peel_lambda_rename_var(calls[i]._0.arguments[1], lastBindName)
            if (projection == null) return null
            elemType = strip_const_ref(clone_type(calls[i]._0._type.firstType))
        } else {
            return null
        }
    }
    if (projection != null) {
        let bn = "`{prefix}v`{at.line}`{at.column}`{totalBindCount}"
        totalBindCount ++
        currentBinds |> push <| qmacro_expr() {
            var $i(bn) := $e(projection)
        }
        lastBindName = bn
    }
    segBinds |> emplace(currentBinds)
    segWheres |> push(currentWhere)
    let itName = lastBindName
    // Recognize the group_proj body's reducer shape (uses post-projection itName for inner-select fold).
    var groupProjBody = peel_lambda_rename_var(groupProjCall.arguments[1], bindName)
    if (groupProjBody == null || groupProjBody._type == null) return null
    var (specs, usesNamedTuple) = recognize_reducer_specs(groupProjBody, bindName, itName)
    if (empty(specs)) return null
    let userVisibleSlotCount = specs |> length
    // Rewrite optional having predicate against accumulator slots; PR-E scan adds hidden slots for reducers referenced by having but absent from select.
    var havingPred : Expression?
    if (havingCall != null) {
        let hbName = qn("{prefix}hb", at)
        var rawPred = peel_lambda_rename_var(havingCall.arguments[1], hbName)
        if (rawPred == null || !extend_specs_for_missing_having_reducers(rawPred, hbName, itName, specs, userVisibleSlotCount, elemType, at)) return null
        havingPred = rewrite_having_pred(rawPred, hbName, kvName, specs)
        if (havingPred == null || expr_uses_var(havingPred, hbName)) return null
    }
    // Peel optional trailing _where (Theme 2). Predicate references the post-aggregate output tuple via outName.
    var trailingWherePred : Expression?
    if (trailingWhereCall != null) {
        trailingWherePred = peel_lambda_rename_var(trailingWhereCall.arguments[1], outName)
        if (trailingWherePred == null) return null
    }
    let hasHidden = (specs |> length) > userVisibleSlotCount
    // Bare reducer + hidden slot needs typedecl(invoke(...)) growth inside a qmacro — can't grow that dynamically. Cascade.
    if (hasHidden && !usesNamedTuple) return null
    var hasAvg = false
    for (s in specs) {
        if (is_average_spec(s)) {
            hasAvg = true
        }
    }
    // Table + matching dummy. Dummy is bound on miss so first-key-wins is detected via addr-compare (one hash op/elem on hits).
    var stmts : array<Expression?>
    if (usesNamedTuple) {
        var tabValueType = strip_const_ref(clone_type(groupProjBody._type))
        if (hasAvg) {
            for (s in specs) {
                if (is_average_spec(s) && s.slot <= userVisibleSlotCount && s.slot < (tabValueType.argTypes |> length)) {
                    tabValueType.argTypes[s.slot] = mk_avg_acc_type(at)
                }
            }
        }
        if (hasHidden) {
            let preserveNames = (tabValueType.argNames |> length) == (tabValueType.argTypes |> length)
            for (s in specs) {
                if (s.slot > userVisibleSlotCount) {
                    tabValueType.argTypes |> push <| clone_type(s.accType)
                    if (preserveNames) {
                        let nameIdx = tabValueType.argNames |> length
                        tabValueType.argNames |> resize(nameIdx + 1)
                        tabValueType.argNames[nameIdx] := "_hidden_{s.slot}"
                    }
                }
            }
        }
        var tabValueType2 = clone_type(tabValueType)
        stmts |> push_from <| qmacro_block_to_array() {
            var inscope $i(tabName) : table<typedecl(_::unique_key(invoke($e(keyBlock), default<$t(elemType)>))); $t(tabValueType)>
            var $i(dummyName) : $t(tabValueType2)
        }
    } else {
        var accType  = clone_type(specs[0].accType)
        var accType2 = clone_type(specs[0].accType)
        stmts |> push_from <| qmacro_block_to_array() {
            var inscope $i(tabName) : table<typedecl(_::unique_key(invoke($e(keyBlock), default<$t(elemType)>))); tuple<typedecl(invoke($e(keyBlock), default<$t(elemType)>)) -const -&; $t(accType)>>
            var $i(dummyName) : tuple<typedecl(invoke($e(keyBlock), default<$t(elemType)>)) -const -&; $t(accType2)>
        }
    }
    // `tab?[uk] ?? dummy` + addr-compare: one hash op/elem on hits, two on misses (insert).
    var missInitStmts : array<Expression?>
    var hitUpdateStmts : array<Expression?>
    for (s in specs) {
        let (mi, hu) = emit_reducer_branches(s, at, entryName, itName)
        if (mi != null) {
            missInitStmts |> push(clone_expression(mi))
        }
        if (hu != null) {
            hitUpdateStmts |> push(clone_expression(hu))
        }
    }
    if (empty(missInitStmts)) return null
    var missInit = stmts_to_expr(missInitStmts)
    let hasHitUpdate = !empty(hitUpdateStmts)
    var tableUpdate : Expression?
    if (hasHitUpdate) {
        var hitUpdate = stmts_to_expr(hitUpdateStmts)
        tableUpdate = qmacro_expr() {
            unsafe {
                var $i(entryName) & = $i(tabName)?[$i(ukName)] ?? $i(dummyName)
                if (addr($i(entryName)) == addr($i(dummyName))) {
                    $e(missInit)
                    $i(dummyName)._0 = $i(kName)
                    $i(tabName)[$i(ukName)] = $i(dummyName)
                    $i(dummyName) = default<typedecl($i(dummyName))>
                } else {
                    $e(hitUpdate)
                }
            }
        }
    } else {
        tableUpdate = qmacro_expr() {
            unsafe {
                var $i(entryName) & = $i(tabName)?[$i(ukName)] ?? $i(dummyName)
                if (addr($i(entryName)) == addr($i(dummyName))) {
                    $e(missInit)
                    $i(dummyName)._0 = $i(kName)
                    $i(tabName)[$i(ukName)] = $i(dummyName)
                    $i(dummyName) = default<typedecl($i(dummyName))>
                }
            }
        }
    }
    // Peel keyBlock so `_.field` becomes a direct field read on itName, eliminating per-element invoke + (on decs) freeing the pruner to drop unused tuple slots that the whole-tup lambda arg otherwise pins.
    var keyExpr = peel_lambda_rename_var(keyBlock, itName)
    var coreBody = qmacro_block() {
        let $i(kName) = $e(keyExpr)
        let $i(ukName) = _::unique_key($i(kName))
        $e(tableUpdate)
    }
    // Walk segments in reverse, wrapping body inside-out: each where guards everything inside it.
    var body : Expression? = coreBody
    var idx = (segBinds |> length) - 1
    while (idx >= 0) {
        if (segWheres[idx] != null) {
            body = wrap_with_condition(body, clone_expression(segWheres[idx]))
        }
        if (!empty(segBinds[idx])) {
            var thisSeg <- segBinds[idx]
            thisSeg |> push(body)
            body = stmts_to_expr(thisSeg)
        }
        idx --
    }
    // Adapter-specific source loop emission (for(it in src) for array, for_each_archetype + inner for for decs).
    stmts |> push(adapter_wrap_source_loop(adapter, body, at))
    // Compute output tuple + bufElemType — needed by to_array always, and by count when trailingWherePred is present (predicate is bound against the constructed output).
    let needOutput = terminatorName != "count" || trailingWherePred != null
    var outputExpr : Expression?
    var bufElemType : TypeDeclPtr
    if (needOutput) {
        if (!usesNamedTuple) {
            if (hasAvg) {
                outputExpr = mk_avg_divide_expr(at, kvName, 1)
            } else {
                outputExpr = qmacro_expr() {
                    $i(kvName)._1
                }
            }
        } elif (hasAvg || hasHidden) {
            // Re-synthesize the user's tuple slot-by-slot: avg slots divide, hidden slots are omitted by iterating only user-visible argTypes; recordNames only when argNames are populated.
            var mt = new ExprMakeTuple(at = at)
            let slotCount = (groupProjBody._type.argTypes |> length)
            let hasNames = (groupProjBody._type.argNames |> length) == slotCount
            if (hasNames) {
                mt.recordNames |> resize(slotCount)
            }
            mt.values |> push(mk_slot_ref(at, kvName, 0))
            if (hasNames) {
                mt.recordNames[0] := string(groupProjBody._type.argNames[0])
            }
            for (i in 1 .. slotCount) {
                var v : Expression?
                for (sp in specs) {
                    if (sp.slot == i) {
                        v = mk_slot_output_expr(at, kvName, sp)
                    }
                }
                mt.values |> push(v)
                if (hasNames) {
                    mt.recordNames[i] := string(groupProjBody._type.argNames[i])
                }
            }
            outputExpr = mt
        } else {
            outputExpr = qmacro_expr() {
                $i(kvName)
            }
        }
        bufElemType = clone_type(groupProjBody._type)
        if (bufElemType != null) {
            bufElemType.flags.constant = false
            bufElemType.flags.ref = false
        }
    }
    // Terminator emission + retType derivation.
    var retType : TypeDeclPtr
    if (terminatorName == "count") {
        retType = new TypeDecl(baseType = Type.tInt)
        if (havingPred != null && trailingWherePred != null) {
            stmts |> push <| qmacro_block() {
                var $i(cntName) = 0
                for ($i(kvName) in values($i(tabName))) {
                    if ($e(havingPred)) {
                        let $i(outName) : $t(bufElemType) = $e(outputExpr)
                        if ($e(trailingWherePred)) {
                            $i(cntName) ++
                        }
                    }
                }
                return $i(cntName)
            }
        } elif (trailingWherePred != null) {
            stmts |> push <| qmacro_block() {
                var $i(cntName) = 0
                for ($i(kvName) in values($i(tabName))) {
                    let $i(outName) : $t(bufElemType) = $e(outputExpr)
                    if ($e(trailingWherePred)) {
                        $i(cntName) ++
                    }
                }
                return $i(cntName)
            }
        } elif (havingPred != null) {
            stmts |> push <| qmacro_block() {
                var $i(cntName) = 0
                for ($i(kvName) in values($i(tabName))) {
                    if ($e(havingPred)) {
                        $i(cntName) ++
                    }
                }
                return $i(cntName)
            }
        } else {
            stmts |> push <| qmacro_expr() {
                return length($i(tabName))
            }
        }
    } else {
        // to_array lane: walk table → buf; iterator-typed context wraps via buffer_return(..., true).
        if (exprIsIterator) {
            retType = new TypeDecl(baseType = Type.tIterator)
            retType.firstType = clone_type(bufElemType)
        } else {
            retType = new TypeDecl(baseType = Type.tArray)
            retType.firstType = clone_type(bufElemType)
        }
        stmts |> push_from <| qmacro_block_to_array() {
            var $i(bufName) : array<$t(bufElemType)>
            $i(bufName) |> reserve(length($i(tabName)))
        }
        if (havingPred != null && trailingWherePred != null) {
            stmts |> push <| qmacro_expr() {
                for ($i(kvName) in values($i(tabName))) {
                    if ($e(havingPred)) {
                        let $i(outName) : $t(bufElemType) = $e(outputExpr)
                        if ($e(trailingWherePred)) {
                            $i(bufName) |> push_clone($i(outName))
                        }
                    }
                }
            }
        } elif (trailingWherePred != null) {
            stmts |> push <| qmacro_expr() {
                for ($i(kvName) in values($i(tabName))) {
                    let $i(outName) : $t(bufElemType) = $e(outputExpr)
                    if ($e(trailingWherePred)) {
                        $i(bufName) |> push_clone($i(outName))
                    }
                }
            }
        } elif (havingPred != null) {
            stmts |> push <| qmacro_expr() {
                for ($i(kvName) in values($i(tabName))) {
                    if ($e(havingPred)) {
                        $i(bufName) |> push_clone($e(outputExpr))
                    }
                }
            }
        } else {
            stmts |> push <| qmacro_expr() {
                for ($i(kvName) in values($i(tabName))) {
                    $i(bufName) |> push_clone($e(outputExpr))
                }
            }
        }
        // Theme 3 Phase 2 C2: trailing `_order_by` / `_order_by_descending` on the output buffer — inline-cmp sort against the user's key. Skips the cascade's separate `order_by_inplace` call by mutating the same buf we just filled. Only inline-able keys (pure single-expression lambda, no sideeffects) splice; non-inline cascades.
        if (orderName != "") {
            var inlineCmp = try_make_inline_cmp(orderKey, orderName, bufElemType, at)
            if (inlineCmp == null) return null
            stmts |> push <| qmacro_expr() {
                sort($i(bufName), $e(inlineCmp))
            }
        }
        stmts |> push(buffer_return(bufName, exprIsIterator))
    }
    return adapter_wrap_invoke(adapter, stmts, retType, false, at)
}

// Shared array/decs/decs-join group_by emitter. Reconstructs head calls (mirrors emit_loop_or_count_lane_decs),
// builds SourceAdapter per source-shape (Array / Decs / DecsJoin), delegates to plan_group_by_core. Cascades on null bail.
[macro_function]
def private emit_group_by(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
    var gbCall = c.single["gb"]
    var keyBlock = clone_expression(gbCall.arguments[1])
    var groupProjCall = c.single["groupproj"]
    var havingCall : ExprCall?
    if (c.single |> key_exists("having")) {
        havingCall = c.single["having"]
    }
    var trailingWhereCall : ExprCall?
    if (c.single |> key_exists("trailing_where")) {
        trailingWhereCall = c.single["trailing_where"]
    }
    var terminatorName = ""
    if (c.single |> key_exists("term")) {
        terminatorName = c.single_name["term"]
    }
    var orderName = ""
    var orderKey : Expression?
    if (c.single |> key_exists("trailing_order")) {
        var orderCall = c.single["trailing_order"]
        orderName = c.single_name["trailing_order"]
        orderKey = clone_expression(orderCall.arguments[1])
    }
    // Reconstruct head calls (flatten_linq shape) from c.many["head"] so plan_group_by_core walks them unchanged.
    var headCalls : array<tuple<ExprCall?; LinqCall?>>
    headCalls |> reserve(length(c.many["head"]))
    for (call in c.many["head"]) {
        let nm = call_norm_name(call)
        if (!(linqCalls |> key_exists(nm))) return null
        var lc & = unsafe(linqCalls?[nm] ?? default<LinqCall>)
        headCalls |> push((call, unsafe(addr(lc))))
    }
    var adapter : SourceAdapter
    if (ctx.src is Decs) {
        var bridge = (ctx.src as Decs)._0
        if (c.single |> key_exists("upstream_join")) {
            var joinCall = c.single["upstream_join"]
            var bridgeB = extract_decs_bridge(joinCall.arguments[1])
            if (bridgeB == null) return null
            var keyaLam = joinCall.arguments[2]
            var keybLam = joinCall.arguments[3]
            var resultLam = joinCall.arguments[4]
            if (keyaLam == null || keybLam == null || resultLam == null
                    || keyaLam._type == null || resultLam._type == null
                    || resultLam._type.firstType == null) return null
            var keyType = strip_const_ref(clone_type(keyaLam._type.firstType))
            if (!is_primitive_join_key_type(keyType)) return null
            var resultType = strip_const_ref(clone_type(resultLam._type.firstType))
            var tupBType = strip_const_ref(clone_type(bridgeB.elementType))
            adapter = SourceAdapter(DecsJoin = DecsJoinShape(
                bridgeA = bridge,
                bridgeB = bridgeB,
                keyaLam = keyaLam,
                keybLam = keybLam,
                resultLam = resultLam,
                keyType = keyType,
                tupBType = tupBType,
                resultType = resultType,
                resBindName = qn("djoin_jres", at)))
        } else {
            adapter = SourceAdapter(Decs = (bridge, qn("decs_tup", at)))
        }
    } else {
        var top = (ctx.src as Array)._0
        if (top._type == null) return null
        adapter = SourceAdapter(Array = (top, qn("source", at)))
    }
    return plan_group_by_core(headCalls, keyBlock, groupProjCall, havingCall, trailingWhereCall,
        terminatorName, orderName, orderKey, ctx.expr_is_iterator, at, adapter)
}

[_macro]
def private register_group_by_rows {
    if (!is_compiling_macros_in_module("linq_fold")) return
    // Array-source group_by: head segments + group_by_lazy + opt having + select(groupproj) + opt trailing_where + opt trailing_order + opt count terminator.
    splice_patterns |> emplace <| SplicePattern(
        name = "group_by_array",
        chain <- [
            slot_chain_of(["where_", "select"], "head"),
            Slot(matcher = m_literal("group_by_lazy"), cardinality = c_one(), capture_name = "gb", arity = 2),
            Slot(matcher = m_literal("having_"),       cardinality = c_opt(), capture_name = "having"),
            Slot(matcher = m_literal("select"),        cardinality = c_one(), capture_name = "groupproj"),
            Slot(matcher = m_literal("where_"),        cardinality = c_opt(), capture_name = "trailing_where", arity = 2),
            Slot(matcher = m_alias("order_by_family"), cardinality = c_opt(), capture_name = "trailing_order", arity = 2),
            Slot(matcher = m_literal("count"),         cardinality = c_opt(), capture_name = "term", arity = 1)
        ],
        requires <- [@@ < RequiresPredicate > array_source, @@ < RequiresPredicate > no_order_with_count],
        emit = @@ < EmitFn > emit_group_by
    )
    // Decs-source: same shape + optional upstream `join` slot; v1 join-shape limits via decs_join_invariants.
    splice_patterns |> emplace <| SplicePattern(
        name = "group_by_decs",
        chain <- [
            slot_chain_of(["where_", "select"], "head"),
            Slot(matcher = m_literal("join"),          cardinality = c_opt(), capture_name = "upstream_join", arity = 5),
            Slot(matcher = m_literal("group_by_lazy"), cardinality = c_one(), capture_name = "gb", arity = 2),
            Slot(matcher = m_literal("having_"),       cardinality = c_opt(), capture_name = "having"),
            Slot(matcher = m_literal("select"),        cardinality = c_one(), capture_name = "groupproj"),
            Slot(matcher = m_literal("where_"),        cardinality = c_opt(), capture_name = "trailing_where", arity = 2),
            Slot(matcher = m_alias("order_by_family"), cardinality = c_opt(), capture_name = "trailing_order", arity = 2),
            Slot(matcher = m_literal("count"),         cardinality = c_opt(), capture_name = "term", arity = 1)
        ],
        requires <- [@@ < RequiresPredicate > decs_source, @@ < RequiresPredicate > decs_join_invariants, @@ < RequiresPredicate > no_order_with_count],
        emit = @@ < EmitFn > emit_group_by
    )
}

// ── decs eager-bridge unroll (Approach Z — for_each_archetype + nested _fold) ───────

struct private DecsBridgeShape {
    reqHashExpr : ExpressionPtr     // ExprConstUInt64 — cloned, ready to splice
    erqExpr     : ExpressionPtr     // ExprAddr — cloned, ready to splice
    archName    : string            // bridge's arch param name (reused as-is so cloned sources stay valid)
    forExpr     : ExpressionPtr     // cloned ExprFor — inner multi-iter for-loop; body replaced when splicing
    iterNames   : array<string>     // bridge's iter var names (prefixed component names)
    userNames   : array<string>     // user-facing field names from the push tuple — feed the named-tuple bind that lets the user's `_.userName` chain access work
    elementType : TypeDeclPtr       // named-tuple type (from resVar._type.firstType) — used by to_array/first when no projection
}

[macro_function]
def private get_call_short_name(c : ExprCall?) : string {
    // Strip module prefix + mangler hash. "__::linq`to_sequence`hash" → "to_sequence".
    let s = string(c.name)
    let bt = s |> rfind("`")
    if (bt >= 0) {
        let prev = s |> slice(0, bt) |> rfind("`")
        if (prev >= 0) return s |> slice(prev + 1, bt)
    }
    let cc = s |> rfind("::")
    if (cc >= 0) return s |> slice(cc + 2)
    return s
}

[macro_function]
def private extract_decs_bridge(var top : Expression?) : DecsBridgeShape? {
    // Pattern-match the post-expansion from_decs*-eager-bridge shape. Mismatch → null cascades to tier-2.
    if (!(top is ExprInvoke)) return null
    var topInv = top as ExprInvoke
    if (topInv.arguments |> length != 1
            || !(topInv.arguments[0] is ExprMakeBlock)) return null
    var mblk = topInv.arguments[0] as ExprMakeBlock
    if (!(mblk._block is ExprBlock)) return null
    var blk = mblk._block as ExprBlock
    if (blk.list |> length != 3
            || !empty(blk.arguments)
            || !(blk.list[0] is ExprLet)
            || !(blk.list[1] is ExprCall)
            || !(blk.list[2] is ExprReturn)) return null
    // Statement 0: var res : array<tuple<...>>
    var letStmt = blk.list[0] as ExprLet
    if (letStmt.variables |> length != 1) return null
    var resVar = letStmt.variables[0]
    if (!resVar._type.isGoodArrayType
            || resVar._type.firstType.baseType != Type.tTuple) return null
    let resVarName = string(resVar.name)
    // Statement 1: for_each_archetype(req_hash, erq, $(arch) { for(iter_vars in get_ro_sources){push((userNames=iter_vars))} })
    var feaCall = blk.list[1] as ExprCall
    if (feaCall.name != "for_each_archetype"
            || feaCall.arguments |> length != 3
            || !(feaCall.arguments[0] is ExprConstUInt64)
            || !(feaCall.arguments[1] is ExprAddr)
            || !(feaCall.arguments[2] is ExprMakeBlock)) return null
    var feaBlkExpr = feaCall.arguments[2] as ExprMakeBlock
    if (!(feaBlkExpr._block is ExprBlock)) return null
    var feaBlk = feaBlkExpr._block as ExprBlock
    if (feaBlk.arguments |> length != 1
            || feaBlk.list |> length != 1
            || !(feaBlk.list[0] is ExprFor)) return null
    let archName = string(feaBlk.arguments[0].name)
    var forExpr = feaBlk.list[0] as ExprFor
    // Body must be ExprBlock with single push call whose tuple recordNames are the user-facing field names.
    if (empty(forExpr.sources)
            || !(forExpr.body is ExprBlock)) return null
    var forBody = forExpr.body as ExprBlock
    if (forBody.list |> length != 1
            || !(forBody.list[0] is ExprCall)) return null
    var pushCall = forBody.list[0] as ExprCall
    // Push target must be the SAME `res` from stmt 0 — guards a user-written invoke that returns res.to_sequence() but pushes elsewhere.
    if (get_call_short_name(pushCall) != "push"
            || pushCall.arguments |> length != 2
            || !(pushCall.arguments[0] is ExprVar)
            || (pushCall.arguments[0] as ExprVar).name != resVarName
            || !(pushCall.arguments[1] is ExprMakeTuple)) return null
    var mkTup = pushCall.arguments[1] as ExprMakeTuple
    if (mkTup.recordNames |> length != forExpr.sources |> length
            || mkTup.values |> length != forExpr.sources |> length) return null
    // forExpr.iteratorVariables can be empty when the bridge feeds a no-chain terminator (e.g. bare `from_decs_template(...).count()` / `.count(p)`): no chain op forces a second inference pass over the for-loop, so the iter-var name list never gets populated. Recover names from mkTup.values, where each tuple element references the corresponding iter var by name — peeled through the ExprRef2Value wrap the typer adds around iter-var reads. The recovered names match what the loop will bind once a later pass infers it.
    var iterNames : array<string>
    iterNames |> reserve(mkTup.values |> length)
    if ((forExpr.iteratorVariables |> length) == (forExpr.sources |> length)) {
        for (v in forExpr.iteratorVariables) {
            iterNames |> push(string(v.name))
        }
    } else {
        for (v in mkTup.values) {
            var inner = v
            if (inner is ExprRef2Value) {
                inner = (inner as ExprRef2Value).subexpr
            }
            if (!(inner is ExprVar)) return null
            iterNames |> push(string((inner as ExprVar).name))
        }
    }
    var userNames <- [for (n in mkTup.recordNames); string(n)]
    // Statement 2: return res.to_sequence() — must reference the same `res`.
    var retStmt = blk.list[2] as ExprReturn
    if (retStmt.subexpr == null || !(retStmt.subexpr is ExprCall)) return null
    let retCall = retStmt.subexpr as ExprCall
    if (get_call_short_name(retCall) != "to_sequence"
            || empty(retCall.arguments)
            || !(retCall.arguments[0] is ExprVar)
            || (retCall.arguments[0] as ExprVar).name != resVarName) return null
    var info = new DecsBridgeShape(
        reqHashExpr = clone_expression(feaCall.arguments[0]),
        erqExpr = clone_expression(feaCall.arguments[1]),
        archName := archName,
        forExpr = clone_expression(forExpr),
        iterNames <- iterNames,
        userNames <- userNames,
        elementType = clone_type(resVar._type.firstType)
    )
    return info
}

[macro_function]
def private build_decs_tup_bind(bridge : DecsBridgeShape?; tupName : string; at : LineInfo) : Expression? {
    // Named-tuple bind: `var tup = (n1=iter1, n2=iter2, ...)` — peel_lambda_rename_var peels user lambdas substituting `_.userName` → `tup.userName`.
    var mkTup = new ExprMakeTuple(at = at)
    mkTup.recordNames |> resize(length(bridge.userNames))
    for (i in 0 .. length(bridge.userNames)) {
        mkTup.recordNames[i] := bridge.userNames[i]
        mkTup.values |> emplace_new(new ExprVar(at = at, name := bridge.iterNames[i]))
    }
    return qmacro_expr() {
        var $i(tupName) = $e(mkTup)
    }
}

[macro_function]
def private build_decs_inner_for(bridge : DecsBridgeShape?; var tupBind : Expression?; var body : Expression?; at : LineInfo) : Expression? {
    // Clone bridge's inner multi-iter for-loop; substitute body with [tupBind, body]. Reuses bridge.archName so cloned get_ro sources stay valid.
    var forBodyStmts <- [tupBind, body]
    var forBody = stmts_to_expr(forBodyStmts)
    var clonedForExpr = clone_expression(bridge.forExpr)
    var clonedFor = clonedForExpr as ExprFor
    var newForBody = new ExprBlock(at = at)
    newForBody.list |> push(forBody)
    clonedFor.body = newForBody
    return clonedForExpr
}

// Walks a chain body to collect which fields of the decs_tup named-tuple bind are actually referenced. allFieldsUsed flips when the bind is referenced as a whole-var (not via field access) — e.g. push_clone(decs_tup) in a bare to_array, or pass-to-user-fn — so the helper falls back to unpruned emission and preserves the user-visible output shape. Also picks up bare iter-var refs (when `iterToUser` is non-empty) so the trivial-let-elision path — which rewrites `decs_sel_N`-bound projections to the iter var directly, bypassing `decs_tup` entirely — still hits the pruned arm instead of falling through to unpruned-default.
class private DecsTupUsageScanner : AstVisitor {
    tupName       : string
    iterToUser    : table<string; string>
    usedFields    : table<string>
    allFieldsUsed : bool = false
    inTargetField : bool = false
    def DecsTupUsageScanner(n : string; var i2u : table<string; string>) {
        tupName = n
        iterToUser <- i2u
    }
    def override preVisitExprField(expr : ExprField?) : void {
        var v = expr.value
        if (v is ExprRef2Value) {
            v = (v as ExprRef2Value).subexpr
        }
        if (v is ExprVar && (v as ExprVar).name == tupName) {
            usedFields |> insert(string(expr.name))
            inTargetField = true
        }
    }
    def override visitExprField(var expr : ExprField?) : ExpressionPtr {
        inTargetField = false
        return expr
    }
    def override preVisitExprVar(expr : ExprVar?) : void {
        if (expr.name == tupName && !inTargetField) {
            allFieldsUsed = true
            return
        }
        let nm = string(expr.name)
        if (key_exists(iterToUser, nm)) {
            usedFields |> insert(iterToUser[nm])
        }
    }
}

[macro_function]
def private collect_decs_tup_usage(var e : Expression?; tupName : string; bridge : DecsBridgeShape?) : tuple<bool; array<string>> {
    // Returns (allFieldsUsed, usedFieldNames). usedFieldNames is unordered — caller filters bridge.userNames in original order to preserve get_ro emission order. The bridge feeds an iter-var → user-name reverse map so bare iter-var refs (from trivial-let elision) count as field usage too.
    var allUsed = false
    var used : array<string>
    if (e == null) return (allUsed, used)
    var iterToUser <- {for (i in 0 .. length(bridge.iterNames)); bridge.iterNames[i] => bridge.userNames[i]}
    var sc = new DecsTupUsageScanner(tupName, iterToUser)
    make_visitor(*sc) $(astVisitorAdapter) {
        visit_expression(e, astVisitorAdapter)
    }
    allUsed = sc.allFieldsUsed
    for (k in keys(sc.usedFields)) {
        used |> push(k)
    }
    unsafe {
        delete sc
    }
    return (allUsed, used)
}

// Rewrites every whole-var ref to `decs_tup` into a freshly synthesized `(userName1 = iterVar1, ...)` named-tuple literal — same shape build_decs_tup_bind produces, but inlined at each use site. Lets emitters that store the whole element (last/single/aggregate seed of element type, min_by/max_by, to_array push_clone) avoid forcing the unpruned bind path: after rewrite the body contains only `decs_tup.<field>` refs (flattenable) plus iter-var refs in the synthesized literals (picked up by the usage scanner). build_decs_inner_for_pruned then flattens + drops the bind entirely. Inside ExprField the inner ExprVar is left alone so the field path still matches.
class private DecsTupWholeRewriter : AstVisitor {
    tupName       : string
    userNames     : array<string>
    iterNames     : array<string>
    inTargetField : bool = false
    rewroteAny    : bool = false
    def DecsTupWholeRewriter(t : string; var un, in_ : array<string>) {
        tupName = t
        userNames <- un
        iterNames <- in_
    }
    def override preVisitExprField(expr : ExprField?) : void {
        var v = expr.value
        if (v is ExprRef2Value) {
            v = (v as ExprRef2Value).subexpr
        }
        if (v is ExprVar && (v as ExprVar).name == tupName) {
            inTargetField = true
        }
    }
    def override visitExprField(var expr : ExprField?) : ExpressionPtr {
        inTargetField = false
        return expr
    }
    def override visitExprVar(var expr : ExprVar?) : ExpressionPtr {
        if (expr.name != tupName || inTargetField) return expr
        var mkTup = new ExprMakeTuple(at = expr.at)
        mkTup.recordNames |> resize(length(userNames))
        for (i in 0 .. length(userNames)) {
            mkTup.recordNames[i] := userNames[i]
            mkTup.values |> emplace_new(new ExprVar(at = expr.at, name := iterNames[i]))
        }
        rewroteAny = true
        return mkTup
    }
}

[macro_function]
def private rewrite_decs_tup_wholes(var body : Expression?; tupName : string; bridge : DecsBridgeShape?) : bool {
    if (body == null) return false
    var unCopy <- [for (n in bridge.userNames); n]
    var inCopy <- [for (n in bridge.iterNames); n]
    var rw = new DecsTupWholeRewriter(tupName, unCopy, inCopy)
    make_visitor(*rw) $(astVisitorAdapter) {
        visit_expression(body, astVisitorAdapter)
    }
    let any = rw.rewroteAny
    unsafe {
        delete rw
    }
    return any
}

// When the body only references `decs_tup` via field access (no whole-var refs), the named-tuple wrap plus every per-element `decs_tup.<field>` read are pure overhead — rewrite each field access to the matching iter var directly and skip the bind. Drops one tuple-make + one field-read per kept field per iteration.
class private DecsTupFieldFlattener : AstVisitor {
    tupName     : string
    fieldToIter : table<string; string>
    def DecsTupFieldFlattener(t : string; var f : table<string; string>) {
        tupName = t
        fieldToIter <- f
    }
    def override visitExprField(var expr : ExprField?) : ExpressionPtr {
        var v = expr.value
        if (v is ExprRef2Value) {
            v = (v as ExprRef2Value).subexpr
        }
        if (v is ExprVar && (v as ExprVar).name == tupName) {
            let fname = string(expr.name)
            if (key_exists(fieldToIter, fname)) return new ExprVar(at = expr.at, name := fieldToIter[fname])
        }
        return expr
    }
}

[macro_function]
def private flatten_decs_tup_to_iter(var body : Expression?; tupName : string; var fieldToIter : table<string; string>) : void {
    if (body == null) return
    var fl = new DecsTupFieldFlattener(tupName, fieldToIter)
    make_visitor(*fl) $(astVisitorAdapter) {
        visit_expression(body, astVisitorAdapter)
    }
    unsafe {
        delete fl
    }
}

[macro_function]
def private build_decs_inner_for_pruned(bridge : DecsBridgeShape?;
                                        tupName : string;
                                        var body : Expression?;
                                        at : LineInfo) : Expression? {
    // Walks body for `tupName.<field>` references; when pruning is safe + beneficial, emits the inner multi-iter for with unused get_ro slots dropped and a matching shrunk named-tuple bind. Otherwise falls through to the unpruned path so user-visible shape stays intact (bare to_array, push_clone(decs_tup), pass-to-user-fn).
    var (allUsed, usedNames) = collect_decs_tup_usage(body, tupName, bridge)
    // Whole-var refs (`lst := decs_tup`, `push_clone(buf, decs_tup)`, etc) would force the unpruned bind path — but they're all semantically equivalent to a synthesized `(userName1=iter1, ...)` literal. Rewrite them inline so the flatten + bind-elision path unlocks; saves the per-iteration `var decs_tup = (...)` wrap (~12 ns/op on last/single/select_where 100K-row benches).
    if (allUsed && rewrite_decs_tup_wholes(body, tupName, bridge)) {
        var (allUsed2, usedNames2) = collect_decs_tup_usage(body, tupName, bridge)
        allUsed = allUsed2
        usedNames <- usedNames2
    }
    // Fallback to the unpruned bind ONLY for whole-var refs that survived rewrite (pass-to-user-fn) or the edge case where the body never touches decs_tup at all. The "all fields used via field access" case still benefits from flatten + bind elision — no slots dropped, but the per-iter tuple-make and field reads disappear.
    if (allUsed || empty(usedNames)) {
        var tupBind = build_decs_tup_bind(bridge, tupName, at)
        return build_decs_inner_for(bridge, tupBind, body, at)
    }
    var usedSet : table<string>
    for (n in usedNames) {
        usedSet |> insert(n)
    }
    var fieldToIter <- {for (i in 0 .. length(bridge.userNames));
        bridge.userNames[i] => bridge.iterNames[i];
        where key_exists(usedSet, bridge.userNames[i])}
    // Diagnostic-friendly fallback when some `usedNames` entry has no bridge match. Today the typer pipeline checks user `_.<field>` lambdas before plan_decs_unroll runs, so this is unreachable in practice — but if a future macro ever synthesizes `decs_tup.<unknownField>`, the unpruned bind keeps the typer's "unknown field of <named-tuple>" diagnostic intact instead of degrading it to "unknown identifier decs_tup".
    if (length(fieldToIter) < length(usedSet)) {
        var tupBind = build_decs_tup_bind(bridge, tupName, at)
        return build_decs_inner_for(bridge, tupBind, body, at)
    }
    // Pruned path: no whole-var ref → no need for the named tuple. Rewrite every `decs_tup.<userName>` in the body to the matching iter var, then drop the bind entirely. Eliminates one tuple-make + N per-element field reads per iter.
    flatten_decs_tup_to_iter(body, tupName, fieldToIter)
    // Clone forExpr; drop the iter slots not in usedSet from all 5 parallel dasvectors (sources/iterators/iteratorsAt/iteratorsAka/iteratorsTags). iteratorVariables is cleared so the typer rebuilds it during re-type — same pattern as daslib/soa.das:381.
    var clonedForExpr = clone_expression(bridge.forExpr)
    var clonedFor = clonedForExpr as ExprFor
    let nSlots = length(bridge.userNames)
    for (i in 0 .. nSlots) {
        let revIdx = nSlots - 1 - i
        if (!key_exists(usedSet, bridge.userNames[revIdx])) {
            clonedFor.sources       |> erase(revIdx)
            clonedFor.iterators     |> erase(revIdx)
            clonedFor.iteratorsAt   |> erase(revIdx)
            clonedFor.iteratorsAka  |> erase(revIdx)
            clonedFor.iteratorsTags |> erase(revIdx)
        }
    }
    clonedFor.iteratorVariables |> clear()
    // Install body — body now references iter vars directly; no tupBind needed.
    var newForBody = new ExprBlock(at = at)
    newForBody.list |> push(body)
    clonedFor.body = newForBody
    return clonedForExpr
}

struct private DecsChainInfo {
    bindAt      : array<string>    // bind name visible at each chain position
    elidedAt    : array<bool>      // true when the select at position i was elided (peeled to a single decs_tup field access → reused iter var as next bind, no `let decs_sel_N = ...` emit needed)
    finalBind   : string            // bind name AFTER full chain — what terminator references
    finalType   : TypeDeclPtr       // element type AFTER full chain (constant + ref stripped)
    selectCount : int               // number of `select` ops in chain; 0 means finalBind == tupName
}

[macro_function]
def private compute_decs_chain_info(var calls : array<tuple<ExprCall?; LinqCall?>>;
                                    intermediateEnd : int;
                                    tupName : string;
                                    bridge : DecsBridgeShape?;
                                    at : LineInfo) : DecsChainInfo? {
    // Each select introduces a fresh `decs_sel{N}` bind; subsequent ops peel against it. Returns null on any non-where_/select op.
    var info = new DecsChainInfo(
        finalBind := tupName,
        finalType = clone_type(bridge.elementType),
        selectCount = 0
    )
    info.bindAt |> reserve(intermediateEnd)
    info.elidedAt |> reserve(intermediateEnd)
    var curBind = tupName
    var curType : TypeDeclPtr = clone_type(bridge.elementType)
    for (i in 0 .. intermediateEnd) {
        info.bindAt |> push(curBind)
        info.elidedAt |> push(false)
        var cll & = unsafe(calls[i])
        let opName = cll._1.name
        if (opName == "select") {
            info.selectCount ++
            var peeled = peel_lambda_rename_var(cll._0.arguments[1], info.bindAt[i])
            if (peeled == null || peeled._type == null) return null
            // Trivial-let elision: `_select(_.userName)` against decs_tup → iter var directly. The pruned-for keeps the iter var (its name = bridge.iterNames[idx]) and the flattener would otherwise rewrite the synthetic `let decs_sel_N = decs_tup.userName` to `let decs_sel_N = <iterVar>` — a pure copy. Renaming finalBind to the iter var name skips the binding entirely and the action references it natively. Typer wraps both the projection root (`peeled` may be ExprRef2Value) AND the ExprVar inside the ExprField in ExprRef2Value — peel both.
            var elided = false
            if (curBind == tupName) {
                var top = peeled
                if (top is ExprRef2Value) {
                    top = (top as ExprRef2Value).subexpr
                }
                if (top is ExprField) {
                    var pf = top as ExprField
                    var pv = pf.value
                    if (pv is ExprRef2Value) {
                        pv = (pv as ExprRef2Value).subexpr
                    }
                    if (pv is ExprVar && (pv as ExprVar).name == tupName) {
                        let fname = string(pf.name)
                        for (ui in 0 .. length(bridge.userNames)) {
                            if (bridge.userNames[ui] == fname) {
                                curBind = bridge.iterNames[ui]
                                elided = true
                                info.elidedAt[i] = true
                                break
                            }
                        }
                    }
                }
            }
            if (!elided) {
                curBind = "`decs_sel`{at.line}`{at.column}`{info.selectCount}"
            }
            curType = clone_type(peeled._type)
        } elif (opName != "where_") return null
    }
    info.finalBind := curBind
    info.finalType = curType
    if (info.finalType != null) {
        info.finalType.flags.constant = false
        info.finalType.flags.ref = false
    }
    return info
}

[macro_function]
def private wrap_decs_chain(var action : Expression?;
                            chainInfo : DecsChainInfo?;
                            var calls : array<tuple<ExprCall?; LinqCall?>>;
                            intermediateEnd : int;
                            at : LineInfo) : Expression? {
    // Reverse-walk chain, wrapping action with `if (pred) { ... }` for where_ and `let bindN+1 = proj; ...` for select.
    var current = action
    for (rev in 0 .. intermediateEnd) {
        let i = intermediateEnd - 1 - rev
        var cll & = unsafe(calls[i])
        let opName = cll._1.name
        let bindHere = chainInfo.bindAt[i]
        if (opName == "where_") {
            var pred = peel_lambda_rename_var(cll._0.arguments[1], bindHere)
            if (pred == null) return null
            current = qmacro_expr() {
                if ($e(pred)) {
                    $e(current)
                }
            }
        } elif (opName == "select") {
            // Skip emission entirely when this select was elided in compute_decs_chain_info — the next bind is already the iter var the flattener would have produced for `let nextBind = decs_tup.userName`. No copy needed; downstream `current` already references the iter var by the elided name.
            continue if (chainInfo.elidedAt[i])
            var proj = peel_lambda_rename_var(cll._0.arguments[1], bindHere)
            if (proj == null) return null
            let nextBind = (i + 1 < intermediateEnd) ? chainInfo.bindAt[i + 1] : chainInfo.finalBind
            var letStmt = qmacro_expr() {
                let $i(nextBind) = $e(proj)
            }
            var stmts : array<Expression?>
            stmts |> reserve(2)
            stmts |> push(letStmt)
            stmts |> push(current)
            current = stmts_to_expr(stmts)
        } else {
            return null
        }
    }
    return current
}

struct private DecsRangeInfo {
    skipExpr      : Expression?
    takeExpr      : Expression?
    skipWhileCond : Expression?    // Slice 5b: predicate peeled against tupName (source tuple)
    takeWhileCond : Expression?    // Slice 5b: predicate peeled against tupName (source tuple)
    chainEnd      : int
}

struct private DecsRangeStateNames {
    skipName      : string
    takeCountName : string
    takeLimitName : string    // decs adds an explicit take-limit local so the take(N) expression evaluates exactly once at invoke entry.
    skippingName  : string
}

[macro_function]
def private make_decs_range_names(at : LineInfo) : DecsRangeStateNames {
    return <- DecsRangeStateNames(
        skipName      = qn("decs_skip", at),
        takeCountName = qn("decs_takec", at),
        takeLimitName = qn("decs_takel", at),
        skippingName  = qn("decs_skipping", at))
}

[macro_function]
def private extract_decs_ranges(var calls : array<tuple<ExprCall?; LinqCall?>>;
                                intermediateEnd : int;
                                tupName : string;
                                at : LineInfo) : DecsRangeInfo? {
    // Slice 5a/5b: split intermediate chain into (where_/select prefix) + (skip? skip_while? take_while? take? suffix). Canonical order matches array-side (linq_fold.das:1607-1634): skip → skip_while → take_while → take, all after any where_. skip_while/take_while predicates use the SOURCE tuple (tupName); bail when a select appears in the prefix (matches array-side seenSelect bail).
    var info = new DecsRangeInfo(chainEnd = intermediateEnd)
    var firstRangeIdx = intermediateEnd
    for (i in 0 .. intermediateEnd) {
        let nm = calls[i]._1.name
        if (nm == "take" || nm == "skip" || nm == "take_while" || nm == "skip_while") {
            firstRangeIdx = i
            break
        }
    }
    if (firstRangeIdx == intermediateEnd) return info
    // Detect whether the suffix contains any predicate-driven range (skip_while/take_while) — if so, prefix must be select-free (mirrors array-side seenSelect bail at linq_fold.das:1615/1623).
    var hasWhilePred = false
    for (i in firstRangeIdx .. intermediateEnd) {
        let nm = calls[i]._1.name
        if (nm == "skip_while" || nm == "take_while") {
            hasWhilePred = true
            break
        }
    }
    if (hasWhilePred) {
        for (i in 0 .. firstRangeIdx) {
            if (calls[i]._1.name == "select") return null
        }
    }
    for (i in firstRangeIdx .. intermediateEnd) {
        var cll & = unsafe(calls[i])
        let nm = cll._1.name
        if (nm == "skip") {
            if (info.skipExpr != null || info.skipWhileCond != null || info.takeWhileCond != null || info.takeExpr != null || length(cll._0.arguments) < 2) return null
            var arg = cll._0.arguments[1]
            if (arg == null || arg._type == null || arg._type.baseType != Type.tInt) return null
            info.skipExpr = clone_expression(arg)
        } elif (nm == "skip_while") {
            if (info.skipWhileCond != null || info.takeWhileCond != null || info.takeExpr != null || length(cll._0.arguments) < 2) return null
            var pred = peel_lambda_rename_var(cll._0.arguments[1], tupName)
            if (pred == null) return null
            info.skipWhileCond = pred
        } elif (nm == "take_while") {
            if (info.takeWhileCond != null || info.takeExpr != null || length(cll._0.arguments) < 2) return null
            var pred = peel_lambda_rename_var(cll._0.arguments[1], tupName)
            if (pred == null) return null
            info.takeWhileCond = pred
        } elif (nm == "take") {
            if (info.takeExpr != null || length(cll._0.arguments) < 2) return null
            var arg = cll._0.arguments[1]
            if (arg == null || arg._type == null || arg._type.baseType != Type.tInt) return null
            info.takeExpr = clone_expression(arg)
        } else {
            return null
        }
    }
    info.chainEnd = firstRangeIdx
    return info
}

[macro_function]
def private wrap_inner_for_with_decs_ranges(var body : Expression?;
                                            rangeInfo : DecsRangeInfo?;
                                            names : DecsRangeStateNames) : Expression? {
    // Slice 5a/5b: prefix per-element body with range guards in canonical order: take-cap → skip-counter → skip_while flag → take_while break → takeC++. Mirrors array-side wrap_with_ranges (linq_fold.das:556). All early exits (`return true`) work both for plain inner-for break AND for the bool-lambda archetype-find — caller switches outer iterator to for_each_archetype_find when takeExpr != null OR takeWhileCond != null. Counters / flags live at invoke scope (decs_range_prelude). Take limit is read from a hoisted local (takeLimitName) so the user's take(N) expression evaluates exactly once at invoke entry.
    if (rangeInfo.skipExpr == null && rangeInfo.takeExpr == null && rangeInfo.skipWhileCond == null && rangeInfo.takeWhileCond == null) return body
    let skipName      = names.skipName
    let takeCountName = names.takeCountName
    let takeLimitName = names.takeLimitName
    let skippingName  = names.skippingName
    var stmts : array<Expression?>
    stmts |> reserve(6)
    if (rangeInfo.takeExpr != null) {
        // `>=` so non-positive N short-circuits on first iteration. `return true` exits both inner for AND the archetype bool lambda.
        stmts |> push <| qmacro_expr() {
            if ($i(takeCountName) >= $i(takeLimitName)) {
                return true
            }
        }
    }
    if (rangeInfo.skipExpr != null) {
        stmts |> push <| qmacro_expr() {
            if ($i(skipName) > 0) {
                $i(skipName) --
                continue
            }
        }
    }
    if (rangeInfo.skipWhileCond != null) {
        stmts |> push <| qmacro_expr() {
            if ($i(skippingName)) {
                if ($e(rangeInfo.skipWhileCond)) {
                    continue
                }
                $i(skippingName) = false
            }
        }
    }
    if (rangeInfo.takeWhileCond != null) {
        // First false ends iteration globally (across all archetypes). `return true` exits inner-for AND archetype-find lambda.
        stmts |> push <| qmacro_expr() {
            if (!($e(rangeInfo.takeWhileCond))) {
                return true
            }
        }
    }
    if (rangeInfo.takeExpr != null) {
        // takeC++ AFTER skip_while / take_while so gated-out elements don't eat the take(N) budget (matches array-side comment at linq_fold.das:558).
        stmts |> push <| qmacro_expr() {
            $i(takeCountName) ++
        }
    }
    stmts |> push(body)
    return stmts_to_expr(stmts)
}

[macro_function]
def private decs_range_prelude(rangeInfo : DecsRangeInfo?; names : DecsRangeStateNames) : array<Expression?> {
    // Counter / flag init statements hoisted ABOVE the for_each_archetype call so they survive across archetypes (take cap, skip count, skip_while flag are all globally-stateful). takeLimit hoisted to a local so the user's take(N) expression evaluates exactly once (matches take(src, total:int) semantics).
    let skipName      = names.skipName
    let takeCountName = names.takeCountName
    let takeLimitName = names.takeLimitName
    let skippingName  = names.skippingName
    var stmts : array<Expression?>
    if (rangeInfo.skipExpr != null) {
        stmts |> push <| qmacro_expr() {
            var $i(skipName) = $e(rangeInfo.skipExpr)
        }
    }
    if (rangeInfo.takeExpr != null) {
        stmts |> push_from <| qmacro_block_to_array() {
            let $i(takeLimitName) = $e(rangeInfo.takeExpr)
            var $i(takeCountName) = 0
        }
    }
    if (rangeInfo.skipWhileCond != null) {
        // Slice 5b: skip_while flag is one-way (flips false on first non-matching elem, stays). Hoisted so the flag persists across archetypes.
        stmts |> push <| qmacro_expr() {
            var $i(skippingName) = true
        }
    }
    return <- stmts
}

[macro_function]
def private emit_default_bind(name : string; argExpr : Expression?; canCopy : bool) : Expression? {
    // Bind user-supplied default once. Non-copyable uses `var dv <- clone_to_move(arg)` — fully-owned local so multiple return-default sites can each clone-out without consuming.
    var out : Expression?
    if (canCopy) {
        out = qmacro_expr() {
            let $i(name) = $e(argExpr)
        }
    } else {
        out = qmacro_expr() {
            var $i(name) <- clone_to_move($e(argExpr))
        }
    }
    return out
}

[macro_function]
def private emit_default_return(name : string; canCopy : bool) : Expression? {
    // Non-copyable uses `return <- clone_to_move(name)` (safe repeated calls — clone_to_move does NOT consume).
    var out : Expression?
    if (canCopy) {
        out = qmacro_expr() {
            return $i(name)
        }
    } else {
        out = qmacro_expr() {
            return <- clone_to_move($i(name))
        }
    }
    return out
}

[macro_function]
def private emit_decs_count_archsize(bridge : DecsBridgeShape?; at : LineInfo) : Expression? {
    // Bare count(): no chain ops, sum arch.size per archetype — skips the per-entity walk entirely.
    let accName = qn("decs_acc", at)
    let archName = qn("decs_arch", at)
    // arch.size is int64; count() returns int and the += site truncates past INT_MAX — chain `long_count()` when an int64-safe total is required.
    var emission : Expression? = qmacro(invoke($() : int {
        var $i(accName) = 0
        for_each_archetype($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) {
            $i(accName) += int($i(archName).size)
        })
        return $i(accName)
    }))
    emission.force_at(at)
    emission.force_generated(true)
    return emission
}

[macro_function]
def private emit_decs_accumulator(bridge : DecsBridgeShape?;
                                  opName : string;
                                  chainInfo : DecsChainInfo?;
                                  rangeInfo : DecsRangeInfo?;
                                  var calls : array<tuple<ExprCall?; LinqCall?>>;
                                  intermediateEnd : int;
                                  terminatorCall : ExprCall?;
                                  var accType : TypeDeclPtr;
                                  at : LineInfo) : Expression? {
    // Slice 2/3a/4 accumulator emission: count / long_count / sum / min / max / average with chained _select + interleaved _where + optional _count(pred). Slice 5a adds take/skip ranges via rangeInfo (counters hoisted at invoke scope, take present → for_each_archetype_find).
    let accName = qn("decs_acc", at)
    let tupName = qn("decs_tup", at)
    let cntName = qn("decs_cnt", at)
    let firstName = qn("decs_first", at)
    let valBindName = qn("decs_val", at)
    let names <- make_decs_range_names(at)
    let finalBind = chainInfo.finalBind
    // Wrap finalBind as an Expression so the shared per-element builder (lane uses `$e(projection)`, decs uses `$e(qmacro($i(finalBind)))`) sees one shape.
    var valueExpr : Expression? = qmacro($i(finalBind))
    let workhorse = (chainInfo.finalType != null && chainInfo.finalType.isWorkhorseType)
    let isDoubleAccType = (chainInfo.finalType != null && chainInfo.finalType.baseType == Type.tDouble)
    var perElementStmts <- build_accumulator_perelement_stmts(opName, accName, valBindName, firstName, cntName, valueExpr, workhorse, isDoubleAccType)
    var perElement : Expression? = stmts_to_expr(perElementStmts)
    // _count(pred): count/long_count with extra predicate — wrap perElement with `if (pred) {...}`. Predicate peels against finalBind so it sees the post-chain element.
    if ((opName == "count" || opName == "long_count") && length(terminatorCall.arguments) > 1) {
        var predExpr = peel_lambda_rename_var(terminatorCall.arguments[1], finalBind)
        if (predExpr == null) return null
        perElement = qmacro_expr() {
            if ($e(predExpr)) {
                $e(perElement)
            }
        }
    }
    // Range guards (take-cap + skip + takeC++) wrap perElement BEFORE chain so where_ predicate fires first — ranges apply to the POST-filter stream (mirrors array-side wrap_with_condition(wrap_with_ranges(...))).
    perElement = wrap_inner_for_with_decs_ranges(perElement, rangeInfo, names)
    var body = wrap_decs_chain(perElement, chainInfo, calls, intermediateEnd, at)
    if (body == null) return null
    var forExprNode = build_decs_inner_for_pruned(bridge, tupName, body, at)
    let archName = bridge.archName
    var preludeStmts : array<Expression?>
    var tailStmts : array<Expression?>
    if (opName == "long_count") {
        preludeStmts |> push <| qmacro_expr() {
            var $i(accName) = 0l
        }
        tailStmts |> push <| qmacro_expr() {
            return $i(accName)
        }
    } elif (opName == "count") {
        preludeStmts |> push <| qmacro_expr() {
            var $i(accName) = 0
        }
        tailStmts |> push <| qmacro_expr() {
            return $i(accName)
        }
    } elif (opName == "sum") {
        preludeStmts |> push <| qmacro_expr() {
            var $i(accName) : $t(accType) = default<$t(accType)>
        }
        tailStmts |> push <| qmacro_expr() {
            return $i(accName)
        }
    } elif (opName == "average") {
        preludeStmts |> push_from <| qmacro_block_to_array() {
            var $i(accName) : double = 0lf
            var $i(cntName) : uint64 = 0ul
        }
        tailStmts |> push <| qmacro_expr() {
            return $i(cntName) != 0ul ? $i(accName) / double($i(cntName)) : 0lf
        }
    } elif (opName == "min" || opName == "max") {
        // No empty-panic — matches non-decs emit_accumulator_lane (returns default-initialized acc).
        preludeStmts |> push_from <| qmacro_block_to_array() {
            var $i(firstName) = true
            var $i(accName) : $t(accType)
        }
        tailStmts |> push <| qmacro_expr() {
            return $i(accName)
        }
    } else {
        return null
    }
    var rangeStmts <- decs_range_prelude(rangeInfo, names)
    var bodyStmts : array<Expression?>
    bodyStmts |> reserve(length(preludeStmts) + length(rangeStmts) + length(tailStmts) + 1)
    bodyStmts |> push_from(preludeStmts)
    bodyStmts |> push_from(rangeStmts)
    if (rangeInfo.takeExpr != null || rangeInfo.takeWhileCond != null) {
        // Slice 5b: take_while also produces inner `return true` on its first-false (terminates iteration globally), so it needs the bool-lambda archetype-find dispatch too.
        bodyStmts |> push <| qmacro_expr() {
            for_each_archetype_find($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) : bool {
                $e(forExprNode)
                return false
            })
        }
    } else {
        bodyStmts |> push <| qmacro_expr() {
            for_each_archetype($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) {
                $e(forExprNode)
            })
        }
    }
    bodyStmts |> push_from(tailStmts)
    var retType : TypeDeclPtr
    if (opName == "long_count") {
        retType = new TypeDecl(baseType = Type.tInt64)
    } elif (opName == "count") {
        retType = new TypeDecl(baseType = Type.tInt)
    } elif (opName == "average") {
        retType = new TypeDecl(baseType = Type.tDouble)
    } else {
        retType = clone_type(accType)
    }
    var emission = qmacro(invoke($() : $t(retType) {
        $b(bodyStmts)
    }))
    emission.force_at(at)
    emission.force_generated(true)
    return emission
}

[macro_function]
def private emit_decs_early_exit(bridge : DecsBridgeShape?;
                                 opName : string;
                                 chainInfo : DecsChainInfo?;
                                 rangeInfo : DecsRangeInfo?;
                                 var calls : array<tuple<ExprCall?; LinqCall?>>;
                                 intermediateEnd : int;
                                 terminatorCall : ExprCall?;
                                 at : LineInfo) : Expression? {
    // Slice 3b/4 early-exit: first / first_or_default / any / all / contains via for_each_archetype_find (block returns true to stop archetype walk). Supports chained _select + interleaved _where via wrap_decs_chain. Slice 5a adds take/skip ranges via rangeInfo (counters hoisted at invoke scope).
    let tupName = qn("decs_tup", at)
    let foundName = qn("decs_found", at)
    let resultName = qn("decs_result", at)
    let containsValName = qn("decs_cv", at)
    let defaultName = qn("decs_dv", at)
    let names <- make_decs_range_names(at)
    let finalBind = chainInfo.finalBind
    var elemType = clone_type(chainInfo.finalType)
    var perElement : Expression?
    var preludeStmts : array<Expression?>
    var tailStmts : array<Expression?>
    if (opName == "first" || opName == "first_or_default") {
        preludeStmts |> push_from <| qmacro_block_to_array() {
            var $i(foundName) = false
            var $i(resultName) : $t(elemType)
        }
        if (opName == "first_or_default") {
            preludeStmts |> push <| qmacro_expr() {
                let $i(defaultName) = $e(terminatorCall.arguments[1])
            }
        }
        perElement = qmacro_block() {
            $i(resultName) := $i(finalBind)
            $i(foundName) = true
            return true
        }
        if (opName == "first") {
            tailStmts |> push_from <| qmacro_block_to_array() {
                if (!$i(foundName)) panic("sequence contains no elements")
                return $i(resultName)
            }
        } else {
            tailStmts |> push_from <| qmacro_block_to_array() {
                if (!$i(foundName)) return $i(defaultName)
                return $i(resultName)
            }
        }
    } elif (opName == "any") {
        let argCount = length(terminatorCall.arguments)
        // When take OR take_while is present, distinguish "match found" from "iteration-stop signal" — both produce `return true` from the inner block, so route through an explicit foundName state and use the find result only as iteration control (tail returns foundName).
        if (rangeInfo.takeExpr != null || rangeInfo.takeWhileCond != null) {
            preludeStmts |> push <| qmacro_expr() {
                var $i(foundName) = false
            }
            if (argCount > 1) {
                var predExpr = peel_lambda_rename_var(terminatorCall.arguments[1], finalBind)
                if (predExpr == null) return null
                perElement = qmacro_expr() {
                    if ($e(predExpr)) {
                        $i(foundName) = true
                        return true
                    }
                }
            } else {
                perElement = qmacro_block() {
                    $i(foundName) = true
                    return true
                }
            }
            tailStmts |> push <| qmacro_expr() {
                return $i(foundName)
            }
        } else {
            if (argCount > 1) {
                var predExpr = peel_lambda_rename_var(terminatorCall.arguments[1], finalBind)
                if (predExpr == null) return null
                perElement = qmacro_expr() {
                    if ($e(predExpr)) return true
                }
            } else {
                perElement = qmacro_expr() {
                    return true
                }
            }
        }
    } elif (opName == "all") {
        var predExpr = peel_lambda_rename_var(terminatorCall.arguments[1], finalBind)
        if (predExpr == null) return null
        if (rangeInfo.takeExpr != null || rangeInfo.takeWhileCond != null) {
            // Take-cap / take_while-stop hits produce inner `return true`; same sentinel as "counterexample found", so route through explicit foundName state ("found a !pred element"). Tail returns !foundName.
            preludeStmts |> push <| qmacro_expr() {
                var $i(foundName) = false
            }
            perElement = qmacro_expr() {
                if (!$e(predExpr)) {
                    $i(foundName) = true
                    return true
                }
            }
            tailStmts |> push <| qmacro_expr() {
                return !$i(foundName)
            }
        } else {
            perElement = qmacro_expr() {
                if (!$e(predExpr)) return true
            }
        }
    } elif (opName == "contains") {
        preludeStmts |> push <| qmacro_expr() {
            let $i(containsValName) = $e(terminatorCall.arguments[1])
        }
        if (rangeInfo.takeExpr != null || rangeInfo.takeWhileCond != null) {
            // Same disambiguation as any: take-cap / take_while-stop and "value matched" both inner-return true, route through foundName.
            preludeStmts |> push <| qmacro_expr() {
                var $i(foundName) = false
            }
            perElement = qmacro_expr() {
                if ($i(finalBind) == $i(containsValName)) {
                    $i(foundName) = true
                    return true
                }
            }
            tailStmts |> push <| qmacro_expr() {
                return $i(foundName)
            }
        } else {
            perElement = qmacro_expr() {
                if ($i(finalBind) == $i(containsValName)) return true
            }
        }
    } else {
        return null
    }
    // Range guards (take-cap + skip + takeC++) wrap perElement BEFORE chain so where_ predicate fires first — ranges apply to the POST-filter stream (mirrors array-side wrap_with_condition(wrap_with_ranges(...))).
    perElement = wrap_inner_for_with_decs_ranges(perElement, rangeInfo, names)
    var body = wrap_decs_chain(perElement, chainInfo, calls, intermediateEnd, at)
    if (body == null) return null
    var forExprNode = build_decs_inner_for_pruned(bridge, tupName, body, at)
    let archName = bridge.archName
    var rangeStmts <- decs_range_prelude(rangeInfo, names)
    // Combine prelude + invocation + tail into ONE bodyStmts list — multiple $b splices in the same qmacro fragment isolate variable scope.
    var bodyStmts : array<Expression?>
    bodyStmts |> reserve(length(preludeStmts) + length(rangeStmts) + length(tailStmts) + 1)
    bodyStmts |> push_from(preludeStmts)
    bodyStmts |> push_from(rangeStmts)
    // When takeExpr OR takeWhileCond is present, any/all/contains route their answer through explicit foundName state (set above) — the find result is unreliable because the iteration-stop signal and "real match" both produce inner `return true`. In that case, treat the find call as iteration control only and emit the return via tailStmts (which already contains `return foundName` / `return !foundName`).
    let useExplicitState = ((rangeInfo.takeExpr != null || rangeInfo.takeWhileCond != null) && (opName == "any" || opName == "all" || opName == "contains"))
    if (useExplicitState) {
        bodyStmts |> push <| qmacro_expr() {
            for_each_archetype_find($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) : bool {
                $e(forExprNode)
                return false
            })
        }
        bodyStmts |> push_from(tailStmts)
    } elif (opName == "any" || opName == "contains") {
        bodyStmts |> push <| qmacro_expr() {
            return for_each_archetype_find($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) : bool {
                $e(forExprNode)
                return false
            })
        }
    } elif (opName == "all") {
        // `all` is "no counterexample found" — inner returns true on FAIL, so negate.
        bodyStmts |> push <| qmacro_expr() {
            return !for_each_archetype_find($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) : bool {
                $e(forExprNode)
                return false
            })
        }
    } else {
        bodyStmts |> push <| qmacro_expr() {
            for_each_archetype_find($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) : bool {
                $e(forExprNode)
                return false
            })
        }
        bodyStmts |> push_from(tailStmts)
    }
    var emission : Expression?
    if (opName == "first" || opName == "first_or_default") {
        emission = qmacro(invoke($() : $t(elemType) {
            $b(bodyStmts)
        }))
    } else {
        emission = qmacro(invoke($() : bool {
            $b(bodyStmts)
        }))
    }
    emission.force_at(at)
    emission.force_generated(true)
    return emission
}

[macro_function]
def private emit_decs_to_array(bridge : DecsBridgeShape?;
                               chainInfo : DecsChainInfo?;
                               rangeInfo : DecsRangeInfo?;
                               var calls : array<tuple<ExprCall?; LinqCall?>>;
                               intermediateEnd : int;
                               at : LineInfo) : Expression? {
    // Slice 3c/4 to_array: hoist `var buf` above outer for_each_archetype; per-element push_clone of post-chain value at chainInfo.finalBind. Slice 5a adds take/skip ranges via rangeInfo.
    let tupName = qn("decs_tup", at)
    let bufName = qn("decs_buf", at)
    let names <- make_decs_range_names(at)
    let finalBind = chainInfo.finalBind
    var elemType = clone_type(chainInfo.finalType)
    var perElement : Expression? = qmacro_expr() {
        $i(bufName) |> push_clone($i(finalBind))
    }
    // Range guards (take-cap + skip + takeC++) wrap perElement BEFORE chain so where_ predicate fires first — ranges apply to the POST-filter stream (mirrors array-side wrap_with_condition(wrap_with_ranges(...))).
    perElement = wrap_inner_for_with_decs_ranges(perElement, rangeInfo, names)
    var body = wrap_decs_chain(perElement, chainInfo, calls, intermediateEnd, at)
    if (body == null) return null
    var forExprNode = build_decs_inner_for_pruned(bridge, tupName, body, at)
    let archName = bridge.archName
    var rangeStmts <- decs_range_prelude(rangeInfo, names)
    var bodyStmts : array<Expression?>
    bodyStmts |> reserve(length(rangeStmts) + 3)
    bodyStmts |> push <| qmacro_expr() {
        var $i(bufName) : array<$t(elemType)>
    }
    bodyStmts |> push_from(rangeStmts)
    if (rangeInfo.takeExpr != null || rangeInfo.takeWhileCond != null) {
        // Slice 5b: take_while also produces inner `return true` on its first-false (terminates iteration globally), so it needs the bool-lambda archetype-find dispatch too.
        bodyStmts |> push <| qmacro_expr() {
            for_each_archetype_find($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) : bool {
                $e(forExprNode)
                return false
            })
        }
    } else {
        bodyStmts |> push <| qmacro_expr() {
            for_each_archetype($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) {
                $e(forExprNode)
            })
        }
    }
    bodyStmts |> push <| qmacro_expr() {
        return <- $i(bufName)
    }
    var emission = qmacro(invoke($() : array<$t(elemType)> {
        $b(bodyStmts)
    }))
    emission.force_at(at)
    emission.force_generated(true)
    return emission
}

[macro_function]
def private emit_decs_min_max_by(bridge : DecsBridgeShape?;
                                 opName : string;
                                 chainInfo : DecsChainInfo?;
                                 rangeInfo : DecsRangeInfo?;
                                 var calls : array<tuple<ExprCall?; LinqCall?>>;
                                 intermediateEnd : int;
                                 terminatorCall : ExprCall?;
                                 at : LineInfo) : Expression? {
    // Slice 4: min_by / max_by — track best (key, element) pair across archetypes. Element retained via `:=`, matches min_by_impl's empty→default semantics. Slice 5a adds take/skip ranges via rangeInfo.
    let tupName = qn("decs_tup", at)
    let firstName = qn("decs_first", at)
    let bestKeyName = qn("decs_bkey", at)
    let bestElemName = qn("decs_belem", at)
    let keyBindName = qn("decs_key", at)
    let names <- make_decs_range_names(at)
    let finalBind = chainInfo.finalBind
    var elemType = clone_type(chainInfo.finalType)
    var keyExpr = peel_lambda_rename_var(terminatorCall.arguments[1], finalBind)
    if (keyExpr == null || keyExpr._type == null) return null
    var keyType = clone_type(keyExpr._type)
    keyType.flags.constant = false
    keyType.flags.ref = false
    let workhorse = keyType.isWorkhorseType
    let opCmp = (opName == "min_by") ? "min" : "max"
    var compareExpr = min_max_compare(workhorse, opCmp, keyBindName, bestKeyName)
    var perElement = qmacro_block() {
        let $i(keyBindName) = $e(keyExpr)
        if ($i(firstName)) {
            $i(bestKeyName) := $i(keyBindName)
            $i(bestElemName) := $i(finalBind)
            $i(firstName) = false
        } elif ($e(compareExpr)) {
            $i(bestKeyName) := $i(keyBindName)
            $i(bestElemName) := $i(finalBind)
        }
    }
    // Range guards (take-cap + skip + takeC++) wrap perElement BEFORE chain so where_ predicate fires first — ranges apply to the POST-filter stream (mirrors array-side wrap_with_condition(wrap_with_ranges(...))).
    perElement = wrap_inner_for_with_decs_ranges(perElement, rangeInfo, names)
    var body = wrap_decs_chain(perElement, chainInfo, calls, intermediateEnd, at)
    if (body == null) return null
    var forExprNode = build_decs_inner_for_pruned(bridge, tupName, body, at)
    let archName = bridge.archName
    var rangeStmts <- decs_range_prelude(rangeInfo, names)
    var bodyStmts : array<Expression?>
    bodyStmts |> reserve(length(rangeStmts) + 5)
    bodyStmts |> push_from <| qmacro_block_to_array() {
        var $i(firstName) = true
        var $i(bestKeyName) : $t(keyType)
        var $i(bestElemName) : $t(elemType)
    }
    bodyStmts |> push_from(rangeStmts)
    if (rangeInfo.takeExpr != null || rangeInfo.takeWhileCond != null) {
        // Slice 5b: take_while also produces inner `return true` on its first-false (terminates iteration globally), so it needs the bool-lambda archetype-find dispatch too.
        bodyStmts |> push <| qmacro_expr() {
            for_each_archetype_find($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) : bool {
                $e(forExprNode)
                return false
            })
        }
    } else {
        bodyStmts |> push <| qmacro_expr() {
            for_each_archetype($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) {
                $e(forExprNode)
            })
        }
    }
    bodyStmts |> push <| qmacro_expr() {
        return $i(bestElemName)
    }
    var emission = qmacro(invoke($() : $t(elemType) {
        $b(bodyStmts)
    }))
    emission.force_at(at)
    emission.force_generated(true)
    return emission
}

[macro_function]
def private emit_decs_walk_lane(bridge : DecsBridgeShape?;
                                opName : string;
                                chainInfo : DecsChainInfo?;
                                rangeInfo : DecsRangeInfo?;
                                var calls : array<tuple<ExprCall?; LinqCall?>>;
                                intermediateEnd : int;
                                terminatorCall : ExprCall?;
                                at : LineInfo) : Expression? {
    // Slice 5f walk-lane: last/single/aggregate. State (found flag + retained element / accumulator) hoisted at invoke scope so it persists across archetype boundaries. single_or_default uses for_each_archetype_find for early-exit on 2nd match; others use plain for_each_archetype unless take/take_while is present.
    let tupName = qn("decs_tup", at)
    let names <- make_decs_range_names(at)
    let finalBind = chainInfo.finalBind
    var elemType = clone_type(chainInfo.finalType)
    var preludeStmts : array<Expression?>
    var tailStmts : array<Expression?>
    var perElement : Expression?
    var retType : TypeDeclPtr
    var earlyExitFind = false
    if (opName == "last" || opName == "last_or_default") {
        retType = clone_type(elemType)
        let foundName = qn("decs_found", at)
        let bindName = qn("decs_lst", at)
        preludeStmts |> push_from <| qmacro_block_to_array() {
            var $i(foundName) = false
            var $i(bindName) : $t(retType)
        }
        perElement = qmacro_block() {
            $i(foundName) = true
            $i(bindName) := $i(finalBind)
        }
        if (opName == "last") {
            tailStmts |> push_from <| qmacro_block_to_array() {
                if (!$i(foundName)) panic("sequence contains no elements")
                return <- $i(bindName)
            }
        } else {
            let defaultName = qn("decs_dv", at)
            let dvCanCopy = (terminatorCall.arguments[1]._type != null && terminatorCall.arguments[1]._type.canCopy)
            preludeStmts |> push(emit_default_bind(defaultName, terminatorCall.arguments[1], dvCanCopy))
            tailStmts |> push_from <| qmacro_block_to_array() {
                if (!$i(foundName)) {
                    $e(emit_default_return(defaultName, dvCanCopy))
                }
                return <- $i(bindName)
            }
        }
    } elif (opName == "single") {
        retType = clone_type(elemType)
        let foundName = qn("decs_found", at)
        let bindName = qn("decs_sng", at)
        preludeStmts |> push_from <| qmacro_block_to_array() {
            var $i(foundName) = false
            var $i(bindName) : $t(retType)
        }
        // Panic on 2nd match terminates the invoke regardless of nesting — no need for early-exit dispatch.
        perElement = qmacro_block() {
            if ($i(foundName)) panic("sequence contains more than one element")
            $i(foundName) = true
            $i(bindName) := $i(finalBind)
        }
        tailStmts |> push_from <| qmacro_block_to_array() {
            if (!$i(foundName)) panic("sequence contains no elements")
            return <- $i(bindName)
        }
    } elif (opName == "single_or_default") {
        retType = clone_type(elemType)
        let foundName = qn("decs_found", at)
        let stopName = qn("decs_stop", at)
        let bindName = qn("decs_sng", at)
        let defaultName = qn("decs_dv", at)
        let dvCanCopy = (terminatorCall.arguments[1]._type != null && terminatorCall.arguments[1]._type.canCopy)
        preludeStmts |> push_from <| qmacro_block_to_array() {
            var $i(foundName) = false
            var $i(stopName) = false
            var $i(bindName) : $t(retType)
        }
        preludeStmts |> push(emit_default_bind(defaultName, terminatorCall.arguments[1], dvCanCopy))
        // 2nd match → set stop, return true (exits inner lambda); for_each_archetype_find then stops the walk.
        perElement = qmacro_block() {
            if ($i(foundName)) {
                $i(stopName) = true
                return true
            }
            $i(foundName) = true
            $i(bindName) := $i(finalBind)
        }
        tailStmts |> push_from <| qmacro_block_to_array() {
            if ($i(stopName) || !$i(foundName)) {
                $e(emit_default_return(defaultName, dvCanCopy))
            }
            return <- $i(bindName)
        }
        earlyExitFind = true
    } elif (opName == "aggregate") {
        var seedExpr = clone_expression(terminatorCall.arguments[1])
        var aggFn = clone_expression(terminatorCall.arguments[2])
        if (seedExpr._type == null) return null
        var accType = clone_type(seedExpr._type)
        accType.flags.constant = false
        accType.flags.ref = false
        retType = clone_type(accType)
        let aggAccName = qn("decs_agg", at)
        let aggIsWorkhorse = seedExpr._type.isWorkhorseType
        if (aggIsWorkhorse) {
            preludeStmts |> push <| qmacro_expr() {
                var $i(aggAccName) : $t(accType) = $e(seedExpr)
            }
        } else {
            preludeStmts |> push <| qmacro_expr() {
                var $i(aggAccName) : $t(accType) <- $e(seedExpr)
            }
        }
        var peeled = peel_lambda_rename_2vars(aggFn, aggAccName, finalBind)
        if (peeled != null) {
            if (aggIsWorkhorse) {
                perElement = qmacro_expr() {
                    $i(aggAccName) = $e(peeled)
                }
            } else {
                perElement = qmacro_expr() {
                    $i(aggAccName) <- $e(peeled)
                }
            }
        } else {
            if (aggIsWorkhorse) {
                perElement = qmacro_expr() {
                    $i(aggAccName) = invoke($e(aggFn), $i(aggAccName), $i(finalBind))
                }
            } else {
                perElement = qmacro_expr() {
                    $i(aggAccName) <- invoke($e(aggFn), $i(aggAccName), $i(finalBind))
                }
            }
        }
        if (aggIsWorkhorse) {
            tailStmts |> push <| qmacro_expr() {
                return $i(aggAccName)
            }
        } else {
            tailStmts |> push <| qmacro_expr() {
                return <- $i(aggAccName)
            }
        }
    } else {
        return null
    }
    perElement = wrap_inner_for_with_decs_ranges(perElement, rangeInfo, names)
    var body = wrap_decs_chain(perElement, chainInfo, calls, intermediateEnd, at)
    if (body == null) return null
    var forExprNode = build_decs_inner_for_pruned(bridge, tupName, body, at)
    let archName = bridge.archName
    var rangeStmts <- decs_range_prelude(rangeInfo, names)
    var bodyStmts : array<Expression?>
    bodyStmts |> reserve(length(preludeStmts) + length(rangeStmts) + length(tailStmts) + 1)
    bodyStmts |> push_from(preludeStmts)
    bodyStmts |> push_from(rangeStmts)
    if (earlyExitFind || rangeInfo.takeExpr != null || rangeInfo.takeWhileCond != null) {
        bodyStmts |> push <| qmacro_expr() {
            for_each_archetype_find($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) : bool {
                $e(forExprNode)
                return false
            })
        }
    } else {
        bodyStmts |> push <| qmacro_expr() {
            for_each_archetype($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) {
                $e(forExprNode)
            })
        }
    }
    bodyStmts |> push_from(tailStmts)
    var emission = qmacro(invoke($() : $t(retType) {
        $b(bodyStmts)
    }))
    emission.force_at(at)
    emission.force_generated(true)
    return emission
}

[macro_function]
def private emit_decs_element_at(bridge : DecsBridgeShape?;
                                 opName : string;
                                 chainInfo : DecsChainInfo?;
                                 rangeInfo : DecsRangeInfo?;
                                 var calls : array<tuple<ExprCall?; LinqCall?>>;
                                 intermediateEnd : int;
                                 terminatorCall : ExprCall?;
                                 at : LineInfo) : Expression? {
    // Slice 5f element_at: counter-driven early-exit via for_each_archetype_find. idx + counter + found + result hoisted at invoke scope; perElement writes result + returns true on cnt == idx match.
    let tupName = qn("decs_tup", at)
    let names <- make_decs_range_names(at)
    let finalBind = chainInfo.finalBind
    var retType = clone_type(chainInfo.finalType)
    let idxName = qn("decs_idx", at)
    let cntName = qn("decs_ec", at)
    let foundName = qn("decs_found", at)
    let resultName = qn("decs_res", at)
    var preludeStmts : array<Expression?>
    var tailStmts : array<Expression?>
    preludeStmts |> push <| qmacro_expr() {
        let $i(idxName) = $e(terminatorCall.arguments[1])
    }
    if (opName == "element_at") {
        preludeStmts |> push <| qmacro_expr() {
            if ($i(idxName) < 0) panic("element index out of range")
        }
        tailStmts |> push_from <| qmacro_block_to_array() {
            if (!$i(foundName)) panic("element index out of range")
            return <- $i(resultName)
        }
    } else {
        // element_at_or_default has no user-supplied default; mirrors linq.das via `var t : TT; return <- t` (move-init, works for both copyable and non-copyable element types).
        let tName = qn("decs_emptyT", at)
        preludeStmts |> push_from <| qmacro_block_to_array() {
            if ($i(idxName) < 0) {
                var $i(tName) : $t(retType)
                return <- $i(tName)
            }
        }
        tailStmts |> push_from <| qmacro_block_to_array() {
            if (!$i(foundName)) {
                var $i(tName) : $t(retType)
                return <- $i(tName)
            }
            return <- $i(resultName)
        }
    }
    preludeStmts |> push_from <| qmacro_block_to_array() {
        var $i(cntName) = 0
        var $i(foundName) = false
        var $i(resultName) : $t(retType)
    }
    var perElement : Expression? = qmacro_block() {
        if ($i(cntName) == $i(idxName)) {
            $i(resultName) := $i(finalBind)
            $i(foundName) = true
            return true
        }
        $i(cntName) ++
    }
    perElement = wrap_inner_for_with_decs_ranges(perElement, rangeInfo, names)
    var body = wrap_decs_chain(perElement, chainInfo, calls, intermediateEnd, at)
    if (body == null) return null
    var forExprNode = build_decs_inner_for_pruned(bridge, tupName, body, at)
    let archName = bridge.archName
    var rangeStmts <- decs_range_prelude(rangeInfo, names)
    var bodyStmts : array<Expression?>
    bodyStmts |> reserve(length(preludeStmts) + length(rangeStmts) + length(tailStmts) + 1)
    bodyStmts |> push_from(preludeStmts)
    bodyStmts |> push_from(rangeStmts)
    bodyStmts |> push <| qmacro_expr() {
        for_each_archetype_find($e(bridge.reqHashExpr), $e(bridge.erqExpr), $($i(archName) : Archetype) : bool {
            $e(forExprNode)
            return false
        })
    }
    bodyStmts |> push_from(tailStmts)
    var emission = qmacro(invoke($() : $t(retType) {
        $b(bodyStmts)
    }))
    emission.force_at(at)
    emission.force_generated(true)
    return emission
}

// ── decs group_by splice (Slice 5e — state-table over for_each_archetype) ───────

[macro_function]
def private is_primitive_join_key_type(keyType : TypeDeclPtr) : bool {
    // Primitive equi-key shape accepted by the hashed-join splice. Non-primitive keys need `_::unique_key()` wrapping at probe/insert sites to match join_impl's hash semantics (daslib/linq.das:1678) — cascade those to tier-2 so join_impl handles them correctly.
    return (keyType != null
        && (keyType.baseType == Type.tInt || keyType.baseType == Type.tInt64
            || keyType.baseType == Type.tUInt || keyType.baseType == Type.tUInt64
            || keyType.baseType == Type.tInt8 || keyType.baseType == Type.tUInt8
            || keyType.baseType == Type.tInt16 || keyType.baseType == Type.tUInt16
            || keyType.baseType == Type.tString
            || keyType.baseType == Type.tFloat || keyType.baseType == Type.tDouble
            || keyType.baseType == Type.tBool))
}

// ── decs join splice (PR D2 — pattern-table stub through splice_patterns) ───────

[_macro]
def private register_decs_join_rows {
    if (!is_compiling_macros_in_module("linq_fold")) return
    splice_patterns |> emplace <| SplicePattern(
        name = "decs_join_general",
        chain <- [
            Slot(matcher = m_literal("join"),    cardinality = c_one(), capture_name = "join", arity = 5),
            Slot(matcher = m_literal("where_"),  cardinality = c_opt(), capture_name = "trailing_where", arity = 2),
            Slot(matcher = m_literal("select"),  cardinality = c_opt(), capture_name = "trailing_select"),
            Slot(matcher = m_literal("count"),   cardinality = c_opt(), capture_name = "term", arity = 1)
        ],
        requires <- [@@ < RequiresPredicate > decs_source, @@ < RequiresPredicate > decs_join_no_select_with_count],
        emit = @@ < EmitFn > emit_decs_join
    )
}

[macro_function]
def private emit_decs_join(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
    // PR D2-A: keep collect+probe inline (not via adapter_wrap_source_loop) to preserve the count-no-where bucket-length fast path.
    var joinCall = c.single["join"]
    var whereLam : Expression?
    if (c.single |> key_exists("trailing_where")) {
        whereLam = joinCall != null && c.single["trailing_where"] != null ? c.single["trailing_where"].arguments[1] : null
        if (whereLam == null) return null
    }
    var selectLam : Expression?
    if (c.single |> key_exists("trailing_select")) {
        var selCall = c.single["trailing_select"]
        if (selCall == null || selCall._type == null || selCall._type.firstType == null) return null
        selectLam = selCall.arguments[1]
        if (selectLam == null) return null
    }
    let terminatorName = (c.single |> key_exists("term")) ? "count" : ""
    // Iterator-typed implicit-to-array: emission returns array<elemType> uniformly; iterator sink would need a wrap that's not applied here. Cascade.
    if (terminatorName == "" && ctx.expr_is_iterator) return null
    // Both sides must be from_decs_template eager bridges.
    var bridgeA = (ctx.src as Decs)._0
    if (bridgeA == null) return null
    var bridgeB = extract_decs_bridge(joinCall.arguments[1])
    if (bridgeB == null) return null
    // keya/keyb are LinqJoin-macro-synthesized: $($i(la_fresh) : iterTypeA) => keya_body — 1-arg single-return. Block return type IS the key type.
    var keyaLam = joinCall.arguments[2]
    var keybLam = joinCall.arguments[3]
    if (keyaLam == null || keybLam == null || keyaLam._type == null) return null
    let keyType = strip_const_ref(clone_type(keyaLam._type.firstType))
    if (!is_primitive_join_key_type(keyType)) return null
    let tupAName = qn("decs_tup_a", at)
    let tupBName = qn("decs_tup_b", at)
    let hashName = qn("decs_hash", at)
    let cntName = qn("decs_jcnt", at)
    let bufName = qn("decs_jbuf", at)
    let bElemName = qn("decs_jb", at)
    let arrName = qn("decs_jarr", at)
    var keyaBody = peel_lambda_rename_var(keyaLam, tupAName)
    var keybBody = peel_lambda_rename_var(keybLam, tupBName)
    if (keyaBody == null || keybBody == null) return null
    var tupBindA = build_decs_tup_bind(bridgeA, tupAName, at)
    var tupBindB = build_decs_tup_bind(bridgeB, tupBName, at)
    if (tupBindA == null || tupBindB == null) return null
    let tupBType = strip_const_ref(clone_type(bridgeB.elementType))
    var collectBody <- qmacro_block_to_array() {
        // nolint:PERF006 per-key bucket size unknown ahead of time
        $i(hashName)[$e(keybBody)] |> push_clone($i(tupBName))
    }
    var collectInner = build_decs_inner_for(bridgeB, tupBindB, stmts_to_expr(collectBody), at)
    var probeStmts : array<Expression?>
    var preludeStmts : array<Expression?>
    var returnStmt : Expression?
    var invokeRetType : TypeDeclPtr
    if (terminatorName == "count") {
        invokeRetType = new TypeDecl(baseType = Type.tInt, at = at)
        preludeStmts |> push <| qmacro_expr() {
            var $i(cntName) : int = 0
        }
        if (whereLam == null) {
            // Fast path (PR D2-A guarantee): bucket-length sum at bucket granularity, never enters per-pair loop.
            probeStmts |> push <| qmacro_expr() {
                $i(cntName) += length($i(arrName))
            }
        } else {
            // HAVING-shape: bind result, evaluate predicate, conditional incr.
            var resultLam = joinCall.arguments[4]
            if (resultLam == null || resultLam._type == null || resultLam._type.firstType == null) return null
            var resultBody = peel_lambda_rename_2vars(resultLam, tupAName, bElemName)
            if (resultBody == null) return null
            let joinResultType = strip_const_ref(clone_type(resultLam._type.firstType))
            let resBindName = qn("decs_jres", at)
            var wherePred = peel_lambda_replace_var(whereLam, qmacro($i(resBindName)))
            probeStmts |> push <| qmacro_expr() {
                for ($i(bElemName) in $i(arrName)) {
                    let $i(resBindName) : $t(joinResultType) = $e(resultBody)
                    if ($e(wherePred)) {
                        $i(cntName) += 1
                    }
                }
            }
        }
        returnStmt = qmacro_expr() {
            return $i(cntName)
        }
    } else {
        var resultLam = joinCall.arguments[4]
        if (resultLam == null || resultLam._type == null || resultLam._type.firstType == null) return null
        var resultBody = peel_lambda_rename_2vars(resultLam, tupAName, bElemName)
        if (resultBody == null) return null
        // Buffer element type = after-select projection type when select is trailing, else result-lam return type.
        var resultType : TypeDeclPtr
        if (selectLam != null) {
            let selCall = c.single["trailing_select"]
            if (selCall == null || selCall._type == null || selCall._type.firstType == null) return null
            resultType = clone_type(selCall._type.firstType)
        } else {
            resultType = clone_type(resultLam._type.firstType)
        }
        if (resultType == null) return null
        invokeRetType = new TypeDecl(baseType = Type.tArray, firstType = clone_type(resultType), at = at)
        preludeStmts |> push <| qmacro_expr() {
            var $i(bufName) : array<$t(resultType)>
        }
        let needBind = selectLam != null || whereLam != null
        if (needBind) {
            let joinResultType = strip_const_ref(clone_type(resultLam._type.firstType))
            let resBindName = qn("decs_jres", at)
            var pushExpr : Expression?
            if (selectLam != null) {
                var projBody = peel_lambda_replace_var(selectLam, qmacro($i(resBindName)))
                pushExpr = qmacro($i(bufName) |> push_clone($e(projBody)))
            } else {
                pushExpr = qmacro($i(bufName) |> push_clone($i(resBindName)))
            }
            if (whereLam != null) {
                var wherePred = peel_lambda_replace_var(whereLam, qmacro($i(resBindName)))
                probeStmts |> push <| qmacro_expr() {
                    for ($i(bElemName) in $i(arrName)) {
                        let $i(resBindName) : $t(joinResultType) = $e(resultBody)
                        if ($e(wherePred)) {
                            $e(pushExpr)
                        }
                    }
                }
            } else {
                probeStmts |> push <| qmacro_expr() {
                    for ($i(bElemName) in $i(arrName)) {
                        let $i(resBindName) : $t(joinResultType) = $e(resultBody)
                        $e(pushExpr)
                    }
                }
            }
        } else {
            probeStmts |> push <| qmacro_expr() {
                for ($i(bElemName) in $i(arrName)) {
                    $i(bufName) |> push_clone($e(resultBody))
                }
            }
        }
        returnStmt = qmacro_expr() {
            return <- $i(bufName)
        }
    }
    // Probe loop: walk srca; on hash hit invoke probeStmts with the matched bucket bound as arrName. Block takes `var arr` so it matches table::get's mutating overload.
    var probeWrapBody <- qmacro_block_to_array() {
        get($i(hashName), $e(keyaBody), $(var $i(arrName) : array<$t(tupBType)>) {
            $b(probeStmts)
        })
    }
    var probeInner = build_decs_inner_for(bridgeA, tupBindA, stmts_to_expr(probeWrapBody), at)
    var allStmts : array<Expression?>
    allStmts |> push_from(preludeStmts)
    allStmts |> push <| qmacro_expr() {
        var $i(hashName) : table<$t(keyType); array<$t(tupBType)>>
    }
    allStmts |> push <| qmacro_expr() {
        for_each_archetype($e(bridgeB.reqHashExpr), $e(bridgeB.erqExpr), $($i(bridgeB.archName) : Archetype) {
            $e(collectInner)
        })
    }
    allStmts |> push <| qmacro_expr() {
        for_each_archetype($e(bridgeA.reqHashExpr), $e(bridgeA.erqExpr), $($i(bridgeA.archName) : Archetype) {
            $e(probeInner)
        })
    }
    allStmts |> push(returnStmt)
    var emission = qmacro(invoke($() : $t(invokeRetType) {
        $b(allStmts)
    }))
    return finalize_decs_emission(emission, at, false)
}

[_macro]
def private register_zip_rows {
    if (!is_compiling_macros_in_module("linq_fold")) return
    splice_patterns |> emplace <| SplicePattern(
        name = "zip_general",
        chain <- [
            Slot(matcher = m_literal("zip"), cardinality = c_one(), capture_name = "zip"),
            slot_chain_of(["where_", "select"], "head"),
            Slot(matcher = m_literal("skip"),       cardinality = c_opt(), capture_name = "skip", arity = 2),
            Slot(matcher = m_literal("skip_while"), cardinality = c_opt(), capture_name = "skip_while", arity = 2),
            Slot(matcher = m_literal("take_while"), cardinality = c_opt(), capture_name = "take_while", arity = 2),
            Slot(matcher = m_literal("take"),       cardinality = c_opt(), capture_name = "take", arity = 2),
            Slot(matcher = m_literal("reverse"),    cardinality = c_opt(), capture_name = "trailing_reverse"),
            Slot(matcher = m_alias("loop_terminator_family"), cardinality = c_opt(), capture_name = "term")
        ],
        requires <- [
            @@ < RequiresPredicate > zip_arity_2_or_3,
            @@ < RequiresPredicate > zip_no_while_after_select,
            @@ < RequiresPredicate > zip_reverse_compatible_with_term,
            @@ < RequiresPredicate > take_arg_is_int
        ],
        emit = @@ < EmitFn > emit_zip
    )
}

[macro_function]
def private emit_zip(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression? {
    // 2-source lockstep zip splice. Bare zip (array/iterator) + fused where_/select/take/skip/take_while/skip_while + count/long_count/sum/min/max/average/first/first_or_default/any/all/contains terminators. 3-arg zip(a,b,sel) pre-lowers the selector. Trailing reverse on array lane emits reverse_inplace (Theme 8 C4). Counter (count/long_count) and array lanes are built inline here; accumulator + early-exit lanes delegate to emit_accumulator_lane / emit_early_exit_lane (parallel-array form).
    var zipCall = c.single["zip"]
    let zipArgCount = zipCall.arguments |> length
    // Terminator name + counter predicate (2-arg count/long_count).
    var lastName = ""
    var counterPred : Expression?
    if (c.single |> key_exists("term")) {
        var termCall = c.single["term"]
        let candidateName = c.single_name["term"]
        let candidateArgs = termCall.arguments |> length
        if (candidateName == "count" || candidateName == "long_count") {
            if (candidateArgs != 1 && candidateArgs != 2) return null
            if (candidateArgs == 2) {
                if (termCall.arguments[1] == null) return null
                counterPred = clone_expression(termCall.arguments[1])
            }
            lastName = candidateName
        } elif (candidateName == "sum" || candidateName == "min" || candidateName == "max" || candidateName == "average" || candidateName == "first") {
            if (candidateArgs != 1) return null
            lastName = candidateName
        } elif (candidateName == "first_or_default" || candidateName == "contains") {
            if (candidateArgs != 2) return null
            lastName = candidateName
        } elif (candidateName == "any") {
            if (candidateArgs != 1 && candidateArgs != 2) return null
            lastName = candidateName
        } elif (candidateName == "all") {
            if (candidateArgs != 2) return null
            lastName = candidateName
        } else {
            // Recognized by loop_terminator_family alias but not by zip splice (e.g. last/single/element_at). Cascade.
            return null
        }
    }
    let srcAName = qn("srcA", at)
    let srcBName = qn("srcB", at)
    let bufName  = qn("buf", at)
    let accName  = qn("acc", at)
    let names <- make_range_names(at)
    let itName = qn("it", at)
    var srcAExpr = peel_each(clone_expression(zipCall.arguments[0]))
    var srcBExpr = peel_each(clone_expression(zipCall.arguments[1]))
    if (srcAExpr == null || srcAExpr._type == null || srcBExpr == null || srcBExpr._type == null) return null
    srcAExpr.genFlags.alwaysSafe = true
    srcBExpr.genFlags.alwaysSafe = true
    var srcAType = invoke_src_param_type(srcAExpr)
    var srcBType = invoke_src_param_type(srcBExpr)
    var elementType = clone_type(zipCall._type.firstType)
    var whereCond : Expression?
    var projection : Expression?
    var skipExpr : Expression?
    var takeExpr : Expression?
    var skipWhileCond : Expression?
    var takeWhileCond : Expression?
    var seenSelect = false
    let seenReverse = c.single |> key_exists("trailing_reverse")
    var allProjectionsPure = true
    if (zipArgCount == 3) {
        var resultLam = zipCall.arguments[2]
        if (resultLam == null || !(resultLam is ExprMakeBlock)) return null
        var mblk = resultLam as ExprMakeBlock
        var blk = mblk._block as ExprBlock
        if (blk == null || blk.arguments |> length != 2 || blk.list |> length != 1
                || !(blk.list[0] is ExprReturn)) return null
        var ret = blk.list[0] as ExprReturn
        if (ret.subexpr == null || ret.subexpr._type == null) return null
        var projBody = clone_expression(ret.subexpr)
        var projElemType = strip_const_ref(clone_type(ret.subexpr._type))
        var zipRules : Template
        zipRules |> replaceVariable(string(blk.arguments[0].name), qmacro($i(itName)._0))
        zipRules |> replaceVariable(string(blk.arguments[1].name), qmacro($i(itName)._1))
        projBody = apply_template(zipRules, projBody.at, projBody)
        projection = projBody
        elementType = projElemType
        seenSelect = true
        if (has_sideeffects(projection)) {
            allProjectionsPure = false
        }
    }
    // Head walk: where_/select left-to-right. After-select where is the same "chained-bind dedup" gap the imperative bailed on (line 6583).
    for (call in c.many["head"]) {
        let opName = call_norm_name(call)
        if (opName == "where_") {
            if (seenSelect) return null
            var predicate : Expression? = peel_lambda_rename_var(call.arguments[1], itName)
            if (predicate == null) return null
            if (whereCond == null) {
                whereCond = predicate
            } else {
                whereCond = qmacro($e(whereCond) && $e(predicate))
            }
        } elif (opName == "select") {
            if (seenSelect) return null
            // selectCall._type.firstType is a typedecl(result_selector(...)) formula that doesn't resolve in _fold context; pull from the projection block's return expression instead.
            var projRet = peel_lambda_single_return(call.arguments[1])
            if (projRet == null || projRet._type == null) return null
            var projElemType = strip_const_ref(clone_type(projRet._type))
            projection = peel_lambda_rename_var(call.arguments[1], itName)
            if (projection == null) return null
            elementType = projElemType
            seenSelect = true
            if (has_sideeffects(projection)) {
                allProjectionsPure = false
            }
        }
    }
    // Range / while ops from c.single (positional slots already enforce canonical order: skip → skip_while → take_while → take).
    if (c.single |> key_exists("skip")) {
        var skipArg = c.single["skip"].arguments[1]
        if (skipArg == null || skipArg._type == null || skipArg._type.baseType != Type.tInt) return null
        skipExpr = clone_expression(skipArg)
    }
    if (c.single |> key_exists("skip_while")) {
        var swArg = c.single["skip_while"].arguments[1]
        if (swArg == null) return null
        skipWhileCond = peel_lambda_rename_var(swArg, itName)
        if (skipWhileCond == null) return null
    }
    if (c.single |> key_exists("take_while")) {
        var twArg = c.single["take_while"].arguments[1]
        if (twArg == null) return null
        takeWhileCond = peel_lambda_rename_var(twArg, itName)
        if (takeWhileCond == null) return null
    }
    if (c.single |> key_exists("take")) {
        var takeArg = c.single["take"].arguments[1]
        if (takeArg == null || takeArg._type == null || takeArg._type.baseType != Type.tInt) return null
        takeExpr = clone_expression(takeArg)
    }
    let vprojName = qn("vproj", at)
    let bindProjForCounter = counterPred != null && projection != null
    if (counterPred != null) {
        if (projection != null) {
            counterPred = peel_lambda_replace_var(counterPred, qmacro($i(vprojName)))
        } else {
            counterPred = peel_lambda_rename_var(counterPred, itName)
        }
        if (counterPred == null) return null
    }
    let noLimits = skipExpr == null && takeExpr == null && skipWhileCond == null && takeWhileCond == null
    let noChain = whereCond == null && projection == null && noLimits && counterPred == null
    let bothHaveLength = type_has_length(srcAExpr._type) && type_has_length(srcBExpr._type)
    let isCounter = lastName == "count" || lastName == "long_count"
    if (seenSelect && !allProjectionsPure && !noLimits) return null
    let lane = classify_terminator(lastName)
    if (lane == LinqLane.ACCUMULATOR && !isCounter) {
        if (projection == null) return null
        var preCondStmts : array<Expression?>
        preCondStmts |> push <| qmacro_expr() {     // nolint:STYLE012
            let $i(itName) = (itA, itB)
        }
        var intermediateBinds : array<Expression?>
        var laneTops <- [srcAExpr, srcBExpr]
        let laneSrcs <- [srcAName, srcBName]
        return emit_accumulator_lane(lastName, laneTops, projection, whereCond, null,
            intermediateBinds, preCondStmts, elementType, laneSrcs, accName, itName, names,
            skipExpr, takeExpr, skipWhileCond, takeWhileCond, at)
    }
    if (lane == LinqLane.EARLY_EXIT) {
        var preCondStmts : array<Expression?>
        preCondStmts |> push <| qmacro_expr() {     // nolint:STYLE012
            let $i(itName) = (itA, itB)
        }
        var intermediateBinds : array<Expression?>
        let terminatorCall = c.single["term"]
        var laneTops <- [srcAExpr, srcBExpr]
        let laneSrcs <- [srcAName, srcBName]
        return emit_early_exit_lane(lastName, laneTops, projection, whereCond, null,
            intermediateBinds, preCondStmts, elementType, terminatorCall, laneSrcs, itName, names,
            skipExpr, takeExpr, skipWhileCond, takeWhileCond, at)
    }
    // ZipShape feeds adapter_wrap_invoke; the for(itA,itB in srcA,srcB) is built locally because emit_zip prepends the optional `let it = (itA,itB)` bind based on needsItBind (D2-D — only emit_zip knows).
    var zipShape = ZipShape(
        srcAExpr = srcAExpr,
        srcBExpr = srcBExpr,
        srcAName := srcAName,
        srcBName := srcBName,
        srcAType = srcAType,
        srcBType = srcBType
    )
    var adapter = SourceAdapter(Zip = zipShape)
    if (noChain && bothHaveLength && isCounter) {
        var bodyStmts : array<Expression?>
        if (lastName == "count") {
            bodyStmts |> push <| qmacro_expr() {
                return length($i(srcAName)) < length($i(srcBName)) ? length($i(srcAName)) : length($i(srcBName))
            }
        } else {
            bodyStmts |> push <| qmacro_expr() {
                return int64(length($i(srcAName)) < length($i(srcBName)) ? length($i(srcAName)) : length($i(srcBName)))
            }
        }
        return adapter_wrap_invoke(adapter, bodyStmts, null, false, at)
    }
    var perStmts : array<Expression?>
    if (isCounter) {
        if (bindProjForCounter) {
            perStmts |> push <| qmacro_expr() {
                var $i(vprojName) = $e(projection)
            }
        } elif (seenSelect && !allProjectionsPure) {
            let finalBindName = qn("vfinal", at)
            perStmts |> push <| qmacro_expr() {
                var $i(finalBindName) = $e(projection)
            }
        }
        if (counterPred != null) {
            perStmts |> push <| qmacro_expr() {
                if ($e(counterPred)) {
                    $i(accName) ++
                }
            }
        } else {
            perStmts |> push <| qmacro_expr() {
                $i(accName) ++
            }
        }
    } else {
        let needsItBindLocal = whereCond != null || projection != null || skipWhileCond != null || takeWhileCond != null
        if (projection != null) {
            perStmts |> push <| qmacro_expr() {
                $i(bufName) |> push_clone($e(projection))
            }
        } elif (needsItBindLocal) {
            perStmts |> push <| qmacro_expr() {
                $i(bufName) |> push_clone($i(itName))
            }
        } else {
            perStmts |> push <| qmacro_expr() {
                $i(bufName) |> push_clone((itA, itB))
            }
        }
    }
    wrap_with_ranges(perStmts, skipExpr, takeExpr, skipWhileCond, takeWhileCond, names)
    var perBody = wrap_with_condition(stmts_to_expr(perStmts), whereCond)
    let projUsedInBody = projection != null && (!isCounter || !allProjectionsPure || bindProjForCounter)
    let needsItBind = whereCond != null || projUsedInBody || skipWhileCond != null || takeWhileCond != null || counterPred != null
    if (needsItBind) {
        var preStmts : array<Expression?>
        preStmts |> reserve(2)
        preStmts |> push <| qmacro_expr() {     // nolint:STYLE012
            let $i(itName) = (itA, itB)
        }
        preStmts |> push(perBody)
        perBody = stmts_to_expr(preStmts)
    }
    var bodyStmts : array<Expression?>
    if (isCounter) {
        if (lastName == "count") {
            bodyStmts |> push <| qmacro_expr() {
                var $i(accName) = 0
            }
        } else {
            bodyStmts |> push <| qmacro_expr() {
                var $i(accName) : int64 = 0l
            }
        }
    } else {
        bodyStmts |> push <| qmacro_expr() {
            var $i(bufName) : array<$t(elementType)>
        }
        if (noChain && bothHaveLength) {
            bodyStmts |> push <| qmacro_expr() {
                $i(bufName) |> reserve(length($i(srcAName)) < length($i(srcBName)) ? length($i(srcAName)) : length($i(srcBName)))
            }
        }
    }
    append_ranges_prelude(bodyStmts, skipExpr, takeExpr, skipWhileCond, names)
    let bodyReferencesIterVars = needsItBind || !isCounter
    if (isCounter && !bodyReferencesIterVars) {
        bodyStmts |> push <| qmacro_expr() {
            for (_itA, _itB in $i(srcAName), $i(srcBName)) {
                $e(perBody)
            }
        }
    } else {
        bodyStmts |> push <| qmacro_expr() {
            for (itA, itB in $i(srcAName), $i(srcBName)) {
                $e(perBody)
            }
        }
    }
    if (isCounter) {
        bodyStmts |> push <| qmacro_expr() {
            return $i(accName)
        }
    } else {
        if (seenReverse) {
            bodyStmts |> push <| qmacro_expr() {
                _::reverse_inplace($i(bufName))
            }
        }
        if (ctx.expr_is_iterator) {
            bodyStmts |> push <| qmacro_expr() {
                return <- $i(bufName).to_sequence_move()
            }
        } else {
            bodyStmts |> push <| qmacro_expr() {
                return <- $i(bufName)
            }
        }
    }
    return adapter_wrap_invoke(adapter, bodyStmts, null, false, at)
}

[macro_function]
def private try_splice_patterns(var expr : Expression?) : Expression? {
    var (top, calls) = flatten_linq(expr)
    if (empty(calls) || expr._type == null) return null
    normalize_order_reverse(calls)
    collapse_chained_selects(calls)
    collapse_chained_wheres(calls)
    let at = calls[0]._0.at
    let exprIsIter = expr._type.isIterator
    // Decs adapter first when a from_decs_template bridge is present (Slice 5d cascade rule).
    let bridge = extract_decs_bridge(top)
    if (bridge != null) {
        let tupName = qn("decs_tup", at)
        for (p in splice_patterns) {
            var r <- match_pattern(p, calls, top)
            if (r is matched) {
                var ctx = EmitCtx(top = null,
                                  src = SourceAdapter(Decs = (bridge, tupName)),
                                  expr_is_iterator = exprIsIter)
                var result = invoke(p.emit, r as matched, ctx, at)
                if (result != null) return result
            }
        }
    }
    top = peel_each(top)
    let srcName = qn("source", at)
    for (p in splice_patterns) {
        var r <- match_pattern(p, calls, top)
        if (r is matched) {
            var topClone = clone_expression(top)
            var ctx = EmitCtx(top = topClone,
                              src = SourceAdapter(Array = (topClone, srcName)),
                              expr_is_iterator = exprIsIter)
            var result = invoke(p.emit, r as matched, ctx, at)
            if (result != null) return result
        }
    }
    return null
}

[call_macro(name="_fold")]
class private LinqFold : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        macro_verify(call.arguments |> length == 1, prog, call.at, "expecting _fold(expression)")
        macro_verify(call.arguments[0]._type != null, prog, call.at, "expecting linq expression")
        var res = try_splice_patterns(call.arguments[0])
        if (res != null) return res
        // Theme 6 — warn when a `from_decs_template` source survives every splice arm with a non-empty chain
        // (the bridge materializes a temp `res` array before the cascade). Suppress with `options _no_decs_perf_warn = true`.
        if (!(prog._options |> find_arg("_no_decs_perf_warn") ?as tBool ?? false)) {
            var (top, calls) = flatten_linq(call.arguments[0])
            if (!empty(calls) && extract_decs_bridge(top) != null) {
                to_compiler_log("{describe(call.at)}: *warning* `_fold`: from_decs_template source survived dispatch — no splice arm claimed this chain, so the bridge materializes a temp `res` array and the tier-2 cascade runs on the materialized buffer. Rewrite the chain to a recognized decs shape (see doc/source/reference/linq_fold_patterns.rst), or suppress with `options _no_decs_perf_warn = true`.\n")
            }
        }
        // Tier 2 — array-shape pipeline with `_inplace` reuse + explicit `delete`.
        res = fold_linq_default(call.arguments[0])
        if (res != null) return res
        // Tier 3 — raw passthrough.
        return clone_expression(call.arguments[0])
    }
}
