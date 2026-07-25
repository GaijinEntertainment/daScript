# Phase 7 — texture / resource breadth (plan)

> Working plan for the Phase-7 PR. The durable record is `MASTERPLAN.md`; this doc is the
> reviewable design sketch written *before* code (the Phase-6 discipline that paid off).

## Goal

Take the image/resource surface from **combined-2D-sampler only** to full coverage, in one phase:

1. **Storage images** — `image2D`, `imageLoad`/`imageStore` (read-write descriptors).
2. **Explicit LOD / fetch** — `textureLod` (lifts the fragment-only limit), `texelFetch`.
3. **More sampler dims** — `sampler3D` / `samplerCube` / `sampler2DArray`.
4. **Separate image + sampler** — distinct `texture2D` + `sampler` globals combined at the call.

And — the reason this phase is the right place for it — upgrade the **real-GPU testing strategy** from
loose pixel-inequality spot-checks to an **exact analytic oracle**, which Phase-7's procedural image
content both *needs* and *makes possible*.

## Scope discipline

Unlike Phase 6 (a zero-feature port), Phase 7 *is* feature work — but the same gate discipline holds:
every new opcode / marker type / call lands with **a test + a census entry + spirv-val**, and
(where it touches the GPU) **a real-driver exact check**. Fail-closed on every unsupported variant
(the #3137/#3139 rule): an unknown image dim, a wrong-stage sampling op, a missing coordinate type is a
clean `error[...]`, never a silent bad blob or a panic.

## Testing strategy (the load-bearing decision)

The current GPU suite has two oracle styles: **exact** (compute reads a storage buffer back, asserts
`out[i]==i*i` — bit-exact, no reference file) and **loose** (graphics renders offscreen, then samples ~5
hand-picked pixels and asserts per-channel *inequalities* like `top.x > top.y + 40`). The loose style is
the debt: it is blind to off-by-one UV, sub-band regressions, and every unsampled pixel, and it is
re-hand-rolled per test.

**Decision: exact analytic oracle is the Phase-7 backbone; golden-image diff is a fallback used only for
genuinely filtered output.** For procedural content the expected value is CPU-computable in daslang, which
is a *stronger* oracle than a golden image and needs no committed reference:

| Content | Expected | Oracle |
|---|---|---|
| storage-image gradient `img[x,y]=f(x,y)` | `f(x,y)` in daslang | **exact** full-frame compare |
| `texelFetch` (unfiltered) / `textureLod` NEAREST | exact texel | **exact** per fetched texel |
| 3D/Cube/2DArray, each slice/face a solid color, NEAREST | the slice's color | **exact** per sampled direction |
| linear-filtered `textureLod` / mip blend | interpolated | **golden** diff, tolerance `images_close` |

**New harness (dasVulkan, where the GPU is):**
- `compute_image_rgba8(words, w, h) : array<uint8>` — the image analog of `run_compute_spirv`: storage-image
  create + GENERAL layout + dispatch + image→buffer readback. Reused by every later image phase.
- `assert_pixels_exact(t, pixels, w, h, block(x,y) -> int3)` — full-frame exact compare reporting the
  **first** mismatch (x, y, got/want). Retires the `px()` + inequality pattern for analytic cases.
- *(fallback, added only when 7.2's filtered LOD needs it)* `save_ppm`/`load_ppm` + `images_close(a,b,tol)`.

**Split unchanged:** main-tree `tests/spirv/` keeps the non-GPU layers (spirv-val + census + opcode asserts
+ golden-disassembly). All real-driver work is dasVulkan-side (lavapipe CI + local GPU), as in Phases 1/3/4.

**Debt paid down this phase:** retrofit the existing **textured-quad** gate to `assert_pixels_exact` (it
uses NEAREST + an integer-ish MVP, so it is analyzable — the highest-value retrofit, in keeping with the
"while we're here" rule). The triangle's interpolated centroid is genuinely fuzzy → left loose (or made the
one golden case if it's cheap). The root `*.ppm` are gitignored example dumps, not committed debt — nothing
to remove.

## Emitter design notes

Pre-Phase-7, `type_image` hardcoded `depth=0 arrayed=0 ms=0 sampled=1 format=Unknown` and `sampler_info`
mapped only `sampler2D → 2D`. Phase 7 parameterizes both.

- **Known-format storage images (no without-format cap).** `image2D` lowers to `OpTypeImage … sampled=2
  format=Rgba8`. A *known* color format means **no** `StorageImageReadWithoutFormat`/`…WriteWithoutFormat`
  capability or device feature is required — portable, lavapipe-safe. (`format=Unknown` would drag in those
  feature bits; avoid.) `imageLoad`/`imageStore` need no capability beyond `Shader`.
- **`type_image` gains `(arrayed, sampled, format)`** folded into its dedup key (depth/ms stay 0); the
  existing 2D-sampled callers keep the defaults. `sampler_info` returns `(dim, arrayed)`; cube sets `Dim.Cube`,
  2DArray sets `arrayed=1`, 3D sets `Dim._3D`.
- **Fetch/explicit-LOD from a combined sampler** needs the underlying image: `OpImage %img %sampledImage`
  then `OpImageFetch` / the sampled-image goes straight to `OpImageSampleExplicitLod` with a `Lod` operand.
- **Separate image+sampler:** `texture2D` → `OpTypeImage sampled=1`; `sampler` → `OpTypeSampler`; the call
  site emits `OpSampledImage %image %sampler` then samples. Both are UniformConstant descriptors with their
  own set/binding; reflection gets `sampled_image` + `sampler` kinds.

## Slices (one commit each, suite green after each)

### 7.1 Storage images — the flagship GPU gate
- **Authoring:** `var @binding = 0 img : image2D`; `imageStore(img, int2(x,y), float4(...))`,
  `imageLoad(img, int2(x,y)) : float4`. New markers + stubs in `spirv_builtins.das`.
- **Emitter:** `image2D` classify branch → UniformConstant pointer to `OpTypeImage(sampled=2, Rgba8)` +
  DescriptorSet/Binding; reflection kind `storage_image`. `imageLoad` → `OpLoad`(image) + `OpImageRead`;
  `imageStore` → `OpLoad`(image) + `OpImageWrite`. Coordinate must be an integer vector (fail-closed else).
- **Census:** + `TypeImage`(already)/`ImageRead`/`ImageWrite`. Reflection enum gains `storage_image`.
- **GPU gate (dasVulkan):** a `[compute_shader]` writes a gradient `img[x,y]=(x,y,0,255)`;
  `compute_image_rgba8` reads it back; `assert_pixels_exact` checks every pixel. Strongest possible image test.

### 7.2 Explicit LOD + texelFetch
- **Authoring:** `textureLod(s, uv, lod) : float4`, `texelFetch(s, int2 coord, int lod) : float4`.
- **Emitter:** `textureLod` → `OpImageSampleExplicitLod` (Lod operand) — **stage-agnostic** (no derivatives),
  so the fragment-only guard lifts for it; `texelFetch` → `OpImage` + `OpImageFetch`.
- **GPU gate:** `texelFetch` over a known checkerboard → exact per-texel; a vertex/compute `textureLod`
  proves the lifted stage limit. Golden fallback only if a *linear*-filtered LOD case is added.

### 7.3 More sampler dims
- **Authoring:** `sampler3D`, `samplerCube`, `sampler2DArray` markers; `texture(s, uv3)` etc.
- **Emitter:** `sampler_info` maps each to `(dim, arrayed)`; `OpTypeImage` dim/arrayed set accordingly.
- **GPU gate:** each face/slice a solid color, NEAREST → sample one direction per face → exact color compare.

### 7.4 Separate image + sampler
- **Authoring:** `var @binding=0 tex : texture2D`, `var @binding=1 smp : sampler`; `sampleTexture(tex, smp, uv)`.
- **Emitter:** `OpTypeImage`(sampled=1) + `OpTypeSampler` globals; call → `OpSampledImage` + sample.
  Reflection kinds `sampled_image` + `sampler`. Defer-if-it-fights-the-marker-rail is resolved: it's in.
- **GPU gate:** same content as 7.3's 2D case, two descriptors → exact compare (proves the split binding).

## Sequencing

7.1 → 7.2 → 7.3 → 7.4, each its own commit keeping all three tiers + census + spirv-val green; the dasVulkan
harness (`compute_image_rgba8` + `assert_pixels_exact`) lands with 7.1 and is reused by the rest. One daslang
PR (the emitter + main-tree tests) and one dasVulkan PR (the harness + GPU gates + the textured-quad retrofit),
mirroring the Phase-1/3/4 split. Standing gates on both: lint + format, Copilot-to-dry, no GC leak.
