// The rows8 matrix-unit form's layout probe the Metal kernel cells share: a cell on the form skips where the box's cooperative
// layout is another generation's, as the engine keeps the form off there.

options gen2
options _dasllama_internal = true

module _rows8_probe shared public

require dasllama/dasllama_lint public
require dastest/testing_boost
require ?das_metal dasllama/dasllama_metal_prefill   // enc_rows8_probe_c: the probe stamp
require ?das_metal _metal_kernel_common   // the MetalCell record, kernel_of

//! the rows8 matrix-unit form's layout probe on this box: the probe stamp over the engine's two one-hot tables; false where the
//! cooperative layout is another generation's (the engine keeps the form off there too), null pipeline where Metal 4 is absent;
//! `mc` a `MetalCell`, untyped so the signature names nothing of the optional module
def public rows8_probe_ok(t : T?; var mc; var err : string&) : bool {
    let kb = kernel_of(@@enc_rows8_probe_c)
    var pso = mc |> keep_stamp(kb, err)
    return false if (pso == null)
    var good = true
    for (tab in range(2)) {   //! two one-hot tables: hot k rising by 7 from 3, falling by 7 from 60
        var inscope k0a <- [for (tk in range(8)); uint(tab == 0 ? 7 * tk + 3 : 60 - 7 * tk)]
        var inscope xt : array<float16>
        xt |> resize(8 * 64)
        for (tk in range(8)) {
            xt[tk * 64 + int(k0a[tk])] = float16(1.0)
        }
        let bxt = mc |> in_plain(xt)
        let bk0 = mc |> in_plain(k0a)
        let bok = mc |> out_words(32, 0u)
        let ran = mc |> cell_record(t, "rows8 probe") $(enc : MetalComputeEncoder?) {
            invoke(kb.enc, enc, pso, mc.buf[bxt], mc.buf[bk0], mc.buf[bok])
        }
        return false if (!ran)
        var inscope oks <- mc |> plane_as(bok, type<uint>, 32)
        for (v in oks) {
            good = good && v == 1u
        }
    }
    return good
}
