options gen2
options _comment_hygiene = true

module dasllama_vulkan_common shared public
options _cyclomatic_complexity = 0   // the init/teardown state walks are the domain shape
options _function_length = 0   // same

require dasllama/dasllama_vulkan_kernels   // the *_spv blobs + the derived-access spec
require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_math
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_gemm_schema
require dasllama/dasllama_env
require dasllama/dasllama_config
require dasllama/dasllama_par
require math
require strings
require daslib/strings_boost
require daslib/fio   // has_env_variable — the VRAM-override probe

// The Vulkan backend's shared infrastructure: device state (GpuState/g_gpu), buffer/descriptor
// makers, upload + stacks + host-import, the weight arena, command/submit plumbing, the
// hazard-mask rail + derived-dispatch registry, the GPU profiler, the shared fill-meta family,
// and the single-op seams (vk_add_rms / vk_decode_attn / vk_rope_kv_store). Module-level state
// is deliberately non-private — an internal driver family (dasllama_metal_common's precedent);
// the drivers (dasllama_vulkan_decode / dasllama_vulkan_prefill) assign it.
//
// Submission model (v2): every stack carries its own descriptor set + host meta/xq/xs/y buffers,
// so descriptors are written ONCE at upload and command buffers are PRE-RECORDED (per stack, plus
// one per gate+up pair — the fused form the engine dispatches). Per token a dispatch is: refresh
// the mapped meta/activation contents, resubmit the recorded buffer, wait on the ONE reusable
// fence, memcpy y back. No per-dispatch allocation, recording, descriptor writes, or queue drain.

// ===== device state (process-lifetime; never finalized by design) =====

let WEIGHT_CAP = 4_200_000_000l    // default ceiling — NV/WDDM heapBudget IGNORES other
                                           // processes (reported 7608 of 8192MB under a 1.7GB desktop);
                                           // over-commit = silent SysMem demotion, act role 35x slower
let VRAM_RESERVE = 805_306_368l    // headroom below the queried heap budget when it clamps
                                           // the cap down: the tier's own scratch (~210MB batch
                                           // planes) + eviction slack
let STAGING_BYTES = 268_435_456l   // one upload chunk (largest single memcpy)
let XQ_BYTES = 1_048_576l          // per-stack activation quants capacity
let XS_BYTES = 262_144l            // ... their scales
let Y_BYTES = 1_048_576l           // per-stack output capacity
let META_BYTES = 4_096l            // params + region table
let MAX_STACKS = 384               // descriptor pool capacity. Dense/shexp/qkv rails add
                                           // ~250 small stacks on top of experts+cls; starving a
                                           // LATER walk section here costs far more than the pool
                                           // (a 224 cap silently cut 6 DN layers = -14% tg)
let WG_X = 64u                     // kernel workgroup width (spec-const axis 0 can retune it)
let BATCH_XQ_BYTES = 33_554_432l   // device-local gathered-activation scratch (chunk cap)
let BATCH_XS_BYTES = 8_388_608l    // ... its scale plane
let BATCH_Y_BYTES = 67_108_864l    // one output plane (two exist: gate/single + up)
let BATCH_META_BYTES = 262_144l    // per-stack batch params + regions + workgroup map
let BATCH_TILE = 32l               // kernel tile edge (rows and cols)

// the Q8_0 batch kernel's tile edge — 128 for the mul_mm L-tile kernel (modes 3/4), 32 otherwise.
// Meta builders MUST use the same edge the bound kernel derives (wtiles = ceil(d / edge)).
def q8_batch_tile : int64 => g_gpu.coopmat_mode >= 3 ? 128l : BATCH_TILE

let MM_SMALL_CNT = 192l   // measured crossover (qwen3-4B pp: sdot4 +42% at 128, L +2.5% at 192)

// mode-3 small-batch tier: 32 = sdot4 (default — beats BOTH coopmat tiles below the crossover:
// pp64 887 sdot4 / 677 M / 426 L), 64 = the coopmat M tile (the experiment arm), 128 = always-L
var g_mm_small = -1l

def mm_small_tile : int64 {
    if (g_mm_small < 0l) {
        let e = env_str("DASLLAMA_MM_SMALL", "")
        g_mm_small = e == "64" ? 64l : (e == "128" ? 128l : BATCH_TILE)   // else fails closed to sdot4
    }
    return g_mm_small
}

// the small-d cutoff (DASLLAMA_MM_SMALLD). k/v (d = kv_dim) gets only 32 wgs for 36 SMs at
// d 1024 / cnt 512 — but widening that grid MEASURES WORSE, so k/v is limited by the L-tile's
// arithmetic efficiency, not parallelism: pp512 L128 4297 > M64 4101 > sdot4-32 3906.
var g_mm_smalld = -1l

def mm_small_d : int64 {
    if (g_mm_smalld < 0l) {
        g_mm_smalld = env_int64("DASLLAMA_MM_SMALLD", 64l)
    }
    return g_mm_smalld
}

// per-dispatch tile router (the lcpp l/m ladder, ours resolved by measurement): in mm mode a
// small-cnt or small-d dispatch drops to the small tier — the L tile starves the grid and pads
// its B panel there. Pure in (d, cnt, mode), so pipe pick and meta fill can never disagree.
def q8_batch_tile_for(d, cnt : int64) : int64 {
    if (g_gpu.coopmat_mode < 3) {
        return q8_batch_tile()
    }
    return d <= mm_small_d() || cnt < MM_SMALL_CNT ? mm_small_tile() : 128l
}

// aligned-L eligibility: every tile interior (d and cnt both 128-multiples) — the guard-free
// pipeline. Pure in (d, cnt, mode) like the tile router, so pipe pick and meta fill agree.
def q8_batch_aligned(d, cnt : int64) : bool
    => d % 128l == 0l && cnt % 128l == 0l && q8_batch_tile_for(d, cnt) == 128l

def q8_batch_pipe_for(d, cnt : int64) : Pipeline
    => q8_batch_pipe(q8_batch_tile_for(d, cnt), q8_batch_aligned(d, cnt))

// a batch dispatch's tile pick + (rows, cols) edges: the cm2 twin (fmt 6) tiles 64w x 128t, q8
// rides the router above, kq the fixed BATCH_TILE — one source so wg counts, meta fills, and the
// pipe pick can never disagree
def batch_tile_edges(fmt : int; d, cnt : int64) : tuple<tile : int64; td : int64; tc : int64> {
    let tile = fmt == 0 ? q8_batch_tile_for(d, cnt) : BATCH_TILE
    return (tile = tile, td = fmt == 6 ? 64l : tile, tc = fmt == 6 ? 128l : tile)
}

// resident prefill GEMM pipe: a fmt-6 role rides the cm2 twin, everything else the q8 tile router
def pf_gemm_pipe(fmt6 : bool; d, cnt : int64) : Pipeline {
    if (fmt6) {
        return weak_copy(g_gpu.batch_cm2)
    }
    return q8_batch_pipe_for(d, cnt)
}
let BATCH_CHUNK_ROWS = 8_192l      // dispatch row-window cap (bounds y and the wg map)
let BATCH_HQ_BYTES = 33_554_432l   // device requantized-hidden quants (the down GEMM's activations)
let BATCH_HS_BYTES = 8_388_608l    // ... its scale plane
let FFN_META_BYTES = 64l           // act/requant params (element count, gelu flag, block count)
                                           // + the combine's [n, k, r0, r1] at words 4..7
let COMB_WI_BYTES = 2_097_152l     // combine w / inv plane cap (npos*k grid entries x 4B)
let CLS_META_BYTES = 64l           // classifier GEMV params (one region, filled once)
var g_coopmat_mode_force = -1

//! Test hook: pin the GEMM mode (0 sdot4 / 1 f16 / 2 int8 / 3 mm) ahead of the mm default and
//! DASLLAMA_COOPMAT — must run before the tier's lazy device init. The CPU-reference tier tests
//! pin 0 so their oracles stay reference-kernel-deterministic under any router default.
def vk_force_coopmat_mode(m : int) {
    g_coopmat_mode_force = m
}

let STREAM_RESERVE = 1_100_000_000l  // LEGACY fallback stream-slot carve — used only when
                                             // the loader never reported the model's exact need
                                             // (moe_gpu_stream_need; big-expert geometries like
                                             // GLM Air exceed this constant by design)

// deltanet chain capacities (npos chunks over DN_WINDOW windows, state carried device-side).
// Maxima match qwen35 (ds 128, 32 v-heads, conv dim 8192, dconv 4) — vk_moe_dn asserts them
// and the engine arm fail-closes on bigger models.
let DN_CS = 64l
let DN_MAX_DS = 128l
let DN_MAX_HEADS = 32l
let DN_MAX_CD = 8_192l
let DN_MAX_DCONV = 4l
let DN_WINDOW = 1_024l
let DN_NCHUNKS = DN_WINDOW / DN_CS
// phase-1 slab per (head x chunk): 7 CS x ds panels + 2 CS x CS tiles + 4 CS smalls; phase-2
// per-head area: state working copy + och rows. ~155MB total — the price of the 2-phase scan
let DN_SLAB = 7l * DN_CS * DN_MAX_DS + 2l * DN_CS * DN_CS + 4l * DN_CS
let DN_P2_OFF = DN_MAX_HEADS * DN_NCHUNKS * DN_SLAB
let DN_P2_SLAB = DN_MAX_DS * DN_MAX_DS + DN_CS * DN_MAX_DS
let DN_O_OFF = DN_P2_OFF + DN_MAX_HEADS * DN_P2_SLAB
let DN_WS_BYTES = (DN_O_OFF + DN_WINDOW * DN_MAX_HEADS * DN_MAX_DS) * 4l
let DN_STATE_BYTES = DN_MAX_HEADS * DN_MAX_DS * DN_MAX_DS * 4l
let DN_META_BYTES = 128l
// smalls plane float offsets, ending in the conv history region — the per-window smalls
// re-stage stops SHORT of it so it never clobbers the device-side prev-window tail
let DN_SM_BETA = 16l
let DN_SM_G = DN_SM_BETA + DN_WINDOW * DN_MAX_HEADS
let DN_SM_TAPS = DN_SM_G + DN_WINDOW * DN_MAX_HEADS
let DN_SM_WNORM = DN_SM_TAPS + DN_MAX_CD * DN_MAX_DCONV
let DN_SM_A = DN_SM_WNORM + DN_MAX_DS
let DN_SM_DT = DN_SM_A + DN_MAX_HEADS
let DN_SM_HIST = DN_SM_DT + DN_MAX_HEADS
let DN_SMALLS_BYTES = (DN_SM_HIST + DN_MAX_CD * (DN_MAX_DCONV - 1l) + 64l) * 4l
let DN_TAIL_BYTES = DN_MAX_CD * (DN_MAX_DCONV - 1l) * 4l

// decode-step scratch: the raw β/γ staging (+ the ring parity word) and the o-row Q8_0 image
// the out GEMV reads. Step smalls carry a SECOND conv-ring image after the shared layout — the
// fused kernel reads one parity image and writes the other (readers span workgroups).
let DND_BG_BYTES = (2l * DN_MAX_HEADS + 4l) * 4l
let DND_OQ_BYTES = DN_MAX_HEADS * DN_MAX_DS
let DND_OS_BYTES = (DN_MAX_HEADS * DN_MAX_DS / 32l) * 4l
let DND_SMALLS_BYTES = DN_SMALLS_BYTES + DN_MAX_CD * (DN_MAX_DCONV - 1l) * 4l

// heat-cache pool stacks live in a SYNTHETIC element-offset space far above any real plane
// offset, so find_stack / fill_stack_meta_rows / the whole FFN GEMV rail run verbatim on them
let HEAT_BASE = 1l << 42l
let HEAT_STRIDE = 1l << 32l          // per pool-stack offset room (>> nslots * ege)
let HEAT_UPLOAD_CAP = 2              // steady-state expert uploads per advise call
let HEAT_UPLOAD_CAP_FILL = 8         // while the pool is underfull (warmup fills fast)

// attention chain capacities: roped K / raw V persist device-side at ABSOLUTE batch positions
// across windows, so later windows attend the whole batch — the engine arm gates on start_pos == 0.
// Maxima cover the qwen3 family; vk_moe_attn asserts them (fail-closed on bigger geometry).
let AT_WINDOW = 512l
let AT_CTX = 4_096l
let AT_MAX_QD = 8_192l
let AT_MAX_KV = 1_024l
let AT_MAX_HS = 256l
let AT_META_BYTES = 128l
let AR_MAX_DIM = 4_096l   // the add+rms row cap — == the ar_row workgroup slab
// smalls plane float offsets: [eps, scale] consts, the per-head qk-rms weight rows, then the
// window's cos/sin rope rows packed contiguously (sin offset = AT_SM_COS + rows*half, meta [13])
let AT_SM_RMSQ = 16l
let AT_SM_RMSK = AT_SM_RMSQ + AT_MAX_HS
let AT_SM_COS = AT_SM_RMSK + AT_MAX_HS
let AT_SMALLS_BYTES = (AT_SM_COS + 2l * AT_WINDOW * (AT_MAX_HS / 2l) + 64l) * 4l

struct HostBuf {
    buf : uint64       // VkBuffer
    mem : uint64       // VkDeviceMemory
    mapped : void?     // persistently mapped (host-coherent)
    bytes : int64
}

struct VkStack {
    base : int64             // first weight-element offset covered (into the q8 plane)
    elems : int64            // element count (offs resolve against [base, base+elems))
    fmt : int                // int(KqFmt): 0 = q8, 1/2/3 = k4/k5/k6 — picks kernels + meta units
    wqbytes : int64          // quant plane bytes (q8: elems; kq: elems/256 * {128,160,192})
    wqbuf : uint64           // device-local row-major quants (bound as array<uint>)
    wsbuf : uint64           // device-local scale plane (q8: f16 halfwords; kq: 20B decoded rows)
    wsbytes : int64
    // decode/GEMV dispatch state (lazy — first GEMV or cls use creates it; dense-GEMM stacks
    // never do, saving ~2.3MB host + 1MB device per stack across the ~130 dense planes)
    raw_set : VkDescriptorSet   // this stack's 6 bindings, written on first GEMV use
    meta : HostBuf
    xq : HostBuf
    xs : HostBuf
    y : HostBuf              // host-cached readback staging (the same-submit DMA copy lands here)
    y_dev : uint64           // device-local y the kernel writes — see batch_y1_dev on why
    gemv_made : bool
    batch_meta : HostBuf     // prefill batch meta STAGING (lazy — first batch dispatch creates it)
    batch_meta_dev : uint64  // device copy the kernel reads — a host-visible meta costs ~26us of
                             // serialized uncached PCIe reads PER WORKGROUP (measured, 12K wgs/dispatch)
    batch_set : VkDescriptorSet   // batch bindings: stack planes + act-image scratch + a y plane
    batch_y : uint64         // which shared y plane batch_set binds (role-fixed per stack)
    batch_xq : uint64        // which activation-quant plane it binds (gate/up: gathered x; down: hq)
    batch_tile : int64       // the tile edge the last fill_batch_meta routed — the bind must match
    batch_aligned : bool     // ... and whether that fill qualified for the guard-free aligned-L pipe
    batch_made : bool
    // classifier dispatch state (lazy — set up on the first cls GEMV against this stack)
    cls_set : VkDescriptorSet
    cls_meta_dev : uint64    // device meta — vocab-row dispatches run ~150K subgroups, each
                             // reading params; host-visible meta would be uncached PCIe reads
    cls_cmd : VkCommandBuffer
    cls_made : bool
}

// one pinned-host-mirror weight stack of a STREAMED layer (device-layout planes kept in
// host-visible memory, DMA'd into a slot per prefill). Stacks arrive gate/up/down per layer
// (loader walk order, descending layers) — group g = stacks [3g, 3g+3), runtime-next = g-1.
struct StreamStack {
    base : int64
    elems : int64
    fmt : int
    wqbytes : int64
    wsbytes : int64
    hwq : HostBuf            // pinned mirror, quant plane (transfer_src)
    hws : HostBuf            // ... scale plane
}

// one streaming slot: 3 pseudo-stacks (entries in g_gpu.stacks whose plane buffers are the
// slot's device scratch, re-based per layer so find_stack + the whole batch rail work verbatim)
struct StreamSlot {
    group : int              // stream group currently resident in the slot (-1 = empty)
    s1 : int                 // pseudo-stack indices: gate / up / down
    s3 : int
    s2 : int
    wait_value : uint64      // timeline value of the slot's in-flight transfer-queue fill (0 = none)
}

// pre-recorded decode-form FFN chain per (gate, up, down) stack triple: GEMV pair, fused
// act+requant, down GEMV, y DMA — refreshed per token via the stacks' host-visible metas,
// resubmitted as-is
struct FfnCmd {
    cmd : VkCommandBuffer
    actrq_set : VkDescriptorSet   // gate y_dev + up y_dev (vk_upf) -> hq/hs device planes
    down_set : VkDescriptorSet    // down stack planes + hq/hs as activations + its y_dev
    nrows : int64                 // row count cmd was recorded with
}

// one recurrent layer's pre-recorded deltanet decode-step chain + its device-resident state.
// valid: the device state/ring/colds match the engine's layer state (cold-upload when not);
// dirty: the device advanced past the CPU copy — the flush hook syncs before any CPU read.
struct DnStep {
    state_dev : uint64       // nvh·ds·ds f32 — the fused step advances it in place (its binding 1)
    smalls_dev : uint64      // per-layer smalls: colds + the two parity ring images + the β/γ region
    z_set : VkDescriptorSet  // z GEMV rerouted into the shared y buffer (meta y base = cd)
    fused_set : VkDescriptorSet
    out_set : VkDescriptorSet
    cmd : VkCommandBuffer    // the warm step chain, recorded once — no per-token re-record
    s_qkv : int
    s_z : int
    s_o : int
    state_bytes : int64
    ring_bytes : int64
    parity : int             // ring image holding current history (the kernel writes the other)
    valid : bool
    dirty : bool
}

struct QkvCmd {
    cmd : VkCommandBuffer   // the warm q/k/v group, recorded once (planes fixed per layer)
    k_set : VkDescriptorSet // k/v read the Q stack's shared acts image and write its shared y
    v_set : VkDescriptorSet
    sq : int
    ybytes : int64
}

struct HeatPool {
    s1 : int = -1                 // pool stacks in the synthetic offset space (-1 = unservable layer)
    s3 : int = -1
    s2 : int = -1
    src1 : int = -1               // stream-mirror stacks (the upload source planes)
    src3 : int = -1
    src2 : int = -1
    ege : int64                   // n*nfe elements per expert per plane
    nslots : int
    slot_expert : array<int64>    // slot -> pinned expert (-1 free)
    emap : table<int64; int>      // expert -> slot
}

struct GpuState {
    instance : Instance
    device : Device
    phys : VkPhysicalDevice
    fam : uint
    queue : VkQueue
    pool : CommandPool
    set_layout : DescriptorSetLayout
    desc_pool : DescriptorPool
    pipe_layout : PipelineLayout
    pipeline : Pipeline
    batch_pipeline : Pipeline   // prefill batched-GEMM kernel (same set/pipe layout)
    batch_coopmat_f16 : Pipeline  // prefill Q8_0 GEMM on tensor cores, f16-dequant (DASLLAMA_COOPMAT=f16)
    batch_coopmat_i8 : Pipeline   // prefill Q8_0 GEMM on tensor cores, native s8 (DASLLAMA_COOPMAT=int8)
    batch_coopmat_q40 : Pipeline  // prefill Q4_0 GEMM on tensor cores, f16-dequant (COOPMAT=f16, fmt 4)
    batch_coopmat_mm : Pipeline   // prefill Q8_0 GEMM, the mul_mm 128x128 L-tile (DASLLAMA_COOPMAT=mm)
    batch_coopmat_mm_m : Pipeline // ... its 64x64 M-tile twin, routed in for small-cnt dispatches
    batch_coopmat_mm_a : Pipeline // ... the aligned-L twin (no staging guards/edge path — 2.2x the L rate)
    batch_cm2 : Pipeline          // the cm2 decode-in-load prefill twin (mode 4, fmt-6 q8n stacks)
    gemv_q8n : Pipeline           // the q8n native-block decode GEMV (mode 4's fmt-6 groupn twin)
    xf16_pipeline : Pipeline      // cm2 chain: Q8_0 activation image -> f16 plane
    actf16_pipeline : Pipeline    // cm2 chain: act(g)*u -> f16 hidden plane (the actrq stand-in)
    f16cvt_pipeline : Pipeline    // arena cm2 chain: f32 rows -> f16 plane (the quant-role stand-in)
    actrq_pipeline : Pipeline   // fused act(g)*u + q8_0 requant of the hidden rows
    gemv_k4 : Pipeline          // native K-quant GEMVs (per-format decode kernels)
    gemv_k5 : Pipeline
    gemv_k6 : Pipeline
    gemv_q40 : Pipeline
    actrq_k_pipeline : Pipeline // fused act + Q8_K requant twin (per-256 scale) for the kq chains
    batch_k4 : Pipeline         // native K-quant prefill tiles (per-format batch kernels)
    batch_k5 : Pipeline
    batch_k6 : Pipeline
    batch_q40 : Pipeline
    comb_pipeline : Pipeline    // routed weighted-sum combine (device-side reduce, prefill batches)
    dn_conv_pipeline : Pipeline // deltanet conv + SiLU + q/k L2-norm (one workgroup per position)
    dnd_fused_pipeline : Pipeline // deltanet decode step, whole chain fused (one workgroup per v-head)
    dn_scan_p1_pipeline : Pipeline // deltanet scan phase 1: chunk-local pieces (wg per head x chunk)
    dn_scan_p2_pipeline : Pipeline // deltanet scan phase 2: sequential state chain (wg per v-head)
    dn_rq_pipeline : Pipeline   // plain Q8_0 requant of the scan's o rows (out-proj image)
    at_prep_q_pipeline : Pipeline   // attention q(|gate) deinterleave + qk-rms + partial rope
    at_prep_k_pipeline : Pipeline   // attention k rms + rope, written at absolute batch positions
    at_attn_pipeline : Pipeline     // flash-style causal attention (GQA, online softmax, out-gate)
    q8k_rq_pipeline : Pipeline      // plain Q8_K requant (the kq wo GEMM's activation image)
    ar_pipeline : Pipeline          // add + RMSNorm, one wg per row (meta[1]=0 = norm only); decode dispatches 1 wg
    ar_rq_pipeline : Pipeline       // resident driver: the above + the Q8_0 requant, one pass (DASLLAMA_VK_FUSE)
    rks_pipeline : Pipeline         // resident driver: fused rope + KV-row store (f32 mirror)
    qkn_rope_pipeline : Pipeline    // resident driver: per-head q/k rmsnorm + rope + KV store, one pass
    da_pipeline : Pipeline          // resident driver: decode attention over the f32 KV mirror
    rks_b_pipeline : Pipeline       // prefill: batched rope + KV-store
    da_b_pipeline : Pipeline        // prefill: batched causal attention (generic hs)
    da_b128_pipeline : Pipeline     // prefill: the hs=128 coopmat flash-attention twin (coopmat devices)
    qkn_pipeline : Pipeline         // resident driver: per-head q/k rmsnorm (qk_norm models), decode + prefill
    staging : HostBuf
    fence : VkFence          // the one reusable submit fence (reset after every wait)
    rows_per_wg : int64      // subgroups per workgroup = output rows per workgroup
    has_coopmat : bool       // device supports VK_KHR_cooperative_matrix (the f16/int8 tensor tiles)
    has_coopmat2 : bool      // device supports NV_cooperative_matrix2 (tensor-addressed wg tiles + decode fns)
    coopmat_mode : int       // Q8_0 prefill GEMM: 0 = sdot4, 1 = f16 coopmat, 2 = int8 coopmat, 3 = mul_mm L-tile (default), 4 = cm2 decode-in-load
    weight_budget : int64    // resident-weight cap — queried heap budget minus reserve, or VRAM_BUDGET
    dry : bool               // OFFLINE BAKE: accept/refuse arithmetic runs, every device call is gated off
    msr : int64              // maxStorageBufferRange — device-queried at init, config-supplied when dry
    @do_not_delete stacks : array<VkStack>
    // the resident driver's weight arenas, one per format present (see the arena section). Separate
    // from `stacks` on purpose: the two tiers never co-arm on one model, and the arena has no
    // per-plane descriptor set to cap.
    arenas : table<int; ArenaFmt>
    // residual-stream state (lazy — the add+rms kernel's buffers and its one descriptor set)
    ar_x_dev : uint64
    ar_a_dev : uint64
    ar_w_dev : uint64
    ar_y_dev : uint64
    ar_meta : HostBuf
    ar_scal : HostBuf
    ar_host : HostBuf
    ar_set : VkDescriptorSet
    ar_ready : bool
    // arena FFN chain state (gate/up/act+requant/down as one submit against arena-resident planes)
    af_meta_g : HostBuf
    af_meta_u : HostBuf
    af_meta_d : HostBuf
    af_xq : HostBuf
    af_xs : HostBuf
    af_ya_dev : uint64        // gate output plane
    af_yb_dev : uint64        // up output plane
    af_yo_dev : uint64        // down output
    af_yo_host : HostBuf
    af_gate_set : VkDescriptorSet
    af_up_set : VkDescriptorSet
    af_actrq_set : VkDescriptorSet
    af_down_set : VkDescriptorSet
    af_ready : bool
    // rope+KV-store state: qkv in, roped q out, K/V planes = one contiguous device KV scratch
    rks_qkv_dev : uint64
    rks_q_dev : uint64
    rks_scal : HostBuf
    rks_meta : HostBuf
    rks_k_dev : uint64        // f32 K mirror (rows x kv_dim, absolute positions)
    rks_v_dev : uint64        // f32 V mirror
    rks_q_host : HostBuf
    rks_kv_host : HostBuf      // K then V readback (test/parity only; the driver keeps them device-side)
    rks_set : VkDescriptorSet
    rks_ready : bool
    // decode-attention state: q in, out; reads the rks K/V mirror planes
    da_q_dev : uint64
    da_scal : HostBuf
    da_meta : HostBuf
    da_out_dev : uint64
    da_out_host : HostBuf
    da_set : VkDescriptorSet
    da_ready : bool
    ffn_cmds : table<int64; FfnCmd>   // (gate, up, down stack idx triple) -> decode FFN chain
    resident_bytes : int64
    // heat-pinned expert cache (decode split-FFN) + the one-in-flight async FFN discipline
    heat_pools : table<int64; HeatPool>   // layer -> slot pool
    ffn_pend_s2 : int = -1        // down stack of the pending async FFN (-1 = none in flight)
    ffn_pend_bytes : int64
    qkv_cmds : table<int64; QkvCmd>   // q plane offset -> the layer's decode q/k/v group
    dev_mem : table<uint64; uint64>   // device buffer -> its VkDeviceMemory (rollback destroy needs it)
    dev_mapped : table<uint64; uint64>   // ReBAR: device buffer -> persistent CPU mapping (uploads write direct)
    rebar_type : int = -1         // DL|HV|HC memory type on the full-VRAM heap (-1 = staging path only)
    has_memprio : bool            // device enabled VK_EXT_memory_priority — allocations chain priority 1.0
    host_mem : table<uint64; uint64>  // host-visible buffer -> memory: every make_host_buf result except
                                      // the process-lifetime pair (staging, profiler readback) — the
                                      // whole-model drop sweeps this
    model_cmds : array<VkCommandBuffer>   // every alloc_cmd result — freed together at model drop
    // FFN mid-chain state (lazy — first FFN dispatch creates it)
    hq_dev : uint64          // device requantized-hidden quants (the down GEMM's activation image)
    hs_dev : uint64          // ... its scales
    ffn_meta : HostBuf       // act/requant params staging
    ffn_meta_dev : uint64
    ffn_ready : bool
    // prefill batch state (lazy — first batch dispatch creates it; decode-only runs skip it)
    batch_xq_dev : uint64    // device-local gathered-activation quants (chunk window)
    batch_xs_dev : uint64    // ... their scales
    // y is written DEVICE-LOCAL and DMA-copied out in the same submit: shader scatter-stores
    // straight into host-visible memory measured ~100MB/s (~350ms per pp512 down dispatch);
    // the copy engine moves the same bytes in ~2ms
    batch_y1_dev : uint64    // gate / single output plane (kernel writes; also the down output)
    batch_y2_dev : uint64    // up output plane (the fused pair's second write target)
    batch_y1 : HostBuf       // host-cached readback staging for batch_y1_dev
    batch_y2 : HostBuf       // ... and batch_y2_dev (idle since the FFN fold — kept for debug taps)
    batch_actrq_set : VkDescriptorSet   // batch twin of FfnCmd's fused act+requant set (y1/y2 planes)
    batch_cmd : VkCommandBuffer   // one reusable batch cmd buffer, re-recorded per submit
    batch_ready : bool
    // cm2 batch state (lazy — first q8n batch dispatch creates it; mode 4 only)
    batch_xf_dev : uint64    // f16 gathered-activation plane (the gate/up twins' B side)
    batch_hf_dev : uint64    // f16 hidden plane (the down twin's B side)
    batch_xf_set : VkDescriptorSet   // the Q8_0 -> f16 dequant dispatch
    batch_hf_set : VkDescriptorSet   // the act -> f16 dispatch
    cm2_batch_ready : bool
    // combined-output state (lazy — first combined prefill dispatch creates it)
    comb_stage : HostBuf     // staging: routing w plane + grid->bucket map (device copies at first-chunk head)
    comb_w_dev : uint64
    comb_inv_dev : uint64
    comb_y_dev : uint64      // accumulated npos x n output (the combine kernel's cross-chunk accumulator)
    comb_y : HostBuf         // host-cached readback staging
    comb_set : VkDescriptorSet
    comb_ready : bool
    // streamed-prefill state (lazy — first streamed dispatch creates the slots)
    @do_not_delete stream_stacks : array<StreamStack>
    stream_slots : array<StreamSlot>
    stream_cmd : VkCommandBuffer  // reusable plane-copy cmd (safe: a fenced chain wait always
                                  // intervenes between two recordings — see copy_stream_group)
    stream_max_gu_wq : int64      // per-plane capacity maxima across streamed stacks
    stream_max_gu_ws : int64
    stream_max_dn_wq : int64
    stream_max_dn_ws : int64
    stream_ready : bool
    stream_reserve : int64        // bytes carved out of weight_budget for the 2 stream slots
    // dedicated transfer family (phase 5): mirror->slot copies overlap compute, handed off via a
    // timeline semaphore the next compute submit waits on. -1 = single-queue mode (the old rail).
    xfer_fam : int = -1
    xfer_queue : VkQueue
    xfer_pool : CommandPool
    xfer_cmd : VkCommandBuffer
    xfer_sem : VkSemaphore
    xfer_value : uint64           // last signaled timeline value
    pend_wait : uint64            // value the NEXT compute submit must wait for (0 = none)
    // deltanet chain state (lazy — first DN dispatch creates it)
    dn_meta : HostBuf             // dn kernel params staging
    dn_meta_dev : uint64
    dn_smalls : HostBuf           // beta/g/taps/wnorm/a/dt/hist staging
    dn_smalls_dev : uint64
    dn_state_stage : HostBuf      // f32 state upload staging (chain start)
    dn_state_host : HostBuf       // host-cached state readback (chain end)
    dn_tail_host : HostBuf        // host-cached conv-tail readback (the caller's history store)
    dn_state_dev : uint64
    dn_ws_dev : uint64            // per-head scan slabs + the gated o rows
    dn_conv_set : VkDescriptorSet
    dn_scan_set : VkDescriptorSet
    dn_rq_set : VkDescriptorSet
    dn_ready : bool
    dn_shared_ok : bool  // device shared-mem cap fits dn_conv's 33 KB tile (false on Apple/MoltenVK)
    // deltanet decode-step state (lazy — first npos == 1 DN dispatch creates it)
    dnd_meta : HostBuf            // step meta staging (npos = 1 geometry, filled once)
    dnd_bg : HostBuf              // per-token raw β/γ + ring-parity staging (copied into the layer smalls)
    dnd_oq_dev : uint64           // the o row's Q8_0 image (fused-step out, the out GEMV's activations)
    dnd_os_dev : uint64
    dnd_steps : table<int64; DnStep>   // qkv plane offset -> the layer's step chain
    dnd_ready : bool
    // attention chain state (lazy — first attention dispatch creates it)
    at_meta : HostBuf             // attention kernel params staging
    at_meta_dev : uint64
    at_smalls : HostBuf           // eps/scale/rms rows + the window's cos/sin rope rows
    at_smalls_dev : uint64
    at_q_dev : uint64             // the window's normed+roped q rows
    at_g_dev : uint64             // gate rows in, attention o rows out (same element, same thread)
    at_k_dev : uint64             // roped k rows at ABSOLUTE batch positions (windows accumulate)
    at_v_dev : uint64             // raw v rows at absolute positions
    at_vraw_dev : uint64          // the v GEMM's window-relative output (device-copied into at_v_dev)
    at_kv_host : HostBuf          // host-cached k/v readback (k at 0, v at AT_WINDOW*AT_MAX_KV floats)
    at_prep_q_set : VkDescriptorSet
    at_prep_k_set : VkDescriptorSet
    at_attn_set : VkDescriptorSet
    at_rq_set : VkDescriptorSet
    at_ready : bool
}

var g_gpu : GpuState?
var g_init_failed = false
var g_vk_prof = false      // DASLLAMA_GPU_PROF=1: per-piece batch-chain timing to log
var g_vk_prof_probed = false
var g_vk_upload_us = 0l
var g_vk_upload_bytes = 0l

def vk_prof : bool {
    if (!g_vk_prof_probed) {
        g_vk_prof_probed = true
        g_vk_prof = env_flag("DASLLAMA_GPU_PROF", false)
    }
    return g_vk_prof
}

def dev_raw : VkDevice {
    return boost_value_to_vk(g_gpu.device)
}

// host-visible memory comes in two flavors on discrete cards: write-combined (fast CPU writes,
// ~100x SLOWER CPU reads) and host-cached. Buffers the CPU reads back (the y planes) MUST be
// cached — a 33MB memcpy out of a WC mapping measured ~400ms/layer at pp512
def find_host_mem(type_bits : uint; cached : bool) : uint {
    if (cached) {
        var mp : VkPhysicalDeviceMemoryProperties
        vkGetPhysicalDeviceMemoryProperties(g_gpu.phys, mp)
        for (i in range(int(mp.memoryTypeCount))) {
            let fl = mp.memoryTypes[i].propertyFlags
            if ((type_bits & (1u << uint(i))) != 0u && fl.host_visible && fl.host_coherent && fl.host_cached) {
                return uint(i)
            }
        }
    }
    var want : VkMemoryPropertyFlags
    want.host_visible = true
    want.host_coherent = true
    return find_memory_type(g_gpu.phys, type_bits, want)
}

def make_host_buf(bytes : int64; storage : bool; cached : bool = false; xfer_shared : bool = false; soft : bool = false) : HostBuf {
    var bci : BufferCreateInfo
    bci.size = uint64(bytes)
    bci.usage.storage_buffer = storage
    bci.usage.transfer_src = !storage
    bci.usage.transfer_dst = true   // readback staging targets (y DMA copies land here)
    if (xfer_shared && g_gpu.xfer_fam >= 0) {
        // stream mirrors: transfer-queue fills + compute-queue heat slices — CONCURRENT dodges ownership transfers
        bci.sharingMode = VkSharingMode.CONCURRENT
        bci.pQueueFamilyIndices <- [g_gpu.fam, uint(g_gpu.xfer_fam)]
    }
    var b <- create_buffer(g_gpu.device, bci)   // deliberately never finalized (process-lifetime)
    let req = get_buffer_memory_requirements(g_gpu.device, b)
    let mai = MemoryAllocateInfo(allocationSize = req.size,
                                 memoryTypeIndex = find_host_mem(req.memoryTypeBits, cached))
    var ares = VkResult.SUCCESS
    var m <- allocate_memory(g_gpu.device, mai, unsafe(addr(ares)))
    if (ares != VkResult.SUCCESS) {
        // soft callers (stream mirrors) degrade — the walk treats it as its budget stop
        if (!soft) {
            panic("dasLLAMA vulkan tier: pinned host alloc failed ({ares}, {bytes} bytes, type {mai.memoryTypeIndex})")
        }
        to_log(LOG_INFO, "dasLLAMA vulkan tier: pinned mirror alloc declined ({ares}, {bytes} bytes, type {mai.memoryTypeIndex}) — streamed walk stops here\n")
        unsafe {
            vkDestroyBuffer(dev_raw(), reinterpret<VkBuffer>(b._vk), null)
        }
        return HostBuf()
    }
    bind_buffer_memory(g_gpu.device, b, m, 0ul)
    var mapped : void? = null
    let mf : VkMemoryMapFlags
    unsafe {
        vk_check(vkMapMemory(dev_raw(), boost_value_to_vk(m), 0ul, uint64(bytes), mf, addr(mapped)), null)
    }
    g_gpu.host_mem |> insert(b._vk, m._vk)
    return HostBuf(buf = b._vk, mem = m._vk, mapped = mapped, bytes = bytes)
}

def make_device_buf(bytes : int64; xfer_shared : bool = false) : uint64 {
    var bci : BufferCreateInfo
    bci.size = uint64(bytes)
    bci.usage.storage_buffer = true
    bci.usage.transfer_dst = true
    bci.usage.transfer_src = true   // y planes DMA back out (device -> host staging)
    if (xfer_shared && g_gpu.xfer_fam >= 0) {
        // stream slots: transfer-queue fills + compute-queue reads — CONCURRENT dodges ownership transfers
        bci.sharingMode = VkSharingMode.CONCURRENT
        bci.pQueueFamilyIndices <- [g_gpu.fam, uint(g_gpu.xfer_fam)]
    }
    var b <- create_buffer(g_gpu.device, bci)
    let req = get_buffer_memory_requirements(g_gpu.device, b)
    // priority 1.0 (default 0.5): under pressure the driver demotes lower-priority (desktop) memory first
    var prio = VkMemoryPriorityAllocateInfoEXT()
    prio.priority = 1.0
    if (g_gpu.rebar_type >= 0 && (req.memoryTypeBits & (1u << uint(g_gpu.rebar_type))) != 0u) {
        // ReBAR attempt — raw alloc so a decline falls back to plain device-local below
        var rmai = VkMemoryAllocateInfo()
        rmai.allocationSize = req.size
        rmai.memoryTypeIndex = uint(g_gpu.rebar_type)
        if (g_gpu.has_memprio) {
            unsafe {
                rmai.pNext = addr<void?>(prio)
            }
        }
        var rmem : VkDeviceMemory
        if (vkAllocateMemory(dev_raw(), rmai, null, rmem) == VkResult.SUCCESS) {
            vk_check(vkBindBufferMemory(dev_raw(), unsafe(reinterpret<VkBuffer>(b._vk)), rmem, 0ul), null)
            var mapped : void? = null
            let mf : VkMemoryMapFlags
            unsafe {
                vk_check(vkMapMemory(dev_raw(), rmem, 0ul, req.size, mf, addr(mapped)), null)
            }
            g_gpu.dev_mem |> insert(b._vk, unsafe(reinterpret<uint64>(rmem)))
            g_gpu.dev_mapped |> insert(b._vk, unsafe(reinterpret<uint64>(mapped)))
            return b._vk
        }
    }
    var want : VkMemoryPropertyFlags
    want.device_local = true
    var mai = MemoryAllocateInfo(allocationSize = req.size,
                                 memoryTypeIndex = find_memory_type(g_gpu.phys, req.memoryTypeBits, want))
    if (g_gpu.has_memprio) {
        unsafe {
            mai.next = addr<void?>(prio)
        }
    }
    var m <- allocate_memory(g_gpu.device, mai)
    bind_buffer_memory(g_gpu.device, b, m, 0ul)
    g_gpu.dev_mem |> insert(b._vk, m._vk)
    return b._vk
}

def destroy_device_buf(h : uint64) {
    unsafe {
        vkDestroyBuffer(dev_raw(), reinterpret<VkBuffer>(h), null)
        if (key_exists(g_gpu.dev_mem, h)) {
            vkFreeMemory(dev_raw(), reinterpret<VkDeviceMemory>(g_gpu.dev_mem[h]), null)   // implicitly unmaps
            g_gpu.dev_mem |> erase(h)
        }
    }
    g_gpu.dev_mapped |> erase(h)
}

def destroy_host_buf(hb : HostBuf) {   // vkFreeMemory implicitly unmaps
    unsafe {
        vkDestroyBuffer(dev_raw(), reinterpret<VkBuffer>(hb.buf), null)
        vkFreeMemory(dev_raw(), reinterpret<VkDeviceMemory>(hb.mem), null)
    }
    g_gpu.host_mem |> erase(hb.buf)
}

def nonowning_buf(h : uint64) : Buffer {
    var b : Buffer
    b._vk = h
    return b
}

// The coopmat-mode ladder, shared by vk_moe_init and the DlimConfiguration source — one
// resolver, so the config's recorded mode can never drift from the mode the device runs.
def resolve_coopmat_mode(has_cm, has_cm2 : bool) : int {
    // fastest MEASURED default (mm; cm2 opt-in until it beats mm); DASLLAMA_COOPMAT overrides
    var cmode = has_cm ? 3 : 0
    if (has_cm) {
        let mode = env_str("DASLLAMA_COOPMAT", "")
        if (mode == "sdot4") {
            cmode = 0
        } elif (mode == "f16") {
            cmode = 1
        } elif (mode == "int8") {
            cmode = 2
        } elif (mode == "mm") {
            cmode = 3
        } elif (mode == "cm2" && has_cm2) {
            cmode = 4
        }
    }
    if (g_coopmat_mode_force >= 0) {
        cmode = has_cm ? g_coopmat_mode_force : 0
    }
    if (cmode == 4 && !has_cm2) {
        cmode = 3   // forced/requested cm2 without the extension: mm
    }
    return cmode
}

struct VkHeapInfo {
    budget : int64   // largest device-local heap's current budget (0 = no VK_EXT_memory_budget)
    used : int64
    size : int64
}

def query_heap_info(phys : VkPhysicalDevice) : VkHeapInfo {
    var r = VkHeapInfo()
    // heap SIZE needs no extension — only the live budget/usage ride VK_EXT_memory_budget
    let has_budget = device_extension_available(phys, "VK_EXT_memory_budget")
    var mb = VkPhysicalDeviceMemoryBudgetPropertiesEXT()   // sType pre-filled by the ctor
    var mp2 = VkPhysicalDeviceMemoryProperties2()
    if (has_budget) {
        unsafe {
            mp2.pNext = addr<void?>(mb)
        }
    }
    vkGetPhysicalDeviceMemoryProperties2(phys, mp2)
    var best = -1
    for (i in range(int(mp2.memoryProperties.memoryHeapCount))) {
        if (!mp2.memoryProperties.memoryHeaps[i].flags.device_local) {
            continue
        }
        // rank by live budget when the ext reports it, by raw size otherwise
        if (best < 0) {
            best = i
        } elif (has_budget ? mb.heapBudget[i] > mb.heapBudget[best]
                : mp2.memoryProperties.memoryHeaps[i].size > mp2.memoryProperties.memoryHeaps[best].size) {
            best = i
        }
    }
    if (best >= 0) {
        r.size = int64(mp2.memoryProperties.memoryHeaps[best].size)
        if (has_budget && mb.heapBudget[best] > 0ul) {
            r.budget = int64(mb.heapBudget[best])
            r.used = int64(mb.heapUsage[best])
        }
    }
    return r
}

// The pre-carve weight cap, shared by vk_moe_init and the DlimConfiguration source. Env wins
// outright (=0 pins WEIGHT_CAP — the misreporting-driver hatch); auto derives from heap SIZE, a
// hardware constant — same number every run. Transient pressure is the memprio shield's job.
def resolve_weight_cap(heap : VkHeapInfo) : int64 {
    let vram_mb = gpu_want_vram_mb()
    if (vram_mb > 0l) {
        return vram_mb * 1_000_000l
    }
    if (has_env_variable("DASLLAMA_GPU_VRAM_MB")) {
        return WEIGHT_CAP
    }
    var cap = WEIGHT_CAP
    if (heap.size > 0l) {
        cap = clamp(heap.size - VRAM_RESERVE, 0l, cap)
    }
    return cap
}

// The stream-slot carve, derived AT READ TIME — g_gpu.weight_budget stores the UNCARVED cap.
// Init-time carving was timing-dependent (the expert_q8n probe can init the device BEFORE the
// loader reports its slot need), which made a dry bake's plan diverge from the live walk's.
def carved_budget : int64 {
    var r = g_gpu.weight_budget
    if (gpu_want_moe_stream() != 0l) {
        let need = moe_gpu_stream_need()
        if (need > 0l) {
            r -= 2l * need
        } elif (gpu_want_moe_stream() > 0l) {
            r -= STREAM_RESERVE
        }
    }
    return max(r, 0l)
}

var g_vk_dry_want = false
var g_vk_dry_cfg = DlimVulkanConfig()

def vk_moe_init : bool {
    if (g_init_failed) {   // checked FIRST: a partially-built g_gpu must never report ready
        return false
    }
    if (g_gpu != null) {
        return true
    }
    if (g_vk_dry_want) {
        // DlimVulkanConfig numbers feed the SAME arithmetic — a dry bake cannot drift from live
        if (g_vk_dry_cfg.subgroup_size < 32) {
            to_log(LOG_ERROR, "dasLLAMA vulkan tier: config subgroupSize {g_vk_dry_cfg.subgroup_size} < 32 — bake declined\n")
            g_init_failed = true
            return false
        }
        g_gpu = new GpuState()
        g_gpu.dry = true
        g_gpu.coopmat_mode = g_vk_dry_cfg.coopmat_mode
        g_gpu.has_coopmat = g_vk_dry_cfg.coopmat_mode > 0
        g_gpu.has_coopmat2 = g_vk_dry_cfg.coopmat_mode == 4
        g_gpu.rows_per_wg = int64((WG_X + uint(g_vk_dry_cfg.subgroup_size) - 1u) / uint(g_vk_dry_cfg.subgroup_size))
        g_gpu.msr = g_vk_dry_cfg.max_storage_range
        g_gpu.weight_budget = g_vk_dry_cfg.vram_mb * 1_000_000l

        return true
    }
    g_init_failed = true   // cleared only on full success
    if (volkInitialize() != 0) {
        to_log(LOG_ERROR, "dasLLAMA vulkan tier: no Vulkan loader\n")
        return false
    }
    g_gpu = new GpuState()
    g_gpu.instance <- create_instance("dasLLAMA vulkan tier", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(g_gpu.instance))
    g_gpu.phys = select_physical_device(g_gpu.instance)
    if (!storage_8_16_supported(g_gpu.phys) || !subgroup_compute_ops_supported(g_gpu.phys)
            || !integer_dot_product_supported(g_gpu.phys)) {
        to_log(LOG_ERROR, "dasLLAMA vulkan tier: device lacks 8/16-bit SSBO storage, compute subgroup ops, or shaderIntegerDotProduct — tier disabled\n")
        return false
    }
    g_gpu.fam = select_graphics_queue_family(g_gpu.phys)
    // coopmat requested whenever supported — the fa twin needs it even in sdot4 GEMM mode
    g_gpu.has_coopmat = cooperative_matrix_supported(g_gpu.phys)
    g_gpu.has_coopmat2 = g_gpu.has_coopmat && cooperative_matrix2_supported(g_gpu.phys)
    g_gpu.coopmat_mode = resolve_coopmat_mode(g_gpu.has_coopmat, g_gpu.has_coopmat2)
    // dedicated transfer family: streamed copies overlap compute (DASLLAMA_VK_XFERQ=0 = bisect hatch)
    var xfam = -1
    if (g_gpu.has_coopmat && env_flag("DASLLAMA_VK_XFERQ", true)
            && timeline_semaphore_supported(g_gpu.phys)) {
        xfam = select_transfer_queue_family(g_gpu.phys)
    }
    if (g_gpu.has_coopmat2 && g_gpu.coopmat_mode == 4) {
        // #77's creator: coopmat2 + BDA + vulkanMemoryModel + the query-then-enable extras
        g_gpu.device <- create_device_storage_8_16_int_dot_coopmat2(g_gpu.phys, g_gpu.fam, xfam)
        to_log(LOG_INFO, "dasLLAMA vulkan tier: Q8_0 prefill on NV_coopmat2 tensor tiles (decode-in-load)\n")
    } elif (g_gpu.has_coopmat) {
        g_gpu.device <- create_device_storage_8_16_int_dot_coopmat(g_gpu.phys, g_gpu.fam, xfam)
        if (g_gpu.coopmat_mode != 0) {
            let cm_name = g_gpu.coopmat_mode == 1 ? "f16" : (g_gpu.coopmat_mode == 2 ? "int8" : "mul_mm L-tile")
            to_log(LOG_INFO, "dasLLAMA vulkan tier: Q8_0 prefill on {cm_name} cooperative-matrix tiles\n")
        }
    } else {
        g_gpu.device <- create_device_storage_8_16_int_dot(g_gpu.phys, g_gpu.fam)
    }
    volkLoadDevice(dev_raw())
    g_gpu.queue = get_device_queue(g_gpu.device, g_gpu.fam, 0u)
    if (xfam >= 0) {
        g_gpu.xfer_fam = xfam
        g_gpu.xfer_queue = get_device_queue(g_gpu.device, uint(xfam), 0u)
        var xpci : CommandPoolCreateInfo
        xpci.queueFamilyIndex = uint(xfam)
        xpci.flags.reset_command_buffer = true
        g_gpu.xfer_pool <- create_command_pool(g_gpu.device, xpci)
        var xai = VkCommandBufferAllocateInfo()
        xai.commandPool = boost_value_to_vk(g_gpu.xfer_pool)
        xai.level = VkCommandBufferLevel.PRIMARY
        xai.commandBufferCount = 1u
        var tci = VkSemaphoreTypeCreateInfo()
        tci.semaphoreType = VkSemaphoreType.TIMELINE
        tci.initialValue = 0ul
        var sci = VkSemaphoreCreateInfo()
        unsafe {
            vk_check(vkAllocateCommandBuffers(dev_raw(), xai, addr(g_gpu.xfer_cmd)), null)
            sci.pNext = addr<void?>(tci)
            vk_check(vkCreateSemaphore(dev_raw(), sci, null, g_gpu.xfer_sem), null)
        }
        to_log(LOG_INFO, "dasLLAMA vulkan tier: transfer queue armed (family {xfam})\n")
    }
    // WDDM residency shield: allocations chain priority 1.0 (DASLLAMA_VK_MEMPRIO=0 = bisect hatch)
    g_gpu.has_memprio = (g_gpu.has_coopmat && memory_priority_supported(g_gpu.phys)
        && env_flag("DASLLAMA_VK_MEMPRIO", true))
    if (g_gpu.has_memprio) {
        to_log(LOG_INFO, "dasLLAMA vulkan tier: memory-priority residency shield armed\n")
    }
    let sg = subgroup_properties(g_gpu.phys).subgroupSize
    if (sg < 32u) {
        // the gid-mapped quant kernels reduce with xor masks up to 16 (q8k amax spans 32 lanes)
        to_log(LOG_ERROR, "dasLLAMA vulkan tier: subgroupSize {sg} < 32 — tier disabled\n")
        return false
    }
    g_gpu.rows_per_wg = int64((WG_X + sg - 1u) / sg)   // = gl_NumSubgroups for our 1D workgroup
    var pdp : VkPhysicalDeviceProperties
    vkGetPhysicalDeviceProperties(g_gpu.phys, pdp)
    g_gpu.msr = int64(pdp.limits.maxStorageBufferRange)
    // env-vs-auto resolution shared with the DlimConfiguration source (resolve_weight_cap)
    let heap = query_heap_info(g_gpu.phys)
    g_gpu.weight_budget = resolve_weight_cap(heap)
    if (heap.budget > 0l) {
        let vram_override = gpu_want_vram_mb() > 0l || has_env_variable("DASLLAMA_GPU_VRAM_MB")
        // the honest cliff leaves 2GB of the raw heap for the desktop (6000MB healthy, 6803 demoted)
        let cliff = clamp(heap.budget - VRAM_RESERVE, 0l, heap.size - 2_147_483_648l)
        if (vram_override && g_gpu.weight_budget > cliff) {
            to_log(LOG_WARNING, "dasLLAMA vulkan tier: DASLLAMA_GPU_VRAM_MB {g_gpu.weight_budget / 1000000l} MB is above the demotion cliff — WDDM will silently demote buffers to SysMem (act-role collapse); lower it to <= {cliff / 1000000l} MB\n")
        }
        to_log(LOG_INFO, "dasLLAMA vulkan tier: device heap budget {heap.budget / 1000000l} MB (used {heap.used / 1000000l}), weight cap {g_gpu.weight_budget / 1000000l} MB\n")
    }

    // ReBAR: DL|HV|HC on the full-VRAM heap (the >1GB check excludes the 256MiB legacy window)
    if (env_flag("DASLLAMA_VK_REBAR", true)) {
        var rmp : VkPhysicalDeviceMemoryProperties
        vkGetPhysicalDeviceMemoryProperties(g_gpu.phys, rmp)
        for (i in range(int(rmp.memoryTypeCount))) {
            let fl = rmp.memoryTypes[i].propertyFlags
            if (fl.device_local && fl.host_visible && fl.host_coherent
                    && int64(rmp.memoryHeaps[int(rmp.memoryTypes[i].heapIndex)].size) > 1_073_741_824l) {
                g_gpu.rebar_type = i
                break
            }
        }
        if (g_gpu.rebar_type >= 0) {
            to_log(LOG_INFO, "dasLLAMA vulkan tier: ReBAR active — uploads write direct to VRAM (DASLLAMA_VK_REBAR=0 restores staging)\n")
        }
    }
    var pci : CommandPoolCreateInfo
    pci.queueFamilyIndex = g_gpu.fam
    pci.flags.reset_command_buffer = true   // per-stack buffers re-record if a shape ever changes
    g_gpu.pool <- create_command_pool(g_gpu.device, pci)

    // one layout, MAX_STACKS sets: each resident stack gets its own set at upload time
    var dslci : DescriptorSetLayoutCreateInfo
    dslci.pBindings |> reserve(6)
    for (bind in range(6)) {
        var b0 = DescriptorSetLayoutBinding(binding = uint(bind),
                                            descriptorType = VkDescriptorType.STORAGE_BUFFER,
                                            descriptorCount = 1u)
        b0.stageFlags.compute = true
        dslci.pBindings |> emplace(b0)
    }
    g_gpu.set_layout <- create_descriptor_set_layout(g_gpu.device, dslci)
    var dpci : DescriptorPoolCreateInfo
    dpci.maxSets = uint(5 * MAX_STACKS)   // decode + batch set per stack, 3 per FFN triple, 3 per DN step layer, batch act/rq
    var psize = DescriptorPoolSize(type_ = VkDescriptorType.STORAGE_BUFFER, descriptorCount = uint(6 * 5 * MAX_STACKS))
    dpci.pPoolSizes |> push(psize)
    g_gpu.desc_pool <- create_descriptor_pool(g_gpu.device, dpci)

    var plci : PipelineLayoutCreateInfo
    plci.pSetLayouts <- [weak_copy(g_gpu.set_layout)]
    g_gpu.pipe_layout <- create_pipeline_layout(g_gpu.device, plci)
    g_pipe_access |> clear()
    g_gpu.pipeline <- make_kernel_pipe("q8_moe_groupn", q8_moe_groupn_spv)
    g_gpu.batch_pipeline <- make_kernel_pipe("q8_moe_batch", q8_moe_batch_spv)   // same 6-binding layout
    if (g_gpu.coopmat_mode == 1) {   // f16 tensor-core prefill GEMMs (coopmat device only)
        g_gpu.batch_coopmat_f16 <- make_kernel_pipe("q8_moe_batch_cmf16", q8_moe_batch_cmf16_spv)
        g_gpu.batch_coopmat_q40 <- make_kernel_pipe("kq_moe_batch_q40_cmf16", kq_moe_batch_q40_cmf16_spv)
    } elif (g_gpu.coopmat_mode == 2) {   // native-s8 tensor-core prefill GEMM
        g_gpu.batch_coopmat_i8 <- make_kernel_pipe("q8_moe_batch_cmi8", q8_moe_batch_cmi8_spv)
    } elif (g_gpu.coopmat_mode >= 3) {   // the mul_mm L/M-tile prefill GEMMs (routed per dispatch;
        // mode 4 keeps the whole mm ladder for fmt-0 stacks and adds the cm2 rails for fmt-6)
        g_gpu.batch_coopmat_mm <- make_kernel_pipe("q8_moe_batch_mm", q8_moe_batch_mm_spv)
        g_gpu.batch_coopmat_mm_m <- make_kernel_pipe("q8_moe_batch_mm_m", q8_moe_batch_mm_m_spv)
        g_gpu.batch_coopmat_mm_a <- make_kernel_pipe("q8_moe_batch_mm_a", q8_moe_batch_mm_a_spv)
        if (g_gpu.coopmat_mode == 4) {
            g_gpu.batch_cm2 <- make_kernel_pipe("q8n_moe_batch_cm2", q8n_moe_batch_cm2_spv)
            g_gpu.gemv_q8n <- make_kernel_pipe("q8n_moe_groupn", q8n_moe_groupn_spv)
            g_gpu.xf16_pipeline <- make_kernel_pipe("q8_moe_xf16", q8_moe_xf16_spv)
            g_gpu.actf16_pipeline <- make_kernel_pipe("q8_moe_actf16", q8_moe_actf16_spv)
            g_gpu.f16cvt_pipeline <- make_kernel_pipe("rd_f16cvt", rd_f16cvt_spv)
        }
    }
    g_gpu.actrq_pipeline <- make_kernel_pipe("q8_moe_actrq", q8_moe_actrq_spv)
    g_gpu.gemv_k4 <- make_kernel_pipe("kq_moe_gemv_k4", kq_moe_gemv_k4_spv)
    g_gpu.gemv_k5 <- make_kernel_pipe("kq_moe_gemv_k5", kq_moe_gemv_k5_spv)
    g_gpu.gemv_k6 <- make_kernel_pipe("kq_moe_gemv_k6", kq_moe_gemv_k6_spv)
    g_gpu.gemv_q40 <- make_kernel_pipe("kq_moe_gemv_q40", kq_moe_gemv_q40_spv)
    g_gpu.actrq_k_pipeline <- make_kernel_pipe("q8k_moe_actrq", q8k_moe_actrq_spv)
    g_gpu.batch_k4 <- make_kernel_pipe("kq_moe_batch_k4", kq_moe_batch_k4_spv)
    g_gpu.batch_k5 <- make_kernel_pipe("kq_moe_batch_k5", kq_moe_batch_k5_spv)
    g_gpu.batch_k6 <- make_kernel_pipe("kq_moe_batch_k6", kq_moe_batch_k6_spv)
    g_gpu.batch_q40 <- make_kernel_pipe("kq_moe_batch_q40", kq_moe_batch_q40_spv)
    g_gpu.comb_pipeline <- make_kernel_pipe("moe_combine", moe_combine_spv)
    // nolint:STYLE015 — dn_conv stages a full row in shared (cv_row+cv_scale = 33024 B); Apple
    // caps threadgroup memory at 32 KB and pipeline COMPILE fails, so the chain gates by limit
    var limits_props : VkPhysicalDeviceProperties
    vkGetPhysicalDeviceProperties(g_gpu.phys, limits_props)
    g_gpu.dn_shared_ok = limits_props.limits.maxComputeSharedMemorySize >= 33024u
    if (g_gpu.dn_shared_ok) {
        g_gpu.dn_conv_pipeline <- make_kernel_pipe("dn_conv", dn_conv_spv)
        g_gpu.dnd_fused_pipeline <- make_kernel_pipe("dn_step_fused", dn_step_fused_spv)
        g_gpu.dn_scan_p1_pipeline <- make_kernel_pipe("dn_scan_p1", dn_scan_p1_spv)
        g_gpu.dn_scan_p2_pipeline <- make_kernel_pipe("dn_scan_p2", dn_scan_p2_spv)
        g_gpu.dn_rq_pipeline <- make_kernel_pipe("dn_requant", dn_requant_spv)
    } else {
        set_moe_gpu_dn_route(false)
        set_moe_gpu_dnd_route(false)
        to_log(LOG_INFO, "dasLLAMA vulkan tier: deltanet chain declined — device shared-memory cap {int(limits_props.limits.maxComputeSharedMemorySize)} B < dn_conv's 33024 B tile; recurrent layers stay on the CPU rail\n")
    }
    // at_* stages a full projection row in shared like dn_conv (33024 B) — same limit gate
    if (g_gpu.dn_shared_ok) {
        g_gpu.at_prep_q_pipeline <- make_kernel_pipe("at_prep_q", at_prep_q_spv)
        g_gpu.at_prep_k_pipeline <- make_kernel_pipe("at_prep_k", at_prep_k_spv)
        g_gpu.at_attn_pipeline <- make_kernel_pipe("at_attn", at_attn_spv)
    } else {
        set_moe_gpu_attn_route(false)
    }
    g_gpu.q8k_rq_pipeline <- make_kernel_pipe("q8k_requant", q8k_requant_spv)
    g_gpu.ar_pipeline <- make_kernel_pipe("ar_add_rms_b", ar_add_rms_b_spv)   // decode seams dispatch it 1-wg
    g_gpu.rks_pipeline <- make_kernel_pipe("rope_kv_store", rope_kv_store_spv)
    g_gpu.da_pipeline <- make_kernel_pipe("da_attn", da_attn_spv)
    g_gpu.rks_b_pipeline <- make_kernel_pipe("rope_kv_store_b", rope_kv_store_b_spv)
    g_gpu.da_b_pipeline <- make_kernel_pipe("da_attn_b", da_attn_b_spv)
    if (g_gpu.has_coopmat) {
        g_gpu.da_b128_pipeline <- make_kernel_pipe("da_attn_b_h128", da_attn_b_h128_spv)
    }
    g_gpu.qkn_pipeline <- make_kernel_pipe("qk_rms", qk_rms_spv)
    g_gpu.ar_rq_pipeline <- make_kernel_pipe("ar_add_rms_rq", ar_add_rms_rq_spv)
    g_gpu.qkn_rope_pipeline <- make_kernel_pipe("qkn_rope_kv", qkn_rope_kv_spv)

    var fence <- create_fence(g_gpu.device, FenceCreateInfo())   // never finalized (process-lifetime)
    g_gpu.fence = boost_value_to_vk(fence)
    g_gpu.staging = make_host_buf(STAGING_BYTES, false)
    g_gpu.host_mem |> erase(g_gpu.staging.buf)   // process-lifetime — exempt from the model-drop sweep
    g_init_failed = false
    to_log(LOG_INFO, "dasLLAMA vulkan tier: device ready (subgroup {int(sg)}, {g_gpu.rows_per_wg} rows/wg)\n")
    if (dasllama_noisy()) {
        // storage-range is the limit that actually binds the arena; allocs/descriptors do not
        var lp : VkPhysicalDeviceProperties
        vkGetPhysicalDeviceProperties(g_gpu.phys, lp)
        to_log(LOG_INFO, "dasLLAMA vulkan tier: limits allocs {lp.limits.maxMemoryAllocationCount} storage-range {lp.limits.maxStorageBufferRange} desc-storage {lp.limits.maxDescriptorSetStorageBuffers}\n")
    }
    return true
}

// ===== upload (loader hook — moe_gpu_upload_resident gathers, we just stage) =====

def staged_upload(dst : uint64; dst_off : int64; bytes : int64) {
    var regions : array<BufferCopy>
    let rc = BufferCopy(srcOffset = 0ul, dstOffset = uint64(dst_off), size = uint64(bytes))
    regions |> push(rc)
    run_cmd_sync(g_gpu.device, g_gpu.pool, g_gpu.queue) $(cmd) {
        cmd_copy_buffer(cmd, nonowning_buf(g_gpu.staging.buf), nonowning_buf(dst), regions)
    }
}

// Stage `total` bytes into `dst` starting at `dst_off`. The arena packs many tensors into one
// buffer, so it needs the offset; the per-op tier's own planes start at 0. ReBAR buffers skip
// staging outright — the CPU writes the mapped VRAM; visibility comes from the next queue submit.
def upload_region_at(dst : uint64; dst_off : int64; src : uint8 const?; total : int64) {
    let t0 = ref_time_ticks()
    if (key_exists(g_gpu.dev_mapped, dst)) {
        unsafe {
            par_memcpy(reinterpret<void?>(g_gpu.dev_mapped[dst] + uint64(dst_off)), reinterpret<void?>(src), total)
        }
    } else {
        var off = 0l
        while (off < total) {
            let chunk = min(total - off, STAGING_BYTES)
            unsafe {
                par_memcpy(g_gpu.staging.mapped, reinterpret<void?>(src + off), chunk)
            }
            staged_upload(dst, dst_off + off, chunk)
            off += chunk
        }
    }
    g_vk_upload_us += int64(get_time_usec(t0))
    g_vk_upload_bytes += total
}

def upload_region(dst : uint64; src : uint8 const?; total : int64) {
    upload_region_at(dst, 0l, src, total)
}

// report the accumulated upload cost (the counters above); `direct` = the caller's mapped-vs-staged tag
def vk_upload_report(direct : bool) {
    if (g_vk_upload_us > 0l) {
        let gbps = double(g_vk_upload_bytes) / (double(g_vk_upload_us) * 1000.0lf)
        to_log(LOG_INFO, "dasLLAMA vulkan tier: uploads {g_vk_upload_bytes / 1000000l} MB in {g_vk_upload_us / 1000l} ms ({gbps} GB/s, {direct ? "direct" : "staged"})\n")
    }
}

def alloc_desc_set : VkDescriptorSet {
    var dsai = VkDescriptorSetAllocateInfo()
    dsai.descriptorPool = boost_value_to_vk(g_gpu.desc_pool)
    dsai.descriptorSetCount = 1u
    var raw_set_layout = boost_value_to_vk(g_gpu.set_layout)
    var raw : VkDescriptorSet
    unsafe {
        dsai.pSetLayouts = addr(raw_set_layout)
        vk_check(vkAllocateDescriptorSets(dev_raw(), dsai, addr(raw)), null)
    }
    return raw
}

def write_buf_desc(var writes : array<WriteDescriptorSet>; set_ : VkDescriptorSet; binding : uint; buf : uint64; bytes : int64) {
    var w = WriteDescriptorSet(dstSet = vk_value_to_boost(set_), dstBinding = binding,
                               descriptorType = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 1u)
    var binfo = DescriptorBufferInfo(buffer = nonowning_buf(buf), range_ = uint64(bytes))
    w.pBufferInfo |> push(binfo)
    writes |> emplace(w)
}

// (re)write all six bindings of a set + declare its hazard region bits — every dispatchable set
// in the family is built through here (or vk_set6, which allocates first)
def vk_write6(set_ : VkDescriptorSet; b0, b1, b2, b3, b4, b5 : uint64; s0, s1, s2, s3, s4, s5 : int64; gbits : uint[6]) {
    var writes : array<WriteDescriptorSet>
    write_buf_desc(writes, set_, 0u, b0, s0)
    write_buf_desc(writes, set_, 1u, b1, s1)
    write_buf_desc(writes, set_, 2u, b2, s2)
    write_buf_desc(writes, set_, 3u, b3, s3)
    write_buf_desc(writes, set_, 4u, b4, s4)
    write_buf_desc(writes, set_, 5u, b5, s5)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(g_gpu.device, writes, no_copies)
    hz_set_bits(set_, gbits)
}

def vk_set6(b0, b1, b2, b3, b4, b5 : uint64; s0, s1, s2, s3, s4, s5 : int64; gbits : uint[6]) : VkDescriptorSet {
    var set_ = alloc_desc_set()
    vk_write6(set_, b0, b1, b2, b3, b4, b5, s0, s1, s2, s3, s4, s5, gbits)
    return set_
}

// weight plane byte sizes for one [rows x n] stack in the tier's device layout
def stack_plane_bytes(n, rows : int64; fmt : int) : tuple<wq : int64; ws : int64> {
    let nb = arena_blocks_for(n, rows, fmt)   // strides live in arena_block_bytes — one source
    let bb = arena_block_bytes(fmt)
    return (wq = nb * bb.wq, ws = nb * bb.ws)
}

// build a VkStack shell (weight plane buffers only — the GEMV/cls scratch is lazy, see
// ensure_gemv_scratch). Resident stacks pass their exact plane sizes; streaming pseudo-stacks
// pass slot capacities and a base of -1 (re-based per layer).
def make_stack_shell(base, elems : int64; fmt : int; wqcap, wscap : int64; xfer_shared : bool = false) : int {
    // q8n has no scale plane (wscap 0) — floor the buffer so its descriptor slot stays valid
    var st = VkStack(base = base, elems = elems, fmt = fmt, wqbytes = wqcap,
                     wqbuf = g_gpu.dry ? 0ul : make_device_buf(wqcap, xfer_shared),
                     wsbuf = g_gpu.dry ? 0ul : make_device_buf(max(wscap, 64l), xfer_shared), wsbytes = wscap)
    g_gpu.stacks |> push(st)
    return length(g_gpu.stacks) - 1
}

// the descriptor range for a stack's ws binding — the 64B floor of the shell above
def ws_range(si : int) : int64 => max(g_gpu.stacks[si].wsbytes, 64l)

// decode/GEMV scratch for one stack (per-stack host bufs + the written 6-binding set). Lazy:
// only stacks that actually serve GEMV or cls dispatches pay for it — dense-GEMM planes never do.
def ensure_gemv_scratch(si : int) {
    if (g_gpu.stacks[si].gemv_made) {
        return
    }
    g_gpu.stacks[si].meta = make_host_buf(META_BYTES, true)
    g_gpu.stacks[si].xq = make_host_buf(XQ_BYTES, true)
    g_gpu.stacks[si].xs = make_host_buf(XS_BYTES, true)
    g_gpu.stacks[si].y = make_host_buf(Y_BYTES, false, [cached = true])
    g_gpu.stacks[si].y_dev = make_device_buf(Y_BYTES)
    // default y role; role-fluid chains (gate/up pair, qkv group, dn step) re-declare before recording
    g_gpu.stacks[si].raw_set = vk_set6(g_gpu.stacks[si].wqbuf, g_gpu.stacks[si].wsbuf,
        g_gpu.stacks[si].meta.buf, g_gpu.stacks[si].xq.buf, g_gpu.stacks[si].xs.buf, g_gpu.stacks[si].y_dev,
        g_gpu.stacks[si].wqbytes, ws_range(si), META_BYTES, XQ_BYTES, XS_BYTES, Y_BYTES,
        fixed_array(0u, 0u, 0u, 0u, 0u, VHG_Y1))
    g_gpu.stacks[si].gemv_made = true
}

// import support is spec-gray per driver: one decline disables imports for the process (say WHY
// once) — every later stack goes straight to the pinned copy instead of churning failed vk calls
def vk_import_declined(why : string) {
    if (!g_vk_import_declined_said) {
        g_vk_import_declined_said = true
        to_log(LOG_INFO, "dasLLAMA vulkan tier: mapped-image import declined ({why}) — pinned mirror copies\n")
    }
}

// P2: import a 4KB-aligned mapped-file window as a transfer_src buffer (VK_EXT_external_memory_host).
// buf == 0 = declined, the caller falls back to the pinned copy. NVIDIA/Windows refuses FILE-BACKED
// pages in BOTH protections while anon imports succeed (full 2x2: _vk_import_probe.das, 2026-07-28)
def make_imported_buf(ptr : void?; bytes : int64) : HostBuf {
    let hb = HostBuf()
    if (ptr == null || bytes <= 0l || !device_extension_available(g_gpu.phys, "VK_EXT_external_memory_host")) {
        return hb
    }
    let algn = int64(external_memory_host_min_alignment(g_gpu.phys))
    if (algn <= 0l || int64(intptr(ptr)) % algn != 0l) {
        return hb
    }
    // the round-up stays inside the mapping: windows start 4KB-aligned, the file is 16KB-padded
    let sz = (bytes + algn - 1l) / algn * algn
    var ht : VkExternalMemoryHandleTypeFlags
    ht.host_allocation_ext = true
    var hpp = VkMemoryHostPointerPropertiesEXT()
    let hres = vkGetMemoryHostPointerPropertiesEXT(dev_raw(), ht, ptr, hpp)
    if (hres != VkResult.SUCCESS) {
        vk_import_declined("vkGetMemoryHostPointerPropertiesEXT {hres}")
        return hb
    }
    var emb = VkExternalMemoryBufferCreateInfo()
    emb.handleTypes = ht
    var bci : BufferCreateInfo
    bci.size = uint64(sz)
    bci.usage.transfer_src = true
    if (g_gpu.xfer_fam >= 0) {
        // same CONCURRENT sharing the pinned mirrors use (transfer-queue fills + compute heat slices)
        bci.sharingMode = VkSharingMode.CONCURRENT
        bci.pQueueFamilyIndices <- [g_gpu.fam, uint(g_gpu.xfer_fam)]
    }
    unsafe {
        bci.next = addr<void?>(emb)
    }
    var cres = VkResult.SUCCESS
    var b <- create_buffer(g_gpu.device, bci, unsafe(addr(cres)))   // deliberately never finalized (make_host_buf's rule)
    if (cres != VkResult.SUCCESS) {
        vk_import_declined("vkCreateBuffer {cres}")
        return hb
    }
    let req = get_buffer_memory_requirements(g_gpu.device, b)
    let usable = hpp.memoryTypeBits & req.memoryTypeBits
    if (usable == 0u || int64(req.size) > sz) {
        unsafe {
            vkDestroyBuffer(dev_raw(), reinterpret<VkBuffer>(b._vk), null)
        }
        vk_import_declined(usable == 0u ? "no usable memory type" : "buffer wants {req.size} bytes over a {sz} byte window")
        return hb
    }
    // prefer host_cached NON-device-local: the lowest usable bit can be the ReBAR heap (multi-GB imports exhaust VRAM), and imported file pages are cached RAM — an attribute mismatch is a driver refusal
    var mp : VkPhysicalDeviceMemoryProperties
    vkGetPhysicalDeviceMemoryProperties(g_gpu.phys, mp)
    var ti = 0u
    while ((usable & (1u << ti)) == 0u) {
        ti++
    }
    var plain = -1
    var cached = -1
    for (i in range(int(mp.memoryTypeCount))) {
        let fl = mp.memoryTypes[i].propertyFlags
        if ((usable & (1u << uint(i))) != 0u && fl.host_visible && !fl.device_local) {
            plain = plain < 0 ? i : plain
            cached = cached < 0 && fl.host_cached ? i : cached
        }
    }
    if (cached >= 0) {
        ti = uint(cached)
    } elif (plain >= 0) {
        ti = uint(plain)
    }
    var imp = VkImportMemoryHostPointerInfoEXT()
    imp.handleType = ht
    imp.pHostPointer = ptr
    var mai = MemoryAllocateInfo(allocationSize = uint64(sz), memoryTypeIndex = ti)
    unsafe {
        mai.next = addr<void?>(imp)
    }
    var ares = VkResult.SUCCESS
    var m <- allocate_memory(g_gpu.device, mai, unsafe(addr(ares)))
    if (ares != VkResult.SUCCESS) {
        unsafe {
            vkDestroyBuffer(dev_raw(), reinterpret<VkBuffer>(b._vk), null)
        }
        vk_import_declined("vkAllocateMemory {ares}, type {ti} of usable {usable} ({mp.memoryTypes[int(ti)].propertyFlags}), {sz} bytes")
        return hb
    }
    var bres = VkResult.SUCCESS
    bind_buffer_memory(g_gpu.device, b, m, 0ul, unsafe(addr(bres)))
    if (bres != VkResult.SUCCESS) {
        unsafe {
            vkFreeMemory(dev_raw(), reinterpret<VkDeviceMemory>(m._vk), null)
            vkDestroyBuffer(dev_raw(), reinterpret<VkBuffer>(b._vk), null)
        }
        vk_import_declined("vkBindBufferMemory {bres}")
        return hb
    }
    g_gpu.host_mem |> insert(b._vk, m._vk)
    return HostBuf(buf = b._vk, mem = m._vk, mapped = ptr, bytes = bytes)
}

// stream arm: pinned host mirror the copy engine DMAs into a slot per prefill (no VRAM spent); P2: a flavor load IMPORTS the gather's mapped-blob window instead — zero pinned RAM, the page cache is the mirror
def vk_moe_stream_stack(wq : uint8 const?; ws : uint8 const?; woff : int64; n, rows : int64; fmt : int) : bool {
    let pb = stack_plane_bytes(n, rows, fmt)
    if (env_flag("DASLLAMA_VK_IMPORT", true) && !g_vk_import_declined_said) {
        let win = moe_gpu_slice_window(woff, fmt)
        if (win.wq != null && win.wqb == pb.wq && (pb.ws <= 0l || (win.ws != null && win.wsb == pb.ws))) {
            var iq = make_imported_buf(win.wq, pb.wq)
            if (iq.buf != 0ul) {
                // ws: import the real window, or a tiny pinned dummy for the descriptor floor (q8n)
                let is_ = pb.ws > 0l ? make_imported_buf(win.ws, pb.ws) : make_host_buf(64l, false, [xfer_shared = true])
                if (is_.buf != 0ul) {
                    var ss = StreamStack(base = woff, elems = rows * n, fmt = fmt,
                                         wqbytes = pb.wq, wsbytes = pb.ws, hwq = iq, hws = is_)
                    if (length(g_gpu.stream_stacks) % 3 == 2) {
                        g_gpu.stream_max_dn_wq = max(g_gpu.stream_max_dn_wq, pb.wq)
                        g_gpu.stream_max_dn_ws = max(g_gpu.stream_max_dn_ws, pb.ws)
                    } else {
                        g_gpu.stream_max_gu_wq = max(g_gpu.stream_max_gu_wq, pb.wq)
                        g_gpu.stream_max_gu_ws = max(g_gpu.stream_max_gu_ws, pb.ws)
                    }
                    g_gpu.stream_stacks |> emplace(ss)
                    if (!g_vk_import_said) {
                        g_vk_import_said = true
                        to_log(LOG_INFO, "dasLLAMA vulkan tier: stream mirrors IMPORT the mapped image (external_memory_host) — zero pinned copies\n")
                    }
                    return true
                }
                destroy_host_buf(iq)
            }
        }
    }
    var ss = StreamStack(base = woff, elems = rows * n, fmt = fmt, wqbytes = pb.wq, wsbytes = pb.ws)
    if (!g_gpu.dry) {
        ss.hwq = make_host_buf(pb.wq, false, [xfer_shared = true, soft = true])
        if (ss.hwq.buf == 0ul) {
            return false   // pinned-mirror stop: the walk rolls the partial triple back and breaks
        }
        ss.hws = make_host_buf(max(pb.ws, 64l), false, [xfer_shared = true, soft = true])
        if (ss.hws.buf == 0ul) {
            destroy_host_buf(ss.hwq)
            return false
        }
        unsafe {
            par_memcpy(ss.hwq.mapped, reinterpret<void?>(wq), pb.wq)
            if (pb.ws > 0l) {   // q8n mirrors carry no scale plane
                par_memcpy(ss.hws.mapped, reinterpret<void?>(ws), pb.ws)
            }
        }
    }
    if (length(g_gpu.stream_stacks) % 3 == 2) {   // walk order gate/up/down: this one is a down
        g_gpu.stream_max_dn_wq = max(g_gpu.stream_max_dn_wq, pb.wq)
        g_gpu.stream_max_dn_ws = max(g_gpu.stream_max_dn_ws, pb.ws)
    } else {
        g_gpu.stream_max_gu_wq = max(g_gpu.stream_max_gu_wq, pb.wq)
        g_gpu.stream_max_gu_ws = max(g_gpu.stream_max_gu_ws, pb.ws)
    }
    g_gpu.stream_stacks |> emplace(ss)
    return true
}

// The resident-accept arithmetic, shared by the upload path and the slice replay's would-accept
// probe — a divergent copy would desync the baked plan at the refusal boundary.
// 0 = accept, 1 = VRAM budget, 2 = descriptor capacity.
def vk_moe_resident_verdict(n, rows : int64; fmt : int) : int {
    let pb = stack_plane_bytes(n, rows, fmt)
    if (g_gpu.resident_bytes + pb.wq + pb.ws > carved_budget()) {
        return 1
    }
    return length(g_gpu.stacks) >= MAX_STACKS ? 2 : 0
}

//! MoeGpuWouldAcceptFn: would the tier accept this stack right now? Pure arithmetic, no side
//! effects — the slice replay uses it to distinguish a refusal boundary (the walk attempts a
//! gather the bake refused, so it is NOT in the plan) from real plan divergence.
def vk_moe_would_accept(n : int64; rows : int64; fmt : int; stream : bool) : bool {
    if (g_gpu == null) {
        return true   // tier not up: claim accept so a mismatch reads as real divergence
    }
    return stream || vk_moe_resident_verdict(n, rows, fmt) == 0
}

//! MoeGpuUploadFn: upload one weight stack ([rows x n] at element offset woff) from the loader's
//! gathered row-major planes. stream = pinned host mirror for per-prefill DMA instead of VRAM.
def vk_moe_upload_stack(wq : uint8 const?; ws : uint8 const?; woff : int64; n : int64; rows : int64; fmt : int; stream : bool) : bool {
    if (!vk_moe_init()) {
        return false
    }
    if (stream) {
        return vk_moe_stream_stack(wq, ws, woff, n, rows, fmt)
    }
    let verdict = vk_moe_resident_verdict(n, rows, fmt)
    if (verdict == 1) {
        to_log(LOG_INFO, "dasLLAMA vulkan tier: VRAM budget reached at {g_gpu.resident_bytes} bytes\n")
        return false
    }
    if (verdict == 2) {
        to_log(LOG_INFO, "dasLLAMA vulkan tier: descriptor capacity reached at {length(g_gpu.stacks)} stacks\n")
        return false
    }
    let pb = stack_plane_bytes(n, rows, fmt)
    let wqbytes = pb.wq
    let wsbytes = pb.ws
    let si = make_stack_shell(woff, rows * n, fmt, wqbytes, wsbytes)
    if (!g_gpu.dry) {
        upload_region(g_gpu.stacks[si].wqbuf, wq, wqbytes)
        upload_region(g_gpu.stacks[si].wsbuf, ws, wsbytes)
    }
    g_gpu.resident_bytes += wqbytes + wsbytes
    return true
}

//! MoeGpuDropFn: roll back the last n uploads (a partial layer triple the budget cut mid-way).
//! Only freshly-uploaded stacks with no dispatch state yet may be dropped.
def vk_moe_drop_stacks(n : int; stream : bool) {
    if (stream) {
        for (_i in range(n)) {
            let k = length(g_gpu.stream_stacks) - 1
            if (!g_gpu.dry) {
                destroy_host_buf(g_gpu.stream_stacks[k].hwq)
                destroy_host_buf(g_gpu.stream_stacks[k].hws)
            }
            g_gpu.stream_stacks |> resize(k)
        }
        return
    }
    for (_i in range(n)) {
        let k = length(g_gpu.stacks) - 1
        assert(!g_gpu.stacks[k].gemv_made && !g_gpu.stacks[k].batch_made && !g_gpu.stacks[k].cls_made,
            "dasLLAMA vulkan tier: dropping a stack with live dispatch state")
        g_gpu.resident_bytes -= g_gpu.stacks[k].wqbytes + g_gpu.stacks[k].wsbytes
        if (!g_gpu.dry) {
            destroy_device_buf(g_gpu.stacks[k].wqbuf)
            destroy_device_buf(g_gpu.stacks[k].wsbuf)
        }
        g_gpu.stacks |> resize(k)
    }
}

//! MoeGpuDropModelFn: destroy every model-owned device object — stacks, arenas, the resident
//! driver, all lazy dispatch state, their descriptor sets and command buffers. Device-lifetime
//! state (instance/device/pipelines/staging/fence/profiler) survives and rebuilds lazily.
def vk_drop_model_state {
    if (g_gpu == null) {
        return
    }
    if (g_gpu.dry) {
        // dry bake owns no device objects — reset the accounting shells only
        g_gpu.stacks |> clear()
        g_gpu.stream_stacks |> clear()
        g_gpu.arenas |> clear()
        g_gpu.resident_bytes = 0l
        return
    }
    vk_check(vkQueueWaitIdle(g_gpu.queue), null)
    if (g_gpu.xfer_fam >= 0 && g_gpu.xfer_value > 0ul) {
        xfer_host_wait(g_gpu.xfer_value)   // in-flight transfer-queue copies retire before the sweeps
    }
    g_gpu.pend_wait = 0ul
    assert(g_gpu.ffn_pend_s2 == -1, "dasLLAMA vulkan tier: model drop with an async FFN in flight")
    // the resident driver's das-side shell first (its VK objects die in the sweeps below)
    if (g_rd != null) {
        unsafe {
            delete g_rd
        }
        g_rd = null
    }
    // every alloc_cmd result is model-owned — one bulk free
    if (!empty(g_gpu.model_cmds)) {
        unsafe {
            vkFreeCommandBuffers(dev_raw(), boost_value_to_vk(g_gpu.pool), uint(length(g_gpu.model_cmds)), addr(g_gpu.model_cmds[0]))
        }
        g_gpu.model_cmds |> clear()
    }
    // every descriptor set came from the one pool and every one is model-owned
    let rdpf : VkDescriptorPoolResetFlags
    vk_check(vkResetDescriptorPool(dev_raw(), boost_value_to_vk(g_gpu.desc_pool), rdpf), null)
    g_set_bits |> clear()   // pool reset recycles set handles — stale region bits must not survive
    // device buffers: dev_mem tracks every make_device_buf result — all model-owned
    for (buf, mem in keys(g_gpu.dev_mem), values(g_gpu.dev_mem)) {
        unsafe {
            vkDestroyBuffer(dev_raw(), reinterpret<VkBuffer>(buf), null)
            vkFreeMemory(dev_raw(), reinterpret<VkDeviceMemory>(mem), null)
        }
    }
    g_gpu.dev_mem |> clear()
    g_gpu.dev_mapped |> clear()
    g_vk_upload_us = 0l   // per-model upload diagnostics — a reloaded model reports its own numbers
    g_vk_upload_bytes = 0l
    g_rdq_n = 0u          // decode-role profiler likewise — averages must not span models
    g_rdq_count = 0l
    g_rdq_total = 0.0lf
    g_rdq_pro = 0.0lf
    g_rdq_tail = 0.0lf
    for (i in range(16)) {
        g_rdq_role[i] = 0.0lf
    }
    // host-visible buffers: host_mem tracks every make_host_buf result minus the exempt pair
    for (buf, mem in keys(g_gpu.host_mem), values(g_gpu.host_mem)) {
        unsafe {
            vkDestroyBuffer(dev_raw(), reinterpret<VkBuffer>(buf), null)
            vkFreeMemory(dev_raw(), reinterpret<VkDeviceMemory>(mem), null)
        }
    }
    g_gpu.host_mem |> clear()
    // das-side shells + the lazy-family latches (the ensure_* paths rebuild on next use).
    // clear(), not delete, where values carry VK handles — nothing das-heap-owned lives inside
    // those structs, and delete would try to finalize the handle fields
    g_gpu.stacks |> clear()
    delete g_gpu.stream_stacks
    delete g_gpu.stream_slots
    g_gpu.arenas |> clear()
    g_gpu.ffn_cmds |> clear()
    g_gpu.qkv_cmds |> clear()
    g_gpu.dnd_steps |> clear()
    delete g_gpu.heat_pools
    g_gpu.stream_max_gu_wq = 0l
    g_gpu.stream_max_gu_ws = 0l
    g_gpu.stream_max_dn_wq = 0l
    g_gpu.stream_max_dn_ws = 0l
    g_gpu.ar_ready = false
    g_gpu.af_ready = false
    g_gpu.rks_ready = false
    g_gpu.da_ready = false
    g_gpu.ffn_ready = false
    g_gpu.batch_ready = false
    g_gpu.comb_ready = false
    g_gpu.stream_ready = false
    g_gpu.dn_ready = false
    g_gpu.dnd_ready = false
    g_gpu.at_ready = false
    g_gpu.ffn_pend_s2 = -1
    g_gpu.ffn_pend_bytes = 0l
    g_gpu.resident_bytes = 0l
}

// ===== dispatch plumbing =====

// stack resolution is by (format, offset) — weight element offsets live in PER-FORMAT plane
// spaces (q8 qblob vs the k4/k5/k6 planes), so a bare offset is ambiguous across formats (a k4
// gate and a k5 down can share the numeric range); within one format's space ranges are disjoint
def find_stack_idx(w0 : int64; fmt : int) : int {
    for (i in range(length(g_gpu.stacks))) {
        if (g_gpu.stacks[i].fmt == fmt && w0 >= g_gpu.stacks[i].base && w0 < g_gpu.stacks[i].base + g_gpu.stacks[i].elems) {
            return i
        }
    }
    return -1
}

// the streamed twin: which 3-stack layer group (gate/up/down) holds this gate offset, or -1
def find_stream_group(w0 : int64; fmt : int) : int {
    for (i in range(length(g_gpu.stream_stacks) / 3)) {
        if (g_gpu.stream_stacks[i * 3].fmt == fmt && w0 >= g_gpu.stream_stacks[i * 3].base
                && w0 < g_gpu.stream_stacks[i * 3].base + g_gpu.stream_stacks[i * 3].elems) {
            return i
        }
    }
    return -1
}

def find_stack(w0 : int64; fmt : int) : int {
    let i = find_stack_idx(w0, fmt)
    if (i < 0) {
        panic("dasLLAMA vulkan tier: dispatch against a non-resident weight region (fmt {fmt} element offset {w0})")
    }
    return i
}

// ===== the resident driver's weight arena =====
// One buffer pair per FORMAT (kernels index both planes off ONE base, so a shared cursor only stays
// aligned at a constant stride); tensors by block index. Sub-allocation, not a hardware-limit dodge.

struct ArenaFmt {
    fmt : int                // int(KqFmt): 0 = q8, 1/2/3 = k4/k5/k6, 4 = q40
    wqbuf : uint64           // device-local quants for every tensor of this format
    wsbuf : uint64           // ... and their scale rows, at the SAME block index
    wqbytes : int64
    wsbytes : int64
    blocks : int64           // bump cursor: 32-weight blocks (q8) or 256-weight superblocks (kq)
    cap_blocks : int64
    // dispatch scratch — ONE set per format: bindings 0/1 stay put, only the meta changes per call
    raw_set : VkDescriptorSet
    meta : HostBuf
    xq : HostBuf
    xs : HostBuf
    y : HostBuf              // host-cached readback (the same-submit DMA lands here)
    y_dev : uint64
    ready : bool
    // batch-GEMM scratch (prefill): shares g_gpu.batch_xq_dev/xs_dev/y1_dev; one set + meta per fmt
    batch_set : VkDescriptorSet
    batch_meta : HostBuf
    batch_ready : bool
}

// per-block plane strides — the block is 32 weights for q8 and a 256-weight superblock for kq.
// Mirrors stack_plane_bytes: it computes the same totals as count * these strides.
// the tier's stack fmt space is int(KqFmt) — bridge to the gemm_schema kq id space (4/5/6/40)
def vk_kq_schema_id(fmt : int) : int {
    if (fmt == 1) return 4
    if (fmt == 2) return 5
    if (fmt == 3) return 6
    if (fmt == 4) return 40
    panic("vk_kq_schema_id: not a kq stack fmt")
    return 0
}

// fmt space here = int(KqFmt): 0 q8, 6 q8n (both 32-elem blocks), 1-4 the kq lattice + q40
def vk_fmt_b32(fmt : int) : bool => fmt == 0 || fmt == 6

// meta/activation block unit of a stack format — q8 AND q8n count 32-weight blocks, the kq
// lattice counts 256-weight superblocks
def fmt_unit(fmt : int) : int64 => vk_fmt_b32(fmt) ? 32l : 256l

def arena_block_bytes(fmt : int) : tuple<wq : int64; ws : int64> {
    if (fmt == 0) {
        return (wq = Q8_QPB, ws = Q8_SPB)     // 32 int8 quants + one f16 scale
    }
    if (fmt == 6) {
        return (wq = Q8N_BPB, ws = 0l)        // q8n: one interleaved 34B plane, scale inside
    }
    // decoded scale row: [f16 d][f16 dmin][8 sc][8 mn]
    return (wq = kq_qsb(vk_kq_schema_id(fmt)), ws = KQ_DEV_SSB)
}

def arena_blocks_for(n, rows : int64; fmt : int) : int64 {
    assert(vk_fmt_b32(fmt) || n % KQ_SUPERBLOCK_ELEMS == 0l, "dasLLAMA vulkan arena: kq row length must be a superblock multiple")
    assert(!vk_fmt_b32(fmt) || n % Q8_BLOCK_ELEMS == 0l, "dasLLAMA vulkan arena: q8 row length must be a 32-block multiple")
    return rows * (n / (vk_fmt_b32(fmt) ? Q8_BLOCK_ELEMS : KQ_SUPERBLOCK_ELEMS))
}

// Reserve one format's arena up front. Sizing is a decision, not a walk: the resident driver knows
// every tensor's shape from Model metadata before a byte moves, so it either fits or declines.
def arena_reserve(fmt : int; cap_blocks : int64) : bool {
    if (key_exists(g_gpu.arenas, fmt)) {
        // model reload: reset the bump cursor so it overwrites; an oversized reload then trips arena_place's -1 (decline to CPU) rather than overflowing
        unsafe {
            g_gpu.arenas[fmt].blocks = 0l
        }
        return true
    }
    let bb = arena_block_bytes(fmt)
    let qbytes = cap_blocks * bb.wq
    // q8n has no scale plane — floor the ws buffer so its descriptor slots stay valid (stack-shell rule)
    let sbytes = max(cap_blocks * bb.ws, 64l)
    // maxStorageBufferRange is the real ceiling (uint32 indexing reaches 16GiB); NV/AMD report 4GiB
    let shard_cap = g_gpu.msr
    if (qbytes > shard_cap || sbytes > shard_cap) {
        to_log(LOG_WARNING, "dasLLAMA vulkan arena: fmt {fmt} needs {qbytes} quant + {sbytes} scale bytes — over the device's {shard_cap}B storage-range cap\n")
        return false
    }
    var a = ArenaFmt(fmt = fmt, cap_blocks = cap_blocks, wqbytes = qbytes, wsbytes = sbytes,
                     wqbuf = g_gpu.dry ? 0ul : make_device_buf(qbytes), wsbuf = g_gpu.dry ? 0ul : make_device_buf(sbytes))
    g_gpu.arenas |> insert(fmt, a)
    return true
}

//! Place one [rows x n] tensor in its format's arena and upload it. Returns the BLOCK INDEX the
//! meta needs (mp[4 + row*2] = block + row * blocks_per_row), or -1 if the arena is exhausted.
def arena_place(wq : uint8 const?; ws : uint8 const?; n, rows : int64; fmt : int) : int64 {
    if (!key_exists(g_gpu.arenas, fmt)) {
        return -1l
    }
    let need = arena_blocks_for(n, rows, fmt)
    unsafe {
        var a & = g_gpu.arenas[fmt]
        if (a.blocks + need > a.cap_blocks) {
            return -1l
        }
        let blk = a.blocks
        a.blocks += need
        let bb = arena_block_bytes(fmt)
        if (!g_gpu.dry) {
            upload_region_at(a.wqbuf, blk * bb.wq, wq, need * bb.wq)
            if (bb.ws > 0l) {
                upload_region_at(a.wsbuf, blk * bb.ws, ws, need * bb.ws)
            }
        }
        return blk
    }
}

//! Arena seam: reserve one format's arena for `cap_blocks` 32-weight blocks (q8) or 256-weight
//! superblocks (kq). false = over the shard cap or no device — the caller turns that into a decline.
def vk_arena_reserve(fmt : int; cap_blocks : int64) : bool {
    if (!vk_moe_init()) {
        return false
    }
    return arena_reserve(fmt, cap_blocks)
}

//! Arena seam: place + upload one [rows x n] tensor from the loader's gathered device-layout
//! planes. Returns its block index (what the meta's weight base wants), or -1 if exhausted.
def vk_arena_place(wq : array<uint8>; ws : array<uint8>; n : int64; rows : int64; fmt : int) : int64 {
    // q8n (fmt 6) carries its scale inside the interleaved plane — an EMPTY ws is its contract
    if (!vk_moe_init() || empty(wq) || (empty(ws) && fmt != 6)) {
        return -1l
    }
    return arena_place(unsafe(addr(wq[0])), empty(ws) ? null : unsafe(addr(ws[0])), n, rows, fmt)
}

// Bindings 0/1 are the arena planes; 2-5 are per-dispatch scratch, written exactly as
// ensure_gemv_scratch writes them for a per-op stack — so an arena-resident tensor runs the SAME
// kernels with the same meta convention. Lazy: a format only pays when it actually dispatches.
def arena_ensure_scratch(fmt : int) {
    unsafe {
        var a & = g_gpu.arenas[fmt]
        if (a.ready) {
            return
        }
        a.meta = make_host_buf(META_BYTES, true)
        a.xq = make_host_buf(XQ_BYTES, true)
        a.xs = make_host_buf(XS_BYTES, true)
        a.y = make_host_buf(Y_BYTES, false, [cached = true])
        a.y_dev = make_device_buf(Y_BYTES)
        a.raw_set = vk_set6(a.wqbuf, a.wsbuf, a.meta.buf, a.xq.buf, a.xs.buf, a.y_dev,
            a.wqbytes, a.wsbytes, META_BYTES, XQ_BYTES, XS_BYTES, Y_BYTES,
            fixed_array(0u, 0u, 0u, 0u, 0u, VHG_YO))
        a.ready = true
    }
}

// residual-stream scratch for the add+rms kernel (lazy — only a resident-driver run creates it)
def ensure_ar_state {
    if (g_gpu.ar_ready) {
        return
    }
    let bytes = AR_MAX_DIM * 4l
    g_gpu.ar_x_dev = make_device_buf(bytes)
    g_gpu.ar_a_dev = make_device_buf(bytes)
    g_gpu.ar_w_dev = make_device_buf(bytes)
    g_gpu.ar_y_dev = make_device_buf(bytes)
    g_gpu.ar_meta = make_host_buf(META_BYTES, true)
    g_gpu.ar_scal = make_host_buf(XS_BYTES, true)
    g_gpu.ar_host = make_host_buf(bytes, false, [cached = true])   // x and y readback
    g_gpu.ar_set = vk_set6(g_gpu.ar_x_dev, g_gpu.ar_a_dev, g_gpu.ar_meta.buf, g_gpu.ar_w_dev, g_gpu.ar_scal.buf, g_gpu.ar_y_dev,
        bytes, bytes, META_BYTES, bytes, XS_BYTES, bytes,
        fixed_array(VHG_X, 0u, 0u, 0u, 0u, VHG_YO))   // a/w/meta/scal fill pre-cmd
    g_gpu.ar_ready = true
}

//! Resident-driver seam: x = (x + a) * ascale then y = rms(x) * w, in ONE dispatch. `add` false
//! runs the plain norm (the stack prologue). x is updated in place — it IS the residual stream.
def vk_add_rms(var x : array<float>; a : array<float>; w : array<float>; var y : array<float>;
               dim : int64; eps, ascale : float; add : bool) {
    verify(vk_moe_init())   // assert() refuses side-effecting expressions; the probe must run
    assert(dim <= AR_MAX_DIM, "dasLLAMA vulkan: add_rms dim exceeds AR_MAX_DIM")
    ensure_ar_state()
    let bytes = dim * 4l
    unsafe {
        var mp = reinterpret<uint?>(g_gpu.ar_meta.mapped)
        mp[0] = uint(dim)
        mp[1] = add ? 1u : 0u
        mp[2] = 0u   // standalone seam: norm weight starts at 0
        var sp = reinterpret<float?>(g_gpu.ar_scal.mapped)
        sp[0] = eps
        sp[1] = ascale
        upload_region_at(g_gpu.ar_x_dev, 0l, addr<uint8 const?>(x[0]), bytes)
        upload_region_at(g_gpu.ar_w_dev, 0l, addr<uint8 const?>(w[0]), bytes)
        if (add) {
            upload_region_at(g_gpu.ar_a_dev, 0l, addr<uint8 const?>(a[0]), bytes)
        }
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        hz_dispatch(raw, h, weak_copy(g_gpu.ar_pipeline), g_gpu.ar_set, 1u)   // one workgroup owns the whole row (it reduces)
        vhz_dep(raw, h, VHG_YO, 0u, true)   // x reads back post-fence in the second cmd
        cmd_copy_whole(raw, g_gpu.ar_y_dev, g_gpu.ar_host.buf, bytes)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
        memcpy(addr<void?>(y[0]), g_gpu.ar_host.mapped, bytes)
        // x is the residual stream: read the updated row back too
        var raw2 = alloc_cmd()
        vk_check(vkBeginCommandBuffer(raw2, begin), null)
        cmd_copy_whole(raw2, g_gpu.ar_x_dev, g_gpu.ar_host.buf, bytes)
        vk_check(vkEndCommandBuffer(raw2), null)
        submit_wait(raw2)
        memcpy(addr<void?>(x[0]), g_gpu.ar_host.mapped, bytes)
    }
}

// The arena FFN chain's four sets. Bindings mirror ffn_cmd_for exactly — only bindings 0/1 change,
// from a stack's own plane pair to its format's arena. Each role needs its OWN set because the
// weight base lives at a fixed meta offset, so it cannot vary within one set.
def ensure_af_state(f1, f3, f2 : int) {
    if (g_gpu.af_ready) {
        return
    }
    ensure_ffn_state()
    g_gpu.af_meta_g = make_host_buf(META_BYTES, true)
    g_gpu.af_meta_u = make_host_buf(META_BYTES, true)
    g_gpu.af_meta_d = make_host_buf(META_BYTES, true)
    g_gpu.af_xq = make_host_buf(XQ_BYTES, true)
    g_gpu.af_xs = make_host_buf(XS_BYTES, true)
    g_gpu.af_ya_dev = make_device_buf(Y_BYTES)
    g_gpu.af_yb_dev = make_device_buf(Y_BYTES)
    g_gpu.af_yo_dev = make_device_buf(Y_BYTES)
    g_gpu.af_yo_host = make_host_buf(Y_BYTES, false, [cached = true])
    let a1 = arena_planes(f1)
    let a3 = arena_planes(f3)
    let a2 = arena_planes(f2)
    g_gpu.af_gate_set = vk_set6(a1.wq, a1.ws, g_gpu.af_meta_g.buf, g_gpu.af_xq.buf, g_gpu.af_xs.buf, g_gpu.af_ya_dev,
        a1.wqb, a1.wsb, META_BYTES, XQ_BYTES, XS_BYTES, Y_BYTES,
        fixed_array(0u, 0u, 0u, 0u, 0u, VHG_Y1))
    g_gpu.af_up_set = vk_set6(a3.wq, a3.ws, g_gpu.af_meta_u.buf, g_gpu.af_xq.buf, g_gpu.af_xs.buf, g_gpu.af_yb_dev,
        a3.wqb, a3.wsb, META_BYTES, XQ_BYTES, XS_BYTES, Y_BYTES,
        fixed_array(0u, 0u, 0u, 0u, 0u, VHG_Y2))
    // the act+requant set has no weights: up@0, gate@5, hq/hs out (binding 1 is filler)
    g_gpu.af_actrq_set = vk_set6(g_gpu.af_yb_dev, a1.ws, g_gpu.ffn_meta_dev, g_gpu.hq_dev, g_gpu.hs_dev, g_gpu.af_ya_dev,
        Y_BYTES, a1.wsb, FFN_META_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES, Y_BYTES,
        fixed_array(VHG_Y2, 0u, VHG_META, VHG_HQ, VHG_HQ, VHG_Y1))
    g_gpu.af_down_set = vk_set6(a2.wq, a2.ws, g_gpu.af_meta_d.buf, g_gpu.hq_dev, g_gpu.hs_dev, g_gpu.af_yo_dev,
        a2.wqb, a2.wsb, META_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES, Y_BYTES,
        fixed_array(0u, 0u, 0u, VHG_HQ, VHG_HQ, VHG_YO))
    g_gpu.af_ready = true
}

//! True when a Vulkan device is up (probes lazily). Non-panicking — the resident-driver seams that
//! `verify(vk_moe_init())` are fine on this hardware, but a test wants to feint on a headless box.
def vk_device_ready() : bool => vk_moe_init()

let RKS_MAX_QD = 8_192l
let RKS_MAX_KVD = 1_024l
let RKS_MAX_CTX = 4_096l   // K/V mirror rows in the standalone-kernel scratch
let KV_READBACK_ROWS = 256l   // bulk mirror->host hydration chunk (rows per submit)

def ensure_rks_state {
    if (g_gpu.rks_ready) {
        return
    }
    g_gpu.rks_qkv_dev = make_device_buf((RKS_MAX_QD + 2l * RKS_MAX_KVD) * 4l)
    g_gpu.rks_q_dev = make_device_buf(RKS_MAX_QD * 4l)
    g_gpu.rks_k_dev = make_device_buf(RKS_MAX_CTX * RKS_MAX_KVD * 4l)
    g_gpu.rks_v_dev = make_device_buf(RKS_MAX_CTX * RKS_MAX_KVD * 4l)
    g_gpu.rks_scal = make_host_buf(XS_BYTES, true)
    g_gpu.rks_meta = make_host_buf(META_BYTES, true)
    g_gpu.rks_q_host = make_host_buf(RKS_MAX_QD * 4l, false, [cached = true])
    g_gpu.rks_kv_host = make_host_buf(2l * RKS_MAX_KVD * 4l, false, [cached = true])
    g_gpu.rks_set = vk_set6(g_gpu.rks_qkv_dev, g_gpu.rks_k_dev, g_gpu.rks_meta.buf, g_gpu.rks_v_dev, g_gpu.rks_scal.buf, g_gpu.rks_q_dev,
        (RKS_MAX_QD + 2l * RKS_MAX_KVD) * 4l, RKS_MAX_CTX * RKS_MAX_KVD * 4l, META_BYTES,
        RKS_MAX_CTX * RKS_MAX_KVD * 4l, XS_BYTES, RKS_MAX_QD * 4l,
        fixed_array(0u, VHG_Y2, 0u, VHG_Y3, 0u, VHG_Y1))   // K mirror / V mirror / roped q
    g_gpu.rks_ready = true
}


def ensure_da_state {
    if (g_gpu.da_ready) {
        return
    }
    ensure_rks_state()   // shares the K/V mirror planes
    g_gpu.da_q_dev = make_device_buf(RKS_MAX_QD * 4l)
    g_gpu.da_out_dev = make_device_buf(RKS_MAX_QD * 4l)
    g_gpu.da_scal = make_host_buf(XS_BYTES, true)
    g_gpu.da_meta = make_host_buf(META_BYTES, true)
    g_gpu.da_out_host = make_host_buf(RKS_MAX_QD * 4l, false, [cached = true])
    g_gpu.da_set = vk_set6(g_gpu.da_q_dev, g_gpu.rks_k_dev, g_gpu.da_meta.buf, g_gpu.rks_v_dev, g_gpu.da_scal.buf, g_gpu.da_out_dev,
        RKS_MAX_QD * 4l, RKS_MAX_CTX * RKS_MAX_KVD * 4l, META_BYTES,
        RKS_MAX_CTX * RKS_MAX_KVD * 4l, XS_BYTES, RKS_MAX_QD * 4l,
        fixed_array(0u, 0u, 0u, 0u, 0u, VHG_YO))   // q/mirrors fill pre-cmd (fence-ordered)
    g_gpu.da_ready = true
}

//! Resident-driver seam: decode attention of the roped q row against the `cnt` cached positions in
//! the device K/V mirror (the same planes vk_rope_kv_store wrote), GQA per kv_mul, into out[0..qd).
def vk_decode_attn(var q : array<float>; var out : array<float>; cnt, qd, kv_dim, head_size, kv_mul : int64; scale : float) {
    verify(vk_moe_init())
    assert(head_size <= 256l && qd <= RKS_MAX_QD, "dasLLAMA vulkan: decode-attn geometry over scratch")
    ensure_da_state()
    let n_heads = qd / head_size
    unsafe {
        var mp = reinterpret<uint?>(g_gpu.da_meta.mapped)
        mp[0] = uint(cnt)
        mp[1] = uint(qd)
        mp[2] = uint(kv_dim)
        mp[3] = uint(head_size)
        mp[4] = uint(kv_mul)
        mp[5] = 0u   // standalone seam: mirror base 0
        mp[6] = 0u
        var sp = reinterpret<float?>(g_gpu.da_scal.mapped)
        sp[0] = scale
        upload_region_at(g_gpu.da_q_dev, 0l, addr<uint8 const?>(q[0]), qd * 4l)
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        hz_dispatch(raw, h, weak_copy(g_gpu.da_pipeline), g_gpu.da_set, uint(n_heads))
        vhz_dep(raw, h, VHG_YO, 0u, true)
        cmd_copy_whole(raw, g_gpu.da_out_dev, g_gpu.da_out_host.buf, qd * 4l)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
        memcpy(addr<void?>(out[0]), g_gpu.da_out_host.mapped, qd * 4l)
    }
}

//! Resident-driver seam (standalone-kernel form): rope the q row in place, rope the k row into the
//! device K mirror at row `pos`, copy v raw into the V mirror at `pos`; then read q + that row's
//! K/V back for parity. `neox` picks llama's adjacent-pair vs qwen/gemma's half-offset rope.
def vk_rope_kv_store(var qkv : array<float>; var qout : array<float>; var krow : array<float>; var vrow : array<float>;
                     cossin : array<float>; qd, kv_dim, head_size, pos : int64; neox : bool) {
    verify(vk_moe_init())
    let half = head_size / 2l
    assert(qd <= RKS_MAX_QD && kv_dim <= RKS_MAX_KVD && pos < RKS_MAX_CTX, "dasLLAMA vulkan: rope_kv geometry over scratch")
    ensure_rks_state()
    let kpair0 = qd / 2l
    let npairs = kpair0 + kv_dim / 2l
    unsafe {
        var mp = reinterpret<uint?>(g_gpu.rks_meta.mapped)
        mp[0] = uint(qd)
        mp[1] = uint(kv_dim)
        mp[2] = uint(head_size)
        mp[3] = uint(half)
        mp[4] = neox ? 1u : 0u
        mp[5] = uint(kpair0)
        mp[6] = uint(npairs)
        mp[7] = uint(pos * kv_dim)   // K mirror row offset (f32 elements)
        mp[8] = uint(pos * kv_dim)   // V mirror row offset
        memcpy(g_gpu.rks_scal.mapped, addr<void?>(cossin[0]), head_size * 4l)   // cos[half] + sin[half]
        upload_region_at(g_gpu.rks_qkv_dev, 0l, addr<uint8 const?>(qkv[0]), (qd + 2l * kv_dim) * 4l)
        // q enters the kernel via vk_y (binding 5) = rks_q_dev — seed it with the raw q rows
        upload_region_at(g_gpu.rks_q_dev, 0l, addr<uint8 const?>(qkv[0]), qd * 4l)
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        hz_dispatch(raw, h, weak_copy(g_gpu.rks_pipeline), g_gpu.rks_set, uint((npairs + int64(WG_X) - 1l) / int64(WG_X)))
        vhz_dep(raw, h, VHG_Y1 | VHG_Y2 | VHG_Y3, 0u, true)   // one declaration for the q/k/v readback group
        cmd_copy_whole(raw, g_gpu.rks_q_dev, g_gpu.rks_q_host.buf, qd * 4l)
        cmd_copy_range(raw, g_gpu.rks_k_dev, pos * kv_dim * 4l, g_gpu.rks_kv_host.buf, 0l, kv_dim * 4l)
        cmd_copy_range(raw, g_gpu.rks_v_dev, pos * kv_dim * 4l, g_gpu.rks_kv_host.buf, kv_dim * 4l, kv_dim * 4l)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
        memcpy(addr<void?>(qout[0]), g_gpu.rks_q_host.mapped, qd * 4l)
        memcpy(addr<void?>(krow[0]), g_gpu.rks_kv_host.mapped, kv_dim * 4l)
        memcpy(addr<void?>(vrow[0]), reinterpret<float?>(g_gpu.rks_kv_host.mapped) + kv_dim, kv_dim * 4l)
    }
}

// ===== resident-driver state + the fill-meta family (the driver itself is decode/prefill) =====
// One cb/token: residual + KV mirror stay device-side, host only uploads x + rope row, reads logits.
// v1 re-records per token (still 1 submit). Activations quantized on-device before each GEMV.

struct RLayer {
    // arena block indices + formats for this layer's 7 planes
    bq, bk, bv, bo, b1, b3, b2 : int64
    fq, fk, fv, fo, f1, f3, f2 : int
    // the layer's descriptor sets (built once, referenced every token)
    s_quant_xb : VkDescriptorSet    // dn_requant of the attn-norm activation
    s_q, s_k, s_v : VkDescriptorSet  // q/k/v GEMV
    s_kv : VkDescriptorSet          // merged k+v two-region GEMV (fk == fv: same arena planes)
    kv_merged : bool
    s_qkn : VkDescriptorSet         // per-head q/k rmsnorm (qk_norm models only)
    s_qkn_rope : VkDescriptorSet    // the fused qk-rmsnorm + rope + KV store (qk_norm models only)
    s_rope : VkDescriptorSet
    s_attn : VkDescriptorSet
    s_quant_at : VkDescriptorSet
    s_wo : VkDescriptorSet
    s_addr_ffn : VkDescriptorSet
    s_addr_ffn_rq : VkDescriptorSet   // the fused add+rms+requant twins of addr_ffn / addr_next:
    s_quant_xb2 : VkDescriptorSet     // same inputs, but xq/xs at bindings 4-5 instead of xb
    s_gate, s_up : VkDescriptorSet
    s_actrq : VkDescriptorSet
    s_down : VkDescriptorSet
    s_addr_next : VkDescriptorSet
    s_addr_next_rq : VkDescriptorSet
    // per-dispatch metas (host-visible; contents rewritten per token)
    m_quant_xb, m_q, m_k, m_v, m_qkn, m_qkr, m_rope, m_attn, m_quant_at, m_wo, m_addr_ffn : HostBuf
    m_quant_xb2, m_gate, m_up, m_actrq, m_down, m_addr_next : HostBuf
    next_norm_off : int64   // rms_ffn is w_off for addr_ffn; addr_next uses rms_att[l+1] or final
    made : bool
}

struct RDec {
    ready : bool
    n_layers, dim, qd, kv_dim, head_size, n_heads, kv_mul, hidden, vocab, seq_cap : int64
    neox : bool
    qk_norm : bool                // per-head q/k rmsnorm role armed (norms buffer carries the rows)
    norms_bytes : int64           // full norms buffer size (grows past the fixed layout under qk_norm)
    ar_scal_off : int64            // eps/ascale pair, appended to the norms buffer (float units) —
                                   // the fused ar+requant needs binding 4 for its scale output
    cos_elems : int64              // cos/sin row + (under qk_norm) the rms_q/rms_k rows + eps
    qkn_row_off : int64             // rms_q[0] in the cos buffer (float units); rms_k[l] follows each
    qkn_eps_off : int64             // eps, past the rms rows in the cos buffer
    eps, scale : float
    // per-token device buffers
    x_dev, xb_dev : uint64        // residual, normed activation
    xq_dev, xs_dev : uint64       // quantized activation (shared across the layer's GEMVs)
    q_dev, kv_dev : uint64        // roped q; the k/v projection buffer rope reads (k@qd, v@qd+kvd)
    attn_dev : uint64             // attention output
    aq_dev, as_dev : uint64       // quantized attn (wo input)
    gate_dev, up_dev : uint64
    ffnout_dev : uint64
    xb2_dev : uint64              // wo output
    k_mirror, v_mirror : uint64   // [n_layers x seq_cap x kv_dim] f32
    norms_dev : uint64            // rms_att[l]/rms_ffn[l] interleaved + final, uploaded once
    cos_dev : uint64              // this token's rope row (cos[half] + sin[half])
    scal_dev : uint64             // da_attn scalars: [0] = attn scale
    ar_scal_dev : uint64          // add_rms scalars: [0] = eps, [1] = ascale (1.0 for the residual add)
    logits_dev : uint64
    x_host, logits_host : HostBuf
    kv_readback : HostBuf         // per-layer K/V row -> CPU cache writeback
    cls_meta_dev : uint64
    cls_set : VkDescriptorSet
    s_prologue : VkDescriptorSet  // xb = rms_att[0](x)
    s_prologue_rq : VkDescriptorSet   // the fused twin (also does layer 0's activation quant)
    m_prologue : HostBuf
    s_quant_final : VkDescriptorSet
    m_quant_final : HostBuf
    dummy : uint64                // filler for unused bindings
    cls_fmt_marker : int
    layers : array<RLayer>
    cmd : VkCommandBuffer
    tok_recorded : bool           // cmd holds the whole-token chain, re-submittable as-is
    cos_host : HostBuf            // this token's rope row, copied to cos_dev in-cmd
    ffn_meta_h : HostBuf          // decode-owned actrq meta source (g_gpu.ffn_meta is shared with the batch rails)
    // prefill batch buffers (lazy — sized MAX_NPOS x their width) + per-layer prefill sets
    pf_ready : bool
    pf_np : int64                 // MAX_NPOS the buffers were sized for
    pf_x, pf_xb, pf_xq, pf_xs : uint64
    pf_q, pf_kv, pf_v, pf_attn, pf_aq, pf_as : uint64
    pf_gate, pf_up, pf_ffnout, pf_xb2 : uint64
    pf_cos : uint64               // [MAX_NPOS x hs] rope rows
    pf_xf, pf_af, pf_hf : uint64  // cm2 (fmt-6) f16 twin feeds: xb rows, attn rows, hidden rows
    pf_logits_host : HostBuf
    pf_meta : array<HostBuf>      // one host meta per (layer, role) + prologue/final/cls
    pf_sets : array<VkDescriptorSet>
    pf_cmd : VkCommandBuffer
}

let PF_WINDOW = 512l   // prefill activation-buffer rows; longer prompts run as sequential windows

var g_rd : RDec?

// a small host-visible meta buffer (uint words), contents filled per token during recording
def rd_meta() : HostBuf => make_host_buf(META_BYTES, true)

// one arena's plane handles + sizes in ONE table lookup (das rejects two lookups in one expr)
def arena_planes(fmt : int) : tuple<wq : uint64; ws : uint64; wqb : int64; wsb : int64> {
    unsafe {
        var a & = g_gpu.arenas[fmt]
        return (wq = a.wqbuf, ws = a.wsbuf, wqb = a.wqbytes, wsb = a.wsbytes)
    }
}

// gemv meta: [0]=n [1]=d [2]=nregions=1 [3]=ybase(row) [4]=weight block [5]=xblock base
def fill_gemv_meta(hb : HostBuf; n, d, ybase, blk : int64) {
    unsafe {
        var mp = reinterpret<uint?>(hb.mapped)
        mp[0] = uint(n)
        mp[1] = uint(d)
        mp[2] = 1u
        mp[3] = uint(ybase)
        mp[4] = uint(blk)
        mp[5] = 0u
    }
}

// two-region form: regions land contiguously in y at ybase, ybase+d (groupn's r*d+row rule)
def fill_gemv_meta2(hb : HostBuf; n, d, ybase, blk_a, blk_b : int64) {
    unsafe {
        var mp = reinterpret<uint?>(hb.mapped)
        mp[0] = uint(n)
        mp[1] = uint(d)
        mp[2] = 2u
        mp[3] = uint(ybase)
        mp[4] = uint(blk_a)
        mp[5] = 0u
        mp[6] = uint(blk_b)
        mp[7] = 0u
    }
}

// dn_requant meta: [10]=input base (0) [18]=block count
def fill_quant_meta(hb : HostBuf; nblocks : int64) {
    unsafe {
        var mp = reinterpret<uint?>(hb.mapped)
        mp[10] = 0u
        mp[18] = uint(nblocks)
    }
}

// the f32->f16 convert's meta (arena cm2 chain): word 0 = element count
def fill_cvt_meta(hb : HostBuf; elems : int64) {
    unsafe {
        var mp = reinterpret<uint?>(hb.mapped)
        mp[0] = uint(elems)
    }
}

// word 3 is the fused ar+requant's eps/ascale offset; the split ar kernel reads only 0-2
def fill_addrms_meta(hb : HostBuf; dim, woff : int64; add : bool) {
    unsafe {
        var mp = reinterpret<uint?>(hb.mapped)
        mp[0] = uint(dim)
        mp[1] = add ? 1u : 0u
        mp[2] = uint(woff)
        mp[3] = uint(g_rd.ar_scal_off)
    }
}

// the fused qk-rmsnorm+rope meta; words 9-11 index the COS buffer, not the norms buffer
def fill_qkn_rope_meta(hb : HostBuf; l : int64) {
    unsafe {
        let hs = g_rd.head_size
        var mp = reinterpret<uint?>(hb.mapped)
        mp[0] = uint(g_rd.qd)
        mp[1] = uint(g_rd.kv_dim)
        mp[2] = uint(hs)
        mp[3] = uint(hs / 2l)
        mp[4] = g_rd.neox ? 1u : 0u
        mp[5] = uint(g_rd.n_heads)
        mp[9] = uint(g_rd.qkn_row_off + l * 2l * hs)
        mp[10] = uint(g_rd.qkn_row_off + (l * 2l + 1l) * hs)
        mp[11] = uint(g_rd.qkn_eps_off)
    }
}

def rd_gemv_wg(rows : int64) : uint => uint((rows + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg)

// ===== GPU per-dispatch profiler (DASLLAMA_GPU_PROF=1) =====
// BOTTOM_OF_PIPE stamps; results ride a same-submit copy, no extra fence. Overlapped hazard
// levels put the whole level's cost on the LAST member's delta (earlier members read near-zero)
let PFQ_CAP = 2048u
var g_pfq_pool : QueryPool
var g_pfq_buf : HostBuf
var g_pfq_ready = false
var g_pfq_on = false
var g_pfq_n = 0u
var g_pfq_period = 0.0

def ensure_pfq {
    if (g_pfq_ready) {
        return
    }
    var lp : VkPhysicalDeviceProperties
    vkGetPhysicalDeviceProperties(g_gpu.phys, lp)
    g_pfq_period = lp.limits.timestampPeriod
    g_pfq_pool <- create_query_pool(g_gpu.device, QueryPoolCreateInfo(queryType = VkQueryType.TIMESTAMP, queryCount = PFQ_CAP))
    g_pfq_buf = make_host_buf(int64(PFQ_CAP) * 8l, true, [cached = true])
    g_gpu.host_mem |> erase(g_pfq_buf.buf)   // process-lifetime — exempt from the model-drop sweep
    g_pfq_ready = true
}

def pfq_begin(raw : VkCommandBuffer) {
    if (!vk_prof()) {
        return
    }
    ensure_pfq()
    g_pfq_on = true
    g_pfq_n = 0u
    cmd_reset_query_pool(vk_value_to_boost(raw), g_pfq_pool, 0u, PFQ_CAP)
}

def pfq_ts(raw : VkCommandBuffer) {
    if (g_pfq_on && g_pfq_n < PFQ_CAP) {
        var stage : VkPipelineStageFlags
        stage.bottom_of_pipe = true
        cmd_write_timestamp(vk_value_to_boost(raw), stage, g_pfq_pool, g_pfq_n)
        g_pfq_n++
    }
}

def pfq_end(raw : VkCommandBuffer) {
    if (g_pfq_on) {
        var qrf : VkQueryResultFlags
        qrf._64 = true
        qrf.wait = true
        cmd_copy_query_pool_results(vk_value_to_boost(raw), g_pfq_pool, 0u, g_pfq_n,
            nonowning_buf(g_pfq_buf.buf), 0ul, 8ul, qrf)
        g_pfq_on = false
    }
}

// query i's delta to its predecessor, usec
def pfq_us(i : uint) : double {
    unsafe {
        var tp = reinterpret<uint64?>(g_pfq_buf.mapped)
        return double(tp[i] - tp[i - 1u]) * double(g_pfq_period) / 1000.0lf
    }
}

// ===== the hazard-mask rail (the dasLLAMA-metal bar_dep port: declared masks, derived barriers) =====
// Nodes declare role-bit (region, not VkBuffer) r/w masks; barrier only on RAW/WAW/WAR vs pending, then reset.
// Barrier dst is always full-stage (the wholesale reset relies on it); src narrows to the pending stages.

let VHZ_X = 0x1u          // residual x
let VHZ_XB = 0x2u         // normed panel xb
let VHZ_XQ = 0x4u         // quantized activations xq+xs
let VHZ_Q = 0x8u          // q rows
let VHZ_KVK = 0x10u       // kv panel, k half
let VHZ_KVV = 0x20u       // kv panel, v half
let VHZ_VST = 0x40u       // prefill v-GEMM staging (pf_v)
let VHZ_MIR = 0x80u       // k/v mirrors
let VHZ_ATT = 0x100u      // attention out
let VHZ_AQ = 0x200u       // quantized attention out aq+as
let VHZ_XB2 = 0x400u      // wo out
let VHZ_GATE = 0x800u     // ffn gate plane
let VHZ_UP = 0x1000u      // ffn up plane
let VHZ_HQ = 0x2000u      // requantized hidden hq+hs
let VHZ_FFO = 0x4000u     // ffn down out
let VHZ_META = 0x8000u    // ffn_meta_dev
let VHZ_LOG = 0x10000u    // logits
let VHZ_COS = 0x20000u    // rope cos/sin rows

// batch/hybrid-tier region bits — a separate namespace over the same rail (each recorder's VkHaz
// is private to its cmd buffer, so the two families never meet in one pending set)
let VHB_ACT = 0x1u        // batch_xq/batch_xs activation staging
let VHB_META = 0x2u       // stack batch metas + the chain's params (ffn/dn/at meta)
let VHB_Y1 = 0x4u         // batch_y1 (gate / qkv / q|gate / down / wo out)
let VHB_Y2 = 0x8u         // batch_y2 (up / z / k out)
let VHB_HQ = 0x10u        // hq+hs requant image
let VHB_COMB = 0x20u      // combine weight/bucket planes
let VHB_CY = 0x40u        // combine accumulator
let VHB_SM = 0x80u        // smalls image (dn/at)
let VHB_ST = 0x100u       // dn recurrent state
let VHB_WS = 0x200u       // dn slabs/o scratch
let VHB_ATQ = 0x400u      // at deinterleaved q rows
let VHB_ATG = 0x800u      // at gate/o rows
let VHB_ATK = 0x1000u     // at roped-k panel
let VHB_ATV = 0x2000u     // at v panel (absolute positions)
let VHB_VRAW = 0x4000u    // at raw-v window
let VHB_XF = 0x8000u      // cm2: f16 gathered-activation plane (dequant out, gate/up twin B)
let VHB_HF = 0x10000u     // cm2: f16 hidden plane (actf16 out, down twin B)

// serial-seam region bits (decode GEMV preps, arena + standalone seams) — third namespace, same rail
let VHG_META = 0x1u       // ffn/dn params staged to device
let VHG_SM = 0x2u         // dn smalls image (β/γ + parity ring)
let VHG_Y1 = 0x4u         // gate / qkv / q out
let VHG_Y2 = 0x8u         // up / z / k out
let VHG_Y3 = 0x10u        // v out (qkv group, rope v mirror)
let VHG_HQ = 0x20u        // hq+hs requant image
let VHG_ST = 0x40u        // dn recurrent state (in place)
let VHG_WS = 0x80u        // dn o scratch (oq+os)
let VHG_X = 0x100u        // add_rms residual x (in place)
let VHG_YO = 0x200u       // the host-bound result plane (the DMA source)

struct VkHaz {
    w : uint            // bits with a pending (unbarriered) write
    r : uint            // bits with a pending read
    pend_compute : bool // stages present in the pending set — the next barrier's src side
    pend_transfer : bool
    barriers : int
    disp : int
}

// ===== the derived-dispatch registry (the vulkan lens) =====
// Pipelines register lens-derived binding access masks at creation, descriptor sets declare
// per-binding region bits at build — hz_dispatch composes the two into vhz_dep masks.

var g_kernel_access : table<string; uint2>   // kernel name -> (read-bindings, write-bindings)
var g_pipe_access : table<uint64; uint2>     // VkPipeline -> (read-bindings, write-bindings)
var g_set_bits : table<uint64; SetBits>      // VkDescriptorSet -> per-binding region-bit masks

struct SetBits {
    b : uint[6]
}

def ensure_kernel_access {
    if (!empty(g_kernel_access)) {
        return
    }
    for (ent in split(vk_kernels_access_spec(), ";")) {
        if (ent == "") {
            continue
        }
        let nv <- split(ent, "=")
        let rw <- split(nv[1], ",")
        g_kernel_access |> insert(nv[0], uint2(uint(to_int(rw[0])), uint(to_int(rw[1]))))
    }
}

// declare the region bits sitting at each of a set's 6 bindings (0 = untracked); a set must be
// declared before hz_dispatch can serve it — the strict rail panics on an undeclared set
def hz_set_bits(set_ : VkDescriptorSet; b0, b1, b2, b3, b4, b5 : uint) {
    var sb : SetBits
    sb.b[0] = b0
    sb.b[1] = b1
    sb.b[2] = b2
    sb.b[3] = b3
    sb.b[4] = b4
    sb.b[5] = b5
    g_set_bits[intptr(set_)] = sb
}

def hz_set_bits(set_ : VkDescriptorSet; gbits : uint[6]) {
    var sb : SetBits
    for (i in range(6)) {
        sb.b[i] = gbits[i]
    }
    g_set_bits[intptr(set_)] = sb
}

def hz_pipe_register(pipe : Pipeline; kernel : string) {
    ensure_kernel_access()
    if (!key_exists(g_kernel_access, kernel)) {
        panic("dasLLAMA vulkan lens: no derived access for kernel {kernel}")
    }
    g_pipe_access[intptr(boost_value_to_vk(pipe))] = g_kernel_access[kernel]
}

// create a kernel's pipeline and register its lens-derived access masks (the shader-module local
// is deliberately never finalized — same lifetime the hand-written creation block had)
def make_kernel_pipe(kernel : string; var spv : array<uint>) : Pipeline {
    var shader <- create_shader_module(g_gpu.device, spv)
    var pipe <- create_compute_pipeline(g_gpu.device, g_gpu.pipe_layout, shader)
    hz_pipe_register(pipe, kernel)
    return <- pipe
}

// derive this dispatch's hazard masks: the set's region bits distributed over the kernel's
// read/write bindings. Undeclared pipeline or set = a programming error, not a fallback.
def hz_masks(pipe : Pipeline; var set_ : VkDescriptorSet) : tuple<r : uint; w : uint> {
    let pk = intptr(boost_value_to_vk(pipe))
    if (!key_exists(g_pipe_access, pk)) {
        panic("dasLLAMA vulkan lens: dispatch through an unregistered pipeline")
    }
    let sk = intptr(set_)
    if (!key_exists(g_set_bits, sk)) {
        panic("dasLLAMA vulkan lens: dispatch against a set with no declared region bits")
    }
    let acc = g_pipe_access[pk]
    let sb = g_set_bits[sk]
    var r = 0u
    var w = 0u
    for (i in range(6)) {
        let bit = 1u << uint(i)
        if ((acc.x & bit) != 0u) {
            r |= sb.b[i]
        }
        if ((acc.y & bit) != 0u) {
            w |= sb.b[i]
        }
    }
    return (r, w)
}

// the derived-mask dispatch node: declare via the composed masks, bind, dispatch
def hz_dispatch(raw : VkCommandBuffer; var h : VkHaz; pipe : Pipeline; var set_ : VkDescriptorSet; groups : uint) {
    let m = hz_masks(pipe, set_)
    vhz_dep(raw, h, m.r, m.w)
    cmd_bind_pipeline(vk_value_to_boost(raw), pipe, VkPipelineBindPoint.COMPUTE)
    cmd_bind_dispatch(raw, set_, groups)
}

var g_vhz_paranoid = -1   // DASLLAMA_VK_HAZARD_PARANOID: barrier before every node
var g_vhz_trace = -1      // DASLLAMA_VK_HAZARD_TRACE: log every declared node (schedule oracle)

def vhz_paranoid : bool {
    if (g_vhz_paranoid < 0) {
        g_vhz_paranoid = env_flag("DASLLAMA_VK_HAZARD_PARANOID", false) ? 1 : 0
    }
    return g_vhz_paranoid != 0
}

def vhz_trace : bool {
    if (g_vhz_trace < 0) {
        g_vhz_trace = env_flag("DASLLAMA_VK_HAZARD_TRACE", false) ? 1 : 0
    }
    return g_vhz_trace != 0
}

var g_vk_fa = -1   // DASLLAMA_VK_FA=0 pins the generic da_attn_b (flash-attn bisect hatch)

def vk_fa_on : bool {
    if (g_vk_fa < 0) {
        g_vk_fa = env_flag("DASLLAMA_VK_FA", true) ? 1 : 0
    }
    return g_vk_fa != 0
}

var g_vk_fuse = -1   // DASLLAMA_VK_FUSE=0 pins the split decode-tail dispatches (same-build A/B)

def vk_fuse_on : bool {
    if (g_vk_fuse < 0) {
        g_vk_fuse = env_flag("DASLLAMA_VK_FUSE", true) ? 1 : 0
    }
    return g_vk_fuse != 0
}

// the per-node gate: barrier iff this node's declared masks conflict with the pending set
def vhz_dep(raw : VkCommandBuffer; var h : VkHaz; rmask, wmask : uint; transfer : bool = false) {
    h.disp++
    if (vhz_trace()) {
        to_log(LOG_INFO, "vhz {h.disp} r={rmask} w={wmask} t={transfer ? 1 : 0}\n")
    }
    var bar = (rmask & h.w) != 0u || (wmask & h.w) != 0u || (wmask & h.r) != 0u
    if (vhz_paranoid() && (h.w != 0u || h.r != 0u)) {
        bar = true
    }
    if (bar) {
        var mb : MemoryBarrier
        mb.srcAccessMask.shader_write = h.pend_compute
        mb.srcAccessMask.transfer_write = h.pend_transfer
        mb.dstAccessMask.shader_read = true
        mb.dstAccessMask.shader_write = true
        mb.dstAccessMask.transfer_read = true
        mb.dstAccessMask.transfer_write = true
        var mbs : array<MemoryBarrier>
        mbs |> emplace(mb)
        var sstage : VkPipelineStageFlags
        sstage.compute_shader = h.pend_compute
        sstage.transfer = h.pend_transfer
        var dstage : VkPipelineStageFlags
        dstage.compute_shader = true
        dstage.transfer = true
        let dflags : VkDependencyFlags
        let no_bufb : array<BufferMemoryBarrier>
        let no_imgb : array<ImageMemoryBarrier>
        cmd_pipeline_barrier(vk_value_to_boost(raw), sstage, dstage, dflags, mbs, no_bufb, no_imgb)
        h.w = 0u
        h.r = 0u
        h.pend_compute = false
        h.pend_transfer = false
        h.barriers++
    }
    h.r |= rmask
    h.w |= wmask
    if (transfer) {
        h.pend_transfer = true
    } else {
        h.pend_compute = true
    }
}

def rd_dispatch(raw : VkCommandBuffer; pipe : Pipeline; var set : VkDescriptorSet; groups : uint; var h : VkHaz) {
    hz_dispatch(raw, h, pipe, set, groups)
    pfq_ts(raw)
}

// GEMV dispatch — the per-format kernel selected off the plane format
def rd_dispatch_gemv(raw : VkCommandBuffer; fmt : int; var set : VkDescriptorSet; groups : uint; var h : VkHaz) {
    rd_dispatch(raw, gemv_pipe(fmt), set, groups, h)
}

// decode per-role accumulation across tokens (stamps overwrite each submit); printed every 32
var g_rdq_n = 0u
var g_rdq_count = 0l
var g_rdq_total = 0.0lf
var g_rdq_pro = 0.0lf
var g_rdq_tail = 0.0lf
var g_rdq_role : double[16]

// One-shot command buffers (the KV readback rails) are freed by their caller — registering them
// in model_cmds would grow it per hydration until the next model drop.
def alloc_transient_cmd : VkCommandBuffer {
    var ai = VkCommandBufferAllocateInfo()
    ai.commandPool = boost_value_to_vk(g_gpu.pool)
    ai.level = VkCommandBufferLevel.PRIMARY
    ai.commandBufferCount = 1u
    var raw : VkCommandBuffer
    unsafe {
        vk_check(vkAllocateCommandBuffers(dev_raw(), ai, addr(raw)), null)
    }
    return raw
}

def free_transient_cmd(var raw : VkCommandBuffer) {
    unsafe {
        vkFreeCommandBuffers(dev_raw(), boost_value_to_vk(g_gpu.pool), 1u, addr(raw))
    }
}

def alloc_cmd : VkCommandBuffer {
    var raw = alloc_transient_cmd()
    g_gpu.model_cmds |> push(raw)   // all persistent cmds are model-owned; freed by the model drop
    return raw
}

def cmd_bind_dispatch(raw : VkCommandBuffer; var set_ : VkDescriptorSet; groups : uint) {
    unsafe {
        vkCmdBindDescriptorSets(raw, VkPipelineBindPoint.COMPUTE, boost_value_to_vk(g_gpu.pipe_layout), 0u, 1u, addr(set_), 0u, null)
    }
    vkCmdDispatch(raw, groups, 1u, 1u)
}

// the GEMV pipeline for a stack's format (0 = the q8 kernel, 1/2/3/4 = kq_moe_gemv_k4/5/6/q40)
def gemv_pipe(fmt : int) : Pipeline {
    if (fmt == 0) {
        return weak_copy(g_gpu.pipeline)
    } elif (fmt == 1) {
        return weak_copy(g_gpu.gemv_k4)
    } elif (fmt == 2) {
        return weak_copy(g_gpu.gemv_k5)
    } elif (fmt == 3) {
        return weak_copy(g_gpu.gemv_k6)
    } elif (fmt == 4) {
        return weak_copy(g_gpu.gemv_q40)
    } elif (fmt == 6) {
        return weak_copy(g_gpu.gemv_q8n)
    }
    panic("dasLLAMA vulkan tier: no GEMV kernel for stack format {fmt}")
    return weak_copy(g_gpu.pipeline)
}

var g_mm_m_logged = false   // one-time mul_mm M-tile engagement log

// The Q8_0 prefill batch pipeline, switched to the tensor-core coopmat variant when DASLLAMA_COOPMAT
// selected one (weak alias — g_gpu owns). `tile` = the edge the caller's meta used; picks L vs M in mm mode.
def q8_batch_pipe(tile : int64; aligned : bool = false) : Pipeline {
    if (g_gpu.coopmat_mode == 1) {
        return weak_copy(g_gpu.batch_coopmat_f16)
    } elif (g_gpu.coopmat_mode == 2) {
        return weak_copy(g_gpu.batch_coopmat_i8)
    } elif (g_gpu.coopmat_mode >= 3) {
        if (tile == BATCH_TILE) {   // the small tier IS the sdot4 kernel — measured, not a fallback
            return weak_copy(g_gpu.batch_pipeline)
        }
        if (tile == 64l) {
            if (!g_mm_m_logged) {
                g_mm_m_logged = true
                to_log(LOG_INFO, "dasLLAMA vulkan tier: mul_mm M-tile engaged\n")
            }
            return weak_copy(g_gpu.batch_coopmat_mm_m)
        }
        return weak_copy(aligned ? g_gpu.batch_coopmat_mm_a : g_gpu.batch_coopmat_mm)
    }
    return weak_copy(g_gpu.batch_pipeline)
}

def batch_pipe(fmt : int; tile : int64; aligned : bool = false) : Pipeline {
    if (fmt == 0) {
        return q8_batch_pipe(tile, aligned)
    } elif (fmt == 1) {
        return weak_copy(g_gpu.batch_k4)
    } elif (fmt == 2) {
        return weak_copy(g_gpu.batch_k5)
    } elif (fmt == 3) {
        return weak_copy(g_gpu.batch_k6)
    } elif (fmt == 4) {
        // Q4_0 rides the f16 coopmat arm too (no s8-native for nibbles — mode 2 falls back to sdot4)
        return weak_copy(g_gpu.coopmat_mode == 1 ? g_gpu.batch_coopmat_q40 : g_gpu.batch_q40)
    } elif (fmt == 6) {
        return weak_copy(g_gpu.batch_cm2)
    }
    panic("dasLLAMA vulkan tier: no batch tile for stack format {fmt}")
    return weak_copy(g_gpu.batch_pipeline)
}

def submit_nowait(var raw : VkCommandBuffer) {
    var submit = VkSubmitInfo()
    submit.commandBufferCount = 1u
    let fence = g_gpu.fence
    // a pending transfer-queue handoff attaches here — later in-order submits inherit the ordering
    var tss = VkTimelineSemaphoreSubmitInfo()
    var wval = g_gpu.pend_wait
    var sem = g_gpu.xfer_sem
    var wstage : VkPipelineStageFlags
    wstage.transfer = true
    wstage.compute_shader = true
    unsafe {
        submit.pCommandBuffers = addr(raw)
        if (wval > 0ul) {
            tss.waitSemaphoreValueCount = 1u
            tss.pWaitSemaphoreValues = addr(wval)
            submit.pNext = addr<void?>(tss)
            submit.waitSemaphoreCount = 1u
            submit.pWaitSemaphores = addr(sem)
            submit.pWaitDstStageMask = addr<uint?>(wstage)
            g_gpu.pend_wait = 0ul
        }
        vk_check(vkQueueSubmit(g_gpu.queue, 1u, addr(submit), fence), null)
    }
}

def wait_fence {
    var fence = g_gpu.fence
    unsafe {
        // spin, never block: vkWaitForFences pays 1-2ms OS wake-up latency per submit (measured)
        while (vkGetFenceStatus(dev_raw(), fence) != VkResult.SUCCESS) {}
        vk_check(vkResetFences(dev_raw(), 1u, addr(fence)), null)
    }
}

def submit_wait(var raw : VkCommandBuffer) {
    submit_nowait(raw)
    wait_fence()
}

// refresh a stack's mapped GEMV meta, one entry per ROW (cnt > 1 regions expand so output lands
// row-major): [n, d, nrows, ybase, (rel_wblock, xblock)*], block units per format (32 q8 / 256 kq).
def fill_stack_meta_rows(si : int; offs : int64 const?; nregions : int64; n, d, nrows, xnb : int64; ybase : int64 = 0l) {
    assert((4l + nrows * 2l) * 4l <= META_BYTES, "dasLLAMA vulkan tier: FFN meta exceeds capacity")
    let unit = fmt_unit(g_gpu.stacks[si].fmt)
    unsafe {
        var mp = reinterpret<uint?>(g_gpu.stacks[si].meta.mapped)
        mp[0] = uint(n)
        mp[1] = uint(d)
        mp[2] = uint(nrows)
        mp[3] = uint(ybase)
        for (r in range64(nregions)) {
            let rel = offs[r * 3l] - g_gpu.stacks[si].base
            let row0 = offs[r * 3l + 1l]
            let cnt = offs[r * 3l + 2l]
            assert(rel % unit == 0l, "dasLLAMA vulkan tier: unaligned region offset")
            for (i in range64(cnt)) {
                mp[4l + (row0 + i) * 2l] = uint(rel / unit)
                mp[5l + (row0 + i) * 2l] = uint((row0 + i) * xnb)
            }
        }
    }
}

def fill_stack_acts(si : int; xqp : int8 const?; xsp : float const?; xq_bytes : int64) {
    assert(xq_bytes <= XQ_BYTES, "dasLLAMA vulkan tier: dispatch exceeds scratch capacity")
    let unit = fmt_unit(g_gpu.stacks[si].fmt)
    unsafe {
        memcpy(g_gpu.stacks[si].xq.mapped, reinterpret<void?>(xqp), xq_bytes)
        memcpy(g_gpu.stacks[si].xs.mapped, reinterpret<void?>(xsp), (xq_bytes / unit) * 4l)
    }
}

// ===== the FFN mid-chain state (shared by the decode-GEMV and batch arms) =====

def ensure_ffn_state {
    if (g_gpu.ffn_ready) {
        return
    }
    g_gpu.hq_dev = make_device_buf(BATCH_HQ_BYTES)
    g_gpu.hs_dev = make_device_buf(BATCH_HS_BYTES)
    g_gpu.ffn_meta = make_host_buf(FFN_META_BYTES, false)   // transfer_src staging
    g_gpu.ffn_meta_dev = make_device_buf(FFN_META_BYTES)
    g_gpu.ffn_ready = true
}

def ensure_dn_state {
    if (!g_gpu.dn_shared_ok) {
        panic("dasLLAMA vulkan tier: deltanet dispatch reached a device whose shared-memory cap declined the chain — route gating failed")
    }
    if (g_gpu.dn_ready) {
        return
    }
    ensure_batch_state()
    g_gpu.dn_meta = make_host_buf(DN_META_BYTES, false)
    g_gpu.dn_meta_dev = make_device_buf(DN_META_BYTES)
    g_gpu.dn_smalls = make_host_buf(DN_SMALLS_BYTES, false)
    g_gpu.dn_smalls_dev = make_device_buf(DN_SMALLS_BYTES)
    g_gpu.dn_state_stage = make_host_buf(DN_STATE_BYTES, false)
    g_gpu.dn_state_host = make_host_buf(DN_STATE_BYTES, true, [cached = true])
    g_gpu.dn_tail_host = make_host_buf(DN_TAIL_BYTES, true, [cached = true])
    g_gpu.dn_state_dev = make_device_buf(DN_STATE_BYTES)
    g_gpu.dn_ws_dev = make_device_buf(DN_WS_BYTES)
    // conv bindings: qkv@5, conv output@0 (vk_upf view of hq_dev), smalls@4, params@2; 1/3 fillers
    g_gpu.dn_conv_set = vk_set6(g_gpu.hq_dev, g_gpu.dn_meta_dev, g_gpu.dn_meta_dev, g_gpu.dn_meta_dev, g_gpu.dn_smalls_dev, g_gpu.batch_y1_dev,
        BATCH_HQ_BYTES, DN_META_BYTES, DN_META_BYTES, DN_META_BYTES, DN_SMALLS_BYTES, BATCH_Y_BYTES,
        fixed_array(VHB_HQ, 0u, VHB_META, 0u, VHB_SM, VHB_Y1))
    // scan bindings: conv rows@0, state@1, slabs/o@3, smalls@4, z rows@5, params@2
    g_gpu.dn_scan_set = vk_set6(g_gpu.hq_dev, g_gpu.dn_state_dev, g_gpu.dn_meta_dev, g_gpu.dn_ws_dev, g_gpu.dn_smalls_dev, g_gpu.batch_y2_dev,
        BATCH_HQ_BYTES, DN_STATE_BYTES, DN_META_BYTES, DN_WS_BYTES, DN_SMALLS_BYTES, BATCH_Y_BYTES,
        fixed_array(VHB_HQ, VHB_ST, VHB_META, VHB_WS, VHB_SM, VHB_Y2))
    // requant bindings: o rows@0, quant words@3, scales@4 (out-proj activation image), params@2; 1/5 fillers
    g_gpu.dn_rq_set = vk_set6(g_gpu.dn_ws_dev, g_gpu.dn_meta_dev, g_gpu.dn_meta_dev, g_gpu.hq_dev, g_gpu.hs_dev, g_gpu.dn_meta_dev,
        DN_WS_BYTES, DN_META_BYTES, DN_META_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES, DN_META_BYTES,
        fixed_array(VHB_WS, 0u, VHB_META, VHB_HQ, VHB_HQ, 0u))
    g_gpu.dn_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: deltanet chain engaged\n")
}

def ensure_batch_state {
    if (g_gpu.batch_ready) {
        return
    }
    ensure_ffn_state()
    g_gpu.batch_xq_dev = make_device_buf(BATCH_XQ_BYTES)
    g_gpu.batch_xs_dev = make_device_buf(BATCH_XS_BYTES)
    g_gpu.batch_y1_dev = make_device_buf(BATCH_Y_BYTES)
    g_gpu.batch_y2_dev = make_device_buf(BATCH_Y_BYTES)
    g_gpu.batch_y1 = make_host_buf(BATCH_Y_BYTES, true, [cached = true])
    g_gpu.batch_y2 = make_host_buf(BATCH_Y_BYTES, true, [cached = true])
    g_gpu.batch_cmd = alloc_cmd()
    // fused act+requant bindings: up@0, params@2, hq/hs@3/4, gate@5; binding 1 is unused filler
    g_gpu.batch_actrq_set = vk_set6(g_gpu.batch_y2_dev, g_gpu.batch_xs_dev, g_gpu.ffn_meta_dev, g_gpu.hq_dev, g_gpu.hs_dev, g_gpu.batch_y1_dev,
        BATCH_Y_BYTES, BATCH_XS_BYTES, FFN_META_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES, BATCH_Y_BYTES,
        fixed_array(VHB_Y2, 0u, VHB_META, VHB_HQ, VHB_HQ, VHB_Y1))
    g_gpu.resident_bytes += BATCH_XQ_BYTES + BATCH_XS_BYTES
    g_gpu.batch_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: prefill batch tier engaged\n")
}

def fill_ffn_meta(elems, nblocks : int64; is_gelu : bool; comb_n : int64 = 0l; comb_k : int64 = 0l; comb_r0 : int64 = 0l; comb_r1 : int64 = 0l; xf_words : int64 = 0l) {
    unsafe {
        var mp = reinterpret<uint?>(g_gpu.ffn_meta.mapped)
        mp[0] = uint(elems)
        mp[1] = is_gelu ? 1u : 0u
        mp[2] = uint(nblocks)
        mp[3] = uint(xf_words)   // cm2 chain: the dequant kernel's word count (q8 image / 4)
        mp[4] = uint(comb_n)
        mp[5] = uint(comb_k)
        mp[6] = uint(comb_r0)
        mp[7] = uint(comb_r1)
    }
}

// the 20-word deltanet params block — ONE layout for the prefill window chain (dn_meta) and the
// decode step (dnd_meta, npos_w = 1 and the slab/history words zeroed): the dn kernels' contract
def fill_dn_meta(hb : HostBuf; npos_w, cd, kd, di, nvh, nkh, ds, dconv : int64; hist_mode : int;
                 slab : int64 = 0l; o_off : int64 = 0l; nblocks : int64 = 0l; p2_off : int64 = 0l) {
    unsafe {
        var mp = reinterpret<uint?>(hb.mapped)
        mp[0] = uint(npos_w)
        mp[1] = uint(cd)
        mp[2] = uint(kd)
        mp[3] = uint(di)
        mp[4] = uint(nvh)
        mp[5] = uint(nkh)
        mp[6] = uint(ds)
        mp[7] = uint(dconv)
        mp[8] = uint(hist_mode)
        mp[9] = uint(slab)
        mp[10] = uint(o_off)
        mp[11] = uint(DN_SM_BETA)
        mp[12] = uint(DN_SM_G)
        mp[13] = uint(DN_SM_TAPS)
        mp[14] = uint(DN_SM_WNORM)
        mp[15] = uint(DN_SM_A)
        mp[16] = uint(DN_SM_DT)
        mp[17] = uint(DN_SM_HIST)
        mp[18] = uint(nblocks)
        mp[19] = uint(p2_off)
    }
}

// parallel host memcpy — the batch chain moves 8-34MB per hop and a single lane runs ~14GB/s
// while the box's DRAM feeds 3-4x that; jobque lanes are idle while the GPU owns the layer
let PAR_COPY_CHUNK = 2_097_152l

def par_memcpy(dst : void?; src : void?; bytes : int64) {
    if (bytes < PAR_COPY_CHUNK * 2l || !is_job_que_available()) {
        unsafe {
            memcpy(dst, src, bytes)
        }
        return
    }
    let nch = int((bytes + PAR_COPY_CHUNK - 1l) / PAR_COPY_CHUNK)
    var d8 = unsafe(reinterpret<uint8?>(dst))
    var s8 = unsafe(reinterpret<uint8?>(src))
    maybe_parallel_for(0, nch, nch) $(cb, ce) {
        unsafe {
            for (c in range(cb, ce)) {
                let off = int64(c) * PAR_COPY_CHUNK
                memcpy(reinterpret<void?>(d8 + off), reinterpret<void?>(s8 + off), min(PAR_COPY_CHUNK, bytes - off))
            }
        }
    }
}

def cmd_copy_whole(raw : VkCommandBuffer; src, dst : uint64; bytes : int64) {
    var regions : array<BufferCopy>
    regions |> push(BufferCopy(srcOffset = 0ul, dstOffset = 0ul, size = uint64(bytes)))
    cmd_copy_buffer(vk_value_to_boost(raw), nonowning_buf(src), nonowning_buf(dst), regions)
}

def cmd_copy_range(raw : VkCommandBuffer; src : uint64; src_off : int64; dst : uint64; dst_off : int64; bytes : int64) {
    var regions : array<BufferCopy>
    regions |> push(BufferCopy(srcOffset = uint64(src_off), dstOffset = uint64(dst_off), size = uint64(bytes)))
    cmd_copy_buffer(vk_value_to_boost(raw), nonowning_buf(src), nonowning_buf(dst), regions)
}

// host-wait the transfer timeline up to `v` — the backstop before re-recording the shared xfer
// cmd, and the sync (cold-miss) completion wait
def xfer_host_wait(v : uint64) {
    var wi = VkSemaphoreWaitInfo()
    var sem = g_gpu.xfer_sem
    var val = v
    unsafe {
        wi.semaphoreCount = 1u
        wi.pSemaphores = addr(sem)
        wi.pValues = addr(val)
        vk_check(vkWaitSemaphores(dev_raw(), wi, 0xFFFFFFFFFFFFFFFFul), null)
    }
}



var g_vk_import_said = false   // one import proof-line per process
var g_vk_import_declined_said = false
