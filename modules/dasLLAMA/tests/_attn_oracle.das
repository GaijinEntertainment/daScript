// The single-query attention oracle the Metal and the Vulkan kernel cells share - one plain two-pass softmax per (query
// row, head) over a key range, with the softcap and the attention sink in the forms the kernels take.

options gen2
options _dasllama_internal = true

module _attn_oracle shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_gpu_math   // softcap_exp - the cap in the exp form both kernel homes take

//! single-query causal attention oracle: one output row per (query row, head) - plain two-pass softmax over [kstart, cnt)
//! (a sliding layer's window starts past 0), the cap in the exp form the kernels take; the device's online form reassociates,
//! so approx bars. `has_sink` / `sink`: the head's attention sink logit (gpt-oss) joins the softmax's max and denominator as
//! a phantom key with no value row
def attn_row_oracle(q : array<float>; qbase : int; kk : array<float>; kbase0 : int;
                    vv : array<float>; vbase0 : int; kvd, hh, hs, kv_mul, cnt : int;
                    scale : float; var out : array<float>; obase : int; kstart : int = 0; cap : float = 0.0;
                    has_sink : bool = false; sink : float = 0.0) {
    let khoff = (hh / kv_mul) * hs
    var sc : array<float>
    sc |> resize(cnt)
    var mm = has_sink ? sink : -3.0e38
    for (j in range(kstart, cnt)) {
        var s = 0.0
        for (e in range(hs)) {
            s += q[qbase + e] * kk[kbase0 + j * kvd + khoff + e]
        }
        sc[j] = s * scale
        if (cap != 0.0) {
            sc[j] = softcap_exp(sc[j], cap)
        }
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
            acc += sc[j] * vv[vbase0 + j * kvd + khoff + e]
        }
        out[obase + e] = acc / tot
    }
    delete sc
}
