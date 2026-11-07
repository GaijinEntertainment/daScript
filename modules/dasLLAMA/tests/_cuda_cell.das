options gen2
options indenting = 4
options _dasllama_internal = true

//! The CUDA kernel cells' rig - `_vkd_oracles`' `CellPlanes` verbs on the tier's stream, so a case body of `_discrete_cells.das`
//! runs on either discrete home: a cell's planes (a run behind poisoned base slots with slack past it, a written plane under its
//! sentinel), `cell_launch` handing a case the planes seated on a CUDA rail, the readbacks, the tier's skip, the launch
//! assert, the home's hooks for the stamps whose names differ per home, and the CUDA stamps only the cells dispatch.
module _cuda_cell shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require cuda/cuda_kernel public
require spirv/spirv_markers
require daslib/templates
require daslib/typemacro_boost
require math
require dasllama/dasllama_cuda_common public   // cud_init, g_cud, cud_sync, CutRail and the tier's buffer helpers
require dasllama/dasllama_cuda_dispatch
require dasllama/dasllama_cuda_classes   // the f16 GEMM's s and m column stamps
require dasllama/dasllama_gpu_rail   // rail_seat
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gpu_kernels_common public
require dasllama/dasllama_gpu_kernels_discrete public
require _compares   // at_base, NAN_SENTINEL_BITS
require _kernel_oracles   // cell_role

//! One cell's device planes: each plane's buffer and bytes, the element its run starts at, the run's length and a written
//! plane's sentinel word (0 on a plane the kernels only read)
struct CuPlanes {
    dev : array<uint64>
    bytes : array<int64>
    base : array<int>
    run : array<int>
    fill : array<uint>
}

def private add_plane(var cp : CuPlanes; dev : uint64; bytes : int64; base, run : int; fill : uint) : int {
    cp.dev |> push(dev)
    cp.bytes |> push(bytes)
    cp.base |> push(base)
    cp.run |> push(run)
    cp.fill |> push(fill)
    return length(cp.dev) - 1
}

//! a plane the kernels only read: `a` behind `base` poisoned slots and followed by `slack` more
def in_(var cp : CuPlanes; a : array<float>; base : int = 0; slack : int = 0) : int {
    var inscope laid <- at_base(a, base, slack)
    return cp |> add_plane(buf_upload(laid, g_cud.stream), long_length(laid) * 4l, base, length(a), 0u)
}

//! a plane the kernels only read, `a` uploaded as it is - ids, words, halves
def in_plain(var cp : CuPlanes; a : array<auto(TT)>) : int =>
    cp |> add_plane(buf_upload(a, g_cud.stream), long_length(a) * int64(typeinfo sizeof(type<TT>)), 0, length(a), 0u)

//! a plane the kernels write: `n` floats behind `base` slots and `slack` more, the whole plane under the NaN sentinel
def out_(var cp : CuPlanes; n : int; base : int = 0; slack : int = 0) : int =>
    cp |> add_plane(buf_fill(base + n + slack, NAN_SENTINEL_BITS, g_cud.stream), int64(base + n + slack) * 4l, base, n, NAN_SENTINEL_BITS)

//! a plane the kernels write that is not floats: `bytes` under the sentinel `word` - a NaN in both halves for f16, -128 bytes for
//! Q8_0 quants
def out_words(var cp : CuPlanes; bytes : int64; word : uint) : int =>
    cp |> add_plane(buf_fill(int(bytes / 4l), word, g_cud.stream), bytes, 0, int(bytes / 4l), word)

//! a plane the kernels read and write in place: `a` as `in_` lays it, refilled to the NaN sentinel
def io_(var cp : CuPlanes; a : array<float>; base : int = 0; slack : int = 0) : int {
    var inscope laid <- at_base(a, base, slack)
    return cp |> add_plane(buf_upload(laid, g_cud.stream), long_length(laid) * 4l, base, length(a), NAN_SENTINEL_BITS)
}

//! the launches of `tag` completed on the stream, asserted
def cu_launched(t : T?; tag : string) {
    t |> success(cud_sync(), "{tag}: the launches complete ({cuda_last_error()})")
}

//! the launches `blk` makes on the rail it is handed - every plane seated at its role (`cell_role`), the tier's stream -
//! completed, asserted under `tag`
def cell_launch(cp : CuPlanes; t : T?; tag : string; blk : block<(var rl : CutRail) : void>) {
    assert(length(cp.dev) <= TOWER_BUFS, "CuPlanes: a cell seats at most one plane a rail role")
    var rl : CutRail
    rl.stream = g_cud.stream
    for (p in range(length(cp.dev))) {
        rail_seat(rl, cell_role(p), cp.dev[p], cp.bytes[p], 0u)
    }
    invoke(blk, rl)
    cu_launched(t, tag)
}

//! `n` elements of `TT` of plane `p`, `at` elements past its base: `plane_as(cp, p, type<uint16>, n)`
[unused_argument(elem)]
def plane_as(cp : CuPlanes; p : int; elem : auto(TT); n : int; at : int = 0) : array<TT -const> =>
    <- buf_download(cp.dev[p] + uint64(cp.base[p]) * 4ul + uint64(at) * uint64(typeinfo sizeof(type<TT>)), n, type<TT -const>)

//! plane `p`'s live run read back as floats
def plane_run(cp : CuPlanes; p : int) : array<float> => <- cp |> plane_as(p, type<float>, cp.run[p])

//! `a` over plane `p` from its base, between two launches: a rerun's poisoned or rescaled input
def plane_upload(cp : CuPlanes; p : int; a : array<auto(TT)>) {
    cuda_memcpy_htod(cp.dev[p] + uint64(cp.base[p]) * 4ul, unsafe(addr<void?>(a[0])), uint64(long_length(a)) * uint64(typeinfo sizeof(type<TT>)), g_cud.stream)
}

//! the plane's sentinel over written plane `p` again, before a launch that must write it anew
def plane_refill(cp : CuPlanes; p : int) {
    var inscope words <- [for (_i in range64(cp.bytes[p] / 4l)); cp.fill[p]]
    cuda_memcpy_htod(cp.dev[p], unsafe(addr<void?>(words[0])), uint64(cp.bytes[p]), g_cud.stream)
}

//! every plane freed and forgotten: the next leg of a case lays its own
def cell_clear(var cp : CuPlanes) {
    for (dev in cp.dev) {
        cuda_mem_free(dev)
    }
    cp.dev |> clear()
    cp.bytes |> clear()
    cp.base |> clear()
    cp.run |> clear()
    cp.fill |> clear()
}

//! a cell's planes for the length of `blk`, every buffer freed after it
def with_cu_planes(blk : block<(var cp : CuPlanes) : void>) {
    var cp : CuPlanes
    invoke(blk, cp)
    cp |> cell_clear()
    delete cp
}

//! true where the CUDA tier is up; else the cell has registered the skip naming why
def cuda_up(t : T?) : bool {
    return true if (cud_init())
    t |> skip("no CUDA device with NVRTC (the cuda tier's log names why)")
    return false
}

//! `built`, asserted: the stamps a cell launches compiled and loaded through NVRTC on a box whose tier is up
def stamps_built(t : T?; built : bool; what : string) : bool {
    t |> success(built, "{what} compile and load through NVRTC ({cuda_last_error()})")
    return built
}

//! the f16 GEMM's s column stamp (`CuF16GemmCm2WS`) over `roles` on the grid `rail_mm16` launches it at
def f16_gemm_s_launch(rl : CutRail; roles : TowerBuf const[3]; var pc : F16GemmArgs; rows, d : int64) {
    enc_f16_gemm_cm2_ws_cls(rl, roles, pc, f16_gemm_cm2_wgs(rows, d, 32l))
}

//! ... and its m column stamp (`CuF16GemmCm2W`), the s column's bit-for-bit twin
def f16_gemm_m_launch(rl : CutRail; roles : TowerBuf const[3]; var pc : F16GemmArgs; rows, d : int64) {
    enc_f16_gemm_cm2_w_cls(rl, roles, pc, f16_gemm_cm2_wgs(rows, d))
}

//! the whisper decoder's attention combine into f32 rows (`GkWdecAttnComb`): the oracle twin the cells hold the fused requant
//! combine and the chunked attention pair to - no engine site launches it
[cuda_dispatch(name = "tower_wdec_attn_comb_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerWdecAttnComb : GkWdecAttnComb {
    override HS = 64u

    [cuda_kernel(local_size_x = 64, name = "tower_wdec_attn_comb_cls_cuda")]
    def run {
        wdec_attn_comb_body()
    }
}
