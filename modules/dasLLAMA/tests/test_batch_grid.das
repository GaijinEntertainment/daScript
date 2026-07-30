options gen2

require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_math_default        // portable backend (knob-inert control)
require dasllama/dasllama_math_aarch64_neon   // arm64 backends (registered under arm64 + JIT only)
require ?llvm/daslib/llvm_tune dasllama/dasllama_math_gen  // nolint:STYLE030 — the gen batch walk (the knob's target)
require dasllama/dasllama_convert
require daslib/jobque_boost
require math

// The 2-D batch chunk grid (batch_grid_2d) must be BIT-FOR-BIT the 1-D walk: only chunk
// boundaries move, never the per-(token,row) dot fold. Every registered backend runs the same
// batch under knob 0/1/2 and the outputs must match exactly (portable/sdot ignore the knob —
// trivial equality; the gen batch walk is the real subject). target_chunk_work=1 admits the
// full lane pool so the starved shapes engage the grid on any box with enough lanes; a tiny
// q8_token_block exercises the TB-block loop INSIDE a token cell.

def private restore_grid_knobs() {
    set_batch_grid_2d(0)
    set_target_chunk_work(1_000_000l)
    set_q8_token_block(128l)
}

def private run_batch(var y : array<float>; wq : array<int8>; ws : array<float>; xq : array<int8>; xs : array<float>; n, d, ntok : int64; knob : int) {
    set_batch_grid_2d(knob)
    matmul_q8q8_batch(y, wq, ws, xq, xs, n, d, ntok)
    set_batch_grid_2d(0)
}

def private mismatches(a, b : array<float>) : int {
    var bad = 0
    for (va, vb in a, b) {
        if (va != vb) {
            bad++
        }
    }
    return bad
}

def private check_shape(t : T?; n, d, ntok, tb : int64) {
    let nb = n / 32l
    var wf : array<float>
    var xf : array<float>
    wf |> resize(n * d)
    xf |> resize(ntok * n)
    for (i in range64(n * d)) {
        wf[i] = sin(float(i) * 0.11f)
    }
    for (i in range64(ntok * n)) {
        xf[i] = cos(float(i) * 0.07f)
    }
    let wq0 <- quantize_q8_0(wf, n * d)
    var xq : array<int8>
    var xs : array<float>
    xq |> resize(ntok * n)
    xs |> resize(ntok * nb)
    var xt : array<float>
    xt |> resize(n)
    for (tk in range64(ntok)) {
        for (i in range64(n)) {
            xt[i] = xf[tk * n + i]
        }
        quantize_q8_0_into(xt, n, xq, xs, tk * n, tk * nb)
    }
    set_q8_token_block(tb)
    set_target_chunk_work(1l)   // admit every lane — the small d is then chunk-starved for real
    let saved = active_kernel_backend()
    for (name in kernel_backend_names()) {
        pin_kernel_backend(name)
        var wq : array<int8>
        var ws : array<float>
        wq := wq0.q
        ws := wq0.scales
        if (kernel_backend_needs_repack(name)) {
            repack_q8q8_weight(wq, ws, 0l, n, d)
        }
        var y0 : array<float>
        var y1 : array<float>
        var y2 : array<float>
        y0 |> resize(ntok * d)
        y1 |> resize(ntok * d)
        y2 |> resize(ntok * d)
        run_batch(y0, wq, ws, xq, xs, n, d, ntok, 0)
        run_batch(y1, wq, ws, xq, xs, n, d, ntok, 1)
        run_batch(y2, wq, ws, xq, xs, n, d, ntok, 2)
        t |> equal(mismatches(y1, y0), 0, "fine grid == 1-D ({n}x{d} ntok={ntok} tb={tb}, backend {name})")
        t |> equal(mismatches(y2, y0), 0, "wave-aligned grid == 1-D ({n}x{d} ntok={ntok} tb={tb}, backend {name})")
        delete wq
        delete ws
        delete y0
        delete y1
        delete y2
    }
    clear_kernel_backend_pin()
    if (saved != "") {
        select_kernel_backend(saved)
    }
    restore_grid_knobs()
    delete wf
    delete xf
    delete xq
    delete xs
    delete xt
}

[test]
def test_batch_grid_bit_exact(t : T?) {
    with_job_que() {
        setup_dasllama_jobque_()
        restore_grid_knobs()
        t |> run("starved 64x64, ragged token tail (33 = 8 tiles + 1)") @(t : T?) {
            check_shape(t, 64l, 64l, 33l, 128l)
        }
        t |> run("row tail (d=40), two fine token cells (ntok=20)") @(t : T?) {
            check_shape(t, 96l, 40l, 20l, 128l)
        }
        t |> run("token cell wider than TB (tb=8): the in-cell TB loop") @(t : T?) {
            check_shape(t, 64l, 64l, 33l, 8l)
        }
    }
}
