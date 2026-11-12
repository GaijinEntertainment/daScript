options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_cuda_common shared public

require dasllama/dasllama_lint public
require cuda/das_cuda_boost public
require dasllama/dasllama_gpu_rail public   // the tower rail's roles and profile roles; `CutRail` below is this home's rail
require dasllama/dasllama_env

//! the device state the CUDA home's dispatch rail reads: the primary context, one stream, the warp as the subgroup
struct CudGpu {
    ready : bool
    tried : bool             //! one arm attempt a process: a box with no driver answers once
    cc : CudaContext?
    name : string
    sm_count : int
    sm : int                 //! the compute capability as major x 10 + minor (sm_120 reads 120)
    subgroup_size : int64    //! a warp
    rows_per_wg : int64      //! subgroups a workgroup at `CUD_WG_X` - the GEMV ladders' output rows a workgroup
    stream : void?
    copy : CudCopy           //! the copy stream: every pinned stack's DMA rides it, apart from the launches on `stream`
    side : CudSide           //! the rail's side streams, a fork's branches (`rail_fork`)
    dry : bool              //! the lane mint's walk: every seat runs its accept / refuse arithmetic and allocates and uploads nothing (`cud_mint_dry`)
}

struct CudCopy {
    stream : void?
}

let CUD_SIDE_STREAMS = 2   //! a rail fork's branches beside the main stream

//! the side streams a rail fork's branches launch on, the fork's event they wait on and the event each records for the join
struct CudSide {
    streams : void? [CUD_SIDE_STREAMS]
    joins : void? [CUD_SIDE_STREAMS]
    fork : void?
}

let CUD_WG_X = 64u   //! the kernel workgroup width the GEMV ladders size against (the Vulkan home's `WG_X`)
let FP4_RUNG_SM = 120   //! the first compute capability with the FP4 tensor rung (the block-scaled mma, an `sm_120a` text): below it the FP4 leaf has no kernel

var g_cud : CudGpu

//! the calling thread bound to the ready device ahead of a serving seat's device work: a host that steps a model from a thread
//! other than the one that loaded it reaches the driver unbound otherwise; no device, nothing to bind
[arch(at="../ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-thread-binding")]
def cud_bind() {
    verify(!g_cud.ready || cuda_bind_context(g_cud.cc), "dasLLAMA cuda: the device is ready and the calling thread cannot bind its context")
}

//! the tier's device, made ready once a context and bound to the calling thread at every entry
[arch(at="../ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-thread-binding")]
def cud_init() : bool {
    return cuda_bind_context(g_cud.cc) if (g_cud.ready)
    return false if (g_cud.tried)
    return cud_init_first()
}

[cold_path]
def private cud_init_first() : bool {
    g_cud.tried = true
    if (!cuda_available()) {
        to_log(LOG_INFO, "dasLLAMA cuda tier: no CUDA driver on this box\n")
        return false
    }
    if (!cuda_nvrtc_available()) {
        to_log(LOG_WARNING, "dasLLAMA cuda tier: the driver loads but NVRTC does not - kernels cannot compile\n")
        return false
    }
    var cc = cuda_create_context(0)
    if (cc == null) {
        to_log(LOG_WARNING, "dasLLAMA cuda tier: no context: {cuda_last_error()}\n")
        return false
    }
    g_cud.cc = cc
    g_cud.name = cuda_device_name(cc)
    g_cud.sm_count = cuda_device_sm_count(cc)
    g_cud.sm = cuda_device_compute_capability(cc)
    g_cud.subgroup_size = 32l
    g_cud.rows_per_wg = int64(CUD_WG_X / 32u)
    g_cud.stream = cuda_stream_create()
    g_cud.copy.stream = cuda_stream_create()
    for (i in range(CUD_SIDE_STREAMS)) {
        g_cud.side.streams[i] = cuda_stream_create()
        g_cud.side.joins[i] = cuda_event_create_untimed()
    }
    g_cud.side.fork = cuda_event_create_untimed()
    g_cud.ready = true
    cuda_set_launch_pdl(g_cud.sm >= 90 && g_env_vulkan.cuda_pdl != 0l)   // every kernel of the home comes through the emitter, which puts the entry wait in each
    to_log(LOG_INFO, "dasLLAMA cuda tier: device ready ({g_cud.name}, {g_cud.sm_count} SMs, sm_{g_cud.sm}, driver {cuda_driver_version()}, nvrtc {cuda_nvrtc_version()}, programmatic dependent launch {cuda_launch_pdl() ? "on" : "off"})\n")
    return true
}

var private g_cud_proc_bufs : array<uint64>   //! device buffers a family holds for the process (the kq grid codebooks): the shutdown frees them

//! a device buffer the shutdown frees
def cud_proc_buf(b : uint64) {
    g_cud_proc_bufs |> push(b)
}

//! the device epoch: a shutdown moves it, so a holder of a process buffer knows its buffer went with the last device
def cud_epoch() : int => g_cud_epoch

var private g_cud_shutdown_hooks : array<function<() : void>>   //! the home's other modules' device state, dropped ahead of the context (the stream chain's)

//! `f` runs at the shutdown before the device goes: a module whose state the context owns registers it once
def cud_on_shutdown(f : function<() : void>) {
    g_cud_shutdown_hooks |> push(f)
}

//! every loaded kernel module released and the device state dropped; a class rebuilds its functions on its next ensure
def cud_shutdown() {
    return if (!g_cud.ready)
    for (f in g_cud_shutdown_hooks) {
        invoke(f)
    }
    for (b in g_cud_proc_bufs) {
        cuda_mem_free(b)
    }
    g_cud_proc_bufs |> clear()
    for (m in g_cud_modules) {
        cuda_release(m)
    }
    g_cud_modules |> clear()
    g_cud_epoch++
    cuda_stream_destroy(g_cud.stream)
    g_cud.stream = null
    cuda_stream_destroy(g_cud.copy.stream)
    g_cud.copy.stream = null
    for (i in range(CUD_SIDE_STREAMS)) {
        cuda_stream_destroy(g_cud.side.streams[i])
        cuda_event_destroy(g_cud.side.joins[i])
    }
    cuda_event_destroy(g_cud.side.fork)
    g_cud.side = CudSide()
    cuda_release(g_cud.cc)
    g_cud.cc = null
    g_cud.ready = false
    g_cud.tried = false
}

def cud_sync() : bool => g_cud.ready && cuda_stream_synchronize(g_cud.stream)

var private g_cud_modules : array<CudaModule?>   //! every kernel module loaded under the live device, released at shutdown
var private g_cud_off_rung : table<string>   //! the kernels this card has no target for: FP4 rung texts below `FP4_RUNG_SM`
var private g_cud_epoch = 0   //! bumped at shutdown: a class built under an older epoch rebuilds

struct CudClass {
    ready : bool
    epoch : int
    fns : array<void?>        //! one per [cuda_dispatch] kernel, by ordinal
    names : array<string>     //! ... the census name each launches under
    smem : array<uint>        //! ... the dynamic shared bytes its launch passes
    blocks : array<uint3>     //! ... its block
}

def cud_class_init(var c : CudClass) {
    c.ready = true
    c.epoch = g_cud_epoch
}

def cud_fn_made(c : CudClass; ord : int) : bool =>
    c.ready && c.epoch == g_cud_epoch && ord < length(c.fns) && c.fns[ord] != null

//! the class modules whose kernels with no shared memory leave the carveout to the driver: the tower families' own
let private CUD_L1_CLASS_MODULES = fixed_array("dasllama_cuda_classes_front", "dasllama_cuda_classes_blocks", "dasllama_cuda_classes_asr")

def private cud_l1_class_module(class_module : string) : bool {
    for (m in CUD_L1_CLASS_MODULES) {
        return true if (m == class_module)
    }
    return false
}

var private g_cud_l1_full = false   //! the tower class modules' no-smem kernels take the full carveout too (`set_cuda_tower_carveout_full`)
var private g_cud_l1_fns : array<void?>   //! the loaded kernels the scoped policy leaves to the driver, this epoch's
var private g_cud_l1_epoch = -1

//! the carveout a scoped kernel asks under the policy: the whole of it, or the driver's split (-1, the driver default)
def private cud_l1_carveout : int => g_cud_l1_full ? 100 : -1

//! Test A/B seat: the tower class modules' no-smem kernels on the full carveout (`on`) or the driver's split, re-applied to every
//! kernel already loaded; answers the setting it replaced
def set_cuda_tower_carveout_full(on : bool) : bool {
    let was = g_cud_l1_full
    g_cud_l1_full = on
    if (on != was) {
        to_log(LOG_INFO, "dasLLAMA cuda tier: set_cuda_tower_carveout_full({on}) - the tower class modules' kernels with no shared memory take {on ? "the full carveout" : "the driver's split"}\n")
        if (g_cud_l1_epoch == g_cud_epoch) {
            for (fn in g_cud_l1_fns) {
                cuda_function_set_shared_carveout(fn, cud_l1_carveout())
            }
        }
    }
    return was
}

def cuda_tower_carveout_full : bool => g_cud_l1_full

//! the kernel's function, stamped in class module `class_module`, loaded from its cached cubin into slot `ord`; false with the reason logged where it fails
[cold_path, arch(at="../ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-carveout")]
def cud_class_fn(var c : CudClass; ord : int; text, entry : string; smem : uint64; blk : uint3; kernel, class_module : string) : bool {
    if (c.epoch != g_cud_epoch) {
        c.fns |> clear()
        c.epoch = g_cud_epoch
    }
    while (length(c.fns) <= ord) {
        c.fns |> push(null)
        c.names |> push("")
        c.smem |> push(0u)
        c.blocks |> push(uint3(1u, 1u, 1u))
    }
    return false if (key_exists(g_cud_off_rung, kernel))
    if (g_cud.sm < FP4_RUNG_SM && cuda_source_is_arch_specific(text)) {
        g_cud_off_rung |> insert(kernel)
        to_log(LOG_INFO, "dasLLAMA cuda tier: {kernel}: on the FP4 rung, which starts at compute capability {FP4_RUNG_SM}; this card is {g_cud.sm}, so it has no kernel\n")
        return false
    }
    var err : string
    var mod : CudaModule?
    let fn = function_from_source(g_cud.cc, text, kernel, entry, mod, err, uint(smem))
    if (fn == null) {
        to_log(LOG_ERROR, "dasLLAMA cuda tier: {kernel}: {err}\n")
        return false
    }
    g_cud_modules |> push(mod)
    c.fns[ord] = fn
    c.names[ord] = kernel
    c.smem[ord] = uint(smem)
    c.blocks[ord] = blk
    let threads = int(blk.x * blk.y * blk.z)
    let scoped = smem == 0ul && cud_l1_class_module(class_module)
    if (scoped) {
        if (g_cud_l1_epoch != g_cud_epoch) {
            g_cud_l1_fns |> clear()
            g_cud_l1_epoch = g_cud_epoch
        }
        g_cud_l1_fns |> push(fn)
    }
    let full_carveout = !scoped || g_cud_l1_full
    if (full_carveout) {
        cuda_function_set_shared_carveout(fn, 100)
    }
    to_log(LOG_INFO, "dasLLAMA cuda tier: {kernel} ({class_module}): {cuda_function_regs(fn)} registers a thread, {smem} bytes of dynamic shared memory, {cuda_function_blocks_per_sm(fn, threads, smem)} blocks of {threads} threads an SM, the carveout {full_carveout ? "full" : "the driver's"}\n")
    return true
}

var private @scratch g_cud_blob : array<uint8>   //! the launch's parameter blob, rebuilt a launch; the driver copies it at the launch call

//! how the launch rail treats a launch: run it and count it, only fold it into the walk's hash and count it (a dry walk), or run it
//! uncounted (the capture of a walk a dry walk counted)
enum CudWalk {
    run
    dry
    quiet
}

var private g_cud_walk = CudWalk.run
var private g_cud_walk_hash = 0ul

//! the launches and transfers `blk` issues, walked under `mode`; a dry walk answers its hash of them in order
[arch(at="../ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-chain-graphs")]
def cud_walk(mode : CudWalk; blk : block<void>) : uint64 {
    g_cud_walk = mode
    g_cud_walk_hash = 0x9e37ul
    invoke(blk)
    g_cud_walk = CudWalk.run
    return g_cud_walk_hash
}

//! a dry walk folds a transfer of `bytes` from `src` to `dst` (`src` 0: a zeroed range) into its hash; true where the walk is dry
def cud_walk_transfer(dst, src : uint64; bytes : int64) : bool {
    return false if (g_cud_walk != CudWalk.dry)
    g_cud_walk_hash = hash_combine64(hash_combine64(hash_combine64(g_cud_walk_hash, dst), src), uint64(bytes))
    return true
}

let CUD_WALK_FORK = 0xf0ul     //! the stream-switch words a dry walk folds: a rail fork, a branch (plus the branch's number), a join
let CUD_WALK_BRANCH = 0xb0ul
let CUD_WALK_JOIN = 0xf1ul

//! a dry walk folds a stream switch `word` (`CUD_WALK_FORK`, `CUD_WALK_BRANCH` + the branch, `CUD_WALK_JOIN`) into its hash; true where the walk is dry
def cud_walk_mark(word : uint64) : bool {
    return false if (g_cud_walk != CudWalk.dry)
    g_cud_walk_hash = hash_combine64(g_cud_walk_hash, word)
    return true
}

//! one launch of the class's kernel `ord`: `bufs` the device addresses in binding order, `pc` the args struct, `groups` blocks
def cud_launch(c : CudClass; ord : int; stream : void?; bufs : uint64 const[]; var pc : auto(PT)&; groups : uint) {
    if (!cud_fn_made(c, ord)) {
        panic("dasLLAMA cuda tier: kernel slot {ord} launched before its ensure")
    }
    let nb = length(bufs)
    let pbytes = typeinfo sizeof(type<PT>)
    let len = (nb * 8 + pbytes + 7) / 8 * 8
    g_cud_blob |> resize(len)
    unsafe {
        memcpy(addr<void?>(g_cud_blob[0]), addr<void?>(bufs[0]), int64(nb * 8))
        memcpy(addr<void?>(g_cud_blob[nb * 8]), addr<void?>(pc), int64(pbytes))
    }
    for (i in range(nb * 8 + pbytes, len)) {
        g_cud_blob[i] = 0u8
    }
    if (g_cud_walk == CudWalk.dry) {
        var h = hash_combine64(hash_combine64(g_cud_walk_hash, intptr(c.fns[ord])), uint64(groups))
        unsafe {
            let words = reinterpret<uint64 const?>(addr<void?>(g_cud_blob[0]))
            for (i in range(len / 8)) {
                h = hash_combine64(h, words[i])
            }
        }
        g_cud_walk_hash = h
    } elif (!cuda_launch(c.fns[ord], uint3(groups, 1u, 1u), c.blocks[ord], c.smem[ord], stream, unsafe(addr<void?>(g_cud_blob[0])), uint64(len))) {
        panic("dasLLAMA cuda tier: {c.names[ord]} launch failed: {cuda_last_error()}")
    }
    return if (g_cud_walk == CudWalk.quiet)
    g_cud_census[c.names[ord]] = (g_cud_census?[c.names[ord]] ?? 0) + 1   // nolint:PERF026 — a kernel's key inserts at its first launch, a count after
}

//! one GEMM site's cm2 pick for an encode: the tile column, its class slot, the k split and the workgroups its schedule map carries
struct CutPick {
    tc : int64
    tci : int
    nsplit : int64
    ksplit : int64
    groups : int64
}

//! the CUDA home's tower rail: the stream a chain body launches on, the planes by role (`TowerBuf`), the lane the chain
//! rides, and its GEMM sites' cm2 picks and bound planes (`dasllama_gpu_rail`); no hazard record, no sets - the stream orders
//! the launches, and a fork's branches launch on side streams its join waits on
struct CutRail {
    stream : void?
    main : void?         //! the stream a fork branched off (`rail_fork`), the one its join returns to
    branched : uint      //! the side streams the open fork launched on, a bit each
    bufs : RailBuf[TOWER_BUFS]
    lane : RailLane
    norms_base : int64   //! 0: this home uploads the blob whole, so every weight offset reads from the plane's start
    ones_off : int64     //! the ones row's element in the norms plane (the weightless rms reads it)
    cap_rows : int64     //! the scratch's row capacity: the im2col fills the column plane to it
    sites : CutPick[RAIL_SITES]          //! each GEMM site's cm2 pick
    binds : RailSiteBind[RAIL_SITES]     //! each GEMM site's planes and weight format
    @scratch @exact_size maps : array<uint>   //! the encode's schedule map offsets, one a GEMM record
}

//! the device address the plane at `role` binds at on a CUDA rail: its buffer's, past the seat's byte offset
def rail_dev(rl : CutRail; role : TowerBuf) : uint64 => rl.bufs[int(role)].dev + uint64(rl.bufs[int(role)].off)

//! kernel `ord` of the class launched over the rail's planes at `roles`: what the [cuda_dispatch] rail overload `enc_<name>(rl, roles, pc, ...)` calls
[arch(at = "../ARCHITECTURE_GPU_TOWER_RAIL.md#tower-rail")]
def cud_rail_launch(c : CudClass; ord : int; rl : CutRail; roles : auto(R); var pc : auto(PT)&; groups : uint) {
    var bufs : uint64[typeinfo dim(roles)]
    for (i in range(typeinfo dim(roles))) {
        bufs[i] = rail_dev(rl, roles[i])
    }
    cud_launch(c, ord, rl.stream, bufs, pc, groups)
}

var private g_cud_census : table<string; int>
var private g_cud_ensures : table<string; function<() : bool>>   //! every [cuda_dispatch] kernel's ensure, seeded at program start: the coverage universe, and the compile gate's reach

//! every [cuda_dispatch] kernel joins the coverage universe at program start (the macro generates an [init] calling this)
def cud_census_seed(kernel : string; ensure : function<() : bool>) {
    g_cud_ensures[kernel] = ensure
}

//! every seeded kernel's ensure run: the names whose function did not compile or load (the home's NVRTC compile gate);
//! a kernel this card has no target for (`cud_off_rung_names`) is not one of them
def cud_ensure_all : array<string> {
    var failed <- [for (name in cud_kernel_names()); name; where !invoke(g_cud_ensures[name]) && !key_exists(g_cud_off_rung, name)]
    return <- failed
}

//! the kernels an ensure found on the FP4 rung with the card below it, sorted
def cud_off_rung_names : array<string> {
    var names <- [for (k in keys(g_cud_off_rung)); k]
    names |> sort()
    return <- names
}

//! the launches `kernel` took this process; a name nothing seeded panics, so a misspelt key cannot read as a zero count
def cud_kernel_coverage_of(kernel : string) : int {
    if (!key_exists(g_cud_ensures, kernel)) {
        panic("dasLLAMA cuda tier: no kernel named {kernel} in the census")
    }
    return g_cud_census?[kernel] ?? 0
}

//! the census count of `kernel`'s dispatches, -1 for a name outside this home's census (a cell's witness over a stamp name
//! the other home spells): the Vulkan cells' `<stem>_spv` names read this home's `<stem>_cuda` through it
def cud_kernel_census_of(kernel : string) : int64 =>
    key_exists(g_cud_ensures, kernel) ? int64(g_cud_census?[kernel] ?? 0) : -1l

def cud_kernel_names : array<string> {
    var names <- [for (k in keys(g_cud_ensures)); k]
    names |> sort()
    return <- names
}

//! the milliseconds `blk`'s launches take on the device stream, event to event
def cud_time_ms(blk : block<void>) : float {
    let e0 = cuda_event_create()
    let e1 = cuda_event_create()
    cuda_event_record(e0, g_cud.stream)
    invoke(blk)
    cuda_event_record(e1, g_cud.stream)
    cuda_event_synchronize(e1)
    let ms = cuda_event_elapsed_ms(e0, e1)
    cuda_event_destroy(e0)
    cuda_event_destroy(e1)
    return ms
}
