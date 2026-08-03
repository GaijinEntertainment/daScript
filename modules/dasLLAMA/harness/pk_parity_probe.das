options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

// v3 oracle gate: dump per-token id/frame/dur_idx/dur_val for fp32 AND q8 loads,
// TSV lines comparable against `parakeet-cli -ng -ps`.
//   bin/daslang -jit modules/dasLLAMA/harness/pk_parity_probe.das -- <model.bin> <audio.wav>

require dasllama/dasllama_asr
require dasllama/dasllama_parakeet
require dasllama/dasllama_audio
require dasllama/dasllama_math
require audio
require daslib/jobque_boost

require strings
require math

def load_audio_16k_mono(path : string) : array<float> {
    var decoder = new ma_decoder
    var config <- ma_decoder_config_init(ma_format.ma_format_f32, 1u, uint(AUDIO_SAMPLE_RATE))
    if (ma_decoder_init_file(path, unsafe(addr(config)), decoder) != ma_result.MA_SUCCESS) {
        panic("cannot open audio file '{path}'")
    }
    var out : array<float>
    let chunk = 65536
    var buf : array<float>
    buf |> resize(chunk)
    while (true) {
        let got = int(ma_decoder_read_pcm_frames(decoder, unsafe(addr(buf[0])), uint64(chunk)))
        break if (got <= 0)
        out |> reserve(length(out) + got)
        for (i in range(got)) {
            out |> push(buf[i])
        }
        break if (got < chunk)
    }
    ma_decoder_uninit(decoder)
    unsafe {
        delete decoder
    }
    return <- out
}

def dump_tokens(tag : string; var m : AsrModel; samples : array<float>) {
    var st = ParakeetState()
    parakeet_log_mel(samples, m.pk, st)
    let tt = parakeet_encode(m.pk, st)
    parakeet_tdt_decode(m.pk, st, tt)
    for (i, t in range(length(st.toks)), st.toks) {
        print("{tag}\t{i}\t{t.id}\t{int(t.frame)}\t{t.dur_idx}\t{t.dur}\n")
    }
    print("{tag}-TEXT\t{parakeet_detokenize(m.pk, st.toks)}\n")
    print("{tag}-COUNT\t{length(st.toks)}\n")
}

def cksum(x : array<float>; n : int64) : double {
    var s = 0.0lf
    for (i in range64(n)) {
        s += double(x[i]) * double(i % 7l + 1l)
    }
    return s
}

// scratch-reuse gate: one state across clips of different lengths — grow, then shrink-use,
// stale-data anywhere shows up as a token diff vs the fresh-state dump
def dump_tokens_reused(tag : string; var m : AsrModel; a, b : array<float>) {
    var st = ParakeetState()
    var r = 0
    for (clip in [0, 1, 0]) {
        r++
        parakeet_log_mel(clip == 0 ? a : b, m.pk, st)
        print("{tag}-CK\trun{r}\tmel\t{cksum(st.mel.data, st.mel.n_len * st.mel.n_mel)}\n")
        let tt = parakeet_encode(m.pk, st)
        print("{tag}-CK\trun{r}\tenc\t{cksum(st.x, tt * 1024l)}\n")
        parakeet_tdt_decode(m.pk, st, tt)
    }
    for (i, t in range(length(st.toks)), st.toks) {
        print("{tag}\t{i}\t{t.id}\t{int(t.frame)}\t{t.dur_idx}\t{t.dur}\n")
    }
    print("{tag}-COUNT\t{length(st.toks)}\n")
}

[export]
def main() {
    let args <- get_command_line_arguments()
    // last two args = model, wav (a "--" separator precedes them)
    let model_path = args[length(args) - 2]
    let wav_path = args[length(args) - 1]
    let samples <- load_audio_16k_mono(wav_path)
    print("SAMPLES\t{length(samples)}\n")
    with_job_que() {
        setup_dasllama_jobque_()
        var half : array<float>
        half |> resize(length(samples) * 2 / 3)
        for (i in range(length(half))) {
            half[i] = samples[i]
        }
        set_asr_fp32(true)
        var mf <- load_asr_model(model_path)
        set_asr_fp32(false)
        print("VOCAB\t{int(mf.pk.n_vocab)}\tLAYERS\t{int(mf.pk.n_layer)}\tQ8\t{mf.pk.q8}\n")
        dump_tokens("F32", mf, samples)
        dump_tokens_reused("F32R", mf, samples, half)      // fp32 reuse probe
        dump_tokens_reused("F32SAME", mf, samples, samples) // fp32 same-size reuse probe
        delete mf.pk.blob
        var mq <- load_asr_model(model_path)
        print("Q8LOAD\tq8={mq.pk.q8}\n")
        dump_tokens("Q8", mq, samples)
        // reuse gate: this clip -> a different-length synthetic clip -> this clip again
        dump_tokens_reused("Q8R", mq, samples, half)
        dump_tokens_reused("Q8SAME", mq, samples, samples)  // same-size reuse probe
        delete mq.pk.blob
    }
}
