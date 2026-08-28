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

- `ARCHITECTURE_IMAGE.md` - sec.2.1-2.1g: the prepared-image rail.
- `ARCHITECTURE_GPU.md` - sec.2.2b: the tensor-GEMM and fused-attention shapes that measured out.
- `ARCHITECTURE_GPU_PREFILL.md` - sec.2.2c-2.2i: the Metal prefill driver's GEMM form ladder,
  dev-W knee map, attention slab, MoE bucket rail, and chunked submission.
- `ARCHITECTURE_GPU_VULKAN.md` - sec.2.2j-2.2p: the Vulkan resident driver - the prefill window
  chain, the cm2 decode lanes and tile pick, the class-pipeline build seat, the residency plan,
  the GPU-slot marks swap, and the Q8 requant byte store.
- `ARCHITECTURE_RUNTIME.md` - sec.2.2, 2.3, 2.3a, 2.4, 2.6-2.9, 2.11, 2.12: kernel shape,
  caches, lint policy, knobs, coverage, the GPU ramp.
- `ARCHITECTURE_MEASUREMENT.md` - sec.2.5, 2.10: the benchmark rig, the tune gate, and the
  sanctioned instrumentation rails.

## 3. Inherited invariants

`ARCHITECTURE_INVARIANTS.md` carries this section.
