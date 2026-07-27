options gen2
options _comment_hygiene = true

//! dasLLAMA's environment-variable registry and the typed readers every knob goes through.
//!
//! Two jobs. First, [[env_flag]] / [[env_int]] / [[env_int64]] / [[env_str]] replace the five
//! ad-hoc `env_int`-shaped helpers the tree grew independently, so "unset", "garbage" and
//! "explicitly off" mean the same thing everywhere. Second, [[env_registry]] is the single list of
//! every variable dasLLAMA reads — `ENVIRONMENT.md` is GENERATED from it (`--dump-env`), and
//! `tests/test_env_registry.das` fails if a `DASLLAMA_*` literal appears in the tree without a
//! matching entry. Add the knob here in the same commit that adds the read, or the test breaks.
//!
//! The readers are deliberately NOT [cold_path]: reading the environment per step is exactly what
//! PERF027 exists to catch, so resolve knobs in an `[init]` (or behind a `[cold_path]` loader) and
//! cache the value in a global. See `skills/perf_lint.md`.

module dasllama_env shared public

require daslib/fio
require daslib/strings_convert
require strings

//! How a knob's value is interpreted. `flag` is the boolean rule in [[env_flag]]; `number` parses
//! as an integer and falls back to the default on garbage; `text` and `path` are taken verbatim.
enum public EnvKind {
    flag
    number
    text
    path
}

//! Which part of dasLLAMA reads the knob — also the section it lands in under `ENVIRONMENT.md`.
enum public EnvArea {
    engine        //!< the inference engine proper (CPU path, model load, session)
    metal         //!< Apple GPU backend (dasMetal)
    vulkan        //!< Vulkan GPU backend
    accelerate    //!< Apple Accelerate / AMX float backend
    harness       //!< tuner and probes under modules/dasLLAMA/harness
    benchmark     //!< modules/dasLLAMA/benchmarks
    profiling     //!< modules/dasLLAMA/performance
    test          //!< modules/dasLLAMA/tests
    example       //!< examples/ and tutorials/
    core          //!< a daslang-core knob dasLLAMA honours but does not own
}

//! One registry entry. `dflt` is the documented default rendered as text ("" when the knob is
//! simply absent by default); `doc` is one line describing what setting it does.
struct public EnvKnob {
    name : string
    kind : EnvKind
    area : EnvArea
    dflt : string
    doc : string
}

// ===== typed readers =====

//! Reads a boolean knob. Unset yields `dflt`. Set to "0", "false", "off", "no" or empty is false;
//! any other value is true. This folds the two spellings the tree grew — `== "1"` opt-in and
//! `!= "0"` opt-out — onto one rule, so `FOO=true` and `FOO=yes` now work everywhere.
def public env_flag(name : string; dflt : bool) : bool {
    return dflt if (!has_env_variable(name))
    let v = to_lower(get_env_variable(name))
    return !(empty(v) || v == "0" || v == "false" || v == "off" || v == "no")
}

//! Reads an integer knob. Unset OR unparseable yields `dflt` — note that plain `to_int` returns 0
//! on garbage, which silently reads as "explicitly zero" and has bitten this tree before.
def public env_int(name : string; dflt : int) : int {
    return dflt if (!has_env_variable(name))
    return try_to_int(get_env_variable(name)) ?? dflt
}

//! 64-bit twin of [[env_int]], same unset/garbage rule.
def public env_int64(name : string; dflt : int64) : int64 {
    return dflt if (!has_env_variable(name))
    return try_to_int64(get_env_variable(name)) ?? dflt
}

//! Reads a string knob verbatim. Unset OR set-but-empty yields `dflt`.
def public env_str(name : string; dflt : string) : string {
    return dflt if (!has_env_variable(name))
    let v = get_env_variable(name)
    return empty(v) ? dflt : v
}

// ===== registry =====

def private k(var out : array<EnvKnob>; name : string; kind : EnvKind; area : EnvArea;
              dflt, doc : string) {
    out |> emplace(EnvKnob(name = name, kind = kind, area = area, dflt = dflt, doc = doc))
}

def private reg_engine(var o : array<EnvKnob>) {
    k(o, "DASLLAMA_IMAGE", EnvKind.flag, EnvArea.engine, "on",
      "Use the prepared-image cache beside a .gguf for millisecond reloads; 0 forces a full parse.")
    k(o, "DASLLAMA_CPU_PREFILL", EnvKind.flag, EnvArea.engine, "off",
      "Allow the CPU prefill path even when a GPU prefill override is registered.")
    k(o, "DASLLAMA_PIN_PREFILL", EnvKind.text, EnvArea.engine, "",
      "Pin prefill to one registered override by name (e.g. metal, vulkan); anything else declines before upload.")
    k(o, "DASLLAMA_PIN_BACKEND", EnvKind.text, EnvArea.engine, "",
      "Pin the matmul backend by name, bypassing the measured auto-selection.")
    k(o, "DASLLAMA_PIN_BATCH_BACKEND", EnvKind.text, EnvArea.engine, "",
      "Pin the batched (prefill) matmul backend independently of the decode one.")
    k(o, "DASLLAMA_EXPERT_REUSE", EnvKind.flag, EnvArea.engine, "off",
      "Arm the MoE expert-reuse counter (probes and servers; benches use set_expert_reuse instead).")
    k(o, "DASLLAMA_NOISY", EnvKind.flag, EnvArea.engine, "off",
      "Print engine diagnostics (tier selection, upload plan, arm/decline reasons).")
    k(o, "DASLLAMA_CALLER_PRIO", EnvKind.number, EnvArea.engine, "",
      "A/B rail for the dispatch caller's thread priority, -2..2; unset claims the top notch.")
    k(o, "DASLLAMA_TRUTH_REFRESH", EnvKind.flag, EnvArea.engine, "off",
      "Regenerate the stored parity truth files instead of comparing against them.")
}

def private reg_gpu_tier(var o : array<EnvKnob>) {
    k(o, "DASLLAMA_GPU", EnvKind.flag, EnvArea.engine, "off",
      "One switch for the measured-best GPU rail set; any DASLLAMA_GPU_* knob still overrides individually.")
    k(o, "DASLLAMA_GPU_MOE_LAYERS", EnvKind.number, EnvArea.engine, "-1 (auto)",
      "How many MoE expert layers to hold resident on the GPU; -1 lets the upload walk place the split.")
    k(o, "DASLLAMA_GPU_MOE_STREAM", EnvKind.number, EnvArea.engine, "-1 (auto)",
      "How many MoE layers to stream rather than hold resident; -1 is auto.")
    k(o, "DASLLAMA_GPU_VRAM_MB", EnvKind.number, EnvArea.engine, "probed",
      "Override the detected VRAM budget in MiB that sizes the resident expert stacks.")
    k(o, "DASLLAMA_GPU_MIN_CTX", EnvKind.number, EnvArea.engine, "built-in floor",
      "Lower the context floor for arming the resident decode driver, for a short-context session on a small card.")
    k(o, "DASLLAMA_GPU_DN", EnvKind.flag, EnvArea.engine, "follows DASLLAMA_GPU",
      "DeltaNet rail on the GPU.")
    k(o, "DASLLAMA_GPU_DND", EnvKind.flag, EnvArea.engine, "follows DASLLAMA_GPU",
      "DeltaNet decode rail on the GPU.")
    k(o, "DASLLAMA_GPU_ATTN", EnvKind.flag, EnvArea.engine, "follows DASLLAMA_GPU",
      "Attention rail on the GPU.")
    k(o, "DASLLAMA_GPU_DENSE", EnvKind.flag, EnvArea.engine, "follows DASLLAMA_GPU",
      "Dense-weight rail on the GPU.")
    k(o, "DASLLAMA_GPU_DENSE_ATTN", EnvKind.flag, EnvArea.engine, "on",
      "The dense rail's attention half (per-layer q/o pairs); opt-OUT, measured a loss on gemma-4.")
    k(o, "DASLLAMA_GPU_DENSE_SHEXP", EnvKind.flag, EnvArea.engine, "off",
      "The dense rail's gemma-4 shared-expert w1/w3/w2 triple; opt-IN, measured a wash-to-loss.")
    k(o, "DASLLAMA_GPU_SHEXP", EnvKind.flag, EnvArea.engine, "follows DASLLAMA_GPU",
      "Shared-expert rail on the GPU.")
    k(o, "DASLLAMA_GPU_QKV", EnvKind.flag, EnvArea.engine, "off",
      "Fused QKV rail on the GPU; stays off under DASLLAMA_GPU (measured a wash).")
    k(o, "DASLLAMA_GPU_CLS", EnvKind.flag, EnvArea.engine, "on",
      "Keep the classifier plane on the GPU — the best ms-per-GB region; 0 is the CPU A/B lever.")
    k(o, "DASLLAMA_GPU_COMBINE", EnvKind.flag, EnvArea.engine, "on",
      "Device-side routed MoE combine; 0 falls back to the host combine.")
    k(o, "DASLLAMA_GPU_HEAT", EnvKind.number, EnvArea.engine, "0",
      "Expert heat threshold: hold the N hottest experts resident regardless of layer placement.")
    k(o, "DASLLAMA_GPU_PROF", EnvKind.flag, EnvArea.engine, "off",
      "Report lifetime GPU queue submissions (real commands plus staging round-trips).")
    k(o, "DASLLAMA_GPU_NAME", EnvKind.text, EnvArea.profiling, "probed",
      "Override the GPU name recorded in a box profile.")
}

def private reg_metal(var o : array<EnvKnob>) {
    k(o, "DASLLAMA_METAL_LOGITS", EnvKind.flag, EnvArea.metal, "on",
      "Produce logits on the GPU; 0 pulls the classifier back to the CPU. Blob-only models force it on.")
    k(o, "DASLLAMA_METAL_ATTN", EnvKind.flag, EnvArea.metal, "on",
      "ggml-geometry QK/AV prefill attention (~10x the trio GEMMs); 0 pins the trio.")
    k(o, "DASLLAMA_METAL_ATTN_D", EnvKind.flag, EnvArea.metal, "on",
      "Fused single-pass decode attention (assumes head_size 128); 0 is the A/B rail to the chunked pair.")
    k(o, "DASLLAMA_METAL_ATTN_SINGLE", EnvKind.number, EnvArea.metal, "64",
      "Row count below which attention uses the single-chunk kernel; clamped to 128.")
    k(o, "DASLLAMA_METAL_MULMM", EnvKind.flag, EnvArea.metal, "on",
      "The mul_mm prefill GEMM; 0 falls back to the legacy per-op path.")
    k(o, "DASLLAMA_METAL_NCB", EnvKind.number, EnvArea.metal, "~4 layers/chunk",
      "Command-buffer split: each chunk commits as encoded so the scheduler overlaps chunk k with k-1.")
    k(o, "DASLLAMA_METAL_UNRETAINED", EnvKind.flag, EnvArea.metal, "off",
      "Skip per-dispatch retain/release on the prefill command buffers.")
    k(o, "DASLLAMA_METAL_PF_CAPTURE", EnvKind.flag, EnvArea.metal, "on",
      "Record each prefill chunk as a step graph and replay on a concurrent encoder; 0 is serial-encode rollback.")
    k(o, "DASLLAMA_METAL_SCHED", EnvKind.number, EnvArea.metal, "1",
      "Graph scheduling mode for replay; 0 keeps capture order.")
    k(o, "DASLLAMA_METAL_DECODE_CONCURRENT", EnvKind.flag, EnvArea.metal, "on",
      "Concurrent encoder for the single-stream decode step, with barriers only at detected hazards.")
    k(o, "DASLLAMA_METAL_FUSE", EnvKind.flag, EnvArea.metal, "on",
      "The R1 epilogue-fusion kernel (post_attn_rms + add_rms); 0 restores separate dispatches for A/B.")
    k(o, "DASLLAMA_METAL_SPEC", EnvKind.number, EnvArea.metal, "-1 (adaptive)",
      "MTP speculative decode chain: 0 off, 1 forced, -1 adaptive.")
    k(o, "DASLLAMA_METAL_GEMV_TG", EnvKind.number, EnvArea.metal, "4",
      "Rows per GEMV threadgroup, clamped 1..32.")
    k(o, "DASLLAMA_METAL_KQ_B8", EnvKind.flag, EnvArea.metal, "on",
      "Single-pass B8 twin for K-quant small-batch mv at B=5..8; 0 is the A/B rail.")
    k(o, "DASLLAMA_METAL_KV_MIRROR_MB", EnvKind.number, EnvArea.metal, "4096",
      "Ceiling in MiB for the device-side KV mirror, clamped 64..4096.")
    k(o, "DASLLAMA_METAL_HAZARD_PARANOID", EnvKind.flag, EnvArea.metal, "off",
      "Barrier at every dispatch instead of at tracker-detected hazards (correctness bisect).")
    k(o, "DASLLAMA_METAL_HAZARD_STRICT", EnvKind.flag, EnvArea.metal, "off",
      "Treat every detected hazard as strict, widening barriers (correctness bisect).")
    k(o, "DASLLAMA_METAL_PIPE_DEBUG", EnvKind.flag, EnvArea.metal, "off",
      "Per-step pipeline trace: GPU envelope and true inter-step handoff idle, first steps plus outliers.")
    k(o, "DASLLAMA_METAL_DECODE_SKIP", EnvKind.text, EnvArea.metal, "",
      "Comma-separated dispatch names to skip in decode — a bring-up bisect that breaks correctness.")
    k(o, "DASLLAMA_METAL_PREFILL_SKIP", EnvKind.text, EnvArea.metal, "",
      "Prefill twin of DASLLAMA_METAL_DECODE_SKIP.")
    k(o, "DASLLAMA_MTP_DEBUG", EnvKind.text, EnvArea.metal, "",
      "MTP bring-up bisect: cold logs the failing gate, reject forces the reject path.")
    k(o, "DASLLAMA_METAL_BATCH_GEMM_MIN", EnvKind.number, EnvArea.metal, "5",
      "Batch size at which batched decode switches from GEMV to GEMM; floor 2.")
    k(o, "DASLLAMA_METAL_BATCH_SK", EnvKind.flag, EnvArea.metal, "on",
      "Split-K in the batched decode GEMM.")
    k(o, "DASLLAMA_METAL_BATCH_NCB", EnvKind.number, EnvArea.metal, "4",
      "Command buffers per batched decode step, clamped 1..16.")
    k(o, "DASLLAMA_METAL_BATCH_CONCURRENT", EnvKind.flag, EnvArea.metal, "on",
      "Concurrent encoder for batched decode.")
    k(o, "DASLLAMA_METAL_BATCH_FUSE", EnvKind.flag, EnvArea.metal, "on",
      "Epilogue fusion in batched decode.")
    k(o, "DASLLAMA_METAL_BATCH_MV", EnvKind.flag, EnvArea.metal, "on",
      "Fixed-B GEMV forms for small batched decode.")
    k(o, "DASLLAMA_METAL_BATCH_MV8", EnvKind.flag, EnvArea.metal, "on",
      "The column-tiling MV8 probe for batched decode.")
    k(o, "DASLLAMA_METAL_BATCH_PIPE", EnvKind.flag, EnvArea.metal, "off",
      "Pipelined batched decode submission.")
    k(o, "DASLLAMA_METAL_BATCH_MM", EnvKind.flag, EnvArea.metal, "off",
      "The mul_mm rail for batched decode; measured negative at M-pad-32, kept as the A/B rail.")
}

def private reg_vulkan(var o : array<EnvKnob>) {
    k(o, "DASLLAMA_COOPMAT", EnvKind.number, EnvArea.vulkan, "auto",
      "Cooperative-matrix mode; the flash-attention twin needs it even when the GEMM runs sdot4.")
    k(o, "DASLLAMA_MM_SMALL", EnvKind.number, EnvArea.vulkan, "32",
      "Small-batch tier: 32 = sdot4 (default, beats both coopmat tiles below the crossover), 64 = coopmat M, 128 = always-L.")
    k(o, "DASLLAMA_CONV_PROF", EnvKind.flag, EnvArea.engine, "off",
      "Bucket gguf -> image conversion time by kind over the weight walk; one clock pair per tensor.")
    k(o, "DASLLAMA_ALLOW_INTERP_LOAD", EnvKind.flag, EnvArea.engine, "off",
      "Permit a big gguf load without -jit; the transforms run interpreted, so expect minutes per GB.")
    k(o, "PROBE_PATH", EnvKind.path, EnvArea.benchmark, "D:/_wcliff.bin",
      "Scratch file the write-cliff probe writes; put it on the drive under test.")
    k(o, "PROBE_GB", EnvKind.number, EnvArea.benchmark, "50",
      "Gigabytes the write-cliff probe writes before reporting.")
    k(o, "DASLLAMA_MM_SMALLD", EnvKind.number, EnvArea.vulkan, "64",
      "Small-d cutoff routing narrow roles (k/v) to the small tier; widening measured worse, so this is an instrument.")
    k(o, "DASLLAMA_VK_FUSE", EnvKind.flag, EnvArea.vulkan, "on",
      "Fused decode tail (add+rms+requant, qk-norm+rope); 0 pins the split dispatches for a same-build A/B.")
    k(o, "DASLLAMA_VK_XFERQ", EnvKind.flag, EnvArea.vulkan, "on",
      "Stream expert uploads on the dedicated transfer queue, overlapped via a timeline semaphore; 0 keeps the single-queue rail.")
    k(o, "DASLLAMA_VK_MEMPRIO", EnvKind.flag, EnvArea.vulkan, "on",
      "Tag allocations high-priority (VK_EXT_memory_priority) so the driver demotes desktop memory, not ours.")
    k(o, "DASLLAMA_VK_FA", EnvKind.flag, EnvArea.vulkan, "on",
      "Vulkan flash-attention kernel; 0 falls back to the chunked path.")
    k(o, "DASLLAMA_VK_REBAR", EnvKind.flag, EnvArea.vulkan, "on",
      "Use a ReBAR device-local host-visible heap when one larger than 1GB is present.")
    k(o, "DASLLAMA_VK_HAZARD_PARANOID", EnvKind.flag, EnvArea.vulkan, "off",
      "Barrier at every dispatch (correctness bisect).")
    k(o, "DASLLAMA_VK_HAZARD_TRACE", EnvKind.flag, EnvArea.vulkan, "off",
      "Log every detected hazard and the barrier it produced.")
}

def private reg_accelerate(var o : array<EnvKnob>) {
    k(o, "DASLLAMA_ACCEL", EnvKind.flag, EnvArea.accelerate, "off",
      "Arm the Apple Accelerate / AMX float-batch override.")
    k(o, "DASLLAMA_ACCEL_F16", EnvKind.flag, EnvArea.accelerate, "off",
      "Use the BNNS f16 lane inside the Accelerate backend.")
    k(o, "DASLLAMA_ACCEL_F16_STRIPS", EnvKind.number, EnvArea.accelerate, "0",
      "Strip count for the f16 lane; 0 lets the backend choose.")
    k(o, "DASLLAMA_ACCEL_MIN_MMAC", EnvKind.number, EnvArea.accelerate, "backend default",
      "Minimum MMAC count below which Accelerate declines and the daslang kernel runs.")
    k(o, "DASLLAMA_ACCEL_MIN_NTOK", EnvKind.number, EnvArea.accelerate, "32",
      "Minimum token count for the Accelerate float-batch override, floor 1.")
}

def private reg_harness(var o : array<EnvKnob>) {
    k(o, "DASLLAMA_MODELS_DIR", EnvKind.path, EnvArea.harness, "",
      "Directory holding the .gguf models the probes, benches and tests load.")
    k(o, "DASLLAMA_CONFIRM_MODEL", EnvKind.path, EnvArea.harness, "",
      "Model used by the tuner's confirm gate. Must be a FULL path, not a bare filename.")
    k(o, "DASLLAMA_BATCH_CHUNKS", EnvKind.number, EnvArea.harness, "",
      "Override the batched-dispatch chunk count in the 1-core GEMM probe.")
    k(o, "DASLLAMA_BATCH_GRID_2D", EnvKind.flag, EnvArea.harness, "off",
      "Use the 2D batch grid in the parity probe.")
    k(o, "DASLLAMA_FOCUS_BACKEND", EnvKind.text, EnvArea.harness, "",
      "Restrict the 1-core GEMM probe to one backend.")
    k(o, "DASLLAMA_FOCUS_SHAPE", EnvKind.text, EnvArea.harness, "",
      "Restrict the 1-core GEMM probe to one shape.")
    k(o, "DASLLAMA_FOCUS_NTOK", EnvKind.number, EnvArea.harness, "",
      "Restrict the 1-core GEMM probe to one token count.")
    k(o, "DASLLAMA_FOCUS_FORKED", EnvKind.flag, EnvArea.harness, "off",
      "Run the 1-core GEMM probe through the fork pool rather than inline.")
    k(o, "GEN_TUNE_COMPILE_ONLY", EnvKind.flag, EnvArea.harness, "off",
      "Compile the generated tune probe without running it.")
    k(o, "SMMLA_PROBE_COMPILE_ONLY", EnvKind.flag, EnvArea.harness, "off",
      "Compile the smmla probe without running it.")
    k(o, "METAL", EnvKind.flag, EnvArea.harness, "off",
      "Run the Accelerate contention probe against the Metal backend.")
    k(o, "DAS_DETAIL", EnvKind.flag, EnvArea.harness, "off",
      "Per-job dump in the dispatch probe.")
    k(o, "DAS_SPIN_NS", EnvKind.number, EnvArea.harness, "",
      "Synthetic per-job spin in nanoseconds for the dispatch probe.")
    k(o, "DAS_BATCH_DISPATCH", EnvKind.flag, EnvArea.harness, "off",
      "Exercise batched dispatch in the dispatch probe.")
    k(o, "DAS_TWO_WAVE", EnvKind.flag, EnvArea.harness, "off",
      "Exercise the two-wave dispatch shape in the dispatch probe.")
    k(o, "DAS_FORK_SKIP_HEAP_RESET", EnvKind.flag, EnvArea.harness, "off",
      "Skip restartHeaps on pooled fork-context reuse (A/B for the fork-pool heap cost).")
    k(o, "DAS_WORKER_SPIN_US", EnvKind.number, EnvArea.harness, "",
      "Worker spin-before-park window in microseconds, applied via set_jobque_spin_us.")
    k(o, "DAS_JOBQUE_JOIN_SPIN", EnvKind.number, EnvArea.harness, "",
      "Join-side spin window for the 1-core GEMM probe.")
}

def private reg_benchmark(var o : array<EnvKnob>) {
    k(o, "DASLLAMA_BENCH_MODEL", EnvKind.text, EnvArea.benchmark, "tinyllama",
      "Model name for the isolated GEMM bench.")
    k(o, "DASLLAMA_BENCH_NTOK", EnvKind.number, EnvArea.benchmark, "",
      "Token count for the isolated GEMM bench.")
    k(o, "DASLLAMA_BENCH_SKIP_ROWMAJOR", EnvKind.flag, EnvArea.benchmark, "off",
      "Skip the row-major arm of the isolated GEMM bench.")
    k(o, "DASLLAMA_BENCH_VERBOSE", EnvKind.flag, EnvArea.benchmark, "off",
      "Per-run detail from the llama.cpp comparison bench.")
    k(o, "DASLLAMA_MIN_CHUNK_ROWS", EnvKind.number, EnvArea.benchmark, "",
      "Override the minimum rows per dispatch chunk in the GEMM bench.")
    k(o, "DASLLAMA_FUSED_DECODE", EnvKind.flag, EnvArea.benchmark, "off",
      "Exercise the fused decode arm in the emission bench.")
    k(o, "DASMETAL_LAB_ROUNDS", EnvKind.number, EnvArea.benchmark, "3",
      "Timing rounds per cell in the Metal kernel labs.")
    k(o, "DASMETAL_LAB_PASSES", EnvKind.number, EnvArea.benchmark, "4",
      "Passes per round in the Metal MoE lab.")
    k(o, "DASMETAL_LAB_ATTN_NSGS", EnvKind.number, EnvArea.benchmark, "16",
      "Simdgroups per threadgroup in the Metal attention lab.")
    k(o, "DASMETAL_LAB_DUMP_MSL", EnvKind.flag, EnvArea.benchmark, "off",
      "Dump generated MSL from the Metal labs instead of running it.")
}

def private reg_profiling(var o : array<EnvKnob>) {
    k(o, "DASLANG_BIN", EnvKind.path, EnvArea.profiling, "bin/daslang",
      "daslang binary the profiling harness shells out to.")
    k(o, "DASLLAMA_BOX", EnvKind.text, EnvArea.profiling, "probed",
      "Override the box identifier recorded in a profile.")
    k(o, "DASLLAMA_CPU", EnvKind.text, EnvArea.profiling, "probed",
      "Override the CPU name recorded in a profile.")
    k(o, "DASLLAMA_OS", EnvKind.text, EnvArea.profiling, "probed",
      "Override the OS name recorded in a profile.")
    k(o, "DASLLAMA_RAM_GB", EnvKind.number, EnvArea.profiling, "probed",
      "Override the RAM size in GiB recorded in a profile.")
    k(o, "DASLLAMA_THREADS", EnvKind.number, EnvArea.profiling, "probed",
      "Override the thread count recorded in a profile.")
    k(o, "LLAMA_BENCH", EnvKind.path, EnvArea.profiling, "",
      "Path to the llama-bench binary used to establish reference baselines.")
    k(o, "LLAMA_BENCH_CLEAN", EnvKind.path, EnvArea.profiling, "",
      "Path to a clean-build llama-bench for the reference column.")
    k(o, "LLAMA_BENCH_STOCK", EnvKind.path, EnvArea.profiling, "",
      "Path to a stock-build llama-bench for the reference column.")
    k(o, "WHISPER_CPP", EnvKind.path, EnvArea.profiling, "",
      "whisper.cpp checkout used for ASR reference baselines.")
    k(o, "WHISPER_CPP_MODELS", EnvKind.path, EnvArea.profiling, "",
      "Directory of whisper.cpp models for ASR reference baselines.")
    k(o, "MTMD_BIN", EnvKind.path, EnvArea.profiling, "",
      "llama-mtmd-cli binary for multimodal reference baselines.")
    k(o, "NEMO_PY", EnvKind.path, EnvArea.profiling, "",
      "Python interpreter of the NeMo oracle venv, for canary/parakeet baselines.")
    k(o, "ONNX_PY", EnvKind.path, EnvArea.profiling, "",
      "Python interpreter of the ONNX oracle venv, for parakeet baselines.")
    k(o, "OS", EnvKind.text, EnvArea.profiling, "set by Windows",
      "Read to detect Windows (Windows_NT); set by the OS, not by dasLLAMA.")
    k(o, "PROCESSOR_ARCHITECTURE", EnvKind.text, EnvArea.profiling, "set by Windows",
      "Read for the CPU architecture on Windows; set by the OS, not by dasLLAMA.")
    k(o, "NUMBER_OF_PROCESSORS", EnvKind.number, EnvArea.profiling, "set by Windows",
      "Read for the core count on Windows; set by the OS, not by dasLLAMA.")
}

def private reg_test(var o : array<EnvKnob>) {
    k(o, "DASLLAMA_PARITY_FULL", EnvKind.flag, EnvArea.test, "off",
      "Run the FULL parity matrix. Without it the big-model rows silently skip.")
    k(o, "DASLLAMA_TEST_ARMS", EnvKind.text, EnvArea.test, "",
      "Comma-separated filter restricting which parity arms run.")
    k(o, "DASLLAMA_TEST_FAMILY", EnvKind.text, EnvArea.test, "",
      "Comma-separated filter restricting which model families run.")
    k(o, "DASLLAMA_LLAMA2C_DIR", EnvKind.path, EnvArea.test, "",
      "Directory of llama2.c reference checkpoints for the forward/decode parity tests.")
    k(o, "DASLLAMA_WHISPER_DIR", EnvKind.path, EnvArea.test, "",
      "Directory of whisper models for the audio tests.")
    k(o, "DASLLAMA_CORPUS_DIR", EnvKind.path, EnvArea.test, "",
      "Directory of audio corpus files for the transcription tests.")
    k(o, "TMPDIR", EnvKind.path, EnvArea.test, "/tmp",
      "Scratch directory for test artifacts; set by the OS on macOS.")
}

def private reg_example(var o : array<EnvKnob>) {
    k(o, "DASLLAMA_MODEL", EnvKind.path, EnvArea.example, "",
      "Model path for the dasLLAMA tutorials when none is passed on the command line.")
}

def private reg_core(var o : array<EnvKnob>) {
    k(o, "DAS_JOBQUE_THREADS", EnvKind.number, EnvArea.core, "conservative default",
      "Total compute lanes for job queues (N-1 workers plus the caller). Overrides set_jobque_threads_cap; see skills/environment_variables.md.")
    k(o, "DAS_JOBQUE_AFFINITY", EnvKind.number, EnvArea.core, "0",
      "Worker affinity: 0 off, 1 ideal-processor hint, 2 hard mask. Matters on big SMT boxes.")
    k(o, "DAS_JOBQUE_TEAM_RANK_GATE", EnvKind.number, EnvArea.core, "profile-driven",
      "Team-dispatch rank gate. When set, it suppresses the box profile's own team_rank_gate knob.")
    k(o, "DAS_TUNE_MANIFEST", EnvKind.path, EnvArea.core, "<app>.tune.json",
      "Kernel-tuning sidecar to read/write. Point it somewhere writable when the app dir is read-only.")
    k(o, "DAS_TUNE_MODE", EnvKind.text, EnvArea.core, "",
      "Kernel-tuning mode. The [tune] framework owns these; see skills/tune.md.")
    k(o, "DAS_TUNE_POLICY", EnvKind.text, EnvArea.core, "",
      "Kernel-tuning policy override. The [tune] framework owns these; see skills/tune.md.")
    k(o, "DAS_TUNE_COMPILE_FALLBACKS", EnvKind.text, EnvArea.core, "",
      "Semicolon-separated kernel fallbacks for the generated tune probe.")
}

//! Every environment variable dasLLAMA reads, in documentation order. This is the single source
//! `ENVIRONMENT.md` and `tests/test_env_registry.das` both consume — adding a read without an
//! entry here fails that test.
def public env_registry() : array<EnvKnob> {
    var out : array<EnvKnob>
    out |> reserve(128)
    reg_engine(out)
    reg_gpu_tier(out)
    reg_metal(out)
    reg_vulkan(out)
    reg_accelerate(out)
    reg_harness(out)
    reg_benchmark(out)
    reg_profiling(out)
    reg_test(out)
    reg_example(out)
    reg_core(out)
    return <- out
}

def private kind_name(k : EnvKind) : string {
    if (k == EnvKind.flag) return "flag"
    if (k == EnvKind.number) return "number"
    if (k == EnvKind.path) return "path"
    return "text"
}

def private area_title(a : EnvArea) : string {
    if (a == EnvArea.engine) return "Engine"
    if (a == EnvArea.metal) return "Metal backend"
    if (a == EnvArea.vulkan) return "Vulkan backend"
    if (a == EnvArea.accelerate) return "Accelerate / AMX backend"
    if (a == EnvArea.harness) return "Harness — tuner and probes"
    if (a == EnvArea.benchmark) return "Benchmarks"
    if (a == EnvArea.profiling) return "Profiling and baselines"
    if (a == EnvArea.test) return "Tests"
    if (a == EnvArea.example) return "Examples and tutorials"
    return "daslang core knobs dasLLAMA honours"
}

def private area_note(a : EnvArea) : string {
    if (a == EnvArea.engine) return "Read by the inference engine itself, so these affect any program that loads a model."
    if (a == EnvArea.metal) return "Apple GPU backend. Absent on non-Apple builds, where setting them does nothing."
    if (a == EnvArea.vulkan) return "Vulkan GPU backend. Present only where the dasVulkan package is installed."
    if (a == EnvArea.accelerate) return "Apple Accelerate / AMX float lane. `DASLLAMA_ACCEL` arms the whole group."
    if (a == EnvArea.core) return "Owned by daslang, not by dasLLAMA — listed because dasLLAMA's behaviour depends on them. `DAS_TUNE_*` is covered in `skills/tune.md`."
    return ""
}

//! Renders the whole registry as the `ENVIRONMENT.md` body, grouped by area. `modules/dasLLAMA/
//! ENVIRONMENT.md` is this output verbatim — `harness/gen_env_doc.das` writes it and
//! `tests/test_env_registry.das` fails when the checked-in file drifts from it.
def public env_markdown() : string {
    var reg <- env_registry()
    let areas <- [EnvArea.engine, EnvArea.metal, EnvArea.vulkan, EnvArea.accelerate,
                  EnvArea.harness, EnvArea.benchmark, EnvArea.profiling, EnvArea.test,
                  EnvArea.example, EnvArea.core]
    let out = build_string() $(var w) {
        w |> write("# dasLLAMA environment variables\n\n")
        w |> write("GENERATED from `env_registry()` in `dasllama/dasllama_env.das` — do not edit by hand.\n")
        w |> write("Regenerate with `daslang modules/dasLLAMA/harness/gen_env_doc.das`; a knob read anywhere\n")
        w |> write("in the tree without a registry entry fails `tests/test_env_registry.das`.\n\n")
        w |> write("Types: **flag** is unset-means-default, `0`/`false`/`off`/`no`/empty is false and\n")
        w |> write("anything else true; **number** falls back to the default when unset or unparseable;\n")
        w |> write("**text** and **path** are taken verbatim. Every read goes through `env_flag` /\n")
        w |> write("`env_int` / `env_int64` / `env_str`, so the rules are the same everywhere.\n\n")
        w |> write("A knob is read ONCE, at init or behind a one-time gate — reading the environment on a\n")
        w |> write("per-token path is what PERF027 exists to catch (see `skills/perf_lint.md`).\n")
        for (a in areas) {
            var any = false
            for (e in reg) {
                if (e.area != a) continue
                if (!any) {
                    w |> write("\n## {area_title(a)}\n\n")
                    let note = area_note(a)
                    if (!empty(note)) {
                        w |> write("{note}\n\n")
                    }
                    w |> write("| Variable | Type | Default | Effect |\n|---|---|---|---|\n")
                    any = true
                }
                let dflt = empty(e.dflt) ? "unset" : e.dflt
                w |> write("| `{e.name}` | {kind_name(e.kind)} | {dflt} | {e.doc} |\n")
            }
        }
    }
    delete reg
    return out
}

//! True when `name` is a registered knob.
def public env_is_registered(name : string) : bool {
    var reg <- env_registry()
    var found = false
    for (e in reg) {
        if (e.name == name) {
            found = true
            break
        }
    }
    delete reg
    return found
}
