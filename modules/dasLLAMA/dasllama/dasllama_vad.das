options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_vad shared public

require dasllama/dasllama_lint public
require math
require strings
require daslib/fio
require daslib/array_boost
require dasllama/dasllama_math
require dasllama/dasllama_gguf
require dasllama/dasllama_gpu_math   // sigmoid_f32
require dasllama/dasllama_env        // DASLLAMA_VAD_STREAM_GPU
require dasllama/dasllama_repack     // repack_conv_in_tap_out: the convs' load-time layout
require dasllama/dasllama_layout     // vad_build_slab, VadSlabOffs: the device slab

// Silero-VAD (v6, 16 kHz) — native reimplementation. Oracle = onnxruntime on the official
// silero_vad_16k_op15.onnx (proven bit-exact); weights convert from that graph's initializers
// via harness/convert_silero_vad.py.
// The shipped weights (models/silero_vad.bin) are Silero VAD, MIT-licensed (c) 2020-present
// Silero Team — see models/silero_vad.LICENSE and https://github.com/snakers4/silero-vad.

let SV_CHUNK    = 512l   // samples consumed per probability
let private SV_CONTEXT = 64l    // samples of previous chunk carried into the window
let private SV_WIN     = 576l   // context ++ chunk
let private SV_PADDED  = 640l   // reflect-padded window (pad right 64, left 0)
let private SV_NFFT    = 256l
let private SV_HOP     = 128l
let private SV_STFT_CH = 258l   // basis channels (129 real + 129 imag)
let private SV_NMAG    = 129l
let private SV_NFRAMES = 4l     // STFT frames / window = (640-256)/128 + 1
let private SV_HIDDEN  = 128l

// encoder conv1d dims (cout, cin), all kernel 3 pad 1; strides below
let private SV_C1_OUT = 128l; let private SV_C1_IN = 129l
let private SV_C2_OUT =  64l; let private SV_C2_IN = 128l
let private SV_C3_OUT =  64l; let private SV_C3_IN =  64l
let private SV_C4_OUT = 128l; let private SV_C4_IN =  64l

//! Silero-VAD model weights (fp32), loaded from a converted silero_vad.bin. The conv weights are laid [cin][3][cout] at
//! load (the file's [cout][cin][3] transposed), so a tap's weights over every output channel sit back to back. `slab`
//! is every tensor again end to end at `slab_offs` (the device copy's source), `slab_key` its content hash.
struct VadModel {
    stft    : array<float>   // [258][256] STFT basis (windowed DFT)
    conv1_w : array<float>;  conv1_b : array<float>
    conv2_w : array<float>;  conv2_b : array<float>
    conv3_w : array<float>;  conv3_b : array<float>
    conv4_w : array<float>;  conv4_b : array<float>
    lstm_ih : array<float>;  lstm_hh : array<float>;  lstm_b : array<float>   // [512][128], [512][128], [512]
    final_w : array<float>;  final_b : float          // [128]
    slab : array<float>
    slab_offs : VadSlabOffs
    slab_key : uint64
}

//! Per-stream VAD state: the 64-sample audio context + LSTM h/c carried across chunks, plus
//! reused scratch so a chunk allocates nothing. Reset with vad_reset before a new stream.
struct VadState {
    context : array<float>   // [64]
    h       : array<float>   // [128]
    c       : array<float>   // [128]
    // scratch
    padded  : array<float>   // [640]
    @scratch acc : array<float>   // [128] a conv output time step over its output channels
    col     : array<float>   // [258]
    mag     : array<float>   // [129*4]
    e1      : array<float>   // [128*4]
    e2      : array<float>   // [64*2]
    e3      : array<float>   // [64*1]
    feat    : array<float>   // [128]
    gates   : array<float>   // [512]
    gates2  : array<float>   // [512]
    reluh   : array<float>   // [128]
}

// ===== bin container (parakeet-style records) =====

struct private SvRec {
    ttype : int
    n_elem : int64
    off : int64
}

def private sv_str(b : array<uint8> | #; o, n : int64) : string {
    if (n <= 0l) {
        return ""
    }
    var s = ""
    unsafe {
        s = string(temp_array(addr<uint8?>(b[o]), n, type<uint8>))
    }
    return s
}

def private sv_read(recs : table<string; SvRec>; b : array<uint8> | #; name : string;
                    var dst : array<float>; n : int64) {
    if (!key_exists(recs, name)) {
        panic("dasLLAMA vad: missing tensor '{name}' in model file")
    }
    let r = recs?[name] ?? default<SvRec>
    if (r.n_elem != n) {
        panic("dasLLAMA vad: tensor '{name}' has {r.n_elem} elements, want {n}")
    }
    dst |> resize(int(n))
    if (r.ttype == GGML_TYPE_F32) {
        for (i in range64(n)) {
            dst[i] = rd_f32(b, r.off + i * 4l)
        }
    } else {
        for (i in range64(n)) {
            dst[i] = f16_to_f32(rd_u16(b, r.off + i * 2l))
        }
    }
}

def private sv_read_conv(recs : table<string; SvRec>; b : array<uint8> | #; name : string;
                         var dst : array<float>; cout, cin : int64) {
    var src : array<float>
    sv_read(recs, b, name, src, cout * cin * 3l)
    repack_conv_in_tap_out(src, dst, cout, cin, 3l)
}

var private g_vad_path_probe = ""   //! a test's stand-in for the shipped weights' path; "" = the shipped one

//! Where the shipped Silero-VAD weights live: ``modules/dasLLAMA/models/silero_vad.bin`` under the daslang root.
def vad_model_path() : string => !empty(g_vad_path_probe) ? g_vad_path_probe : path_join(get_das_root(), "modules/dasLLAMA/models/silero_vad.bin")

//! A test's stand-in for `vad_model_path` (`path`; "" reads the shipped one again) - the missing-weights refusal's seam.
//! Answers the stand-in it replaced.
def set_vad_model_path_probe_(path : string) : string {
    let was = g_vad_path_probe
    g_vad_path_probe = path
    return was
}

//! Load a converted Silero-VAD bin (harness/convert_silero_vad.py output, fp32).
def load_vad_model(path : string) : VadModel {
    var m = VadModel()
    let f = fopen(path, "rb")
    if (f == null) {
        panic("dasLLAMA vad: cannot open model '{path}'")
    }
    let n_bytes = int64(stat(path).size)
    if (n_bytes < 8l) {
        panic("dasLLAMA vad: '{path}' is too small to be a model")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var o = 0l
        if (rd_u32(bytes, o) != 0x67676d6cu) {
            panic("dasLLAMA vad: bad magic in '{path}' (not a silero_vad.bin)")
        }
        o += 4l
        // 10 i32 header: version, sr, n_fft, hop, context, win, pad_right, stft_ch, n_mag, hidden
        let version = rd_i32(bytes, o); o += 4l
        let sr      = rd_i32(bytes, o); o += 4l
        o += 8l * 4l   // remaining 8 header fields are fixed and validated by the tensor shapes
        if (version != 1) {
            panic("dasLLAMA vad: unsupported bin version {version}")
        }
        if (sr != 16000) {
            panic("dasLLAMA vad: model sample rate {sr}, only 16000 supported")
        }
        // tensor records
        var recs : table<string; SvRec>
        while (o + 12l <= n_bytes) {
            let n_dims = int64(rd_i32(bytes, o)); o += 4l
            let name_len = int64(rd_i32(bytes, o)); o += 4l
            let ttype = rd_i32(bytes, o); o += 4l
            if (n_dims < 0l || n_dims > 8l || name_len < 0l || o + n_dims * 4l + name_len > n_bytes) {
                panic("dasLLAMA vad: corrupt tensor record in '{path}'")
            }
            var n_elem = 1l
            for (_d in range64(n_dims)) {
                n_elem *= int64(rd_i32(bytes, o)); o += 4l
            }
            let name = sv_str(bytes, o, name_len)
            o += name_len
            recs[name] = SvRec(ttype = ttype, n_elem = n_elem, off = o)
            o += n_elem * (ttype == GGML_TYPE_F32 ? 4l : 2l)
        }
        sv_read(recs, bytes, "stft_conv", m.stft, SV_STFT_CH * SV_NFFT)
        sv_read_conv(recs, bytes, "conv1.w", m.conv1_w, SV_C1_OUT, SV_C1_IN)
        sv_read(recs, bytes, "conv1.b", m.conv1_b, SV_C1_OUT)
        sv_read_conv(recs, bytes, "conv2.w", m.conv2_w, SV_C2_OUT, SV_C2_IN)
        sv_read(recs, bytes, "conv2.b", m.conv2_b, SV_C2_OUT)
        sv_read_conv(recs, bytes, "conv3.w", m.conv3_w, SV_C3_OUT, SV_C3_IN)
        sv_read(recs, bytes, "conv3.b", m.conv3_b, SV_C3_OUT)
        sv_read_conv(recs, bytes, "conv4.w", m.conv4_w, SV_C4_OUT, SV_C4_IN)
        sv_read(recs, bytes, "conv4.b", m.conv4_b, SV_C4_OUT)
        sv_read(recs, bytes, "lstm.w_ih", m.lstm_ih, 4l * SV_HIDDEN * SV_HIDDEN)
        sv_read(recs, bytes, "lstm.w_hh", m.lstm_hh, 4l * SV_HIDDEN * SV_HIDDEN)
        sv_read(recs, bytes, "lstm.b", m.lstm_b, 4l * SV_HIDDEN)
        sv_read(recs, bytes, "final.w", m.final_w, SV_HIDDEN)
        var fb : array<float>
        sv_read(recs, bytes, "final.b", fb, 1l)
        m.final_b = fb[0]
    }
    fclose(f)
    vad_build_slab(m, SV_STFT_CH, SV_NFFT, SV_HIDDEN)
    return <- m
}

// ===== forward =====

def private sv_ensure(@scratch var a : array<float>; n : int64) {
    if (long_length(a) != n) {
        a |> resize(n)
    }
}

//! Reset a stream: zero the audio context and LSTM state, size scratch. Call before a new stream.
def vad_reset(var st : VadState) {
    sv_ensure(st.context, SV_CONTEXT); sv_ensure(st.h, SV_HIDDEN); sv_ensure(st.c, SV_HIDDEN)
    sv_ensure(st.padded, SV_PADDED);   sv_ensure(st.acc, SV_C1_OUT)
    sv_ensure(st.col, SV_STFT_CH);     sv_ensure(st.mag, SV_NMAG * SV_NFRAMES)
    sv_ensure(st.e1, SV_C1_OUT * SV_NFRAMES); sv_ensure(st.e2, SV_C2_OUT * 2l)
    sv_ensure(st.e3, SV_C3_OUT * 1l);  sv_ensure(st.feat, SV_C4_OUT * 1l)
    sv_ensure(st.gates, 4l * SV_HIDDEN); sv_ensure(st.gates2, 4l * SV_HIDDEN)
    sv_ensure(st.reluh, SV_HIDDEN)
    for (i in range64(SV_CONTEXT)) { st.context[i] = 0.0 }
    for (i in range64(SV_HIDDEN)) { st.h[i] = 0.0; st.c[i] = 0.0 }
}

//! conv1d, kernel 3, pad 1 (SAME), + ReLU. in/out are channel-major [ch][time]; `w` is [cin][3][cout]. Each output sums
//! its in-range taps in (input channel, tap) order, the pad's taps skipped.
def private sv_conv1d_relu(var out : array<float>; var acc : array<float>; inp : array<float>; w : array<float>; b : array<float>;
                           cin, cout, ti, stride : int64) {
    let to = (ti + 2l - 3l) / stride + 1l
    unsafe {
        var ap = addr(acc[0])
        let wp = addr<float const?>(w[0])
        for (ot in range64(to)) {
            let base = ot * stride - 1l   // -1 accounts for pad-left
            let k0 = max(0l, -base)
            let k1 = min(3l, ti - base)
            copy_floats(ap, addr<float const?>(b[0]), cout)
            for (ic in range64(cin)) {
                for (k in range64(k0, k1)) {
                    axpy(ap, wp + (ic * 3l + k) * cout, inp[ic * ti + base + k], cout)
                }
            }
            for (oc in range64(cout)) {
                out[oc * to + ot] = max(ap[oc], 0.0)
            }
        }
    }
}

//! Advance one 512-sample chunk and return its speech probability, updating streaming state.
//! `chunk` must hold exactly SV_CHUNK samples (16 kHz mono f32).
[hot_path]
def vad_chunk_prob(m : VadModel; var st : VadState; chunk : array<float> | #) : float {
    guard_interp_inference("VAD")   // the one seam every VAD verb crosses (~31 calls/s: one branch)
    // 1. window = context ++ chunk, reflect-pad right 64 (left 0)
    for (i in range64(SV_CONTEXT)) { st.padded[i] = st.context[i] }
    for (i in range64(SV_CHUNK)) { st.padded[SV_CONTEXT + i] = chunk[i] }
    for (k in range64(SV_PADDED - SV_WIN)) { st.padded[SV_WIN + k] = st.padded[SV_WIN - 2l - k] }

    // 2. STFT -> magnitude [129][4]
    for (t in range64(SV_NFRAMES)) {
        unsafe {
            matvec_rows(addr(st.col[0]), addr<float const?>(m.stft[0]), addr<float const?>(st.padded[t * SV_HOP]), SV_NFFT, 0l, SV_STFT_CH)
        }
        for (fb in range64(SV_NMAG)) {
            let re = st.col[fb]
            let im = st.col[SV_NMAG + fb]
            st.mag[fb * SV_NFRAMES + t] = sqrt(re * re + im * im)
        }
    }

    // 3. encoder conv stack (strides 1,2,2,1)
    sv_conv1d_relu(st.e1, st.acc, st.mag,  m.conv1_w, m.conv1_b, SV_C1_IN, SV_C1_OUT, SV_NFRAMES, 1l)
    sv_conv1d_relu(st.e2, st.acc, st.e1,   m.conv2_w, m.conv2_b, SV_C2_IN, SV_C2_OUT, SV_NFRAMES, 2l)
    sv_conv1d_relu(st.e3, st.acc, st.e2,   m.conv3_w, m.conv3_b, SV_C3_IN, SV_C3_OUT, 2l, 2l)
    sv_conv1d_relu(st.feat, st.acc, st.e3, m.conv4_w, m.conv4_b, SV_C4_IN, SV_C4_OUT, 1l, 1l)

    // 4. LSTM cell (pytorch gate order i,f,g,o)
    unsafe {
        matvec_rows(addr(st.gates[0]), addr<float const?>(m.lstm_ih[0]), addr<float const?>(st.feat[0]), SV_HIDDEN, 0l, 4l * SV_HIDDEN)
        matvec_rows(addr(st.gates2[0]), addr<float const?>(m.lstm_hh[0]), addr<float const?>(st.h[0]), SV_HIDDEN, 0l, 4l * SV_HIDDEN)
    }
    for (j in range64(SV_HIDDEN)) {
        let gi = st.gates[j]                 + st.gates2[j]                 + m.lstm_b[j]
        let gf = st.gates[SV_HIDDEN + j]      + st.gates2[SV_HIDDEN + j]      + m.lstm_b[SV_HIDDEN + j]
        let gg = st.gates[2l * SV_HIDDEN + j] + st.gates2[2l * SV_HIDDEN + j] + m.lstm_b[2l * SV_HIDDEN + j]
        let go = st.gates[3l * SV_HIDDEN + j] + st.gates2[3l * SV_HIDDEN + j] + m.lstm_b[3l * SV_HIDDEN + j]
        let ig = sigmoid_f32(gi)
        let fg = sigmoid_f32(gf)
        let og = sigmoid_f32(go)
        let cg = tanh(gg)
        let cn = fg * st.c[j] + ig * cg
        st.c[j] = cn
        st.h[j] = og * tanh(cn)
    }

    // 5. decoder: relu(h) -> final_conv (128->1) -> sigmoid
    for (j in range64(SV_HIDDEN)) { st.reluh[j] = max(st.h[j], 0.0) }
    let wfp = unsafe(addr<float const?>(m.final_w[0]))
    let rhp = unsafe(addr<float const?>(st.reluh[0]))
    let prob = sigmoid_f32(dot(wfp, rhp, SV_HIDDEN) + m.final_b)

    // 6. carry context forward (last 64 of chunk)
    for (i in range64(SV_CONTEXT)) { st.context[i] = chunk[SV_CHUNK - SV_CONTEXT + i] }
    return prob
}

//! the GPU seat: advance the stream `st` over the ceil(n / 512) chunks of the `n` samples at `src` (the ragged tail
//! zero-padded), its context, h and c carried as `vad_chunk_prob` carries them, appending each chunk's probability to
//! `probs`; false declines with `st` and `probs` untouched
typedef VadGpuChunksFn = function<(m : VadModel; var st : VadState; src : float const?; n : int64; var probs : array<float>) : bool>
var private g_vad_gpu : VadGpuChunksFn
var private g_vad_gpu_set = false
var private g_vad_stream_gpu = g_env_engine.vad_stream_gpu
var private g_vad_said : table<string>   //! the overrides' announces already made

[arch(at="../ARCHITECTURE_MEDIA.md#tower-gpu-hook")]
def register_vad_gpu(fn : VadGpuChunksFn) {
    g_vad_gpu = fn
    g_vad_gpu_set = true
}

//! Route the streaming `vad_iter_feed` through the GPU seat (`on`) or the CPU chunk step; answers the setting it replaced
//! (DASLLAMA_VAD_STREAM_GPU's in-process twin). The batch `vad_probs` asks the seat whatever this says.
def set_vad_stream_gpu(on : bool) : bool {
    let was = g_vad_stream_gpu
    g_vad_stream_gpu = on
    return was
}

//! the n > 0 samples at `src` through the GPU seat onto `st`, the probabilities appended to `probs`; false = no seat, or declined
def private vad_gpu_chunks(m : VadModel; var st : VadState; src : float const?; n : int64; var probs : array<float>) : bool {
    return false if (!g_vad_gpu_set)
    guard_interp_inference("VAD")
    return invoke(g_vad_gpu, m, st, src, n, probs)
}

//! Batch: reset state and return one speech probability per 512-sample chunk over `samples`
//! (the ragged tail is zero-padded, matching silero get_speech_timestamps); the GPU seat serves it where one is registered.
def vad_probs(m : VadModel; samples : array<float> | #) : array<float> {
    var st : VadState
    vad_reset(st)
    var out : array<float>
    if (!empty(samples) && vad_gpu_chunks(m, st, unsafe(addr<float const?>(samples[0])), long_length(samples), out)) {
        return <- out
    }
    return <- vad_probs_cpu(m, samples)
}

//! `vad_probs` on the CPU chunk step alone - the specification the GPU seat is held to.
def vad_probs_cpu(m : VadModel; samples : array<float> | #) : array<float> {
    var st : VadState
    vad_reset(st)
    var out, chunk : array<float>
    chunk |> resize(int(SV_CHUNK))
    let n = long_length(samples)
    var start = 0l
    while (start < n) {
        for (i in range64(SV_CHUNK)) {
            let idx = start + i
            chunk[i] = idx < n ? samples[idx] : 0.0
        }
        out |> push(vad_chunk_prob(m, st, chunk))
        start += SV_CHUNK
    }
    return <- out
}

// ===== segmenter =====

//! One detected speech region, in sample indices at 16 kHz.
struct SpeechSegment {
    start : int64
    end   : int64
}

//! Segmenter tuning (silero defaults). `neg_threshold` closes a segment once prob drops below
//! it (hysteresis); build with default_vad_opts() and override fields as needed.
struct VadOpts {
    threshold      : float
    neg_threshold  : float
    min_speech_ms  : int
    min_silence_ms : int
    speech_pad_ms  : int
    sampling_rate  : int
}

//! Silero's default 16 kHz thresholds (get_speech_timestamps): thr 0.5, neg 0.35,
//! min-speech 250 ms, min-silence 100 ms, pad 30 ms.
def default_vad_opts() : VadOpts {
    return VadOpts(threshold = 0.5, neg_threshold = 0.35, min_speech_ms = 250,
                   min_silence_ms = 100, speech_pad_ms = 30, sampling_rate = 16000)
}

//! Split `samples` into speech segments — the batch VAD. Transcribed from silero
//! get_speech_timestamps (default max_speech = inf path): onset at prob≥threshold, offset at
//! prob<neg_threshold sustained for min_silence, short segments dropped, boundaries padded.
def find_speech_timestamps(m : VadModel; samples : array<float> | #; opts : VadOpts) : array<SpeechSegment> {
    let probs <- vad_probs(m, samples)
    return <- speech_timestamps_of(probs, long_length(samples), opts)
}

//! The segmenter alone: the speech regions a clip of `audio_len` samples holds, read off its per-chunk probabilities.
def speech_timestamps_of(probs : array<float>; audio_len : int64; opts : VadOpts) : array<SpeechSegment> {
    let sr = int64(opts.sampling_rate)
    let min_speech = sr * int64(opts.min_speech_ms) / 1000l
    let min_silence = sr * int64(opts.min_silence_ms) / 1000l
    let speech_pad = sr * int64(opts.speech_pad_ms) / 1000l

    var segs : array<SpeechSegment>
    var triggered = false
    var cur = SpeechSegment()
    var has_cur = false
    var temp_end = 0l
    for (i in range(length(probs))) {
        let prob = probs[i]
        let cur_sample = SV_CHUNK * int64(i)
        if (prob >= opts.threshold && temp_end != 0l) {
            temp_end = 0l
        }
        if (prob >= opts.threshold && !triggered) {
            triggered = true
            cur.start = cur_sample
            has_cur = true
            continue
        }
        if (prob < opts.neg_threshold && triggered) {
            if (temp_end == 0l) {
                temp_end = cur_sample
            }
            if (cur_sample - temp_end < min_silence) {
                continue
            }
            cur.end = temp_end
            if (cur.end - cur.start > min_speech) {
                segs |> push(cur)
            }
            has_cur = false
            temp_end = 0l
            triggered = false
        }
    }
    if (has_cur && audio_len - cur.start > min_speech) {
        cur.end = audio_len
        segs |> push(cur)
    }

    // pad boundaries, splitting shared silence between neighbours (get_speech_timestamps tail)
    let ns = length(segs)
    for (i in range(ns)) {
        if (i == 0) {
            segs[i].start = max(0l, segs[i].start - speech_pad)
        }
        if (i != ns - 1) {
            let sil = segs[i + 1].start - segs[i].end
            if (sil < 2l * speech_pad) {
                segs[i].end += sil / 2l
                segs[i + 1].start = max(0l, segs[i + 1].start - sil / 2l)
            } else {
                segs[i].end = min(audio_len, segs[i].end + speech_pad)
                segs[i + 1].start = max(0l, segs[i + 1].start - speech_pad)
            }
        } else {
            segs[i].end = min(audio_len, segs[i].end + speech_pad)
        }
    }
    return <- segs
}

// ===== online iterator (for live streams / dictation) =====

enum VadEventKind {
    speech_start
    speech_end
}

//! An online VAD boundary event: a speech onset or offset at `sample` (16 kHz index).
struct VadEvent {
    kind   : VadEventKind
    sample : int64
}

//! Streaming segmenter (silero VADIterator). Feed arbitrary-length audio; it buffers into
//! 512-sample windows and emits speech_start / speech_end events online. Use for live capture.
struct VadIter {
    st      : VadState
    opts    : VadOpts
    pending : array<float>   // samples not yet formed into a 512-window
    chunk   : array<float>   // reused 512 window
    cur     : int64          // total samples consumed into windows
    triggered : bool
    temp_end  : int64
}

//! Reset an iterator for a new stream (zeros model + segmenter state).
def vad_iter_reset(var it : VadIter; opts : VadOpts) {
    vad_reset(it.st)
    it.opts = opts
    it.pending |> clear()
    it.chunk |> resize(SV_CHUNK)
    it.cur = 0l
    it.triggered = false
    it.temp_end = 0l
}

//! Feed audio; emit a VadEvent for every boundary crossed. Leftover (<512) samples are buffered
//! for the next call. `sample` on a start/end is the padded boundary in the total stream.
def vad_iter_feed(m : VadModel; var it : VadIter; samples : array<float> | #;
                  blk : block<(ev : VadEvent)>) {
    let sr = int64(it.opts.sampling_rate)
    let min_silence = sr * int64(it.opts.min_silence_ms) / 1000l
    let speech_pad = sr * int64(it.opts.speech_pad_ms) / 1000l
    it.pending |> push_from(samples)
    var consumed = 0l
    let total = long_length(it.pending)
    var gpu_probs : array<float>
    let whole = total / SV_CHUNK * SV_CHUNK
    if (g_vad_stream_gpu) {
        gpu_said_once(g_vad_said, LOG_INFO, "vad: ", "DASLLAMA_VAD_STREAM_GPU=1 or set_vad_stream_gpu(true) - the streaming VAD feeds its whole chunks to the GPU seat")
    }
    let gpu = (g_vad_stream_gpu && whole > 0l
        && vad_gpu_chunks(m, it.st, unsafe(addr<float const?>(it.pending[0])), whole, gpu_probs))
    var ci = 0
    while (consumed + SV_CHUNK <= total) {
        var prob = 0.0
        if (gpu) {
            prob = gpu_probs[ci]
            ci++
        } else {
            for (j in range64(SV_CHUNK)) {
                it.chunk[j] = it.pending[consumed + j]
            }
            prob = vad_chunk_prob(m, it.st, it.chunk)
        }
        consumed += SV_CHUNK
        it.cur += SV_CHUNK
        if (prob >= it.opts.threshold && it.temp_end != 0l) {
            it.temp_end = 0l
        }
        if (prob >= it.opts.threshold && !it.triggered) {
            it.triggered = true
            let s = max(0l, it.cur - speech_pad - SV_CHUNK)
            invoke(blk, VadEvent(kind = VadEventKind.speech_start, sample = s))
        } elif (prob < it.opts.neg_threshold && it.triggered) {
            if (it.temp_end == 0l) {
                it.temp_end = it.cur
            }
            if (it.cur - it.temp_end >= min_silence) {
                // clamp the padded end to consumed audio: at pad > 32 ms the raw padded offset can
                // point past it.cur (silero VADIterator leaves it unclamped) — a no-op at the 30 ms
                // default, so the event-for-event parity gate is unaffected.
                let e = clamp(it.temp_end + speech_pad - SV_CHUNK, 0l, it.cur)
                it.temp_end = 0l
                it.triggered = false
                invoke(blk, VadEvent(kind = VadEventKind.speech_end, sample = e))
            }
        }
    }
    // keep the ragged tail for next feed
    if (consumed > 0l) {
        var rest : array<float>
        rest |> reserve(int(total - consumed))
        for (i in range64(consumed, total)) {
            rest |> push(it.pending[i])
        }
        it.pending <- rest
    }
}
