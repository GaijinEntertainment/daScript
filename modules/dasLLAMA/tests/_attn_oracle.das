// The single-query attention oracle the Metal, Vulkan and CUDA kernel cells share - one plain two-pass softmax per (query
// row, head) over a key range, with the softcap and the attention sink in the forms the kernels take - and the flash tiles'
// oracle over it on the kernel's f16 grid.

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

//! `attn_row_oracle` over every head of one query row: head hh reads q at qbase + hh * hs and lands at obase + hh * hs
def attn_heads_oracle(q : array<float>; qbase : int; kk : array<float>; kbase : int; vv : array<float>; vbase : int;
                      kvd, nheads, hs, kv_mul, cnt : int; scale : float; var out : array<float>; obase : int;
                      kstart : int = 0; cap : float = 0.0) {
    for (hh in range(nheads)) {
        attn_row_oracle(q, qbase + hh * hs, kk, kbase, vv, vbase, kvd, hh, hs, kv_mul, cnt, scale, out, obase + hh * hs, kstart, cap)
    }
}

//! `attn_heads_oracle` with head hh's attention sink logit `sinks[hh]` in its softmax
def attn_heads_oracle(q : array<float>; qbase : int; kk : array<float>; kbase : int; vv : array<float>; vbase : int;
                      kvd, nheads, hs, kv_mul, cnt : int; scale : float; var out : array<float>; obase : int;
                      sinks : array<float>; kstart : int = 0; cap : float = 0.0) {
    for (hh in range(nheads)) {
        attn_row_oracle(q, qbase + hh * hs, kk, kbase, vv, vbase, kvd, hh, hs, kv_mul, cnt, scale, out, obase + hh * hs, kstart, cap, true, sinks[hh])
    }
}

//! the flash tiles' attention oracle on the kernel's f16 grid over `rows` query rows at window base `w0`: q pre-scaled then
//! rounded, K/V rounded through the mirror, softmax in f32 over the keys to each row's own (the last `window` of them where it is
//! nonzero), the scores under `cap` where it is nonzero; under `gated` each head's q row carries its gate half after it (a head
//! stride of 2 `hs`) and the head's row is scaled by the sigmoid of that raw half; a non-empty `sinks` puts head hh's sink logit in
//! its softmax
def fa_oracle(qh, kh, vh : array<float>; rows, w0, nheads, hs, kv_mul, kvd : int; scale : float; window : int; gated : bool;
              sinks : array<float>; cap : float = 0.0) : array<float> {
    let qd = nheads * hs
    let qhs = gated ? 2 * hs : hs
    let qrow = nheads * qhs
    let kpos = w0 + rows
    var qh2 <- [for (i in range(rows * qrow)); float(float16(qh[i] * scale))]
    var kh2 <- [for (i in range(kpos * kvd)); float(float16(kh[i]))]
    var vh2 <- [for (i in range(kpos * kvd)); float(float16(vh[i]))]
    var y_ref : array<float>
    y_ref |> resize(rows * qd)
    for (r in range(rows)) {
        for (hh in range(nheads)) {
            let cnt = w0 + r + 1
            attn_row_oracle(qh2, r * qrow + hh * qhs, kh2, 0, vh2, 0, kvd, hh, hs, kv_mul, cnt, 1.0, y_ref, r * qd + hh * hs,
                window > 0 && cnt > window ? cnt - window : 0, cap, !empty(sinks), empty(sinks) ? 0.0 : sinks[hh])
            if (gated) {
                for (i in range(hs)) {
                    y_ref[r * qd + hh * hs + i] *= sigmoid_f32(qh[r * qrow + hh * qhs + hs + i])
                }
            }
        }
    }
    delete qh2
    delete kh2
    delete vh2
    return <- y_ref
}

//! the q halves of a [q | gate] panel whose heads sit `qhs` apart: the rows `fa_oracle` reads ungated
def q_halves(qh : array<float>; rows, nheads, hs, qhs : int) : array<float> => <- [for (i in range(rows * nheads * hs)); qh[(i / hs) * qhs + i % hs]]

//! one attention sink logit a head for a single query row: past the head's top scaled score over `keys` keys by 2.5 + 0.5 x the head,
//! so the phantom key carries measurable mass over a flat key row
def sink_logits(q, kk : array<float>; nheads, hs, kv_mul, keys, kvd : int; scale : float) : array<float> {
    var sinkh : array<float>
    sinkh |> resize(nheads)
    for (hh in range(nheads)) {
        let khoff = (hh / kv_mul) * hs
        var smax = -3.0e38
        for (j in range(keys)) {
            var sc = 0.0
            for (e in range(hs)) {
                sc += q[hh * hs + e] * kk[j * kvd + khoff + e]
            }
            smax = max(smax, sc * scale)
        }
        sinkh[hh] = smax + 2.5 + 0.5 * float(hh)
    }
    return <- sinkh
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
