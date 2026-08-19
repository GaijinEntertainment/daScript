options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

// Whisper phase-0 reuse gate: session scratch (ds.mel/win/prompt/toks) is reused across
// transcribes — every leg on a REUSED session must fingerprint-match a fresh session on the
// same clip. Legs: jfk->gb1->jfk (shrink/grow), gb1->jfk->gb1, jfk x3, feed/flush stream.
// Run per session-scratch change alongside wh_q8_probe; --short = jfk-only (turbo-sized).
//
//   bin/daslang -jit modules/dasLLAMA/harness/wh_reuse_probe.das -- <model.bin> <jfk.wav> <gb1.wav> [--fp32] [--short]

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

// full fingerprint: per-segment times + token ids
def fp_of(m : AsrModel; var s : AsrSession; samples : array<float>) : string {
    return build_string() $(writer) {
        transcribe(m, s, samples) $(seg : TranscribeSegment) {
            writer |> write("{seg.t0}:{seg.t1}[")
            for (tk in seg.tokens) {
                writer |> write("{tk},")
            }
            writer |> write("]")
        }
    }
}

def fresh_fp(m : AsrModel; samples : array<float>) : string {
    var s = create_session(m, "en")
    return fp_of(m, s, samples)
}

// ids-only fingerprint (feed/flush legs have shifted t_base)
def ids_of_flush(m : AsrModel; var s : AsrSession; samples : array<float>) : string {
    return build_string() $(writer) {
        feed(m, s, samples)
        flush(m, s) $(seg : TranscribeSegment) {
            for (tk in seg.tokens) {
                writer |> write("{tk},")
            }
        }
    }
}

def ids_only(fp : string) : string {
    // strip "t0:t1[" prefixes: keep just the bracketed id lists concatenated
    return build_string() $(writer) {
        var inside = false
        peek_data(fp) $(arr) {
            for (ch in arr) {
                if (int(ch) == 0x5B) {         // '['
                    inside = true
                } elif (int(ch) == 0x5D) {     // ']'
                    inside = false
                } elif (inside) {
                    writer |> write(to_char(int(ch)))
                }
            }
        }
    }
}

var g_fail = 0

def check(name : string; got, want : string) {
    if (got == want) {
        print("  PASS {name}\n")
    } else {
        g_fail++
        print("  FAIL {name}\n    got:  {got}\n    want: {want}\n")
    }
}

[export]
def main() : int {
    let args <- get_command_line_arguments()
    var fp32 = false
    var short_run = false
    var paths : array<string>
    for (a in args) {
        if (a == "--fp32") {
            fp32 = true
        } elif (a == "--short") {
            short_run = true
        } else {
            paths |> push(a)
        }
    }
    if (length(paths) < 3) {
        panic("usage: wh_reuse_probe.das -- <model.bin> <jfk.wav> <gb1.wav> [--fp32] [--short]")
    }
    let model_path = paths[length(paths) - 3]   // last three non-flag args (arg 0/1 = binary/script)
    let jfk <- load_audio_16k_mono(paths[length(paths) - 2])
    let gb1 <- load_audio_16k_mono(paths[length(paths) - 1])
    set_asr_fp32(fp32)
    var m <- load_asr_model(model_path)
    print("model={model_path} q8={m.whisper.enc.q8} short={short_run}\n")
    with_job_que() {
        setup_dasllama_jobque_()
        let ja = fresh_fp(m, jfk)
        if (short_run) {
            // jfk x2 on one session (long clips skipped — turbo-sized smoke)
            var s = create_session(m, "en")
            check("jfk #1", fp_of(m, s, jfk), ja)
            check("jfk #2 (reuse)", fp_of(m, s, jfk), ja)
        } else {
            let ga = fresh_fp(m, gb1)
            {
                var s = create_session(m, "en")
                check("jfk->gb1->jfk leg1", fp_of(m, s, jfk), ja)
                check("jfk->gb1->jfk leg2 (grow)", fp_of(m, s, gb1), ga)
                check("jfk->gb1->jfk leg3 (shrink)", fp_of(m, s, jfk), ja)
            }
            {
                var s = create_session(m, "en")
                check("gb1->jfk->gb1 leg1", fp_of(m, s, gb1), ga)
                check("gb1->jfk->gb1 leg2 (shrink)", fp_of(m, s, jfk), ja)
                check("gb1->jfk->gb1 leg3 (grow)", fp_of(m, s, gb1), ga)
            }
            {
                var s = create_session(m, "en")
                check("jfk x3 #1", fp_of(m, s, jfk), ja)
                check("jfk x3 #2", fp_of(m, s, jfk), ja)
                check("jfk x3 #3", fp_of(m, s, jfk), ja)
            }
            {
                // feed/flush stream reuse: ids must match the one-shot stream (times shift by t_base)
                var s = create_session(m, "en")
                check("feed/flush jfk ids", ids_of_flush(m, s, jfk), ids_only(ja))
                check("feed/flush gb1 ids (reuse)", ids_of_flush(m, s, gb1), ids_only(ga))
            }
        }
    }
    print(g_fail == 0 ? "REUSE GATE: ALL PASS\n" : "REUSE GATE: {g_fail} FAILED\n")
    return g_fail == 0 ? 0 : 1
}
