# dasLLAMA GPU Tower Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_GPU_TOWER.md`, `ARCHITECTURE_GPU_TOWER_VULKAN.md`,
`ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md`, `ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md`,
`ARCHITECTURE_MEDIA.md`, `ARCHITECTURE_GPU_ASYMMETRIES.md`. Planned work: `followup_metal.md`,
`followup_vulkan.md`.

A tower driver file is `dasllama/dasllama_metal_tower.das`, `dasllama/dasllama_vulkan_tower.das`,
`dasllama/dasllama_vulkan_tts.das` or a `dasllama/dasllama_cuda_tower*.das` file. An ASR-decoder
driver file is a `dasllama/dasllama_metal_asr_dec*.das`, `dasllama/dasllama_vulkan_asr_dec*.das` or
`dasllama/dasllama_cuda_asr_dec*.das` file. A tower is one backend's (Metal, Vulkan or CUDA) driver
set that runs a family's encoder or synthesis stages through hooks - stage seats the CPU chain calls,
drop hooks, reload or weights-epoch listeners - that its tower driver files register, from an `[init]`
or from an install function an `[init]` hands `register_gpu_home`, directly or through the functions
that install calls (`ARCHITECTURE_GPU_TOWER.md#tower-reach`). A hook an ASR-decoder driver file
registers the same way is an ASR-decoder seat of its backend's tower.

A diff reaches a tower when a function it touches is one of those hooks, calls one directly, or is
reachable from one through any chain of calls, or when it touches a kernel class or builder a driver
of that tower or its ASR decoder dispatches, a kernel-argument struct such a driver fills, or a
kernel template such a class stamps (instantiates into a concrete kernel); the Metal tower is also
reached by a touch of `dasllama/dasllama_metal_asr_dec.das` or `dasllama/dasllama_metal_common.das`.
An edit that changes no compiled code - a comment, an `[arch(at=...)]` citation - reaches no tower.

The TTS seat cells are `test_kitten_synthesis_metal` and `test_kitten_oracle` in
`tests/test_tts_kitten.das`, `test_kokoro_synthesis_metal` and `test_kokoro_oracle` in
`tests/test_tts_kokoro.das`, and `test_pocket_codec_metal`, `test_pocket_codec_metal_served`,
`test_pocket_frames_metal`, `test_pocket_frames_metal_served`, `test_pocket_frames_after_model_drop`,
`test_pocket_synthesis_metal`, `test_pocket_small_models` and `test_pocket_small_refl` in
`tests/test_tts_pocket.das`.

**A non-comment diff routed here that reaches no tower says so in the PR body.**

**A cell any run list in this checklist names that skips for a missing model counts as not run.**

**A diff reaching the Metal tower runs the vision gates `tests/test_gemma4uv.das`,
`tests/test_gemma4v.das`, `tests/test_gemma3v.das`, `test_qwen3v_tier1_metal` and
`test_qwen3v_tier1_metal_f16` in `tests/test_qwen3v.das` and `test_qwen25v_tier1_gpu` in
`tests/test_qwen25v.das`; the audio gates `tests/test_whisper.das`, `tests/test_audio.das`,
`tests/test_audio_embedder.das`, `tests/test_vad.das`, `test_rnnt_device_twin` in
`tests/test_rnnt.das` and `test_gigaam_device_decode_twin` in `tests/test_gigaam.das`; the TTS
seat cells; `test_metal_prefill_kernels` in `tests/test_metal_prefill_kernels.das`, the gate of
the kernels the tower chains share; and the audio-tower run, `tests/test_model_image.das` with the
`mtower` arm and no `--family` filter (`DASLLAMA_TEST_FAMILY` unset), with `metal_tower_stats()`'s
encode count rising across the run.** A family filter skips other families' cells; a shared path
or borrowed kernel reaches the ASR decoder with its file untouched.

**A diff reaching the Vulkan tower runs, on a build without the Metal module and with
`DASLLAMA_CUDA` unset, `test_gemma4v_tier1_vulkan` and `test_gemma4v_discrete_twin` in `tests/test_gemma4v.das`,
`test_gemma3v_discrete_twin` in `tests/test_gemma3v.das`, `test_qwen3v_discrete_twin` in
`tests/test_qwen3v.das`, `test_qwen25v_discrete_twin` in `tests/test_qwen25v.das`,
`test_gemma4uv_tier1_vulkan` in `tests/test_gemma4uv.das`, `test_whisper_discrete_twin` in
`tests/test_whisper.das`, `test_encoder_blocks_vulkan`, `test_gemma4a_discrete_twin`,
`test_canary_discrete_twin`, `test_gigaam_discrete_twin` and `test_qwen3a_vulkan_front` in
`tests/test_audio.das`, `test_vad_gpu_twin` in `tests/test_vad.das`,
`tests/test_vulkan_tower_kernels.das`, the TTS kernel gates `tests/test_vulkan_tts_kernels.das`,
`tests/test_vulkan_tts_conv_kernels.das`, `tests/test_vulkan_tts_source_kernels.das` and
`tests/test_vulkan_tts_pocket_kernels.das`, and the TTS seat cells; each named `test_*` cell sees
the counter of every hook the cell covers rise.**

**A diff reaching the CUDA tower runs, on a build carrying dasCUDA on a machine with a CUDA
device and with `DASLLAMA_GPU=1` and `DASLLAMA_CUDA=1` set, `test_gemma4v_tier1_vulkan` and
`test_gemma4v_discrete_twin` in `tests/test_gemma4v.das`, `test_gemma3v_discrete_twin` in
`tests/test_gemma3v.das`, `test_qwen3v_discrete_twin` in `tests/test_qwen3v.das`,
`test_qwen25v_discrete_twin` in `tests/test_qwen25v.das`, `test_gemma4uv_tier1_vulkan` and
`test_gemma4uv_device_twin` in `tests/test_gemma4uv.das`, `test_vision_chat_e2b`,
`test_vision_chat_gemma3`, `test_vision_chat_deepstack` and `test_vision_chat_qwen25o` in
`tests/test_vision_chat.das`, and the audio files `tests/test_audio.das`,
`tests/test_audio_embedder.das`, `tests/test_whisper.das`, `tests/test_cuda_tower_whisper_chain.das`,
`tests/test_gigaam.das`, `tests/test_rnnt.das`, `tests/test_vad.das` and
`tests/test_cuda_tower_g4a_chain.das`, and the CUDA tower kernel gates `tests/test_cuda_tower_front.das`,
`tests/test_cuda_tower_blocks.das` and `tests/test_cuda_tower_asr.das`; each twin and device-twin
cell of the run sees the counter of every hook the cell covers rise; the two tier-1 cells run against the encode dumps of the reference
build (the third-party engine this module's figures compare with), and a tier-1 cell that skips
for a missing dump counts as not run.**

**A change to a Vulkan or CUDA ASR-decoder driver file, to a kernel class it dispatches, or to a
function it calls that another file defines runs `test_whisper_vulkan_wdec_lifetime`
(`tests/test_whisper.das`), `test_rnnt_device_twin` (`tests/test_rnnt.das`),
`test_parakeet_device_decode_twin` (`tests/test_audio.das`) and `test_gigaam_device_decode_twin`
(`tests/test_gigaam.das`) on a build without the Metal module, once for each driver file the
change reaches: with `DASLLAMA_CUDA` unset for a Vulkan file, and on a build carrying dasCUDA, on a
machine with a CUDA device, with `DASLLAMA_GPU=1` and `DASLLAMA_CUDA=1` set, for a CUDA file.**
Their CPU-vs-GPU cells are the decoder driver's parity instrument, and a shared function reaches
the driver with no line of its own file touched.

**A diff that registers a hook into a tower seat from a driver's `[init]`, or changes which hook
a seat reaches, names in the run list of the tower whose driver registers it a gate that asserts
the hook's counter rising on a leg where that hook is the only reachable hook raising it, adding
the cell in the same change.** The counter is one the hook's own driver raises only when that
hook serves.

**A diff that adds a tower kernel (a kernel class a tower driver dispatches), widens the rows an
existing one reads, or changes the upload or class that fills a padded buffer leaves zeroed
every row past the live row count that a tower kernel reads into a live output row - by the
upload that fills the buffer, or by the class that writes the padded panel.** A reused buffer
comes back holding the previous encode's bytes, infinities included.

**A diff that sizes a tower attention buffer sizes it from the largest row count any kernel's
grid reads, never from the query-row pad alone.** On Metal the pool's power-of-two bucket hides
a device overrun at some row sizes, so an undersized panel reads past its allocation only on
specific canvas sizes.

**A diff that changes the dispatch sequence of a family's chain in a tower driver keeps the chain
running the operations of the CPU code the seat's hook replaces, in that code's dependency order
(an operation moves past another only when neither writes what the other reads) and at the same
operand shapes; a chain may fuse consecutive operations into one dispatch, or split one
operation across several dispatches.** That CPU code is what the call site runs for the
family's CPU image (the `.dlim` whose planes the CPU chain reads, as against the GPU image carrying a
driver's device block): the CPU block loop, front, tail or mel beside the call, plus every CPU step
the call site skips when the hook serves.

**A diff that makes a tower chain compute anything the CPU code its seat's hook replaces does not
compute changes that CPU code the same way, in the same diff.**

**A diff that adds or changes a family's chain in the Metal tower or in
`dasllama/dasllama_metal_asr_dec.das` puts on the device, in the same change, every stage that
family's CPU chain runs inside a transcription, synthesis or image-encode call, for every model and
weight lane the family serves.** It binds whenever the stage would serve: a Metal device is
present, the stage's knob is on (`DASLLAMA_METAL_TOWER`; `DASLLAMA_METAL_WDEC` for the cross-KV;
`DASLLAMA_METAL_WDEC` and `DASLLAMA_METAL_WDEC_STEP` for the decode step; `DASLLAMA_METAL_RNNT` for
the transducer decode), and the stage's command buffer succeeded. Only an
`ARCHITECTURE_GPU_ASYMMETRIES.md#gpu-asymmetries` entry or a `followup_metal.md` row ledgers a
case. A CPU stage behind a Metal chain looks served.

**A diff that changes what a TTS seat's chain computes in a tower driver applies `REVIEW_TTS.md`
too.**

**A diff giving a block hook that takes an `AudioTower` (every one through `vt_aud_blocks`) or the
conv stem (`vulkan_audio_conv_front`) in `dasllama/dasllama_vulkan_tower.das`, or the stem
(`cuda_audio_conv_front`) or blocks seat (`cut_aud_blocks`) in
`dasllama/dasllama_cuda_tower_whisper.das`, its own copy of a weight, kernel class or buffer a
chain helper provides is a defect - route it through that helper: on Vulkan `vt_aud_attach` (the
device weights, `VtResident`), `vt_ln_chain_ensure` (the kernel classes) and `vt_ln_scratch` (the
scratch buffers and schedule table), directly or through `vt_ln_ready`; on CUDA `cut_aud_attach`,
`cut_ln_ensure` and `cut_aud_scratch`.** The stem leaves its output rows on the device, in the
chain's x buffer; a path that rebuilds the weights or scratch on its own drops those rows unread,
and the CPU block loop then reads stale rows.

**A diff that adds or changes a route in a tower driver file, an ASR-decoder driver file or a chain
body they run (`dasllama/dasllama_*_chains*.das`) puts every kernel class or builder that route
dispatches in the route's ensure chain - the pipeline builds the route checks before it serves.** A
class outside the chain dispatches into a null pipeline when its build failed.

**A diff that adds or changes a family's flavor pick - the `*_serves_model` function a GPU driver
registers, which the load calls before staging to choose the family's CPU image (the `.dlim` whose
planes the CPU chain reads) or its GPU image (the `.dlim` carrying that driver's device block) -
makes the pick ask, before it admits a lane, the driver's tower knob, the device the driver armed,
the pipelines the chain's ensure chain builds, the tile family the chain's GEMMs ride, the f16 GEMM
feed where the chain's front rides it, and, for a pick on the model's record (`[tower_flavor]` with
`lane = "model"`), the planes the lane holds.** A GPU image never runs on the CPU: where its chain
declines, the call refuses (`tower_cpu_refuse`), so a capability the pick leaves out turns a box
that cannot serve the lane into that refusal.

**A diff that changes a block region list (`ln_block_regions`, `gemma4v_block_regions`,
`qwen3v_block_regions`, a family's `*_all_regions`, a `vt_*_regions` list) or a driver's mapping of one
(the `tw_q8_regions` overloads) changes, in the same change, every index a tower chain passes to
its GEMM dispatch - `wt_mm` (Metal), the record ids the shared chain bodies pass to `rail_lane_mm`,
`tower_mm_clamped` and `rail_mm_kq` and their `tower_*_rec` schedule rows (Vulkan and CUDA); weakening the
cells that go red on a misindexed site - the Metal q8-lane cells of `tests/test_gemma3v.das`,
`tests/test_gemma4v.das` and `tests/test_qwen3v.das`, the families' `*_discrete_twin` cells with
and without `DASLLAMA_CUDA=1` - is a defect.** A Metal site index is the region's position in the
driver's list; a Vulkan or CUDA record index is its position in the family's list, a layer.
