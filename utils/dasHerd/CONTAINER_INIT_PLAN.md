# E4.5 — container default-init: plan of record

Status: PLANNED 2026-07-31; core verdicts locked with Boris the same day.
The language-core interlude between editor E4 and E5 — element-lifetime
correctness in containers lands before the editor grows on top of it.
Every file:line below was verified against master (post-#3601) on the day
of writing.

## Where this comes from (Boris, 2026-07-31)

The original design made element lifetime the user's job — the language
was for professional engine programmers. That audience assumption is
gone: daslang now targets AI-driven use and regular engineers, which
turns "resize gives zeroed-but-unconstructed elements" and "table index
auto-insert never runs initializers" from documented sharp edges into
footguns. Direction: containers construct their elements, guarded to
cost exactly nothing when construction is trivial.

## The footgun, in numbers (probe 2026-07-31, `struct Foo { x : int = 7 }`)

- `default<Foo>.x == 7` — `default<T>` applies field initializers; it is
  NOT the raw zero value (CLAUDE.md corrected in this branch).
- `resize_and_init` → 7. Correct today.
- plain `resize` → 0, and requires `unsafe` for such types
  (builtin.das:40 `static_if is_unsafe_when_uninitialized` →
  `make_function_unsafe()`).
- `tab["a"].x += 5` → **5, not 12** — the auto-inserted slot is zeroed
  (TableHash memsets on alloc runtime_table.h:519 and on erase :383 /
  :402 / :417) but initializers never run, and no `unsafe` is required
  anywhere.

## Settled verdicts (Boris, 2026-07-31)

- Policy name **`default_init_containers`** (active voice — the policy
  fixes rather than bans). **Default ON** ("we try with yes").
- `unsafe_table_lookup` (ast.h:1571) is historical; leave it alone.
- Array half: trait-driven, builtin.das only.
- Table half: builtin.das generic + C++ infer-side rewrite "with lots of
  guards as to when to call". No new C++ table builtin in v1 — freshness
  by size delta. (Boris considered a
  `_builtin_table_insert_new_flavor(tab, key, ..., new_key&)` extern:
  "not that much better". Recorded as a later optimization, only if the
  wrapper ever shows in a profile.)
- `tab[k] = v` / `tab[k] <- v` / `tab[k] := v` skip the rewrite on the
  C++ infer side **by shape alone**, regardless of needs-init.
- `resize` no longer needing `unsafe` is a feature, not fallout.
- Finalize banner (was "lint 1"): erase and clear delete removed
  elements under one banner, gated on the element having a finalizer,
  with the raw-pointer carve-out below.

## Design

### Policy + traits (C++)

- ast.h CodeOfPolicies (next to :1600): `bool default_init_containers = true;`
- InferTypes ctor (ast_infer_type.cpp:108 pattern): read
  `options default_init_containers` with the policy as fallback.
- Trait table (ast_infer_type.cpp:2549 neighborhood), two new traits:
  - `needs_container_init(type<T>)` = policy && `T->unsafeInit()`
  - `needs_nontrivial_init(type<T>)` = `T->unsafeInit()` (policy-free)
- `unsafeInit()` (ast_typedecl.cpp:1387, ast.cpp:551) is exactly the
  wanted predicate: default-initializer ctor, any field with `= init`,
  recursion through fields / fixed arrays / tuples / variants; handled
  types via `annotation->hasNonTrivialCtor()`; `safe_when_uninitialized`
  opts out. (`hasNonTrivialCtor` alone is the WRONG gate — it ignores
  `fd.init` entirely, ast.cpp:579.)
- AST serialization: policies stream at
  module_builtin_ast_serialize.cpp:2744 — add the field, bump the
  serializer version constant.
- RTTI CodeOfPolicies annotation (module_builtin_rtti.cpp:964 area):
  add the field.

### Array half (builtin.das)

- `resize` (:39 int, :46 int64) grows three arms:
  `static_if needs_container_init` → the init body;
  `static_elif is_unsafe_when_uninitialized` → `make_function_unsafe()`
  + raw call (policy-off behavior, unchanged); else raw call.
- `resize_and_init` (:53, :85): guard the default-value loop with
  `needs_nontrivial_init` — for trivially-zero element types the loop
  compiles OUT (today it rewrites zeros over memory
  `__builtin_array_resize` already zeroed — `array_resize` zero=true,
  module_builtin_array.cpp:49). The initValue overload keeps its loop
  unconditionally. The workhorse arm of the ladder becomes dead under
  the guard — simplify it away.

### Table half (C++ infer rewrite + builtin.das generic)

New generic in builtin.das; pointer return, and the rewrite wraps the
call in a compiler-generated deref, so no `unsafe` surfaces in user
code:

```
def _table_index_and_init(var Tab : table<auto(KT); auto(VT)>; key : KT) : VT? {
    let sz0 = length(Tab)              // O(1) size read
    var p = unsafe(addr(Tab[key]))     // ONE hash lookup; inserts zeroed slot if missing
    if (length(Tab) != sz0) {          // size grew ⇒ fresh slot ⇒ construct
        *p <- default<VT>              // ladder per can_move / can_copy / clone,
    }                                  //   same shape as resize_and_init
    return p
}
```

Known bound: initialization is `default<VT>`, NOT "run every initializer
reachable inside VT". `default<>` of a struct applies its field
initializers; `default<>` of a tuple/variant is zero even when its
members carry them. So the rewrite fires for a composite value type (the
`unsafeInit()` gate recurses) but lands zeros — pinned in
container_init_table.das so a future `default<>`-for-composites change
flips it consciously. Docs say `default<T>` semantics for this reason.

C++ rewrite in the ExprAt table branch (ast_infer_type.cpp:3292): when
ALL hold — policy on; `VT->unsafeInit()`; not a direct store target; not
inside module builtin (the wrapper's own `Tab[key]` must not recurse);
node is not ExprSafeAt (`tab?[k]` never inserts — untouched) — replace
with `Ptr2Ref(Call _table_index_and_init(tab, key))` and
reportAstChanged. Same machinery as `:=` → `_::clone` emission.

Store-target marking: preVisit of ExprCopy / ExprMove / ExprClone flags
a LEFT that is directly ExprAt-on-table — shape only, per the verdict.
Partial stores (`tab[k].x = 5` — left is ExprField over the At) still
rewrite the inner index: a fresh slot gets its defaults, then x = 5.
The skip on `<-` is correctness, not just perf: field initializers can
allocate (`arr : array<int> = [1, 2]`), and init-then-move-over would
leak the initializer's allocations.

Fast paths preserved by construction: a trivial VT (all-workhorse
tables — the overwhelming majority) never rewrites, so
SimNode_TableIndex + the const-string hash-baked node
(ast_simulate.cpp:1992), the JIT inline probe/insert
(llvm_jit.das:1852-2250), and AOT `das_index` (aot_cpp.das:2537) stay
exactly as they are. A nontrivial VT pays one das call + two size reads
+ a branch.

### Finalize banner (builtin.das)

Erase-family operations finalize the elements they drop, under one
policy-composed trait — the SAME policy as the init half (Boris,
2026-07-31: "fold it over. it needs the same guards"), so
`default_init_containers = false` restores the whole pre-0.6.5 world,
construct and finalize together:

```
needs_container_finalize(elemT)
  = default_init_containers && need_delete && is_safe_to_delete && !is_const
```

(AMENDED during implementation. The planned `!is_pointer` carve-out was
too narrow: linq's fold planner erases `array<tuple<ExprCall?; LinqCall?>>`
slots whose pointees stay wired into the AST — a pointer-carrying
COMPOSITE the pointer check missed, and finalizing it double-freed live
AST nodes (13 suite files died on `not a chunk pointer`). The honest
predicate already existed: `TypeDecl::isSafeToDelete()`
(ast_typedecl.cpp:1630) — false for raw pointers, lambdas, blocks, and
any composite carrying one — i.e. exactly "finalize would follow a raw
pointer". Exposed as the new `is_safe_to_delete` typeinfo trait. The
`!is_const` leg preserves the pre-existing no-finalize behavior for
const-element arrays (`delete` of const is illegal, error 30917). Bonus:
everything the gate admits deletes safely, so the banner needs no
`unsafe` blocks except table-erase's `reinterpret`.)

- Array: `erase(at)`, `erase(at, count)`, `clear`, SHRINKING `resize`
  (finalize `[newSize, oldSize)` before `__builtin_array_resize`);
  `pop` rides resize.
- Table: `erase(key)` — find first, finalize the value, then erase (the
  second lookup is paid only by finalizable value types); `clear` —
  iterate values, finalize, then clear.
- The pointer carve-out IS the design decision: for `array<T?>`,
  finalize means DELETE THE POINTEES, and `arr |> clear()` before
  `delete arr` is the documented borrowed-pointer escape precisely
  because clear does not finalize. The banner keeps that split:
  **delete = ownership teardown** (unchanged, still frees pointees),
  **erase/clear = release** (nested containers/structs get their
  generated finalize, which honors per-field `@do_not_delete`; bare
  pointer elements stay untouched). The asymmetry is the feature.
- Keys never finalize: workhorse keys are trivial, string keys belong
  to the string heap.

## Tests

Home: tests/language (already AOT-registered, runs in the per-PR
`test_aot_subset` and the jit lane — no new lane work). Written per
skills/writing_tests.md + skills/tests_in_repo.md at implementation
time. Four files:

1. **container_init_traits.das** — trait truth table.
   `needs_nontrivial_init` FALSE for: int, plain struct, `array<T>`,
   `T?`; TRUE for: field-init struct, user-default-ctor struct
   (hasDefaultInitializer), a struct nesting one, fixed_array / tuple /
   variant carrying one, a handled type with a nontrivial ctor; FALSE
   again under `[safe_when_uninitialized]` (opt-out honored).
   `needs_container_init` flips with
   `options default_init_containers = false`.
2. **container_init_resize.das** — resize on `array<Foo>` now needs no
   `unsafe` and yields x==7 (int AND int64 size overloads); `array<int>`
   resize still zeros; resize_and_init unchanged on trivial types and
   with initValue; shrink-then-grow re-initializes the regrown range;
   policy-off twin: resize demands `unsafe` again (compile-fail pin) and
   yields zeros.
3. **container_init_table.das** — fresh `tab["a"].x += 5` == 12; second
   `+= 5` == 17 (no re-init of an existing slot); partial store on a
   fresh key gives (x=5, other fields defaulted); `tab[k] = Foo(x=1)`
   gives (1, 7) — construction carries its own defaults, no double
   init; `tab[k] <- v` with allocating members — leak-free (heap
   measurement) and correct; `tab[k] := v` clone into a fresh slot;
   `tab[k]` passed as a var-ref argument initializes; trivial
   `table<string; int>` `+=` unchanged; `table<string; array<int>>`
   push-through-index unchanged (array elements are trivially-zero — the
   negative case); variant/tuple values with inits; policy-off twin
   (zeros, today's behavior).
4. **container_finalize.das** — counts finalizer calls (a `Probe` struct
   with a user `finalize` bumping a module global) for erase /
   erase_range / clear / shrink-resize / pop and table erase / clear /
   missing-key erase; survivor integrity after the erase memmove; the
   carve-out: borrowed `Foo?` pointers, the linq-planner
   tuple-of-pointers shape, and `Probe?` (counter stays 0) all survive
   erase/clear while `delete` still owns. One heap-delta assert anchors
   actual release (array clear).
   (AMENDED: heap deltas first read as broken under dastest —
   `delete arr[i]` moved `heap_bytes_allocated` not at all where the same
   code from the CLI freed 4096. Not a bug: **the default linear (bump)
   heap does not track individual frees**, and the CLI runs persistent
   (Boris, 2026-07-31). `options persistent_heap` in the test file makes
   the counter exact in both environments — probe: 0/0/4096 linear vs
   4096/4096/4096 persistent, same code, same child context. So the file
   carries that option and asserts BOTH signals per operation: the
   finalizer ran, and the bytes came back.)

## Docs and sweep

- CLAUDE.md: `default<T>` correction (DONE, rides this branch); the
  table-ops bullet gains the auto-init sentence; the "clear does NOT
  finalize" line and the borrowed-pointer escape paragraph get rewritten
  to the banner + carve-out; the resize-is-unsafe note drops.
- doc/source: document `default_init_containers` beside
  `no_unsafe_uninitialized_structures` in the options/policy reference;
  array + table reference pages get the semantics change.
- In-repo fallout: existing `unsafe(resize(...))` wraps become STYLE024
  findings once the policy lands — cleaned in the changed-set lint pass
  (pop at builtin.das:137 included).
- skills/*: any line stating "clear does not finalize" updated.

## Order of work

1. C++ slice: policy + two traits + serialization bump + rtti field
   (ast.h touch ⇒ budget the LNK1103 per-project /t:Clean dance).
2. Array half + tests 1-2.
3. Finalize banner, arrays + test 4 (array part).
4. Table half (infer rewrite + generic) + test 3.
5. Table finalize + test 4 (table part).
6. Docs + CLAUDE.md + STYLE024 fallout + full gates (test suite,
   tests-cpp-small, imgui, watcher, mcp test_tools, test_aot_subset,
   docs gate, changed-set lint, preflight mirrors).

## Deferred (out of this PR — Boris, 2026-07-31: "push the rest for later")

- fio/UTF-8 arc (tests-first) + the fread-on-pipe runtime panic that
  rides it.
- Move-assign leak / force_inscope_pod fork; parallel-array indexing
  lint; stale-nolint detection.
- The freshness-builtin optimization for the table wrapper.

## Risks

- builtin.das semantic-hash shift: JIT DLL cache self-invalidates (no
  LLVM_JIT_CODEGEN_VERSION bump — no emitter change); stale AOT objects
  rebuild.
- Infer convergence: the rewrite adds a reportAstChanged round; the
  shape skip + builtin-module skip prevent rewrite loops.
- Externals: resize/erase/clear signatures unchanged, one generic
  added — additive, no ABI-break sweep needed.
- The behavior change is the point, but two edges to watch in review:
  code that relied on zeroed table slots for init-carrying value types
  (slots are now constructed), and code that erased/cleared nested
  containers counting on the leak keeping children alive (none should
  exist; the heap tests catch in-tree surprises).
