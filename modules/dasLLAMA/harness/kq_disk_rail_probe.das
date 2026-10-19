options gen2
options stack = 524288
options _dasllama_internal = true

// ATTRIBUTION SWEEP - the rare-experts rail's CPU cost by weight layout, one format (iq3s4, Flash-Next's gate and
// up stacks) over one synthetic expert (d rows of n): the tuned grp<mr> body over a plane repacked once (the serving
// form), the same body with the repack paid on every read (an arena in disk order, interleaved into a scratch as
// the rail reads it), and the untuned portable body over the disk order itself (the body a device-layout twin
// starts from). Each form at 1, 2 and 4 activation rows an expert - the rail's shape - as microseconds an expert, the
// forms' rows held to each other. Args: [n] [d] (default 2560 512).

require strings
require math
require dasllama/dasllama_math
require dasllama/dasllama_math_gen
require dasllama/dasllama_kqformat
require ../tests/_kq_fixtures.das

let private ROUNDS = 100
let private FMT_IQ3S4 = 35

var private g_exit = 1

//! one row-call of the active backend's rows core over every row of the expert, for activation row `acts`
def private gemv_rows(kq, ks : array<uint8>; acts : Q8kActs; var y : array<float>; t, n, d : int64) {
    let fn = kq_rows_fn(FMT_IQ3S4)
    unsafe {
        invoke(fn, addr(y[t * d]), addr<uint8 const?>(kq[0]), addr<uint8 const?>(ks[0]),
            addr<int8 const?>(acts.xq[0]), addr<float const?>(acts.xs[0]), addr<int const?>(acts.xbs[0]), n, 0l, d)
    }
}

//! the engine's reference row dot over the disk order (mr 1, every row its own group): the untuned body a twin starts from
def private gemv_disk_ref(kq, ks : array<uint8>; acts : Q8kActs; var y : array<float>; t, n, d : int64) {
    let nsb = n / 256l
    let qsb = kq_qsb(KqFmt.iq3s4)
    let ssb = kq_ssb(KqFmt.iq3s4)
    unsafe {
        for (i in range64(d)) {
            y[t * d + i] = kq_grp_row_dot_rt(KqFmt.iq3s4, addr<uint8 const?>(kq[i * nsb * qsb]), addr<uint8 const?>(ks[i * nsb * ssb]), 0l, 1l,
                addr<int8 const?>(acts.xq[0]), addr<float const?>(acts.xs[0]), addr<int const?>(acts.xbs[0]), n)
        }
    }
}

//! the mean of `rounds` timed walks of `body`, as microseconds, after three warm walks
def private mean_us(rounds : int; body : block<() : void>) : double {
    for (_w in range(3)) {
        invoke(body)
    }
    let t0 = ref_time_ticks()
    for (_r in range(rounds)) {
        invoke(body)
    }
    return double(get_time_usec(t0)) / double(rounds)
}

def private max_rel(a, b : array<float>) : float {
    var m = 0.0
    var scale = 1e-6
    for (v in b) {
        scale = max(scale, abs(v))
    }
    for (x, y in a, b) {
        m = max(m, abs(x - y) / scale)
    }
    return m
}

def private run_probe(n, d : int64) {
    var disk_q, disk_s : array<uint8>
    synth_kq_region(FMT_IQ3S4, disk_q, disk_s, d, n, 0x1234u, 0x85EBCA6Bu)
    let qbytes = long_length(disk_q)
    let sbytes = long_length(disk_s)
    select_matmul_backend_for_load_()
    let tuned = active_kernel_backend()
    print("iq3s4 expert {d} x {n}: {qbytes + sbytes} bytes; tuned backend '{tuned}', rows core {kq_rows_fn(FMT_IQ3S4) == null ? "unset" : "set"}\n")
    // the serving form: the plane repacked once into the backend's grp<mr> layout
    var packed_q := disk_q
    var packed_s := disk_s
    repack_kq_weight(FMT_IQ3S4, packed_q, packed_s, 0l, n, d)
    var scratch_q, scratch_s : array<uint8>
    scratch_q |> resize(qbytes)
    scratch_s |> resize(sbytes)
    for (ntok in [1l, 2l, 4l]) {
        var inscope acts <- [for (t in range64(ntok)); build_q8k_acts(1l, n, Wave(a = 0.7, fa = 0.013 + 0.002 * float(t), b = 0.4, fb = 0.031))]
        var inscope y_packed, y_read, y_disk : array<float>
        y_packed |> resize(ntok * d)
        y_read |> resize(ntok * d)
        y_disk |> resize(ntok * d)
        let us_packed = mean_us(ROUNDS) $ {
            for (t in range64(ntok)) {
                gemv_rows(packed_q, packed_s, acts[t], y_packed, t, n, d)
            }
        }
        let us_repack = mean_us(ROUNDS) $ {
            unsafe {
                memcpy(addr<void?>(scratch_q[0]), addr<void const?>(disk_q[0]), qbytes)
                memcpy(addr<void?>(scratch_s[0]), addr<void const?>(disk_s[0]), sbytes)
            }
            repack_kq_weight(FMT_IQ3S4, scratch_q, scratch_s, 0l, n, d)
        }
        let us_read = mean_us(ROUNDS) $ {
            unsafe {
                memcpy(addr<void?>(scratch_q[0]), addr<void const?>(disk_q[0]), qbytes)
                memcpy(addr<void?>(scratch_s[0]), addr<void const?>(disk_s[0]), sbytes)
            }
            repack_kq_weight(FMT_IQ3S4, scratch_q, scratch_s, 0l, n, d)
            for (t in range64(ntok)) {
                gemv_rows(scratch_q, scratch_s, acts[t], y_read, t, n, d)
            }
        }
        let us_disk = mean_us(ROUNDS) $ {
            for (t in range64(ntok)) {
                gemv_disk_ref(disk_q, disk_s, acts[t], y_disk, t, n, d)
            }
        }
        print("rows {ntok}: tuned grp8 (repacked once) {us_packed} us | repack on read {us_repack} us + gemv = {us_read} us | reference disk order {us_disk} us | rows agree: read vs packed {max_rel(y_read, y_packed)}, disk vs packed {max_rel(y_disk, y_packed)}\n")
    }
    g_exit = 0
}

[export]
def main : int {
    var n = 2560l
    var d = 512l
    var i = 0
    for (a in get_command_line_arguments()) {
        let v = to_int(a)
        if (v > 0) {
            if (i == 0) {
                n = int64(v)
            } else {
                d = int64(v)
            }
            i++
        }
    }
    run_probe(n, d)
    return g_exit
}
