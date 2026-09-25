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
names - says so in the PR body in place of the run.**

**The Metal gates are the family gates `tests/test_gemma4uv.das`, `tests/test_gemma4v.das`,
`tests/test_gemma3v.das`, `test_qwen3v_tier1_metal` and `test_qwen3v_tier1_metal_f16` in
`tests/test_qwen3v.das`, and `test_qwen25v_tier1_gpu` in `tests/test_qwen25v.das`;
`tests/test_whisper.das`, `tests/test_audio.das` and `tests/test_audio_embedder.das`; the StyleTTS2
synthesis gates - `test_kitten_synthesis_metal` and `test_kitten_oracle` in
`tests/test_tts_kitten.das`, `test_kokoro_synthesis_metal` and `test_kokoro_oracle` in
`tests/test_tts_kokoro.das`, and `test_metal_prefill_kernels` in
`tests/test_metal_prefill_kernels.das`; plus a `tests/test_model_image.das` run with the `mtower`
arm and no `--family` filter (`DASLLAMA_TEST_FAMILY` unset - the arm's cells span several
families, and a filter skips the rest), with `metal_tower_stats()`'s encode count rising across
the run.**

**The Vulkan gates are `test_gemma4v_vulkan_twin` in `tests/test_gemma4v.das`,
`test_gemma3v_vulkan_twin` in `tests/test_gemma3v.das`, `test_qwen3v_vulkan_twin` in
`tests/test_qwen3v.das`, `test_qwen25v_vulkan_twin` in `tests/test_qwen25v.das`, and
`tests/test_vulkan_tower_kernels.das`, with `vulkan_tower_stats()`'s encode count rising across
the twin cells.**

**A hook registered in `dasllama_metal_tower_register` or `dasllama_vulkan_tower_register` that
no gate this checklist names covers is a defect - add the gate covering it in the same change.**

**A change to `dasllama/dasllama_metal_asr_dec.das`, to `dasllama/dasllama_metal_common.das`,
or to any kernel class the ASR decoder dispatches or builder the ASR decoder borrows, ships a
`tests/test_model_image.das` run with the `mtower` arm** - its CPU-vs-GPU transcript cells are
the ASR-decoder driver's parity instrument, and the shared common paths and borrowed kernels
reach that driver with no line of its own file touched.

**A diff that adds a tower kernel, widens the rows an existing one reads, or changes the upload or
class that fills a padded buffer leaves every row past the live row count that any tower kernel
reads zeroed - by the upload that fills the buffer, or by the class that writes the padded
panel.** A reused buffer comes back holding the previous encode's bytes, infinities included.

**A tower attention buffer is sized from the largest row count any kernel's grid reads, never
from the query-row pad alone** - the Metal QK grid reads `nk64` rows (the key count rounded up to
64), the Vulkan flash tile reads whole 16-row key groups. On Metal the pool's power-of-two bucket
hides the overrun at some row sizes, so an undersized panel reads past its allocation only on
specific canvas sizes.

**A diff that changes the GPU dispatch sequence a family's encode runs in a tower driver
(`dasllama/dasllama_metal_tower.das`, `dasllama/dasllama_vulkan_tower.das`) leaves it matching,
step for step, the CPU block loop the family's registered hook replaces; a diff that changes what
the sequence computes changes that CPU loop in the same diff.** A fused dispatch matches the run of
consecutive CPU steps it replaces, computed in their CPU order. The CPU loop is the chain's
specification and the parity cells its instrument.

**A diff to the harmonic-source kernels (`MetalSt2Src*`, `dasllama/dasllama_metal_kernels.das`)
computes the CPU `sine_source`'s operations (`dasllama/dasllama_tts_blocks.das`) in the CPU's
order, with fast math off, and keeps the two GPU replacements for CPU precision: a two-float
running sum that carries its rounding error, in place of the torch law's `double` phase
accumulator, and the four-piece 2 pi reduction of the phase before `sin`; reordering an add or
dropping either replacement is a defect.** `st2_source_gate` in
`tests/test_metal_prefill_kernels.das` is the instrument; one float32 ulp of the accumulated phase
is a hundredth of a radian.

**A driver route that dispatches a borrowed kernel set - the builders one driver borrows from
another driver - is gated on every pipeline of that set having compiled, never on a subset; one
absent pipeline keeps the CPU route.** A partly-armed set dispatches into a null pipeline.
