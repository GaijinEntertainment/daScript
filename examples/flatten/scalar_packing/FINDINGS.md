# Scalar Packing — Findings

Empirical study: does packing independent scalars into vector (float2/3/4) ops reduce
node count in the EdenSpark shader-graph backend (`examples/flatten/backend/`)? Run with
the daslang binary + the `flatten_shaders.das_project`.

## Backend cost model (the *why*)

The backend lowers a flattened shader to a node graph. An op-node carries an op name and
a **width (1–4); the width is free** — a `mul` node costs the same whether its pins are
`Float` or `Float4` (`ShaderPinType` is polymorphic, "widest input wins",
`shader_dsl_boost.das:223`). `splatX/Y/Z/W` (lane extract) and `combineFloatN` (pack
scalars → vector) are *pure overhead* nodes. So **node count is the cost, and packing N
scalar ops into one width-N vector op is N→1** — as long as the pack/unpack glue at the
boundaries doesn't exceed the saving.

## Method

`[pixel_shader(flatten=false)]` (a backend mode added for this study) emits IR for the
source **verbatim** — no inline / fold / optimize / fuse — so a hand-vectorized shader can
be A/B'd against its scalar twin with the backend producing exactly what's written. Node
count = `… | grep -c '^node '`.

Two configs:
- **flatten=off** — raw, counts every hand-written splat. *Understates* the vec win.
- **optimize on** (drop `(flatten=false)`) — the backend folds redundant splat/combine
  chains. This is the **shipping config and the fair one** to judge by.

## Results (optimize on)

| pattern | width | live-range | scalar | packed | **win** |
|---|---|---|---|---|---|
| within-cell x/y (voronoi, 1 cell) | float2 | ~10 ops | 59 | 44 | **−25%** |
| across-cell (voronoi, 2×2 block)  | float4 | ~20 ops | 169 | 72 | **−57%** |
| within-vector multi-sin (plasma)  | float4 | 1 op    | 43 | 40 | **−7%** |

`flatten=off` understated all three (61→53, 185→74, 45→**47** — plasma even looked like a
+2 *loss* before folding).

## Key insights

1. **Every case is net-positive optimize-on.** The pass is low-risk.
2. **Win scales with width × live-range** — the number of vector ops the packed value
   flows through before it is unpacked/reduced. plasma packs 4 sins but sums them
   immediately (1-op live-range) → tiny win; voronoi cells flow through ~20 vector ops
   before the min → huge win.
3. **The backend's splat/combine folding is the enabler.** It's what flips plasma from
   loss to win (folds 8 boundary splats → 2). Our packed output relies on it — hence judge
   the pass *post-optimize*, not at `flatten=off`.
4. **Transcendentals drive the big wins** — sin/cos/frac/sqrt go exactly width× (voronoi
   4-cell: `sin` 12→3, `cos` 4→1, `frac` 8→2, `sqrt` 4→1). They can't be CSE'd across
   lanes (different args), so that saving is real regardless of CSE.
5. **False friends** (look like candidates, aren't):
   - `raymarch_spheres` (10 trig) — the trig is **uniform** (depends only on `g_Time` →
     preshaded, per-draw), and both loops are **sequential** (the march accumulates depth;
     the inner loop is a sequential smooth-union fold) — not data-parallel.
   - `blur` / `loop_multi` — data-parallel but **texture-bound** (each tap is a `tex2d`,
     already float3, no scalar arithmetic to pack).

## Backend suggestion (EdenSpark's call)

A **horizontal-reduce node** `reduce(b, c, CONST) = Σ bᵢ·cᵢ·CONSTᵢ` (a generalized dot —
sum / weighted-sum / dot all fall out of it) collapses every unpack-at-a-reduction
(voronoi's min, plasma's sum) from `W splats + (W−1) ops` to **one node**, lowering the
break-even for packing. We suggest it; the backend is EdenSpark's.

## Reproduce

A/B shader pairs live in this dir. With the binary built and the project wired:

```
daslang -compile-only -project ../flatten_shaders.das_project <shader> | grep -c '^node '
```

Drop `(flatten=false)` from the `[pixel_shader]` annotation for the optimize-on number
(`sed 's/(flatten=false)//'`).
