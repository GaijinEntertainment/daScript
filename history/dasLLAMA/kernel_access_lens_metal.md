# Porting the kernel-access lens to Metal

> **IMPLEMENTED** (2026-07). The lensed classes derive the
> read/write axis from their bodies (declarations deleted); un-lensed classes keep declared
> roles, now cross-checked against the same derivation by the census lint. The migration was
> validated at compile time — deriving while every declaration was still present made each of
> the 592 roles an assertion; two stale hand roles surfaced (a scatter-writer declaring
> `readwrite`, and a `readwrite` left from an older in-place implementation the field comment
> still described), and one hand pattern was made structural: owners of alias views used to
> carry the buffer-level union by hand (`readwrite` on a read-only view whose alias sibling is
> written) — alias views now derive and stage their OWN access, and the tracker's
> buffer-identity keying unions the views naturally. Deviations from
> the plan below, learned the hard way: bodies are PRE-infer at `[structure_macro]` time, so
> field accesses are bare `ExprVar` names and member access is parser-level `ExprField` with
> untyped bases — a name-only `ExprField` match collides with float4 components (`wv.y` vs a
> buffer named `y`); the field mode therefore matches bare names + explicit `self.<name>` only,
> does NOT recurse into free callees (intrinsic shims' locals would collide), and refuses
> `self` passed whole. Claims are keyed by source location, not node pointer (infer clones
> subtrees). The runtime trace-diff oracle was superseded by the compile-time cross-check.

The Vulkan tier no longer hand-writes hazard masks: a compile-time lens derives each kernel's
per-binding read/write access from its body, descriptor sets declare region bits where their
buffers are named, and the dispatch helper composes the two. This doc is the map for doing the
same under the Metal lens. The Vulkan implementation (commit `a9d2e8933`) is the worked example.

## What already exists (reuse, don't rewrite)

**`dasllama/dasllama_kernel_access.das`** — the shared analyzer, deliberately kept
backend-agnostic:

```das
def classify_global_access(fn : Function?; mod : Module?; tracked : table<string>) : AccessResult
struct AccessResult { reads, writes : table<string>; err : string }
```

Given a function and a set of tracked global names, it walks the body and reports which tracked
globals are read and which are written. Mechanics:

- write detection: `ExprCopy`/`ExprMove`/`ExprClone` left sides, compound assigns (`+=` family
  on `ExprOp2`), `++`/`--` (`ExprOp1`); the write-root peels `ExprAt`/`ExprSafeAt`/`ExprField`/
  `ExprSwizzle`/`ExprRef2Value` down to the base `ExprVar`
- a pure store claims its target node so the generic `ExprVar` pass doesn't double-count it as a
  read; read-modify-write forms count as both
- intrinsic table: `coopmatStore` writes its arg 1, `coopmatLoad` only reads — extend here for
  any Metal builtin that takes a whole array and writes through it
- interprocedural: same-module callees are analyzed recursively (memoized, cycle-guarded);
  helpers that access the globals directly Just Work
- **the ratchet**: a tracked global passed WHOLE as an argument to a call the analyzer does not
  model sets `err` — the lens turns that into a compile error instead of silently under-tracking

**`dasllama/dasllama_vulkan_lens.das`** — the consumer shape to copy:

- a `[call_macro]` (`vk_kernel_access_spec()`) that fires during infer of the requiring module —
  the whole module is parsed by then, so it can scan every kernel with zero per-kernel markup
  and no add-function timing games
- it splices the result as a **spec string constant** (`"kernel=readmask,writemask;..."`,
  decimal, name-sorted for determinism) that the runtime parses once — this dodges every
  AST-literal-construction subtlety; keep that trick
- `[vk_access(reads = "...", writes = "...", force = false)]` — the override annotation.
  Without `force` it must MATCH the derived masks (drift guard, compile error naming the diff);
  `force = true` is the deliberate lie, and the only way through when the body is unanalyzable

## The Metal mapping

Metal's `[metal_dispatch]` lens today requires `@role` on every `@ssbo` field. The port makes
`@role`'s read/write/readwrite axis **derived**, keeps everything the body cannot know
**declared**:

| today | after the port |
|---|---|
| `@role = "read" / "write" / "readwrite"` | derived from the kernel body; `@role` stays legal as an override, cross-checked like `[vk_access]` (mismatch = compile error unless forced) |
| `@role = "weight"` (never written in a step, untracked by design) | stays declared — frame-level policy, not a kernel fact |
| `@role = "alias"` (hazard lives on another field) | stays declared |
| `@off` / `@span` (exact byte ranges over runtime params) | stays declared — the ranges are what make the decode tracker finer than region bits; a body walk cannot produce symbolic spans |

So in `MetalDispatch.apply` (dasllama_metal_lens.das): instead of erroring on a missing
`@role`, run the analyzer on the kernel and use the derived direction; when `@role` IS present,
compare and enforce the `force` contract. The `hz_read`/`hz_write` emission and everything else
in the builder is unchanged — only where the direction COMES FROM changes.

## The one analyzer extension the port needs

Metal kernels are **classes**; their buffers are **fields**, not module globals. The analyzer
tracks `ExprVar` by name, and its write-root peel goes THROUGH `ExprField` to the base variable
— so `self.bx[i] = v` currently resolves to a write of `self`, not `bx`.

Extend `dasllama_kernel_access` with a field-tracking mode (second entry point or a flag):

- tracked set = the class's `@ssbo`/`@uniform` field names
- the write-root peel STOPS at the first `ExprField` whose name is in the tracked set (and
  records that name) instead of peeling to the base var; same for the read pass — visit
  `ExprField` nodes, match `name`, honor the claimed-node set
- interprocedural: methods of the same class calling each other resolve like same-module
  functions; a field passed whole to an unmodeled call hits the same ratchet

Everything else (setops, claims, intrinsic table, memoization, `err`) reuses as-is. Keep the
extension in the shared module — the point of the split is one analyzer, two lenses.

## The migration oracle (do not skip this)

The Vulkan port was proven with a trace diff, and it caught three stale hand masks that had
survived review (a write bit on a buffer the kernel never writes, two over-declared reads).
Same discipline on Metal:

1. add a trace env that logs every declared node's masks in order (Vulkan:
   `DASLLAMA_VK_HAZARD_TRACE=1` in `vhz_dep`; Metal wants the same one line in `bar_dep` and/or
   the `hz_gate` staging dump)
2. capture BEFORE traces on the current hand-annotated build — one run per arm that covers each
   recorder (the metal suites' decode + prefill + matrix arms; `run.das --arm ...`)
3. switch to derived, capture AFTER, `diff` the ordered streams **grouped by distinct
   (r, w) pattern** — every group must be either identical or adjudicated as "derivation more
   accurate than hand" (you will find stale annotations; that's the lens paying for itself)
4. then the usual gates: bit-identity on the parity arms, `DASLLAMA_METAL_HAZARD_PARANOID=1`
   == elided, STRICT run clean

## Gotchas from the Vulkan round

- new `.das` files must be listed in `modules/dasLLAMA/.das_module` or `require` fails with
  20605
- a module declared `shared private` hides ALL its symbols from direct requirers — the shared
  analyzer must stay `shared public`
- `var out : string` parameters are by-VALUE (workhorse type) — an out-param must be
  `var out : string&` (this was a real bug lint caught in the Vulkan lens)
- indexing a `const` table param is a compile error — use `t?[k] ?? d` under a `key_exists`
  guard
- aliased views of one buffer (Vulkan: `vk_wq8` over `vk_wq`) union onto one tracked slot; the
  Metal analog is two fields bound to the same buffer — that's what `alias` still declares
