options gen2
options persistent_heap
options stack = 524288
options _dasllama_internal = true
require dasllama/dasllama_tts
require dasllama/dasllama_core
require dasllama/dasllama_math
require dasllama/dasllama_wav
require daslib/fio
require daslib/strings_boost
require math

// The pace of our Pocket files on three texts, the twin of `pocket_ref_pace.py`: total seconds, the lead and tail
// silence, the pauses (runs of five or more 20 ms windows more than 40 dB under the loudest, inside the speech) and
// the words a second of speech; under DAS_LOG_LEVEL=info the facade's timings line gives each synthesis's stage split.
//   bin/daslang -jit modules/dasLLAMA/harness/pocket_pace.das -- <models dir> <out dir> [file.gguf ...]

let TEXTS <- [
    "Whose woods these are I think I know, his house is in the village though, he will not see me stopping here, to watch his woods fill up with snow.",
    "Whose woods these are I think I know. His house is in the village though. He will not see me stopping here. To watch his woods fill up with snow.",
    "The quick brown fox jumps over the lazy dog, and the small model speaks it in the browser's own host."
]

def analyze(pcm : array<float>; rate : int) {
    let win = rate / 50
    let n = length(pcm) / win
    var inscope rms : array<float>
    rms |> reserve(n)
    var peak = 0.0
    for (i in range(n)) {
        var s = 0.0
        for (j in range(win)) {
            let v = pcm[i * win + j]
            s += v * v
        }
        let r = sqrt(s / float(win))
        rms |> push(r)
        peak = max(peak, r)
    }
    let thr = peak * 0.01
    var first = -1
    var last = -1
    for (i in range(n)) {
        if (rms[i] > thr) {
            if (first < 0) {
                first = i
            }
            last = i
        }
    }
    var pauses = 0
    var pause_win = 0
    var run = 0
    for (i in range(first, last + 1)) {
        if (rms[i] <= thr) {
            run++
        } else {
            if (run >= 5) {
                pauses++
                pause_win += run
            }
            run = 0
        }
    }
    let speech_s = float(last - first + 1 - pause_win) * 0.02
    return (total = float(length(pcm)) / float(rate), lead = float(first) * 0.02, tail = float(n - 1 - last) * 0.02, pauses = pauses,
        pause_s = float(pause_win) * 0.02, speech_s = speech_s)
}

[export]
def main {
    let args <- get_command_line_arguments()
    let at = find_index(args, "--")
    if (at < 0 || length(args) < at + 3) {
        print("usage: pocket_pace.das -- <models dir> <out dir> [file.gguf ...]\n")
        return
    }
    let dir = args[at + 1]
    let out = args[at + 2]
    var inscope files : array<string>
    files |> reserve(length(args))
    for (i in range(at + 3, length(args))) {
        files |> push(args[i])
    }
    if (empty(files)) {
        files |> push("pocket-tts-en-kq.gguf")
        files |> push("pocket-tts-en-18m-kq.gguf")
    }
    with_dasllama_jobque_() {
        for (file in files) {
            var inscope m <- load_tts_model(path_join(dir, file))
            for (voice in ["alba", "stuart_bell"]) {
                for (ti in [0, 2]) {
                    var inscope a <- synthesize(m, TEXTS[ti], voice)
                    let r = analyze(a.pcm, a.sample_rate)
                    let words = length(split(TEXTS[ti], " "))
                    print("{file} {voice} text{ti}: total {r.total:.2f} s, lead {r.lead:.2f}, tail {r.tail:.2f}, pauses {r.pauses} ({r.pause_s:.2f} s), speech {r.speech_s:.2f} s, {float(words) / r.speech_s:.2f} words/s\n")
                    write_wav_pcm16(path_join(out, "pace_{file}_{voice}_{ti}.wav"), a.pcm, a.sample_rate)
                }
            }
        }
    }
}
