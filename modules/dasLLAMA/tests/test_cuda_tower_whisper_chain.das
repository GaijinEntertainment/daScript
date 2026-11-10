options gen2
options persistent_heap
options indenting = 4
options _dasllama_internal = true

// The rail primitives the whisper decoder is the first chain to run, on the CUDA rail against CPU references: the q8 GEMV over one
// record's region pairs narrowed out of a larger schedule plane (`rail_gemv_q8` through `rail_view`), and the plane copy and zero
// (`rail_copy`, `rail_zero`); and the GEMM sites' schedule bind (`rail_kq_map`). Model-free; every device cell skips without a
// CUDA device and NVRTC.

require dastest/testing_boost public
require math
require dasllama/dasllama_lint public
require dasllama/dasllama_cuda_common
require dasllama/dasllama_cuda_classes
require dasllama/dasllama_cuda_rail
require dasllama/dasllama_tower_rail
require dasllama/dasllama_gpu_kernels_discrete
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_audio   // log_mel_chunks
require dasllama/dasllama_qwen3a   // log_mel_qwen3a
require _gpu_knobs
require _cuda_cell
require _compares
require _kernel_oracles   // q8_gemm_oracle
require _vk_kq_fixtures   // gemv_fix, ramp_ws_plane: the q8 GEMV cells' planes

let private GEMV_BAR = Bar(rel = 1e-3, poison_of_max = true, poison = 2.0, poison_abs = 1.0)   //! the q8 GEMV cells' bar: 1e-3 of the larger magnitude floored at 1
let private META_OFF = 512l     //! the GEMV site's schedule offset: two alignment units of foreign records ahead of it
let private REC_WORDS = 64u     //! the record's map past the site's offset: one more alignment unit

//! the rail over the cell's planes: the GEMV site's planes at the block sites' roles, its schedule from `META_OFF`
def private gemv_rail(cp : CuPlanes; wq, ws, meta, xq, xs, y : int; meta_bytes : int64) : CutRail {
    var rl : CutRail
    rl.stream = g_cud.stream
    rail_binds_reset(rl)
    rl.binds[int(RailSite.gemv)].meta_off = META_OFF
    rail_seat(rl, TowerBuf.wq, cp.dev[wq], 1l, 0u)
    rail_seat(rl, TowerBuf.ws, cp.dev[ws], 1l, 0u)
    rail_seat(rl, TowerBuf.meta, cp.dev[meta], meta_bytes, 0u)
    rail_seat(rl, TowerBuf.xq, cp.dev[xq], 1l, 0u)
    rail_seat(rl, TowerBuf.xs, cp.dev[xs], 1l, 0u)
    rail_seat(rl, TowerBuf.logits, cp.dev[y], 1l, 0u)
    rl.maps <- [0u, REC_WORDS]
    return <- rl
}

//! `nreg` regions of `d` rows over `nb` blocks, the record one alignment unit past the site's offset amid zero words: a missed view reads region 0 for every region
def private rail_gemv_cell(t : T?; nreg, d, nb : int) {
    let n = nb * 32
    let rows = nreg * d
    let tag = "rail_gemv_q8 n {n} d {d} x {nreg}"
    var wsh <- ramp_ws_plane(nreg * d * nb)
    var f <- gemv_fix(nreg, d, nb, 8, 55u, wsh, nb, 8, 17u, 9u, -0.25, 0.125, 5)
    var y_ref : array<float>
    y_ref |> resize(rows)
    var rec <- [0u, 0u, 1u, 0u]
    q8_gemm_oracle(f.wqh, f.wsh, f.xqh, f.xsh, rec, 1, n, rows, y_ref)
    let at = int(META_OFF / 4l) + int(REC_WORDS)
    var meta : array<uint>
    meta |> resize(at + int(RAIL_GEMV_META_BYTES / 4l))
    for (i in range(length(f.regs))) {
        meta[at + i] = f.regs[i]
    }
    with_cu_planes() $(var cp) {
        let wq = cp |> in_plain(f.wqh)
        let ws = cp |> in_plain(f.wsh)
        let mp = cp |> in_plain(meta)
        let xq = cp |> in_plain(f.xqh)
        let xs = cp |> in_plain(f.xsh)
        let y = cp |> out_(rows)
        var rl <- gemv_rail(cp, wq, ws, mp, xq, xs, y, long_length(meta) * 4l)
        rail_gemv_q8(rl, RailSite.gemv, 1l, TowerBuf.logits, int64(n), int64(d), int64(nreg), 1l)
        cu_launched(t, tag)
        var got <- plane_run(cp, y)
        check_rows(t, "{tag} vs the CPU oracle", got, y_ref, GEMV_BAR, 1)
        let seat = rail_buf(rl, TowerBuf.meta)
        t |> success(seat.off == 0l && seat.bytes == long_length(meta) * 4l, "{tag}: the schedule's seat is put back whole after the view")
        plane_refill(cp, y)
        rail_gemv_q8(rl, RailSite.gemv, 0l, TowerBuf.logits, int64(n), int64(d), int64(nreg), 1l)
        cu_launched(t, "{tag}, record 0")
        var miss <- plane_run(cp, y)
        if (nreg > 1) {
            check_exceeds(t, "{tag}: the view at record 0's map reads the zero words, not the record (the control)", miss, y_ref, GEMV_BAR)
        }
        delete miss
        delete got
        delete rl
    }
    delete meta
    delete rec
    delete y_ref
    delete f
}

[test(description = "CUDA `rail_gemv_q8` viewed onto one record's region pairs inside a larger schedule plane matches the CPU q8 oracle within 1e-3, restores the schedule seat whole, and a view at record 0 reads zero words instead (control)")]
def test_cud_rail_gemv_q8(t0 : T?) {
    t0 |> run("CUDA rail_gemv_q8: one record's region pairs viewed out of the schedule == the CPU oracle") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, dasllama_cuda_classes::ensure_q8_gemv_cls(), "the q8 GEMV stamp"))
        rail_gemv_cell(t, 3, 5, 8)
        rail_gemv_cell(t, 2, 64, 16)
    }
}

//! `rail_kq_map` over a rail whose `site` binds the schedule at `off`: the record's map word, and `refusal` "refused" where it panicked
def private kq_map_or_refusal(site : RailSite; off : int64) : tuple<map : uint; refusal : string> {
    var rl : CutRail
    rail_binds_reset(rl)
    rl.binds[int(site)].meta_off = off
    rl.maps <- [0u, REC_WORDS]
    var map = 0u
    var refusal = ""
    try {
        map = rail_kq_map(rl, site, 1l)
    } recover {
        refusal = "refused"
    }
    delete rl
    return (map = map, refusal = refusal)
}

[test(description = "`rail_kq_map` on a GEMM site bound at the schedule start returns the record's map word, a GEMM site bound past the start panics, and a block site bound at the start passes as the control")]
def test_cud_rail_kq_map(t0 : T?) {
    t0 |> run("rail_kq_map: a GEMM site reads its record's map from the schedule's start, and a GEMM site bound past it refuses") <| @(t : T?) {
        let whole = kq_map_or_refusal(RailSite.cross, 0l)
        t |> equal(whole.map, REC_WORDS, "the cross site at the schedule's start reads record 1's map word")
        t |> equal(whole.refusal, "", "a bind at the start passes")
        let past = kq_map_or_refusal(RailSite.cross, META_OFF)
        t |> equal(past.refusal, "refused", "the cross site bound {META_OFF} bytes in panics: its tile reads the record from the plane's first word")
        let other = kq_map_or_refusal(RailSite.d, 0l)
        t |> equal(other.refusal, "", "the control: the block site bound at the start passes")
    }
}

[test(description = "CUDA `rail_copy` moves a byte range between two planes bit for bit honoring both source and destination offsets, `rail_zero` zeroes its range, and every other word keeps the NaN sentinel")]
def test_cud_rail_copy_zero(t0 : T?) {
    t0 |> run("CUDA rail_copy and rail_zero: a byte range between two planes, a zeroed range, nothing past either") <| @(t : T?) {
        return if (!cuda_up(t))
        var src <- [for (i in range(300)); float(i) * 0.5 + 1.0]
        with_cu_planes() $(var cp) {
            let ps = cp |> in_(src)
            let pd = cp |> out_(300)
            var rl : CutRail
            rl.stream = g_cud.stream
            rail_seat(rl, TowerBuf.rows_out, cp.dev[ps], 1200l, 0u)
            rail_seat(rl, TowerBuf.xb, cp.dev[pd], 1200l, 0u)
            rail_copy(rl, TowerBuf.rows_out, 16l, TowerBuf.xb, 32l, 200l * 4l)
            rail_zero(rl, TowerBuf.xb, 1000l, 40l)
            cu_launched(t, "rail_copy / rail_zero")
            var got <- plane_as(cp, pd, type<float>, 300)
            var copied = 0
            var zeroed = 0
            var kept = 0
            var unshifted = 0
            for (i in range(300)) {
                if (i >= 8 && i < 208) {
                    copied += got[i] == src[i - 4] ? 1 : 0
                    unshifted += got[i] == src[i] ? 1 : 0
                } elif (i >= 250 && i < 260) {
                    zeroed += got[i] == 0.0 ? 1 : 0
                } else {
                    kept += is_nan(got[i]) ? 1 : 0
                }
            }
            t |> equal(copied, 200, "the copied range lands at its byte offset bit for bit, read from the source's own offset")
            t |> equal(zeroed, 10, "the zeroed range reads 0")
            t |> equal(kept, 90, "every other word keeps the sentinel")
            t |> equal(unshifted, 0, "the control: a copy that read the source at the destination's offset matches no word")
            delete got
            delete rl
        }
        delete src
    }
}

// The device log-mel (`tower_mel`, `dasllama_tower_chains_g4a`) on the home the whisper family's seats serve on - the CUDA
// home's under the CUDA ask, else the Vulkan home's - against the CPU mel over a synthetic signal: whisper's 80-mel chunks, then
// qwen3a's 128-mel window on the same residency (the scratch rebuilt for the wider filterbank). Model-free; skips without a
// discrete device the home serves.

let private MEL_BAR = Bar(abs_floor = 1e-3)   //! the device log-mel's distance from the CPU's after the global clamp: the qwen3a twin's mel bar

//! a 2.5 s signal of three tones under a slow envelope with a seeded noise floor
def private mel_signal(seed : int) : array<float> {
    var s : array<float>
    let n = 40000
    s |> reserve(n)
    var x = uint(seed)
    for (i in range(n)) {
        x = x * 1664525u + 1013904223u
        let tt = float(i) / 16000.0
        let env = 0.5 + 0.5 * sin(tt * 3.1)
        s |> push(env * (0.3 * sin(tt * 2765.0) + 0.2 * sin(tt * 5530.0 + 1.0) + 0.1 * sin(tt * 1170.0)) + 0.01 * (float(x >> 8u) / 16777216.0 - 0.5))
    }
    return <- s
}

//! the mel rows of `chunks` back to back
def private mel_rows(chunks : array<MelChunk>) : array<float> {
    var rows : array<float>
    for (c in chunks) {
        rows |> push_from(c.data)
    }
    return <- rows
}

//! `mel` over `mel_signal(7)` with the knob off (the CPU, no device mel) and on (the home's device): one device mel, no decline,
//! within `MEL_BAR`; the CPU mel of `mel_signal(11)` outside it the control
def private mel_twin(t : T?; tag : string; mel : block<(s : array<float>) : array<float>>) {
    let sig <- mel_signal(7)
    let other <- mel_signal(11)
    set_serving_gpu_tower(false, TowerFamily.whisper)
    let off0 = gpu_tower_counters(TowerFamily.whisper)
    let cpu <- invoke(mel, sig)
    let cpu_other <- invoke(mel, other)
    t |> equal(gpu_tower_counters(TowerFamily.whisper).mels - off0.mels, 0l, "{tag}: the knob-off mels ran on the CPU (the mels counter's control)")
    set_serving_gpu_tower(true, TowerFamily.whisper)
    let c0 = gpu_tower_counters(TowerFamily.whisper)
    let dev <- invoke(mel, sig)
    let c1 = gpu_tower_counters(TowerFamily.whisper)
    t |> equal(c1.mels - c0.mels, 1l, "{tag}: the device mel served (mels delta)")
    t |> equal(c1.declines, c0.declines, "{tag}: no decline")
    check_rows(t, "{tag}, mel_signal(7): the device log-mel against the CPU's", dev, cpu, MEL_BAR, 0)
    check_exceeds(t, "{tag}: the control - the CPU mel of mel_signal(11) against the device's of mel_signal(7)", dev, cpu_other, MEL_BAR)
}

[test(description = "The device log-mel on the whisper family's GPU home serves exactly one mel with no decline and lands within 1e-3 of the CPU mel for whisper 80-mel chunks and the qwen3a 128-mel window, while a different signal's CPU mel exceeds the bar")]
def test_device_mel_twin(t : T?) {
    t |> run("the device log-mel on the whisper family's home against the CPU mel: 80-mel chunks, then 128-mel rows on one residency") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
            if (!gpu_tower_tiles_up(TowerFamily.whisper)) {
                t |> skip("no discrete GPU device the whisper family's home serves (DASLLAMA_GPU=1, DASLLAMA_CUDA=1 for the CUDA home)")
                return
            }
            with_dasllama_jobque_() {
                with_gpu_knobs() {
                    gpu_tower_shutdown(TowerFamily.whisper)
                    mel_twin(t, "whisper 80-mel chunks") $(s) {
                        var inscope chunks <- log_mel_chunks(s, 80l)
                        return <- mel_rows(chunks)
                    }
                    mel_twin(t, "qwen3a 128-mel window") $(s) {
                        var inscope m <- log_mel_qwen3a(s, 128l)
                        return <- clone_to_move(m.data)
                    }
                    gpu_tower_shutdown(TowerFamily.whisper)
                }
            }
        } else {
            t |> skip("no discrete GPU home in this build (dasVulkan, dasCUDA)")
        }
    }
}
