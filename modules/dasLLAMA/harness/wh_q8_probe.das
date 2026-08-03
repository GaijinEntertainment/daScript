options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

// whisper q8 gate (tower + decoder): q8-vs-fp32 token streams for one (model, wav) — run per
// numeric change alongside the suite's tiny oracle. Timestamp tokens (id >= tok_beg) are
// counted separately: greedy near-ties flip segment-split timestamps first — the accepted
// class, like parakeet's duration-only jitter. WORD flips are the real q8 quality signal;
// adjudicate each with a teacher-forced top-2-gap probe before accepting (decoder-q8 base
// on jfk: one comma dropped on a 0.018-logit fp32 top-2 near-tie — accepted).
//
//   bin/daslang -jit modules/dasLLAMA/harness/wh_q8_probe.das -- <model.bin> <audio.wav>

require dasllama/dasllama_asr
require dasllama/dasllama_whisper
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

def collect(m : AsrModel; samples : array<float>) : array<int> {
    var s = create_session(m, "en")
    var toks : array<int>
    transcribe(m, s, samples) $(seg : TranscribeSegment) {
        for (tk in seg.tokens) {
            toks |> push(tk)
        }
    }
    return <- toks
}

[export]
def main() {
    let args <- get_command_line_arguments()
    let model_path = args[length(args) - 2]
    let wav_path = args[length(args) - 1]
    let samples <- load_audio_16k_mono(wav_path)
    var mq <- load_asr_model(model_path)      // q8 default
    set_asr_fp32(true)
    var mf <- load_asr_model(model_path)
    set_asr_fp32(false)
    print("q8={mq.whisper.enc.q8} fp32-pin={!mf.whisper.enc.q8}\n")
    with_job_que() {
        setup_dasllama_jobque_()
        let tq <- collect(mq, samples)
        let tf <- collect(mf, samples)
        // timestamps (>= tok_beg) flip on near-tie segment splits — compare word streams
        let beg = mq.whisper.tok_beg
        var wq : array<int>
        var wf : array<int>
        for (tk in tq) {
            if (tk < beg) {
                wq |> push(tk)
            }
        }
        for (tk in tf) {
            if (tk < beg) {
                wf |> push(tk)
            }
        }
        var word_flips = length(wq) != length(wf) ? max(length(wq), length(wf)) : 0
        if (word_flips == 0) {
            for (a, b in wq, wf) {
                if (a != b) {
                    word_flips++
                }
            }
        }
        print("Q8GATE\twords q8={length(wq)} fp32={length(wf)} word_flips={word_flips}\ttotal q8={length(tq)} fp32={length(tf)}\n")
        if (word_flips > 0) {
            print("q8:   {tq}\n")
            print("fp32: {tf}\n")
        }
    }
}
