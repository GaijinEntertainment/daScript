options gen2
options _dasllama_internal = true

// Shared GPU plumbing for the per-class Metal kernel-oracle suites (test_metal_*_kernels): the
// MetalCell record, the readback compares, the device shell and the pipeline stamps. The suites
// require it behind `?das_metal`; off-Apple its das_metal require is a missing prerequisite, so
// nothing loads it and the lint lane skips it.
module _metal_kernel_common shared public

require dasllama/dasllama_lint public
require math
require daslib/ast
require daslib/ast_boost
require daslib/templates_boost   // qmacro: the pipeline-stamp macros
require strings
require daslib/strings_boost   // split: a stamp= family's fields
require dasllama/dasllama_convert   // dequant_k{4,5,6}_plane_superblock — the kq reference dequant
require dasllama/dasllama_gemm_schema  // kq_qsb — the ONE per-format quant-plane stride source
require daslib/f16_cvt              // f32_to_f16 — packing synthetic scale planes
require dastest/testing_boost
require _compares
require _metal4_probe   // metal4_tensor_available: the tensor stamps' skip rule
require dasllama/dasllama_metal_common   // g_dev: the decode set's device
require dasllama/dasllama_metal_decode   // metal_decode_init / metal_decode_shutdown: the decode-device shell
require dasllama/dasllama_metal_lens   // msl_global_of / metal_dispatch_names: the emitter's own naming rules
require metal/das_metal_boost public

def buf_u32(dev; v : uint) : auto => buf_fill(dev, 1, v)

def buf_f32(dev; v : float) : auto => buf_fill(dev, 1, v)

//! One 64-lane 1-D dispatch of `total` threads: the pipeline set, the block binds, the threadgroups dispatched.
def dispatch_64_lanes(t : T?; queue; tag : string; pso; total : int; blk : block<(enc : MetalComputeEncoder?)>) : bool {   //! the block's type is what lets a caller write `$(enc)`; every consumer loads this module behind `?das_metal`
    var err2 : string
    let ran = with_compute_encoder(queue, err2) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        invoke(blk, enc)
        metal_dispatch_threadgroups(enc, uint3(uint((total + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag} dispatch: {err2}")
    return ran
}

def sigmoid_d(x : double) : double => 1.0lf / (1.0lf + exp(-x))

def silu_d(x : double) : double => x * sigmoid_d(x)

//! One Metal kernel cell's planes: each plane's buffer, bytes, element base and live run, and for a plane the kernels write
//! the bytes of the sentinel it holds before a run; the queue the cell records on and the pipelines it releases with them.
struct MetalCell {
    dev : MetalDevice?
    queue : MetalCommandQueue?
    buf : array<MetalBuffer?>
    bytes : array<int64>
    base : array<int>
    run : array<int>
    sentinel : array<array<uint8>>
    pso : array<MetalComputePipeline?>
}

def private add_plane(var mc : MetalCell; var buf : MetalBuffer?; bytes : int64; base, run : int; var sentinel : array<uint8>) : int {
    mc.buf |> push(buf)
    mc.bytes |> push(bytes)
    mc.base |> push(base)
    mc.run |> push(run)
    mc.sentinel |> emplace(sentinel)
    return length(mc.buf) - 1
}

//! a plane the kernels only read: `a` behind `base` poisoned slots and followed by `slack` more
def in_(var mc : MetalCell; a : array<float>; base : int = 0; slack : int = 0) : int {
    var none : array<uint8>
    if (base == 0 && slack == 0) {
        return mc |> add_plane(buf_upload(mc.dev, a), long_length(a) * 4l, 0, length(a), none)   //! uploaded where it lies: a weight plane past the unreserved-resize guard takes no host copy
    }
    var inscope whole <- at_base(a, base, slack)
    return mc |> add_plane(buf_upload(mc.dev, whole), long_length(whole) * 4l, base, length(a), none)
}

//! a plane the kernels only read, `a` uploaded as it is - ids, words, halves, bytes
def in_plain(var mc : MetalCell; a : array<auto(TT)>) : int {
    var none : array<uint8>
    return mc |> add_plane(buf_upload(mc.dev, a), long_length(a) * int64(typeinfo sizeof(type<TT>)), 0, length(a), none)
}

//! a plane the kernels write: `n` elements of `sentinel`, the value a cell reads where no run wrote
def out_words(var mc : MetalCell; n : int; sentinel : auto(TT)) : int {
    var v : TT -const = sentinel
    var bits : array<uint8>
    bits |> resize(typeinfo sizeof(type<TT>))
    unsafe {
        memcpy(addr(bits[0]), addr(v), typeinfo sizeof(type<TT>))
    }
    return mc |> add_plane(buf_fill(mc.dev, n, v), int64(n) * int64(typeinfo sizeof(type<TT>)), 0, n, bits)
}

//! a plane the kernels write: `n` floats behind `base` slots and `slack` more, the whole plane under the NaN sentinel
def out_(var mc : MetalCell; n : int; base : int = 0; slack : int = 0) : int {
    let p = mc |> out_words(base + n + slack, NAN_SENTINEL)
    mc.base[p] = base
    mc.run[p] = n
    return p
}

//! a plane the kernels read and write in place: `a` as `in_` lays it, read back as `out_`
def io_(var mc : MetalCell; a : array<float>; base : int = 0; slack : int = 0) : int => mc |> in_(a, base, slack)

//! plane `p`'s live run, read as floats
def plane_run(mc : MetalCell; p : int) : array<float> {
    let src = unsafe(reinterpret<float const?>(metal_buffer_contents(mc.buf[p])))
    return <- [for (i in range(mc.run[p])); unsafe(src[mc.base[p] + i])]
}

//! plane `p` whole, read as floats: the slots before its base, its run and its slack
def plane_whole(mc : MetalCell; p : int) : array<float> => buf_download(mc.buf[p], int(mc.bytes[p] / 4l), type<float>)

//! `n` elements of plane `p` from element `at`, read as `TT` - halves, bytes, words
[unused_argument(elem)]
def plane_as(mc : MetalCell; p : int; elem : auto(TT); n : int; at : int = 0) : array<TT -const> {
    assert(int64(at + n) * int64(typeinfo sizeof(type<TT>)) <= mc.bytes[p], "plane_as: a read past the plane's end")
    let src = unsafe(reinterpret<TT -const const?>(metal_buffer_contents(mc.buf[p])))
    var res : array<TT -const>
    res |> resize(n)
    for (i in range(n)) {
        res[i] = unsafe(src[at + i])
    }
    return <- res
}

//! `a` over the start of plane `p` again, before a run that must read it anew
def plane_upload(mc : MetalCell; p : int; a : array<auto(TT)>) {
    assert(long_length(a) * int64(typeinfo sizeof(type<TT>)) <= mc.bytes[p], "plane_upload: a write past the plane's end")
    buf_store(mc.buf[p], a)
}

//! written plane `p`'s sentinel over the whole plane again, before a run that must write it anew
def plane_refill(mc : MetalCell; p : int) {
    let w = length(mc.sentinel[p])
    assert(w > 0, "plane_refill: the plane is not a written plane")
    var dst = unsafe(reinterpret<uint8?>(metal_buffer_contents(mc.buf[p])))
    for (i in range64(mc.bytes[p])) {
        unsafe(dst[i]) = mc.sentinel[p][int(i % int64(w))]
    }
}

//! `pso` released with the cell's planes; `pso` back, null where its compile failed
def keep_pso(var mc : MetalCell; var pso : MetalComputePipeline?) : MetalComputePipeline? {
    mc.pso |> push(pso)
    return pso
}

//! one command buffer on `queue`: `rec` encodes the dispatches; its encode asserted under `tag`, and true when it ran
def queue_record(t : T?; queue : MetalCommandQueue?; tag : string; rec : block<(enc : MetalComputeEncoder?) : void>) : bool {
    var err : string
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        invoke(rec, enc)
    }
    t |> success(ran, "{tag}: encode: {err}")
    return ran
}

//! `queue_record` on the cell's queue
def cell_record(mc : MetalCell; t : T?; tag : string; rec : block<(enc : MetalComputeEncoder?) : void>) : bool => queue_record(t, mc.queue, tag, rec)

//! a kernel cell's planes on `dev`, recording on `queue`, for the length of `blk`; every buffer and kept pipeline released after it
def with_metal_cell(var dev : MetalDevice?; var queue : MetalCommandQueue?; blk : block<(var mc : MetalCell) : void>) {
    var mc = MetalCell(dev = dev, queue = queue)
    invoke(blk, mc)
    for (b in mc.buf) {
        metal_release(b)
    }
    for (p in mc.pso) {
        metal_release(p)
    }
    mc.buf |> clear()
    mc.pso |> clear()
    unsafe {
        delete mc.buf
        delete mc.pso
    }
    delete mc.bytes
    delete mc.base
    delete mc.run
    delete mc.sentinel
}

//! a `stamp_of`'s pipeline compiled on the cell's device and released with its planes; null with `err` where the compile failed
def keep_stamp(var mc : MetalCell; st : auto(ST); var err : string&) : MetalComputePipeline? {
    return mc |> keep_pso(invoke(st.make, mc.dev, err))
}

//! `pso` - `st`'s own pipeline - in the pipeline global `st`'s builder reads for the length of `blk`, and the global's own
//! value back after: a dispatch through `st.enc` inside `blk` runs the gate's pipeline with the builder's binds and grid
def stamp_run(st : auto(ST); var pso : MetalComputePipeline?; blk : block<() : void>) {
    var was = invoke(st.seat, pso)
    invoke(blk)
    invoke(st.seat, was)
}

//! the pipeline asserted under `tag`; true when it compiled
def pso_ok(t : T?; tag : string; pso : MetalComputePipeline?; err : string) : bool {
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return pso != null
}

//! A Metal-4 tensor stamp's pipeline: true when it compiled; where it did not, a skip when the capability probe finds no
//! tensor toolchain on the box, else a red with the compile error.
def tensor_pso_or_skip(t : T?; tag : string; pso : MetalComputePipeline?; dev : MetalDevice?; err : string) : bool {
    if (pso == null && !metal4_tensor_available(dev)) {
        t |> skip("{tag}: no Metal-4 tensor toolchain on this box")
        return false
    }
    return pso_ok(t, tag, pso, err)
}

//! A suite's device shell: `blk` on the system device and a fresh queue - the two a `MetalCell` holds, so they reach the
//! gates writable - a skip where the box has no Metal device, and the leak gate after: every Metal object the cells made
//! is released.
def with_metal_gate(t : T?; blk : block<(var dev : MetalDevice?; var queue : MetalCommandQueue?) : void>) {
    var dev = metal_create_system_default_device()
    if (dev == null) {
        t |> skip("no Metal device on this box; GPU kernel checks skipped")
    } else {
        var queue = metal_new_command_queue(dev)
        invoke(blk, dev, queue)
        metal_release(queue)
        metal_release(dev)
    }
    t |> equal(metal_log_leaks_if_any(), 0l)
}

//! A suite's shell on the decode set's device: `blk` on a fresh queue over `g_dev` with the decode PSOs compiled, a skip
//! where they cannot be, and the decode set released after.
def with_metal_decode_gate(t : T?; blk : block<(var queue : MetalCommandQueue?) : void>) {
    if (!metal_decode_init()) {
        t |> skip("no Metal device")
        return
    }
    var queue = metal_new_command_queue(g_dev)
    invoke(blk, queue)
    metal_release(queue)
    metal_decode_shutdown()
}

//! The GEMV/GEMM cells' envelope bar, |kernel - reference| <= max(`rel` x env, 1e-3), with their control: element 0 moved by
//! env[0] x 0.1 + 1.0, past the bar at any envelope.
def env_bar(rel : float; env : array<float>) : Bar => Bar(kind = BarKind.envelope, rel = rel, abs_floor = 1e-3, poison = env[0] * 0.1 + 1.0, poison_at = 0)

//! `bar` with the q8 GEMV cells' control: element 0 moved by |want[0]| x 0.1 + 1.0, past a relative bar at any magnitude
def lead_poisoned(bar : Bar; want : array<float>) : Bar {
    var b = bar
    b.poison = abs(want[0]) * 0.1 + 1.0
    b.poison_scaled = false
    b.poison_of_max = false
    b.poison_at = 0
    return b
}

//! `check` over the first length(want) floats of `buf`: a cell's readback and its compare in one
def check_buf(t : T?; tag : string; buf : MetalBuffer?; want : array<auto(WT)>; bar : Bar; shape : Shape) : int {
    var inscope got <- buf_download(buf, length(want), type<float>)
    return check(t, tag, got, want, bar, shape)
}

//! `check_buf` on an `envelope` or `per_element` bar
def check_buf(t : T?; tag : string; buf : MetalBuffer?; want : array<auto(WT)>; bar : Bar; shape : Shape; env : array<auto(ET)>) : int {
    var inscope got <- buf_download(buf, length(want), type<float>)
    return check(t, tag, got, want, bar, shape, env)
}

//! `check_buf` over a buffer of halves, each widened to float
def check_buf_h(t : T?; tag : string; buf : MetalBuffer?; want : array<auto(WT)>; bar : Bar; shape : Shape) : int {
    var inscope halves <- buf_download(buf, length(want), type<float16>)
    var inscope got <- [for (h in halves); float(h)]
    return check(t, tag, got, want, bar, shape)
}

//! `check_buf_h` on an `envelope` or `per_element` bar
def check_buf_h(t : T?; tag : string; buf : MetalBuffer?; want : array<auto(WT)>; bar : Bar; shape : Shape; env : array<auto(ET)>) : int {
    var inscope halves <- buf_download(buf, length(want), type<float16>)
    var inscope got <- [for (h in halves); float(h)]
    return check(t, tag, got, want, bar, shape, env)
}

//! The HX twin's check: `bh`'s `n` halves bit-equal to the RNE narrow of what the float panel `bf` holds over its `live`
//! prefix, and the -1000 sentinel past it.
def check_hx_narrow(t : T?; tag : string; bh, bf : MetalBuffer?; n, live : int) : int {
    var inscope fgpu <- buf_download(bf, n, type<float>)
    var inscope wanth <- [for (k in range(n)); float(k < live ? float16(fgpu[k]) : float16(-1000.0))]
    return check_buf_h(t, tag, bh, wanth, BAR_EXACT, FLAT_SHAPE)
}

//! `check_exact` over the first length(want) elements of `buf`, read as `want`'s type
def check_buf_exact(t : T?; tag : string; buf : MetalBuffer?; want : array<auto(TT)>) : int {
    var inscope got <- buf_download(buf, length(want), type<TT>)
    return check_exact(t, tag, got, want)
}

//! the stem a pipeline-stamp macro reads - `<Class>_<kernel>_msl`, or a ternary of stems - with each leaf spelled by `leaf`
def private stamp_pick(prog : ProgramPtr; name : string; stem : ExpressionPtr; leaf : block<(src : string) : ExpressionPtr>) : ExpressionPtr {
    if (stem is ExprVar) {
        let src = "{(stem as ExprVar).name}"
        if (src |> ends_with("_msl")) {
            return <- invoke(leaf, src)
        }
    } elif (stem is ExprOp3) {
        let op3 = stem as ExprOp3
        var l <- stamp_pick(prog, name, op3.left, leaf)
        var r <- stamp_pick(prog, name, op3.right, leaf)
        return null if (l == null || r == null)
        return <- qmacro($e(op3.subexpr) ? $e(l) : $e(r))
    }
    macro_error(prog, stem.at, "{name}: '{describe(stem)}' is not a kernel source global (<Class>_<kernel>_msl) or a ternary of them")
    return null
}

[call_macro(name = "pso_stamp")]
class private PsoStampMacro : AstCallMacro {
    //! ``pso_stamp(dev, <Class>_<kernel>_msl, err)`` - a gate's pipeline from ONE kernel stem on its own device, the
    //! engine's ``compile_stamp`` rule; a ternary of stems compiles the one it picks.
    def override visit(prog : ProgramPtr; mod : Module?; var expr : ExprCallMacro?) : ExpressionPtr {
        if (length(expr.arguments) != 3) {
            macro_error(prog, expr.at, "pso_stamp expects (dev, <Class>_<kernel>_msl, err), got {describe(expr)}")
            return null
        }
        let dev = expr.arguments[0]
        let err = expr.arguments[2]
        return <- stamp_pick(prog, "pso_stamp", expr.arguments[1]) $(src) {
            return <- qmacro(pipeline_from_source($e(dev), $i(src), $i("{src}_entry"), $i("{src}_fastmath"), $e(err)))
        }
    }
}

[call_macro(name = "stamp_tgmem")]
class private StampTgmemMacro : AstCallMacro {
    //! ``stamp_tgmem(<Class>_<kernel>_msl)`` - the threadgroup-memory length of the stem (or the ternary of stems) a
    //! ``pso_stamp`` compiled, read off the same spelling.
    def override visit(prog : ProgramPtr; mod : Module?; var expr : ExprCallMacro?) : ExpressionPtr {
        if (length(expr.arguments) != 1) {
            macro_error(prog, expr.at, "stamp_tgmem expects (<Class>_<kernel>_msl), got {describe(expr)}")
            return null
        }
        return <- stamp_pick(prog, "stamp_tgmem", expr.arguments[0]) $(src) {
            return <- qmacro($i("{src}_tgmem"))
        }
    }
}

//! the kernel stem and the pipeline global of the class in `mod` whose [metal_dispatch] generates builder `bname`; empty
//! where no class does
def private builder_stamp(mod : Module?; bname : string) : tuple<stem : string; pso : string; tgmem : string> {
    var stem = ""
    var gl = ""
    var tgm = ""
    var has_wg = false
    for_each_structure(mod) $(st) {
        for (ann in st.annotations) {
            continue if (ann.annotation.name != "metal_dispatch")
            let names = metal_dispatch_names(ann.arguments)
            continue if (names.builder != bname)
            gl = names.pso
            tgm = find_arg(ann.arguments, "tgmem") ?as tString ?? ""
            for (field in st.fields) {
                for (fa in field.annotation) {
                    has_wg ||= fa.name == "workgroup"
                }
            }
            var kern = find_arg(ann.arguments, "kernel") ?as tString ?? ""
            if (empty(kern) && (find_arg(ann.arguments, "stamp") ?as tString ?? "") |> starts_with("moe_mm_split")) {
                kern = "metal_moe_mulmm*"   //! the split family's classes carry sibling kernels; the lens picks its own by this prefix
            }
            let prefix = kern |> ends_with("*")
            let want = "{st.name}`{prefix ? chop(kern, 0, length(kern) - 1) : kern}"
            for_each_function(mod, "") $(fn) {
                if (empty(stem) && !empty(fn.arguments) && fn.arguments[0]._type.structType == st) {
                    let mname = "{fn.name}"
                    if (empty(kern) || (prefix ? mname |> starts_with(want) : mname == want)) {
                        for (fa in fn.annotations) {
                            if (fa.annotation.name == "metal_kernel") {
                                stem = msl_global_of(fn)
                            }
                        }
                    }
                }
            }
        }
    }
    return (stem = stem, pso = gl, tgmem = empty(tgm) && has_wg && !empty(stem) ? "{stem}_tgmem" : tgm)   //! the lens's rule: a stamp with @workgroup state binds its kernel stem's global
}

[call_macro(name = "stamp_of")]
class private StampOfMacro : AstCallMacro {
    //! ``stamp_of(@@<enc_* builder>)`` - a gate's stamp from the ONE generated builder: ``make`` compiling, on a gate's
    //! device, the kernel source of the class the builder dispatches (``stamp_pso``), ``seat`` swapping a pipeline into the
    //! global the builder reads (the old one back), ``enc`` the builder - so a gate runs its own pipeline through the
    //! production binds and grid (``stamp_run``), and a builder never pairs with another class's kernel.
    def override visit(prog : ProgramPtr; mod : Module?; var expr : ExprCallMacro?) : ExpressionPtr {
        if (length(expr.arguments) != 1 || !(expr.arguments[0] is ExprAddr)) {
            macro_error(prog, expr.at, "stamp_of expects (@@<enc_* builder>), got {describe(expr)}")
            return null
        }
        let fad = expr.arguments[0] as ExprAddr
        let bfn = fad.func
        if (bfn == null) {
            macro_error(prog, expr.at, "stamp_of: '{describe(fad)}' names no function")
            return null
        }
        let (stem, gl, _) = builder_stamp(bfn._module, "{bfn.name}")
        if (empty(stem) || empty(gl)) {
            macro_error(prog, expr.at, "stamp_of: '{bfn.name}' is not a [metal_dispatch] builder (kernel stem '{stem}', pipeline global '{gl}')")
            return null
        }
        return <- qmacro((make = @@(dev : MetalDevice?; var err : string&) : MetalComputePipeline? {
                return pipeline_from_source(dev, $i(stem), $i("{stem}_entry"), $i("{stem}_fastmath"), err)
            },
            seat = @@(var p : MetalComputePipeline?) : MetalComputePipeline? {
                var was = $i(gl)
                $i(gl) = p
                return was
            },
            enc = $e(fad)))
    }
}

[call_macro(name = "kernel_of")]
class private KernelOfMacro : AstCallMacro {
    //! ``kernel_of(@@<enc_* builder>)`` - the kernel of the class that generated the builder, for a gate whose builder reads
    //! a pipeline global private to its driver: ``make`` compiling the class's kernel source on a gate's device
    //! (``keep_stamp``), ``tgmem`` its threadgroup-memory length (0 without @workgroup state), ``enc`` the builder's
    //! ``_pso`` form, which encodes the same binds and grid through the pipeline it is handed.
    def override visit(prog : ProgramPtr; mod : Module?; var expr : ExprCallMacro?) : ExpressionPtr {
        if (length(expr.arguments) != 1 || !(expr.arguments[0] is ExprAddr)) {
            macro_error(prog, expr.at, "kernel_of expects (@@<enc_* builder>), got {describe(expr)}")
            return null
        }
        let bfn = (expr.arguments[0] as ExprAddr).func
        if (bfn == null) {
            macro_error(prog, expr.at, "kernel_of: '{describe(expr.arguments[0])}' names no function")
            return null
        }
        let (stem, _, tgm) = builder_stamp(bfn._module, "{bfn.name}")
        if (empty(stem)) {
            macro_error(prog, expr.at, "kernel_of: '{bfn.name}' is not a [metal_dispatch] builder")
            return null
        }
        var tgmem <- empty(tgm) ? qmacro(0ul) : qmacro($i(tgm))
        var enc = new ExprAddr(at = expr.at, target := "{bfn.name}_pso")
        return <- qmacro((make = @@(dev : MetalDevice?; var err : string&) : MetalComputePipeline? {
                return pipeline_from_source(dev, $i(stem), $i("{stem}_entry"), $i("{stem}_fastmath"), err)
            },
            tgmem = $e(tgmem),
            enc = $e(enc)))
    }
}

//! The envelope count over `buf`'s floats, |kernel - reference| outside max(`rel` x env, 1e-3), the first six logged under
//! `tag` - the kq race bench's verdict, which holds no cell; a cell holds `check_buf` at `env_bar`.
def buf_mismatch_env(buf; want, env : array<float>; rel : float; tag : string) : int {
    var inscope got <- buf_download(buf, length(want), type<float>)
    var bad = 0
    for (g, w, e, k in got, want, env, count()) {
        if (!(abs(g - w) <= max(rel * e, 1e-3))) {
            if (bad < 6) {
                to_log(LOG_ERROR, "  {tag} mismatch [{k}] gpu {g} cpu {w} env {e}\n")
            }
            bad++
        }
    }
    return bad
}

// ===== synthetic K-quant split planes (the metal layout) + the reference row dequant =====

def kq_hash_byte(i : int) : uint8 {
    var h = uint(i) * 2654435761u
    h ^= h >> 15u
    h *= 2246822519u
    return uint8((h >> 8u) & 0xFFu)
}

let KQ_SAFE_SCALES = fixed_array(0.25, 0.5, 1.0, 0.125, 2.0, 0.75, 1.5, 0.375)

// quant plane = hash bytes (any bit pattern is valid); scale plane K4/K5 = [d f16][dmin f16]
// [12 packed 6-bit bytes] per superblock, K6 = [16B sub-scales] x nsb + f16 d tail, IQ4_XS (44) =
// the same split form with [8 signed sub-scales in -31..31][8 zero] strips, Q3_K (3) = the k6 form over 96B quants
def kq_fill_planes(fmt, nsb : int; var kq : array<uint8>; var ks : array<uint8>) {
    let qsb = int(kq_qsb(fmt))
    kq |> resize(nsb * qsb)
    for (i in range(nsb * qsb)) {
        kq[i] = kq_hash_byte(i * 3 + fmt)
    }
    if (fmt == 2) {
        // k2: the metal split form - [16 pair-byte strips x nsb][4B d+dmin tail x nsb]
        ks |> resize(nsb * 20)
        for (i in range(nsb * 16)) {
            ks[i] = kq_hash_byte(i * 7 + 13)
        }
        for (b in range(nsb)) {
            let dbits = f32_to_f16(KQ_SAFE_SCALES[b % 8])
            let mbits = f32_to_f16(KQ_SAFE_SCALES[(b + 3) % 8])
            ks[nsb * 16 + b * 4] = uint8(dbits & 0xFFu)
            ks[nsb * 16 + b * 4 + 1] = uint8(dbits >> 8u)
            ks[nsb * 16 + b * 4 + 2] = uint8(mbits & 0xFFu)
            ks[nsb * 16 + b * 4 + 3] = uint8(mbits >> 8u)
        }
    } elif (fmt == 45 || fmt == 40) {
        // iq4nl and q40: 16B/sb = 8 per-32-block f16 d's, verbatim (no strips)
        ks |> resize(nsb * 16)
        for (b in range(nsb * 8)) {
            let dbits = f32_to_f16(KQ_SAFE_SCALES[(b + (b / 8) * 3) % 8])
            ks[b * 2] = uint8(dbits & 0xFFu)
            ks[b * 2 + 1] = uint8(dbits >> 8u)
        }
    } elif (fmt == 6 || fmt == 44 || fmt == 3 || fmt == 33 || fmt == 34 || fmt == 23 || fmt == 24 || fmt == 25) {
        ks |> resize(nsb * 18)
        for (i in range(nsb * 16)) {
            ks[i] = fmt == 6 || fmt == 3 || fmt == 23 || fmt == 24 || fmt == 25 ? kq_hash_byte(i * 7 + 13) : uint8(i % 16 < 8 ? int(kq_hash_byte(i * 7 + 13)) % 63 - 31 : 0)   // 44/33: signed strip bytes + zero pad; 23: 16 unsigned strips
        }
        for (b in range(nsb)) {
            let dbits = f32_to_f16(KQ_SAFE_SCALES[b % 8])
            ks[nsb * 16 + b * 2] = uint8(dbits & 0xFFu)
            ks[nsb * 16 + b * 2 + 1] = uint8(dbits >> 8u)
        }
    } else {
        ks |> resize(nsb * 16)
        for (b in range(nsb)) {
            let dbits = f32_to_f16(KQ_SAFE_SCALES[b % 8])
            let mbits = f32_to_f16(KQ_SAFE_SCALES[(b + 3) % 8])
            ks[b * 16] = uint8(dbits & 0xFFu)
            ks[b * 16 + 1] = uint8(dbits >> 8u)
            ks[b * 16 + 2] = uint8(mbits & 0xFFu)
            ks[b * 16 + 3] = uint8(mbits >> 8u)
            for (i in range(12)) {
                ks[b * 16 + 4 + i] = kq_hash_byte(b * 31 + i * 5 + 7)
            }
        }
    }
}

// ===== q8 (34B-block) blob fixture =====

// one 34B-block q8 blob (nrows x nkb blocks) + its exact dequant: pow2 scales and small-int
// quants keep every staged f16/f32 value exact, leaving only accumulation order to the envelope
//! `span`: the quants' value count - 16 stays exact under f16 staging, 256 covers the whole int8 range
def q8_blob_fill(nrows, nkb : int; var blob : array<uint8>; var wdeq : array<float>; span : int = 16) {
    let lut = fixed_array(0.25, 0.5, 1.0, 2.0)
    blob |> resize(nrows * nkb * 34)
    wdeq |> resize(nrows * nkb * 32)
    for (b in range(nrows * nkb)) {
        let d = lut[(b * 7 + 1) % 4]
        let dbits = f32_to_f16(d)
        blob[b * 34] = uint8(dbits & 0xFFu)
        blob[b * 34 + 1] = uint8(dbits >> 8u)
        for (c in range(32)) {
            let q = (b * 5 + c * 11) % span - span / 2
            blob[b * 34 + 2 + c] = uint8(q & 0xFF)
            wdeq[b * 32 + c] = float(q) * d
        }
    }
}

// exact-under-f16-staging X rows: small ints scaled by pow2 (the q8 gates' shared shape)
def q8_x_fill(nn : int; var xf : array<float>) {
    let lut = fixed_array(0.25, 0.5, 1.0, 2.0)
    xf |> resize(nn)
    for (i in range(nn)) {
        xf[i] = float((i * 5 + i / 17) % 16 - 8) * lut[(i / 3) % 4]
    }
}

//! the byte offset a GEMV stamp binds a split-scale plane's f16 d tail at: past the `nsb` 16-byte strip rows on the formats
//! that split their scales (k6, k3, k2, iq4xs and the grid formats), 0 on the ones whose d rides the superblock row
def kq_split_doff(fmt, nsb : int) : uint64 {
    return fmt == 6 || fmt == 44 || fmt == 3 || fmt == 33 || fmt == 34 || fmt == 2 || fmt == 23 || fmt == 24 || fmt == 25 ? uint64(nsb * 16) : 0ul
}

//! One row's reference dequant (nb superblocks from sb_base) off split-scale planes, through the engine's own plane-row dequant.
def kq_row_ref(fmt, sb_base, nb, nsb_total : int; kq, ks : array<uint8>; var wrow : array<float>) {
    let tail = fmt == 2 ? 4l : 2l   //! the engine finds the d tail from the plane's length: the fixture's plane holds nsb_total superblocks exactly
    assert(fmt == 4 || fmt == 5 || fmt == 40 || fmt == 45 || long_length(ks) == int64(nsb_total) * (16l + tail), "kq_row_ref: the scale plane holds nsb_total superblocks")
    let f = kq_fmt_of_id(fmt, "kq_row_ref")
    kq_plane_row_f32(f, kq, ks, int64(nb) * kq_elems(f), 1l, 1l, 0l, wrow, 0l, int64(sb_base), true)
}
