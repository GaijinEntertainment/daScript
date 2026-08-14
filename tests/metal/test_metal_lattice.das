options gen2
options indenting = 4

// Real-GPU behavioral gate for the Phase-4 16/8-bit lattice. Three kernels, three claims:
// (1) halfarith [fastmath=false]: das CPU fp16 (per-op promote-to-f32-round-back == correctly
//     rounded fp16 per op) must match Metal's native half BIT-EXACTLY, one op per store — Metal
//     keeps fp-contraction (fma fusing) and mixed-precision freedom across multi-op chains even
//     in safe math mode (measured on M1 Max: a 4-op chain drifts 1-64 ULP), so each op's result
//     goes straight to a buffer, which pins every rounding point on both sides;
// (2) latconv [default fastmath]: int-lattice widen/narrow/saturate round-trips and fp16<->f32
//     converts are exact regardless of fastmath (conversions + single ops are deterministic);
// (3) packed3 [default fastmath]: the packed_T3 stride story on real hardware — das-packed
//     byte3 (3B) / half3 (6B) / float3 / int3 (12B) buffers round-trip bit-exactly (float ops
//     chosen exact: *2.0 is a power-of-two scale, so even an fma contraction cannot diverge).
// The CPU-reference oracle is the same method body replayed with gl_GlobalInvocationID set.

require dastest/testing_boost public
require metal/msl_shader
require _metal_common  // nolint:STYLE030 — used only inside the Apple static_if half; off-Apple lint sees the branch compiled out

let N = 256
let TPG = 64u

class HalfArith {
    @ssbo @binding = 0 ha : array<half4>
    @ssbo @binding = 1 hb : array<half4>
    @ssbo @binding = 2 o_add : array<half4>
    @ssbo @binding = 3 o_sub : array<half4>
    @ssbo @binding = 4 o_mul : array<half4>
    @ssbo @binding = 5 o_div : array<half4>
    @ssbo @binding = 6 hs : array<float16>
    @ssbo @binding = 7 hd : array<half2>
    @ssbo @binding = 8 hflags : array<int>

    [metal_kernel(name="halfarith_gpu_msl", fastmath=false), marker(no_coverage)]
    def halfarith {
        let i = gl_GlobalInvocationID.x
        let a = ha[i]
        let b = hb[i] * 0.5h       // *0.5 is an exponent shift — exact, so `b` is pinned too
        o_add[i] = a + b
        o_sub[i] = a - b
        o_mul[i] = a * b
        o_div[i] = a / b
        let s = hs[i]
        let t = (s * 2.0h) + 0.25h  // s*2 exact -> a contracted fma cannot diverge here
        hd[i] = half2(t, -s)
        hflags[i] = (a == b) ? 1 : ((a.wzyx != b) ? 2 : ((s < t) ? 3 : 4))
    }
}

class LatticeConvert {
    @ssbo @binding = 0 bi : array<byte4>
    @ssbo @binding = 1 io : array<int4>
    @ssbo @binding = 2 bo : array<byte4>
    @ssbo @binding = 3 bs : array<byte4>
    @ssbo @binding = 4 st : array<short4>
    @ssbo @binding = 5 ui : array<ubyte2>
    @ssbo @binding = 6 uso : array<ushort2>
    @ssbo @binding = 7 uo : array<uint2>
    @ssbo @binding = 8 usat : array<ushort2>
    @ssbo @binding = 9 hi : array<half2>
    @ssbo @binding = 10 fo : array<float2>
    @ssbo @binding = 11 ho : array<half2>
    @ssbo @binding = 12 so : array<int>

    [metal_kernel(name="latconv_gpu_msl"), marker(no_coverage)]
    def latconv {
        let i = gl_GlobalInvocationID.x
        let w = int4(bi[i])
        io[i] = (w * w) + w
        bo[i] = byte4((w * 3) - int4(100))
        bs[i] = byte4_sat((w * 40) + w)
        st[i] = short4(bi[i])
        let uw = uint2(ui[i])
        uso[i] = ushort2(ui[i])
        uo[i] = ((uw * 3u) + uw) + uint2(7u, 9u)
        usat[i] = ushort2_sat(uw * 1000u)
        let f = float2(hi[i])
        fo[i] = f * 2.0
        ho[i] = half2(f + float2(0.25, 0.5))
        let sc = int16(w.x)
        so[i] = int(sc) + int(bi[i].s0)
    }
}

class PackedVec3 {
    @ssbo @binding = 0 pf : array<float3>
    @ssbo @binding = 1 pfo : array<float3>
    @ssbo @binding = 2 ph : array<half3>
    @ssbo @binding = 3 pho : array<half3>
    @ssbo @binding = 4 pb : array<byte3>
    @ssbo @binding = 5 pio : array<int3>
    @uniform @binding = 6 scale : float16
    @uniform @binding = 7 off : float3

    [metal_kernel(name="packed3_gpu_msl"), marker(no_coverage)]
    def packed3 {
        let i = gl_GlobalInvocationID.x
        let v = pf[i]
        pfo[i] = (v * 2.0) + off
        let h = ph[i]
        pho[i] = (h + h) * half3(scale)
        pio[i] = int3(pb[i]) * 3
    }
}

[test]
def test_halfarith_gpu_vs_cpu(t : T?) { // nolint:STYLE038 - linear GPU-vs-CPU comparison body
    t |> run("fp16 arithmetic [fastmath=false]: GPU == CPU-reference bit-exact") <| @(t : T?) {
        var oracle = new HalfArith()
        oracle.ha |> resize(N)
        oracle.hb |> resize(N)
        oracle.o_add |> resize(N)
        oracle.o_sub |> resize(N)
        oracle.o_mul |> resize(N)
        oracle.o_div |> resize(N)
        oracle.hs |> resize(N)
        oracle.hd |> resize(N)
        oracle.hflags |> resize(N)
        for (k in range(N)) {
            let base = float4(0.37 * float(k % 19) + 0.25, 1.5 + 0.11 * float(k % 7),
                3.0 - 0.2 * float(k % 5), 0.625 + 0.03 * float(k % 11))
            oracle.ha[k] = half4(base)
            // every 5th row: hb == 2*ha exactly (fp16 doubling is exact), so b == a -> flag 1
            oracle.hb[k] = ((k % 5 == 0) ? oracle.ha[k] + oracle.ha[k]
                : half4(base + float4(1.0, 0.75, 0.5, 0.25)))
            oracle.hs[k] = half4(base).s0
        }
        for (k in range(N)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            oracle->halfarith()
        }
        t |> equal(oracle.hflags[0], 1)     // row 0 is a k%5==0 row by construction
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() <| $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, halfarith_gpu_msl, halfarith_gpu_msl_entry, halfarith_gpu_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                t |> success(!halfarith_gpu_msl_fastmath, "kernel carries fastmath=false")
                var queue = metal_new_command_queue(dev)
                var bha = buf_upload(dev, oracle.ha)
                var bhb = buf_upload(dev, oracle.hb)
                var badd = buf_fill(dev, N, half4(0.))
                var bsub = buf_fill(dev, N, half4(0.))
                var bmul = buf_fill(dev, N, half4(0.))
                var bdiv = buf_fill(dev, N, half4(0.))
                var bhs = buf_upload(dev, oracle.hs)
                var bhd = buf_fill(dev, N, half2(0.))
                var bfl = buf_fill(dev, N, 0)
                var bufs <- [bha, bhb, badd, bsub, bmul, bdiv, bhs, bhd, bfl]
                var rerr : string
                let ran = run_compute_1d(queue, pso, bufs, uint(N), TPG, rerr)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    gpu_bad = (((buf_mismatch_exact(badd, oracle.o_add) + buf_mismatch_exact(bsub, oracle.o_sub)) +
                        (buf_mismatch_exact(bmul, oracle.o_mul) + buf_mismatch_exact(bdiv, oracle.o_div))) +
                        (buf_mismatch_exact(bhd, oracle.hd) + buf_mismatch_exact(bfl, oracle.hflags)))
                }
                bufs |> clear()             // non-owning handles — clear() skips pointee finalize
                unsafe {
                    delete bufs
                }
                metal_release(bha)
                metal_release(bhb)
                metal_release(badd)
                metal_release(bsub)
                metal_release(bmul)
                metal_release(bdiv)
                metal_release(bhs)
                metal_release(bhd)
                metal_release(bfl)
                metal_release(pso)
                metal_release(queue)
            }
            if (gpu_bad >= 0) {
                t |> equal(gpu_bad, 0)
            }
            t |> equal(metal_live_object_count(), 0l)
        } else {
            feint("das_metal is not built on this platform; CPU-reference half only\n")
        }
        unsafe {
            delete oracle
        }
    }
}

[test]
def test_latconv_gpu_vs_cpu(t : T?) { // nolint:STYLE038 - linear GPU-vs-CPU comparison body
    t |> run("int-lattice converts + saturation + fp16 converts: GPU == CPU-reference") <| @(t : T?) {
        var oracle = new LatticeConvert()
        oracle.bi |> resize(N)
        oracle.io |> resize(N)
        oracle.bo |> resize(N)
        oracle.bs |> resize(N)
        oracle.st |> resize(N)
        oracle.ui |> resize(N)
        oracle.uso |> resize(N)
        oracle.uo |> resize(N)
        oracle.usat |> resize(N)
        oracle.hi |> resize(N)
        oracle.fo |> resize(N)
        oracle.ho |> resize(N)
        oracle.so |> resize(N)
        for (k in range(N)) {
            oracle.bi[k] = byte4(k % 251 - 125, (k * 3) % 251 - 125, (k * 7) % 251 - 125, (k * 11) % 251 - 125)
            oracle.ui[k] = ubyte2(uint2(uint(k % 256), uint((k * 5) % 256)))
            oracle.hi[k] = half2(float2(0.3 * float(k), 1.7 + 0.1 * float(k)))
        }
        for (k in range(N)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            oracle->latconv()
        }
        t |> equal(oracle.io[0].x, 15500)   // w.x=-125: (-125)^2 + (-125) — the oracle itself computed
        t |> equal(int(oracle.bs[0].x), -128)   // (-125*40 - 125) saturates to -128
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() <| $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, latconv_gpu_msl, latconv_gpu_msl_entry, latconv_gpu_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var bbi = buf_upload(dev, oracle.bi)
                var bio = buf_fill(dev, N, int4(0))
                var bbo = buf_fill(dev, N, byte4(0, 0, 0, 0))
                var bbs = buf_fill(dev, N, byte4(0, 0, 0, 0))
                var bst = buf_fill(dev, N, short4(0, 0, 0, 0))
                var bui = buf_upload(dev, oracle.ui)
                var buso = buf_fill(dev, N, ushort2(0u, 0u))
                var buo = buf_fill(dev, N, uint2(0u))
                var busat = buf_fill(dev, N, ushort2(0u, 0u))
                var bhi = buf_upload(dev, oracle.hi)
                var bfo = buf_fill(dev, N, float2(0.))
                var bho = buf_fill(dev, N, half2(0.))
                var bso = buf_fill(dev, N, 0)
                var bufs <- [bbi, bio, bbo, bbs, bst, bui, buso, buo, busat, bhi, bfo, bho, bso]
                var rerr : string
                let ran = run_compute_1d(queue, pso, bufs, uint(N), TPG, rerr)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    gpu_bad = (((buf_mismatch_exact(bio, oracle.io) + buf_mismatch_exact(bbo, oracle.bo)) +
                        (buf_mismatch_exact(bbs, oracle.bs) + buf_mismatch_exact(bst, oracle.st))) +
                        ((buf_mismatch_exact(buso, oracle.uso) + buf_mismatch_exact(buo, oracle.uo)) +
                        (buf_mismatch_exact(busat, oracle.usat) + buf_mismatch_exact(bfo, oracle.fo))) +
                        (buf_mismatch_exact(bho, oracle.ho) + buf_mismatch_exact(bso, oracle.so)))
                }
                bufs |> clear()
                unsafe {
                    delete bufs
                }
                metal_release(bbi)
                metal_release(bio)
                metal_release(bbo)
                metal_release(bbs)
                metal_release(bst)
                metal_release(bui)
                metal_release(buso)
                metal_release(buo)
                metal_release(busat)
                metal_release(bhi)
                metal_release(bfo)
                metal_release(bho)
                metal_release(bso)
                metal_release(pso)
                metal_release(queue)
            }
            if (gpu_bad >= 0) {
                t |> equal(gpu_bad, 0)
            }
            t |> equal(metal_live_object_count(), 0l)
        } else {
            feint("das_metal is not built on this platform; CPU-reference half only\n")
        }
        unsafe {
            delete oracle
        }
    }
}

[test]
def test_packed3_gpu_vs_cpu(t : T?) {
    t |> run("packed 3-lane buffers (byte3/half3/float3/int3 das strides): GPU == CPU-reference") <| @(t : T?) {
        var oracle = new PackedVec3()
        oracle.pf |> resize(N)
        oracle.pfo |> resize(N)
        oracle.ph |> resize(N)
        oracle.pho |> resize(N)
        oracle.pb |> resize(N)
        oracle.pio |> resize(N)
        oracle.scale = 1.5h
        oracle.off = float3(0.5, 0.25, -1.0)
        for (k in range(N)) {
            oracle.pf[k] = float3(0.37 * float(k % 23), float(k) - 100.0, 0.125 * float(k % 9) + 0.5)
            oracle.ph[k] = half3(float3(0.25 * float(k % 13), 1.0 + 0.09 * float(k % 6), 2.0 - 0.11 * float(k % 4)))
            oracle.pb[k] = byte3(k % 251 - 125, (k * 3) % 251 - 125, (k * 7) % 251 - 125)
        }
        for (k in range(N)) {
            gl_GlobalInvocationID = uint3(uint(k), 0u, 0u)
            oracle->packed3()
        }
        t |> equal(oracle.pio[0].x, -375)   // pb[0].x = -125, widened * 3 — the oracle itself computed
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var gpu_bad = -1
            with_metal_device() <| $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU compare skipped\n")
                    return
                }
                var perr : string
                var pso = pipeline_from_source(dev, packed3_gpu_msl, packed3_gpu_msl_entry, packed3_gpu_msl_fastmath, perr)
                t |> success(pso != null, "pipeline_from_source: {perr}")
                if (pso == null) {
                    return
                }
                var queue = metal_new_command_queue(dev)
                var bpf = buf_upload(dev, oracle.pf)
                var bpfo = buf_fill(dev, N, float3(0.))
                var bph = buf_upload(dev, oracle.ph)
                var bpho = buf_fill(dev, N, half3(0., 0., 0.))
                var bpb = buf_upload(dev, oracle.pb)
                var bpio = buf_fill(dev, N, int3(0))
                var bscale = buf_fill(dev, 1, oracle.scale)
                var boff = buf_fill(dev, 1, oracle.off)
                var bufs <- [bpf, bpfo, bph, bpho, bpb, bpio, bscale, boff]
                var rerr : string
                let ran = run_compute_1d(queue, pso, bufs, uint(N), TPG, rerr)
                t |> success(ran, "run_compute_1d: {rerr}")
                if (ran) {
                    gpu_bad = ((buf_mismatch_exact(bpfo, oracle.pfo) + buf_mismatch_exact(bpho, oracle.pho)) +
                        buf_mismatch_exact(bpio, oracle.pio))
                }
                bufs |> clear()
                unsafe {
                    delete bufs
                }
                metal_release(bpf)
                metal_release(bpfo)
                metal_release(bph)
                metal_release(bpho)
                metal_release(bpb)
                metal_release(bpio)
                metal_release(bscale)
                metal_release(boff)
                metal_release(pso)
                metal_release(queue)
            }
            if (gpu_bad >= 0) {
                t |> equal(gpu_bad, 0)
            }
            t |> equal(metal_live_object_count(), 0l)
        } else {
            feint("das_metal is not built on this platform; CPU-reference half only\n")
        }
        unsafe {
            delete oracle
        }
    }
}
