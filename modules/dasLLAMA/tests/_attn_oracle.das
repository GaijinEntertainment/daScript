// The single-query attention oracle the Metal and the Vulkan kernel cells share - one plain two-pass softmax per (query
// row, head) over a key range, with the softcap and the attention sink in the forms the kernels take.

options gen2
options _dasllama_internal = true

module _attn_oracle shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_gpu_math

//! single-query causal attention oracle: one output row per (query row, head) - plain two-pass softmax over [kstart, cnt)
//! (a sliding layer's window starts past 0), the cap in the exp form the kernels take; the device's online form reassociates,
//! so approx bars. `has_sink` / `sink`: the head's attention sink logit (gpt-oss) joins the softmax's max and denominator as
//! a phantom key with no value row
def attn_row_oracle(q : array<float>; qbase : int; kk : array<float>; kbase0 : int;
                    vv : array<float>; vbase0 : int; kvd, hh, hs, kv_mul, cnt : int;
                    scale : float; var out : array<float>; obase : int; kstart : int = 0; cap : float = 0.0;
                    has_sink : bool = false; sink : float = 0.0) {
    let khoff = (hh / kv_mul) * hs
    attn_row_scored(vv, vbase0 + khoff, kvd, hs, kstart, cnt, out, obase, has_sink, sink) $(j : int) : float {
        var s = 0.0
        for (e in range(hs)) {
            s += q[qbase + e] * kk[kbase0 + j * kvd + khoff + e]
        }
        return cap != 0.0 ? softcap_exp(s * scale, cap) : s * scale
    }
}

//! gemma4a's windowed RPE attention, spelled as `gemma4a_encode`'s loop: per head (`hs` wide in rows `d` wide), query row i
//! attends keys (i - past, i]; a logit is the query times `qscale` and the per-dim `pds`, dotted with the key times `kscale`
//! plus the relative-position row `rpe[(past - (i - j)) * d ...]`, capped as `cap * tanh(x / cap)`
def g4a_attn_oracle(q, k, v, rpe, pds : array<float>; rows, d, heads, hs, past : int; qscale, kscale, cap : float) : array<float> {
    var ao : array<float>
    ao |> resize(rows * d)
    for (hh in range(heads)) {
        let hoff = hh * hs
        for (i in range(rows)) {
            attn_row_scored(v, hoff, d, hs, max(0, i - (past - 1)), i + 1, ao, i * d + hoff, false, 0.0) $(j : int) : float {
                let rr = (past - (i - j)) * d
                var cs = 0.0
                var rs = 0.0
                for (n in range(hs)) {
                    let qv = q[i * d + hoff + n] * qscale * pds[n]
                    cs += qv * (k[j * d + hoff + n] * kscale)
                    rs += qv * rpe[rr + hoff + n]
                }
                return cap * tanh((cs + rs) / cap)
            }
        }
    }
    return <- ao
}

//! two-pass softmax row: key j in [kstart, cnt) scores `score(j)`, weighting vv[vbase + j * kvd + e]; `has_sink` adds a valueless `sink` key
def attn_row_scored(vv : array<float>; vbase, kvd, hs, kstart, cnt : int; var out : array<float>; obase : int;
                    has_sink : bool; sink : float; score : block<(j : int) : float>) {
    var sc : array<float>
    sc |> resize(cnt)
    var mm = has_sink ? sink : -3.0e38
    for (j in range(kstart, cnt)) {
        sc[j] = invoke(score, j)
        mm = max(mm, sc[j])
    }
    var tot = has_sink ? exp(sink - mm) : 0.0
    for (j in range(kstart, cnt)) {
        sc[j] = exp(sc[j] - mm)
        tot += sc[j]
    }
    for (e in range(hs)) {
        var acc = 0.0
        for (j in range(kstart, cnt)) {
            acc += sc[j] * vv[vbase + j * kvd + e]
        }
        out[obase + e] = acc / tot
    }
    delete sc
}
