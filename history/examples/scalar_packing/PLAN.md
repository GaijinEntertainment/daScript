# Scalar Packing — Plan

A scalar-packing (**SLP / loop re-roll**) pass in our flatten layer. `flatten` unrolls
data-parallel loops into N scalar copies; this pass packs structurally-identical scalar
groups into width-W vector ops (SoA lanes), so the EdenSpark backend emits one width-W
node where it currently emits N scalar nodes. We produce the vectorized *demand*; their
backend's folding (+ a future horizontal-reduce node) finishes it.

See `FINDINGS.md` for the empirical justification (−7% / −25% / −57%, always positive
optimize-on, scales with width × live-range).

## Where it slots

A new pass in `daslib/flatten_opt.das`, run **after `flatten_function`** (post-unroll /
inline, so data-parallel iterations are visible as isomorphic scalar groups) and **inside
the optimize cycle**, so the existing splat/combine fold cleans our boundary glue — that
fold is what makes packing net-positive (FINDINGS #3).

## What it detects

A **lane group**: N scalar values produced by structurally-identical expression trees
whose leaf inputs differ only by the lane index or a known per-lane constant. Two sources:

- **Loop-unroll groups** (Phase 1) — the N copies `flatten` emitted from `for (i in range(N))`.
- **Hand-destructured components** (Phase 2) — independent same-shape scalar chains within
  one iteration (the x/y of a 2D quantity the source pulled apart, e.g. voronoi's `uvx`/`uvy`).

## How it packs

1. Identify the lane group + its width W (2/3/4; split N>4 into ⌈N/4⌉ groups).
2. SoA-ify: create vector vars holding the W lanes; rewrite each scalar op as the
   corresponding vector op.
3. Pack inputs at the sources (`combineFloatW`); reduce/unpack at the sinks.
4. Reductions (min/sum) → horizontal op: splat + scalar-reduce for now (folds fine), swap
   to EdenSpark's `reduce(b,c,CONST)` node when it ships.

## Phasing

- **Phase 1 — map-reduce loop re-roll (the −57% case, lowest risk).** Recognize an unrolled
  loop that is a *map-reduce*: an independent per-iteration value + an associative reduction
  (voronoi's `minDist = min(minDist, d)`, written as `d < minDist ? d : minDist`). Emit a
  vector-map across W lanes + one horizontal reduce. Prove on **voronoi** (348 → target ~150).
- **Phase 2 — general SLP.** Pack isomorphic scalar trees not from a loop (the within-vector
  −25% case). **Evaluated and declined** — see Status below and `FINDINGS.md`.

## Heuristic

Permissive (worst case still +7%), gated on **body size**: pack only when the live-range
(vector ops between pack and reduce) ≥ K. Prefer width 4. Skip **uniform/preshaded** groups
and **texture-bound** taps — no per-pixel arithmetic to save (the raymarch / blur false
friends in FINDINGS #5).

## Risks / open questions

- Reduction recognition: associativity, and the `?:`-mask form vs an explicit `min()`.
- Isomorphism-detection cost on the flat AST (the bounded post-flatten node set helps).
- Ordering vs CSE/fold: run packing *before* them, let fold clean the boundaries.
- Per-lane constants must pack into a const-vector (`float4Const`).
- Width padding: 9 cells → 2×float4 + 1, or 3×float3 — which wastes fewer lanes/nodes.

## Status

- [x] Backend cost model understood; `flatten=off` A/B mode added.
- [x] Empirical bracket measured (FINDINGS.md).
- [x] **Phase 1 — map-reduce re-roll, proven on voronoi: 348 → 171 nodes (−51%).**
- [x] **Phase 2 — general SLP: evaluated, NOT pursued.** Corpus survey showed Phase 1 already
  absorbs every high-live-range opportunity (the within-cell x/y chains are subsumed into the
  across-cell float4 component math — `FLATTEN_DUMP_DAS=1` confirmed). General SLP would add
  only the low-live-range cases: fan-into-a-common-builtin (plasma, **−7%**, one corpus
  instance) and standalone isomorphic chains (**zero** corpus instances — the candidates are
  blocked by non-vectorizable `noise`/`tex2d` leaves). Not worth the code + risk for −7% on one
  shader. Revisit only if a *client* shader surfaces a long-live-range destructured computation.
  Full reasoning in `FINDINGS.md` § "Phase 2 (general SLP) — evaluated, not pursued".

### Phase 1 — landed (`pack_map_reduce` in `daslib/flatten_opt.das`)

Pass `flatten_pack` (wrapper in flatten.das), wired as a gated `packed` stage in the backend's
cycle-2 **before** fold (`shader_dsl_boost.das`). Recognizes a min/max SSA reduction spine
(`var acc' = (d cmp acc) ? d : acc`) over N isomorphic, independent lane bodies, packs into
width-≤4 SoA groups (`plan_groups` avoids width-1 tails: 9 → [4,3,2]), and replaces the spine
with a horizontal reduce `min(init, hmin(g0), …)`.

- **Result:** voronoi 348 → 171 (−51%). Full regression 86/86 compile; A/B diff confirms **only
  voronoi changed** — zero collateral (raymarch/blur/plasma untouched). Output verified
  semantically (all 9 cells map once; reduction = `min(10, all d)`).
- **Key finding:** daslang has **no scalar→vector broadcast** for `+`/`-`/call-args (only
  `vec * scalar`). So invariants are splatted to `floatN(x)` and the downstream splat-fold
  collapses the collapsible ones — this is exactly FINDINGS #3's "fold-cleans-the-glue" pipeline.
- **Gated** on `!no_fast_math` (reduction-tree reassociation), `M ≥ 3` lanes, uniform lane layout,
  per-lane independence (no lane reads a spine accumulator or another lane's temp) — bails clean
  (no mutation) on any mismatch.

**Open tuning (not blocking):** [3,3,3] vs [4,3,2] grouping (≈equal node count); the boundary
const-vec literals (`float4(127.1,…)`) survive mad-fusion (width is free, so each is 1 node) — a
horizontal-reduce backend node (FINDINGS suggestion) would shave the unpack-at-reduction further.
