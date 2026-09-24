# dasLLAMA GPU Tower Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU_TOWER.md`, `ARCHITECTURE_MEDIA.md`. Planned work: `followup_metal.md` for
Metal, `followup_vulkan.md` for Vulkan.

**Routed from `REVIEW_GPU.md`: a diff that checklist routes here applies this list together
with `REVIEW_GPU.md`'s and `REVIEW.md`'s.**

**A change other than a comment-only one to a tower driver (`dasllama/dasllama_metal_tower.das`,
`dasllama/dasllama_vulkan_tower.das`), to a kernel-argument struct the Metal tower fills for a
dispatch (`dasllama/dasllama_metal_prefill.das`), to any kernel class a tower dispatches or builder
a tower borrows, to the `[metal_dispatch]` structure macro in `dasllama/dasllama_metal_lens.das`,
or to a module-level `g_tw_*` seat outside the tower file (`dasllama/dasllama_audio.das`) runs
every gate this checklist names for the backend the change reaches, or - where no function the
change touches is reachable from a hook `dasllama_metal_tower_register` or
`dasllama_vulkan_tower_register` registers or from a function another file under `dasllama/`
names - says so in the PR body in place of the run.** The Metal gates are the family gates
`tests/test_gemma4uv.das`, `tests/test_gemma4v.das`, `tests/test_gemma3v.das`,
`test_qwen3v_tier1_metal` and `test_qwen3v_tier1_metal_f16` in `tests/test_qwen3v.das`, and
`test_qwen25v_tier1_gpu` in `tests/test_qwen25v.das`; `tests/test_whisper.das`,
`tests/test_audio.das` and `tests/test_audio_embedder.das`; plus a `tests/test_model_image.das`
run with the `mtower` arm, with `metal_tower_stats()`'s encode count rising across the run. The
Vulkan gates are `test_gemma4v_vulkan_twin` in `tests/test_gemma4v.das`, `test_gemma3v_vulkan_twin`
in `tests/test_gemma3v.das`, `test_qwen3v_vulkan_twin` in `tests/test_qwen3v.das`,
`test_qwen25v_vulkan_twin` in `tests/test_qwen25v.das`, and `tests/test_vulkan_tower_kernels.das`,
with `vulkan_tower_stats()`'s encode count rising across the twin cells.

**A hook registered in `dasllama_metal_tower_register` or `dasllama_vulkan_tower_register` that
no gate this checklist names covers is a defect - add the gate covering it in the same change.**

**A change to `dasllama/dasllama_metal_asr_dec.das`, to `dasllama/dasllama_metal_common.das`,
or to any kernel class the ASR decoder dispatches or builder the ASR decoder borrows, ships a
`tests/test_model_image.das` run with the `mtower` arm** - its CPU-vs-GPU transcript cells are
the ASR-decoder driver's parity instrument, and the shared common paths and borrowed kernels
reach that driver with no line of its own file touched.

**Every row past the live count that a tower kernel reads holds zeros: the upload that fills the
buffer zeroes it (the Metal tower's padded upload), or the class that writes the padded panel
zeroes it (the Vulkan restride classes).** A reused buffer comes back holding the previous
encode's bytes, infinities included.

**A tower attention buffer is sized from the largest row count any kernel's grid reads, never
from the query-row pad alone** - the Metal QK grid reads `nk64` rows (the key count rounded up to
64), the Vulkan flash tile reads whole 16-row key groups. On Metal the pool's power-of-two bucket
hides the overrun at some row sizes, so an undersized panel reads past its allocation only on
specific canvas sizes.

**A diff that changes a family's chain in a tower driver (`dasllama/dasllama_metal_tower.das`,
`dasllama/dasllama_vulkan_tower.das`) also changes that family's CPU encoder block loop -
`encoder_blocks` and `audio_encode_blocks` in
`dasllama/dasllama_audio.das`, or a vision family's own block loop in
`dasllama/dasllama_<family>.das` - in the same change, dispatch for dispatch.** The CPU loop is
the chain's specification and the parity cells its instrument.

**A driver route that dispatches a borrowed kernel set - the builders one driver borrows from
another driver - is gated on every pipeline of that set having compiled, never on a subset; one
absent pipeline keeps the CPU route.** A partly-armed set dispatches into a null pipeline.
