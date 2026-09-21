# dasLLAMA GPU Tower Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU_TOWER.md`, `ARCHITECTURE_MEDIA.md`. Planned work: `followup_metal.md`.

**Routed from `REVIEW_GPU.md`: a diff that checklist routes here applies this list together
with `REVIEW_GPU.md`'s.**

**A change other than a comment-only one to `dasllama/dasllama_metal_tower.das`, to the
`AttnArgs` kernel-argument struct, to any kernel class the tower dispatches or builder the tower
borrows, to the `[metal_dispatch]` structure macro in `dasllama/dasllama_metal_lens.das`, or to
state the whole driver shares (a module-level `g_tw_*` variable, `metal_tower_init`,
`dasllama_metal_tower_register`) runs every gate this checklist names, or - where the change is
confined to functions no other file under `dasllama/` names - says so in the PR body in place
of the run.** The gates are the family gates `tests/test_gemma4uv.das`, `tests/test_gemma4v.das`,
`tests/test_gemma3v.das`, `test_qwen3v_tier1_metal` and `test_qwen3v_tier1_metal_f16` in
`tests/test_qwen3v.das`, and `test_qwen25v_tier1_gpu` in `tests/test_qwen25v.das`;
`tests/test_whisper.das`, `tests/test_audio.das` and `tests/test_audio_embedder.das`; plus a
`tests/test_model_image.das` run with the `mtower` arm, with `metal_tower_stats()`'s encode
count rising across the run.

**A hook registered in `dasllama_metal_tower_register` that no gate this checklist names covers
is a defect - add the gate covering it in the same change.**

**A change to `dasllama/dasllama_metal_asr_dec.das`, to `dasllama/dasllama_metal_common.das`,
or to any kernel class the ASR decoder dispatches or builder the ASR decoder borrows, ships a
`tests/test_model_image.das` run with the `mtower` arm** - its CPU-vs-GPU transcript cells are
the ASR-decoder driver's parity instrument, and the shared common paths and borrowed kernels
reach that driver with no line of its own file touched.

**A diff that uploads fewer rows than the padded row count the kernels walk zeroes the pad rows
in the same upload** - `dasllama/dasllama_metal_tower.das`. A pooled buffer comes back holding
the previous encode's bytes, infinities included.

**A tower attention buffer is sized from the largest row count any kernel's grid reads, never
from `mp` alone** - `mp` is the query-panel row pad, and the QK key grid reads `nk64` rows (the
key count rounded up to 64), more than `mp`. The pool's power-of-two bucket hides the overrun at
some row sizes, so an undersized panel reads past its allocation only on specific canvas sizes.

**A diff that changes a family's chain in `dasllama/dasllama_metal_tower.das` also changes that
family's CPU encoder block loop - `encoder_blocks` and `audio_encode_blocks` in
`dasllama/dasllama_audio.das`, or a vision family's own block loop in
`dasllama/dasllama_<family>.das` - in the same change, dispatch for dispatch.** The CPU loop is
the chain's specification and the parity cells its instrument.

**A driver route that dispatches a borrowed kernel set - the builders one driver borrows from
another driver - is gated on every pipeline of that set having compiled, never on a subset; one
absent pipeline keeps the CPU route.** A partly-armed set dispatches into a null pipeline.
