options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

// Streaming-iterator gate: feed a wav through vad_iter_feed in odd-sized chunks (exercising the
// pending buffer) and print `EVT<TAB>start|end<TAB>sample` per boundary — diff against silero's
// VADIterator (harness/silero_iter_oracle.py).
//   bin/daslang silero_iter_probe.das -- <silero_vad.bin> <audio.wav>

require dasllama/dasllama_vad
require dasllama/dasllama_audio
require dasllama/dasllama_math
require _audio_probe_fixture
require daslib/jobque_boost
require math

[export]
def main() {
    let args <- get_command_line_arguments()
    let model_path = args[length(args) - 2]
    let wav_path = args[length(args) - 1]
    let samples <- load_audio_16k_mono(wav_path)
    with_job_que() {
        setup_dasllama_jobque_()
        var m <- load_vad_model(model_path)
        var it : VadIter
        vad_iter_reset(it, default_vad_opts())
        // feed in 777-sample chunks (deliberately not a multiple of 512) to exercise buffering
        var buf : array<float>
        let step = 777
        var pos = 0
        while (pos < length(samples)) {
            let n = min(step, length(samples) - pos)
            buf |> resize(n)
            for (i in range(n)) {
                buf[i] = samples[pos + i]
            }
            vad_iter_feed(m, it, buf) $(ev : VadEvent) {
                let kind = ev.kind == VadEventKind.speech_start ? "start" : "end"
                print("EVT\t{kind}\t{ev.sample}\n")
            }
            pos += n
        }
    }
}
