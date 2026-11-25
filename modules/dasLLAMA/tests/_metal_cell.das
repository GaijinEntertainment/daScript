options gen2
options indenting = 4
options _dasllama_internal = true

//! The Metal rail cells' rig - `_vkd_oracles`' `CellPlanes` verbs on the engine's Metal device, so a case body of
//! `_discrete_tts_cells.das` runs on the Metal home as it runs on the discrete two: a cell's planes (a run behind poisoned
//! base slots with slack past it, a written plane under its sentinel), `cell_launch` handing a case the planes seated on a
//! Metal rail over one command buffer, the readbacks, the device's skip and the stamp assert.
module _metal_cell shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require metal/das_metal_boost public
require dasllama/dasllama_gpu_rail   // rail_seat
require dasllama/dasllama_metal_rail public
require _compares   // at_base, NAN_SENTINEL_BITS
require _kernel_oracles   // cell_role

//! One cell's device planes: each plane's buffer and bytes, the element its run starts at, the run's length and a written
//! plane's sentinel word (0 on a plane the kernels only read); the queue the cell records on
struct MtPlanes {
    queue : MetalCommandQueue?
    buf : array<MetalBuffer?>
    bytes : array<int64>
    base : array<int>
    run : array<int>
    fill : array<uint>
}

def private add_plane(var cp : MtPlanes; var buf : MetalBuffer?; bytes : int64; base, run : int; fill : uint) : int {
    cp.buf |> push(buf)
    cp.bytes |> push(bytes)
    cp.base |> push(base)
    cp.run |> push(run)
    cp.fill |> push(fill)
    return length(cp.buf) - 1
}

//! a plane the kernels only read: `a` behind `base` poisoned slots and followed by `slack` more
def in_(var cp : MtPlanes; a : array<float>; base : int = 0; slack : int = 0) : int {
    var inscope laid <- at_base(a, base, slack)
    return cp |> add_plane(buf_upload(g_dev, laid), long_length(laid) * 4l, base, length(a), 0u)
}

//! a plane the kernels only read, `a` uploaded as it is - ids, words, halves
def in_plain(var cp : MtPlanes; a : array<auto(TT)>) : int =>
    cp |> add_plane(buf_upload(g_dev, a), long_length(a) * int64(typeinfo sizeof(type<TT>)), 0, length(a), 0u)

//! a plane the kernels write: `n` floats behind `base` slots and `slack` more, the whole plane under the NaN sentinel
def out_(var cp : MtPlanes; n : int; base : int = 0; slack : int = 0) : int =>
    cp |> add_plane(buf_fill(g_dev, base + n + slack, NAN_SENTINEL_BITS), int64(base + n + slack) * 4l, base, n, NAN_SENTINEL_BITS)

//! a written plane that is not floats: `bytes` under the sentinel `word` (f16 NaNs, Q8_0 -128)
def out_words(var cp : MtPlanes; bytes : int64; word : uint) : int =>
    cp |> add_plane(buf_fill(g_dev, int(bytes / 4l), word), bytes, 0, int(bytes / 4l), word)

//! a plane the kernels read and write in place: `a` as `in_` lays it, refilled to the NaN sentinel
def io_(var cp : MtPlanes; a : array<float>; base : int = 0; slack : int = 0) : int {
    var inscope laid <- at_base(a, base, slack)
    return cp |> add_plane(buf_upload(g_dev, laid), long_length(laid) * 4l, base, length(a), NAN_SENTINEL_BITS)
}

//! this home's bar of `bars`: the stamps a cell holds to the CPU's transcendentals compile with fast math off here
[unused_argument(cp)]
def home_bar(cp : MtPlanes; bars : HomeBars) : Bar => bars.metal

//! a rail over the cell's planes, every plane seated at its role (`cell_role`)
def cell_rail(cp : MtPlanes) : MtRail {
    assert(length(cp.buf) <= TOWER_BUFS, "MtPlanes: a cell seats at most one plane a rail role")
    var rl : MtRail
    for (p in range(length(cp.buf))) {
        rail_seat(rl, cell_role(p), intptr(cp.buf[p]), cp.bytes[p], 0u)
    }
    return <- rl
}

//! `blk`'s dispatches on the cell's rail in one command buffer, committed, waited, `tag` asserted
def cell_launch(cp : MtPlanes; t : T?; tag : string; blk : block<(var rl : MtRail) : void>) {
    var rl <- cell_rail(cp)
    var err : string
    let ran = with_compute_encoder(cp.queue, err) $(enc : MetalComputeEncoder?) {
        rl.enc = unsafe(reinterpret<MetalComputeEncoder?>(enc))
        invoke(blk, rl)
    }
    t |> success(ran, "{tag}: the dispatches complete ({err})")
}

//! `n` elements of `TT` of plane `p`, `at` elements past its base: `plane_as(cp, p, type<uint16>, n)`
[unused_argument(elem)]
def plane_as(cp : MtPlanes; p : int; elem : auto(TT); n : int; at : int = 0) : array<TT -const> {
    assert(int64(cp.base[p]) * 4l + int64(at + n) * int64(typeinfo sizeof(type<TT>)) <= cp.bytes[p], "plane_as: a read past the plane's end")
    var res : array<TT -const>
    res |> reserve(n)
    unsafe {
        let bytes = reinterpret<uint8 const?>(metal_buffer_contents(cp.buf[p])) + cp.base[p] * 4
        let src = reinterpret<TT -const const?>(bytes)
        for (i in range(n)) {
            res |> push(src[at + i])
        }
    }
    return <- res
}

//! plane `p`'s live run read back as floats
def plane_run(cp : MtPlanes; p : int) : array<float> => <- cp |> plane_as(p, type<float>, cp.run[p])

//! plane `p` whole as floats: the slots before its base, its run and its slack
def plane_whole(cp : MtPlanes; p : int) : array<float> => <- buf_download(cp.buf[p], int(cp.bytes[p] / 4l), type<float>)

//! `a` over plane `p` from its base, between two launches: a rerun's poisoned or rescaled input
def plane_upload(cp : MtPlanes; p : int; a : array<auto(TT)>) {
    assert(int64(cp.base[p]) * 4l + long_length(a) * int64(typeinfo sizeof(type<TT>)) <= cp.bytes[p], "plane_upload: a write past the plane's end")
    unsafe {
        let bytes = reinterpret<uint8?>(metal_buffer_contents(cp.buf[p])) + cp.base[p] * 4
        var dst = reinterpret<TT -const?>(bytes)
        for (i in range(length(a))) {
            dst[i] = a[i]
        }
    }
}

//! the plane's sentinel over written plane `p` again, before a launch that must write it anew
def plane_refill(cp : MtPlanes; p : int) {
    var dst = unsafe(reinterpret<uint?>(metal_buffer_contents(cp.buf[p])))
    for (i in range64(cp.bytes[p] / 4l)) {
        unsafe(dst[i]) = cp.fill[p]
    }
}

//! every plane released and forgotten: the next leg of a case lays its own
def cell_clear(var cp : MtPlanes) {
    for (b in cp.buf) {
        metal_release(b)
    }
    cp.buf |> clear()
    cp.bytes |> clear()
    cp.base |> clear()
    cp.run |> clear()
    cp.fill |> clear()
}

//! a cell's planes for `blk` on a fresh queue, every buffer and the rail's pipelines freed after
def with_mt_planes(blk : block<(var cp : MtPlanes) : void>) {
    var cp : MtPlanes
    cp.queue = metal_new_command_queue(g_dev)
    invoke(blk, cp)
    cp |> cell_clear()
    metal_release(cp.queue)
    unsafe {
        delete cp
    }
    mtl_rail_shutdown()
}

//! true where the engine's Metal device is up; else the cell has registered the skip naming why
def metal_up(t : T?) : bool {
    return true if (metal_common_init())
    t |> skip("no Metal device on this box")
    return false
}

//! `built`, asserted: the stamps a cell dispatches compiled on the device
def stamps_built(t : T?; built : bool; what : string) : bool {
    t |> success(built, "{what} compile on the Metal device")
    return built
}
