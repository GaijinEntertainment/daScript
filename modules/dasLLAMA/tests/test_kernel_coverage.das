// The parakeet tower row needs ggml-parakeet-tdt-0.6b-v2-f32.bin under whisper_dir() and skips without it, the GigaAM row
// gigaam-v3-e2e-rnnt-f16.gguf under models_dir() and the gigaam family on; the
// StyleTTS2 tower row needs kitten-nano.gguf (and kokoro-82m.gguf for the torch-law stamps) under models_dir() and the kitten
// family on, and skips without them; the Pocket row needs pocket-tts-en.gguf (and pocket-tts-en-kq.gguf for the q8 head stamps) and the pocket family on;
// the Bergamot row needs bergamot/ru-en-tiny/model.ruen.intgemm.alphas.bin (its vocab.ruen.spm beside it) under models_dir() and
// the bergamot family on, and runs on das_metal builds alone.
options gen2
options persistent_heap   // + explicit deletes: the models free before the process exits
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // nolint:STYLE029,LINT019 — the public facade (engine + arch registrations + the metal overrides); only the metal half uses more than dasllama_common
require dasllama/dasllama_math   // set_wscale_f16 — the s16 axis
require dasllama/dasllama_layout   // convert_model_to_metal_blob — the blob-flavor transform
require ?das_metal dasllama/dasllama_metal_common  // the coverage census rail
require ?das_metal dasllama/dasllama_metal_decode  // decode stats + shutdown rails
require ?das_metal metal/das_metal_boost  // device probe
require ?vulkan dasllama/dasllama_vulkan_common  // the vulkan census rail (spec-only here)
require ?vulkan dasllama/dasllama_vulkan_tower   // set_vulkan_tower - the serving census pins the TTS rows' knob ON
require daslib/defer
require dasllama/dasllama_gpu_resident  // moe_gpu_drop_model_ — the census's model-switch drops
require dasllama/dasllama_whisper   // the cov_tower whisper row
require dasllama/dasllama_audio     // load_audio_tower — the cov_tower erf-GELU carrier
require dasllama/dasllama_gemma4uv  // the cov_tower embedder row
require dasllama/dasllama_gemma4v   // the cov_tower ViT row
require dasllama/dasllama_gemma3v   // the cov_tower SigLIP row (the head-restride kernel)
require dasllama/dasllama_qwen3a    // the cov_tower conv2d + GPU-mel carrier
require dasllama/dasllama_gemma4a   // the cov_tower Conformer whole-chunk row
require dasllama/dasllama_canary    // the cov_tower FastConformer row
require dasllama/dasllama_parakeet  // the cov_tower parakeet row
require dasllama/dasllama_gigaam    // the cov_tower GigaAM row
require dasllama/dasllama_nmt       // stage_nmt_model/mint_nmt_model: the cov_tower Bergamot row, minted in memory
require ?das_metal dasllama/dasllama_metal_nmt   // set_metal_nmt - the cov_tower Bergamot row's knob
require dasllama/dasllama_vad       // nolint:STYLE030,LINT019 — the cov_tower silero VAD row, inside the GPU static_if halves
require dasllama/dasllama_qwen25v   // the cov_tower window-ViT row
require dasllama/dasllama_qwen3v    // the cov_tower GPU-stem row
require dasllama/dasllama_styletts2
require dasllama/dasllama_pocket   // nolint:STYLE030,LINT019 — the Pocket census leg sits in the das_metal static_if half
require dasllama/dasllama_tts_types   // nolint:STYLE030,LINT019 — read only inside the Apple half
require ?das_metal dasllama/dasllama_metal_tower   // set_metal_tower — the census pins the rail ON
require ?das_metal dasllama/dasllama_metal_prefill // metal_prefill_shutdown — the crowned twin-W leg re-arms PSOs
require ?das_metal dasllama/dasllama_metal_asr_dec // set_metal_wdec — the cov_tower mixed chunk's decoder half
require ?das_metal dasllama/dasllama_audio_io
require daslib/jobque_boost
require daslib/fio  // the zoo paths
require math  // nolint:STYLE030,LINT019 — used only inside the das_metal static_if half
require strings
require _model_tier
require ../performance/model_specs.das   // nolint:STYLE030,LINT019 — counting_prompt, read only inside the das_metal and vulkan static_if halves
require _gpu_knobs   // nolint:STYLE030,LINT019 — gpu_knobs: the vulkan census's knob record, read inside the vulkan half
require dasllama/dasllama_env   // nolint:STYLE030,LINT019 — the read sites sit in the apple static_if half; the linux lint flavor sees the require as unused

// The KERNEL COVERAGE census (REVIEW_GPU_KERNEL_CLASSES.md: a Metal kernel class is dispatched by a census row here or named in
// CENSUS_NEVER_DISPATCHED; a Vulkan kernel class by a census row or VK_CENSUS_NEVER_DISPATCHED): drive the small-model zoo across the
// format / graph / batch-shape / KV-codec axes and report which compiled kernels were never
// dispatched — LOUD WARNINGS, never an auto-dead verdict. A zero means "nothing this zoo runs
// dispatched it", never "unreachable" — deleting a kernel additionally needs a reachability
// audit of its dispatch predicate (models/quants/shapes not stocked here still count). This
// suite is the gate a kernel deletion must run before ("was it ever dispatched by anything
// we run?") and the home a NEW kernel's small-model run joins. Expected warnings on the small tier: MoE / mx4 / suppress and other
// kernels whose carriers sit above the tier (their census rows serve under
// DASLLAMA_PARITY_FULL=1); the report stays honest about what THIS run covered. The vulkan
// half: arm `coverage` = the device-free rail unit (spec parse + zero table); arm `coverage-vk`
// = the SERVING census (needs a vulkan device + DASLLAMA_GPU=1 + the local model zoo).

// The census's DECLARED blind spots: kernel classes no zoo row below can dispatch, each with
// the reason. A new [metal_kernel] belongs in a census row, not here; a class lands here only
// when no stocked carrier can reach it, and it leaves the moment a row can. Reconcile the list
// against the report's NEVER DISPATCHED warnings — a name here that the report shows dispatched
// is stale, and a warning with no entry here is a coverage gap to close with a row.
let private CENSUS_NEVER_DISPATCHED <- [
    // the classifier tail's selection (compiled on every Metal arm, dispatched only for a sampled stream's step; the zoo's rows
    // step greedy); its coverage is cls_select_gate in test_metal_misc_kernels.das
    "metal_cls_select",
    // the MoE expert mul_mm tall M-tile pair (128-row floor + remainder) in q5_K: the zoo
    // stocks no Q5_K MoE carrier, where every other format's pair rides a census row's
    // window. Its coverage is the tall-pair kernel-unit gate in test_metal_gemm_kernels.das
    "MetalMoeMulMmK5TH128", "MetalMoeMulMmK5THR",
    // the q5_1 MoE tensor twins, ALL stamps: no stocked carrier holds a Q5_1 expert plane
    // (the zoo's MoE carriers are Q8_0 / Q5_K_M / Q4_K_M / mxfp4; Q5_1 appears only when
    // expert_ffn % 256 != 0 forces the converter off K-quants). Their coverage is the
    // bit-exact twin + tall-pair gate in test_metal_prefill_kernels.das (moe_mulmm_b32_gate)
    "MetalMoeMulMmQ51T", "MetalMoeMulMmQ51TH",
    "MetalMoeMulMmQ51TH128", "MetalMoeMulMmQ51THR",
    // the iq4nl32 MoE tensor twins, ALL stamps: the per-32 IQ4_NL expert planes ride only a
    // carrier whose expert rows are off the 256 lattice (Qwen3.8-Flash-Next's 640-wide down
    // stacks, far above the zoo's tier). Their coverage is the same gate's iq4nl32 arm
    "MetalMoeMulMmIq4nl32T", "MetalMoeMulMmIq4nl32TH",
    "MetalMoeMulMmIq4nl32TH128", "MetalMoeMulMmIq4nl32THR",
    // the dense tall in-kernel-dequant kq stamps: they engage only when a site's f16 panel
    // would reach TALLKQ_MIN_PANEL (96 MiB) - the zoo's small carriers top out far below it,
    // and the iq3 pair additionally wants a plane in a format the zoo does not stock.
    // Their coverage is the tall kernel-unit leg in test_metal_gemm_kernels.das (kq_mulmm_gate)
    "MetalKqMulMmK4TH128", "MetalKqMulMmK5TH128", "MetalKqMulMmK6TH128",
    "MetalKqMulMmIq3sTH128", "MetalKqMulMmIq3xxsTH128",
    // the nine formats the iquant arc added (Q2_K, Q3_K, IQ4_XS, IQ4_NL, IQ3_S, IQ3_XXS, IQ2_S,
    // IQ2_XS, IQ2_XXS): no stocked census carrier holds a plane in any of them - the zoo quants
    // are Q8_0 / Q5_K_M / Q4_K_M / mxfp4. Their coverage is the kernel-unit gates in
    // test_metal_gemv_kernels.das / test_metal_gemm_kernels.das and the per-format e2e parity
    // rows recorded in HOW_TO_ADD_A_FORMAT.md. A class naming its own kernel records the bare
    // shader entry name, so those stamps sit here under it, not under the class
    "metal_kq_gemv_k2", "MetalKqMvB2K2", "MetalKqMvB4K2", "MetalKqMvB8K2", "MetalKqMulMmK2",
    "metal_kq_gemv_k3", "MetalKqMvB2K3", "MetalKqMvB4K3", "MetalKqMvB8K3", "MetalKqMulMmK3",
    "MetalKqGemvIq4xs", "MetalKqMvB2Iq4xs", "MetalKqMvB4Iq4xs", "MetalKqMvB8Iq4xs", "MetalKqMulMmIq4xs",
    "MetalKqGemvIq4nl", "MetalKqMvB2Iq4nl", "MetalKqMvB4Iq4nl", "MetalKqMvB8Iq4nl", "MetalKqMulMmIq4nl",
    "metal_kq_gemv_iq3s", "MetalKqMvB2Iq3s", "MetalKqMvB4Iq3s", "MetalKqMvB8Iq3s", "MetalKqMulMmIq3s",
    "metal_kq_gemv_iq3xxs", "MetalKqMvB2Iq3xxs", "MetalKqMvB4Iq3xxs", "MetalKqMvB8Iq3xxs", "MetalKqMulMmIq3xxs",
    "metal_kq_gemv_iq2s", "MetalKqMvB2Iq2s", "MetalKqMvB4Iq2s", "MetalKqMvB8Iq2s", "MetalKqMulMmIq2s",
    "metal_kq_gemv_iq2xs", "MetalKqMvB2Iq2xs", "MetalKqMvB4Iq2xs", "MetalKqMvB8Iq2xs", "MetalKqMulMmIq2xs",
    "MetalKqGemvIq2xxs", "MetalKqMvB2Iq2xxs", "MetalKqMvB4Iq2xxs", "MetalKqMvB8Iq2xxs", "MetalKqMulMmIq2xxs",
    // the same nine formats' crown-gated forms (the metal-kq-race arc): the f4-slab GEMV twins
    // and the tensor mul_mm T/TH stamps compile only behind a box crown and dispatch only on
    // planes the zoo does not stock. Their coverage is the same kernel-unit gates plus the
    // tensor-twins/iq2xxs matrix row and the mint's METAL_TWIN race rows
    "metal_kq_gemv_iq3s_f4", "metal_kq_gemv_iq3xxs_f4", "MetalKqGemvIq2xxsF4", "metal_moe_gemv_iq3s_f4",
    "MetalKqMulMmIq4xsT", "MetalKqMulMmIq4xsTH", "MetalKqMulMmIq4nlT", "MetalKqMulMmIq4nlTH",
    "MetalKqMulMmK3T", "MetalKqMulMmK3TH", "MetalKqMulMmK2T", "MetalKqMulMmK2TH",
    "MetalKqMulMmIq3sT", "MetalKqMulMmIq3sTH", "MetalKqMulMmIq3xxsT", "MetalKqMulMmIq3xxsTH",
    "MetalKqMulMmIq2sT", "MetalKqMulMmIq2sTH", "MetalKqMulMmIq2xsT", "MetalKqMulMmIq2xsTH",
    "MetalKqMulMmIq2xxsT", "MetalKqMulMmIq2xxsTH",
    // the device attention pair's 128-row AV tail: it serves a head off the 64 lattice (phi-3's 96) when the
    // window's query panel divides by 128, and the census prompt's never does (its 32-row twin serves every
    // phi3 window). Its coverage is attn_dev_pair_gate's mt=128 hs=96 arm in test_metal_gemm_kernels.das
    "MetalAttnAVDev128Tail",
    // the MTP round's kernels: the same-slab verify's per-row argmax dispatches only inside a
    // speculative round (the census prompt never speculates), and the k4 two-row tile only under
    // the kq_mvb2_k4_r2 crown. Their coverage is the argmax_rows gate in test_metal_misc_kernels.das,
    // the r2 arm of kq_mvb_gate plus the pair-walk gate in test_metal_gemv_kernels.das, and the
    // mtp suite's parity arms
    "metal_argmax_rows", "MetalKqMvB2K4R2",
    // the half-x stamps of the formats the zoo's batched carriers DO hold (k4's wide forms, k5 and k6's
    // small-batch forms): every half stamp compiles at decode init, and dispatches only where the box crowned
    // its `kq_rows_half_<fmt>`. Their coverage is test_metal_kq_half_forms in test_metal_gemv_kernels.das and the
    // batchB8-kq half leg of the decode parity file. The nine formats above cover their own half stamps by
    // prefix (the stamp is the f32 class's name plus H)
    "MetalKqMvW2K4H", "MetalKqMvW4K4H", "MetalKqMvW8K4H",
    "MetalKqMvB2K5H", "MetalKqMvB4K5H", "MetalKqMvB8K5H", "MetalKqMvB2K6H", "MetalKqMvB4K6H", "MetalKqMvB8K6H",
    // the Q4_0 small-batch twins: the gemma4moe row reaches every other q40 dense stamp, but its
    // decode steps are single rows, so the B2/B4/B8 tiles need a batched carrier no zoo row runs.
    // The simdgroup dense mul_mm loses the box race to its own tensor stamp, so it never serves
    // either - its entry carries the method suffix, since the bare class name prefix-matches the
    // T/TH keys the row DOES dispatch. Their coverage is kq_mvb_gate(fmt 40) in
    // test_metal_gemv_kernels.das and kq_mulmm_gate(fmt 40) in test_metal_gemm_kernels.das
    "MetalKqMvB2Q40", "MetalKqMvB4Q40", "MetalKqMvB8Q40", "MetalKqMulMmQ40_metal_kq_mulmm",
    // the nv4 half-x small-batch twins: the NVFP4 llama row's batched steps dispatch the f32 forms, and the half forms wait
    // on a `kq_rows_half_nv4` crown no box has raced. Their coverage is test_metal_kq_half_forms in test_metal_gemv_kernels.das;
    // the nv4 token-table gather's carrier (an NVFP4 table) is minted nowhere - its coverage is embed_kq_gate (fmt 49) in
    // test_metal_misc_kernels.das
    "MetalKqMvB2Nv4H", "MetalKqMvB4Nv4H", "MetalKqMvB8Nv4H", "MetalEmbedNv4",
    // the nv4 dense tensor twins the NVFP4 llama row dispatches only under the `kq_mulmm_nv4` crown (full keys: the T stem is a
    // prefix of every other twin's); their coverage is kq_mulmm_gate(fmt 49) in test_metal_gemm_kernels.das
    "MetalKqMulMmNv4T_metal_kq_mulmm_iq4xs_t", "MetalKqMulMmNv4TH_metal_kq_mulmm_iq4xs_t",
    // the dev-W dequant kernels of the split-scale and iquant formats: the image mint's dev-W bake
    // runs one per format, and no stocked census carrier holds a plane in any of them (same zoo
    // quants as above). Their coverage is kq_devw_gate in test_metal_gemm_kernels.das, whose
    // kq_dq_kernel picks the stamp per format
    "MetalKqDequantIq4xs", "MetalKqDequantIq4nl", "MetalKqDequantK3",
    "MetalKqDequantIq3s", "MetalKqDequantIq3xxs", "MetalKqDequantK2", "MetalKqDequantIq2s",
    "MetalKqDequantIq2xs", "MetalKqDequantIq2xxs", "MetalKqDequantIq3s4",
    // the tall and double-buffered dense twins of every split-scale and per-32 format, and the per-32 formats' dense tensor
    // twins and dev-W passes: no stocked carrier holds a dense plane in these formats at a DRAM-class panel (the gemma4moe
    // row reaches q40's under its crown). Their coverage is kq_mulmm_gate (tall) and kq_dbuf_gate (the 128- and 32-row
    // pairs) in test_metal_gemm_kernels.das, and b32_dense_twins in test_metal_prefill_kernels.das
    "MetalKqMulMmIq4xsTH128", "MetalKqMulMmIq3s4TH128", "MetalKqMulMmIq4nlTH128", "MetalKqMulMmQ40TH128", "MetalKqMulMmNv4TH128", "MetalKqMulMmK3TH128", "MetalKqMulMmK2TH128", "MetalKqMulMmIq2sTH128", "MetalKqMulMmIq2xsTH128", "MetalKqMulMmIq2xxsTH128",
    "MetalKqMulMmIq4xsTHDb", "MetalKqMulMmIq3s4THDb", "MetalKqMulMmIq4nlTHDb", "MetalKqMulMmQ40THDb", "MetalKqMulMmNv4THDb", "MetalKqMulMmK3THDb", "MetalKqMulMmK2THDb", "MetalKqMulMmIq2sTHDb", "MetalKqMulMmIq2xsTHDb", "MetalKqMulMmIq2xxsTHDb", "MetalKqMulMmIq3sTHDb", "MetalKqMulMmIq3xxsTHDb",
    "MetalKqMulMmIq4xsTHDb128", "MetalKqMulMmIq3s4THDb128", "MetalKqMulMmIq4nlTHDb128", "MetalKqMulMmQ40THDb128", "MetalKqMulMmNv4THDb128", "MetalKqMulMmK3THDb128", "MetalKqMulMmK2THDb128", "MetalKqMulMmIq2sTHDb128", "MetalKqMulMmIq2xsTHDb128", "MetalKqMulMmIq2xxsTHDb128", "MetalKqMulMmIq3sTHDb128", "MetalKqMulMmIq3xxsTHDb128",
    "MetalKqMulMmQ51T", "MetalKqMulMmQ51TH", "MetalKqMulMmQ51TH128", "MetalKqMulMmQ51THDb", "MetalKqMulMmQ51THDb128", "MetalKqDequantQ51",
    "MetalKqMulMmIq4nl32T", "MetalKqMulMmIq4nl32TH", "MetalKqMulMmIq4nl32TH128", "MetalKqMulMmIq4nl32THDb", "MetalKqMulMmIq4nl32THDb128", "MetalKqDequantIq4nl32",
    "MetalKqMulMmQ4032T", "MetalKqMulMmQ4032TH", "MetalKqMulMmQ4032TH128", "MetalKqMulMmQ4032THDb", "MetalKqMulMmQ4032THDb128",
    "MetalKqMulMmQ5032T", "MetalKqMulMmQ5032TH", "MetalKqMulMmQ5032TH128", "MetalKqMulMmQ5032THDb", "MetalKqMulMmQ5032THDb128", "MetalKqDequantQ5032",
    // the short window's split-K pair (ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k) of the split-scale and grid formats, and the
    // f32-X stamp of k4 and k6: the zoo's small windows hold no dense plane in these formats, and its Q4_K_M rows reach the f32-X
    // split sites on k5 planes alone (the attention output and the down sites of the k4/k6 layers carry the half twin). The
    // per-32 formats' pairs fall under their T / TH prefixes above. The ladder files (fmt-ladder/Qwen3-Embedding-0.6B-<T>-pure-local,
    // the stocked embedding queries) dispatch every one; their coverage is kq_split_gate in test_metal_gemm_kernels.das and
    // b32_mm_split_gate in test_metal_prefill_kernels.das
    "MetalKqMulMmIq4xsTHSk_metal_kq_mulmm_db", "MetalKqMulMmIq4xsTSk_metal_kq_mulmm_db", "MetalKqMulMmIq3s4THSk_metal_kq_mulmm_db",
    "MetalKqMulMmIq3s4TSk_metal_kq_mulmm_db", "MetalKqMulMmIq4nlTHSk_metal_kq_mulmm_db", "MetalKqMulMmIq4nlTSk_metal_kq_mulmm_db",
    "MetalKqMulMmQ40THSk_metal_kq_mulmm_db", "MetalKqMulMmQ40TSk_metal_kq_mulmm_db", "MetalKqMulMmK3THSk_metal_kq_mulmm_db",
    "MetalKqMulMmK3TSk_metal_kq_mulmm_db", "MetalKqMulMmK2THSk_metal_kq_mulmm_db", "MetalKqMulMmK2TSk_metal_kq_mulmm_db",
    "MetalKqMulMmIq3sTHSk_metal_kq_mulmm_db", "MetalKqMulMmIq3sTSk_metal_kq_mulmm_db", "MetalKqMulMmIq3xxsTHSk_metal_kq_mulmm_db",
    "MetalKqMulMmIq3xxsTSk_metal_kq_mulmm_db", "MetalKqMulMmIq2sTHSk_metal_kq_mulmm_db", "MetalKqMulMmIq2sTSk_metal_kq_mulmm_db",
    "MetalKqMulMmIq2xsTHSk_metal_kq_mulmm_db", "MetalKqMulMmIq2xsTSk_metal_kq_mulmm_db", "MetalKqMulMmIq2xxsTHSk_metal_kq_mulmm_db",
    "MetalKqMulMmIq2xxsTSk_metal_kq_mulmm_db", "MetalKqMulMmK4TSk_metal_kq_mulmm_db", "MetalKqMulMmK6TSk_metal_kq_mulmm_db",
    // the short window's 16-row split stamps (ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k), every format and the q8 blob: they
    // serve a window of 5 to 16 rows, and every census row prefills past that. The stocked kq-tail cells
    // (test_metal_prefill_kq_tail: Llama-3.2-1B Q4_K_M / IQ4_XS / Q5_1, Qwen3-4B Q4_K_M, Qwen2.5-0.5B IQ4_NL at 5 and 8 rows) dispatch the
    // K-quant, grid and per-32 stamps they carry; their coverage is q8_split_gate and kq_split_gate at 16 rows in
    // test_metal_gemm_kernels.das and b32_mm_split_gate in test_metal_prefill_kernels.das
    "MetalKqMulMmK4THSk16_metal_kq_mulmm_db", "MetalKqMulMmK4TSk16_metal_kq_mulmm_db", "MetalKqMulMmK5THSk16_metal_kq_mulmm_db",
    "MetalKqMulMmK5TSk16_metal_kq_mulmm_db", "MetalKqMulMmK6THSk16_metal_kq_mulmm_db", "MetalKqMulMmK6TSk16_metal_kq_mulmm_db",
    "MetalKqMulMmIq4xsTHSk16_metal_kq_mulmm_db", "MetalKqMulMmIq4xsTSk16_metal_kq_mulmm_db", "MetalKqMulMmIq3s4THSk16_metal_kq_mulmm_db",
    "MetalKqMulMmIq3s4TSk16_metal_kq_mulmm_db", "MetalKqMulMmIq4nlTHSk16_metal_kq_mulmm_db", "MetalKqMulMmIq4nlTSk16_metal_kq_mulmm_db",
    "MetalKqMulMmQ40THSk16_metal_kq_mulmm_db", "MetalKqMulMmQ40TSk16_metal_kq_mulmm_db", "MetalKqMulMmNv4THSk16_metal_kq_mulmm_db",
    "MetalKqMulMmNv4TSk16_metal_kq_mulmm_db", "MetalKqMulMmK3THSk16_metal_kq_mulmm_db", "MetalKqMulMmK3TSk16_metal_kq_mulmm_db",
    "MetalKqMulMmK2THSk16_metal_kq_mulmm_db", "MetalKqMulMmK2TSk16_metal_kq_mulmm_db", "MetalKqMulMmIq3sTHSk16_metal_kq_mulmm_db",
    "MetalKqMulMmIq3sTSk16_metal_kq_mulmm_db", "MetalKqMulMmIq3xxsTHSk16_metal_kq_mulmm_db", "MetalKqMulMmIq3xxsTSk16_metal_kq_mulmm_db",
    "MetalKqMulMmIq2sTHSk16_metal_kq_mulmm_db", "MetalKqMulMmIq2sTSk16_metal_kq_mulmm_db", "MetalKqMulMmIq2xsTHSk16_metal_kq_mulmm_db",
    "MetalKqMulMmIq2xsTSk16_metal_kq_mulmm_db", "MetalKqMulMmIq2xxsTHSk16_metal_kq_mulmm_db", "MetalKqMulMmIq2xxsTSk16_metal_kq_mulmm_db",
    "MetalKqMulMmQ51THSk16_metal_kq_mulmm_db", "MetalKqMulMmQ51TSk16_metal_kq_mulmm_db", "MetalKqMulMmIq4nl32THSk16_metal_kq_mulmm_db",
    "MetalKqMulMmIq4nl32TSk16_metal_kq_mulmm_db", "MetalKqMulMmQ4032THSk16_metal_kq_mulmm_db", "MetalKqMulMmQ4032TSk16_metal_kq_mulmm_db",
    "MetalKqMulMmQ5032THSk16_metal_kq_mulmm_db", "MetalKqMulMmQ5032TSk16_metal_kq_mulmm_db", "MetalQ8MulMmTHSk16_metal_q8_mulmm_t",
    "MetalQ8MulMmTSk16_metal_q8_mulmm_t",
    // the simdgroup base tiles' split-K stamps (ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k), the q8 blob and every format:
    // they serve a box whose dense race crowned no tensor twin (the M1), and this box's crowns take every census site; their
    // coverage is base_split_gate in test_metal_gemm_kernels.das and b32_mm_base_split_gate in test_metal_prefill_kernels.das
    "MetalQ8MulMmSk_metal_q8_mulmm", "MetalKqMulMmK4Sk_metal_kq_mulmm", "MetalKqMulMmK5Sk_metal_kq_mulmm", "MetalKqMulMmK6Sk_metal_kq_mulmm",
    "MetalKqMulMmIq4xsSk_metal_kq_mulmm", "MetalKqMulMmIq3s4Sk_metal_kq_mulmm", "MetalKqMulMmK3Sk_metal_kq_mulmm", "MetalKqMulMmIq3sSk_metal_kq_mulmm",
    "MetalKqMulMmIq3xxsSk_metal_kq_mulmm", "MetalKqMulMmK2Sk_metal_kq_mulmm", "MetalKqMulMmIq2sSk_metal_kq_mulmm", "MetalKqMulMmIq2xsSk_metal_kq_mulmm",
    "MetalKqMulMmIq2xxsSk_metal_kq_mulmm", "MetalKqMulMmIq4nlSk_metal_kq_mulmm", "MetalKqMulMmQ40Sk_metal_kq_mulmm", "MetalKqMulMmNv4Sk_metal_kq_mulmm",
    "MetalKqMulMmQ51Sk_metal_kq_mulmm", "MetalKqMulMmIq4nl32Sk_metal_kq_mulmm", "MetalKqMulMmQ4032Sk_metal_kq_mulmm", "MetalKqMulMmQ5032Sk_metal_kq_mulmm",
    // the split-format MoE expert mul_mm stamps, all four forms (f32-X base, half-X twin, and the
    // tall pair): no stocked MoE carrier holds an expert plane in these formats - the zoo's MoE
    // carriers put Q4_K_M (k4/k6), Q8_0 and mxfp4 expert planes on the schedule. The half-X twin
    // and the tall pair are covered by moe_mulmm_split_gate in test_metal_gemm_kernels.das; the
    // f32-X base of every one of these formats is NOT - moe_split_twin has no MoeSplitForm.t arm,
    // and the only path to that stamp is a served routed model whose prefill minted no half panel
    // (pf_enc_moe_mm's last arm). followup_metal.md sec.14 owes that arm
    "MetalMoeMulMmK3T", "MetalMoeMulMmK3TH", "MetalMoeMulMmK3TH128", "MetalMoeMulMmK3THR",
    "MetalMoeMulMmIq4xsT", "MetalMoeMulMmIq4xsTH", "MetalMoeMulMmIq4xsTH128", "MetalMoeMulMmIq4xsTHR",
    "MetalMoeMulMmNv4T", "MetalMoeMulMmNv4TH", "MetalMoeMulMmNv4TH128", "MetalMoeMulMmNv4THR",
    "MetalMoeMulMmIq3sT", "MetalMoeMulMmIq3sTH", "MetalMoeMulMmIq3sTH128", "MetalMoeMulMmIq3sTHR",
    "MetalMoeMulMmIq3xxsT", "MetalMoeMulMmIq3xxsTH", "MetalMoeMulMmIq3xxsTH128", "MetalMoeMulMmIq3xxsTHR",
    "MetalMoeMulMmIq2sT", "MetalMoeMulMmIq2sTH", "MetalMoeMulMmIq2sTH128", "MetalMoeMulMmIq2sTHR",
    "MetalMoeMulMmIq2xsT", "MetalMoeMulMmIq2xsTH", "MetalMoeMulMmIq2xsTH128", "MetalMoeMulMmIq2xsTHR",
    "MetalMoeMulMmIq2xxsT", "MetalMoeMulMmIq2xxsTH", "MetalMoeMulMmIq2xxsTH128", "MetalMoeMulMmIq2xxsTHR",
    // the q40 expert twins the gemma4moe row cannot reach: the tall pair engages at
    // npos * n_expert_used >= 128 * n_expert = 16384 rows, and its coverage is
    // moe_mulmm_split_gate(fmt 40) in test_metal_gemm_kernels.das. The f32-X base serves only
    // where the prefill minted no half panel (pf_enc_moe_mm's last arm), which no served routed
    // model on this box takes, and moe_split_twin has no MoeSplitForm.t arm for any format - so
    // the T stamp has NO model-less cell, in q40 or in the seven formats above. followup_metal.md
    // sec.14 owes that arm
    "MetalMoeMulMmQ40T_metal_moe_mulmm_iq4xs_t",
    // the expert-indexed GEMVs of the same formats, on the same expert planes nothing stocks.
    // Their coverage is moe_kq_gemv_gate in test_metal_gemv_kernels.das
    "metal_moe_gemv_k3", "metal_moe_gemv_iq4xs", "metal_moe_gemv_iq3s", "metal_moe_gemv_iq3xxs",
    "metal_moe_gemv_iq2s", "metal_moe_gemv_iq2xs", "metal_moe_gemv_iq2xxs", "metal_moe_gemv_nv4",
    // the Q2_K and superblock IQ4_NL routed forms - the gathered GEMV, the simdgroup base mul_mm and the four tensor
    // twins: no stocked MoE carrier holds a k2 or iq4nl expert plane. Their coverage is moe_kq_gemv_gate (fmt 2 / 45) in
    // test_metal_gemv_kernels.das and moe_mulmm_kq_gate / moe_mulmm_split_gate in test_metal_gemm_kernels.das
    "metal_moe_gemv_k2", "metal_moe_gemv_iq4nl", "MetalMoeMulMmK2T", "MetalMoeMulMmIq4nlT",
    // the simdgroup base mul_mm of the split formats - the routed site's form on a box without the tensor toolchain, or
    // where the window took no gather panel: the same expert planes nothing stocks. Their coverage is moe_mulmm_kq_gate
    // in test_metal_gemm_kernels.das
    "metal_moe_mulmm_k3", "metal_moe_mulmm_k2", "metal_moe_mulmm_iq4xs", "metal_moe_mulmm_iq4nl", "metal_moe_mulmm_q40", "metal_moe_mulmm_nv4",
    "metal_moe_mulmm_iq3s", "metal_moe_mulmm_iq3xxs", "metal_moe_mulmm_iq2s", "metal_moe_mulmm_iq2xs", "metal_moe_mulmm_iq2xxs",
    // the dense per-32 forms (q51, iq4nl32): a dense Q5_1 or off-lattice IQ4_NL plane loads in its own form
    // (`kq_fmt_row_ok`), and no stocked census carrier holds one - the census-q51 / census-iq4nl32 arms mint
    // theirs locally. Their coverage is b32_gemv_gate and b32_mm_gate in test_metal_prefill_kernels.das
    "metal_kq_gemv_q51", "metal_kq_gemv_iq4nl32", "MetalKqMulMmQ51", "MetalKqMulMmIq4nl32",
    // the 16-deep router select stamp: it serves past 256 experts, and the stocked MoE carriers top
    // out at 128 (Qwen3-30B-A3B), 60 (Qwen1.5-MoE) and 32 (gpt-oss-20b). Its coverage is
    // moe_select_gate's ne=512 and ne=300 cases in test_metal_prefill_kernels.das
    "MetalMoeSelect512",
    // its down-scale twin: a gemma-4 MoE past 256 experts, which no file carries (the 26B routes 128). Its coverage is
    // moe_select_gate's ne=300 down-scale case in test_metal_prefill_kernels.das
    "MetalMoeSelect512Ds",
    // the unconverted geglu arm of the bound-total MoE activation: the gemma4moe row picks the
    // geglu family on moe_dense_shexp, and its panel always clears CVT_MIN_ROWS, so the HX twin
    // serves and this one never does. Its coverage is ew2_hx_gate(geglu = true, bounded = true) in
    // test_metal_misc_kernels.das
    "MetalGegluB",
    // the hyper-connection mixer, its scatter, the n-gram side input and the σ(z) deltanet gate
    // (qwen4exp): the one carrier is Qwen3.8-Flash-Next (94 GB, the large tier), which no census
    // row loads. Their coverage is the hc_*/ple_* gates in test_metal_misc_kernels.das and the
    // sigmoid leg of dn_chain_gate in test_metal_prefill_kernels.das
    "metal_hc_norm", "MetalHcLo", "metal_hc_init", "metal_hc_mix", "metal_hc_combine", "metal_ple_gate", "metal_ple_conv",
    "MetalDnGateSig",
    // the device attention pair's 128-row stamps: they serve a window whose padded row count is a
    // multiple of 128, and the census prompt pads to 608 rows, which the 32-row stamps take. Their
    // coverage is attn_qk_mm_gate's pair arms and attn_dev_pair_gate in test_metal_gemm_kernels.das and the
    // attn-dev arm's 384-row continuation in test_metal_prefill_parity.das
    "MetalAttnQKDev128", "MetalAttnAVDev128",
    // the f32 rows' element-walk GEMV pair: it serves an f32 row set whose dim, base or column stride sits off the float4
    // lattice (an E-series pre-step projection or per-layer plane in f32 rows at such a width), and every stocked carrier's
    // widths step whole float4s, so no census row reaches it. Its coverage is f32_rows_gate's off-lattice arms in
    // test_metal_prefill_kernels.das
    "metal_f32_rows_one", "metal_f32_rows_b",
    // the E-series projection's bf16 halfword walk: it serves bf16 projection rows whose model dim or base is odd, and every
    // stocked E-series carrier's dim is even on an even base. Its coverage is ple_proj_bf16_gate's odd arms in
    // test_metal_prefill_kernels.das
    "metal_ple_proj_bf16_hw_one", "metal_ple_proj_bf16_hw",
    // gigaam's depthwise conv over f32 taps: it serves a carrier whose depthwise taps hold no f16, and the census's gigaam
    // carrier (gigaam-v3-e2e-rnnt-f16.gguf) holds them in f16, which `metal_ga_hdw` serves. Its coverage is ga_dw_gate in
    // test_metal_prefill_kernels.das
    "metal_ga_dw"
]

[unused_argument(t, path, family, kdts, nbs)]   // off-Apple the gated body is empty
def private cov_model(t : T?; path, family : string; kdts : array<KVDtype>; nbs : array<int>) {
    // one model's coverage sweep: per KV codec — GPU prefill (the ~120-id counting prompt
    // clears the npos gate), two greedy decode steps, then one batched step per requested B.
    // No parity asserts here — the census wants DISPATCH DIVERSITY, the parity suites own truth.
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (!family_on(t, family) || !arm_on(t, "coverage") || !model_available(t, path)) {
            return
        }
        var tr <- load_model_(path, QuantMode.q8)
        if (!convert_model_to_metal_blob(tr)) {
            t |> failure("{base_name(path)}: metal-blob transform failed")
            delete tr
            return
        }
        // 1024, not 512: the digit-splitting tokenizers (gemma, qwen) encode the 140-number
        // counting prompt to ~590 tokens, and the eval capacity tripwire panics past seq_len
        tr.config.seq_len = min(tr.config.seq_len, 1024l)
        let vocab = tr.config.vocab_size
        select_prefill_override("metal")
        select_decode_override("metal")
        var batch_sessions : array<Session>       //! the batch's sessions, their handles and their picks, reused across the shapes
        var sess : array<Session?>    //! borrowed handles into batch_sessions: cleared before batch_sessions goes, never deleted whole
        var curs : array<int64>
        with_dasllama_jobque() {
            for (kdt in kdts) {
                var s = make_run_state(tr.config, kdt, kdt)
                // >= 256 rows: the half-X convert (CVT_MIN_ROWS), the tall M-tile stamps and
                // the MoE gather engage only past their row floors — a short prompt leaves
                // every one of those kernels undispatched and the census blind to them
                var inscope p <- encode(tr, counting_prompt(1, 140), true, false)
                eval_(tr, s, p)
                var cur = argmax_of(s.logits, vocab)
                for (_i in range(2)) {
                    var inscope one <- [cur]
                    eval_(tr, s, one)
                    cur = argmax_of(s.logits, vocab)
                }
                // continuation prefill (start_pos > 0) — the pf window-landing utilities only
                // serve when a second window lands on an already-warm cache
                var inscope p2 <- encode(tr, counting_prompt(50, 90), false, false)
                eval_(tr, s, p2)
                delete s
                for (nb in nbs) {
                    batch_sessions |> reserve(nb)
                    for (i in range(nb)) {
                        batch_sessions |> emplace <| make_run_state(tr.config, kdt, kdt)
                        var inscope pb <- encode(tr, counting_prompt(3 + i, 44 + i), true, false)
                        eval_(tr, batch_sessions[i], pb)
                        sess |> push(unsafe(addr(batch_sessions[i])))
                        curs |> push(argmax_of(batch_sessions[i].logits, vocab))
                    }
                    var ws <- create_batch_workspace_(tr)
                    eval_batch_(tr, ws, sess, curs)
                    sess |> clear()   // borrowed handles — delete of array<T?> cascades to the pointees
                    curs |> clear()
                    delete ws
                    delete batch_sessions
                }
            }
        }
        unsafe {
            delete sess
        }
        delete curs
        select_prefill_override("")
        select_decode_override("")
        delete tr
    }
}

def private census_zoo(t : T?) {
    // llama: the dim-ladder base — all three KV codecs, the B=2/4/8 batch shapes
    cov_model(t, path_join(models_dir(), "Llama-3.2-1B-Instruct-Q8_0.gguf"), "llama",
        [KVDtype.f16, KVDtype.q8_0, KVDtype.tq4], [2, 4, 8])
    // the s16 axis (f16 weight-scale plane) on the same carrier
    set_wscale_f16(true)
    cov_model(t, path_join(models_dir(), "Llama-3.2-1B-Instruct-Q8_0.gguf"), "llama",
        [KVDtype.f16], [2])
    set_wscale_f16(false)
    // the q5_K tail on the same carrier — Q5_K_M rows drive the k5 GEMV/mulmm/batch tiles
    cov_model(t, path_join(models_dir(), "Llama-3.2-1B-Instruct-Q5_K_M.gguf"), "llama",
        [KVDtype.f16], [2, 4, 8])
    // the NVFP4 mint of the same carrier (every dense plane nv4 under a Q6_K table): the nv4 GEMV, its small-batch forms
    // and its dense mul_mm under the box's crown
    cov_model(t, path_join(models_dir(), "Llama-3.2-1B-Instruct-NVFP4-local.gguf"), "llama",
        [KVDtype.f16], [2, 4, 8])
    // gemma2: K-quant weights x softcap/sliding/geglu/wide heads; B=4/8 drive the
    // k4/k6 batch tiles; the three codecs reach the head-of-256 fused attention's three stamps
    cov_model(t, path_join(models_dir(), "gemma-2-2b-it-Q4_K_M.gguf"), "gemma2",
        [KVDtype.f16, KVDtype.q8_0, KVDtype.tq4], [2, 4, 8])
    //! phi3: a head of 96 (the fused attention's narrow stamps, the device pair's AV tail) and an untied K-quant embedding (the greedy chain's fp32 gather)
    cov_model(t, path_join(models_dir(), "Phi-3.5-mini-instruct-Q4_K_M.gguf"), "phi3",
        [KVDtype.f16, KVDtype.q8_0], [2])
    // qwen3: qk_norm + qd != dim
    cov_model(t, path_join(models_dir(), "Qwen3-0.6B-Q8_0.gguf"), "qwen3",
        [KVDtype.f16, KVDtype.tq4], [2])
    // qwen2: qkv_bias
    cov_model(t, path_join(models_dir(), "Qwen2.5-0.5B-Instruct-Q8_0.gguf"), "qwen2",
        [KVDtype.f16, KVDtype.q8_0], [2])
    // qwen3.5: the DeltaNet hybrid (batch = per-row fallback by design — still coverage)
    cov_model(t, path_join(models_dir(), "Qwen3.5-0.8B-Q8_0.gguf"), "qwen35",
        [KVDtype.f16], [2])
    // gemma4e: PLE + V-from-K (no PLE batch arm — a blob-twin CPU batch fallback
    // would trip the blob-only panic, so no batch shapes here); large tier; the three codecs
    // reach the head-of-512 fused attention's three stamps on the full-context layers
    let none : array<int>
    cov_model(t, path_join(models_dir(), "gemma-4-E4B-it-Q8_0.gguf"), "gemma4e",
        [KVDtype.f16, KVDtype.q8_0, KVDtype.tq4], none)
    // gemma4: the checkpoint suppress list (suppress_tokens metadata rides the 12B
    // family, NOT the E-series) + K-quant at 12B shapes; large tier
    cov_model(t, path_join(models_dir(), "gemma-4-12B-it-Q4_K_M.gguf"), "gemma4",
        [KVDtype.f16], [2])
    // qwen3moe: the whole-graph GPU MoE family (router/bucket/select/combine +
    // expert GEMV/GEMMs); no batch shapes — MoE batch declines are graph-level and a
    // blob-twin CPU batch fallback would trip the blob-only panic; large tier
    cov_model(t, path_join(models_dir(), "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf"), "qwen3moe",
        [KVDtype.f16], none)
    // gptoss: the MXFP4 expert stacks + swiglu_oai + attention sinks — the only carrier for the
    // mx4 expert kernels; no batch shapes for the same reason as the other MoE rows; large tier
    cov_model(t, path_join(models_dir(), "gpt-oss-20b-mxfp4.gguf"), "gptoss",
        [KVDtype.f16], none)
    // qwen2moe: q8 expert stacks + the sigmoid-gated shared expert on std attention; large tier
    cov_model(t, path_join(models_dir(), "Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf"), "qwen2moe",
        [KVDtype.f16], none)
    // gemma4moe: the only carrier of a Q4_0 routed expert plane - the gemma-4 dense-shared MoE
    // whose fused gate_up stacks are Q4_0 (the 704-wide down stacks serve as q4032, off the 256
    // lattice). No batch shapes, same reason as the other MoE rows; large tier
    cov_model(t, path_join(models_dir(), "gemma-4-26B-A4B-it-Q4_0.gguf"), "gemma4moe",
        [KVDtype.f16], none)
    cov_tower(t)
}

// the gemma3v row of cov_tower: the fixed 896 canvas through the gemma-3-4b SigLIP tower on
// the f32 planes, BOTH attention routes — the 4096-token %64 canvas serves the tower flash
// (flash + kv_hc), then a flash-off encode drives the slab route (head_restride + padded QK/AV)
[unused_argument(t)]   // off-Apple the gated body is empty
def private cov_tower_gemma3v(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let vpath = path_join(models_dir(), "mmproj-gemma-3-4b-it-F16.gguf")
        if (family_on(t, "gemma3v") && model_available(t, vpath)) {
            set_gemma3v_q8(false)   // the driver serves f32 planes (the lane policy picks this with the tower on; pinned for the census)
            var inscope g3stg <- stage_gemma3v_tower(vpath)   // stage+mint: no .dlim written, no sibling-lane GC
            var inscope tw = Gemma3vTower()
            mint_gemma3v_tower(g3stg, tw)
            reset_gemma3v_q8()
            var st = Gemma3vState()
            var planes : array<float>
            planes |> reserve_resize(896l * 896l * 3l)
            for (i in range64(896l * 896l * 3l)) {
                planes[i] = sin(float(i) * 0.017) * 0.5 + 0.5
            }
            var out : array<float>
            let prev_flash = get_metal_tower_flash()
            with_dasllama_jobque() {
                gemma3v_encode(tw, st, planes, 896l, 896l, out)
                set_metal_tower_flash(false)
                var out2 : array<float>
                gemma3v_encode(tw, st, planes, 896l, 896l, out2)
                set_metal_tower_flash(prev_flash)
                delete out2
            }
            delete out
            delete st
            // the crowned twin-W leg: a fresh load under pinned crowns bakes the halfword
            // twin, so the f16-route kernels (cvt_half + the half GEMMs) dispatch - the PIN
            // form, since the load re-applies the box profile's crowns over a bare setter
            let prev_crowns = get_metal_tensor_crowns()
            let prev_tower_f16 = get_metal_tower_f16()
            pin_metal_tensor_crowns("mulmm_q8")
            metal_prefill_shutdown()
            set_metal_tower_f16(true)
            set_gemma3v_q8(false)   // the exact lane is the one that bakes the twin
            var inscope g3stg2 <- stage_gemma3v_tower(vpath)   // in-memory: the crowned bake must not purge the serving flavor
            var twt = Gemma3vTower()
            mint_gemma3v_tower(g3stg2, twt)
            reset_gemma3v_q8()
            var stt = Gemma3vState()
            var outt : array<float>
            with_dasllama_jobque() {
                gemma3v_encode(twt, stt, planes, 896l, 896l, outt)
            }
            unpin_metal_tensor_crowns()
            set_metal_tensor_crowns(prev_crowns)
            set_metal_tower_f16(prev_tower_f16)
            metal_prefill_shutdown()
            delete outt
            delete stt
            delete twt
            delete planes
        }
    }
}

// the qwen25v row of cov_tower: one 112x112 window (16 tokens) through the Qwen2.5-Omni
// window ViT — the per-window attention kernel + the row-gather stem sort
[unused_argument(t)]   // off-Apple the gated body is empty
def private cov_tower_qwen25v(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let vpath = path_join(models_dir(), "mmproj-Qwen2.5-Omni-3B-f16.gguf")
        if (family_on(t, "qwen25v") && model_available(t, vpath)) {
            var inscope q25stg <- stage_qwen25v_tower(vpath)   // stage+mint: no .dlim written, no sibling-lane GC
            var tw = Qwen25vTower()
            mint_qwen25v_tower(q25stg, tw)
            var st = Qwen25vState()
            var planes : array<float>
            planes |> reserve_resize(112l * 112l * 3l)
            for (i in range64(112l * 112l * 3l)) {
                planes[i] = sin(float(i) * 0.019) * 0.5 + 0.5
            }
            var out : array<float>
            with_dasllama_jobque() {
                qwen25v_encode(tw, st, planes, 112l, 112l, out)
            }
            delete out
            delete planes
            delete st
            delete tw
        }
    }
}

// the qwen3v row of cov_tower: a 96x96 canvas through the Omni-family merger tower — the
// fused GPU stem (patch GEMM + merge-walk assemble) ahead of the block slab
[unused_argument(t)]   // off-Apple the gated body is empty
def private cov_tower_qwen3v(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let vpath = path_join(models_dir(), "mmproj-Qwen3VL-4B-Instruct-F16.gguf")
        if (family_on(t, "qwen3v") && model_available(t, vpath)) {
            set_qwen3v_q8(false)   // pinned for the census — the box's lane policy must not pick the row's kernel set
            var inscope q3stg <- stage_qwen3v_tower(vpath)   // stage+mint: no .dlim written, no sibling-lane GC
            var tw = Qwen3vTower()
            mint_qwen3v_tower(q3stg, tw)
            reset_qwen3v_q8()
            var st = Qwen3vState()
            var planes : array<float>
            planes |> reserve_resize(96l * 96l * 3l)
            for (i in range64(96l * 96l * 3l)) {
                planes[i] = sin(float(i) * 0.023) * 0.5 + 0.5
            }
            var out : array<float>
            with_dasllama_jobque() {
                qwen3v_encode(tw, st, planes, 96l, 96l, out)
            }
            delete out
            delete planes
            delete st
            delete tw
        }
    }
}

// the gemma4a row of cov_tower: one whole chunk from raw samples — GPU mel (twiddle-T DFT
// GEMM + mag + filter + log), both subsample convs, and the 12-block Conformer chain
[unused_argument(t, samples)]   // off-Apple the gated body is empty
def private cov_tower_gemma4a(t : T?; samples : array<float>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let gpath = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
        if (family_on(t, "gemma4a") && model_available(t, gpath) && !empty(samples)) {
            set_gemma4a_q8(false)   // pinned for the census — the box's lane policy must not pick the row's kernel set
            var inscope gstg <- stage_gemma4a_encoder(gpath)   // stage+mint: no .dlim written, no sibling-lane GC
            var inscope genc = Gemma4aEncoder()
            mint_gemma4a_encoder(gstg, genc)
            reset_gemma4a_q8()
            var gst = Gemma4aState()
            var grows : array<float>
            with_dasllama_jobque() {
                gemma4a_encode_all(genc, gst, samples, grows)
            }
            delete grows
            delete gst
        }
    }
}

// the canary row of cov_tower: CPU mel, then the 32-block Metal FastConformer chain — the
// bidirectional rel-pos XL attention and the centered-k9 depthwise kernels
[unused_argument(t, samples)]   // off-Apple the gated body is empty
def private cov_tower_canary(t : T?; samples : array<float>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let cnpath = path_join(models_dir(), "canary-qwen-2.5b-encoder-f32.bin")
        if (family_on(t, "canary") && model_available(t, cnpath) && !empty(samples)) {
            var inscope cstg <- stage_canary_encoder(cnpath)   // stage+mint: no .dlim written, no sibling-lane GC
            var inscope cenc = CanaryEncoder()
            mint_canary_encoder(cstg, cenc)
            var cst = CanaryState()
            with_dasllama_jobque() {
                canary_log_mel(cenc, samples, cst)
                canary_encode(cenc, cst)
            }
            delete cst
        }
    }
}

//! reaches the tap-major depthwise stamp (enc_pk_dw), which the canary row does not
[unused_argument(t, samples)]   // off-Apple the gated body is empty
def private cov_tower_parakeet(t : T?; samples : array<float>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let pkpath = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-v2-f32.bin")
        if (family_on(t, "parakeet") && model_available(t, pkpath) && !empty(samples)) {
            var inscope pstg <- stage_parakeet_model(pkpath, false)
            var inscope pm = ParakeetModel()
            mint_parakeet_model(pstg, pm, false)
            var pst = ParakeetState()
            with_dasllama_jobque() {
                parakeet_log_mel(samples, pm, pst)
                let tt = parakeet_encode(pm, pst)
                parakeet_tdt_decode(pm, pst, tt)   //! the transducer decode's four stamps off the encode's device rows
            }
            delete pst
        }
    }
}

//! the GigaAM rotary Conformer's whole encode off the raw samples on the carrier's f16 planes (the exact lane the GPU hook
//! serves): the device mel, the conv front and the blocks
[unused_argument(t, samples)]   // off-Apple the gated body is empty
def private cov_tower_gigaam(t : T?; samples : array<float>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let gpath = path_join(models_dir(), "gigaam-v3-e2e-rnnt-f16.gguf")
        if (family_on(t, "gigaam") && model_available(t, gpath) && !empty(samples)) {
            var inscope gstg <- stage_gigaam_model(gpath, false)
            var inscope gm = GigaamModel()
            mint_gigaam_model(gstg, gm, false)
            var gst = GigaamState()
            with_dasllama_jobque() {
                let tt = gigaam_encode_span(samples, 0l, long_length(samples), gm, gst)
                gigaam_decode(gm, gst, tt)   //! the transducer decode's four stamps off the encode's device rows
            }
            delete gst
        }
    }
}

//! the Bergamot translation row: two Russian sentences through ru-en tiny on the q8 lane, the whole sentence on the device -
//! the embed, SSRU and pick stamps beside the borrowed GEMM, attention and seam kernels
[unused_argument(t)]   // off-Apple the gated body is empty
def private cov_tower_bergamot(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let path = path_join(models_dir(), "bergamot/ru-en-tiny/model.ruen.intgemm.alphas.bin")
        if (family_on(t, "bergamot") && model_available(t, path)) {
            let was = metal_nmt_knob()
            set_metal_nmt(true)
            with_dasllama_jobque() {
                var inscope m = NmtModel()
                var inscope stg <- stage_nmt_model(path, true)
                mint_nmt_model(stg, m)   //! minted in memory: the census row writes no image
                var inscope st = NmtState()
                for (ln in ["Где ближайшая аптека?", "Сегодня мы идём в горы."]) {   // nolint:STYLE039,LINT019 - the source language's own text; the rule fires only on the darwin flavor
                    nmt_translate(m, st, ln)
                }
            }
            set_metal_nmt(was)
        }
    }
}

// the gemma4v row of cov_tower: a 96x96 canvas through the E2B ViT tower on the file's planes
[unused_argument(t)]   // off-Apple the gated body is empty
def private cov_tower_gemma4v(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let vpath = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
        if (model_available(t, vpath)) {
            set_gemma4v_q8(false)   // the driver serves the file's planes (the lane policy picks this with the tower on; pinned for the census)
            var inscope g4stg <- stage_gemma4v_tower(vpath)   // stage+mint: no .dlim written, no sibling-lane GC
            var inscope tw = Gemma4vTower()
            mint_gemma4v_tower(g4stg, tw)
            reset_gemma4v_q8()
            var st = Gemma4vState()
            var planes : array<float>
            planes |> resize(96l * 96l * 3l)
            for (i in range64(96l * 96l * 3l)) {
                planes[i] = sin(float(i) * 0.017) * 0.5 + 0.5
            }
            var out : array<float>
            with_dasllama_jobque() {
                gemma4v_encode(tw, st, planes, 96l, 96l, out)
            }
            delete out
            delete planes
            delete st
        }
    }
}

// cov_tower: the tower-driver kernels (LN / f32 mul_mm / the gelu flavors / posadd / the
// borrowed trio / the gemma4v clamp, rope2d, GEGLU-quick) AND the decoder-side cross-KV kernels
// go non-zero — a whisper tiny MIXED chunk (f32 enc = the tower LUT flavor, q8 dec = the wdec
// driver) plus one row per tower family, each row's own comment naming the kernels it reaches
[unused_argument(t)]   // off-Apple the gated body is empty
def private cov_tower(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let knobs = gpu_knobs()   //! the tower and decoder knobs put back as found at the end
        set_metal_tower(true)
        set_metal_wdec(true)
        set_metal_wdec_step(true)
        set_metal_wdec_step_min_d(0l)   // the census carrier is tiny — under the serving floor
        let wpath = path_join(whisper_dir(), "ggml-tiny.bin")
        var samples <- load_audio_16k_mono(asr_clip("jfk.wav"))
        if (model_available(t, wpath) && !empty(samples)) {
            var inscope w = WhisperModel()
            var inscope stg <- stage_whisper_model(wpath, false, true)
            mint_whisper_model(stg, w, false, true)
            with_dasllama_jobque() {
                var ws <- create_session(w)
                transcribe(w, ws, samples) $(_seg) {
                    pass
                }
                delete ws
            }
        }
        // the erf-GELU carrier (whisper is the LUT flavor): one qwen2audio chunk on varied mel
        let apath = path_join(models_dir(), "qwen2audio-mmproj-f32.gguf")
        if (model_available(t, apath)) {
            var atw <- load_audio_tower(apath)
            var mel = MelChunk(n_mel = atw.n_mel, n_len = AUDIO_CHUNK_FRAMES)
            mel.data |> resize(atw.n_mel * AUDIO_CHUNK_FRAMES)
            for (i in range(length(mel.data))) {
                mel.data[i] = sin(float(i) * 0.0137) * 0.8
            }
            var aenc <- make_encoder_state(atw)
            with_dasllama_jobque() {
                audio_encode(atw, aenc, mel)
            }
            delete aenc
            delete atw
        }
        // the qwen3a conv2d + mel carrier: the mel is built from raw samples so the GPU mel hook
        // (twiddle-T DFT GEMM + power + mel-major log store) dispatches ahead of the im2col2d
        // window and its padded tile GEMMs
        let qpath = path_join(models_dir(), "mmproj-Qwen3-ASR-0.6B-bf16.gguf")
        if (model_available(t, qpath) && !empty(samples)) {
            var inscope qtw <- load_qwen3a_tower(qpath)
            var qenc <- make_encoder_state(qtw.core)
            var qcs = Qwen3aConvState()
            var qrows : array<float>
            with_dasllama_jobque() {
                var qmel <- log_mel_qwen3a(samples, qtw.core.n_mel)
                qwen3a_encode_all(qtw, qenc, qcs, qmel, qrows)
                delete qmel
            }
            delete qrows
            delete qcs
            delete qenc
        }
        cov_tower_gemma4a(t, samples)
        cov_tower_canary(t, samples)
        cov_tower_parakeet(t, samples)
        cov_tower_gigaam(t, samples)
        cov_tower_bergamot(t)
        delete samples
        cov_tower_gemma4uv(t)
        cov_tower_gemma4v(t)
        cov_tower_gemma3v(t)
        cov_tower_qwen25v(t)
        cov_tower_qwen3v(t)
        cov_tower_tail_rows(t)
        set_gpu_knobs(knobs)
    }
}

//! the rows both homes' censuses run past their tower families: the StyleTTS2 synthesis, the Pocket codec and frames, the VAD
def private cov_tower_tail_rows(t : T?) {
    cov_tower_styletts2(t)
    cov_tower_pocket(t)
    cov_tower_vad(t)
}

//! the silero VAD's two kernels: the batch probabilities over five seconds of a synthetic signal (the weights ship in-tree, no
//! carrier needed) and one streaming feed on the device lever; family token `vad`
[unused_argument(t)]   //! without a GPU module the gated body is empty
def private cov_tower_vad(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal) || typeinfo builtin_module_exists(vulkan)) {
        return if (!family_on(t, "vad"))
        var inscope v <- load_vad_model(vad_model_path())
        var sig : array<float>
        sig |> resize(5 * 16000)
        for (i in range(length(sig))) {
            sig[i] = 0.3 * sin(float(i) * 0.05) * sin(float(i) * 0.0007)
        }
        with_dasllama_jobque() {
            var inscope probs <- vad_probs(v, sig)
            let was = set_vad_stream_gpu(true)
            var it : VadIter
            vad_iter_reset(it, default_vad_opts())
            vad_iter_feed(v, it, sig) $(_ev) {
                pass
            }
            set_vad_stream_gpu(was)
        }
        delete sig
    }
}

[unused_argument(t)]   //! without a GPU module the gated body is empty
def private cov_tower_styletts2(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal) || typeinfo builtin_module_exists(vulkan)) {
        for (name in ["kitten-nano.gguf", "kokoro-82m.gguf"]) {   //! the ONNX law's stamps and the torch law's
            let path = path_join(models_dir(), name)
            continue if (!family_on(t, name == "kokoro-82m.gguf" ? "kokoro" : "kitten") || !model_available(t, path))
            var inscope stg <- stage_styletts2(path, false)   // stage+mint: no .dlim written, no sibling-lane GC
            var inscope m = StyleTts2Model()
            mint_styletts2(stg, m)
            var inscope ids : array<int>
            ids |> resize(48)
            for (i in range(48)) {
                ids[i] = 1 + i % (int(m.n_symbols) - 1)
            }
            var inscope style : array<float>
            styletts2_style(m, m.voice_names[0], 0, style)
            var inscope noise = TtsNoise()
            var tm = TtsTimings()
            var inscope sc = St2Scratch()
            with_dasllama_jobque() {
                styletts2_synthesize(m, ids, style, 1.0, noise, 0x5eedul, null, tm, sc)
            }
        }
    }
}

[unused_argument(t)]   //! without a GPU module the gated body is empty
def private cov_tower_pocket(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal) || typeinfo builtin_module_exists(vulkan)) {
        let path = path_join(models_dir(), "pocket-tts-en.gguf")
        if (family_on(t, "pocket") && model_available(t, path)) {
            var inscope m <- load_pocket(path)   //! the Pocket loader stages in memory: no .dlim
            var inscope sc = PocketScratch()
            let frames = 24l
            var inscope lat : array<float>
            lat |> resize(frames * m.latent_dim)
            for (i in range(length(lat))) {
                lat[i] = 0.1 * float(i % 17) - 0.8
            }
            var inscope wave : array<float>
            var tm = TtsTimings()
            with_dasllama_jobque() {
                pocket_decode_latents(m, lat, frames, sc, wave)
                pocket_speak(m, "The census row speaks one short line.", m.default_voice, m.default_temperature, 1ul, tm, sc)
                let kq = path_join(models_dir(), "pocket-tts-en-kq.gguf")   //! the small form: its head's q8 linears take the q8 stamps
                if (model_available(t, kq)) {
                    var inscope mk <- load_pocket(kq)
                    var inscope sck = PocketScratch()
                    pocket_speak(mk, "The small form speaks the same line.", mk.default_voice, mk.default_temperature, 1ul, tm, sck)
                }
            }
        }
    }
}

// the gemma4uv row of cov_tower: a small canvas through the 12B dense embedder chain
[unused_argument(t)]   // off-Apple the gated body is empty
def private cov_tower_gemma4uv(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let mpath = path_join(models_dir(), "mmproj-gemma-4-12B-it-BF16.gguf")
        if (model_available(t, mpath)) {
            var inscope emb <- load_gemma4uv_embedder(mpath)
            var st = Gemma4uvState()
            var planes : array<float>
            planes |> resize(96l * 96l * 3l)
            for (i in range64(96l * 96l * 3l)) {
                planes[i] = sin(float(i) * 0.013) * 0.7
            }
            var out : array<float>
            with_dasllama_jobque() {
                gemma4uv_encode(emb, st, planes, 96l, 96l, out)
            }
            delete out
            delete planes
            delete st
        }
    }
}

//! Blind entries whose stamp compiles only behind a box crown, and the crown that reaches them:
//! where the crown IS pinned a stocked carrier may dispatch the stamp, and where it is NOT the stamp
//! never compiled and the entry matches no census key - the drain assert would red on a correct box
//! either way. `CENSUS_NEVER_DISPATCHED` has no vocabulary for "blind only off this crown", so the
//! pair is named here.
let private CENSUS_CROWN_CONDITIONAL : table<string; string> <- {
    // the dense tensor twins every format stamps behind its own kq_mulmm crown: the tall and double-buffered forms of the
    // split-scale formats, every form of the per-32 formats
    "MetalKqMulMmIq4xsTH128" => "kq_mulmm_iq4xs", "MetalKqMulMmIq4xsTHDb" => "kq_mulmm_iq4xs", "MetalKqMulMmIq4xsTHDb128" => "kq_mulmm_iq4xs",
    "MetalKqMulMmIq3s4TH128" => "kq_mulmm_iq3s4", "MetalKqMulMmIq3s4THDb" => "kq_mulmm_iq3s4", "MetalKqMulMmIq3s4THDb128" => "kq_mulmm_iq3s4",
    "MetalKqMulMmIq4nlTH128" => "kq_mulmm_iq4nl", "MetalKqMulMmIq4nlTHDb" => "kq_mulmm_iq4nl", "MetalKqMulMmIq4nlTHDb128" => "kq_mulmm_iq4nl",
    "MetalKqMulMmQ40TH128" => "kq_mulmm_q40", "MetalKqMulMmQ40THDb" => "kq_mulmm_q40", "MetalKqMulMmQ40THDb128" => "kq_mulmm_q40",
    "MetalKqMulMmNv4TH128" => "kq_mulmm_nv4", "MetalKqMulMmNv4THDb" => "kq_mulmm_nv4", "MetalKqMulMmNv4THDb128" => "kq_mulmm_nv4",
    "MetalKqMulMmNv4T_metal_kq_mulmm_iq4xs_t" => "kq_mulmm_nv4", "MetalKqMulMmNv4TH_metal_kq_mulmm_iq4xs_t" => "kq_mulmm_nv4",
    "MetalKqMvB2Nv4H" => "kq_rows_half_nv4", "MetalKqMvB4Nv4H" => "kq_rows_half_nv4", "MetalKqMvB8Nv4H" => "kq_rows_half_nv4",
    "MetalKqMulMmK3TH128" => "kq_mulmm_k3", "MetalKqMulMmK3THDb" => "kq_mulmm_k3", "MetalKqMulMmK3THDb128" => "kq_mulmm_k3",
    "MetalKqMulMmK2TH128" => "kq_mulmm_k2", "MetalKqMulMmK2THDb" => "kq_mulmm_k2", "MetalKqMulMmK2THDb128" => "kq_mulmm_k2",
    "MetalKqMulMmIq2sTH128" => "kq_mulmm_iq2s", "MetalKqMulMmIq2sTHDb" => "kq_mulmm_iq2s", "MetalKqMulMmIq2sTHDb128" => "kq_mulmm_iq2s",
    "MetalKqMulMmIq2xsTH128" => "kq_mulmm_iq2xs", "MetalKqMulMmIq2xsTHDb" => "kq_mulmm_iq2xs", "MetalKqMulMmIq2xsTHDb128" => "kq_mulmm_iq2xs",
    "MetalKqMulMmIq2xxsTH128" => "kq_mulmm_iq2xxs", "MetalKqMulMmIq2xxsTHDb" => "kq_mulmm_iq2xxs", "MetalKqMulMmIq2xxsTHDb128" => "kq_mulmm_iq2xxs",
    "MetalKqMulMmIq3sTHDb" => "kq_mulmm_iq3s", "MetalKqMulMmIq3sTHDb128" => "kq_mulmm_iq3s",
    "MetalKqMulMmIq3xxsTHDb" => "kq_mulmm_iq3xxs", "MetalKqMulMmIq3xxsTHDb128" => "kq_mulmm_iq3xxs",
    "MetalKqMulMmQ51T" => "kq_mulmm_q51", "MetalKqMulMmQ51TH" => "kq_mulmm_q51", "MetalKqMulMmQ51TH128" => "kq_mulmm_q51", "MetalKqMulMmQ51THDb" => "kq_mulmm_q51", "MetalKqMulMmQ51THDb128" => "kq_mulmm_q51",
    "MetalKqMulMmIq4nl32T" => "kq_mulmm_iq4nl32", "MetalKqMulMmIq4nl32TH" => "kq_mulmm_iq4nl32", "MetalKqMulMmIq4nl32TH128" => "kq_mulmm_iq4nl32", "MetalKqMulMmIq4nl32THDb" => "kq_mulmm_iq4nl32", "MetalKqMulMmIq4nl32THDb128" => "kq_mulmm_iq4nl32",
    "MetalKqMulMmQ4032T" => "kq_mulmm_q4032", "MetalKqMulMmQ4032TH" => "kq_mulmm_q4032", "MetalKqMulMmQ4032TH128" => "kq_mulmm_q4032", "MetalKqMulMmQ4032THDb" => "kq_mulmm_q4032", "MetalKqMulMmQ4032THDb128" => "kq_mulmm_q4032",
    "MetalKqMulMmQ5032T" => "kq_mulmm_q5032", "MetalKqMulMmQ5032TH" => "kq_mulmm_q5032", "MetalKqMulMmQ5032TH128" => "kq_mulmm_q5032", "MetalKqMulMmQ5032THDb" => "kq_mulmm_q5032", "MetalKqMulMmQ5032THDb128" => "kq_mulmm_q5032",
    // the split-K pair behind each format's own kq_mulmm crown
    // ... and the 16-row stamps behind the same crowns (the q8 blob's behind mulmm_q8)
    "MetalKqMulMmK4THSk16_metal_kq_mulmm_db" => "kq_mulmm_k4",
    "MetalKqMulMmK4TSk16_metal_kq_mulmm_db" => "kq_mulmm_k4",
    "MetalKqMulMmK5THSk16_metal_kq_mulmm_db" => "kq_mulmm_k5",
    "MetalKqMulMmK5TSk16_metal_kq_mulmm_db" => "kq_mulmm_k5",
    "MetalKqMulMmK6THSk16_metal_kq_mulmm_db" => "kq_mulmm_k6",
    "MetalKqMulMmK6TSk16_metal_kq_mulmm_db" => "kq_mulmm_k6",
    "MetalKqMulMmIq4xsTHSk16_metal_kq_mulmm_db" => "kq_mulmm_iq4xs",
    "MetalKqMulMmIq4xsTSk16_metal_kq_mulmm_db" => "kq_mulmm_iq4xs",
    "MetalKqMulMmIq3s4THSk16_metal_kq_mulmm_db" => "kq_mulmm_iq3s4",
    "MetalKqMulMmIq3s4TSk16_metal_kq_mulmm_db" => "kq_mulmm_iq3s4",
    "MetalKqMulMmIq4nlTHSk16_metal_kq_mulmm_db" => "kq_mulmm_iq4nl",
    "MetalKqMulMmIq4nlTSk16_metal_kq_mulmm_db" => "kq_mulmm_iq4nl",
    "MetalKqMulMmQ40THSk16_metal_kq_mulmm_db" => "kq_mulmm_q40",
    "MetalKqMulMmQ40TSk16_metal_kq_mulmm_db" => "kq_mulmm_q40",
    "MetalKqMulMmNv4THSk16_metal_kq_mulmm_db" => "kq_mulmm_nv4",
    "MetalKqMulMmNv4TSk16_metal_kq_mulmm_db" => "kq_mulmm_nv4",
    "MetalKqMulMmK3THSk16_metal_kq_mulmm_db" => "kq_mulmm_k3",
    "MetalKqMulMmK3TSk16_metal_kq_mulmm_db" => "kq_mulmm_k3",
    "MetalKqMulMmK2THSk16_metal_kq_mulmm_db" => "kq_mulmm_k2",
    "MetalKqMulMmK2TSk16_metal_kq_mulmm_db" => "kq_mulmm_k2",
    "MetalKqMulMmIq3sTHSk16_metal_kq_mulmm_db" => "kq_mulmm_iq3s",
    "MetalKqMulMmIq3sTSk16_metal_kq_mulmm_db" => "kq_mulmm_iq3s",
    "MetalKqMulMmIq3xxsTHSk16_metal_kq_mulmm_db" => "kq_mulmm_iq3xxs",
    "MetalKqMulMmIq3xxsTSk16_metal_kq_mulmm_db" => "kq_mulmm_iq3xxs",
    "MetalKqMulMmIq2sTHSk16_metal_kq_mulmm_db" => "kq_mulmm_iq2s",
    "MetalKqMulMmIq2sTSk16_metal_kq_mulmm_db" => "kq_mulmm_iq2s",
    "MetalKqMulMmIq2xsTHSk16_metal_kq_mulmm_db" => "kq_mulmm_iq2xs",
    "MetalKqMulMmIq2xsTSk16_metal_kq_mulmm_db" => "kq_mulmm_iq2xs",
    "MetalKqMulMmIq2xxsTHSk16_metal_kq_mulmm_db" => "kq_mulmm_iq2xxs",
    "MetalKqMulMmIq2xxsTSk16_metal_kq_mulmm_db" => "kq_mulmm_iq2xxs",
    "MetalKqMulMmQ51THSk16_metal_kq_mulmm_db" => "kq_mulmm_q51",
    "MetalKqMulMmQ51TSk16_metal_kq_mulmm_db" => "kq_mulmm_q51",
    "MetalKqMulMmIq4nl32THSk16_metal_kq_mulmm_db" => "kq_mulmm_iq4nl32",
    "MetalKqMulMmIq4nl32TSk16_metal_kq_mulmm_db" => "kq_mulmm_iq4nl32",
    "MetalKqMulMmQ4032THSk16_metal_kq_mulmm_db" => "kq_mulmm_q4032",
    "MetalKqMulMmQ4032TSk16_metal_kq_mulmm_db" => "kq_mulmm_q4032",
    "MetalKqMulMmQ5032THSk16_metal_kq_mulmm_db" => "kq_mulmm_q5032",
    "MetalKqMulMmQ5032TSk16_metal_kq_mulmm_db" => "kq_mulmm_q5032",
    "MetalQ8MulMmTHSk16_metal_q8_mulmm_t" => "mulmm_q8",
    "MetalQ8MulMmTSk16_metal_q8_mulmm_t" => "mulmm_q8",
    "MetalKqMulMmIq4xsTHSk_metal_kq_mulmm_db" => "kq_mulmm_iq4xs",
    "MetalKqMulMmIq4xsTSk_metal_kq_mulmm_db" => "kq_mulmm_iq4xs",
    "MetalKqMulMmIq3s4THSk_metal_kq_mulmm_db" => "kq_mulmm_iq3s4",
    "MetalKqMulMmIq3s4TSk_metal_kq_mulmm_db" => "kq_mulmm_iq3s4",
    "MetalKqMulMmIq4nlTHSk_metal_kq_mulmm_db" => "kq_mulmm_iq4nl",
    "MetalKqMulMmIq4nlTSk_metal_kq_mulmm_db" => "kq_mulmm_iq4nl",
    "MetalKqMulMmQ40THSk_metal_kq_mulmm_db" => "kq_mulmm_q40",
    "MetalKqMulMmQ40TSk_metal_kq_mulmm_db" => "kq_mulmm_q40",
    "MetalKqMulMmK3THSk_metal_kq_mulmm_db" => "kq_mulmm_k3",
    "MetalKqMulMmK3TSk_metal_kq_mulmm_db" => "kq_mulmm_k3",
    "MetalKqMulMmK2THSk_metal_kq_mulmm_db" => "kq_mulmm_k2",
    "MetalKqMulMmK2TSk_metal_kq_mulmm_db" => "kq_mulmm_k2",
    "MetalKqMulMmIq3sTHSk_metal_kq_mulmm_db" => "kq_mulmm_iq3s",
    "MetalKqMulMmIq3sTSk_metal_kq_mulmm_db" => "kq_mulmm_iq3s",
    "MetalKqMulMmIq3xxsTHSk_metal_kq_mulmm_db" => "kq_mulmm_iq3xxs",
    "MetalKqMulMmIq3xxsTSk_metal_kq_mulmm_db" => "kq_mulmm_iq3xxs",
    "MetalKqMulMmIq2sTHSk_metal_kq_mulmm_db" => "kq_mulmm_iq2s",
    "MetalKqMulMmIq2sTSk_metal_kq_mulmm_db" => "kq_mulmm_iq2s",
    "MetalKqMulMmIq2xsTHSk_metal_kq_mulmm_db" => "kq_mulmm_iq2xs",
    "MetalKqMulMmIq2xsTSk_metal_kq_mulmm_db" => "kq_mulmm_iq2xs",
    "MetalKqMulMmIq2xxsTHSk_metal_kq_mulmm_db" => "kq_mulmm_iq2xxs",
    "MetalKqMulMmIq2xxsTSk_metal_kq_mulmm_db" => "kq_mulmm_iq2xxs",
    "MetalKqMulMmK4TSk_metal_kq_mulmm_db" => "kq_mulmm_k4",
    "MetalKqMulMmK6TSk_metal_kq_mulmm_db" => "kq_mulmm_k6",
    "MetalKqMvB2K4R2" => "kq_mvb2_k4_r2",
    "MetalKqMvW2K4H" => "kq_rows_half_k4",
    "MetalKqMvW4K4H" => "kq_rows_half_k4",
    "MetalKqMvW8K4H" => "kq_rows_half_k4",
    "MetalKqMvB2K5H" => "kq_rows_half_k5",
    "MetalKqMvB4K5H" => "kq_rows_half_k5",
    "MetalKqMvB8K5H" => "kq_rows_half_k5",
    "MetalKqMvB2K6H" => "kq_rows_half_k6",
    "MetalKqMvB4K6H" => "kq_rows_half_k6",
    "MetalKqMvB8K6H" => "kq_rows_half_k6",
    "metal_kq_gemv_iq3s_f4" => "kq_gemv_iq3s_f4",
    "metal_moe_gemv_iq3s_f4" => "kq_gemv_iq3s_f4",
    "metal_kq_gemv_iq3xxs_f4" => "kq_gemv_iq3xxs_f4",
    "MetalKqGemvIq2xxsF4" => "kq_gemv_iq2xxs_f4",
    "MetalAttnQKDev128" => "attn_dev",
    "MetalAttnAVDev128" => "attn_dev"
}

//! Did this run compile the GATED stamp families the blind list names? Two gates decide it, both
//! read from the run itself rather than re-probed: the Metal-4 tensor toolchain - the prefill's PSO
//! init puts every tensor stamp behind one probe, so the dense q8 tensor family's presence in the
//! census answers for all of them - and the box's tensor crowns, since every crown-gated twin
//! compiles only behind `metal_tensor_crowned` and an empty crown list means none of them exist.
def private census_gated_stamps_compiled(cov : table<string; int64>) : bool {
    if (empty(get_metal_tensor_crowns())) {
        return false
    }
    for (k in keys(cov)) {
        if (starts_with(k, "MetalQ8MulMmT")) {
            return true
        }
    }
    return false
}

//! The blind-spot list self-drains: census keys are the compiled kernels' MSL entry names, so a
//! declared entry whose key counted dispatches is a STALE entry, and one matching no key at all
//! names nothing and would sit green forever. A box that compiled none of the gated stamp families
//! cannot tell that second case from a misspelt key, so it SKIPS the entry loudly, never passes it.
def private metal_census_blind_drain(t : T?; cov : table<string; int64>) {
    let judge_unmatched = census_gated_stamps_compiled(cov)
    for (blind in CENSUS_NEVER_DISPATCHED) {
        var hits = 0l
        var named = false
        for (k, n in keys(cov), values(cov)) {
            if (starts_with(k, blind)) {
                hits += n
                named = true
            }
        }
        if (!named && !judge_unmatched) {
            t |> skip("{blind}: this box compiled no tensor or crowned stamp, so an entry matching no census key cannot be told from a misspelt one here")
            continue
        }
        let crown = CENSUS_CROWN_CONDITIONAL?[blind] ?? ""
        if (!named && !empty(crown) && !metal_tensor_crowned(crown)) {
            t |> skip("{blind}: the {crown} crown is not pinned on this box, so its stamp never compiled and the entry matches no census key here")
            continue
        }
        if (hits > 0l && !empty(crown) && metal_tensor_crowned(crown)) {
            t |> skip("{blind}: the {crown} crown is pinned on this box, so a stocked carrier reaches it ({hits}) - the entry is blind only off that crown")
            continue
        }
        t |> success(named,
            "{blind} names no compiled kernel - spell the census key (the class's own shader entry name, or <Class>_<kernel method>)")
        t |> success(hits == 0l,
            "{blind} is a DECLARED blind spot - a stocked carrier now dispatches it ({hits}): drop the entry, add its census row")
    }
}

[test]
def test_metal_kernel_coverage(tst : T?) {
    tst |> run("Metal kernel coverage census: the small-model zoo across format/graph/batch/KV axes") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!arm_on(t, "coverage") || !jit_only(t))
            var dev = metal_create_system_default_device()
            if (dev == null) {
                feint("no Metal device; kernel coverage census skipped\n")
                return
            }
            metal_release(dev)
            let prior_wscale = get_wscale_f16()
            let prior_backend = active_kernel_backend()
            set_wscale_f16(false)
            pin_kernel_backend("portable")   // row-major weights — the blob transform's donor contract
            metal_kernel_coverage_reset()
            census_zoo(t)
            // the census verdict: the report prints counts + LOUD WARNINGS for the zeros;
            // the assert pins only that the RAIL counts — coverage gaps are human calls
            metal_kernel_coverage_report()
            var cov <- metal_kernel_coverage()
            metal_census_blind_drain(t, cov)
            var served = 0
            for (n in values(cov)) {
                if (n > 0l) {
                    served++
                }
            }
            let famfilter = g_env_test.test_family
            if (empty(famfilter)) {
                t |> success(served >= 40,
                    "the census counted {served} distinct dispatched kernels (rail floor 40)")
            } else {
                // a family filter narrows the zoo by design — the floor only holds unfiltered
                to_log(LOG_INFO, "census floor not asserted (family filter '{famfilter}'): {served} served\n")
            }
            delete cov
            metal_tower_shutdown()
            metal_decode_shutdown()
            clear_kernel_backend_pin()
            select_kernel_backend(prior_backend)
            set_wscale_f16(prior_wscale)
        } else {
            feint("das_metal is not built on this platform; nothing to census\n")
        }
    }
}

// The Vulkan census's DECLARED blind spots, by census name (`<vk_dispatch name>_spv`): kernel
// classes no zoo row below can dispatch, each with the reason and the kernel-unit cell that does.
let private VK_CENSUS_NEVER_DISPATCHED <- [
    // the q51 rail (the 32-block down planes of the 26B-A4B's Q4_K_M, a carrier above the zoo's tier; the dense planes of the
    // Llama-3.2-1B Q5_1 mint, which no zoo row loads) - its l, m and s stamps (the m stamp its e column too) and its decode
    // GEMV; their coverage is test_vkd_q51_cm2_batch and test_vkd_q51_gemv
    "q51_batch_cm2l_cls_spv", "q51_batch_cm2m_cls_spv", "q51_batch_cm2s_cls_spv", "q51_gemv_cls_spv",
    // the mx4 expert rail (gpt-oss's native-MXFP4 routed stacks: a carrier above the zoo's tier) - its s and e stamps and
    // its decode GEMV; their coverage is test_vkd_mx4_cm2_batch and test_vkd_mx4_gemv
    "mx4_batch_cm2s_cls_spv", "mx4_batch_cm2e_cls_spv", "mx4_gemv_cls_spv",
    // the iq4nl32 rail (the 32-block IQ4_NL down planes off the 256 lattice - Qwen3.8-Flash-Next, above the zoo's tier; the
    // dense planes of the Qwen2.5-0.5B IQ4_NL mint, which no zoo row loads) - its l, m and s stamps (the m stamp its e
    // column too) and its decode GEMV; their coverage is test_vkd_iq4nl32_cm2_batch and test_vkd_iq4nl32_gemv
    "iq4nl32_batch_cm2l_cls_spv", "iq4nl32_batch_cm2m_cls_spv", "iq4nl32_batch_cm2s_cls_spv", "iq4nl32_gemv_cls_spv",
    // the iq3s4 rail's cm2 l stamp (IQ3_S served as a nibble codebook plane: the zoo's IQ3_S rows dispatch its decode GEMV, its
    // batch tile and its m, s and KHR stamps, none of them a plane tall enough for the l column); its coverage is test_vkd_iq3s4_cm2_batch
    "kq_batch_iq3s4_cm2l_cls_spv",
    // the nv4 rail (NVFP4 planes: no zoo row loads an nv4 file - the 1B mint is machine-local and the ModelOpt files keep BF16 k/v
    // planes the whole-model driver declines) - its decode GEMV and token-table gather, its per-op batch tile, its cm2 l, m and s
    // stamps and its KHR tile; their coverage is test_vkd_gemv_ncol, test_vkd_emb_gather_family, test_vkd_kq_batch_family and
    // test_vkd_nv4_cm2_batch
    "kq_gemv_nv4_cls_spv", "kq_emb_nv4_cls_spv", "kq_batch_nv4_cls_spv", "kq_batch_nv4_cm2l_cls_spv", "kq_batch_nv4_cm2m_cls_spv",
    "kq_batch_nv4_cm2s_cls_spv", "kq_batch_nv4_khr_cls_spv",
    // the hyper-connection chain (Qwen3.8-Flash-Next, the one qwen4exp carrier, above the zoo's tier): the mixer seams and the
    // n-gram side input; their coverage is test_vkd_hc_seams, test_vkd_hc_rows and test_vkd_ple_side, the serving run
    // test_gpu_resident_hc.das
    "hc_init_cls_spv", "hc_norm_cls_spv", "hc_lo_rq_cls_spv", "hc_mix_cls_spv", "hc_comb_cls_spv", "ple_gate_cls_spv", "ple_conv_cls_spv", "ple_commit_cls_spv",
    // ... and the deltanet out-gate's sigmoid stamps the same carrier's recurrent layers take; their coverage is the sigmoid
    // arms of test_vkd_dn_family
    "dn_step_sig_cls_spv", "dn_scan_p3_sig_cls_spv",
    // the checkpoint capture's history write (compiled for every hybrid, dispatched only when a Vulkan prefill window carries a
    // checkpoint stop - a scheduler's stable opening; the census rows prefill with no stop); its coverage is test_vkd_dn_cap_tail
    // in test_vulkan_kernels.das
    "dn_cap_tail_cls_spv",
    // the per-32 expert rails' KHR arms (the expert schedule on a KHR-mode card: the 26B-A4B Q4_K_M's q51 down stacks, gpt-oss's
    // mx4 stacks, Flash-Next's iq4nl32 stacks - carriers above the zoo's tier); their coverage is the KHR arm of
    // test_vkd_q51_cm2_batch, test_vkd_mx4_cm2_batch and test_vkd_iq4nl32_cm2_batch
    "q51_batch_khr_cls_spv", "mx4_batch_khr_cls_spv", "iq4nl32_batch_khr_cls_spv",
    // the mx4 experts' fused gate+up+act+requant GEMV (gpt-oss's token command, above the zoo's tier); its coverage is
    // test_vkd_mx4_gemv_gu in test_vulkan_kernels.das
    "mx4_gemv_gu_cls_spv",
    // the classifier tail's selection (compiled on every resident arm, dispatched only for a sampled stream's step; the zoo's
    // rows step greedy); its coverage is test_vkd_cls_select in test_vulkan_kernels.das
    "cls_select_spv",
    // the IQ4_XS, iq3s4 and IQ2_S experts' fused gate+up+act+requant GEMVs (the gemma-4 26B's Unsloth dynamic files' gate_up stacks,
    // above the zoo's tier); their coverage is test_vkd_kq_gemv_k4_gu in test_vulkan_kernels.das
    "kq_gemv_iq4xs_gu_cls_spv", "kq_gemv_iq3s4_gu_cls_spv", "kq_gemv_iq2s_gu_cls_spv",
    // the biased act twins (gpt-oss's expert stacks carry gate and up bias rows: a carrier above the zoo's tier) - the
    // Q8_0 requant form the token command runs and the f16 form the window chain runs; their coverage is test_vkd_act_family
    "q8_actrq_b_cls_spv", "actf16_b_cls_spv",
    // the attention-sink twins (gpt-oss's per-head sink logits, a carrier above the zoo's tier): the h64 flash tiles on both
    // arms with their f16-out twins, the token command's pass; their coverage is the sink arms of
    // test_vkd_fa_cm2, test_vkd_fa_khr and test_vkd_da_attn in test_vulkan_kernels.das
    "fa_cm2_h64_sink_cls_spv", "fa_cm2_h64_sink_f16_cls_spv", "fa_khr_h64_sink_cls_spv", "fa_khr_h64_sink_f16_cls_spv",
    "da_attn_sink_cls_spv", "da_attn_f16_sink_cls_spv",
    // the token command's f32-mirror attention stamps (the four-head slab and the two-head one): the census steps every
    // session on the f16 mirror the tier arms, the f32 twin serves under DASLLAMA_VK_KV32=1 (gemma-2-2b Q8_0's f32 parity
    // run dispatches both); their coverage is test_vkd_da_attn and test_vkd_da_attn_rqk in test_vulkan_kernels.das
    "da_attn_cls_spv", "da_attn_g2_cls_spv",
    // the E-series per-layer-embedding tail (the act, its requant and the proj GEMV in one): its carriers are the
    // gemma-4 E2B / E4B, no zoo row; its coverage is test_vkd_q8_gemv_pleact in test_vulkan_kernels.das
    "q8_gemv_pleact_cls_spv",
    // the per-expert streamed chain's by-row gather and combine: the whole-model driver's streamed layers dispatch them, and
    // no stocked carrier streams (every zoo MoE fits the card whole); their coverage is test_vkd_moe_gather_row and
    // test_vkd_combine_row in test_vulkan_kernels.das, the chain's test_vulkan_moe_cm2_split, and the model-level
    // test_gpu_resident_moe_formats cells (large-tier, DASLLAMA_PARITY_FULL=1)
    "moe_gather_f16_row_cls_spv", "combine_row_cls_spv",
    // the Bergamot translation driver's step and attention stamps: the Vulkan serving census loads no translation model (its
    // Bergamot row, `cov_tower_bergamot`, serves on the Metal seat alone); their coverage is test_vkt_nmt and test_vkt_nmt_attn
    // in test_vulkan_tower_kernels.das, the driver's whole sentence test_nmt_device in test_nmt.das
    "nmt_embed_cls", "nmt_ssru_cls", "nmt_pick_cls", "nmt_attn_part_cls", "nmt_attn_comb_cls",
    // the KHR flash-attention tile's 256 and 512 heads: the mode sweep's mm row reaches the 64 and 128 heads through the
    // zoo's qwens; no zoo row carries a 512 head or an ungated or capped 256 head (the gemmas sit above the tier); the
    // gated 256 keys are the zoo's Qwen3.5-9B's, which the census runs at the box's coopmat mode - cm2 on the pod, so its
    // row takes the cm2 stamp; a KHR-only box (the zen4 760M) dispatches them through that file and the Qwen3.6-35B-A3B
    // files; their coverage is test_vkd_fa_khr, test_vkd_fa_khr_h256_softcap and test_vkd_fa_khr_h256_gated in
    // test_vulkan_kernels.das
    "fa_khr_h256_cls_spv", "fa_khr_h256_f16_cls_spv", "fa_khr_h256_cap_cls_spv", "fa_khr_h256_cap_f16_cls_spv",
    "fa_khr_h256_g_cls_spv", "fa_khr_h256_g_f16_cls_spv",
    "fa_khr_h512_cls_spv", "fa_khr_h512_f16_cls_spv",
    // the bidirectional flash tiles (the vision towers' encoder attention): the 64 head is the gemma4v tower's (the stocked
    // mmproj-gemma-4-E2B-it-bf16.gguf, dispatched by test_gemma4v_vulkan_twin in test_gemma4v.das), the padded 128 the
    // gemma3v / qwen3v towers' (mmproj-gemma-3-4b-it-F16.gguf via test_gemma3v_vulkan_twin, mmproj-Qwen3VL-4B-Instruct-F16.gguf
    // via test_qwen3v_vulkan_twin) - no zoo row; their kernel coverage is test_vkt_fa_bidir in test_vulkan_tower_kernels.das
    "fa_cm2_h64_bidir_cls_spv", "fa_cm2_h128_bidir_cls_spv", "fa_khr_h64_bidir_cls_spv", "fa_khr_h128_bidir_cls_spv",
    // the audio fronts' classes (the Vulkan tower driver's mel and conv seats - no zoo row, no audio carrier in the census);
    // the carriers are the stocked mmproj-Qwen3-ASR-0.6B-bf16.gguf (test_qwen3a_vulkan_front in test_audio.das),
    // mmproj-gemma-4-E2B-it-bf16.gguf (test_gemma4a_vulkan_twin) and canary-qwen-2.5b-encoder-f32.bin (test_canary_vulkan_twin);
    // every covering cell below is in test_vulkan_tower_kernels.das
    // the spectrum pair (Qwen3-ASR's mel); its coverage is test_vkt_tower_front_mel
    "tower_spec_dft_cls_spv",
    // the mel-major log store (Qwen3-ASR's mel); its coverage is test_vkt_tower_front_mel
    "tower_mel_log_cls_spv",
    // the qwen3a front's k3 s2 p1 im2col; its coverage is test_vkt_tower_front_movers
    "tower_im2col_cls_spv",
    // the qwen3a front's conv-out shuffle; its coverage is test_vkt_tower_front_movers
    "tower_q3a_shuffle_cls_spv",
    // the LayerNorm + ReLU rows of the gemma4a subsample convs; its coverage is test_vkt_tower_front_movers
    "tower_ln_relu_cls_spv",
    // the whisper-class projector tails' pair pool and swapped SwiGLU; their coverage is test_vkt_tower_tail_rows
    "tower_pool_rows_cls_spv",
    "tower_swiglu_swapped_cls_spv",
    // gigaam's biased tap-major depthwise conv: no census row reaches it because the Vulkan census loads no gigaam carrier
    // (gigaam-v3-e2e-rnnt-f16.gguf, run by test_gigaam_vulkan_twin); its coverage is test_vkt_tower_fastconformer
    "tower_ga_dw_cls_spv",
    // the same conv over the carrier's f16 taps, and GigaAM's CTC head pick (the ASR-decoder driver's CTC seat, run by a
    // gigaam CTC carrier's decode - gigaam-v3-ctc-f16.gguf and gigaam-v3-e2e-ctc-f16.gguf via test_gigaam_ctc_vulkan_twin in
    // test_audio.das): no census row loads a gigaam carrier; their coverage is test_vkt_tower_ga_dwh and test_vkt_ctc_pick
    "tower_ga_dwh_cls_spv", "ctc_pick_cls_spv",
    // the transducer decode's four stamps (the ASR-decoder driver's transducer seat, run by GigaAM's and parakeet's decode - no
    // zoo row, no audio carrier in the census); their coverage is test_vkt_rnnt
    "rnnt_gates_cls_spv", "rnnt_proj_cls_spv", "rnnt_joint_cls_spv", "rnnt_pick_cls_spv",
    // gigaam's device mel's frame window: no census row reaches it because the Vulkan census loads no gigaam carrier
    // (gigaam-v3-e2e-rnnt-f16.gguf, run by test_gigaam_vulkan_twin); its coverage is test_vkt_tower_ga_mel
    "tower_ga_frames_cls_spv",
    // parakeet's tap-major subsample depthwise conv2d; its coverage is test_vkt_tower_dw2d
    "tower_pk_dw2d_cls_spv",
    // the bias-less seam of parakeet's blocks; its coverage is test_vkt_tower_ln_rows
    "tower_post_add_ln_nb_cls_spv",
    // the whisper decoder's f16 K/V store; its coverage is test_vkt_wdec_classes
    "tower_wdec_kv16_cls_spv",
    // canary's per-feature mel normalization; its coverage is test_vkt_tower_front_mel
    "tower_mel_norm_cls_spv",
    // canary's front depthwise conv; its coverage is test_vkt_tower_front_mel
    "tower_dw_conv_cls_spv",
    // canary's feature rows (the feature shuffle); its coverage is test_vkt_tower_front_movers
    "tower_feat_shuffle_cls_spv",
    // canary's sinusoid rel table; its coverage is test_vkt_tower_front_mel
    "tower_rel_sincos_cls_spv",
    // canary's rel-plane attention (the tiled online softmax at both head widths; the head panels ahead of the rel GEMM are the
    // restride stamps at one head): the canary twin dispatches the 128-wide stamp (canary's head width); the 64-wide stamp
    // serves no stocked carrier; their kernel coverage is test_vkt_tower_cn_attn_rel_plane (both stamps) in
    // test_vulkan_tower_kernels.das
    "tower_cn_attn_r_cls_spv", "tower_cn_attn_r64_cls_spv",
    // the whisper decoder's classes (the Vulkan ASR-decoder driver - no zoo row, no whisper carrier in the census): the K/V
    // stores and the chunked attention's two halves, the stocked ggml-tiny.bin and ggml-large-v3-turbo.bin via
    // test_whisper_vulkan_wdec in test_whisper.das; their kernel coverage is test_vkt_wdec_classes in
    // test_vulkan_tower_kernels.das
    "tower_wdec_kv_cls_spv", "tower_wdec_attn_part_cls_spv", "tower_wdec_attn_comb_cls_spv",
    // the decode step's fused row passes (the layernorm, the post-add with its next norm, the bias + tanh GELU and the attention
    // combine, each landing its Q8_0 feed): the same two stocked files via test_whisper_vulkan_wdec; their kernel coverage is
    // test_vkt_wdec_fused_rq in test_vulkan_tower_kernels.das
    "tower_ln_rq_cls_spv", "tower_post_add_ln_rq_cls_spv", "tower_bias_act_rq_cls_spv", "tower_wdec_attn_comb_rq_cls_spv",
    // the pre-LN chain's f16-feed stamps (the cm2 arm: the norms and seams storing the feed, the biased restrides, the unpad and
    // the bias + GELU storing it): the whisper twin and the gemma3v twin (mmproj-gemma-3-4b-it-F16.gguf) dispatch them; their
    // kernel coverage is test_vkt_tower_f16_feeds in test_vulkan_tower_kernels.das
    "tower_ln16_cls_spv", "tower_post_add_ln16_cls_spv", "tower_hpad_b_cls_spv", "tower_hpad16_b_cls_spv", "tower_hunpad16_cls_spv",
    "tower_bias_act_cvt16_cls_spv",
    // the gemma4v tower's row classes (the clamps, feeds, the two-axis rope, the post-add, the GEGLU and the weighted rms the
    // vision tails share): the stocked mmproj-gemma-4-E2B-it-bf16.gguf, dispatched by test_gemma4v_vulkan_twin in
    // test_gemma4v.das - no zoo row; their kernel coverage is test_vkt_tower_rows in test_vulkan_tower_kernels.das (the clamp +
    // halfword feed is no entry: it is the LLM prefill's f32 -> f16 rows too, which the zoo rows dispatch)
    "tower_clamp_cls_spv", "tower_clamp_rq_cls_spv", "tower_rope2d_cls_spv",
    "tower_post_add_cls_spv", "tower_geglu_cls_spv", "tower_rms_cls_spv",
    // the vision chains' stems and tails on the device (the patch im2col, the 2-axis position add, the qwen3v stem assemble, the
    // grid pool, the standardize rows) and the exact lane's clamp copy ahead of the f32 tile (gemma4v): the stocked
    // mmproj-gemma-3-4b-it-F16.gguf, mmproj-gemma-4-E2B-it-bf16.gguf and mmproj-Qwen3VL-4B-Instruct-F16.gguf through the
    // families' Vulkan twins - no zoo row; their kernel coverage is test_vkt_tower_vision_rows in test_vulkan_tower_kernels.das
    // (the clamp copy's is test_vkt_tower_rows), their bodies the templates both homes stamp, whose Metal cells are
    // test_metal_misc_kernels.das' pos_add, q3v_stem and patch_im2col gates and test_metal_prefill_kernels.das' pool and affine
    // gates
    "tower_patch_im2col_cls_spv", "tower_pos_add_2d_cls_spv", "tower_stem_asm_cls_spv", "tower_pool_2d_cls_spv", "tower_affine_rows_cls_spv",
    "tower_clamp_copy_cls_spv",
    // the biased-block (SigLIP) row classes: the bias with its GELU, the head restrides to the flash tile's width (the
    // stocked mmproj-gemma-3-4b-it-F16.gguf, dispatched by test_gemma3v_vulkan_twin in test_gemma3v.das; the rope on a
    // fused row's slot is the qwen3v tower's - mmproj-Qwen3VL-4B-Instruct-F16.gguf via test_qwen3v_vulkan_twin in
    // test_qwen3v.das); their kernel coverage is test_vkt_tower_ln_rows. The layernorm, the seam with its next
    // layernorm and the bias pass serve the TTS rows too (the PL-BERT seat, the decoder convs' bias after the f16
    // tile), so the census reaches them on a box with a StyleTTS2 carrier
    "tower_hpad_cls_spv", "tower_hpad16_cls_spv",
    "tower_hunpad_cls_spv", "tower_rope_tab_cls_spv",
    // the window-attention ViT's classes (qwen25v): the f32 per-window attention over the compact rows and the rms seam (the
    // gated hidden is the shared bias-gate stamp (`TowerBiasGate16`); the stocked mmproj-Qwen2.5-Omni-3B-f16.gguf,
    // dispatched by test_qwen25v_vulkan_twin in test_qwen25v.das); their kernel coverage is test_vkt_tower_win_rows
    "tower_win_attn_cls_spv", "tower_post_add_rms_cls_spv", "tower_bias_gate16_cls_spv",
    // the audio towers' classes - no zoo row runs an audio tower: the Conformer's silu rows (the bias class's unbiased stamp),
    // GLU, causal depthwise taps, windowed RPE attention (the 128-wide stamp serves the stocked mmproj-gemma-4-E2B-it-bf16.gguf's
    // audio encoder through test_gemma4a_vulkan_twin in test_audio.das) and the plain residual seam; the FastConformer's
    // centered depthwise conv (the stocked canary-qwen-2.5b-encoder-f32.bin through test_canary_vulkan_twin); the 64-wide
    // attention stamp serves no stocked carrier; their kernel coverage is test_vkt_tower_conformer,
    // test_vkt_tower_fastconformer and test_vkt_tower_rows
    "tower_act_cls_spv", "tower_glu_sig_cls_spv", "tower_dw_conv5_cls_spv", "tower_g4a_attn_cls_spv", "tower_g4a_attn64_cls_spv",
    "tower_post_add_plain_cls_spv", "tower_cn_dw_cls_spv",
    // the KHR f16 GEMM (the E-series projection off cm2): no zoo row carries a per-layer embedding; its coverage is
    // test_vkd_f16_gemm_khr
    "f16_gemm_khr_cls_spv",
    // the gemma-4 MoE combine (the two normed branches' residual step, f32 and f16 normed-row forms):
    // its carrier is the 26B-A4B, above the zoo's tier; its coverage is test_vkd_moe_routing's gemma-4
    // combine arm in test_vulkan_kernels.das
    "cls_ar_comb_g4_spv", "cls_ar_comb_g4_f16_b_spv", "cls_ar_comb_g4_rq_spv",
    // the h512 flash tile (gemma-4's global heads) with its f16 twin: its carriers are the gemma-4 dense files,
    // no zoo row; its coverage is test_vkd_fa_cm2's h512 arm in test_vulkan_kernels.das
    "fa_cm2_h512_cls_spv", "fa_cm2_h512_f16_cls_spv",
    // the batched windowed decode attention and its f16 twin (a sliding window over the batch rows: the gemma
    // carriers, no zoo row); its coverage is test_vkd_da_attn_bw in test_vulkan_kernels.das
    "da_attn_bw_cls_spv", "da_attn_bw_f16_cls_spv",
    // the per-32 routed down GEMVs' summing twins serve only the gemma-4 MoE's device experts, and no zoo row loads a 26B-A4B file
    // (above the zoo's tier). A key a line - its carrier's down stacks, the stocked file that runs it, the model-free cell's summed arm:
    // q51: gemma-4-26B-A4B-it-Q4_K_M.gguf (Q5_1 down), test_gpu_resident_gemma4_26b_k.das and
    //      test_gpu_resident_regions_gemma4moe.das; test_vkd_q51_gemv
    // iq4nl32: gemma-4-26B-A4B-it-UD-IQ3_XXS.gguf (IQ4_NL down), test_gpu_resident_gemma4_26b.das; test_vkd_iq4nl32_gemv
    // q4032: gemma-4-26B-A4B-it-Q4_0.gguf (Q4_0 down), no stocked Vulkan file (the Metal census's gemma4moe row alone loads it);
    //        test_vkd_q4032_gemv
    // q5032: no stocked file carries a Q5_0 down stack; test_vkd_q5032_gemv
    // mx4: gemma-4-26B-A4B-it-MXFP4_MOE-q4km-local.gguf (MXFP4 down), test_gpu_resident_gemma4_26b_mx4.das; test_vkd_mx4_gemv
    "q51_gemv_sum_cls_spv", "iq4nl32_gemv_sum_cls_spv", "q4032_gemv_sum_cls_spv", "q5032_gemv_sum_cls_spv", "mx4_gemv_sum_cls_spv",
    // the E-series per-layer-embedding pre-step's classes serve only an E-series load, and no Vulkan census row loads one (the
    // E-series rows are Metal's). A key a line - the stocked file that dispatches it, the model-free cell:
    // ple_act_f32: gemma-4-E2B-it-Q4_K_M.gguf's F32 per-layer planes, test_gpu_resident_gemma4_e2b.das (arm g4e-kq, its count
    //              asserted); test_vkd_ple_act_f32
    // ple_finish: every E-series device pre-step, gemma-4-E2B-it-Q8_0.gguf in test_gpu_resident_gemma4_e2b.das; test_vkd_ple_finish
    // router_gemv_bf16: the BF16 projection's decode column, gemma-4-E2B-it-Q8_0.gguf in test_gpu_resident_gemma4_e2b.das;
    //                   test_vkd_router_gemv_bf16
    // ple_bf16_cvt: the BF16 projection's per-window f16 panel, the same file's prefill windows; test_vkd_ple_bf16_cvt
    // f32_batch_khr: the staged KHR tile over the F32 per-layer gate and projection rows, gemma-4-E2B-it-Q4_K_M.gguf's
    //                prefill windows in test_gpu_resident_gemma4_e2b.das (arm g4e-kq, its count asserted); test_vkd_f32_khr_batch
    // router_gemv_bf16_hw: the BF16 projection's halfword stamp, which serves an odd model dim or base - every stocked E-series
    //                      carrier's dim is even on an even base, so no file reaches it; test_vkd_router_gemv_cols's halfword arms
    "ple_act_f32_cls_spv", "ple_finish_cls_spv", "router_gemv_bf16_cls_spv", "ple_bf16_cvt_cls_spv", "f32_batch_khr_cls_spv",
    "router_gemv_bf16_hw_cls_spv",
    // the scalar router GEMM: on a cm2 box every MoE carrier whose dim the small f16 GEMM's eight
    // 64-deep chunks divide takes the f16 route, so the scalar tile runs only off it (a dim off
    // 512, the KHR route). Its coverage is test_vkd_moe_routing's router arm in test_vulkan_kernels.das
    "router_gemm_cls_spv",
    // the N-column decode GEMV leaves serve only the N-row token command, which the zoo's one-region loads never
    // engage (a batched step here rides f32 sessions on the per-op rails); the q8 row-pair stamps serve only past
    // `g_q8_n2_min_n`, off until a box's token command proves the pair. Their coverage is test_vkd_gemv_ncol in
    // test_vulkan_kernels.das; the stocked carriers are the regions files - test_gpu_resident_regions_qwen3.das and
    // the gemma files for q8, test_gpu_resident_regions_llama.das for k4 and k6, test_gpu_resident_regions_hybrid_iq3s.das
    // for iq3s, test_gpu_resident_regions_hybrid_q3kxl.das for the eight low-bit leaves
    "q8_gemv_n2_cls_spv", "q8_gemv_n4_cls_spv", "q8_gemv_n8_cls_spv",
    "q8_gemv_n2p_cls_spv", "q8_gemv_n4p_cls_spv", "q8_gemv_n8p_cls_spv",
    "kq_gemv_k4_n_cls_spv", "kq_gemv_k5_n_cls_spv", "kq_gemv_k6_n_cls_spv", "kq_gemv_q40_n_cls_spv", "kq_gemv_iq4xs_n_cls_spv", "kq_gemv_iq3s4_n_cls_spv",
    "kq_gemv_k3_n_cls_spv", "kq_gemv_k2_n_cls_spv", "kq_gemv_iq3s_n_cls_spv", "kq_gemv_iq3xxs_n_cls_spv", "kq_gemv_iq4nl_n_cls_spv", "kq_gemv_nv4_n_cls_spv",
    "kq_gemv_iq2s_n_cls_spv", "kq_gemv_iq2xs_n_cls_spv", "kq_gemv_iq2xxs_n_cls_spv",
    // the fused gate-up GEMV's N stamps serve only the N-row token command, which the zoo's one-region load never engages
    // (the batched rows need a region a row; the stocked regions files - test_gpu_resident_regions_qwen3.das and the
    // gemma files - reach them at two regions); their coverage is test_vkd_q8_gemv_gu_n in test_vulkan_kernels.das
    "q8_gemv_gu_n2_cls_spv", "q8_gemv_gu_n4_cls_spv", "q8_gemv_gu_n8_cls_spv",
    // the sink stamps over a block-codec K/V mirror (gpt-oss with the mirror asked in q8_0 or tq4: the census loads no sink
    // carrier); their coverage is the sink arms of test_vkc_q8_da_attn and test_vkc_tq4_da_attn in test_vulkan_kv_codec_kernels.das
    "da_attn_q8_sink_cls_spv", "da_attn_tq4_sink_cls_spv"]

//! a prefill over the counting prompt and two greedy decode steps through `s`
def private cov_prefill_two_steps(tr : Model; var s : Session) {
    let vocab = tr.config.vocab_size
    let p <- encode(tr, counting_prompt(1, 40), true, false)
    eval_(tr, s, p)
    var cur = argmax_of(s.logits, vocab)
    for (_i in range(2)) {
        var inscope one <- [cur]
        eval_(tr, s, one)
        delete one
        cur = argmax_of(s.logits, vocab)
    }
}

//! one model's sweep with the K/V mirror asked in block codec `kv`, on a session of that codec: a prefill (the window's staged
//! store, the shadow, the flash tiles over it) and two decode steps (the token store, the codec's attention stamp, tq4's un-rotation)
[unused_argument(t, path, family, kv)]   // without vulkan the gated body is empty
def private vk_cov_codec_model(t : T?; path, family : string; kv : KVDtype) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (!family_on(t, family) || !arm_on(t, "coverage-vk") || !model_available(t, path)) {
            return
        }
        let pin = gpu_kv_dtype_pin_()
        set_gpu_kv_dtype_(kv)
        var tr <- load_model_(path, QuantMode.q8)
        restore_gpu_kv_dtype(pin)
        tr.config.seq_len = min(tr.config.seq_len, 512l)
        with_dasllama_jobque() {
            var s = make_run_state(tr.config, kv, kv)
            cov_prefill_two_steps(tr, s)
            delete s
        }
        moe_gpu_drop_model_()
        delete tr
    }
}

[unused_argument(t, path, family, nbs)]   // without vulkan the gated body is empty
def private vk_cov_model(t : T?; path, family : string; nbs : array<int>) {
    // one model's vulkan coverage sweep: GPU prefill (the counting prompt clears the npos gate),
    // two greedy decode steps, a continuation prefill, then one batched step per requested B.
    // No parity asserts — the census wants DISPATCH DIVERSITY, the parity suites own truth.
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (!family_on(t, family) || !arm_on(t, "coverage-vk") || !model_available(t, path)) {
            return
        }
        var tr <- load_model_(path, QuantMode.q8)
        tr.config.seq_len = min(tr.config.seq_len, 512l)
        let vocab = tr.config.vocab_size
        with_dasllama_jobque() {
            // an f32 session rides the per-op rails; an f16 one (the armed mirror codec) the whole-model driver
            for (kdt in [KVDtype.f32, KVDtype.f16]) {
                var s = make_run_state(tr.config, kdt, kdt)
                cov_prefill_two_steps(tr, s)
                var inscope p2 <- encode(tr, counting_prompt(50, 90), false, false)
                eval_(tr, s, p2)
                delete p2
                delete s
            }
            for (nb in nbs) {
                var inscope ss : array<Session>
                ss |> reserve(nb)
                var sess : array<Session?>
                var inscope curs : array<int64>
                for (i in range(nb)) {
                    ss |> emplace <| make_run_state(tr.config, KVDtype.f32, KVDtype.f32)
                    var inscope pb <- encode(tr, counting_prompt(3 + i, 44 + i), true, false)
                    eval_(tr, ss[i], pb)
                    sess |> push(unsafe(addr(ss[i])))
                    curs |> push(argmax_of(ss[i].logits, vocab))
                }
                var ws <- create_batch_workspace_(tr)
                eval_batch_(tr, ws, sess, curs)
                sess |> clear()   // borrowed handles — delete of array<T?> cascades to the pointees
                unsafe {
                    delete ws
                    delete sess
                }
                delete curs
                delete ss
            }
        }
        // the model-switch contract: whoever puts a DIFFERENT model on the GPU drops the old one
        // first (the slot machinery / server do) — without this the stack table accumulates
        // across the zoo until the MAX_STACKS decline
        moe_gpu_drop_model_()
        delete tr
    }
}

//! the two carriers the block codecs' attention stamps need: the four-head slab (qwen2: fourteen heads over two) and the
//! two-head slab (gemma-2: eight over four)
[cold_path]
def private vk_cov_codecs(t : T?) {
    for (kv in [KVDtype.q8_0, KVDtype.tq4]) {
        vk_cov_codec_model(t, path_join(models_dir(), "Qwen2.5-0.5B-Instruct-Q8_0.gguf"), "qwen2", kv)
        vk_cov_codec_model(t, path_join(models_dir(), "gemma-2-2b-it-Q8_0.gguf"), "gemma2", kv)
    }
}

//! every prefill tile family under the coopmat modes its planes have twins in - the box's resolved mode, mm,
//! sdot4, and for the q8 and q40 planes the f16 and int8 coopmat twins too; the carriers are the qwen3 files
//! (the Q8_0 also carries the resident fused qk_norm rail; the Q4_K_M k4/k6 with the Q8_K activation quant
//! forms and qk_norm on the split rail) and the 1B requants the format arc used
[unused_argument(t)]   // without vulkan the gated body is empty
def private vk_cov_mode_sweep(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let vehicles = fixed_array(
            (file = "Qwen3-0.6B-Q8_0.gguf", family = "qwen3", nmodes = 5),
            (file = "Llama-3.2-1B-Instruct-Q4_0-local.gguf", family = "llama", nmodes = 5),
            (file = "Qwen3-4B-Instruct-2507-Q4_K_M.gguf", family = "qwen3", nmodes = 3),          // k4 + k6
            (file = "Llama-3.2-1B-Instruct-IQ4_XS.gguf", family = "llama", nmodes = 3),
            (file = "Llama-3.2-1B-Instruct-Q3_K_L.gguf", family = "llama", nmodes = 3),           // k3 + k5
            (file = "Llama-3.2-1B-Instruct-IQ3_M.gguf", family = "llama", nmodes = 3),            // iq3s
            (file = "Llama-3.2-1B-Instruct-IQ3_XXS-local.gguf", family = "llama", nmodes = 3),
            (file = "Llama-3.2-1B-Instruct-IQ4_NL-local.gguf", family = "llama", nmodes = 3),
            (file = "Llama-3.2-1B-Instruct-Q2_K-local.gguf", family = "llama", nmodes = 3),
            (file = "Llama-3.2-1B-Instruct-IQ2_XS-local.gguf", family = "llama", nmodes = 3),
            (file = "Llama-3.2-1B-Instruct-IQ2_XXS-local.gguf", family = "llama", nmodes = 3),
            (file = "Llama-3.2-1B-Instruct.i1-IQ3_XXS.gguf", family = "llama", nmodes = 3))       // iq2s on the attention planes
        if (!vk_moe_init()) {   //! the sweep reads the device's resolved mode before its first row loads (a family filter may have skipped every earlier row)
            t |> skip("no Vulkan device for the coopmat mode sweep")
            return
        }
        let none : array<int>
        let box_mode = g_gpu.coopmat_mode
        let modes = fixed_array(box_mode, COOPMAT_MM, COOPMAT_SDOT4, COOPMAT_F16, COOPMAT_INT8)
        for (v in vehicles) {
            let vpath = path_join(models_dir(), v.file)
            for (mi in range(v.nmodes)) {
                g_gpu.coopmat_mode = modes[mi]
                vk_cov_model(t, vpath, v.family, none)
            }
            g_gpu.coopmat_mode = box_mode
        }
    }
}

[test]
def test_vulkan_kernel_coverage_serving(tst : T?) {
    tst |> run("Vulkan serving census: the local zoo across resident/arena/kq/deltanet/MoE") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!arm_on(t, "coverage-vk")) {
                return
            }
            vk_kernel_coverage_reset()
            let none : array<int>
            // llama q8: the resident dense ladder (device prefill + decode) + the per-op rails' batched steps at 2, 4
            // and 8 f32 sessions (the resident N-row command needs a region a row: the regions files carry the N stamps)
            vk_cov_model(t, path_join(models_dir(), "tinyllama-1.1b-chat-v1.0.Q8_0.gguf"), "llama", [2, 4, 8])
            // gemma3 q8: the per-op arena rails + qk_norm
            vk_cov_model(t, path_join(models_dir(), "gemma-3-1b-it-Q8_0.gguf"), "gemma3", none)
            //! phi3 K-quant: a head of 96 - the window's head-96 flash tiles (`fa_cm2_h96*`), the token step's narrow attention
            vk_cov_model(t, path_join(models_dir(), "Phi-3.5-mini-instruct-Q4_K_M.gguf"), "phi3", none)
            vk_cov_codecs(t)
            vk_cov_mode_sweep(t)
            // qwen3.5 q8: the DeltaNet chains (conv/step/scan + the decode step)
            vk_cov_model(t, path_join(models_dir(), "Qwen3.5-0.8B-Q8_0.gguf"), "qwen35", none)
            // qwen3.5 9B (large tier): head-256 gated attention on the cm2 flash stamps, the
            // K-quant deltanet planes on the resident driver
            vk_cov_model(t, path_join(models_dir(), "Qwen3.5-9B-MTP-UD-Q5_K_XL.gguf"), "qwen35", none)
            // the MoE rows (expert FFN/qkv chains + heat cache) sit above the small tier
            vk_cov_model(t, path_join(models_dir(), "Qwen1.5-MoE-A2.7B-Chat.Q8_0.gguf"), "qwen2moe", none)
            vk_cov_model(t, path_join(models_dir(), "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf"), "qwen3moe", none)
            // the Q4_K_M twin fits a 16 GB card whole: the whole-model driver's MoE block (the router on the f16
            // cm2 GEMM, the per-row select, the bucket schedule, the combine-folded residual step, the token command's routed block)
            vk_cov_model(t, path_join(models_dir(), "Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local.gguf"), "qwen2moe", none)
            // the UD-IQ2_XXS 30B: the resident MoE block's s and e stamps over iq2xxs expert planes, whole on a 16 GB card
            vk_cov_model(t, path_join(models_dir(), "Qwen3-30B-A3B-Instruct-2507-UD-IQ2_XXS.gguf"), "qwen3moe", none)
            let knobs = gpu_knobs()   // the TTS rows serve on the tower knob: pinned on, restored after
            set_vulkan_tower(true)
            defer() {
                set_gpu_knobs(knobs)
            }
            cov_tower_tail_rows(t)
            vk_kernel_coverage_report()
            var cov <- vk_kernel_coverage()
            var served = 0
            for (n in values(cov)) {
                if (n > 0l) {
                    served++
                }
            }
            if (served == 0) {
                delete cov
                feint("no vulkan dispatches - arm DASLLAMA_GPU=1 on a device box (the census needs the serving tier)\n")
                return
            }
            for (blind in VK_CENSUS_NEVER_DISPATCHED) {
                var named = false
                for (k in keys(cov)) {
                    if (starts_with(k, blind)) {
                        named = true
                    }
                }
                // the entry must reach a COMPILED key: cov carries every seeded kernel at zero,
                // so an entry matching none names nothing and would sit green forever
                t |> success(named,
                    "{blind} names no seeded kernel - spell the census key (<vk_dispatch name>_spv)")
                let hits = named ? vk_kernel_coverage_of(blind) : 0l   //! the accessor panics on an unseeded name; the assert above is the legible red
                t |> success(hits == 0l,
                    "{blind} is a DECLARED blind spot - a stocked carrier now dispatches it ({hits}): drop the entry, its census row exists")
            }
            delete cov
            let famfilter = g_env_test.test_family
            if (empty(famfilter)) {
                // floor calibration: 25 measured on the default 5-model zoo (MoE rows skip without
                // PARITY_FULL; coopmat-mode variants and mode-gated twins legitimately idle here);
                // 24 catches a gross wiring regression without tripping on one near-boundary row
                t |> success(served >= 24, "the serving census dispatched {served} distinct kernels (rail floor 24)")
            } else {
                to_log(LOG_INFO, "census floor not asserted (family filter '{famfilter}'): {served} served\n")
            }
        } else {
            feint("vulkan is not built on this platform; nothing to census\n")
        }
    }
}

[test]
def test_vulkan_kernel_coverage_rail(tst : T?) {
    tst |> run("Vulkan kernel coverage rail: the lens spec seeds the census (device-free)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!arm_on(t, "coverage")) {
                return
            }
            //! the compile-time lens spec parses into the census universe, every kernel at zero after a reset - the
            //! cells before this one in the file may have dispatched on a live device
            vk_kernel_coverage_reset()
            var cov <- vk_kernel_coverage()
            t |> success(length(cov) >= 40, "the lens spec seeds {length(cov)} kernels (floor 40)")
            var zeros = 0
            for (n in values(cov)) {
                if (n == 0l) {
                    zeros++
                }
            }
            t |> equal(zeros, length(cov))
            delete cov
        } else {
            feint("vulkan is not built on this platform; nothing to census\n")
        }
    }
}
