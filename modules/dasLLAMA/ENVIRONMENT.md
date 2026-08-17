# dasLLAMA environment variables

GENERATED from the `[EnvConfig]` declarations in `dasllama/dasllama_env.das` - do not edit
by hand. Regenerate with `daslang modules/dasLLAMA/harness/gen_env_doc.das`; a knob read
anywhere in the tree without a declaration fails `tests/test_env_registry.das`.

Types: **flag** is unset-means-default, `0`/`false`/`off`/`no` (any case) is false and
anything else true; **number** falls back to the default when unset or unparseable, with
a logged warning on garbage; **text** and **path** are taken verbatim. A SET-BUT-EMPTY
variable counts as unset everywhere. Every knob loads ONCE, at context init, into the
`g_env_*` globals - hot code reads struct fields, and `set_env_variable` after startup
is invisible (arm a child process's environment instead).

## Engine

Read by the inference engine itself, so these affect any program that loads a model.

| Variable | Type | Default | Effect |
|---|---|---|---|
| `DASLLAMA_IMAGE` | flag | on | Use the prepared-image file beside a source model for millisecond reloads. 0 skips the file: LLM ggufs take the full parse; audio carriers mint the image in memory and serve that (they are always image-backed). |
| `DASLLAMA_IMAGE_SAVE` | flag | on | Persist the prepared image beside the source; 0 builds it in memory and serves that, leaving no .dlim behind. |
| `DASLLAMA_CPU_PREFILL` | flag | off | Allow the CPU prefill path even when a GPU prefill override is registered. |
| `DASLLAMA_PIN_PREFILL` | text | unset | Pin prefill to one registered override by name (e.g. metal, vulkan); anything else declines before upload. |
| `DASLLAMA_PIN_BACKEND` | text | unset | Pin the matmul backend by name, bypassing the measured auto-selection. |
| `DASLLAMA_PIN_BATCH_BACKEND` | text | unset | Pin the batched (prefill) matmul backend independently of the decode one. |
| `DASLLAMA_EXPERT_REUSE` | flag | off | Arm the MoE expert-reuse counter (probes and servers; benches use set_expert_reuse instead). |
| `DASLLAMA_NOISY` | flag | off | Print engine diagnostics (tier selection, upload plan, arm/decline reasons). |
| `DASLLAMA_CALLER_PRIO` | number | 2 (the top notch) | A/B rail for the dispatch caller's thread priority, -2..2; unset claims the top notch. |
| `DASLLAMA_TRUTH_REFRESH` | flag | off | Regenerate the stored parity truth files instead of comparing against them. |
| `DASLLAMA_CONV_PROF` | flag | off | Bucket gguf -> image conversion time by kind over the weight walk; one clock pair per tensor. |
| `DASLLAMA_ALLOW_INTERP_LOAD` | flag | off | Permit a big gguf load without -jit; the transforms run interpreted, so expect minutes per GB. |
| `DASLLAMA_PREFETCH` | flag | on | Advisory source-mapping readahead at gguf load (cold-conversion fix); =0 restores on-demand faulting. |
| `DASLLAMA_GPU` | flag | off | One switch for the measured-best GPU rail set; any DASLLAMA_GPU_* knob still overrides individually. |
| `DASLLAMA_GPU_MOE_LAYERS` | number | -1 (auto) | How many MoE expert layers to hold resident on the GPU; -1 lets the upload walk place the split. |
| `DASLLAMA_GPU_MOE_STREAM` | number | -1 (auto) | How many MoE layers to stream rather than hold resident; -1 is auto. |
| `DASLLAMA_GPU_VRAM_MB` | number | probed | Override the detected VRAM budget in MiB that sizes the resident expert stacks. |
| `DASLLAMA_GPU_MIN_CTX` | number | built-in floor | Lower the context floor for arming the resident decode driver, for a short-context session on a small card. |
| `DASLLAMA_GPU_DN` | flag | follows DASLLAMA_GPU | DeltaNet rail on the GPU. |
| `DASLLAMA_GPU_DND` | flag | follows DASLLAMA_GPU | DeltaNet decode rail on the GPU. |
| `DASLLAMA_GPU_ATTN` | flag | follows DASLLAMA_GPU | Attention rail on the GPU. |
| `DASLLAMA_GPU_DENSE` | flag | follows DASLLAMA_GPU | Dense-weight rail on the GPU. |
| `DASLLAMA_GPU_DENSE_ATTN` | flag | on | The dense rail's attention half (per-layer q/o pairs); opt-OUT, measured a loss on gemma-4. |
| `DASLLAMA_GPU_DENSE_SHEXP` | flag | off | The dense rail's gemma-4 shared-expert w1/w3/w2 triple; opt-IN, measured a wash-to-loss. |
| `DASLLAMA_GPU_SHEXP` | flag | follows DASLLAMA_GPU | Shared-expert rail on the GPU. |
| `DASLLAMA_GPU_QKV` | flag | off | Fused QKV rail on the GPU; stays off under DASLLAMA_GPU (measured a wash). |
| `DASLLAMA_GPU_CLS` | flag | on | Keep the classifier plane on the GPU — the best ms-per-GB region; 0 is the CPU A/B lever. |
| `DASLLAMA_GPU_COMBINE` | flag | on | Device-side routed MoE combine; 0 falls back to the host combine. |
| `DASLLAMA_GPU_HEAT` | number | 0 | Expert heat threshold: hold the N hottest experts resident regardless of layer placement. |
| `DASLLAMA_GPU_PROF` | flag | off | Report lifetime GPU queue submissions (real commands plus staging round-trips). |

## Vision

The image preprocessing rail: dynamic-resolution token bounds and the debug dump.

| Variable | Type | Default | Effect |
|---|---|---|---|
| `DASLLAMA_VISION_MIN_TOKENS` | number | 40 (the gemma-4 model default) | Minimum soft tokens per image; dynamic-resolution geometry upscales to reach it. |
| `DASLLAMA_VISION_MAX_TOKENS` | number | 280 (the gemma-4 model default) | Maximum soft tokens per image; dynamic-resolution geometry downscales to fit. |
| `DASLLAMA_VISION_DUMP` | text | unset | Path prefix: dump every letterboxed image fed to a vision embedder as <prefix><tag>.ppm — the human smoke test; empty = off. |

## Metal backend

Apple GPU backend. Absent on non-Apple builds, where setting them does nothing.

| Variable | Type | Default | Effect |
|---|---|---|---|
| `DASLLAMA_METAL_LOGITS` | flag | on | Produce logits on the GPU; 0 pulls the classifier back to the CPU. Blob-only models force it on. |
| `DASLLAMA_METAL_ATTN` | flag | on | ggml-geometry QK/AV prefill attention (~10x the trio GEMMs); 0 pins the trio. |
| `DASLLAMA_METAL_SPAN` | flag | on | Serve the non-causal media span on the prefill driver (AttnArgs.uend); 0 declines it to the CPU arm, which then needs declared CPU intent. |
| `DASLLAMA_METAL_TOWER` | flag | on | Serve tower/embedder encodes (gemma4uv; the whisper-class towers as they land) on the Metal tower driver; 0 pins the CPU tower. |
| `DASLLAMA_METAL_WDEC` | flag | on | Serve the whisper decoder side (cross-KV; the decode step as it lands) on the Metal ASR-decoder driver; 0 pins the CPU decoder. |
| `DASLLAMA_METAL_ATTN_D` | flag | on | Fused single-pass decode attention (assumes head_size 128); 0 is the A/B rail to the chunked pair. |
| `DASLLAMA_METAL_ATTN_SINGLE` | number | 64 | Row count below which attention uses the single-chunk kernel; clamped to 128. |
| `DASLLAMA_METAL_MULMM` | flag | on | The mul_mm prefill GEMM; 0 falls back to the legacy per-op path. |
| `DASLLAMA_METAL_NCB` | number | ~4 layers/chunk | Command-buffer split: each chunk commits as encoded so the scheduler overlaps chunk k with k-1. |
| `DASLLAMA_METAL_UNRETAINED` | flag | off | Skip per-dispatch retain/release on the prefill command buffers. |
| `DASLLAMA_METAL_PF_CAPTURE` | flag | on | Record each prefill chunk as a step graph and replay on a concurrent encoder; 0 is serial-encode rollback. |
| `DASLLAMA_METAL_SCHED` | number | 1 | Graph scheduling mode for replay; 0 keeps capture order. |
| `DASLLAMA_METAL_DECODE_CONCURRENT` | flag | on | Concurrent encoder for the single-stream decode step, with barriers only at detected hazards. |
| `DASLLAMA_METAL_FUSE` | flag | on | The R1 epilogue-fusion kernel (post_attn_rms + add_rms); 0 restores separate dispatches for A/B. |
| `DASLLAMA_METAL_SPEC` | number | -1 (adaptive) | MTP speculative decode chain: 0 off, 1 forced, -1 adaptive. |
| `DASLLAMA_METAL_GEMV_TG` | number | 4 | Rows per GEMV threadgroup, clamped 1..32. |
| `DASLLAMA_METAL_KQ_B8` | flag | on | Single-pass B8 twin for K-quant small-batch mv at B=5..8; 0 is the A/B rail. |
| `DASLLAMA_METAL_KV_MIRROR_MB` | number | 4096 | Ceiling in MiB for the device-side KV mirror, clamped 64..4096. |
| `DASLLAMA_METAL_HAZARD_PARANOID` | flag | off | Barrier at every dispatch instead of at tracker-detected hazards (correctness bisect). |
| `DASLLAMA_METAL_HAZARD_STRICT` | flag | off | Treat every detected hazard as strict, widening barriers (correctness bisect). |
| `DASLLAMA_METAL_PIPE_DEBUG` | flag | off | Per-step pipeline trace: GPU envelope and true inter-step handoff idle, first steps plus outliers. |
| `DASLLAMA_METAL_DECODE_SKIP` | text | unset | Comma-separated dispatch names to skip in decode — a bring-up bisect that breaks correctness. |
| `DASLLAMA_METAL_PREFILL_SKIP` | text | unset | Prefill twin of DASLLAMA_METAL_DECODE_SKIP. |
| `DASLLAMA_MTP_DEBUG` | text | unset | MTP bring-up bisect: cold logs the failing gate, reject forces the reject path. |
| `DASLLAMA_METAL_BATCH_GEMM_MIN` | number | 5 | Batch size at which batched decode switches from GEMV to GEMM; floor 2. |
| `DASLLAMA_METAL_BATCH_SK` | flag | on | Split-K in the batched decode GEMM. |
| `DASLLAMA_METAL_BATCH_NCB` | number | 4 | Command buffers per batched decode step, clamped 1..16. |
| `DASLLAMA_METAL_BATCH_CONCURRENT` | flag | on | Concurrent encoder for batched decode. |
| `DASLLAMA_METAL_BATCH_FUSE` | flag | on | Epilogue fusion in batched decode. |
| `DASLLAMA_METAL_BATCH_MV` | flag | on | Fixed-B GEMV forms for small batched decode. |
| `DASLLAMA_METAL_BATCH_MV8` | flag | on | The column-tiling MV8 probe for batched decode. |
| `DASLLAMA_METAL_BATCH_PIPE` | flag | off | Pipelined batched decode submission. |
| `DASLLAMA_METAL_BATCH_MM` | flag | off | The mul_mm rail for batched decode; measured negative at M-pad-32, kept as the A/B rail. |

## Vulkan backend

Vulkan GPU backend. Present only where the dasVulkan package is installed.

| Variable | Type | Default | Effect |
|---|---|---|---|
| `DASLLAMA_COOPMAT` | text | auto | Cooperative-matrix mode; the flash-attention twin needs it even when the GEMM runs sdot4. |
| `DASLLAMA_MM_SMALL` | text | 32 | Small-batch tier: 32 = sdot4 (default, beats both coopmat tiles below the crossover), 64 = coopmat M, 128 = always-L. |
| `DASLLAMA_MM_SMALLD` | number | 64 | Small-d cutoff routing narrow roles (k/v) to the small tier; widening measured worse, so this is an instrument. |
| `DASLLAMA_VK_FUSE` | flag | on | Fused decode tail (add+rms+requant, qk-norm+rope); 0 pins the split dispatches for a same-build A/B. |
| `DASLLAMA_VK_XFERQ` | flag | on | Stream expert uploads on the dedicated transfer queue, overlapped via a timeline semaphore; 0 keeps the single-queue rail. |
| `DASLLAMA_VK_IMPORT` | flag | on | Stream mirrors import the mapped .dlim (VK_EXT_external_memory_host) instead of pinned copies; =0 restores the copy path. |
| `DASLLAMA_TRIM` | flag | off | Serve from P3-trimmed vulkan images (big CPU weight families dropped; folded into the flavor identity). |
| `DASLLAMA_VK_MEMPRIO` | flag | on | Tag allocations high-priority (VK_EXT_memory_priority) so the driver demotes desktop memory, not ours. |
| `DASLLAMA_VK_FA` | flag | on | Vulkan flash attention: the decode fa kernel pick AND the cm2 prefill fa tile; 0 falls back to the chunked/scalar paths. |
| `DASLLAMA_VK_REBAR` | flag | on | Use a ReBAR device-local host-visible heap when one larger than 1GB is present. |
| `DASLLAMA_VK_KV32` | number | 0 | Arm the resident driver with f32 KV mirrors instead of the f16 default (A/B instrument; only sessions of the armed codec are served). |
| `DASLLAMA_CM2_TILE` | number | 0 | cm2 prefill tile pick: 0 = occupancy heuristic, 128 = force the m tile, 256 = force the l tile (A/B instrument). |
| `DASLLAMA_CM2_SPLITK` | number | 0 | cm2 split-k: 0 = occupancy heuristic, 1 = off, N = force N k-chunks (A/B instrument; shrinks if N strands an empty tail). |
| `DASLLAMA_VK_SPV_OVERRIDE` | path | unset | Directory of <kernel>.spv files served instead of the emitted words at pipeline creation (offline spirv-opt / hand-patched A/B instrument). |
| `DASLLAMA_VK_HAZARD_PARANOID` | flag | off | Barrier at every dispatch (correctness bisect). |
| `DASLLAMA_VK_HAZARD_TRACE` | flag | off | Log every detected hazard and the barrier it produced. |

## MoltenVK (macOS)

MoltenVK's own configuration - listed because the vulkan tier arms it on Apple.

| Variable | Type | Default | Effect |
|---|---|---|---|
| `MVK_CONFIG_USE_METAL_ARGUMENT_BUFFERS` | flag | unset (tier forces 0) | MoltenVK's own knob. Its argument-buffers default miscompiles the batch tile kernels (stores never land, silently); the tier forces discrete bindings when this is unset. |

## Accelerate / AMX backend

Apple Accelerate / AMX float lane. `DASLLAMA_ACCEL` arms the whole group.

| Variable | Type | Default | Effect |
|---|---|---|---|
| `DASLLAMA_ACCEL` | flag | off | Arm the Apple Accelerate / AMX float-batch override. |
| `DASLLAMA_ACCEL_F16` | flag | off | Use the BNNS f16 lane inside the Accelerate backend. |
| `DASLLAMA_ACCEL_F16_STRIPS` | number | 0 | Strip count for the f16 lane; 0 lets the backend choose. |
| `DASLLAMA_ACCEL_STRIPS` | number | d/256, max 8 | Strip count for the f32 sgemm lane; 0 keeps the shape-derived default. |
| `DASLLAMA_ACCEL_MIN_MMAC` | number | backend default | Minimum MMAC count below which Accelerate declines and the daslang kernel runs. |
| `DASLLAMA_ACCEL_MIN_NTOK` | number | 32 | Minimum token count for the Accelerate float-batch override, floor 1. |

## Harness - tuner and probes

| Variable | Type | Default | Effect |
|---|---|---|---|
| `DASLLAMA_MODELS_DIR` | path | unset | Directory holding the .gguf models the probes, benches and tests load. |
| `DASLLAMA_CONFIRM_MODEL` | path | auto-resolved from the models dir | Model used by the tuner's confirm gate (FULL path, not a bare filename). Unset: the gate auto-resolves from the models dir — the preferred confirm carrier, else the largest present q8 gguf; the fallback pins only when the box has no q8 model at all. |
| `DASLLAMA_BATCH_CHUNKS` | text | unset | Override the batched-dispatch chunk count in the 1-core GEMM probe. |
| `DASLLAMA_BATCH_GRID_2D` | number | unset | Use the 2D batch grid in the parity probe. |
| `DASLLAMA_FOCUS_BACKEND` | text | unset | Restrict the 1-core GEMM probe to one backend. |
| `DASLLAMA_FOCUS_SHAPE` | text | unset | Restrict the 1-core GEMM probe to one shape. |
| `DASLLAMA_FOCUS_NTOK` | number | unset | Restrict the 1-core GEMM probe to one token count. |
| `DASLLAMA_FOCUS_FORKED` | flag | off | Run the 1-core GEMM probe through the fork pool rather than inline. |
| `GEN_TUNE_COMPILE_ONLY` | flag | off | Compile the generated tune probe without running it. |
| `SMMLA_PROBE_COMPILE_ONLY` | flag | off | Compile the smmla probe without running it. |
| `METAL` | flag | off | Run the Accelerate contention probe against the Metal backend. |
| `DAS_DETAIL` | flag | off | Per-job dump in the dispatch probe. |
| `DAS_SPIN_NS` | number | unset | Synthetic per-job spin in nanoseconds for the dispatch probe. |
| `DAS_BATCH_DISPATCH` | flag | off | Exercise batched dispatch in the dispatch probe. |
| `DAS_TWO_WAVE` | flag | off | Exercise the two-wave dispatch shape in the dispatch probe. |
| `DAS_FORK_SKIP_HEAP_RESET` | flag | off | Skip restartHeaps on pooled fork-context reuse (A/B for the fork-pool heap cost). |
| `DAS_WORKER_SPIN_US` | number | unset | Worker spin-before-park window in microseconds, applied via set_jobque_spin_us. |
| `DAS_JOBQUE_JOIN_SPIN` | number | unset | Join-side spin window for the 1-core GEMM probe. |

## Benchmarks

| Variable | Type | Default | Effect |
|---|---|---|---|
| `DASLLAMA_ALLOW_UNTUNED` | flag | off | Let a bench cell measure on fallback/absent tune winners, and skip the auto-tune mint entirely (untuned kernels keep fallback stamps instead of paying the minutes-long tune). The gate exists because a stale sidecar silently downgrades every kernel and the whole board sandbags; only set for dev runs whose numbers are never published. |
| `DASLLAMA_BENCH_MODEL` | text | tinyllama | Model name for the isolated GEMM bench. |
| `DASLLAMA_BENCH_NTOK` | number | unset | Token count for the isolated GEMM bench. |
| `DASLLAMA_BENCH_SKIP_ROWMAJOR` | flag | off | Skip the row-major arm of the isolated GEMM bench. |
| `DASLLAMA_BENCH_VERBOSE` | flag | off | Per-run detail from the llama.cpp comparison bench. |
| `DASLLAMA_MIN_CHUNK_ROWS` | number | unset | Override the minimum rows per dispatch chunk in the GEMM bench. |
| `DASLLAMA_FUSED_DECODE` | flag | unset | Exercise the fused decode arm in the emission bench. |
| `DASMETAL_LAB_ROUNDS` | number | 3 | Timing rounds per cell in the Metal kernel labs. |
| `DASMETAL_LAB_PASSES` | number | 4 | Passes per round in the Metal MoE lab. |
| `DASMETAL_LAB_ATTN_NSGS` | number | 16 | Simdgroups per threadgroup in the Metal attention lab. |
| `DASMETAL_LAB_DUMP_MSL` | text | unset | Dump generated MSL from the Metal labs instead of running it. |
| `DASMETAL_LAB_ARMS` | text | unset | Substring filter restricting which arms the Metal MoE lab runs. |
| `DASMETAL_LAB_SHAPES` | text | unset | Substring filter restricting which shapes the Metal GEMV/GEMM labs run. |
| `DASMETAL_LAB_VARIANTS` | text | unset | Substring filter restricting which kernel variants the Metal GEMV/GEMM labs run. |
| `PROBE_PATH` | path | _wcliff.bin | Scratch file the write-cliff probe writes, cwd-relative by default; point it at the drive under test. |
| `PROBE_GB` | number | 50 | Gigabytes the write-cliff probe writes before reporting. |

## Profiling and baselines

| Variable | Type | Default | Effect |
|---|---|---|---|
| `DASLANG_BIN` | path | bin/daslang | daslang binary the profiling harness shells out to. |
| `DASLLAMA_BOX` | text | probed | Override the box identifier recorded in a profile. |
| `DASLLAMA_CPU` | text | probed | Override the CPU name recorded in a profile. |
| `DASLLAMA_OS` | text | probed | Override the OS name recorded in a profile. |
| `DASLLAMA_RAM_GB` | number | probed | Override the RAM size in GiB recorded in a profile. |
| `DASLLAMA_THREADS` | number | probed | Override the thread count recorded in a profile. |
| `DASLLAMA_GPU_NAME` | text | probed | Override the GPU name recorded in a box profile. |
| `LLAMA_BENCH` | path | unset | Path to the llama-bench binary used to establish reference baselines. |
| `LLAMA_BENCH_CLEAN` | path | unset | Path to a clean-build llama-bench for the reference column. |
| `LLAMA_BENCH_STOCK` | path | unset | Path to a stock-build llama-bench for the reference column. |
| `WHISPER_CPP` | path | ../whisper.cpp | whisper.cpp checkout used for ASR reference baselines. |
| `WHISPER_CPP_MODELS` | path | unset | Directory of whisper.cpp models for ASR reference baselines. |
| `MTMD_BIN` | path | unset | llama-mtmd-cli binary for multimodal reference baselines. |
| `DASLLAMA_BASE_PYTHON` | path | unset | Base CPython 3.10-3.12 for creating the ASR oracle venvs, when neither PATH names nor uv resolve one in range. |
| `NEMO_PY` | path | unset | Python interpreter of the NeMo oracle venv, for canary/parakeet baselines. |
| `ONNX_PY` | path | unset | Python interpreter of the ONNX oracle venv, for parakeet baselines. |
| `OS` | text | set by Windows | Read to detect Windows (Windows_NT); set by the OS, not by dasLLAMA. |
| `PROCESSOR_ARCHITECTURE` | text | set by Windows | Read for the CPU architecture on Windows; set by the OS, not by dasLLAMA. |
| `NUMBER_OF_PROCESSORS` | number | set by Windows | Read for the core count on Windows; set by the OS, not by dasLLAMA. |

## Tests

| Variable | Type | Default | Effect |
|---|---|---|---|
| `DASLLAMA_PARITY_FULL` | flag | off | Run the FULL parity matrix. Without it the big-model rows silently skip. |
| `DASLLAMA_TEST_ARMS` | text | unset | Comma-separated filter restricting which parity arms run. |
| `DASLLAMA_TEST_FAMILY` | text | unset | Comma-separated filter restricting which model families run. |
| `DASLLAMA_LLAMA2C_DIR` | path | unset | Directory of llama2.c reference checkpoints for the forward/decode parity tests. |
| `DASLLAMA_WHISPER_DIR` | path | unset | Directory of whisper models for the audio tests. |
| `DASLLAMA_CORPUS_DIR` | path | unset | Directory of audio corpus files for the transcription tests. |
| `TMPDIR` | path | /tmp | Scratch directory for test artifacts; set by the OS on macOS. |
| `TEMP` | path | unset | Windows scratch-directory fallback when TMPDIR is unset (the test runner's log dir). |

## Examples and tutorials

| Variable | Type | Default | Effect |
|---|---|---|---|
| `DASLLAMA_MODEL` | path | unset | Model path for the dasLLAMA tutorials when none is passed on the command line. |

## Sidecar exchange

Overrides for the dasllama.io exchange client.

| Variable | Type | Default | Effect |
|---|---|---|---|
| `DASLLAMA_EXCHANGE_URL` | text | unset | Sidecar exchange base URL override (tests, mirrors); unset = the baked-in dasllama.io. |
| `DASLLAMA_EXCHANGE_ACCEPT` | text | unset | One-shot exchange accept-policy override: verified | any | off; unset = the app config's exchange_accept. The watchdog arms 'any' on a relaunch when the user adopts an unverified sidecar over finishing a tune. |

## daslang core knobs dasLLAMA honours

Owned by daslang, not by dasLLAMA - listed because dasLLAMA's behaviour depends on them. `DAS_TUNE_*` is covered in `skills/tune.md`.

| Variable | Type | Default | Effect |
|---|---|---|---|
| `HF_HOME` | path | ~/.cache/huggingface | Hugging Face hub root; fetch_models --convert resolves checkpoint blobs under <HF_HOME>/hub. Falls back to HOME's default cache location when unset. |
| `HOME` | path | unset | Ambient platform variable; read only to derive the default Hugging Face cache when HF_HOME is unset. |
| `DAS_JOBQUE_THREADS` | number | conservative default | Total compute lanes for job queues (N-1 workers plus the caller). Overrides set_jobque_threads_cap; see skills/internal/environment_variables.md. |
| `DAS_JOBQUE_AFFINITY` | number | 2 on darwin (engine [init]), else 0 | Worker affinity: 0 off, 1 ideal-processor hint, 2 hard mask — on darwin mode 2 is QoS classification (no pin API) and dasLLAMA defaults it on. Matters on big SMT boxes. |
| `DAS_JOBQUE_TEAM_RANK_GATE` | number | profile-driven | Team-dispatch rank gate. When set, it suppresses the box profile's own team_rank_gate knob. |
| `DAS_TUNE_MANIFEST` | path | <app>.tune.json | Kernel-tuning sidecar to read/write. Point it somewhere writable when the app dir is read-only. |
| `DAS_TUNE_MODE` | text | unset | Kernel-tuning mode. The [tune] framework owns these; see skills/tune.md. |
| `DAS_TUNE_POLICY` | text | unset | Kernel-tuning policy override. The [tune] framework owns these; see skills/tune.md. |
| `DAS_TUNE_COMPILE_FALLBACKS` | text | unset | Semicolon-separated kernel fallbacks for the generated tune probe. |
| `JOBQUE_PROFILING` | flag | off | Compile the jobque marker rail in (read at COMPILE time by daslib/build_const). Without it trace_tag/trace_marker erase from the program; decode_prof --trace refuses on a build without it. |
