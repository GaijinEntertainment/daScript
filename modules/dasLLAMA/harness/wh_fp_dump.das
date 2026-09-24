options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

// Dump the full transcription fingerprint (per-segment t0:t1[token ids]) for one
// (model, wav) in both q8 and fp32-pin modes — pre/post A/B rail for decode changes.
//
//   bin/daslang -jit modules/dasLLAMA/harness/wh_fp_dump.das -- <model.bin> <audio.wav>

require dasllama/dasllama_asr
require dasllama/dasllama_whisper
require dasllama/dasllama_audio
require dasllama/dasllama_math
require _audio_probe_fixture
require daslib/jobque_boost
require strings
require math

def fp_of(m : AsrModel; samples : array<float>) : string {
    var s = create_session(m, "en")
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

[export]
def main() {
    let args <- get_command_line_arguments()
    let model_path = args[length(args) - 2]
    let wav_path = args[length(args) - 1]
    let samples <- load_audio_16k_mono(wav_path)
    var mq <- load_asr_model(model_path)
    set_asr_fp32(true)
    var mf <- load_asr_model(model_path)
    set_asr_fp32(false)
    with_job_que() {
        setup_dasllama_jobque_()
        print("Q8FP\t{fp_of(mq, samples)}\n")
        print("F32FP\t{fp_of(mf, samples)}\n")
    }
}
