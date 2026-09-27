# dasLLAMA GPU Tower Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU_TOWER.md`, `ARCHITECTURE_GPU_TOWER_VULKAN.md`,
`ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md`, `ARCHITECTURE_MEDIA.md`. Planned work: `followup_metal.md`,
`followup_vulkan.md`.

A tower is the Metal or the Vulkan GPU driver set that runs a family's encoder or synthesis
stages through hooks - stage seats the CPU chain calls, drop hooks, reload or weights-epoch
listeners - registered by `dasllama_metal_tower_register` (Metal) or by
`dasllama_vulkan_tower_register` and `dasllama_vulkan_tts_register` (Vulkan)
(`ARCHITECTURE_GPU_TOWER.md#tower-reach`). A diff reaches a tower when a function it touches is,
calls, or is reachable from one of those hooks, and reaches the Metal tower also when it touches
`dasllama/dasllama_metal_asr_dec.das`, `dasllama/dasllama_metal_common.das`, or a kernel class
or builder the ASR decoder uses.

**A non-comment diff routed here that reaches neither tower says so in the PR body.**

**A diff reaching the Metal tower runs the vision gates `tests/test_gemma4uv.das`,
`tests/test_gemma4v.das`, `tests/test_gemma3v.das`, `test_qwen3v_tier1_metal` and
`test_qwen3v_tier1_metal_f16` in `tests/test_qwen3v.das` and `test_qwen25v_tier1_gpu` in
`tests/test_qwen25v.das`; the audio gates `tests/test_whisper.das`, `tests/test_audio.das` and
`tests/test_audio_embedder.das`; the TTS gates `test_kitten_synthesis_metal` and
`test_kitten_oracle` in `tests/test_tts_kitten.das`, `test_kokoro_synthesis_metal` and
`test_kokoro_oracle` in `tests/test_tts_kokoro.das`, `test_pocket_codec_metal`,
`test_pocket_codec_metal_served`, `test_pocket_frames_metal`, `test_pocket_frames_metal_served`,
`test_pocket_frames_after_model_drop` and `test_pocket_synthesis_metal` in
`tests/test_tts_pocket.das`; `test_metal_prefill_kernels` in
`tests/test_metal_prefill_kernels.das`, the gate of the kernels the tower chains share; and the
audio-tower run, `tests/test_model_image.das` with the `mtower` arm and no `--family` filter
(`DASLLAMA_TEST_FAMILY` unset), with `metal_tower_stats()`'s encode count rising across the
run.** A family filter skips the other families' cells, and a shared path or borrowed kernel
reaches the ASR decoder with no line of its file touched.

**A diff reaching the Vulkan tower runs, on a build without the Metal module,
`test_gemma4v_vulkan_twin` in `tests/test_gemma4v.das`, `test_gemma3v_vulkan_twin` in
`tests/test_gemma3v.das`, `test_qwen3v_vulkan_twin` in `tests/test_qwen3v.das`,
`test_qwen25v_vulkan_twin` in `tests/test_qwen25v.das`, `test_whisper_vulkan_twin` in
`tests/test_whisper.das`,
`test_encoder_blocks_vulkan`, `test_gemma4a_vulkan_twin`, `test_canary_vulkan_twin` and
`test_qwen3a_vulkan_front` in `tests/test_audio.das`, `tests/test_vulkan_tower_kernels.das`, the
TTS kernel gates `tests/test_vulkan_tts_kernels.das`, `tests/test_vulkan_tts_conv_kernels.das`,
`tests/test_vulkan_tts_source_kernels.das` and `tests/test_vulkan_tts_pocket_kernels.das`, and
the TTS seat cells - `test_kitten_synthesis_metal` and `test_kitten_oracle` in
`tests/test_tts_kitten.das`, `test_kokoro_synthesis_metal` and `test_kokoro_oracle` in
`tests/test_tts_kokoro.das`, `test_pocket_codec_metal`, `test_pocket_codec_metal_served`,
`test_pocket_frames_metal`, `test_pocket_frames_metal_served`,
`test_pocket_frames_after_model_drop` and `test_pocket_synthesis_metal` in
`tests/test_tts_pocket.das` - the TTS cells serving through the Vulkan TTS driver there; each
named `test_*` cell sees the counter of every hook it covers rise.**

**A change to `dasllama/dasllama_vulkan_asr_dec.das`, to a kernel class it dispatches, or to a
function it calls that another file defines runs `test_whisper_vulkan_wdec`
(`tests/test_whisper.das`).** Its CPU-vs-GPU transcript cells are the Vulkan ASR-decoder driver's
parity instrument, and a shared function reaches the driver with no line of its own file touched.

**A hook that any `[init]` of a tower's driver files registers into a seat, and that no gate this
checklist names covers, is a defect - add a test cell covering it and name it in this checklist,
in the same change.** A gate covers a hook when it asserts a counter rising on a leg where that
hook is the only hook reachable that raises the counter: `vulkan_tower_stats()`'s or
`metal_tower_stats()`'s `encodes` and `blocks` for a blocks hook, `encodes` for an encode hook,
`convs` for a front, conv or chunk hook, on Vulkan `mels` for qwen3a's mel hook; on either
tower, `styletts2_gpu_stats(<seat>)`'s or `pocket_gpu_stats(<seat>)`'s `served` for a TTS seat.

**A diff that adds a tower kernel (a kernel class a tower driver dispatches), widens the rows an
existing one reads, or changes the upload or class that fills a padded buffer leaves every row
past the live row count that any tower kernel reads zeroed - by the upload that fills the
buffer, or by the class that writes the padded panel.** A reused buffer comes back holding the
previous encode's bytes, infinities included.

**A diff that sizes a tower attention buffer sizes it from the largest row count any kernel's
grid reads, never from the query-row pad alone.** On Metal the pool's power-of-two bucket hides
a device overrun at some row sizes, so an undersized panel reads past its allocation only on
specific canvas sizes.

**A diff that changes the dispatch sequence of a family's chain in a tower driver keeps the chain
running the operations of the CPU code the seat's hook replaces, in the same order and at
the same operand shapes; a chain may fuse consecutive operations into one dispatch, or split one
operation across several dispatches.** That CPU code is what the hook's call site runs when the
hook declines: the CPU block loop, front, tail or mel beside the call, plus every CPU step the
call site skips when the hook serves.

**A diff that makes a tower chain compute anything the CPU code its seat's hook replaces does not
compute changes that CPU code the same way, in the same diff.**

**A diff that changes what a TTS seat's chain computes in a tower driver applies `REVIEW_TTS.md`
too.**

**In `dasllama/dasllama_vulkan_tower.das`, the block hooks that take an `AudioTower` (both through
`vt_aud_blocks`) and the conv stem (`vulkan_audio_conv_front`) each get their device weights
(`VtResident`) from `vt_aud_attach`, their kernel classes from `vt_ln_chain_ensure`, and their
scratch buffers and schedule table from `vt_ln_scratch` - directly or through `vt_ln_chain`; a
diff giving either path its own weight upload, class build or scratch sizing is a defect - route
it through those three.** The stem leaves its output rows on the device, in the chain's x buffer;
a path that rebuilds the weights or scratch on its own drops those rows unread, and the CPU block
loop then reads stale rows.

**A diff that adds or changes a route in a tower driver or an ASR-decoder driver
(`dasllama/dasllama_metal_asr_dec.das`, `dasllama/dasllama_vulkan_asr_dec.das`) puts every kernel
class or builder that route dispatches in the route's ensure chain - the pipeline builds the
route checks before it serves - so one absent pipeline keeps the CPU route.** A class outside the
chain dispatches into a null pipeline when its build failed.
