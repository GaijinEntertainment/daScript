options gen2
options _comment_hygiene = true

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

require daslib/clargs
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

    @clarg_doc = "Allow the CPU prefill path even when a GPU prefill override is registered."
    cpu_prefill : bool = false

    @clarg_doc = "Pin prefill to one registered override by name (e.g. metal, vulkan); anything else declines before upload."
    pin_prefill : string = ""

    @clarg_doc = "Pin the matmul backend by name, bypassing the measured auto-selection."
    pin_backend : string = ""

    @clarg_doc = "Pin the batched (prefill) matmul backend independently of the decode one."
    pin_batch_backend : string = ""

    @clarg_doc = "Arm the MoE expert-reuse counter (probes and servers; benches use set_expert_reuse instead)."
    expert_reuse : bool = false

    @clarg_doc = "Print engine diagnostics (tier selection, upload plan, arm/decline reasons)."
    noisy : bool = false

    @clarg_default_doc = "2 (the top notch)"
    @clarg_doc = "A/B rail for the dispatch caller's thread priority, -2..2; unset claims the top notch."
    caller_prio : int = 2

    @clarg_doc = "Regenerate the stored parity truth files instead of comparing against them."
    truth_refresh : bool = false

    @clarg_doc = "Bucket gguf -> image conversion time by kind over the weight walk; one clock pair per tensor."
    conv_prof : bool = false

    @clarg_doc = "Permit a big gguf load without -jit; the transforms run interpreted, so expect minutes per GB."
    allow_interp_load : bool = false

    @clarg_doc = "Advisory source-mapping readahead at gguf load (cold-conversion fix); =0 restores on-demand faulting."
    prefetch : bool = true
}

// Tri-state on purpose: the tier's effective defaults are computed from set_gpu_tier_want state
// at query time, so these keep the "was it set" bit (gpu_want_* in dasllama_gpu_tier.das).
[EnvConfig(env_prefix = "DASLLAMA")]
struct public GpuTierEnv {
    @clarg_default_doc = "off"
    @clarg_doc = "One switch for the measured-best GPU rail set; any DASLLAMA_GPU_* knob still overrides individually."
    gpu : Option<bool>

    @clarg_default_doc = "-1 (auto)"
    @clarg_doc = "How many MoE expert layers to hold resident on the GPU; -1 lets the upload walk place the split."
    gpu_moe_layers : Option<int64>

    @clarg_default_doc = "-1 (auto)"
    @clarg_doc = "How many MoE layers to stream rather than hold resident; -1 is auto."
    gpu_moe_stream : Option<int64>

    @clarg_default_doc = "probed"
    @clarg_doc = "Override the detected VRAM budget in MiB that sizes the resident expert stacks."
    gpu_vram_mb : Option<int64>

    @clarg_default_doc = "built-in floor"
    @clarg_doc = "Lower the context floor for arming the resident decode driver, for a short-context session on a small card."
    gpu_min_ctx : int64 = 0l

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

    @clarg_default_doc = "follows DASLLAMA_GPU"
    @clarg_doc = "Shared-expert rail on the GPU."
    gpu_shexp : Option<bool>

    @clarg_default_doc = "off"
    @clarg_doc = "Fused QKV rail on the GPU; stays off under DASLLAMA_GPU (measured a wash)."
    gpu_qkv : Option<bool>

    @clarg_default_doc = "on"
    @clarg_doc = "Keep the classifier plane on the GPU — the best ms-per-GB region; 0 is the CPU A/B lever."
    gpu_cls : Option<bool>

    @clarg_doc = "Device-side routed MoE combine; 0 falls back to the host combine."
    gpu_combine : bool = true

    @clarg_default_doc = "0"
    @clarg_doc = "Expert heat threshold: hold the N hottest experts resident regardless of layer placement."
    gpu_heat : Option<int64>

    @clarg_doc = "Report lifetime GPU queue submissions (real commands plus staging round-trips)."
    gpu_prof : bool = false
}

// ===== Metal backend =====

[EnvConfig(env_prefix = "DASLLAMA_METAL")]
struct public MetalEnv {
    @clarg_doc = "Produce logits on the GPU; 0 pulls the classifier back to the CPU. Blob-only models force it on."
    logits : bool = true

    @clarg_doc = "ggml-geometry QK/AV prefill attention (~10x the trio GEMMs); 0 pins the trio."
    attn : bool = true

    @clarg_doc = "Fused single-pass decode attention (assumes head_size 128); 0 is the A/B rail to the chunked pair."
    attn_d : bool = true

    @clarg_doc = "Row count below which attention uses the single-chunk kernel; clamped to 128."
    attn_single : int = 64

    @clarg_doc = "The mul_mm prefill GEMM; 0 falls back to the legacy per-op path."
    mulmm : bool = true

    @clarg_default_doc = "~4 layers/chunk"
    @clarg_doc = "Command-buffer split: each chunk commits as encoded so the scheduler overlaps chunk k with k-1."
    ncb : int = 0

    @clarg_doc = "Skip per-dispatch retain/release on the prefill command buffers."
    unretained : bool = false

    @clarg_doc = "Record each prefill chunk as a step graph and replay on a concurrent encoder; 0 is serial-encode rollback."
    pf_capture : bool = true

    @clarg_doc = "Graph scheduling mode for replay; 0 keeps capture order."
    sched : int = 1

    @clarg_doc = "Concurrent encoder for the single-stream decode step, with barriers only at detected hazards."
    decode_concurrent : bool = true

    @clarg_doc = "The R1 epilogue-fusion kernel (post_attn_rms + add_rms); 0 restores separate dispatches for A/B."
    fuse : bool = true

    @clarg_default_doc = "-1 (adaptive)"
    @clarg_doc = "MTP speculative decode chain: 0 off, 1 forced, -1 adaptive."
    spec : Option<int>

    @clarg_doc = "Rows per GEMV threadgroup, clamped 1..32."
    gemv_tg : int = 4

    @clarg_doc = "Single-pass B8 twin for K-quant small-batch mv at B=5..8; 0 is the A/B rail."
    kq_b8 : bool = true

    @clarg_doc = "Ceiling in MiB for the device-side KV mirror, clamped 64..4096."
    kv_mirror_mb : int = 4096

    @clarg_doc = "Barrier at every dispatch instead of at tracker-detected hazards (correctness bisect)."
    hazard_paranoid : bool = false

    @clarg_doc = "Treat every detected hazard as strict, widening barriers (correctness bisect)."
    hazard_strict : bool = false

    @clarg_doc = "Per-step pipeline trace: GPU envelope and true inter-step handoff idle, first steps plus outliers."
    pipe_debug : bool = false

    @clarg_doc = "Comma-separated dispatch names to skip in decode — a bring-up bisect that breaks correctness."
    decode_skip : string = ""

    @clarg_doc = "Prefill twin of DASLLAMA_METAL_DECODE_SKIP."
    prefill_skip : string = ""

    @clarg_env = "DASLLAMA_MTP_DEBUG"
    @clarg_doc = "MTP bring-up bisect: cold logs the failing gate, reject forces the reject path."
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

    @clarg_doc = "The mul_mm rail for batched decode; measured negative at M-pad-32, kept as the A/B rail."
    batch_mm : bool = false
}

// ===== Vulkan backend =====

[EnvConfig(env_prefix = "DASLLAMA")]
struct public VulkanEnv {
    @clarg_default_doc = "auto"
    @clarg_doc = "Cooperative-matrix mode; the flash-attention twin needs it even when the GEMM runs sdot4."
    coopmat : string = ""

    @clarg_default_doc = "32"
    @clarg_doc = "Small-batch tier: 32 = sdot4 (default, beats both coopmat tiles below the crossover), 64 = coopmat M, 128 = always-L."
    mm_small : string = ""

    @clarg_doc = "Small-d cutoff routing narrow roles (k/v) to the small tier; widening measured worse, so this is an instrument."
    mm_smalld : int64 = 64l

    @clarg_doc = "Fused decode tail (add+rms+requant, qk-norm+rope); 0 pins the split dispatches for a same-build A/B."
    vk_fuse : bool = true

    @clarg_doc = "Stream expert uploads on the dedicated transfer queue, overlapped via a timeline semaphore; 0 keeps the single-queue rail."
    vk_xferq : bool = true

    @clarg_doc = "Stream mirrors import the mapped .dlim (VK_EXT_external_memory_host) instead of pinned copies; =0 restores the copy path."
    vk_import : bool = true

    @clarg_doc = "Serve from P3-trimmed vulkan images (big CPU weight families dropped; folded into the flavor identity)."
    trim : bool = false

    @clarg_doc = "Tag allocations high-priority (VK_EXT_memory_priority) so the driver demotes desktop memory, not ours."
    vk_memprio : bool = true

    @clarg_doc = "Vulkan flash-attention kernel; 0 falls back to the chunked path."
    vk_fa : bool = true

    @clarg_doc = "Use a ReBAR device-local host-visible heap when one larger than 1GB is present."
    vk_rebar : bool = true

    @clarg_doc = "cm2 prefill tile pick: 0 = occupancy heuristic, 128 = force the m tile, 256 = force the l tile (A/B instrument)."
    cm2_tile : int64 = 0l

    @clarg_doc = "cm2 split-k: 0 = occupancy heuristic, 1 = off, N = force N k-chunks (A/B instrument; shrinks if N strands an empty tail)."
    cm2_splitk : int64 = 0l

    @clarg_path
    @clarg_doc = "Directory of <kernel>.spv files served instead of the emitted words at pipeline creation (offline spirv-opt / hand-patched A/B instrument)."
    vk_spv_override : string = ""

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
    @clarg_doc = "Directory holding the .gguf models the probes, benches and tests load."
    models_dir : string = ""

    @clarg_path
    @clarg_default_doc = "auto-resolved from the models dir"
    @clarg_doc = "Model used by the tuner's confirm gate (FULL path, not a bare filename). Unset: the gate auto-resolves from the models dir — the preferred confirm carrier, else the largest present q8 gguf; the fallback pins only when the box has no q8 model at all."
    confirm_model : string = ""

    @clarg_doc = "Override the batched-dispatch chunk count in the 1-core GEMM probe."
    batch_chunks : string = ""

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
    @clarg_doc = "Let a bench cell measure on fallback/absent tune winners. The gate exists because a stale sidecar silently downgrades every kernel and the whole board sandbags; only set for dev runs whose numbers are never published."
    allow_untuned : bool = false

    @clarg_doc = "Model name for the isolated GEMM bench."
    bench_model : string = "tinyllama"

    @clarg_doc = "Token count for the isolated GEMM bench."
    bench_ntok : Option<int64>

    @clarg_doc = "Skip the row-major arm of the isolated GEMM bench."
    bench_skip_rowmajor : bool = false

    @clarg_doc = "Per-run detail from the llama.cpp comparison bench."
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
    @clarg_doc = "Path to a stock-build llama-bench for the reference column."
    llama_bench_stock : string = ""

    @clarg_env = "WHISPER_CPP"
    @clarg_path
    @clarg_doc = "whisper.cpp checkout used for ASR reference baselines."
    whisper_cpp : string = "../whisper.cpp"

    @clarg_env = "WHISPER_CPP_MODELS"
    @clarg_path
    @clarg_doc = "Directory of whisper.cpp models for ASR reference baselines."
    whisper_cpp_models : string = ""

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

[EnvConfig(env_prefix = "DAS")]
struct public CoreEnv {
    @clarg_env = "HF_HOME"
    @clarg_path
    @clarg_default_doc = "~/.cache/huggingface"
    @clarg_doc = "Hugging Face hub root; fetch_models --convert resolves checkpoint blobs under <HF_HOME>/hub. Falls back to HOME's default cache location when unset."
    hf_home : string = ""

    @clarg_env = "HOME"
    @clarg_path
    @clarg_doc = "Ambient platform variable; read only to derive the default Hugging Face cache when HF_HOME is unset."
    home : string = ""

    @clarg_default_doc = "conservative default"
    @clarg_doc = "Total compute lanes for job queues (N-1 workers plus the caller). Overrides set_jobque_threads_cap; see skills/environment_variables.md."
    jobque_threads : Option<int>

    @clarg_default_doc = "2 on darwin (engine [init]), else 0"
    @clarg_doc = "Worker affinity: 0 off, 1 ideal-processor hint, 2 hard mask — on darwin mode 2 is QoS classification (no pin API) and dasLLAMA defaults it on. Matters on big SMT boxes."
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

    @clarg_doc = "Kernel-tuning policy override. The [tune] framework owns these; see skills/tune.md."
    tune_policy : string = ""

    @clarg_doc = "Semicolon-separated kernel fallbacks for the generated tune probe."
    tune_compile_fallbacks : string = ""
}

// ===== the loaded config globals =====
// One load per context, at init. Read sites touch fields; nothing hot ever calls the OS.

let public g_env_engine = env_config(type<EngineEnv>)
let public g_env_gpu = env_config(type<GpuTierEnv>)
let public g_env_metal = env_config(type<MetalEnv>)
let public g_env_vulkan = env_config(type<VulkanEnv>)
let public g_env_mvk = env_config(type<MoltenVkEnv>)
let public g_env_accel = env_config(type<AccelerateEnv>)
let public g_env_harness = env_config(type<HarnessEnv>)
let public g_env_bench = env_config(type<BenchmarkEnv>)
let public g_env_prof = env_config(type<ProfilingEnv>)
let public g_env_test = env_config(type<TestEnv>)
let public g_env_core = env_config(type<CoreEnv>)

//! The ONE models-dir resolution (DASLLAMA_MODELS_DIR, else the primary rig's dir) with the
//! trailing separator guaranteed — callers concat "{models_dir_resolved()}{file}", and a
//! slashless override would silently skip every catalog file.
def public models_dir_resolved() : string {
    let env = g_env_harness.models_dir
    if (empty(env)) {
        return "/Users/borisbatkin/Work/llama.cpp/models/"
    }
    return ends_with(env, "/") || ends_with(env, "\\") ? env : "{env}/"
}

// ===== dynamic-name reads (the ONE sanctioned escape hatch) =====
// For names known only at runtime: the bench repro-line forwarder, lever env names from data,
// lab filter variables. Same set-but-empty-is-unset rule as the loaders. Not for hot paths.

def public env_is_set(name : string) : bool {
    return has_env_variable(name) && !empty(get_env_variable(name))
}

def public env_value_of(name : string) : string {
    return get_env_variable(name)
}

// ===== the generated documentation (ENVIRONMENT.md) =====

// Compile-time knobs are not runtime config — no struct field would tell the truth about them —
// so their doc rows are declared here and appended to their section.
struct private ExtraDocRow {
    name : string
    kind : string
    dflt : string
    doc : string
}

def private core_extra_rows() : array<ExtraDocRow> {
    return <- [ExtraDocRow(name = "JOBQUE_PROFILING", kind = "flag", dflt = "off",
        doc = "Compile the jobque marker rail in (read at COMPILE time by daslib/build_const). Without it trace_tag/trace_marker erase from the program; decode_prof --trace refuses on a build without it.")]
}

def private write_row(var w : StringBuilderWriter; name, kind, dflt, doc : string) {
    w |> write("| `{name}` | {kind} | {empty(dflt) ? "unset" : dflt} | {doc} |\n")
}

def private write_info_rows(var w : StringBuilderWriter; info : CommandInfo) {
    for (a in info.args) {
        var kind = "number"
        if (a.value_type == Type.tBool) {
            kind = "flag"
        } elif (a.value_type == Type.tString) {
            kind = a.is_path ? "path" : "text"
        }
        // flags document as on/off, matching how the knobs are described everywhere else
        var dflt = a.default_doc
        if (a.value_type == Type.tBool) {
            dflt = dflt == "true" ? "on" : (dflt == "false" ? "off" : dflt)
        }
        write_row(w, a.env_name, kind, dflt, a.doc_string)
    }
}

def private write_section(var w : StringBuilderWriter; title, note : string) {
    w |> write("\n## {title}\n\n")
    if (!empty(note)) {
        w |> write("{note}\n\n")
    }
    w |> write("| Variable | Type | Default | Effect |\n|---|---|---|---|\n")
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
        w |> write("Types: **flag** is unset-means-default, `0`/`false`/`off`/`no` (any case) is false and\n")
        w |> write("anything else true; **number** falls back to the default when unset or unparseable, with\n")
        w |> write("a logged warning on garbage; **text** and **path** are taken verbatim. A SET-BUT-EMPTY\n")
        w |> write("variable counts as unset everywhere. Every knob loads ONCE, at context init, into the\n")
        w |> write("`g_env_*` globals - hot code reads struct fields, and `set_env_variable` after startup\n")
        w |> write("is invisible (arm a child process's environment instead).\n")
        write_section(w, "Engine",
            "Read by the inference engine itself, so these affect any program that loads a model.")
        write_info_rows(w, get_env_config_info(type<EngineEnv>))
        write_info_rows(w, get_env_config_info(type<GpuTierEnv>))
        write_section(w, "Metal backend",
            "Apple GPU backend. Absent on non-Apple builds, where setting them does nothing.")
        write_info_rows(w, get_env_config_info(type<MetalEnv>))
        write_section(w, "Vulkan backend",
            "Vulkan GPU backend. Present only where the dasVulkan package is installed.")
        write_info_rows(w, get_env_config_info(type<VulkanEnv>))
        write_section(w, "MoltenVK (macOS)",
            "MoltenVK's own configuration - listed because the vulkan tier arms it on Apple.")
        write_info_rows(w, get_env_config_info(type<MoltenVkEnv>))
        write_section(w, "Accelerate / AMX backend",
            "Apple Accelerate / AMX float lane. `DASLLAMA_ACCEL` arms the whole group.")
        write_info_rows(w, get_env_config_info(type<AccelerateEnv>))
        write_section(w, "Harness - tuner and probes", "")
        write_info_rows(w, get_env_config_info(type<HarnessEnv>))
        write_section(w, "Benchmarks", "")
        write_info_rows(w, get_env_config_info(type<BenchmarkEnv>))
        write_section(w, "Profiling and baselines", "")
        write_info_rows(w, get_env_config_info(type<ProfilingEnv>))
        write_section(w, "Tests", "")
        write_info_rows(w, get_env_config_info(type<TestEnv>))
        write_section(w, "Examples and tutorials", "")
        write_info_rows(w, get_env_config_info(type<ExampleEnv>))
        write_section(w, "daslang core knobs dasLLAMA honours",
            "Owned by daslang, not by dasLLAMA - listed because dasLLAMA's behaviour depends on them. `DAS_TUNE_*` is covered in `skills/tune.md`.")
        write_info_rows(w, get_env_config_info(type<CoreEnv>))
        var extras <- core_extra_rows()
        for (e in extras) {
            write_row(w, e.name, e.kind, e.dflt, e.doc)
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
    push_info_names(out, get_env_config_info(type<MetalEnv>))
    push_info_names(out, get_env_config_info(type<VulkanEnv>))
    push_info_names(out, get_env_config_info(type<MoltenVkEnv>))
    push_info_names(out, get_env_config_info(type<AccelerateEnv>))
    push_info_names(out, get_env_config_info(type<HarnessEnv>))
    push_info_names(out, get_env_config_info(type<BenchmarkEnv>))
    push_info_names(out, get_env_config_info(type<ProfilingEnv>))
    push_info_names(out, get_env_config_info(type<TestEnv>))
    push_info_names(out, get_env_config_info(type<ExampleEnv>))
    push_info_names(out, get_env_config_info(type<CoreEnv>))
    var extras <- core_extra_rows()
    for (e in extras) {
        out |> push(e.name)
    }
    delete extras
    return <- out
}

def private push_info_names(var out : array<string>; info : CommandInfo) {
    out |> reserve(length(out) + length(info.args))
    for (a in info.args) {
        out |> push(a.env_name)
    }
}
