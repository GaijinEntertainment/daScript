# dasLLAMA Architecture

The three documents divide as follows, and the division is load-bearing - the same fact written
in two of them will drift:

| doc | audience | carries |
|---|---|---|
| `README.md` | users | what each file *is*, one line, plus how to run things |
| `ARCHITECTURE.md` | me | what *belongs* in each file and why the system is shaped this way |
| `REVIEW.md` | `/code-review`, and us while writing | criteria checkable against a diff |

Shipped-arc plan docs live in `history/dasLLAMA/` (the archive log is `history/README.md`);
a `*_plan.md` in this folder is an ACTIVE arc's working plan. Passages retired from this
document are archived in `history/dasLLAMA/architecture_retired_notes.md`.

The companions carry this document's sections, each keeping its sections' numbers. The
routing block under each numbered heading below is the index: it names every companion that
section runs to, and the sections that companion holds.

---

## 1. File charters

When placement charters disagree on one function, the sec.1 charter line decides.

- `ARCHITECTURE_ENGINE.md` - sec.1.1-1.4, 1.6, 1.8, 1.9: the engine, format, load, CPU-tier,
  arch-registration, support and serving charters.
- `ARCHITECTURE_GPU.md` - sec.1.5: the GPU backend role table and the backend asymmetries.
- `ARCHITECTURE_MEDIA.md` - sec.1.7, 1.7b: the encoder-tower, audio, ASR and vision charters.

Every file under `dasllama/` appears in a companion. Two carry generated CONTENT rather
than being generated themselves: `dasllama_env.das` holds the `[EnvConfig]` knob declarations
(`ENVIRONMENT.md` is generated FROM it by `harness/gen_env_doc.das`), and `dasllama_unicode.das`
is hand-written around transcoded Unicode data tables (the RANGES/WS tables - regenerate by
re-transcoding `$LCPP/src/unicode-data.cpp`).

## 2. Mechanisms

- `ARCHITECTURE_IMAGE.md` - sec.2.1-2.1i: the prepared-image rail, the baked dev-W f16 plane,
  and the baked tower twin-W plane.
- `ARCHITECTURE_GPU.md` - sec.2.2b, 2.2w-2.2z: the tensor-GEMM and fused-attention shapes that
  measured out, the tower attention routes, the tower driver's encode chains, and the iquant
  GEMV grid read with its f4-slab twin.
- `ARCHITECTURE_GPU_PREFILL.md` - sec.2.2c-2.2i, 2.2u-2.2v, 2.2aa: the Metal prefill driver's
  GEMM form ladder, dev-W knee map, attention slab, MoE bucket rail, chunked submission, the
  f16 twin dual-store, the last-layer FFN tail, and the dense-KQ tensor mul_mm scaffold.
- `ARCHITECTURE_GPU_VULKAN.md` - sec.2.2j-2.2q: the Vulkan resident driver - the prefill window
  chain, the cm2 decode lanes and tile pick, the class-pipeline build seat, the residency plan,
  the GPU-slot marks swap, the Q8 requant byte store, and the MoE expert chain on the cm2 tiles.
- `ARCHITECTURE_GPU_VULKAN_DECODE.md` - sec.2.2r-2.2t: the per-op tier's decode era - the decode
  attention block over per-layer K/V mirrors, the streamed expert layer's GPU/CPU split, and the
  whole-token decode span.
- `ARCHITECTURE_GPU_MTP.md` - sec.2.28-2.30: the Metal speculative round over the batch driver's
  same-slab verify, the box knob that sets the depth a round drafts, and the kernel
  argument-alignment contract enforced at every dispatch.
- `ARCHITECTURE_RUNTIME.md` - sec.2.2, 2.3, 2.3a, 2.4, 2.6-2.9, 2.11, 2.12, 2.18-2.19: kernel
  shape, caches, lint policy, knobs, coverage, the GPU ramp, the hybrid worker pool, and the
  MoE region split.
- `ARCHITECTURE_MEDIA.md` - sec.2.13-2.16: the padded tower GEMM widths, the family GPU hooks,
  the tower weight lane, and the plain-Model ASR decoders.
- `ARCHITECTURE_MEASUREMENT.md` - sec.2.5, 2.10, 2.20, 2.21, 2.26-2.27: the benchmark rig, the tune
  gate, the sanctioned instrumentation rails, kernel-race fidelity, the gemv's own tune seat, and
  the CPU kernel bench's fixture conditions.
- `ARCHITECTURE_CPU_KERNELS.md` - sec.2.22-2.24: the sub-block-packed k3/k6 planes, the grid
  formats' panel and row-group decodes, and the VBMI symbol lattice.

## 3. Inherited invariants

`ARCHITECTURE_INVARIANTS.md` carries this section.
