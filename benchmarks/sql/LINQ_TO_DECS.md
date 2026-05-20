# LINQ → DECS — design notes

Sibling of [LINQ.md](LINQ.md). Captures the design horizon for stitching linq's
`zip` family together with decs archetype iteration.

**Status (2026-05-20):** Pieces 1-3 landed. Piece 4 is the active design surface.

| Piece | Status | PR(s) |
|---|---|---|
| 1. N-ary `zip` arities 4-8 in `daslib/linq.das` | ✅ landed | #2737 |
| 2. `plan_zip` splice arm in `daslib/linq_fold.das` (Phase 2A bare zip, 2B chain ops, accum + early-exit terminators) | ✅ landed | #2738, #2741, #2742 |
| 3. `from_decs_template(type<Foo>)` macro — eager bridge (Stage A) | 🟡 in flight | #2745 |
| 4. `plan_from_decs_template` splice in `linq_fold` + Stage B marker (the real payoff) | ⬜ next | — |
| 5. sqlite_linq N-ary zip recognition | ⬜ deferred (no concrete use case) | — |

## What this is

Today decs has two iteration surfaces:

1. **`query() $(...){body}`** — the canonical "act on each entity matching a
   component set" form. Lowered by [`DecsQueryMacro`](../../daslib/decs_boost.das#L315)
   to `for_each_archetype($(arch) { for (a, b, c in get_ro(arch,"a",...), ...) body })`
   — a multi-iterator for-loop, ONE pass per archetype, zero buffering.
2. **`from_decs($(...){})`** — pull form returning an iterator. Lowered by
   [`FromDecsMacro`](../../daslib/decs_boost.das#L633) to
   `invoke($() { var res : array<tuple>; query(...){res |> push(...)}; return res.to_sequence() })`.
   **Materializes** the entire matched set into a heap array, then yields. Plays
   nicely with linq but loses the SoA-without-buffer win.

Linq has `zip(a, b)` and `zip(a, b, c)` (lockstep + result-selector forms,
iterator + array overloads — [linq.das:3028-3157](../../daslib/linq.das#L3028)).
Both arities cascade to tier 2 in `_fold` today — `zip` is listed in
[`is_buffer_required_op`](../../daslib/linq_fold.das#L401) as a marker arm.

The bridge insight: **`zip` over decs `get_ro(arch, "name", type<T>)` calls IS
the SoA per-archetype prolog**. If linq_fold's zip planner emits a multi-source
for-loop (rather than buffering a tuple stream), `from_decs_template(type<Foo>)
|> linq_chain |> terminator` can splice straight into the
`for_each_archetype` body — same shape `query` already builds, no intermediate
`array<tuple>`.

## North star

```das
[decs_template(prefix="particle")]
struct Particle {
    pos, vel : float3
}

// Authoring form (does not yet exist):
let total = (
    from_decs_template(type<Particle>)
        ._where(p => length(p.vel) > 0.0f)
        ._select(p => length(p.pos))
        ._sum()
)

// What we want the splice to lower to (same as query() would emit):
var total = 0.0f
for_each_archetype(req, $(arch) {
    for (pos in get_ro(arch, "particle_pos", type<float3>),
         vel in get_ro(arch, "particle_vel", type<float3>)) {
        if (length(vel) > 0.0f) {
            total += length(pos)
        }
    }
})
```

No `array<Particle>` materialization. No tuple stream. Same body shape as a
hand-written `query` — composable via the linq operator catalogue, which the
query macro alone doesn't give you.

## Layered work plan

Five separable pieces; each can ship independently. Numbered in build order, not
priority order. **Only piece 1 is in the BufferZip PR scope** — the rest are
later PRs.

### 1. N-ary `zip` in `daslib/linq.das` (BufferZip PR)

Extend the existing 2-ary / 3-ary `zip` family to 4..8 arity. Each arity gets
four overloads (lockstep iterator, lockstep array, result-selector iterator,
result-selector array) plus a `zip_to_array` companion — matches the established
pattern. Naming: `zip4_impl`, `zip5_impl`, … (current convention; `zip3_impl`
exists).

Open question: 8 arity is what was suggested as "should be good." Could go
higher if there's a decs-driven reason (component-rich templates with 10+
fields). Easier to ship 8 and extend later than to overshoot.

**Tests:** functional parity vs hand-rolled `for (a, b, ..., h in srcA, srcB, ..., srcH)`
for each arity + iterator/array shape. AST tests not needed at this stage
(linq, not linq_fold).

**Docs:** `daslib/linq.rst` (or wherever zip is currently documented).

### 2. `plan_zip` splice arm in `daslib/linq_fold.das`

Recognize `zip(srcA, srcB[, srcC, ...]) [|> chain]* |> terminator` and emit:

```das
// Pseudo-skeleton (lockstep form, to_array terminator):
var inscope buf : array<...>
{
    var inscope itA <- each_or_pass(srcA)
    var inscope itB <- each_or_pass(srcB)
    // ... per source ...
    for (a, b, ... in itA, itB, ...) {
        // fused downstream: where_/select*/take/etc.
        buf |> push_clone((a, b, ...))
    }
}
return <- buf
```

Multi-iterator for-loop is the natural lowering — the compiler already iterates
the shortest source's length. **NO buffered tuple stream.** Selects and wheres
downstream fuse the same way as the existing planners.

Bail-list (cascade to tier 2):
- Mixed array+iterator sources (uniform only — matches the design constraint)
- Sources that aren't `each(arr)` / iterator-form / direct array (no nested
  fold-spliceable chain inside zip arg — that's a PR-3 extension)
- Result-selector zip (the `block<...>` arg) — defer; needs block-body splice
- Anything that lands `zip` past a `where_`/`select` in the chain (canonical
  order requires `zip` first, like the existing source slot)

Trim `zip` from `is_buffer_required_op` after this lands.

Removed marker (line 401) and corresponding cascade path.

**Tests:** functional parity + AST shape (the anti-cascade gate from
`tests/linq/test_linq_fold_ast.das` — assert no `var pass_0` tier-2 shape).

**Benchmark:** refresh `benchmarks/sql/zip_dot_product.das`; add `zip3_*`
shapes if the perf delta is interesting.

### 3. `from_decs_template(type<Foo>)` macro

New `[call_macro]` in `daslib/decs_boost.das`. Single argument: `type<Foo>`
where `Foo` is annotated `[decs_template]`. Returns an `iterator<Foo>` (or a
tuple-of-refs — open question, see "Open questions" below).

**Two-stage implementation**:

- **Stage A (eager bridge)** — produces the same shape as `from_decs(...)` does
  today: materializes via `query`, returns `to_sequence()` of the buffer.
  Composes with all existing linq ops. ~50 LOC. Lands in the same PR as
  piece 2 or shortly after.
- **Stage B (splice-able marker)** — emits a recognizable AST shape that
  `linq_fold`'s planner peels and inlines into the
  `for_each_archetype` body. The whole point of the design. Depends on
  piece 4 below.

Macro reads `type<Foo>`'s `structType.fields` (same path the
`DecsTemplate.apply` method uses to enumerate fields — see
[decs_boost.das:68](../../daslib/decs_boost.das#L68)). Build an `EcsRequest`
the same way `build_req_from_args` does ([decs_boost.das:169](../../daslib/decs_boost.das#L169))
— one `req.req |> push("{prefix}{field.name}")` per field.

**Tests:** smoke parity test (Stage A) — `from_decs_template(type<Foo>) ==
materialized-query`. Splice-shape test (Stage B) gets added when piece 4 lands.

### 4. `from_decs_template` recognizer in `linq_fold` planner

New `plan_from_decs_template` planner that detects the
`from_decs_template(type<Foo>) |> chain` shape and emits the full
`for_each_archetype($(arch) { for (a, b in get_ro(arch, "a", ...), ...) body
})` template — body fused from the downstream linq chain.

Critical reuse: the **same per-field iterator-emit logic** as the `query` macro
uses ([decs_boost.das:239 `append_iterator`](../../daslib/decs_boost.das#L239)).
Two options:

- **Option A — extract a shared helper** in `decs_boost.das` and call it from
  the linq_fold planner. Cleanest. Cross-module dependency: `linq_fold.das`
  starts depending on `decs_boost.das` (one-way; decs_boost already requires
  linq).
- **Option B — duplicate the emit logic.** Faster to ship but creates a drift
  hazard.

Likely Option A — but worth a discussion point. The cross-module dependency is
the only awkward bit (linq_fold becoming "aware of" decs). One way to avoid the
direct dep: invert it — `decs_boost` exports the planner extension via a
registration hook (`add_linq_planner`). More plumbing, but linq_fold stays
ECS-unaware.

This piece is where the **N-ary zip planner from piece 2 pays its decs dividend**.
The per-archetype emission is a zip — and the linq chain that follows is
exactly the same chain `plan_zip` already fuses.

### 5. sqlite_linq N-ary zip recognition

Currently `daslib/sqlite_linq.das` has **zero** zip references. The mapping for
`zip` over SQL sources is not obvious:
- N-ary lockstep over SQL tables isn't a standard relational operation.
- Closest analogue: `INNER JOIN` on synthetic row-number columns
  (`ROW_NUMBER() OVER ()` in SQLite).
- Or — for the common case where one side is a `_sql` source and the other is
  an in-memory array — generate an `IN (...)` clause from the array. But that's
  a different operator semantically.

**Likely outcome**: defer until there's a concrete authoring shape we want to
support. Stub in `sqlite_linq` (cascade to tier 2 — call `from_sql` to
materialize, then run the linq zip in-memory).

Note: this is the **only** piece where the user mentioned "we'll need to expand
support for that there as well" — keep on the radar but not necessarily in the
critical path.

## Decisions (locked-in design)

These are settled — captured here so future-self doesn't relitigate.

1. **`from_decs_template` element type = tuple** of refs to component fields.
   `tuple<float3 const&, float3 const&>` for `[decs_template] struct Particle{
   pos, vel : float3 }`. Zero-copy. Matches the SoA primitive already used by
   `query`'s multi-iterator for-loop body. Composes with linq operators
   through the existing tuple-element path.
2. **N-ary zip ceiling = 8.** Expand if a real use case lands that needs more.
3. **`linq_fold` does NOT require `decs_boost`.** The splice emits **names**
   (`for_each_archetype`, `get_ro`) that resolve in the consuming scope — the
   user's code already requires `decs_boost` if they're calling
   `from_decs_template`. linq_fold stays ECS-unaware at the dependency level;
   it just knows the canonical decs splice template by name. (This is the same
   pattern already used elsewhere — e.g., the planner emits `_::unique_key`
   and `_::less` without `require`-ing the modules that define them.)

## Splice complexity — where the work actually is

Per the user's framing: **per-archetype is "one set of arrays" (easy); the
outer multi-archetype loop is the tricky part.** Both `from_decs($(args){})`
and `from_decs_template(type<Foo>)` walk multiple archetypes via
`for_each_archetype` — the inner body is a multi-iterator for-loop in lockstep
over per-component arrays (= zip), but most linq operators need state hoisted
*above* the archetype loop.

Operator-by-operator hoist plan:

| Operator | Hoist | Per-archetype body | Cross-archetype concern |
|---|---|---|---|
| `count` | `var cnt = 0` above outer | `cnt += archLen` (length shortcut) or `cnt++` per element | trivial accumulator |
| `sum`/`min`/`max`/`average`/`long_count` | accumulator above outer (incl. avg's 2-slot) | inline reduce | trivial accumulator |
| `to_array` | `var buf : array<T>` above outer | `buf \|> push_clone(...)` per element | trivial; reserve sum-of-archetype-sizes upfront if cheap |
| `first` / `first_or_default(d)` | n/a — direct return | first match → `return val` | early-exit across outer loop: needs flag-or-return — `return` from outer block works because `for_each_archetype` block is callee-invoked, so a return statement inside the block exits the *block*, not the outer function. Needs either (a) wrap whole emission in `invoke($() { ... })` and use `return`, or (b) flag + `break` from `for_each_archetype` (no built-in break — would need `for_each_archetype_find`'s `bool` return shape) |
| `any` / `contains` | n/a — direct return | matching element → `return true` | same early-exit pattern; `for_each_archetype_find` is the natural fit (returns `bool`) |
| `all` | n/a — direct return | non-matching → `return false` | symmetric to any |
| `take(N)` | shared counter above outer | `cnt++; break if (cnt >= N)` (inner break) + outer-loop early-exit when N reached | needs *both*: inner break to stop archetype walk, plus signal to outer loop to stop. Same `for_each_archetype_find` pattern works (return `true` when N reached) |
| `skip(K)` | shared counter above outer | `if (remaining > 0) { remaining--; continue }` | counter survives archetype boundary; no early-exit |
| `distinct` / `distinct_by` | `var inscope seen : table<...>` above outer | streaming dedup per element | shared dedup table — exactly the same shape as the existing `plan_distinct`, just one level deeper |
| `group_by` | `var inscope tab : table<...>` above outer | per-element table update (the `tab?[uk] ?? dummy` pattern) | shared table — same shape as existing `plan_group_by` |
| `reverse` | `var buf : array<T>` above outer | per-element push | `reverse_inplace(buf)` after both loops complete |
| `order_by [+ take(N)]` | `var buf : array<T>` (or `top_n` heap) above outer | per-element push | sort / `top_n_select` after both loops |

The pattern that recurs: **hoist state above `for_each_archetype`, mutate per
element in the inner loop, finalize after both loops**. The existing planners
(`plan_distinct`, `plan_group_by`, `plan_order_family`, `plan_reverse`) already
have this two-level structure (init / per-element / finalize); the
decs-splice variant just adds one more outer loop level. Bulk of the splice
infrastructure carries over.

**Early-exit terminators** (`first`/`first_or_default`/`any`/`all`/`contains`/`take(N)`)
are the genuine new complexity. Lowering to `for_each_archetype_find` (returns
`bool`, callee can early-exit by returning `true`) is the cleanest path — the
non-find `for_each_archetype` doesn't have a break channel. This means the
splice-emission has to choose between `for_each_archetype` and
`for_each_archetype_find` based on the terminator class, mirroring the
existing per-lane planner split.

## Lingering open questions

1. **sqlite_linq zip semantics** — defer until there's an authoring shape with
   a real use case. Don't speculate now.
2. **Result-selector zip splicing** — `zip(a, b, $(l, r) => ...)` block form.
   Defer past piece 2; the unselectored form covers the decs case (multi-iter
   for-loop yields the tuple directly).
3. **`from_decs_template` + REQUIRE / REQUIRE_NOT clauses** — the existing
   `query` macro accepts `REQUIRE(c1, c2)` and `REQUIRE_NOT(c3)` annotations.
   `from_decs_template(type<Foo>)` derives the must-have set from `Foo`'s
   fields, but there's no surface for the must-not-have set. Possible answer:
   `from_decs_template(type<Foo>, REQUIRE_NOT(deleted))` as a follow-up; v1
   ships without it.

## What this doesn't change

- The existing `query` / `find_query` / `from_decs` macros stay. They're the
  canonical "act on / find / materialize" forms. `from_decs_template` is
  additive — a fluent entry point for linq-style composition.
- The decs storage layout (Archetype + Component) is untouched. All the work
  is at the macro / planner layer; per-component `array<uint8>` reinterpret
  via `get_ro` stays the SoA primitive.

## Pointers to current code

- Zip overloads: [daslib/linq.das:3028-3157](../../daslib/linq.das#L3028)
- zip in linq_fold call table: [daslib/linq_fold.das:129](../../daslib/linq_fold.das#L129)
- zip marker arm: [daslib/linq_fold.das:401](../../daslib/linq_fold.das#L401)
- Archetype + Component storage: [daslib/decs.das:48-70](../../daslib/decs.das#L48)
- `get_ro` / `get` SoA accessors: [daslib/decs.das:778-825](../../daslib/decs.das#L778)
- `for_each_archetype`: [daslib/decs.das:644](../../daslib/decs.das#L644)
- `[decs_template]` structure_macro: [daslib/decs_boost.das:50-133](../../daslib/decs_boost.das#L50)
- `query` macro lowering: [daslib/decs_boost.das:315-516](../../daslib/decs_boost.das#L315)
- `append_iterator` (per-field for-loop source emit): [daslib/decs_boost.das:239](../../daslib/decs_boost.das#L239)
- `from_decs` macro (current eager form): [daslib/decs_boost.das:633-705](../../daslib/decs_boost.das#L633)
- Existing test: [tests/linq/test_linq_from_decs.das](../../tests/linq/test_linq_from_decs.das)
- sqlite_linq (lives in dasSQLITE module): [modules/dasSQLITE/daslib/sqlite_linq.das](../../modules/dasSQLITE/daslib/sqlite_linq.das)

---

# Phase 4 — `plan_from_decs_template` splice (active design, 2026-05-20)

## Reference template — what the query macro already builds

The lowering target IS what `DecsQueryMacro.implement` already produces. From [decs_boost.das:432-510](../../daslib/decs_boost.das#L432) the relevant assembly is:

```das
// query($(pos : float3; vel : float3){ body }) lowers to:
for_each_archetype(tag_req, tag_erq) $(arch) {
    for (pos in get_ro(arch, "pos", type<float3>),
         vel in get_ro(arch, "vel", type<float3>)) {
        body
    }
}
```

Built from:
1. `var req <- build_req_from_args(qblk)` — `EcsRequest` from field list. Already handles `is_decs_template` struct expansion (auto-fans `[decs_template]` struct args into per-field component names).
2. `var erq_fun = qmacro() @@ { return <- $v(req) }` — runtime lambda producing the EcsRequest.
3. `var qloop = new ExprFor(...)` + `append_iterator(arch_name, qloop, field, prefix, suffix, const_parent, can_be_optional)` per field — builds the inner multi-iter for-loop **directly as an ExprFor AST node**, ONE iterator slot per field. **This bypasses the qmacro multi-iter splice limitation entirely.**
4. `quote() { for_each_archetype(tag_req, tag_erq) $(tag_arch) { tag_loop } }` — outer template. `apply_template` substitutes `tag_req`, `tag_erq`, `tag_arch`, `tag_loop` placeholders. For early-exit terminators, swap the outer template to `for_each_archetype_find(tag_req, tag_erq) $(tag_arch) { tag_loop; return false }`.

**Implication:** the splice has the entire query-macro emission machinery available as a building block — `append_iterator`, `quote()` + `apply_template`, `build_req_from_args`. The Phase 4 work is mostly *assembling* these existing pieces with the linq-fold chain splice on top.

## Cross-module dependency — resolved

The "linq_fold doesn't require decs_boost" lock-in (decision #3 above) is **preserved**. The splice emits names (`for_each_archetype`, `for_each_archetype_find`, `get_ro`) that resolve in user scope — same `_::` / unqualified name dispatch pattern used elsewhere in linq_fold.

What the splice DOES need from decs_boost: the EcsRequest construction logic. **Three considered resolutions:**

- **Option A: Marker form.** `from_decs_template` macro emits a splice-anchor `__from_decs_template_marker(...)` carrying the request shape pre-built (field list + prefix already resolved at macro-time). Splice consumes the marker's payload, doesn't re-derive.
- **Option B: Splice re-derives via shared `[macro_function] def build_req_from_struct(st, prefix) : EcsRequest`** in decs_boost. Splice imports it. Functional but couples linq_fold to decs_boost types (`EcsRequest`).
- **Option C (chosen, implemented in PR #2748): Pure pattern-match on the post-expansion eager-bridge AST.** Macro expansion order means `_fold` sees the fully-expanded shape (`invoke($() { var res; for_each_archetype(req_hash, erq, $(arch) { for (...) { res |> push(...) } }); return res.to_sequence() })`) — identical between `FromDecsMacro` and `FromDecsTemplateMacro`. Splice reuses `req_hash` / `erq_factory` / inner-for via `clone_expression`; no marker, no shared builder, no decs_boost change. One splice serves both source forms.

**Decision: Option C.** Single recognizer covers both source macros, no cross-module coupling, no annotation surface to maintain. Trade-off: splice depends on the eager-bridge AST shape staying stable. Mitigated by AST-gate tests + safe degradation (any pattern mismatch cascades to tier-2 eager).

## Sub-PR breakdown for Phase 4

Each sub-PR is a self-contained landable slice. Ordering reflects build dependencies + risk gradient.

### Sub-PR 4a (PR #2748, ~500 LOC): `plan_decs_eager_bridge` — pattern-match splice, count + to_array

**Implemented via Option C.** No marker, no `FromDecsTemplateMacro` change. Splice pattern-matches the post-expansion eager-bridge AST shape (same shape from both `FromDecsMacro` and `FromDecsTemplateMacro`) and replaces it with terminator-specific emission. If pattern-match fails, returns null → tier-2 cascade runs the eager bridge unchanged.

- **`extract_eager_bridge(top)`** — recognizer. Walks `invoke($() { var res; for_each_archetype(req_hash, erq_factory, $(arch) { inner_for }); return res.to_sequence() })`. Captures `req_hash` (ExprConstUInt64), `erq_factory` (ExprAddr), arch param name, cloned inner ExprFor, tuple element type, and `res` variable name (verified to match the to_sequence target). Returns `DecsBridgeInfo` or null.
- **`emit_decs_count_splice(bridge, at)`** — arch.size shortcut. No inner for-loop, no buffer:

  ```das
  invoke($() : int {
      var acc = 0
      for_each_archetype(reqHash, erqFactory, $(arch : Archetype) {
          acc += arch.size
      })
      return acc
  })
  ```
- **`emit_decs_to_array_splice(bridge, at)`** — reuses cloned inner for-loop as-is; renames bridge's captured `res` (typically the gensym `res`) → splice's gensym buffer name. Returns buffer directly (skips `to_sequence()` roundtrip):

  ```das
  invoke($() : array<tuple<...>> {
      var decs_buf : array<tuple<...>>
      for_each_archetype(reqHash, erqFactory, $(arch : Archetype) {
          for (...) { decs_buf |> push(...) }   // cloned from bridge, var renamed
      })
      return <- decs_buf
  })
  ```

**Splice fires when:** `plan_decs_eager_bridge` peels `from_decs*(...) [no chain ops] |> {to_array, count}` (calls list empty + array result type → to_array; one trailing `count` call → count splice). Anything else: returns null → cascade.

**Tests:** 4 functional parity (count over from_decs_template, count over from_decs block form, to_array, empty-archetype); 3 AST gates (count uses arch.size shortcut + no to_sequence, to_array skips to_sequence, `_select` cascades to tier-2).

### Sub-PR 4b (D3 + D4): Accumulator + early-exit terminators

Once 4a lands, adding terminator support is mechanical — leverage the existing `emit_accumulator_lane` / `emit_early_exit_lane` helpers from `plan_zip` (already multi-source via parallel arrays after PR #2742). The decs splice is conceptually "N-source zip wrapped in an outer archetype loop" — the inner emission shape matches.

- **D3.** Accumulator terminators: `sum`, `min`, `max`, `average`, `long_count`. Pattern: hoist accumulator above outer; per-element reduce in inner; emit accumulator-final after outer. `average` honors the existing 2-slot (sum + count) shape.
- **D4.** Early-exit terminators: `first`, `first_or_default(d)`, `any`, `all`, `contains`, `take(N)`. Switch outer wrap from `for_each_archetype` to `for_each_archetype_find` (block returns `bool`; `true` exits archetype walk). Wrap whole emission in `invoke($() { ... })` so the `return` from inside the block can exit the outer expression. `take(N)` shares-counter across archetypes — return `true` from block when N reached.

**Critical risk to validate during 4b:** the `for_each_archetype_find` block return semantics — does a `return false` inside the inner for-loop's body return from the block (exiting the inner loop but continuing per-archetype outer) or return from the surrounding function? Per the `query` macro's emission at [decs_boost.das:483-487](../../daslib/decs_boost.das#L483), the block ends with `return false` — meaning the block-return propagates to `for_each_archetype_find`. So a `return true` inside the body exits the archetype walk. Validate with a smoke AST test.

### Sub-PR 4c (D5): Chain-op fusion (where_ / select / take / skip / take_while / skip_while)

Once terminators work end-to-end (4a + 4b), add chain-op support — `where_`, `select`, `take(N)`, `skip(K)`, `take_while`, `skip_while` between the source and the terminator. Pattern fully mirrors `plan_zip` chain fusion from PR #2741: predicates wrap with `if (...) { continue }`; projections bind via `var inscope` in body prelude; take/skip use shared counter above outer (already in D4 for take).

**Bails:** result-selector forms, impure projections combined with skip/take_while (same as plan_zip bails — see PR #2741's `impure-proj+range` cascade).

### Sub-PR 4d (D6): Reverse + state-table terminators (distinct, group_by, order_by)

Higher-order terminators that need a buffer or table hoisted above the outer loop, then post-process after both loops complete. Bulk of the splice infrastructure carries over from existing `plan_reverse` / `plan_distinct` / `plan_group_by` / `plan_order_family` — just add one more outer-loop level.

- `distinct` / `distinct_by` — hoisted `var inscope seen : table<...>`; per-element dedup check; same shape as plan_distinct, one level deeper.
- `group_by` / `group_by_lazy` — hoisted `var inscope tab : table<...>`; per-element table update (`tab?[uk] ?? dummy` pattern); same shape as plan_group_by.
- `reverse` — hoisted buf; per-element push; `reverse_inplace(buf)` after both loops.
- `order_by [+ take(N)]` — hoisted buf or top-n heap; sort / top_n after both loops.

This is the largest sub-PR (~800 LOC). Could split further (4d-1 distinct/group_by/reverse, 4d-2 order_by) if too large to review.

## Open questions for Phase 4 (to resolve before coding)

1. **Marker payload encoding.** Synthesized ExprMakeBlock with one arg per field is the most natural shape (matches what FromDecsMacro builds internally), but the splice has to walk the block's arguments to extract names + types + prefix. Alternative: marker takes (a) prefix string + (b) struct type reference; splice walks struct fields itself. The first form is more uniform (treats from_decs and from_decs_template the same once unrolled); the second is more compact. **Lean: first form (uniform with FromDecsMacro).**

2. **`get_ro` vs `get` vs `get_default_ro` selection.** Per [decs_boost.das:200 `getter_name`](../../daslib/decs_boost.das#L200) the choice depends on field const-ness, optional-ness, default value. For `from_decs_template` v1, all fields are const-ref read (no mutation through the template iterator) — defaults to `get_ro`. Defaults values use `get_default_ro`. **Lean: reuse `getter_name` directly with `const_parent=true` to force read-only.**

3. **`from_decs_template` retains use outside `_fold`.** Today's eager bridge form must keep working when the macro is used outside a `_fold` expression — e.g. `let it : iterator<...> = from_decs_template(type<Foo>); for (x in it) { ... }`. The marker emission only fires inside `_fold`; otherwise emit the eager shape. Macro needs to detect whether it's inside a `_fold` call (parent context). **Open: how to detect parent context in a call_macro.** Alternative: always emit the marker, and add a separate `[lint_macro]` or pre-resolve pass that converts marker→eager outside `_fold`. Simpler at the macro level: emit BOTH forms wrapped in a `static_if (in_fold) { marker } else { eager }`-equivalent shape — but that's not a real thing. **Cleanest: have plan_from_decs_template ALWAYS recognize the marker form, and if it can't splice, emit the eager bridge as the cascade. This way the macro always emits the marker, and the splice OR cascade always gives something runnable.**

4. **Empty-archetype skip.** `for_each_archetype` already skips archetypes with `arch.size == 0` per [decs.das:650](../../daslib/decs.das#L650). Confirm the splice doesn't break this — the inner for-loop should not be entered for empty archetypes. Should be free since we're using the same outer template.

5. **Multi-archetype overhead vs single.** For workloads with one archetype, the splice produces `for_each_archetype` wrap overhead vs a direct single-archetype loop. Acceptable — the multi-archetype generality is the whole point. (The eager bridge has the same overhead via query macro.)

## Tests + benchmarks to write alongside 4a

**Functional parity:** create entities with 1, 2, 3 archetypes (mix of components); confirm splice output matches eager-bridge output element-for-element. Same test scaffold as test_linq_from_decs.das with `restart()` for isolation.

**AST shape gates:**
- Splice emission contains `for_each_archetype` call (not `query`).
- Splice emission contains ONE inner `ExprFor` (not nested).
- Splice emission contains NO `invoke(${ ... query ... })` cascade leak (the eager bridge fallback shape).
- For early-exit (4b): switches to `for_each_archetype_find`; body contains `return true` at the exit point.

**Benchmark:** `benchmarks/sql/decs_count_from_template.das`, `benchmarks/sql/decs_select_sum_from_template.das` — compare splice vs eager bridge vs hand-written `query` macro. Splice should match `query` and beat eager bridge by the buffer-roundtrip cost (one `array<tuple>` materialize-plus-iterator-roundtrip per matched archetype).

## Validation spikes (2026-05-20 — completed)

Three spikes run to confirm the approach holds. Results changed the architecture meaningfully.

### Spike 1 — `for_each_archetype_find` block-return: ✅ validated by API contract

[decs.das:699](../../daslib/decs.das#L699) signature: `block<(arch : Archetype) : bool>` — `return true` from block stops iteration. Existing `find_query` macro at [decs_boost.das:483-487](../../daslib/decs_boost.das#L483) uses this in production. No runtime spike needed.

### Spike 3 — marker round-trip: ✅ validated, 5/5 tests pass

A regular daslang function returning `iterator<T>` survives `_fold`'s planner cascade gracefully:
- Bare `_fold(_marker_iter(30))` → tier-3 passthrough, runtime calls function.
- `_fold(_marker_iter(0).count())` → counter-lane splice fires past the unrecognized source!
- `_fold(_marker_iter(0) |> where_(p) |> sum())` → splice fires past unrecognized source.

So an iterator-returning marker IS a viable handle for the planner to recognize — even unrecognized sources cascade safely.

### Spike 2 (revised) — macro expansion order: ⚠️ critical finding

`mcp__daslang__ast_dump --mode=source` on `_fold(from_decs_template(type<Foo>).count())` shows that **by the time `_fold` runs, BOTH `from_decs_template` AND `query` have already expanded fully**. `_fold` receives raw `for_each_archetype(req_hash, erq_factory, $(arch) { for (val in get_ro(arch, ...)) { res |> push(...) } })` — no `query()` call, no `from_decs_template()` call.

**Implication:** "emit a `query(...)` call in the splice and let it re-expand" doesn't apply here — the splice has only AST to pattern-match against. Two reasonable resolutions:

- **Walk-and-rebuild from the post-expansion AST.** Workable but brittle (depends on the exact shape of the eager bridge).
- **Tag-the-block-with-an-annotation (recommended).** FromDecsTemplateMacro attaches `[from_decs_template_meta(prefix="...")]` to the lambda body block. Annotation is metadata-only — no compile-time effect. Planner walks AST looking for ExprInvoke whose body has this tag; finds the entry point in O(N) instead of pattern-matching the deep shape. Once found, extracts the field list from the body's `get_ro` calls (already in the AST). Then builds fresh emission for the terminator.

`AnnotationArgument` is limited to `tString/tBool/tInt/tFloat` (no list type) — the annotation can carry `prefix` (string) but not the full field list. That's fine — the field list is reconstructible from the for_each_archetype's inner for-loop iterators + get_ro calls. Annotation only needs to mark the entry point.

## Updated Phase 4 plan (post-validation)

The work is now:

1. **Add `[block_macro(name="from_decs_template_meta")]` to `daslib/decs_boost.das`.** Class extends `AstBlockAnnotation`. `apply` is a no-op (return `true`). Just a tag.
2. **Modify FromDecsTemplateMacro** to attach the tag annotation to the lambda body block as part of the eager-bridge emission. Annotation arg: `prefix = "<resolved prefix>"`. No other behavior change.
3. **Add `plan_from_decs_template` planner** in `daslib/linq_fold.das`. Slot between `plan_group_by` and `plan_zip`. Walks `flatten_linq(expr)` for ExprInvoke whose body block has `from_decs_template_meta` annotation. When found:
   - Extracts `prefix` from annotation args.
   - Walks the body's for_each_archetype call to extract:
     - `req_hash` (uint64 constant)
     - `erq_factory` (function pointer)
     - Field iter names (e.g. `spike_val`)
     - Field component names (from `get_ro(arch, "name", type<T>)` 2nd arg)
     - Field types (from get_ro 3rd arg)
   - Builds a new ExprInvoke with terminator-specific shell (accumulator/buffer init, new for_each_archetype + inner for, fused chain body, return).
4. **Sub-PR 4a scope (D1 + D2):** to_array + count terminators only. Tier-2 cascade for everything else (the eager bridge runs unchanged). End-to-end architecture validated, smallest possible terminator surface.
5. **4b-4d:** add accumulator/early-exit/state-table terminators following the same template.

### Why this beats the alternatives we considered

- **Pure AST pattern match** (no annotation): planner has to walk every ExprInvoke in the AST and check shape. Annotation is a fast filter.
- **Marker call** (`from_decs_template_marker(req_hash, ...)`): needs a stub function with the right return type per template, generated per call site. Annotation is lighter.
- **Compile-time helper** (`emit_for_each_archetype_invoke`): still requires the planner to extract metadata first; doesn't save the metadata-extraction step. Useful as a refactor of the macro emission code but doesn't change the planner architecture.

### Things still to verify in 4a implementation

- Annotation survives `query()` macro expansion (since `query` expands inside the annotated block, modifying the block's statement list — does it preserve the annotation list?). **High confidence yes** (annotations are on the block itself, query macro adds statements not annotations) but verify with the first AST-shape test.
- The `erq_factory` function pointer in the original `for_each_archetype` call refers to a function that returns the EcsRequest. When we build a new `for_each_archetype` for the splice, reuse the same `erq_factory` reference. Confirm function references can be cloned/reused across emissions.
- The `req_hash` is a `ExprConstUInt64` literal embedded in the for_each_archetype call. Easy to extract + reuse.
