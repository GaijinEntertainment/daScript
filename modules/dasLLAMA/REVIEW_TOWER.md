# dasLLAMA GPU Tower Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_MEDIA.md`.

**Routed from `REVIEW_GPU.md`: a diff touching the tower driver
(`dasllama/dasllama_metal_tower.das`), a kernel class or builder the tower dispatches, the
`[metal_dispatch]` emission those builders are generated from
(`dasllama/dasllama_metal_lens.das`), or the Metal ASR decoder applies this list together
with `REVIEW_GPU.md`'s.**

**A change other than a comment-only one to `dasllama/dasllama_metal_tower.das`, to the
`AttnArgs` kargs struct, to any kernel class the tower dispatches or builder the tower
borrows, to the builders' `[metal_dispatch]` emission, or to state the whole driver shares
(a module-level `g_tw_*` variable, `metal_tower_init`, `dasllama_metal_tower_register`) runs
every gate below.** One cheap path: a change confined to a function no other file under
`dasllama/` names may instead state that confinement where the run would be claimed. The
gates are the family gates `tests/test_gemma4uv.das`, `tests/test_gemma4v.das`,
`tests/test_gemma3v.das`, `test_qwen3v_tier1_metal` in `tests/test_qwen3v.das`, and
`test_qwen25v_tier1_gpu` in `tests/test_qwen25v.das`; the encoder-blocks leg's
`tests/test_whisper.das`; the conv legs' `tests/test_audio.das` and
`tests/test_audio_embedder.das`; plus a `tests/test_model_image.das` run with the `mtower`
arm, with `metal_tower_stats()`'s encode count rising across the run.

**A hook registered in `dasllama_metal_tower_register` that the gate list above does not
cover is a defect - extend the list in the same change.**

**A change to `dasllama/dasllama_metal_asr_dec.das`, to `dasllama/dasllama_metal_common.das`,
to any kernel class the ASR decoder dispatches or builder the ASR decoder borrows, or to the
builders' `[metal_dispatch]` emission, ships a `tests/test_model_image.das` run with the
`mtower` arm** - its CPU-vs-GPU transcript cells are the ASR-decoder driver's parity
instrument, and the shared common paths and borrowed kernels reach that driver with no line
of its own file touched.

**A tower driver that uploads fewer rows than the padded row count its kernels walk zeroes the
pad rows in the same upload** - `dasllama/dasllama_metal_tower.das`. A pooled buffer comes back
holding the previous encode's bytes, infinities included.

**A tower attention buffer is sized from the widest extent any kernel's grid walks - the QK
key grid walks `nk64` rows (the key count rounded up to 64), past the `mp` (the query-panel
row pad) a query panel needs - never from the query pad alone.** The pool's power-of-two
bucket hides the overrun for some row sizes and not others, so an undersized panel reads
past its allocation only on specific canvas sizes.

**A diff that changes a family's CPU encoder block loop also changes that family's chain in
`dasllama/dasllama_metal_tower.das` in the same change, dispatch for dispatch - and the
reverse.** The CPU loop is the chain's specification and the parity cells its instrument.
