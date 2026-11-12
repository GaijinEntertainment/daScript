options gen2
options _comment_hygiene = true
options _dasllama_internal = true

//! dasLLAMA's environment knobs, declared as `[EnvConfig]` structs (daslib/clargs) — one struct
//! per area, one field per variable, the field initializer as the default and the doc line on the
//! declaration. `env_config(type<T>)` loads each struct ONCE at context init into the `g_env_*`
//! globals below; read sites touch plain struct fields, never the environment. `ENVIRONMENT.md`
//! is GENERATED from the declarations (`harness/gen_env_doc.das`) and `tests/test_env_registry.das`
//! fails when a variable is read anywhere in the tree without a declaration here.
//!
//! Semantics (shared with clargs and daslib/build_const): flags read ""/0/false/off/no any-case as
//! false and anything else as true; set-but-empty counts as unset; garbage numeric text logs a
//! warning and keeps the default. `Option<T>` fields are tri-state — they stay `none` when the
//! variable is unset, for knobs whose effective default is computed at runtime.
//!
//! The globals load once, at context init. `set_env_variable` after startup is INVISIBLE to them —
//! arm a child process's environment instead of mutating your own.

module dasllama_env shared public

require dasllama/dasllama_lint public
require daslib/clargs
require daslib/env_registry public
require daslib/option public
require daslib/rtti
require daslib/fio
require strings

// ===== Engine =====

[EnvConfig(env_prefix = "DASLLAMA")]
struct public EngineEnv {
    @clarg_doc = "Use the prepared-image file beside a source model for millisecond reloads. 0 skips the file: LLM ggufs take the full parse; audio carriers mint the image in memory and serve that (they are always image-backed)."
    image : bool = true

    @clarg_doc = "Persist the prepared image beside the source; 0 builds it in memory and serves that, leaving no .dlim behind."
    image_save : bool = true

    @clarg_doc = "A development lever: also accept a prepared image of this older image version where its meta layout fingerprint is this build's, in place of re-minting it (0 = the build's version alone). The caller vouches that nothing the image's lane holds changed between the two versions - a version that moved another lane's bytes; an image whose planes did change reads wrong with no diagnostic."
    image_accept_version : int = 0

    @clarg_doc = "Allow the CPU prefill path even when a GPU prefill override is registered."
    cpu_prefill : bool = false

    @clarg_doc = "Pin prefill to one registered override by name (e.g. metal, vulkan); anything else declines before upload."
    pin_prefill : string = ""

    @clarg_path
    @clarg_doc = "Split NextN/MTP head GGUF to attach to the trunk being loaded (the split mtp-<model>.gguf form: its blk.<n_layers>.* tensors become the draft block; its copies of the trunk's tables are ignored). Default: the mtp-<trunk basename> sibling beside the trunk when present. The head rides the split-shard walk, so one prepared image carries trunk and head."
    mtp_head : string = ""

    @clarg_doc = "RoPE scaling override for the load: yarn | linear | none; unset keeps the file's rope.scaling keys, none drops them (a file's per-pair factor tensors and attn_factor stay, as llama.cpp keeps them). yarn folds the NTK-by-parts frequency ramp and the 1 + 0.1 ln(s) magnitude into the rope tables (llama.cpp --rope-scaling yarn). The Qwen families publish the recipe (Qwen2.5-Instruct 7B and up, Qwen3, Qwen3-Next / 3.5 / 3.8: factor 4 over the trained context) and ship no scaling keys because static YaRN costs a little on short texts - arm it when a conversation needs the length; no other vendor validates it, and a non-Qwen file logs a warning. The override is baked into the prepared image, under its own lane."
    rope_scaling : string = ""

    @clarg_doc = "The RoPE scaling factor s (the context multiplier) for the override; 0 = the file's rope.scaling.factor, and yarn needs one."
    rope_scale : float = 0.0

    @clarg_doc = "YaRN: the original training context the factor extends; 0 = the file's original_context_length, else its context_length (llama.cpp --yarn-orig-ctx)."
    yarn_orig_ctx : int64 = 0l

    @clarg_doc = "YaRN: the low correction dim beta_fast; 0 = the file's, else 32."
    yarn_beta_fast : float = 0.0

    @clarg_doc = "YaRN: the high correction dim beta_slow; 0 = the file's, else 1."
    yarn_beta_slow : float = 0.0

    @clarg_doc = "Pin the matmul backend by name, bypassing the measured auto-selection."
    pin_backend : string = ""

    @clarg_doc = "The streamed split's host share runs each (expert, row) pair's whole chain on one lane in one fork a batch - gate, up, act, requant, down, park; 0 = the three region-list GEMV passes with the act and requant between them (the A/B arm)."
    rare_onepass : bool = true

    @clarg_doc = "The one-pass rare rail runs an expert's rows through the batched expert cores (the plane read once a tile); 0 = the rows core a row (the A/B arm)."
    rare_batched : bool = true

    @clarg_doc = "The rare rail reads a streamed layer's experts off the tier's pinned device-layout planes through the format's row-major kernel - the prompt's host share and a token's host picks alike. =1 (the default) takes the rail where the load kept no planar planes (a trimmed lane: one copy of the experts); where the planar planes are loaded (the CUDA home, a lane minted with DASLLAMA_TRIM=0, DASLLAMA_IMAGE=0) the tuned planar kernels serve them. =2 takes the rail on the planar planes too (the A/B arm of the row-major kernels, the environment's twin of `set_rare_arena(1)`); =0 keeps every format on the planar planes."
    rare_arena : int64 = 1l

    @clarg_doc = "The meet hands the host this many of a streamed layer's rarest experts before the device claims any (0 = none: the two sides claim at their own pace) - a measurement lever, so a carrier the device would take whole still runs the host rail."
    moe_host_first : int64 = 0l

    @clarg_doc = "How an IQ3_S weight is served: grid = the disk form (the 3.44-bit grid planes, decoded by a gather), lut = the iq3s4 form (a 4-bit codebook index a superblock, decoded by the LUT kernels - exact, 17% more bytes, the CPU decode several times faster), auto = lut on the CPU rails and the Vulkan tier, grid under the Metal tier (every home serves both forms at every site)."
    iq3s_serve : string = "auto"

    @clarg_doc = "Pin the batched (prefill) matmul backend independently of the decode one."
    pin_batch_backend : string = ""

    @clarg_doc = "Arm the MoE expert-reuse counter (probes and servers; benches use set_expert_reuse instead)."
    expert_reuse : bool = false

    @clarg_doc = "Print engine diagnostics (tier selection, upload plan, arm/decline reasons)."
    noisy : bool = false

    @clarg_doc = "A paged scheduler serving device-home streams keeps their openings in its prefix cache too - a recurrent model's checkpoints and the finished turns' rows copied down to host RAM under the cache's budget, taken back up into a region when a request opens with one the device no longer holds; 0 keeps the regions' parked claims and checkpoint rows alone - the A/B rail."
    device_pool : bool = true

    @clarg_default_doc = "2 (the top notch)"
    @clarg_doc = "A/B rail for the dispatch caller's thread priority, -2..2; unset claims the top notch."
    caller_prio : int = 2

    @clarg_doc = "Let the engine dispatch on a job queue nobody configured with setup_dasllama_jobque: a loud warning instead of the panic. A bare queue decodes ~200x slower, so only a run measuring that regime wants it."
    allow_bare_jobque : bool = false

    @clarg_doc = "Run every kernel on the calling thread: no dispatch to the job queue at all, whatever queue exists (set_single_thread is the runtime spelling). The browser build's baseline and the figure that says what the worker pool buys."
    single_thread : bool = false

    @clarg_doc = "Regenerate the stored parity truth files instead of comparing against them."
    truth_refresh : bool = false

    @clarg_doc = "Bucket gguf -> image conversion time by kind over the weight walk; one clock pair per tensor."
    conv_prof : bool = false

    @clarg_doc = "Permit a big gguf load without -jit; the transforms run interpreted, so expect minutes per GB."
    allow_interp_load : bool = false

    @clarg_doc = "Advisory source-mapping readahead at gguf load (cold-conversion fix); =0 restores on-demand faulting."
    prefetch : bool = true

    @clarg_doc = "Readahead of the prepared image at its map (madvise WILLNEED, synchronous on macOS): a purged page cache then reads the whole image at the SSD's rate before the first token - 103 GB in 15 s on the M5 - instead of paying 2.5x on the routed expert stacks over the first 128 tokens, and a WARM map pays ~2.5 s for the page walk in place of 65 ms. A serving process that runs for hours wants it; a bench, a test or a short session does not. An n-gram model's hash table is read ahead either way: its rows are random pages of the whole table, so an unread table hard-faults every token."
    prefetch_image : bool = false

    @clarg_doc = "The streaming silero VAD (`vad_iter_feed`, the live-mic callers) runs each feed's whole chunks through the GPU driver's VAD seat (Metal, or Vulkan where it serves) instead of the CPU chunk step; off, the measured-faster form for one 32 ms chunk. The batch VAD (`vad_probs`, `find_speech_timestamps`) takes the seat whatever this says; `set_vad_stream_gpu` is the in-process twin."
    vad_stream_gpu : bool = false

    @clarg_default_doc = "unset (each family's own lane)"
    @clarg_doc = "The weight lane an audio tower load asks for where the program pins none, on every home: whisper's encoder, qwen3a, the chat towers (load_audio_tower: qwen2audio, voxtral, ultravox, omni), canary, parakeet, gemma4a and GigaAM - not the vision towers, TTS or the LLM. 8 = the q8 lane; 16 = the exact lane - the f16 image on a GPU home, the family's exact f32 planes on a CPU-only run; unset = each family's policy (q8 on every GPU home, GigaAM's exact lane on Metal, the chat towers' f32 load on a CPU-only run). Any other number logs a warning and keeps the policy. A family's API pin beats it (set_<family>_q8, set_asr_tower_q8, set_audio_tower_q8, the facade's set_asr_fp32 / set_asr_tower_fp32) and reset_<family>_q8 returns to it; set, it beats DASLLAMA_VK_GIGAAM_EXACT, which moves only GigaAM's default on the discrete homes. It asks a lane and changes nothing about what serves it: a lane a home cannot serve maps the CPU image as the pin does, and a quantized GigaAM carrier keeps its own planes (a pin alone dequantizes it). A lane it moves off the policy says so once a process (`dasLLAMA: DASLLAMA_AUDIO_LANE=<v> ...`), and each load's lane line names it."
    audio_lane : Option<int>
}

// Tri-state on purpose: the tier's effective defaults are computed from set_gpu_tier_want state
// at query time, so these keep the "was it set" bit (gpu_want_* in dasllama_gpu_tier.das).
[EnvConfig(env_prefix = "DASLLAMA")]
struct public GpuTierEnv {
    @clarg_default_doc = "off"
    @clarg_doc = "One switch for the measured-best GPU rail set; any DASLLAMA_GPU_* knob still overrides individually. On a Vulkan build it is also the want the tower driver reads before any device init: the vision and audio towers' block loops serve on the device only while it is on, and decline `device` otherwise."
    gpu : Option<bool>

    @clarg_doc = "The environment's spelling of the GPU home (the program's is `set_gpu_home`, dasllama-server's its `--gpu vulkan | cuda` key): on, the CUDA home takes the GPU tier's seats in place of the Vulkan home (with DASLLAMA_GPU on) - the whole-model driver for a dense q8 model on an NVIDIA card through the driver API and NVRTC, no Vulkan call made, and the tower seats on the CUDA tower driver - the vision towers (gemma3v, gemma4v, gemma4uv, qwen3v, qwen25v), every audio tower (the whisper-class towers, qwen3a, gemma4a, canary, parakeet, GigaAM), the whisper decoder, the transducer and CTC heads and the silero VAD. Off, the Vulkan home holds the seats until a program moves them. A build without dasCUDA (`DAS_CUDA_DISABLED`, or a 32-bit host) has no CUDA home: the knob is ignored with one warning and the Vulkan home keeps the tier."
    cuda : bool = false

    @clarg_default_doc = "-1 (auto)"
    @clarg_doc = "How many MoE expert layers to hold resident on the GPU; -1 lets the upload walk place the split."
    gpu_moe_layers : Option<int64>

    @clarg_default_doc = "-1 (auto)"
    @clarg_doc = "How many MoE layers to stream rather than hold resident; -1 is auto."
    gpu_moe_stream : Option<int64>

    @clarg_default_doc = "probed"
    @clarg_doc = "Override the detected VRAM budget in MiB that sizes the resident expert stacks; a pinned cap takes no headroom and reads no OS room."
    gpu_vram_mb : Option<int64>

    @clarg_default_doc = "built-in floor"
    @clarg_doc = "Lower the context floor for arming the resident decode driver, for a short-context session on a small card."
    gpu_min_ctx : int64 = 0l

    @clarg_default_doc = "the model's context"
    @clarg_doc = "Cap the resident driver's KV mirror at this many positions (the card's room and the 4 GiB binding cap on the widest layer's rows still bound it); a cap under the arming floor lowers the floor to the cap (never under 256), since the cap is the shape the caller serves; 0 follows the model's context; a server keeps the model's."
    gpu_ctx_max : int64 = 0l

    @clarg_doc = "The context pin (DASLLAMA_GPU_CTX_MAX, set_gpu_ctx_max) is the context asked, not a cap: a K/V mirror the card cannot hold at it declines the resident driver instead of shortening, and the per-op rails serve the asked context; the load log names the room."
    gpu_ctx_strict : bool = false

    @clarg_default_doc = "0"
    @clarg_doc = "The whole-model driver's prompt window in rows: the prefill panels' size, and the rows a hyper-connection model's host expert step and the hot pool's window chain take at once; a longer prompt runs as sequential windows, each paying every routed layer's expert pass again. 0 (the default) lets the driver pick: 4096 where its plan fits with the wider window's scratch, else 2048. Rounded down to a multiple of 128, never under 128; the plan charges the panels at this count, so a wider window shortens the K/V mirror or the pool."
    gpu_pf_rows : int64 = 0l

    @clarg_doc = "Stream each routed layer's expert stack to the device for the prompt, where the routed experts serve on the host (a hyper-connection model, or a plain MoE's host layers) - on the Vulkan tier over its transfer queue, on the CUDA home over its copy stream: pinned device-layout mirrors of every routed layer at load, a device slot (a layer's stack, or under DASLLAMA_GPU_MOE_SPLIT the per-expert chain's ring of experts), the copies landing while the chain computes, the host summing what the meet hands it. 0 keeps the host's sums and the hot pool's window chain."
    gpu_pf_stream : bool = true

    @clarg_default_doc = "0 (every host layer)"
    @clarg_doc = "Under DASLLAMA_GPU_PF_STREAM, mirror only the first this many host layers' expert stacks; the rest keep the host's sums. A parity cell's lever: a few layers' mirrors beside a planar load, where every layer's would not fit the box."
    gpu_pf_stream_layers : int64 = 0l

    @clarg_default_doc = "0 (the plan's own count)"
    @clarg_doc = "Serve at least this many routed layers' experts on the host - the first in layer order - on the whole-model driver of any MoE that routes no more picks a row than the split command serves, the gemma-4 MoE with its parallel dense expert included, whatever the card holds (set_gpu_host_layers in process): the host-layers form, with its split token command and cut window chain, reached on any card. A hyper-connection model serves every routed layer on the host regardless."
    gpu_host_layers : int64 = 0l

    @clarg_doc = "The K/V codec the whole-model driver's mirror is asked to arm at, where no caller set one in process (set_gpu_kv_dtype): f16 (the default), q8_0 (half the mirror's bytes) or tq4 (a quarter). Sessions of the armed codec are served; any other spelling reads as f16."
    gpu_kv : string = ""

    @clarg_doc = "The whole-model resident driver for a model that fits the card, MoE included; 0 keeps the per-op rails (the A/B lever)."
    gpu_resident : bool = true

    @clarg_doc = "A sliding-window layer's K/V mirror rows on a GPU home hold its window plus the rows one command writes, a position's row at its slot of that ring; 0 keeps every position's row (the flat slab, the A/B lever). A ringed history serves a continuation that starts no more than that slack under the rows it holds; one further back prefills again."
    gpu_kv_ring : bool = true

    @clarg_default_doc = "follows DASLLAMA_GPU"
    @clarg_doc = "DeltaNet rail on the GPU."
    gpu_dn : Option<bool>

    @clarg_default_doc = "follows DASLLAMA_GPU"
    @clarg_doc = "DeltaNet decode rail on the GPU."
    gpu_dnd : Option<bool>

    @clarg_default_doc = "follows DASLLAMA_GPU"
    @clarg_doc = "Attention rail on the GPU."
    gpu_attn : Option<bool>

    @clarg_default_doc = "follows DASLLAMA_GPU"
    @clarg_doc = "Dense-weight rail on the GPU."
    gpu_dense : Option<bool>

    @clarg_doc = "The dense rail's attention half (per-layer q/o pairs); opt-OUT, measured a loss on gemma-4."
    gpu_dense_attn : bool = true

    @clarg_doc = "The dense rail's gemma-4 shared-expert w1/w3/w2 triple; opt-IN, measured a wash-to-loss."
    gpu_dense_shexp : bool = false

    @clarg_doc = "The dense rail's FFN half (each dense layer's w1/w3/w2 triple, the decode row's FFN on the device); 0 keeps a dense layer's FFN on the CPU (the A/B rail)."
    gpu_dense_ffn : bool = true

    @clarg_default_doc = "follows DASLLAMA_GPU"
    @clarg_doc = "Shared-expert rail on the GPU."
    gpu_shexp : Option<bool>

    @clarg_default_doc = "off"
    @clarg_doc = "Fused QKV rail on the GPU; stays off under DASLLAMA_GPU (measured a wash)."
    gpu_qkv : Option<bool>

    @clarg_default_doc = "on"
    @clarg_doc = "Keep the classifier plane on the GPU - the best ms-per-GB region; 0 is the CPU A/B lever."
    gpu_cls : Option<bool>

    @clarg_doc = "Device-side routed MoE combine; 0 falls back to the host combine."
    gpu_combine : bool = true

    @clarg_doc = "Split each STREAMED expert layer's prefill between the GPU (the experts it can stream in the CPU's time) and the CPU (the rest, concurrently); 0 streams whole layers."
    gpu_moe_split : bool = true

    @clarg_doc = "A decode token's picks on a streamed layer take the prefill's route: the meet claims them as the GPU's per-expert DMAs from the busy end and the CPU's rail from the rare end, in place of the one-row host step. The experiment knob; the window floors lift under it."
    gpu_dec_meet : bool = false

    @clarg_doc = "The hot expert pool fills by DMA: a token's miss is copied off its pinned stack into the pool's coldest unpicked slot on the home's copy queue (the Vulkan tier's transfer queue, the CUDA home's copy stream), every miss a step and no host time, so the next token finds it; and a prompt's streamed window serves the pool's hits off their slots first and lands its busiest misses in the pool's free and coldest slots, so the decode behind the prompt starts warm. 0 keeps the host's gather-and-upload fill under its step budget and the window off the pool. The A/B knob."
    gpu_hot_dma : bool = true

    @clarg_default_doc = "0"
    @clarg_doc = "The share of a decode step's pool misses the DMA fill admits past the heat gate - the warmest first, each into the coldest unpicked slot, landing for the next step while this step's host sums them - so the pool's working set follows the stream instead of the gate's hot few. 0 keeps the heat gate alone; 1 admits every miss the tier can copy. Needs DASLLAMA_GPU_HOT_DMA."
    gpu_hot_share : float = 0.0

    @clarg_doc = "Decode FFN tail: a resident expert layer's FFN carries the combine, the residual and the next layer's attention feed on the device, so the two submits share one fence wait; 0 collects every FFN on the host."
    gpu_dec_tail : bool = true

    @clarg_doc = "Whole-token decode span: every resident expert layer's attention, router, top-k, FFN and combine as ONE recorded chain and one submit per token; 0 runs the per-layer submits."
    gpu_dec_span : bool = true

    @clarg_default_doc = "0"
    @clarg_doc = "Hot expert slots a layer. On the per-op tier: hold the N hottest experts of a streamed layer resident (unset = none). On the whole-model driver with the routed experts on the host (a hyper-connection MoE): the slots of the hot expert pool - unset = every slot that fits beside the weights and the mirror, else up to 32 out of the mirror's context; a count asks that many; 0 keeps every routed expert on the host."
    gpu_heat : Option<int64>

    @clarg_doc = "Report lifetime GPU queue submissions (real commands plus staging round-trips), and trace every routed row the host sums - the split token command's host segment and the CPU forward alike - as one moe_k line (the layer, the session, the first pick, the L1 of the feed row and of the routed sum)."
    gpu_prof : bool = false

    @clarg_doc = "Record the routed picks of every layer's host expert step under the whole-model driver - a layer, its rows, k expert ids and weights a row, in serving order - and write them to this file when the model drops: the hot expert pool's policy experiments replay the trace offline (harness/hot_pool_sim.das). Unset: no trace."
    moe_trace : string = ""
}

// ===== Vision =====

[EnvConfig(env_prefix = "DASLLAMA")]
struct public VisionEnv {
    @clarg_default_doc = "40 (the gemma-4 model default)"
    @clarg_doc = "Minimum soft tokens per image; dynamic-resolution geometry upscales to reach it."
    vision_min_tokens : int = 40

    @clarg_default_doc = "280 (the gemma-4 model default)"
    @clarg_doc = "Maximum soft tokens per image; dynamic-resolution geometry downscales to fit."
    vision_max_tokens : int = 280

    @clarg_doc = "Path prefix: dump every letterboxed image fed to a vision embedder as <prefix><tag>.ppm - the human smoke test; empty = off."
    vision_dump : string = ""

    @clarg_doc = "Serve the image turn as ONE fused prefill (per-query span mask); 0 pins the three-eval splice (head / media rows / tail) - the parity A/B rail."
    span_fuse : bool = true
}

// ===== Metal backend =====

[EnvConfig(env_prefix = "DASLLAMA_METAL")]
struct public MetalEnv {
    @clarg_path
    @clarg_doc = "Directory to write each kernel's generated source as <entry>.metal when its pipeline is built - the SPIR-V dump's twin, for diffing a kernel across a change."
    msl_dump : string = ""

    @clarg_doc = "Produce logits on the GPU; 0 pulls the classifier back to the CPU. Blob-only models force it on."
    logits : bool = true

    @clarg_doc = "Tiled QK/AV prefill attention (~10x the trio GEMMs); 0 pins the trio."
    attn : bool = true

    @clarg_doc = "Serve the non-causal media span on the prefill driver (AttnArgs.uend); 0 declines it to the CPU arm, which then needs declared CPU intent."
    span : bool = true

    @clarg_doc = "Serve tower/embedder encodes on the Metal tower driver - the gemma4uv chain, the gemma4v ViT and gemma3v SigLIP block loops (their exact-plane lanes), the whisper-class block loops with their projector tails, qwen3a's front and projector, the gemma4a Conformer and the canary / parakeet FastConformer block loops (every audio family's q8 lane off the q8 slab, and its exact lane on the halfword plane under DASLLAMA_METAL_TOWER_F16 and a tensor crown), the whole StyleTTS2 (kitten / kokoro) synthesis - PL-BERT, the text and duration encoders, the duration head, prosody, the decoder, the harmonic source, the generator and the inverse STFT - on both weight lanes, the Pocket TTS codec and frame loop, the conv frontends (whisper im2col + the qwen3a conv2d, which serves the q8 default), the GigaAM encode and CTC head (all three weight lanes: the f16 halfwords, the q8 slab, the kq lane's own planes), and the silero VAD seat; 0 loads each family's CPU image, the lane whole on the CPU."
    tower : bool = true

    @clarg_doc = "Serve a tower's halfword GEMM weights on the crowned tensor mul_mm twins: every audio family's exact lane on Metal - the whisper-class towers, qwen3a, canary, parakeet, gemma4a and GigaAM - whose image carries the halfword plane alone, and a vision tower's baked f16 twin plane where its mint bakes one (only for f16-sourced GEMMs, so it holds the file's own values). Needs a tensor crown: 0, or no crown, maps an audio family's exact lane to its CPU image and keeps a vision tower on the f32 route - the A/B rail."
    tower_f16 : bool = true

    @clarg_doc = "Serve head-72 tower attention at %64 canvases on the lifted flash kernel (no score slab, no rowstat pass, no head restrides - the gemma3v-geometry ViTs); 0 pins the three-pass slab - the A/B rail."
    tower_flash : bool = true

    @clarg_doc = "Serve the whisper decoder side (cross-KV; the decode step under wdec_step) on the Metal ASR-decoder driver; 0 loads the lane's CPU image, the whole lane on the CPU."
    wdec : bool = true

    @clarg_doc = "Serve the whisper DECODE STEP on the GPU too (needs wdec); 0 loads the lane's CPU image, the whole lane on the CPU."
    wdec_step : bool = true

    @clarg_doc = "Bake a whisper decoder narrower than 512 (tiny) into its Metal image with the decode STEP on the CPU - the device step's per-dispatch latency loses at that width; cross-KV stays on the device; that image is its own file (`-cstep`), keyed on the box's CPU configuration. 0 bakes the step onto the device at every width - the A/B rail."
    wdec_small_cpu : bool = true

    @clarg_doc = "Serve the transducer greedy decode (parakeet, GigaAM's RNNT head) on the Metal ASR-decoder driver: the joint's encoder projection off the encode's device rows and the whole loop in command buffers of rounds, the tokens read back once; 0 pins the CPU decoder on a CPU image - the A/B rail - while a Metal image's decode refuses: the image never runs on the CPU."
    rnnt : bool = true

    @clarg_doc = "Fused single-pass decode attention (a head of 64, 96, 128, 256 or 512 on every KV codec; sliding windows, attention sinks, the logit soft cap and a second head class included), on the single row, the speculative verify's rows and the batch; 0 is the A/B rail to the chunked pair."
    attn_d : bool = true

    @clarg_doc = "Row count below which attention uses the single-chunk kernel; clamped to 128."
    attn_single : int = 64

    @clarg_doc = "The mul_mm prefill GEMM; 0 falls back to the legacy per-op path."
    mulmm : bool = true

    @clarg_doc = "GEMV-tail prefill dispatch: npos % 32 in [1,8] peels the last M tile's real rows onto the fixed-B GEMV family instead of billing a full 32-row tile; 0 pins the padded-tile path (the A/B rail)."
    mm_tail : bool = true

    @clarg_doc = "The K-quant rows sites (two to eight rows: the verify, the batch step, the prefill's short window) read x as the f16 twin the row producer stores beside it, for every format whose half-x route the box crowned (kq_rows_half_<fmt>); 0 keeps the f32 forms under every crown (the A/B rail)."
    kq_half : bool = true

    @clarg_doc = "The per-32 planes' one-row GEMV (q5_1, iq4nl32, q4032, q5032 and the affine members) takes its wide form - a whole block a lane, x prescaled so an affine code decodes in place, one scale fold a block - on every plane of 512 rows or more; 0 pins the two-row and K-split forms (the A/B rail)."
    b32_wide : bool = true

    @clarg_doc = "The single row's dense FFN on a per-32 member (gate and up planes of one member in one buffer, 512 rows or more, the wide form on) reads both planes in one dispatch that stores act(gate) * up - no activation pass, no gate and up rows written; off by default - the 27B step measures it slower than the two GEMVs and the activation pass it replaces; 1 is the experiment rail."
    b32_glu : bool = false

    @clarg_doc = "The per-32 routed stacks' expert-indexed GEMV (q5_1, q5032 and the affine members; the codebook-table members iq4nl32 and q4032 keep the two-row form) takes the wide form - a whole block a lane, one expert row a simdgroup - on every expert of 512 rows or more; 0 pins the two-row form (the A/B rail)."
    b32_moe_wide : bool = true

    @clarg_doc = "The rows sites of three to eight rows (the verify, the batch step's rows tier) take the eight-row tensor stamps - a K-quant plane's where the box crowned its tensor tile (kq_mulmm_<fmt>), a q8 plane's under mulmm_q8: W streamed and decoded once for the panel on the tensor op, where the row forms stream it twice or decode it once a column; 0 keeps the row forms (the A/B rail)."
    rows_mm8 : bool = true

    @clarg_doc = "The rows sites of three to eight rows take the matrix-unit form where the box's layout probe passed: a 16 x 8 product a weight group with the codes filled from registers as exact halves and the scale applied once in fp32, K split over a threadgroup's eight simdgroups and met in threadgroup memory, no reduce dispatch; a k4, k6, iq4xs or q8 plane's and an affine member's (DASLLAMA_METAL_PREFILL_SKIP=rows8nomlx keeps the members on the small-batch forms, rows8noiq4xs the iq4xs planes on the eight-row stamps). 0 keeps the eight-row stamps (the A/B rail)."
    rows_mt : bool = true

    @clarg_doc = "The fewest rows a rows site carries on the eight-row forms (the matrix-unit form, the eight-row tensor stamps), its panel padded to eight; fewer rows take the row forms."
    rows8_min : int = 3

    @clarg_doc = "A step of two to sixteen rows reads each distinct routed expert once over the rows that picked it (the rows' expert union) instead of once a (row, slot), over iq4xs stacks: the fused pair's per-row arithmetic over each expert's rows, its values the pair's bit for bit. 1 is the oracle rail, never a serving default - it measures slower on the 35B's verify, whose repeated expert reads already hit in the cache; 0 keeps the fused pair."
    moe_union : bool = false

    @clarg_doc = "The hyper-connection mixer of a three-to-eight-row step over four streams runs as four dispatches on the matrix unit: the grouped norm into the rows8 x table with the inverse norms and the inject rows' stream partials, the q8 down split by stream, the partials' finish through silu(lo / hc) into the up's table with the scatter logits, and the q8 up with the gated stream mean in its landing; 0 keeps the eight-dispatch chain (the A/B rail)."
    hc_mix8 : bool = true

    @clarg_doc = "The one-row hyper-connection mixer over an affine member's planes (mlx4b, mlx6b, mlx8b) runs as the fused pair - the down under the grouped norm with the inject rows, the up over silu(lo / hc) with the gated stream mean - on the member's fill; 0 keeps those sites on the staged chain (the norm, the down GEMV, silu, the up GEMV, the mean), a q8 pair keeps the fused pair either way (the A/B rail)."
    hc_fused_aff : bool = true

    @clarg_doc = "The routed block's fused pair (gate + up + act, down + shared down + combine) takes mlx4b and mlx8b expert stacks on the per-32 family's stamps, the wide gathered GEMV's lane map; 0 keeps those stacks on the per-stack route (the A/B rail)."
    moe_aff_pair : bool = true

    @clarg_doc = "The DFlash2 drafter's selector on the device: its pass's command buffer ends in the top-k over the draft rows' logits and the selector's walk, and the verify embeds its rows off the walk's picks, committed after the pass with no host wait between; 0 lands the logits and walks them on the host after a waited pass (the A/B rail)."
    dflash_select : bool = true

    @clarg_doc = "The DFlash2 drafter's device selector also draws a rejection-sampling round's drafts: each position's pick one draw of the stream's rng from the softmax of its top-k selector scores at the drafter's temperature, the candidates, their q and the stepped rng landed beside the picks instead of every draft row's logits; 0 lands the logits and draws on the host (the A/B rail)."
    dflash_soft_pick : bool = true

    @clarg_doc = "A rejected speculative round's recurrent replay is committed with no host wait: the next pass and verify queue behind it on the device while the host goes on, and the replay is waited when the parked verify it reads is released - by the next verify, a plain step, a quiesce; 0 waits it in the rollback seat (the A/B rail). An n-gram model's rollback waits regardless."
    replay_unwaited : bool = true

    @clarg_doc = "A rejected speculative round's recurrent replay binds the verify's tape slot as its stage inputs - the conv rows, the projections, beta and alpha at their offsets in the slot - so a recurrent layer replays as the history advance and the scan alone; 0 copies the slot back into the stage planes first, four copies a layer (the A/B rail)."
    replay_tape : bool = true

    @clarg_doc = "The DFlash2 drafter's greedy accept on the device: the verify a held pass feeds ends in the accept over its rows' picks and the drafted ids, landing the count, every row's pick and the committed row's logits for the host instead of every row's logits; 0 lands every row's logits and takes the committed row on the host (the A/B rail)."
    dflash_accept : bool = true

    @clarg_doc = "The DFlash2 round's verify encoded while its pass runs. 2 (default): a round whose verify reads no drafted id on the host before its join holds its pass unwaited under every acceptance, not only the greedy one - a sampled round's verify (the selection landing asking no penalty window over the drafts) is encoded and committed behind the pass, embedding its rows off the selector's picks, and the picks, with a drawing pass's candidates, q and stepped rng, land after the verify - and a hyper-connection or n-gram side-input model's round has its verify encoded while the pass runs, then the pass waited, the picks landed, the side rows gathered and poked, and the verify committed; 1 leaves the side-input model's verify encoded after its pass; 0 holds the greedy round of a model with neither alone (the A/B rails)."
    verify_ahead : int = 2

    @clarg_doc = "An n-gram side-input model's speculative commit stage as two buffers instead of five waited ones: the conv ring's save and the verify's rows into the ring encoded at the verify's tail, and a rejected round's ring restore and committed rows encoded after the recurrent replay in its buffer, which then commits unwaited like any model's replay; 0 runs the five waited buffers (the A/B rail)."
    commit_one : bool = true

    @clarg_doc = "Apply the box profile's tensor-lane crowns (its runtime.metal_tensor set), so every family the box's race crowned serves its tensor twin; 0 leaves the set empty and every family on its base form past the routed twins' knee (`set_metal_moe_kq_twin_avg`) and the dev-W route, which ride the toolchain probe - what a GPU without the tensor lane (the M4 class) serves - the arm that proves a change, and the base kernel set, on that class from an M5."
    tensor : bool = true

    @clarg_doc = "The dense Q8_0 and K-quant prefill sites read a stored f16 converted copy of their planes (the dev-W panels the image bakes, or the resident ones); the A/B rail of the default - per-window scratch panels past each site's row floor, its own bytes under it - which the image lane name carries as -devw."
    devw : bool = false

    @clarg_doc = "Fused prefill qk_norm+rope pass (one panel rewrite, q panel dual-stores its f16 twin); 0 restores the split norm/rope/cvt dispatches (the A/B rail)."
    qk_rope : bool = true

    @clarg_doc = "1-D-grid tall hmm GEMM twin: the threadgroup launch order is forced linear (M-fastest) instead of the driver's 2-D walk; experiment rail, off by default."
    grid1d : bool = false

    @clarg_doc = "Last-layer FFN tail narrowing: the final dense layer's FFN runs the last row alone (only the classifier reads past the final attention); 0 pins the full-panel tail (the A/B rail). Session keep_hidden opts a forward out."
    lastrow : bool = true

    @clarg_doc = "Double-buffered W staging on the dense staged prefill GEMMs (q8 and kq stamps; the MoE family measured a loss and stays single-tile): two ping-pong tiles, one barrier per 64-chunk, staging overlaps the tensor op; 0 pins the single-tile forms (the A/B rail)."
    dbuf : bool = true

    @clarg_doc = "Split-K on the short window's prefill tiles (q8, every dense K-quant, grid and per-32 format, bf16): a 32-row tile grid of at most half the sidecar's metal_split_k_tgs threadgroups divides its K across up to eight threadgroups a tile, the partial planes summed by one reduce dispatch; 0 pins the whole-K tiles (the A/B rail)."
    split_k : bool = true

    @clarg_default_doc = "~4 layers/chunk"
    @clarg_doc = "Command-buffer split: each chunk commits as encoded so the scheduler overlaps chunk k with k-1."
    ncb : int = 0

    @clarg_doc = "Skip per-dispatch retain/release on the prefill command buffers."
    unretained : bool = false

    @clarg_default_doc = "0 (off)"
    @clarg_doc = "MB budget for RESIDENT dev-W f16 panels under DASLLAMA_METAL_DEVW=1: static weights dequantize ONCE and the panels persist across forwards (vs the per-forward scratch re-dequant). Sites past the budget fall back to scratch."
    devw_resident : int = 0

    @clarg_doc = "Record each prefill chunk as a step graph and replay on a concurrent encoder; 0 is serial-encode rollback."
    pf_capture : bool = true

    @clarg_doc = "Graph scheduling mode for replay; 0 keeps capture order."
    sched : int = 1

    @clarg_doc = "Concurrent encoder for the single-stream decode step, with barriers only at detected hazards."
    decode_concurrent : bool = true

    @clarg_doc = "Every decode sandwich seam as one residual step (the branch's post norm, the add, the next norm); 0 restores the split chain for A/B."
    fuse : bool = true

    @clarg_default_doc = "-1 (adaptive)"
    @clarg_doc = "The single-row Metal decode's greedy chain (the next step pre-encoded on the GPU's own argmax): 0 off, 1 forced, -1 adaptive (backs off after a miss). A session sampled at temp > 0 never chains, whatever the mode."
    spec : Option<int>

    @clarg_doc = "Rows per MoE router/expert GEMV threadgroup, clamped 1..32 (the dense GEMV geometry is fixed)."
    gemv_tg : int = 4

    @clarg_doc = "The single-row decode's step wait spins on the command buffer's GPU end time before the blocking wait: the calling thread owns the step and a blocking wake returns late on a box whose cores the jobque lanes fill (M4 Pro: 0.35 ms per token, a step every third step); 0 blocks in waitUntilCompleted alone - the A/B rail."
    wait_spin : bool = true

    @clarg_doc = "Single-pass B8 twin for K-quant small-batch mv at B=5..8; 0 is the A/B rail."
    kq_b8 : bool = true

    @clarg_doc = "The decode step's fused dispatches: 0 the plain chain, 1 the fat-threadgroup iq4xs GEMV shape at every single-row site it serves and the deltanet conv + scan + gate as one dispatch wherever a group holds one row (the single row, the batch's rows), 2 the single row's fused chain as well - the layer norms in the GEMV prologues, the residual adds in the wo / deltanet-out epilogues, the router under its own norm, the routed gate, up and act in one dispatch and the routed down with the shared expert's and the combine onto the residual in one, no seam dispatch (a model every layer of which the chain serves; the plain chain elsewhere)."
    kq_fat : int = 2

    @clarg_doc = "The device-side KV mirror's idle budget in MiB: the bytes idle mirrors evict down to once a step is served. A live step's demand is bounded by the device's largest buffer alone (unified memory: what fits, serves), and a step past that takes the CPU rails with one logged line."
    kv_mirror_mb : int = 4096

    @clarg_doc = "A released paged session's KV mirror goes to the next session whose leading pages are the same pool groups, its rows not uploaded again; 0 uploads every attached row - the A/B rail."
    mirror_adopt : bool = true

    @clarg_doc = "An f16-KV prefill writes its K/V rows into the session's KV mirror and its attention reads the cached rows there (the device pair, at any row count), so a continuation gathers nothing on the CPU and the first decode step uploads nothing; without the attn_dev crown the attention keeps its f32 panels and the gather, and the whole panel still converts into the mirror, so that first step uploads nothing there either; 0 keeps the panels and leaves the mirror to the decode step's upload - the A/B rail."
    pf_mirror : bool = true

    @clarg_doc = "Pin the served working set (weight regions, planes, the recycled pool buffers) in an MTLResidencySet, macOS 15+: the per-commit residency pass stays a no-op, so the first submission after a CPU-only window stops repaying it (~15 ms/encode on the M1 Max released-rig image cell; 0 is the A/B rail."
    residency : bool = true

    @clarg_doc = "Seconds the residency-set heartbeat keeps re-requesting residency after the last served step (a requested set still un-wires over a long CPU-only window; 0 disables the heartbeat - the A/B rail."
    heartbeat_s : int = 180

    @clarg_doc = "Barrier at every dispatch instead of at tracker-detected hazards (correctness bisect)."
    hazard_paranoid : bool = false

    @clarg_doc = "Treat every detected hazard as strict, widening barriers (correctness bisect)."
    hazard_strict : bool = false

    @clarg_doc = "Per-step pipeline trace: GPU envelope and true inter-step handoff idle, first steps plus outliers."
    pipe_debug : bool = false

    @clarg_doc = "Comma-separated dispatch names to skip in decode - a bring-up bisect that breaks correctness."
    decode_skip : string = ""

    @clarg_doc = "Comma-separated kernel-name fragments: a dispatch of a kernel whose census name holds one is bound but not issued - per-kernel timing attribution, output is garbage."
    kernel_skip : string = ""

    @clarg_doc = "Prefill twin of DASLLAMA_METAL_DECODE_SKIP."
    prefill_skip : string = ""

    @clarg_env = "DASLLAMA_MTP_DEBUG"
    @clarg_doc = "MTP bring-up bisect: cold logs why the Metal seats left a round to the plain step (the forced reject is set_mtp_force_reject_every)."
    mtp_debug : string = ""

    @clarg_doc = "Batch size at which batched decode switches from GEMV to GEMM; floor 2."
    batch_gemm_min : int = 5

    @clarg_doc = "Split-K in the batched decode GEMM."
    batch_sk : bool = true

    @clarg_doc = "Command buffers per batched decode step, clamped 1..16."
    batch_ncb : int = 4

    @clarg_doc = "Concurrent encoder for batched decode."
    batch_concurrent : bool = true

    @clarg_doc = "Epilogue fusion in batched decode."
    batch_fuse : bool = true

    @clarg_doc = "Fixed-B GEMV forms for small batched decode."
    batch_mv : bool = true

    @clarg_doc = "The column-tiling MV8 probe for batched decode."
    batch_mv8 : bool = true

    @clarg_doc = "Pipelined batched decode submission."
    batch_pipe : bool = false

    @clarg_doc = "The batched decode pre-encodes the next step under the current one's GPU run and pokes its token inputs at commit. Off by default: it times the same as building at the call and holds a second step's buffers."
    batch_pre : bool = false

    @clarg_doc = "The mul_mm rail for batched decode; measured negative at M-pad-32, kept as the A/B rail."
    batch_mm : bool = false
}

// ===== Vulkan backend =====

[EnvConfig(env_prefix = "DASLLAMA"), arch(at="../ARCHITECTURE_RUNTIME.md#env-knobs")]
struct public VulkanEnv {
    @clarg_doc = "Probe and serve Vulkan on macOS (MoltenVK). Off, the Vulkan home declines on macOS by name - no device, no arm - and every Vulkan test skips on its no-device path: the Mac build carries the module so its sources compile there, and Vulkan runtime evidence comes from a Linux box. On, the macOS probe runs as on any other host"
    vk_mac : bool = false

    @clarg_default_doc = "auto"
    @clarg_doc = "Cooperative-matrix mode (auto = cm2 where the device has NV_cooperative_matrix2, else mm, else sdot4; `f16` and `int8` pick the KHR f16 and int8 batch tiles, which auto never does); the flash-attention twin needs it even when the GEMM runs sdot4."
    coopmat : string = ""

    @clarg_default_doc = "32"
    @clarg_doc = "Small-batch tier: 32 = sdot4 (default, beats both coopmat tiles below the crossover), 64 = coopmat M, 128 = always-L."
    mm_small : string = ""

    @clarg_doc = "Small-d cutoff routing narrow roles (k/v) to the small tier; widening measured worse, so this is an instrument."
    mm_smalld : int64 = 64l

    @clarg_doc = "Fused add+rms+requant on every site of the decode token command and the prefill's batch pairs, plus the fused qk-norm+rope; 0 pins the split dispatches for a same-build A/B."
    vk_fuse : bool = true

    @clarg_doc = "The whole-model driver's greedy chain: the next one-row step submitted behind the running one on the device's own pick; 0 off, 1 every greedy step, -1 adaptive (backs off after a miss). A sampled session never chains."
    vk_chain : int = 0

    @clarg_doc = "The resident prefill's FFN GEMMs (gate, up, down) take the f16 activation feed where the tile family admits it; 0 pins the FFN group to the Q8 activation blocks the CPU chain quantizes, while the attention group keeps its own pick - the A/B rail that takes the feed's rounding out of a resident-vs-CPU compare (gemma's logits move with it)."
    vk_f16_ffn : bool = true

    @clarg_doc = "Stream expert uploads on the dedicated transfer queue, overlapped via a timeline semaphore, and land the resident token command's logits there behind a compute-timeline wait; 0 keeps the single-queue rail, where the token command copies its own logits under its fence."
    vk_xferq : bool = true

    @clarg_doc = "Run the cm2 tiles' four-wide decode callback where the device has VK_NV_cooperative_matrix_decode_vector; 0 strips it and serves the scalar callback - the same-build A/B and the fallback probe."
    vk_decvec : bool = true

    @clarg_doc = "The resident prefill window's per-layer-embedding gate and projection serve a file's f32 rows on the staged KHR tile (the rows' words rounded to f16 on the tile's stage ahead of the KHR multiply, at subgroup 32); 0 keeps them on the scalar f32 tile - the same-build A/B."
    vk_ple_f32_tile : bool = true

    @clarg_doc = "Stream mirrors import the mapped .dlim (VK_EXT_external_memory_host) instead of pinned copies, on the copy arm alone (`DASLLAMA_VK_STREAM_FILE_READ=0`): the file-read arm, the default, reads into mirrors of its own and never imports; =0 restores the copy path."
    vk_import : bool = true

    @clarg_doc = "A streamed stack's planes are read off the .dlim file straight into its pinned memory, the mapping's expert pages never faulted; =0 copies them out of the mapping (the A/B arm)."
    vk_stream_file_read : bool = true

    @clarg_doc = "The expert cache's pinned budget in MiB for the streamed layers' expert stacks: the resident slots of every streamed layer and the transient ring together; 0 holds every streamed expert resident (the whole stacks in pinned memory, as a box with the RAM does). Under it each streamed layer keeps a quota of its experts resident and the rest are read off the lane file on demand into the ring, through the format's row-major kernels on the host and the DMA on the device alike; needs a mapped lane (a chunk-built image has no file to read from)."
    expert_cache_mb : int64 = 0l

    @clarg_doc = "The expert cache's transient ring in experts a plane kind (gate, up, down): the slots the on-demand reads land in, freed at a window's or a token's end. 0 sizes it to the largest streamed layer's non-resident count, so one window's misses always fit."
    expert_ring : int64 = 0l

    @clarg_doc = "The expert cache's reader threads: the on-demand reads off the lane in flight at once, each thread one read at a time on its own file handle. 0 reads on the calling thread, one at a time."
    expert_readers : int64 = 8l

    @clarg_doc = "The experts one bundle of the streamed expert chain takes at most (0 = a sixteenth of the layer's experts, 1 to 32). A bundle is the unit the device claims at the meet, at most two in flight, so the cap sets the device's pace against the host's share; the A/B lever."
    vk_xf_bundle : int64 = 0l

    @clarg_doc = "Mint the TRIMMED GPU lane - the armed home's, vulkan or cuda (the default; =0 mints the lane with its planar families): the device twin plus the packed emb region the CPU embed reads, the planar weight families dropped - a MoE's expert stacks too, the device holding them or the host reading its streamed layers' pinned mirrors through the rare rail (so the prompt stream and the rail's arena arm must be on where routed layers serve on the host). A model the whole-model driver would not take whole keeps its families, and the load says so, as does one with per-layer embeddings, biased expert stacks or the gemma-4 MoE form (the host reads their planar planes); a call the driver passes to the CPU rails panics by name. Folded into the lane's identity, so a flip re-bakes the lane; `set_vulkan_trim` is the in-process lever."
    trim : bool = true

    @clarg_doc = "Tag every device allocation priority 1.0 (VK_EXT_memory_priority). Off by default: on the NVIDIA WDDM driver the tag makes the tagged weights the pageable set, and a resident model then decodes at PCIe speed once anything is demoted."
    vk_memprio : bool = false

    @clarg_doc = "Vulkan flash attention: the decode fa kernel pick AND the cm2 prefill fa tile; 0 falls back to the chunked/scalar paths."
    vk_fa : bool = true

    @clarg_doc = "Serve the vision towers' block loops (gemma4v, gemma3v, qwen3v, qwen25v), the audio towers' (the whisper-class towers, gemma4a, canary) and the TTS seats (the StyleTTS2 families kitten and kokoro, Pocket) on the Vulkan tower and TTS drivers - the announce is `dasLLAMA vulkan tower: <family> serves on the device`, once per model when it first serves; 0 loads every tower lane's CPU image, the whole lane on the CPU."
    vk_tower : bool = true

    @clarg_doc = "GigaAM's lane policy on the discrete tower homes (the Vulkan tower, and the CUDA tower under the CUDA ask): 1 prefers the exact lane - the block GEMMs on the f16 GEMM over the carrier's own halfwords - over the q8 image for an un-pinned load; 0 (the default, until the two lanes are timed on a discrete card) keeps the q8 image. Both lanes serve on the device on the f16 GEMM feed alone (off it, every lane loads the CPU image); the family's lane pin overrides it, `set_vulkan_gigaam_exact_lane` / `set_cuda_gigaam_exact_lane` are the in-process levers."
    vk_gigaam_exact : bool = false

    @clarg_doc = "Serve the whisper decoder side (the per-window cross-KV and the decode step) on the Vulkan ASR-decoder driver - the announce is `dasLLAMA vulkan wdec: the whisper decoder serves on the device`, once per model; 0 loads the lane's CPU image, the whole lane on the CPU."
    vk_wdec : bool = true

    @clarg_doc = "Serve the transducer greedy decode (parakeet, GigaAM's RNNT head) on the Vulkan ASR-decoder driver: the joint's encoder projection off the encode's device rows where the lane's projection is f32, and the whole loop in command buffers of rounds, the tokens read back once; 0 pins the CPU decoder on a CPU image - the A/B rail - while a Vulkan image's decode refuses: the image never runs on the CPU."
    vk_rnnt : bool = true

    @clarg_doc = "Merged k|v prefill GEMM - one dispatch over the adjacent k+v arena planes; 0 pins the split k + v dispatches for a same-build A/B."
    vk_kv_merge : bool = true

    @clarg_doc = "The last layer's FFN runs on the window's last 32 rows only (the classifier reads one); 0 runs it over the whole window for a same-build A/B."
    vk_ffn_slice : bool = true

    @clarg_doc = "Prefill record/execute overlap: the window chain submits in ramped chunks (1,2,4,8 layers) so the GPU starts while the CPU still records; 0 pins the single fenced submit (the per-role GPU profile pins it too, so a chunk gap never bills to a role)."
    vk_overlap : bool = true

    @clarg_doc = "Layers one prefill submit holds at most (the overlap ramp's cap, 1..8); 0 = 1 on an integrated GPU, 8 on a discrete one. A Linux amdgpu resets the device under a job past its watchdog (about 10 s), which an iGPU's 8-layer chunk reaches on a long prompt."
    vk_chunk : int64 = 0l

    @clarg_doc = "Device-side token-embedding gather for the resident prefill and every token command (the one-row step, the N-row command, the NextN verify and draft): a q8 or K-quant table off its own plane (the tied classifier's, or an untied table's where it fits the room past the weights and the K/V), an F32/F16/BF16 table off its exact f32 rows uploaded under a 512 MB cap; 0 keeps the CPU embed loop. `set_gpu_resident_embed` pins it for the next loads in process."
    vk_gpu_embed : bool = true

    @clarg_doc = "Pin REQUIRE_FULL_SUBGROUPS on every class pipeline (instrument; measured slower than plain pipelines on the mm_a gate shape, so those are the default)."
    vk_fullsg : bool = false

    @clarg_doc = "The subgroup width every class pipeline pins where the device's own differs and its compute stage may pin it (Vulkan 1.3 subgroupSizeControl; RDNA runs 32 or 64 lanes and defaults to 64): the coopmat tiles, the h128 attention twin and the tower tiles own four 32-lane fragments a workgroup, so a device left at 64 serves the sdot4 tiles and the scalar attention instead. 0 = the device's own width."
    vk_subgroup : int64 = 32l

    @clarg_doc = "Place the planes in the ReBAR device-local host-visible memory type (one larger than 1 GB present) and write uploads straight into them. Off by default: on the NVIDIA WDDM driver a mapped plane is a host-side shadow the manager parks in system memory under pressure, and a host-experts plan then decodes at PCIe speed (gemma-4 26B Q8_0 8.1 tok/s mapped against 32.7 staged, the 35B-A3B 38 against 71: debug-jit readings, the ReBAR entry in modules/dasLLAMA/PERF_LEDGER.md) - the default stages every upload into unmapped device-local planes."
    vk_rebar : bool = false

    @clarg_doc = "Arm the resident driver with f32 KV mirrors instead of the f16 default (A/B instrument; only sessions of the armed codec are served)."
    vk_kv32 : int64 = 0l

    @clarg_doc = "`benchmarks/lcpp_bench.das --npl N` only: drops a class of dispatch from the recorded N-row token command so a profile prices it - bits 1 = no attention, 2 = the one-column GEMVs (row 0 alone), 4 = no q/k norm and no rope, 8 = no activation requant; the logits are garbage under any bit. The bits reach the head and the dense triple; the routed block and the per-layer-embedding branch run their one-row leaves and column forms whole."
    vk_nrow_bisect : int64 = 0l

    @clarg_doc = "Keeps a fused form of the token command on its split dispatches (a same-build A/B; the logits stay right; the N-row command shares the gate-up and the fused q/k norm through the layer flags read at load) - bits 1 = the wo GEMV's residual epilogue, 2 = the down GEMV's, 4 = the fused gate-up twin, 8 = the prologue's add+rms+requant stamp, 16 = the post-attention one (the wo epilogue too), 32 = the post-FFN one (the down epilogue too), 64 = the fused qk-norm+rope; the load log names each bit as taken or inert on the model"
    vk_fuse_bisect : int64 = 0l

    @clarg_doc = "cm2 prefill tile pick: 0 = the wave model's pick (`cm2_gemm_pick` on Vulkan, the CUDA towers' own `cut_cm2_tc`), 32 = force the s tile, 128 = force the m tile, 256 = force the l tile (A/B instrument: on Vulkan the window's and the towers' GEMMs; on CUDA the towers' GEMMs alone, the column held at a split site too - the CUDA window keeps its own pick, `cud_cm2_pick`, and never reads this knob; the f16 lane's f16 tile, which has no l column, takes 32 and 128 alone, its own pick `f16_cm2_col` otherwise). Inert on the KHR arm (DASLLAMA_COOPMAT=mm, or a device without NV_coopmat2), whose kq tile has one geometry."
    cm2_tile : int64 = 0l

    @clarg_doc = "cm2 split-k: 0 = the pick's own (the Vulkan wave model `cm2_gemm_pick`; the CUDA home's `cud_cm2_pick` splits a grid short of two blocks an SM until it fills the card and runs a full grid whole), 1 = off, N = force N k-chunks on either home (A/B instrument; shrinks if N strands an empty tail). On the KHR arm the chunks ride its one 128-column geometry."
    cm2_splitk : int64 = 0l

    @clarg_doc = "The CUDA home's window chain on the MMQ GEMMs: the int8 one (`q8_mmq` and the leaves of Q4_K, IQ4_XS, Q4_0, IQ4_NL, Q5_0, Q5_1), the activations requantized to Q8_0 blocks between the GEMMs, as the token step's feeds are; and NVFP4's FP4 tensor leaf (`nv4_mmq`, W4A4: the activations quantized to e2m1 per token); a routed layer's MXFP4 or NVFP4 expert stacks ride the same rung fed from the expert schedule (`mx4_fp4_mmq_moe` / `nv4_fp4_mmq_moe`, the gathered rows as the stacks' FP4 feed; 0 keeps them on the cm2 s and e tiles). 1 = every plane whose format's leaf beat the cm2 tiles in the measured cards' windows, where its shape fits the tile (the output width a multiple of 128, the reduction of 256), runs on that leaf, the other planes on the f16-fed cm2 tiles; 2 = every leaf, the ones the cards split or lost on too (Q5_0, Q5_1) - a new card's own measurement; 0 = the cm2 tiles everywhere (A/B instrument). The FP4 leaf needs compute capability 120; below it an nv4 plane stays on the tiles under every value. The first window reads the value once and keeps it. The load log names the pick, and the value with the setter that gave it (`set_cuda_mmq_pin` under a test's pin)."
    cuda_mmq : int64 = 1l

    @clarg_doc = "The CUDA home's launches under programmatic dependent launch: a kernel is launched while the one before it in the stream still runs, and its entry wait (`griddepcontrol.wait`, in every emitted kernel on sm_90 and up) holds its reads until that one completes, so the launch latency rides under the predecessor's tail. 1 = on where the card is sm_90 or newer, 0 = plain stream order (A/B instrument). The device-ready log line names the state."
    cuda_pdl : int64 = 1l

    @clarg_doc = "The window chain's deltanet scan on the discrete homes: 1 = the chunked form on the 16 x 16 cooperative tiles (`dn_scan_chunk_cls`: sixteen tokens a chunk, the state advanced once a chunk) wherever the device has them - CUDA, and Vulkan with KHR cooperative matrices at subgroup 32 - the per-token scan elsewhere; 0 = the per-token scan everywhere (A/B instrument). The token step's fused scan is not this knob's."
    dn_chunk : int64 = 1l

    @clarg_doc = "The CUDA home's greedy chain: the next one-row step launched behind the running one on the device's own pick; 0 off, 1 every greedy step, -1 adaptive (backs off after a miss). A sampled session never chains."
    cuda_chain : int = 0

    @clarg_doc = "The CUDA home's tower chains replayed from captured graphs: a chain whose rail state recurs is walked dry (each launch's parameters hashed, nothing launched) and replayed as one graph launch where the walk matches the captured one; 1 = on, 0 = every chain launched kernel by kernel (A/B instrument)."
    cuda_graphs : int64 = 1l

    @clarg_path
    @clarg_doc = "Directory of <kernel>.spv files served instead of the emitted words at pipeline creation (offline spirv-opt / hand-patched A/B instrument)."
    vk_spv_override : string = ""

    @clarg_path
    @clarg_doc = "Directory to write each kernel's emitted words as <kernel>.spv at pipeline creation (the override instrument's capture half)."
    vk_spv_dump : string = ""

    @clarg_doc = "Barrier at every dispatch (correctness bisect)."
    vk_hazard_paranoid : bool = false

    @clarg_doc = "Log every detected hazard and the barrier it produced."
    vk_hazard_trace : bool = false
}

// ===== MoltenVK (foreign knobs the vulkan tier arms on Apple) =====

[EnvConfig(env_prefix = "MVK_CONFIG")]
struct public MoltenVkEnv {
    @clarg_default_doc = "unset (tier forces 0)"
    @clarg_doc = "MoltenVK's own knob. Its argument-buffers default miscompiles the batch tile kernels (stores never land, silently); the tier forces discrete bindings when this is unset."
    use_metal_argument_buffers : Option<bool>
}

// ===== Accelerate / AMX backend =====

[EnvConfig(env_prefix = "DASLLAMA")]
struct public AccelerateEnv {
    @clarg_doc = "Arm the Apple Accelerate / AMX float-batch override."
    accel : bool = false

    @clarg_doc = "Use the BNNS f16 lane inside the Accelerate backend."
    accel_f16 : bool = false

    @clarg_default_doc = "0"
    @clarg_doc = "Strip count for the f16 lane; 0 lets the backend choose."
    accel_f16_strips : Option<int64>

    @clarg_default_doc = "d/256, max 8"
    @clarg_doc = "Strip count for the f32 sgemm lane; 0 keeps the shape-derived default."
    accel_strips : Option<int64>

    @clarg_default_doc = "backend default"
    @clarg_doc = "Minimum MMAC count below which Accelerate declines and the daslang kernel runs."
    accel_min_mmac : Option<int64>

    @clarg_default_doc = "32"
    @clarg_doc = "Minimum token count for the Accelerate float-batch override, floor 1."
    accel_min_ntok : Option<int64>
}

// ===== Harness — tuner and probes =====

[EnvConfig(env_prefix = "DASLLAMA")]
struct public HarnessEnv {
    @clarg_path
    @clarg_doc = "Directory holding the .gguf models the probes, benches and tests load. dasllama-server's model catalog downloads here too when set."
    models_dir : string = ""

    @clarg_path
    @clarg_default_doc = "auto-resolved from the models dir"
    @clarg_doc = "Model used by the tuner's confirm gate (FULL path, not a bare filename). Unset: the gate auto-resolves from the models dir - the preferred confirm carrier, else the largest present q8 gguf; the fallback pins only when the box has no q8 model at all."
    confirm_model : string = ""

    @clarg_path
    @clarg_default_doc = "auto-resolved from the models dir"
    @clarg_doc = "IQ2_XXS vehicle for the tuner's serving-crown confirm (FULL path). Unset: the confirm auto-resolves the first *IQ2_XXS*.gguf in the models dir; none found = no crown (the base kernel serves)."
    confirm_iq2xxs : string = ""

    @clarg_path
    @clarg_default_doc = "auto-resolved from the models dir"
    @clarg_doc = "gemma-4 vehicle for the tuner's assistant-depth confirm (FULL path; its mtp- head must sit beside it). Unset: the confirm auto-resolves the first gemma-4*.gguf with a head in the models dir; none found = depth 1 serves"
    confirm_mtp_gemma : string = ""

    @clarg_doc = "Override the batched-dispatch chunk count in the 1-core GEMM probe."
    batch_chunks : string = ""

    @clarg_doc = "Model filename (inside the models dir) the image-turn attribution probe loads."
    probe_model : string = "Qwen3VL-8B-Instruct-Q8_0.gguf"

    @clarg_doc = "harness/rows8_mt_race.das: a comma list of the arm names to race; empty races every arm."
    rows8_arms : string = ""

    @clarg_doc = "harness/rows8_mt_race.das: a comma list of words, a shape racing where its tag holds one; empty races every shape."
    rows8_shapes : string = ""

    @clarg_doc = "harness/rows8_mt_race.das: the rounds of the in-threadgroup split race, each timing every arm once."
    rows8_rounds : int = 3

    @clarg_doc = "harness/rows8_mt_race.das: race the in-threadgroup K split arms against the split-K form and its reduce."
    rows8_tg : bool = false

    @clarg_doc = "harness/moe_pair_race.das: a comma list of the arm names to race; empty races every arm (the two bases always run)."
    moe_pair_arms : string = ""

    @clarg_doc = "harness/moe_pair_race.das: per mille of a later row's picks drawn from the earlier rows' experts (450 reads ~38 distinct of 64 at eight rows)."
    moe_pair_reuse : int = 450

    @clarg_doc = "harness/moe_pair_race.das: the rows (streams), 1 to 8."
    moe_pair_rows : int = 8

    @clarg_doc = "harness/moe_pair_race.das: mlx4b races the engine's affine stamps on the Flash-Next MLX pack's routed block (the per-stack route against the fused pair, at one row and at eight) in place of the iq4xs raw-MSL forms."
    moe_pair_fmt : string = "iq4xs"

    @clarg_doc = "harness/hc_mix_race.das: a comma list of the arm names to race; empty races every arm (the two bases always run)."
    hc_mix_arms : string = ""

    @clarg_doc = "harness/rows8_tensor_probe.das: probe only the cooperative layout of this name; empty probes every one."
    rows8_probe : string = ""

    @clarg_doc = "Use the 2D batch grid in the parity probe."
    batch_grid_2d : Option<int>

    @clarg_doc = "Restrict the 1-core GEMM probe to one backend."
    focus_backend : string = ""

    @clarg_doc = "Restrict the 1-core GEMM probe to one shape."
    focus_shape : string = ""

    @clarg_doc = "Restrict the 1-core GEMM probe to one token count."
    focus_ntok : Option<int64>

    @clarg_doc = "Run the 1-core GEMM probe through the fork pool rather than inline."
    focus_forked : bool = false

    @clarg_env = "GEN_TUNE_COMPILE_ONLY"
    @clarg_doc = "Compile the generated tune probe without running it."
    gen_tune_compile_only : bool = false

    @clarg_env = "SMMLA_PROBE_COMPILE_ONLY"
    @clarg_doc = "Compile the smmla probe without running it."
    smmla_probe_compile_only : bool = false

    @clarg_env = "METAL"
    @clarg_doc = "Run the Accelerate contention probe against the Metal backend."
    metal_contention : bool = false

    @clarg_env = "DAS_DETAIL"
    @clarg_doc = "Per-job dump in the dispatch probe."
    das_detail : bool = false

    @clarg_env = "DAS_SPIN_NS"
    @clarg_doc = "Synthetic per-job spin in nanoseconds for the dispatch probe."
    das_spin_ns : Option<int64>

    @clarg_env = "DAS_BATCH_DISPATCH"
    @clarg_doc = "Exercise batched dispatch in the dispatch probe."
    das_batch_dispatch : bool = false

    @clarg_env = "DAS_TWO_WAVE"
    @clarg_doc = "Exercise the two-wave dispatch shape in the dispatch probe."
    das_two_wave : bool = false

    @clarg_env = "DAS_FORK_SKIP_HEAP_RESET"
    @clarg_doc = "Skip restartHeaps on pooled fork-context reuse (A/B for the fork-pool heap cost)."
    das_fork_skip_heap_reset : bool = false

    @clarg_env = "DAS_WORKER_SPIN_US"
    @clarg_doc = "Worker spin-before-park window in microseconds, applied via set_jobque_spin_us."
    das_worker_spin_us : Option<int>

    @clarg_env = "DAS_JOBQUE_JOIN_SPIN"
    @clarg_doc = "Join-side spin window for the 1-core GEMM probe."
    das_jobque_join_spin : Option<int>
}

// ===== Benchmarks =====

[EnvConfig(env_prefix = "DASLLAMA")]
struct public BenchmarkEnv {
    @clarg_doc = "Let a bench cell measure on fallback/absent tune winners, and skip the auto-tune mint entirely (untuned kernels keep fallback stamps instead of paying the minutes-long tune). The gate exists because a stale sidecar silently downgrades every kernel and the whole board sandbags; only set for dev runs whose numbers are never published."
    allow_untuned : bool = false

    @clarg_doc = "Model name for the isolated GEMM bench."
    bench_model : string = "tinyllama"

    @clarg_doc = "Token count for the isolated GEMM bench."
    bench_ntok : Option<int64>

    @clarg_doc = "Skip the row-major arm of the isolated GEMM bench."
    bench_skip_rowmajor : bool = false

    @clarg_doc = "Per-run detail from the comparison bench (lcpp_bench)."
    bench_verbose : bool = false

    @clarg_doc = "Override the minimum rows per dispatch chunk in the GEMM bench."
    min_chunk_rows : Option<int>

    @clarg_doc = "Exercise the fused decode arm in the emission bench."
    fused_decode : Option<bool>

    @clarg_env = "DASMETAL_LAB_ROUNDS"
    @clarg_doc = "Timing rounds per cell in the Metal kernel labs."
    lab_rounds : int = 3

    @clarg_env = "DASMETAL_LAB_PASSES"
    @clarg_doc = "Passes per round in the Metal MoE lab."
    lab_passes : int = 4

    @clarg_env = "DASMETAL_LAB_ATTN_NSGS"
    @clarg_doc = "Simdgroups per threadgroup in the Metal attention lab."
    lab_attn_nsgs : int = 16

    @clarg_env = "DASMETAL_LAB_DUMP_MSL"
    @clarg_doc = "Dump generated MSL from the Metal labs instead of running it."
    lab_dump_msl : string = ""

    @clarg_env = "DASMETAL_LAB_ARMS"
    @clarg_doc = "Substring filter restricting which arms the Metal MoE lab runs."
    lab_arms : string = ""

    @clarg_env = "DASMETAL_LAB_SHAPES"
    @clarg_doc = "Substring filter restricting which shapes the Metal GEMV/GEMM labs run."
    lab_shapes : string = ""

    @clarg_env = "DASMETAL_LAB_VARIANTS"
    @clarg_doc = "Substring filter restricting which kernel variants the Metal GEMV/GEMM labs run."
    lab_variants : string = ""

    @clarg_env = "PROBE_PATH"
    @clarg_path
    @clarg_doc = "Scratch file the write-cliff probe writes, cwd-relative by default; point it at the drive under test."
    probe_path : string = "_wcliff.bin"

    @clarg_env = "PROBE_GB"
    @clarg_doc = "Gigabytes the write-cliff probe writes before reporting."
    probe_gb : int64 = 50l
}

// ===== Profiling and baselines =====

[EnvConfig(env_prefix = "DASLLAMA")]
struct public ProfilingEnv {
    @clarg_env = "DASLANG_BIN"
    @clarg_path
    @clarg_doc = "daslang binary the profiling harness shells out to."
    daslang_bin : string = "bin/daslang"

    @clarg_default_doc = "probed"
    @clarg_doc = "Override the box identifier recorded in a profile."
    box : string = ""

    @clarg_default_doc = "probed"
    @clarg_doc = "Override the CPU name recorded in a profile."
    cpu : string = ""

    @clarg_env = "DASLLAMA_OS"
    @clarg_default_doc = "probed"
    @clarg_doc = "Override the OS name recorded in a profile."
    os_name : string = ""

    @clarg_default_doc = "probed"
    @clarg_doc = "Override the RAM size in GiB recorded in a profile."
    ram_gb : Option<int>

    @clarg_default_doc = "probed"
    @clarg_doc = "Override the thread count recorded in a profile."
    threads : Option<int>

    @clarg_default_doc = "probed"
    @clarg_doc = "Override the GPU name recorded in a box profile."
    gpu_name : string = ""

    @clarg_env = "LLAMA_BENCH"
    @clarg_path
    @clarg_doc = "Path to the llama-bench binary used to establish reference baselines."
    llama_bench : string = ""

    @clarg_env = "LLAMA_BENCH_CLEAN"
    @clarg_path
    @clarg_doc = "Path to a clean-build llama-bench for the reference column."
    llama_bench_clean : string = ""

    @clarg_env = "LLAMA_BENCH_STOCK"
    @clarg_path
    @clarg_doc = "Path to a stock-build llama-bench for the reference column; its sibling llama-mtmd-cli serves as the metal/amx image-chat reference."
    llama_bench_stock : string = ""

    @clarg_env = "LLAMA_BENCH_VULKAN"
    @clarg_path
    @clarg_doc = "Path to a Vulkan-build llama-bench (setup_lcpp_ref's vulkan flavor) for the vulkan leg's reference column."
    llama_bench_vulkan : string = ""

    @clarg_env = "LLAMA_BENCH_CUDA"
    @clarg_path
    @clarg_doc = "Path to a CUDA-build llama-bench (-DGGML_CUDA=ON at the reference tag) for the cuda leg's reference column."
    llama_bench_cuda : string = ""

    @clarg_env = "WHISPER_CPP"
    @clarg_path
    @clarg_doc = "whisper.cpp checkout used for ASR reference baselines."
    whisper_cpp : string = "../whisper.cpp"

    @clarg_env = "WHISPER_CPP_MODELS"
    @clarg_path
    @clarg_doc = "Directory of whisper.cpp models for ASR reference baselines."
    whisper_cpp_models : string = ""

    @clarg_env = "CRISPASR"
    @clarg_path
    @clarg_doc = "CrispASR checkout used for the GigaAM ASR reference baselines (its Metal build in build/, its CPU-only build in build-cpu/)."
    crispasr : string = "../CrispASR"

    @clarg_env = "MARIAN"
    @clarg_path
    @clarg_doc = "Mozilla translations checkout whose patched inference/build/marian-decoder is the Bergamot translation reference."
    marian : string = "../translations"

    @clarg_env = "MTMD_BIN"
    @clarg_path
    @clarg_doc = "llama-mtmd-cli binary for multimodal reference baselines."
    mtmd_bin : string = ""

    @clarg_env = "DASLLAMA_BASE_PYTHON"
    @clarg_path
    @clarg_doc = "Base CPython 3.10-3.12 for creating the ASR oracle venvs, when neither PATH names nor uv resolve one in range."
    base_python : string = ""

    @clarg_env = "NEMO_PY"
    @clarg_path
    @clarg_doc = "Python interpreter of the NeMo oracle venv, for canary/parakeet baselines."
    nemo_py : string = ""

    @clarg_env = "ONNX_PY"
    @clarg_path
    @clarg_doc = "Python interpreter of the ONNX oracle venv, for parakeet baselines."
    onnx_py : string = ""

    @clarg_env = "MLX_PY"
    @clarg_path
    @clarg_doc = "Python interpreter of the MLX reference venv (mlx-audio, mlx-lm), for the Apple-GPU baselines."
    mlx_py : string = ""

    @clarg_env = "TTS_PY"
    @clarg_path
    @clarg_doc = "Python interpreter of the TTS reference venv (kokoro, kittentts, onnxruntime, pocket-tts), for the text-to-speech baselines."
    tts_py : string = ""

    @clarg_env = "GIGAAM_PY"
    @clarg_path
    @clarg_doc = "Python interpreter of the GigaAM oracle venv (upstream gigaam + torch), for the GigaAM external-reference dumps."
    gigaam_py : string = ""

    @clarg_env = "NMT_PY"
    @clarg_path
    @clarg_doc = "Python interpreter of the translation oracle venv (numpy, sentencepiece), for the Bergamot marian stage dumps."
    nmt_py : string = ""

    @clarg_env = "OS"
    @clarg_default_doc = "set by Windows"
    @clarg_doc = "Read to detect Windows (Windows_NT); set by the OS, not by dasLLAMA."
    win_os : string = ""

    @clarg_env = "PROCESSOR_ARCHITECTURE"
    @clarg_default_doc = "set by Windows"
    @clarg_doc = "Read for the CPU architecture on Windows; set by the OS, not by dasLLAMA."
    win_arch : string = ""

    @clarg_env = "NUMBER_OF_PROCESSORS"
    @clarg_default_doc = "set by Windows"
    @clarg_doc = "Read for the core count on Windows; set by the OS, not by dasLLAMA."
    win_ncpu : int = 0
}

// ===== Tests =====

[EnvConfig(env_prefix = "DASLLAMA")]
struct public TestEnv {
    @clarg_doc = "Run the FULL parity matrix. Without it the big-model rows silently skip."
    parity_full : bool = false

    @clarg_doc = "Comma-separated filter restricting which parity arms run."
    test_arms : string = ""

    @clarg_doc = "Comma-separated filter restricting which model families run."
    test_family : string = ""

    @clarg_path
    @clarg_doc = "Directory of llama2.c reference checkpoints for the forward/decode parity tests."
    llama2c_dir : string = ""

    @clarg_path
    @clarg_doc = "Directory of whisper models for the audio tests."
    whisper_dir : string = ""

    @clarg_env = "DISPLAY"
    @clarg_doc = "Ambient platform variable; read only by the browser examples' smoke tests (storyteller, storywish, parrot) to tell whether a Linux box has a window server for the example's window."
    display : string = ""

    @clarg_env = "WAYLAND_DISPLAY"
    @clarg_doc = "Ambient platform variable (the Wayland twin of DISPLAY); read only by the browser examples' smoke tests."
    wayland_display : string = ""

    @clarg_path
    @clarg_doc = "Directory of audio corpus files for the transcription tests."
    corpus_dir : string = ""

    @clarg_env = "TMPDIR"
    @clarg_path
    @clarg_default_doc = "/tmp"
    @clarg_doc = "Scratch directory for test artifacts; set by the OS on macOS."
    tmpdir : string = ""

    @clarg_env = "TEMP"
    @clarg_path
    @clarg_doc = "Windows scratch-directory fallback when TMPDIR is unset (the test runner's log dir)."
    temp_dir : string = ""
}

// ===== Examples and tutorials =====

[EnvConfig(env_prefix = "DASLLAMA")]
struct public ExampleEnv {
    @clarg_path
    @clarg_doc = "Model path for the dasLLAMA tutorials when none is passed on the command line."
    model : string = ""
}

// ===== daslang core knobs dasLLAMA honours =====

[EnvConfig(env_prefix = "DASLLAMA")]
struct public ExchangeEnv {
    @clarg_doc = "Sidecar exchange base URL override (tests, mirrors); unset = the baked-in dasllama.io. Setting it is an expressed choice to use the exchange - the first-contact consent question is skipped (announced at boot)."
    exchange_url : string = ""

    @clarg_doc = "One-shot exchange accept-policy override: verified | any | off; unset = the app config's exchange_accept. A host may arm 'any' for one relaunch to adopt an unverified sidecar over finishing a tune. Setting it is an expressed choice to use the exchange - the first-contact consent question is skipped (announced at boot)."
    exchange_accept : string = ""
}

[EnvConfig(env_prefix = "DAS")]
struct public CoreEnv {
    @clarg_env = "HF_HOME"
    @clarg_path
    @clarg_default_doc = "~/.cache/huggingface"
    @clarg_doc = "Hugging Face hub root; fetch_models --convert resolves checkpoint blobs under <HF_HOME>/hub. Falls back to HOME's default cache location when unset."
    hf_home : string = ""

    @clarg_env = "HOME"
    @clarg_path
    @clarg_doc = "Ambient platform variable; read to derive the per-user ~/.dasllama directory (when USERPROFILE is unset) and the default Hugging Face cache when HF_HOME is unset."
    home : string = ""

    @clarg_env = "USERPROFILE"
    @clarg_path
    @clarg_doc = "Ambient platform variable (Windows twin of HOME); read only to derive the per-user ~/.dasllama directory."
    user_profile : string = ""

    @clarg_default_doc = "conservative default"
    @clarg_doc = "Total compute lanes for job queues (N-1 workers plus the caller). Overrides set_jobque_threads_cap; see skills/internal/environment_variables.md."
    jobque_threads : Option<int>

    @clarg_default_doc = "2 on darwin (engine [init]), else 0"
    @clarg_doc = "Worker affinity: 0 off, 1 ideal-processor hint, 2 hard mask - on darwin mode 2 is QoS classification (no pin API) and dasLLAMA defaults it on. Matters on big SMT boxes."
    jobque_affinity : Option<int>

    @clarg_default_doc = "profile-driven"
    @clarg_doc = "Team-dispatch rank gate. When set, it suppresses the box profile's own team_rank_gate knob."
    jobque_team_rank_gate : Option<int>

    @clarg_path
    @clarg_default_doc = "<app>.tune.json"
    @clarg_doc = "Kernel-tuning sidecar to read/write. Point it somewhere writable when the app dir is read-only."
    tune_manifest : string = ""

    @clarg_doc = "Kernel-tuning mode. The [tune] framework owns these; see skills/tune.md."
    tune_mode : string = ""

    @clarg_default_doc = "warning (daslang's to_log floor); tests/run.das sets info for itself and its children when unset"
    @clarg_doc = "The to_log floor daslang reads per call (trace, debug, info, warning, error or critical); see skills/internal/environment_variables.md."
    log_level : string = ""

    @clarg_default_doc = "DAS_LOG_LEVEL, then warning"
    @clarg_doc = "The daslib/logger floor, read once at init; it wins over DAS_LOG_LEVEL where both are set. See skills/internal/environment_variables.md."
    logger_level : string = ""

    @clarg_doc = "Kernel-tuning policy override. The [tune] framework owns these; see skills/tune.md."
    tune_policy : string = ""

    @clarg_doc = "Semicolon-separated kernel fallbacks for the generated tune probe."
    tune_compile_fallbacks : string = ""
}

// ===== the loaded config globals =====
// One load per context, at init. Read sites touch fields; nothing hot ever calls the OS.

let public g_env_engine = env_config(type<EngineEnv>)
let public g_env_vision = env_config(type<VisionEnv>)
let public g_env_gpu = env_config(type<GpuTierEnv>)
let public g_env_metal = env_config(type<MetalEnv>)
let public g_env_vulkan = env_config(type<VulkanEnv>)
let public g_env_mvk = env_config(type<MoltenVkEnv>)
let public g_env_accel = env_config(type<AccelerateEnv>)
let public g_env_harness = env_config(type<HarnessEnv>)
let public g_env_bench = env_config(type<BenchmarkEnv>)
let public g_env_prof = env_config(type<ProfilingEnv>)
let public g_env_test = env_config(type<TestEnv>)
let public g_env_exchange = env_config(type<ExchangeEnv>)
let public g_env_core = env_config(type<CoreEnv>)

let public PF_WINDOW_MIN = 128l   //! the prompt window's floor and its rounding unit: the chain's tiles step rows by 128
let public PF_ROWS_DEFAULT = 2_048l   //! the prompt window where neither a pin, the knob nor the driver's pick names one
let public PF_ROWS_WIDE = 4_096l      //! the window the driver tries first: one expert sweep for a 3k prompt

var private g_pf_rows_pin = 0l    //! the prompt window a program pinned in process (0 = the knob's)
var private g_pf_rows_auto = 0l   //! the window the driver's plan picked where no pin and no knob named one (0 = none yet)

//! the prompt window a program asks of the driver for the loads that follow, over DASLLAMA_GPU_PF_ROWS; 0 lifts the pin
def public set_pf_window_rows_pin(rows : int64) {
    g_pf_rows_pin = rows
}

//! the pin as it was written (0 = none): a fixture puts back what it found
def public pf_window_rows_pin : int64 => g_pf_rows_pin

//! the driver's own pick, under a pin and the knob: the widest window its plan fits with (0 = none picked)
def public set_pf_window_rows_auto(rows : int64) {
    g_pf_rows_auto = rows
}

//! true where neither a pin nor the knob names the window: the driver picks it at the plan
def public pf_window_rows_unasked : bool => g_pf_rows_pin <= 0l && g_env_gpu.gpu_pf_rows <= 0l

def public pf_window_rows : int64 {
    let asked = g_pf_rows_pin > 0l ? g_pf_rows_pin : (g_env_gpu.gpu_pf_rows > 0l ? g_env_gpu.gpu_pf_rows : (g_pf_rows_auto > 0l ? g_pf_rows_auto : PF_ROWS_DEFAULT))
    let units = asked / PF_WINDOW_MIN
    return (units < 1l ? 1l : units) * PF_WINDOW_MIN   // nolint:PERF015 — the env module sees no math
}

//! The per-user dasLLAMA directory under `home`: the standalone programs' models, config, tune
//! state and logs, so an install directory can stay read-only.
def public dasllama_home_dir_from(home : string) : string => path_join(home, ".dasllama")

//! `<home>/.dasllama`, home being USERPROFILE (Windows), else HOME.
def public dasllama_home_dir() : string =>
    dasllama_home_dir_from(!empty(g_env_core.user_profile) ? g_env_core.user_profile : g_env_core.home)

//! A dasLLAMA app's config `name`, looked for in the working directory, then in `dasllama_home`,
//! then in `anchor_dir` (beside the program or its script); "" when none exists. The one lookup
//! the server, its CLI and the tune exchange share.
def public dasllama_find_config(name, dasllama_home, anchor_dir : string) : string {
    for (dir in [".", dasllama_home, anchor_dir]) {
        let path = dir == "." ? name : path_join(dir, name)
        return path if (stat(path).is_valid)
    }
    return ""
}

//! The rig/probe/test models-dir resolution (DASLLAMA_MODELS_DIR, else the primary rig's dir),
//! trailing separator guaranteed — callers concat "{models_dir_resolved()}{file}". The server's
//! catalog resolves its own chain (model_catalog.das: env > models_dir key > ~/.dasllama/models).
def public models_dir_resolved() : string {
    let env = g_env_harness.models_dir
    if (empty(env)) {
        return "/Users/borisbatkin/Work/llama.cpp/models/"
    }
    return ends_with(env, "/") || ends_with(env, "\\") ? env : "{env}/"
}

// Dynamic-name reads (`env_is_set` / `env_value_of` — the ONE sanctioned escape hatch: the
// bench repro-line forwarder, lever env names from data, lab filter variables) and the doc
// renderers both come from daslib/env_registry, re-exported public above.

// Compile-time knobs are not runtime config — no struct field would tell the truth about them —
// so their doc rows are declared here and appended to their section.
def private core_extra_rows() : array<EnvDocRow> {
    return <- [EnvDocRow(name = "JOBQUE_PROFILING", kind = "flag", dflt = "off",
        doc = "Compile the jobque marker rail in (read at COMPILE time by daslib/build_const). Without it trace_tag/trace_marker erase from the program; decode_prof --trace refuses on a build without it.")]
}

//! Renders every declared knob as the `ENVIRONMENT.md` body, grouped by area. `modules/dasLLAMA/
//! ENVIRONMENT.md` is this output verbatim — `harness/gen_env_doc.das` writes it and
//! `tests/test_env_registry.das` fails when the checked-in file drifts from it.
def public env_markdown() : string {
    return build_string() $(var w) {
        w |> write("# dasLLAMA environment variables\n\n")
        w |> write("GENERATED from the `[EnvConfig]` declarations in `dasllama/dasllama_env.das` - do not edit\n")
        w |> write("by hand. Regenerate with `daslang modules/dasLLAMA/harness/gen_env_doc.das`; a knob read\n")
        w |> write("anywhere in the tree without a declaration fails `tests/test_env_registry.das`.\n\n")
        write_env_doc_semantics(w)
        write_env_section(w, "Engine",
            "Read by the inference engine itself, so these affect any program that loads a model.")
        write_env_info_rows(w, get_env_config_info(type<EngineEnv>))
        write_env_info_rows(w, get_env_config_info(type<GpuTierEnv>))
        write_env_section(w, "Vision",
            "The image preprocessing rail: dynamic-resolution token bounds and the debug dump.")
        write_env_info_rows(w, get_env_config_info(type<VisionEnv>))
        write_env_section(w, "Metal backend",
            "Apple GPU backend. Absent on non-Apple builds, where setting them does nothing.")
        write_env_info_rows(w, get_env_config_info(type<MetalEnv>))
        write_env_section(w, "Vulkan backend",
            "Vulkan GPU backend. Present only where the dasVulkan package is installed.")
        write_env_info_rows(w, get_env_config_info(type<VulkanEnv>))
        write_env_section(w, "MoltenVK (macOS)",
            "MoltenVK's own configuration - listed because the vulkan tier arms it on Apple.")
        write_env_info_rows(w, get_env_config_info(type<MoltenVkEnv>))
        write_env_section(w, "Accelerate / AMX backend",
            "Apple Accelerate / AMX float lane. `DASLLAMA_ACCEL` arms the whole group.")
        write_env_info_rows(w, get_env_config_info(type<AccelerateEnv>))
        write_env_section(w, "Harness - tuner and probes", "")
        write_env_info_rows(w, get_env_config_info(type<HarnessEnv>))
        write_env_section(w, "Benchmarks", "")
        write_env_info_rows(w, get_env_config_info(type<BenchmarkEnv>))
        write_env_section(w, "Profiling and baselines", "")
        write_env_info_rows(w, get_env_config_info(type<ProfilingEnv>))
        write_env_section(w, "Tests", "")
        write_env_info_rows(w, get_env_config_info(type<TestEnv>))
        write_env_section(w, "Examples and tutorials", "")
        write_env_info_rows(w, get_env_config_info(type<ExampleEnv>))
        write_env_section(w, "Sidecar exchange", "Overrides for the dasllama.io exchange client.")
        write_env_info_rows(w, get_env_config_info(type<ExchangeEnv>))
        write_env_section(w, "daslang core knobs dasLLAMA honours",
            "Owned by daslang, not by dasLLAMA - listed because dasLLAMA's behaviour depends on them. `DAS_TUNE_*` is covered in `skills/tune.md`.")
        write_env_info_rows(w, get_env_config_info(type<CoreEnv>))
        var extras <- core_extra_rows()
        for (e in extras) {
            write_env_row(w, e)
        }
        delete extras
    }
}

//! Every declared knob name, in documentation order — the coverage test and the bench repro-line
//! forwarder consume this.
def public registered_env_names() : array<string> {
    var out : array<string>
    out |> reserve(140)
    push_info_names(out, get_env_config_info(type<EngineEnv>))
    push_info_names(out, get_env_config_info(type<GpuTierEnv>))
    push_info_names(out, get_env_config_info(type<VisionEnv>))
    push_info_names(out, get_env_config_info(type<MetalEnv>))
    push_info_names(out, get_env_config_info(type<VulkanEnv>))
    push_info_names(out, get_env_config_info(type<MoltenVkEnv>))
    push_info_names(out, get_env_config_info(type<AccelerateEnv>))
    push_info_names(out, get_env_config_info(type<HarnessEnv>))
    push_info_names(out, get_env_config_info(type<BenchmarkEnv>))
    push_info_names(out, get_env_config_info(type<ProfilingEnv>))
    push_info_names(out, get_env_config_info(type<TestEnv>))
    push_info_names(out, get_env_config_info(type<ExampleEnv>))
    push_info_names(out, get_env_config_info(type<ExchangeEnv>))
    push_info_names(out, get_env_config_info(type<CoreEnv>))
    var extras <- core_extra_rows()
    for (e in extras) {
        out |> push(e.name)
    }
    delete extras
    return <- out
}

//! a tri-state pin over an environment knob: -1 the environment's `env`, 0 off, any other value on
def pin_over_env(pin : int; env : bool) : bool => pin < 0 ? env : pin != 0
