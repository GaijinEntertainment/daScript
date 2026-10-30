options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_cuda_stream shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_cuda_common public
require dasllama/dasllama_cuda_classes
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_gpu_stream_cache
require dasllama/dasllama_env
require dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gpu_kernels_common
require dasllama/dasllama_gpu_kernels_discrete
require dasllama/dasllama_math   // grow_resize
require dasllama/dasllama_boot_restore
require math

// The CUDA home's streamed prompt: the Vulkan tier's form (`ARCHITECTURE_GPU_VULKAN_STREAM.md`) over the home's streams and
// events - a stream slot on the device (a whole layer's planes, or the per-expert chain's ring of experts), a copy stream
// the pinned stacks DMA on, the chain's bundles on the device stream each waiting its own copy's event, the hot pool's DMA
// fill off the same stacks, and the expert cache the two homes share (`dasllama_gpu_stream_cache`).

let private CXF_WS_FLOOR = 64l             //! a scale plane's least bytes: a format with no scales still binds one

//! one stream slot: three plane pairs (gate, up, down) on the device, the group resident in it whole (-1 none, or a ring),
//! and the event its last whole-layer copy recorded
struct private CxfSlot {
    wq : uint64[3]
    ws : uint64[3]
    qcap : int64[3]
    scap : int64[3]
    group : int = -1
    ev : void?
}

//! a ring entry of the per-expert chain: its copy's and its chain's events, the push it carries, its three schedule planes
//! (records then the map) with their pinned staging, and its chunk rows' grid entries
struct private CxfEntry {
    ev_c : void?
    ev_x : void?
    ev_s : void?                //! the whole-layer chunk's staged copies landed: its pinned schedules and expert map may be rewritten
    pushed : int64 = -1l
    sched : uint64[3]
    sched_host : array<void?>   //! three pinned staging planes, the schedules' host side
    sched_words : int64
    bkt_dev : uint64
    bkt_host : void?
    rex_dev : uint64            //! the chunk rows' experts: the biased act's row map
    rex_host : void?
}

//! a layer's hot pool as the chain and the DMA fill read it: the three plane pairs (gate, up, down), an expert's blocks a plane
struct CxfPool {
    on : bool
    wq : uint64[3]
    ws : uint64[3]
    blk : int64[3]
    slots : int64
}

typedef CxfPoolPlanesFn = function<(l : int64) : CxfPool>

[unused_argument(l)]
def private cxf_no_pool(l : int64) : CxfPool => CxfPool()

var private g_cxf_pool_planes = @@cxf_no_pool

//! the resident driver's seat: layer `l`'s pool planes, for the pool window's copies and the DMA fill
def set_cxf_pool_planes_fn(f : CxfPoolPlanesFn) {
    g_cxf_pool_planes = f
}

typedef CxfBiasPlanesFn = function<() : tuple<gb : uint64; ub : uint64>>

def private cxf_no_bias_planes : tuple<gb : uint64; ub : uint64> => (gb = 0ul, ub = 0ul)

var private g_cxf_bias_planes = @@cxf_no_bias_planes

//! the resident driver's seat: the routed experts' gate and up bias planes (every routed layer's rows), 0 on a model with none
def set_cxf_bias_planes_fn(f : CxfBiasPlanesFn) {
    g_cxf_bias_planes = f
}

[init, boot_restore]
def private restore_cuda_stream_fn_defaults {}

//! an opened window of the per-expert chain (`XfMeetHost`) and its three planes' formats
struct private CxfMeet : XfMeetHost {
    f1 : int
    f3 : int
    f2 : int
}

struct private CudXf {
    ready : bool
    slots : array<CxfSlot>
    entries : array<CxfEntry>
    land_entry : array<int>     //! a ring position -> the entry of the last chain that read it (-1 none)
    land_push : array<int64>    //! ... and the push that entry carried then: a later push on the entry means that chain retired
    land_at : int64
    n : int64                   //! the chain planes' geometry
    nfe : int64
    cap : int64
    xf_dev, xg, hf, y1, y2, ydn, acc, w_dev, inv_dev : uint64
    x_bytes : int64             //! the x rows' and the accumulator's bytes
    wi_bytes : int64            //! the weight and map planes' bytes
    x_stage, w_stage, inv_stage, out_stage : void?
    ev_last : void?             //! the close's copy-out, the join's wait
    pending : bool
    per_expert : bool
    empty : bool
    pend_bytes : int64
    m : CxfMeet
    pool_copies : int const?
    pool_n : int64
    pool_g : int = -1
    pool_l : int64
    said_split : bool
    dma_evs : array<void?>      //! the DMA fill's flush events, a ring
    dma_at : int
    dma_open : bool
    dma_fills : int64
    windows : int64
    bundles : int64
    experts : int64
    biased : int64              //! the chain's acts that took the routed form's bias rows
}

let private CXF_DMA_RING = 4   //! DMA fill batches in flight: a layer's a batch

var private g_cxf = CudXf()
var private g_cxf_said : table<string>

//! StreamPinnedAllocFn: page-locked host memory the copy stream DMAs from; `soft` turns a refusal into a null record
def cud_pinned_alloc(bytes : int64; soft : bool) : PinnedBuf {
    cud_bind()
    let m = cuda_mem_alloc_host_mapped(uint64(bytes))
    if (m.host == null && !soft) {
        panic("dasLLAMA cuda stream: {bytes} bytes of pinned memory refused: {cuda_last_error()}")
    }
    return PinnedBuf(handle = 0ul, aux = m.dev, mapped = m.host, bytes = bytes)
}

//! StreamPinnedFreeFn
def cud_pinned_free(b : PinnedBuf) {
    cud_bind()
    cuda_mem_free_host(b.mapped)
}

//! the chain's streams, events and seats, once a device
def private cxf_init : bool {
    return true if (g_cxf.ready)
    return false if (!cud_init())
    set_stream_pinned_fns(STREAM_HOME_CUDA, @@cud_pinned_alloc, @@cud_pinned_free)
    g_cxf.ev_last = cuda_event_create_untimed()
    if (g_cud.copy.stream == null || g_cxf.ev_last == null) {
        panic("dasLLAMA cuda stream: the copy stream is not open ({cuda_last_error()})")
    }
    cud_on_shutdown(@@cud_stream_shutdown)
    g_cxf.ready = true
    return true
}

//! the device-layout bytes of a [rows x n] stack in format `fmt`
def private cxf_plane_bytes(n, rows : int64; fmt : int) : tuple<q : int64; s : int64> => dev_rows_bytes(kq_fmt_of_slot(fmt), n, rows)

//! MoeGpuUploadFn: a streamed stack into the expert cache's pinned memory; a resident stack is the whole-model driver's, never this
//! tier's (false, the walk keeps the layer on the host)
[cold_path]
def cud_moe_upload_stack(wq : uint8 const?; ws : uint8 const?; woff : int64; n, rows : int64; fmt : int; stream : bool) : bool {
    return false if (!stream || !cxf_init())
    if (!kq_rows_ok(n, fmt)) {
        to_log(LOG_WARNING, "dasLLAMA cuda stream: stack at {woff} declined - row length {n} is not a {kq_unit(fmt)}-block multiple (fmt {fmt})\n")
        return false
    }
    return true if (g_cud.dry)   // the mint's walk: the stack admitted, no pinned mirror made
    let pb = cxf_plane_bytes(n, rows, fmt)
    return stream_stack_make(wq, ws, woff, n, rows, fmt, pb.q, pb.s, false)
}

//! MoeGpuDropFn: the last `n` streamed stacks freed (a resident stack is none of this tier's)
def cud_moe_drop_stacks(n : int; stream : bool) {
    cud_bind()
    return if (!stream)
    stream_stacks_pop(n, true)
}

//! the stream slots the carve counts: one under the split (the chain's ring fills on demand), two with the whole-layer rail
//! (compute on one while the other fills)
def cud_stream_slot_count : int => gpu_want_moe_split() ? 1 : 2

//! the bytes the streamed slots take off the weight budget where the prompt streams: the driver's need a slot
def cud_stream_carve : int64 {
    return 0l if (gpu_want_moe_stream() == 0l || !gpu_want_pf_stream())
    let need = moe_gpu_stream_need()
    return need > 0l ? int64(cud_stream_slot_count()) * need : 0l
}

//! MoeGpuFfnXfOkFn: the chain serves a layer whose triple the cm2 tiles stamp (every expert format of the arena set), the
//! window inside the f32 x plane's cap
def cud_moe_ffn_xf_ok(npos, n : int64; f1, f3, f2 : int) : bool {
    return false if (!cud_init())
    for (f in [f1, f3, f2]) {
        return false if (!expert_chain_fmt_ok(kq_fmt_of_slot(f)))
    }
    return npos * n * 4l <= BATCH_Y_BYTES
}

//! MoeGpuFfnXfRowsFn
def cud_moe_ffn_xf_async_rows(n, nfe : int64) : int64 => xf_chunk_rows(n, nfe)

def private cxf_dev(bytes : int64) : uint64 {
    let b = cuda_mem_alloc(uint64(bytes))
    if (b == 0ul) {
        panic("dasLLAMA cuda stream: {bytes} bytes of device memory refused: {cuda_last_error()}")
    }
    cuda_memset_d8(b, 0u8, uint64(bytes), g_cud.stream)
    return b
}

def private cxf_host(bytes : int64) : void? {
    let m = cuda_mem_alloc_host_mapped(uint64(bytes))
    if (m.host == null) {
        panic("dasLLAMA cuda stream: {bytes} bytes of pinned staging refused: {cuda_last_error()}")
    }
    return m.host
}

//! the chain planes at the model's (n, nfe) geometry, remade where it grows; the x rows, the weights and the map at the window's
def private cxf_planes(n, nfe, npos, nk : int64) {
    if (n != g_cxf.n || nfe != g_cxf.nfe) {
        cxf_planes_drop()
        g_cxf.n = n
        g_cxf.nfe = nfe
        g_cxf.cap = xf_chunk_rows(n, nfe)
        let cap = g_cxf.cap
        g_cxf.xg = cxf_dev((cap + TILE_READ_SLACK) * n * 2l)
        g_cxf.hf = cxf_dev((cap + TILE_READ_SLACK) * nfe * 2l)
        g_cxf.y1 = cxf_dev(cap * nfe * 4l)
        g_cxf.y2 = cxf_dev(cap * nfe * 4l)
        g_cxf.ydn = cxf_dev(cap * n * 4l)
    }
    let xb = npos * n * 4l
    if (xb > g_cxf.x_bytes) {
        for (b in [g_cxf.xf_dev, g_cxf.acc]) {
            cuda_mem_free(b)
        }
        for (p in [g_cxf.x_stage, g_cxf.out_stage]) {
            cuda_mem_free_host(p)
        }
        g_cxf.x_bytes = xb
        g_cxf.xf_dev = cxf_dev(xb)
        g_cxf.acc = cxf_dev(xb)
        g_cxf.x_stage = cxf_host(xb)
        g_cxf.out_stage = cxf_host(xb)
    }
    let wb = nk * 4l
    if (wb > g_cxf.wi_bytes) {
        for (b in [g_cxf.w_dev, g_cxf.inv_dev]) {
            cuda_mem_free(b)
        }
        for (p in [g_cxf.w_stage, g_cxf.inv_stage]) {
            cuda_mem_free_host(p)
        }
        g_cxf.wi_bytes = wb
        g_cxf.w_dev = cxf_dev(wb)
        g_cxf.inv_dev = cxf_dev(wb)
        g_cxf.w_stage = cxf_host(wb)
        g_cxf.inv_stage = cxf_host(wb)
    }
}

def private cxf_planes_drop {
    for (b in [g_cxf.xg, g_cxf.hf, g_cxf.y1, g_cxf.y2, g_cxf.ydn, g_cxf.xf_dev, g_cxf.acc, g_cxf.w_dev, g_cxf.inv_dev]) {
        cuda_mem_free(b)
    }
    for (p in [g_cxf.x_stage, g_cxf.out_stage, g_cxf.w_stage, g_cxf.inv_stage]) {
        cuda_mem_free_host(p)
    }
    g_cxf.xg = 0ul
    g_cxf.hf = 0ul
    g_cxf.y1 = 0ul
    g_cxf.y2 = 0ul
    g_cxf.ydn = 0ul
    g_cxf.xf_dev = 0ul
    g_cxf.acc = 0ul
    g_cxf.w_dev = 0ul
    g_cxf.inv_dev = 0ul
    g_cxf.x_stage = null
    g_cxf.out_stage = null
    g_cxf.w_stage = null
    g_cxf.inv_stage = null
    g_cxf.n = 0l
    g_cxf.nfe = 0l
    g_cxf.x_bytes = 0l
    g_cxf.wi_bytes = 0l
}

//! the stream slots, made at the first streamed window (the caller holds it to that window): the carve's count, each the caps
//! the stacks ask
[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#stream-slot-ring")]
def private cxf_slots {
    let nslots = cud_stream_slot_count()
    let cap = stream_slot_caps()
    let slot_bytes = 2l * (cap.gu_wq + cap.gu_ws) + cap.dn_wq + cap.dn_ws
    let need = moe_gpu_stream_need()
    if (need > 0l && slot_bytes > need) {
        panic("dasLLAMA cuda stream: a stream slot ({slot_bytes} bytes) exceeds the carved stream reserve ({need})")
    }
    assert(XF_AHEAD * XF_BUNDLE <= MOE_GPU_LAND_EXPERTS,
        "dasLLAMA cuda stream: the ring holds fewer experts than the chain keeps in flight - a position would be reused under a chain still reading it")
    g_cxf.slots |> reserve(nslots)
    for (_i in range(nslots)) {
        var sl = CxfSlot()
        for (k in range(3)) {
            let q = k == 2 ? cap.dn_wq : cap.gu_wq
            let s = k == 2 ? cap.dn_ws : cap.gu_ws
            sl.wq[k] = cxf_dev(max(q, 64l))
            sl.ws[k] = cxf_dev(max(s, CXF_WS_FLOOR))
            sl.qcap[k] = q
            sl.scap[k] = s
        }
        sl.ev = cuda_event_create_untimed()
        g_cxf.slots |> emplace(sl)
    }
    let ring = max(moe_gpu_stream_ring_experts(), 1l)
    g_cxf.land_entry |> clear()
    g_cxf.land_entry |> resize(ring)
    g_cxf.land_push |> clear()
    g_cxf.land_push |> resize(ring)
    for (i in range64(ring)) {
        g_cxf.land_entry[i] = -1
    }
    g_cxf.land_at = 0l
    cuda_event_record(g_cxf.ev_last, g_cud.stream)   // the slots' clears ride the compute stream: the copy stream's first fills wait them out
    panic("dasLLAMA cuda stream: the copy stream's wait on the slots' clears did not queue ({cuda_last_error()})") if (!cuda_stream_wait_event(g_cud.copy.stream, g_cxf.ev_last))
    let form = moe_gpu_stream_ring() ? "a ring of {ring} experts" : "a whole layer"
    to_log(LOG_INFO, "dasLLAMA cuda stream: streamed prefill engaged ({length(g_stream_stacks) / 3} layers, slot {slot_bytes} bytes x {nslots}, {form})\n")
}

//! the ring entries, once a model: each its three events, and its chunk rows' grid entries and row experts with their pinned staging
[cold_path]
def private cxf_entries_make {
    let cap = g_cxf.cap
    g_cxf.entries |> reserve(XF_RING)
    for (_i in range(XF_RING)) {
        var e = CxfEntry()
        e.sched_host |> resize(3)
        e.ev_c = cuda_event_create_untimed()
        e.ev_x = cuda_event_create_untimed()
        e.ev_s = cuda_event_create_untimed()
        e.bkt_dev = cxf_dev(cap * 4l)
        e.bkt_host = cxf_host(cap * 4l)
        e.rex_dev = cxf_dev(cap * 4l)
        e.rex_host = cxf_host(cap * 4l)
        g_cxf.entries |> emplace(e)
    }
    to_log(LOG_INFO, "dasLLAMA cuda stream: per-expert streamed chain engaged (ring {XF_RING}, the copy stream)\n")
}

//! entry `e`'s three schedule planes remade at `words` words, its last chain waited out first
[cold_path]
def private cxf_entry_regrow(var e : CxfEntry; words : int64) {
    if (e.pushed >= 0l) {
        cuda_event_synchronize(e.ev_c)
    }
    for (k in range(3)) {
        cuda_mem_free(e.sched[k])
        cuda_mem_free_host(e.sched_host[k])
        e.sched[k] = cxf_dev(words * 4l)
        e.sched_host[k] = cxf_host(words * 4l)
    }
    e.sched_words = words
}

//! the ring entries hold a schedule of `nreg_max` regions over the chunk cap: made on the first window, a plane regrown where the
//! schedule outgrew it
def private cxf_entries(nreg_max : int64) {
    if (empty(g_cxf.entries)) {
        cxf_entries_make()
    }
    let wtiles = (max(g_cxf.n, g_cxf.nfe) + 127l) / 128l
    let words = 4l * nreg_max + ((g_cxf.cap + 31l) / 32l + nreg_max) * wtiles
    for (e in g_cxf.entries) {
        if (e.sched_words < words) {
            cxf_entry_regrow(e, words)
        }
    }
}

//! the cm2 tile column for `cnt` rows of a `d`-row plane in `fmt`: the s column alone where the stamps skip the others, else the card's pick
def private cxf_tc(fmt : int; d, cnt : int64) : int64 =>
    cud_cm2_s_only(fmt) ? 32l : cm2_wave_col(d, cnt, int64(g_cud.sm_count), 2l * int64(g_cud.sm_count))

def private cxf_tc_key(tc : int64) : int => tc == 256l ? CUD_CM2_TC_L : (tc == 128l ? CUD_CM2_TC_M : CUD_CM2_TC_S)

//! a plane's schedule over `nb` regions into entry `e`'s plane `k`: 4-word records [wblk, row0, cnt, wg0] then the per-workgroup
//! map; `wblk` the region's block base (`blks[i]`), `row0` its chunk-local row base, `cnt` its rows. Returns the workgroup count
//! and the column; the records and the map are staged in pinned memory and copied on the device stream
def private cxf_sched(var e : CxfEntry; k : int; fmt : int; d : int64; blks, row0s, cnts : array<int64>; nb : int64) : tuple<wgs : int64; tc : int64> {
    var rows = 0l
    for (c in cnts) {
        rows += c
    }
    let tc = cxf_tc(fmt, d, nb > 0l ? rows / nb : 1l)
    let wg = sched_regions(unsafe(reinterpret<uint?>(e.sched_host[k])), blks, row0s, cnts, tc, (d + 127l) / 128l)
    let map_off = nb * 4l
    if (map_off + wg > e.sched_words) {
        panic("dasLLAMA cuda stream: a schedule of {nb} regions and {wg} workgroups passes its plane ({e.sched_words} words)")
    }
    cuda_memcpy_htod(e.sched[k], e.sched_host[k], uint64((map_off + wg) * 4l), g_cud.stream)
    return (wgs = wg, tc = tc)
}

//! one plane's tiles over the schedule in entry `e`'s plane `k`: [wq, ws, schedule, x rows, y] at `n` x `d`
def private cxf_tiles(e : CxfEntry; k : int; fmt : int; wq, ws, xg, y : uint64; n, d : int64; sc : tuple<wgs : int64; tc : int64>) {
    return if (sc.wgs == 0l)
    var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(g_cxf_nb[k] * 4l), ksplit = 0u)
    cud_cm2_enc(fmt, cxf_tc_key(sc.tc), g_cud.stream, fixed_array(wq, ws, e.sched[k], xg, y), pc, sc.wgs)
}

var private g_cxf_nb : int64[3]   //! the last schedule's region count a plane: its map's word offset

//! the slot holding group `g`, or -1
def private cxf_slot_of(g : int) : int {
    for (i in range(length(g_cxf.slots))) {
        return i if (g_cxf.slots[i].group == g)
    }
    return -1
}

//! group `g`'s three whole stacks onto slot `si` on the copy stream; the slot's event marks the landing
def private cxf_copy_group(si, g : int) {
    verify(!moe_gpu_stream_ring(), "dasLLAMA cuda stream: the streamed slot is the chain's ring of experts - a whole layer's copy has no room in it")
    var sl & = unsafe(g_cxf.slots[si])
    if (sl.group >= 0) {
        panic("dasLLAMA cuda stream: the slot's wait on its last reader did not queue ({cuda_last_error()})") if (!cuda_stream_wait_event(g_cud.copy.stream, g_cxf.ev_last))   // the slot's last reader retired before its planes change
    }
    for (k in range(3)) {
        let ss & = unsafe(g_stream_stacks[g * 3 + k])
        if (ss.slots < ss.nexp) {
            panic("dasLLAMA cuda stream: the whole-layer stream arm copies every expert of streamed stack {g * 3 + k}, and the expert cache holds {ss.slots} of {ss.nexp} - a partial cache serves the split chain alone (DASLLAMA_GPU_MOE_SPLIT=1)")
        }
        assert(ss.wqbytes <= sl.qcap[k] && ss.wsbytes <= max(sl.scap[k], CXF_WS_FLOOR), "dasLLAMA cuda stream: a streamed stack passes its slot's capacity")
        cuda_memcpy_htod(sl.wq[k], ss.hwq.mapped, uint64(ss.wqbytes), g_cud.copy.stream)
        if (ss.wsbytes > 0l) {
            cuda_memcpy_htod(sl.ws[k], ss.hws.mapped, uint64(ss.wsbytes), g_cud.copy.stream)
        }
    }
    cuda_event_record(sl.ev, g_cud.copy.stream)
    sl.group = g
}

//! group `g` bound for a window: its slot filled where it is not, the device stream waiting the fill
def private cxf_group_bind(g : int; f3, f2 : int) : int {
    assert(g_stream_stacks[g * 3 + 1].fmt == f3 && g_stream_stacks[g * 3 + 2].fmt == f2, "dasLLAMA cuda stream: streamed group format mismatch")
    if (empty(g_cxf.slots)) {
        cxf_slots()
    }
    var si = cxf_slot_of(g)
    if (si < 0) {
        si = g % length(g_cxf.slots)
        cxf_copy_group(si, g)
    }
    panic("dasLLAMA cuda stream: the compute stream's wait on the slot's fill did not queue ({cuda_last_error()})") if (!cuda_stream_wait_event(g_cud.stream, g_cxf.slots[si].ev))
    return si
}

//! the next group runs descending while the prompt runs ascending: its copy on the other slot while this one computes
def private cxf_group_prefetch(g : int) {
    let nslots = length(g_cxf.slots)
    if (nslots > 1 && g > 0 && g_cxf.slots[(g - 1) % nslots].group != g - 1) {
        cxf_copy_group((g - 1) % nslots, g - 1)
    }
}

//! the stream group a gate offset belongs to; a region no streamed stack holds is a panic - this home keeps no resident per-op stack
def private cxf_group_of(offs1 : int64 const?; f1 : int) : int {
    let w0 = unsafe(offs1[0])
    let g = stream_group_at(w0, f1)
    if (g < 0) {
        panic("dasLLAMA cuda stream: dispatch against a region no streamed stack holds (fmt {f1} element offset {w0})")
    }
    return g
}

//! the window staged: the x rows, the weights and the map in pinned memory, the chain planes sized
def private cxf_prepare(xp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64) {
    assert(npos > 0l && nrows % npos == 0l, "dasLLAMA cuda stream: the chain serves the combined form only")
    assert(nrows * 4l <= COMB_WI_BYTES && npos * n * 4l <= BATCH_Y_BYTES, "dasLLAMA cuda stream: combined output exceeds capacity")
    cxf_planes(n, nfe, npos, nrows)
    unsafe {
        par_memcpy(g_cxf.x_stage, reinterpret<void?>(xp), npos * n * 4l)
        par_memcpy(g_cxf.w_stage, reinterpret<void?>(wp), nrows * 4l)
        par_memcpy(g_cxf.inv_stage, reinterpret<void?>(invp), nrows * 4l)
    }
}

//! the window's rows, weights and map onto the device, the accumulator zeroed - the first launch of a window
def private cxf_land_window(nk, npos, n : int64) {
    var s = g_cud.stream
    cuda_memcpy_htod(g_cxf.xf_dev, g_cxf.x_stage, uint64(npos * n * 4l), s)
    cuda_memcpy_htod(g_cxf.w_dev, g_cxf.w_stage, uint64(nk * 4l), s)
    cuda_memcpy_htod(g_cxf.inv_dev, g_cxf.inv_stage, uint64(nk * 4l), s)
    cuda_memset_d8(g_cxf.acc, 0u8, uint64(npos * n * 4l), s)
}

var private @scratch g_cxf_rb1 : array<int64>   //! a schedule's region lists: block bases a plane, the kept regions' indices, chunk-local row bases, counts
var private @scratch g_cxf_rb3 : array<int64>
var private @scratch g_cxf_rb2 : array<int64>
var private @scratch g_cxf_reg : array<int64>
var private @scratch g_cxf_row0 : array<int64>
var private @scratch g_cxf_cnt : array<int64>

//! the block lists emptied, room for `n` regions (`sched_clip` empties the other three)
def private cxf_lists_begin(n : int64) {
    g_cxf_rb1 |> clear()
    g_cxf_rb3 |> clear()
    g_cxf_rb2 |> clear()
    g_cxf_row0 |> clear()
    g_cxf_cnt |> clear()
    g_cxf_rb1 |> reserve(n)
    g_cxf_rb3 |> reserve(n)
    g_cxf_rb2 |> reserve(n)
    g_cxf_row0 |> reserve(n)
    g_cxf_cnt |> reserve(n)
}

//! one chunk [r0, r1) of the window form over the slot: the window gather, the three schedules over the regions clipped to the
//! chunk, the tiles, the act, the combine
def private cxf_chunk(si : int; g : int; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; n, nfe, nk, npos, r0, r1, ci : int64; f1, f3, f2 : int; is_gelu : bool) {
    var s = g_cud.stream
    let sl & = unsafe(g_cxf.slots[si])
    let nent = long_length(g_cxf.entries)
    var e & = unsafe(g_cxf.entries[ci % nent])   // a chunk its own entry: its pinned staging stays until the copies off it landed
    if (ci >= nent) {
        panic("dasLLAMA cuda stream: the chunk's staged copies did not land ({cuda_last_error()})") if (!cuda_event_synchronize(e.ev_s))
    }
    cxf_lists_begin(nregions)
    sched_clip(offs1, nregions, r0, r1, g_cxf_reg, g_cxf_row0, g_cxf_cnt)
    let ege = n * nfe   // nolint:LINT021 — the expert index is plane-offset (64-bit) math, narrowed once to the device word
    unsafe {
        var rx = reinterpret<uint?>(e.rex_host)
        for (r, b0, cnt in g_cxf_reg, g_cxf_row0, g_cxf_cnt) {
            let ex = uint((offs1[r * 3l] - g_stream_stacks[g * 3].base) / ege)
            for (b in range64(b0, b0 + cnt)) {
                rx[b] = ex
            }
            g_cxf_rb1 |> push((offs1[r * 3l] - g_stream_stacks[g * 3].base) / kq_unit(f1))
            g_cxf_rb3 |> push((offs3[r * 3l] - g_stream_stacks[g * 3 + 1].base) / kq_unit(f3))
            g_cxf_rb2 |> push((offs2[r * 3l] - g_stream_stacks[g * 3 + 2].base) / kq_unit(f2))
        }
    }
    let nb = long_length(g_cxf_cnt)
    let rows = r1 - r0
    let k = uint(nk / npos)
    var pcg = GatherArgs(n = uint(n), k = k, r0 = uint(r0), r1 = uint(r1))
    enc_moe_gather_f16_cls(s, fixed_array(g_cxf.xf_dev, g_cxf.inv_dev, g_cxf.xg), pcg, nk)
    g_cxf_nb[0] = nb
    g_cxf_nb[1] = nb
    g_cxf_nb[2] = nb
    let sc1 = cxf_sched(e, 0, f1, nfe, g_cxf_rb1, g_cxf_row0, g_cxf_cnt, nb)
    let sc3 = cxf_sched(e, 1, f3, nfe, g_cxf_rb3, g_cxf_row0, g_cxf_cnt, nb)
    let sc2 = cxf_sched(e, 2, f2, n, g_cxf_rb2, g_cxf_row0, g_cxf_cnt, nb)
    cxf_tiles(e, 0, f1, sl.wq[0], sl.ws[0], g_cxf.xg, g_cxf.y1, n, nfe, sc1)
    cxf_tiles(e, 1, f3, sl.wq[1], sl.ws[1], g_cxf.xg, g_cxf.y2, n, nfe, sc3)
    cxf_act(e, rows, nfe, is_gelu)
    cxf_tiles(e, 2, f2, sl.wq[2], sl.ws[2], g_cxf.hf, g_cxf.ydn, nfe, n, sc2)
    var pcc = GkMoeCombArgs(n = uint(n), k = k, r0 = uint(r0), r1 = uint(r1))
    enc_combine_cls(s, fixed_array(g_cxf.acc, g_cxf.inv_dev, g_cxf.w_dev, g_cxf.ydn), pcc, npos)
    cuda_event_record(e.ev_s, s)
}

//! the accumulator out: the copy queued on the device stream, the join's event after it
def private cxf_close_out(npos, n : int64) {
    cuda_memcpy_dtoh(g_cxf.out_stage, g_cxf.acc, uint64(npos * n * 4l), g_cud.stream)
    cuda_event_record(g_cxf.ev_last, g_cud.stream)
}

//! MoeGpuFfnXfFn: the whole-layer rail - the group bound to a slot (its copy, or the prefetch's), the window chain over it chunk
//! by chunk, each waited, the next group's copy queued behind
[arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-split")]
def cud_moe_ffn_xf(var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    cud_bind()
    assert(!g_cxf.pending, "dasLLAMA cuda stream: a chain opened while one is in flight")
    let t0 = ref_time_ticks()
    let g = cxf_group_of(offs1, f1)
    let cold = cxf_slot_of(g) < 0
    let si = cxf_group_bind(g, f3, f2)
    cxf_prepare(xp, wp, invp, n, nfe, nrows, npos)
    cxf_entries(max(nregions, 1l))
    cxf_land_window(nrows, npos, n)
    let chunk = g_cxf.cap
    var r0 = 0l
    var ci = 0l
    while (r0 < nrows) {
        let r1 = min(r0 + chunk, nrows)
        cxf_chunk(si, g, offs1, offs3, offs2, nregions, n, nfe, nrows, npos, r0, r1, ci, f1, f3, f2, is_gelu)
        r0 = r1
        ci++
    }
    cxf_close_out(npos, n)
    g_cxf.pending = true
    g_cxf.per_expert = false
    g_cxf.empty = false
    g_cxf.pend_bytes = npos * n * 4l
    cxf_group_prefetch(g)
    cud_moe_ffn_xf_join(goutp)
    g_cxf.windows++
    if (g_env_gpu.gpu_prof) {
        to_log(LOG_INFO, "cud_stream g{g} {cold ? "cold" : "warm"} {get_time_usec(t0)} us, {nregions} regions {nrows} rows\n")   // nolint:PERF026,PERF028,LINT019 — gpu_prof-gated report leg
    }
}

//! MoeGpuFfnXfBeginFn: the ring form takes the per-expert chain whole (open, every bundle, close); the whole-layer rail queues
//! one chunk over the slot and returns, the join collecting
def cud_moe_ffn_xf_begin(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    cud_bind()
    assert(!g_cxf.pending, "dasLLAMA cuda stream: a chain begun while one is in flight")
    if (moe_gpu_stream_ring() || cxf_pool_on()) {
        verify(cud_moe_ffn_xf_open(offs1, offs3, offs2, nregions, xp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu), "cud expert chain: the ring form opens over a streamed group")
        var r = 0l
        while (r < nregions) {
            let r_hi = cud_moe_ffn_xf_bundle_end(r, nregions)
            verify(cud_moe_ffn_xf_push(r, r_hi, true), "cud expert chain: a waited push always queues")
            r = r_hi
        }
        cud_moe_ffn_xf_close()
        return
    }
    let cap = xf_chunk_rows(n, nfe)
    assert(nrows <= cap, "dasLLAMA cuda stream: the async chain serves one chunk")
    let g = cxf_group_of(offs1, f1)
    let si = cxf_group_bind(g, f3, f2)
    cxf_prepare(xp, wp, invp, n, nfe, nrows, npos)
    cxf_entries(max(nregions, 1l))
    cxf_land_window(nrows, npos, n)
    cxf_chunk(si, g, offs1, offs3, offs2, nregions, n, nfe, nrows, npos, 0l, nrows, 0l, f1, f3, f2, is_gelu)
    cxf_close_out(npos, n)
    g_cxf.pending = true
    g_cxf.per_expert = false
    g_cxf.empty = false
    g_cxf.pend_bytes = npos * n * 4l
    cxf_group_prefetch(g)
}

//! MoeGpuFfnXfJoinFn: the pending chain's copy-out waited and its combined rows handed out
def cud_moe_ffn_xf_join(var goutp : float?) {
    cud_bind()
    assert(g_cxf.pending, "dasLLAMA cuda stream: a join with nothing in flight")
    if (g_cxf.empty) {   // the window pushed no expert: its share of the rows is zero
        for (i in range64(g_cxf.pend_bytes / 4l)) {
            unsafe(goutp[i]) = 0.0
        }
    } else {
        if (!cuda_event_synchronize(g_cxf.ev_last)) {
            panic("dasLLAMA cuda stream: the chain did not complete: {cuda_last_error()}")
        }
        par_memcpy(unsafe(reinterpret<void?>(goutp)), g_cxf.out_stage, g_cxf.pend_bytes)
    }
    g_cxf.pending = false
    g_cxf.per_expert = false
}

def private cxf_pool_on : bool => g_cxf.pool_copies != null

//! MoeGpuXfPoolWindowFn: the next window's first `npool` regions on layer `l`'s pool, copies sourced off the group holding `woff1`
def cud_xf_pool_window(copies : int const?; npool, l, woff1 : int64; fmt1 : int) {
    cud_bind()
    g_cxf.pool_copies = copies
    g_cxf.pool_n = copies == null ? 0l : npool
    g_cxf.pool_g = copies == null ? -1 : stream_group_of(woff1, fmt1)
    g_cxf.pool_l = l
    if (copies != null && g_cxf.pool_g < 0) {
        panic("dasLLAMA cuda stream: the pool window names a layer at {woff1} (fmt {fmt1}) with no streamed stack to copy from")
    }
}

//! MoeGpuFfnXfOpenFn: the window over group `g` (the pool window's, or the first region's) prepared: the slots, the chain planes,
//! the x rows staged; nothing queued until a push
[arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-split")]
def cud_moe_ffn_xf_open(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) : bool {
    cud_bind()
    return false if (!cxf_init())
    let g = cxf_pool_on() ? g_cxf.pool_g : stream_group_at(unsafe(offs1[0]), f1)
    return false if (g < 0)
    assert(!g_cxf.pending && !g_cxf.m.on, "dasLLAMA cuda stream: a chain opened while one is in flight")
    assert(nregions > 0l, "dasLLAMA cuda stream: the per-expert chain opens over no region")
    if (!g_cxf.said_split) {
        g_cxf.said_split = true
        to_log(LOG_INFO, "dasLLAMA cuda stream: streamed prefill split engaged (DASLLAMA_GPU_MOE_SPLIT=0 streams whole layers)\n")
    }
    assert(g_stream_stacks[g * 3 + 1].fmt == f3 && g_stream_stacks[g * 3 + 2].fmt == f2, "dasLLAMA cuda stream: streamed group format mismatch")
    if (empty(g_cxf.slots)) {
        cxf_slots()
    }
    let t0 = ref_time_ticks()
    let npool = cxf_pool_on() ? min(g_cxf.pool_n, nregions) : 0l
    verify(!moe_gpu_stream_ring() || cxf_pool_on(), "dasLLAMA cuda stream: the ring form opens a window with no copy list - every stream region names its expert through it")
    if (npool < nregions) {
        g_cxf.slots[0].group = -1   // a partial fill: a later window never mistakes the slot for the whole group
    }
    cxf_prepare(xp, wp, invp, n, nfe, nrows, npos)
    cxf_entries(XF_BUNDLE)
    g_cxf_pos |> grow_resize(nregions)
    g_cxf_bkt |> grow_resize(nrows)
    for (gi in range64(nrows)) {
        g_cxf_bkt[int64(unsafe(invp[gi]))] = uint(gi)
    }
    let ege = nfe * n
    var exp_bytes = 0l
    for (k in range(3)) {
        let ss & = unsafe(g_stream_stacks[g * 3 + k])
        exp_bytes += (ss.wqbytes + ss.wsbytes) / (ss.elems / ege)
    }
    g_cxf.m = CxfMeet(on = true, g = g, npool = npool, offs1 = offs1, offs3 = offs3, offs2 = offs2, nregions = nregions, ege = ege, cap = g_cxf.cap,
        n = n, nfe = nfe, nk = nrows, npos = npos, is_gelu = is_gelu, f1 = f1, f3 = f3, f2 = f2, pushed = 0l, done = 0l, experts = 0l, t0 = t0, exp_bytes = exp_bytes)
    return true
}

var private @scratch @exact_size g_cxf_bkt : array<uint>    //! the opened window's bucket row -> grid entry (the slot map's inverse)
var private @scratch @exact_size g_cxf_pos : array<int64>   //! the ring form: the opened window's region -> its ring position

//! MoeGpuFfnXfBundleFn: the end of the bundle starting at region `r_lo` of the opened window
def cud_moe_ffn_xf_bundle_end(r_lo, r_max : int64) : int64 => xf_bundle_end(g_cxf.m, r_lo, r_max)

//! the ring form: regions [r_lo, r_hi) past the pool prefix take the next ring positions, the copy stream waiting the chain that
//! last read each position where that chain may still run
[arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#stream-slot-ring")]
def private cxf_assign_land(r_lo, r_hi : int64) {
    let m & = unsafe(g_cxf.m)
    let ring = moe_gpu_stream_ring_experts()
    for (r in range64(max(r_lo, m.npool), r_hi)) {
        let pos = g_cxf.land_at % ring
        g_cxf.land_at++
        g_cxf_pos[r] = pos
        let je = g_cxf.land_entry[pos]
        if (je >= 0 && g_cxf.entries[je].pushed == g_cxf.land_push[pos]) {
            panic("dasLLAMA cuda stream: the copy stream's wait on the ring entry's chain did not queue ({cuda_last_error()})") if (!cuda_stream_wait_event(g_cud.copy.stream, g_cxf.entries[je].ev_c))
        }
    }
}

//! the ring form: the bundle's positions read by the chain on entry `j` at push `pushed`
def private cxf_land_done(r_lo, r_hi : int64; j : int; pushed : int64) {
    let m & = unsafe(g_cxf.m)
    for (r in range64(max(r_lo, m.npool), r_hi)) {
        g_cxf.land_entry[g_cxf_pos[r]] = j
        g_cxf.land_push[g_cxf_pos[r]] = pushed
    }
}

//! expert `e` of group `g`'s three planes, the cache slot's bytes onto slot `dslot` of the planes `wq` / `ws` whose expert
//! stride is `blk[k]` blocks; an expert the cache does not hold is a panic naming `who`
def private cxf_expert_copy(g : int; e : int64; wq, ws : uint64[3]; blk : int64[3]; dslot : int64; who : string) {
    for (k in range(3)) {
        let sk = g * 3 + k
        let src = stream_slot(sk, e)
        if (!src.present) {
            panic("dasLLAMA cuda stream: {who} claimed expert {e} of streamed stack {sk}, which the expert cache does not hold - the window's fetch missed it")
        }
        let ss & = unsafe(g_stream_stacks[sk])
        let b = dev_block_bytes(kq_fmt_of_slot(ss.fmt))
        assert(ss.qlen == blk[k] * b.qsb, "dasLLAMA cuda stream: a destination slot and a streamed expert differ in bytes")
        unsafe {
            cuda_memcpy_htod(wq[k] + uint64(dslot * ss.qlen), reinterpret<void?>(src.q), uint64(ss.qlen), g_cud.copy.stream)
            if (ss.slen > 0l) {
                cuda_memcpy_htod(ws[k] + uint64(dslot * ss.slen), reinterpret<void?>(src.s), uint64(ss.slen), g_cud.copy.stream)
            }
        }
    }
}

//! the ring slot's expert stride a plane, in blocks
def private cxf_slot_blocks(g : int) : int64[3] {
    var blk : int64[3]
    for (k in range(3)) {
        let ss & = unsafe(g_stream_stacks[g * 3 + k])
        blk[k] = ss.qlen / dev_block_bytes(kq_fmt_of_slot(ss.fmt)).qsb
    }
    return blk
}

//! a bundle region's copy: into its pool slot for a pool region (none for a hit), onto the stream slot otherwise - at the region's
//! ring position under the ring form, at the expert's own index in a whole-layer slot
def private cxf_push_copy(r, base : int64; pool : CxfPool) {
    let m & = unsafe(g_cxf.m)
    if (r < m.npool) {
        let e = int64(unsafe(g_cxf.pool_copies[r]))
        if (e >= 0l) {
            let slot = (unsafe(m.offs1[r * 3l]) - g_cxf.pool_l * 3l) / m.ege
            cxf_expert_copy(m.g, e, pool.wq, pool.ws, pool.blk, slot, "the pool window")
        }
        return
    }
    let e = (unsafe(m.offs1[r * 3l]) - base) / m.ege   // the expert off the live offset table: the meet rotates the regions after the window opened
    let sl & = unsafe(g_cxf.slots[0])
    cxf_expert_copy(m.g, e, sl.wq, sl.ws, cxf_slot_blocks(m.g), moe_gpu_stream_ring() ? g_cxf_pos[r] : e, "the expert chain")
}

//! the bundle's three region lists at chunk-local row bases (region after region) and its chunk rows' grid entries into entry `j`
def private cxf_bundle_lists(r_lo, r_hi : int64; var e : CxfEntry; pool : CxfPool; base_blk : int64[3]) : int64 {
    let m & = unsafe(g_cxf.m)
    let nb = r_hi - r_lo
    cxf_lists_begin(nb)
    var rows = 0l
    let ring = moe_gpu_stream_ring()
    let pooled = r_lo < m.npool
    let base = g_stream_stacks[m.g * 3].base
    unsafe {
        var gl = reinterpret<uint?>(e.bkt_host)
        var rx = reinterpret<uint?>(e.rex_host)
        for (i in range64(nb)) {
            let r = r_lo + i
            let b0 = m.offs1[r * 3l + 1l]
            let cnt = m.offs1[r * 3l + 2l]
            let ex = uint(xf_region_expert(m, r, base))
            if (pooled) {
                let slot = (m.offs1[r * 3l] - g_cxf.pool_l * 3l) / m.ege
                g_cxf_rb1 |> push(slot * pool.blk[0])
                g_cxf_rb3 |> push(slot * pool.blk[1])
                g_cxf_rb2 |> push(slot * pool.blk[2])
            } elif (ring) {
                g_cxf_rb1 |> push(g_cxf_pos[r] * base_blk[0])
                g_cxf_rb3 |> push(g_cxf_pos[r] * base_blk[1])
                g_cxf_rb2 |> push(g_cxf_pos[r] * base_blk[2])
            } else {
                g_cxf_rb1 |> push((m.offs1[r * 3l] - g_stream_stacks[m.g * 3].base) / kq_unit(m.f1))
                g_cxf_rb3 |> push((m.offs3[r * 3l] - g_stream_stacks[m.g * 3 + 1].base) / kq_unit(m.f3))
                g_cxf_rb2 |> push((m.offs2[r * 3l] - g_stream_stacks[m.g * 3 + 2].base) / kq_unit(m.f2))
            }
            g_cxf_row0 |> push(rows)
            g_cxf_cnt |> push(cnt)
            for (b in range64(cnt)) {
                gl[rows + b] = g_cxf_bkt[b0 + b]
                rx[rows + b] = ex
            }
            rows += cnt
        }
    }
    return rows
}

//! the chain's act over `rows` chunk rows into the f16 hidden plane under the routed form (`ffn_form_act`), and under a bias the
//! rows' experts (entry `e`'s row map) naming their gate and up bias rows off the resident driver's planes
def private cxf_act(e : CxfEntry; rows, nfe : int64; is_gelu : bool) {
    var s = g_cud.stream
    var pca = ffn_form_act(rows * nfe, nfe, is_gelu)
    pca.nblk = uint(rows * nfe / Q8_BLOCK_ELEMS)
    let wgs = (rows * nfe / 4l + 255l) / 256l
    if (pca.bias_on == 0u) {
        enc_actf16_cls(s, fixed_array(g_cxf.y1, g_cxf.y2, g_cxf.hf), pca, wgs)
        return
    }
    let bp = invoke(g_cxf_bias_planes)
    if (bp.gb == 0ul) {
        panic("dasLLAMA cuda stream: the routed form asks a biased act, and the resident driver holds no bias planes")
    }
    cuda_memcpy_htod(e.rex_dev, e.rex_host, uint64(rows * 4l), s)
    enc_actf16_b_cls(s, fixed_array(g_cxf.y1, g_cxf.y2, g_cxf.hf, bp.gb, bp.ub, e.rex_dev), pca, wgs)
    g_cxf.biased++
}

//! one bundle's chain on entry `j` over its `rows` chunk rows: the copy's event waited, the first bundle landing the window,
//! the grid list, the by-row gather, gate and up, the act, down, the by-row combine a region at a time, the chain's event
[arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-split")]
def private cxf_record_bundle(var e : CxfEntry; rows : int64; nb : int64; first : bool; wq, ws : uint64[3]) {
    let m & = unsafe(g_cxf.m)
    var s = g_cud.stream
    panic("dasLLAMA cuda stream: the compute stream's wait on the entry's copies did not queue ({cuda_last_error()})") if (!cuda_stream_wait_event(s, e.ev_x))
    if (first) {
        cxf_land_window(m.nk, m.npos, m.n)
    }
    cuda_memcpy_htod(e.bkt_dev, e.bkt_host, uint64(rows * 4l), s)
    let k = uint(m.nk / m.npos)
    var pcg = GatherArgs(n = uint(m.n), k = k, r0 = 0u, r1 = uint(rows))
    enc_moe_gather_f16_row_cls(s, fixed_array(g_cxf.xf_dev, e.bkt_dev, g_cxf.xg), pcg, rows)
    g_cxf_nb[0] = nb
    g_cxf_nb[1] = nb
    g_cxf_nb[2] = nb
    let sc1 = cxf_sched(e, 0, m.f1, m.nfe, g_cxf_rb1, g_cxf_row0, g_cxf_cnt, nb)
    let sc3 = cxf_sched(e, 1, m.f3, m.nfe, g_cxf_rb3, g_cxf_row0, g_cxf_cnt, nb)
    let sc2 = cxf_sched(e, 2, m.f2, m.n, g_cxf_rb2, g_cxf_row0, g_cxf_cnt, nb)
    cxf_tiles(e, 0, m.f1, wq[0], ws[0], g_cxf.xg, g_cxf.y1, m.n, m.nfe, sc1)
    cxf_tiles(e, 1, m.f3, wq[1], ws[1], g_cxf.xg, g_cxf.y2, m.n, m.nfe, sc3)
    cxf_act(e, rows, m.nfe, m.is_gelu)
    cxf_tiles(e, 2, m.f2, wq[2], ws[2], g_cxf.hf, g_cxf.ydn, m.nfe, m.n, sc2)
    // the combine a region at a time: a bundle's experts share positions, and the by-row add takes no two rows of one dispatch on one position
    for (i in range64(nb)) {
        let rb = uint(g_cxf_row0[i])
        let cnt = g_cxf_cnt[i]
        var pcc = GkMoeCombArgs(n = uint(m.n), k = k, r0 = rb, r1 = rb + uint(cnt), goff = rb)
        enc_combine_row_cls(s, fixed_array(g_cxf.acc, g_cxf.w_dev, g_cxf.ydn, e.bkt_dev), pcc, cnt)
    }
    cuda_event_record(e.ev_c, s)
}

//! MoeGpuFfnXfPushFn: regions [r_lo, r_hi) as one bundle on ring entry `pushed % XF_RING`; without `wait` an entry still in
//! flight, or XF_AHEAD bundles already unfinished, answers false and nothing is queued
[arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-split")]
def cud_moe_ffn_xf_push(r_lo, r_hi : int64; wait : bool) : bool {
    cud_bind()
    assert(g_cxf.m.on, "dasLLAMA cuda stream: expert chain push with no window open")
    var m & = unsafe(g_cxf.m)
    let j = int(m.pushed % int64(XF_RING))
    var e & = unsafe(g_cxf.entries[j])
    if (!wait) {
        while (m.done < m.pushed && cuda_event_ready(g_cxf.entries[int(m.done % int64(XF_RING))].ev_c)) {
            m.done++
        }
        return false if (m.pushed - m.done >= XF_AHEAD || (e.pushed >= 0l && (!cuda_event_ready(e.ev_c) || !cuda_event_ready(e.ev_x))))
    }
    let bundle_rows = xf_bundle_rows(m, r_lo, r_hi)
    assert(bundle_rows >= 0l, "dasLLAMA cuda stream: the expert chain's bundle passes the chunk cap or XF_BUNDLE")
    if (e.pushed >= 0l) {   // this entry's earlier chain and copy retired: its events, its schedules and its grid list are free
        cuda_event_synchronize(e.ev_c)
        cuda_event_synchronize(e.ev_x)
    }
    let base = g_stream_stacks[m.g * 3].base
    let ring = moe_gpu_stream_ring()
    if (ring) {
        cxf_assign_land(r_lo, r_hi)
    }
    let pooled = r_lo < m.npool
    let pool = pooled ? invoke(g_cxf_pool_planes, g_cxf.pool_l) : CxfPool()
    if (pooled && !pool.on) {
        panic("dasLLAMA cuda stream: the pool window names layer {g_cxf.pool_l}, which has no pool on this home")
    }
    for (r in range64(r_lo, r_hi)) {
        cxf_push_copy(r, base, pool)
    }
    cuda_event_record(e.ev_x, g_cud.copy.stream)
    let rows = cxf_bundle_lists(r_lo, r_hi, e, pool, cxf_slot_blocks(m.g))
    let nb = r_hi - r_lo
    let sl & = unsafe(g_cxf.slots[0])
    cxf_record_bundle(e, rows, nb, m.pushed == 0l, pooled ? pool.wq : sl.wq, pooled ? pool.ws : sl.ws)
    e.pushed = m.pushed
    if (ring) {
        cxf_land_done(r_lo, r_hi, j, m.pushed)
    }
    m.pushed++
    m.experts += nb
    g_cxf.bundles++
    g_cxf.experts += nb
    return true
}

//! MoeGpuFfnXfCloseFn: the accumulator's copy-out queued behind the pushed chains; a window that pushed nothing leaves the join to answer zero rows
[arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-split")]
def cud_moe_ffn_xf_close {
    cud_bind()
    assert(g_cxf.m.on, "dasLLAMA cuda stream: per-expert chain close with no window open")
    var m & = unsafe(g_cxf.m)
    if (m.pushed > 0l) {
        cxf_close_out(m.npos, m.n)
    }
    g_cxf.pending = true
    g_cxf.per_expert = true
    g_cxf.empty = m.pushed == 0l
    g_cxf.pend_bytes = m.npos * m.n * 4l
    g_cxf.windows++
    if (g_env_gpu.gpu_prof) {
        to_log(LOG_INFO, "cud_xf_experts g{m.g} {m.experts} of {m.nregions} experts in {m.pushed} bundles, {m.nk} rows, {m.exp_bytes * m.experts / 1000000l} MB queued in {get_time_usec(m.t0)} us\n")   // nolint:PERF026,PERF028,LINT019 — gpu_prof-gated report leg
    }
    m.on = false
}

//! RdecHotDmaFn: expert `e`'s planes off its streamed stack into slot `slot` of layer `l`'s pool on the copy stream; false where
//! the stack is not streamed or the cache does not hold the expert - the host's fill then takes it
[arch(at="../ARCHITECTURE_GPU_VULKAN_POOL.md#pool-dma-fill")]
def cud_rdec_hot_dma(l, slot, e, woff1 : int64; f1 : int) : bool {
    cud_bind()
    return false if (!cxf_init())
    let g = stream_group_of(woff1, f1)
    if (g < 0) {
        gpu_said_once(g_cxf_said, LOG_WARNING, "cud_rdec_hot_dma: ", "a routed layer's expert stack (fmt {f1}) is not a streamed stack - the host fill takes its misses")
        return false
    }
    let pool = invoke(g_cxf_pool_planes, l)
    assert(pool.on && slot >= 0l && slot < pool.slots, "cud_rdec_hot_dma: a slot outside the layer's pool")
    for (k in range(3)) {
        let src = stream_slot(g * 3 + k, e)
        if (!src.present) {
            gpu_said_once(g_cxf_said, LOG_WARNING, "cud_rdec_hot_dma: ", "a pool miss outside the streamed stack's resident experts - the host fill takes every such miss (a partial expert cache, DASLLAMA_EXPERT_CACHE_MB, leaves the non-resident experts to it)")
            return false
        }
        let ss & = unsafe(g_stream_stacks[g * 3 + k])
        let b = dev_block_bytes(kq_fmt_of_slot(ss.fmt))
        if (ss.qlen != pool.blk[k] * b.qsb) {   // the host's gather decodes the expert into the slot's form; a copy cannot
            gpu_said_once(g_cxf_said, LOG_WARNING, "cud_rdec_hot_dma: ", "layer {l}'s pool slot ({pool.blk[k] * b.qsb} quant bytes) and its streamed expert ({ss.qlen} bytes in stack {g * 3 + k}, format {ss.fmt}) differ - the host fill takes it")
            return false
        }
    }
    if (!g_cxf.dma_open) {
        if (empty(g_cxf.dma_evs)) {
            g_cxf.dma_evs |> reserve(CXF_DMA_RING)
            for (_i in range(CXF_DMA_RING)) {
                g_cxf.dma_evs |> push(cuda_event_create_untimed())
            }
        }
        g_cxf.dma_open = true
    }
    cxf_expert_copy(g, e, pool.wq, pool.ws, pool.blk, slot, "the DMA fill")
    g_cxf.dma_fills++
    return true
}

//! RdecHotDmaFlushFn: layer `l`'s open DMA batch fenced - the device stream's later work waits its landing
[unused_argument(l)]
def cud_rdec_hot_dma_flush(l : int64) {
    cud_bind()
    return if (!g_cxf.dma_open)
    let ev = g_cxf.dma_evs[g_cxf.dma_at]
    cuda_event_record(ev, g_cud.copy.stream)
    panic("dasLLAMA cuda stream: the compute stream's wait on the DMA fill did not queue ({cuda_last_error()})") if (!cuda_stream_wait_event(g_cud.stream, ev))
    g_cxf.dma_at = (g_cxf.dma_at + 1) % CXF_DMA_RING
    g_cxf.dma_open = false
}

//! the DMA fill's expert copies since the device came up (a cell's witness)
def cud_stream_dma_fills : int64 => g_cxf.dma_fills

//! the chain's windows, bundles, experts and biased acts since the device came up (a cell's witness)
def cud_stream_stats : tuple<windows : int64; bundles : int64; experts : int64; biased : int64> =>
    (windows = g_cxf.windows, bundles = g_cxf.bundles, experts = g_cxf.experts, biased = g_cxf.biased)

//! the model's stream state dropped: the slots, the ring entries, the chain planes and the expert cache's pinned memory; the
//! copy stream and the seats stay for the next model
def cud_stream_drop {
    cud_bind()
    return if (!g_cxf.ready)
    assert(!g_cxf.pending && !g_cxf.m.on, "dasLLAMA cuda stream: model drop with a chain in flight")
    cuda_stream_synchronize(g_cud.copy.stream)
    cud_sync()
    for (sl in g_cxf.slots) {
        for (k in range(3)) {
            cuda_mem_free(sl.wq[k])
            cuda_mem_free(sl.ws[k])
        }
        cuda_event_destroy(sl.ev)
    }
    g_cxf.slots |> clear()
    g_cxf_said |> clear()   // the next model's declines log afresh
    for (e in g_cxf.entries) {
        cuda_event_destroy(e.ev_c)
        cuda_event_destroy(e.ev_x)
        cuda_event_destroy(e.ev_s)
        for (k in range(3)) {
            cuda_mem_free(e.sched[k])
            cuda_mem_free_host(e.sched_host[k])
        }
        cuda_mem_free(e.bkt_dev)
        cuda_mem_free_host(e.bkt_host)
        cuda_mem_free(e.rex_dev)
        cuda_mem_free_host(e.rex_host)
    }
    g_cxf.entries |> clear()
    g_cxf.land_entry |> clear()
    g_cxf.land_push |> clear()
    cxf_planes_drop()
    g_cxf.pool_copies = null
    g_cxf.pool_n = 0l
    g_cxf.pool_g = -1
    g_cxf.dma_open = false
    stream_cache_drop(true)
}

//! the device's stream state at the home's shutdown: the model's state dropped, then the events
def cud_stream_shutdown {
    return if (!g_cxf.ready)
    cud_stream_drop()
    for (ev in g_cxf.dma_evs) {
        cuda_event_destroy(ev)
    }
    g_cxf.dma_evs |> clear()
    cuda_event_destroy(g_cxf.ev_last)
    g_cxf.ev_last = null
    g_cxf.ready = false
}
