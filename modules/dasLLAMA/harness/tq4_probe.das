options gen2

require math

// TurboQuant stage-1 math probe: seeded-sign FWHT rotation + two 4-bit grids
// (uniform amax, q4_0-style vs Lloyd-Max on per-block sigma), per-32 block scales.
// Validates: FWHT identities, self-computed Lloyd table vs Max-1960 constants,
// round-trip MSE and Q.K dot error on gaussian + persistent-outlier-channel data.
// Usage: bin/daslang modules/dasLLAMA/harness/tq4_probe.das

let HEAD_DIM = 128
let N_ROWS = 2048
let BLK = 32

// Max (1960) optimal 16-level quantizer for N(0,1), positive half
let LLOYD16_REF = fixed_array(0.1284f, 0.3881f, 0.6568f, 0.9424f, 1.2562f, 1.6181f, 2.0690f, 2.7326f)

def rng_u(var s : uint&) : float {
    s = s * 1664525u + 1013904223u
    return float(s >> 8u) * (1.0f / 16777216.0f)
}

def rng_gauss(var s : uint&) : float {
    let u1 = max(rng_u(s), 1e-7f)
    let u2 = rng_u(s)
    return sqrt(-2.0f * log(u1)) * cos(6.2831853f * u2)
}

def f16r(x : float) : float => float(half(x))

// ===== FWHT (orthonormal, self-inverse) =====

def fwht_inplace(var v : array<float>) {
    let n = length(v)
    var h = 1
    while (h < n) {
        var i = 0
        while (i < n) {
            for (j in range(i, i + h)) {
                let a = v[j]
                let b = v[j + h]
                v[j] = a + b
                v[j + h] = a - b
            }
            i += h * 2
        }
        h *= 2
    }
    let inv = 1.0f / sqrt(float(n))
    for (x in v) {
        x *= inv
    }
}

def make_signs(n : int; seed : uint) : array<float> {
    var s = seed
    return <- [for (_i in range(n)); rng_u(s) < 0.5f ? -1.0f : 1.0f]
}

def rotate_row(var v : array<float>; signs : array<float>) {
    for (x, s in v, signs) {
        x *= s
    }
    fwht_inplace(v)
}

def unrotate_row(var v : array<float>; signs : array<float>) {
    fwht_inplace(v)
    for (x, s in v, signs) {
        x *= s
    }
}

// ===== 4-bit grids, per-32 block scale (fp16-rounded) =====

def quant_uniform(src : array<float>; var dst : array<float>) {
    var i0 = 0
    while (i0 < length(src)) {
        var amax = 0.0f
        for (j in range(i0, i0 + BLK)) {
            amax = max(amax, abs(src[j]))
        }
        let d = amax > 0.0f ? f16r(amax / 8.0f) : 0.0f
        let id = d > 0.0f ? 1.0f / d : 0.0f
        for (j in range(i0, i0 + BLK)) {
            let q = clamp(int(floor(src[j] * id + 0.5f)), -8, 7)
            dst[j] = float(q) * d
        }
        i0 += BLK
    }
}

def quant_lloyd(src : array<float>; var dst : array<float>; cb : array<float>) {
    var i0 = 0
    while (i0 < length(src)) {
        var ss = 0.0f
        for (j in range(i0, i0 + BLK)) {
            ss += src[j] * src[j]
        }
        let sigma = f16r(sqrt(ss / float(BLK)))
        let isigma = sigma > 0.0f ? 1.0f / sigma : 0.0f
        for (j in range(i0, i0 + BLK)) {
            let u = src[j] * isigma
            var best = 0
            var bestd = abs(u - cb[0])
            for (k in range(1, 16)) {
                let dd = abs(u - cb[k])
                if (dd < bestd) {
                    bestd = dd
                    best = k
                }
            }
            dst[j] = cb[best] * sigma
        }
        i0 += BLK
    }
}

// ===== Lloyd-Max on N(0,1), discrete grid =====

def compute_lloyd16 : array<float> {
    let NG = 160001
    let X0 = -8.0lf
    let DX = 16.0lf / double(NG - 1)
    var xs : array<double>
    var ws : array<double>
    xs |> resize(NG)
    ws |> resize(NG)
    for (i in range(NG)) {
        let x = X0 + double(i) * DX
        xs[i] = x
        ws[i] = exp(-0.5lf * x * x)
    }
    var c <- [for (k in range(16)); -3.0lf + 6.0lf * (double(k) + 0.5lf) / 16.0lf]
    for (_it in range(2000)) {
        var wsum : double[16]
        var xsum : double[16]
        var cell = 0
        for (i in range(NG)) {
            let x = xs[i]
            while (cell < 15 && x > (c[cell] + c[cell + 1]) * 0.5lf) {
                cell++
            }
            wsum[cell] += ws[i]
            xsum[cell] += ws[i] * x
        }
        for (k in range(16)) {
            if (wsum[k] > 0.0lf) {
                c[k] = xsum[k] / wsum[k]
            }
        }
    }
    return <- [for (k in range(16)); float(c[k])]
}

def table_mse(cb : array<float>) : double {
    // E[(x - nearest(cb, x))^2] under N(0,1), fine grid
    let NG = 160001
    let X0 = -8.0lf
    let DX = 16.0lf / double(NG - 1)
    var esum = 0.0lf
    var wsum = 0.0lf
    for (i in range(NG)) {
        let x = X0 + double(i) * DX
        let w = exp(-0.5lf * x * x)
        var bestd = 1e30lf
        for (k in range(16)) {
            let dd = (x - double(cb[k])) * (x - double(cb[k]))
            if (dd < bestd) {
                bestd = dd
            }
        }
        esum += w * bestd
        wsum += w
    }
    return esum / wsum
}

// ===== experiments =====

def gen_row(var v : array<float>; var seed : uint&; outliers : bool) {
    for (x in v) {
        x = rng_gauss(seed)
    }
    if (outliers) {
        v[7] *= 25.0f
        v[77] *= 25.0f
    }
}

def row_dot(a, b : array<float>) : float {
    var s = 0.0f
    for (x, y in a, b) {
        s += x * y
    }
    return s
}

def row_norm(a : array<float>) : float => sqrt(row_dot(a, a))

struct Acc {
    err2 : double
    pow : double
    dot2 : double
    ndot : int
}

def run_model(name : string; outliers : bool; cb : array<float>; signs : array<float>) {
    // configs: 0 = direct uniform, 1 = direct lloyd, 2 = rot uniform, 3 = rot lloyd
    var acc : Acc[4]
    var seed = 0x12345u
    var k : array<float>
    var q : array<float>
    var work : array<float>
    var dq : array<float>
    k |> resize(HEAD_DIM)
    q |> resize(HEAD_DIM)
    work |> resize(HEAD_DIM)
    dq |> resize(HEAD_DIM)
    for (_row in range(N_ROWS)) {
        gen_row(k, seed, outliers)
        gen_row(q, seed, false)
        let tru = row_dot(q, k)
        let qk = row_norm(q) * row_norm(k)
        for (cfg in range(4)) {
            let rot = cfg >= 2
            for (i in range(HEAD_DIM)) {
                work[i] = k[i]
            }
            if (rot) {
                rotate_row(work, signs)
            }
            if ((cfg & 1) == 0) {
                quant_uniform(work, dq)
            } else {
                quant_lloyd(work, dq, cb)
            }
            var e2 = 0.0lf
            var p2 = 0.0lf
            for (i in range(HEAD_DIM)) {
                let e = double(dq[i] - work[i])
                e2 += e * e
                p2 += double(work[i] * work[i])
            }
            acc[cfg].err2 += e2
            acc[cfg].pow += p2
            var est = 0.0f
            if (rot) {
                for (i in range(HEAD_DIM)) {
                    work[i] = q[i]
                }
                rotate_row(work, signs)
                est = row_dot(work, dq)
            } else {
                est = row_dot(q, dq)
            }
            let de = double((est - tru) / qk)
            acc[cfg].dot2 += de * de
            acc[cfg].ndot++
        }
    }
    print("model {name}\n")
    let names = fixed_array("direct+uniform", "direct+lloyd  ", "rot+uniform   ", "rot+lloyd     ")
    for (cfg in range(4)) {
        let mse = acc[cfg].err2 / acc[cfg].pow
        let dotrms = sqrt(acc[cfg].dot2 / double(acc[cfg].ndot))
        print("  {names[cfg]}  rel-mse {mse}  dot-rms-err {dotrms}\n")
    }
    delete k
    delete q
    delete work
    delete dq
}

[export]
def main {
    // Lloyd table: computed vs Max-1960 reference
    var cb <- compute_lloyd16()
    print("lloyd16 computed vs ref (positive half):\n")
    for (k in range(8)) {
        let c = cb[8 + k]
        let r = LLOYD16_REF[k]
        print("  {c}  ref {r}  diff {c - r}\n")
    }
    var cbref <- [for (k in range(16)); k < 8 ? -LLOYD16_REF[7 - k] : LLOYD16_REF[k - 8]]
    print("table mse: computed {table_mse(cb)}  ref {table_mse(cbref)}\n")
    delete cbref

    let signs <- make_signs(HEAD_DIM, 0xC0FFEEu)

    // FWHT identities
    {
        var seed = 777u
        var x : array<float>
        var y : array<float>
        x |> resize(HEAD_DIM)
        y |> resize(HEAD_DIM)
        gen_row(x, seed, true)
        gen_row(y, seed, false)
        let d0 = row_dot(x, y)
        var xr := x
        var yr := y
        rotate_row(xr, signs)
        rotate_row(yr, signs)
        let d1 = row_dot(xr, yr)
        unrotate_row(xr, signs)
        var maxd = 0.0f
        for (i in range(HEAD_DIM)) {
            maxd = max(maxd, abs(xr[i] - x[i]))
        }
        print("fwht: dot-preserve diff {d1 - d0} (of {d0}), roundtrip max diff {maxd}\n")
        delete x
        delete y
        delete xr
        delete yr
    }

    run_model("gauss iid", false, cb, signs)
    run_model("outlier ch7,77 x25", true, cb, signs)
    delete cb
}
