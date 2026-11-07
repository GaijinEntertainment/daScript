options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

//! The case bodies the tower kernel cells of both discrete homes run - `test_vulkan_tower_kernels.das` over `_vkd_oracles`'
//! `CellPlanes`, `test_cuda_tower_{front,blocks,asr}.das` and the f16 s-column cells over `_cuda_cell`'s `CuPlanes` - each
//! written once, generic over the rig: a case lays its planes with the rig's verbs, launches the home's stamp through its
//! rail overload `enc_<cls>(rl, seats(...), ...)` on the rail `cell_launch` seats the planes on, and holds what the stamp
//! wrote to its CPU oracle, its controls and the lanes past its run. The `[test]` shells, the device skip and the stamps'
//! build asserts stay each home's.
module _discrete_cells shared public

require dastest/testing_boost public
require math
require daslib/math_boost   // round_up: a dispatch's lanes
require daslib/f16_cvt
require daslib/math_bits   // uint_bits_to_float: the CTC records' plogs
require daslib/linq   // count: the slots a run left at the sentinel
require dasllama/dasllama_lint public
require dasllama/dasllama_gpu_rail
require dasllama/dasllama_gpu_kernels_common
require dasllama/dasllama_gpu_kernels_discrete
require dasllama/dasllama_kqformat   // kq_elems: the Q8_0 block width
require dasllama/dasllama_plane   // PlaneF: the CPU rms rows' weight plane
require dasllama/dasllama_tower
require dasllama/dasllama_common   // requant_rows_q8_sized
require dasllama/dasllama_whisper   // wdec_kscale
require dasllama/dasllama_gemma4a   // gemma4a_attn_scales, gemma4a_attn_past, gemma4a_softcap
require dasllama/dasllama_qwen3a   // QWEN3A_TOK_PER_CHUNK
require dasllama/dasllama_audio   // build_dft_twiddles
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan _vkd_oracles
require ?cuda dasllama/dasllama_cuda_classes
require ?cuda dasllama/dasllama_cuda_classes_front
require ?cuda dasllama/dasllama_cuda_classes_blocks
require ?cuda dasllama/dasllama_cuda_classes_asr
require ?cuda _cuda_cell
require _compares
require _kernel_oracles
require _attn_oracle
require _vad_oracle
require _rnnt_oracle
require _ctc_oracle

let F16_SENTINEL_BITS = 0x7e007e00u   //! a half plane's fill: a NaN in both halves, where the f32 word's low half would read as a legitimate zero pad
let F16_SENTINEL = 0x7e00u            //! ... one half of it
let I8_SENTINEL_BITS = 0x80808080u    //! a Q8_0 quant plane's fill: -128 in every byte, a value the requant never produces (|q| <= 127)
let I8_SENTINEL = int8(-128)          //! ... one byte of it

let private ROWS_D = 256          //! the row classes' width: four 64-wide heads, a 64-multiple
let private ROWS_N = 37           //! rows: off every tile and workgroup multiple
let private ROWS_HEADS = 4
let private ROWS_EPS = 1.0e-6
let private FM_IW = 7             //! the k3 s2 p1 conv over the 7 x 5 image gives the 4 x 3 output
let private FM_IH = 5
let private FM_CIN = 3
let private FM_CHUNKS = 2
let private FM_OW = int(conv_s2_out(int64(FM_IW)))
let private FM_OH = int(conv_s2_out(int64(FM_IH)))
let private FM_KDP = 64           //! the column row's padded width: cin x 9 = 27 live columns, the rest zero
let private MEL_NFFT = 64
let private MEL_NBINS = 33
let private MEL_NLEN = 5
let private MEL_NMEL = 6
let private MEL_MAX_FLOOR_G4A = 0.001                  //! gemma4a's floor under the max
let private MEL_FLOOR_24 = 5.960464477539063e-08       //! the 2^-24 floor canary adds and the whisper preprocessors max against
let private MEL_LOG10_E = 0.43429448190325176
let private NORM_NLEN = 9
let private NORM_VALID = 6
let private DW_CH = 6
let private DW2D_IH = 11          //! the depthwise conv2d image: odd along one axis, even along the other, so each edge drops its own taps
let private DW2D_IW = 10
let private DW2D_CH = 8
let private REL_D = 64
let private REL_PAD_ROWS = 3      //! the rel table's zero rows past 2 tt - 1 on the padded leg
let private CN_CONV_KERNEL = 9    //! canary's depthwise taps (centered)
let private G4A_PAST = int(gemma4a_attn_past())   //! the RPE table carries past + 1 rows
let private G4A_CAP = gemma4a_softcap()
let private WDEC_HEADS = 3
let private WDEC_TSTRIDE = 300    //! the K/V planes' positions a head: two 256-key chunks, the second partial
let private WDEC_P0 = 290         //! the batch's first position: its rows attend past the chunk boundary
let private WDEC_ROWS = 3
let private WDEC_CROSS_KEYS = 150 //! inside the first chunk, so the cross rows reach the combine with the second chunk's partial empty
let private WG = 256              //! the row and element classes' workgroup
//! the norm rows' bar on both homes: the inverse norm scales its whole row, and CUDA's `rsqrt` (NVRTC's fast-math `rsqrtf`, 2 ulp)
//! or Vulkan's `1 / sqrt` (under 8 ulp by Vulkan's precision table) moves an element by under 1e-6 of itself, `APPROX_REL` / 2000
let private NORM_BAR = BAR_APPROX

//! the plane ids of a launch, in binding order, as the rail roles `cell_launch` seats them at
def seats(idx) {
    var r : TowerBuf[typeinfo dim(idx)]
    for (i in range(typeinfo dim(idx))) {
        r[i] = cell_role(idx[i])
    }
    return r
}

//! `n` elements of `TT` of plane `p` from `at` past its base still `fill` - the slots past a run a dispatch's idle lanes
//! reach; a plane whose dispatch covers its run exactly has none, and says so
[unused_argument(elem)]
def slack_kept(t : T?; tag : string; cp : auto(C); p : int; elem : auto(TT); at, n : int; fill : TT) {
    if (n == 0) {
        to_log(LOG_INFO, "{tag}: the dispatch covers the run exactly - no lane past it\n")
        return
    }
    var inscope got <- cp |> plane_as(p, type<TT>, n, at)
    var inscope want <- [for (_i in range(n)); fill]
    check_exact(t, "{tag}: the {n} slots past the run keep their fill", got, want)
}

//! no element of `a` still `fill` - every slot the run owns written - the count left and the first one's index logged
def check_written(t : T?; tag : string; a : array<auto(TT)>; fill : TT) {
    let left = count(a, $(v) => v == fill)
    let verdict = "{left} of {length(a)} slots still the sentinel, the first at {find_index(a, fill)}"
    to_log(LOG_INFO, "{tag}: {verdict}\n")
    t |> success(left == 0, "{tag}: {verdict}")
}

//! the dispatch's lanes past `n` elements: `n` rounded to whole workgroups of `wg`
def lanes_past(n, wg : int) : int => round_up(n, wg) - n

//! Q8_0 pair `qd` / `sd` against `qr` / `sr` at `mismatch_q8`'s bars, the count, the first quant byte off and the largest scale
//! difference with its block logged; a reference scale poisoned by half its magnitude (at least 0.5) must red it
def check_q8(t : T?; tag : string; qd : array<int8>; sd : array<float>; qr : array<int8>; sr : array<float>; scale_bar : float; qtol : int) {
    let bad = mismatch_q8(qd, sd, qr, sr, scale_bar, qtol) + abs(length(qd) - length(qr)) + abs(length(sd) - length(sr))
    var first = -1
    for (i in range(min(length(qd), length(qr)))) {
        if (abs(int(qd[i]) - int(qr[i])) > qtol) {
            first = i
            break
        }
    }
    let worst = span_maxdiff(sd, sr, 0, 0, min(length(sd), length(sr)))
    let verdict = "{bad} off, the first quant byte off at {first}, the largest scale difference {worst.diff} at block {worst.at}"
    to_log(LOG_INFO, "{tag}: {verdict}\n")
    t |> success(bad == 0, "{tag}: {verdict}")
    var inscope poisoned <- poisoned_copy(sr, 0, 0.5 * max(1.0, abs(sr[0])))
    t |> success(mismatch_q8(qd, sd, qr, poisoned, scale_bar, qtol) > 0, "{tag}: the bar reds a poisoned scale")
}

//! a Q8_0 pair as the floats it stands for: each quant byte times its block's scale
def q8_values(q : array<int8>; s : array<float>) : array<float> {
    let q8e = int(kq_elems(KqFmt.q8))
    return <- [for (i in range(length(q))); float(q[i]) * s[i / q8e]]
}

//! a Q8_0 pair: the quant bytes and a scale a block
struct Q8Rows {
    q : array<int8>
    s : array<float>
}

//! `x` requantized to Q8_0 as the CPU does, rows of `d`
def q8_rows_ref(x : array<float>; d, rows : int) : Q8Rows {
    var r : Q8Rows
    requant_rows_q8_sized(x, int64(d), int64(rows), r.q, r.s)
    return <- r
}

//! the k3 s2 p1 columns over two chunks of the 7 x 5 x 3 image, as halves, the pad and the zero tail
def im2col_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tower_im2col"
    let chunk_in = FM_IH * FM_IW * FM_CIN
    let rows = FM_CHUNKS * FM_OH * FM_OW
    let n_col = rows * FM_KDP
    let lanes = round_up(n_col, WG)
    var inscope src <- row_fill(FM_CHUNKS * chunk_in, 31, 5, 0.07, 3.3)
    var inscope want <- im2col2d_ref(src, FM_IW, FM_IH, FM_CIN, FM_KDP, rows)
    let srcp = cp |> in_(src)
    let colp = cp |> out_words(int64(lanes) * 2l, F16_SENTINEL_BITS)
    var pa = GkIm2col2dArgs(iw = uint(FM_IW), ih = uint(FM_IH), cin = uint(FM_CIN), sstride = uint(FM_CIN), kdp = uint(FM_KDP), rows = uint(rows), soff = 0u)
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tower_im2col_cls(rl, seats(fixed_array(srcp, colp)), pa, int64(lanes / WG))
    }
    var inscope col_dev <- cp |> plane_as(colp, type<uint16>, n_col)
    check_halves(t, "{tag}: the k3 s2 p1 columns over two chunks, the pad and the zero tail", col_dev, want)
    slack_kept(t, tag, cp, colp, type<uint16>, n_col, lanes - n_col, uint16(F16_SENTINEL))
}

//! one `tts_im2col16` arm: the conv's columns as halves at `xoff` / `coloff`, the halves before and the lanes past kept
def private im2col16_arm(t : T?; var cp : auto(C); tag : string; x : array<float>; cin, tin, k, stride, kdim : int; row_major : bool; xoff, coloff : int) {
    cp |> cell_clear()
    let tout = (tin + 2 - k) / stride + 1
    let total = tout * kdim
    let lanes = coloff + round_up(total, 1024)
    var inscope want <- im2col1d_ref(x, cin, tin, tout, k, stride, 1, kdim, row_major)
    let xp = cp |> in_(x, xoff)
    let colp = cp |> out_words(int64(round_up(lanes, 2)) * 2l, F16_SENTINEL_BITS)
    var pa = GkIm2col1dArgs(cin = uint(cin), tin = uint(tin), tout = uint(tout), k = uint(k), stride = uint(stride), pad_l = 1u, dilation = 1u,
        kdim = uint(kdim), xoff = uint(xoff), coloff = uint(coloff), row_major = row_major ? 1u : 0u, r0 = 0u, mp = uint(tout))
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tts_im2col16_cls(rl, seats(fixed_array(xp, colp)), pa, int64(total))
    }
    var inscope col_dev <- cp |> plane_as(colp, type<uint16>, total, coloff)
    check_halves(t, tag, col_dev, want)
    slack_kept(t, "{tag}: the lanes past the panel", cp, colp, type<uint16>, coloff + total, lanes - coloff - total, uint16(F16_SENTINEL))
    slack_kept(t, "{tag}: the halves before coloff", cp, colp, type<uint16>, 0, coloff, uint16(F16_SENTINEL))
}

//! the stem's half im2col on both source layouts and strides, off and on the four-channel lattice, x and the panel at bases
def im2col16_case(t : T?; var cp : auto(C)) {
    let len1 = 9
    let nch1 = 5
    let nch2 = 6
    let nch4 = 8
    var inscope mel1 <- row_fill(nch1 * len1, 23, 7, 0.09, 2.0)
    var inscope cv2 <- row_fill(len1 * nch2, 19, 3, 0.08, 2.2)
    var inscope x4 <- row_fill(len1 * nch4, 17, 5, 0.06, 2.4)
    im2col16_arm(t, cp, "the stem's im2col (tts_im2col16, channel-major, stride 1): the mel's k3 columns with the zero pad and tail", mel1, nch1, len1, 3, 1, 64, false, 0, 0)
    im2col16_arm(t, cp, "the stem's im2col (tts_im2col16, row-major, stride 2): the conv output's k3 columns at every second position and the zero pad columns",
        cv2, nch2, len1, 3, 2, 3 * nch2 + 2, true, 0, 0)
    im2col16_arm(t, cp, "tts_im2col16 on the four-channel lattice (row-major, stride 1), x and the panel at element bases", x4, nch4, len1, 3, 1, 3 * nch4, true, 3, 6)
    im2col16_arm(t, cp, "tts_im2col16 on the four-channel lattice (channel-major, stride 2), four zero pad columns", x4, nch4, len1, 3, 2, 3 * nch4 + 4, false, 0, 0)
}

//! qwen3a's feature rows off the third conv stage, as halves
def q3a_shuffle_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tower_q3a_shuffle"
    let q3_nch = 5
    let q3_tok = int(QWEN3A_TOK_PER_CHUNK)
    let q3_pos = FM_CHUNKS * q3_tok
    let n = q3_pos * 16 * q3_nch
    let lanes = round_up(n, WG)
    var inscope y3 <- row_fill(FM_CHUNKS * 16 * q3_tok * q3_nch, 11, 2, 0.06, 2.9)
    var inscope want <- q3a_shuffle_ref(y3, q3_nch, q3_tok, q3_pos)
    let y3p = cp |> in_(y3)
    let featp = cp |> out_words(int64(lanes) * 2l, F16_SENTINEL_BITS)
    var pa = GkQ3aShuffleArgs(nch = uint(q3_nch), ncp = uint(q3_nch), chunk_stride = uint(16 * q3_tok * q3_nch), npos = uint(q3_pos))
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tower_q3a_shuffle_cls(rl, seats(fixed_array(y3p, featp)), pa, int64(lanes / WG))
    }
    var inscope feat_dev <- cp |> plane_as(featp, type<uint16>, n)
    check_halves(t, "{tag}: the feature rows off the third conv stage", feat_dev, want)
    slack_kept(t, "{tag}: the lanes past the rows", cp, featp, type<uint16>, n, lanes - n, uint16(F16_SENTINEL))
}

//! the [time][ch][freq] rows off the [time][freq][ch] image, with no pad row and with two exact-zero pad rows
def feat_shuffle_case(t : T?; var cp : auto(C)) {
    let fs_t = 3
    let fs_w = 4
    let fs_c = 5
    var inscope src <- row_fill(fs_t * fs_w * fs_c, 13, 9, 0.05, 1.5)
    for (pad in [0, 2]) {
        cp |> cell_clear()
        let tag = "tower_feat_shuffle (rows_pad t + {pad})"
        let rows_pad = fs_t + pad
        let n = rows_pad * fs_w * fs_c
        let lanes = round_up(n, WG)
        var inscope want <- feat_shuffle_ref(src, fs_t, fs_w, fs_c, rows_pad)
        let srcp = cp |> in_(src)
        let dstp = cp |> out_(n, 0, lanes - n)
        var pa = GkFeatShuffleArgs(w = uint(fs_w), ch = uint(fs_c), t = uint(fs_t), rows_pad = uint(rows_pad))
        cp |> cell_launch(t, tag) $(var rl) {
            enc_tower_feat_shuffle_cls(rl, seats(fixed_array(srcp, dstp)), pa, int64(lanes / WG))
        }
        var inscope dev <- cp |> plane_run(dstp)
        check_rows(t, "{tag}: the rows and the pad rows", dev, want, BAR_EXACT, fs_w * fs_c)
        if (pad > 0) {
            var inscope pad_dev <- subarray(dev, range(fs_t * fs_w * fs_c, n))
            var inscope zeros <- plane_of(n - fs_t * fs_w * fs_c, 0.0)
            check_exact(t, "{tag}: the pad rows are exact zeros", pad_dev, zeros)
        }
        slack_kept(t, "{tag}: the lanes past the rows", cp, dstp, type<uint>, n, lanes - n, NAN_SENTINEL_BITS)
    }
}

//! the channel LayerNorm then ReLU in place, a lane a row: the rows past the live ones the idle lanes would reach kept
def ln_relu_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tower_ln_relu"
    let d = ROWS_D
    let rows = ROWS_N
    let idle = lanes_past(rows, WG) * d
    var inscope x <- row_fill(rows * d, 41, 7, 0.05, 2.1)
    var inscope wn <- row_fill((3 + int(QWEN3A_TOK_PER_CHUNK)) * d, 7, 1, 0.01, -0.6)
    var inscope want <- ln_relu_rows_ref(x, wn, rows, d, ROWS_EPS)
    let lnp = cp |> io_(x, 0, idle)
    let wp = cp |> in_(wn)
    var pa = GkLnReluRowsArgs(cout = uint(d), ldin = uint(d), ldout = uint(d), rows = uint(rows), woff = 0u, eps = ROWS_EPS)
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tower_ln_relu_cls(rl, seats(fixed_array(lnp, lnp, wp)), pa, int64(round_up(rows, WG) / WG))
    }
    var inscope dev <- cp |> plane_run(lnp)
    check_rows(t, "{tag} channel LayerNorm then ReLU vs the CPU loop", dev, want, BAR_APPROX, d)
    let live = find_index_if(want, $(v) => v != 0.0)
    t |> success(live >= 0 && dev[live] != x[live], "{tag} control: a live element moved off x")
    slack_kept(t, "{tag}: the rows past the live ones", cp, lnp, type<uint>, rows * d, idle, float_bits_to_uint(POISON))
}

//! the frames' spectra on the magnitude and the power arm against the double DFT
def spec_dft_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tower_spec_dft"
    let n_pw = MEL_NLEN * MEL_NBINS
    let lanes = round_up(n_pw, WG)
    var inscope xw <- row_fill(MEL_NLEN * MEL_NFFT, 37, 5, 0.03, 1.4)
    var inscope tw <- build_dft_twiddles(int64(MEL_NFFT))
    var inscope mag_want <- spec_dft_ref(xw, tw, MEL_NFFT, MEL_NLEN, true)
    var inscope pow_want <- spec_dft_ref(xw, tw, MEL_NFFT, MEL_NLEN, false)
    let xwp = cp |> in_(xw)
    let twp = cp |> in_(tw)
    let magp = cp |> out_(n_pw, 0, lanes - n_pw)
    let powp = cp |> out_(n_pw, 0, lanes - n_pw)
    var pa_mag = TowerSpecDftArgs(n_fft = uint(MEL_NFFT), n_frames = uint(MEL_NLEN), mag = 1u)
    var pa_pow = TowerSpecDftArgs(n_fft = uint(MEL_NFFT), n_frames = uint(MEL_NLEN), mag = 0u)
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tower_spec_dft_cls(rl, seats(fixed_array(xwp, twp, magp)), pa_mag, int64(lanes / WG))
        enc_tower_spec_dft_cls(rl, seats(fixed_array(xwp, twp, powp)), pa_pow, int64(lanes / WG))
    }
    var inscope mag_dev <- cp |> plane_run(magp)
    var inscope pow_dev <- cp |> plane_run(powp)
    check_rows(t, "{tag} (magnitude) frames' spectra vs the double DFT", mag_dev, mag_want, BAR_APPROX, MEL_NBINS)
    check_rows(t, "{tag} (power) vs re^2 + im^2", pow_dev, pow_want, BAR_APPROX, MEL_NBINS)
    t |> success(!approx(pow_dev[1], mag_want[1]), "{tag} control: the power arm is not the magnitude")
    slack_kept(t, "{tag} (magnitude): the lanes past the bins", cp, magp, type<uint>, n_pw, lanes - n_pw, NAN_SENTINEL_BITS)
    slack_kept(t, "{tag} (power): the lanes past the bins", cp, powp, type<uint>, n_pw, lanes - n_pw, NAN_SENTINEL_BITS)
}

//! the floor laws over one power plane: gemma4a's max floor, canary's added floor, the whisper preprocessors' mel-major log10
def mel_log_floor_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tower_mel_log (the floor laws)"
    let n_pw = MEL_NLEN * MEL_NBINS
    let n_mel = MEL_NLEN * MEL_NMEL
    let slack = lanes_past(n_mel, WG)
    var inscope pw <- row_fill(n_pw, 17, 3, 0.02, -0.9)
    for (v in pw) {
        v = abs(v) + 0.01
    }
    for (b in range(MEL_NBINS)) {   // frame 0 sits under gemma4a's floor: the max arm floors it, the add arm reads under it
        pw[b] = 1.0e-6
    }
    var inscope fil <- row_fill(MEL_NMEL * MEL_NBINS, 13, 7, 0.01, -0.5)
    for (v in fil) {
        v = abs(v) * 0.1
    }
    var inscope g4a_want <- mel_log_ref(pw, fil, MEL_NBINS, MEL_NMEL, MEL_NLEN, 0, GK_MEL_LOG_MAX, MEL_MAX_FLOOR_G4A, 0.0, 1.0, true, false)
    var inscope cn_want <- mel_log_ref(pw, fil, MEL_NBINS, MEL_NMEL, MEL_NLEN, 0, GK_MEL_LOG_ADD, MEL_FLOOR_24, 0.0, 1.0, true, false)
    var inscope wh_want <- mel_log_ref(pw, fil, MEL_NBINS, MEL_NMEL, MEL_NLEN, 0, GK_MEL_LOG_MAX, MEL_FLOOR_24, 0.0, MEL_LOG10_E, false, false)
    let pwp = cp |> in_(pw)
    let filp = cp |> in_(fil)
    let g4ap = cp |> out_(n_mel, 0, slack)
    let cnp = cp |> out_(n_mel, 0, slack)
    let whp = cp |> out_(n_mel, 0, slack)
    var pa_g4a = GkMelLogArgs(n_bins = uint(MEL_NBINS), n_mel = uint(MEL_NMEL), frames = uint(MEL_NLEN),
        foff = 0u, lo = MEL_MAX_FLOOR_G4A, hi = 0.0, scale = 1.0, frame_major = 1u, form = GK_MEL_LOG_MAX)
    var pa_cn = GkMelLogArgs(n_bins = uint(MEL_NBINS), n_mel = uint(MEL_NMEL), frames = uint(MEL_NLEN),
        foff = 0u, lo = MEL_FLOOR_24, hi = 0.0, scale = 1.0, frame_major = 1u, form = GK_MEL_LOG_ADD)
    var pa_wh = GkMelLogArgs(n_bins = uint(MEL_NBINS), n_mel = uint(MEL_NMEL), frames = uint(MEL_NLEN),
        foff = 0u, lo = MEL_FLOOR_24, hi = 0.0, scale = MEL_LOG10_E, frame_major = 0u, form = GK_MEL_LOG_MAX)
    let wgs = int64(round_up(n_mel, WG) / WG)
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tower_mel_log_cls(rl, seats(fixed_array(pwp, filp, g4ap)), pa_g4a, wgs)
        enc_tower_mel_log_cls(rl, seats(fixed_array(pwp, filp, cnp)), pa_cn, wgs)
        enc_tower_mel_log_cls(rl, seats(fixed_array(pwp, filp, whp)), pa_wh, wgs)
    }
    var inscope g4a_dev <- cp |> plane_run(g4ap)
    var inscope cn_dev <- cp |> plane_run(cnp)
    var inscope wh_dev <- cp |> plane_run(whp)
    check_rows(t, "tower_mel_log (frame-major, max floor) vs the gemma4a form", g4a_dev, g4a_want, BAR_APPROX, MEL_NMEL)
    check_rows(t, "tower_mel_log (frame-major, added floor) vs the canary form", cn_dev, cn_want, BAR_APPROX, MEL_NMEL)
    check_rows(t, "tower_mel_log (mel-major, log10) vs the whisper preprocessors' form", wh_dev, wh_want, BAR_APPROX, MEL_NLEN)
    t |> success(approx(g4a_dev[0], log(MEL_MAX_FLOOR_G4A)) && cn_dev[0] < g4a_dev[0] - 1.0,
        "tower_mel_log: the quiet frame floors on the max arm and reads under the floor on the add arm")
    slack_kept(t, "tower_mel_log (max floor): the lanes past the rows", cp, g4ap, type<uint>, n_mel, slack, NAN_SENTINEL_BITS)
    slack_kept(t, "tower_mel_log (added floor): the lanes past the rows", cp, cnp, type<uint>, n_mel, slack, NAN_SENTINEL_BITS)
    slack_kept(t, "tower_mel_log (log10): the lanes past the rows", cp, whp, type<uint>, n_mel, slack, NAN_SENTINEL_BITS)
}

//! gigaam's clamp law over 6 frames of 37 bins past a filter lead: frame 1 all zero reads the floor's log, frame 4 the
//! ceiling's, frame 0 neither
def mel_log_clamp_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tower_mel_log (gigaam's clamp)"
    let nb = 37
    let nm = 5
    let nf = 6
    let foff = 11
    let lo = 1e-9
    let hi = 1e9
    let lanes = round_up(nf * nm, WG)
    var inscope gpw <- plane_of(nf * nb, 0.0)
    for (f in range(nf)) {
        for (k in range(nb)) {
            let v = 0.3 + 0.2 * sin(float(f * 13 + k) * 0.71)
            gpw[f * nb + k] = f == 1 ? 0.0 : (f == 4 ? v * 1.0e10 : v * float(1 + f * f))
        }
    }
    var inscope gfil <- [for (i in range(foff + nm * nb)); i < foff ? 1.0e30 : 0.05 * float(((i - foff) * 7) % 11)]
    var inscope want <- mel_log_ref(gpw, gfil, nb, nm, nf, foff, GK_MEL_LOG_CLAMP, lo, hi, 1.0, true, true)
    let gpwp = cp |> in_(gpw)
    let gfilp = cp |> in_(gfil)
    let gap = cp |> out_(nf * nm, 0, lanes - nf * nm)
    var pa = GkMelLogArgs(n_bins = uint(nb), n_mel = uint(nm), frames = uint(nf), foff = uint(foff), lo = lo, hi = hi,
        scale = 1.0, frame_major = 1u, form = GK_MEL_LOG_CLAMP)
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tower_mel_log_cls(rl, seats(fixed_array(gpwp, gfilp, gap)), pa, int64(lanes / WG))
    }
    var inscope dev <- cp |> plane_run(gap)
    check_rows(t, "{tag} the clamped-log filterbank vs the CPU float form", dev, want, Bar(rel = 1e-6, abs_floor = 1e-6, poison_scaled = true), nm)
    t |> success(abs(want[1 * nm + 2] - log(lo)) < 1e-5, "{tag}: the zero frame reads the floor's log")
    t |> success(abs(want[4 * nm + 2] - log(hi)) < 1e-5, "{tag}: the loud frame reads the ceiling's log")
    t |> success(abs(want[2] - log(lo)) > 1e-5 && abs(want[2] - log(hi)) > 1e-5,
        "{tag}: the negative twin - a live frame reads neither the floor's log nor the ceiling's ({want[2]})")
    slack_kept(t, "{tag}: the lanes past the live rows", cp, gap, type<uint>, nf * nm, lanes - nf * nm, NAN_SENTINEL_BITS)
}

//! canary's per-feature normalization in place over the valid frames, the tail zero: a column a lane, so an idle column lane
//! under a broken guard writes up to its column on the last frame
def mel_norm_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tower_mel_norm"
    let n_norm = NORM_NLEN * MEL_NMEL
    let col_lanes = round_up(MEL_NMEL, 64)
    let reach = (col_lanes - 1) + (NORM_NLEN - 1) * MEL_NMEL + 1
    var inscope mel_in <- row_fill(n_norm, 29, 11, 0.07, 2.4)
    var inscope want <- mel_norm_ref(mel_in, MEL_NMEL, NORM_NLEN, NORM_VALID)
    let normp = cp |> io_(mel_in, 0, reach - n_norm)
    var pa = GkMelNormArgs(n_mel = uint(MEL_NMEL), n_frames = uint(NORM_NLEN), valid = uint(NORM_VALID))
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tower_mel_norm_cls(rl, seats(fixed_array(normp)), pa, int64(col_lanes / 64))
    }
    var inscope dev <- cp |> plane_run(normp)
    check_rows(t, "{tag} column statistics over the valid frames and the zero tail vs the CPU loop", dev, want, BAR_APPROX, MEL_NMEL)
    var inscope tail <- subarray(dev, range(NORM_VALID * MEL_NMEL, n_norm))
    var inscope zeros <- plane_of(n_norm - NORM_VALID * MEL_NMEL, 0.0)
    check_exact(t, "{tag}: the tail is exact zeros", tail, zeros)
    t |> success(dev[0] != mel_in[0], "{tag}: the rows moved off the input")
    slack_kept(t, "{tag}: the elements past the plane a column lane reaches", cp, normp, type<uint>, n_norm, reach - n_norm, float_bits_to_uint(POISON))
}

//! gigaam's windowed frames bit for bit with the zero row pad, the window at an element base: 5 frames of 40 taps at hop 16
//! written as 8 rows of 40
def ga_frames_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tower_ga_frames"
    let n_fft = 40
    let hop = 16
    let frames = 5
    let rows = 8
    let woff = 7
    let n_samp = 110
    let lanes = round_up(rows * n_fft, WG)
    var inscope samp <- [for (i in range(n_samp)); sin(float(i) * 0.37) * 0.8 + 0.01 * float(i % 7)]
    var inscope win <- [for (i in range(woff + n_fft)); i < woff ? 1.0e30 : 0.5 - 0.5 * cos(6.2831855 * float(i - woff) / float(n_fft))]
    var inscope want <- frame_window_ref(samp, win, woff, n_fft, hop, frames, rows)
    let sp = cp |> in_(samp)
    let wp = cp |> in_(win)
    let xwp = cp |> out_(rows * n_fft, 0, lanes - rows * n_fft)
    var pa = GkFrameWindowArgs(n_fft = uint(n_fft), hop = uint(hop), frames = uint(frames), rows = uint(rows), woff = uint(woff))
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tower_ga_frames_cls(rl, seats(fixed_array(sp, wp, xwp)), pa, int64(lanes / WG))
    }
    var inscope dev <- cp |> plane_run(xwp)
    check_rows(t, "{tag} the windowed frames with their zero pads vs the CPU frame loop", dev, want, BAR_EXACT, n_fft)
    slack_kept(t, "{tag}: the lanes past the live rows", cp, xwp, type<uint>, rows * n_fft, lanes - rows * n_fft, NAN_SENTINEL_BITS)
}

//! the 11 x 10 depthwise conv2d fixture: f16-exact image, tap-major taps and bias
struct private Dw2dFix {
    oh : int
    ow : int
    total : int
    x : array<float>
    taps : array<float>   //! tap-major [9][ch], parakeet's
    bias : array<float>
}

def private dw2d_fix : Dw2dFix {
    let oh = int(conv_s2_out(int64(DW2D_IH)))
    let ow = int(conv_s2_out(int64(DW2D_IW)))
    return <- Dw2dFix(oh = oh, ow = ow, total = oh * ow * DW2D_CH,
        x <- [for (i in range(DW2D_IH * DW2D_IW * DW2D_CH)); float(float16(0.11 * float((i * 5) % 37) - 2.0))],
        taps <- [for (i in range(9 * DW2D_CH)); float(float16(0.07 * float((i * 7) % 29) - 1.0))],
        bias <- [for (i in range(DW2D_CH)); float(float16(0.3 * float(i % 5) - 0.6))])
}

//! canary's k3 s2 p1 depthwise conv over channel-major taps with its bias: the 7 x 5 front image behind an image row of poison,
//! and the 11 x 10 image
def dw_conv_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tower_dw_conv"
    let dw_in = FM_IH * FM_IW * DW_CH
    let dw_out = FM_OH * FM_OW * DW_CH
    var inscope dw_x <- row_fill(dw_in, 23, 9, 0.06, 2.7)
    var inscope dw_w <- row_fill(DW_CH * 9 + DW_CH, 7, 2, 0.05, 0.9)   // the taps at 0, the bias row at ch x 9
    var inscope wv <- taps_tap_major(dw_w, DW_CH, 9)
    var inscope bv <- subarray(dw_w, range(DW_CH * 9, DW_CH * 10))
    var inscope want <- dw_s2_front_ref(false, false, FM_IW, DW_CH, FM_OH, FM_OW, FM_IH, dw_x, wv, bv)
    var inscope fx <- dw2d_fix()
    var inscope want2 <- dw_s2_front_ref(false, false, DW2D_IW, DW2D_CH, fx.oh, fx.ow, DW2D_IH, fx.x, fx.taps, fx.bias)
    var inscope wn_cn <- [for (i in range(9 * DW2D_CH)); fx.taps[(i % 9) * DW2D_CH + i / 9]]   // canary's [ch][9] taps
    wn_cn |> push_from(fx.bias)
    let idle = lanes_past(dw_out, WG)
    let idle2 = lanes_past(fx.total, WG)
    let dwxp = cp |> in_(dw_x, 0, FM_IW * DW_CH)   // an image row of poison past the source: a tap below the image the row guard lets through reads it
    let dwyp = cp |> out_(dw_out, 0, idle)
    let dwwp = cp |> in_(dw_w)
    let xp = cp |> in_(fx.x)
    let wcp = cp |> in_(wn_cn)
    let ycp = cp |> out_(fx.total, 0, idle2)
    var pa = GkDwConv2dArgs(iw = uint(FM_IW), ih = uint(FM_IH), ih_valid = uint(FM_IH), ch = uint(DW_CH), woff = 0u, boff = uint(DW_CH * 9))
    var pa2 = GkDwConv2dArgs(iw = uint(DW2D_IW), ih = uint(DW2D_IH), ih_valid = uint(DW2D_IH), ch = uint(DW2D_CH), woff = 0u, boff = uint(9 * DW2D_CH))
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tower_dw_conv_cls(rl, seats(fixed_array(dwxp, dwyp, dwwp)), pa, int64(round_up(dw_out, WG) / WG))
        enc_tower_dw_conv_cls(rl, seats(fixed_array(xp, ycp, wcp)), pa2, int64(round_up(fx.total, WG) / WG))
    }
    var inscope dev <- cp |> plane_run(dwyp)
    var inscope dev2 <- cp |> plane_run(ycp)
    check_rows(t, "{tag} k3 s2 p1 depthwise taps with the bias vs the CPU form (7 x 5)", dev, want, BAR_APPROX, DW_CH)
    check_rows(t, "{tag} over channel-major taps vs the CPU loop (11 x 10)", dev2, want2, BAR_APPROX, DW2D_CH)
    slack_kept(t, "{tag} (7 x 5): the idle lanes past the output", cp, dwyp, type<uint>, dw_out, idle, NAN_SENTINEL_BITS)
    slack_kept(t, "{tag} (11 x 10): the idle lanes past the output", cp, ycp, type<uint>, fx.total, idle2, NAN_SENTINEL_BITS)
}

//! parakeet's k3 s2 p1 depthwise conv over tap-major taps, the rows past the valid length masked
def pk_dw2d_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tower_pk_dw2d"
    let valid = 7
    var inscope fx <- dw2d_fix()
    var inscope want <- dw_s2_front_ref(false, false, DW2D_IW, DW2D_CH, fx.oh, fx.ow, DW2D_IH, fx.x, fx.taps, fx.bias)
    var inscope masked <- dw_s2_front_ref(false, false, DW2D_IW, DW2D_CH, fx.oh, fx.ow, valid, fx.x, fx.taps, fx.bias)
    var inscope wn_pk := fx.taps
    wn_pk |> push_from(fx.bias)
    let idle = lanes_past(fx.total, WG)
    let xp = cp |> in_(fx.x)
    let wpk = cp |> in_(wn_pk)
    let ypk = cp |> out_(fx.total, 0, idle)
    let ymask = cp |> out_(fx.total, 0, idle)
    var pa = GkDwConv2dArgs(iw = uint(DW2D_IW), ih = uint(DW2D_IH), ih_valid = uint(DW2D_IH), ch = uint(DW2D_CH), woff = 0u, boff = uint(9 * DW2D_CH))
    var pa_mask = pa
    pa_mask.ih_valid = uint(valid)
    let wgs = int64(round_up(fx.total, WG) / WG)
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tower_pk_dw2d_cls(rl, seats(fixed_array(xp, ypk, wpk)), pa, wgs)
        enc_tower_pk_dw2d_cls(rl, seats(fixed_array(xp, ymask, wpk)), pa_mask, wgs)
    }
    var inscope pk_dev <- cp |> plane_run(ypk)
    var inscope mask_dev <- cp |> plane_run(ymask)
    check_rows(t, "{tag} over tap-major taps vs the CPU loop", pk_dev, want, BAR_APPROX, DW2D_CH)
    check_rows(t, "{tag} under the length mask vs the CPU loop over the valid rows", mask_dev, masked, BAR_APPROX, DW2D_CH)
    t |> success(mismatch_exact(want, masked) > 0, "{tag} control: the mask moves the rows past the valid length")
    slack_kept(t, "{tag}: the idle lanes past the output", cp, ypk, type<uint>, fx.total, idle, NAN_SENTINEL_BITS)
    slack_kept(t, "{tag} under the mask: the idle lanes past the output", cp, ymask, type<uint>, fx.total, idle, NAN_SENTINEL_BITS)
}

//! the Conformer rows: the GLU, the causal depthwise taps and the windowed RPE attention on both head widths - the 128-wide
//! stamp over the rows read as two heads, queries scaled to the softcap's knee, the oracle at four times the cap its control
def conformer_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let d = ROWS_D
    let rows = ROWS_N
    let hs = d / ROWS_HEADS
    let nd = rows * d
    let idle = lanes_past(nd, WG)
    var inscope glu_pw_rows <- row_fill(rows * 2 * d, 31, 7, 0.05, 2.5)
    var inscope q <- row_fill(nd, 41, 3, 0.03, 1.1)
    var inscope k <- row_fill(nd, 17, 11, 0.04, 1.3)
    var inscope v <- row_fill(nd, 29, 13, 0.05, 1.7)
    var inscope rpe <- row_fill((G4A_PAST + 1) * d, 13, 2, 0.02, 0.9)
    var inscope wn <- row_fill(hs + d * 5, 7, 1, 0.01, 0.4)   // the per-dim query scale row at 0, the conv taps at hs
    let hp = cp |> in_(glu_pw_rows)
    let gp = cp |> out_(nd, 0, idle)
    let cpl = cp |> out_(nd, 0, idle)
    let qp = cp |> in_(q)
    let kp = cp |> in_(k)
    let vp = cp |> in_(v)
    let aop = cp |> out_(nd, 0, d)
    let rp = cp |> in_(rpe)
    let wp = cp |> in_(wn)
    let sc = gemma4a_attn_scales(int64(hs))
    var pa_convmod = GkGatedRowsArgs(d = uint(d), nelem = uint(nd))
    var pa_dw = GkDwConvRowsArgs(c = uint(d), t_in = uint(rows), t_out = uint(rows), woff = uint(hs))
    var pa_attn = GkG4aAttnArgs(d = uint(d), npos = uint(rows), hs = uint(hs), pdsoff = 0u, cap = GK_G4A_ATTN_CAP, qscale = sc.qscale, kscale = sc.kscale)
    let wgs = int64(round_up(nd, WG) / WG)
    cp |> cell_launch(t, "the Conformer stamps") $(var rl) {
        enc_tower_glu_sig_cls(rl, seats(fixed_array(hp, gp)), pa_convmod, wgs)
        enc_tower_dw_conv5_cls(rl, seats(fixed_array(gp, cpl, wp)), pa_dw, wgs)
        enc_tower_g4a_attn64_cls(rl, seats(fixed_array(qp, kp, vp, aop, rp, wp)), pa_attn, int64(round_up(rows * ROWS_HEADS, 64) / 64))
    }
    var inscope g_dev <- cp |> plane_run(gp)
    var inscope c_dev <- cp |> plane_run(cpl)
    var inscope ao_dev <- cp |> plane_run(aop)
    var inscope g_ref <- glu_sig_ref(glu_pw_rows, rows, d)
    check_rows(t, "tower_glu_sig value . sigmoid(gate) off the pointwise rows", g_dev, g_ref, BAR_APPROX, d)
    slack_kept(t, "tower_glu_sig", cp, gp, type<uint>, nd, idle, NAN_SENTINEL_BITS)
    var inscope c_ref <- dw_conv5_causal_ref(g_ref, wn, rows, d, hs)
    check_rows(t, "tower_dw_conv5 causal depthwise taps vs the CPU loop", c_dev, c_ref, BAR_APPROX, d)
    slack_kept(t, "tower_dw_conv5", cp, cpl, type<uint>, nd, idle, NAN_SENTINEL_BITS)
    var inscope ao_ref <- g4a_attn_oracle(q, k, v, rpe, wn, rows, d, ROWS_HEADS, hs, G4A_PAST, sc.qscale, sc.kscale, G4A_CAP)
    check_rows(t, "tower_g4a_attn64 windowed RPE attention vs the CPU loop", ao_dev, ao_ref, BAR_APPROX, d, hs)
    slack_kept(t, "tower_g4a_attn64", cp, aop, type<uint>, nd, d, NAN_SENTINEL_BITS)
    var inscope k_scaled <- scaled_copy(k, 1.5)
    var inscope ao_ctl <- g4a_attn_oracle(q, k_scaled, v, rpe, wn, rows, d, ROWS_HEADS, hs, G4A_PAST, sc.qscale, sc.kscale, G4A_CAP)
    check_exceeds(t, "tower_g4a_attn64 control: scaled keys land the oracle elsewhere", ao_dev, ao_ctl, BAR_APPROX)
    let heads2 = ROWS_HEADS / 2
    let hs2 = d / heads2
    var inscope q_at_cap_knee <- scaled_copy(q, 30.0)
    cp |> plane_upload(qp, q_at_cap_knee)
    cp |> plane_refill(aop)
    let qscale2 = gemma4a_attn_scales(int64(hs2)).qscale
    var pa_attn128 = GkG4aAttnArgs(d = uint(d), npos = uint(rows), hs = uint(hs2), pdsoff = 0u, cap = GK_G4A_ATTN_CAP, qscale = qscale2, kscale = sc.kscale)
    cp |> cell_launch(t, "the 128-wide RPE attention stamp") $(var rl) {
        enc_tower_g4a_attn_cls(rl, seats(fixed_array(qp, kp, vp, aop, rp, wp)), pa_attn128, int64(round_up(rows * heads2, 64) / 64))
    }
    var inscope ao_knee_dev <- cp |> plane_run(aop)
    var inscope ao_capped_ref <- g4a_attn_oracle(q_at_cap_knee, k, v, rpe, wn, rows, d, heads2, hs2, G4A_PAST, qscale2, sc.kscale, G4A_CAP)
    check_rows(t, "tower_g4a_attn (128-wide stamp) windowed RPE attention over scaled queries vs the CPU loop", ao_knee_dev, ao_capped_ref, BAR_APPROX, d, hs2)
    slack_kept(t, "tower_g4a_attn", cp, aop, type<uint>, nd, d, NAN_SENTINEL_BITS)
    var inscope ao_cap4_ref <- g4a_attn_oracle(q_at_cap_knee, k, v, rpe, wn, rows, d, heads2, hs2, G4A_PAST, qscale2, sc.kscale, 4.0 * G4A_CAP)
    check_exceeds(t, "tower_g4a_attn control: the softcap engages on the scaled queries - the oracle at four times the cap lands elsewhere", ao_knee_dev, ao_cap4_ref, BAR_APPROX)
}

//! the FastConformer conv module tails: the centered depthwise conv + folded BatchNorm + silu on channel-major and tap-major
//! taps, and gigaam's biased form, the source's rows past npos carrying poison
def fastconformer_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let d = ROWS_D
    let rows = ROWS_N
    let nd = rows * d
    let idle = lanes_past(nd, WG)
    var inscope glu_out <- row_fill(nd, 31, 7, 0.06, 2.5)
    var inscope wn <- row_fill(2 * d + d * CN_CONV_KERNEL + 2 * d, 7, 1, 0.01, 0.3)
    let taps_off = 2 * d
    let bnw_off = taps_off + d * CN_CONV_KERNEL
    let bnb_off = bnw_off + d
    let kpad = (CN_CONV_KERNEL - 1) / 2
    var inscope wn_tm := wn   // parakeet's layout: the taps tap-major [k][c], the BatchNorm rows as they are
    for (c in range(d)) {
        for (kk in range(CN_CONV_KERNEL)) {
            wn_tm[taps_off + kk * d + c] = wn[taps_off + c * CN_CONV_KERNEL + kk]
        }
    }
    let gp = cp |> in_(glu_out, 0, kpad * d)   // the rows past npos carry poison: a tap past the source's end the bound lets through reads it
    let cpl = cp |> out_(nd, 0, idle)
    let wp = cp |> in_(wn)
    let cpl_pk = cp |> out_(nd, 0, idle)
    let wp_tm = cp |> in_(wn_tm)
    let cpl_ga = cp |> out_(nd, 0, idle)
    var pa_dw = GkDwConvRowsArgs(c = uint(d), k = uint(CN_CONV_KERNEL), t_in = uint(rows), t_out = uint(rows), woff = uint(taps_off), boff = uint(bnb_off),
        bnwoff = uint(bnw_off), bnboff = uint(bnb_off))
    let wgs = int64(round_up(nd, WG) / WG)
    cp |> cell_launch(t, "the conv-tail stamps") $(var rl) {
        enc_tower_cn_dw_cls(rl, seats(fixed_array(gp, cpl, wp)), pa_dw, wgs)
        enc_tower_pk_dw_cls(rl, seats(fixed_array(gp, cpl_pk, wp_tm)), pa_dw, wgs)
        enc_tower_ga_dw_cls(rl, seats(fixed_array(gp, cpl_ga, wp_tm)), pa_dw, wgs)
    }
    var inscope c_dev <- cp |> plane_run(cpl)
    var inscope c_pk_dev <- cp |> plane_run(cpl_pk)
    var inscope c_ga_dev <- cp |> plane_run(cpl_ga)
    var inscope c_ref <- fc_dw_ref(glu_out, wn, rows, d, CN_CONV_KERNEL, taps_off, bnw_off, bnb_off)
    check_rows(t, "tower_cn_dw centered taps, folded BatchNorm and silu vs the CPU loop", c_dev, c_ref.bn, BAR_APPROX, d)
    check_rows(t, "tower_pk_dw the same over tap-major taps vs the CPU loop", c_pk_dev, c_ref.bn, BAR_APPROX, d)
    check_rows(t, "tower_ga_dw the biased centered conv over tap-major taps vs the CPU loop", c_ga_dev, c_ref.ga, BAR_APPROX, d)
    slack_kept(t, "tower_cn_dw", cp, cpl, type<uint>, nd, idle, NAN_SENTINEL_BITS)
    slack_kept(t, "tower_pk_dw", cp, cpl_pk, type<uint>, nd, idle, NAN_SENTINEL_BITS)
    slack_kept(t, "tower_ga_dw", cp, cpl_ga, type<uint>, nd, idle, NAN_SENTINEL_BITS)
}

//! gigaam's f16-tap depthwise conv (`GkDwConvRows` over the carrier's halfwords) vs the CPU loop over the same halfwords, itself
//! bit-exact to the f32 loop over the widened taps; a poisoned halfword tap the control
def ga_dwh_case(t : T?; var cp : auto(C)) {
    for (sh in [(rows = 13, d = 64, taps = 5), (rows = 40, d = 96, taps = 31)]) {
        cp |> cell_clear()
        let tag = "tower_ga_dwh rows {sh.rows} d {sh.d} k {sh.taps}"
        var inscope fx <- dw_f16_fix(sh.rows, sh.d, sh.taps)
        var inscope want <- dw_f16_cpu(fx, false)
        var inscope wide <- dw_f16_cpu(fx, true)
        t |> equal(mismatch_exact(want, wide), 0, "{tag}: the CPU loop over the halfwords is the f32 loop over the widened taps, bit for bit")
        let nd = fx.np * fx.d
        let idle = lanes_past(nd, WG)
        let bar = Bar(rel = 1e-6, abs_floor = 1e-7, poison_scaled = true)
        let gp = cp |> in_(fx.x)
        let yp = cp |> out_(nd, 0, idle)
        let wp = cp |> in_(fx.wn)
        let hp = cp |> in_plain(fx.taps)
        var pa = GkDwConvRowsArgs(c = uint(fx.d), k = uint(fx.nk), t_in = uint(fx.np), t_out = uint(fx.np), woff = 0u, boff = uint(fx.boff))
        for (leg in range(2)) {
            if (leg == 1) {
                fx.taps[2 * fx.d + 7] = uint16(f32_to_f16(3.0))
                cp |> plane_upload(hp, fx.taps)
                cp |> plane_refill(yp)
            }
            cp |> cell_launch(t, "{tag} leg {leg}") $(var rl) {
                enc_tower_ga_dwh_cls(rl, seats(fixed_array(gp, yp, wp, hp)), pa, int64(round_up(nd, WG) / WG))
            }
            var inscope got <- cp |> plane_run(yp)
            if (leg == 0) {
                check_rows(t, "{tag} vs the CPU loop", got, want, bar, fx.d)
                slack_kept(t, tag, cp, yp, type<uint>, nd, idle, NAN_SENTINEL_BITS)
            } else {
                check_exceeds(t, "{tag}: the control - a poisoned halfword tap reds the bars", got, want, bar)
            }
        }
    }
}

//! the chat tails' pair pool and swapped gate (`swiglu_swapped_rows`) at an off-multiple width; the unswapped gate the control
def tail_rows_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let width = 70
    let rows = 5
    let total = rows * width
    let idle = lanes_past(total, WG)
    var inscope x <- row_fill(2 * total, 37, 5, 0.043, 2.1)
    let xp = cp |> in_(x)
    let pp = cp |> out_(total, 0, idle)
    let gp = cp |> out_(total, 0, idle)
    var pa_pool = GkPoolRowsArgs(width = uint(width), total = uint(total))
    var pa_gate = GkGatedRowsArgs(d = uint(width), nelem = uint(total))
    let wgs = int64(round_up(total, WG) / WG)
    cp |> cell_launch(t, "the tail stamps") $(var rl) {
        enc_tower_pool_rows_cls(rl, seats(fixed_array(xp, pp)), pa_pool, wgs)
        enc_tower_swiglu_swapped_cls(rl, seats(fixed_array(xp, gp)), pa_gate, wgs)
    }
    var inscope p_dev <- cp |> plane_run(pp)
    var inscope g_dev <- cp |> plane_run(gp)
    var inscope p_ref <- pool_pairs_ref(x, width, total)
    check_rows(t, "tower_pool_rows the mean of each row pair", p_dev, p_ref, BAR_APPROX, width)
    slack_kept(t, "tower_pool_rows", cp, pp, type<uint>, total, idle, NAN_SENTINEL_BITS)
    var inscope g_ref <- plane_of(total, 0.0)
    swiglu_swapped_rows(g_ref, x, int64(width), int64(rows))
    check_rows(t, "tower_swiglu_swapped silu(second half) . first half vs swiglu_swapped_rows", g_dev, g_ref, BAR_APPROX, width)
    slack_kept(t, "tower_swiglu_swapped", cp, gp, type<uint>, total, idle, NAN_SENTINEL_BITS)
    var inscope swapped <- swiglu_unswapped_ref(x, width, total)
    check_exceeds(t, "tower_swiglu_swapped control: the gate with its halves swapped lands elsewhere", g_dev, swapped, BAR_APPROX)
}

//! the Transformer-XL rel table vs the CPU loop in double: rel 0 exactly (0, 1), and on the padded leg the rows past 2 tt - 1
//! exactly zero
def rel_sincos_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tt = ROWS_N
    let ww = 2 * tt - 1
    let n_rel = ww * REL_D
    let n_pad = (ww + REL_PAD_ROWS) * REL_D
    let idle = lanes_past(n_rel, WG)
    let idle_pad = lanes_past(n_pad, WG)
    var inscope rel_want <- rel_sincos_ref(tt, REL_D)
    let relp = cp |> out_(n_rel, 0, idle)
    let padp = cp |> out_(n_pad, 0, idle_pad)
    var pa_rel = GkRelSinCosArgs(d = uint(REL_D), tt = uint(tt), rows_pad = uint(ww))
    var pa_pad = GkRelSinCosArgs(d = uint(REL_D), tt = uint(tt), rows_pad = uint(ww + REL_PAD_ROWS))
    cp |> cell_launch(t, "the rel table stamp") $(var rl) {
        enc_tower_rel_sincos_cls(rl, seats(fixed_array(relp)), pa_rel, int64(round_up(n_rel, WG) / WG))
        enc_tower_rel_sincos_cls(rl, seats(fixed_array(padp)), pa_pad, int64(round_up(n_pad, WG) / WG))
    }
    var inscope rel_dev <- cp |> plane_run(relp)
    var inscope pad_dev <- cp |> plane_run(padp)
    check_rows(t, "tower_rel_sincos table vs the CPU loop", rel_dev, rel_want, BAR_APPROX, REL_D)
    slack_kept(t, "tower_rel_sincos", cp, relp, type<uint>, n_rel, idle, NAN_SENTINEL_BITS)
    var inscope zero_row <- copy_span(rel_dev, (tt - 1) * REL_D, REL_D)
    var inscope zero_row_want <- [for (i in range(REL_D)); i % 2 == 0 ? 0.0 : 1.0]
    check_exact(t, "tower_rel_sincos: the rel 0 row is exactly (0, 1)", zero_row, zero_row_want)
    var inscope pad_live <- copy_span(pad_dev, 0, n_rel)
    check_rows(t, "tower_rel_sincos rows_pad {ww + REL_PAD_ROWS}: the live rows vs the CPU loop", pad_live, rel_want, BAR_APPROX, REL_D)
    var inscope pad_rows <- copy_span(pad_dev, n_rel, n_pad - n_rel)
    var inscope zeros <- plane_of(n_pad - n_rel, 0.0)
    check_rows(t, "tower_rel_sincos rows_pad {ww + REL_PAD_ROWS}: the {REL_PAD_ROWS} rows past 2 tt - 1 exactly zero", pad_rows, zeros, BAR_EXACT, REL_D)
    slack_kept(t, "tower_rel_sincos rows_pad", cp, padp, type<uint>, n_pad, idle_pad, NAN_SENTINEL_BITS)
}

//! the in-place seam's teeth: the element where the reference moves furthest off the input moved there by more than the bar,
//! and the kernel's output moved off the input there too
def private moved_in_place(t : T?; tag : string; got, want, input : array<float>) {
    let at = maxdiff_at(want, input)
    t |> success(!approx(want[at], input[at]), "{tag}: the reference moves element {at} off the input ({input[at]} -> {want[at]})")
    t |> success(!approx(got[at], input[at]), "{tag}: the kernel moved element {at} off the input ({input[at]} -> {got[at]})")
}

//! the plain seam (`GkPostAddLn` with no post-norm) with the next rms, `ln_on = 0` the seam alone; the post-norm seam's CPU form
//! the control
def post_add_plain_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tower_post_add_plain"
    let d = ROWS_D
    let rows = ROWS_N
    let nd = rows * d
    var inscope x <- row_fill(nd, 41, 7, 0.05, 2.1)
    var inscope y <- row_fill(nd, 17, 3, 0.04, 1.9)
    var inscope wn <- row_fill(2 * d, 7, 1, 0.01, -0.6)   // two weight rows: the pre-norm's at 0, the post-norm's at d - both near 1
    let xp = cp |> io_(x)
    let yp = cp |> in_(y)
    let xbp = cp |> out_(nd, 0, d)
    let x0p = cp |> io_(x)
    let xb0p = cp |> out_(nd, 0, d)
    let wp = cp |> in_(wn)
    var pa_plain = GkPostAddLnArgs(d = uint(d), rows = uint(rows), woff = 0u, boff = 0u, eps = ROWS_EPS, ln_on = 1u, lnoff = 0u, lnboff = 0u, ascale = 0.5, xboff = 0u)
    var pa_plain0 = pa_plain
    pa_plain0.ln_on = 0u
    cp |> cell_launch(t, "the plain seam stamp") $(var rl) {
        enc_tower_post_add_plain_cls(rl, seats(fixed_array(xp, yp, xbp, wp)), pa_plain, int64(rows))
        enc_tower_post_add_plain_cls(rl, seats(fixed_array(x0p, yp, xb0p, wp)), pa_plain0, int64(rows))
    }
    var inscope x_dev <- cp |> plane_run(xp)
    var inscope xb_dev <- cp |> plane_run(xbp)
    var inscope x0_dev <- cp |> plane_run(x0p)
    var inscope xb0_words <- cp |> plane_as(xb0p, type<uint>, nd)
    var inscope xp_ref <- [for (xv, yv in x, y); xv + 0.5 * yv]
    check_rows(t, "{tag} x += 0.5 . y with no post-norm", x_dev, xp_ref, BAR_APPROX, d)
    moved_in_place(t, tag, x_dev, xp_ref, x)
    check_rows(t, "{tag} ln_on = 0 the seam alone lands the same x", x0_dev, xp_ref, BAR_APPROX, d)
    moved_in_place(t, "{tag} ln_on = 0", x0_dev, xp_ref, x)
    var inscope sentinel <- [for (_i in range(nd)); NAN_SENTINEL_BITS]
    check_exact(t, "{tag} ln_on = 0: the pre-norm output stays untouched", xb0_words, sentinel)
    let wplane = PlaneF(p = unsafe(addr<float const?>(wn[0])), n = int64(2 * d))
    var inscope xb_ref <- plane_of(nd, 0.0)
    rms_rows(xb_ref, xp_ref, wplane, 0l, int64(d), int64(rows), ROWS_EPS)
    check_rows(t, "{tag} the next pre-norm over the half-weight rows", xb_dev, xb_ref, BAR_APPROX, d)
    slack_kept(t, tag, cp, xbp, type<uint>, nd, d, NAN_SENTINEL_BITS)
    slack_kept(t, "{tag} ln_on = 0", cp, xb0p, type<uint>, nd, d, NAN_SENTINEL_BITS)
    var inscope ypost <- plane_of(nd, 0.0)
    rms_rows(ypost, y, wplane, int64(d), int64(d), int64(rows), ROWS_EPS)
    var inscope x_post_ref <- [for (xv, pv in x, ypost); xv + pv]
    check_exceeds(t, "{tag} control: the plain stamp is not the post-norm seam", x_dev, x_post_ref, BAR_APPROX)
}

//! the bias-less seam with the next layernorm, `ln_on = 0` the seam alone; the biased seam's form the control
def post_add_ln_nb_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tower_post_add_ln_nb"
    let d = ROWS_D
    let rows = ROWS_N
    let nd = rows * d
    var inscope x <- row_fill(nd, 41, 7, 0.05, 2.1)
    var inscope y <- row_fill(nd, 17, 3, 0.04, 1.9)
    var inscope wn <- row_fill(4 * d, 7, 1, 0.01, -0.6)   // rows: ln w at 0, ln b at d, the bias at 2d, the next ln w at 3d (its bias reads the first ln's)
    let xp = cp |> io_(x)
    let yp = cp |> in_(y)
    let xbp = cp |> out_(nd, 0, d)
    let x0p = cp |> io_(x)
    let xb0p = cp |> out_(nd, 0, d)
    let wp = cp |> in_(wn)
    var pa_post = GkPostAddLnArgs(d = uint(d), rows = uint(rows), boff = uint(2 * d), eps = ROWS_EPS, ln_on = 1u, lnoff = uint(3 * d), lnboff = uint(d), ascale = 1.0, xboff = 0u)
    var pa_post0 = pa_post
    pa_post0.ln_on = 0u
    cp |> cell_launch(t, "the bias-less seam stamp") $(var rl) {
        enc_tower_post_add_ln_nb_cls(rl, seats(fixed_array(xp, yp, xbp, wp)), pa_post, int64(rows))
        enc_tower_post_add_ln_nb_cls(rl, seats(fixed_array(x0p, yp, xb0p, wp)), pa_post0, int64(rows))
    }
    var inscope x_dev <- cp |> plane_run(xp)
    var inscope xb_dev <- cp |> plane_run(xbp)
    var inscope x0_dev <- cp |> plane_run(x0p)
    var inscope xb0_words <- cp |> plane_as(xb0p, type<uint>, nd)
    var inscope xnb_ref <- [for (xv, yv in x, y); xv + yv]
    check_rows(t, "{tag} the seam without the bias row vs x + y", x_dev, xnb_ref, BAR_APPROX, d)
    moved_in_place(t, tag, x_dev, xnb_ref, x)
    check_rows(t, "{tag} ln_on = 0 the seam alone lands the same x", x0_dev, xnb_ref, BAR_APPROX, d)
    moved_in_place(t, "{tag} ln_on = 0", x0_dev, xnb_ref, x)
    var inscope sentinel <- [for (_i in range(nd)); NAN_SENTINEL_BITS]
    check_exact(t, "{tag} ln_on = 0: the layernorm output stays untouched", xb0_words, sentinel)
    var inscope xb_ref <- plane_of(nd, 0.0)
    layernorm_batch(xb_ref, xnb_ref, wn, int64(3 * d), int64(d), int64(d), int64(rows), ROWS_EPS)
    check_rows(t, "{tag} the next layernorm over the bare seam vs layernorm_batch", xb_dev, xb_ref, BAR_APPROX, d)
    slack_kept(t, tag, cp, xbp, type<uint>, nd, d, NAN_SENTINEL_BITS)
    slack_kept(t, "{tag} ln_on = 0", cp, xb0p, type<uint>, nd, d, NAN_SENTINEL_BITS)
    var inscope xb_biased_ref <- [for (i in range(nd)); x[i] + y[i] + wn[2 * d + i % d]]
    check_exceeds(t, "{tag} control: the biased seam's form lands off the bare one (the bias row is live)", x_dev, xb_biased_ref, BAR_APPROX)
}

//! the weighted rms rows alone (`GkPostAddLn`, ADD off, RMS) vs `rms_rows` at `NORM_BAR`, x left as found; the rows under the
//! other weight row the control
def rms_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tower_rms"
    let d = ROWS_D
    let rows = ROWS_N
    let nd = rows * d
    var inscope x <- row_fill(nd, 41, 7, 0.05, 2.1)
    var inscope wn <- row_fill(2 * d, 7, 1, 0.01, -0.6)
    let xp = cp |> io_(x)
    let xbp = cp |> out_(nd, 0, d)
    let wp = cp |> in_(wn)
    var pa = gk_rms_args(uint(d), uint(rows), ROWS_EPS, 0u)
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tower_rms_cls(rl, seats(fixed_array(xp, xbp, wp)), pa, int64(rows))
    }
    var inscope xb_dev <- cp |> plane_run(xbp)
    var inscope x_dev <- cp |> plane_run(xp)
    let wplane = PlaneF(p = unsafe(addr<float const?>(wn[0])), n = int64(2 * d))
    var inscope xb_ref <- plane_of(nd, 0.0)
    rms_rows(xb_ref, x, wplane, 0l, int64(d), int64(rows), ROWS_EPS)
    check_rows(t, "{tag} weighted rms rows vs rms_rows", xb_dev, xb_ref, NORM_BAR, d)
    check_rows(t, "{tag} leaves x as it found it", x_dev, x, BAR_EXACT, d)
    slack_kept(t, tag, cp, xbp, type<uint>, nd, d, NAN_SENTINEL_BITS)
    var inscope xb_other_ref <- plane_of(nd, 0.0)
    rms_rows(xb_other_ref, x, wplane, int64(d), int64(d), int64(rows), ROWS_EPS)
    check_exceeds(t, "{tag} control: the rows under the weight row at d land off the stamp's", xb_dev, xb_other_ref, NORM_BAR)
}

//! the gemma4v seam (`GkPostAddLn`, POST_NORM, RMS): x += ascale . rms(y) . wpost, then the next rms - at ascale 1, at `ln_on = 0`
//! (the rms output untouched) and at ascale 0.5; the add without the post-norm the control
def post_add_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tower_post_add"
    let d = ROWS_D
    let rows = ROWS_N
    let nd = rows * d
    var inscope x <- row_fill(nd, 41, 7, 0.05, 2.1)
    var inscope y <- row_fill(nd, 17, 3, 0.04, 1.9)
    var inscope wn <- row_fill(2 * d, 7, 1, 0.01, -0.6)   // the next rms's weight row at 0, the post-norm's at d
    let xp = cp |> io_(x)
    let yp = cp |> in_(y)
    let xbp = cp |> out_(nd, 0, d)
    let x0p = cp |> io_(x)
    let xb0p = cp |> out_(nd, 0, d)
    let xhp = cp |> io_(x)
    let xbhp = cp |> out_(nd, 0, d)
    let wp = cp |> in_(wn)
    var pa = GkPostAddLnArgs(d = uint(d), rows = uint(rows), woff = uint(d), boff = 0u, eps = ROWS_EPS, ln_on = 1u, lnoff = 0u, lnboff = 0u, ascale = 1.0, xboff = 0u)
    var pa0 = pa
    pa0.ln_on = 0u
    var pah = pa0
    pah.ascale = 0.5
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tower_post_add_cls(rl, seats(fixed_array(xp, yp, xbp, wp)), pa, int64(rows))
        enc_tower_post_add_cls(rl, seats(fixed_array(x0p, yp, xb0p, wp)), pa0, int64(rows))
        enc_tower_post_add_cls(rl, seats(fixed_array(xhp, yp, xbhp, wp)), pah, int64(rows))
    }
    var inscope x_dev <- cp |> plane_run(xp)
    var inscope xb_dev <- cp |> plane_run(xbp)
    var inscope x0_dev <- cp |> plane_run(x0p)
    var inscope xh_dev <- cp |> plane_run(xhp)
    let wplane = PlaneF(p = unsafe(addr<float const?>(wn[0])), n = int64(2 * d))
    var inscope ypost <- plane_of(nd, 0.0)
    rms_rows(ypost, y, wplane, int64(d), int64(d), int64(rows), ROWS_EPS)
    var inscope x_ref <- [for (xv, pv in x, ypost); xv + pv]
    check_rows(t, "{tag} x += rms(y) . wpost", x_dev, x_ref, NORM_BAR, d)
    moved_in_place(t, tag, x_dev, x_ref, x)
    var inscope xb_ref <- plane_of(nd, 0.0)
    rms_rows(xb_ref, x_ref, wplane, 0l, int64(d), int64(rows), ROWS_EPS)
    check_rows(t, "{tag} the next rms over the updated rows", xb_dev, xb_ref, NORM_BAR, d)
    slack_kept(t, tag, cp, xbp, type<uint>, nd, d, NAN_SENTINEL_BITS)
    check_rows(t, "{tag} ln_on = 0 the seam alone lands the same x", x0_dev, x_ref, NORM_BAR, d)
    moved_in_place(t, "{tag} ln_on = 0", x0_dev, x_ref, x)
    var inscope sentinel <- [for (_i in range(nd + d)); NAN_SENTINEL_BITS]
    var inscope xb0_words <- cp |> plane_as(xb0p, type<uint>, nd + d)
    check_exact(t, "{tag} ln_on = 0: the rms output and its slack stay untouched", xb0_words, sentinel)
    var inscope xh_ref <- [for (xv, pv in x, ypost); xv + 0.5 * pv]
    check_rows(t, "{tag} ascale = 0.5 x += 0.5 . rms(y) . wpost", xh_dev, xh_ref, NORM_BAR, d)
    moved_in_place(t, "{tag} ascale = 0.5", xh_dev, xh_ref, x)
    var inscope xbh_words <- cp |> plane_as(xbhp, type<uint>, nd + d)
    check_exact(t, "{tag} ascale = 0.5, ln_on = 0: the rms output and its slack stay untouched", xbh_words, sentinel)
    var inscope x_raw_ref <- [for (xv, yv in x, y); xv + yv]
    check_exceeds(t, "{tag} control: the add without the branch's post-norm lands off the seam (the post-norm is live)", x_dev, x_raw_ref, NORM_BAR)
}

//! the qwen25v seam (`GkPostAddLn`, RMS): x += y + b, then the next rms, and at `ln_on = 0` the seam alone with the rms output
//! untouched; the add without the bias row the control
def post_add_rms_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tower_post_add_rms"
    let d = ROWS_D
    let rows = ROWS_N
    let nd = rows * d
    var inscope x <- row_fill(nd, 41, 7, 0.05, 2.1)
    var inscope y <- row_fill(nd, 17, 3, 0.04, 1.9)
    var inscope wn <- row_fill(2 * d, 7, 1, 0.01, -0.6)   // the branch's bias row at 0, the next rms's weight row at d
    let xp = cp |> io_(x)
    let yp = cp |> in_(y)
    let xbp = cp |> out_(nd, 0, d)
    let x0p = cp |> io_(x)
    let xb0p = cp |> out_(nd, 0, d)
    let wp = cp |> in_(wn)
    var pa = GkPostAddLnArgs(d = uint(d), rows = uint(rows), woff = 0u, boff = 0u, eps = ROWS_EPS, ln_on = 1u, lnoff = uint(d), lnboff = 0u, ascale = 1.0, xboff = 0u)
    var pa0 = pa
    pa0.ln_on = 0u
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tower_post_add_rms_cls(rl, seats(fixed_array(xp, yp, xbp, wp)), pa, int64(rows))
        enc_tower_post_add_rms_cls(rl, seats(fixed_array(x0p, yp, xb0p, wp)), pa0, int64(rows))
    }
    var inscope x_dev <- cp |> plane_run(xp)
    var inscope xb_dev <- cp |> plane_run(xbp)
    var inscope x0_dev <- cp |> plane_run(x0p)
    var inscope x_ref <- [for (i in range(nd)); x[i] + y[i] + wn[i % d]]
    check_rows(t, "{tag} x += y + b", x_dev, x_ref, BAR_APPROX, d)
    moved_in_place(t, tag, x_dev, x_ref, x)
    let wplane = PlaneF(p = unsafe(addr<float const?>(wn[0])), n = int64(2 * d))
    var inscope xb_ref <- plane_of(nd, 0.0)
    rms_rows(xb_ref, x_ref, wplane, int64(d), int64(d), int64(rows), ROWS_EPS)
    check_rows(t, "{tag} the next weighted rms over the updated rows", xb_dev, xb_ref, NORM_BAR, d)
    slack_kept(t, tag, cp, xbp, type<uint>, nd, d, NAN_SENTINEL_BITS)
    check_rows(t, "{tag} ln_on = 0 the seam alone lands the same x", x0_dev, x_ref, BAR_APPROX, d)
    moved_in_place(t, "{tag} ln_on = 0", x0_dev, x_ref, x)
    var inscope sentinel <- [for (_i in range(nd + d)); NAN_SENTINEL_BITS]
    var inscope xb0_words <- cp |> plane_as(xb0p, type<uint>, nd + d)
    check_exact(t, "{tag} ln_on = 0: the rms output and its slack stay untouched", xb0_words, sentinel)
    var inscope x_nb_ref <- [for (xv, yv in x, y); xv + yv]
    check_exceeds(t, "{tag} control: the add without the bias row lands off the seam (the bias row is live)", x_dev, x_nb_ref, BAR_APPROX)
}

//! the half rows of a norm stamp widened: `n` halves of plane `p` as floats
def private halves_run(cp : auto(C); p, n : int) : array<float> {
    var inscope h <- cp |> plane_as(p, type<uint16>, n)
    return <- [for (hv in h); f16_to_f32(uint(hv))]
}

//! the layernorm into halves (`GkPostAddLn`, ADD off, OUT16) bit for bit the f32 stamp's rows rounded - one inverse spelling a home
//! - and both against `layernorm_batch` at `NORM_BAR`, which a half's rounding (4.9e-4) keeps; the bias row dropped the control
def ln16_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tower_ln16"
    let d = ROWS_D
    let rows = ROWS_N
    let nd = rows * d
    var inscope x <- row_fill(nd, 41, 7, 0.05, 2.1)
    var inscope wn <- row_fill(2 * d, 7, 1, 0.01, -0.6)   // the layernorm's weight row at 0, its bias row at d
    let xp = cp |> io_(x)
    let xbp = cp |> out_(nd, 0, d)
    let xbhp = cp |> out_words(int64(nd + d) * 2l, F16_SENTINEL_BITS)
    let wp = cp |> in_(wn)
    var pa = gk_ln_args(uint(d), uint(rows), ROWS_EPS, 0u, uint(d))
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tower_ln_cls(rl, seats(fixed_array(xp, xbp, wp)), pa, int64(rows))
        enc_tower_ln16_cls(rl, seats(fixed_array(xp, wp, xbhp)), pa, int64(rows))
    }
    var inscope xb_dev <- cp |> plane_run(xbp)
    var inscope xbh_dev <- cp |> plane_as(xbhp, type<uint16>, nd)
    var inscope xbh_f <- halves_run(cp, xbhp, nd)
    var inscope xb_ref <- plane_of(nd, 0.0)
    layernorm_batch(xb_ref, x, wn, 0l, int64(d), int64(d), int64(rows), ROWS_EPS)
    check_rows(t, "tower_ln the f32 rows vs layernorm_batch", xb_dev, xb_ref, NORM_BAR, d)
    check_halves(t, "{tag}: the half rows are the f32 stamp's rows rounded to half", xbh_dev, xb_dev)
    check_rows(t, "{tag} the half rows widened vs layernorm_batch", xbh_f, xb_ref, NORM_BAR, d)
    slack_kept(t, tag, cp, xbhp, type<uint16>, nd, d, uint16(F16_SENTINEL))
    slack_kept(t, "tower_ln", cp, xbp, type<uint>, nd, d, NAN_SENTINEL_BITS)
    var inscope xb_nb_ref <- [for (i in range(nd)); xb_ref[i] - wn[d + i % d]]
    check_exceeds(t, "{tag} control: the layernorm without its bias row lands off the half rows (the bias row is live)", xbh_f, xb_nb_ref, NORM_BAR)
}

//! the biased seam with the next layernorm into halves (`GkPostAddLn`, OUT16), at `ln_on = 0` the seam alone with the halves
//! untouched: the halves bit for bit the f32 stamp's rounded and against `layernorm_batch` at `NORM_BAR`; the add without the bias
//! the control
def post_add_ln16_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tower_post_add_ln16"
    let d = ROWS_D
    let rows = ROWS_N
    let nd = rows * d
    var inscope x <- row_fill(nd, 41, 7, 0.05, 2.1)
    var inscope y <- row_fill(nd, 17, 3, 0.04, 1.9)
    var inscope wn <- row_fill(3 * d, 7, 1, 0.01, -0.6)   // the next layernorm's bias row at 0, the branch's bias at d, its weight row at 2d
    let x32p = cp |> io_(x)
    let yp = cp |> in_(y)
    let xb32p = cp |> out_(nd, 0, d)
    let xp = cp |> io_(x)
    let xbhp = cp |> out_words(int64(nd + d) * 2l, F16_SENTINEL_BITS)
    let x0p = cp |> io_(x)
    let xbh0p = cp |> out_words(int64(nd + d) * 2l, F16_SENTINEL_BITS)
    let wp = cp |> in_(wn)
    var pa = gk_add_ln_args(uint(d), uint(rows), ROWS_EPS, uint(d), true, uint(2 * d), 0u)
    var pa0 = gk_add_ln_args(uint(d), uint(rows), ROWS_EPS, uint(d), false, uint(2 * d), 0u)
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tower_post_add_ln_cls(rl, seats(fixed_array(x32p, yp, xb32p, wp)), pa, int64(rows))
        enc_tower_post_add_ln16_cls(rl, seats(fixed_array(xp, yp, wp, xbhp)), pa, int64(rows))
        enc_tower_post_add_ln16_cls(rl, seats(fixed_array(x0p, yp, wp, xbh0p)), pa0, int64(rows))
    }
    var inscope xb32_dev <- cp |> plane_run(xb32p)
    var inscope x_dev <- cp |> plane_run(xp)
    var inscope x0_dev <- cp |> plane_run(x0p)
    var inscope xbh_dev <- cp |> plane_as(xbhp, type<uint16>, nd)
    var inscope xbh_f <- halves_run(cp, xbhp, nd)
    var inscope x_ref <- [for (i in range(nd)); x[i] + y[i] + wn[d + i % d]]
    check_rows(t, "{tag} x += y + b", x_dev, x_ref, BAR_APPROX, d)
    moved_in_place(t, tag, x_dev, x_ref, x)
    check_rows(t, "{tag} ln_on = 0 the seam alone lands the same x", x0_dev, x_ref, BAR_APPROX, d)
    moved_in_place(t, "{tag} ln_on = 0", x0_dev, x_ref, x)
    var inscope xbh0_words <- cp |> plane_as(xbh0p, type<uint>, (nd + d) / 2)
    var inscope sentinel <- [for (_i in range((nd + d) / 2)); F16_SENTINEL_BITS]
    check_exact(t, "{tag} ln_on = 0: the half layernorm output and its slack stay untouched", xbh0_words, sentinel)
    var inscope xb_ref <- plane_of(nd, 0.0)
    layernorm_batch(xb_ref, x_ref, wn, int64(2 * d), 0l, int64(d), int64(rows), ROWS_EPS)
    check_rows(t, "tower_post_add_ln the f32 twin's next layernorm vs layernorm_batch", xb32_dev, xb_ref, NORM_BAR, d)
    check_halves(t, "{tag}: the half rows are the f32 stamp's rows rounded to half", xbh_dev, xb32_dev)
    check_rows(t, "{tag} the next layernorm's half rows widened vs layernorm_batch", xbh_f, xb_ref, NORM_BAR, d)
    slack_kept(t, tag, cp, xbhp, type<uint16>, nd, d, uint16(F16_SENTINEL))
    var inscope x_nb_ref <- [for (xv, yv in x, y); xv + yv]
    check_exceeds(t, "{tag} control: the add without the bias row lands off the seam (the bias row is live)", x_dev, x_nb_ref, BAR_APPROX)
}

//! canary's rel-plane attention on both head stamps vs the rel-pos loop with u / v biases, a row a lane: the idle lanes' rows
//! past `rows` keep the sentinel; scaled keys and a scaled rel table the controls
def cn_attn_rel_plane_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let d = ROWS_D
    let rows = ROWS_N
    let hs = d / ROWS_HEADS
    let heads2 = ROWS_HEADS / 2
    let hs2 = d / heads2
    let nd = rows * d
    let ww = 2 * rows - 1
    let nidle = lanes_past(rows, 64) * d
    var inscope q <- row_fill(nd, 41, 3, 0.03, 1.1)
    var inscope k <- row_fill(nd, 17, 11, 0.04, 1.3)
    var inscope v <- row_fill(nd, 29, 13, 0.05, 1.7)
    var inscope posl <- row_fill(ww * d, 13, 2, 0.02, 0.9)
    var inscope wn <- row_fill(2 * d, 7, 1, 0.01, 0.3)   // bias_u at 0, bias_v at d
    let wg = int64(round_up(rows, 64) / 64)
    let qp = cp |> in_(q)
    let kp = cp |> in_(k)
    let vp = cp |> in_(v)
    let aop = cp |> out_(nd, 0, nidle)
    let ao2p = cp |> out_(nd, 0, nidle)
    let wp = cp |> in_(wn)
    var inscope head_rel_planes <- [for (hh in range(ROWS_HEADS + heads2));   // a plane a head: the 64-wide heads first, then the two 128-wide ones
        cp |> in_(cn_rel_plane(q, posl, wn, rows, ww, d, hh < ROWS_HEADS ? hs : hs2, hh < ROWS_HEADS ? hh * hs : (hh - ROWS_HEADS) * hs2, d))]
    cp |> cell_launch(t, "the rel-plane attention stamps") $(var rl) {
        for (hh in range(ROWS_HEADS)) {
            var pa = TowerCnAttnRArgs(d = uint(d), npos = uint(rows), buoff = 0u, hoff = uint(hh * hs))
            enc_tower_cn_attn_r64_cls(rl, seats(fixed_array(qp, kp, vp, aop, head_rel_planes[hh], wp)), pa, wg)
        }
        for (hh in range(heads2)) {
            var pa = TowerCnAttnRArgs(d = uint(d), npos = uint(rows), buoff = 0u, hoff = uint(hh * hs2))
            enc_tower_cn_attn_r_cls(rl, seats(fixed_array(qp, kp, vp, ao2p, head_rel_planes[ROWS_HEADS + hh], wp)), pa, wg)
        }
    }
    var inscope ao_dev <- cp |> plane_run(aop)
    var inscope ao2_dev <- cp |> plane_run(ao2p)
    slack_kept(t, "tower_cn_attn_r64: the idle lanes' rows", cp, aop, type<uint>, nd, nidle, NAN_SENTINEL_BITS)
    slack_kept(t, "tower_cn_attn_r: the idle lanes' rows", cp, ao2p, type<uint>, nd, nidle, NAN_SENTINEL_BITS)
    var inscope ao_ref <- cn_attn_ref(q, k, v, posl, wn, rows, d, ROWS_HEADS, hs, 0, d)
    var inscope ao2_ref <- cn_attn_ref(q, k, v, posl, wn, rows, d, heads2, hs2, 0, d)
    check_rows(t, "tower_cn_attn_r64 rel-plane attention over four 64-wide heads vs the CPU loop", ao_dev, ao_ref, BAR_APPROX, d, hs)
    check_rows(t, "tower_cn_attn_r rel-plane attention over two 128-wide heads vs the CPU loop", ao2_dev, ao2_ref, BAR_APPROX, d, hs2)
    var inscope k_scaled <- scaled_copy(k, 1.5)
    var inscope posl_scaled <- scaled_copy(posl, 1.5)
    var inscope ctl_k <- cn_attn_ref(q, k_scaled, v, posl, wn, rows, d, ROWS_HEADS, hs, 0, d)
    var inscope ctl_p <- cn_attn_ref(q, k, v, posl_scaled, wn, rows, d, heads2, hs2, 0, d)
    check_exceeds(t, "tower_cn_attn_r64 control: scaled keys land the oracle elsewhere", ao_dev, ctl_k, BAR_APPROX)
    check_exceeds(t, "tower_cn_attn_r control: a scaled rel table lands the oracle elsewhere", ao2_dev, ctl_p, BAR_APPROX)
}

//! the whisper decoder's K/V planes as the store must leave them, as bits
struct private WdecKvWant {
    k : array<uint16>    //! the f16 keys at every position of the history then the batch, the half sentinel past it
    v : array<uint16>    //! ... the values, the v bias folded
    kx : array<uint>     //! the CPU chain's transposed, hs^-0.25-scaled keys at the batch's positions alone, the NaN sentinel elsewhere
    vx : array<uint>     //! ... its biased values
}

def private wdec_kv_store_want(hist, src, wn : array<float>; heads, hs, tstride, p0, rows, dim, d : int; kscale : float) : WdecKvWant {
    let n = heads * tstride * hs
    var w <- WdecKvWant(k <- [for (_i in range(n)); uint16(F16_SENTINEL)], v <- [for (_i in range(n)); uint16(F16_SENTINEL)],
        kx <- [for (_i in range(n)); NAN_SENTINEL_BITS], vx <- [for (_i in range(n)); NAN_SENTINEL_BITS])
    for (h in range(heads)) {
        for (tt in range(p0 + rows)) {
            for (j in range(hs)) {
                let pi = (h * tstride + tt) * hs + j
                let srow = tt < p0 ? hist[tt * dim + d + h * hs + j] : src[(tt - p0) * dim + d + h * hs + j]
                let vrow = (tt < p0 ? hist[tt * dim + 2 * d + h * hs + j] : src[(tt - p0) * dim + 2 * d + h * hs + j]) + wn[d + h * hs + j]
                w.k[pi] = uint16(f32_to_f16(srow))
                w.v[pi] = uint16(f32_to_f16(vrow))
                if (tt >= p0) {
                    w.kx[(h * hs + j) * tstride + tt] = float_bits_to_uint(srow * kscale)
                    w.vx[pi] = float_bits_to_uint(vrow)
                }
            }
        }
    }
    return <- w
}

//! the K/V store's args over the cell's fused [q | k | v] rows: `rows` rows landing from position `p0`, the k slot or - `v` - the
//! v slot with its bias row folded, in layout `mode`
def private wdec_kv_args(rows, p0 : int; v : bool; mode : uint; kscale : float) : GkWdecKvArgs {
    let d = WDEC_HEADS * int(WDEC_HS)
    return GkWdecKvArgs(rows = uint(rows), sstride = uint(3 * d), soff = uint(v ? 2 * d : d), nheads = uint(WDEC_HEADS), tstride = uint(WDEC_TSTRIDE),
        p0 = uint(p0), dst_off = 0u, boff = uint(v ? d : 0), bias_on = v ? 1u : 0u, mode = mode, hs = WDEC_HS, kscale = kscale)
}

//! the four K/V planes the store wrote - `kp` / `vp` the f16 ones, `kxp` / `vxp` the CPU chain's - each against its expected
//! plane bit for bit, every slot
def private wdec_kv_store_check(t : T?; cp : auto(C); kp, vp, kxp, vxp, nkv : int; want : WdecKvWant) {
    var inscope k_dev <- cp |> plane_as(kp, type<uint16>, nkv)
    var inscope v_dev <- cp |> plane_as(vp, type<uint16>, nkv)
    var inscope kx_dev <- cp |> plane_as(kxp, type<uint>, nkv)
    var inscope vx_dev <- cp |> plane_as(vxp, type<uint>, nkv)
    check_exact(t, "tower_wdec_kv F16: the keys land [h][t][hs] at their positions, the history then the batch, the positions past it the sentinel", k_dev, want.k)
    check_exact(t, "tower_wdec_kv F16: the values land with the bias row folded, the positions past the batch the sentinel", v_dev, want.v)
    check_exact(t, "tower_wdec_kv KX: the CPU chain's transposed, hs^-0.25-scaled keys at the batch's positions alone", kx_dev, want.kx)
    check_exact(t, "tower_wdec_kv VX: the CPU chain's biased values at the batch's positions alone", vx_dev, want.vx)
}

//! the whisper decoder's K/V store in its three layouts and the chunked attention pair over the planes it wrote: the causal
//! rows across the chunk boundary, then - the partials refilled - the cross rows inside the first chunk with the second chunk's
//! partial empty; the causal-versus-cross and the scaled-keys controls
def wdec_classes_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let hs = int(WDEC_HS)
    let heads = WDEC_HEADS
    let d = heads * hs
    let dim = 3 * d
    let tstride = WDEC_TSTRIDE
    let rows = WDEC_ROWS
    let p0 = WDEC_P0
    let scale = 1.0 / sqrt(float(hs))
    let kscale = wdec_kscale(int64(hs))
    let nkv = heads * tstride * hs
    let nch = (tstride + int(WDEC_CHUNK) - 1) / int(WDEC_CHUNK)
    var inscope src <- row_fill(rows * dim, 31, 5, 0.05, 2.3)        // the batch's fused [q | k | v] rows
    var inscope hist <- row_fill(p0 * dim, 23, 7, 0.04, 1.7)         // the positions before the batch, the same row shape
    var inscope wn <- row_fill(2 * d, 11, 3, 0.02, 0.9)              // the q bias row at 0, the v bias row at d
    let srcp = cp |> in_(src)
    let histp = cp |> in_(hist)
    let wp = cp |> in_(wn)
    let kp = cp |> out_words(int64(nkv) * 2l, F16_SENTINEL_BITS)
    let vp = cp |> out_words(int64(nkv) * 2l, F16_SENTINEL_BITS)
    let kxp = cp |> out_(nkv)
    let vxp = cp |> out_(nkv)
    let pmsp = cp |> out_(nch * heads * rows * 2)
    let pyp = cp |> out_(nch * heads * rows * hs)
    let yp = cp |> out_(rows * d)
    let ycp = cp |> out_(rows * d)
    var pa_kh = wdec_kv_args(p0, 0, false, WDEC_KV_F16, kscale)
    var pa_vh = wdec_kv_args(p0, 0, true, WDEC_KV_F16, kscale)
    var pa_kb = wdec_kv_args(rows, p0, false, WDEC_KV_F16, kscale)
    var pa_vb = wdec_kv_args(rows, p0, true, WDEC_KV_F16, kscale)
    var pa_kx = wdec_kv_args(rows, p0, false, WDEC_KV_KX, kscale)
    var pa_vx = wdec_kv_args(rows, p0, true, WDEC_KV_VX, kscale)
    var pa_as = GkWdecAttnArgs(dim = uint(dim), tstride = uint(tstride), len0 = uint(p0 + 1), causal = 1u, nheads = uint(heads),
        nrows = uint(rows), koff = 0u, voff = 0u, qboff = 0u, hs = uint(hs), scale = scale)
    var pa_ac = pa_as
    pa_ac.len0 = uint(WDEC_CROSS_KEYS)
    pa_ac.causal = 0u
    var pa_c = GkWdecCombArgs(dim = uint(d), nchunks = uint(nch), nheads = uint(heads), nrows = uint(rows), hs = uint(hs))
    let wg_h = int64(round_up(p0 * d, WG) / WG)
    let wg_b = int64(round_up(rows * d, WG) / WG)
    let wg_attn = int64(nch * heads * rows)
    let wg_comb = int64(heads * rows)
    cp |> cell_launch(t, "tower_wdec: the stores and the causal rows") $(var rl) {
        enc_tower_wdec_kv16_cls(rl, seats(fixed_array(histp, kp, wp)), pa_kh, wg_h)
        enc_tower_wdec_kv16_cls(rl, seats(fixed_array(histp, vp, wp)), pa_vh, wg_h)
        enc_tower_wdec_kv16_cls(rl, seats(fixed_array(srcp, kp, wp)), pa_kb, wg_b)
        enc_tower_wdec_kv16_cls(rl, seats(fixed_array(srcp, vp, wp)), pa_vb, wg_b)
        enc_tower_wdec_kv_cls(rl, seats(fixed_array(srcp, kxp, wp)), pa_kx, wg_b)
        enc_tower_wdec_kv_cls(rl, seats(fixed_array(srcp, vxp, wp)), pa_vx, wg_b)
        enc_tower_wdec_attn_part_cls(rl, seats(fixed_array(srcp, kp, vp, pmsp, pyp, wp)), pa_as, wg_attn)
        enc_tower_wdec_attn_comb_cls(rl, seats(fixed_array(pmsp, pyp, yp)), pa_c, wg_comb)
    }
    cp |> plane_refill(pmsp)
    cp |> plane_refill(pyp)
    cp |> cell_launch(t, "tower_wdec: the cross rows") $(var rl) {
        enc_tower_wdec_attn_part_cls(rl, seats(fixed_array(srcp, kp, vp, pmsp, pyp, wp)), pa_ac, wg_attn)
        enc_tower_wdec_attn_comb_cls(rl, seats(fixed_array(pmsp, pyp, ycp)), pa_c, wg_comb)
    }
    var inscope want <- wdec_kv_store_want(hist, src, wn, heads, hs, tstride, p0, rows, dim, d, kscale)
    wdec_kv_store_check(t, cp, kp, vp, kxp, vxp, nkv, want)
    var inscope k_dev <- cp |> plane_as(kp, type<uint16>, nkv)
    var inscope v_dev <- cp |> plane_as(vp, type<uint16>, nkv)
    var inscope y_dev <- cp |> plane_run(yp)
    var inscope yc_dev <- cp |> plane_run(ycp)
    var inscope y_ref <- wdec_attn_ref(src, k_dev, v_dev, wn, rows, dim, d, heads, hs, tstride, p0 + 1, 0, true, scale)
    check_rows(t, "tower_wdec_attn part + comb, causal, across the chunk boundary vs the CPU form", y_dev, y_ref, BAR_APPROX, d, hs)
    var inscope yc_ref <- wdec_attn_ref(src, k_dev, v_dev, wn, rows, dim, d, heads, hs, tstride, WDEC_CROSS_KEYS, 0, false, scale)
    check_rows(t, "tower_wdec_attn part + comb, cross, over {WDEC_CROSS_KEYS} keys (the second chunk empty) vs the CPU form", yc_dev, yc_ref, BAR_APPROX, d, hs)
    check_exceeds(t, "tower_wdec_attn control: the causal rows are not the cross rows", y_dev, yc_ref, BAR_APPROX)
    var inscope ys_ref <- wdec_attn_ref(src, k_dev, v_dev, wn, rows, dim, d, heads, hs, tstride, p0 + 1, 0, true, scale * 2.0)
    check_exceeds(t, "tower_wdec_attn control: the keys scaled twice move the rows", y_dev, ys_ref, BAR_APPROX)
}

let private FUSED_PAIRS = 4          //! the fused cell's Q8_0 pairs: the separate pass's at an even index, the fused stamp's at the odd one after it
let private FUSED_BIAS_PAIR = 2      //! the bias arm's pair, whose planes are the ff-wide hidden's

//! The fused cell's planes: the separate passes' and the fused stamps' inputs and outputs, and the four Q8_0 pairs
struct private FusedPlanes {
    x : int
    x2 : int
    x3 : int
    xb : int
    xb2 : int
    y : int
    yc : int
    h : int
    h2 : int
    wn : int
    pms : int
    py : int
    q : int[8]
    s : int[8]
}

//! the elements of Q8_0 pair `i`'s source: the hidden's for the bias arm, the rows' for the others
def private pair_elems(i, nd, nff : int) : int => i / 2 == FUSED_BIAS_PAIR ? nff : nd

//! the Q8_0 quant words a requant grid's lanes reach past `n` elements (a lane four elements), and the scales past `n / 32`
def private q8_slack(n : int) : tuple<words : int; blocks : int> {
    let lanes = round_up(n / 4, WG)
    return (words = lanes - n / 4, blocks = lanes / 8 - n / int(kq_elems(KqFmt.q8)))
}

//! the four fused row passes and their separate twins in the Vulkan cell's order, the separate passes' requant `TowerClampRq`
def private fused_rq_encode(var rl : auto(R); b : FusedPlanes; d, rows, nch : int) {
    let q8e = int(kq_elems(KqFmt.q8))
    let ff = 4 * d
    let nd = rows * d
    let nff = rows * ff
    let heads = d / int(WDEC_HS)
    var pa_ln = GkPostAddLnArgs(d = uint(d), rows = uint(rows), eps = ROWS_EPS, ln_on = 1u, lnoff = 0u, lnboff = uint(d), xboff = 0u)
    var pa_rq = RqArgs(inbase = 0u, nblk = uint(nd / q8e), lo = -3.0e38, hi = 3.0e38)
    var pa_rqh = RqArgs(inbase = 0u, nblk = uint(nff / q8e), lo = -3.0e38, hi = 3.0e38)
    var pa_post = GkPostAddLnArgs(d = uint(d), rows = uint(rows), boff = uint(2 * d), eps = ROWS_EPS, ln_on = 1u, lnoff = uint(3 * d), lnboff = uint(4 * d), ascale = 1.0, xboff = 0u)
    var pa_bias = GkBiasActArgs(d = uint(ff), nelem = uint(nff), boff = uint(5 * d), act = BIAS_ACT_GELU_TANH)
    var pa_biasrq = TowerBiasRqArgs(d = uint(ff), nblk = uint(nff / q8e), boff = uint(5 * d))
    var pa_c = GkWdecCombArgs(dim = uint(d), nchunks = uint(nch), nheads = uint(heads), nrows = uint(rows), hs = WDEC_HS)
    let wg4 = int64(round_up(nd / 4, WG) / WG)
    let wg4h = int64(round_up(nff / 4, WG) / WG)
    let wg_comb = int64(heads * rows)
    enc_tower_ln_cls(rl, seats(fixed_array(b.x, b.xb, b.wn)), pa_ln, int64(rows))
    enc_tower_clamp_rq_cls(rl, seats(fixed_array(b.xb, b.q[0], b.s[0])), pa_rq, wg4)
    enc_tower_ln_rq_cls(rl, seats(fixed_array(b.x, b.wn, b.q[1], b.s[1])), pa_ln, int64(rows))
    enc_tower_post_add_ln_cls(rl, seats(fixed_array(b.x2, b.y, b.xb2, b.wn)), pa_post, int64(rows))
    enc_tower_clamp_rq_cls(rl, seats(fixed_array(b.xb2, b.q[2], b.s[2])), pa_rq, wg4)
    enc_tower_post_add_ln_rq_cls(rl, seats(fixed_array(b.x3, b.y, b.wn, b.q[3], b.s[3])), pa_post, int64(rows))
    enc_tower_bias_act_cls(rl, seats(fixed_array(b.h, b.wn)), pa_bias, wg4h)
    enc_tower_clamp_rq_cls(rl, seats(fixed_array(b.h, b.q[4], b.s[4])), pa_rqh, wg4h)
    enc_tower_bias_act_rq_cls(rl, seats(fixed_array(b.h2, b.wn, b.q[5], b.s[5])), pa_biasrq, wg4h)
    enc_tower_wdec_attn_comb_cls(rl, seats(fixed_array(b.pms, b.py, b.yc)), pa_c, wg_comb)
    enc_tower_clamp_rq_cls(rl, seats(fixed_array(b.yc, b.q[6], b.s[6])), pa_rq, wg4)
    enc_tower_wdec_attn_comb_rq_cls(rl, seats(fixed_array(b.pms, b.py, b.q[7], b.s[7])), pa_c, wg_comb)
}

//! every partial's exp-sum (the odd slot of its [max | sum] pair) made positive, as a softmax's is: the combine divides by their total
def private positive_sums(var pms : array<float>) {
    for (i in range(length(pms) / 2)) {
        pms[2 * i + 1] = abs(pms[2 * i + 1]) + 0.25
    }
}

//! Q8_0 pair `pair`'s two forms - the pass then the requant, the fused stamp - against `cpu_rows` (`width` wide) requantized as
//! the CPU does: one quant step, the scales at the approx bar
def private pair_vs_cpu(t : T?; tag : string; qs : array<array<int8>>; ss : array<array<float>>; pair : int; cpu_rows : array<float>; width, rows : int) {
    var inscope want <- q8_rows_ref(cpu_rows, width, rows)
    check_q8(t, "{tag}, the pass then the requant, vs the CPU form requantized", qs[2 * pair], ss[2 * pair], want.q, want.s, APPROX_REL, 1)
    check_q8(t, "{tag}, the fused stamp, vs the CPU form requantized", qs[2 * pair + 1], ss[2 * pair + 1], want.q, want.s, APPROX_REL, 1)
}

//! the four fused row passes at row width `d` (a 64-multiple), `rows` and `nch` chunks, each byte for byte and scale for scale
//! against the pass then `TowerClampRq`, every pass and stamp held to its CPU form as well, a poisoned input moving each fused
//! stamp's bytes
def private wdec_fused_rq_shape(t : T?; var cp : auto(C); d, rows, nch : int) {   // nolint:STYLE038 — one flat gate over the four fused classes: the fixture, the pairs, the oracles and their poisons
    cp |> cell_clear()
    let q8e = int(kq_elems(KqFmt.q8))
    let hs = int(WDEC_HS)
    let ff = 4 * d
    let heads = d / hs
    let nd = rows * d
    let nff = rows * ff
    let shape = "d {d}, {rows} rows, {nch} chunks"
    var inscope x <- row_fill(nd, 29, 3, 0.03, 1.4)
    var inscope y <- row_fill(nd, 17, 11, 0.05, 2.1)
    var inscope hrows <- row_fill(nff, 13, 5, 0.06, 2.8)
    var inscope wn <- row_fill(5 * d + ff, 7, 2, 0.02, 0.9)   // the layernorm's w at 0 and b at d, the post-add bias at 2d, the next norm's w at 3d and b at 4d, the activation's bias row at 5d
    var inscope pms <- row_fill(nch * heads * rows * 2, 19, 1, 0.01, 0.3)
    positive_sums(pms)
    var inscope py <- row_fill(nch * heads * rows * hs, 23, 9, 0.04, 1.9)
    var b : FusedPlanes
    b.x = cp |> in_(x)
    b.x2 = cp |> io_(x)
    b.x3 = cp |> io_(x)
    b.xb = cp |> out_(nd)
    b.xb2 = cp |> out_(nd)
    b.y = cp |> in_(y)
    b.yc = cp |> out_(nd)
    b.h = cp |> io_(hrows)
    b.h2 = cp |> in_(hrows)
    b.wn = cp |> in_(wn)
    b.pms = cp |> in_(pms)
    b.py = cp |> in_(py)
    for (i in range(2 * FUSED_PAIRS)) {
        let n = pair_elems(i, nd, nff)
        let sl = q8_slack(n)
        b.q[i] = cp |> out_words(int64(n + 4 * sl.words), I8_SENTINEL_BITS)
        b.s[i] = cp |> out_(n / q8e, 0, sl.blocks)
    }
    cp |> cell_launch(t, "the fused row passes ({shape})") $(var rl) {
        fused_rq_encode(rl, b, d, rows, nch)
    }
    var qs : array<array<int8>>
    var ss : array<array<float>>
    qs |> reserve(2 * FUSED_PAIRS)
    ss |> reserve(2 * FUSED_PAIRS)
    for (i in range(2 * FUSED_PAIRS)) {
        let n = pair_elems(i, nd, nff)
        let sl = q8_slack(n)
        qs |> emplace(cp |> plane_as(b.q[i], type<int8>, n))
        ss |> emplace(cp |> plane_run(b.s[i]))
        slack_kept(t, "Q8_0 pair {i / 2} plane {i % 2} ({shape}): the quant words past the run", cp, b.q[i], type<uint>, n / 4, sl.words, I8_SENTINEL_BITS)
        slack_kept(t, "Q8_0 pair {i / 2} plane {i % 2} ({shape}): the scales past the run", cp, b.s[i], type<uint>, n / q8e, sl.blocks, NAN_SENTINEL_BITS)
    }
    var inscope x2 <- cp |> plane_run(b.x2)
    var inscope x3 <- cp |> plane_run(b.x3)
    var inscope xb_dev <- cp |> plane_run(b.xb)
    var inscope xb2_dev <- cp |> plane_run(b.xb2)
    var inscope h_dev <- cp |> plane_run(b.h)
    var inscope yc_dev <- cp |> plane_run(b.yc)
    let names <- ["layernorm", "post-add + next norm", "bias + tanh GELU", "attention combine"]
    for (i in range(FUSED_PAIRS)) {
        check_q8(t, "{names[i]} ({shape}) fused with the requant lands the separate pass's Q8_0 bytes and scales", qs[2 * i + 1], ss[2 * i + 1], qs[2 * i], ss[2 * i], 0.0, 0)
        check_written(t, "{names[i]} ({shape}): every block written", qs[2 * i + 1], I8_SENTINEL)
    }
    check_exact(t, "the fused post-add ({shape}) updates the residual rows in place as the separate pass does", x3, x2)
    var inscope xb_ref <- plane_of(nd, 0.0)
    layernorm_batch(xb_ref, x, wn, 0l, int64(d), int64(d), int64(rows), ROWS_EPS)
    check_rows(t, "the layernorm ({shape}) vs layernorm_batch", xb_dev, xb_ref, BAR_APPROX, d)
    var inscope x2_ref <- [for (i in range(nd)); x[i] + y[i] + wn[2 * d + i % d]]
    check_rows(t, "the post-add ({shape}) x += y + b vs the CPU form", x2, x2_ref, BAR_APPROX, d)
    moved_in_place(t, "the post-add ({shape})", x2, x2_ref, x)
    var inscope xb2_ref <- plane_of(nd, 0.0)
    layernorm_batch(xb2_ref, x2_ref, wn, int64(3 * d), int64(4 * d), int64(d), int64(rows), ROWS_EPS)
    check_rows(t, "the post-add's next layernorm ({shape}) vs layernorm_batch over the updated rows", xb2_dev, xb2_ref, BAR_APPROX, d)
    var inscope h_ref := hrows
    add_bias_rows(h_ref, wn, int64(5 * d), int64(ff), int64(rows))
    gelu_tanh_lut_batch(h_ref, int64(ff), int64(rows))
    check_rows(t, "the bias + tanh GELU ({shape}) vs add_bias_rows + gelu_tanh_lut_batch", h_dev, h_ref, BAR_APPROX, ff)
    var inscope yc_ref <- wdec_comb_ref(pms, py, rows, heads, hs, nch)
    check_rows(t, "the attention combine ({shape}) vs the CPU combine over the chunk partials", yc_dev, yc_ref, BAR_APPROX, d, hs)
    pair_vs_cpu(t, "{names[0]} ({shape})", qs, ss, 0, xb_ref, d, rows)
    pair_vs_cpu(t, "{names[1]} ({shape})", qs, ss, 1, xb2_ref, d, rows)
    pair_vs_cpu(t, "{names[2]} ({shape})", qs, ss, FUSED_BIAS_PAIR, h_ref, ff, rows)
    pair_vs_cpu(t, "{names[3]} ({shape})", qs, ss, 3, yc_ref, d, rows)
    x[7]++
    y[7 + d]++
    hrows[3]++
    py[5]++
    cp |> plane_upload(b.x, x)
    cp |> plane_upload(b.x3, x)
    cp |> plane_upload(b.y, y)
    cp |> plane_upload(b.h2, hrows)
    cp |> plane_upload(b.py, py)
    for (i in range(2 * FUSED_PAIRS)) {
        if (i % 2 == 1) {
            cp |> plane_refill(b.q[i])
            cp |> plane_refill(b.s[i])
        }
    }
    cp |> cell_launch(t, "the fused row passes over the poisoned inputs ({shape})") $(var rl) {
        fused_rq_encode(rl, b, d, rows, nch)
    }
    for (i in range(2 * FUSED_PAIRS)) {
        if (i % 2 == 1) {
            let n = pair_elems(i, nd, nff)
            var inscope qr <- cp |> plane_as(b.q[i], type<int8>, n)
            var inscope sr <- cp |> plane_run(b.s[i])
            var inscope moved <- q8_values(qr, sr)
            var inscope clean <- q8_values(qs[i], ss[i])
            check_exceeds(t, "{names[i / 2]} ({shape}): the poisoned input moves the fused stamp's values", moved, clean, BAR_EXACT)
        }
    }
    delete qs
    delete ss
}

//! the layernorm, the post-add, the bias-activation and the attention combine fused with the Q8_0 requant, at the other cells'
//! width over two chunks, whisper tiny's (the strided loops' partial tail) and turbo's (the block pass's second trip)
def wdec_fused_rq_case(t : T?; var cp : auto(C)) {
    wdec_fused_rq_shape(t, cp, ROWS_D, ROWS_N, 2)
    wdec_fused_rq_shape(t, cp, 384, ROWS_N, 1)
    wdec_fused_rq_shape(t, cp, 1280, ROWS_N, 1)
}

//! the clamped Q8_0 feed against the CPU clamp and requant; the unclamped requant the control
def clamp_rq_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tower_clamp_rq"
    let d = ROWS_D
    let rows = ROWS_N
    let nd = rows * d
    let q8e = int(kq_elems(KqFmt.q8))
    let lo = -0.75
    let hi = 0.9
    let sl = q8_slack(nd)
    var inscope x <- row_fill(nd, 41, 7, 0.05, 2.1)
    let xp = cp |> in_(x)
    let qp = cp |> out_words(int64(nd + 4 * sl.words), I8_SENTINEL_BITS)
    let sp = cp |> out_(nd / q8e, 0, sl.blocks)
    var pa = RqArgs(inbase = 0u, nblk = uint(nd / q8e), lo = lo, hi = hi)
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tower_clamp_rq_cls(rl, seats(fixed_array(xp, qp, sp)), pa, int64(round_up(nd / 4, WG) / WG))
    }
    var inscope xq_dev <- cp |> plane_as(qp, type<int8>, nd)
    var inscope xs_dev <- cp |> plane_run(sp)
    var inscope clamped <- plane_of(nd, 0.0)
    clamp_rows(clamped, x, int64(nd), lo, hi)
    var inscope want <- q8_rows_ref(clamped, d, rows)
    check_q8(t, "{tag}: the Q8_0 feed is the clamped rows requantized as the CPU does (one quant step, the scales at the approx bar)", xq_dev, xs_dev, want.q, want.s, APPROX_REL, 1)
    var inscope raw <- q8_rows_ref(x, d, rows)
    t |> success(mismatch_q8(xq_dev, xs_dev, raw.q, raw.s, APPROX_REL, 1) > 0, "{tag} control: the unclamped rows' requant misses the feed")
    slack_kept(t, "{tag}: the quant words past the run", cp, qp, type<uint>, nd / 4, sl.words, I8_SENTINEL_BITS)
    slack_kept(t, "{tag}: the scales past the run", cp, sp, type<uint>, nd / q8e, sl.blocks, NAN_SENTINEL_BITS)
}

//! the silero VAD's two stamps against `_vad_oracle.das` at the approx bar: the encoder over three chunks, the recurrence at
//! one chunk and five from a nonzero state in place
def vad_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let nch = 3
    var inscope fx <- vad_fixture(nch)
    var inscope x <- vad_signal(nch)
    var inscope gx_ref <- vad_enc_oracle(x, fx.slab, fx.pa)
    let xp = cp |> in_(x)
    let wp = cp |> in_(fx.slab)
    let gp = cp |> out_(nch * 512)
    var pa = fx.pa
    cp |> cell_launch(t, "tower_vad_enc") $(var rl) {
        enc_tower_vad_enc_cls(rl, seats(fixed_array(xp, wp, gp)), pa, int64(nch))
    }
    var inscope gx_dev <- cp |> plane_run(gp)
    check_rows(t, "tower_vad_enc three chunks' input gates vs the CPU reference", gx_dev, gx_ref, BAR_APPROX, 512)
    for (steps in [1, 5]) {
        cp |> cell_clear()
        var inscope fl <- vad_fixture(steps)
        var inscope xl <- vad_signal(steps)
        var inscope gx <- vad_enc_oracle(xl, fl.slab, fl.pa)
        var inscope st0 <- [for (i in range(256)); 0.3 * sin(float(i) * 0.7)]
        var inscope r <- vad_lstm_oracle(gx, fl.slab, st0, fl.pa)
        t |> success(r.max_gate < 6.0, "tower_vad_lstm/{steps}: the fixture's gates sit off saturation (largest |gate| {r.max_gate})")
        let glp = cp |> in_(gx)
        let wlp = cp |> in_(fl.slab)
        let stp = cp |> io_(st0)
        let pp = cp |> out_(steps)
        var pl = fl.pa
        cp |> cell_launch(t, "tower_vad_lstm/{steps}") $(var rl) {
            enc_tower_vad_lstm_cls(rl, seats(fixed_array(glp, wlp, stp, pp)), pl, 1l)
        }
        var inscope p_dev <- cp |> plane_run(pp)
        var inscope st_dev <- cp |> plane_run(stp)
        check_rows(t, "tower_vad_lstm/{steps} probabilities vs the CPU reference", p_dev, r.probs, BAR_APPROX, 1)
        check_rows(t, "tower_vad_lstm/{steps} state left vs the CPU reference", st_dev, r.st, BAR_APPROX, 128)
        t |> success(moved_off(r.st, st0, 3, BAR_APPROX), "tower_vad_lstm/{steps}: the state moved off its input")
    }
}

//! the transducer gate legs: the first layer at parity 0, the second at parity 1, a state asking no step, a done clip
def private rnnt_gates_legs(t : T?; var cp : auto(C); fx : RnntFix) {
    for (leg in [(layer = 0, parity = 0, need = 1, done = 0), (layer = 1, parity = 1, need = 1, done = 0),
                 (layer = 0, parity = 0, need = 0, done = 0), (layer = 1, parity = 0, need = 1, done = 1)]) {
        cp |> cell_clear()
        let active = leg.need != 0 && leg.done == 0
        let tag = "rnnt_gates {rc_tag(fx.sh)} layer {leg.layer} parity {leg.parity} need {leg.need} done {leg.done}"
        var inscope st <- rnnt_state(0, 0, leg.done, leg.need, leg.parity, 0)
        var inscope hb_in <- rnnt_gates_input(fx, leg.layer, leg.parity, active)
        let sp = cp |> in_plain(st)
        let whp = cp |> in_plain(fx.wh)
        let wfp = cp |> in_(fx.wf)
        let xp = cp |> in_(fx.xin)
        let hp = cp |> io_(hb_in)
        let cbp = cp |> io_(fx.cb)
        var ka = rnnt_gates_args(fx, leg.layer)
        cp |> cell_launch(t, tag) $(var rl) {
            enc_rnnt_gates_cls(rl, seats(fixed_array(sp, whp, wfp, xp, hp, cbp)), ka, int64(round_up(fx.sh.pd, 8) / 8))
        }
        rnnt_gates_verify(t, tag, cp |> plane_run(hp), cp |> plane_run(cbp), fx, leg.layer, leg.parity, active)
    }
}

//! the transducer projection legs: both parities, then a state asking no step
def private rnnt_proj_legs(t : T?; var cp : auto(C); fx : RnntFix) {
    for (leg in [(parity = 0, need = 1), (parity = 1, need = 1), (parity = 0, need = 0)]) {
        cp |> cell_clear()
        let tag = "rnnt_proj {rc_tag(fx.sh)} parity {leg.parity} need {leg.need}"
        var inscope st <- rnnt_state(0, 0, 0, leg.need, leg.parity, 0)
        let sp = cp |> in_plain(st)
        let whp = cp |> in_plain(fx.wh)
        let wfp = cp |> in_(fx.wf)
        let hp = cp |> in_(fx.hb)
        let pp = cp |> out_(fx.sh.jd)
        var ka = fx.pa
        cp |> cell_launch(t, tag) $(var rl) {
            enc_rnnt_proj_cls(rl, seats(fixed_array(sp, whp, wfp, hp, pp)), ka, int64(round_up(fx.sh.jd, 8) / 8))
        }
        rnnt_proj_verify(t, tag, cp |> plane_run(pp), fx, leg.parity, leg.need != 0)
    }
}

//! the transducer joint legs: eight frames off a projection biased on the device, three before the clip's end off a biased one,
//! a done clip
def private rnnt_joint_legs(t : T?; var cp : auto(C); fx : RnntFix) {
    for (leg in [(t0 = 0, eb_on = 1, done = 0), (t0 = RC_FRAMES - 3, eb_on = 0, done = 0), (t0 = 2, eb_on = 1, done = 1)]) {
        cp |> cell_clear()
        let tag = "rnnt_joint {rc_tag(fx.sh)} t0 {leg.t0} eb_on {leg.eb_on} done {leg.done}"
        var inscope st <- rnnt_state(leg.t0, 0, leg.done, 1, 0, 0)
        let sp = cp |> in_plain(st)
        let ep = cp |> in_(fx.encp)
        let wfp = cp |> in_(fx.wf)
        let pp = cp |> in_(fx.pred)
        let jwp = cp |> in_plain(fx.jw)
        let lp = cp |> out_(8 * fx.sh.lstride)
        var ka = fx.pa
        ka.eb_on = uint(leg.eb_on)
        cp |> cell_launch(t, tag) $(var rl) {
            enc_rnnt_joint_cls(rl, seats(fixed_array(sp, ep, wfp, pp, jwp, lp)), ka, int64(fx.sh.nblk * 4))
        }
        rnnt_joint_verify(t, tag, cp |> plane_run(lp), fx, leg.t0, leg.eb_on != 0, leg.done == 0)
    }
}

//! the transducer pick: every case of `rnnt_pick_cases`, the state words written in place
def private rnnt_pick_legs(t : T?; var cp : auto(C); fx : RnntFix) {
    var inscope cases <- rnnt_pick_cases(fx)
    for (c in cases) {
        cp |> cell_clear()
        let tag = "rnnt_pick {rc_tag(fx.sh)} {c.tag}"
        var inscope want <- rnnt_pick_ref(c.lg, c.st0, fx, c.pa)
        let ntoks = int(c.pa.cap) * 8
        let sp = cp |> out_words(int64(GK_RNNT_ST_WORDS) * 4l, 0u)
        cp |> plane_upload(sp, c.st0)
        let lp = cp |> in_(c.lg)
        let wfp = cp |> in_(fx.wf)
        let xp = cp |> out_(fx.sh.pd)
        let tp = cp |> out_words(int64(ntoks) * 4l, NAN_SENTINEL_BITS)
        var ka = c.pa
        cp |> cell_launch(t, tag) $(var rl) {
            enc_rnnt_pick_cls(rl, seats(fixed_array(sp, lp, wfp, xp, tp)), ka, 1l)
        }
        var inscope stg <- cp |> plane_as(sp, type<uint>, int(GK_RNNT_ST_WORDS))
        var inscope toks <- cp |> plane_as(tp, type<uint>, ntoks)
        rnnt_pick_verify(t, tag, c, stg, toks, cp |> plane_run(xp), want)
    }
}

//! the transducer decode's four stamps on the Metal cells' legs at both `rc_shapes`, against `_rnnt_oracle.das`
def rnnt_case(t : T?; var cp : auto(C)) {
    for (sh in rc_shapes()) {
        var inscope fx <- rnnt_fixture(sh)
        rnnt_gates_legs(t, cp, fx)
        rnnt_proj_legs(t, cp, fx)
        rnnt_joint_legs(t, cp, fx)
        rnnt_pick_legs(t, cp, fx)
    }
}

//! the CTC head's pick on `ctc_pick_cases` (three widths, a tie every third row picking the lower class, NaN pad slots, an
//! all-NaN row picking class 0): every record written, each class exact, each plog within 1e-5 relative; a poisoned bias
//! element moving the plogs off the reference the control
def ctc_pick_case(t : T?; var cp : auto(C)) {
    for (c in ctc_pick_cases()) {
        cp |> cell_clear()
        let nc = c.x
        let nan_row = c.w != 0
        let tag = "ctc_pick nc {nc} rows {c.y} ldl {c.z}{nan_row ? " NaN row" : ""}"
        var inscope fx <- ctc_pick_fix(nc, c.y, c.z, nan_row)
        let want = ctc_pick_ref(fx)
        let r0 = nan_row ? 1 : 0
        let bar = Bar(rel = 1e-5, abs_floor = 1e-6, poison_scaled = true)
        let lp = cp |> in_(fx.lg)
        let wp = cp |> in_(fx.wb)
        let pp = cp |> out_words(int64(fx.rows) * 8l, NAN_SENTINEL_BITS)
        var ka = GkCtcPickArgs(nc = uint(fx.nc), ldl = uint(fx.ldl), boff = uint(fx.boff))
        for (leg in range(2)) {
            if (leg == 1) {
                fx.wb[fx.boff + 3] += 40.0
                cp |> plane_upload(wp, fx.wb)
                cp |> plane_refill(pp)
            }
            cp |> cell_launch(t, "{tag} leg {leg}") $(var rl) {
                enc_ctc_pick_cls(rl, seats(fixed_array(lp, wp, pp)), ka, int64(fx.rows))
            }
            var inscope got <- cp |> plane_as(pp, type<uint>, fx.rows * 2)
            var inscope toks <- [for (r in range(r0, fx.rows)); got[r * 2]]
            var inscope wtoks <- [for (r in range(r0, fx.rows)); want.tok[r]]
            var inscope plogs <- [for (r in range(r0, fx.rows)); uint_bits_to_float(got[r * 2 + 1])]
            var inscope wplogs <- [for (r in range(r0, fx.rows)); want.plog[r]]
            if (leg == 0) {
                var inscope picks <- [for (r in range(fx.rows)); got[r * 2]]
                check_written(t, "{tag}: every row's record written", picks, NAN_SENTINEL_BITS)
                check_exact(t, "{tag}: each row's class", toks, wtoks)
                check(t, "{tag}: each row's plog", plogs, wplogs, bar, FLAT_SHAPE)
                if (nan_row) {
                    t |> equal(got[0], 0u, "{tag}: the NaN row picks class 0")
                }
            } else {
                to_log(LOG_INFO, "{tag}: the poisoned bias moves {mismatch_exact(toks, wtoks)} picks\n")
                check_exceeds(t, "{tag}: the control - a poisoned bias element moves the plogs off the reference", plogs, wplogs, bar)
            }
        }
    }
}

//! one f16 s-column shape: `n` the K, `d` the weight rows, `rows` the positions, the weight rows at element `woff` behind a lead
//! of poisoned halves, the output rows `ystride` apart
struct private F16sShape {
    tag : string
    n : int
    d : int
    rows : int
    woff : int
    ystride : int
}

//! the f16 GEMM's s column stamp against the f32 dot of the same halves - the m column's cell's reference - and bit for bit the
//! m column stamp's output on the same planes, the gaps between the rows and the tile grid's footprint past them kept; a
//! poisoned weight half the control
def f16_gemm_s_case(t : T?; var cp : auto(C)) {
    let lead = 256   // the halves ahead of the weight rows' base: a stamp reading from 0 instead of `woff` reads them
    let shapes <- [
        F16sShape(tag = "one s column, one k step", n = 64, d = 128, rows = 32, woff = 0, ystride = 128),
        F16sShape(tag = "rows off the 32 column", n = 512, d = 200, rows = 37, woff = lead, ystride = 200),
        F16sShape(tag = "under one column, a k tail", n = 96, d = 61, rows = 13, woff = lead, ystride = 61),
        F16sShape(tag = "two whole columns at a wider row stride", n = 384, d = 256, rows = 64, woff = lead, ystride = 272)]
    for (sh in shapes) {
        cp |> cell_clear()
        let tag = "f16 GEMM s column, {sh.tag} ({sh.rows} x {sh.d} over K {sh.n})"
        var inscope wh <- [for (i in range(sh.woff + sh.d * sh.n)); i < sh.woff ? float16(1000.0) : float16(float(((i - sh.woff) * 7) % 61) * 0.02 - 0.6)]
        var inscope xh <- [for (i in range(sh.rows * sh.n)); float16(float((i * 11) % 53) * 0.03 - 0.8)]
        var inscope ref <- plane_of(sh.rows * sh.d, 0.0)
        for (p in range(sh.rows)) {
            for (e in range(sh.d)) {
                var acc = 0.0
                for (kk in range(sh.n)) {
                    acc += float(xh[p * sh.n + kk]) * float(wh[sh.woff + e * sh.n + kk])
                }
                ref[p * sh.d + e] = acc
            }
        }
        let bar = Bar(kind = BarKind.rel_of_max, rel = 2e-5, poison = 8e-5, poison_of_max = true)   // f16-exact operands: every product exact in f32, the stamps differing from the dot by f32 reassociation over K alone
        let ylen = (sh.rows - 1) * sh.ystride + sh.d
        let footprint = (round_up(sh.rows, 32) - 1) * sh.ystride + round_up(sh.d, 128)
        let wp = cp |> in_plain(wh)
        let xp = cp |> in_plain(xh)
        let ysp = cp |> out_(ylen, 0, footprint - ylen)
        let ymp = cp |> out_(ylen, 0, footprint - ylen)
        var pc = F16GemmArgs(n = uint(sh.n), d = uint(sh.d), rows = uint(sh.rows), woff = uint(sh.woff), ybase = 0u, ystride = uint(sh.ystride), ksplit = 0u)
        cp |> cell_launch(t, tag) $(var rl) {
            f16_gemm_s_launch(rl, seats(fixed_array(wp, xp, ysp)), pc, int64(sh.rows), int64(sh.d))
            f16_gemm_m_launch(rl, seats(fixed_array(wp, xp, ymp)), pc, int64(sh.rows), int64(sh.d))
        }
        var inscope ys <- cp |> plane_as(ysp, type<float>, ylen)
        var inscope got <- [for (i in range(sh.rows * sh.d)); ys[(i / sh.d) * sh.ystride + i % sh.d]]
        check_rows(t, "{tag} vs the f32 dot", got, ref, bar, sh.d)
        var inscope ys_bits <- cp |> plane_as(ysp, type<uint>, footprint)
        var inscope ym_bits <- cp |> plane_as(ymp, type<uint>, footprint)
        check_exact(t, "{tag}: bit for bit the m column stamp's plane", ys_bits, ym_bits)
        var inscope gaps <- [for (i in range(footprint)); i >= ylen || i % sh.ystride >= sh.d ? ys_bits[i] : NAN_SENTINEL_BITS]
        var inscope sentinel <- [for (_i in range(footprint)); NAN_SENTINEL_BITS]
        check_exact(t, "{tag}: the gaps between the rows and the footprint past them keep the sentinel", gaps, sentinel)
        wh[sh.woff + 5 * sh.n + 3] = float16(9.0)
        cp |> plane_upload(wp, wh)
        cp |> plane_refill(ysp)
        cp |> cell_launch(t, "{tag}, a poisoned weight half") $(var rl) {
            f16_gemm_s_launch(rl, seats(fixed_array(wp, xp, ysp)), pc, int64(sh.rows), int64(sh.d))
        }
        var inscope ys2 <- cp |> plane_as(ysp, type<float>, ylen)
        var inscope poisoned <- [for (i in range(sh.rows * sh.d)); ys2[(i / sh.d) * sh.ystride + i % sh.d]]
        check_exceeds(t, "{tag}: the control - a poisoned weight half moves the rows", poisoned, ref, bar)
    }
}
