options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_audio shared public
options _function_length = 0   // per-family transcribe pipelines mirror the model schema

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_env  // the g_env_* knob globals ([EnvConfig] declarations; ENVIRONMENT.md is generated from them)
require strings
require daslib/fio
require daslib/apply
require daslib/archive
require dasllama/dasllama_math
require dasllama/dasllama_tower   // the family-neutral encoder-tower pieces this tower composes
require dasllama/dasllama_plane public   // PlaneF/PlaneI8 — plane forms of the blob helpers; re-exported for deleters
require dasllama/dasllama_par
require dasllama/dasllama_gguf
require dasllama/dasllama_convert
require dasllama/dasllama_common   // apply_box_profile_runtime for the tower-image identity
require dasllama/dasllama_image

// Whisper-family audio frontend: 16 kHz mono PCM -> log-mel chunks, in two oracle-exact
// flavors: log_mel_chunks mirrors llama.cpp mtmd (tools/mtmd/mtmd-audio.cpp, qwen2a tower),
// log_mel_whisper mirrors whisper.cpp's log_mel_spectrogram (different padding/floor/fill
// rules — see whisper_plan.md). The 400-point DFT is two GEMM columns of a precomputed
// twiddle matrix (minfft is pow2-only, and 400 = 2^4*25 is not) — see qwen2_audio_plan.md.

let AUDIO_SAMPLE_RATE = 16000l
let AUDIO_N_FFT = 400l
let AUDIO_HOP = 160l
let AUDIO_CHUNK_FRAMES = 3000l  // 30 s of hops per encoder chunk

//! One 30 s log-mel chunk, mel-major ``[n_mel][n_len]`` (mtmd layout, conv-ready).
struct MelChunk {
    n_mel : int64
    n_len : int64
    data : array<float>
}

//! Hann window of length ``n`` (periodic when ``periodic``), float-exact vs mtmd's
//! ``fill_hann_window``: 0.5·(1 − cos(2πi/denom)) with denom = n (periodic) / n−1.
def build_hann_window(n : int64; periodic : bool = true) : array<float> {
    let denom = double(periodic ? n : n - 1l)
    return <- [for (i in range64(n)); float(0.5lf * (1.0lf - double(cos(float(2.0lf * DBL_PI * double(i) / denom)))))]
}

//! Slaney-scale triangular mel filterbank ``[n_mel][n_fft/2+1]`` (librosa default, area-
//! normalized) — a direct port of mtmd's ``fill_mel_filterbank_matrix`` with fmin=0,
//! fmax=sample_rate/2. Double internally, float out.
def build_mel_filterbank(n_mel, n_fft, sample_rate : int64) : array<float> {
    let n_bins = n_fft / 2l + 1l
    // slaney hz<->mel: linear below 1 kHz, log above
    let min_log_hz = 1000.0lf
    let lin_slope = 3.0lf / 200.0lf
    let min_log_mel = min_log_hz * lin_slope
    let log_step = log(6.4lf) / 27.0lf
    let fmax = 0.5lf * double(sample_rate)
    let m_hi = fmax < min_log_hz ? fmax * lin_slope : min_log_mel + log(fmax / min_log_hz) / log_step
    // mel grid edges back to hz (m_lo = hz_to_mel(0) = 0)
    var hz_pts : array<double>
    hz_pts |> resize(n_mel + 2l)
    for (i in range64(n_mel + 2l)) {
        let m = m_hi * double(i) / double(n_mel + 1l)
        hz_pts[i] = m < min_log_mel ? m / lin_slope : min_log_hz * exp((m - min_log_mel) * log_step)
    }
    let bin_hz_step = double(sample_rate) / double(n_fft)
    var out : array<float>
    out |> resize(n_mel * n_bins)
    for (m in range64(n_mel)) {
        let f_left = hz_pts[m]
        let f_center = hz_pts[m + 1l]
        let f_right = hz_pts[m + 2l]
        let denom_l = max(1e-30lf, f_center - f_left)
        let denom_r = max(1e-30lf, f_right - f_center)
        let enorm = 2.0lf / max(1e-30lf, f_right - f_left)  // slaney area norm
        for (k in range64(n_bins)) {
            let f = double(k) * bin_hz_step
            var w = 0.0lf
            if (f >= f_left && f <= f_center) {
                w = (f - f_left) / denom_l
            } elif (f > f_center && f <= f_right) {
                w = (f_right - f) / denom_r
            }
            out[m * n_bins + k] = float(w * enorm)
        }
    }
    return <- out
}

//! DFT twiddle matrix [n_fft][2·n_bins]: column 2k = cos(2πjk/n), column 2k+1 = −sin(2πjk/n).
//! XW[frames×n_fft] · B -> interleaved re/im rows, same table values as mtmd's sin/cos cache
//! (double theta, float trig). Shared by every mel flavor (whisper/mtmd here, qwen3a's module).
def build_dft_twiddles(n_fft : int64) : array<float> {
    let n_bins = n_fft / 2l + 1l
    var cos_tab : array<float>
    var sin_tab : array<float>
    cos_tab |> resize(n_fft)
    sin_tab |> resize(n_fft)
    for (i in range64(n_fft)) {
        let theta = 2.0lf * DBL_PI * double(i) / double(n_fft)
        cos_tab[i] = cos(float(theta))
        sin_tab[i] = sin(float(theta))
    }
    var b : array<float>
    b |> resize(n_fft * n_bins * 2l)
    for (j in range64(n_fft)) {
        for (k in range64(n_bins)) {
            let idx = (j * k) % n_fft
            b[j * n_bins * 2l + k * 2l] = cos_tab[idx]
            b[j * n_bins * 2l + k * 2l + 1l] = -sin_tab[idx]
        }
    }
    return <- b
}

//! Iterative radix-2 FFT plan for a power-of-2 size: bit-reversal permutation + twiddle
//! table tw[k] = (cos, −sin)(2πk/n) interleaved, k < n/2 — same forward (e^{−2πi/n})
//! convention as build_dft_twiddles, so power spectra agree to fp32 rounding.
struct FftPlan {
    n : int64 = 0l
    rev : array<int>
    tw : array<float>
}

def build_fft_plan(n : int64) : FftPlan {
    if (n < 2l || (n & (n - 1l)) != 0l) {
        panic("build_fft_plan: {n} is not a power of 2")
    }
    var p = FftPlan(n = n)
    var bits = 0l
    while ((1l << bits) < n) {
        bits++
    }
    p.rev |> resize(n)
    for (i in range64(n)) {
        var r = 0l
        var v = i
        for (_b in range64(bits)) {
            r = (r << 1l) | (v & 1l)
            v >>= 1l
        }
        p.rev[i] = int(r)
    }
    p.tw |> resize(n)   // n/2 interleaved (cos, −sin) pairs
    for (k in range64(n / 2l)) {
        let theta = 2.0lf * DBL_PI * double(k) / double(n)
        p.tw[2l * k] = cos(float(theta))
        p.tw[2l * k + 1l] = -sin(float(theta))
    }
    return <- p
}

//! In-place radix-2 complex FFT over n interleaved (re, im) pairs at ``reim`` — pointer
//! core so parallel workers can run it on stack buffers. ``rev``/``tw`` from an FftPlan
//! of the same n.
def fft_pow2_run(n : int64; rev : int const?; tw : float const?; var reim : float?) {
    unsafe {
        for (i in range64(n)) {   // bit-reversal permutation (swap once per pair)
            let r = int64(rev[i])
            if (i < r) {
                let a = i * 2l
                let b = r * 2l
                let tre = reim[a]
                let tim = reim[a + 1l]
                reim[a] = reim[b]
                reim[a + 1l] = reim[b + 1l]
                reim[b] = tre
                reim[b + 1l] = tim
            }
        }
        var len = 2l
        while (len <= n) {
            let half = len >> 1l
            let step = n / len
            var base = 0l
            while (base < n) {
                for (j in range64(half)) {
                    let w = j * step * 2l
                    let wre = tw[w]
                    let wim = tw[w + 1l]
                    let a = (base + j) * 2l
                    let b = (base + j + half) * 2l
                    let bre = reim[b]
                    let bim = reim[b + 1l]
                    let tre = bre * wre - bim * wim
                    let tim = bre * wim + bim * wre
                    reim[b] = reim[a] - tre
                    reim[b + 1l] = reim[a + 1l] - tim
                    reim[a] += tre
                    reim[a + 1l] += tim
                }
                base += len
            }
            len <<= 1l
        }
    }
}

//! Whisper log-mel spectrogram over the full padded input, split into full 3000-frame chunks —
//! the mtmd whisper preprocessor end-to-end (pad to ≥31 s, reflect-200 head, Hann/hop-160 power
//! spectra, slaney mel, log10, global max−8 clamp). Audio ≤30 s yields exactly one chunk.
def log_mel_chunks(samples : array<float> | #; n_mel : int64) : array<MelChunk> {
    let n_in = length(samples)
    // mtmd zero-extends short input to chunk+1 seconds BEFORE the stage pads — the head
    // reflect below must read from the extended buffer, not the raw input
    let min_samples = int(AUDIO_SAMPLE_RATE * 31l)
    let n_base = max(n_in, min_samples)
    let head = int(AUDIO_N_FFT / 2l)                       // reflect pad
    let tail = int(AUDIO_SAMPLE_RATE * 30l) + head         // 30 s zeros + 200
    let n_padded = head + n_base + tail
    var padded : array<float>
    padded |> reserve(n_padded)
    padded |> resize(n_padded)                             // resize zero-fills
    for (i in range(n_in)) {
        padded[head + i] = samples[i]
    }
    for (i in range(head)) {                               // reflect: padded[199-i] = base[1+i]
        padded[head - 1 - i] = padded[head + 1 + i]
    }

    let n_fft = AUDIO_N_FFT
    let n_bins = n_fft / 2l + 1l
    let n_len = (int64(n_padded) - n_fft) / AUDIO_HOP + 1l
    let hann <- build_hann_window(n_fft)
    let twiddles <- build_dft_twiddles(n_fft)
    let filters <- build_mel_filterbank(n_mel, n_fft, AUDIO_SAMPLE_RATE)

    // windowed frames [n_len × n_fft]
    var xw : array<float>
    xw |> reserve_resize(int(n_len * n_fft))
    for (i in range64(n_len)) {
        let off = i * AUDIO_HOP
        for (j in range64(n_fft)) {
            xw[i * n_fft + j] = hann[j] * padded[off + j]
        }
    }

    // DFT as one GEMM: [n_len × n_fft] · [n_fft × 2·n_bins] -> interleaved re/im
    var reim : array<float>
    reim |> reserve(int(n_len * n_bins * 2l))
    reim |> resize(int(n_len * n_bins * 2l))               // gemm accumulates: starts zeroed
    gemm_f32(unsafe(addr(reim[0])), unsafe(addr(xw[0])), unsafe(addr(twiddles[0])),
             n_len, n_fft, n_bins * 2l)

    // power spectra -> mel (float products, double sum — mtmd's exact accumulation), log10
    let mel_floor = 5.960464477539063e-08lf                // 2^-24, mtmd filter_params default
    let log10_e = 0.43429448190325176lf
    var mel : array<float>
    mel |> reserve(int(n_mel * n_len))
    mel |> resize(int(n_mel * n_len))                      // mel-major [n_mel][n_len]
    var power : array<float>
    power |> resize(int(n_bins))
    for (i in range64(n_len)) {
        for (k in range64(n_bins)) {
            let re = reim[i * n_bins * 2l + k * 2l]
            let im = reim[i * n_bins * 2l + k * 2l + 1l]
            power[k] = re * re + im * im
        }
        for (m in range64(n_mel)) {
            var sum = 0.0lf
            for (k in range64(n_bins)) {
                sum += double(power[k] * filters[m * n_bins + k])
            }
            mel[m * n_len + i] = float(log(max(sum, mel_floor)) * log10_e)
        }
    }

    // whisper-style dynamic clamp + scale, GLOBAL over the padded spectrogram; mtmd's exact
    // rounding — the clamp value round-trips through float before the (x+4)/4 double math
    var mmax = -1e20lf
    for (v in mel) {
        mmax = max(mmax, double(v))
    }
    mmax -= 8.0lf
    for (v in mel) {
        let clamped = double(v) < mmax ? float(mmax) : v
        v = float((double(clamped) + 4.0lf) / 4.0lf)
    }

    // full 3000-frame chunks only; the padded tail chunk is dropped (mtmd: "safe to ignore")
    var chunks : array<MelChunk>
    var off = 0l
    while (off + AUDIO_CHUNK_FRAMES <= n_len) {
        var c = MelChunk(n_mel = n_mel, n_len = AUDIO_CHUNK_FRAMES)
        c.data |> resize(int(n_mel * AUDIO_CHUNK_FRAMES))
        for (m in range64(n_mel)) {
            for (i in range64(AUDIO_CHUNK_FRAMES)) {
                c.data[m * AUDIO_CHUNK_FRAMES + i] = mel[m * n_len + off + i]
            }
        }
        chunks |> emplace(c)
        off += AUDIO_CHUNK_FRAMES
    }
    return <- chunks
}

//! Full-length whisper.cpp-style log-mel spectrogram: total frame count (audio + the 30 s
//! zero tail), the real-audio frame count (``n_len_org`` = the driver's seek_end), and
//! mel-major ``[n_mel][n_len]`` data.
struct WhisperMel {
    n_mel : int64
    n_len : int64
    n_len_org : int64
    @scratch data : array<float>   // reused across windows; mel_sized grows it exact
}

// per-frame stack buffers in log_mel_whisper are sized for the fixed 400-pt whisper DFT
let private MEL_DFT_N = 400
let private MEL_DFT_BINS = 201

//! Hoisted whisper-mel tables (built once at model load): Hann-400 window + the DFT twiddle
//! matrix, TRANSPOSED to ``[2·n_bins][n_fft]`` so each spectrum bin is two contiguous ``dot``
//! rows (log_mel_whisper's per-frame matvec form).
struct WhisperMelPlan {
    n_fft : int64 = 0l
    hann : array<float>
    twt : array<float>   // [2·n_bins][n_fft]: row 2k = cos(2πjk/n), row 2k+1 = −sin(2πjk/n)
}

def build_whisper_mel_plan(n_fft : int64) : WhisperMelPlan {
    if (n_fft != int64(MEL_DFT_N)) {
        panic("build_whisper_mel_plan: log_mel_whisper stack frames are sized for {MEL_DFT_N}-pt DFT, got {n_fft}")
    }
    var p = WhisperMelPlan(n_fft = n_fft)
    p.hann <- build_hann_window(n_fft)
    let n_bins = n_fft / 2l + 1l
    var cos_tab : array<float>
    var sin_tab : array<float>
    cos_tab |> resize(n_fft)
    sin_tab |> resize(n_fft)
    for (i in range64(n_fft)) {
        let theta = 2.0lf * DBL_PI * double(i) / double(n_fft)
        cos_tab[i] = cos(float(theta))
        sin_tab[i] = sin(float(theta))
    }
    p.twt |> resize(2l * n_bins * n_fft)
    for (k in range64(n_bins)) {
        for (j in range64(n_fft)) {
            let idx = (j * k) % n_fft
            p.twt[(2l * k) * n_fft + j] = cos_tab[idx]
            p.twt[(2l * k + 1l) * n_fft + j] = -sin_tab[idx]
        }
    }
    delete cos_tab
    delete sin_tab
    return <- p
}

// exact-length grow for reused mel scratch: reserve once (exact), resize every call —
// length stays exact so `for (v in arr)` loops and array-bound reads stay valid on reuse
def private mel_sized(@scratch var a : array<float>; n : int64) {
    if (long_capacity(a) < n) {
        a |> reserve(n)
    }
    a |> resize(n)
}

//! whisper.cpp's ``log_mel_spectrogram`` — diverges from the mtmd flavor above: reflect-200 pad
//! from the RAW input (no 31 s pre-extend), drops the last frame, past-audio frames skip the FFT
//! (floor 1e-10). Returning form; the ``var padded``/``var m`` overload reuses session scratch.
def log_mel_whisper(samples : array<float> | #; plan : WhisperMelPlan; filters : array<float>; n_mel : int64) : WhisperMel {
    var padded : array<float>
    var m = WhisperMel(n_mel = n_mel)
    log_mel_whisper(samples, plan, filters, n_mel, padded, m)
    delete padded
    return <- m
}

//! Borrowed-plane form — the filterbank lives in a prepared image.
def log_mel_whisper(samples : array<float> | #; plan : WhisperMelPlan; filters : PlaneF; n_mel : int64;
                    var padded : array<float>; var m : WhisperMel) {
    log_mel_whisper_(samples, plan, plane_at(filters, 0l), n_mel, padded, m)
}

def log_mel_whisper(samples : array<float> | #; plan : WhisperMelPlan; filters : array<float>; n_mel : int64;
                    var padded : array<float>; var m : WhisperMel) {
    log_mel_whisper_(samples, plan, unsafe(addr<float const?>(filters[0])), n_mel, padded, m)
}

[hot_path]
def private log_mel_whisper_(samples : array<float> | #; plan : WhisperMelPlan; filters : float const?; n_mel : int64;
                             var padded : array<float>; var m : WhisperMel) {
    let n_in = long_length(samples)
    let head = AUDIO_N_FFT / 2l                            // 200
    let n_padded = n_in + AUDIO_SAMPLE_RATE * 30l + 2l * head
    mel_sized(padded, n_padded)
    let nref = clamp(n_in - 1l, 0l, head)                  // whisper.cpp reads samples[1..200] unguarded
    for (i in range64(head - nref)) {                      // scratch is reused, not fresh: zero every region
        padded[i] = 0.0
    }
    for (i in range64(nref)) {
        padded[head - 1l - i] = samples[i + 1l]
    }
    for (i in range(int(n_in))) {
        padded[int(head) + i] = samples[i]
    }
    for (i in range64(head + n_in, n_padded)) {            // 30 s zero tail (+200)
        padded[i] = 0.0
    }

    m.n_mel = n_mel
    m.n_len = (n_padded - AUDIO_N_FFT) / AUDIO_HOP         // "remove the last frame"
    m.n_len_org = 1l + (n_in + head - AUDIO_N_FFT) / AUDIO_HOP
    mel_sized(m.data, n_mel * m.n_len)

    // frames whose window starts inside the real (head-padded) audio get the DFT; the rest
    // are pure zero-tail and fill with log10(1e-10) directly
    let n_fft = AUDIO_N_FFT
    let n_bins = n_fft / 2l + 1l
    let n_compute = min((n_in + head) / AUDIO_HOP + 1l, m.n_len)
    if (plan.n_fft != n_fft) {
        panic("log_mel_whisper: plan is {plan.n_fft}-pt, want {n_fft} - build_whisper_mel_plan(AUDIO_N_FFT)")
    }
    let log10_e = 0.43429448190325176lf
    let fill = float(log(1e-10lf) * log10_e)

    // Per-frame window+DFT matvec + mel dot, threaded over frames (bit-exact at any lane count);
    // stack-resident scratch avoids the ~170 MB [frames×n_fft]+[frames×2·n_bins] images the GEMM form needed.
    let n_len = m.n_len
    unsafe {
        let pdd = addr<float const?>(padded[0])
        let hw = addr<float const?>(plan.hann[0])
        let twt = addr<float const?>(plan.twt[0])
        let flt = filters
        var md = addr(m.data[0])
        maybe_parallel_for(0, int(n_compute), lanes_for_work(n_compute * n_fft * n_bins * 2l, 0)) $(rb, re) {
            var xwin : float[MEL_DFT_N]    // windowed samples, stack-resident
            var pw : float[MEL_DFT_BINS]   // power spectrum
            unsafe {
                var xp = addr(xwin[0])
                var pp = addr(pw[0])
                for (i in range64(int64(rb), int64(re))) {
                    let off = i * AUDIO_HOP
                    for (j in range64(n_fft)) {
                        xp[j] = hw[j] * pdd[off + j]
                    }
                    for (k in range64(n_bins)) {
                        let sre = dot(twt + (2l * k) * n_fft, xp, n_fft)
                        let sim = dot(twt + (2l * k + 1l) * n_fft, xp, n_fft)
                        pp[k] = sre * sre + sim * sim
                    }
                    for (jm in range64(n_mel)) {
                        let sum = double(dot(flt + jm * n_bins, pp, n_bins))
                        md[jm * n_len + i] = float(log(max(sum, 1e-10lf)) * log10_e)
                    }
                }
            }
        }
    }
    for (i in range64(n_compute, m.n_len)) {
        for (j in range64(n_mel)) {
            m.data[j * m.n_len + i] = fill
        }
    }

    // global max−8 clamp + (x+4)/4, double math over float storage (same as the mtmd flavor)
    var mmax = -1e20lf
    for (v in m.data) {
        mmax = max(mmax, double(v))
    }
    mmax -= 8.0lf
    for (v in m.data) {
        let clamped = double(v) < mmax ? float(mmax) : v
        v = float((double(clamped) + 4.0lf) / 4.0lf)
    }
}

//! Slice one 3000-frame encoder window starting at mel frame ``seek``; frames past
//! ``m.n_len`` zero-fill (whisper.cpp's conv-input copy). Returning form — the ``var c``
//! overload reuses session scratch (zero per-window alloc).
def whisper_mel_window(m : WhisperMel; seek : int64) : MelChunk {
    var c = MelChunk()
    whisper_mel_window(m, seek, c)
    return <- c
}

def whisper_mel_window(m : WhisperMel; seek : int64; var c : MelChunk) {
    c.n_mel = m.n_mel
    c.n_len = AUDIO_CHUNK_FRAMES
    mel_sized(c.data, m.n_mel * AUDIO_CHUNK_FRAMES)
    let i0 = min(seek, m.n_len)
    let i1 = min(seek + AUDIO_CHUNK_FRAMES, m.n_len)
    for (j in range64(m.n_mel)) {
        for (i in range64(i1 - i0)) {
            c.data[j * AUDIO_CHUNK_FRAMES + i] = m.data[j * m.n_len + i0 + i]
        }
        for (i in range64(i1 - i0, AUDIO_CHUNK_FRAMES)) {   // scratch is reused: zero the tail explicitly
            c.data[j * AUDIO_CHUNK_FRAMES + i] = 0.0
        }
    }
}

// ===== Whisper encoder tower (fp32 bring-up; q8 fast path is ledgered) =====

//! Which projector tail maps encoder positions to decoder soft tokens (mtmd family names).
enum AudioProjKind {
    qwen2a      // avg-pool -> post-LN -> linear w+b (Qwen2-Audio; Qwen2.5-Omni audio is identical)
    ultravox    // post-LN -> stack(8) -> RMS-pre -> mm1 -> swiglu_swapped -> RMS-mid -> mm2 (no biases)
    voxtral     // avg-pool -> post-LN -> stack(4) -> mm1 -> gelu_erf -> mm2 (no biases)
}

//! The audio tower: Whisper encoder weights + a model-specific projector tail, all fp32 in
//! one blob. Per-layer tensors sit at ``layers_off + l·layer_stride`` in the fixed order
//! ``enc_layer_offsets`` computes — the loader lays them out, forward never touches names.
struct AudioTower {
    n_mel : int64
    d_model : int64
    n_head : int64
    n_layer : int64
    n_ff : int64
    n_ctx : int64      // encoder positions per chunk (1500 = 3000 mel frames / conv stride 2)
    proj_dim : int64   // decoder embedding width the projector maps into (0 = no projector)
    gelu_tanh : bool = false  // whisper.cpp oracle uses the tanh-fp16-LUT gelu; mtmd uses erf
    eps : float = 1e-5
    proj_kind : AudioProjKind = AudioProjKind.qwen2a
    stack_factor : int64 = 1  // frame-concat width before the projector (ultravox 8, voxtral 4)
    mm1_out : int64 = 0l      // projector hidden width (ultravox: pre-swiglu, voxtral: pre-gelu)
    has_k_bias : bool = false // qwen3a attention carries a k bias; whisper towers don't
    fblob : PlaneF = PlaneF()
    q8 : bool = false         // encoder GEMMs quantized: qblob/qscales carry the COMPACT Q8_0 set
    qblob : PlaneI8 = PlaneI8()   // dense GEMM-only Q8_0 planes (fblob keeps just the non-GEMM rest)
    qscales : PlaneF = PlaneF()   // per-32-block scales, indexed at woff/32
    qlayers_off : int64 = 0l  // q8 layout: per-layer GEMM base/stride in the dense qblob
    qlayer_stride : int64 = 0l
    conv1_w_off : int64
    conv1_b_off : int64
    conv2_w_off : int64
    conv2_b_off : int64
    pos_off : int64
    layers_off : int64
    layer_stride : int64
    lnp_w_off : int64
    lnp_b_off : int64
    proj_w_off : int64        // qwen2a: fc weight; ultravox/voxtral: mm1 weight
    proj_b_off : int64        // qwen2a: fc bias
    normpre_off : int64       // ultravox RMS-pre weight [d·stack]
    normmid_off : int64       // ultravox RMS-mid weight [mm1_out/2]
    mm2_w_off : int64         // ultravox/voxtral second projector matrix
    image_map : void?         // non-null when the planes borrow a mapped .dlim (dasllama_image)
    image_bytes : uint64
}

// the tower owns exactly one thing — its image backing. Every plane borrows.
def finalize(var t : AudioTower) {
    if (t.image_map != null) {
        image_backing_release(t.image_map, t.image_bytes)
        t.image_map = null
        t.image_bytes = 0ul
    }
}

// the tripwire: serialize_image_meta(AudioTower) covers 28 fields; image_map + image_bytes
// are deliberate skips. A new non-array AudioTower field trips this until BOTH agree.
let private TOWER_META_FIELDS = 30 + 2

// every non-array AudioTower field, explicitly, both directions (see dasllama_image).
// Public: WhisperModel's serialize_image_meta reuses it for its nested `enc` tower.
def serialize_image_meta(var arch : Archive; var t : AudioTower) {
    verify(count_meta_fields(t) == TOWER_META_FIELDS)   // grew AudioTower? extend this list
    arch |> serialize_raw(t.n_mel)
    arch |> serialize_raw(t.d_model)
    arch |> serialize_raw(t.n_head)
    arch |> serialize_raw(t.n_layer)
    arch |> serialize_raw(t.n_ff)
    arch |> serialize_raw(t.n_ctx)
    arch |> serialize_raw(t.proj_dim)
    arch |> serialize_raw(t.gelu_tanh)
    arch |> serialize_raw(t.eps)
    arch |> serialize_raw(t.proj_kind)
    arch |> serialize_raw(t.stack_factor)
    arch |> serialize_raw(t.mm1_out)
    arch |> serialize_raw(t.has_k_bias)
    arch |> serialize_raw(t.q8)
    arch |> serialize_raw(t.conv1_w_off)
    arch |> serialize_raw(t.conv1_b_off)
    arch |> serialize_raw(t.conv2_w_off)
    arch |> serialize_raw(t.conv2_b_off)
    arch |> serialize_raw(t.pos_off)
    arch |> serialize_raw(t.layers_off)
    arch |> serialize_raw(t.layer_stride)
    arch |> serialize_raw(t.lnp_w_off)
    arch |> serialize_raw(t.lnp_b_off)
    arch |> serialize_raw(t.proj_w_off)
    arch |> serialize_raw(t.proj_b_off)
    arch |> serialize_raw(t.normpre_off)
    arch |> serialize_raw(t.normmid_off)
    arch |> serialize_raw(t.mm2_w_off)
    arch |> serialize_raw(t.qlayers_off)
    arch |> serialize_raw(t.qlayer_stride)
}

[unused_argument(t)]
def private image_post_load(var t : AudioTower) {}   // no derived state — offsets ARE the meta

//! A nested tower's plane BYTES at mint time. Embedded in another family's staging under the
//! served field name (WhisperStaging.enc, Qwen3aStaging.core), so the image walk writes the
//! dotted sections the parse walk will bind (`enc.fblob`).
struct AudioTowerPlanes {
    image_planes : bool = true   // the marker the image write walk static_ifs on
    fblob : array<float>
    qblob : array<int8>
    qscales : array<float>
}

def finalize(var pl : AudioTowerPlanes) {
    delete pl.fblob
    delete pl.qblob
    delete pl.qscales
}

//! The standalone tower's mint-side carrier: ``e`` holds every scalar the meta blob carries,
//! the arrays hold the plane bytes until the writer has them. Public for the off-rail suites.
struct AudioTowerStaging {
    e : AudioTower = AudioTower()
    fblob : array<float>
    qblob : array<int8>
    qscales : array<float>
}

def private serialize_image_meta(var arch : Archive; var st : AudioTowerStaging) {
    serialize_image_meta(arch, st.e)
}

def finalize(var st : AudioTowerStaging) {
    delete st.fblob
    delete st.qblob
    delete st.qscales
    delete st.e
}

[unused_argument(st)]
def private image_post_load(var st : AudioTowerStaging) {}

[init]
def private register_tower_image_tags {
    register_image_family_tag("tower-q8")
    register_image_family_tag("tower-f32")
}

def audio_has_avgpool(t : AudioTower) : bool => (
    t.proj_kind == AudioProjKind.qwen2a || t.proj_kind == AudioProjKind.voxtral)

//! Soft tokens one 30 s (3000-frame) mel chunk becomes after the projector — matches mtmd's
//! clip_n_output_tokens: conv halves, avg-pool halves, stacking divides (floor: the ragged
//! tail positions are DROPPED, mirroring ggml's build_stack view math — no zero-pad row).
def audio_tokens_per_chunk(t : AudioTower) : int64 {
    var n = t.n_ctx
    if (audio_has_avgpool(t)) {
        n /= 2l
    }
    if (t.stack_factor > 1l) {
        n /= t.stack_factor
    }
    return n
}

// Per-layer tensor offsets inside fblob, in layout order. conv weights are stored PERMUTED
// for the im2col column order (dk-major, then channel): W'[o][dk·C + c] = torch_w[o][c][dk].
struct LayerOffs {
    ln1_w : int64
    ln1_b : int64
    q_w : int64
    q_b : int64
    k_w : int64
    k_b : int64   // whisper towers have no k bias — the slot stays zeroed and is skipped
    v_w : int64
    v_b : int64
    o_w : int64
    o_b : int64
    ln2_w : int64
    ln2_b : int64
    fc1_w : int64
    fc1_b : int64
    fc2_w : int64
    fc2_b : int64
}

def enc_layer_offsets(t : AudioTower; l : int64) : LayerOffs {
    let d = t.d_model
    let ff = t.n_ff
    if (t.q8) {
        // compact q8 layout: GEMM mats dense in qblob, vectors dense in the compact fblob —
        // field-for-field the order tower_quantize_'s walk writes
        var og = t.qlayers_off + l * t.qlayer_stride
        var of = t.layers_off + l * t.layer_stride
        var q = LayerOffs()
        q.ln1_w = of; of += d
        q.ln1_b = of; of += d
        q.q_w = og; og += d * d
        q.q_b = of; of += d
        q.k_w = og; og += d * d
        q.k_b = of; of += d
        q.v_w = og; og += d * d
        q.v_b = of; of += d
        q.o_w = og; og += d * d
        q.o_b = of; of += d
        q.ln2_w = of; of += d
        q.ln2_b = of; of += d
        q.fc1_w = og; og += ff * d
        q.fc1_b = of; of += ff
        q.fc2_w = og; og += d * ff
        q.fc2_b = of; of += d
        return q
    }
    var o = t.layers_off + l * t.layer_stride
    var r = LayerOffs()
    r.ln1_w = o; o += d
    r.ln1_b = o; o += d
    r.q_w = o; o += d * d
    r.q_b = o; o += d
    r.k_w = o; o += d * d
    r.k_b = o; o += d
    r.v_w = o; o += d * d
    r.v_b = o; o += d
    r.o_w = o; o += d * d
    r.o_b = o; o += d
    r.ln2_w = o; o += d
    r.ln2_b = o; o += d
    r.fc1_w = o; o += ff * d
    r.fc1_b = o; o += ff
    r.fc2_w = o; o += d * ff
    r.fc2_b = o; o += d
    return r
}

// 4 square attn mats + 2 ffn mats + 9 d-vectors (ln1 w/b, q/k/v/o bias, ln2 w/b, fc2 bias) + fc1 bias
def enc_layer_stride(d, ff : int64) : int64 => 4l * d * d + 2l * ff * d + 9l * d + ff

//! Per-row Q8_0 requant of a token-major activation image, through common's thresholded +
//! profiled form (small rows stay serial; the work lands in the mm_requant bucket).
//! Feeds matmul_q8q8_batch/group3.
def asr_requant_rows_q8(x : array<float>; n, npos : int64; var xqb : array<int8>; var xsb : array<float>) {
    requant_rows_q8(x, n, npos, xqb, xsb)
}

// ===== tower Q8 rail (parakeet's v1 recipe: whole-blob Q8_0 image + per-GEMM-region repack) =====

//! Repack one GEMM region for the active backend (scales index at woff/32 ⇒ woff must be
//! 32-aligned); n%32 != 0 regions stay fp32 — the callers fall back per call on the same
//! condition, so plane and route can never disagree.
def tw_repack(var qblob : array<int8>; var qscales : array<float>; woff, n, d : int64) {
    if (n % 32l != 0l) {
        return
    }
    if (woff % 32l != 0l) {
        panic("audio tower q8: GEMM weight at {woff} is not 32-aligned")
    }
    repack_q8q8_weight(qblob, qscales, woff, n, d)
}

//! Quantizes the tower's GEMM weights to Q8_0 (conv2, per-layer q/k/v/o, fc1/fc2, projector mats).
//! conv1 stays fp32 (its 3·n_mel contraction isn't a 32-multiple on 80-mel models); non-GEMM slots
//! are quantized-but-unread — fblob stays the fp32 truth for them.
def tower_quantize(var st : AudioTowerStaging) {
    tower_quantize_(st.e, st.fblob, st.qblob, st.qscales)
}

def tower_quantize(var t : AudioTower; var pl : AudioTowerPlanes) {
    tower_quantize_(t, pl.fblob, pl.qblob, pl.qscales)
}

//! Copy one fp32 region into a compact blob, returning its new offset (the shared piece
//! of the compact-q8 recipe — whisper's decoder rail walks with it too).
def tq_keep(var nf : array<float>; fblob : array<float>; var of : int64&; off, n : int64) : int64 {
    for (i in range64(n)) {
        nf[of + i] = fblob[off + i]
    }
    let r = of
    of += n
    return r
}

//! Quantize one GEMM region into dense q planes, returning its new (32-aligned) offset.
def tq_quant(var qblob : array<int8>; var qscales : array<float>; fblob : array<float>; var og : int64&; off, n : int64) : int64 {
    unsafe {
        quantize_q8_0_into_ptr(addr(fblob[off]), n, addr(qblob[0]), addr(qscales[0]), og, og / 32l)
    }
    let r = og
    og += n
    return r
}

//! Rewrite a tower's stored offsets to the compact-q8 layout WITHOUT touching data — the
//! streamed mint's twin of tower_quantize_'s walk (the streamed-vs-staged image cell gates
//! that the two stay identical).
def tower_compact_offsets(var t : AudioTower) {
    let d = t.d_model
    let ff = t.n_ff
    let has_conv = t.conv2_w_off != 0l
    var og = 0l
    var of = 0l
    if (has_conv) {
        t.conv1_w_off = of; of += d * 3l * t.n_mel
        t.conv1_b_off = of; of += d
        t.conv2_w_off = og; og += 3l * d * d
        t.conv2_b_off = of; of += d
        t.pos_off = of; of += t.n_ctx * d
    }
    t.qlayers_off = og
    t.qlayer_stride = 4l * d * d + 2l * ff * d
    og += t.n_layer * t.qlayer_stride
    t.layers_off = of
    t.layer_stride = 9l * d + ff
    of += t.n_layer * t.layer_stride
    t.lnp_w_off = of; of += d
    t.lnp_b_off = of; of += d
    if (t.proj_dim > 0l) {
        let m1in = d * t.stack_factor
        let m2in = t.proj_kind == AudioProjKind.ultravox ? t.mm1_out / 2l : t.mm1_out
        if (t.proj_kind == AudioProjKind.qwen2a) {
            t.proj_w_off = og; og += t.proj_dim * d
            t.proj_b_off = of; of += t.proj_dim
        } else {
            if (t.proj_kind == AudioProjKind.ultravox) {
                t.normpre_off = of; of += m1in
            }
            if (m1in % 32l == 0l) {
                t.proj_w_off = og; og += t.mm1_out * m1in
            } else {
                t.proj_w_off = of; of += t.mm1_out * m1in
            }
            if (t.proj_kind == AudioProjKind.ultravox) {
                t.normmid_off = of; of += m2in
            }
            if (m2in % 32l == 0l) {
                t.mm2_w_off = og; og += t.proj_dim * m2in
            } else {
                t.mm2_w_off = of; of += t.proj_dim * m2in
            }
        }
    }
    t.q8 = true
}

def private tower_quantize_(var t : AudioTower; var fblob : array<float>; var qblob : array<int8>; var qscales : array<float>) { // nolint:STYLE037 — the walk mirrors enc_layer_offsets field-for-field; splitting would decouple the two orders
    let d = t.d_model
    let ff = t.n_ff
    if (d % 32l != 0l || ff % 32l != 0l) {
        panic("audio tower q8: d_model {d} / n_ff {ff} are not 32-multiples - the compact layout cannot serve this tower")
    }
    let has_conv = t.conv2_w_off != 0l   // a conv-less core (qwen3a: layers at 0) has no conv front end
    let m1in = d * t.stack_factor
    let m2in = t.proj_kind == AudioProjKind.ultravox ? t.mm1_out / 2l : t.mm1_out
    // a projector mat whose contraction is not a 32-multiple stays fp32 (tw_mm's own condition)
    let qlayer = 4l * d * d + 2l * ff * d
    var pq1 = false
    var pq2 = false
    var qtotal = t.n_layer * qlayer + (has_conv ? 3l * d * d : 0l)
    if (t.proj_dim > 0l) {
        pq1 = t.proj_kind == AudioProjKind.qwen2a || m1in % 32l == 0l
        pq2 = t.proj_kind != AudioProjKind.qwen2a && m2in % 32l == 0l
        qtotal += pq1 ? (t.proj_kind == AudioProjKind.qwen2a ? t.proj_dim * d : t.mm1_out * m1in) : 0l
        qtotal += pq2 ? t.proj_dim * m2in : 0l
    }
    qblob |> reserve_resize(qtotal)
    qscales |> reserve_resize(qtotal / 32l)
    var nf : array<float>
    nf |> reserve_resize(long_length(fblob) - qtotal)
    var og = 0l
    var of = 0l
    if (has_conv) {   // conv1 always stays fp32 (3·n_mel contraction; served via mm_blob_b)
        t.conv1_w_off = tq_keep(nf, fblob, of, t.conv1_w_off, d * 3l * t.n_mel)
        t.conv1_b_off = tq_keep(nf, fblob, of, t.conv1_b_off, d)
        t.conv2_w_off = tq_quant(qblob, qscales, fblob, og, t.conv2_w_off, 3l * d * d)
        t.conv2_b_off = tq_keep(nf, fblob, of, t.conv2_b_off, d)
        t.pos_off = tq_keep(nf, fblob, of, t.pos_off, t.n_ctx * d)
    }
    // layers: the exact field order enc_layer_offsets' q8 branch reads back
    t.qlayers_off = og
    t.qlayer_stride = qlayer
    let old_base = t.layers_off
    let old_stride = t.layer_stride
    let new_base = of
    for (l in range64(t.n_layer)) {
        var oo = old_base + l * old_stride
        tq_keep(nf, fblob, of, oo, d); oo += d                            // ln1_w
        tq_keep(nf, fblob, of, oo, d); oo += d                            // ln1_b
        tq_quant(qblob, qscales, fblob, og, oo, d * d); oo += d * d       // q_w
        tq_keep(nf, fblob, of, oo, d); oo += d                            // q_b
        tq_quant(qblob, qscales, fblob, og, oo, d * d); oo += d * d       // k_w
        tq_keep(nf, fblob, of, oo, d); oo += d                            // k_b
        tq_quant(qblob, qscales, fblob, og, oo, d * d); oo += d * d       // v_w
        tq_keep(nf, fblob, of, oo, d); oo += d                            // v_b
        tq_quant(qblob, qscales, fblob, og, oo, d * d); oo += d * d       // o_w
        tq_keep(nf, fblob, of, oo, d); oo += d                            // o_b
        tq_keep(nf, fblob, of, oo, d); oo += d                            // ln2_w
        tq_keep(nf, fblob, of, oo, d); oo += d                            // ln2_b
        tq_quant(qblob, qscales, fblob, og, oo, ff * d); oo += ff * d     // fc1_w
        tq_keep(nf, fblob, of, oo, ff); oo += ff                          // fc1_b
        tq_quant(qblob, qscales, fblob, og, oo, d * ff); oo += d * ff     // fc2_w
        tq_keep(nf, fblob, of, oo, d); oo += d                            // fc2_b
    }
    t.layers_off = new_base
    t.layer_stride = 9l * d + ff
    t.lnp_w_off = tq_keep(nf, fblob, of, t.lnp_w_off, d)
    t.lnp_b_off = tq_keep(nf, fblob, of, t.lnp_b_off, d)
    if (t.proj_dim > 0l) {   // whisper .bin towers have no projector
        if (t.proj_kind == AudioProjKind.qwen2a) {
            t.proj_w_off = tq_quant(qblob, qscales, fblob, og, t.proj_w_off, t.proj_dim * d)
            t.proj_b_off = tq_keep(nf, fblob, of, t.proj_b_off, t.proj_dim)
        } else {
            if (t.proj_kind == AudioProjKind.ultravox) {
                t.normpre_off = tq_keep(nf, fblob, of, t.normpre_off, m1in)
            }
            t.proj_w_off = (pq1 ? tq_quant(qblob, qscales, fblob, og, t.proj_w_off, t.mm1_out * m1in)
                : tq_keep(nf, fblob, of, t.proj_w_off, t.mm1_out * m1in))
            if (t.proj_kind == AudioProjKind.ultravox) {
                t.normmid_off = tq_keep(nf, fblob, of, t.normmid_off, m2in)
            }
            t.mm2_w_off = (pq2 ? tq_quant(qblob, qscales, fblob, og, t.mm2_w_off, t.proj_dim * m2in)
                : tq_keep(nf, fblob, of, t.mm2_w_off, t.proj_dim * m2in))
        }
    }
    verify(og == qtotal)
    delete fblob
    fblob <- nf
    t.q8 = true
    if (!select_matmul_backend_for_load_()) {
        return   // row-major backend reads the dense Q8_0 planes as-is
    }
    if (has_conv) {
        tw_repack(qblob, qscales, t.conv2_w_off, 3l * d, d)
    }
    for (l in range64(t.n_layer)) {
        let lo = enc_layer_offsets(t, l)   // t.q8 is set: dense offsets
        tw_repack(qblob, qscales, lo.q_w, d, d)
        tw_repack(qblob, qscales, lo.k_w, d, d)
        tw_repack(qblob, qscales, lo.v_w, d, d)
        tw_repack(qblob, qscales, lo.o_w, d, d)
        tw_repack(qblob, qscales, lo.fc1_w, d, ff)
        tw_repack(qblob, qscales, lo.fc2_w, ff, d)
    }
    if (t.proj_dim > 0l) {
        if (t.proj_kind == AudioProjKind.qwen2a) {
            tw_repack(qblob, qscales, t.proj_w_off, d, t.proj_dim)
        } elif (pq1) {
            tw_repack(qblob, qscales, t.proj_w_off, m1in, t.mm1_out)
        }
        if (pq2) {
            tw_repack(qblob, qscales, t.mm2_w_off, m2in, t.proj_dim)
        }
    }
}

// q8 route for a tower GEMM: activation image (xq, xs) pre-quantized by the caller so q/k/v
// share one requant (asr_requant_rows_q8); falls back to the fp32 blob when the region wasn't
// quantizable (same n%32 condition as tw_repack)
def private tw_mm(t : AudioTower; var y : array<float>; woff : int64; x : array<float>;
                  xq : array<int8>; xs : array<float>; n, d, npos : int64) {
    if (t.q8 && n % 32l == 0l) {
        matmul_q8q8_batch(y, t.qblob, t.qscales, woff, xq, xs, n, d, npos)
    } else {
        mm_blob_b(y, t.fblob, woff, x, n, d, npos)
    }
}

def private tw_requant(t : AudioTower; x : array<float>; n, npos : int64; var xq : array<int8>; var xs : array<float>) {
    if (t.q8 && n % 32l == 0l) {
        asr_requant_rows_q8(x, n, npos, xq, xs)
    }
}

// requant-and-multiply for single-consumer activations
def private tw_mm_rq(t : AudioTower; var y : array<float>; woff : int64; x : array<float>;
                     var xq : array<int8>; var xs : array<float>; n, d, npos : int64) {
    tw_requant(t, x, n, npos, xq, xs)
    tw_mm(t, y, woff, x, xq, xs, n, d, npos)
}

// Read a conv1d weight (GGUF flat [c_out][c_in][k], k contiguous — torch layout) permuted to
// the im2col order W'[o][dk·C + c].
def private load_conv_permuted(m : GGUFMeta; bytes : array<uint8> | #; name : string;
                               var blob : array<float>; off : int64; c_out, c_in : int64) {
    var raw : array<float>
    raw |> reserve_resize(int(c_out * c_in * 3l))
    gguf_read_tensor_f32(m, bytes, name, raw, 0l, c_out * c_in * 3l)
    for (o in range64(c_out)) {
        for (c in range64(c_in)) {
            for (k in range64(3l)) {
                blob[off + o * c_in * 3l + k * c_in + c] = raw[o * c_in * 3l + c * 3l + k]
            }
        }
    }
}

//! Loads an audio mmproj GGUF (qwen2a/qwen2.5o/ultravox/voxtral) into an AudioTower, skipping
//! vision tensors in combined omni mmprojs. Like ``load_model``, caches a box/knob-specific
//! PREPARED IMAGE for fast reload; ``DASLLAMA_IMAGE=0`` disables it, a ``.dlim`` path loads one directly.
def load_audio_tower(path : string; q8 : bool = false) : AudioTower {
    let tag = q8 ? "tower-q8" : "tower-f32"
    if (path |> ends_with(".dlim")) {
        // named directly — no mmproj to regenerate from, so a wrong identity panics
        apply_box_profile_runtime()
        let ts_img = ref_time_ticks()
        var t = AudioTower()
        if (!load_image(path, t, tag)) {
            panic("dasLLAMA: '{path}' is not a prepared audio-tower image for this box/knobs (identity {image_identity(tag)}) - regenerate it from the source mmproj gguf")
        }
        to_log(LOG_INFO, "dasLLAMA: prepared tower image mapped in {get_time_usec(ts_img) / 1000} ms - {path}\n")
        return <- t
    }
    if (g_env_engine.image) {
        apply_box_profile_runtime()   // a backend pin must precede the identity (it names the backend)
        let img = image_path_for(path, tag)
        let ts_img = ref_time_ticks()
        var t = AudioTower()
        if (load_image(img, t, tag)) {
            to_log(LOG_INFO, "dasLLAMA: prepared tower image mapped in {get_time_usec(ts_img) / 1000} ms - {img}\n")
            return <- t
        }
        var inscope st <- stage_audio_tower(path, q8)
        var built = AudioTower()
        if (cache_via_image_staged(st, built, img, tag)) {
            dlim_gc_stale(path, tag)   // the fresh bake ON DISK proves its lane's siblings dead; an in-memory build reaps nothing
        }
        return <- built
    }
    // no persisted image: mint one in memory and serve THAT — a served tower is always
    // image-backed, so there is no second, array-owning shape of this struct to maintain
    var inscope st <- stage_audio_tower(path, q8)
    var built = AudioTower()
    cache_via_image_staged(st, built, "", tag)
    return <- built
}

//! Mint a served tower from a staged read WITHOUT touching disk — the off-rail control for suites.
def mint_audio_tower(var st : AudioTowerStaging; var out : AudioTower; q8 : bool = false) {
    cache_via_image_staged(st, out, "", q8 ? "tower-q8" : "tower-f32")
}

//! The mmproj-gguf reader proper: scalars land in the served tower, plane bytes in the staging
//! arrays beside it. Nothing here is servable — the image build is what produces a live tower.
def stage_audio_tower(path : string; q8 : bool = false) : AudioTowerStaging {
    var st = AudioTowerStaging()
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA: cannot open mmproj gguf '{path}'")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        let proj = gguf_str(m, bytes, "clip.projector_type")
        if (proj == "qwen2a" || proj == "qwen2.5o") {   // omni's audio side is qwen2a verbatim
            st.e.proj_kind = AudioProjKind.qwen2a
        } elif (proj == "ultravox") {
            st.e.proj_kind = AudioProjKind.ultravox
        } elif (proj == "voxtral") {
            st.e.proj_kind = AudioProjKind.voxtral
        } else {
            panic("dasLLAMA: audio tower supports qwen2a/qwen2.5o/ultravox/voxtral projectors, got '{proj}'")
        }
        st.e.n_mel = gguf_int(m, bytes, "clip.audio.num_mel_bins")
        st.e.d_model = gguf_int(m, bytes, "clip.audio.embedding_length")
        st.e.n_head = gguf_int(m, bytes, "clip.audio.attention.head_count")
        st.e.n_layer = gguf_int(m, bytes, "clip.audio.block_count")
        st.e.n_ff = gguf_int(m, bytes, "clip.audio.feed_forward_length")
        st.e.proj_dim = gguf_int(m, bytes, "clip.audio.projection_dim")
        if (gguf_has(m, "clip.audio.attention.layer_norm_epsilon")) {
            st.e.eps = gguf_f32(m, bytes, "clip.audio.attention.layer_norm_epsilon")
        }
        if (gguf_has(m, "clip.audio.projector.stack_factor")) {
            st.e.stack_factor = gguf_int(m, bytes, "clip.audio.projector.stack_factor")
        }
        let d = st.e.d_model
        let ff = st.e.n_ff
        let nm = st.e.n_mel
        // encoder positions from the position-embedding tensor (1500 for whisper-large-v3)
        let pi = gguf_find_tensor(m, "a.position_embd.weight")
        if (pi < 0) {
            panic("dasLLAMA: a.position_embd.weight missing from mmproj")
        }
        st.e.n_ctx = m.tensors[pi].n_elem / d

        // shapes come from the tensors, not clip.audio.projection_dim — it lies for ultravox-1b (4096 vs actual 2048)
        var mm1_in = 0l
        if (st.e.proj_kind != AudioProjKind.qwen2a) {
            mm1_in = d * st.e.stack_factor
            let i1 = gguf_find_tensor(m, "mm.a.mlp.1.weight")
            let i2 = gguf_find_tensor(m, "mm.a.mlp.2.weight")
            if (i1 < 0 || i2 < 0) {
                panic("dasLLAMA: mm.a.mlp.1/2.weight missing from '{proj}' mmproj")
            }
            st.e.mm1_out = m.tensors[i1].n_elem / mm1_in
            let mm2_in = st.e.proj_kind == AudioProjKind.ultravox ? st.e.mm1_out / 2l : st.e.mm1_out
            st.e.proj_dim = m.tensors[i2].n_elem / mm2_in
        }

        // fixed blob layout: convs, pos, per-layer region, post-ln, projector
        var o = 0l
        st.e.conv1_w_off = o; o += d * 3l * nm
        st.e.conv1_b_off = o; o += d
        st.e.conv2_w_off = o; o += d * 3l * d
        st.e.conv2_b_off = o; o += d
        st.e.pos_off = o; o += st.e.n_ctx * d
        st.e.layers_off = o
        st.e.layer_stride = enc_layer_stride(d, ff)
        o += st.e.n_layer * st.e.layer_stride
        st.e.lnp_w_off = o; o += d
        st.e.lnp_b_off = o; o += d
        if (st.e.proj_kind == AudioProjKind.qwen2a) {
            st.e.proj_w_off = o; o += st.e.proj_dim * d
            st.e.proj_b_off = o; o += st.e.proj_dim
        } elif (st.e.proj_kind == AudioProjKind.ultravox) {
            st.e.normpre_off = o; o += mm1_in
            st.e.proj_w_off = o; o += st.e.mm1_out * mm1_in
            st.e.normmid_off = o; o += st.e.mm1_out / 2l
            st.e.mm2_w_off = o; o += st.e.proj_dim * (st.e.mm1_out / 2l)
        } else {   // voxtral
            st.e.proj_w_off = o; o += st.e.mm1_out * mm1_in
            st.e.mm2_w_off = o; o += st.e.proj_dim * st.e.mm1_out
        }
        st.fblob |> reserve_resize(o)

        load_conv_permuted(m, bytes, "a.conv1d.1.weight", st.fblob, st.e.conv1_w_off, d, nm)
        gguf_read_tensor_f32(m, bytes, "a.conv1d.1.bias", st.fblob, st.e.conv1_b_off, d)
        load_conv_permuted(m, bytes, "a.conv1d.2.weight", st.fblob, st.e.conv2_w_off, d, d)
        gguf_read_tensor_f32(m, bytes, "a.conv1d.2.bias", st.fblob, st.e.conv2_b_off, d)
        gguf_read_tensor_f32(m, bytes, "a.position_embd.weight", st.fblob, st.e.pos_off, st.e.n_ctx * d)
        for (l in range64(st.e.n_layer)) {
            let lo = enc_layer_offsets(st.e, l)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ln1.weight", st.fblob, lo.ln1_w, d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ln1.bias", st.fblob, lo.ln1_b, d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.attn_q.weight", st.fblob, lo.q_w, d * d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.attn_q.bias", st.fblob, lo.q_b, d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.attn_k.weight", st.fblob, lo.k_w, d * d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.attn_v.weight", st.fblob, lo.v_w, d * d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.attn_v.bias", st.fblob, lo.v_b, d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.attn_out.weight", st.fblob, lo.o_w, d * d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.attn_out.bias", st.fblob, lo.o_b, d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ln2.weight", st.fblob, lo.ln2_w, d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ln2.bias", st.fblob, lo.ln2_b, d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ffn_up.weight", st.fblob, lo.fc1_w, ff * d)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ffn_up.bias", st.fblob, lo.fc1_b, ff)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ffn_down.weight", st.fblob, lo.fc2_w, d * ff)
            gguf_read_tensor_f32(m, bytes, "a.blk.{l}.ffn_down.bias", st.fblob, lo.fc2_b, d)
        }
        gguf_read_tensor_f32(m, bytes, "a.post_ln.weight", st.fblob, st.e.lnp_w_off, d)
        gguf_read_tensor_f32(m, bytes, "a.post_ln.bias", st.fblob, st.e.lnp_b_off, d)
        if (st.e.proj_kind == AudioProjKind.qwen2a) {
            gguf_read_tensor_f32(m, bytes, "mm.a.fc.weight", st.fblob, st.e.proj_w_off, st.e.proj_dim * d)
            gguf_read_tensor_f32(m, bytes, "mm.a.fc.bias", st.fblob, st.e.proj_b_off, st.e.proj_dim)
        } elif (st.e.proj_kind == AudioProjKind.ultravox) {
            let m1in = d * st.e.stack_factor
            gguf_read_tensor_f32(m, bytes, "mm.a.norm_pre.weight", st.fblob, st.e.normpre_off, m1in)
            gguf_read_tensor_f32(m, bytes, "mm.a.mlp.1.weight", st.fblob, st.e.proj_w_off, st.e.mm1_out * m1in)
            gguf_read_tensor_f32(m, bytes, "mm.a.norm_mid.weight", st.fblob, st.e.normmid_off, st.e.mm1_out / 2l)
            gguf_read_tensor_f32(m, bytes, "mm.a.mlp.2.weight", st.fblob, st.e.mm2_w_off, st.e.proj_dim * (st.e.mm1_out / 2l))
        } else {   // voxtral
            let m1in = d * st.e.stack_factor
            gguf_read_tensor_f32(m, bytes, "mm.a.mlp.1.weight", st.fblob, st.e.proj_w_off, st.e.mm1_out * m1in)
            gguf_read_tensor_f32(m, bytes, "mm.a.mlp.2.weight", st.fblob, st.e.mm2_w_off, st.e.proj_dim * st.e.mm1_out)
        }
    }
    fclose(f)
    if (q8) {
        tower_quantize(st)
    }
    return <- st
}

//! Encoder scratch, sized once per tower and reused across chunks/calls.
struct EncoderState {
    @exact_size x1 : array<float>    // conv1 im2col [T_in × 3·n_mel]
    @exact_size y1 : array<float>    // conv1 out [T_in × d]
    @exact_size x2 : array<float>    // conv2 im2col [T × 3·d]
    @exact_size x : array<float>     // residual stream [T × d]
    @exact_size xb : array<float>    // norm output [T × d]
    @exact_size xb2 : array<float>   // attn/ffn projection out [T × d]
    @exact_size qq : array<float>    // q token-major [T × d]
    @exact_size kk : array<float>    // k token-major [T × d]
    @exact_size vv : array<float>    // v token-major [T × d]
    @exact_size hp : array<float>    // per-head packs q,kT,v,out: 4 × [T × d]
    @scratch @exact_size att : array<float>   // per-slot blocked scores [get_dispatch_slot_bound() × ATTN_ENC_QB × T]
    @exact_size h : array<float>     // ffn hidden [T × ff]
    @exact_size pooled : array<float>// [T/2 × d]
    @scratch ph : array<float>    // projector hidden [rows × mm1_out] (ultravox/voxtral)
    pg : array<float>    // ultravox swiglu output [rows × mm1_out/2]
    @scratch out : array<float>   // projected soft tokens [tokens_per_chunk × proj_dim]
    @exact_size xqi : array<int8>    // q8 requant image [T × max(3d, ff)] (empty on fp32 towers)
    @exact_size xsi : array<float>   // per-32-block scales for xqi
}

def make_encoder_state(t : AudioTower) : EncoderState {
    let tin = AUDIO_CHUNK_FRAMES
    let tt = t.n_ctx
    let d = t.d_model
    var s = EncoderState()
    // whole-clip towers carry tt = the clip's own frame count — minutes-scale buffers here
    s.x1 |> reserve_resize(tin * 3l * t.n_mel)
    s.y1 |> reserve_resize(tin * d)
    s.x2 |> reserve_resize(tt * 3l * d)
    s.x |> reserve_resize(tt * d)
    s.xb |> reserve_resize(tt * d)
    s.xb2 |> reserve_resize(tt * d)
    s.qq |> reserve_resize(tt * d)
    s.kk |> reserve_resize(tt * d)
    s.vv |> reserve_resize(tt * d)
    s.hp |> reserve_resize(4l * tt * d)
    s.att |> reserve_resize(int64(get_dispatch_slot_bound()) * ATTN_ENC_QB * tt)
    s.h |> reserve_resize(tt * t.n_ff)
    s.pooled |> reserve_resize(tt / 2l * d)
    let rows = audio_tokens_per_chunk(t)
    if (t.proj_kind != AudioProjKind.qwen2a) {
        s.ph |> reserve_resize(rows * t.mm1_out)
        if (t.proj_kind == AudioProjKind.ultravox) {
            s.pg |> reserve_resize(rows * t.mm1_out / 2l)
        }
    }
    s.out |> reserve_resize(rows * t.proj_dim)
    if (t.q8) {   // largest requant image any tower GEMM feeds (conv2 3d / fc2 ff / projector mats)
        var need = tt * max(3l * d, t.n_ff)
        if (t.proj_dim > 0l && t.proj_kind != AudioProjKind.qwen2a) {
            need = max(need, rows * max(d * t.stack_factor, t.mm1_out))
        }
        s.xqi |> reserve_resize(need)
        s.xsi |> reserve_resize(need / 32l)
    }
    return <- s
}

def private enc_attention(t : AudioTower; var s : EncoderState; tt : int64) {
    let hs = t.d_model / t.n_head
    attention_bidir(s.xb2, s.qq, s.kk, s.vv, s.hp, s.att, tt, t.d_model, t.n_head, 1.0 / sqrt(float(hs)))
}

// Stage-diff witness: when set (debug only), audio_encode maxdiffs each stage against
// <dir>/np_<stage>.f32 raw dumps (the numpy oracle in the arc's scratchpad rig).
var g_audio_ref_dir = ""

def set_audio_encode_ref_dir(dir : string) {
    g_audio_ref_dir = dir
}

[cold_path]
def private diff_stage(name : string; x : array<float>; n : int64) {
    if (empty(g_audio_ref_dir)) {
        return
    }
    let f = fopen("{g_audio_ref_dir}/np_{name}.f32", "rb")
    if (f == null) {
        print("stage {name}: no ref file\n")
        return
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var maxd = 0.0
        var maxi = 0
        unsafe {
            let rp = addr<float?>(bytes[0])
            // int loop: raw-pointer indexing by int64 is AOT-ambiguous (int32/uint32 overloads only)
            let ni = int(n)
            for (i in range(ni)) {
                let d = abs(x[i] - rp[i])
                if (d > maxd) {
                    maxd = d
                    maxi = i
                }
            }
            print("stage {name}: maxdiff {maxd} at {maxi} (das {x[maxi]} ref {rp[maxi]})\n")
        }
    }
    fclose(f)
}

// t.gelu_tanh selects the oracle-exact activation flavor (see gelu_tanh_lut); threaded
// flavor dispatch for row images (both flavors elementwise ⇒ bit-exact at any lane count)
def private enc_gelu_batch(t : AudioTower; var x : array<float>; d, npos : int64) {
    if (t.gelu_tanh) {
        gelu_tanh_lut_batch(x, d, npos)
    } else {
        gelu_erf_batch(x, d, npos)
    }
}

//! Shared Whisper-encoder core over one 30 s mel chunk (conv frontend + pre-LN blocks); leaves
//! the residual stream in ``s.x``. Model-specific tail (ln_post, pooling, projector) is the caller's.
def audio_encode_blocks(t : AudioTower; var s : EncoderState; mel : MelChunk) {
    if (mel.n_len != AUDIO_CHUNK_FRAMES || mel.n_mel != t.n_mel) {
        panic("audio_encode: mel chunk is {mel.n_mel}x{mel.n_len}, want {t.n_mel}x{AUDIO_CHUNK_FRAMES}")
    }
    let tin = AUDIO_CHUNK_FRAMES
    let tt = t.n_ctx
    let d = t.d_model
    let nm = t.n_mel
    let tc = ref_time_ticks()
    trace_tag(TRACE_TAG_ENC_CONV)
    if (g_tw_conv_gpu_set && empty(g_audio_ref_dir) && invoke(g_tw_conv_gpu, t, s, mel)) {
        asr_prof_add("enc.conv", tc)
        encoder_blocks(t, s, tt)
        return
    }

    // conv1 k3 s1 same: im2col column (dk, c) — matches the loader's weight permute
    for (i in range64(tin)) {
        for (dk in range64(3l)) {
            let src = i + dk - 1l
            for (c in range64(nm)) {
                s.x1[i * 3l * nm + dk * nm + c] = (src >= 0l && src < tin
                    ? mel.data[c * tin + src]
                    : 0.0)
            }
        }
    }
    mm_blob_b(s.y1, t.fblob, t.conv1_w_off, s.x1, 3l * nm, d, tin)
    add_bias_rows(s.y1, t.fblob, t.conv1_b_off, d, tin)
    enc_gelu_batch(t, s.y1, d, tin)

    // conv2 k3 s2 same -> T = tin/2 positions
    for (i in range64(tt)) {
        for (dk in range64(3l)) {
            let src = 2l * i + dk - 1l
            for (c in range64(d)) {
                s.x2[i * 3l * d + dk * d + c] = (src >= 0l && src < tin
                    ? s.y1[src * d + c]
                    : 0.0)
            }
        }
    }
    tw_mm_rq(t, s.x, t.conv2_w_off, s.x2, s.xqi, s.xsi, 3l * d, d, tt)
    add_bias_rows(s.x, t.fblob, t.conv2_b_off, d, tt)
    enc_gelu_batch(t, s.x, d, tt)

    // + learned/sinusoidal position rows (loaded from the mmproj)
    for (i in range64(tt * d)) {
        s.x[i] += t.fblob[t.pos_off + i]
    }
    asr_prof_add("enc.conv", tc)
    diff_stage("after_conv_pos", s.x, tt * d)

    encoder_blocks(t, s, tt)
}

// ===== the GPU tower hook (registered by dasllama_metal_tower at [init]; requires-back, so it
// cannot be required from here). Serves the whole block loop or declines; the CPU loop below
// stays the correct fallback. The diff-stage rail keeps the CPU loop (its dumps ARE the point).
typedef TowerBlocksGpuFn = function<(t : AudioTower; var s : EncoderState; npos : int64) : bool>
var private g_tw_blocks_gpu : TowerBlocksGpuFn
var private g_tw_blocks_gpu_set = false

def register_tower_blocks_gpu(fn : TowerBlocksGpuFn) {
    g_tw_blocks_gpu = fn
    g_tw_blocks_gpu_set = true
}

//! The conv-frontend twin: serves conv1/conv2/pos-add for one mel chunk (s.x filled) or
//! declines with false — the CPU conv above stays the correct fallback, best-effort inside
//! the tower knob (a shape misfit like tiny's 3x80 im2col width just keeps the CPU conv).
typedef TowerConvGpuFn = function<(t : AudioTower; var s : EncoderState; mel : MelChunk) : bool>
var private g_tw_conv_gpu : TowerConvGpuFn
var private g_tw_conv_gpu_set = false

def register_tower_conv_gpu(fn : TowerConvGpuFn) {
    g_tw_conv_gpu = fn
    g_tw_conv_gpu_set = true
}

//! The pre-LN transformer block loop over ``npos`` positions of the residual stream in ``s.x``
//! (token-major). Shared by the whisper conv frontend above and the qwen3a conv2d frontend —
//! any frontend that leaves ``npos × d_model`` rows in ``s.x`` can run the blocks.
def encoder_blocks(t : AudioTower; var s : EncoderState; npos : int64) {
    if (g_tw_blocks_gpu_set && empty(g_audio_ref_dir)) {
        let te = ref_time_ticks()
        if (invoke(g_tw_blocks_gpu, t, s, npos)) {
            asr_prof_add("enc.blocks.gpu", te)
            return
        }
    }
    let tt = npos
    let d = t.d_model
    for (l in range64(t.n_layer)) {
        let lo = enc_layer_offsets(t, l)
        var te = ref_time_ticks()
        trace_tag(TRACE_TAG_ENC_ATTN_PROJ)
        layernorm_batch(s.xb, s.x, t.fblob, lo.ln1_w, lo.ln1_b, d, tt, t.eps)
        tw_requant(t, s.xb, d, tt, s.xqi, s.xsi)   // q/k/v share the post-LN activation: one requant
        tw_mm(t, s.qq, lo.q_w, s.xb, s.xqi, s.xsi, d, d, tt)
        tw_mm(t, s.kk, lo.k_w, s.xb, s.xqi, s.xsi, d, d, tt)
        tw_mm(t, s.vv, lo.v_w, s.xb, s.xqi, s.xsi, d, d, tt)
        add_bias_rows(s.qq, t.fblob, lo.q_b, d, tt)   // q bias
        if (t.has_k_bias) {
            add_bias_rows(s.kk, t.fblob, lo.k_b, d, tt)
        }
        add_bias_rows(s.vv, t.fblob, lo.v_b, d, tt)   // v bias
        asr_prof_add("enc.attn_proj", te)
        if (l == 0l) {
            diff_stage("b0_ln1", s.xb, tt * d)
            diff_stage("b0_q", s.qq, tt * d)
            diff_stage("b0_k", s.kk, tt * d)
            diff_stage("b0_v", s.vv, tt * d)
        }
        te = ref_time_ticks()
        trace_tag(TRACE_TAG_ENC_ATTN)
        enc_attention(t, s, tt)
        asr_prof_add("enc.attn_heads", te)
        if (l == 0l) {
            diff_stage("b0_attnout", s.xb2, tt * d)
        }
        te = ref_time_ticks()
        trace_tag(TRACE_TAG_ENC_ATTN_OUT)
        tw_mm_rq(t, s.xb, lo.o_w, s.xb2, s.xqi, s.xsi, d, d, tt)
        add_bias_rows(s.xb, t.fblob, lo.o_b, d, tt)
        add_inplace_rows(s.x, s.xb, tt * d)
        asr_prof_add("enc.attn_out", te)

        te = ref_time_ticks()
        trace_tag(TRACE_TAG_ENC_FC1)
        var tf = ref_time_ticks()
        layernorm_batch(s.xb, s.x, t.fblob, lo.ln2_w, lo.ln2_b, d, tt, t.eps)
        asr_prof_add("enc.ffn.fc1.ln", tf)
        tf = ref_time_ticks()
        tw_requant(t, s.xb, d, tt, s.xqi, s.xsi)
        asr_prof_add("enc.ffn.fc1.rq", tf)
        tf = ref_time_ticks()
        tw_mm(t, s.h, lo.fc1_w, s.xb, s.xqi, s.xsi, d, t.n_ff, tt)
        asr_prof_add("enc.ffn.fc1.mm", tf)
        tf = ref_time_ticks()
        add_bias_rows(s.h, t.fblob, lo.fc1_b, t.n_ff, tt)
        asr_prof_add("enc.ffn.fc1.bias", tf)
        asr_prof_add("enc.ffn.fc1", te)
        te = ref_time_ticks()
        enc_gelu_batch(t, s.h, t.n_ff, tt)
        asr_prof_add("enc.ffn.gelu", te)
        te = ref_time_ticks()
        trace_tag(TRACE_TAG_ENC_FC2)
        tf = ref_time_ticks()
        tw_requant(t, s.h, t.n_ff, tt, s.xqi, s.xsi)
        asr_prof_add("enc.ffn.fc2.rq", tf)
        tf = ref_time_ticks()
        tw_mm(t, s.xb2, lo.fc2_w, s.h, s.xqi, s.xsi, t.n_ff, d, tt)
        asr_prof_add("enc.ffn.fc2.mm", tf)
        tf = ref_time_ticks()
        add_bias_rows(s.xb2, t.fblob, lo.fc2_b, d, tt)
        add_inplace_rows(s.x, s.xb2, tt * d)
        asr_prof_add("enc.ffn.fc2.bias", tf)
        asr_prof_add("enc.ffn.fc2", te)
        if (l == 0l) {
            diff_stage("after_block0", s.x, tt * d)
        }
    }
    trace_tag(0)   // clear: dispatches after the encoder must not inherit the last stage's color
    diff_stage("after_blocks", s.x, tt * d)
}

// RMS-norm each d-wide row in place against a weight vector in the blob (ultravox projector
// norms use eps 1e-6 hardcoded in mtmd's whisper-enc graph, independent of the tower eps)
def private rms_rows_blob(var x : array<float>; blob : array<float>; w_off, dim, rows : int64) {
    rms_rows_blob_at(x, unsafe(addr<float const?>(blob[w_off])), dim, rows)
}

def private rms_rows_blob(var x : array<float>; blob : PlaneF; w_off, dim, rows : int64) {
    rms_rows_blob_at(x, plane_at(blob, w_off), dim, rows)
}

def private rms_rows_blob_at(var x : array<float>; wp : float const?; dim, rows : int64) {
    unsafe {
        var xp = addr(x[0])
        for (r in range64(rows)) {
            rmsnorm(xp + r * dim, xp + r * dim, wp, dim, 1e-6)
        }
    }
}

//! Runs the Whisper encoder + projector tail over one 30 s mel chunk; soft tokens land in ``s.out``.
//! Stacking floors rows — ragged tail positions are dropped, not zero-padded (ggml build_stack view math).
[hot_path]
def audio_encode(t : AudioTower; var s : EncoderState; mel : MelChunk) {
    audio_encode_blocks(t, s, mel)
    trace_tag(TRACE_TAG_ENC_PROJ)
    let tt = t.n_ctx
    let d = t.d_model
    let rows = audio_tokens_per_chunk(t)

    if (t.proj_kind == AudioProjKind.qwen2a) {
        // avg-pool pairs over time (BEFORE the post-LN — mtmd build_vit order), then ln_post
        let tp = tt / 2l
        for (p in range64(tp)) {
            for (c in range64(d)) {
                s.pooled[p * d + c] = 0.5 * (s.x[2l * p * d + c] + s.x[(2l * p + 1l) * d + c])
            }
        }
        layernorm_batch(s.pooled, s.pooled, t.fblob, t.lnp_w_off, t.lnp_b_off, d, tp, t.eps)

        // projector: one linear into the decoder's embedding space
        tw_mm_rq(t, s.out, t.proj_w_off, s.pooled, s.xqi, s.xsi, d, t.proj_dim, tp)
        add_bias_rows(s.out, t.fblob, t.proj_b_off, t.proj_dim, tp)
    } elif (t.proj_kind == AudioProjKind.ultravox) {
        // post-LN over all positions (no avg-pool), stacked rows straight off s.xb's memory
        layernorm_batch(s.xb, s.x, t.fblob, t.lnp_w_off, t.lnp_b_off, d, tt, t.eps)
        let m1in = d * t.stack_factor
        rms_rows_blob(s.xb, t.fblob, t.normpre_off, m1in, rows)   // RMS-pre on stacked rows
        tw_mm_rq(t, s.ph, t.proj_w_off, s.xb, s.xqi, s.xsi, m1in, t.mm1_out, rows)
        swiglu_swapped_rows(s.pg, s.ph, t.mm1_out / 2l, rows)
        rms_rows_blob(s.pg, t.fblob, t.normmid_off, t.mm1_out / 2l, rows)
        tw_mm_rq(t, s.out, t.mm2_w_off, s.pg, s.xqi, s.xsi, t.mm1_out / 2l, t.proj_dim, rows)
    } else {   // voxtral: avg-pool -> post-LN -> stack -> mm1 -> gelu_erf -> mm2
        let tp = tt / 2l
        for (p in range64(tp)) {
            for (c in range64(d)) {
                s.pooled[p * d + c] = 0.5 * (s.x[2l * p * d + c] + s.x[(2l * p + 1l) * d + c])
            }
        }
        layernorm_batch(s.pooled, s.pooled, t.fblob, t.lnp_w_off, t.lnp_b_off, d, tp, t.eps)
        let m1in = d * t.stack_factor
        tw_mm_rq(t, s.ph, t.proj_w_off, s.pooled, s.xqi, s.xsi, m1in, t.mm1_out, rows)
        gelu_erf(s.ph, rows * t.mm1_out)
        tw_mm_rq(t, s.out, t.mm2_w_off, s.ph, s.xqi, s.xsi, t.mm1_out, t.proj_dim, rows)
    }
    trace_tag(0)   // clear: the decoder's dispatches must not inherit the projector's color
    diff_stage("out", s.out, rows * t.proj_dim)
}
