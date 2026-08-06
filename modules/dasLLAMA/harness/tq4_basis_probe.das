options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require daslib/fio
require math
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require dasllama/dasllama_math
require dasllama/dasllama_kv_codec  // the tq4 codec + FWHT rotation under probe
require dasllama/dasllama_tokenizer
require llvm/daslib/f16_cvt
require dasllama/dasllama_env

// Basis A/B on REAL cache rows: run SmolLM2 over real text with an f32 KV cache, then re-quantize
// every stored K (post-RoPE) and V head-row at 4 bits (production tq4 kernels) under four bases —
// identity / seeded-sign FWHT / orthonormal DCT-II / orthonormal real DFT — plus an 8-bit-low-block
// 4-bit-high-block mixed arm. Reports rel-MSE and energy-compaction per basis per side.
// Usage: bin/daslang -jit modules/dasLLAMA/harness/tq4_basis_probe.das

let TEXT = "The old lighthouse keeper climbed the spiral staircase for the last time, counting each of the one hundred and twelve steps as he had done every evening for forty years. Below him the harbor lights flickered on one by one, and the fishing boats came home through the narrow channel between the rocks. He remembered the winter of the great storm, when three ships had run aground in a single night and the whole town had formed a human chain through the freezing surf to pull the sailors ashore. His daughter wrote him letters from the city, asking when he would finally retire and come live with her family, but he always answered the same way: the light needs someone who knows the water. Tomorrow the coast guard would install the new automated beacon, a small gray box that needed no keeper at all. He polished the great lens one final time, watching the beam sweep across the darkening sea, and wondered what the gulls would make of an empty tower. In the morning he packed his few belongings, left the key in the lock as instructed, and walked down the hill without looking back, while behind him the new light blinked its precise, indifferent rhythm into the fog."

let TEXT_B = "To configure the build system, first install the toolchain packages: cmake version 3.20 or newer, ninja, and a C compiler with C++20 support. Clone the repository with submodules, set the SDK environment variable, run the configure script with the release preset, and then build the default target. If the linker reports missing symbols for the math library, add the flag explicitly at the end of the command line. The test suite runs in parallel by default; pass the jobs flag to limit concurrency on machines with little memory. On Windows the paths use backslashes and the scripts expect PowerShell five or newer, while on Linux and macOS a POSIX shell is enough. Package the artifacts with the install target and verify the checksum of the resulting archive before uploading it to the release page. The continuous integration pipeline repeats these steps on every commit, caching the dependency downloads to keep the total build time under ten minutes, and publishes the binaries only when every platform in the matrix is green."

let N_BASES = 4
let BASE_NAMES = fixed_array("identity", "fwht    ", "dct     ", "dft     ")

// learn a per-(layer, head, side) channel order on sL's rows, score 4-bit + mixed 8/4 on sS's rows.
// pairs = sort RoPE pairs (2i, 2i+1) by pair RMS instead of single channels (the loader-bakeable
// granularity for K — rope tables move pairwise).
def measure_sorted(sL, sS : Session; c : Config; hs, npastL, npastS : int64; pairs : bool; tag : string) {   // nolint:STYLE037,STYLE038 — one measurement kernel over (layer, head, side); order-learning and scoring are loop-coupled
    var e2 : double[2]
    var e2mix : double[2]
    var pw : double[2]
    var chrms : array<double>
    var order : array<int>
    var porder : array<int>
    var seg : array<float>
    var dq : array<float>
    var blk : array<uint8>
    chrms |> resize(hs)
    order |> resize(hs)
    porder |> resize(hs / 2l)
    seg |> resize(hs)
    dq |> resize(hs)
    blk |> resize(hs / 32l * 18l)
    unsafe {
        let kbL = addr<uint8 const?>(sL.key_cache[0])
        let vbL = addr<uint8 const?>(sL.value_cache[0])
        let kbS = addr<uint8 const?>(sS.key_cache[0])
        let vbS = addr<uint8 const?>(sS.value_cache[0])
        for (l in range64(c.n_layers)) {
            if (layer_kv_source(c, l) != l) {
                continue
            }
            let kvd = layer_kv_dim(c, l)
            let kpL = reinterpret<float const?>(kbL + sL.kv_bprefix_k[l] * c.seq_len)
            let vpL = reinterpret<float const?>(vbL + sL.kv_bprefix_v[l] * c.seq_len)
            let kpS = reinterpret<float const?>(kbS + sS.kv_bprefix_k[l] * c.seq_len)
            let vpS = reinterpret<float const?>(vbS + sS.kv_bprefix_v[l] * c.seq_len)
            for (h in range64(kvd / hs)) {
                for (side in range64(2l)) {
                    let srcL = side == 0l ? kpL : vpL
                    let srcS = side == 0l ? kpS : vpS
                    for (i in range64(hs)) {
                        chrms[i] = 0.0lf
                    }
                    for (row in range64(npastL)) {
                        let off = row * kvd + h * hs
                        for (i in range64(hs)) {
                            chrms[i] += double(srcL[off + i] * srcL[off + i])
                        }
                    }
                    if (pairs) {
                        for (p in range64(hs / 2l)) {
                            porder[p] = int(p)
                        }
                        porder |> sort() $(oa, ob) => chrms[2 * oa] + chrms[2 * oa + 1] > chrms[2 * ob] + chrms[2 * ob + 1]
                        for (p in range64(hs / 2l)) {
                            order[2l * p] = 2 * porder[p]
                            order[2l * p + 1l] = 2 * porder[p] + 1
                        }
                    } else {
                        for (i in range64(hs)) {
                            order[i] = int(i)
                        }
                        order |> sort() $(oa, ob) => chrms[oa] > chrms[ob]
                    }
                    for (row in range64(npastS)) {
                        let off = row * kvd + h * hs
                        for (i in range64(hs)) {
                            seg[i] = srcS[off + int64(order[i])]
                        }
                        for (i in range64(hs)) {
                            pw[side] += double(seg[i] * seg[i])
                        }
                        quantize_tq4kv_row(addr(blk[0]), addr(seg[0]), hs)
                        cvt_tq4kv_to_f32(addr(dq[0]), addr(blk[0]), hs)
                        for (i in range64(hs)) {
                            let e = double(dq[i] - seg[i])
                            e2[side] += e * e
                        }
                        for (i in range64(hs)) {
                            dq[i] = seg[i]
                        }
                        quant_int8_block(dq, 0l, 32l)
                        if (hs > 32l) {
                            quantize_tq4kv_row(addr(blk[0]), addr(seg[0]) + 32l, hs - 32l)
                            cvt_tq4kv_to_f32(addr(dq[0]) + 32l, addr(blk[0]), hs - 32l)
                        }
                        for (i in range64(hs)) {
                            let e = double(dq[i] - seg[i])
                            e2mix[side] += e * e
                        }
                    }
                }
            }
        }
    }
    print("  {tag}  K: 4b {e2[0] / pw[0]}  8/4 {e2mix[0] / pw[0]}   V: 4b {e2[1] / pw[1]}  8/4 {e2mix[1] / pw[1]}\n")
    delete chrms
    delete order
    delete porder
    delete seg
    delete dq
    delete blk
}

def build_dct(var m : array<float>; n : int64) {
    m |> resize(n * n)
    for (k in range64(n)) {
        let sk = k == 0l ? sqrt(1.0 / float(n)) : sqrt(2.0 / float(n))
        for (j in range64(n)) {
            m[k * n + j] = sk * cos(PI * (2.0 * float(j) + 1.0) * float(k) / (2.0 * float(n)))
        }
    }
}

def build_dft(var m : array<float>; n : int64) {
    // orthonormal real-DFT rows ordered low frequency first: dc, cos1, sin1, ..., nyquist
    m |> resize(n * n)
    var r = 0l
    for (j in range64(n)) {
        m[r * n + j] = sqrt(1.0 / float(n))
    }
    r ++
    for (k in range64(1l, n / 2l)) {
        for (j in range64(n)) {
            m[r * n + j] = sqrt(2.0 / float(n)) * cos(2.0 * PI * float(k) * float(j) / float(n))
        }
        r ++
        for (j in range64(n)) {
            m[r * n + j] = sqrt(2.0 / float(n)) * sin(2.0 * PI * float(k) * float(j) / float(n))
        }
        r ++
    }
    for (j in range64(n)) {
        m[r * n + j] = sqrt(1.0 / float(n)) * ((j & 1l) == 0l ? 1.0 : -1.0)
    }
}

def apply_mat(m : array<float>; x : array<float>; var y : array<float>; n : int64) {
    for (k in range64(n)) {
        var acc = 0.0
        for (j in range64(n)) {
            acc += m[k * n + j] * x[j]
        }
        y[k] = acc
    }
}

// probe-local 8-bit symmetric amax block quant (f16-rounded scale), for the mixed 8/4 arm
def quant_int8_block(var v : array<float>; i0, n : int64) {
    var amax = 0.0
    for (j in range64(i0, i0 + n)) {
        amax = max(amax, abs(v[j]))
    }
    let d = f16_to_f32(f32_to_f16(amax / 127.0))
    let id = d != 0.0 ? 1.0 / d : 0.0
    for (j in range64(i0, i0 + n)) {
        v[j] = float(clamp(int(floor(v[j] * id + 0.5)), -127, 127)) * d
    }
}

struct Acc {
    e2 : double      // tq4 4-bit error
    e2mix : double   // 8-bit block0 + 4-bit rest
    pw : double
    blk0 : double    // energy in coefficients [0, 32)
    vprof : array<double>
}

[export]
def main {   // nolint:STYLE037,STYLE038 — flat probe driver: every basis arm walks the same cache rows through the same scratch buffers
    let env = g_env_harness.models_dir
    let mdir = empty(env) ? "D:/Work/llama.cpp/models/" : env
    let path = path_join(mdir, "SmolLM2-135M-Instruct-Q8_0.gguf")
    if (!stat(path).is_valid) {
        panic("model not found: {path}")
    }
    var tr <- load_gguf(path, QuantMode.q8)
    let tok <- load_tokenizer_auto(path)
    var ids <- encode(tok, TEXT)
    print("prompt tokens: {length(ids)}\n")
    tr.config.seq_len = min(tr.config.seq_len, 512l)
    var s <- make_run_state(tr.config, KVDtype.f32, KVDtype.f32)
    with_job_que() {
        eval_(tr, s, ids)
    }
    let c & = unsafe(tr.config)
    let npast = s.n_past
    let hs = layer_head_size(c, 0l)
    print("layers {c.n_layers}, kv_heads {layer_n_kv_heads(c, 0l)}, head_size {hs}, rows {npast}\n")

    var dct : array<float>
    var dft : array<float>
    build_dct(dct, hs)
    build_dft(dft, hs)
    var signs : array<float>
    tq4_fill_signs(signs, hs)

    var acc : Acc[8]   // [side * 4 + basis], side 0 = K, 1 = V
    for (a in acc) {
        a.vprof |> resize(hs)
    }
    var seg : array<float>
    var y : array<float>
    var dq : array<float>
    var blk : array<uint8>
    seg |> resize(hs)
    y |> resize(hs)
    dq |> resize(hs)
    blk |> resize(hs / 32l * 18l)

    unsafe {
        let kb = addr<uint8 const?>(s.key_cache[0])
        let vb = addr<uint8 const?>(s.value_cache[0])
        let sgp = addr<float const?>(signs[0])
        for (l in range64(c.n_layers)) {
            if (layer_kv_source(c, l) != l) {
                continue
            }
            let kvd = layer_kv_dim(c, l)
            let kp = reinterpret<float const?>(kb + s.kv_bprefix_k[l] * c.seq_len)
            let vp = reinterpret<float const?>(vb + s.kv_bprefix_v[l] * c.seq_len)
            for (row in range64(npast)) {
                for (h in range64(kvd / hs)) {
                    for (side in range64(2l)) {
                        let src = side == 0l ? kp : vp
                        let off = row * kvd + h * hs
                        for (basis in range64(int64(N_BASES))) {
                            for (i in range64(hs)) {
                                seg[i] = src[off + i]
                            }
                            if (basis == 1l) {
                                fwht_signs_row(addr(seg[0]), sgp, hs)
                                for (i in range64(hs)) {
                                    y[i] = seg[i]
                                }
                            } elif (basis == 2l) {
                                apply_mat(dct, seg, y, hs)
                            } elif (basis == 3l) {
                                apply_mat(dft, seg, y, hs)
                            } else {
                                for (i in range64(hs)) {
                                    y[i] = seg[i]
                                }
                            }
                            var a & = acc[side * 4l + basis]
                            for (i in range64(hs)) {
                                let e = double(y[i] * y[i])
                                a.pw += e
                                a.vprof[i] += e
                                if (i < 32l) {
                                    a.blk0 += e
                                }
                            }
                            // tq4 4-bit arm through the production kernels
                            quantize_tq4kv_row(addr(blk[0]), addr(y[0]), hs)
                            cvt_tq4kv_to_f32(addr(dq[0]), addr(blk[0]), hs)
                            for (i in range64(hs)) {
                                let e = double(dq[i] - y[i])
                                a.e2 += e * e
                            }
                            // mixed arm: 8-bit block0, tq4 4-bit for the rest
                            for (i in range64(hs)) {
                                dq[i] = y[i]
                            }
                            quant_int8_block(dq, 0l, 32l)
                            if (hs > 32l) {
                                quantize_tq4kv_row(addr(blk[0]), addr(y[0]) + 32l, hs - 32l)
                                cvt_tq4kv_to_f32(addr(dq[0]) + 32l, addr(blk[0]), hs - 32l)
                            }
                            for (i in range64(hs)) {
                                let e = double(dq[i] - y[i])
                                a.e2mix += e * e
                            }
                        }
                    }
                }
            }
        }
    }

    // sorted-blocks permutation arm: per-(layer, head, side) channels sorted by RMS (in-sample
    // upper bound for a loader-baked channel reorder) — outlier channels quarantine into block 0,
    // which the mixed arm then serves at 8 bits
    var acc_s : Acc[2]
    for (a in acc_s) {
        a.vprof |> resize(hs)
    }
    var chrms : array<double>
    var order : array<int>
    chrms |> resize(hs)
    order |> resize(hs)
    unsafe {
        let kb = addr<uint8 const?>(s.key_cache[0])
        let vb = addr<uint8 const?>(s.value_cache[0])
        for (l in range64(c.n_layers)) {
            if (layer_kv_source(c, l) != l) {
                continue
            }
            let kvd = layer_kv_dim(c, l)
            let kp = reinterpret<float const?>(kb + s.kv_bprefix_k[l] * c.seq_len)
            let vp = reinterpret<float const?>(vb + s.kv_bprefix_v[l] * c.seq_len)
            for (h in range64(kvd / hs)) {
                for (side in range64(2l)) {
                    let src = side == 0l ? kp : vp
                    for (i in range64(hs)) {
                        chrms[i] = 0.0lf
                    }
                    for (row in range64(npast)) {
                        let off = row * kvd + h * hs
                        for (i in range64(hs)) {
                            chrms[i] += double(src[off + i] * src[off + i])
                        }
                    }
                    for (i in range64(hs)) {
                        order[i] = int(i)
                    }
                    order |> sort() $(oa, ob) => chrms[oa] > chrms[ob]
                    var aa & = acc_s[side]
                    for (row in range64(npast)) {
                        let off = row * kvd + h * hs
                        for (i in range64(hs)) {
                            seg[i] = src[off + int64(order[i])]
                        }
                        for (i in range64(hs)) {
                            let e = double(seg[i] * seg[i])
                            aa.pw += e
                            aa.vprof[i] += e
                            if (i < 32l) {
                                aa.blk0 += e
                            }
                        }
                        quantize_tq4kv_row(addr(blk[0]), addr(seg[0]), hs)
                        cvt_tq4kv_to_f32(addr(dq[0]), addr(blk[0]), hs)
                        for (i in range64(hs)) {
                            let e = double(dq[i] - seg[i])
                            aa.e2 += e * e
                        }
                        for (i in range64(hs)) {
                            dq[i] = seg[i]
                        }
                        quant_int8_block(dq, 0l, 32l)
                        if (hs > 32l) {
                            quantize_tq4kv_row(addr(blk[0]), addr(seg[0]) + 32l, hs - 32l)
                            cvt_tq4kv_to_f32(addr(dq[0]) + 32l, addr(blk[0]), hs - 32l)
                        }
                        for (i in range64(hs)) {
                            let e = double(dq[i] - seg[i])
                            aa.e2mix += e * e
                        }
                    }
                }
            }
        }
    }

    for (side in range64(2l)) {
        print("side {side == 0l ? "K(post-rope)" : "V"}:\n")
        print("  basis     rel-mse(4b)      rel-mse(8/4)     blk0-energy  top8-var\n")
        for (basis in range64(int64(N_BASES))) {
            let a & = unsafe(acc[side * 4l + basis])
            var vs : array<double>
            vs := a.vprof
            // top-8 coefficient variance share (sorted descending)
            vs |> sort() $(x, yy) => x > yy
            var t8 = 0.0lf
            for (i in range64(8l)) {
                t8 += vs[i]
            }
            print("  {BASE_NAMES[basis]}  {a.e2 / a.pw}  {a.e2mix / a.pw}  {a.blk0 / a.pw}  {t8 / a.pw}\n")
            delete vs
        }
        let a & = unsafe(acc_s[side])
        var t8 = 0.0lf
        for (i in range64(8l)) {
            t8 += a.vprof[i]   // already sorted by construction
        }
        print("  sorted    {a.e2 / a.pw}  {a.e2mix / a.pw}  {a.blk0 / a.pw}  {t8 / a.pw}\n")
    }
    delete chrms
    delete order
    for (a in acc_s) {
        delete a.vprof
    }

    // robustness matrix: order granularity (channel vs rope-pair) × source (in-sample vs held-out)
    print("sorted-order robustness (scored on TEXT_B rows):\n")
    var ids_b <- encode(tok, TEXT_B)
    print("  held-out prompt tokens: {length(ids_b)}\n")
    var s_b <- make_run_state(tr.config, KVDtype.f32, KVDtype.f32)
    with_job_que() {
        eval_(tr, s_b, ids_b)
    }
    measure_sorted(s_b, s_b, c, hs, s_b.n_past, s_b.n_past, false, "chan in-sample")
    measure_sorted(s_b, s_b, c, hs, s_b.n_past, s_b.n_past, true, "pair in-sample")
    measure_sorted(s, s_b, c, hs, npast, s_b.n_past, false, "chan held-out ")
    measure_sorted(s, s_b, c, hs, npast, s_b.n_past, true, "pair held-out ")
    delete ids_b
    delete s_b
    delete seg
    delete y
    delete dq
    delete blk
    delete dct
    delete dft
    delete signs
    for (a in acc) {
        delete a.vprof
    }
    delete ids
    delete s
    delete tr
}
