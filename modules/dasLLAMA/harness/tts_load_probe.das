options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap

// One speech model through `load_tts_model`, one line at its first voice, then the line at up to five voices: at each
// stage the das heap over its figure before the load, in MB of 1e6 bytes - a mapped image's pages are not heap. Then
// the voice turnover, three rounds: the line at the first voice while the model holds it, and again after two other
// voices spoke. On a Pocket model the second pass rebuilds the voice state the model dropped; on a StyleTTS2 model
// the two passes are the same work.
//
//   [DASLLAMA_METAL_TOWER=0] bin/daslang -jit modules/dasLLAMA/harness/tts_load_probe.das -- <model.gguf | image.dlim>

require dasllama/dasllama
require daslib/fio
require math

let LINE = "The quick brown fox jumps over the lazy dog, and then it rests for a while under the old oak tree."
let VOICES = 5
let ROUNDS = 3

def private heap_mb() : double => double(int64(heap_bytes_allocated())) / 1.0e6lf

def private speak(var m : TtsModel; voice : string) {
    let t0 = ref_time_ticks()
    var inscope a <- synthesize(m, LINE, voice)
    return (ms = double(get_time_usec(t0)) / 1000.0lf, samples = length(a.pcm))
}

[export, cold_path]
def main {
    let args <- get_command_line_arguments()
    let path = args[length(args) - 1]
    with_dasllama_jobque() {
        let h0 = heap_mb()
        var inscope m <- load_tts_model(path)
        let h1 = heap_mb()
        let c <- caps(m)
        let first = speak(m, c.voices[0])
        let h2 = heap_mb()
        let spoken = min(length(c.voices), VOICES)
        for (v in range(spoken)) {
            speak(m, c.voices[v])
        }
        let h3 = heap_mb()
        print("HEAP {base_name(path)}: {fmt(":.1f", h0)} MB before the load, +{fmt(":.1f", h1 - h0)} loaded,"
            + " +{fmt(":.1f", h2 - h0)} after one line ({first.samples} samples), +{fmt(":.1f", h3 - h0)} after {spoken} voices\n")
        if (length(c.voices) < 3) {
            print("TURNOVER {base_name(path)}: skipped, the model carries {length(c.voices)} voices\n")
            return
        }
        for (r in range(ROUNDS)) {
            speak(m, c.voices[0])
            let held = speak(m, c.voices[0])
            speak(m, c.voices[1])
            speak(m, c.voices[2])
            let back = speak(m, c.voices[0])
            print("TURNOVER {base_name(path)} round {r}: {fmt(":.1f", held.ms)} ms held, {fmt(":.1f", back.ms)} ms after two other voices"
                + " ({held.samples} and {back.samples} samples)\n")
        }
    }
}
