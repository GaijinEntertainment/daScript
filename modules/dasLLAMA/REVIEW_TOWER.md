# dasLLAMA GPU Tower Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU_TOWER.md`, `ARCHITECTURE_MEDIA.md`. Planned work: `followup_metal.md`,
`followup_vulkan.md`.

**Routed from `REVIEW_GPU.md`: a diff that checklist routes here applies this list together
with `REVIEW_GPU.md`'s and `REVIEW.md`'s.**

**Every non-comment diff routed here runs every gate this checklist names for each tower it
reaches - a diff reaches a tower when a function it touches is, calls, or is reachable from a
hook that tower's register (`dasllama_metal_tower_register`, `dasllama_vulkan_tower_register`)
registers, and reaches the Metal tower also when it touches `dasllama/dasllama_metal_asr_dec.das`,
`dasllama/dasllama_metal_common.das` or a kernel class or builder the ASR decoder uses - or,
reaching neither, says so in the PR body in place of the run.**

**A diff reaching the Metal tower (one touching a function that is, calls, or is reachable from a
hook `dasllama_metal_tower_register` registers, or `dasllama/dasllama_metal_asr_dec.das`,
`dasllama/dasllama_metal_common.das` or a kernel class or builder the ASR decoder uses) runs the
vision gates `tests/test_gemma4uv.das`, `tests/test_gemma4v.das`, `tests/test_gemma3v.das`,
`test_qwen3v_tier1_metal` and `test_qwen3v_tier1_metal_f16` in `tests/test_qwen3v.das` and
`test_qwen25v_tier1_gpu` in `tests/test_qwen25v.das`; the audio gates `tests/test_whisper.das`,
`tests/test_audio.das` and `tests/test_audio_embedder.das`; the TTS gates
`test_kitten_synthesis_metal` and `test_kitten_oracle` in `tests/test_tts_kitten.das`,
`test_kokoro_synthesis_metal` and `test_kokoro_oracle` in `tests/test_tts_kokoro.das`,
`test_pocket_codec_metal`, `test_pocket_codec_metal_served`, `test_pocket_frames_metal`,
`test_pocket_frames_metal_served` and `test_pocket_synthesis_metal` in `tests/test_tts_pocket.das`;
`test_metal_prefill_kernels` in `tests/test_metal_prefill_kernels.das`, the gate of the kernels
the tower chains share; and the audio-tower run, `tests/test_model_image.das` with the `mtower`
arm and no `--family` filter (`DASLLAMA_TEST_FAMILY` unset), with `metal_tower_stats()`'s encode
count rising across the run.** The arm's cells span several families, and a filter skips the
rest; the shared common paths and borrowed kernels reach the ASR decoder with no line of its own
file touched.

**A diff reaching the Vulkan tower (one touching a function that is, calls, or is reachable from a
hook `dasllama_vulkan_tower_register` registers) runs `test_gemma4v_vulkan_twin` in
`tests/test_gemma4v.das`, `test_gemma3v_vulkan_twin` in `tests/test_gemma3v.das`,
`test_qwen3v_vulkan_twin` in `tests/test_qwen3v.das`, `test_qwen25v_vulkan_twin` in
`tests/test_qwen25v.das`, `test_whisper_vulkan_twin` in `tests/test_whisper.das`,
`test_encoder_blocks_vulkan`, `test_gemma4a_vulkan_twin`, `test_canary_vulkan_twin` and
`test_qwen3a_vulkan_front` in `tests/test_audio.das`, and `tests/test_vulkan_tower_kernels.das`;
each named `test_*` cell sees the counter of every hook it covers rise.**

**A change to `dasllama/dasllama_vulkan_asr_dec.das`, to a kernel class it dispatches, or to a
tower helper it rides (`vt_tile*`, `vt_sched_map`, `vt_feed_enc`, `active_q8_repack_layout` in
`dasllama/dasllama_math.das`) runs
`test_whisper_vulkan_wdec` (`tests/test_whisper.das`).** Its CPU-vs-GPU transcript cells are the
Vulkan ASR-decoder driver's parity instrument, and the shared helpers reach the driver with no
line of its own file touched.

**A hook registered in `dasllama_metal_tower_register` or `dasllama_vulkan_tower_register` that
no gate this checklist names covers is a defect - add a test cell covering it and name it in this
checklist, in the same change.** A gate covers a hook when it asserts a counter only that hook
raises: on Vulkan, `vulkan_tower_stats()`'s `encodes` and `blocks` for a blocks hook, `convs` for
a front or conv hook, `mels` for qwen3a's mel hook.

**A diff that adds a tower kernel, widens the rows an existing one reads, or changes the upload or
class that fills a padded buffer leaves every row past the live row count that any tower kernel
reads zeroed - by the upload that fills the buffer, or by the class that writes the padded
panel.** A reused buffer comes back holding the previous encode's bytes, infinities included.

**A diff that sizes a tower attention buffer sizes it from the largest row count any kernel's
grid reads, never from the query-row pad alone.** On Metal the pool's power-of-two bucket hides
a device overrun at some row sizes, so an undersized panel reads past its allocation only on
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

**A diff to the harmonic-source kernels (`MetalSt2SrcLow*`, `MetalSt2SrcCumsum*`,
`MetalSt2SrcSines*` in `dasllama/dasllama_metal_kernels.das`) computes the CPU `sine_source`'s
operations (`dasllama/dasllama_tts_blocks.das`) in the CPU's order, with fast math off, and keeps
the two GPU replacements for CPU precision: a two-float running sum that carries its rounding
error, in place of the torch resample path's (the `*Torch` stamps') `double` phase accumulator,
and the reduction of the phase into one period by subtracting 2 pi split into four float parts
before `sin`.** `st2_source_gate` in `tests/test_metal_prefill_kernels.das` checks this.

**A diff to the source's noise-fill kernel (`MetalSt2SrcNoise`) keeps it drawing the normal
sample `st2_hash_normal` hashes from the seed and the element index.** The draw feeds the sines
kernel's rows in place of a noise stream recorded from the reference run; `st2_source_gate`
holds it to `st2_hash_normal` evaluated on the host, bit-equal when repeated under one seed and
different under another.

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
