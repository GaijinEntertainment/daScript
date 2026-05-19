# LINQ → DECS — design notes

Sibling of [LINQ.md](LINQ.md). Captures the design horizon for stitching linq's
`zip` family together with decs archetype iteration, so the upcoming
**BufferZip** PR is shaped with the decs angle in mind even though it doesn't
have to ship the decs bits.

Status: **discussion draft** — no PR open, no plan committed. Edited in-place
as the design firms up.

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
