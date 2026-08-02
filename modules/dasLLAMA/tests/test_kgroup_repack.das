options gen2
require dastest/testing_boost public
require dasllama/dasllama_math_gen
require dasllama/dasllama_repack
require dasllama/dasllama_convert

// The kg8 plane rail (slice J — the smmla stamp's row-pair layout): repack_q8q8_grp with
// kgroup=8 must write [kg8][r][8 bytes] groups, and q8q8_token_grp_generic must read kg4 and
// kg8 planes to the SAME result (identical int accumulators + identical fold order => exact
// float equality). Pure das — proves the layout contract on any box, no i8mm needed.

def build_q8(n, d : int64; var q : array<int8>; var s : array<float>) {
    var w : array<float>
    w |> resize(n * d)
    for (i in range64(n * d)) {
        w[i] = float(i % 19l) * 0.11 - 0.9
    }
    let qt <- quantize_q8_0(w, n * d)
    q |> resize(n * d)
    s |> resize(d * (n / 32l))
    for (i in range64(n * d)) {
        q[i] = qt.q[i]
    }
    for (i in range64(d * (n / 32l))) {
        s[i] = qt.scales[i]
    }
    delete w
}

[test]
def test_kg8_plane_bytes(t : T?) {
    t |> run("kg8 repack writes [kg8][r][8b] groups; tails stay row-major") @(t : T?) {
        let n = 96l    // nb = 3 (odd block count)
        let d = 20l    // mr=8 -> 2 groups + 4 row-major tail rows
        let mr = 8l
        var q : array<int8>
        var s : array<float>
        build_q8(n, d, q, s)
        var qr := q    // row-major master
        unsafe {
            repack_q8q8_grp(addr(q[0]), addr(s[0]), n, d, mr, 0l, 8l)
        }
        var bytesOk = true
        for (g in range64(d / mr)) {
            let gbase = g * mr * n
            for (kg in range64(n / 8l)) {
                for (r in range64(mr)) {
                    for (j in range64(8l)) {
                        bytesOk &&= q[gbase + kg * 8l * mr + r * 8l + j] == qr[(g * mr + r) * n + kg * 8l + j]
                    }
                }
            }
        }
        t |> success(bytesOk, "kg8 group bytes match the row-major master")
        var tailOk = true
        for (i in range64((d / mr) * mr * n, d * n)) {
            tailOk &&= q[i] == qr[i]
        }
        t |> success(tailOk, "row tail stays row-major")
        delete q
        delete s
        delete qr
    }
}

[test]
def test_kg8_generic_dot_parity(t : T?) {
    t |> run("token generic reads kg4 and kg8 planes to the exact same result") @(t : T?) {
        let n = 96l
        let d = 16l
        var q : array<int8>
        var s : array<float>
        build_q8(n, d, q, s)
        var xrow : array<float>
        var xq : array<int8>
        var xs : array<float>
        xrow |> resize(int(n))
        xq |> resize(int(n))
        xs |> resize(int(n / 32l))
        for (i in range64(n)) {
            xrow[i] = float(i % 23l) * 0.07 - 0.8
        }
        quantize_q8_0_into(xrow, n, xq, xs, 0l, 0l)
        for (mr in [4l, 8l]) {
            var q4 := q
            var s4 := s
            var q8v := q
            var s8v := s
            unsafe {
                repack_q8q8_grp(addr(q4[0]), addr(s4[0]), n, d, mr, 0l, 4l)
                repack_q8q8_grp(addr(q8v[0]), addr(s8v[0]), n, d, mr, 0l, 8l)
                var y4 : array<float>
                var y8 : array<float>
                y4 |> resize(int(d))
                y8 |> resize(int(d))
                for (g in range64(d / mr)) {
                    q8q8_token_grp_generic(addr(y4[0]), addr(q4[g * mr * n]), addr(s4[g * mr * (n / 32l)]),
                        addr(xq[0]), addr(xs[0]), n, d, g * mr, 0l, mr, 0l, 4l)
                    q8q8_token_grp_generic(addr(y8[0]), addr(q8v[g * mr * n]), addr(s8v[g * mr * (n / 32l)]),
                        addr(xq[0]), addr(xs[0]), n, d, g * mr, 0l, mr, 0l, 8l)
                }
                var same = true
                for (i in range64((d / mr) * mr)) {
                    same &&= y4[i] == y8[i]
                }
                t |> success(same, "kg4/kg8 outputs identical at mr {mr}")
                delete y4
                delete y8
            }
            delete q4
            delete s4
            delete q8v
            delete s8v
        }
        delete q
        delete s
        delete xrow
        delete xq
        delete xs
    }
}
