options gen2
options indenting = 4

// The behavioral gate: the same kernel method runs on the CPU - a driver loop sets the builtin
// IDs per thread and calls it on a class instance whose members are plain arrays - and on the
// GPU through the emitted text; ints compare bit-exact, floats under the fastmath tolerance.
// Kernels whose semantics live in the warp (reductions, shared-memory hand-offs, atomics)
// compare against hand-computed expectations instead. Every GPU arm feints where no device is.

require dastest/testing_boost public
require _cuda_common
require daslib/fio
require daslib/math_bits   // uint_bits_to_float - the convert sweep's NaN input
require math

let N = 256
let TPB = 64u

struct Bufs3 {
    a : uint64
    b : uint64
    c : uint64
}

struct Bufs4 {
    a : uint64
    b : uint64
    c : uint64
    d : uint64
}

struct AxpyParams {
    o : uint64
    y : uint64
    ka : AxpyArgs
}

//! launches `entry` of `text` with `params` over `threads` at `tpb` a block and the kernel's `_smem` bytes; false (with
//! `err`) on any failure, the module released
def private launch(cc : CudaContext?; text, name, entry : string; var params : auto(PA); threads : uint; var err : string&; smem : uint64 = 0ul; tpb : uint = TPB) : bool {
    var mod : CudaModule?
    let fn = function_from_source(cc, text, name, entry, mod, err, uint(smem), cache_dir = path_join(test_temp_dir(), "cubin"))
    return false if (fn == null)
    let ok = run_compute_1d(fn, params, threads, tpb, err, uint(smem))
    cuda_release(mod)
    return ok
}

def private mismatch_exact(got : array<auto(TT)>; want : array<TT>) : int {
    var bad = 0
    for (g, w in got, want) {
        if (g != w) {
            bad++
        }
    }
    return bad
}

def private mismatch_approx(got : array<float>; want : array<float>) : int {
    var bad = 0
    for (g, w in got, want) {
        if (!fapprox(g, w)) {
            bad++
        }
    }
    return bad
}

def private mismatch_approx4(got : array<float4>; want : array<float4>) : int {
    var bad = 0
    for (g, w in got, want) {
        if (!(fapprox(g.x, w.x) && fapprox(g.y, w.y) && fapprox(g.z, w.z) && fapprox(g.w, w.w))) {
            bad++
        }
    }
    return bad
}

def private gpu_ready(cc : CudaContext?) : bool {
    if (cc == null || !cuda_available() || !cuda_nvrtc_available()) {
        feint("no CUDA device with NVRTC on this box; GPU compare skipped\n")
        return false
    }
    return true
}

[test]
def test_arith_gpu_vs_cpu(t : T?) {
    t |> run("uint, int and float arithmetic: GPU == CPU-reference") <| @(t : T?) {
        var uo = new UArith()
        var io = new IArith()
        var fo = new FArith()
        uo.a |> resize(N)
        uo.b |> resize(N)
        uo.c |> resize(N)
        uo.d |> resize(N)
        io.a |> resize(N)
        io.b |> resize(N)
        io.c |> resize(N)
        io.d |> resize(N)
        fo.a |> resize(N)
        fo.b |> resize(N)
        fo.c |> resize(N)
        for (k in range(N)) {
            uo.a[k] = uint(k * 7 + 3)
            uo.b[k] = uint(k % 13)
            io.a[k] = k * 5 - 300
            io.b[k] = (k % 17) - 8
            fo.a[k] = float(k) * 0.37 - 20.0
            fo.b[k] = float(k % 11) * 0.5 + 0.25
        }
        for (k in range(N)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            uo->uarith()
            io->iarith()
            fo->farith()
        }
        t |> equal(uo.c[0], 4u)
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            var err : string
            var up = Bufs4(a = buf_upload(uo.a), b = buf_upload(uo.b), c = buf_fill(N, 0u), d = buf_fill(N, 0u))
            t |> success(launch(cc, uarith_cuda, "uarith", uarith_cuda_entry, up, uint(N), err), "uarith launch: {err}")
            t |> equal(mismatch_exact(buf_download(up.c, N, type<uint>), uo.c) + mismatch_exact(buf_download(up.d, N, type<uint>), uo.d), 0, "uarith")
            var ip = Bufs4(a = buf_upload(io.a), b = buf_upload(io.b), c = buf_fill(N, 0), d = buf_fill(N, 0))
            t |> success(launch(cc, iarith_cuda, "iarith", iarith_cuda_entry, ip, uint(N), err), "iarith launch: {err}")
            t |> equal(mismatch_exact(buf_download(ip.c, N, type<int>), io.c) + mismatch_exact(buf_download(ip.d, N, type<int>), io.d), 0, "iarith")
            var fp = Bufs3(a = buf_upload(fo.a), b = buf_upload(fo.b), c = buf_fill(N, 0.0))
            t |> success(launch(cc, farith_cuda, "farith", farith_cuda_entry, fp, uint(N), err), "farith launch: {err}")
            t |> equal(mismatch_approx(buf_download(fp.c, N, type<float>), fo.c), 0, "farith")
            for (p in [up.a, up.b, up.c, up.d, ip.a, ip.b, ip.c, ip.d, fp.a, fp.b, fp.c]) {
                cuda_mem_free(p)
            }
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
        unsafe {
            delete uo
            delete io
            delete fo
        }
    }
}

[test]
def test_precise_gpu_vs_cpu(t : T?) {
    t |> run("a precise kernel's f32 sums, products and quotients: GPU == CPU-reference bit for bit") <| @(t : T?) {
        var o = new Precise()
        o.a |> resize(N)
        o.b |> resize(N)
        o.c |> resize(N)
        for (k in range(N)) {
            o.a[k] = float(k) * 0.37 - 20.0
            o.b[k] = float(k % 11) * 0.5 + 0.25
        }
        for (k in range(N)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            o->precise()
        }
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            var err : string
            var p = Bufs3(a = buf_upload(o.a), b = buf_upload(o.b), c = buf_fill(N, 0.0))
            t |> success(launch(cc, precise_cuda, "precise", precise_cuda_entry, p, uint(N), err), "precise launch: {err}")
            t |> equal(mismatch_exact(buf_download(p.c, N, type<float>), o.c), 0, "precise: every stored float is the CPU's")
            for (q in [p.a, p.b, p.c]) {
                cuda_mem_free(q)
            }
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
        unsafe {
            delete o
        }
    }
}

[test]
def test_axpy_gpu_vs_cpu(t : T?) {
    t |> run("the args struct and a method: GPU == CPU-reference") <| @(t : T?) {
        var o = new Axpy()
        o.o |> resize(N)
        o.y |> resize(N)
        o.ka = AxpyArgs(nelem = uint(N - 10), scale = 1.5, zero = 0u)
        for (k in range(N)) {
            o.o[k] = float(k)
            o.y[k] = 2.0
        }
        var o_in := o.o
        for (k in range(N)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            o->run()
        }
        t |> equal(o.o[0], 3.0)
        t |> equal(o.o[N - 1], float(N - 1), "past nelem the row is untouched")
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            var err : string
            var p = AxpyParams(o = buf_upload(o_in), y = buf_upload(o.y), ka = o.ka)
            t |> success(launch(cc, axpy_cuda, "axpy", axpy_cuda_entry, p, uint(N), err), "axpy launch: {err}")
            t |> equal(mismatch_approx(buf_download(p.o, N, type<float>), o.o), 0, "axpy")
            cuda_mem_free(p.o)
            cuda_mem_free(p.y)
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
        unsafe {
            delete o
        }
    }
}

struct VecParams {
    fa : uint64
    fb : uint64
    fc : uint64
    ia : uint64
    ic : uint64
    fl : uint64
    f2 : uint64
}

[test]
def test_vec_gpu_vs_cpu(t : T?) {
    t |> run("vector arithmetic, swizzles, converts: GPU == CPU-reference") <| @(t : T?) {
        var o = new VecArith()
        o.fa |> resize(N)
        o.fb |> resize(N)
        o.fc |> resize(N)
        o.ia |> resize(N)
        o.ic |> resize(N)
        o.fl |> resize(N)
        o.f2 |> resize(N)
        for (k in range(N)) {
            let f = float(k)
            o.fa[k] = float4(f * 0.5, f * 0.25 + 1.0, 3.0 - f * 0.125, f)
            o.fb[k] = k % 7 == 0 ? o.fa[k] : float4(1.0 + f * 0.01, 2.0, 0.5, f * 0.75 + 2.0)
            o.ia[k] = int2(k - 100, (k * 3) % 29 - 14)
        }
        for (k in range(N)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            o->vecarith()
        }
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            var err : string
            var p = VecParams(fa = buf_upload(o.fa), fb = buf_upload(o.fb), fc = buf_fill(N, float4(0.0)),
                ia = buf_upload(o.ia), ic = buf_fill(N, int2(0)), fl = buf_fill(N, 0.0), f2 = buf_fill(N, float2(0.0)))
            t |> success(launch(cc, vecarith_cuda, "vecarith", vecarith_cuda_entry, p, uint(N), err), "vecarith launch: {err}")
            t |> equal(mismatch_approx4(buf_download(p.fc, N, type<float4>), o.fc), 0, "fc")
            t |> equal(mismatch_exact(buf_download(p.ic, N, type<int2>), o.ic), 0, "ic")
            t |> equal(mismatch_approx(buf_download(p.fl, N, type<float>), o.fl), 0, "fl")
            let g2 <- buf_download(p.f2, N, type<float2>)
            var bad2 = 0
            for (g, w in g2, o.f2) {
                if (!(fapprox(g.x, w.x) && fapprox(g.y, w.y))) {
                    bad2++
                }
            }
            t |> equal(bad2, 0, "f2")
            for (q in [p.fa, p.fb, p.fc, p.ia, p.ic, p.fl, p.f2]) {
                cuda_mem_free(q)
            }
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
        unsafe {
            delete o
        }
    }
}

struct CtlParams {
    a : uint64
    b : uint64
    p : uint64
    q : uint64
    f : uint64
    g : uint64
    out1 : uint64
    out2 : uint64
    out3 : uint64
}

[test]
def test_control_gpu_vs_cpu(t : T?) {
    t |> run("control flow: GPU == CPU-reference") <| @(t : T?) {
        var o = new Control()
        o.a |> resize(N)
        o.b |> resize(N)
        o.out1 |> resize(N)
        o.p |> resize(N)
        o.q |> resize(N)
        o.out2 |> resize(N)
        o.f |> resize(N)
        o.g |> resize(N)
        o.out3 |> resize(N)
        for (k in range(N)) {
            o.a[k] = uint(k * 3 % 23)
            o.b[k] = uint(k % 9)
            o.p[k] = k - 128
            o.q[k] = (k * 7) % 31 - 15
            o.f[k] = float(k) * 0.3 - 10.0
            o.g[k] = float(k % 5) * 2.0 - 4.0
        }
        for (k in range(N)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            o->control()
        }
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            var err : string
            var p = CtlParams(a = buf_upload(o.a), b = buf_upload(o.b), p = buf_upload(o.p), q = buf_upload(o.q), f = buf_upload(o.f), g = buf_upload(o.g),
                out1 = buf_fill(N, 0u), out2 = buf_fill(N, 0), out3 = buf_fill(N, 0.0))
            t |> success(launch(cc, control_cuda, "control", control_cuda_entry, p, uint(N), err), "control launch: {err}")
            t |> equal(mismatch_exact(buf_download(p.out1, N, type<uint>), o.out1), 0, "out1")
            t |> equal(mismatch_exact(buf_download(p.out2, N, type<int>), o.out2), 0, "out2")
            t |> equal(mismatch_approx(buf_download(p.out3, N, type<float>), o.out3), 0, "out3")
            for (q in [p.a, p.b, p.p, p.q, p.f, p.g, p.out1, p.out2, p.out3]) {
                cuda_mem_free(q)
            }
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
        unsafe {
            delete o
        }
    }
}

struct MathParams {
    a : uint64
    b : uint64
    c : uint64
    ia : uint64
    ic : uint64
    va : uint64
    vc : uint64
    ua : uint64
    uc : uint64
}

[test]
def test_math_gpu_vs_cpu(t : T?) {
    t |> run("the math whitelist: GPU ~= CPU-reference under fastmath") <| @(t : T?) {
        var o = new MathCalls()
        o.a |> resize(N)
        o.b |> resize(N)
        o.c |> resize(N)
        o.ia |> resize(N)
        o.ic |> resize(N)
        o.va |> resize(N)
        o.vc |> resize(N)
        o.ua |> resize(N)
        o.uc |> resize(N)
        for (k in range(N)) {
            o.a[k] = float(k) * 0.05 - 3.0
            o.b[k] = float(k % 13) * 0.3 - 1.0
            o.ia[k] = k - 100
            o.va[k] = float4(float(k) * 0.01, -1.0 + float(k % 7), 0.5, 2.0 - float(k % 3))
            o.ua[k] = uint(k * 977)
        }
        for (k in range(N)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            o->mathcalls()
        }
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            var err : string
            var p = MathParams(a = buf_upload(o.a), b = buf_upload(o.b), c = buf_fill(N, 0.0), ia = buf_upload(o.ia), ic = buf_fill(N, 0),
                va = buf_upload(o.va), vc = buf_fill(N, float4(0.0)), ua = buf_upload(o.ua), uc = buf_fill(N, 0u))
            t |> success(launch(cc, mathcalls_cuda, "mathcalls", mathcalls_cuda_entry, p, uint(N), err), "mathcalls launch: {err}")
            let gc <- buf_download(p.c, N, type<float>)
            var bad = 0
            for (g, w in gc, o.c) {
                let tol = 1e-3 * max(1.0, abs(w))
                if (abs(g - w) > tol) {
                    bad++
                }
            }
            t |> equal(bad, 0, "c within 1e-3 relative (fastmath transcendentals)")
            t |> equal(mismatch_exact(buf_download(p.ic, N, type<int>), o.ic), 0, "ic")
            t |> equal(mismatch_approx4(buf_download(p.vc, N, type<float4>), o.vc), 0, "vc")
            t |> equal(mismatch_exact(buf_download(p.uc, N, type<uint>), o.uc), 0, "uc")
            for (q in [p.a, p.b, p.c, p.ia, p.ic, p.va, p.vc, p.ua, p.uc]) {
                cuda_mem_free(q)
            }
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
        unsafe {
            delete o
        }
    }
}

struct HalfParams {
    a : uint64
    b : uint64
    c : uint64
    f : uint64
    va : uint64
    vf : uint64
}

[test]
def test_half_gpu_vs_cpu(t : T?) {
    t |> run("the half family: GPU == CPU-reference") <| @(t : T?) {
        var o = new HalfArith()
        o.a |> resize(N)
        o.b |> resize(N)
        o.c |> resize(N)
        o.f |> resize(N)
        o.va |> resize(N)
        o.vf |> resize(N)
        for (k in range(N)) {
            o.a[k] = float16(float(k) * 0.125 - 8.0)
            o.b[k] = float16(float(k % 9) * 0.5 + 0.25)
            o.va[k] = half4(float4(float(k) * 0.25, 1.5, -2.0, float(k % 4)))
        }
        var va_in := o.va
        for (k in range(N)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            o->halfarith()
        }
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            var err : string
            var p = HalfParams(a = buf_upload(o.a), b = buf_upload(o.b), c = buf_fill(N, float16(0.0)), f = buf_fill(N, 0.0),
                va = buf_upload(va_in), vf = buf_fill(N, float4(0.0)))
            t |> success(launch(cc, halfarith_cuda, "halfarith", halfarith_cuda_entry, p, uint(N), err), "halfarith launch: {err}")
            let gc <- buf_download(p.c, N, type<float16>)
            var bad = 0
            for (g, w in gc, o.c) {
                if (!fapprox(float(g), float(w))) {
                    bad++
                }
            }
            t |> equal(bad, 0, "c")
            t |> equal(mismatch_approx(buf_download(p.f, N, type<float>), o.f), 0, "f")
            t |> equal(mismatch_approx4(buf_download(p.vf, N, type<float4>), o.vf), 0, "vf")
            let gva <- buf_download(p.va, N, type<half4>)
            var bad4 = 0
            for (g, w in gva, o.va) {
                let gf = float4(g)
                let wf = float4(w)
                if (!(fapprox(gf.x, wf.x) && fapprox(gf.y, wf.y) && fapprox(gf.z, wf.z) && fapprox(gf.w, wf.w))) {
                    bad4++
                }
            }
            t |> equal(bad4, 0, "va")
            for (q in [p.a, p.b, p.c, p.f, p.va, p.vf]) {
                cuda_mem_free(q)
            }
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
        unsafe {
            delete o
        }
    }
}

struct ReduceParams {
    x : uint64
    o : uint64
    oi : uint64
    ka : ReduceArgs
}

[test]
def test_reduce_gpu(t : T?) {
    t |> run("warp reductions, shared memory, the IDs: GPU == hand-computed") <| @(t : T?) {
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            let blocks = 4
            let n = blocks * 256
            var x : array<float>
            x |> resize(n)
            for (k in range(n)) {
                x[k] = float(k % 32) * 0.5
            }
            var err : string
            var p = ReduceParams(x = buf_upload(x), o = buf_fill(blocks, 0.0), oi = buf_fill(blocks, 0), ka = ReduceArgs(n = uint(n)))
            t |> success(launch(cc, reduce_cuda, "reduce", reduce_cuda_entry, p, uint(n), err, reduce_cuda_smem, 256u), "reduce launch: {err}")
            let warp_sum = 496.0
            let warp_max = 31.0
            let warp_min = 0.0
            let warp_first = 0.0
            let lane3 = 3.0
            let xor1 = 1.0
            let xor16 = 16.0
            let cluster4 = 6.0
            let any_lane = 1.0
            let all_lanes = 2.0
            let per_warp = warp_sum + warp_max + warp_min + warp_first + lane3 + xor1 + xor16 + cluster4 + any_lane + all_lanes
            let want = per_warp * 8.0 + 256.0 + float(blocks)
            let go <- buf_download(p.o, blocks, type<float>)
            let goi <- buf_download(p.oi, blocks, type<int>)
            for (bi in range(blocks)) {
                t |> success(fapprox(go[bi], want), "block {bi}: {go[bi]} == {want}")
                t |> equal(goi[bi], 8 + 32, "block {bi}: subgroup count + size")
            }
            cuda_mem_free(p.x)
            cuda_mem_free(p.o)
            cuda_mem_free(p.oi)
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
    }
}

struct DivergeParams {
    x : uint64
    o : uint64
    ka : DivergeArgs
}

[test]
def test_diverge_gpu(t : T?) {
    t |> run("a lane-pair shuffle, the first broadcast and the elect inside a guard half the warp skips: GPU == hand-computed") <| @(t : T?) {
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            let n = 128
            var x : array<float>
            x |> resize(n)
            for (k in range(n)) {
                x[k] = float(k % 32) * 0.25 + float(k / 32)
            }
            for (live in [16u, 6u]) {
                var err : string
                var p = DivergeParams(x = buf_upload(x), o = buf_fill(n, -1.0), ka = DivergeArgs(n = live))
                t |> success(launch(cc, diverge_cuda, "diverge", diverge_cuda_entry, p, uint(n), err), "diverge launch: {err}")
                var inscope go <- buf_download(p.o, n, type<float>)
                var bad = 0
                for (k in range(n)) {
                    let lane = k % 32
                    let want = lane < int(live) ? x[k] + x[k ^ 1] + x[k - lane] + (lane == 0 ? 100.0 : 0.0) : -1.0
                    if (!fapprox(go[k], want)) {
                        bad++
                    }
                }
                t |> equal(bad, 0, "live {live}: every lane reads its partner, the first active lane and the elect ({bad} of {n} off)")
                cuda_mem_free(p.x)
                cuda_mem_free(p.o)
            }
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
    }
}

struct UfParams {
    xs : uint64
    hs : uint64
    yo : uint64
}

[test]
def test_userfns_gpu(t : T?) {
    t |> run("free functions over buffers, shared memory and references: GPU == hand-computed") <| @(t : T?) {
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            var xs : array<float>
            var hs : array<float16>
            xs |> resize(N)
            hs |> resize(N)
            for (k in range(N)) {
                xs[k] = float(k)
                hs[k] = float16(0.5)
            }
            var err : string
            var p = UfParams(xs = buf_upload(xs), hs = buf_upload(hs), yo = buf_fill(N, 0.0))
            t |> success(launch(cc, userfns_cuda, "userfns", userfns_cuda_entry, p, uint(N), err, userfns_cuda_smem), "userfns launch: {err}")
            let go <- buf_download(p.yo, N, type<float>)
            var bad = 0
            for (k in range(N)) {
                let j = k ^ 1
                let want = 3.0 * float(j) + 1.0 + float(j) + 0.5 + float(k)
                if (!fapprox(go[k], want)) {
                    bad++
                }
            }
            t |> equal(bad, 0, "yo reads the partner lane's accumulator through shared memory, plus its own x through the pointers")
            cuda_mem_free(p.xs)
            cuda_mem_free(p.hs)
            cuda_mem_free(p.yo)
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
    }
}

struct Bufs5 {
    a : uint64
    b : uint64
    c : uint64
    d : uint64
    e : uint64
}

//! the uint array viewed as uint4 lanes, the way one binding serves both members on the device
def private as_uint4(src : array<uint>) : array<uint4> {
    var out : array<uint4>
    out |> resize(length(src) / 4)
    for (k in range(length(out))) {
        out[k] = uint4(src[4 * k], src[4 * k + 1], src[4 * k + 2], src[4 * k + 3])
    }
    return <- out
}

[test]
def test_alias_gpu_vs_cpu(t : T?) {
    t |> run("aliased bindings: GPU (one buffer per binding) == CPU-reference (one array per view)") <| @(t : T?) {
        var o = new Alias()
        o.wq |> resize(N)
        o.o |> resize(2 * N)
        o.o2 |> resize(N)
        o.p |> resize(4 * N)
        o.p4 |> resize(N)
        for (k in range(N)) {
            o.wq[k] = uint(k * 5 + 1)
            o.o2[k] = float2(float(k) * 0.5, 1.0 - float(k) * 0.25)
        }
        for (k in range(4 * N)) {
            o.p[k] = uint(k * 3)
        }
        o.wq4 <- as_uint4(o.wq)
        var o2_in := o.o2
        var p_in := o.p
        for (k in range(N)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            o->alias()
        }
        t |> equal(o.p4[1], uint4(13u, 17u, 21u, 25u))
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            var err : string
            var p = Bufs3(a = buf_upload(o.wq), b = buf_upload(o2_in), c = buf_upload(p_in))
            t |> success(launch(cc, alias_cuda, "alias", alias_cuda_entry, p, uint(N), err), "alias launch: {err}")
            let o_lo <- [for (k in range(N)); o.o[k]]
            t |> equal(mismatch_approx(buf_download(p.b, N, type<float>), o_lo), 0, "o through the float view")
            t |> equal(mismatch_exact(buf_download(p.c, N, type<uint4>), o.p4), 0, "p4 through the uint4 view")
            for (q in [p.a, p.b, p.c]) {
                cuda_mem_free(q)
            }
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
        unsafe {
            delete o
        }
    }
}

[test]
def test_blocks_gpu_vs_cpu(t : T?) {
    t |> run("struct elements, vector lanes by index: GPU == CPU-reference") <| @(t : T?) {
        var o = new Blocks()
        o.blk |> resize(N)
        o.meta |> resize(N)
        o.w4 |> resize(N)
        o.o |> resize(N)
        for (k in range(N)) {
            for (j in range(8)) {
                o.blk[k].qs[j] = int16(k * 3 - j * 50)
                o.meta[k].blk.qs[j] = int16(j)
            }
            o.blk[k].sc = 0.125 * float(k % 9)
            o.meta[k] = Meta(pos = uint(k), cnt = uint(k * 7), blk = o.meta[k].blk)
            o.meta[k].blk.sc = float(k) * 0.5
            o.w4[k] = uint4(uint(k), uint(k + 1), uint(k + 2), uint(k + 3))
        }
        for (k in range(N)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            o->blocks()
        }
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            var err : string
            var p = Bufs4(a = buf_upload(o.blk), b = buf_upload(o.meta), c = buf_upload(o.w4), d = buf_fill(N, 0.0))
            t |> success(launch(cc, blocks_cuda, "blocks", blocks_cuda_entry, p, uint(N), err), "blocks launch: {err}")
            t |> equal(mismatch_approx(buf_download(p.d, N, type<float>), o.o), 0, "blocks")
            for (q in [p.a, p.b, p.c, p.d]) {
                cuda_mem_free(q)
            }
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
        unsafe {
            delete o
        }
    }
}

[test]
def test_packs_gpu_vs_cpu(t : T?) {
    t |> run("the pack family: GPU == CPU-reference") <| @(t : T?) {
        var o = new Packs()
        o.f |> resize(N)
        o.u |> resize(N)
        o.o |> resize(N)
        o.ob |> resize(N)
        o.ou |> resize(N)
        for (k in range(N)) {
            o.f[k] = float2(float(k) * 0.375 - 20.0, 1.0 / float(k + 1))
            o.u[k] = uint(int64(k) * 2654435761l)
        }
        for (k in range(N)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            o->packs()
        }
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            var err : string
            var p = Bufs5(a = buf_upload(o.f), b = buf_upload(o.u), c = buf_fill(N, float2(0.0)), d = buf_fill(N, 0), e = buf_fill(N, 0u))
            t |> success(launch(cc, packs_cuda, "packs", packs_cuda_entry, p, uint(N), err), "packs launch: {err}")
            let go <- buf_download(p.c, N, type<float2>)
            var bad = 0
            for (g, w in go, o.o) {
                if (!(fapprox(g.x, w.x) && fapprox(g.y, w.y))) {
                    bad++
                }
            }
            t |> equal(bad, 0, "half2 round trip")
            t |> equal(mismatch_exact(buf_download(p.d, N, type<int>), o.ob), 0, "signed bytes")
            t |> equal(mismatch_exact(buf_download(p.e, N, type<uint>), o.ou), 0, "unsigned bytes")
            for (q in [p.a, p.b, p.c, p.d, p.e]) {
                cuda_mem_free(q)
            }
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
        unsafe {
            delete o
        }
    }
}

struct CoopParams {
    a : uint64
    b : uint64
    o : uint64
    a8 : uint64
    b8 : uint64
    o32 : uint64
    a8t : uint64
    b8r : uint64
}

[test]
def test_coop_gpu(t : T?) {
    t |> run("the KHR tiles: GPU == a CPU reference GEMM, every layout, every accumulator") <| @(t : T?) {
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            // integer operands, |AB| <= 96, |A8 B8| <= 640: every sum exact
            var a, b : array<float16>
            var a8, b8, a8t, b8r : array<int8>
            a |> resize(256)
            b |> resize(256)
            a8 |> resize(512)
            b8 |> resize(512)
            a8t |> resize(512)
            b8r |> resize(512)
            for (r in range(16)) {
                for (k in range(16)) {
                    a[r * 16 + k] = float16(float((r + k) % 5 - 2))
                    b[k * 16 + r] = float16(float((r * 3 + k) % 7 - 3))   // (k = r, n = k) at n * 16 + k
                }
            }
            for (r in range(16)) {
                for (c in range(32)) {
                    a8[r * 32 + c] = int8((r * 7 + c) % 11 - 5)
                    a8t[c * 16 + r] = a8[r * 32 + c]
                    b8[r * 32 + c] = int8((c + r * 5) % 9 - 4)   // (k = c, n = r) at n * 32 + k
                    b8r[c * 16 + r] = b8[r * 32 + c]
                }
            }
            var err : string
            var p = CoopParams(a = buf_upload(a), b = buf_upload(b), o = buf_fill(1024, 0.0), a8 = buf_upload(a8), b8 = buf_upload(b8),
                o32 = buf_fill(512, 0), a8t = buf_upload(a8t), b8r = buf_upload(b8r))
            t |> success(launch(cc, coop_cuda, "coop", coop_cuda_entry, p, 32u, err, coop_cuda_smem, 32u), "coop launch: {err}")
            let go <- buf_download(p.o, 1024, type<float>)
            let gs <- buf_download(p.o32, 512, type<int>)
            var bad = 0
            for (r in range(16)) {
                for (n in range(16)) {
                    var c = 0.0
                    var ct = 0.0
                    for (k in range(16)) {
                        c += float(a[r * 16 + k]) * float(b[n * 16 + k])
                        ct += float(a[k * 16 + r]) * float(b[k * 16 + n])   // A^T B^T
                    }
                    var s = 0
                    for (k in range(32)) {
                        s += int(a8[r * 32 + k]) * int(b8[n * 32 + k])
                    }
                    bad += go[r * 16 + n] == c ? 0 : 1
                    bad += go[256 + n * 16 + r] == ct ? 0 : 1
                    bad += go[512 + r * 16 + n] == c ? 0 : 1
                    bad += go[768 + r * 16 + n] == c ? 0 : 1
                    bad += gs[r * 16 + n] == s ? 0 : 1
                    bad += gs[256 + n * 16 + r] == s ? 0 : 1
                }
            }
            t |> equal(bad, 0, "every tile element matches the reference (f32 row, f32 column, f16 -> f32, f16 staged, s32 row, s32 column)")
            for (q in [p.a, p.b, p.o, p.a8, p.b8, p.o32, p.a8t, p.b8r]) {
                cuda_mem_free(q)
            }
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
    }
}

struct Cm2Params {
    a : uint64
    bt : uint64
    c : uint64
    o2 : uint64
    red : uint64
    e : uint64
    rs : uint64
    t : uint64
}

struct Cm2DecParams {
    wq : uint64
    sc : uint64
    bt : uint64
    c : uint64
}

struct Cm2W8Params {
    a : uint64
    bt : uint64
    red : uint64
    e : uint64
    e2 : uint64
}

//! the fixture's integer-valued A and B^T: every stage of the worked product stays exact in f16 and f32
def private cm2_inputs(var a, bt : array<float16>) {
    a |> resize(4096)
    bt |> resize(4096)
    for (r in range(64)) {
        for (k in range(64)) {
            a[r * 64 + k] = float16(float((r * 3 + k) % 7 - 3))
            bt[r * 64 + k] = float16(float((k + 2 * r) % 5 - 2))   //! (n = r, k) at n * 64 + k
        }
    }
}

def private cm2_dot(a, bt : array<float16>; r, n : int) : float {
    var s = 0.0
    for (k in range(64)) {
        s += float(a[r * 64 + k]) * float(bt[n * 64 + k])
    }
    return s
}

[test]
def test_cm2_gpu(t : T?) {
    t |> run("the workgroup tiles: GPU == a CPU reference through every form") <| @(t : T?) {
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            var a, bt : array<float16>
            cm2_inputs(a, bt)
            var err : string
            var p = Cm2Params(a = buf_upload(a), bt = buf_upload(bt), c = buf_fill(4096, -1.0), o2 = buf_fill(4096, float16(0.0)),
                red = buf_fill(8192, 0.0), e = buf_fill(4096, 0.0), rs = buf_fill(4096, 0.0), t = buf_fill(4096, 0.0))
            t |> success(launch(cc, cm2_cuda, "cm2", cm2_cuda_entry, p, 128u, err, cm2_cuda_smem, 128u), "cm2 launch: {err}")
            let gc <- buf_download(p.c, 4096, type<float>)
            let go2 <- buf_download(p.o2, 4096, type<float16>)
            let gred <- buf_download(p.red, 8192, type<float>)
            let ge <- buf_download(p.e, 4096, type<float>)
            let grs <- buf_download(p.rs, 4096, type<float>)
            let gt <- buf_download(p.t, 4096, type<float>)
            var worked_product : array<float>
            worked_product |> resize(4096)
            for (r in range(64)) {
                for (n in range(64)) {
                    let v = cm2_dot(a, bt, r, n) + float(r) - float(n)
                    worked_product[r * 64 + n] = clamp(max(v, 2.0 * v), -100.0, 100.0)
                }
            }
            var bad = 0
            for (r in range(64)) {
                var rmax = -1e30
                for (n in range(64)) {
                    rmax = max(rmax, worked_product[r * 64 + n])
                }
                for (n in range(64)) {
                    let i = r * 64 + n
                    let prod = cm2_dot(a, bt, r, n)
                    bad += gc[i] == (r < 60 ? worked_product[i] : -1.0) ? 0 : 1
                    bad += float(go2[i]) == prod ? 0 : 1
                    bad += gred[i] == rmax ? 0 : 1
                    var csum = 0.0
                    for (rr in range(64)) {
                        csum += worked_product[rr * 64 + n]
                    }
                    bad += gred[4096 + i] == csum ? 0 : 1
                    var pe = 0.0
                    var prs = 0.0
                    for (k in range(64)) {
                        pe += worked_product[r * 64 + k] * (float(bt[n * 64 + k]) + float(a[k * 64 + n]))
                        prs += worked_product[r * 64 + k]
                    }
                    bad += ge[i] == pe ? 0 : 1
                    bad += grs[i] == prs ? 0 : 1
                    bad += gt[i] == worked_product[i] * worked_product[i] ? 0 : 1
                }
            }
            t |> equal(bad, 0, "every tile element matches (pad-clamped store, f16 accumulator, row max, column sum, P B over the [n][k] and the [k][n] shared tiles, row sums, Hadamard)")
            for (q in [p.a, p.bt, p.c, p.o2, p.red, p.e, p.rs, p.t]) {
                cuda_mem_free(q)
            }
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
    }
}

[test]
def test_cm2w8_gpu(t : T?) {
    t |> run("the workgroup tiles over eight warps (column-owned accumulators): the row reduce through scratch and the block-wide convert == the CPU reference") <| @(t : T?) {
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            var a, bt : array<float16>
            cm2_inputs(a, bt)
            var err : string
            var p = Cm2W8Params(a = buf_upload(a), bt = buf_upload(bt), red = buf_fill(4096, 0.0), e = buf_fill(4096, 0.0), e2 = buf_fill(4096, 0.0))
            t |> success(launch(cc, cm2w8_cuda, "cm2w8", cm2w8_cuda_entry, p, 256u, err, cm2w8_cuda_smem, 256u), "cm2w8 launch: {err}")
            let gred <- buf_download(p.red, 4096, type<float>)
            let ge <- buf_download(p.e, 4096, type<float>)
            let ge2 <- buf_download(p.e2, 4096, type<float>)
            var bad = 0
            var bad2 = 0
            for (r in range(64)) {
                var rmax = -1e30
                for (n in range(64)) {
                    rmax = max(rmax, cm2_dot(a, bt, r, n))
                }
                for (n in range(64)) {
                    let i = r * 64 + n
                    bad += gred[i] == rmax ? 0 : 1
                    var pe = 0.0
                    var pe2 = 0.0
                    for (k in range(64)) {
                        pe += cm2_dot(a, bt, r, k) * float(bt[n * 64 + k])
                        pe2 += float(a[r * 64 + k]) * (float(a[k * 64 + n]) + 0.5 + 0.25 + float(bt[n * 64 + k]))
                    }
                    bad += ge[i] == pe ? 0 : 1
                    bad2 += ge2[i] == pe2 ? 0 : 1
                }
            }
            t |> equal(bad, 0, "every tile element matches under the k permutation (row max through the scratch, P B through the block-wide convert)")
            t |> equal(bad2, 0, "the products over the permuted shared B tiles - the [k][n] store through the transposing ldmatrix, then filled with a half, the [n][k] store filled with a quarter - and over the global view match")
            for (q in [p.a, p.bt, p.red, p.e, p.e2]) {
                cuda_mem_free(q)
            }
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
    }
}

struct IntrinParams {
    a8 : uint64
    b8 : uint64
    row_scales : uint64
    col_scales : uint64
    o : uint64
    misc : uint64
}

struct IntrinFp4Params {
    a4 : uint64
    b4 : uint64
    a_scale_words : uint64
    b_scale_words : uint64
    o : uint64
    xin : uint64
    cvt_out : uint64
}

def private e2m1_value(c : uint) : float {
    let mag = fixed_array<float>(0.0, 0.5, 1.0, 1.5, 2.0, 3.0, 4.0, 6.0)
    let v = mag[int(c & 7u)]
    return (c & 8u) != 0u ? -v : v
}

def private ue4m3_value(c : uint) : float {
    let e = int((c >> 3u) & 15u)
    let m = float(c & 7u)
    return e == 0 ? m * pow(2.0, -9.0) : pow(2.0, float(e - 7)) * (1.0 + m / 8.0)
}

//! every e2m1 tie in [-8, 7.875], the UE4M3 subnormal half-steps and mantissa midpoints, a geometric run to saturation
def private fp4_cvt_inputs() : array<float> {
    var xin <- [for (i in range(128)); float(i - 64) * 0.125]
    var inscope ue <- [for (i in range(64)); i < 16 ? float(i) * pow(2.0, -10.0) : (i < 48 ? 1.0 + float(i - 16) * 0.0625 : pow(2.0, float(i - 48)) * 1.3)]
    var inscope dyadic_col_factors <- [for (i in range(16)); 0.5 + 0.25 * float(i % 5)]
    //! the signs and the non-numbers: -0.0 carries its sign into the e2m1 nibble, a negative or NaN UE4M3 input reads as code 0
    xin[0] = -0.0
    xin[1] = uint_bits_to_float(0x7FC00000u)
    ue[62] = -0.5
    ue[63] = uint_bits_to_float(0x7FC00000u)
    xin |> push_from(ue, dyadic_col_factors)
    return <- xin
}

[test]
def test_intrin_fp4_gpu(t : T?) {
    t |> run("the FP4 tensor rung: the block-scaled e2m1 product through cp.async + ldmatrix == the CPU sum, the conversions == their CPU bodies") <| @(t : T?) {
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            var a8, b8 : array<uint8>
            var a_scale_words, b_scale_words : array<uint>
            a8 |> resize(16 * 32)
            b8 |> resize(16 * 32)
            for (i in range(16 * 32)) {
                a8[i] = uint8((i * 7 + 3) % 256)
                b8[i] = uint8((i * 13 + 5) % 256)
            }
            for (i in range(16)) {   // scale codes in [0x30, 0x40), values in [0.5, 2): every product and sum stays dyadic and short
                var wa = 0u
                var wb = 0u
                for (s in range(4)) {
                    wa |= (0x30u + uint((i * 4 + s * 3) % 16)) << uint(s * 8)
                    wb |= (0x30u + uint((i * 5 + s * 7 + 2) % 16)) << uint(s * 8)
                }
                a_scale_words |> push(wa)
                b_scale_words |> push(wb)
            }
            var inscope xin <- fp4_cvt_inputs()
            var err : string
            var p = IntrinFp4Params(a4 = buf_upload(a8), b4 = buf_upload(b8), a_scale_words = buf_upload(a_scale_words), b_scale_words = buf_upload(b_scale_words),
                o = buf_fill(256, 0.0), xin = buf_upload(xin), cvt_out = buf_fill(128, 0u))
            t |> success(launch(cc, intrin_fp4_cuda, "intrin_fp4", intrin_fp4_cuda_entry, p, 32u, err, intrin_fp4_cuda_smem, 32u), "intrin_fp4 launch: {err}")
            let go <- buf_download(p.o, 256, type<float>)
            let gcv <- buf_download(p.cvt_out, 128, type<uint>)
            var bad = 0
            var worst = 0.0
            for (r in range(16)) {
                for (n in range(16)) {
                    var s = 0.0
                    for (k in range(64)) {
                        let ac = (uint(a8[r * 32 + k / 2]) >> uint((k & 1) * 4)) & 15u
                        let bc = (uint(b8[n * 32 + k / 2]) >> uint((k & 1) * 4)) & 15u
                        let sc = ue4m3_value((a_scale_words[r] >> uint((k / 16) * 8)) & 0xFFu) * ue4m3_value((b_scale_words[n] >> uint((k / 16) * 8)) & 0xFFu)
                        s += e2m1_value(ac) * e2m1_value(bc) * sc
                    }
                    s *= xin[int(FP4_XIN_COL_FACTORS) + n]
                    let d = abs(go[r * 16 + n] - s)
                    worst = max(worst, d)
                    bad += d <= 1.0e-4 * max(1.0, abs(s)) ? 0 : 1
                }
            }
            t |> equal(bad, 0, "the block-scaled e2m1 product under its column factors matches the CPU sum (worst difference {worst})")
            var badc = 0
            for (l in range(32)) {
                badc += gcv[l] == cvt_e2m1x2(xin[2 * l], xin[2 * l + 1]) ? 0 : 1
                badc += gcv[32 + l] == cvt_e2m1x2(xin[64 + 2 * l], xin[64 + 2 * l + 1]) ? 0 : 1
                badc += gcv[int(FP4_CV_UE4M3) + l] == cvt_ue4m3(xin[int(FP4_XIN_UE4M3) + l]) ? 0 : 1
                badc += gcv[int(FP4_CV_UE4M3) + 32 + l] == cvt_ue4m3(xin[int(FP4_XIN_UE4M3) + 32 + l]) ? 0 : 1
            }
            t |> equal(badc, 0, "cvt_e2m1x2 and cvt_ue4m3 match their CPU bodies over the sweep (ties to even, saturation, the subnormals)")
            for (q in [p.a4, p.b4, p.a_scale_words, p.b_scale_words, p.o, p.xin, p.cvt_out]) {
                cuda_mem_free(q)
            }
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
    }
}

struct Cm2I8Params {
    a8 : uint64
    b8 : uint64
    rs : uint64
    cs : uint64
    c : uint64
}

[test]
def test_cm2i8_gpu(t : T?) {
    t |> run("the int8 workgroup tiles: the s32 products of two k windows scaled per row and column off shared arrays == the CPU reference") <| @(t : T?) {
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            var a8, b8 : array<int8>
            var rs, cs : array<float>
            a8 |> resize(64 * 64)
            b8 |> resize(64 * 64)
            for (i in range(64 * 64)) {
                a8[i] = int8((i * 7 + 3) % 23 - 11)
                b8[i] = int8((i * 5 + 1) % 19 - 9)
            }
            for (i in range(128)) {
                rs |> push(float(i % 3) * 0.5 + 0.25)
                cs |> push(float(i % 5) * 0.125 + 1.0)
            }
            var err : string
            var p = Cm2I8Params(a8 = buf_upload(a8), b8 = buf_upload(b8), rs = buf_upload(rs), cs = buf_upload(cs), c = buf_fill(4096, 0.0))
            t |> success(launch(cc, cm2i8_cuda, "cm2i8", cm2i8_cuda_entry, p, 128u, err, cm2i8_cuda_smem, 128u), "cm2i8 launch: {err}")
            let gc <- buf_download(p.c, 4096, type<float>)
            var bad = 0
            for (r in range(64)) {
                for (n in range(64)) {
                    var want = 0.0
                    for (w in range(2)) {
                        var s = 0
                        for (k in range(32)) {
                            s += int(a8[r * 64 + w * 32 + k]) * int(b8[n * 64 + w * 32 + k])
                        }
                        want += float(s) * rs[w * 64 + r] * cs[w * 64 + n]
                    }
                    bad += gc[r * 64 + n] == want ? 0 : 1
                }
            }
            t |> equal(bad, 0, "the two windows' int8 products, each scaled by its row and column scales, match")
            for (q in [p.a8, p.b8, p.rs, p.cs, p.c]) {
                cuda_mem_free(q)
            }
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
    }
}

[test]
def test_intrin_gpu(t : T?) {
    t |> run("the CUDA-only tier: cp.async into shared, ldmatrix fragments, the block-scaled int8 product, byte_perm, a named barrier under clock32, inline PTX == the CPU reference") <| @(t : T?) {
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            var a8, b8 : array<uint8>
            var rs, cs : array<float>
            a8 |> resize(16 * 32)
            b8 |> resize(16 * 32)
            for (i in range(16 * 32)) {
                a8[i] = uint8((i * 7 + 3) % 23 - 11)
                b8[i] = uint8((i * 5 + 1) % 19 - 9)
            }
            for (i in range(16)) {
                rs |> push(float(i % 3) * 0.5 + 0.25)
                cs |> push(float(i % 5) * 0.125 + 1.0)
            }
            var err : string
            var p = IntrinParams(a8 = buf_upload(a8), b8 = buf_upload(b8), row_scales = buf_upload(rs), col_scales = buf_upload(cs), o = buf_fill(256, 0.0), misc = buf_fill(96, 0.0))
            t |> success(launch(cc, intrin_cuda, "intrin", intrin_cuda_entry, p, 32u, err, intrin_cuda_smem, 32u), "intrin launch: {err}")
            let go <- buf_download(p.o, 256, type<float>)
            let gm <- buf_download(p.misc, 96, type<uint>)
            var bad = 0
            for (r in range(16)) {
                for (n in range(16)) {
                    var s = 0
                    for (k in range(32)) {
                        s += int(int8(a8[r * 32 + k])) * int(int8(b8[n * 32 + k]))
                    }
                    bad += go[r * 16 + n] == float(s) * rs[r] * cs[n] ? 0 : 1
                }
            }
            t |> equal(bad, 0, "the int8 product through cp.async + ldmatrix, scaled by row and column, matches")
            var badm = 0
            for (l in range(32)) {
                badm += gm[l] == byte_perm(0x03020100u + uint(l), 0x8f0e0d0cu, 0x7654u + uint(l) + uint(l & 1) * 0x8000u) ? 0 : 1
                badm += gm[32 + l] == 1u ? 0 : 1
                badm += gm[64 + l] == uint(l) + 5u ? 0 : 1
            }
            t |> equal(badm, 0, "byte_perm matches its CPU emulation, clock32 moves forward across the named barrier, the inline-PTX add returns")
            for (q in [p.a8, p.b8, p.row_scales, p.col_scales, p.o, p.misc]) {
                cuda_mem_free(q)
            }
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
    }
}

[test]
def test_cm2deck_gpu(t : T?) {
    t |> run("the decode-in-load form over two k windows, the view B tile reloaded a window at a time == the CPU reference") <| @(t : T?) {
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            var a, bt : array<float16>
            cm2_inputs(a, bt)
            var wq : array<DecBlk>
            var sc : array<float16>
            wq |> resize(64 * 16)
            sc |> resize(64)
            for (r in range(64)) {
                sc[r] = float16(float(r % 4) * 0.5 + 0.5)
                for (k in range(64)) {
                    wq[r * 16 + k / 4].q[k % 4] = int16((r + k * 5) % 9 - 4)
                }
            }
            var err : string
            var p = Cm2DecParams(wq = buf_upload(wq), sc = buf_upload(sc), bt = buf_upload(bt), c = buf_fill(4096, 0.0))
            t |> success(launch(cc, cm2deck_cuda, "cm2deck", cm2deck_cuda_entry, p, 128u, err, cm2deck_cuda_smem, 128u), "cm2deck launch: {err}")
            let gc <- buf_download(p.c, 4096, type<float>)
            var bad = 0
            for (r in range(64)) {
                for (n in range(32)) {
                    var s = 0.0
                    for (k in range(64)) {
                        s += float(sc[r]) * float(int(wq[r * 16 + k / 4].q[k % 4])) * float(bt[n * 64 + k])
                    }
                    bad += gc[r * 64 + n] == s ? 0 : 1
                }
            }
            t |> equal(bad, 0, "the two-window decoded product matches")
            for (q in [p.wq, p.sc, p.bt, p.c]) {
                cuda_mem_free(q)
            }
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
    }
}

[test]
def test_cm2raw_gpu(t : T?) {
    t |> run("the pipelined decode-in-load over two k windows: the first synchronous, the second decoded past the first product == the CPU reference") <| @(t : T?) {
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            var a, bt : array<float16>
            cm2_inputs(a, bt)
            var wq : array<RawBlk>
            var sc : array<float16>
            wq |> resize(64 * 2)
            sc |> resize(64)
            for (r in range(64)) {
                sc[r] = float16(float(r % 4) * 0.5 + 0.5)
                for (k in range(64)) {
                    wq[r * 2 + k / 32].q[k % 32] = int8((r + k * 5) % 9 - 4)
                }
            }
            var err : string
            var p = Cm2DecParams(wq = buf_upload(wq), sc = buf_upload(sc), bt = buf_upload(bt), c = buf_fill(4096, 0.0))
            t |> success(launch(cc, cm2raw_cuda, "cm2raw", cm2raw_cuda_entry, p, 128u, err, cm2raw_cuda_smem, 128u), "cm2raw launch: {err}")
            let gc <- buf_download(p.c, 4096, type<float>)
            var bad = 0
            for (r in range(64)) {
                for (n in range(32)) {
                    var s = 0.0
                    for (k in range(64)) {
                        s += float(sc[r]) * float(int(wq[r * 2 + k / 32].q[k % 32])) * float(bt[n * 64 + k])
                    }
                    bad += gc[r * 64 + n] == s ? 0 : 1
                }
            }
            t |> equal(bad, 0, "the two-window pipelined product matches")
            for (q in [p.wq, p.sc, p.bt, p.c]) {
                cuda_mem_free(q)
            }
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
    }
}

[test]
def test_cm2dec_gpu(t : T?) {
    t |> run("the decode-in-load form: GPU == a CPU reference through the block decode") <| @(t : T?) {
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            var a, bt : array<float16>
            cm2_inputs(a, bt)
            var wq : array<DecBlk>
            var sc : array<float16>
            wq |> resize(64 * 16)
            sc |> resize(64)
            for (r in range(64)) {
                sc[r] = float16(float(r % 4) * 0.5 + 0.5)
                for (k in range(64)) {
                    wq[r * 16 + k / 4].q[k % 4] = int16((r + k * 5) % 9 - 4)
                }
            }
            var err : string
            var p = Cm2DecParams(wq = buf_upload(wq), sc = buf_upload(sc), bt = buf_upload(bt), c = buf_fill(4096, 0.0))
            t |> success(launch(cc, cm2dec_cuda, "cm2dec", cm2dec_cuda_entry, p, 128u, err, cm2dec_cuda_smem, 128u), "cm2dec launch: {err}")
            let gc <- buf_download(p.c, 4096, type<float>)
            var bad = 0
            for (r in range(64)) {
                for (n in range(64)) {
                    var s = 0.0
                    for (k in range(64)) {
                        s += float(sc[r]) * float(int(wq[r * 16 + k / 4].q[k % 4])) * (n < 60 ? float(bt[n * 64 + k]) : 0.0)
                    }
                    bad += gc[r * 64 + n] == s ? 0 : 1
                }
            }
            t |> equal(bad, 0, "the decoded product matches, B's rows past the pad bound read as zero through the view's clamped gather")
            for (q in [p.wq, p.sc, p.bt, p.c]) {
                cuda_mem_free(q)
            }
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
    }
}

struct AtomParams {
    cnt : uint64
    sum : uint64
    ext : uint64
}

[test]
def test_atomics_gpu(t : T?) {
    t |> run("atomics: GPU == hand-computed") <| @(t : T?) {
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            var ext <- [ -1, 100000 ]
            var err : string
            var p = AtomParams(cnt = buf_fill(8, 0u), sum = buf_fill(1, 0.0), ext = buf_upload(ext))
            t |> success(launch(cc, atomics_cuda, "atomics", atomics_cuda_entry, p, uint(N), err), "atomics launch: {err}")
            let gc <- buf_download(p.cnt, 8, type<uint>)
            let gs <- buf_download(p.sum, 1, type<float>)
            let ge <- buf_download(p.ext, 2, type<int>)
            t |> equal(gc[0], uint(N), "N increments")
            t |> equal(gc[1], 0xFFFFFFFFu, "every bit set by or")
            t |> equal(gc[2], 0u, "and clears")
            t |> equal(gc[3], uint(N % 2), "xor parity")
            t |> success(gc[4] < uint(N), "exchange left some lane's id")
            t |> success(gc[5] < uint(N), "compare-swap left the first lane's id")
            t |> equal(gc[6], 0u, "no old value was negative")
            t |> equal(gs[0], 1.5 * float(N))
            t |> equal(ge[0], N - 1)
            t |> equal(ge[1], 0)
            cuda_mem_free(p.cnt)
            cuda_mem_free(p.sum)
            cuda_mem_free(p.ext)
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
    }
}

struct TabParams {
    idx : uint64
    o : uint64
}

struct LatParams {
    bs : uint64
    us : uint64
    b4 : uint64
    u2 : uint64
    ob : uint64
    o4 : uint64
    s2 : uint64
    pk : uint64
    od : uint64
}

struct PtrParams {
    src : uint64
    dst : uint64
}

[test]
def test_tables_lattice_ptrs_gpu_vs_cpu(t : T?) {
    t |> run("constant tables, the lattice, pointer locals: GPU == CPU-reference") <| @(t : T?) {
        var ct = new ConstTab()
        ct.idx |> resize(N)
        ct.o |> resize(N)
        var la = new Lattice()
        la.bs |> resize(N)
        la.ob |> resize(N)
        la.us |> resize(N)
        la.b4 |> resize(N)
        la.u2 |> resize(N)
        la.o4 |> resize(N)
        la.s2 |> resize(N)
        la.pk |> resize(N)
        la.od |> resize(N)
        var pt = new Ptrs()
        pt.src |> resize(2 * N)
        pt.dst |> resize(N)
        for (k in range(N)) {
            ct.idx[k] = uint(k * 5)
            la.bs[k] = int8(k - 100)
            la.us[k] = uint16(k * 200)
            la.b4[k] = byte4(int4(k - 128, 3, -3, k % 7))
            la.u2[k] = ubyte2(uint2(uint(k), 7u))
            la.pk[k] = uint(k) * 0x01010101u
            pt.src[2 * k] = float(k)
            pt.src[2 * k + 1] = 0.5
        }
        for (k in range(N)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            ct->consttab()
            la->lattice()
            pt->ptrs()
        }
        with_cuda_context() $(cc : CudaContext?) {
            return if (!gpu_ready(cc))
            var err : string
            var tp = TabParams(idx = buf_upload(ct.idx), o = buf_fill(N, 0.0))
            t |> success(launch(cc, consttab_cuda, "consttab", consttab_cuda_entry, tp, uint(N), err), "consttab launch: {err}")
            t |> equal(mismatch_approx(buf_download(tp.o, N, type<float>), ct.o), 0, "consttab")
            var lp = LatParams(bs = buf_upload(la.bs), us = buf_upload(la.us), b4 = buf_upload(la.b4), u2 = buf_upload(la.u2),
                ob = buf_fill(N, int8(0)), o4 = buf_fill(N, byte4(int4(0))), s2 = buf_fill(N, ushort2(uint2(0u))), pk = buf_upload(la.pk), od = buf_fill(N, 0))
            t |> success(launch(cc, lattice_cuda, "lattice", lattice_cuda_entry, lp, uint(N), err), "lattice launch: {err}")
            t |> equal(mismatch_exact(buf_download(lp.ob, N, type<int8>), la.ob), 0, "ob")
            t |> equal(mismatch_exact(buf_download(lp.o4, N, type<byte4>), la.o4), 0, "o4")
            t |> equal(mismatch_exact(buf_download(lp.s2, N, type<ushort2>), la.s2), 0, "s2")
            t |> equal(mismatch_exact(buf_download(lp.od, N, type<int>), la.od), 0, "od")
            var pp = PtrParams(src = buf_upload(pt.src), dst = buf_fill(N, 0.0))
            t |> success(launch(cc, ptrs_cuda, "ptrs", ptrs_cuda_entry, pp, uint(N), err), "ptrs launch: {err}")
            t |> equal(mismatch_approx(buf_download(pp.dst, N, type<float>), pt.dst), 0, "ptrs")
            for (q in [tp.idx, tp.o, lp.bs, lp.us, lp.b4, lp.u2, lp.ob, lp.o4, lp.s2, lp.pk, lp.od, pp.src, pp.dst]) {
                cuda_mem_free(q)
            }
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
        unsafe {
            delete ct
            delete la
            delete pt
        }
    }
}
