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
`tests/test_qwen3v.das`, `test_qwen25v_vulkan_twin` in `tests/test_qwen25v.das`, `test_whisper_vulkan_twin` in
`tests/test_whisper.das`, `test_encoder_blocks_vulkan`, `test_gemma4a_vulkan_twin`,
`test_canary_vulkan_twin` and `test_qwen3a_vulkan_front` in `tests/test_audio.das`, and
`tests/test_vulkan_tower_kernels.das`; each named `test_*` cell sees the counter of every hook it
covers rise.**

**A hook registered in `dasllama_metal_tower_register` or `dasllama_vulkan_tower_register` that
no gate this checklist names covers is a defect - add the gate covering it in the same change.** A
gate covers a hook when it asserts a counter only that hook raises: on Vulkan,
`vulkan_tower_stats()`'s `encodes` and `blocks` for a blocks hook, `convs` for a front or conv
hook, `mels` for qwen3a's mel hook.

**A change to `dasllama/dasllama_metal_asr_dec.das`, to `dasllama/dasllama_metal_common.das`,
or to any kernel class the Metal ASR decoder dispatches or builder it borrows, ships a
`tests/test_model_image.das` run with the `mtower` arm; a change to
`dasllama/dasllama_vulkan_asr_dec.das`, to a kernel class it dispatches, or to a tower helper it
rides (`vt_tile*`, `vt_sched_map`, `vt_feed_enc`, the repack layout) ships a
`test_whisper_vulkan_wdec` run (`tests/test_whisper.das`)** - the CPU-vs-GPU transcript cells are
each ASR-decoder driver's parity instrument, and the shared common paths and borrowed kernels
reach a driver with no line of its own file touched.

**A diff that adds a tower kernel, widens the rows an existing one reads, or changes the upload or
class that fills a padded buffer leaves every row past the live row count that any tower kernel
reads zeroed - by the upload that fills the buffer, or by the class that writes the padded
panel.** A reused buffer comes back holding the previous encode's bytes, infinities included.

**A tower attention buffer is sized from the largest row count any kernel's grid reads, never
from the query-row pad alone** - the Metal QK grid reads `nk64` rows (the key count rounded up to
64), the Vulkan flash tile reads whole 16-row key groups. On Metal the pool's power-of-two bucket
hides the overrun at some row sizes, so an undersized panel reads past its allocation only on
specific canvas sizes.

**A diff that changes the dispatch sequence of a family's chain in
`dasllama/dasllama_metal_tower.das` or `dasllama/dasllama_vulkan_tower.das` keeps the chain
running the operations of the CPU code the family's GPU hook replaces, in the same order and at
the same operand shapes - a chain may fuse consecutive operations into one dispatch; a diff that
changes what the chain computes changes that CPU code in the same diff.** That CPU code is what the
hook's call site runs when the hook declines: for a blocks hook `encoder_blocks` or
`audio_encode_blocks` in `dasllama/dasllama_audio.das`, the block loop inside `gemma4a_encode`,
`cn_blocks_cpu`, or a vision family's own block loop; for a front or tail hook the CPU front or
tail beside its call (`cn_front_cpu`, `g4a_projector_tail`, the CPU conv stem in
`dasllama/dasllama_audio.das`, `log_mel_qwen3a`'s CPU mel).

**A diff to the harmonic-source kernels (`MetalSt2Src*`, `dasllama/dasllama_metal_kernels.das`)
computes the CPU `sine_source`'s operations (`dasllama/dasllama_tts_blocks.das`) in the CPU's
order, with fast math off, and keeps the two GPU replacements for CPU precision: a two-float
running sum that carries its rounding error, in place of the torch law's `double` phase
accumulator, and the four-piece 2 pi reduction of the phase before `sin`; reordering an add or
dropping either replacement is a defect.** `st2_source_gate` in
`tests/test_metal_prefill_kernels.das` is the instrument; one float32 ulp of the accumulated phase
is a hundredth of a radian.

**A diff to `dasllama/dasllama_vulkan_tower.das` after which the whisper-class blocks hook
(`vulkan_audio_tower_blocks`) and the conv stem (`vulkan_audio_conv_front`) no longer both -
directly or through `vt_ln_chain` - key the resident through `vt_aud_attach` (the residency key
and the weight upload), build their kernel classes through `vt_aud_ensure`, and size the scratch
and its schedule meta through `vt_ln_scratch` is a defect - route the new path through those
three.** The
stem leaves its residual rows in the chain's x buffer; a resident or scratch the chain rebuilds on
sizing of its own drops them, and the chain uploads them again.

**A driver route that dispatches a borrowed kernel set - the builders one driver borrows from
another driver - is gated on every pipeline of that set having compiled, never on a subset; one
absent pipeline keeps the CPU route.** A partly-armed set dispatches into a null pipeline.
