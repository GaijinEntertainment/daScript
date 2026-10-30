options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_gpu_stream_cache shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_gpu_tier   // MoeGpuStreamExpert / MoeGpuStreamView, stream_page_span, STREAM_PAGE / STREAM_RING_BASE, moe_gpu_stream_source, gpu_said_once
require dasllama/dasllama_env        // the expert cache's knobs (the readers, the prof line)
require dasllama/dasllama_math       // the engine's job-queue arm
require dasllama/dasllama_par        // maybe_parallel_for: par_memcpy's chunks
require dasllama/dasllama_boot_restore
require daslib/fio
require daslib/jobque_boost
require math

// The expert cache both discrete homes keep for the streamed layers' expert stacks: a stack's pinned planes in the device
// layout (every expert's, or the resident quota of a partial stack), the resident set by heat, the transient ring an expert
// absent from its stack lands in off the lane file, the reader threads that fill it, and the view the host rail reads through
// (`ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-arena`, `#expert-cache`). The home supplies the pinned memory - a
// Vulkan host-cached buffer, a CUDA page-locked allocation - through `set_stream_pinned_fns`, and sources its DMA copies off
// a slot's `handle` / `mapped` pointer; everything else here is host code with no device word in it.

//! a pinned host allocation the home made for the cache: `handle` the home's buffer object (a VkBuffer; 0 on a home that addresses
//! pinned memory by pointer), `aux` its memory object, `mapped` the host pointer the readers fill and the DMA sources, `bytes` its
//! size; a refused allocation has `mapped` null
struct PinnedBuf {
    handle : uint64
    aux : uint64
    mapped : void?
    bytes : int64
    home : int   //! the home that made it (`STREAM_HOME_*`): its own free takes it back, whichever home serves by then
}

//! the discrete homes the cache draws pinned memory from; a buffer carries its maker's
let STREAM_HOME_VULKAN = 1
let STREAM_HOME_CUDA = 2

typedef StreamPinnedAllocFn = function<(bytes : int64; soft : bool) : PinnedBuf>
typedef StreamPinnedFreeFn = function<(b : PinnedBuf) : void>

[unused_argument(bytes, soft)]
def private stream_pinned_alloc_unset(bytes : int64; soft : bool) : PinnedBuf {
    panic("dasLLAMA expert cache: a streamed stack asked for pinned memory before its home installed an allocator - require dasllama/dasllama_math_vulkan or dasllama/dasllama_math_cuda, whose init calls set_stream_pinned_fns")
    return PinnedBuf()
}

[unused_argument(b)]
def private stream_pinned_free_unset(b : PinnedBuf) {}

var private g_stream_pinned_alloc_vk = @@stream_pinned_alloc_unset
var private g_stream_pinned_alloc_cud = @@stream_pinned_alloc_unset
var private g_stream_pinned_free_vk = @@stream_pinned_free_unset
var private g_stream_pinned_free_cud = @@stream_pinned_free_unset

//! `home`'s pinned memory: `alloc` returns a buffer with `mapped` null where it refuses (a `soft` ask degrades, a hard one panics in
//! the home), `free` returns one of its own; both homes may install in one process, and a buffer is freed by the home that made it
def set_stream_pinned_fns(home : int; alloc : StreamPinnedAllocFn; free : StreamPinnedFreeFn) {
    if (home == STREAM_HOME_CUDA) {
        g_stream_pinned_alloc_cud = alloc
        g_stream_pinned_free_cud = free
    } else {
        g_stream_pinned_alloc_vk = alloc
        g_stream_pinned_free_vk = free
    }
}

//! the home serving this process's GPU tier: the CUDA home where it is asked for, else the Vulkan one
def private stream_pinned_home : int => moe_gpu_backend_name() == "cuda" ? STREAM_HOME_CUDA : STREAM_HOME_VULKAN

//! a pinned buffer from the serving home, stamped with it
def private stream_pinned_take(bytes : int64; soft : bool) : PinnedBuf {
    let home = stream_pinned_home()
    var b = invoke(home == STREAM_HOME_CUDA ? g_stream_pinned_alloc_cud : g_stream_pinned_alloc_vk, bytes, soft)
    b.home = home
    return b
}

[init, boot_restore]
def private restore_stream_cache_fn_defaults {}

let STREAM_PLANE_KINDS = 3   //! a streamed layer's stacks come gate, up, down: stack k's kind is k % STREAM_PLANE_KINDS

//! the per-expert chain's protocol, the same on both discrete homes: the ring depth of commands and schedule entries in flight before
//! a re-record waits, the experts one command takes at most (its rows under the chunk cap besides), the share of a layer's regions a
//! window's bundle takes where no pin sizes it, and the meet's bundles the device holds unfinished before the host stops offering it more
let XF_RING = 16
let XF_BUNDLE = 32l
let XF_BUNDLE_SHARE = 16l
let XF_AHEAD = 2l

//! one streamed layer plane's stack: the pinned planes in device layout and the expert cache's table over them
struct StreamStack {
    base : int64
    elems : int64
    fmt : int
    wqbytes : int64          //! the whole stack's plane bytes, every expert's
    wsbytes : int64
    hwq : PinnedBuf          //! the pinned planes (the DMA's source): every expert's, or `slots` experts' of a partial stack
    hws : PinnedBuf
    nexp : int64             //! the experts the stack holds, `qlen` / `slen` plane bytes each
    qlen : int64
    slen : int64
    slots : int64            //! the stack's resident slots (nexp where the stack is whole)
    @exact_size slot_of : array<int>     //! expert e's slot: a resident slot, `STREAM_RING_BASE` + a ring slot, or -1 where the cache does not hold it
    @exact_size expert_of : array<int>   //! the resident slots' experts
    qoff : int64 = -1l       //! the stack's planes in the lane file (the slice walk's source, captured at the load: the walk's table is gone by the windows)
    soff : int64 = -1l
}

var @do_not_delete @exact_size g_stream_stacks : array<StreamStack>   //! the streamed stacks in walk order (gate, up, down a layer)
var g_stream_max_gu_wq = 0l   //! the widest gate-or-up and down planes among the streamed stacks: the whole-layer slot's capacity
var g_stream_max_gu_ws = 0l
var g_stream_max_dn_wq = 0l
var g_stream_max_dn_ws = 0l

//! a streamed stack registered in walk order (gate, up, down), the down's or the gate/up pair's plane maxima grown by its planes
def stream_stack_push(var ss : StreamStack) {
    if (length(g_stream_stacks) % STREAM_PLANE_KINDS == 2) {
        g_stream_max_dn_wq = max(g_stream_max_dn_wq, ss.wqbytes)
        g_stream_max_dn_ws = max(g_stream_max_dn_ws, ss.wsbytes)
    } else {
        g_stream_max_gu_wq = max(g_stream_max_gu_wq, ss.wqbytes)
        g_stream_max_gu_ws = max(g_stream_max_gu_ws, ss.wsbytes)
    }
    g_stream_stacks |> emplace(ss)
}

//! the streamed chain's plane caps, the same on both discrete homes: the window's f32 rows and the accumulator (one output plane;
//! the Vulkan batch arm keeps two), the gathered f16 rows, the chunk's row cap (it bounds y and the workgroup map), the f16
//! hidden rows (the down GEMM's feed) and the combine's weight and map planes (npos x k grid entries x 4 bytes)
let BATCH_Y_BYTES = 67_108_864l
let BATCH_XQ_BYTES = 33_554_432l
let BATCH_CHUNK_ROWS = 8_192l
let BATCH_HQ_BYTES = 33_554_432l
let COMB_WI_BYTES = 2_097_152l

//! the chunk's row cap of the expert chain: the gathered f16 rows, the f16 hidden rows and the f32 output planes all fit,
//! 128-aligned as the tiles' columns
[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def xf_chunk_rows(n, nfe : int64) : int64 {
    var r = min(BATCH_XQ_BYTES / (n * 2l), BATCH_HQ_BYTES / (nfe * 2l)) - TILE_READ_SLACK
    r = min(r, BATCH_Y_BYTES / (max(nfe, n) * 4l))
    let rows = min(r, BATCH_CHUNK_ROWS - TILE_READ_SLACK) / 128l * 128l
    assert(rows >= 128l, "dasLLAMA expert chain: the chain's planes hold no chunk at this width")
    return rows
}

//! a stream slot's plane capacities: a whole layer's widest planes, or the ring's experts of the widest expert planes under the
//! ring form
def stream_slot_caps : tuple<gu_wq : int64; gu_ws : int64; dn_wq : int64; dn_ws : int64> {
    var cap = (gu_wq = g_stream_max_gu_wq, gu_ws = g_stream_max_gu_ws, dn_wq = g_stream_max_dn_wq, dn_ws = g_stream_max_dn_ws)
    return cap if (!moe_gpu_stream_ring())
    let ring = moe_gpu_stream_ring_experts()
    cap = (gu_wq = 0l, gu_ws = 0l, dn_wq = 0l, dn_ws = 0l)
    for (k in range(length(g_stream_stacks))) {
        let ss & = unsafe(g_stream_stacks[k])
        if (k % STREAM_PLANE_KINDS == 2) {
            cap.dn_wq = max(cap.dn_wq, ss.qlen * ring)
            cap.dn_ws = max(cap.dn_ws, ss.slen * ring)
        } else {
            cap.gu_wq = max(cap.gu_wq, ss.qlen * ring)
            cap.gu_ws = max(cap.gu_ws, ss.slen * ring)
        }
    }
    return cap
}

//! an opened window of the per-expert chain as the host sees it on either home: the group, the pool prefix, the regions' offset
//! triples, the shape, and the bundles pushed so far; a home's meet derives from it with its own device handles
struct XfMeetHost {
    on : bool
    g : int
    npool : int64
    offs1 : int64 const?
    offs3 : int64 const?
    offs2 : int64 const?
    nregions : int64
    ege : int64
    cap : int64
    n : int64
    nfe : int64
    nk : int64
    npos : int64
    is_gelu : bool
    pushed : int64
    done : int64      //! the commands whose chains retired, in push order
    experts : int64
    t0 : int64
    exp_bytes : int64
}

//! the rows of regions [r_lo, r_hi) of the opened window `m`; -1 past the chunk cap or the bundle's expert count (a window over
//! the pool alone takes a token's device share in one submit; a bundle binds one plane set, the pool's or the slot's, never
//! across the seam)
def xf_bundle_rows(m : XfMeetHost; r_lo, r_hi : int64) : int64 {
    let cap_experts = m.npool == m.nregions && m.npool > 0l ? XF_BUNDLE : (g_env_vulkan.vk_xf_bundle > 0l ? min(g_env_vulkan.vk_xf_bundle, XF_BUNDLE) : clamp(m.nregions / XF_BUNDLE_SHARE, 1l, XF_BUNDLE))
    return -1l if (r_hi - r_lo > cap_experts || (r_lo < m.npool && r_hi > m.npool))
    var rows = 0l
    for (r in range64(r_lo, r_hi)) {
        rows += unsafe(m.offs1[r * 3l + 2l])
    }
    return rows <= m.cap ? rows : -1l
}

//! the end of the bundle starting at region `r_lo` of the opened window `m`: as many regions as XF_BUNDLE and the chunk cap admit,
//! at least one
def xf_bundle_end(m : XfMeetHost; r_lo, r_max : int64) : int64 {
    var r_hi = r_lo + 1l
    while (r_hi < r_max && xf_bundle_rows(m, r_lo, r_hi + 1l) >= 0l) {
        r_hi++
    }
    return r_hi
}

//! region `r`'s expert of the opened window `m`: the engine's list where it names one (a pool region's, whose offsets name a
//! slot), else off the region's gate offset against the group's `base`
def xf_region_expert(m : XfMeetHost; r, base : int64) : int64 {
    let le = moe_gpu_ffn_expert(r)
    return le >= 0l ? le : (unsafe(m.offs1[r * 3l]) - base) / m.ege
}

//! the last `n` streamed stacks dropped (a partial layer triple the pinned budget cut mid-way), their pinned planes freed under `free`
def stream_stacks_pop(n : int; free : bool) {
    for (_i in range(n)) {
        let k = length(g_stream_stacks) - 1
        if (free) {
            stream_pinned_release(g_stream_stacks[k].hwq)
            stream_pinned_release(g_stream_stacks[k].hws)
        }
        g_stream_stacks |> pop()
    }
}

//! a pinned buffer back to the home that made it (`b.home`), whichever home serves by then; a refused one goes nowhere
def stream_pinned_release(b : PinnedBuf) {
    if (b.mapped != null) {
        invoke(b.home == STREAM_HOME_CUDA ? g_stream_pinned_free_cud : g_stream_pinned_free_vk, b)
    }
}

//! the streamed stack at (`woff`, `fmt`), -1 where none sits there
def stream_stack_at(woff : int64; fmt : int) : int {
    for (i in range(length(g_stream_stacks))) {
        if (g_stream_stacks[i].base == woff && g_stream_stacks[i].fmt == fmt) {
            return i
        }
    }
    return -1
}

//! the stream group of the stack at (`woff`, `fmt`), -1 where no stack sits there - never the stack index divided, whose -1 rounds to group 0
def stream_group_of(woff : int64; fmt : int) : int {
    let k = stream_stack_at(woff, fmt)
    return k < 0 ? -1 : k / STREAM_PLANE_KINDS
}

//! the stream group (a layer's three stacks) holding gate offset `w0` of format `fmt`, -1 where none does
def stream_group_at(w0 : int64; fmt : int) : int {
    for (g in range(length(g_stream_stacks) / STREAM_PLANE_KINDS)) {
        let ss & = unsafe(g_stream_stacks[g * STREAM_PLANE_KINDS])
        return g if (ss.fmt == fmt && w0 >= ss.base && w0 < ss.base + ss.elems)
    }
    return -1
}

//! `len` bytes at `off` of the lane read through the buffered handle `f` into `dst`; true where every byte came (0 bytes trivially).
//! Buffered, not direct: the lane stays mapped for the planes the host reads, and the kernel serializes unbuffered reads of a
//! mapped file behind its coherency flush (eight readers at one reader's rate) where buffered reads keep their parallelism
def private stream_read_span(f : file; off : int64; dst : void?; len : int64) : bool {
    return true if (len <= 0l)
    return false if (fseek(f, off, seek_set) != 0l)
    return _builtin_read64(f, dst, len) == len
}

def private stream_read_plane(path : string; off : int64; dst : void?; bytes : int64) {
    fopen(path, "rb") $(f) {
        if (f == null) {
            panic("dasLLAMA expert cache: cannot open '{path}' to read a streamed stack")
        }
        if (!stream_read_span(f, off, dst, bytes)) {
            panic("dasLLAMA expert cache: '{path}' came up short reading {bytes} bytes at {off} - the lane is corrupt, delete it to re-mint")
        }
    }
}

let PAR_COPY_CHUNK = 2_097_152l

[arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-arena")]
def par_memcpy(var dst : void?; src : void?; bytes : int64) {
    if (bytes < PAR_COPY_CHUNK * 2l || !is_job_que_available()) {
        unsafe(memcpy(dst, src, bytes))
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

var private g_cache_nexp = 0l        //! the experts a streamed stack holds (`stream_cache_layer`, set by the driver before each layer's upload)
var private g_cache_quota = 0l       //! ... and how many of them get a resident slot (0 = every one)
var private g_cache_ring_slots = 0l  //! the transient ring's slots a plane kind (`stream_cache_ring`)
var private g_cache_path = ""        //! the lane file the on-demand reads come off
var private g_cache_order : array<int>   //! the next stacks' experts by heat, the hottest first (`stream_cache_order`); empty = the id order
var private g_cache_said = false
var private g_cache_nolane_said = false
var private g_stream_read_said = false
var private g_release_stall_said : table<string>   //! the release's stall warning, once a model

//! MoeGpuStreamCacheOrderFn: the experts of the next streamed layers by heat; a partial stack keeps its quota off its head
def stream_cache_order(order : array<int>) {
    g_cache_order := order
}

//! MoeGpuStreamCacheLayerFn: the next streamed layer's stacks hold `nexp` experts, `quota` of them resident (0 = every one)
def stream_cache_layer(nexp, quota : int64) {
    g_cache_nexp = nexp
    g_cache_quota = quota
}

//! MoeGpuStreamCacheRingFn: the transient ring takes `slots` experts a plane kind, read off `path`; made at the first fetch, each kind's slot at the widest expert stride of that kind
def stream_cache_ring(slots : int64; path : string) {
    g_cache_ring_slots = slots
    g_cache_path = path
}

//! the experts a streamed stack holds under the cache's current layer word (1 where none was said)
def stream_cache_nexp : int64 => max(g_cache_nexp, 1l)

//! the resident experts of a partial stack holding `keep` of `nexp`: the head of the heat order where one covers the stack,
//! the first ids otherwise, ascending either way so the pinned stack keeps the lane's order and reads in runs
def private stream_resident_set(nexp, keep : int64) : array<int> {
    if (keep < nexp && long_length(g_cache_order) >= nexp) {
        var chosen <- [for (s in range64(keep)); g_cache_order[s]]
        sort(chosen)
        return <- chosen
    }
    return <- [for (e in range64(keep)); int(e)]
}

//! the resident experts' planes read into the stack's slots, a run of consecutive ids one read (or one copy off `src`)
def private stream_fill_slots(chosen : array<int>; path : string; src : uint8 const?; var dst : void?; off, len : int64) {
    return if (len <= 0l)
    var s = 0l
    let keep = long_length(chosen)
    while (s < keep) {
        var m = 1l
        while (s + m < keep && chosen[s + m] == chosen[s] + int(m)) {
            m++
        }
        let a = int64(chosen[s])
        unsafe {
            var d = reinterpret<void?>(reinterpret<uint8?>(dst) + s * len)
            if (!empty(path)) {
                stream_read_plane(path, off + a * len, d, m * len)
            } else {
                par_memcpy(d, reinterpret<void?>(src + a * len), m * len)
            }
        }
        s += m
    }
}

//! a streamed stack over pinned memory: `keep` of the layer's `nexp` experts get a resident slot (by id, or by heat), filled off the
//! lane file the slice walk named, else off the gathered planes `wq` / `ws`; false where the home's pinned memory refused (the walk
//! rolls the partial triple back and stops); under `dry` the record alone, for the plan
def stream_stack_make(wq : uint8 const?; ws : uint8 const?; woff : int64; n, rows : int64; fmt : int; wqbytes, wsbytes : int64; dry : bool) : bool {
    let nexp = stream_cache_nexp()
    let src = moe_gpu_stream_source(woff, fmt)
    let lane_backed = dry || !empty(src.path)   // a stack with no lane behind it (a live regather) stays whole: the ring would have nothing to read its misses off
    let keep = g_cache_quota > 0l && g_cache_quota < nexp && lane_backed ? g_cache_quota : nexp
    if (!lane_backed && g_cache_quota > 0l && g_cache_quota < nexp && !g_cache_nolane_said) {
        g_cache_nolane_said = true
        to_log(LOG_WARNING, "dasLLAMA expert cache: the budget keeps {g_cache_quota} of {nexp} experts, and the stack at {woff} was not read off a lane - this stack stays resident whole\n")
    }
    var ss = StreamStack(base = woff, elems = rows * n, fmt = fmt, wqbytes = wqbytes, wsbytes = wsbytes,
                         nexp = nexp, qlen = wqbytes / nexp, slen = wsbytes / nexp, slots = keep)
    let kq = keep * ss.qlen
    let ks = keep * ss.slen
    let chosen <- stream_resident_set(nexp, keep)
    if (!dry) {
        ss.hwq = stream_pinned_take(kq, true)
        if (ss.hwq.mapped == null) {
            return false
        }
        ss.hws = stream_pinned_take(max(ks, 64l), true)
        if (ss.hws.mapped == null) {
            stream_pinned_release(ss.hwq)
            return false
        }
        if (!empty(src.path)) {
            if (src.qbytes != wqbytes || (wsbytes > 0l && src.sbytes != wsbytes)) {
                panic("dasLLAMA expert cache: the image's streamed stack at {woff} (fmt {fmt}) holds {src.qbytes}+{src.sbytes} bytes, the home wants {wqbytes}+{wsbytes} - the lane is corrupt, delete it to re-mint")
            }
            ss.qoff = src.qoff
            ss.soff = src.soff
            stream_fill_slots(chosen, src.path, null, ss.hwq.mapped, src.qoff, ss.qlen)
            stream_fill_slots(chosen, src.path, null, ss.hws.mapped, src.soff, ss.slen)
            if (!g_stream_read_said) {
                g_stream_read_said = true
                to_log(LOG_INFO, "dasLLAMA expert cache: stream stacks READ off the image file into their pinned memory - the mapping's expert pages are never faulted (DASLLAMA_VK_STREAM_FILE_READ=0 copies them out of the mapping)\n")
            }
        } else {
            if (wq == null) {
                panic("dasLLAMA expert cache: the streamed stack at {woff} (fmt {fmt}) was handed no planes and names no file source")
            }
            stream_fill_slots(chosen, "", wq, ss.hwq.mapped, 0l, ss.qlen)
            stream_fill_slots(chosen, "", ws, ss.hws.mapped, 0l, ss.slen)
        }
    }
    ss.slot_of |> reserve_resize(nexp)
    ss.expert_of |> reserve_resize(keep)
    for (e in range64(nexp)) {
        ss.slot_of[e] = -1
    }
    for (s in range64(keep)) {
        ss.expert_of[s] = chosen[s]
        ss.slot_of[chosen[s]] = int(s)
    }
    if (keep < nexp && !g_cache_said) {
        g_cache_said = true
        to_log(LOG_INFO, "dasLLAMA expert cache: {keep} of {nexp} experts a streamed stack resident - the rest read off the lane on demand into the transient ring\n")
    }
    stream_stack_push(ss)
    return true
}

//! a streamed stack whose planes the home holds already (a mapped window imported as device memory): every expert at its own index
def stream_stack_whole(woff : int64; n, rows : int64; fmt : int; wqbytes, wsbytes : int64; hwq, hws : PinnedBuf) {
    let nexp = stream_cache_nexp()
    var ss = StreamStack(base = woff, elems = rows * n, fmt = fmt, wqbytes = wqbytes, wsbytes = wsbytes, hwq = hwq, hws = hws,
                         nexp = nexp, qlen = wqbytes / nexp, slen = wsbytes / nexp, slots = nexp)
    ss.slot_of |> reserve_resize(nexp)
    ss.expert_of |> reserve_resize(nexp)
    for (e in range64(nexp)) {
        ss.slot_of[e] = int(e)
        ss.expert_of[e] = int(e)
    }
    stream_stack_push(ss)
}

var private g_ring_hwq : array<PinnedBuf>   //! a slot a plane kind (0 gate, 1 up, 2 down) for an expert read off the lane on demand
var private g_ring_hws : array<PinnedBuf>
var private g_ring_qlen : array<int64>      //! a kind's slot stride: the kind's widest expert rounded up a page, plus one
var private g_ring_slen : array<int64>
var private g_ring_slots = 0l
var private g_ring_stack : array<int>       //! the slot's holder (-1 free)
var private g_ring_expert : array<int>
var private g_ring_next = 0l                //! the next slot to hand out, round robin
var private g_ring_reader : file            //! the lane file open for the inline on-demand reads (no reader threads), opened at the first fetch
var private g_ring_fetches = 0l             //! experts read off the lane since the arm (the cells' witness)
var private g_ring_fetch_us = 0l

//! the slot's quant and scale pointers and DMA sources of expert `e` of streamed stack `k`; present false with no slot
def stream_slot(k : int; e : int64) : MoeGpuStreamExpert {
    unsafe {
        let ss & = g_stream_stacks[k]
        let s = ss.slot_of[e]
        return default<MoeGpuStreamExpert> if (s < 0)
        if (s >= STREAM_RING_BASE) {
            let r = int64(s - STREAM_RING_BASE)
            let kind = k % STREAM_PLANE_KINDS
            let qo = r * g_ring_qlen[kind] + stream_page_span(ss.qoff, ss.qlen, e).skew
            let so = r * g_ring_slen[kind] + stream_page_span(ss.soff, ss.slen, e).skew
            return (q = reinterpret<uint8 const?>(g_ring_hwq[kind].mapped) + qo, s = reinterpret<uint8 const?>(g_ring_hws[kind].mapped) + so,
                    qbuf = g_ring_hwq[kind].handle, qoff = qo, sbuf = g_ring_hws[kind].handle, soff = so, present = true)
        }
        let qo = int64(s) * ss.qlen
        let so = int64(s) * ss.slen
        return (q = reinterpret<uint8 const?>(ss.hwq.mapped) + qo, s = reinterpret<uint8 const?>(ss.hws.mapped) + so,
                qbuf = ss.hwq.handle, qoff = qo, sbuf = ss.hws.handle, soff = so, present = true)
    }
}

//! MoeGpuStreamExpertFn: expert `e` of the streamed stack at (`woff`, `fmt`) as the host reads it and the DMA sources it
def stream_expert(woff : int64; fmt : int; e : int64) : MoeGpuStreamExpert {
    let k = stream_stack_at(woff, fmt)
    return k < 0 ? default<MoeGpuStreamExpert> : stream_slot(k, e)
}

//! MoeGpuStreamViewFn: the streamed stack at (`woff`, `fmt`) as the cache lays it out, for a reader that captures it in the main context and resolves experts on a lane
def stream_view(woff : int64; fmt : int) : MoeGpuStreamView {
    let k = stream_stack_at(woff, fmt)
    return default<MoeGpuStreamView> if (k < 0)
    unsafe {
        let ss & = g_stream_stacks[k]
        let kind = k % STREAM_PLANE_KINDS
        let ring = !empty(g_ring_hwq)
        return (q = reinterpret<uint8 const?>(ss.hwq.mapped), s = reinterpret<uint8 const?>(ss.hws.mapped), slot_of = addr<int const?>(ss.slot_of[0]),
                nexp = ss.nexp, qlen = ss.qlen, slen = ss.slen, qoff = ss.qoff, soff = ss.soff,
                ring_q = ring ? reinterpret<uint8 const?>(g_ring_hwq[kind].mapped) : null, ring_s = ring ? reinterpret<uint8 const?>(g_ring_hws[kind].mapped) : null,
                ring_qlen = ring ? g_ring_qlen[kind] : 0l, ring_slen = ring ? g_ring_slen[kind] : 0l)
    }
}

//! the ring's planes for every kind, at each kind's widest expert stride among the streamed stacks, and the lane's reader
[cold_path]
def private stream_ring_make {
    g_ring_hwq |> resize(STREAM_PLANE_KINDS)
    g_ring_hws |> resize(STREAM_PLANE_KINDS)
    g_ring_qlen |> resize(STREAM_PLANE_KINDS)
    g_ring_slen |> resize(STREAM_PLANE_KINDS)
    for (i in range(length(g_stream_stacks))) {
        let kind = i % STREAM_PLANE_KINDS
        g_ring_qlen[kind] = max(g_ring_qlen[kind], g_stream_stacks[i].qlen)
        g_ring_slen[kind] = max(g_ring_slen[kind], g_stream_stacks[i].slen)
    }
    for (kind in range(STREAM_PLANE_KINDS)) {   // a slot holds its expert's page-widened span: the stride is the widest expert rounded up a page, plus one
        g_ring_qlen[kind] = ((g_ring_qlen[kind] + STREAM_PAGE - 1l) & ~(STREAM_PAGE - 1l)) + STREAM_PAGE
        g_ring_slen[kind] = ((g_ring_slen[kind] + STREAM_PAGE - 1l) & ~(STREAM_PAGE - 1l)) + STREAM_PAGE
    }
    for (kind in range(STREAM_PLANE_KINDS)) {
        g_ring_hwq[kind] = stream_pinned_take(g_cache_ring_slots * g_ring_qlen[kind], false)
        g_ring_hws[kind] = stream_pinned_take(max(g_cache_ring_slots * g_ring_slen[kind], 64l), false)
    }
    g_ring_slots = g_cache_ring_slots
    g_ring_stack |> reserve_resize(g_cache_ring_slots * int64(STREAM_PLANE_KINDS))
    g_ring_expert |> reserve_resize(g_cache_ring_slots * int64(STREAM_PLANE_KINDS))
    for (i in range64(g_cache_ring_slots * int64(STREAM_PLANE_KINDS))) {
        g_ring_stack[i] = -1
        g_ring_expert[i] = -1
    }
    g_ring_reader = fopen(g_cache_path, "rb")
    if (g_ring_reader == null) {
        panic("dasLLAMA expert cache: cannot open '{g_cache_path}' for its on-demand reads")
    }
    to_log(LOG_INFO, "dasLLAMA expert cache: the transient ring: {g_cache_ring_slots} slots a plane kind ({(g_cache_ring_slots * (g_ring_qlen[0] + g_ring_slen[0] + g_ring_qlen[1] + g_ring_slen[1] + g_ring_qlen[2] + g_ring_slen[2])) >> 20l} MB of pinned memory; DASLLAMA_EXPERT_RING sizes it) over '{g_cache_path}'\n")
}

let private FETCH_QUIT = -1   //! the `stack` of a request that orders a reader to quit, and of the answer that acknowledges it

//! one on-demand read a reader thread takes: expert `expert` of stack `stack` into ring slot `slot` - the page-aligned lane
//! spans of its two planes and the slot's pointers
struct private FetchReq {
    stack : int
    expert : int
    slot : int
    qoff : int64
    soff : int64
    qlen : int64
    slen : int64
    dq : void?
    ds : void?
}

//! what a reader thread answers: the request's slot landed (`ok` false where a read came up short), or its quit acknowledged
struct private FetchDone {
    stack : int
    expert : int
    slot : int
    ok : bool
    t0 : int64   //! the reader's clock at the read's start and end: the window's busy sum against its wall says how many overlapped
    t1 : int64
}

var private g_fetch_req : Channel?    //! the main context's requests to the readers
var private g_fetch_done : Channel?   //! the readers' answers to the main context
var private g_fetch_threads = 0       //! the reader threads alive
var private g_fetch_span = 0l         //! the widest slot of any kind: a reader's bounce holds one
var private g_fetch_bounce : array<void?>   //! the readers' bounce buffers, freed at the quit
var private g_fetch_pending = 0l      //! requests queued and not yet answered
var private g_ring_pending : array<bool>   //! a ring slot whose read is queued or in flight: the window may not read it yet
var private g_cache_win_fetches = 0l   //! the counters at the last release: the prof line reports a window's own
var private g_cache_win_us = 0l
var private g_fetch_busy_us = 0l   //! the window's reads summed, and the first start and last end among them
var private g_fetch_first = 0l
var private g_fetch_last = 0l

//! the reader threads: each on its own direct handle of the lane (opened here, in the main context - a handle is a C++ pointer
//! a thread may own, where a path string across contexts is a dangling read), looping on the request channel
[cold_path]
def private stream_readers_make {
    let n = int(g_env_vulkan.expert_readers)
    return if (n <= 0 || g_fetch_threads > 0)
    g_fetch_req = unsafe(channel_create())
    g_fetch_done = unsafe(channel_create())
    g_fetch_req |> append(1)   // a channel's timed pop waits only while a job is outstanding: one stands for the readers' lifetime
    g_fetch_done |> append(1)
    for (kind in range(STREAM_PLANE_KINDS)) {
        g_fetch_span = max(g_fetch_span, max(g_ring_qlen[kind], g_ring_slen[kind]))
    }
    g_fetch_bounce |> reserve(n)
    for (_i in range(n)) {
        let h = fopen(g_cache_path, "rb")
        if (h == null) {
            panic("dasLLAMA expert cache: cannot open '{g_cache_path}' for a reader thread")
        }
        var req = g_fetch_req
        var done = g_fetch_done
        var b : void?
        unsafe {
            let raw = malloc(uint64(g_fetch_span + STREAM_PAGE))   // the thread's own page-aligned ordinary memory: a direct read into the pinned slot itself runs at a tenth the rate
            g_fetch_bounce |> push(raw)
            b = reinterpret<void?>((reinterpret<uint64>(raw) + uint64(STREAM_PAGE) - 1ul) & ~(uint64(STREAM_PAGE) - 1ul))
        }
        new_thread_loop() @() : bool {
            var cont = true
            pop_with_timeout_clone(req, 50) $(rq : FetchReq#) {
                if (rq.stack == FETCH_QUIT) {
                    fclose(h)
                    var ack_to = done   // `release` nulls the handle it releases: the answer goes through a copy of the pointer
                    req |> release()
                    done |> release()
                    ack_to |> push_clone(FetchDone(stack = FETCH_QUIT, expert = -1, slot = -1, ok = true))
                    cont = false
                } else {
                    let t0 = ref_time_ticks()
                    var ok = false
                    unsafe {
                        ok = stream_read_span(h, rq.qoff, b, rq.qlen)   // the popped request is a temporary: its fields are passed on as they are
                        if (ok) {
                            memcpy(rq.dq, b, rq.qlen)
                            ok = stream_read_span(h, rq.soff, b, rq.slen)
                            if (ok && rq.slen > 0l) {
                                memcpy(rq.ds, b, rq.slen)
                            }
                        }
                    }
                    done |> push_clone(FetchDone(stack = rq.stack, expert = rq.expert, slot = rq.slot, ok = ok, t0 = t0, t1 = ref_time_ticks()))
                }
            }
            return cont
        }
        g_fetch_threads++
    }
    to_log(LOG_INFO, "dasLLAMA expert cache: {n} reader threads are up\n")
}

//! the readers quit in order - each takes a quit request and answers it, then the channels go
[finalize]
def stream_readers_quit {
    return if (g_fetch_threads == 0)
    for (_i in range(g_fetch_threads)) {
        g_fetch_req |> push_clone(FetchReq(stack = FETCH_QUIT, expert = -1, slot = -1, qoff = 0l, soff = 0l, qlen = 0l, slen = 0l, dq = null, ds = null))
    }
    var acked = 0
    var rounds = 0
    let rounds_max = 20 + 2 * g_fetch_threads   // ten seconds past the answers still queued: a reader that never answers is left behind, a hung exit is worse
    while (acked < g_fetch_threads && rounds < rounds_max) {
        rounds++
        pop_with_timeout_clone(g_fetch_done, 500) $(d : FetchDone#) {
            if (d.stack == FETCH_QUIT) {
                acked++
            }
        }
    }
    if (acked < g_fetch_threads) {
        // the readers left behind keep their bounce buffers and the channels: removing a channel a thread still reads is the worse exit
        g_fetch_req |> notify()   // the lifetime job done: a waiter left behind wakes and leaves
        g_fetch_done |> notify()
        g_fetch_req |> release()
        g_fetch_done |> release()
        to_log(LOG_WARNING, "dasLLAMA expert cache: {g_fetch_threads - acked} of the reader threads did not answer the quit order - their bounce buffers and the request channels are left to them\n")
    } else {
        unsafe {
            channel_remove(g_fetch_req)
            channel_remove(g_fetch_done)
        }
        for (b in g_fetch_bounce) {
            unsafe(free(b))
        }
    }
    g_fetch_req = null
    g_fetch_done = null
    g_fetch_bounce |> clear()
    g_fetch_threads = 0
    g_fetch_pending = 0l
}

//! the readers' answers taken: a landed slot is readable, a short read a panic; `wait` waits for one answer where any is owed
def private stream_drain(wait : bool) {
    return if (g_fetch_threads == 0)
    var any = true
    var waited = !wait
    while (any) {
        any = false
        if (!waited && g_fetch_pending > 0l) {
            waited = true
            any = pop_with_timeout_clone(g_fetch_done, 2000) $(d : FetchDone#) {
                stream_landed(d)
            }
            continue
        }
        any = try_pop_clone(g_fetch_done) $(d : FetchDone#) {
            stream_landed(d)
        }
    }
}

def private stream_landed(d : FetchDone const#) {
    return if (d.stack == FETCH_QUIT)
    if (!d.ok) {
        panic("dasLLAMA expert cache: expert {d.expert} of streamed stack {d.stack}: a reader thread's read off '{g_cache_path}' came up short - the lane is corrupt, delete it to re-mint")
    }
    g_ring_pending[(d.stack % STREAM_PLANE_KINDS) * int(g_ring_slots) + d.slot] = false
    g_fetch_pending--
    g_fetch_busy_us += int64(get_time_usec(d.t0)) - int64(get_time_usec(d.t1))
    g_fetch_first = g_fetch_first == 0l ? d.t0 : min(g_fetch_first, d.t0)
    g_fetch_last = max(g_fetch_last, d.t1)
}

//! MoeGpuStreamRequestFn: true where expert `e` has or will have a slot; a slot the window uses is never taken (the window's end frees them all), and with no reader threads the read runs here
[arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#expert-cache")]
def stream_request(woff : int64; fmt : int; e : int64) : bool {
    let k = stream_stack_at(woff, fmt)
    return false if (k < 0)
    unsafe {
        var ss & = g_stream_stacks[k]
        return true if (ss.slot_of[e] >= 0)
        return false if (g_cache_ring_slots <= 0l)
        if (empty(g_ring_hwq)) {
            stream_ring_make()
            g_ring_pending |> reserve_resize(g_cache_ring_slots * int64(STREAM_PLANE_KINDS))
            stream_readers_make()
        }
        if (ss.qoff < 0l) {
            panic("dasLLAMA expert cache: no lane file to read expert {e} of the stack at {woff} from (the stack was not read off a lane)")
        }
        let kind = k % STREAM_PLANE_KINDS
        let r0 = kind * int(g_ring_slots)
        var r = -1
        for (_i in range64(g_ring_slots)) {
            let cand = int(g_ring_next % g_ring_slots)
            g_ring_next++
            if (g_ring_stack[r0 + cand] < 0) {
                r = cand
                break
            }
        }
        if (r < 0) {
            panic("dasLLAMA expert cache: the ring of {g_ring_slots} slots a kind is full inside one window - DASLLAMA_EXPERT_RING must hold a layer's misses")
        }
        g_ring_stack[r0 + r] = k
        g_ring_expert[r0 + r] = int(e)
        ss.slot_of[e] = STREAM_RING_BASE + r
        let qs = stream_page_span(ss.qoff, ss.qlen, e)
        let sp = stream_page_span(ss.soff, ss.slen, e)
        let rq <- FetchReq(stack = k, expert = int(e), slot = r, qoff = qs.aoff, soff = sp.aoff, qlen = qs.alen, slen = ss.slen > 0l ? sp.alen : 0l,
                           dq = reinterpret<void?>(reinterpret<uint8?>(g_ring_hwq[kind].mapped) + int64(r) * g_ring_qlen[kind]),
                           ds = reinterpret<void?>(reinterpret<uint8?>(g_ring_hws[kind].mapped) + int64(r) * g_ring_slen[kind]))
        g_ring_fetches++
        if (g_fetch_threads > 0) {
            g_ring_pending[r0 + r] = true
            g_fetch_req |> push_clone(rq)
            g_fetch_pending++
        } else {
            let t0 = ref_time_ticks()
            if (!stream_read_span(g_ring_reader, rq.qoff, rq.dq, rq.qlen) || !stream_read_span(g_ring_reader, rq.soff, rq.ds, rq.slen)) {
                panic("dasLLAMA expert cache: expert {e} of the stack at {woff}: the read of {rq.qlen} + {rq.slen} bytes at {rq.qoff} / {rq.soff} off '{g_cache_path}' came up short - the lane is corrupt, delete it to re-mint")
            }
            g_ring_fetch_us += int64(get_time_usec(t0))
        }
        return true
    }
}

//! MoeGpuStreamReadyFn: whether expert `e` of the streamed stack at (`woff`, `fmt`) sits in a slot the window may read now
def stream_ready(woff : int64; fmt : int; e : int64) : bool {
    let k = stream_stack_at(woff, fmt)
    return false if (k < 0)
    let s = g_stream_stacks[k].slot_of[e]
    return false if (s < 0)
    return true if (s < STREAM_RING_BASE)
    stream_drain(false)
    return !g_ring_pending[(k % STREAM_PLANE_KINDS) * int(g_ring_slots) + (s - STREAM_RING_BASE)]
}

//! MoeGpuStreamFetchFn: `stream_request` and the wait for its read: true with a readable slot after
def stream_fetch(woff : int64; fmt : int; e : int64) : bool {
    return false if (!stream_request(woff, fmt, e))
    let t0 = ref_time_ticks()
    while (!stream_ready(woff, fmt, e)) {
        if (g_fetch_pending <= 0l) {
            panic("dasLLAMA expert cache: expert {e} of the stack at {woff} is neither readable nor in flight")
        }
        stream_drain(true)
    }
    g_ring_fetch_us += int64(get_time_usec(t0))
    return true
}

//! MoeGpuStreamRingReleaseFn: every ring slot freed, a read still in flight landing first
def stream_ring_release {
    return if (empty(g_ring_stack))
    let t0 = ref_time_ticks()
    var rounds = 0
    while (g_fetch_pending > 0l) {
        stream_drain(true)
        rounds++
        if (rounds == 5) {
            gpu_said_once(g_release_stall_said, LOG_WARNING, "stream_ring_release: ", "the expert cache's release waits on queued reads past its first rounds - the reads run behind the window (DASLLAMA_GPU_PROF reports the tail)")
        }
    }
    if (g_env_gpu.gpu_prof) {
        let tail_us = int64(get_time_usec(t0))
        let wall_us = g_fetch_first == 0l ? 0l : int64(get_time_usec(g_fetch_first)) - int64(get_time_usec(g_fetch_last))
        to_log(LOG_INFO, "expert_cache: {g_ring_fetches - g_cache_win_fetches} fetches this window, {g_ring_fetch_us - g_cache_win_us} us waited on them, {tail_us} us for the tail at the release; the readers {g_fetch_busy_us} us busy over a {wall_us} us wall\n")
        g_cache_win_fetches = g_ring_fetches
        g_cache_win_us = g_ring_fetch_us
        g_fetch_busy_us = 0l
        g_fetch_first = 0l
        g_fetch_last = 0l
    }
    for (i in range64(g_ring_slots * int64(STREAM_PLANE_KINDS))) {
        let k = g_ring_stack[i]
        if (k >= 0) {
            g_stream_stacks[k].slot_of[g_ring_expert[i]] = -1
            g_ring_stack[i] = -1
            g_ring_expert[i] = -1
        }
    }
}

//! MoeGpuStreamCacheStatsFn
def stream_cache_stats : tuple<fetches : int64; us : int64> => (fetches = g_ring_fetches, us = g_ring_fetch_us)

//! the cache dropped with the model: the readers quit, the ring's shells and the stacks' records go, their pinned memory freed
//! through the home's seat under `free` (a home whose own sweep frees every pinned buffer passes false)
[cold_path]
def stream_cache_drop(free : bool) {
    stream_readers_quit()
    if (free) {
        for (ss in g_stream_stacks) {
            stream_pinned_release(ss.hwq)
            stream_pinned_release(ss.hws)
        }
        for (hwq, hws in g_ring_hwq, g_ring_hws) {
            stream_pinned_release(hwq)
            stream_pinned_release(hws)
        }
    }
    delete g_stream_stacks
    g_stream_max_gu_wq = 0l
    g_stream_max_gu_ws = 0l
    g_stream_max_dn_wq = 0l
    g_stream_max_dn_ws = 0l
    g_ring_hwq |> clear()
    g_ring_hws |> clear()
    g_ring_qlen |> clear()
    g_ring_slen |> clear()
    g_ring_stack |> clear()
    g_ring_expert |> clear()
    g_ring_slots = 0l
    g_ring_next = 0l
    g_ring_pending |> clear()
    if (g_ring_reader != null) {
        fclose(g_ring_reader)
        g_ring_reader = null
    }
    g_cache_nexp = 0l
    g_cache_quota = 0l
    g_cache_ring_slots = 0l
}
