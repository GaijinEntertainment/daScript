options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

// Segment gate: print `SEG<TAB>start<TAB>end` (sample indices) from find_speech_timestamps,
// diff against silero get_speech_timestamps (harness/silero_segment_oracle.py).
//   bin/daslang silero_segment_probe.das -- <silero_vad.bin> <audio.wav>

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
        let segs <- find_speech_timestamps(m, samples, default_vad_opts())
        for (s in segs) {
            print("SEG\t{s.start}\t{s.end}\n")
        }
    }
}
