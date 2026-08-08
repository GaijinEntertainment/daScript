options gen2
options persistent_heap
options _jit_fast_math = true   // match the engine config the trace numbers came from

// Flash-decode stage-2 mechanism probe (30B emission investigation): the traced kv-partials
// stage runs ~118us/chain at d544 vs ~40us byte-bound, format-independent (q8 KV barely moves
// it) — so the cost is in the per-(pos, head) work structure, not the KV bytes. This isolates
// the production nesting (8 small dot()/axpy() calls per position, one per GQA head) against a
// fused 8-head walk (K/V row loaded once per j-step, 8 accumulators) and a stream-only floor,
// on the exact 30B decode geometry. Single thread — per-lane kernel cost, no dispatch.
//   bin/daslang -jit modules/dasLLAMA/harness/flash_stage2_probe.das

require dasllama/dasllama_math
require dasllama/dasllama_math_default
require daslib/jobque_boost
require math

let HS = 128l          // head_size
let KV_MUL = 8l        // q heads per kv head (GQA-8)
let NKVH = 4l          // kv heads
let KV_DIM = 512l      // NKVH * HS
let NQH = 32l          // q heads
let CNT = 544l         // window positions (d512 decode window mid-point)
let SLICE = 32l        // FLASH_DECODE_SLICE_LEN
let SEQ = 2048l        // att row stride (config.seq_len)
let NL = 24l           // layer-slab rotation (2x26.7MB streams: defeats LLC between rounds)
let ROUNDS = 5

def private fill(var a : array<float>; n : int64; seed : float) {
    a |> resize(int(n))
    for (i in range64(n)) {
        a[i] = sin(float(i) * 0.017 + seed) * 0.5
    }
}

// Variant A — the production kv_slice_scores/kv_slice_av nesting: per position, kv_mul small
// dot()/axpy() calls (one per q head of the group); K/V row re-read per head (L1-hot).
def private stage2_calls(kp, vp, qp : float const?; var attp : float?; var flop : float?; nsl : int64) {
    unsafe {
        for (u in range64(NKVH * nsl)) {
            let kvh = u / nsl
            let sl = u % nsl
            let kvhoff = kvh * HS
            let qh0 = kvh * KV_MUL
            let j0 = sl * SLICE
            let j1 = min(j0 + SLICE, CNT)
            for (j in range64(j0, j1)) {
                let koff = j * KV_DIM + kvhoff
                for (g in range64(KV_MUL)) {
                    attp[(qh0 + g) * SEQ + j] = dot(qp + (qh0 + g) * HS, kp + koff, HS) * 0.088388
                }
            }
            for (g in range64(KV_MUL)) {
                let hh = qh0 + g
                var op = flop + (hh * nsl + sl) * HS
                for (i in range64(HS)) {
                    op[i] = 0.0
                }
            }
            for (j in range64(j0, j1)) {
                let voff = j * KV_DIM + kvhoff
                for (g in range64(KV_MUL)) {
                    let hh = qh0 + g
                    axpy(flop + (hh * nsl + sl) * HS, vp + voff, attp[hh * SEQ + j], HS)
                }
            }
        }
    }
}

// Variant B — fused 8-head walk: K row streamed ONCE per j-quad into 8 float4 accumulator
// chains (no per-head call, no per-head hsum until the row ends); V pass accumulates all 8
// partials off one V row read. Same flops, same reads at L1 level — isolates call/hsum/loop
// structure. Fold order differs from dot() (tolerance-compared, fast-math anyway).
def private stage2_fused(kp, vp, qp : float const?; var attp : float?; var flop : float?; nsl : int64) {
    unsafe {
        for (u in range64(NKVH * nsl)) {
            let kvh = u / nsl
            let sl = u % nsl
            let kvhoff = kvh * HS
            let qh0 = kvh * KV_MUL
            let j0 = sl * SLICE
            let j1 = min(j0 + SLICE, CNT)
            let q4 = reinterpret<float4 const?>(qp + qh0 * HS)   // 8 contiguous head rows
            for (j in range64(j0, j1)) {
                let k4 = reinterpret<float4 const?>(kp + j * KV_DIM + kvhoff)
                var a0 = float4(0.0)
                var a1 = float4(0.0)
                var a2 = float4(0.0)
                var a3 = float4(0.0)
                var a4 = float4(0.0)
                var a5 = float4(0.0)
                var a6 = float4(0.0)
                var a7 = float4(0.0)
                let hq = HS / 4l
                for (c in range64(hq)) {
                    let kv = k4[c]
                    a0 += kv * q4[c]
                    a1 += kv * q4[hq + c]
                    a2 += kv * q4[2l * hq + c]
                    a3 += kv * q4[3l * hq + c]
                    a4 += kv * q4[4l * hq + c]
                    a5 += kv * q4[5l * hq + c]
                    a6 += kv * q4[6l * hq + c]
                    a7 += kv * q4[7l * hq + c]
                }
                attp[(qh0 + 0l) * SEQ + j] = (a0.x + a0.y + a0.z + a0.w) * 0.088388
                attp[(qh0 + 1l) * SEQ + j] = (a1.x + a1.y + a1.z + a1.w) * 0.088388
                attp[(qh0 + 2l) * SEQ + j] = (a2.x + a2.y + a2.z + a2.w) * 0.088388
                attp[(qh0 + 3l) * SEQ + j] = (a3.x + a3.y + a3.z + a3.w) * 0.088388
                attp[(qh0 + 4l) * SEQ + j] = (a4.x + a4.y + a4.z + a4.w) * 0.088388
                attp[(qh0 + 5l) * SEQ + j] = (a5.x + a5.y + a5.z + a5.w) * 0.088388
                attp[(qh0 + 6l) * SEQ + j] = (a6.x + a6.y + a6.z + a6.w) * 0.088388
                attp[(qh0 + 7l) * SEQ + j] = (a7.x + a7.y + a7.z + a7.w) * 0.088388
            }
            for (g in range64(KV_MUL)) {
                var op = reinterpret<float4?>(flop + ((qh0 + g) * nsl + sl) * HS)
                for (c in range64(HS / 4l)) {
                    op[c] = float4(0.0)
                }
            }
            for (j in range64(j0, j1)) {
                let v4 = reinterpret<float4 const?>(vp + j * KV_DIM + kvhoff)
                for (g in range64(KV_MUL)) {
                    let hh = qh0 + g
                    let w = attp[hh * SEQ + j]
                    var op = reinterpret<float4?>(flop + (hh * nsl + sl) * HS)
                    for (c in range64(HS / 4l)) {
                        op[c] += v4[c] * w
                    }
                }
            }
        }
    }
}

// Variant C — stream floor: touch every K/V byte of the window once, minimal ALU.
def private stage2_stream(kp, vp : float const?; var sink : float&) {
    unsafe {
        var s = float4(0.0)
        let k4 = reinterpret<float4 const?>(kp)
        let v4 = reinterpret<float4 const?>(vp)
        for (c in range64(CNT * KV_DIM / 4l)) {
            s += k4[c] + v4[c]
        }
        sink += s.x + s.y + s.z + s.w
    }
}

[export]
def main() {
    if (!jit_enabled()) {
        print("JIT-only probe - run with: daslang -jit <file>\n")
        return
    }
    let nsl = (CNT + SLICE - 1l) / SLICE
    var k : array<float>
    var v : array<float>
    var q : array<float>
    var att : array<float>
    var flop : array<float>
    fill(k, NL * CNT * KV_DIM, 0.1)
    fill(v, NL * CNT * KV_DIM, 0.7)
    fill(q, NQH * HS, 1.3)
    att |> resize(int(NQH * SEQ))
    flop |> resize(int(NQH * nsl * HS))
    var att2 : array<float>
    var flop2 : array<float>
    att2 |> resize(int(NQH * SEQ))
    flop2 |> resize(int(NQH * nsl * HS))

    let kvbytes = CNT * KV_DIM * 4l * 2l   // K+V per layer-window
    print("geometry: cnt={CNT} kv_dim={KV_DIM} kv_mul={KV_MUL} nsl={nsl} | {NL} slabs x {kvbytes / 1024l}KB KV\n")

    unsafe {
        var qp = addr<float const?>(q[0])
        var attp = addr(att[0])
        var flp = addr(flop[0])
        var attp2 = addr(att2[0])
        var flp2 = addr(flop2[0])
        // correctness: A vs B on slab 0 (fold order differs — tolerance)
        stage2_calls(addr(k[0]), addr(v[0]), qp, attp, flp, nsl)
        stage2_fused(addr(k[0]), addr(v[0]), qp, attp2, flp2, nsl)
        var worst = 0.0
        for (h in range64(NQH)) {
            for (j in range64(CNT)) {
                worst = max(worst, abs(att[h * SEQ + j] - att2[h * SEQ + j]))
            }
        }
        var worstf = 0.0
        for (i in range64(NQH * nsl * HS)) {
            worstf = max(worstf, abs(flop[i] - flop2[i]))
        }
        print("A-vs-B worst |att| diff {worst}, worst |partial| diff {worstf} (fast-math fold tolerance)\n")

        var sink = 0.0
        for (vr in range(3)) {
            var best = 1.0e18lf
            for (_r in range(ROUNDS)) {
                let t0 = ref_time_ticks()
                for (l in range64(NL)) {
                    let kb = addr(k[0]) + l * CNT * KV_DIM
                    let vb = addr(v[0]) + l * CNT * KV_DIM
                    if (vr == 0) {
                        stage2_calls(kb, vb, qp, attp, flp, nsl)
                    } elif (vr == 1) {
                        stage2_fused(kb, vb, qp, attp, flp, nsl)
                    } else {
                        stage2_stream(kb, vb, sink)
                    }
                }
                let us = double(get_time_usec(t0))
                best = min(best, us)
            }
            let per_layer = best / double(NL)
            let ns_pos = per_layer * 1000.0lf / double(CNT)
            let gbs = double(NL * kvbytes) / (best * 1.0e-6lf) / (1024.0lf * 1024.0lf * 1024.0lf)
            let name = vr == 0 ? "A per-head dot/axpy calls" : (vr == 1 ? "B fused 8-head walk     " : "C stream floor          ")
            print("{name}: {per_layer}us/window  {ns_pos}ns/pos  {gbs}GB/s effective\n")
        }
        print("sink {sink}\n")
    }
}
